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
    LFPIPE = 375,
    RPIPE = 376,
    CLONEEQU = 377,
    ROTL = 378,
    ROTR = 379,
    ROTLEQU = 380,
    ROTREQU = 381,
    MAPTO = 382,
    COLCOL = 383,
    ANDAND = 384,
    OROR = 385,
    XORXOR = 386,
    ANDANDEQU = 387,
    OROREQU = 388,
    XORXOREQU = 389,
    BRABRAB = 390,
    BRACBRB = 391,
    CBRCBRB = 392,
    INTEGER = 393,
    LONG_INTEGER = 394,
    UNSIGNED_INTEGER = 395,
    UNSIGNED_LONG_INTEGER = 396,
    FLOAT = 397,
    DOUBLE = 398,
    NAME = 399,
    BEGIN_STRING = 400,
    STRING_CHARACTER = 401,
    END_STRING = 402,
    BEGIN_STRING_EXPR = 403,
    END_STRING_EXPR = 404,
    END_OF_READ = 405,
    UNARY_MINUS = 406,
    UNARY_PLUS = 407,
    PRE_INC = 408,
    PRE_DEC = 409,
    POST_INC = 410,
    POST_DEC = 411,
    DEREF = 412
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

#line 407 "ds_parser.cpp"

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
#define YYLAST   7664

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  185
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  532
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  951

#define YYUNDEFTOK  2
#define YYMAXUTOK   412

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
       2,     2,     2,   168,     2,   184,   179,   164,   157,     2,
     177,   178,   162,   161,   151,   160,   173,   163,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   154,   181,
     158,   152,   159,   153,   180,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   175,     2,   176,   156,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   182,   155,   183,   167,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   165,   166,   169,   170,
     171,   172,   174
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   442,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   458,   473,   474,   478,   482,
     487,   496,   504,   520,   525,   533,   533,   563,   585,   589,
     592,   598,   607,   628,   651,   652,   656,   660,   661,   665,
     668,   674,   680,   683,   689,   690,   694,   695,   696,   705,
     706,   710,   719,   735,   743,   753,   762,   763,   764,   765,
     766,   767,   771,   776,   784,   785,   786,   787,   788,   789,
     790,   791,   797,   810,   826,   827,   831,   838,   850,   854,
     861,   862,   866,   867,   868,   872,   877,   884,   888,   889,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,   908,   909,
     910,   911,   912,   913,   914,   915,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,   926,   927,   928,   929,
     933,   955,   993,  1000,  1004,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,  1032,  1033,  1037,  1042,  1048,  1054,  1075,  1078,
    1084,  1085,  1096,  1100,  1106,  1109,  1112,  1116,  1122,  1126,
    1130,  1133,  1136,  1141,  1144,  1152,  1155,  1160,  1163,  1171,
    1177,  1178,  1182,  1216,  1216,  1216,  1219,  1219,  1219,  1224,
    1224,  1224,  1232,  1236,  1241,  1247,  1247,  1247,  1251,  1251,
    1251,  1256,  1256,  1256,  1265,  1268,  1274,  1275,  1282,  1293,
    1294,  1295,  1299,  1300,  1301,  1302,  1306,  1311,  1319,  1320,
    1324,  1329,  1393,  1394,  1395,  1396,  1397,  1398,  1402,  1403,
    1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1424,  1425,  1426,
    1427,  1431,  1442,  1447,  1457,  1461,  1461,  1461,  1468,  1468,
    1468,  1482,  1486,  1486,  1486,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,
    1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,
    1528,  1529,  1530,  1531,  1532,  1533,  1534,  1535,  1536,  1540,
    1544,  1547,  1550,  1551,  1552,  1553,  1562,  1571,  1572,  1575,
    1575,  1575,  1578,  1582,  1586,  1590,  1591,  1592,  1593,  1594,
    1595,  1596,  1606,  1616,  1671,  1672,  1676,  1677,  1681,  1689,
    1692,  1697,  1696,  1741,  1797,  1806,  1811,  1819,  1820,  1824,
    1827,  1836,  1837,  1841,  1850,  1851,  1856,  1857,  1861,  1867,
    1873,  1876,  1880,  1886,  1895,  1896,  1897,  1901,  1902,  1906,
    1909,  1914,  1919,  1927,  1938,  1941,  1948,  1949,  1953,  1980,
    1980,  1992,  1995,  2004,  2017,  2029,  2030,  2034,  2038,  2049,
    2064,  2065,  2069,  2106,  2107,  2111,  2111,  2195,  2202,  2211,
    2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,  2220,  2221,
    2222,  2223,  2224,  2225,  2226,  2227,  2228,  2229,  2230,  2231,
    2232,  2233,  2234,  2235,  2239,  2240,  2241,  2242,  2243,  2244,
    2248,  2259,  2263,  2273,  2280,  2289,  2289,  2289,  2302,  2303,
    2304,  2305,  2306,  2320,  2326,  2330,  2334,  2339,  2344,  2349,
    2354,  2358,  2362,  2367,  2371,  2376,  2376,  2376,  2382,  2389,
    2389,  2389,  2394,  2394,  2394,  2400,  2400,  2400,  2405,  2409,
    2409,  2409,  2414,  2414,  2414,  2423,  2427,  2427,  2427,  2432,
    2432,  2432,  2441,  2445,  2445,  2445,  2450,  2450,  2450,  2459,
    2459,  2459,  2465,  2465,  2465,  2474,  2474,  2489,  2489,  2509,
    2510,  2511,  2512,  2516,  2523,  2530,  2536,  2545,  2550,  2557,
    2558,  2562,  2568,  2575,  2583,  2590,  2598,  2610,  2613,  2627,
    2636,  2637,  2641,  2646,  2653,  2658,  2669,  2674,  2681,  2693,
    2694,  2698,  2716
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
  "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"",
  "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
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
     405,    44,    61,    63,    58,   124,    94,    38,    60,    62,
      45,    43,    42,    47,    37,   406,   407,   126,    33,   408,
     409,   410,   411,    46,   412,    91,    93,    40,    41,    36,
      64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -595

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-595)))

