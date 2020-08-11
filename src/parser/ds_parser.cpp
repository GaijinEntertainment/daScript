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
#define YYLAST   7692

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  185
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  531
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  946

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
     450,   451,   452,   453,   454,   458,   470,   471,   475,   479,
     484,   493,   501,   517,   522,   530,   530,   560,   582,   586,
     589,   595,   604,   625,   648,   649,   653,   657,   658,   662,
     665,   671,   677,   680,   686,   687,   691,   692,   693,   702,
     703,   707,   716,   732,   740,   750,   759,   760,   761,   762,
     763,   764,   768,   773,   781,   782,   783,   784,   785,   786,
     787,   788,   794,   807,   823,   824,   828,   835,   847,   851,
     858,   859,   863,   864,   865,   869,   874,   881,   885,   886,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     930,   951,   989,   996,  1000,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1033,  1038,  1044,  1050,  1071,  1074,
    1080,  1081,  1092,  1096,  1102,  1105,  1108,  1112,  1118,  1122,
    1126,  1129,  1132,  1137,  1140,  1148,  1151,  1156,  1159,  1167,
    1173,  1174,  1178,  1212,  1212,  1212,  1215,  1215,  1215,  1220,
    1220,  1220,  1228,  1232,  1237,  1243,  1243,  1243,  1247,  1247,
    1247,  1252,  1252,  1252,  1261,  1264,  1270,  1271,  1278,  1289,
    1290,  1291,  1295,  1296,  1297,  1298,  1302,  1307,  1315,  1316,
    1320,  1325,  1389,  1390,  1391,  1392,  1393,  1394,  1398,  1399,
    1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,
    1410,  1411,  1412,  1413,  1414,  1415,  1416,  1420,  1421,  1422,
    1423,  1427,  1438,  1443,  1453,  1457,  1457,  1457,  1464,  1464,
    1464,  1478,  1482,  1482,  1482,  1490,  1491,  1492,  1493,  1494,
    1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,  1504,
    1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,
    1515,  1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,
    1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1537,
    1541,  1544,  1547,  1548,  1549,  1550,  1559,  1568,  1569,  1572,
    1572,  1572,  1575,  1579,  1583,  1587,  1588,  1589,  1590,  1591,
    1592,  1593,  1603,  1613,  1668,  1669,  1673,  1674,  1678,  1686,
    1689,  1694,  1693,  1738,  1796,  1801,  1809,  1810,  1814,  1817,
    1826,  1827,  1831,  1840,  1841,  1846,  1847,  1851,  1857,  1863,
    1866,  1870,  1876,  1885,  1886,  1887,  1891,  1892,  1896,  1899,
    1904,  1909,  1917,  1928,  1931,  1938,  1939,  1943,  1970,  1970,
    1982,  1985,  1994,  2007,  2019,  2020,  2024,  2028,  2039,  2054,
    2055,  2059,  2096,  2097,  2101,  2101,  2185,  2192,  2201,  2202,
    2203,  2204,  2205,  2206,  2207,  2208,  2209,  2210,  2211,  2212,
    2213,  2214,  2215,  2216,  2217,  2218,  2219,  2220,  2221,  2222,
    2223,  2224,  2225,  2229,  2230,  2231,  2232,  2233,  2234,  2238,
    2249,  2253,  2263,  2270,  2279,  2279,  2279,  2292,  2293,  2294,
    2295,  2296,  2310,  2316,  2320,  2324,  2329,  2334,  2339,  2344,
    2348,  2352,  2357,  2361,  2366,  2366,  2366,  2372,  2379,  2379,
    2379,  2384,  2384,  2384,  2390,  2390,  2390,  2395,  2399,  2399,
    2399,  2404,  2404,  2404,  2413,  2417,  2417,  2417,  2422,  2422,
    2422,  2431,  2435,  2435,  2435,  2440,  2440,  2440,  2449,  2449,
    2449,  2455,  2455,  2455,  2464,  2464,  2479,  2479,  2499,  2500,
    2501,  2502,  2506,  2513,  2520,  2526,  2535,  2540,  2547,  2548,
    2552,  2558,  2565,  2573,  2580,  2588,  2600,  2603,  2617,  2626,
    2627,  2631,  2636,  2643,  2648,  2659,  2664,  2671,  2683,  2684,
    2688,  2706
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