#define YYTABLE_NINF -487

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -595,    20,  -595,  -595,   -69,   -84,   -23,   -15,  -119,  -595,
      -8,  -595,  -595,    86,  -595,  -595,  -595,  -595,  -595,   352,
    -595,    95,  -595,  -595,  -595,  -595,  -595,  -595,    14,  -595,
       4,    57,    52,  -595,  -595,    15,  -595,   -93,    90,    95,
     103,   113,  -595,   140,   126,   115,  -595,   -35,  -595,  -595,
    -595,    -6,   163,  -595,   152,    19,   -69,   200,   -84,   209,
    -595,   216,   219,  -595,   258,  -595,   217,  -595,   -73,  -595,
     188,   190,  -595,   233,   -69,    86,  -595,  7496,   344,   350,
    -595,   205,   207,  -595,   237,  -595,  -595,   391,  -595,  -595,
    -595,  -595,  -595,   247,    79,  -595,  -595,  -595,  -595,   341,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,   213,   -39,
    7411,  -595,  -595,   252,   253,  -595,     3,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
     -87,   244,  -595,  -595,   109,  -595,   218,   -27,   220,  -595,
     196,  -595,  -595,  -595,  -595,  -595,  -595,  -595,    40,  -595,
    -595,    54,  -595,   241,   248,   249,   254,  -595,  -595,  -595,
     225,  -595,  -595,  -595,  -595,  -595,   255,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,   256,  -595,  -595,  -595,
     257,   265,  -595,  -595,  -595,  -595,   266,   267,  -595,  -595,
    -595,  -595,  -595,    45,  -595,   235,   273,  -595,   268,  -595,
     -69,  -595,   -14,  -595,    67,  7411,  -595,  1336,  -595,  -595,
    -595,  -595,  -595,   197,  -595,  -595,  -595,    79,  -595,    25,
    4736,  -595,  -595,  -595,  -595,  -595,   284,  -595,    97,   156,
     178,  -595,  -595,  -595,  -595,  -595,  -595,   397,  -595,  -595,
       5,  2984,  -595,  -595,    49,  7411,    55,    88,   292,  -595,
     -71,  -595,   302,  3488,  -595,  4736,  -595,  -595,   292,  -595,
    -595,  7327,   270,  1987,  -595,  -595,   207,  4736,   271,  4736,
     293,   297,   279,  -595,   276,   306,   325,  3121,   207,  -595,
     313,  2144,  4736,  4736,   182,   182,  7159,  7243,  4736,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  4736,  4736,  4736,   109,
    4736,  4736,  4736,  -595,   296,  -595,   436,  -595,  -595,   298,
     300,  4736,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
     -92,   314,  -595,   315,   322,  -595,  -595,  -595,   292,  -595,
    -595,  -595,   308,  -595,  -595,   -89,  -595,  -595,  -595,  -595,
    -595,  6375,   317,  -595,  -595,  -595,  -595,  -595,    11,   366,
    7411,  -595,   -44,  -595,  -595,   702,  7411,  7411,  7411,  7411,
     295,   252,  7411,   205,  7411,   205,  7411,   205,  7495,   253,
    -595,  -595,  -595,   336,  -595,  -595,  6032,  -595,  -595,  3488,
    -595,  -595,   337,  -595,  -595,  -595,  7411,  -595,  2281,  -595,
     329,    26,   530,  -595,   109,  2281,  -595,  -595,   396,  4825,
     502,  6695,  4736,   329,  -595,  -595,  4736,  -595,  -595,   362,
     379,  6695,  -595,  7411,  2281,  -595,  4923,   533,   533,   338,
    -595,  -595,   292,  1058,   292,  1677,  6473,  -595,   141,   240,
     533,   533,   -72,  -595,   533,   533,  5487,   -21,   345,  -595,
    -595,   329,  -595,  2418,  -595,  -595,  -595,  -595,   384,   182,
    -595,    21,   376,  4736,  4736,  4736,  4736,  4736,  4736,  4736,
    4736,  4736,  4736,  -595,  -595,  4736,  4736,  4736,  4736,  4736,
    4736,   382,  2573,  4736,   386,  4736,  4736,  4736,  4736,  4736,
    4736,  4736,  4736,  4736,  4736,  4736,  4736,  4736,  4736,  2710,
    3258,  4736,  4736,  4736,  4736,  4736,  4736,  4736,  4736,  4736,
    4736,   387,  4736,  3395,    86,  -595,   521,   491,   353,  -595,
     348,  -595,  -595,  -595,  2281,  -595,   738,   886,  1278,  1618,
    -595,    85,  1775,   244,  1831,   244,  1929,   244,   -90,  -595,
     118,  3488,   137,  -595,  -595,  -595,  2085,  -595,  6276,  -595,
    4736,  3550,  2847,  -595,  5021,  4736,  4736,  -595,   207,  5564,
    -595,  7411,  7411,  5643,  7411,  -595,  -595,  2087,  -595,  5119,
    -595,  -595,    31,   182,   -46,  1832,  3705,  6473,   385,     9,
     359,   388,  -595,  -595,    87,    41,  3860,     9,   148,  4736,
    4736,   354,  -595,  4736,   392,   394,  -595,   181,  -595,  -595,
     272,   398,  -595,    53,  6695,   -57,   205,  -595,   383,  -595,
    -595,  6695,  6695,  6695,  6695,  6695,  6695,  6695,  6695,   305,
     305,    10,  6695,  6695,    10,  7039,  7039,   367,  4736,  4736,
    6695,   155,  -595,  6111,   -28,   -28,  6695,   305,   305,  6695,
    6695,  6843,   504,  6769,  6695,  6695,  6695,  4736,  4736,  6695,
     401,  6547,  6867,  6941,  6965,    10,    10,   478,   478,   155,
     155,   155,  -595,   545,  6189,  -595,    62,    99,   323,  -595,
     292,  -595,   397,  -595,  2281,  -595,  5217,  -595,  7411,  -595,
    -595,  -595,   403,  -595,   389,  -595,   390,  -595,   393,  7411,
    -595,  7495,  -595,   253,   397,  4736,   -77,  -595,    74,   399,
     406,  5720,  -595,  -595,  -595,  2219,  2356,  -595,  2511,   384,
    -595,  4736,  -595,  4736,  4736,    -9,   407,   398,   380,   400,
    4736,   402,  4736,  4736,   398,   372,   404,  6695,  -595,  -595,
    6621,  -595,   412,  7411,   205,  1519,   207,  -595,  -595,  4736,
    -595,   -43,   110,  4736,  -595,   415,   416,   417,   419,  -595,
     114,   244,  -595,  4015,  1150,  6695,  -595,  1150,  6695,  -595,
    4736,  -595,  -595,  -595,   408,  -595,    -6,  -595,  -595,  5315,
    -595,  -595,  2648,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  3488,  -595,  -595,  -595,  6695,   207,  -595,  -595,
     139,  -595,  -595,  -595,  -595,   395,   -33,  6695,  6695,     9,
     405,    27,   385,   409,  -595,  6695,  -595,  -595,   -31,     9,
     420,  -595,  -595,  -595,  2785,   244,  -595,  -595,   329,   410,
    6695,  -595,  -595,  -595,  -595,   -57,   421,   -86,  7411,  -595,
      77,  6695,  -595,  -595,    -6,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  7411,  4170,   422,  4736,  4736,  4736,  4307,  4736,
     424,   425,  4736,  4736,  -595,  4736,   381,  -595,  -595,   411,
     272,  -595,  -595,  -595,  4462,  -595,  -595,  2922,  -595,   430,
    -595,  3059,   426,  5799,   414,    10,    10,    10,  -595,  5876,
    5413,   427,  -595,  6695,  6695,  5413,   429,   109,  -595,  -595,
    4736,  6695,  -595,  -595,  -595,  -595,  -595,  4599,  -595,   516,
     441,  -595,   439,  -595,  -595,   109,  6695,  -595,   434,  7411,
     440,  5955,  4736,   448,   450,  -595,  -595,  3196,  -595,  -595,
    6695,  -595,  -595,  -595,  7411,   451,  3333,  -595,  -595,   452,
    -595
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   180,     0,     0,     0,     0,     0,   181,
       0,   497,   495,     0,    14,     3,    10,     9,     8,     0,
       7,   376,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,   376,
       0,     0,    75,     0,   160,    76,    78,     0,    74,   394,
     393,     0,     0,   377,   379,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   381,     0,   385,     0,    15,
       0,     0,   162,     0,     0,     0,    81,     0,     0,     0,
      87,    82,     0,   130,   390,   395,   374,     0,    68,    69,
      66,    67,    65,     0,     0,    64,    73,    40,    38,    34,
      31,    30,   424,   427,   425,   428,   426,   429,     0,     0,
       0,   387,   386,     0,     0,   161,     0,    79,    90,    91,
      93,    92,    94,    95,    96,    97,   122,   123,   120,   121,
     113,   124,   125,   114,   111,   112,   126,   127,   128,   129,
     101,   102,   103,    98,    99,   100,   116,   117,   115,   109,
     110,   105,   104,   106,   107,   108,    89,    88,   118,   119,
     334,    85,   154,   132,     0,   392,     0,     0,     0,    16,
       0,    60,    61,    58,    59,    57,    56,    62,     0,    33,
     381,     0,   388,     0,     0,     0,     0,   399,   419,   400,
     431,   401,   405,   406,   407,   408,   423,   412,   413,   414,
     415,   416,   417,   418,   420,   421,   468,   404,   411,   422,
     475,   482,   402,   409,   403,   410,     0,     0,   430,   438,
     441,   439,   440,     0,   433,     0,     0,   354,     0,    77,
       0,    83,     0,   347,     0,     0,   131,     0,   391,   339,
     397,   378,   375,   367,   380,    17,    18,     0,    71,     0,
       0,   382,   459,   462,   465,   455,     0,   435,   469,   476,
     483,   489,   492,   446,   451,   445,   458,     0,   454,   448,
       0,     0,   384,   450,     0,     0,     0,     0,     0,   345,
     358,    84,   334,    86,   156,     0,    49,    50,     0,   271,
     272,     0,     0,     0,   265,   168,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   222,
     224,   223,   225,   226,   227,    19,     0,     0,     0,     0,
       0,     0,     0,   209,   210,   135,   133,   269,   268,     0,
       0,     0,   149,   144,   142,   141,   143,   220,   155,   136,
     266,     0,   327,     0,     0,   147,   148,   150,     0,   140,
     326,   325,    80,   330,   267,     0,   137,   329,   328,   307,
     273,   228,     0,   270,   499,   500,   501,   502,   334,     0,
       0,   368,     0,    63,   389,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,    82,     0,    82,     0,     0,
     453,   447,   449,     0,   452,   443,     0,   434,   498,   353,
     496,   355,     0,   346,   357,   356,     0,   359,     0,   348,
       0,     0,   164,   167,     0,     0,   170,   173,     0,   228,
       0,   163,     0,     0,   183,   186,     0,   153,   189,     0,
       0,    43,    53,     0,     0,   177,   228,   299,   300,   210,
     158,   159,     0,   509,     0,     0,     0,   526,     0,     0,
     277,   276,   312,    25,   275,   274,     0,   211,     0,   151,
     152,     0,   333,     0,   139,   145,   146,   182,   218,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,   396,     0,   336,     0,   398,
       0,   365,   366,   364,     0,   383,     0,     0,     0,     0,
     432,     0,     0,    85,     0,    85,     0,    85,   160,   351,
       0,   349,     0,   444,   442,   335,   360,   363,   362,    54,
       0,     0,     0,   174,   228,     0,     0,   171,     0,     0,
      55,     0,     0,     0,     0,    41,    42,     0,   178,   228,
     175,   211,     0,     0,   160,     0,     0,   517,   507,   509,
       0,   520,   521,   522,     0,     0,     0,   509,     0,     0,
       0,     0,    22,     0,    20,     0,   303,   255,   254,   154,
      46,     0,   308,     0,   204,     0,    82,   157,     0,   322,
     323,   238,   239,   241,   240,   242,   232,   233,   234,   278,
     279,   291,   243,   244,   292,   289,   290,     0,     0,     0,
     230,   317,   306,     0,   331,   332,   231,   280,   281,   245,
     246,   296,   297,   298,   235,   236,   237,     0,     0,   229,
       0,     0,   294,   295,   293,   287,   288,   283,   282,   284,
     285,   286,   261,     0,     0,   310,     0,     0,   336,   337,
       0,   340,   364,   369,     0,   373,   228,   460,     0,   466,
     456,   436,     0,   470,     0,   477,     0,   484,     0,     0,
     490,     0,   493,     0,   356,     0,     0,   165,     0,     0,
       0,     0,   172,   179,   313,     0,     0,   314,     0,   218,
     176,     0,   510,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   519,   527,   528,
       0,    23,    26,     0,    82,     0,     0,    44,    45,     0,
      51,     0,     0,     0,   309,     0,     0,     0,     0,   216,
       0,    85,   319,     0,   249,   250,   305,   247,   248,   324,
       0,   263,   304,   311,    81,   341,     0,   338,   371,   228,
     372,   461,     0,   467,   457,   437,   471,   473,   478,   480,
     485,   487,   350,   491,   352,   494,   361,     0,   166,   195,
       0,   192,   184,   187,   190,     0,     0,   504,   503,   509,
       0,     0,   508,     0,   512,   518,   524,   523,     0,   509,
       0,   525,    21,    24,     0,    85,   134,    47,     0,     0,
     205,   214,   215,   213,   212,     0,     0,     0,     0,   252,
       0,   318,   264,   344,     0,   343,   370,   463,   474,   481,
     488,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   511,     0,     0,   515,   256,     0,
      46,   251,   217,   219,     0,   206,   221,     0,   253,     0,
     464,     0,     0,     0,     0,   185,   188,   191,   315,     0,
     529,     0,   513,   506,   505,   529,     0,     0,   259,    48,
       0,   207,   320,   342,   196,   198,   193,     0,   316,     0,
       0,   514,     0,   516,   257,     0,   208,   321,     0,     0,
       0,     0,     0,     0,     0,   260,   197,     0,   201,   194,
     530,   531,   532,   199,     0,     0,     0,   200,   202,     0,
     203
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -595,  -595,  -595,   108,   519,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,   476,  -595,  -595,   573,  -595,  -595,
    -595,  -303,  -595,  -595,  -595,  -595,  -595,  -595,   332,  -595,
     576,   -63,  -595,   561,   105,  -305,  -383,  -538,  -595,  -595,
    -211,  -139,   -81,  -595,    33,  -286,   -13,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,   654,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -524,  -595,  -595,  -189,  -595,   -60,  -292,  -595,  -233,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
     449,  -595,  -595,   280,   -18,  -595,  -595,  -595,   375,  -595,
     -40,  -595,  -271,   274,  -278,  -272,   132,  -595,   316,  -595,
     637,  -595,  -595,   500,   613,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -146,  -107,  -595,  -595,  -595,   301,  -595,
    -595,  -595,   -75,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,   413,  -594,  -442,  -583,  -595,  -595,  -310,   -58,   234,
    -595,  -595,  -595,  -217,  -595
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   170,   176,   459,   337,   752,   338,   615,
      16,    17,    35,    36,    63,    18,    32,    33,   339,   340,
     759,   760,   341,   342,   343,   344,   345,   346,   177,   178,
      29,    30,    45,    46,    47,    19,   161,   236,    81,    20,
      82,    83,   347,   348,   237,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   581,   865,   582,
     866,   584,   867,   361,   862,   928,   929,   945,   944,   949,
     623,   886,   362,   769,   770,   626,   363,   364,   428,   366,
     367,   368,   369,   753,   907,   754,   925,   370,   683,   852,
     624,   848,   927,   232,   690,   538,   378,   854,   233,   234,
     559,   560,   227,   228,   734,   279,   544,   382,   242,   167,
      54,    22,    87,   109,    67,    68,    23,    24,   165,    85,
      52,    25,   166,   280,   372,   108,   220,   221,   225,   222,
     391,   795,   561,   389,   794,   386,   791,   387,   890,   388,
     793,   392,   796,   393,   858,   394,   798,   395,   859,   396,
     800,   397,   860,   398,   803,   399,   805,    26,    41,    27,
      40,   373,   598,   599,   600,   374,   601,   602,   603,   604,
     375,   458,   376,   920,   377
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   163,   418,   219,   365,   411,   413,   427,   457,   686,
     553,   116,   555,   607,   557,   704,   738,   706,   -80,   708,
       2,   243,   450,   451,   745,   445,     3,   762,    77,   479,
     481,   482,   278,    88,    89,   223,    59,   401,    73,   628,
     593,   884,   414,   504,   505,   570,   716,   718,   230,     4,
     731,     5,   472,     6,    31,     7,   765,   478,   593,     8,
     743,    64,    48,    38,   709,   766,     9,   414,    60,   541,
     414,    66,    10,   536,   763,    28,   733,   263,   542,   733,
     379,   415,    73,   416,   501,   473,   417,   504,   505,    65,
      11,   231,   480,   171,   172,   767,   162,   218,    78,    79,
     768,   264,   265,   532,   807,   181,   415,    43,   543,   415,
     111,    12,   491,   492,   493,   494,    75,   240,   763,    42,
     763,    34,   501,    44,   503,   504,   505,   506,   219,    37,
     240,   507,   567,   509,   510,   761,    39,   617,    80,   573,
     414,    76,   421,   822,   182,   531,   230,   532,   869,   872,
     875,   238,   820,    53,    56,    56,   241,    90,   588,   266,
     283,    91,   402,    92,    93,   629,    55,   277,   219,   181,
     526,   527,   528,   529,   530,   481,   482,   379,    61,   415,
     403,   229,   379,   531,   219,   532,   534,   627,    62,   404,
     737,   247,   379,   407,   535,    13,    94,   267,   268,   226,
     409,    14,   269,    58,   763,   270,   250,   816,   384,   219,
     219,    57,   243,   763,    43,   430,   422,   173,   248,   828,
     271,   174,   218,   175,    93,   763,   272,   442,   763,   273,
      44,   764,   408,   847,    66,   251,   870,    43,   410,    56,
     783,   453,   455,   771,   701,   281,   876,    70,   282,   850,
      75,  -472,   808,    44,    73,   888,  -472,    71,   695,   493,
     494,   736,   218,   741,   412,   845,   702,   501,   742,   503,
     504,   505,   506,   219,  -472,   784,   507,   710,   218,   219,
     219,   219,   219,   756,    72,   219,   839,   219,   715,   219,
     846,   219,    74,   819,   757,   758,   712,   879,   863,   711,
     748,   732,   829,   218,   218,   540,   592,    84,   605,   219,
    -479,   546,   547,   548,   549,  -479,   463,   552,   713,   554,
     864,   556,   610,   102,   611,   481,   482,   103,   531,   742,
     532,   746,  -486,  -479,    86,  -258,   219,  -486,    97,   569,
    -258,   566,   245,   246,   104,   105,   106,   107,   379,   481,
     482,   380,   580,    99,   381,  -486,    49,    50,  -258,    51,
     100,   333,   449,   101,   162,   689,   785,   218,   587,   110,
     113,   835,   114,   218,   218,   218,   218,   115,   158,   218,
     263,   218,   160,   218,   159,   218,   169,   612,   613,   162,
     620,   164,   168,   169,    60,   180,   224,   226,   235,   252,
     239,   244,   256,   218,   264,   265,   253,   254,   788,   493,
     494,   572,   255,   257,   258,   259,   274,   501,   787,   503,
     504,   505,   506,   260,   261,   262,   507,   275,   390,   400,
     218,   491,   492,   493,   494,   495,   240,   230,   498,   499,
     500,   501,   411,   503,   504,   505,   506,   424,   432,   276,
     507,   434,   509,   510,   618,   435,   436,   437,   513,   514,
     515,   541,   266,   439,   438,   526,   527,   528,   529,   530,
     542,   443,   468,   550,   219,   219,   467,   219,   531,   469,
     532,   470,   520,    13,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   533,   474,   475,   723,   481,   482,
     692,   268,   531,   476,   532,   269,   725,   726,   270,   728,
     539,   162,   563,   565,   479,   578,   585,   586,   591,   625,
     630,    48,   365,   271,   481,   482,   647,   619,   688,   693,
     652,   682,   273,   689,   691,   739,   736,   749,   245,   740,
     751,   772,   761,   873,   773,   779,   781,   407,   797,   799,
     810,   821,   801,   481,   482,   830,   823,   809,   833,   841,
     842,   843,   263,   844,   906,   932,   894,   614,   218,   218,
     908,   218,   868,   917,    95,   179,   824,   909,   826,   383,
     831,   871,   493,   494,   915,   874,   264,   265,   881,   853,
     501,   219,   503,   504,   505,   506,   877,   883,   938,   507,
     901,   902,   219,   921,   219,   923,   491,   492,   493,   494,
     495,   913,   936,   498,   499,   500,   501,   933,   503,   504,
     505,   506,   934,   792,   941,   507,   942,   509,   510,   947,
     950,    98,    96,   513,   802,   515,   117,   493,   494,   687,
     528,   529,   530,   889,   266,   501,   219,   855,   504,   505,
     506,   531,   755,   532,   507,    21,   882,   419,   537,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   815,
     786,   804,   694,   562,   477,   837,    69,   531,   834,   532,
     249,   112,   267,   268,   827,   218,   371,   269,   922,   608,
     270,     0,   551,     0,     0,     0,   218,     0,   218,   385,
       0,     0,     0,     0,   423,   271,   531,   571,   532,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
     406,     0,   481,   482,     0,     0,   861,     0,     0,     0,
       0,     0,     0,     0,   420,     0,     0,     0,     0,     0,
     218,   219,   429,     0,     0,     0,   431,     0,   433,     0,
       0,     0,     0,     0,     0,   219,   441,   880,     0,     0,
     446,   447,   448,     0,     0,     0,   885,   456,     0,     0,
     263,     0,     0,   887,     0,   460,   461,   462,     0,   464,
     465,   466,     0,     0,     0,     0,     0,   891,     0,     0,
     471,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,   494,   495,     0,
       0,   498,   499,   500,   501,     0,   503,   504,   505,   506,
       0,     0,   219,   507,     0,   509,   510,     0,     0,     0,
       0,   513,   514,   515,     0,   218,     0,   219,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   218,
       0,     0,   266,     0,   937,   520,     0,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   568,     0,   946,
       0,     0,     0,     0,   574,   531,     0,   532,     0,     0,
       0,   579,     0,   545,     0,   583,     0,     0,     0,     0,
     267,   268,     0,   589,   924,   269,     0,   697,   270,     0,
       0,     0,   597,     0,   597,     0,     0,     0,     0,     0,
       0,     0,   935,   271,     0,     0,   218,     0,   263,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,   218,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   264,   265,   641,   642,   643,   644,   645,   646,
       0,   650,   651,     0,   653,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,   664,   665,   666,   669,   671,
     672,   673,   674,   675,   676,   677,   678,   679,   680,   681,
       0,   684,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   696,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   721,     0,     0,   650,   669,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   268,
       0,     0,     0,   269,   460,   466,   270,     0,     0,     0,
       0,     0,     0,     0,     0,   466,     0,     0,   747,   456,
       0,   271,   750,     0,     0,     0,     0,   698,     0,     0,
     273,     0,   289,   290,   291,   292,     0,     0,     0,     0,
       0,     0,     0,     0,   294,     0,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   298,   774,   775,   300,
       0,     0,   301,     0,     0,   302,     0,   593,     0,   305,
       0,     0,     0,     0,   264,   265,   777,   778,     0,   187,
     188,   189,     0,   191,   192,   193,   194,   195,   309,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,   207,
     208,   209,     0,   789,   212,   213,   214,   215,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,   806,     0,     0,     0,     0,     0,
     481,   482,   266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,    43,     0,     0,   825,
       0,   597,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   594,   325,   371,     0,     0,     0,   838,     0,
     267,   268,   840,     0,     0,   269,     0,     0,   595,   327,
     328,     0,   329,     0,     0,   330,   331,     0,     0,   851,
       0,     0,     0,   271,     0,   596,     0,   333,   334,     0,
     162,     0,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   491,   492,     0,     0,   495,     0,     0,   498,
     499,   500,   501,     0,   503,   504,   505,   506,     0,     0,
       0,   507,     0,   509,   510,     0,     0,     0,     0,   513,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,     0,   521,   522,   523,   524,   525,
     263,     0,   893,   529,   895,   896,   897,   899,   900,     0,
       0,   903,   904,   531,   905,   532,     0,     0,     0,     0,
       0,     0,     0,   911,   264,   265,     0,   284,     0,     0,
       0,     0,     3,     0,   285,   286,   287,     0,   288,     0,
     289,   290,   291,   292,     0,     0,     0,     0,     0,   926,
       0,   293,   294,   295,   296,     0,   931,     0,     0,     0,
       0,     0,     0,   297,   298,     0,   299,   300,     0,     0,
     301,   940,     9,   302,   303,     0,   304,   305,     0,     0,
     306,   307,   266,     0,     0,     0,   308,   187,   188,   189,
       0,   191,   192,   193,   194,   195,   309,   197,   198,   199,
     200,   201,   202,   203,   204,   205,     0,   207,   208,   209,
       0,     0,   212,   213,   214,   215,     0,     0,   310,   311,
     267,   268,     0,     0,     0,   269,     0,   699,   270,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,   314,   315,     0,     0,     0,
       0,     0,   273,     0,    43,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,   325,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   327,   328,     0,
     329,     0,     0,   330,   331,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,   333,   334,   335,   162,   336,
     284,     0,     0,     0,     0,     3,     0,   285,   286,   287,
       0,   288,     0,   289,   290,   291,   292,     0,     0,     0,
       0,     0,     0,     0,   293,   294,   295,   296,     0,     0,
       0,     0,     0,     0,     0,     0,   297,   298,     0,   299,
     300,     0,     0,   301,     0,     9,   302,   303,     0,   304,
     305,     0,     0,   306,   307,     0,     0,     0,     0,   308,
     187,   188,   189,     0,   191,   192,   193,   194,   195,   309,
     197,   198,   199,   200,   201,   202,   203,   204,   205,     0,
     207,   208,   209,     0,     0,   212,   213,   214,   215,     0,
       0,   310,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   314,   315,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
     263,     0,     0,     0,   316,   317,   318,   319,   320,   321,
     322,   323,   324,    44,   325,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,   326,
     327,   328,     0,   329,     0,     0,   330,   331,     0,     0,
       0,   289,   290,   291,   292,     0,   332,     0,   333,   334,
     335,   162,   836,   294,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,   298,     0,     0,   300,     0,
       0,   301,     0,     0,   302,     0,     0,     0,   305,     0,
       0,     0,   266,   264,   265,     0,     0,     0,   187,   188,
     189,     0,   191,   192,   193,   194,   195,   309,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,   207,   208,
     209,     0,     0,   212,   213,   214,   215,     0,     0,   310,
     267,   268,     0,     0,     0,   269,     0,   700,   270,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,    43,     0,   263,     0,     0,
       0,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   594,   325,     0,     0,     0,     0,     0,     0,   267,
     268,   264,   265,     0,   269,     0,     0,   595,   327,   328,
       0,   329,     0,     0,   330,   331,   289,   290,   291,   292,
       0,     0,   271,     0,   606,     0,   333,   334,   294,   162,
       0,   273,     0,   263,   401,     0,     0,     0,     0,     0,
     298,     0,     0,   300,     0,     0,   301,     0,     0,   302,
       0,     0,     0,   305,     0,     0,     0,   264,   265,   266,
       0,     0,     0,   187,   188,   189,     0,   191,   192,   193,
     194,   195,   309,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,   207,   208,   209,     0,     0,   212,   213,
     214,   215,     0,     0,   310,     0,     0,   267,   268,     0,
       0,     0,   269,     0,   703,   270,   312,   313,     0,     0,
       0,     0,     0,     0,     0,   266,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,     0,     0,     0,   273,
      43,   263,     0,     0,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,    44,   325,     0,     0,
       0,     0,     0,   267,   268,   264,   265,     0,   269,   402,
     705,   270,   326,   327,   328,     0,   329,     0,     0,   330,
     331,   289,   290,   291,   292,     0,   271,   403,     0,   332,
       0,   333,   334,   294,   162,   273,   404,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,   300,     0,
       0,   301,     0,     0,   302,     0,     0,     0,   305,     0,
       0,     0,     0,   266,     0,     0,     0,     0,   187,   188,
     189,     0,   191,   192,   193,   194,   195,   309,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,   207,   208,
     209,     0,     0,   212,   213,   214,   215,     0,     0,   310,
       0,   267,   268,     0,     0,     0,   269,     0,   707,   270,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
     425,     0,     0,     0,   271,     0,   314,   315,     0,     0,
       0,     0,     0,   273,     0,    43,     0,   263,     0,   263,
       0,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,    44,   325,     0,     0,     0,     0,     0,     0,     0,
       0,   264,   265,   264,   265,     0,     0,   326,   327,   328,
       0,   329,     0,     0,   330,   331,     0,     0,   289,   290,
     291,   292,     0,     0,   332,     0,   333,   334,   426,   162,
     294,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,   300,     0,     0,   301,     0,
       0,   302,     0,     0,     0,   305,     0,     0,   414,   266,
       0,   266,     0,     0,     0,   187,   188,   189,     0,   191,
     192,   193,   194,   195,   309,   197,   198,   199,   200,   201,
     202,   203,   204,   205,     0,   207,   208,   209,     0,     0,
     212,   213,   214,   215,     0,     0,   310,   714,   268,   267,
     268,     0,   269,     0,   269,   270,   729,   270,   312,   313,
       0,   263,     0,     0,     0,     0,     0,   444,     0,     0,
     271,     0,   271,   314,   315,     0,     0,     0,     0,   273,
       0,   273,    43,     0,     0,   264,   265,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,     0,     0,     0,   289,   290,   291,   292,     0,
       0,     0,     0,     0,   326,   327,   328,   294,   329,     0,
       0,   330,   331,     0,     0,     0,     0,     0,     0,   298,
       0,   332,   300,   333,   334,   301,   162,     0,   302,     0,
       0,     0,   305,   266,     0,     0,     0,     0,     0,     0,
       0,     0,   187,   188,   189,     0,   191,   192,   193,   194,
     195,   309,   197,   198,   199,   200,   201,   202,   203,   204,
     205,     0,   207,   208,   209,     0,     0,   212,   213,   214,
     215,   267,   268,   310,     0,     0,   269,     0,   812,   270,
       0,     0,     0,     0,     0,   312,   313,     0,   263,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
     314,   315,     0,   273,     0,     0,     0,     0,     0,    43,
       0,     0,   264,   265,     0,     0,   316,   317,   318,   319,
     320,   321,   322,   323,   324,    44,   325,     0,     0,     0,
       0,     0,   289,   290,   291,   292,     0,     0,     0,     0,
       0,   326,   327,   328,   294,   329,     0,     0,   330,   331,
       0,     0,     0,     0,     0,     0,   298,     0,   332,   300,
     333,   334,   301,   162,     0,   302,     0,     0,     0,   305,
     266,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     188,   189,     0,   191,   192,   193,   194,   195,   309,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,   207,
     208,   209,     0,     0,   212,   213,   214,   215,   267,   268,
     310,     0,     0,   269,     0,   813,   270,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,     0,     0,     0,
     273,     0,     0,   263,     0,     0,    43,     0,     0,     0,
       0,     0,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,    44,   325,     0,     0,     0,   264,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,   327,
     328,     0,   329,     0,     0,   330,   331,   289,   290,   291,
     292,     0,     0,   621,     0,   332,   622,   333,   334,   294,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,   300,     0,     0,   301,     0,     0,
     302,     0,     0,     0,   305,   266,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   188,   189,     0,   191,   192,
     193,   194,   195,   309,   197,   198,   199,   200,   201,   202,
     203,   204,   205,     0,   207,   208,   209,     0,     0,   212,
     213,   214,   215,   267,   268,   310,     0,     0,   269,     0,
     814,   270,     0,     0,     0,     0,     0,   312,   313,     0,
     263,     0,     0,     0,     0,     0,   271,     0,     0,     0,
       0,     0,   648,   649,     0,   273,     0,     0,     0,     0,
       0,    43,     0,     0,   264,   265,     0,     0,   316,   317,
     318,   319,   320,   321,   322,   323,   324,    44,   325,     0,
       0,     0,     0,     0,   289,   290,   291,   292,     0,     0,
       0,     0,     0,   326,   327,   328,   294,   329,     0,     0,
     330,   331,     0,     0,     0,     0,     0,     0,   298,     0,
     332,   300,   333,   334,   301,   162,     0,   302,     0,     0,
       0,   305,   266,     0,     0,     0,     0,     0,     0,     0,
       0,   187,   188,   189,     0,   191,   192,   193,   194,   195,
     309,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,   207,   208,   209,     0,     0,   212,   213,   214,   215,
     267,   268,   310,     0,     0,   269,     0,   857,   270,     0,
       0,     0,     0,     0,   312,   313,     0,   263,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,   667,
     668,     0,   273,     0,     0,     0,     0,     0,    43,     0,
       0,   264,   265,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,     0,     0,
       0,   289,   290,   291,   292,   719,     0,     0,     0,     0,
     326,   327,   328,   294,   329,     0,     0,   330,   331,     0,
       0,     0,     0,     0,     0,   298,     0,   332,   300,   333,
     334,   301,   162,     0,   302,     0,     0,     0,   305,   266,
       0,     0,     0,     0,     0,     0,     0,     0,   187,   188,
     189,     0,   191,   192,   193,   194,   195,   309,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,   207,   208,
     209,     0,     0,   212,   213,   214,   215,   267,   268,   310,
       0,     0,   269,     0,   878,   270,     0,     0,     0,     0,
       0,   312,   313,     0,   263,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,    43,     0,     0,   264,   265,
       0,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,    44,   325,     0,     0,     0,     0,     0,   289,   290,
     291,   292,     0,     0,     0,   720,     0,   326,   327,   328,
     294,   329,     0,     0,   330,   331,     0,     0,     0,     0,
       0,     0,   298,     0,   332,   300,   333,   334,   301,   162,
       0,   302,     0,     0,     0,   305,   266,     0,     0,     0,
       0,     0,     0,     0,     0,   187,   188,   189,     0,   191,
     192,   193,   194,   195,   309,   197,   198,   199,   200,   201,
     202,   203,   204,   205,     0,   207,   208,   209,     0,     0,
     212,   213,   214,   215,   267,   268,   310,     0,     0,   269,
       0,   912,   270,     0,     0,     0,     0,     0,   312,   313,
       0,   263,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,    43,     0,     0,   264,   265,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,     0,     0,     0,   289,   290,   291,   292,     0,
       0,     0,     0,     0,   326,   327,   328,   294,   329,     0,
       0,   330,   331,     0,     0,     0,     0,     0,     0,   298,
     405,   332,   300,   333,   334,   301,   162,     0,   302,     0,
       0,     0,   305,   266,     0,   440,     0,     0,     0,     0,
       0,     0,   187,   188,   189,     0,   191,   192,   193,   194,
     195,   309,   197,   198,   199,   200,   201,   202,   203,   204,
     205,     0,   207,   208,   209,     0,     0,   212,   213,   214,
     215,   267,   268,   310,     0,     0,   269,     0,   914,   270,
       0,     0,     0,     0,     0,   312,   313,     0,   263,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
       0,     0,     0,   273,     0,     0,     0,     0,     0,    43,
       0,     0,   264,   265,     0,     0,   316,   317,   318,   319,
     320,   321,   322,   323,   324,    44,   325,     0,     0,     0,
       0,     0,   289,   290,   291,   292,     0,     0,     0,   670,
       0,   326,   327,   328,   294,   329,     0,     0,   330,   331,
       0,     0,     0,     0,     0,     0,   298,     0,   332,   300,
     333,   334,   301,   162,     0,   302,     0,     0,     0,   305,
     266,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     188,   189,     0,   191,   192,   193,   194,   195,   309,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,   207,
     208,   209,     0,     0,   212,   213,   214,   215,   267,   268,
     310,     0,     0,   269,     0,   943,   270,     0,     0,     0,
       0,     0,   312,   313,     0,   263,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,     0,     0,     0,
     273,     0,     0,     0,     0,     0,    43,     0,     0,   264,
     265,     0,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,    44,   325,     0,     0,     0,     0,     0,   289,
     290,   291,   292,     0,     0,     0,     0,     0,   326,   327,
     328,   294,   329,     0,     0,   330,   331,     0,     0,     0,
       0,     0,     0,   298,     0,   332,   300,   333,   334,   301,
     162,     0,   302,     0,     0,     0,   305,   266,     0,     0,
       0,     0,     0,     0,     0,     0,   187,   188,   189,     0,
     191,   192,   193,   194,   195,   309,   197,   198,   199,   200,
     201,   202,   203,   204,   205,     0,   207,   208,   209,     0,
       0,   212,   213,   214,   215,   267,   268,   310,     0,     0,
     269,     0,   948,   270,     0,     0,     0,     0,     0,   312,
     313,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
     263,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     316,   317,   318,   319,   320,   321,   322,   323,   324,    44,
     325,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,   327,   328,     0,   329,
       0,     0,   330,   331,   289,   290,   291,   292,     0,     0,
       0,     0,   332,   685,   333,   334,   294,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   300,     0,     0,   301,     0,     0,   302,     0,     0,
       0,   305,   266,     0,     0,     0,     0,     0,     0,     0,
       0,   187,   188,   189,     0,   191,   192,   193,   194,   195,
     309,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,   207,   208,   209,     0,     0,   212,   213,   214,   215,
     267,   268,   310,     0,     0,   269,     0,     0,   270,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,   328,     0,   329,     0,     0,   330,   331,   289,
     290,   291,   292,     0,     0,     0,     0,   332,   717,   333,
     334,   294,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   300,     0,     0,   301,
       0,     0,   302,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   187,   188,   189,     0,
     191,   192,   193,   194,   195,   309,   197,   198,   199,   200,
     201,   202,   203,   204,   205,     0,   207,   208,   209,     0,
       0,   212,   213,   214,   215,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     316,   317,   318,   319,   320,   321,   322,   323,   324,    44,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,   327,   328,     0,   329,
       0,     0,   330,   331,   289,   290,   291,   292,     0,     0,
       0,     0,   332,   735,   333,   334,   294,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   300,     0,     0,   301,     0,     0,   302,     0,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,   188,   189,     0,   191,   192,   193,   194,   195,
     309,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,   207,   208,   209,     0,     0,   212,   213,   214,   215,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,   328,     0,   329,     0,     0,   330,   331,   289,
     290,   291,   292,     0,     0,     0,     0,   332,   744,   333,
     334,   294,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   300,     0,     0,   301,
       0,     0,   302,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   187,   188,   189,     0,
     191,   192,   193,   194,   195,   309,   197,   198,   199,   200,
     201,   202,   203,   204,   205,     0,   207,   208,   209,     0,
       0,   212,   213,   214,   215,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     316,   317,   318,   319,   320,   321,   322,   323,   324,    44,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,   327,   328,     0,   329,
       0,     0,   330,   331,   289,   290,   291,   292,   892,     0,
       0,     0,   332,   849,   333,   334,   294,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,   300,     0,     0,   301,     0,     0,   302,     0,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,   188,   189,     0,   191,   192,   193,   194,   195,
     309,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,   207,   208,   209,     0,     0,   212,   213,   214,   215,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,     0,     0,
       0,   289,   290,   291,   292,     0,     0,     0,     0,     0,
     326,   327,   328,   294,   329,     0,     0,   330,   331,     0,
       0,     0,     0,     0,     0,   298,     0,   332,   300,   333,
     334,   301,   162,     0,   302,     0,     0,     0,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,   188,
     189,     0,   191,   192,   193,   194,   195,   309,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,   207,   208,
     209,     0,     0,   212,   213,   214,   215,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,    44,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,   327,   328,
       0,   329,     0,     0,   330,   331,   289,   290,   291,   292,
       0,     0,     0,     0,   332,   898,   333,   334,   294,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,   300,     0,     0,   301,     0,     0,   302,
       0,     0,     0,   305,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   187,   188,   189,     0,   191,   192,   193,
     194,   195,   309,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,   207,   208,   209,     0,     0,   212,   213,
     214,   215,     0,     0,   310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,   910,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,    44,   325,     0,     0,
       0,     0,     0,   289,   290,   291,   292,   930,     0,     0,
       0,     0,   326,   327,   328,   294,   329,     0,     0,   330,
     331,     0,     0,     0,     0,     0,     0,   298,     0,   332,
     300,   333,   334,   301,   162,     0,   302,     0,     0,     0,
     305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,   188,   189,     0,   191,   192,   193,   194,   195,   309,
     197,   198,   199,   200,   201,   202,   203,   204,   205,     0,
     207,   208,   209,     0,     0,   212,   213,   214,   215,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   316,   317,   318,   319,   320,   321,
     322,   323,   324,    44,   325,     0,     0,     0,     0,     0,
     289,   290,   291,   292,     0,     0,     0,     0,     0,   326,
     327,   328,   294,   329,     0,     0,   330,   331,     0,     0,
       0,     0,     0,     0,   298,     0,   332,   300,   333,   334,
     301,   162,     0,   302,     0,     0,     0,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   187,   188,   189,
       0,   191,   192,   193,   194,   195,   309,   197,   198,   199,
     200,   201,   202,   203,   204,   205,     0,   207,   208,   209,
       0,     0,   212,   213,   214,   215,     0,     0,   310,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,   325,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   327,   328,     0,
     329,     0,     0,   330,   331,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,   333,   334,     0,   162,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   575,   503,
     504,   505,   506,   481,   482,     0,   507,   508,   509,   510,
     511,   512,     0,     0,   513,   514,   515,   516,   517,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   576,   520,     0,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,     0,     0,     0,     0,   577,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   575,   503,   504,   505,
     506,   481,   482,     0,   507,   508,   509,   510,   511,   512,
       0,     0,   513,   514,   515,   516,   517,   518,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   576,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,     0,
       0,     0,     0,     0,   590,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   575,   503,   504,   505,   506,   481,
     482,     0,   507,   508,   509,   510,   511,   512,     0,     0,
     513,   514,   515,   516,   517,   518,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   576,   520,     0,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,     0,     0,
       0,     0,     0,     0,   531,     0,   532,     0,     0,     0,
       0,     0,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   575,   503,   504,   505,   506,   481,   482,     0,
     507,   508,   509,   510,   511,   512,     0,     0,   513,   514,
     515,   516,   517,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   576,   520,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,     0,     0,     0,     0,     0,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     575,   503,   504,   505,   506,   481,   482,     0,   507,   508,
     509,   510,   511,   512,     0,     0,   513,   514,   515,   516,
     517,   518,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   576,
     520,     0,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,     0,     0,     0,
     531,     0,   532,     0,     0,     0,     0,     0,   790,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   575,   503,
     504,   505,   506,   481,   482,     0,   507,   508,   509,   510,
     511,   512,     0,     0,   513,   514,   515,   516,   517,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   576,   520,     0,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,     0,     0,     0,     0,   856,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
       0,     0,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,   481,   482,   531,     0,   532,   491,
     492,   493,   494,   495,   919,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,     0,     0,     0,   507,     0,
     509,   510,     0,     0,     0,     0,   513,   514,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,     0,     0,     0,
     531,     0,   532,   481,   482,   616,   491,   492,   493,   494,
     495,     0,     0,   498,   499,   500,   501,     0,   503,   504,
     505,   506,     0,     0,     0,   507,     0,   509,   510,     0,
       0,     0,     0,   513,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,     0,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,     0,     0,   531,     0,   532,
     481,   482,   724,     0,     0,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
       0,     0,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,   481,
     482,   727,   491,   492,   493,   494,   495,     0,     0,   498,
     499,   500,   501,     0,   503,   504,   505,   506,     0,     0,
       0,   507,     0,   509,   510,     0,     0,     0,     0,   513,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,     0,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,     0,     0,   531,     0,   532,   481,   482,   811,     0,
       0,   491,   492,   493,   494,   495,     0,     0,   498,   499,
     500,   501,     0,   503,   504,   505,   506,     0,     0,     0,
     507,     0,   509,   510,     0,     0,     0,     0,   513,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   481,   482,   916,   491,   492,
     493,   494,   495,     0,     0,   498,   499,   500,   501,     0,
     503,   504,   505,   506,     0,     0,     0,   507,     0,   509,
     510,     0,     0,     0,     0,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   520,
       0,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,     0,     0,   531,
       0,   532,   481,   482,   918,     0,     0,   491,   492,   493,
     494,   495,     0,     0,   498,   499,   500,   501,     0,   503,
     504,   505,   506,     0,     0,     0,   507,     0,   509,   510,
       0,     0,     0,     0,   513,   514,   515,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   520,     0,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,   481,   482,   939,   491,   492,   493,   494,   495,     0,
       0,   498,   499,   500,   501,     0,   503,   504,   505,   506,
       0,     0,     0,   507,     0,   509,   510,     0,     0,     0,
       0,   513,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,     0,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,     0,     0,   531,     0,   532,   564,   481,
     482,     0,     0,   491,   492,   493,   494,   495,     0,     0,
     498,   499,   500,   501,     0,   503,   504,   505,   506,     0,
       0,     0,   507,     0,   509,   510,     0,     0,     0,     0,
     513,   514,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,     0,     0,
       0,     0,     0,     0,   531,     0,   532,   776,     0,     0,
       0,   491,   492,   493,   494,   495,   481,   482,   498,   499,
     500,   501,     0,   503,   504,   505,   506,     0,     0,     0,
     507,     0,   509,   510,     0,     0,     0,     0,   513,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   782,     0,     0,     0,     0,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   575,
     503,   504,   505,   506,  -228,   481,   482,   507,   508,   509,
     510,   511,   512,     0,     0,   513,   514,   515,   516,   517,
     518,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,   520,
       0,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,     0,     0,   531,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   481,   482,     0,   507,   508,   509,   510,
     511,   512,     0,     0,   513,   514,   515,   516,   517,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   519,   520,     0,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
     609,     0,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,   481,   482,     0,     0,     0,   531,     0,   532,   491,
     492,   493,   494,   495,     0,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,     0,     0,     0,   507,     0,
     509,   510,     0,     0,     0,     0,   513,   514,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,   780,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   481,   482,     0,     0,     0,
     531,     0,   532,   491,   492,   493,   494,   495,     0,     0,
     498,   499,   500,   501,     0,   503,   504,   505,   506,     0,
       0,     0,   507,     0,   509,   510,     0,     0,     0,     0,
     513,   514,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     832,     0,     0,     0,   520,     0,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,     0,   481,
     482,     0,     0,     0,   531,     0,   532,   491,   492,   493,
     494,   495,     0,     0,   498,   499,   500,   501,     0,   503,
     504,   505,   506,     0,     0,     0,   507,     0,   509,   510,
       0,     0,     0,     0,   513,   514,   515,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   520,     0,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,     0,   481,   482,     0,     0,     0,   531,     0,
     532,   491,   492,   493,   494,   495,     0,     0,   498,   499,
     500,   501,     0,   503,   504,   505,   506,   481,   482,     0,
     507,     0,   509,   510,     0,     0,     0,     0,   513,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,   481,   482,     0,   507,     0,   509,   510,     0,   491,
     492,   493,   494,   495,     0,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,   481,   482,     0,   507,     0,
     509,   510,     0,     0,     0,     0,     0,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,     0,
       0,     0,     0,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,     0,     0,     0,
     531,     0,   532,   491,   492,   493,   494,   495,     0,     0,
     498,   499,   500,   501,     0,   503,   504,   505,   506,   481,
     482,     0,   507,     0,   509,   510,     0,   491,   492,   493,
     494,   495,     0,     0,   498,   499,   500,   501,     0,   503,
     504,   505,   506,     0,     0,     0,   507,     0,   509,   510,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,     0,     0,
       0,     0,     0,     0,   531,     0,   532,     0,     0,     0,
       0,     0,     0,   524,   525,   526,   527,   528,   529,   530,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,   491,   492,   493,   494,   495,     0,     0,   498,     0,
       0,   501,     0,   503,   504,   505,   506,     0,     0,     0,
     507,     0,   509,   510,     0,     0,     0,     0,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,     0,     0,     0,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,   524,   525,   526,
     527,   528,   529,   530,   185,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,     0,     0,     0,   186,     0,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,   185,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,    44,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,   185,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,    44,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,     0,     0,     0,     0,
       0,   184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   185,     0,     0,     0,
       0,     0,   316,   317,   318,     0,     0,     0,     0,     0,
     186,    44,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   183,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,    44,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,     0,     0,     0,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
     137,   138,   139,    43,     0,   140,   141,   142,   143,   144,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,     0,     0,   156,   157
};

static const yytype_int16 yycheck[] =
{
      13,    82,   280,   110,   237,   276,   278,   293,   318,   533,
     393,    74,   395,   455,   397,   553,   599,   555,     7,   557,
       0,   167,   314,   315,   607,   311,     6,   621,    34,   118,
      20,    21,    46,    14,    15,   110,    21,    32,   128,    18,
      49,   127,   113,   115,   116,    19,   570,   571,   135,    29,
      19,    31,   144,    33,   138,    35,   113,   362,    49,    39,
      19,   154,    75,   182,   154,   122,    46,   113,    53,   113,
     113,   144,    52,   378,   151,   144,   122,    32,   122,   122,
     151,   152,   128,   154,   112,   177,   157,   115,   116,   182,
      70,   178,   181,    14,    15,   152,   182,   110,   104,   105,
     157,    56,    57,   175,   181,   144,   152,   128,   152,   152,
     183,    91,   102,   103,   104,   105,   151,   144,   151,    33,
     151,   144,   112,   144,   114,   115,   116,   117,   235,   144,
     144,   121,   418,   123,   124,   144,   144,   158,   144,   425,
     113,   176,   288,   737,   183,   173,   135,   175,   181,   122,
     181,   164,   735,    58,   151,   151,   183,   138,   444,   114,
     235,   142,   157,   144,   145,   144,   152,   230,   275,   144,
     160,   161,   162,   163,   164,    20,    21,   151,   163,   152,
     175,   178,   151,   173,   291,   175,   175,   479,   173,   184,
     181,   151,   151,   144,   183,   175,   177,   152,   153,   144,
     275,   181,   157,   151,   151,   160,   152,   731,   183,   316,
     317,   154,   358,   151,   128,   296,   291,   138,   178,   743,
     175,   142,   235,   144,   145,   151,   181,   308,   151,   184,
     144,   178,   183,   771,   144,   181,   819,   128,   183,   151,
     178,   316,   317,   626,   159,   178,   829,   144,   181,   773,
     151,   154,   178,   144,   128,   178,   159,   144,   544,   104,
     105,   151,   275,   176,   176,   151,   181,   112,   181,   114,
     115,   116,   117,   380,   177,   176,   121,   159,   291,   386,
     387,   388,   389,    11,   144,   392,   176,   394,   566,   396,
     176,   398,   177,   735,    22,    23,   159,   835,   159,   181,
     610,   593,   744,   316,   317,   380,   452,   144,   454,   416,
     154,   386,   387,   388,   389,   159,   329,   392,   181,   394,
     181,   396,   181,    65,   183,    20,    21,    69,   173,   181,
     175,   183,   154,   177,   182,   154,   443,   159,   138,   420,
     159,   416,   146,   147,    86,    87,    88,    89,   151,    20,
      21,   154,   433,   144,   157,   177,     4,     5,   177,     7,
     144,   179,   180,   144,   182,    42,    43,   380,   443,   152,
     182,   754,   182,   386,   387,   388,   389,   144,    34,   392,
      32,   394,   177,   396,    34,   398,   146,   147,   148,   182,
     471,   154,     1,   146,    53,   182,   144,   144,   154,   158,
     182,   181,   177,   416,    56,    57,   158,   158,   694,   104,
     105,   424,   158,   158,   158,   158,   181,   112,   690,   114,
     115,   116,   117,   158,   158,   158,   121,   154,   144,    32,
     443,   102,   103,   104,   105,   106,   144,   135,   109,   110,
     111,   112,   713,   114,   115,   116,   117,   177,   177,   181,
     121,   158,   123,   124,   467,   158,   177,   181,   129,   130,
     131,   113,   114,   138,   158,   160,   161,   162,   163,   164,
     122,   158,    36,   178,   581,   582,   180,   584,   173,   181,
     175,   181,   153,   175,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   177,   181,   181,   578,    20,    21,
     152,   153,   173,   181,   175,   157,   581,   582,   160,   584,
     144,   182,   176,   176,   118,    13,   154,   138,   180,   135,
     144,   534,   755,   175,    20,    21,   144,   182,     7,   181,
     144,   144,   184,    42,   181,   176,   151,   183,   146,   151,
     146,   158,   144,   821,   177,   144,     1,   144,   159,   159,
     144,   144,   159,    20,    21,   183,   176,   158,   146,   144,
     144,   144,    32,   144,   183,    49,   144,   459,   581,   582,
     159,   584,   177,   159,    55,    99,   176,   880,   176,   247,
     176,   176,   104,   105,   158,   176,    56,    57,   178,   181,
     112,   698,   114,   115,   116,   117,   176,   176,   158,   121,
     176,   176,   709,   176,   711,   176,   102,   103,   104,   105,
     106,   181,   178,   109,   110,   111,   112,   176,   114,   115,
     116,   117,   183,   698,   176,   121,   176,   123,   124,   178,
     178,    58,    56,   129,   709,   131,    75,   104,   105,   534,
     162,   163,   164,   854,   114,   112,   753,   786,   115,   116,
     117,   173,   619,   175,   121,     1,   845,   282,   378,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   729,
     688,   711,   540,   399,   358,   756,    39,   173,   753,   175,
     180,    68,   152,   153,   742,   698,   237,   157,   905,   455,
     160,    -1,   391,    -1,    -1,    -1,   709,    -1,   711,   250,
      -1,    -1,    -1,    -1,   291,   175,   173,   177,   175,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,
     271,    -1,    20,    21,    -1,    -1,   807,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,
     753,   848,   293,    -1,    -1,    -1,   297,    -1,   299,    -1,
      -1,    -1,    -1,    -1,    -1,   862,   307,   838,    -1,    -1,
     311,   312,   313,    -1,    -1,    -1,   847,   318,    -1,    -1,
      32,    -1,    -1,   848,    -1,   326,   327,   328,    -1,   330,
     331,   332,    -1,    -1,    -1,    -1,    -1,   862,    -1,    -1,
     341,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   929,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,   129,   130,   131,    -1,   848,    -1,   944,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   862,
      -1,    -1,   114,    -1,   929,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   418,    -1,   944,
      -1,    -1,    -1,    -1,   425,   173,    -1,   175,    -1,    -1,
      -1,   432,    -1,   181,    -1,   436,    -1,    -1,    -1,    -1,
     152,   153,    -1,   444,   907,   157,    -1,   159,   160,    -1,
      -1,    -1,   453,    -1,   455,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   925,   175,    -1,    -1,   929,    -1,    32,    -1,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   944,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,    56,    57,   495,   496,   497,   498,   499,   500,
      -1,   502,   503,    -1,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
      -1,   532,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   544,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   572,    -1,    -1,   575,   576,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,
      -1,    -1,    -1,   157,   595,   596,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   606,    -1,    -1,   609,   610,
      -1,   175,   613,    -1,    -1,    -1,    -1,   181,    -1,    -1,
     184,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,   648,   649,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    49,    -1,    51,
      -1,    -1,    -1,    -1,    56,    57,   667,   668,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,   694,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   715,    -1,    -1,    -1,    -1,    -1,
      20,    21,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   733,   734,    -1,    -1,   128,    -1,    -1,   740,
      -1,   742,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   755,    -1,    -1,    -1,   759,    -1,
     152,   153,   763,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,   164,    -1,    -1,   167,   168,    -1,    -1,   780,
      -1,    -1,    -1,   175,    -1,   177,    -1,   179,   180,    -1,
     182,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,   159,
      32,    -1,   863,   163,   865,   866,   867,   868,   869,    -1,
      -1,   872,   873,   173,   875,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   884,    56,    57,    -1,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,   910,
      -1,    25,    26,    27,    28,    -1,   917,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,
      44,   932,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,   114,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    93,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,   160,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   179,   180,   181,   182,   183,
       1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      41,    -1,    -1,    44,    -1,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      32,    -1,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,    -1,    -1,
      -1,    14,    15,    16,    17,    -1,   177,    -1,   179,   180,
     181,   182,   183,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   114,    56,    57,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,   160,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,   128,    -1,    32,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,   152,
     153,    56,    57,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    14,    15,    16,    17,
      -1,    -1,   175,    -1,   177,    -1,   179,   180,    26,   182,
      -1,   184,    -1,    32,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    56,    57,   114,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,   160,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
     128,    32,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,   152,   153,    56,    57,    -1,   157,   157,
     159,   160,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    -1,   175,   175,    -1,   177,
      -1,   179,   180,    26,   182,   184,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,   152,   153,    -1,    -1,    -1,   157,    -1,   159,   160,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,   175,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   128,    -1,    32,    -1,    32,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    56,    57,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,   113,   114,
      -1,   114,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,   152,   153,   152,
     153,    -1,   157,    -1,   157,   160,   159,   160,   104,   105,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
     175,    -1,   175,   119,   120,    -1,    -1,    -1,    -1,   184,
      -1,   184,   128,    -1,    -1,    56,    57,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,   160,   161,   162,    26,   164,    -1,
      -1,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   177,    41,   179,   180,    44,   182,    -1,    47,    -1,
      -1,    -1,    51,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,   152,   153,    92,    -1,    -1,   157,    -1,   159,   160,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
     119,   120,    -1,   184,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    56,    57,    -1,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   160,   161,   162,    26,   164,    -1,    -1,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   177,    41,
     179,   180,    44,   182,    -1,    47,    -1,    -1,    -1,    51,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   152,   153,
      92,    -1,    -1,   157,    -1,   159,   160,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    32,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,
     162,    -1,   164,    -1,    -1,   167,   168,    14,    15,    16,
      17,    -1,    -1,   175,    -1,   177,   178,   179,   180,    26,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,   152,   153,    92,    -1,    -1,   157,    -1,
     159,   160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,   119,   120,    -1,   184,    -1,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    56,    57,    -1,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    -1,   160,   161,   162,    26,   164,    -1,    -1,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     177,    41,   179,   180,    44,   182,    -1,    47,    -1,    -1,
      -1,    51,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
     152,   153,    92,    -1,    -1,   157,    -1,   159,   160,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,   119,
     120,    -1,   184,    -1,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    56,    57,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     160,   161,   162,    26,   164,    -1,    -1,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   177,    41,   179,
     180,    44,   182,    -1,    47,    -1,    -1,    -1,    51,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,   152,   153,    92,
      -1,    -1,   157,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,    56,    57,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,   158,    -1,   160,   161,   162,
      26,   164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   177,    41,   179,   180,    44,   182,
      -1,    47,    -1,    -1,    -1,    51,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,   152,   153,    92,    -1,    -1,   157,
      -1,   159,   160,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    56,    57,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,   160,   161,   162,    26,   164,    -1,
      -1,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    38,
     176,   177,    41,   179,   180,    44,   182,    -1,    47,    -1,
      -1,    -1,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,   152,   153,    92,    -1,    -1,   157,    -1,   159,   160,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    56,    57,    -1,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    21,
      -1,   160,   161,   162,    26,   164,    -1,    -1,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   177,    41,
     179,   180,    44,   182,    -1,    47,    -1,    -1,    -1,    51,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   152,   153,
      92,    -1,    -1,   157,    -1,   159,   160,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,    56,
      57,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,   160,   161,
     162,    26,   164,    -1,    -1,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   177,    41,   179,   180,    44,
     182,    -1,    47,    -1,    -1,    -1,    51,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   152,   153,    92,    -1,    -1,
     157,    -1,   159,   160,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
      32,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,   161,   162,    -1,   164,
      -1,    -1,   167,   168,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,    26,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
     152,   153,    92,    -1,    -1,   157,    -1,    -1,   160,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,    26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,   161,   162,    -1,   164,
      -1,    -1,   167,   168,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,    26,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,    26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,   161,   162,    -1,   164,
      -1,    -1,   167,   168,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   177,   178,   179,   180,    26,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    26,   164,    -1,    -1,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   177,    41,   179,
     180,    44,   182,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,    26,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   160,   161,   162,    26,   164,    -1,    -1,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   177,
      41,   179,   180,    44,   182,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,   160,
     161,   162,    26,   164,    -1,    -1,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   177,    41,   179,   180,
      44,   182,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   179,   180,    -1,   182,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    20,    21,    -1,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    20,    21,    -1,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    20,
      21,    -1,   121,   122,   123,   124,   125,   126,    -1,    -1,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    20,    21,    -1,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    20,    21,    -1,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    20,    21,    -1,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,   173,    -1,   175,   102,
     103,   104,   105,   106,   181,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    20,    21,   178,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      20,    21,   178,    -1,    -1,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    20,
      21,   178,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    20,    21,   178,    -1,
      -1,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    20,    21,   178,   102,   103,
     104,   105,   106,    -1,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    20,    21,   178,    -1,    -1,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    20,    21,   178,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,   176,    20,
      21,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,   176,    -1,    -1,
      -1,   102,   103,   104,   105,   106,    20,    21,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,   176,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,   125,   126,    -1,    -1,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    20,    21,    -1,   121,   122,   123,   124,
     125,   126,    -1,    -1,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
     127,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   173,    -1,   175,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     173,    -1,   175,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   173,    -1,   175,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   173,    -1,
     175,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    20,    21,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    20,    21,    -1,   121,    -1,   123,   124,    -1,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    20,    21,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    20,
      21,    -1,   121,    -1,   123,   124,    -1,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     161,   162,   163,   164,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,   144,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,   144,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    45,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      59,   144,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,   144,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   128,    -1,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   186,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    70,    91,   175,   181,   187,   195,   196,   200,   220,
     224,   239,   296,   301,   302,   306,   342,   344,   144,   215,
     216,   138,   201,   202,   144,   197,   198,   144,   182,   144,
     345,   343,    33,   128,   144,   217,   218,   219,   231,     4,
       5,     7,   305,    58,   295,   152,   151,   154,   151,    21,
      53,   163,   173,   199,   154,   182,   144,   299,   300,   295,
     144,   144,   144,   128,   177,   151,   176,    34,   104,   105,
     144,   223,   225,   226,   144,   304,   182,   297,    14,    15,
     138,   142,   144,   145,   177,   189,   215,   138,   202,   144,
     144,   144,    65,    69,    86,    87,    88,    89,   310,   298,
     152,   183,   299,   182,   182,   144,   216,   218,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   123,   124,   125,   126,
     129,   130,   131,   132,   133,   134,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   167,   168,    34,    34,
     177,   221,   182,   227,   154,   303,   307,   294,     1,   146,
     188,    14,    15,   138,   142,   144,   189,   213,   214,   199,
     182,   144,   183,    24,    30,    45,    59,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   231,   309,
     311,   312,   314,   317,   144,   313,   144,   287,   288,   178,
     135,   178,   278,   283,   284,   154,   222,   229,   231,   182,
     144,   183,   293,   308,   181,   146,   147,   151,   178,   298,
     152,   181,   158,   158,   158,   158,   177,   158,   158,   158,
     158,   158,   158,    32,    56,    57,   114,   152,   153,   157,
     160,   175,   181,   184,   181,   154,   181,   216,    46,   290,
     308,   178,   181,   317,     1,     8,     9,    10,    12,    14,
      15,    16,    17,    25,    26,    27,    28,    37,    38,    40,
      41,    44,    47,    48,    50,    51,    54,    55,    60,    70,
      92,    93,   104,   105,   119,   120,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   145,   160,   161,   162,   164,
     167,   168,   177,   179,   180,   181,   183,   191,   193,   203,
     204,   207,   208,   209,   210,   211,   212,   227,   228,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   248,   257,   261,   262,   263,   264,   265,   266,   267,
     272,   275,   309,   346,   350,   355,   357,   359,   281,   151,
     154,   157,   292,   213,   183,   275,   320,   322,   324,   318,
     144,   315,   326,   328,   330,   332,   334,   336,   338,   340,
      32,    32,   157,   175,   184,   176,   275,   144,   183,   317,
     183,   287,   176,   290,   113,   152,   154,   157,   289,   283,
     275,   308,   317,   346,   177,   113,   181,   230,   263,   275,
     227,   275,   177,   275,   158,   158,   177,   181,   158,   138,
      54,   275,   227,   158,   113,   230,   275,   275,   275,   180,
     261,   261,    12,   317,    12,   317,   275,   352,   356,   190,
     275,   275,   275,   231,   275,   275,   275,   180,    36,   181,
     181,   275,   144,   177,   181,   181,   181,   293,   220,   118,
     181,    20,    21,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   121,   122,   123,
     124,   125,   126,   129,   130,   131,   132,   133,   134,   152,
     153,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   173,   175,   177,   175,   183,   220,   278,   280,   144,
     317,   113,   122,   152,   291,   181,   317,   317,   317,   317,
     178,   313,   317,   221,   317,   221,   317,   221,   144,   285,
     286,   317,   288,   176,   176,   176,   317,   230,   275,   227,
      19,   177,   231,   230,   275,   113,   152,   181,    13,   275,
     227,   242,   244,   275,   246,   154,   138,   317,   230,   275,
     181,   180,   308,    49,   144,   160,   177,   275,   347,   348,
     349,   351,   352,   353,   354,   308,   177,   348,   354,   127,
     181,   183,   147,   148,   188,   194,   178,   158,   231,   182,
     227,   175,   178,   255,   275,   135,   260,   261,    18,   144,
     144,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   144,   119,   120,
     275,   275,   144,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   119,   120,   275,
      21,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   144,   273,   275,   178,   255,   219,     7,    42,
     279,   181,   152,   181,   291,   230,   275,   159,   181,   159,
     159,   159,   181,   159,   222,   159,   222,   159,   222,   154,
     159,   181,   159,   181,   152,   289,   255,   178,   255,    18,
     158,   275,   181,   227,   178,   317,   317,   178,   317,   159,
     181,    19,   261,   122,   289,   178,   151,   181,   349,   176,
     151,   176,   181,    19,   178,   349,   183,   275,   352,   183,
     275,   146,   192,   268,   270,   229,    11,    22,    23,   205,
     206,   144,   347,   151,   178,   113,   122,   152,   157,   258,
     259,   221,   158,   177,   275,   275,   176,   275,   275,   144,
     154,     1,   176,   178,   176,    43,   279,   290,   230,   275,
     181,   321,   317,   325,   319,   316,   327,   159,   331,   159,
     335,   159,   317,   339,   285,   341,   275,   181,   178,   158,
     144,   178,   159,   159,   159,   260,   255,   275,   275,   348,
     349,   144,   347,   176,   176,   275,   176,   353,   255,   348,
     183,   176,   149,   146,   317,   221,   183,   227,   275,   176,
     275,   144,   144,   144,   144,   151,   176,   222,   276,   178,
     255,   275,   274,   181,   282,   226,   181,   159,   329,   333,
     337,   227,   249,   159,   181,   243,   245,   247,   177,   181,
     349,   176,   122,   289,   176,   181,   349,   176,   159,   222,
     227,   178,   258,   176,   127,   227,   256,   317,   178,   225,
     323,   317,    18,   275,   144,   275,   275,   275,   178,   275,
     275,   176,   176,   275,   275,   275,   183,   269,   159,   206,
     113,   275,   159,   181,   159,   158,   178,   159,   178,   181,
     358,   176,   358,   176,   231,   271,   275,   277,   250,   251,
      18,   275,    49,   176,   183,   231,   178,   317,   158,   178,
     275,   176,   176,   159,   253,   252,   317,   178,   159,   254,
     178
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   185,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   187,   188,   188,   189,   190,
     190,   190,   191,   192,   192,   194,   193,   195,   196,   197,
     197,   197,   198,   198,   199,   199,   200,   201,   201,   202,
     202,   203,   204,   204,   205,   205,   206,   206,   206,   207,
     207,   208,   209,   210,   211,   212,   213,   213,   213,   213,
     213,   213,   214,   214,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   216,   217,   217,   218,   218,   219,   219,
     220,   220,   221,   221,   221,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     224,   225,   226,   227,   227,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   229,   229,   229,   230,   230,   230,
     231,   231,   231,   232,   233,   233,   233,   233,   234,   235,
     236,   236,   236,   236,   236,   237,   237,   237,   237,   238,
     239,   239,   240,   242,   243,   241,   244,   245,   241,   246,
     247,   241,   248,   248,   248,   249,   250,   248,   251,   252,
     248,   253,   254,   248,   255,   255,   256,   256,   256,   257,
     257,   257,   258,   258,   258,   258,   259,   259,   260,   260,
     261,   261,   262,   262,   262,   262,   262,   262,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   264,   264,   264,
     264,   265,   266,   266,   267,   268,   269,   267,   270,   271,
     267,   272,   273,   274,   272,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   276,
     277,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   278,   278,   279,   279,   280,   281,
     281,   282,   281,   281,   281,   283,   283,   284,   284,   285,
     285,   286,   286,   287,   288,   288,   289,   289,   290,   290,
     290,   290,   290,   290,   291,   291,   291,   292,   292,   293,
     293,   293,   293,   293,   294,   294,   295,   295,   296,   297,
     296,   298,   298,   298,   299,   300,   300,   301,   302,   302,
     303,   303,   304,   305,   305,   307,   306,   308,   308,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   310,   310,   310,   310,   310,   310,
     311,   312,   312,   313,   313,   315,   316,   314,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   318,   319,   317,   317,   320,
     321,   317,   322,   323,   317,   324,   325,   317,   317,   326,
     327,   317,   328,   329,   317,   317,   330,   331,   317,   332,
     333,   317,   317,   334,   335,   317,   336,   337,   317,   338,
     339,   317,   340,   341,   317,   343,   342,   345,   344,   346,
     346,   346,   346,   347,   347,   347,   347,   348,   348,   349,
     349,   350,   350,   350,   350,   350,   350,   351,   351,   352,
     353,   353,   354,   354,   355,   355,   356,   356,   357,   358,
     358,   359,   359
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     1,     2,     3,     0,
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
       1,     2,     2,     2,     0,     2,     2,     3,     2,     2,
       1,     3,     2,     2,     2,     4,     5,     2,     1,     1,
       2,     3,     4,     2,     3,     3,     4,     2,     3,     4,
       1,     1,     2,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     5,     8,    10,     0,     0,    10,     0,     0,
      13,     0,     0,    15,     1,     3,     1,     2,     3,     1,
       1,     2,     2,     2,     2,     2,     1,     3,     0,     4,
       1,     6,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     6,     5,     6,     3,     0,     0,     8,     0,     0,
       9,     3,     0,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     3,     4,     4,     3,     1,     3,     4,
       3,     4,     2,     4,     4,     7,     8,     3,     5,     0,
       0,     8,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     0,     4,     0,     1,     3,     0,
       3,     0,     7,     5,     5,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     2,     0,     1,     5,     0,
       5,     0,     3,     5,     4,     1,     2,     4,     5,     7,
       0,     2,     2,     1,     1,     0,     7,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     3,     0,     0,     6,     1,     1,
       1,     1,     4,     3,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     8,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     0,     7,     0,     7,     1,
       1,     1,     1,     3,     3,     5,     5,     1,     3,     0,
       2,     6,     5,     7,     8,     6,     8,     1,     3,     3,
       1,     1,     1,     3,     5,     5,     1,     3,     4,     0,
       3,    10,    10
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
    case 144: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3441 "ds_parser.cpp"
        break;

    case 188: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3447 "ds_parser.cpp"
        break;

    case 189: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3453 "ds_parser.cpp"
        break;

    case 190: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3459 "ds_parser.cpp"
        break;

    case 191: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3465 "ds_parser.cpp"
        break;

    case 193: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3471 "ds_parser.cpp"
        break;

    case 197: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3477 "ds_parser.cpp"
        break;

    case 203: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3483 "ds_parser.cpp"
        break;

    case 204: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3489 "ds_parser.cpp"
        break;

    case 206: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3495 "ds_parser.cpp"
        break;

    case 208: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 209: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3507 "ds_parser.cpp"
        break;

    case 210: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3513 "ds_parser.cpp"
        break;

    case 211: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3519 "ds_parser.cpp"
        break;

    case 212: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3525 "ds_parser.cpp"
        break;

    case 213: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3531 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3537 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3543 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3549 "ds_parser.cpp"
        break;

    case 217: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3555 "ds_parser.cpp"
        break;

    case 218: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3561 "ds_parser.cpp"
        break;

    case 219: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3567 "ds_parser.cpp"
        break;

    case 220: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3573 "ds_parser.cpp"
        break;

    case 221: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3579 "ds_parser.cpp"
        break;

    case 222: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3585 "ds_parser.cpp"
        break;

    case 223: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3591 "ds_parser.cpp"
        break;

    case 227: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3597 "ds_parser.cpp"
        break;

    case 228: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3603 "ds_parser.cpp"
        break;

    case 229: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3609 "ds_parser.cpp"
        break;

    case 230: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3615 "ds_parser.cpp"
        break;

    case 231: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3621 "ds_parser.cpp"
        break;

    case 232: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3627 "ds_parser.cpp"
        break;

    case 233: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3633 "ds_parser.cpp"
        break;

    case 234: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3639 "ds_parser.cpp"
        break;

    case 235: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3645 "ds_parser.cpp"
        break;

    case 236: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 237: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3657 "ds_parser.cpp"
        break;

    case 238: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 240: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3669 "ds_parser.cpp"
        break;

    case 241: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 248: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 255: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 256: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 258: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3699 "ds_parser.cpp"
        break;

    case 259: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3705 "ds_parser.cpp"
        break;

    case 260: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3711 "ds_parser.cpp"
        break;

    case 261: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3717 "ds_parser.cpp"
        break;

    case 262: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3723 "ds_parser.cpp"
        break;

    case 263: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3729 "ds_parser.cpp"
        break;

    case 264: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3735 "ds_parser.cpp"
        break;

    case 265: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3741 "ds_parser.cpp"
        break;

    case 266: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3747 "ds_parser.cpp"
        break;

    case 267: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3753 "ds_parser.cpp"
        break;

    case 272: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3759 "ds_parser.cpp"
        break;

    case 275: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3765 "ds_parser.cpp"
        break;

    case 278: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3771 "ds_parser.cpp"
        break;

    case 280: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3777 "ds_parser.cpp"
        break;

    case 281: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3783 "ds_parser.cpp"
        break;

    case 283: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3789 "ds_parser.cpp"
        break;

    case 284: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3795 "ds_parser.cpp"
        break;

    case 285: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3801 "ds_parser.cpp"
        break;

    case 286: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3807 "ds_parser.cpp"
        break;

    case 287: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3813 "ds_parser.cpp"
        break;

    case 288: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3819 "ds_parser.cpp"
        break;

    case 290: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3825 "ds_parser.cpp"
        break;

    case 293: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3831 "ds_parser.cpp"
        break;

    case 294: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3837 "ds_parser.cpp"
        break;

    case 298: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3843 "ds_parser.cpp"
        break;

    case 303: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3849 "ds_parser.cpp"
        break;

    case 308: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3855 "ds_parser.cpp"
        break;

    case 311: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3861 "ds_parser.cpp"
        break;

    case 312: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3867 "ds_parser.cpp"
        break;

    case 313: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3873 "ds_parser.cpp"
        break;

    case 314: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3879 "ds_parser.cpp"
        break;

    case 317: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3885 "ds_parser.cpp"
        break;

    case 346: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3891 "ds_parser.cpp"
        break;

    case 347: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3897 "ds_parser.cpp"
        break;

    case 348: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3903 "ds_parser.cpp"
        break;

    case 349: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3909 "ds_parser.cpp"
        break;

    case 350: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3915 "ds_parser.cpp"
        break;

    case 351: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3921 "ds_parser.cpp"
        break;

    case 352: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3927 "ds_parser.cpp"
        break;

    case 353: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3933 "ds_parser.cpp"
        break;

    case 354: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3939 "ds_parser.cpp"
        break;

    case 355: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3945 "ds_parser.cpp"
        break;

    case 356: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3951 "ds_parser.cpp"
        break;

    case 357: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3957 "ds_parser.cpp"
        break;

    case 358: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3963 "ds_parser.cpp"
        break;

    case 359: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3969 "ds_parser.cpp"
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
#line 458 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        if ( !g_Program->policies.ignore_shared_modules ) {
            g_Program->promoteToBuiltin = (yyvsp[0].b);
        }
        delete (yyvsp[-1].s);
    }