#define YYTABLE_NINF -486

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -595,    21,  -595,  -595,    12,   -38,    64,    74,   -55,  -595,
      87,  -595,  -595,    -2,  -595,  -595,  -595,  -595,  -595,   317,
    -595,    97,  -595,  -595,  -595,  -595,  -595,  -595,    49,  -595,
      54,   104,   106,  -595,  -595,    17,  -595,    31,   107,  -595,
     123,   132,  -595,   139,    13,   118,  -595,   -30,  -595,  -595,
    -595,     0,   169,  -595,   145,    27,    12,   193,   -38,   188,
    -595,   207,   209,  -595,   254,  -595,   204,  -595,   -89,   175,
     180,  -595,   220,    12,    -2,  -595,  7524,   331,   337,  -595,
     199,   195,  -595,   225,  -595,  -595,   380,  -595,  -595,  -595,
    -595,  -595,   237,   100,  -595,  -595,  -595,  -595,   332,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,   205,   -70,  7439,
    -595,  -595,   242,   244,  -595,   -29,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,   -90,
     235,  -595,  -595,    -5,  -595,   210,   -65,   212,  -595,   -81,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,    10,  -595,  -595,
     -17,  -595,   236,   240,   241,   243,  -595,  -595,  -595,   214,
    -595,  -595,  -595,  -595,  -595,   245,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,   246,  -595,  -595,  -595,   249,
     251,  -595,  -595,  -595,  -595,   253,   257,  -595,  -595,  -595,
    -595,  -595,   302,  -595,   219,   258,  -595,   239,  -595,    12,
    -595,   -11,  -595,    93,  7439,  -595,  1331,  -595,  -595,  -595,
    -595,  -595,   115,  -595,  -595,  -595,   100,  -595,   -63,  4886,
    -595,  -595,  -595,  -595,  -595,   278,  -595,   101,   109,   111,
    -595,  -595,  -595,  -595,  -595,  -595,   395,  -595,  -595,    57,
    3134,  -595,  -595,   -51,  7439,   -25,    -6,   284,  -595,   -67,
    -595,   296,  4103,  -595,  4886,  -595,  -595,   284,  -595,  -595,
    7355,   255,  2137,  -595,  -595,   195,  4886,   256,  4886,   276,
     277,   259,  -595,   262,   279,   300,  3271,   195,  -595,   282,
    2294,  4886,  4886,   181,   181,  7187,  7271,  4886,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  4886,  4886,  4886,    -5,  4886,
    4886,  4886,  -595,   264,  -595,   409,  -595,  -595,   265,   266,
    4886,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,   -68,
     267,  -595,   268,   269,  -595,  -595,  -595,   284,  -595,  -595,
    -595,   281,  -595,  -595,   -82,  -595,  -595,  -595,  -595,  -595,
    6403,   274,  -595,  -595,  -595,  -595,  -595,    16,   308,  7439,
    -595,   -44,  -595,  -595,  5367,  7439,  7439,  7439,  7439,   280,
     242,  7439,   199,  7439,   199,  7439,   199,  7523,   244,  -595,
    -595,  -595,   285,  -595,  -595,  6060,  -595,  -595,  4103,  -595,
    -595,   287,  -595,  -595,  -595,  7439,  -595,  2431,  -595,   398,
      -3,   569,  -595,    -5,  2431,  -595,  -595,   339,   968,   447,
    6723,  4886,   398,  -595,  -595,  4886,  -595,  -595,   310,   327,
    6723,  -595,  7439,  2431,  -595,  1133,    77,    77,   286,  -595,
    -595,   284,  1672,   284,  1827,  6501,  -595,   137,   222,    77,
      77,   -58,  -595,    77,    77,  5515,   -33,   288,  -595,  -595,
     398,  -595,  2568,  -595,  -595,  -595,  -595,   333,   181,  -595,
      19,   323,  4886,  4886,  4886,  4886,  4886,  4886,  4886,  4886,
    4886,  4886,  -595,  -595,  4886,  4886,  4886,  4886,  4886,  4886,
     325,  2723,  4886,   328,  4886,  4886,  4886,  4886,  4886,  4886,
    4886,  4886,  4886,  4886,  4886,  4886,  4886,  4886,  2860,  3408,
    4886,  4886,  4886,  4886,  4886,  4886,  4886,  4886,  4886,  4886,
     334,  4886,  3545,  -595,   464,   433,   298,  -595,  2079,  -595,
    -595,  -595,  2431,  -595,   583,  1273,   998,  1613,  -595,    36,
    1920,   235,  2235,   235,  2369,   235,   -79,  -595,   103,  4103,
     116,  -595,  -595,  -595,  2665,  -595,  6304,  -595,  4886,  3700,
    2997,  -595,  4975,  4886,  4886,  -595,   195,  5592,  -595,  7439,
    7439,  5671,  7439,  -595,  -595,  2506,  -595,  5073,  -595,  -595,
       1,   181,   -60,  1982,  3855,  6501,   329,   -21,   305,   336,
    -595,  -595,  -117,    11,  4010,   -21,   163,  4886,  4886,   306,
    -595,  4886,   342,   344,  -595,   158,  -595,  -595,   303,   341,
    -595,    55,  6723,   -50,   199,  -595,   340,  -595,  -595,  6723,
    6723,  6723,  6723,  6723,  6723,  6723,  6723,    62,    62,   421,
    6723,  6723,   421,  7067,  7067,   314,  4886,  4886,  6723,   309,
    -595,  6139,   -72,   -72,  6723,    62,    62,  6723,  6723,  6945,
    6797,  6871,  6723,  6723,  6723,  4886,  4886,  6723,   350,  6575,
     543,  6969,  7043,   421,   421,   233,   233,   309,   309,   309,
    -595,   496,  6217,  -595,    61,    94,  -595,   284,  -595,   395,
    -595,  2431,  -595,  5171,  -595,  7439,  -595,  -595,  -595,   355,
    -595,   346,  -595,   347,  -595,   352,  7439,  -595,  7523,  -595,
     244,   395,  4886,   -27,  -595,    65,   358,   373,  5748,  -595,
    -595,  -595,  2798,  2935,  -595,  3072,   333,  -595,  4886,  -595,
    4886,  4886,   -10,   376,   341,   354,   356,  4886,   363,  4886,
    4886,   341,   348,   364,  6723,  -595,  -595,  6649,  -595,   388,
    7439,   199,  1514,   195,  -595,  -595,  4886,  -595,   -42,    35,
    4886,  -595,   397,   402,   403,   404,  -595,    83,   235,  -595,
    4165,   690,  6723,  -595,   690,  6723,  -595,  4886,  -595,  -595,
    -595,  -595,     0,  -595,  -595,  5269,  -595,  -595,  3209,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  4103,  -595,
    -595,  -595,  6723,   195,  -595,  -595,   152,  -595,  -595,  -595,
    -595,   372,   -22,  6723,  6723,   -21,   374,   -36,   329,   389,
    -595,  6723,  -595,  -595,    -8,   -21,   392,  -595,  -595,  -595,
    3346,   235,  -595,  -595,   398,   394,  6723,  -595,  -595,  -595,
    -595,   -50,   399,   -76,  7439,  -595,    69,  6723,  -595,     0,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  7439,  4320,   408,
    4886,  4886,  4886,  4457,  4886,   400,   401,  4886,  4886,  -595,
    4886,   387,  -595,  -595,   415,   303,  -595,  -595,  -595,  4612,
    -595,  -595,  3483,  -595,   405,  -595,  3638,   420,  5827,   428,
     421,   421,   421,  -595,  5904,  5441,   412,  -595,  6723,  6723,
    5441,   414,    -5,  -595,  -595,  4886,  6723,  -595,  -595,  -595,
    -595,  -595,  4749,  -595,   530,   416,  -595,   410,  -595,  -595,
      -5,  6723,  -595,   413,  7439,   437,  5983,  4886,   422,   423,
    -595,  -595,  3793,  -595,  -595,  6723,  -595,  -595,  -595,  7439,
     419,  3948,  -595,  -595,   425,  -595
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   180,     0,     0,     0,     0,     0,   181,
       0,   496,   494,     0,    14,     3,    10,     9,     8,     0,
       7,   375,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    75,     0,   160,    76,    78,     0,    74,   393,
     392,     0,     0,   376,   378,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   380,     0,   384,     0,     0,
       0,   162,     0,     0,     0,    81,     0,     0,     0,    87,
      82,     0,   130,   389,   394,   373,     0,    68,    69,    66,
      67,    65,     0,     0,    64,    73,    40,    38,    34,    31,
      30,   423,   426,   424,   427,   425,   428,     0,     0,     0,
     386,   385,     0,     0,   161,     0,    79,    90,    91,    93,
      92,    94,    95,    96,    97,   122,   123,   120,   121,   113,
     124,   125,   114,   111,   112,   126,   127,   128,   129,   101,
     102,   103,    98,    99,   100,   116,   117,   115,   109,   110,
     105,   104,   106,   107,   108,    89,    88,   118,   119,   334,
      85,   154,   132,     0,   391,     0,     0,     0,    16,     0,
      60,    61,    58,    59,    57,    56,    62,     0,    33,   380,
       0,   387,     0,     0,     0,     0,   398,   418,   399,   430,
     400,   404,   405,   406,   407,   422,   411,   412,   413,   414,
     415,   416,   417,   419,   420,   467,   403,   410,   421,   474,
     481,   401,   408,   402,   409,     0,     0,   429,   437,   440,
     438,   439,     0,   432,     0,     0,   353,     0,    77,     0,
      83,     0,   346,     0,     0,   131,     0,   390,   339,   396,
     377,   374,   366,   379,    17,    18,     0,    71,     0,     0,
     381,   458,   461,   464,   454,     0,   434,   468,   475,   482,
     488,   491,   445,   450,   444,   457,     0,   453,   447,     0,
       0,   383,   449,     0,     0,     0,     0,     0,   344,   357,
      84,   334,    86,   156,     0,    49,    50,     0,   271,   272,
       0,     0,     0,   265,   168,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,     0,     0,     0,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,   224,
     223,   225,   226,   227,    19,     0,     0,     0,     0,     0,
       0,     0,   209,   210,   135,   133,   269,   268,     0,     0,
       0,   149,   144,   142,   141,   143,   220,   155,   136,   266,
       0,   327,     0,     0,   147,   148,   150,     0,   140,   326,
     325,    80,   330,   267,     0,   137,   329,   328,   307,   273,
     228,     0,   270,   498,   499,   500,   501,   334,     0,     0,
     367,     0,    63,   388,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,    82,     0,    82,     0,     0,   452,
     446,   448,     0,   451,   442,     0,   433,   497,   352,   495,
     354,     0,   345,   356,   355,     0,   358,     0,   347,     0,
       0,   164,   167,     0,     0,   170,   173,     0,   228,     0,
     163,     0,     0,   183,   186,     0,   153,   189,     0,     0,
      43,    53,     0,     0,   177,   228,   299,   300,   210,   158,
     159,     0,   508,     0,     0,     0,   525,     0,     0,   277,
     276,   312,    25,   275,   274,     0,   211,     0,   151,   152,
       0,   333,     0,   139,   145,   146,   182,   218,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,   395,     0,   336,     0,   397,     0,   364,
     365,   363,     0,   382,     0,     0,     0,     0,   431,     0,
       0,    85,     0,    85,     0,    85,   160,   350,     0,   348,
       0,   443,   441,   335,   359,   362,   361,    54,     0,     0,
       0,   174,   228,     0,     0,   171,     0,     0,    55,     0,
       0,     0,     0,    41,    42,     0,   178,   228,   175,   211,
       0,     0,   160,     0,     0,   516,   506,   508,     0,   519,
     520,   521,     0,     0,     0,   508,     0,     0,     0,     0,
      22,     0,    20,     0,   303,   255,   254,   154,    46,     0,
     308,     0,   204,     0,    82,   157,     0,   322,   323,   238,
     239,   241,   240,   242,   232,   233,   234,   278,   279,   291,
     243,   244,   292,   289,   290,     0,     0,     0,   230,   317,
     306,     0,   331,   332,   231,   280,   281,   245,   246,   296,
     297,   298,   235,   236,   237,     0,     0,   229,     0,     0,
     294,   295,   293,   287,   288,   283,   282,   284,   285,   286,
     261,     0,     0,   310,     0,   336,   337,     0,   340,   363,
     368,     0,   372,   228,   459,     0,   465,   455,   435,     0,
     469,     0,   476,     0,   483,     0,     0,   489,     0,   492,
       0,   355,     0,     0,   165,     0,     0,     0,     0,   172,
     179,   313,     0,     0,   314,     0,   218,   176,     0,   509,
       0,     0,   508,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   526,   527,     0,    23,    26,
       0,    82,     0,     0,    44,    45,     0,    51,     0,     0,
       0,   309,     0,     0,     0,     0,   216,     0,    85,   319,
       0,   249,   250,   305,   247,   248,   324,     0,   263,   304,
     311,   341,     0,   338,   370,   228,   371,   460,     0,   466,
     456,   436,   470,   472,   477,   479,   484,   486,   349,   490,
     351,   493,   360,     0,   166,   195,     0,   192,   184,   187,
     190,     0,     0,   503,   502,   508,     0,     0,   507,     0,
     511,   517,   523,   522,     0,   508,     0,   524,    21,    24,
       0,    85,   134,    47,     0,     0,   205,   214,   215,   213,
     212,     0,     0,     0,     0,   252,     0,   318,   264,     0,
     343,   369,   462,   473,   480,   487,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   510,
       0,     0,   514,   256,     0,    46,   251,   217,   219,     0,
     206,   221,     0,   253,     0,   463,     0,     0,     0,     0,
     185,   188,   191,   315,     0,   528,     0,   512,   505,   504,
     528,     0,     0,   259,    48,     0,   207,   320,   342,   196,
     198,   193,     0,   316,     0,     0,   513,     0,   515,   257,
       0,   208,   321,     0,     0,     0,     0,     0,     0,     0,
     260,   197,     0,   201,   194,   529,   530,   531,   199,     0,
       0,     0,   200,   202,     0,   203
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -595,  -595,  -595,   146,   550,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,   508,  -595,  -595,   549,  -595,  -595,
    -595,  -266,  -595,  -595,  -595,  -595,  -595,  -595,   365,  -595,
     554,   -54,  -595,   538,  -595,  -113,  -384,  -538,  -595,  -595,
    -236,  -168,   -80,  -595,    -1,  -286,   -13,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,   616,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -521,  -595,  -595,  -223,  -595,  -105,  -281,  -595,  -234,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
     449,  -595,  -595,   247,   -62,  -595,  -595,  -595,   349,  -595,
     -88,  -595,  -271,   224,  -274,  -270,    89,  -595,   271,  -595,
    -595,  -595,  -595,   450,   563,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -157,  -106,  -595,  -595,  -595,   248,  -595,
    -595,  -595,   -87,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,   343,  -594,  -425,  -579,  -595,  -595,  -303,  -107,   182,
    -595,  -595,  -595,  -265,  -595
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   169,   175,   458,   336,   749,   337,   613,
      16,    17,    35,    36,    63,    18,    32,    33,   338,   339,
     756,   757,   340,   341,   342,   343,   344,   345,   176,   177,
      29,    30,    45,    46,    47,    19,   160,   235,    80,    20,
      81,    82,   346,   347,   236,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   579,   860,   580,
     861,   582,   862,   360,   857,   923,   924,   940,   939,   944,
     621,   881,   361,   766,   767,   624,   362,   363,   427,   365,
     366,   367,   368,   750,   902,   751,   920,   369,   681,   848,
     622,   844,   922,   231,   687,   536,   377,   849,   232,   233,
     557,   558,   226,   227,   731,   278,   542,   381,   241,   166,
      54,    22,    86,   108,    67,    68,    23,    24,   164,    84,
      52,    25,   165,   279,   371,   107,   219,   220,   224,   221,
     390,   791,   559,   388,   790,   385,   787,   386,   885,   387,
     789,   391,   792,   392,   853,   393,   794,   394,   854,   395,
     796,   396,   855,   397,   799,   398,   801,    26,    41,    27,
      40,   372,   596,   597,   598,   373,   599,   600,   601,   602,
     374,   457,   375,   915,   376
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   162,   364,   218,   410,   417,   426,   412,   551,   242,
     553,   684,   555,   701,   456,   703,   568,   705,   735,   115,
     728,     2,   222,   -80,   444,   759,   742,     3,   591,   605,
     740,    42,   449,   450,    76,   277,   478,   626,    59,   591,
     500,    87,    88,   503,   504,   229,   413,   713,   715,    72,
       4,   879,     5,   413,     6,    66,     7,   503,   504,   738,
       8,    48,   730,   762,   739,   244,   245,     9,    72,   539,
      60,   413,   763,    10,   180,   706,   471,   413,   540,   239,
     730,   180,   480,   481,   378,   414,   867,   415,   230,   400,
     416,    11,   414,   406,   110,    43,   217,   480,   481,   479,
      31,   530,   764,   531,    77,    78,   161,   765,   541,   472,
     414,    44,    12,   181,   170,   171,   414,   531,   240,   225,
     383,    74,    56,    43,   760,   615,    43,    38,   218,   760,
     420,   565,   407,   239,   758,   249,   686,   781,   571,    44,
     818,    72,    44,   760,    79,    56,    75,   282,   378,   228,
     237,   229,   378,   816,   803,    53,    28,   586,   409,   864,
     734,   246,   378,   627,   250,    89,   492,   493,   218,    90,
     411,    91,    92,   870,   500,   276,   502,   503,   504,   505,
      61,   492,   493,   506,   218,    64,   733,   408,   247,   500,
      62,    13,   503,   504,   505,   698,    13,   625,   506,   533,
     242,    55,    14,   421,    93,    56,   760,   812,    34,   218,
     218,   835,   760,    65,   401,   429,   760,   699,    37,   824,
     760,   217,   525,   526,   527,   528,   529,   441,   452,   454,
     843,    39,   402,   761,   841,   530,   865,   531,   172,   780,
     768,   403,   173,   804,   174,    92,   871,   883,   477,   846,
     530,    66,   531,   480,   481,  -471,   692,    58,    57,   842,
    -471,   217,   707,  -478,   534,  -485,   378,    69,  -478,   379,
    -485,   280,   380,   218,   281,   709,    70,   217,  -471,   218,
     218,   218,   218,    71,   708,   218,  -478,   218,  -485,   218,
     712,   218,   538,   874,   590,    73,   603,   710,   544,   545,
     546,   547,   217,   217,   550,   745,   552,   815,   554,   218,
     729,   858,  -258,    83,   753,   462,   825,  -258,   608,   101,
     609,    49,    50,   102,    51,   754,   755,    85,   564,   480,
     481,    96,    98,   859,   262,  -258,   218,   492,   493,   567,
     103,   104,   105,   106,   739,   500,   743,   502,   503,   504,
     505,    99,   578,   100,   506,   585,   109,   112,   263,   264,
     332,   448,   113,   161,   114,   157,   217,   831,   168,   610,
     611,   158,   217,   217,   217,   217,   159,   161,   217,   163,
     217,   167,   217,   168,   217,    60,   223,   179,   225,   234,
     618,   255,   238,   243,   251,   527,   528,   529,   252,   253,
     273,   254,   217,   256,   257,   784,   530,   258,   531,   259,
     570,   260,   274,   492,   493,   261,   265,   783,   480,   481,
     275,   500,   389,   502,   503,   504,   505,   399,   239,   217,
     506,   229,   423,   431,   433,   434,   435,   437,   438,   410,
     442,   480,   481,   436,   466,   467,   468,   469,   473,   474,
     475,   532,   537,   616,   266,   267,    13,   478,   548,   268,
     576,   561,   269,   563,   583,   584,   589,   628,   623,   645,
     617,   685,   650,   218,   218,   686,   218,   270,   680,   688,
     733,   736,   530,   271,   531,   758,   272,   737,   244,   746,
     748,   770,   722,   723,   776,   725,   720,   778,   769,   406,
     490,   491,   492,   493,   494,   793,   795,   497,   498,   499,
     500,   797,   502,   503,   504,   505,   805,   806,   364,   506,
     817,   508,   509,   490,   491,   492,   493,   512,   513,   514,
     819,   826,   820,   500,   829,   502,   503,   504,   505,   822,
     827,   837,   506,   868,   508,   509,   838,   839,   840,   863,
     866,   519,   889,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   480,   481,   869,   217,   217,   872,   217,
     901,   530,   876,   531,   903,   878,   896,   897,   910,   927,
     161,   525,   526,   527,   528,   529,   908,   912,   916,   218,
     918,   931,   928,   929,   530,   933,   531,   942,   936,   937,
     218,   262,   218,   945,   612,    94,   178,    97,   788,   904,
      95,   382,   116,   884,   850,   262,   752,    21,   877,   798,
     800,   811,   560,   782,   535,   263,   264,   691,   476,   248,
     418,   111,   823,   422,     0,   917,   606,     0,   549,   263,
     264,     0,     0,     0,   218,   490,   491,   492,   493,   494,
       0,     0,   497,   498,   499,   500,     0,   502,   503,   504,
     505,     0,     0,   830,   506,     0,   508,   509,     0,     0,
       0,     0,     0,   833,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   265,     0,   370,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   217,     0,   265,   384,   521,
     522,   523,   524,   525,   526,   527,   528,   529,     0,     0,
     480,   481,     0,     0,     0,     0,   530,     0,   531,   405,
       0,   266,   267,   856,     0,     0,   268,     0,     0,   269,
       0,     0,     0,   419,     0,   266,   267,   217,   218,     0,
     268,   428,   694,   269,   270,   430,   569,   432,     0,     0,
       0,   218,     0,   272,   875,   440,     0,   882,   270,   445,
     446,   447,     0,   880,     0,     0,   455,   272,     0,     0,
     886,     0,     0,     0,   459,   460,   461,     0,   463,   464,
     465,     0,     0,     0,     0,     0,     0,     0,     0,   470,
       0,     0,   490,   491,     0,     0,   494,     0,     0,   497,
     498,   499,   500,     0,   502,   503,   504,   505,     0,     0,
       0,   506,     0,   508,   509,     0,     0,     0,   218,   512,
     513,   514,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   217,     0,   218,     0,     0,     0,   932,     0,     0,
       0,     0,     0,   519,   217,   520,   521,   522,   523,   524,
       0,     0,   941,   528,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   530,     0,   531,   566,     0,     0,     0,
       0,     0,     0,   572,     0,     0,     0,     0,     0,     0,
     577,     0,     0,     0,   581,     0,     0,     0,     0,   919,
       0,     0,   587,     0,     0,     0,     0,     0,     0,     0,
       0,   595,     0,   595,     0,     0,     0,   930,     0,     0,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,     0,     0,
       0,   629,   630,   631,   632,   633,   634,   635,   636,   637,
     638,     0,     0,   639,   640,   641,   642,   643,   644,     0,
     648,   649,     0,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,   664,   667,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,     0,
     682,     0,     0,     0,     0,     0,     0,     0,   480,   481,
       0,   693,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   718,
       0,     0,   648,   667,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   459,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   263,   264,   744,   455,     0,     0,
     747,     0,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   573,   502,   503,   504,   505,     0,     0,     0,   506,
     507,   508,   509,   510,   511,   771,   772,   512,   513,   514,
     515,   516,   517,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,   774,   775,     0,     0,     0,     0,
     574,   519,     0,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,     0,     0,     0,     0,     0,     0,     0,
     785,   530,     0,   531,     0,     0,     0,     0,     0,   575,
     266,   267,     0,   480,   481,   268,     0,   696,   269,     0,
       0,   802,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,   813,
     814,     0,   272,     0,     0,     0,   821,     0,   595,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,   834,     0,     0,     0,   836,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   847,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   573,   502,   503,   504,
     505,     0,     0,     0,   506,   507,   508,   509,   510,   511,
       0,     0,   512,   513,   514,   515,   516,   517,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   574,   519,     0,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,     0,     0,
       0,     0,     0,     0,     0,   262,   530,   888,   531,   890,
     891,   892,   894,   895,   588,     0,   898,   899,     0,   900,
       0,     0,     0,     0,     0,     0,     0,     0,   906,   263,
     264,     0,   283,     0,     0,     0,     0,     3,     0,   284,
     285,   286,     0,   287,     0,   288,   289,   290,   291,     0,
       0,     0,     0,     0,   921,     0,   292,   293,   294,   295,
       0,   926,     0,     0,     0,     0,     0,     0,   296,   297,
       0,   298,   299,     0,     0,   300,   935,     9,   301,   302,
       0,   303,   304,     0,     0,   305,   306,   265,     0,     0,
       0,   307,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   308,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,     0,     0,   309,   310,   266,   267,     0,     0,     0,
     268,     0,     0,   269,     0,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,     0,
     313,   314,     0,     0,   695,     0,     0,   272,     0,    43,
       0,     0,     0,     0,     0,     0,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    44,   324,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,   328,     0,     0,   329,   330,
       0,     0,     0,     0,     0,     0,     0,     0,   331,     0,
     332,   333,   334,   161,   335,   283,     0,     0,     0,     0,
       3,     0,   284,   285,   286,     0,   287,     0,   288,   289,
     290,   291,     0,     0,     0,     0,     0,     0,     0,   292,
     293,   294,   295,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   297,     0,   298,   299,     0,     0,   300,     0,
       9,   301,   302,     0,   303,   304,     0,     0,   305,   306,
       0,     0,     0,     0,   307,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,     0,     0,   309,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   314,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,   262,     0,     0,     0,   315,
     316,   317,   318,   319,   320,   321,   322,   323,    44,   324,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
     264,     0,     0,     0,   325,   326,   327,     0,   328,     0,
       0,   329,   330,     0,     0,     0,   288,   289,   290,   291,
       0,   331,     0,   332,   333,   334,   161,   832,   293,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,   591,     0,   304,     0,     0,     0,   265,   263,   264,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,     0,     0,   309,   266,   267,     0,     0,     0,
     268,     0,   697,   269,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,   270,     0,
       0,     0,     0,     0,     0,     0,     0,   272,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   592,   324,     0,     0,
       0,     0,     0,     0,   266,   267,     0,     0,     0,   268,
       0,     0,   593,   326,   327,     0,   328,     0,     0,   329,
     330,   288,   289,   290,   291,     0,     0,   270,     0,   594,
       0,   332,   333,   293,   161,     0,   272,     0,     0,   262,
       0,     0,     0,     0,     0,   297,     0,     0,   299,     0,
       0,   300,     0,     0,   301,     0,     0,     0,   304,     0,
       0,     0,     0,   263,   264,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   308,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,     0,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,   312,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   592,   324,     0,     0,     0,   263,   264,     0,   266,
     267,     0,     0,     0,   268,     0,     0,   593,   326,   327,
       0,   328,     0,     0,   329,   330,   288,   289,   290,   291,
       0,     0,   270,     0,   604,     0,   332,   333,   293,   161,
       0,   272,     0,     0,   400,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,     0,     0,   304,   265,     0,     0,     0,     0,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,   266,   267,   309,     0,     0,   268,     0,   700,
     269,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,     0,
      43,   262,     0,     0,     0,     0,     0,   315,   316,   317,
     318,   319,   320,   321,   322,   323,    44,   324,     0,     0,
       0,     0,     0,     0,     0,   263,   264,     0,     0,   401,
       0,     0,   325,   326,   327,     0,   328,     0,     0,   329,
     330,   288,   289,   290,   291,     0,     0,   402,     0,   331,
       0,   332,   333,   293,   161,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   297,     0,     0,   299,     0,
       0,   300,     0,     0,   301,     0,     0,     0,   304,     0,
       0,     0,   539,   265,     0,     0,     0,     0,   186,   187,
     188,   540,   190,   191,   192,   193,   194,   308,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,     0,     0,   309,
       0,   689,   267,     0,     0,     0,   268,     0,     0,   269,
       0,   311,   312,     0,     0,     0,     0,     0,     0,     0,
     424,     0,     0,     0,   270,     0,   313,   314,     0,     0,
     690,     0,     0,   272,     0,    43,     0,   262,     0,     0,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    44,   324,     0,     0,     0,     0,     0,     0,     0,
       0,   263,   264,     0,     0,     0,     0,   325,   326,   327,
       0,   328,     0,     0,   329,   330,     0,     0,   288,   289,
     290,   291,     0,     0,   331,     0,   332,   333,   425,   161,
     293,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,     0,     0,   299,     0,     0,   300,     0,
       0,   301,     0,     0,     0,   304,     0,     0,     0,   265,
       0,     0,     0,     0,     0,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,     0,     0,   309,   266,   267,     0,
       0,     0,   268,     0,   702,   269,     0,     0,   311,   312,
       0,   262,     0,     0,     0,     0,     0,   443,     0,     0,
     270,     0,     0,   313,   314,     0,     0,     0,     0,   272,
       0,     0,    43,     0,     0,   263,   264,     0,     0,   315,
     316,   317,   318,   319,   320,   321,   322,   323,    44,   324,
       0,     0,     0,     0,     0,   288,   289,   290,   291,     0,
       0,     0,     0,     0,   325,   326,   327,   293,   328,     0,
       0,   329,   330,     0,     0,     0,     0,     0,     0,   297,
       0,   331,   299,   332,   333,   300,   161,     0,   301,     0,
       0,     0,   304,   265,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   308,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,   266,   267,   309,     0,     0,   268,     0,   704,   269,
       0,     0,     0,     0,     0,   311,   312,     0,   262,     0,
       0,     0,     0,     0,   270,     0,     0,     0,     0,     0,
     313,   314,     0,   272,     0,     0,     0,     0,     0,    43,
       0,     0,   263,   264,     0,     0,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    44,   324,     0,     0,     0,
       0,     0,   288,   289,   290,   291,     0,     0,     0,     0,
       0,   325,   326,   327,   293,   328,     0,     0,   329,   330,
       0,     0,     0,     0,     0,     0,   297,     0,   331,   299,
     332,   333,   300,   161,     0,   301,     0,     0,     0,   304,
     265,     0,     0,     0,     0,     0,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,   726,   269,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,     0,    43,   262,     0,     0,
       0,     0,     0,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    44,   324,     0,     0,     0,     0,     0,     0,
       0,   263,   264,     0,     0,     0,     0,     0,   325,   326,
     327,     0,   328,     0,     0,   329,   330,   288,   289,   290,
     291,     0,     0,   619,     0,   331,   620,   332,   333,   293,
     161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   297,     0,     0,   299,     0,     0,   300,     0,     0,
     301,     0,     0,     0,   304,     0,     0,     0,   413,   265,
       0,     0,     0,     0,   186,   187,   188,     0,   190,   191,
     192,   193,   194,   308,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   206,   207,   208,     0,     0,   211,
     212,   213,   214,     0,     0,   309,     0,   711,   267,     0,
       0,     0,   268,     0,     0,   269,     0,   311,   312,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,     0,   646,   647,     0,     0,     0,     0,     0,   272,
       0,    43,     0,     0,   263,   264,     0,     0,   315,   316,
     317,   318,   319,   320,   321,   322,   323,    44,   324,     0,
       0,     0,     0,     0,   288,   289,   290,   291,     0,     0,
       0,     0,     0,   325,   326,   327,   293,   328,     0,     0,
     329,   330,     0,     0,     0,     0,     0,     0,   297,     0,
     331,   299,   332,   333,   300,   161,     0,   301,     0,     0,
       0,   304,   265,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
     266,   267,   309,     0,     0,   268,     0,   808,   269,     0,
       0,     0,     0,     0,   311,   312,     0,   262,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,   665,
     666,     0,   272,     0,     0,     0,     0,     0,    43,     0,
       0,   263,   264,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    44,   324,     0,     0,     0,     0,
       0,   288,   289,   290,   291,   716,     0,     0,     0,     0,
     325,   326,   327,   293,   328,     0,     0,   329,   330,     0,
       0,     0,     0,     0,     0,   297,     0,   331,   299,   332,
     333,   300,   161,     0,   301,     0,     0,     0,   304,   265,
       0,     0,     0,     0,     0,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   308,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,   266,   267,   309,
       0,     0,   268,     0,   809,   269,     0,     0,     0,     0,
       0,   311,   312,     0,   262,     0,     0,     0,     0,     0,
     270,     0,     0,     0,     0,     0,     0,     0,     0,   272,
       0,     0,     0,     0,     0,    43,     0,     0,   263,   264,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    44,   324,     0,     0,     0,     0,     0,   288,   289,
     290,   291,     0,     0,     0,   717,     0,   325,   326,   327,
     293,   328,     0,     0,   329,   330,     0,     0,     0,     0,
       0,     0,   297,     0,   331,   299,   332,   333,   300,   161,
       0,   301,     0,     0,     0,   304,   265,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,   266,   267,   309,     0,     0,   268,
       0,   810,   269,     0,     0,     0,     0,     0,   311,   312,
       0,   262,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,     0,    43,     0,     0,   263,   264,     0,     0,   315,
     316,   317,   318,   319,   320,   321,   322,   323,    44,   324,
       0,     0,     0,     0,     0,   288,   289,   290,   291,     0,
       0,     0,     0,     0,   325,   326,   327,   293,   328,     0,
       0,   329,   330,     0,     0,     0,     0,     0,     0,   297,
     404,   331,   299,   332,   333,   300,   161,     0,   301,     0,
       0,     0,   304,   265,     0,   439,     0,     0,     0,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   308,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,   266,   267,   309,     0,     0,   268,     0,   852,   269,
       0,     0,     0,     0,     0,   311,   312,     0,   262,     0,
       0,     0,     0,     0,   270,     0,     0,     0,     0,     0,
       0,     0,     0,   272,     0,     0,     0,     0,     0,    43,
       0,     0,   263,   264,     0,     0,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    44,   324,     0,     0,     0,
       0,     0,   288,   289,   290,   291,     0,     0,     0,   668,
       0,   325,   326,   327,   293,   328,     0,     0,   329,   330,
       0,     0,     0,     0,     0,     0,   297,     0,   331,   299,
     332,   333,   300,   161,     0,   301,     0,     0,     0,   304,
     265,     0,     0,     0,     0,     0,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,   873,   269,     0,     0,     0,
       0,     0,   311,   312,     0,   262,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,     0,    43,     0,     0,   263,
     264,     0,     0,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    44,   324,     0,     0,     0,     0,     0,   288,
     289,   290,   291,     0,     0,     0,     0,     0,   325,   326,
     327,   293,   328,     0,     0,   329,   330,     0,     0,     0,
       0,     0,     0,   297,     0,   331,   299,   332,   333,   300,
     161,     0,   301,     0,     0,     0,   304,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   308,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,   266,   267,   309,     0,     0,
     268,     0,   907,   269,     0,     0,     0,     0,     0,   311,
     312,     0,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,     0,     0,     0,   272,     0,     0,
     262,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    44,
     324,     0,     0,     0,   263,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,   328,
       0,     0,   329,   330,   288,   289,   290,   291,     0,     0,
       0,     0,   331,   683,   332,   333,   293,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
       0,   299,     0,     0,   300,     0,     0,   301,     0,     0,
       0,   304,   265,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
     266,   267,   309,     0,     0,   268,     0,   909,   269,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,     0,
       0,     0,   272,     0,     0,   262,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    44,   324,     0,     0,     0,   263,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   326,   327,     0,   328,     0,     0,   329,   330,   288,
     289,   290,   291,     0,     0,     0,     0,   331,   714,   332,
     333,   293,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   297,     0,     0,   299,     0,     0,   300,
       0,     0,   301,     0,     0,     0,   304,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   308,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,   266,   267,   309,     0,     0,
     268,     0,   938,   269,     0,     0,     0,     0,     0,   311,
     312,     0,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,     0,     0,     0,   272,     0,     0,
     262,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    44,
     324,     0,     0,     0,   263,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,   328,
       0,     0,   329,   330,   288,   289,   290,   291,     0,     0,
       0,     0,   331,   732,   332,   333,   293,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
       0,   299,     0,     0,   300,     0,     0,   301,     0,     0,
       0,   304,   265,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
     266,   267,   309,     0,     0,   268,     0,   943,   269,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,     0,
       0,     0,   272,     0,     0,   262,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    44,   324,     0,     0,     0,   263,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   326,   327,     0,   328,     0,     0,   329,   330,   288,
     289,   290,   291,     0,     0,     0,     0,   331,   741,   332,
     333,   293,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   297,     0,     0,   299,     0,     0,   300,
       0,     0,   301,     0,     0,     0,   304,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   308,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,   266,   267,   309,     0,     0,
     268,     0,     0,   269,     0,     0,     0,     0,     0,   311,
     312,     0,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,     0,     0,     0,   272,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    44,
     324,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,   328,
       0,     0,   329,   330,   288,   289,   290,   291,   887,     0,
       0,     0,   331,   845,   332,   333,   293,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
       0,   299,     0,     0,   300,     0,     0,   301,     0,     0,
       0,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    44,   324,     0,     0,     0,     0,
       0,   288,   289,   290,   291,     0,     0,     0,     0,     0,
     325,   326,   327,   293,   328,     0,     0,   329,   330,     0,
       0,     0,     0,     0,     0,   297,     0,   331,   299,   332,
     333,   300,   161,     0,   301,     0,     0,     0,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   308,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,     0,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    44,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,   328,     0,     0,   329,   330,   288,   289,   290,   291,
       0,     0,     0,     0,   331,   893,   332,   333,   293,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,     0,     0,   304,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,   905,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   315,   316,   317,
     318,   319,   320,   321,   322,   323,    44,   324,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   925,     0,     0,
       0,     0,   325,   326,   327,   293,   328,     0,     0,   329,
     330,     0,     0,     0,     0,     0,     0,   297,     0,   331,
     299,   332,   333,   300,   161,     0,   301,     0,     0,     0,
     304,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   187,   188,     0,   190,   191,   192,   193,   194,   308,
     196,   197,   198,   199,   200,   201,   202,   203,   204,     0,
     206,   207,   208,     0,     0,   211,   212,   213,   214,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   311,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   315,   316,   317,   318,   319,   320,
     321,   322,   323,    44,   324,     0,     0,     0,     0,     0,
     288,   289,   290,   291,     0,     0,     0,     0,     0,   325,
     326,   327,   293,   328,     0,     0,   329,   330,     0,     0,
       0,     0,     0,     0,   297,     0,   331,   299,   332,   333,
     300,   161,     0,   301,     0,     0,     0,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   187,   188,
       0,   190,   191,   192,   193,   194,   308,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   206,   207,   208,
       0,     0,   211,   212,   213,   214,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,   315,   316,   317,   318,   319,   320,   321,   322,   323,
      44,   324,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
     328,     0,     0,   329,   330,     0,     0,     0,     0,     0,
       0,     0,     0,   331,     0,   332,   333,     0,   161,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   573,   502,
     503,   504,   505,   480,   481,     0,   506,   507,   508,   509,
     510,   511,     0,     0,   512,   513,   514,   515,   516,   517,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   574,   519,     0,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
       0,     0,     0,     0,     0,     0,     0,     0,   530,     0,
     531,     0,     0,     0,     0,     0,   719,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   573,   502,   503,   504,
     505,   480,   481,     0,   506,   507,   508,   509,   510,   511,
       0,     0,   512,   513,   514,   515,   516,   517,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   574,   519,     0,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,   530,     0,   531,     0,
       0,     0,     0,     0,   727,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   573,   502,   503,   504,   505,   480,
     481,     0,   506,   507,   508,   509,   510,   511,     0,     0,
     512,   513,   514,   515,   516,   517,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   574,   519,     0,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,     0,     0,     0,     0,
       0,     0,     0,     0,   530,     0,   531,     0,     0,     0,
       0,     0,   786,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   573,   502,   503,   504,   505,   480,   481,     0,
     506,   507,   508,   509,   510,   511,     0,     0,   512,   513,
     514,   515,   516,   517,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   574,   519,     0,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,     0,     0,     0,     0,     0,     0,
       0,     0,   530,     0,   531,     0,     0,     0,     0,     0,
     851,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,   490,
     491,   492,   493,   494,     0,     0,   497,   498,   499,   500,
       0,   502,   503,   504,   505,     0,     0,     0,   506,     0,
     508,   509,     0,     0,     0,     0,   512,   513,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     519,     0,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,     0,     0,     0,   480,   481,     0,     0,     0,
     530,     0,   531,   490,   491,   492,   493,   494,   543,     0,
     497,   498,   499,   500,     0,   502,   503,   504,   505,     0,
       0,     0,   506,     0,   508,   509,     0,     0,     0,     0,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,     0,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,     0,     0,     0,     0,
       0,     0,   480,   481,   530,     0,   531,   490,   491,   492,
     493,   494,   914,     0,   497,   498,   499,   500,     0,   502,
     503,   504,   505,     0,     0,     0,   506,     0,   508,   509,
       0,     0,     0,     0,   512,   513,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   519,     0,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
       0,     0,     0,     0,     0,     0,     0,     0,   530,     0,
     531,   480,   481,   614,   490,   491,   492,   493,   494,     0,
       0,   497,   498,   499,   500,     0,   502,   503,   504,   505,
       0,     0,     0,   506,     0,   508,   509,     0,     0,     0,
       0,   512,   513,   514,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   519,     0,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,     0,     0,     0,
       0,     0,     0,     0,     0,   530,     0,   531,   480,   481,
     721,     0,     0,   490,   491,   492,   493,   494,     0,     0,
     497,   498,   499,   500,     0,   502,   503,   504,   505,     0,
       0,     0,   506,     0,   508,   509,     0,     0,     0,     0,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,     0,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,     0,     0,     0,     0,
       0,     0,     0,     0,   530,     0,   531,   480,   481,   724,
     490,   491,   492,   493,   494,     0,     0,   497,   498,   499,
     500,     0,   502,   503,   504,   505,     0,     0,     0,   506,
       0,   508,   509,     0,     0,     0,     0,   512,   513,   514,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   519,     0,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,     0,     0,     0,     0,     0,     0,     0,
       0,   530,     0,   531,   480,   481,   807,     0,     0,   490,
     491,   492,   493,   494,     0,     0,   497,   498,   499,   500,
       0,   502,   503,   504,   505,     0,     0,     0,   506,     0,
     508,   509,     0,     0,     0,     0,   512,   513,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     519,     0,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,     0,     0,     0,     0,     0,     0,     0,     0,
     530,     0,   531,   480,   481,   911,   490,   491,   492,   493,
     494,     0,     0,   497,   498,   499,   500,     0,   502,   503,
     504,   505,     0,     0,     0,   506,     0,   508,   509,     0,
       0,     0,     0,   512,   513,   514,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   519,     0,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,     0,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
     480,   481,   913,     0,     0,   490,   491,   492,   493,   494,
       0,     0,   497,   498,   499,   500,     0,   502,   503,   504,
     505,     0,     0,     0,   506,     0,   508,   509,     0,     0,
       0,     0,   512,   513,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   519,     0,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,   530,     0,   531,   480,
     481,   934,   490,   491,   492,   493,   494,     0,     0,   497,
     498,   499,   500,     0,   502,   503,   504,   505,     0,     0,
       0,   506,     0,   508,   509,     0,     0,     0,     0,   512,
     513,   514,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   519,     0,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,     0,     0,     0,     0,     0,
       0,     0,     0,   530,     0,   531,   562,   480,   481,     0,
       0,   490,   491,   492,   493,   494,     0,     0,   497,   498,
     499,   500,     0,   502,   503,   504,   505,     0,     0,     0,
     506,     0,   508,   509,     0,     0,     0,     0,   512,   513,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   519,     0,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,     0,     0,     0,     0,     0,     0,
       0,     0,   530,     0,   531,   773,     0,     0,     0,   490,
     491,   492,   493,   494,   480,   481,   497,   498,   499,   500,
       0,   502,   503,   504,   505,     0,     0,     0,   506,     0,
     508,   509,     0,     0,     0,     0,   512,   513,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     519,     0,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,     0,     0,     0,     0,     0,     0,     0,     0,
     530,     0,   531,   779,     0,     0,     0,     0,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   573,   502,   503,
     504,   505,  -228,   480,   481,   506,   507,   508,   509,   510,
     511,     0,     0,   512,   513,   514,   515,   516,   517,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   574,   519,     0,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,     0,
       0,     0,     0,     0,     0,     0,     0,   530,     0,   531,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   480,   481,     0,   506,   507,   508,   509,   510,   511,
       0,     0,   512,   513,   514,   515,   516,   517,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,     0,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,   530,     0,   531,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,   490,   491,   492,   493,   494,     0,     0,
     497,   498,   499,   500,     0,   502,   503,   504,   505,     0,
       0,     0,   506,     0,   508,   509,     0,     0,   607,     0,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,     0,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,     0,     0,     0,   480,
     481,     0,     0,     0,   530,     0,   531,   490,   491,   492,
     493,   494,     0,     0,   497,   498,   499,   500,     0,   502,
     503,   504,   505,     0,     0,     0,   506,     0,   508,   509,
       0,     0,     0,     0,   512,   513,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   519,   777,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
       0,     0,     0,   480,   481,     0,     0,     0,   530,     0,
     531,   490,   491,   492,   493,   494,     0,     0,   497,   498,
     499,   500,     0,   502,   503,   504,   505,     0,     0,     0,
     506,     0,   508,   509,     0,     0,     0,     0,   512,   513,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   828,     0,
       0,     0,   519,     0,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,     0,     0,     0,   480,   481,     0,
       0,     0,   530,     0,   531,   490,   491,   492,   493,   494,
       0,     0,   497,   498,   499,   500,     0,   502,   503,   504,
     505,     0,     0,     0,   506,     0,   508,   509,     0,     0,
       0,     0,   512,   513,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   519,     0,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,     0,     0,
       0,   480,   481,     0,     0,     0,   530,     0,   531,   490,
     491,   492,   493,   494,     0,     0,   497,   498,   499,   500,
       0,   502,   503,   504,   505,     0,     0,     0,   506,     0,
     508,   509,     0,     0,     0,     0,   512,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,     0,     0,     0,   480,   481,     0,     0,     0,
     530,     0,   531,   490,   491,   492,   493,   494,     0,     0,
     497,   498,   499,   500,     0,   502,   503,   504,   505,   480,
     481,     0,   506,     0,   508,   509,     0,     0,     0,     0,
     512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,     0,     0,     0,     0,
       0,     0,     0,     0,   530,     0,   531,   490,   491,   492,
     493,   494,     0,     0,   497,   498,   499,   500,     0,   502,
     503,   504,   505,   480,   481,     0,   506,     0,   508,   509,
       0,   490,   491,   492,   493,   494,     0,     0,   497,   498,
     499,   500,     0,   502,   503,   504,   505,   480,   481,     0,
     506,     0,   508,   509,     0,     0,     0,     0,     0,     0,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
       0,     0,     0,     0,     0,     0,     0,     0,   530,     0,
     531,     0,     0,     0,     0,     0,   522,   523,   524,   525,
     526,   527,   528,   529,     0,     0,     0,     0,     0,     0,
       0,     0,   530,     0,   531,   490,   491,   492,   493,   494,
       0,     0,   497,   498,   499,   500,     0,   502,   503,   504,
     505,     0,     0,     0,   506,     0,   508,   509,     0,   490,
     491,   492,   493,   494,     0,     0,   497,     0,     0,   500,
       0,   502,   503,   504,   505,     0,     0,     0,   506,     0,
     508,   509,     0,     0,     0,     0,     0,     0,     0,   451,
       0,   523,   524,   525,   526,   527,   528,   529,     0,     0,
       0,   182,     0,     0,     0,     0,   530,   183,   531,     0,
       0,     0,     0,     0,     0,   523,   524,   525,   526,   527,
     528,   529,   184,     0,     0,     0,     0,     0,     0,     0,
     530,     0,   531,     0,     0,     0,   185,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,    44,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   182,
       0,     0,     0,     0,     0,   183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,    44,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,     0,     0,     0,     0,     0,   183,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   184,     0,     0,     0,     0,     0,
     315,   316,   317,     0,     0,     0,     0,     0,   185,    44,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,     0,     0,
       0,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   185,    44,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,     0,     0,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,    43,     0,   139,   140,   141,   142,   143,   144,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,     0,
       0,   155,   156
};

static const yytype_int16 yycheck[] =
{
      13,    81,   236,   109,   275,   279,   292,   277,   392,   166,
     394,   532,   396,   551,   317,   553,    19,   555,   597,    73,
      19,     0,   109,     7,   310,   619,   605,     6,    49,   454,
      19,    33,   313,   314,    34,    46,   118,    18,    21,    49,
     112,    14,    15,   115,   116,   135,   113,   568,   569,   128,
      29,   127,    31,   113,    33,   144,    35,   115,   116,   176,
      39,    74,   122,   113,   181,   146,   147,    46,   128,   113,
      53,   113,   122,    52,   144,   154,   144,   113,   122,   144,
     122,   144,    20,    21,   151,   152,   122,   154,   178,    32,
     157,    70,   152,   144,   183,   128,   109,    20,    21,   181,
     138,   173,   152,   175,   104,   105,   182,   157,   152,   177,
     152,   144,    91,   183,    14,    15,   152,   175,   183,   144,
     183,   151,   151,   128,   151,   158,   128,   182,   234,   151,
     287,   417,   183,   144,   144,   152,    42,    43,   424,   144,
     734,   128,   144,   151,   144,   151,   176,   234,   151,   178,
     163,   135,   151,   732,   181,    58,   144,   443,   183,   181,
     181,   151,   151,   144,   181,   138,   104,   105,   274,   142,
     176,   144,   145,   181,   112,   229,   114,   115,   116,   117,
     163,   104,   105,   121,   290,   154,   151,   274,   178,   112,
     173,   175,   115,   116,   117,   159,   175,   478,   121,   183,
     357,   152,   181,   290,   177,   151,   151,   728,   144,   315,
     316,   176,   151,   182,   157,   295,   151,   181,   144,   740,
     151,   234,   160,   161,   162,   163,   164,   307,   315,   316,
     768,   144,   175,   178,   151,   173,   815,   175,   138,   178,
     624,   184,   142,   178,   144,   145,   825,   178,   361,   770,
     173,   144,   175,    20,    21,   154,   542,   151,   154,   176,
     159,   274,   159,   154,   377,   154,   151,   144,   159,   154,
     159,   178,   157,   379,   181,   159,   144,   290,   177,   385,
     386,   387,   388,   144,   181,   391,   177,   393,   177,   395,
     564,   397,   379,   831,   451,   177,   453,   181,   385,   386,
     387,   388,   315,   316,   391,   608,   393,   732,   395,   415,
     591,   159,   154,   144,    11,   328,   741,   159,   181,    65,
     183,     4,     5,    69,     7,    22,    23,   182,   415,    20,
      21,   138,   144,   181,    32,   177,   442,   104,   105,   419,
      86,    87,    88,    89,   181,   112,   183,   114,   115,   116,
     117,   144,   432,   144,   121,   442,   152,   182,    56,    57,
     179,   180,   182,   182,   144,    34,   379,   751,   146,   147,
     148,    34,   385,   386,   387,   388,   177,   182,   391,   154,
     393,     1,   395,   146,   397,    53,   144,   182,   144,   154,
     470,   177,   182,   181,   158,   162,   163,   164,   158,   158,
     181,   158,   415,   158,   158,   691,   173,   158,   175,   158,
     423,   158,   154,   104,   105,   158,   114,   687,    20,    21,
     181,   112,   144,   114,   115,   116,   117,    32,   144,   442,
     121,   135,   177,   177,   158,   158,   177,   158,   138,   710,
     158,    20,    21,   181,   180,    36,   181,   181,   181,   181,
     181,   177,   144,   466,   152,   153,   175,   118,   178,   157,
      13,   176,   160,   176,   154,   138,   180,   144,   135,   144,
     182,     7,   144,   579,   580,    42,   582,   175,   144,   181,
     151,   176,   173,   181,   175,   144,   184,   151,   146,   183,
     146,   177,   579,   580,   144,   582,   576,     1,   158,   144,
     102,   103,   104,   105,   106,   159,   159,   109,   110,   111,
     112,   159,   114,   115,   116,   117,   158,   144,   752,   121,
     144,   123,   124,   102,   103,   104,   105,   129,   130,   131,
     176,   183,   176,   112,   146,   114,   115,   116,   117,   176,
     176,   144,   121,   817,   123,   124,   144,   144,   144,   177,
     176,   153,   144,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    20,    21,   176,   579,   580,   176,   582,
     183,   173,   178,   175,   159,   176,   176,   176,   158,    49,
     182,   160,   161,   162,   163,   164,   181,   159,   176,   695,
     176,   178,   176,   183,   173,   158,   175,   178,   176,   176,
     706,    32,   708,   178,   458,    55,    98,    58,   695,   875,
      56,   246,    74,   849,   782,    32,   617,     1,   841,   706,
     708,   726,   398,   685,   377,    56,    57,   538,   357,   179,
     281,    68,   739,   290,    -1,   900,   454,    -1,   390,    56,
      57,    -1,    -1,    -1,   750,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   750,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,   753,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   695,   114,    -1,   236,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   706,    -1,   708,    -1,   114,   249,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   173,    -1,   175,   270,
      -1,   152,   153,   803,    -1,    -1,   157,    -1,    -1,   160,
      -1,    -1,    -1,   284,    -1,   152,   153,   750,   844,    -1,
     157,   292,   159,   160,   175,   296,   177,   298,    -1,    -1,
      -1,   857,    -1,   184,   834,   306,    -1,   844,   175,   310,
     311,   312,    -1,   843,    -1,    -1,   317,   184,    -1,    -1,
     857,    -1,    -1,    -1,   325,   326,   327,    -1,   329,   330,
     331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,
      -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,   924,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   844,    -1,   939,    -1,    -1,    -1,   924,    -1,    -1,
      -1,    -1,    -1,   153,   857,   155,   156,   157,   158,   159,
      -1,    -1,   939,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,   417,    -1,    -1,    -1,
      -1,    -1,    -1,   424,    -1,    -1,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,   435,    -1,    -1,    -1,    -1,   902,
      -1,    -1,   443,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   452,    -1,   454,    -1,    -1,    -1,   920,    -1,    -1,
      -1,   924,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   939,    -1,    -1,    -1,
      -1,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,    -1,    -1,   494,   495,   496,   497,   498,   499,    -1,
     501,   502,    -1,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,    -1,
     531,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   542,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   570,
      -1,    -1,   573,   574,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   593,   594,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   604,    56,    57,   607,   608,    -1,    -1,
     611,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   646,   647,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,   665,   666,    -1,    -1,    -1,    -1,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     691,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,   181,
     152,   153,    -1,    20,    21,   157,    -1,   159,   160,    -1,
      -1,   712,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,   730,
     731,    -1,   184,    -1,    -1,    -1,   737,    -1,   739,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   752,    -1,    -1,    -1,   756,    -1,    -1,    -1,   760,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   777,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   173,   858,   175,   860,
     861,   862,   863,   864,   181,    -1,   867,   868,    -1,   870,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   879,    56,
      57,    -1,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,   905,    -1,    25,    26,    27,    28,
      -1,   912,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    41,    -1,    -1,    44,   927,    46,    47,    48,
      -1,    50,    51,    -1,    -1,    54,    55,   114,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    93,   152,   153,    -1,    -1,    -1,
     157,    -1,    -1,   160,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
     119,   120,    -1,    -1,   181,    -1,    -1,   184,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,   182,   183,     1,    -1,    -1,    -1,    -1,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    -1,    -1,    44,    -1,
      46,    47,    48,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    32,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,   177,    -1,   179,   180,   181,   182,   183,    26,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    49,    -1,    51,    -1,    -1,    -1,   114,    56,    57,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,   152,   153,    -1,    -1,    -1,
     157,    -1,   159,   160,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    -1,    -1,   175,    -1,   177,
      -1,   179,   180,    26,   182,    -1,   184,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    56,    57,    -1,   152,
     153,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    14,    15,    16,    17,
      -1,    -1,   175,    -1,   177,    -1,   179,   180,    26,   182,
      -1,   184,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   152,   153,    92,    -1,    -1,   157,    -1,   159,
     160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,
     128,    32,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    -1,    -1,   175,    -1,   177,
      -1,   179,   180,    26,   182,    -1,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    61,    62,
      63,   122,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,   152,   153,    -1,    -1,    -1,   157,    -1,    -1,   160,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,   175,    -1,   119,   120,    -1,    -1,
     181,    -1,    -1,   184,    -1,   128,    -1,    32,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   177,    -1,   179,   180,   181,   182,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,   160,    -1,    -1,   104,   105,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
     175,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,   184,
      -1,    -1,   128,    -1,    -1,    56,    57,    -1,    -1,   135,
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
     184,    -1,    -1,    -1,    -1,    -1,   128,    32,    -1,    -1,
      -1,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,   160,   161,
     162,    -1,   164,    -1,    -1,   167,   168,    14,    15,    16,
      17,    -1,    -1,   175,    -1,   177,   178,   179,   180,    26,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,   152,   153,    -1,
      -1,    -1,   157,    -1,    -1,   160,    -1,   104,   105,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,   184,
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
     152,   153,    92,    -1,    -1,   157,    -1,   159,   160,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    32,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,    26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,   114,    -1,    -1,
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
     152,   153,    92,    -1,    -1,   157,    -1,   159,   160,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    32,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,    26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   152,   153,    92,    -1,    -1,
     157,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
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
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     173,    -1,   175,   102,   103,   104,   105,   106,   181,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   173,    -1,   175,   102,   103,   104,
     105,   106,   181,    -1,   109,   110,   111,   112,    -1,   114,
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
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    20,    21,
     178,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,    20,    21,   178,
     102,   103,   104,   105,   106,    -1,    -1,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,   175,    20,    21,   178,    -1,    -1,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
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
      -1,    -1,    -1,   173,    -1,   175,   176,    20,    21,    -1,
      -1,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,   176,    -1,    -1,    -1,   102,
     103,   104,   105,   106,    20,    21,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,   176,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,   125,
     126,    -1,    -1,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    20,    21,    -1,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   173,    -1,   175,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   173,    -1,
     175,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   173,    -1,   175,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   173,    -1,   175,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     173,    -1,   175,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    20,
      21,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    20,    21,    -1,   121,    -1,   123,   124,
      -1,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    20,    21,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,   102,
     103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,   173,    30,   175,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,   162,
     163,   164,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    45,    -1,    -1,    -1,    -1,    -1,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    59,   144,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,   144,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   128,    -1,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168
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
      53,   163,   173,   199,   154,   182,   144,   299,   300,   144,
     144,   144,   128,   177,   151,   176,    34,   104,   105,   144,
     223,   225,   226,   144,   304,   182,   297,    14,    15,   138,
     142,   144,   145,   177,   189,   215,   138,   202,   144,   144,
     144,    65,    69,    86,    87,    88,    89,   310,   298,   152,
     183,   299,   182,   182,   144,   216,   218,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   123,   124,   125,   126,   129,
     130,   131,   132,   133,   134,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   167,   168,    34,    34,   177,
     221,   182,   227,   154,   303,   307,   294,     1,   146,   188,
      14,    15,   138,   142,   144,   189,   213,   214,   199,   182,
     144,   183,    24,    30,    45,    59,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   231,   309,   311,
     312,   314,   317,   144,   313,   144,   287,   288,   178,   135,
     178,   278,   283,   284,   154,   222,   229,   231,   182,   144,
     183,   293,   308,   181,   146,   147,   151,   178,   298,   152,
     181,   158,   158,   158,   158,   177,   158,   158,   158,   158,
     158,   158,    32,    56,    57,   114,   152,   153,   157,   160,
     175,   181,   184,   181,   154,   181,   216,    46,   290,   308,
     178,   181,   317,     1,     8,     9,    10,    12,    14,    15,
      16,    17,    25,    26,    27,    28,    37,    38,    40,    41,
      44,    47,    48,    50,    51,    54,    55,    60,    70,    92,
      93,   104,   105,   119,   120,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   145,   160,   161,   162,   164,   167,
     168,   177,   179,   180,   181,   183,   191,   193,   203,   204,
     207,   208,   209,   210,   211,   212,   227,   228,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     248,   257,   261,   262,   263,   264,   265,   266,   267,   272,
     275,   309,   346,   350,   355,   357,   359,   281,   151,   154,
     157,   292,   213,   183,   275,   320,   322,   324,   318,   144,
     315,   326,   328,   330,   332,   334,   336,   338,   340,    32,
      32,   157,   175,   184,   176,   275,   144,   183,   317,   183,
     287,   176,   290,   113,   152,   154,   157,   289,   283,   275,
     308,   317,   346,   177,   113,   181,   230,   263,   275,   227,
     275,   177,   275,   158,   158,   177,   181,   158,   138,    54,
     275,   227,   158,   113,   230,   275,   275,   275,   180,   261,
     261,    12,   317,    12,   317,   275,   352,   356,   190,   275,
     275,   275,   231,   275,   275,   275,   180,    36,   181,   181,
     275,   144,   177,   181,   181,   181,   293,   220,   118,   181,
      20,    21,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   121,   122,   123,   124,
     125,   126,   129,   130,   131,   132,   133,   134,   152,   153,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     173,   175,   177,   183,   220,   278,   280,   144,   317,   113,
     122,   152,   291,   181,   317,   317,   317,   317,   178,   313,
     317,   221,   317,   221,   317,   221,   144,   285,   286,   317,
     288,   176,   176,   176,   317,   230,   275,   227,    19,   177,
     231,   230,   275,   113,   152,   181,    13,   275,   227,   242,
     244,   275,   246,   154,   138,   317,   230,   275,   181,   180,
     308,    49,   144,   160,   177,   275,   347,   348,   349,   351,
     352,   353,   354,   308,   177,   348,   354,   127,   181,   183,
     147,   148,   188,   194,   178,   158,   231,   182,   227,   175,
     178,   255,   275,   135,   260,   261,    18,   144,   144,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   144,   119,   120,   275,   275,
     144,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   119,   120,   275,    21,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     144,   273,   275,   178,   255,     7,    42,   279,   181,   152,
     181,   291,   230,   275,   159,   181,   159,   159,   159,   181,
     159,   222,   159,   222,   159,   222,   154,   159,   181,   159,
     181,   152,   289,   255,   178,   255,    18,   158,   275,   181,
     227,   178,   317,   317,   178,   317,   159,   181,    19,   261,
     122,   289,   178,   151,   181,   349,   176,   151,   176,   181,
      19,   178,   349,   183,   275,   352,   183,   275,   146,   192,
     268,   270,   229,    11,    22,    23,   205,   206,   144,   347,
     151,   178,   113,   122,   152,   157,   258,   259,   221,   158,
     177,   275,   275,   176,   275,   275,   144,   154,     1,   176,
     178,    43,   279,   290,   230,   275,   181,   321,   317,   325,
     319,   316,   327,   159,   331,   159,   335,   159,   317,   339,
     285,   341,   275,   181,   178,   158,   144,   178,   159,   159,
     159,   260,   255,   275,   275,   348,   349,   144,   347,   176,
     176,   275,   176,   353,   255,   348,   183,   176,   149,   146,
     317,   221,   183,   227,   275,   176,   275,   144,   144,   144,
     144,   151,   176,   222,   276,   178,   255,   275,   274,   282,
     226,   181,   159,   329,   333,   337,   227,   249,   159,   181,
     243,   245,   247,   177,   181,   349,   176,   122,   289,   176,
     181,   349,   176,   159,   222,   227,   178,   258,   176,   127,
     227,   256,   317,   178,   225,   323,   317,    18,   275,   144,
     275,   275,   275,   178,   275,   275,   176,   176,   275,   275,
     275,   183,   269,   159,   206,   113,   275,   159,   181,   159,
     158,   178,   159,   178,   181,   358,   176,   358,   176,   231,
     271,   275,   277,   250,   251,    18,   275,    49,   176,   183,
     231,   178,   317,   158,   178,   275,   176,   176,   159,   253,
     252,   317,   178,   159,   254,   178
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
     281,   282,   281,   281,   283,   283,   284,   284,   285,   285,
     286,   286,   287,   288,   288,   289,   289,   290,   290,   290,
     290,   290,   290,   291,   291,   291,   292,   292,   293,   293,
     293,   293,   293,   294,   294,   295,   295,   296,   297,   296,
     298,   298,   298,   299,   300,   300,   301,   302,   302,   303,
     303,   304,   305,   305,   307,   306,   308,   308,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   310,   310,   310,   310,   310,   310,   311,
     312,   312,   313,   313,   315,   316,   314,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   318,   319,   317,   317,   320,   321,
     317,   322,   323,   317,   324,   325,   317,   317,   326,   327,
     317,   328,   329,   317,   317,   330,   331,   317,   332,   333,
     317,   317,   334,   335,   317,   336,   337,   317,   338,   339,
     317,   340,   341,   317,   343,   342,   345,   344,   346,   346,
     346,   346,   347,   347,   347,   347,   348,   348,   349,   349,
     350,   350,   350,   350,   350,   350,   351,   351,   352,   353,
     353,   354,   354,   355,   355,   356,   356,   357,   358,   358,
     359,   359
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
       3,     0,     7,     5,     2,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     1,     1,     2,     3,
       5,     3,     3,     1,     1,     1,     0,     1,     4,     6,
       5,     5,     4,     0,     2,     0,     1,     5,     0,     5,
       0,     3,     5,     4,     1,     2,     4,     5,     7,     0,
       2,     2,     1,     1,     0,     7,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     3,     0,     0,     6,     1,     1,     1,
       1,     4,     3,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     8,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     0,     7,     0,     7,     1,     1,
       1,     1,     3,     3,     5,     5,     1,     3,     0,     2,
       6,     5,     7,     8,     6,     8,     1,     3,     3,     1,
       1,     1,     3,     5,     5,     1,     3,     4,     0,     3,
      10,    10
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
#line 3444 "ds_parser.cpp"
        break;

    case 188: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3450 "ds_parser.cpp"
        break;

    case 189: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3456 "ds_parser.cpp"
        break;

    case 190: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3462 "ds_parser.cpp"
        break;

    case 191: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3468 "ds_parser.cpp"
        break;

    case 193: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3474 "ds_parser.cpp"
        break;

    case 197: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3480 "ds_parser.cpp"
        break;

    case 203: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3486 "ds_parser.cpp"
        break;

    case 204: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3492 "ds_parser.cpp"
        break;

    case 206: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3498 "ds_parser.cpp"
        break;

    case 208: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3504 "ds_parser.cpp"
        break;

    case 209: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp"
        break;

    case 210: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp"
        break;

    case 211: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp"
        break;

    case 212: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 213: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3534 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3540 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3546 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3552 "ds_parser.cpp"
        break;

    case 217: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3558 "ds_parser.cpp"
        break;

    case 218: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3564 "ds_parser.cpp"
        break;

    case 219: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3570 "ds_parser.cpp"
        break;

    case 220: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3576 "ds_parser.cpp"
        break;

    case 221: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3582 "ds_parser.cpp"
        break;

    case 222: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3588 "ds_parser.cpp"
        break;

    case 223: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3594 "ds_parser.cpp"
        break;

    case 227: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3600 "ds_parser.cpp"
        break;

    case 228: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3606 "ds_parser.cpp"
        break;

    case 229: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3612 "ds_parser.cpp"
        break;

    case 230: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3618 "ds_parser.cpp"
        break;

    case 231: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3624 "ds_parser.cpp"
        break;

    case 232: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3630 "ds_parser.cpp"
        break;

    case 233: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3636 "ds_parser.cpp"
        break;

    case 234: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3642 "ds_parser.cpp"
        break;

    case 235: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3648 "ds_parser.cpp"
        break;

    case 236: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3654 "ds_parser.cpp"
        break;

    case 237: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3660 "ds_parser.cpp"
        break;

    case 238: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3666 "ds_parser.cpp"
        break;

    case 240: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3672 "ds_parser.cpp"
        break;

    case 241: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3678 "ds_parser.cpp"
        break;

    case 248: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3684 "ds_parser.cpp"
        break;

    case 255: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3690 "ds_parser.cpp"
        break;

    case 256: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3696 "ds_parser.cpp"
        break;

    case 258: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3702 "ds_parser.cpp"
        break;

    case 259: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3708 "ds_parser.cpp"
        break;

    case 260: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3714 "ds_parser.cpp"
        break;

    case 261: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3720 "ds_parser.cpp"
        break;

    case 262: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3726 "ds_parser.cpp"
        break;

    case 263: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3732 "ds_parser.cpp"
        break;

    case 264: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3738 "ds_parser.cpp"
        break;

    case 265: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3744 "ds_parser.cpp"
        break;

    case 266: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3750 "ds_parser.cpp"
        break;

    case 267: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3756 "ds_parser.cpp"
        break;

    case 272: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3762 "ds_parser.cpp"
        break;

    case 275: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3768 "ds_parser.cpp"
        break;

    case 278: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3774 "ds_parser.cpp"
        break;

    case 280: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3780 "ds_parser.cpp"
        break;

    case 281: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3786 "ds_parser.cpp"
        break;

    case 283: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3792 "ds_parser.cpp"
        break;

    case 284: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3798 "ds_parser.cpp"
        break;

    case 285: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3804 "ds_parser.cpp"
        break;

    case 286: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3810 "ds_parser.cpp"
        break;

    case 287: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3816 "ds_parser.cpp"
        break;

    case 288: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3822 "ds_parser.cpp"
        break;

    case 290: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3828 "ds_parser.cpp"
        break;

    case 293: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3834 "ds_parser.cpp"
        break;

    case 294: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3840 "ds_parser.cpp"
        break;

    case 298: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3846 "ds_parser.cpp"
        break;

    case 303: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3852 "ds_parser.cpp"
        break;

    case 308: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3858 "ds_parser.cpp"
        break;

    case 311: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3864 "ds_parser.cpp"
        break;

    case 312: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3870 "ds_parser.cpp"
        break;

    case 313: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3876 "ds_parser.cpp"
        break;

    case 314: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3882 "ds_parser.cpp"
        break;

    case 317: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3888 "ds_parser.cpp"
        break;

    case 346: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3894 "ds_parser.cpp"
        break;

    case 347: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3900 "ds_parser.cpp"
        break;

    case 348: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3906 "ds_parser.cpp"
        break;

    case 349: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3912 "ds_parser.cpp"
        break;

    case 350: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3918 "ds_parser.cpp"
        break;

    case 351: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3924 "ds_parser.cpp"
        break;

    case 352: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3930 "ds_parser.cpp"
        break;

    case 353: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3936 "ds_parser.cpp"
        break;

    case 354: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3942 "ds_parser.cpp"
        break;

    case 355: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3948 "ds_parser.cpp"
        break;

    case 356: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3954 "ds_parser.cpp"
        break;

    case 357: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3960 "ds_parser.cpp"
        break;

    case 358: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3966 "ds_parser.cpp"
        break;

    case 359: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3972 "ds_parser.cpp"
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
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4272 "ds_parser.cpp"
    break;

  case 16:
#line 470 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4278 "ds_parser.cpp"
    break;

  case 17:
#line 471 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4284 "ds_parser.cpp"
    break;

  case 18:
#line 475 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4290 "ds_parser.cpp"
    break;

  case 19:
#line 479 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4300 "ds_parser.cpp"
    break;

  case 20:
#line 484 "ds_parser.ypp"
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
#line 493 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4324 "ds_parser.cpp"
    break;

  case 22:
#line 501 "ds_parser.ypp"
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
#line 517 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4352 "ds_parser.cpp"
    break;

  case 24:
#line 522 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4362 "ds_parser.cpp"
    break;

  case 25:
#line 530 "ds_parser.ypp"
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
#line 551 "ds_parser.ypp"
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
#line 560 "ds_parser.ypp"
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
#line 586 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4433 "ds_parser.cpp"
    break;

  case 30:
#line 589 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4444 "ds_parser.cpp"
    break;

  case 31:
#line 595 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4455 "ds_parser.cpp"
    break;

  case 32:
#line 604 "ds_parser.ypp"
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
#line 625 "ds_parser.ypp"
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
#line 648 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4512 "ds_parser.cpp"
    break;

  case 35:
#line 649 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4518 "ds_parser.cpp"
    break;

  case 39:
#line 662 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4526 "ds_parser.cpp"
    break;

  case 40:
#line 665 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4534 "ds_parser.cpp"
    break;

  case 41:
#line 671 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4542 "ds_parser.cpp"
    break;

  case 42:
#line 677 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4550 "ds_parser.cpp"
    break;

  case 43:
#line 680 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4558 "ds_parser.cpp"
    break;

  case 44:
#line 686 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4564 "ds_parser.cpp"
    break;

  case 45:
#line 687 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4570 "ds_parser.cpp"
    break;

  case 46:
#line 691 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4576 "ds_parser.cpp"
    break;

  case 47:
#line 692 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4582 "ds_parser.cpp"
    break;

  case 48:
#line 693 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4593 "ds_parser.cpp"
    break;

  case 49:
#line 702 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4599 "ds_parser.cpp"
    break;

  case 50:
#line 703 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4605 "ds_parser.cpp"
    break;

  case 51:
#line 707 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4616 "ds_parser.cpp"
    break;

  case 52:
#line 716 "ds_parser.ypp"
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
#line 732 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4644 "ds_parser.cpp"
    break;

  case 54:
#line 740 "ds_parser.ypp"
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
#line 750 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4667 "ds_parser.cpp"
    break;

  case 56:
#line 759 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4673 "ds_parser.cpp"
    break;

  case 57:
#line 760 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4679 "ds_parser.cpp"
    break;

  case 58:
#line 761 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4685 "ds_parser.cpp"
    break;

  case 59:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4691 "ds_parser.cpp"
    break;

  case 60:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4697 "ds_parser.cpp"
    break;

  case 61:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4703 "ds_parser.cpp"
    break;

  case 62:
#line 768 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4713 "ds_parser.cpp"
    break;

  case 63:
#line 773 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4723 "ds_parser.cpp"
    break;

  case 64:
#line 781 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4729 "ds_parser.cpp"
    break;

  case 65:
#line 782 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4735 "ds_parser.cpp"
    break;

  case 66:
#line 783 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4741 "ds_parser.cpp"
    break;

  case 67:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4747 "ds_parser.cpp"
    break;

  case 68:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4753 "ds_parser.cpp"
    break;

  case 69:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4759 "ds_parser.cpp"
    break;

  case 70:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4765 "ds_parser.cpp"
    break;

  case 71:
#line 788 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4773 "ds_parser.cpp"
    break;

  case 72:
#line 794 "ds_parser.ypp"
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
#line 807 "ds_parser.ypp"
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
#line 823 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4815 "ds_parser.cpp"
    break;

  case 75:
#line 824 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4821 "ds_parser.cpp"
    break;

  case 76:
#line 828 "ds_parser.ypp"
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
#line 835 "ds_parser.ypp"
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
#line 847 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4856 "ds_parser.cpp"
    break;

  case 79:
#line 851 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4865 "ds_parser.cpp"
    break;

  case 80:
#line 858 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4871 "ds_parser.cpp"
    break;

  case 81:
#line 859 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4877 "ds_parser.cpp"
    break;

  case 82:
#line 863 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4883 "ds_parser.cpp"
    break;

  case 83:
#line 864 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4889 "ds_parser.cpp"
    break;

  case 84:
#line 865 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4895 "ds_parser.cpp"
    break;

  case 85:
#line 869 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4905 "ds_parser.cpp"
    break;

  case 86:
#line 874 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4914 "ds_parser.cpp"
    break;

  case 87:
#line 881 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4923 "ds_parser.cpp"
    break;

  case 88:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4929 "ds_parser.cpp"
    break;

  case 89:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4935 "ds_parser.cpp"
    break;

  case 90:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4941 "ds_parser.cpp"
    break;

  case 91:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4947 "ds_parser.cpp"
    break;

  case 92:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4953 "ds_parser.cpp"
    break;

  case 93:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4959 "ds_parser.cpp"
    break;

  case 94:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4965 "ds_parser.cpp"
    break;

  case 95:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4971 "ds_parser.cpp"
    break;

  case 96:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4977 "ds_parser.cpp"
    break;

  case 97:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4983 "ds_parser.cpp"
    break;

  case 98:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4989 "ds_parser.cpp"
    break;

  case 99:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4995 "ds_parser.cpp"
    break;

  case 100:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5001 "ds_parser.cpp"
    break;

  case 101:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5007 "ds_parser.cpp"
    break;

  case 102:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5013 "ds_parser.cpp"
    break;

  case 103:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5019 "ds_parser.cpp"
    break;

  case 104:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5025 "ds_parser.cpp"
    break;

  case 105:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5031 "ds_parser.cpp"
    break;

  case 106:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5037 "ds_parser.cpp"
    break;

  case 107:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5043 "ds_parser.cpp"
    break;

  case 108:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5049 "ds_parser.cpp"
    break;

  case 109:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5055 "ds_parser.cpp"
    break;

  case 110:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5061 "ds_parser.cpp"
    break;

  case 111:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5067 "ds_parser.cpp"
    break;

  case 112:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5073 "ds_parser.cpp"
    break;

  case 113:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5079 "ds_parser.cpp"
    break;

  case 114:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5085 "ds_parser.cpp"
    break;

  case 115:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5091 "ds_parser.cpp"
    break;

  case 116:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5097 "ds_parser.cpp"
    break;

  case 117:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5103 "ds_parser.cpp"
    break;

  case 118:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5109 "ds_parser.cpp"
    break;

  case 119:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5115 "ds_parser.cpp"
    break;

  case 120:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5121 "ds_parser.cpp"
    break;

  case 121:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5127 "ds_parser.cpp"
    break;

  case 122:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5133 "ds_parser.cpp"
    break;

  case 123:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5139 "ds_parser.cpp"
    break;

  case 124:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5145 "ds_parser.cpp"
    break;

  case 125:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5151 "ds_parser.cpp"
    break;

  case 126:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5157 "ds_parser.cpp"
    break;

  case 127:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5163 "ds_parser.cpp"
    break;

  case 128:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5169 "ds_parser.cpp"
    break;

  case 129:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5175 "ds_parser.cpp"
    break;

  case 130:
#line 930 "ds_parser.ypp"
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
#line 5198 "ds_parser.cpp"
    break;

  case 131:
#line 951 "ds_parser.ypp"
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
#line 5238 "ds_parser.cpp"
    break;

  case 132:
#line 989 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5247 "ds_parser.cpp"
    break;

  case 133:
#line 996 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5256 "ds_parser.cpp"
    break;

  case 134:
#line 1000 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5269 "ds_parser.cpp"
    break;

  case 135:
#line 1011 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5275 "ds_parser.cpp"
    break;

  case 136:
#line 1012 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5281 "ds_parser.cpp"
    break;

  case 137:
#line 1013 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5287 "ds_parser.cpp"
    break;

  case 138:
#line 1014 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5293 "ds_parser.cpp"
    break;

  case 139:
#line 1015 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5299 "ds_parser.cpp"
    break;

  case 140:
#line 1016 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5305 "ds_parser.cpp"
    break;

  case 141:
#line 1017 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5311 "ds_parser.cpp"
    break;

  case 142:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5317 "ds_parser.cpp"
    break;

  case 143:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5323 "ds_parser.cpp"
    break;

  case 144:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5329 "ds_parser.cpp"
    break;

  case 145:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5335 "ds_parser.cpp"
    break;

  case 146:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5341 "ds_parser.cpp"
    break;

  case 147:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5347 "ds_parser.cpp"
    break;

  case 148:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5353 "ds_parser.cpp"
    break;

  case 149:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5359 "ds_parser.cpp"
    break;

  case 150:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5365 "ds_parser.cpp"
    break;

  case 151:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5371 "ds_parser.cpp"
    break;

  case 152:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5377 "ds_parser.cpp"
    break;

  case 153:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5383 "ds_parser.cpp"
    break;

  case 154:
#line 1033 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5393 "ds_parser.cpp"
    break;

  case 155:
#line 1038 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5404 "ds_parser.cpp"
    break;

  case 156:
#line 1044 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5412 "ds_parser.cpp"
    break;

  case 157:
#line 1050 "ds_parser.ypp"
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
#line 5438 "ds_parser.cpp"
    break;

  case 158:
#line 1071 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5446 "ds_parser.cpp"
    break;

  case 159:
#line 1074 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5454 "ds_parser.cpp"
    break;

  case 160:
#line 1080 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5460 "ds_parser.cpp"
    break;

  case 161:
#line 1081 "ds_parser.ypp"
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
#line 5476 "ds_parser.cpp"
    break;

  case 162:
#line 1092 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5482 "ds_parser.cpp"
    break;

  case 163:
#line 1096 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5490 "ds_parser.cpp"
    break;

  case 164:
#line 1102 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5498 "ds_parser.cpp"
    break;

  case 165:
#line 1105 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5506 "ds_parser.cpp"
    break;

  case 166:
#line 1108 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5515 "ds_parser.cpp"
    break;

  case 167:
#line 1112 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5523 "ds_parser.cpp"
    break;

  case 168:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5529 "ds_parser.cpp"
    break;

  case 169:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5535 "ds_parser.cpp"
    break;

  case 170:
#line 1126 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5543 "ds_parser.cpp"
    break;

  case 171:
#line 1129 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5551 "ds_parser.cpp"
    break;

  case 172:
#line 1132 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5561 "ds_parser.cpp"
    break;

  case 173:
#line 1137 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5569 "ds_parser.cpp"
    break;

  case 174:
#line 1140 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5579 "ds_parser.cpp"
    break;

  case 175:
#line 1148 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5587 "ds_parser.cpp"
    break;

  case 176:
#line 1151 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5597 "ds_parser.cpp"
    break;

  case 177:
#line 1156 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5605 "ds_parser.cpp"
    break;

  case 178:
#line 1159 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5615 "ds_parser.cpp"
    break;

  case 179:
#line 1167 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5623 "ds_parser.cpp"
    break;

  case 180:
#line 1173 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5629 "ds_parser.cpp"
    break;

  case 181:
#line 1174 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5635 "ds_parser.cpp"
    break;

  case 182:
#line 1178 "ds_parser.ypp"
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
#line 5671 "ds_parser.cpp"
    break;

  case 183:
#line 1212 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5677 "ds_parser.cpp"
    break;

  case 184:
#line 1212 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5683 "ds_parser.cpp"
    break;

  case 185:
#line 1212 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5691 "ds_parser.cpp"
    break;

  case 186:
#line 1215 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5697 "ds_parser.cpp"
    break;

  case 187:
#line 1215 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5703 "ds_parser.cpp"
    break;

  case 188:
#line 1215 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5713 "ds_parser.cpp"
    break;

  case 189:
#line 1220 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5719 "ds_parser.cpp"
    break;

  case 190:
#line 1220 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5725 "ds_parser.cpp"
    break;

  case 191:
#line 1220 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5735 "ds_parser.cpp"
    break;

  case 192:
#line 1228 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5744 "ds_parser.cpp"
    break;

  case 193:
#line 1232 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5754 "ds_parser.cpp"
    break;

  case 194:
#line 1237 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5765 "ds_parser.cpp"
    break;

  case 195:
#line 1243 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5771 "ds_parser.cpp"
    break;

  case 196:
#line 1243 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5777 "ds_parser.cpp"
    break;

  case 197:
#line 1243 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5786 "ds_parser.cpp"
    break;

  case 198:
#line 1247 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5792 "ds_parser.cpp"
    break;

  case 199:
#line 1247 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5798 "ds_parser.cpp"
    break;

  case 200:
#line 1247 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5808 "ds_parser.cpp"
    break;

  case 201:
#line 1252 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5814 "ds_parser.cpp"
    break;

  case 202:
#line 1252 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5820 "ds_parser.cpp"
    break;

  case 203:
#line 1252 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5831 "ds_parser.cpp"
    break;

  case 204:
#line 1261 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5839 "ds_parser.cpp"
    break;

  case 205:
#line 1264 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5847 "ds_parser.cpp"
    break;

  case 206:
#line 1270 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5853 "ds_parser.cpp"
    break;

  case 207:
#line 1271 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5865 "ds_parser.cpp"
    break;

  case 208:
#line 1278 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5878 "ds_parser.cpp"
    break;

  case 209:
#line 1289 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5884 "ds_parser.cpp"
    break;

  case 210:
#line 1290 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5890 "ds_parser.cpp"
    break;

  case 211:
#line 1291 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5896 "ds_parser.cpp"
    break;

  case 212:
#line 1295 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5902 "ds_parser.cpp"
    break;

  case 213:
#line 1296 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5908 "ds_parser.cpp"
    break;

  case 214:
#line 1297 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5914 "ds_parser.cpp"
    break;

  case 215:
#line 1298 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5920 "ds_parser.cpp"
    break;

  case 216:
#line 1302 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5930 "ds_parser.cpp"
    break;

  case 217:
#line 1307 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5940 "ds_parser.cpp"
    break;

  case 218:
#line 1315 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5946 "ds_parser.cpp"
    break;

  case 219:
#line 1316 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5952 "ds_parser.cpp"
    break;

  case 220:
#line 1320 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5962 "ds_parser.cpp"
    break;

  case 221:
#line 1326 "ds_parser.ypp"
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
#line 6027 "ds_parser.cpp"
    break;

  case 222:
#line 1389 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6033 "ds_parser.cpp"
    break;

  case 223:
#line 1390 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6039 "ds_parser.cpp"
    break;

  case 224:
#line 1391 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6045 "ds_parser.cpp"
    break;

  case 225:
#line 1392 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6051 "ds_parser.cpp"
    break;

  case 226:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6057 "ds_parser.cpp"
    break;

  case 227:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6063 "ds_parser.cpp"
    break;

  case 228:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6069 "ds_parser.cpp"
    break;

  case 229:
#line 1399 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6075 "ds_parser.cpp"
    break;

  case 230:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6081 "ds_parser.cpp"
    break;

  case 231:
#line 1401 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6087 "ds_parser.cpp"
    break;

  case 232:
#line 1402 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6093 "ds_parser.cpp"
    break;

  case 233:
#line 1403 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6099 "ds_parser.cpp"
    break;

  case 234:
#line 1404 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6105 "ds_parser.cpp"
    break;

  case 235:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6111 "ds_parser.cpp"
    break;

  case 236:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6117 "ds_parser.cpp"
    break;

  case 237:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6123 "ds_parser.cpp"
    break;

  case 238:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6129 "ds_parser.cpp"
    break;

  case 239:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6135 "ds_parser.cpp"
    break;

  case 240:
#line 1410 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6141 "ds_parser.cpp"
    break;

  case 241:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6147 "ds_parser.cpp"
    break;

  case 242:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6153 "ds_parser.cpp"
    break;

  case 243:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6159 "ds_parser.cpp"
    break;

  case 244:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6165 "ds_parser.cpp"
    break;

  case 245:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6171 "ds_parser.cpp"
    break;

  case 246:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6177 "ds_parser.cpp"
    break;

  case 247:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6183 "ds_parser.cpp"
    break;

  case 248:
#line 1421 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6189 "ds_parser.cpp"
    break;

  case 249:
#line 1422 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6195 "ds_parser.cpp"
    break;

  case 250:
#line 1423 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6201 "ds_parser.cpp"
    break;

  case 251:
#line 1427 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6213 "ds_parser.cpp"
    break;

  case 252:
#line 1438 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6223 "ds_parser.cpp"
    break;

  case 253:
#line 1443 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6235 "ds_parser.cpp"
    break;

  case 254:
#line 1453 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6244 "ds_parser.cpp"
    break;

  case 255:
#line 1457 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6250 "ds_parser.cpp"
    break;

  case 256:
#line 1457 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6256 "ds_parser.cpp"
    break;

  case 257:
#line 1457 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6268 "ds_parser.cpp"
    break;

  case 258:
#line 1464 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6274 "ds_parser.cpp"
    break;

  case 259:
#line 1464 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6280 "ds_parser.cpp"
    break;

  case 260:
#line 1464 "ds_parser.ypp"
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
#line 6296 "ds_parser.cpp"
    break;

  case 261:
#line 1478 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6305 "ds_parser.cpp"
    break;

  case 262:
#line 1482 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6311 "ds_parser.cpp"
    break;

  case 263:
#line 1482 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6317 "ds_parser.cpp"
    break;

  case 264:
#line 1482 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6327 "ds_parser.cpp"
    break;

  case 265:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6333 "ds_parser.cpp"
    break;

  case 266:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6339 "ds_parser.cpp"
    break;

  case 267:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6345 "ds_parser.cpp"
    break;

  case 268:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6351 "ds_parser.cpp"
    break;

  case 269:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6357 "ds_parser.cpp"
    break;

  case 270:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6363 "ds_parser.cpp"
    break;

  case 271:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6369 "ds_parser.cpp"
    break;

  case 272:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6375 "ds_parser.cpp"
    break;

  case 273:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6381 "ds_parser.cpp"
    break;

  case 274:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6387 "ds_parser.cpp"
    break;

  case 275:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6393 "ds_parser.cpp"
    break;

  case 276:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6399 "ds_parser.cpp"
    break;

  case 277:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6405 "ds_parser.cpp"
    break;

  case 278:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6411 "ds_parser.cpp"
    break;

  case 279:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6417 "ds_parser.cpp"
    break;

  case 280:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6423 "ds_parser.cpp"
    break;

  case 281:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6429 "ds_parser.cpp"
    break;

  case 282:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6435 "ds_parser.cpp"
    break;

  case 283:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6441 "ds_parser.cpp"
    break;

  case 284:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6447 "ds_parser.cpp"
    break;

  case 285:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 286:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 287:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 288:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 289:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 290:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 291:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 292:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 293:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 294:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 295:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 296:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 297:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 298:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 299:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 300:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 301:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 302:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 303:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6561 "ds_parser.cpp"
    break;

  case 304:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 305:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6573 "ds_parser.cpp"
    break;

  case 306:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6579 "ds_parser.cpp"
    break;

  case 307:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6585 "ds_parser.cpp"
    break;

  case 308:
#line 1533 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6594 "ds_parser.cpp"
    break;

  case 309:
#line 1537 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6603 "ds_parser.cpp"
    break;

  case 310:
#line 1541 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6611 "ds_parser.cpp"
    break;

  case 311:
#line 1544 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6619 "ds_parser.cpp"
    break;

  case 312:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6625 "ds_parser.cpp"
    break;

  case 313:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6631 "ds_parser.cpp"
    break;

  case 314:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6637 "ds_parser.cpp"
    break;

  case 315:
#line 1550 "ds_parser.ypp"
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
#line 1559 "ds_parser.ypp"
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
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6671 "ds_parser.cpp"
    break;

  case 318:
#line 1569 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6679 "ds_parser.cpp"
    break;

  case 319:
#line 1572 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6685 "ds_parser.cpp"
    break;

  case 320:
#line 1572 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6691 "ds_parser.cpp"
    break;

  case 321:
#line 1572 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6699 "ds_parser.cpp"
    break;

  case 322:
#line 1575 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6708 "ds_parser.cpp"
    break;

  case 323:
#line 1579 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6717 "ds_parser.cpp"
    break;

  case 324:
#line 1583 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6726 "ds_parser.cpp"
    break;

  case 325:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6732 "ds_parser.cpp"
    break;

  case 326:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6738 "ds_parser.cpp"
    break;

  case 327:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6744 "ds_parser.cpp"
    break;

  case 328:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6750 "ds_parser.cpp"
    break;

  case 329:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6756 "ds_parser.cpp"
    break;

  case 330:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6762 "ds_parser.cpp"
    break;

  case 331:
#line 1593 "ds_parser.ypp"
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
#line 1603 "ds_parser.ypp"
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
#line 1613 "ds_parser.ypp"
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
#line 1668 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6855 "ds_parser.cpp"
    break;

  case 335:
#line 1669 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6861 "ds_parser.cpp"
    break;

  case 336:
#line 1673 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6867 "ds_parser.cpp"
    break;

  case 337:
#line 1674 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6873 "ds_parser.cpp"
    break;

  case 338:
#line 1678 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6883 "ds_parser.cpp"
    break;

  case 339:
#line 1686 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6891 "ds_parser.cpp"
    break;

  case 340:
#line 1689 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6900 "ds_parser.cpp"
    break;

  case 341:
#line 1694 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6906 "ds_parser.cpp"
    break;

  case 342:
#line 1694 "ds_parser.ypp"
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
#line 1738 "ds_parser.ypp"
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
#line 7015 "ds_parser.cpp"
    break;

  case 344:
#line 1796 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7025 "ds_parser.cpp"
    break;

  case 345:
#line 1801 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7035 "ds_parser.cpp"
    break;

  case 346:
#line 1809 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7041 "ds_parser.cpp"
    break;

  case 347:
#line 1810 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7047 "ds_parser.cpp"
    break;

  case 348:
#line 1814 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7055 "ds_parser.cpp"
    break;

  case 349:
#line 1817 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7066 "ds_parser.cpp"
    break;

  case 350:
#line 1826 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7072 "ds_parser.cpp"
    break;

  case 351:
#line 1827 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7078 "ds_parser.cpp"
    break;

  case 352:
#line 1831 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7089 "ds_parser.cpp"
    break;

  case 353:
#line 1840 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7095 "ds_parser.cpp"
    break;

  case 354:
#line 1841 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7101 "ds_parser.cpp"
    break;

  case 355:
#line 1846 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7107 "ds_parser.cpp"
    break;

  case 356:
#line 1847 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7113 "ds_parser.cpp"
    break;

  case 357:
#line 1851 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7124 "ds_parser.cpp"
    break;

  case 358:
#line 1857 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7135 "ds_parser.cpp"
    break;

  case 359:
#line 1863 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7143 "ds_parser.cpp"
    break;

  case 360:
#line 1866 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7152 "ds_parser.cpp"
    break;

  case 361:
#line 1870 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7163 "ds_parser.cpp"
    break;

  case 362:
#line 1876 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7174 "ds_parser.cpp"
    break;

  case 363:
#line 1885 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7180 "ds_parser.cpp"
    break;

  case 364:
#line 1886 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7186 "ds_parser.cpp"
    break;

  case 365:
#line 1887 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7192 "ds_parser.cpp"
    break;

  case 366:
#line 1891 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7198 "ds_parser.cpp"
    break;

  case 367:
#line 1892 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7204 "ds_parser.cpp"
    break;

  case 368:
#line 1896 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7212 "ds_parser.cpp"
    break;

  case 369:
#line 1899 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7222 "ds_parser.cpp"
    break;

  case 370:
#line 1904 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7232 "ds_parser.cpp"
    break;

  case 371:
#line 1909 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7245 "ds_parser.cpp"
    break;

  case 372:
#line 1917 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7258 "ds_parser.cpp"
    break;

  case 373:
#line 1928 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7266 "ds_parser.cpp"
    break;

  case 374:
#line 1931 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7275 "ds_parser.cpp"
    break;

  case 375:
#line 1938 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7281 "ds_parser.cpp"
    break;

  case 376:
#line 1939 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7287 "ds_parser.cpp"
    break;

  case 377:
#line 1943 "ds_parser.ypp"
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
#line 7319 "ds_parser.cpp"
    break;

  case 378:
#line 1970 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7328 "ds_parser.cpp"
    break;

  case 379:
#line 1973 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7339 "ds_parser.cpp"
    break;

  case 380:
#line 1982 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7347 "ds_parser.cpp"
    break;

  case 381:
#line 1985 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7361 "ds_parser.cpp"
    break;

  case 382:
#line 1994 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7375 "ds_parser.cpp"
    break;

  case 383:
#line 2007 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7389 "ds_parser.cpp"
    break;

  case 387:
#line 2028 "ds_parser.ypp"
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
#line 7405 "ds_parser.cpp"
    break;

  case 388:
#line 2039 "ds_parser.ypp"
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
#line 7422 "ds_parser.cpp"
    break;

  case 389:
#line 2054 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7428 "ds_parser.cpp"
    break;

  case 390:
#line 2055 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7434 "ds_parser.cpp"
    break;

  case 391:
#line 2059 "ds_parser.ypp"
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
#line 7473 "ds_parser.cpp"
    break;

  case 392:
#line 2096 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7479 "ds_parser.cpp"
    break;

  case 393:
#line 2097 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7485 "ds_parser.cpp"
    break;

  case 394:
#line 2101 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7491 "ds_parser.cpp"
    break;

  case 395:
#line 2101 "ds_parser.ypp"
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
#line 7577 "ds_parser.cpp"
    break;

  case 396:
#line 2185 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7589 "ds_parser.cpp"
    break;

  case 397:
#line 2192 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7600 "ds_parser.cpp"
    break;

  case 398:
#line 2201 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7606 "ds_parser.cpp"
    break;

  case 399:
#line 2202 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7612 "ds_parser.cpp"
    break;

  case 400:
#line 2203 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7618 "ds_parser.cpp"
    break;

  case 401:
#line 2204 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7624 "ds_parser.cpp"
    break;

  case 402:
#line 2205 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7630 "ds_parser.cpp"
    break;

  case 403:
#line 2206 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7636 "ds_parser.cpp"
    break;

  case 404:
#line 2207 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7642 "ds_parser.cpp"
    break;

  case 405:
#line 2208 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7648 "ds_parser.cpp"
    break;

  case 406:
#line 2209 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7654 "ds_parser.cpp"
    break;

  case 407:
#line 2210 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7660 "ds_parser.cpp"
    break;

  case 408:
#line 2211 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7666 "ds_parser.cpp"
    break;

  case 409:
#line 2212 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7672 "ds_parser.cpp"
    break;

  case 410:
#line 2213 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7678 "ds_parser.cpp"
    break;

  case 411:
#line 2214 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7684 "ds_parser.cpp"
    break;

  case 412:
#line 2215 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7690 "ds_parser.cpp"
    break;

  case 413:
#line 2216 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7696 "ds_parser.cpp"
    break;

  case 414:
#line 2217 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7702 "ds_parser.cpp"
    break;

  case 415:
#line 2218 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7708 "ds_parser.cpp"
    break;

  case 416:
#line 2219 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7714 "ds_parser.cpp"
    break;

  case 417:
#line 2220 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7720 "ds_parser.cpp"
    break;

  case 418:
#line 2221 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7726 "ds_parser.cpp"
    break;

  case 419:
#line 2222 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7732 "ds_parser.cpp"
    break;

  case 420:
#line 2223 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7738 "ds_parser.cpp"
    break;

  case 421:
#line 2224 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7744 "ds_parser.cpp"
    break;

  case 422:
#line 2225 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7750 "ds_parser.cpp"
    break;

  case 423:
#line 2229 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7756 "ds_parser.cpp"
    break;

  case 424:
#line 2230 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7762 "ds_parser.cpp"
    break;

  case 425:
#line 2231 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7768 "ds_parser.cpp"
    break;

  case 426:
#line 2232 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7774 "ds_parser.cpp"
    break;

  case 427:
#line 2233 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7780 "ds_parser.cpp"
    break;

  case 428:
#line 2234 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7786 "ds_parser.cpp"
    break;

  case 429:
#line 2238 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7799 "ds_parser.cpp"
    break;

  case 430:
#line 2249 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7808 "ds_parser.cpp"
    break;

  case 431:
#line 2253 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7820 "ds_parser.cpp"
    break;

  case 432:
#line 2263 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7832 "ds_parser.cpp"
    break;

  case 433:
#line 2270 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7843 "ds_parser.cpp"
    break;

  case 434:
#line 2279 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7849 "ds_parser.cpp"
    break;

  case 435:
#line 2279 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7855 "ds_parser.cpp"
    break;

  case 436:
#line 2279 "ds_parser.ypp"
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
#line 7870 "ds_parser.cpp"
    break;

  case 437:
#line 2292 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7876 "ds_parser.cpp"
    break;

  case 438:
#line 2293 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7882 "ds_parser.cpp"
    break;

  case 439:
#line 2294 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7888 "ds_parser.cpp"
    break;

  case 440:
#line 2295 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7894 "ds_parser.cpp"
    break;

  case 441:
#line 2296 "ds_parser.ypp"
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
#line 7913 "ds_parser.cpp"
    break;

  case 442:
#line 2310 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7924 "ds_parser.cpp"
    break;

  case 443:
#line 2316 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7933 "ds_parser.cpp"
    break;

  case 444:
#line 2320 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7942 "ds_parser.cpp"
    break;

  case 445:
#line 2324 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7952 "ds_parser.cpp"
    break;

  case 446:
#line 2329 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7962 "ds_parser.cpp"
    break;

  case 447:
#line 2334 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7972 "ds_parser.cpp"
    break;

  case 448:
#line 2339 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7982 "ds_parser.cpp"
    break;

  case 449:
#line 2344 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7991 "ds_parser.cpp"
    break;

  case 450:
#line 2348 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8000 "ds_parser.cpp"
    break;

  case 451:
#line 2352 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8010 "ds_parser.cpp"
    break;

  case 452:
#line 2357 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8019 "ds_parser.cpp"
    break;

  case 453:
#line 2361 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8029 "ds_parser.cpp"
    break;

  case 454:
#line 2366 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8035 "ds_parser.cpp"
    break;

  case 455:
#line 2366 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8041 "ds_parser.cpp"
    break;

  case 456:
#line 2366 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8052 "ds_parser.cpp"
    break;

  case 457:
#line 2372 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8064 "ds_parser.cpp"
    break;

  case 458:
#line 2379 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8070 "ds_parser.cpp"
    break;

  case 459:
#line 2379 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8076 "ds_parser.cpp"
    break;

  case 460:
#line 2379 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8086 "ds_parser.cpp"
    break;

  case 461:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8092 "ds_parser.cpp"
    break;

  case 462:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8098 "ds_parser.cpp"
    break;

  case 463:
#line 2384 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8109 "ds_parser.cpp"
    break;

  case 464:
#line 2390 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8115 "ds_parser.cpp"
    break;

  case 465:
#line 2390 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8121 "ds_parser.cpp"
    break;

  case 466:
#line 2390 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8131 "ds_parser.cpp"
    break;

  case 467:
#line 2395 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8140 "ds_parser.cpp"
    break;

  case 468:
#line 2399 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8146 "ds_parser.cpp"
    break;

  case 469:
#line 2399 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8152 "ds_parser.cpp"
    break;

  case 470:
#line 2399 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8162 "ds_parser.cpp"
    break;

  case 471:
#line 2404 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8168 "ds_parser.cpp"
    break;

  case 472:
#line 2404 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8174 "ds_parser.cpp"
    break;

  case 473:
#line 2404 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8188 "ds_parser.cpp"
    break;

  case 474:
#line 2413 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8197 "ds_parser.cpp"
    break;

  case 475:
#line 2417 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8203 "ds_parser.cpp"
    break;

  case 476:
#line 2417 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8209 "ds_parser.cpp"
    break;

  case 477:
#line 2417 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8219 "ds_parser.cpp"
    break;

  case 478:
#line 2422 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8225 "ds_parser.cpp"
    break;

  case 479:
#line 2422 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8231 "ds_parser.cpp"
    break;

  case 480:
#line 2422 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8245 "ds_parser.cpp"
    break;

  case 481:
#line 2431 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8254 "ds_parser.cpp"
    break;

  case 482:
#line 2435 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8260 "ds_parser.cpp"
    break;

  case 483:
#line 2435 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8266 "ds_parser.cpp"
    break;

  case 484:
#line 2435 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8276 "ds_parser.cpp"
    break;

  case 485:
#line 2440 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8282 "ds_parser.cpp"
    break;

  case 486:
#line 2440 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8288 "ds_parser.cpp"
    break;

  case 487:
#line 2440 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8302 "ds_parser.cpp"
    break;

  case 488:
#line 2449 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8308 "ds_parser.cpp"
    break;

  case 489:
#line 2449 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8314 "ds_parser.cpp"
    break;

  case 490:
#line 2449 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8325 "ds_parser.cpp"
    break;

  case 491:
#line 2455 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8331 "ds_parser.cpp"
    break;

  case 492:
#line 2455 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8337 "ds_parser.cpp"
    break;

  case 493:
#line 2455 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8348 "ds_parser.cpp"
    break;

  case 494:
#line 2464 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8354 "ds_parser.cpp"
    break;

  case 495:
#line 2464 "ds_parser.ypp"
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
#line 8371 "ds_parser.cpp"
    break;

  case 496:
#line 2479 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8377 "ds_parser.cpp"
    break;

  case 497:
#line 2479 "ds_parser.ypp"
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
#line 8398 "ds_parser.cpp"
    break;

  case 498:
#line 2499 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8404 "ds_parser.cpp"
    break;

  case 499:
#line 2500 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8410 "ds_parser.cpp"
    break;

  case 500:
#line 2501 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8416 "ds_parser.cpp"
    break;

  case 501:
#line 2502 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8422 "ds_parser.cpp"
    break;

  case 502:
#line 2506 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8434 "ds_parser.cpp"
    break;

  case 503:
#line 2513 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8446 "ds_parser.cpp"
    break;

  case 504:
#line 2520 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8457 "ds_parser.cpp"
    break;

  case 505:
#line 2526 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8468 "ds_parser.cpp"
    break;

  case 506:
#line 2535 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8478 "ds_parser.cpp"
    break;

  case 507:
#line 2540 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8487 "ds_parser.cpp"
    break;

  case 508:
#line 2547 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8493 "ds_parser.cpp"
    break;

  case 509:
#line 2548 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8499 "ds_parser.cpp"
    break;

  case 510:
#line 2552 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8510 "ds_parser.cpp"
    break;

  case 511:
#line 2558 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8522 "ds_parser.cpp"
    break;

  case 512:
#line 2565 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8535 "ds_parser.cpp"
    break;

  case 513:
#line 2573 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8547 "ds_parser.cpp"
    break;

  case 514:
#line 2580 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8560 "ds_parser.cpp"
    break;

  case 515:
#line 2588 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8574 "ds_parser.cpp"
    break;

  case 516:
#line 2600 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8582 "ds_parser.cpp"
    break;

  case 517:
#line 2603 "ds_parser.ypp"
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
#line 8598 "ds_parser.cpp"
    break;

  case 518:
#line 2617 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8609 "ds_parser.cpp"
    break;

  case 519:
#line 2626 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8615 "ds_parser.cpp"
    break;

  case 520:
#line 2627 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8621 "ds_parser.cpp"
    break;

  case 521:
#line 2631 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8631 "ds_parser.cpp"
    break;

  case 522:
#line 2636 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8640 "ds_parser.cpp"
    break;

  case 523:
#line 2643 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8650 "ds_parser.cpp"
    break;

  case 524:
#line 2648 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8663 "ds_parser.cpp"
    break;

  case 525:
#line 2659 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8673 "ds_parser.cpp"
    break;

  case 526:
#line 2664 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8682 "ds_parser.cpp"
    break;

  case 527:
#line 2671 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8696 "ds_parser.cpp"
    break;

  case 528:
#line 2683 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8702 "ds_parser.cpp"
    break;

  case 529:
#line 2684 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8708 "ds_parser.cpp"
    break;

  case 530:
#line 2688 "ds_parser.ypp"
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
#line 8731 "ds_parser.cpp"
    break;

  case 531:
#line 2706 "ds_parser.ypp"
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
#line 8754 "ds_parser.cpp"
    break;


#line 8758 "ds_parser.cpp"

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
#line 2726 "ds_parser.ypp"


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