#line 4272 "ds_parser.cpp"
    break;

  case 16:
#line 473 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4278 "ds_parser.cpp"
    break;

  case 17:
#line 474 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4284 "ds_parser.cpp"
    break;

  case 18:
#line 478 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4290 "ds_parser.cpp"
    break;

  case 19:
#line 482 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4300 "ds_parser.cpp"
    break;

  case 20:
#line 487 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4314 "ds_parser.cpp"
    break;

  case 21:
#line 496 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4324 "ds_parser.cpp"
    break;

  case 22:
#line 504 "ds_parser.ypp"
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
#line 4342 "ds_parser.cpp"
    break;

  case 23:
#line 520 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4352 "ds_parser.cpp"
    break;

  case 24:
#line 525 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4362 "ds_parser.cpp"
    break;

  case 25:
#line 533 "ds_parser.ypp"
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
#line 4389 "ds_parser.cpp"
    break;

  case 26:
#line 554 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4401 "ds_parser.cpp"
    break;

  case 27:
#line 563 "ds_parser.ypp"
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
#line 4425 "ds_parser.cpp"
    break;

  case 29:
#line 589 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4433 "ds_parser.cpp"
    break;

  case 30:
#line 592 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4444 "ds_parser.cpp"
    break;

  case 31:
#line 598 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4455 "ds_parser.cpp"
    break;

  case 32:
#line 607 "ds_parser.ypp"
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
#line 4481 "ds_parser.cpp"
    break;

  case 33:
#line 628 "ds_parser.ypp"
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
#line 4506 "ds_parser.cpp"
    break;

  case 34:
#line 651 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4512 "ds_parser.cpp"
    break;

  case 35:
#line 652 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4518 "ds_parser.cpp"
    break;

  case 39:
#line 665 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4526 "ds_parser.cpp"
    break;

  case 40:
#line 668 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4534 "ds_parser.cpp"
    break;

  case 41:
#line 674 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4542 "ds_parser.cpp"
    break;

  case 42:
#line 680 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4550 "ds_parser.cpp"
    break;

  case 43:
#line 683 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4558 "ds_parser.cpp"
    break;

  case 44:
#line 689 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4564 "ds_parser.cpp"
    break;

  case 45:
#line 690 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4570 "ds_parser.cpp"
    break;

  case 46:
#line 694 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4576 "ds_parser.cpp"
    break;

  case 47:
#line 695 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4582 "ds_parser.cpp"
    break;

  case 48:
#line 696 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4593 "ds_parser.cpp"
    break;

  case 49:
#line 705 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4599 "ds_parser.cpp"
    break;

  case 50:
#line 706 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4605 "ds_parser.cpp"
    break;

  case 51:
#line 710 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4616 "ds_parser.cpp"
    break;

  case 52:
#line 719 "ds_parser.ypp"
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
#line 4634 "ds_parser.cpp"
    break;

  case 53:
#line 735 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4644 "ds_parser.cpp"
    break;

  case 54:
#line 743 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4656 "ds_parser.cpp"
    break;

  case 55:
#line 753 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4667 "ds_parser.cpp"
    break;

  case 56:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4673 "ds_parser.cpp"
    break;

  case 57:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4679 "ds_parser.cpp"
    break;

  case 58:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4685 "ds_parser.cpp"
    break;

  case 59:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4691 "ds_parser.cpp"
    break;

  case 60:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4697 "ds_parser.cpp"
    break;

  case 61:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4703 "ds_parser.cpp"
    break;

  case 62:
#line 771 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4713 "ds_parser.cpp"
    break;

  case 63:
#line 776 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4723 "ds_parser.cpp"
    break;

  case 64:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4729 "ds_parser.cpp"
    break;

  case 65:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4735 "ds_parser.cpp"
    break;

  case 66:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4741 "ds_parser.cpp"
    break;

  case 67:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4747 "ds_parser.cpp"
    break;

  case 68:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4753 "ds_parser.cpp"
    break;

  case 69:
#line 789 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4759 "ds_parser.cpp"
    break;

  case 70:
#line 790 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4765 "ds_parser.cpp"
    break;

  case 71:
#line 791 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4773 "ds_parser.cpp"
    break;

  case 72:
#line 797 "ds_parser.ypp"
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
#line 4791 "ds_parser.cpp"
    break;

  case 73:
#line 810 "ds_parser.ypp"
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
#line 4809 "ds_parser.cpp"
    break;

  case 74:
#line 826 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4815 "ds_parser.cpp"
    break;

  case 75:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4821 "ds_parser.cpp"
    break;

  case 76:
#line 831 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4833 "ds_parser.cpp"
    break;

  case 77:
#line 838 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4847 "ds_parser.cpp"
    break;

  case 78:
#line 850 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4856 "ds_parser.cpp"
    break;

  case 79:
#line 854 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4865 "ds_parser.cpp"
    break;

  case 80:
#line 861 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4871 "ds_parser.cpp"
    break;

  case 81:
#line 862 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4877 "ds_parser.cpp"
    break;

  case 82:
#line 866 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4883 "ds_parser.cpp"
    break;

  case 83:
#line 867 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4889 "ds_parser.cpp"
    break;

  case 84:
#line 868 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4895 "ds_parser.cpp"
    break;

  case 85:
#line 872 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4905 "ds_parser.cpp"
    break;

  case 86:
#line 877 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4914 "ds_parser.cpp"
    break;

  case 87:
#line 884 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4923 "ds_parser.cpp"
    break;

  case 88:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4929 "ds_parser.cpp"
    break;

  case 89:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4935 "ds_parser.cpp"
    break;

  case 90:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4941 "ds_parser.cpp"
    break;

  case 91:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4947 "ds_parser.cpp"
    break;

  case 92:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4953 "ds_parser.cpp"
    break;

  case 93:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4959 "ds_parser.cpp"
    break;

  case 94:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4965 "ds_parser.cpp"
    break;

  case 95:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4971 "ds_parser.cpp"
    break;

  case 96:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4977 "ds_parser.cpp"
    break;

  case 97:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4983 "ds_parser.cpp"
    break;

  case 98:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4989 "ds_parser.cpp"
    break;

  case 99:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4995 "ds_parser.cpp"
    break;

  case 100:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5001 "ds_parser.cpp"
    break;

  case 101:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5007 "ds_parser.cpp"
    break;

  case 102:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5013 "ds_parser.cpp"
    break;

  case 103:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5019 "ds_parser.cpp"
    break;

  case 104:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5025 "ds_parser.cpp"
    break;

  case 105:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5031 "ds_parser.cpp"
    break;

  case 106:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5037 "ds_parser.cpp"
    break;

  case 107:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5043 "ds_parser.cpp"
    break;

  case 108:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5049 "ds_parser.cpp"
    break;

  case 109:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5055 "ds_parser.cpp"
    break;

  case 110:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5061 "ds_parser.cpp"
    break;

  case 111:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5067 "ds_parser.cpp"
    break;

  case 112:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5073 "ds_parser.cpp"
    break;

  case 113:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5079 "ds_parser.cpp"
    break;

  case 114:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5085 "ds_parser.cpp"
    break;

  case 115:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5091 "ds_parser.cpp"
    break;

  case 116:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5097 "ds_parser.cpp"
    break;

  case 117:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5103 "ds_parser.cpp"
    break;

  case 118:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5109 "ds_parser.cpp"
    break;

  case 119:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5115 "ds_parser.cpp"
    break;

  case 120:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5121 "ds_parser.cpp"
    break;

  case 121:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5127 "ds_parser.cpp"
    break;

  case 122:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5133 "ds_parser.cpp"
    break;

  case 123:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5139 "ds_parser.cpp"
    break;

  case 124:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5145 "ds_parser.cpp"
    break;

  case 125:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5151 "ds_parser.cpp"
    break;

  case 126:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5157 "ds_parser.cpp"
    break;

  case 127:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5163 "ds_parser.cpp"
    break;

  case 128:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5169 "ds_parser.cpp"
    break;

  case 129:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5175 "ds_parser.cpp"
    break;

  case 130:
#line 933 "ds_parser.ypp"
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt((yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
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
#line 5199 "ds_parser.cpp"
    break;

  case 131:
#line 955 "ds_parser.ypp"
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
#line 5239 "ds_parser.cpp"
    break;

  case 132:
#line 993 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5248 "ds_parser.cpp"
    break;

  case 133:
#line 1000 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5257 "ds_parser.cpp"
    break;

  case 134:
#line 1004 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5270 "ds_parser.cpp"
    break;

  case 135:
#line 1015 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5276 "ds_parser.cpp"
    break;

  case 136:
#line 1016 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5282 "ds_parser.cpp"
    break;

  case 137:
#line 1017 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5288 "ds_parser.cpp"
    break;

  case 138:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5294 "ds_parser.cpp"
    break;

  case 139:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5300 "ds_parser.cpp"
    break;

  case 140:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5306 "ds_parser.cpp"
    break;

  case 141:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5312 "ds_parser.cpp"
    break;

  case 142:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5318 "ds_parser.cpp"
    break;

  case 143:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5324 "ds_parser.cpp"
    break;

  case 144:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5330 "ds_parser.cpp"
    break;

  case 145:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5336 "ds_parser.cpp"
    break;

  case 146:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5342 "ds_parser.cpp"
    break;

  case 147:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5348 "ds_parser.cpp"
    break;

  case 148:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5354 "ds_parser.cpp"
    break;

  case 149:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5360 "ds_parser.cpp"
    break;

  case 150:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5366 "ds_parser.cpp"
    break;

  case 151:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5372 "ds_parser.cpp"
    break;

  case 152:
#line 1032 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5378 "ds_parser.cpp"
    break;

  case 153:
#line 1033 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5384 "ds_parser.cpp"
    break;

  case 154:
#line 1037 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5394 "ds_parser.cpp"
    break;

  case 155:
#line 1042 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5405 "ds_parser.cpp"
    break;

  case 156:
#line 1048 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5413 "ds_parser.cpp"
    break;

  case 157:
#line 1054 "ds_parser.ypp"
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
#line 5439 "ds_parser.cpp"
    break;

  case 158:
#line 1075 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5447 "ds_parser.cpp"
    break;

  case 159:
#line 1078 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5455 "ds_parser.cpp"
    break;

  case 160:
#line 1084 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5461 "ds_parser.cpp"
    break;

  case 161:
#line 1085 "ds_parser.ypp"
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
#line 5477 "ds_parser.cpp"
    break;

  case 162:
#line 1096 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5483 "ds_parser.cpp"
    break;

  case 163:
#line 1100 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5491 "ds_parser.cpp"
    break;

  case 164:
#line 1106 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5499 "ds_parser.cpp"
    break;

  case 165:
#line 1109 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5507 "ds_parser.cpp"
    break;

  case 166:
#line 1112 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5516 "ds_parser.cpp"
    break;

  case 167:
#line 1116 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5524 "ds_parser.cpp"
    break;

  case 168:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5530 "ds_parser.cpp"
    break;

  case 169:
#line 1126 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5536 "ds_parser.cpp"
    break;

  case 170:
#line 1130 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5544 "ds_parser.cpp"
    break;

  case 171:
#line 1133 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5552 "ds_parser.cpp"
    break;

  case 172:
#line 1136 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5562 "ds_parser.cpp"
    break;

  case 173:
#line 1141 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5570 "ds_parser.cpp"
    break;

  case 174:
#line 1144 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5580 "ds_parser.cpp"
    break;

  case 175:
#line 1152 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5588 "ds_parser.cpp"
    break;

  case 176:
#line 1155 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5598 "ds_parser.cpp"
    break;

  case 177:
#line 1160 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5606 "ds_parser.cpp"
    break;

  case 178:
#line 1163 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5616 "ds_parser.cpp"
    break;

  case 179:
#line 1171 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5624 "ds_parser.cpp"
    break;

  case 180:
#line 1177 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5630 "ds_parser.cpp"
    break;

  case 181:
#line 1178 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5636 "ds_parser.cpp"
    break;

  case 182:
#line 1182 "ds_parser.ypp"
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
#line 5672 "ds_parser.cpp"
    break;

  case 183:
#line 1216 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5678 "ds_parser.cpp"
    break;

  case 184:
#line 1216 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5684 "ds_parser.cpp"
    break;

  case 185:
#line 1216 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5692 "ds_parser.cpp"
    break;

  case 186:
#line 1219 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5698 "ds_parser.cpp"
    break;

  case 187:
#line 1219 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5704 "ds_parser.cpp"
    break;

  case 188:
#line 1219 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5714 "ds_parser.cpp"
    break;

  case 189:
#line 1224 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5720 "ds_parser.cpp"
    break;

  case 190:
#line 1224 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5726 "ds_parser.cpp"
    break;

  case 191:
#line 1224 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5736 "ds_parser.cpp"
    break;

  case 192:
#line 1232 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5745 "ds_parser.cpp"
    break;

  case 193:
#line 1236 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5755 "ds_parser.cpp"
    break;

  case 194:
#line 1241 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5766 "ds_parser.cpp"
    break;

  case 195:
#line 1247 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5772 "ds_parser.cpp"
    break;

  case 196:
#line 1247 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5778 "ds_parser.cpp"
    break;

  case 197:
#line 1247 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5787 "ds_parser.cpp"
    break;

  case 198:
#line 1251 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5793 "ds_parser.cpp"
    break;

  case 199:
#line 1251 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5799 "ds_parser.cpp"
    break;

  case 200:
#line 1251 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5809 "ds_parser.cpp"
    break;

  case 201:
#line 1256 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5815 "ds_parser.cpp"
    break;

  case 202:
#line 1256 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5821 "ds_parser.cpp"
    break;

  case 203:
#line 1256 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5832 "ds_parser.cpp"
    break;

  case 204:
#line 1265 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5840 "ds_parser.cpp"
    break;

  case 205:
#line 1268 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5848 "ds_parser.cpp"
    break;

  case 206:
#line 1274 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5854 "ds_parser.cpp"
    break;

  case 207:
#line 1275 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5866 "ds_parser.cpp"
    break;

  case 208:
#line 1282 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5879 "ds_parser.cpp"
    break;

  case 209:
#line 1293 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5885 "ds_parser.cpp"
    break;

  case 210:
#line 1294 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5891 "ds_parser.cpp"
    break;

  case 211:
#line 1295 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5897 "ds_parser.cpp"
    break;

  case 212:
#line 1299 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5903 "ds_parser.cpp"
    break;

  case 213:
#line 1300 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5909 "ds_parser.cpp"
    break;

  case 214:
#line 1301 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5915 "ds_parser.cpp"
    break;

  case 215:
#line 1302 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5921 "ds_parser.cpp"
    break;

  case 216:
#line 1306 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5931 "ds_parser.cpp"
    break;

  case 217:
#line 1311 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5941 "ds_parser.cpp"
    break;

  case 218:
#line 1319 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5947 "ds_parser.cpp"
    break;

  case 219:
#line 1320 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5953 "ds_parser.cpp"
    break;

  case 220:
#line 1324 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5963 "ds_parser.cpp"
    break;

  case 221:
#line 1330 "ds_parser.ypp"
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
#line 6028 "ds_parser.cpp"
    break;

  case 222:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6034 "ds_parser.cpp"
    break;

  case 223:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6040 "ds_parser.cpp"
    break;

  case 224:
#line 1395 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6046 "ds_parser.cpp"
    break;

  case 225:
#line 1396 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6052 "ds_parser.cpp"
    break;

  case 226:
#line 1397 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6058 "ds_parser.cpp"
    break;

  case 227:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6064 "ds_parser.cpp"
    break;

  case 228:
#line 1402 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6070 "ds_parser.cpp"
    break;

  case 229:
#line 1403 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6076 "ds_parser.cpp"
    break;

  case 230:
#line 1404 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6082 "ds_parser.cpp"
    break;

  case 231:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6088 "ds_parser.cpp"
    break;

  case 232:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6094 "ds_parser.cpp"
    break;

  case 233:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6100 "ds_parser.cpp"
    break;

  case 234:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6106 "ds_parser.cpp"
    break;

  case 235:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6112 "ds_parser.cpp"
    break;

  case 236:
#line 1410 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6118 "ds_parser.cpp"
    break;

  case 237:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6124 "ds_parser.cpp"
    break;

  case 238:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6130 "ds_parser.cpp"
    break;

  case 239:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6136 "ds_parser.cpp"
    break;

  case 240:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6142 "ds_parser.cpp"
    break;

  case 241:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6148 "ds_parser.cpp"
    break;

  case 242:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6154 "ds_parser.cpp"
    break;

  case 243:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6160 "ds_parser.cpp"
    break;

  case 244:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6166 "ds_parser.cpp"
    break;

  case 245:
#line 1419 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6172 "ds_parser.cpp"
    break;

  case 246:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6178 "ds_parser.cpp"
    break;

  case 247:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6184 "ds_parser.cpp"
    break;

  case 248:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6190 "ds_parser.cpp"
    break;

  case 249:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6196 "ds_parser.cpp"
    break;

  case 250:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6202 "ds_parser.cpp"
    break;

  case 251:
#line 1431 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6214 "ds_parser.cpp"
    break;

  case 252:
#line 1442 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6224 "ds_parser.cpp"
    break;

  case 253:
#line 1447 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6236 "ds_parser.cpp"
    break;

  case 254:
#line 1457 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6245 "ds_parser.cpp"
    break;

  case 255:
#line 1461 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6251 "ds_parser.cpp"
    break;

  case 256:
#line 1461 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6257 "ds_parser.cpp"
    break;

  case 257:
#line 1461 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6269 "ds_parser.cpp"
    break;

  case 258:
#line 1468 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6275 "ds_parser.cpp"
    break;

  case 259:
#line 1468 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6281 "ds_parser.cpp"
    break;

  case 260:
#line 1468 "ds_parser.ypp"
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
#line 6297 "ds_parser.cpp"
    break;

  case 261:
#line 1482 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6306 "ds_parser.cpp"
    break;

  case 262:
#line 1486 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6312 "ds_parser.cpp"
    break;

  case 263:
#line 1486 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6318 "ds_parser.cpp"
    break;

  case 264:
#line 1486 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6327 "ds_parser.cpp"
    break;

  case 265:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6333 "ds_parser.cpp"
    break;

  case 266:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6339 "ds_parser.cpp"
    break;

  case 267:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6345 "ds_parser.cpp"
    break;

  case 268:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6351 "ds_parser.cpp"
    break;

  case 269:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6357 "ds_parser.cpp"
    break;

  case 270:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6363 "ds_parser.cpp"
    break;

  case 271:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6369 "ds_parser.cpp"
    break;

  case 272:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6375 "ds_parser.cpp"
    break;

  case 273:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6381 "ds_parser.cpp"
    break;

  case 274:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6387 "ds_parser.cpp"
    break;

  case 275:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6393 "ds_parser.cpp"
    break;

  case 276:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6399 "ds_parser.cpp"
    break;

  case 277:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6405 "ds_parser.cpp"
    break;

  case 278:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6411 "ds_parser.cpp"
    break;

  case 279:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6417 "ds_parser.cpp"
    break;

  case 280:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6423 "ds_parser.cpp"
    break;

  case 281:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6429 "ds_parser.cpp"
    break;

  case 282:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6435 "ds_parser.cpp"
    break;

  case 283:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6441 "ds_parser.cpp"
    break;

  case 284:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6447 "ds_parser.cpp"
    break;

  case 285:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 286:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 287:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 288:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 289:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 290:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 291:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 292:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 293:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 294:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 295:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 296:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 297:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 298:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 299:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 300:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 301:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 302:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 303:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6561 "ds_parser.cpp"
    break;

  case 304:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 305:
#line 1533 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6573 "ds_parser.cpp"
    break;

  case 306:
#line 1534 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6579 "ds_parser.cpp"
    break;

  case 307:
#line 1535 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6585 "ds_parser.cpp"
    break;

  case 308:
#line 1536 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6594 "ds_parser.cpp"
    break;

  case 309:
#line 1540 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6603 "ds_parser.cpp"
    break;

  case 310:
#line 1544 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6611 "ds_parser.cpp"
    break;

  case 311:
#line 1547 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6619 "ds_parser.cpp"
    break;

  case 312:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6625 "ds_parser.cpp"
    break;

  case 313:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6631 "ds_parser.cpp"
    break;

  case 314:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6637 "ds_parser.cpp"
    break;

  case 315:
#line 1553 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6651 "ds_parser.cpp"
    break;

  case 316:
#line 1562 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6665 "ds_parser.cpp"
    break;

  case 317:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6671 "ds_parser.cpp"
    break;

  case 318:
#line 1572 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6679 "ds_parser.cpp"
    break;

  case 319:
#line 1575 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6685 "ds_parser.cpp"
    break;

  case 320:
#line 1575 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6691 "ds_parser.cpp"
    break;

  case 321:
#line 1575 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6699 "ds_parser.cpp"
    break;

  case 322:
#line 1578 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6708 "ds_parser.cpp"
    break;

  case 323:
#line 1582 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6717 "ds_parser.cpp"
    break;

  case 324:
#line 1586 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6726 "ds_parser.cpp"
    break;

  case 325:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6732 "ds_parser.cpp"
    break;

  case 326:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6738 "ds_parser.cpp"
    break;

  case 327:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6744 "ds_parser.cpp"
    break;

  case 328:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6750 "ds_parser.cpp"
    break;

  case 329:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6756 "ds_parser.cpp"
    break;

  case 330:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6762 "ds_parser.cpp"
    break;

  case 331:
#line 1596 "ds_parser.ypp"
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
#line 6777 "ds_parser.cpp"
    break;

  case 332:
#line 1606 "ds_parser.ypp"
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
#line 6792 "ds_parser.cpp"
    break;

  case 333:
#line 1616 "ds_parser.ypp"
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
#line 6849 "ds_parser.cpp"
    break;

  case 334:
#line 1671 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6855 "ds_parser.cpp"
    break;

  case 335:
#line 1672 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6861 "ds_parser.cpp"
    break;

  case 336:
#line 1676 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6867 "ds_parser.cpp"
    break;

  case 337:
#line 1677 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6873 "ds_parser.cpp"
    break;

  case 338:
#line 1681 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6883 "ds_parser.cpp"
    break;

  case 339:
#line 1689 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6891 "ds_parser.cpp"
    break;

  case 340:
#line 1692 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6900 "ds_parser.cpp"
    break;

  case 341:
#line 1697 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6906 "ds_parser.cpp"
    break;

  case 342:
#line 1697 "ds_parser.ypp"
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
#line 6955 "ds_parser.cpp"
    break;

  case 343:
#line 1741 "ds_parser.ypp"
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
            assignDefaultArguments((yyvsp[0].pFuncDecl));
            runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt((yylsp[-3])));
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[0].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7016 "ds_parser.cpp"
    break;

  case 344:
#line 1797 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7027 "ds_parser.cpp"
    break;

  case 345:
#line 1806 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7037 "ds_parser.cpp"
    break;

  case 346:
#line 1811 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7047 "ds_parser.cpp"
    break;

  case 347:
#line 1819 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7053 "ds_parser.cpp"
    break;

  case 348:
#line 1820 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7059 "ds_parser.cpp"
    break;

  case 349:
#line 1824 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7067 "ds_parser.cpp"
    break;

  case 350:
#line 1827 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7078 "ds_parser.cpp"
    break;

  case 351:
#line 1836 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7084 "ds_parser.cpp"
    break;

  case 352:
#line 1837 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7090 "ds_parser.cpp"
    break;

  case 353:
#line 1841 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7101 "ds_parser.cpp"
    break;

  case 354:
#line 1850 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7107 "ds_parser.cpp"
    break;

  case 355:
#line 1851 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7113 "ds_parser.cpp"
    break;

  case 356:
#line 1856 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7119 "ds_parser.cpp"
    break;

  case 357:
#line 1857 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7125 "ds_parser.cpp"
    break;

  case 358:
#line 1861 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7136 "ds_parser.cpp"
    break;

  case 359:
#line 1867 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7147 "ds_parser.cpp"
    break;

  case 360:
#line 1873 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7155 "ds_parser.cpp"
    break;

  case 361:
#line 1876 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7164 "ds_parser.cpp"
    break;

  case 362:
#line 1880 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7175 "ds_parser.cpp"
    break;

  case 363:
#line 1886 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7186 "ds_parser.cpp"
    break;

  case 364:
#line 1895 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7192 "ds_parser.cpp"
    break;

  case 365:
#line 1896 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7198 "ds_parser.cpp"
    break;

  case 366:
#line 1897 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7204 "ds_parser.cpp"
    break;

  case 367:
#line 1901 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7210 "ds_parser.cpp"
    break;

  case 368:
#line 1902 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7216 "ds_parser.cpp"
    break;

  case 369:
#line 1906 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7224 "ds_parser.cpp"
    break;

  case 370:
#line 1909 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7234 "ds_parser.cpp"
    break;

  case 371:
#line 1914 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7244 "ds_parser.cpp"
    break;

  case 372:
#line 1919 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7257 "ds_parser.cpp"
    break;

  case 373:
#line 1927 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7270 "ds_parser.cpp"
    break;

  case 374:
#line 1938 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7278 "ds_parser.cpp"
    break;

  case 375:
#line 1941 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7287 "ds_parser.cpp"
    break;

  case 376:
#line 1948 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7293 "ds_parser.cpp"
    break;

  case 377:
#line 1949 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7299 "ds_parser.cpp"
    break;

  case 378:
#line 1953 "ds_parser.ypp"
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
#line 7331 "ds_parser.cpp"
    break;

  case 379:
#line 1980 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7340 "ds_parser.cpp"
    break;

  case 380:
#line 1983 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7351 "ds_parser.cpp"
    break;

  case 381:
#line 1992 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7359 "ds_parser.cpp"
    break;

  case 382:
#line 1995 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7373 "ds_parser.cpp"
    break;

  case 383:
#line 2004 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7387 "ds_parser.cpp"
    break;

  case 384:
#line 2017 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7401 "ds_parser.cpp"
    break;

  case 388:
#line 2038 "ds_parser.ypp"
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
#line 7417 "ds_parser.cpp"
    break;

  case 389:
#line 2049 "ds_parser.ypp"
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
#line 7434 "ds_parser.cpp"
    break;

  case 390:
#line 2064 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7440 "ds_parser.cpp"
    break;

  case 391:
#line 2065 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7446 "ds_parser.cpp"
    break;

  case 392:
#line 2069 "ds_parser.ypp"
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
#line 7485 "ds_parser.cpp"
    break;

  case 393:
#line 2106 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7491 "ds_parser.cpp"
    break;

  case 394:
#line 2107 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7497 "ds_parser.cpp"
    break;

  case 395:
#line 2111 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7503 "ds_parser.cpp"
    break;

  case 396:
#line 2111 "ds_parser.ypp"
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
#line 7589 "ds_parser.cpp"
    break;

  case 397:
#line 2195 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7601 "ds_parser.cpp"
    break;

  case 398:
#line 2202 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7612 "ds_parser.cpp"
    break;

  case 399:
#line 2211 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7618 "ds_parser.cpp"
    break;

  case 400:
#line 2212 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7624 "ds_parser.cpp"
    break;

  case 401:
#line 2213 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7630 "ds_parser.cpp"
    break;

  case 402:
#line 2214 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7636 "ds_parser.cpp"
    break;

  case 403:
#line 2215 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7642 "ds_parser.cpp"
    break;

  case 404:
#line 2216 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7648 "ds_parser.cpp"
    break;

  case 405:
#line 2217 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7654 "ds_parser.cpp"
    break;

  case 406:
#line 2218 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7660 "ds_parser.cpp"
    break;

  case 407:
#line 2219 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7666 "ds_parser.cpp"
    break;

  case 408:
#line 2220 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7672 "ds_parser.cpp"
    break;

  case 409:
#line 2221 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7678 "ds_parser.cpp"
    break;

  case 410:
#line 2222 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7684 "ds_parser.cpp"
    break;

  case 411:
#line 2223 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7690 "ds_parser.cpp"
    break;

  case 412:
#line 2224 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7696 "ds_parser.cpp"
    break;

  case 413:
#line 2225 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7702 "ds_parser.cpp"
    break;

  case 414:
#line 2226 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7708 "ds_parser.cpp"
    break;

  case 415:
#line 2227 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7714 "ds_parser.cpp"
    break;

  case 416:
#line 2228 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7720 "ds_parser.cpp"
    break;

  case 417:
#line 2229 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7726 "ds_parser.cpp"
    break;

  case 418:
#line 2230 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7732 "ds_parser.cpp"
    break;

  case 419:
#line 2231 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7738 "ds_parser.cpp"
    break;

  case 420:
#line 2232 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7744 "ds_parser.cpp"
    break;

  case 421:
#line 2233 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7750 "ds_parser.cpp"
    break;

  case 422:
#line 2234 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7756 "ds_parser.cpp"
    break;

  case 423:
#line 2235 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7762 "ds_parser.cpp"
    break;

  case 424:
#line 2239 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7768 "ds_parser.cpp"
    break;

  case 425:
#line 2240 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7774 "ds_parser.cpp"
    break;

  case 426:
#line 2241 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7780 "ds_parser.cpp"
    break;

  case 427:
#line 2242 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7786 "ds_parser.cpp"
    break;

  case 428:
#line 2243 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7792 "ds_parser.cpp"
    break;

  case 429:
#line 2244 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7798 "ds_parser.cpp"
    break;

  case 430:
#line 2248 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7811 "ds_parser.cpp"
    break;

  case 431:
#line 2259 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7820 "ds_parser.cpp"
    break;

  case 432:
#line 2263 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7832 "ds_parser.cpp"
    break;

  case 433:
#line 2273 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7844 "ds_parser.cpp"
    break;

  case 434:
#line 2280 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7855 "ds_parser.cpp"
    break;

  case 435:
#line 2289 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7861 "ds_parser.cpp"
    break;

  case 436:
#line 2289 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7867 "ds_parser.cpp"
    break;

  case 437:
#line 2289 "ds_parser.ypp"
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
#line 7882 "ds_parser.cpp"
    break;

  case 438:
#line 2302 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7888 "ds_parser.cpp"
    break;

  case 439:
#line 2303 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7894 "ds_parser.cpp"
    break;

  case 440:
#line 2304 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7900 "ds_parser.cpp"
    break;

  case 441:
#line 2305 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7906 "ds_parser.cpp"
    break;

  case 442:
#line 2306 "ds_parser.ypp"
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
#line 7925 "ds_parser.cpp"
    break;

  case 443:
#line 2320 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7936 "ds_parser.cpp"
    break;

  case 444:
#line 2326 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7945 "ds_parser.cpp"
    break;

  case 445:
#line 2330 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7954 "ds_parser.cpp"
    break;

  case 446:
#line 2334 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7964 "ds_parser.cpp"
    break;

  case 447:
#line 2339 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7974 "ds_parser.cpp"
    break;

  case 448:
#line 2344 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7984 "ds_parser.cpp"
    break;

  case 449:
#line 2349 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7994 "ds_parser.cpp"
    break;

  case 450:
#line 2354 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8003 "ds_parser.cpp"
    break;

  case 451:
#line 2358 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8012 "ds_parser.cpp"
    break;

  case 452:
#line 2362 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8022 "ds_parser.cpp"
    break;

  case 453:
#line 2367 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8031 "ds_parser.cpp"
    break;

  case 454:
#line 2371 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8041 "ds_parser.cpp"
    break;

  case 455:
#line 2376 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8047 "ds_parser.cpp"
    break;

  case 456:
#line 2376 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8053 "ds_parser.cpp"
    break;

  case 457:
#line 2376 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8064 "ds_parser.cpp"
    break;

  case 458:
#line 2382 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8076 "ds_parser.cpp"
    break;

  case 459:
#line 2389 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8082 "ds_parser.cpp"
    break;

  case 460:
#line 2389 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8088 "ds_parser.cpp"
    break;

  case 461:
#line 2389 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8098 "ds_parser.cpp"
    break;

  case 462:
#line 2394 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8104 "ds_parser.cpp"
    break;

  case 463:
#line 2394 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8110 "ds_parser.cpp"
    break;

  case 464:
#line 2394 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8121 "ds_parser.cpp"
    break;

  case 465:
#line 2400 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8127 "ds_parser.cpp"
    break;

  case 466:
#line 2400 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8133 "ds_parser.cpp"
    break;

  case 467:
#line 2400 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8143 "ds_parser.cpp"
    break;

  case 468:
#line 2405 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8152 "ds_parser.cpp"
    break;

  case 469:
#line 2409 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8158 "ds_parser.cpp"
    break;

  case 470:
#line 2409 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8164 "ds_parser.cpp"
    break;

  case 471:
#line 2409 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8174 "ds_parser.cpp"
    break;

  case 472:
#line 2414 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8180 "ds_parser.cpp"
    break;

  case 473:
#line 2414 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8186 "ds_parser.cpp"
    break;

  case 474:
#line 2414 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8200 "ds_parser.cpp"
    break;

  case 475:
#line 2423 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8209 "ds_parser.cpp"
    break;

  case 476:
#line 2427 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8215 "ds_parser.cpp"
    break;

  case 477:
#line 2427 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8221 "ds_parser.cpp"
    break;

  case 478:
#line 2427 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8231 "ds_parser.cpp"
    break;

  case 479:
#line 2432 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8237 "ds_parser.cpp"
    break;

  case 480:
#line 2432 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8243 "ds_parser.cpp"
    break;

  case 481:
#line 2432 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8257 "ds_parser.cpp"
    break;

  case 482:
#line 2441 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8266 "ds_parser.cpp"
    break;

  case 483:
#line 2445 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8272 "ds_parser.cpp"
    break;

  case 484:
#line 2445 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8278 "ds_parser.cpp"
    break;

  case 485:
#line 2445 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8288 "ds_parser.cpp"
    break;

  case 486:
#line 2450 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8294 "ds_parser.cpp"
    break;

  case 487:
#line 2450 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8300 "ds_parser.cpp"
    break;

  case 488:
#line 2450 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8314 "ds_parser.cpp"
    break;

  case 489:
#line 2459 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8320 "ds_parser.cpp"
    break;

  case 490:
#line 2459 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8326 "ds_parser.cpp"
    break;

  case 491:
#line 2459 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8337 "ds_parser.cpp"
    break;

  case 492:
#line 2465 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8343 "ds_parser.cpp"
    break;

  case 493:
#line 2465 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8349 "ds_parser.cpp"
    break;

  case 494:
#line 2465 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8360 "ds_parser.cpp"
    break;

  case 495:
#line 2474 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8366 "ds_parser.cpp"
    break;

  case 496:
#line 2474 "ds_parser.ypp"
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
#line 8383 "ds_parser.cpp"
    break;

  case 497:
#line 2489 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8389 "ds_parser.cpp"
    break;

  case 498:
#line 2489 "ds_parser.ypp"
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
#line 8410 "ds_parser.cpp"
    break;

  case 499:
#line 2509 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8416 "ds_parser.cpp"
    break;

  case 500:
#line 2510 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8422 "ds_parser.cpp"
    break;

  case 501:
#line 2511 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8428 "ds_parser.cpp"
    break;

  case 502:
#line 2512 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8434 "ds_parser.cpp"
    break;

  case 503:
#line 2516 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8446 "ds_parser.cpp"
    break;

  case 504:
#line 2523 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8458 "ds_parser.cpp"
    break;

  case 505:
#line 2530 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8469 "ds_parser.cpp"
    break;

  case 506:
#line 2536 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8480 "ds_parser.cpp"
    break;

  case 507:
#line 2545 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8490 "ds_parser.cpp"
    break;

  case 508:
#line 2550 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8499 "ds_parser.cpp"
    break;

  case 509:
#line 2557 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8505 "ds_parser.cpp"
    break;

  case 510:
#line 2558 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8511 "ds_parser.cpp"
    break;

  case 511:
#line 2562 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8522 "ds_parser.cpp"
    break;

  case 512:
#line 2568 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8534 "ds_parser.cpp"
    break;

  case 513:
#line 2575 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8547 "ds_parser.cpp"
    break;

  case 514:
#line 2583 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8559 "ds_parser.cpp"
    break;

  case 515:
#line 2590 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8572 "ds_parser.cpp"
    break;

  case 516:
#line 2598 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8586 "ds_parser.cpp"
    break;

  case 517:
#line 2610 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8594 "ds_parser.cpp"
    break;

  case 518:
#line 2613 "ds_parser.ypp"
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
#line 8610 "ds_parser.cpp"
    break;

  case 519:
#line 2627 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8621 "ds_parser.cpp"
    break;

  case 520:
#line 2636 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8627 "ds_parser.cpp"
    break;

  case 521:
#line 2637 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8633 "ds_parser.cpp"
    break;

  case 522:
#line 2641 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8643 "ds_parser.cpp"
    break;

  case 523:
#line 2646 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8652 "ds_parser.cpp"
    break;

  case 524:
#line 2653 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8662 "ds_parser.cpp"
    break;

  case 525:
#line 2658 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8675 "ds_parser.cpp"
    break;

  case 526:
#line 2669 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8685 "ds_parser.cpp"
    break;

  case 527:
#line 2674 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8694 "ds_parser.cpp"
    break;

  case 528:
#line 2681 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8708 "ds_parser.cpp"
    break;

  case 529:
#line 2693 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8714 "ds_parser.cpp"
    break;

  case 530:
#line 2694 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8720 "ds_parser.cpp"
    break;

  case 531:
#line 2698 "ds_parser.ypp"
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
#line 8743 "ds_parser.cpp"
    break;

  case 532:
#line 2716 "ds_parser.ypp"
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
#line 8766 "ds_parser.cpp"
    break;


#line 8770 "ds_parser.cpp"

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
#line 2736 "ds_parser.ypp"


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
