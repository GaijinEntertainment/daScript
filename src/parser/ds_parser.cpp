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
#define YYLAST   7673

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  185
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  532
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  950

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
    1030,  1031,  1032,  1033,  1037,  1042,  1048,  1054,  1088,  1091,
    1097,  1098,  1109,  1113,  1119,  1122,  1125,  1129,  1135,  1139,
    1143,  1146,  1149,  1154,  1157,  1165,  1168,  1173,  1176,  1184,
    1190,  1191,  1195,  1229,  1229,  1229,  1232,  1232,  1232,  1237,
    1237,  1237,  1245,  1249,  1254,  1260,  1260,  1260,  1264,  1264,
    1264,  1269,  1269,  1269,  1278,  1281,  1287,  1288,  1295,  1306,
    1307,  1308,  1312,  1313,  1314,  1315,  1319,  1324,  1332,  1333,
    1337,  1342,  1406,  1407,  1408,  1409,  1410,  1411,  1415,  1416,
    1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,
    1427,  1428,  1429,  1430,  1431,  1432,  1433,  1437,  1438,  1439,
    1440,  1444,  1455,  1460,  1470,  1474,  1474,  1474,  1481,  1481,
    1481,  1495,  1499,  1499,  1499,  1506,  1507,  1508,  1509,  1510,
    1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1532,  1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,
    1541,  1542,  1543,  1544,  1545,  1546,  1547,  1548,  1549,  1553,
    1557,  1560,  1563,  1564,  1565,  1566,  1575,  1584,  1585,  1588,
    1588,  1588,  1591,  1595,  1599,  1603,  1604,  1605,  1606,  1607,
    1608,  1609,  1625,  1641,  1696,  1697,  1701,  1702,  1706,  1714,
    1717,  1722,  1721,  1766,  1822,  1831,  1836,  1844,  1845,  1849,
    1852,  1861,  1862,  1866,  1875,  1876,  1881,  1882,  1886,  1892,
    1898,  1901,  1905,  1911,  1920,  1921,  1922,  1926,  1927,  1931,
    1934,  1939,  1944,  1952,  1963,  1966,  1973,  1974,  1978,  2005,
    2005,  2033,  2036,  2045,  2058,  2070,  2071,  2075,  2079,  2090,
    2105,  2106,  2110,  2147,  2148,  2152,  2152,  2236,  2243,  2252,
    2253,  2254,  2255,  2256,  2257,  2258,  2259,  2260,  2261,  2262,
    2263,  2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,  2272,
    2273,  2274,  2275,  2276,  2280,  2281,  2282,  2283,  2284,  2285,
    2289,  2300,  2304,  2314,  2321,  2330,  2330,  2330,  2343,  2344,
    2345,  2346,  2347,  2361,  2367,  2371,  2375,  2380,  2385,  2390,
    2395,  2399,  2403,  2408,  2412,  2417,  2417,  2417,  2423,  2430,
    2430,  2430,  2435,  2435,  2435,  2441,  2441,  2441,  2446,  2450,
    2450,  2450,  2455,  2455,  2455,  2464,  2468,  2468,  2468,  2473,
    2473,  2473,  2482,  2486,  2486,  2486,  2491,  2491,  2491,  2500,
    2500,  2500,  2506,  2506,  2506,  2515,  2515,  2530,  2530,  2550,
    2551,  2552,  2553,  2557,  2564,  2571,  2577,  2586,  2591,  2598,
    2599,  2603,  2609,  2616,  2624,  2631,  2639,  2651,  2654,  2668,
    2677,  2678,  2682,  2687,  2694,  2699,  2710,  2715,  2722,  2734,
    2735,  2739,  2757
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

#define YYPACT_NINF -585

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-585)))

#define YYTABLE_NINF -487

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -585,    25,  -585,  -585,   -75,   -66,   -62,    18,  -103,  -585,
      30,  -585,  -585,    55,  -585,  -585,  -585,  -585,  -585,   350,
    -585,   105,  -585,  -585,  -585,  -585,  -585,  -585,    41,  -585,
      46,    86,   119,  -585,  -585,    13,  -585,   -39,   134,   105,
     143,   177,  -585,   184,   223,   186,  -585,   -58,  -585,  -585,
    -585,    40,   215,  -585,   187,    14,   -75,   232,   -66,   228,
    -585,   229,   230,  -585,   244,  -585,   224,  -585,   -79,  -585,
     193,   195,  -585,   238,   -75,    55,  -585,  7505,   349,   351,
    -585,   210,   207,  -585,   237,  -585,  -585,   253,  -585,  -585,
    -585,  -585,  -585,   257,   109,  -585,  -585,  -585,  -585,   356,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,   233,    43,
    7420,  -585,  -585,   266,   268,  -585,    10,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
     -84,   263,  -585,  -585,    75,  -585,   236,    45,  -585,  -585,
     199,  -585,   114,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
      74,  -585,  -585,  -100,  -585,   262,   264,   265,   267,  -585,
    -585,  -585,   247,  -585,  -585,  -585,  -585,  -585,   270,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,   271,  -585,
    -585,  -585,   272,   273,  -585,  -585,  -585,  -585,   282,   284,
    -585,  -585,  -585,  -585,  -585,   597,  -585,   274,   302,  -585,
     276,  -585,   -75,  -585,    -2,  -585,   127,  7420,  -585,  1363,
    -585,  -585,  -585,  -585,   300,  7420,  -585,   -54,  -585,  -585,
     109,  -585,    54,  4918,  -585,  -585,  -585,  -585,  -585,   314,
    -585,    64,   164,   175,  -585,  -585,  -585,  -585,  -585,  -585,
     428,  -585,  -585,    38,  3166,  -585,  -585,    63,  7420,    72,
      87,   253,  -585,   -40,  -585,   327,    52,  -585,  4918,  -585,
    -585,   253,  -585,  -585,  7336,   287,  2169,  -585,  -585,   207,
    4918,   291,  4918,   309,   311,   293,  -585,   280,   313,   334,
    3303,   207,  -585,   317,  2326,  4918,  4918,   182,   182,  7165,
    7252,  4918,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  4918,
    4918,  4918,    75,  4918,  4918,  4918,  -585,   298,  -585,   444,
    -585,  -585,   306,   307,  4918,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,   -99,   308,  -585,   310,   312,  -585,  -585,
    -585,   253,  -585,  -585,  -585,   315,  -585,  -585,   -82,  -585,
    -585,  -585,  -585,  -585,  6459,   304,  -585,  -585,  -585,  -585,
    -585,    19,  -585,  2111,  -585,  -585,  -585,  2463,  -585,  -585,
    1159,  7420,  7420,  7420,  7420,   316,   266,  7420,   210,  7420,
     210,  7420,   210,  7504,   268,  -585,  -585,  -585,   320,  -585,
    -585,  6116,  -585,  -585,    52,  -585,  -585,   323,  -585,  -585,
    -585,  7420,  -585,  2463,  -585,   453,     4,  1040,  -585,    75,
    2463,  -585,  -585,   374,  1010,   485,  6779,  4918,   453,  -585,
    -585,  4918,  -585,  -585,   346,   363,  6779,  -585,  7420,  2463,
    -585,  5007,   412,   412,   325,  -585,  -585,   253,  1704,   253,
    1859,  6557,  -585,   -56,   220,   412,   412,   -74,  -585,   412,
     412,  5571,   -48,   321,  -585,  -585,   453,  -585,  2600,  -585,
    -585,  -585,  -585,   367,   182,  -585,     6,   362,  4918,  4918,
    4918,  4918,  4918,  4918,  4918,  4918,  4918,  4918,  -585,  -585,
    4918,  4918,  4918,  4918,  4918,  4918,   366,  2755,  4918,   368,
    4918,  4918,  4918,  4918,  4918,  4918,  4918,  4918,  4918,  4918,
    4918,  4918,  4918,  4918,  2892,  3440,  4918,  4918,  4918,  4918,
    4918,  4918,  4918,  4918,  4918,  4918,   369,  4918,  3577,    55,
    -585,   500,   472,   337,   428,  -585,  2463,  -585,  5105,  -585,
     671,  1305,  1645,  1952,  -585,   -33,  2267,   263,  2401,   263,
    2538,   263,    42,  -585,    99,    52,   120,  -585,  -585,  -585,
    2697,  -585,  6360,  -585,  4918,  3732,  3029,  -585,  5203,  4918,
    4918,  -585,   207,  5648,  -585,  7420,  7420,  5727,  7420,  -585,
    -585,  2830,  -585,  5301,  -585,  -585,    16,   182,   -67,  2014,
    3887,  6557,   364,   -17,   343,   371,  -585,  -585,   117,    20,
    4042,   -17,   155,  4918,  4918,   340,  -585,  4918,   379,   384,
    -585,   181,  -585,  -585,   326,   376,  -585,    93,  6779,   -65,
     210,  -585,   373,  -585,  -585,  6779,  6779,  6779,  6779,  6779,
    6779,  6779,  6779,   322,   322,    17,  6779,  6779,    17,  7099,
    7099,   355,  4918,  4918,  6779,   152,  -585,  6195,   -26,   -26,
    6779,   322,   322,  6779,  6779,  6927,  6853,   290,  6779,  6779,
    6779,  4918,  4918,  6779,   390,  6631,  6951,   560,   780,    17,
      17,   526,   526,   152,   152,   152,  -585,   534,  6273,  -585,
      97,   121,   303,  -585,   253,  -585,  -585,  5399,  -585,  -585,
    7420,  -585,  -585,  -585,   392,  -585,   378,  -585,   380,  -585,
     382,  7420,  -585,  7504,  -585,   268,   428,  4918,   -76,  -585,
      98,   385,   394,  5804,  -585,  -585,  -585,  2967,  3104,  -585,
    3241,   367,  -585,  4918,  -585,  4918,  4918,    -9,   398,   376,
     372,   375,  4918,   377,  4918,  4918,   376,   361,   395,  6779,
    -585,  -585,  6705,  -585,   399,  7420,   210,  1546,   207,  -585,
    -585,  4918,  -585,   -46,   123,  4918,  -585,   405,   406,   408,
     410,  -585,   126,   263,  -585,  4197,  7025,  6779,  -585,  7025,
    6779,  -585,  4918,  -585,  -585,  -585,   397,  -585,    40,  -585,
    -585,  -585,  3378,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,    52,  -585,  -585,  -585,  6779,   207,  -585,  -585,
     145,  -585,  -585,  -585,  -585,   383,   -68,  6779,  6779,   -17,
     403,   -22,   364,   413,  -585,  6779,  -585,  -585,   -44,   -17,
     414,  -585,  -585,  -585,  3515,   263,  -585,  -585,   453,   388,
    6779,  -585,  -585,  -585,  -585,   -65,   416,   -80,  7420,  -585,
     108,  6779,  -585,  -585,    40,  -585,  -585,  -585,  -585,  -585,
    -585,  7420,  4352,   417,  4918,  4918,  4918,  4489,  4918,   418,
     419,  4918,  4918,  -585,  4918,   415,  -585,  -585,   427,   326,
    -585,  -585,  -585,  4644,  -585,  -585,  3670,  -585,   407,  -585,
    3825,   435,  5883,   437,    17,    17,    17,  -585,  5960,  5497,
     421,  -585,  6779,  6779,  5497,   423,    75,  -585,  -585,  4918,
    6779,  -585,  -585,  -585,  -585,  -585,  4781,  -585,   551,   425,
    -585,   420,  -585,  -585,    75,  6779,  -585,   429,  7420,   447,
    6039,  4918,   442,   443,  -585,  -585,  3980,  -585,  -585,  6779,
    -585,  -585,  -585,  7420,   446,  4135,  -585,  -585,   449,  -585
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
     334,    85,   154,   132,     0,   392,     0,     0,   397,   380,
     367,    16,     0,    60,    61,    58,    59,    57,    56,    62,
       0,    33,   381,     0,   388,     0,     0,     0,     0,   399,
     419,   400,   431,   401,   405,   406,   407,   408,   423,   412,
     413,   414,   415,   416,   417,   418,   420,   421,   468,   404,
     411,   422,   475,   482,   402,   409,   403,   410,     0,     0,
     430,   438,   441,   439,   440,     0,   433,     0,     0,   354,
       0,    77,     0,    83,     0,   347,     0,     0,   131,     0,
     391,   339,   378,   375,     0,     0,   368,     0,    17,    18,
       0,    71,     0,     0,   382,   459,   462,   465,   455,     0,
     435,   469,   476,   483,   489,   492,   446,   451,   445,   458,
       0,   454,   448,     0,     0,   384,   450,     0,     0,     0,
       0,     0,   345,   358,    84,   334,    86,   156,     0,    49,
      50,     0,   271,   272,     0,     0,     0,   265,   168,     0,
       0,     0,     0,     0,     0,     0,   169,     0,     0,     0,
       0,     0,   423,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,   224,   223,   225,   226,   227,    19,     0,
       0,     0,     0,     0,     0,     0,   209,   210,   135,   133,
     269,   268,     0,     0,     0,   149,   144,   142,   141,   143,
     220,   155,   136,   266,     0,   327,     0,     0,   147,   148,
     150,     0,   140,   326,   325,    80,   330,   267,     0,   137,
     329,   328,   307,   273,   228,     0,   270,   499,   500,   501,
     502,   334,   398,     0,   365,   366,   364,     0,    63,   389,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      82,     0,    82,     0,     0,   453,   447,   449,     0,   452,
     443,     0,   434,   498,   353,   496,   355,     0,   346,   357,
     356,     0,   359,     0,   348,     0,     0,   164,   167,     0,
       0,   170,   173,     0,   228,     0,   163,     0,     0,   183,
     186,     0,   153,   189,     0,     0,    43,    53,     0,     0,
     177,   228,   299,   300,   210,   158,   159,     0,   509,     0,
       0,     0,   526,     0,     0,   277,   276,   312,    25,   275,
     274,     0,   211,     0,   151,   152,     0,   333,     0,   139,
     145,   146,   182,   218,     0,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   301,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
     396,     0,   336,     0,   364,   369,     0,   373,   228,   383,
       0,     0,     0,     0,   432,     0,     0,    85,     0,    85,
       0,    85,   160,   351,     0,   349,     0,   444,   442,   335,
     360,   363,   362,    54,     0,     0,     0,   174,   228,     0,
       0,   171,     0,     0,    55,     0,     0,     0,     0,    41,
      42,     0,   178,   228,   175,   211,     0,     0,   160,     0,
       0,   517,   507,   509,     0,   520,   521,   522,     0,     0,
       0,   509,     0,     0,     0,     0,    22,     0,    20,     0,
     303,   255,   254,   154,    46,     0,   308,     0,   204,     0,
      82,   157,     0,   322,   323,   238,   239,   241,   240,   242,
     232,   233,   234,   278,   279,   291,   243,   244,   292,   289,
     290,     0,     0,     0,   230,   317,   306,     0,   331,   332,
     231,   280,   281,   245,   246,   296,   297,   298,   235,   236,
     237,     0,     0,   229,     0,     0,   294,   295,   293,   287,
     288,   283,   282,   284,   285,   286,   261,     0,     0,   310,
       0,     0,   336,   337,     0,   340,   371,   228,   372,   460,
       0,   466,   456,   436,     0,   470,     0,   477,     0,   484,
       0,     0,   490,     0,   493,     0,   356,     0,     0,   165,
       0,     0,     0,     0,   172,   179,   313,     0,     0,   314,
       0,   218,   176,     0,   510,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   519,
     527,   528,     0,    23,    26,     0,    82,     0,     0,    44,
      45,     0,    51,     0,     0,     0,   309,     0,     0,     0,
       0,   216,     0,    85,   319,     0,   249,   250,   305,   247,
     248,   324,     0,   263,   304,   311,    81,   341,     0,   338,
     370,   461,     0,   467,   457,   437,   471,   473,   478,   480,
     485,   487,   350,   491,   352,   494,   361,     0,   166,   195,
       0,   192,   184,   187,   190,     0,     0,   504,   503,   509,
       0,     0,   508,     0,   512,   518,   524,   523,     0,   509,
       0,   525,    21,    24,     0,    85,   134,    47,     0,     0,
     205,   214,   215,   213,   212,     0,     0,     0,     0,   252,
       0,   318,   264,   344,     0,   343,   463,   474,   481,   488,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   511,     0,     0,   515,   256,     0,    46,
     251,   217,   219,     0,   206,   221,     0,   253,     0,   464,
       0,     0,     0,     0,   185,   188,   191,   315,     0,   529,
       0,   513,   506,   505,   529,     0,     0,   259,    48,     0,
     207,   320,   342,   196,   198,   193,     0,   316,     0,     0,
     514,     0,   516,   257,     0,   208,   321,     0,     0,     0,
       0,     0,     0,     0,   260,   197,     0,   201,   194,   530,
     531,   532,   199,     0,     0,     0,   200,   202,     0,   203
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -585,  -585,  -585,   156,   566,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,   523,  -585,  -585,   567,  -585,  -585,
    -585,  -247,  -585,  -585,  -585,  -585,  -585,  -585,   386,  -585,
     577,   -57,  -585,   562,   100,  -151,  -391,  -539,  -585,  -585,
    -210,  -143,   -78,  -585,    26,  -284,   -13,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,   647,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -525,  -585,  -585,  -195,  -585,   -73,  -302,  -585,  -236,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
     477,  -585,  -585,   275,   -41,  -585,  -585,  -585,   370,  -585,
     -61,  -585,  -274,   255,  -281,  -273,   277,  -585,  -146,  -585,
     618,  -585,  -585,   479,   599,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -228,  -109,  -585,  -585,  -585,   283,  -585,
    -585,  -585,   -77,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,   391,  -582,  -446,  -584,  -585,  -585,  -311,   -71,   208,
    -585,  -585,  -585,  -222,  -585
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   172,   178,   464,   340,   754,   341,   619,
      16,    17,    35,    36,    63,    18,    32,    33,   342,   343,
     761,   762,   344,   345,   346,   347,   348,   349,   179,   180,
      29,    30,    45,    46,    47,    19,   161,   238,    81,    20,
      82,    83,   350,   351,   239,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   585,   864,   586,
     865,   588,   866,   364,   861,   927,   928,   944,   943,   948,
     627,   885,   365,   771,   772,   630,   366,   367,   433,   369,
     370,   371,   372,   755,   906,   756,   924,   373,   687,   852,
     628,   848,   926,   234,   694,   543,   381,   854,   235,   236,
     563,   564,   229,   230,   736,   282,   387,   247,   169,   167,
      54,    22,    87,   109,    67,    68,    23,    24,   165,    85,
      52,    25,   166,   170,   375,   108,   222,   223,   227,   224,
     396,   795,   565,   394,   794,   391,   791,   392,   889,   393,
     793,   397,   796,   398,   857,   399,   798,   400,   858,   401,
     800,   402,   859,   403,   803,   404,   805,    26,    41,    27,
      40,   376,   602,   603,   604,   377,   605,   606,   607,   608,
     378,   463,   379,   919,   380
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   221,   423,   368,   163,   416,   283,   557,   418,   559,
     462,   561,   432,   690,   611,   455,   456,   116,   706,   740,
     708,   243,   710,   574,   632,     2,   -80,   747,    88,    89,
     450,     3,   597,   225,    59,   733,   484,   486,   487,   745,
     597,   509,   510,   764,   281,   477,   419,   883,   767,   718,
     720,   232,   253,   283,     4,   735,     5,   768,     6,   384,
       7,    73,    48,   426,     8,    66,    60,   419,   385,    28,
     406,     9,    31,   419,    77,   765,   735,    10,   478,    38,
      43,   254,    34,   765,   266,   420,   506,   769,    42,   509,
     510,   419,   770,    75,   233,    11,    44,   220,   386,   485,
     871,   537,   162,   547,   111,   807,   420,   765,   267,   268,
     621,   244,   420,   868,   421,    64,    12,   422,    76,   496,
     497,   498,   499,   173,   174,   614,   703,   615,   221,   506,
     420,   508,   509,   510,   511,   763,   221,   874,   512,   571,
     514,   515,   168,    65,    78,    79,   577,   536,   704,   537,
     633,   240,    90,   820,   232,   244,    91,   822,    92,    93,
     286,    56,    37,    53,   739,   592,   269,   244,   383,   221,
      73,   244,   486,   487,    39,   280,    61,   531,   532,   533,
     534,   535,   631,    43,    80,   221,    62,   183,   231,   168,
     536,    94,   537,    55,   539,   407,   711,    56,   183,    44,
      13,   414,   540,    43,   270,   271,    14,   412,   816,   272,
     221,   221,   273,   408,   483,   482,   228,   427,  -472,    44,
     828,   435,   409,  -472,   220,   250,   184,   274,   242,   596,
     541,   609,   220,   447,   847,   869,   276,   389,    56,   773,
      57,  -472,   458,   460,   765,   875,   413,   175,   765,   765,
     850,   176,   251,   177,    93,   415,   498,   499,   712,   765,
     248,   249,   696,   417,   506,   220,   508,   509,   510,   511,
      58,   766,    75,   512,   738,   785,   808,   845,    66,   714,
     713,   220,   221,   221,   221,   221,   887,    70,   221,   717,
     221,   819,   221,   743,   221,   734,   878,   786,   744,   839,
     829,   715,   846,   750,   862,   284,   220,   220,   285,   102,
     486,   487,   221,   103,   550,   551,   552,   553,  -479,   468,
     556,    71,   558,  -479,   560,   536,   863,   537,    72,  -486,
     104,   105,   106,   107,  -486,  -258,   744,   758,   748,   221,
    -258,  -479,   486,   487,   570,   693,   787,   573,   759,   760,
     244,    73,  -486,   245,    49,    50,   246,    51,  -258,    84,
     584,   336,   454,    74,   162,   835,   171,   616,   617,    86,
      97,   591,    99,   100,   101,   113,   110,   114,   220,   220,
     220,   220,   115,   158,   220,   159,   220,   160,   220,   162,
     220,   164,   496,   497,   498,   499,   500,   168,   624,   503,
     504,   505,   506,   171,   508,   509,   510,   511,   220,    60,
     226,   512,   228,   514,   515,   182,   576,   237,   241,   518,
     255,   789,   256,   257,   259,   258,   498,   499,   260,   261,
     262,   263,   486,   487,   506,   220,   508,   509,   510,   511,
     264,   416,   265,   512,   382,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   277,   278,   279,   395,   622,
     405,   442,   232,   536,   429,   537,   283,   439,   437,   440,
     441,   443,   444,   486,   487,   448,   221,   221,   472,   221,
     473,   538,   531,   532,   533,   534,   535,   474,   475,   479,
      13,   480,   484,   481,   554,   536,   567,   537,   582,   569,
     589,   590,   629,   623,   725,   595,   634,   692,   727,   728,
     651,   730,   656,   686,   693,   738,   498,   499,   695,   741,
     763,   368,   742,   751,   506,   248,    48,   509,   510,   511,
     753,   774,   775,   512,   781,   783,   412,   797,   810,   799,
     872,   801,   821,   809,   830,   833,   486,   487,   823,   841,
     842,   824,   843,   826,   844,   496,   497,   498,   499,   500,
     867,   893,   503,   504,   505,   506,   880,   508,   509,   510,
     511,   831,   220,   220,   512,   220,   514,   515,   853,   870,
     486,   487,   518,   519,   520,   536,   907,   537,   912,   873,
     876,   221,   882,   914,   900,   901,   916,   920,   905,   922,
     931,   932,   221,   933,   221,   937,   525,   935,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   940,   941,
     618,    95,   181,   792,   946,    98,   536,   949,   537,   266,
     498,   499,   908,    96,   802,   162,   388,   117,   506,   691,
     508,   509,   510,   511,   888,   855,   221,   512,    21,   757,
     881,   788,   804,   267,   268,   424,   542,    69,   815,   566,
     546,   252,   496,   497,   498,   499,   500,   112,   612,   503,
     504,   505,   506,   827,   508,   509,   510,   511,   834,   555,
     837,   512,   921,   514,   515,   428,     0,   220,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,   220,   536,
     220,   537,     0,   266,     0,     0,     0,     0,     0,     0,
       0,   269,     0,     0,     0,     0,   374,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,   267,   268,   860,
     390,     0,     0,   536,     0,   537,     0,     0,     0,   221,
       0,     0,   220,     0,     0,     0,     0,     0,     0,   270,
     271,   411,   221,     0,   272,     0,     0,   273,     0,     0,
     879,     0,     0,     0,     0,   425,     0,     0,     0,   884,
       0,   886,   274,   434,     0,     0,     0,   436,   275,   438,
       0,   276,     0,     0,   890,   269,     0,   446,     0,     0,
       0,   451,   452,   453,     0,     0,     0,     0,   461,     0,
     486,   487,     0,     0,     0,     0,   465,   466,   467,     0,
     469,   470,   471,     0,     0,     0,     0,     0,     0,   221,
       0,   476,     0,   270,   271,     0,     0,     0,   272,     0,
     699,   273,     0,     0,   221,   220,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   220,     0,
       0,   936,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,   548,     0,   945,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,   499,   500,     0,     0,   503,
     504,   505,   506,   923,   508,   509,   510,   511,     0,     0,
     572,   512,     0,   514,   515,     0,     0,   578,     0,     0,
       0,   934,     0,     0,   583,   220,     0,     0,   587,     0,
       0,     0,     0,     0,     0,     0,   593,     0,     0,     0,
     220,     0,     0,     0,     0,   601,     0,   601,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   635,   636,   637,   638,   639,
     640,   641,   642,   643,   644,     0,     0,   645,   646,   647,
     648,   649,   650,     0,   654,   655,     0,   657,   658,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,   669,
     670,   673,   675,   676,   677,   678,   679,   680,   681,   682,
     683,   684,   685,     0,   688,     0,     0,     0,     0,     0,
       0,     0,     0,   697,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,     0,     0,   654,   673,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,     0,     0,   465,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
     749,   461,     0,     0,   752,     0,   267,   268,     0,     0,
       0,     0,     0,     0,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   579,   508,   509,   510,   511,     0,   776,
     777,   512,   513,   514,   515,   516,   517,     0,     0,   518,
     519,   520,   521,   522,   523,     0,     0,     0,   779,   780,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,   580,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,   486,
     487,     0,     0,   536,     0,   537,     0,     0,     0,     0,
       0,   581,   270,   271,   806,     0,     0,   272,     0,     0,
     273,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,   274,     0,   575,     0,   825,
       0,   601,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,   838,     0,
       0,     0,   840,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   496,   497,   498,   499,   500,     0,     0,   503,   504,
     505,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,     0,     0,   266,     0,   892,
     549,   894,   895,   896,   898,   899,     0,     0,   902,   903,
       0,   904,     0,     0,     0,     0,     0,     0,     0,     0,
     910,   267,   268,     0,   287,     0,     0,     0,     0,     3,
       0,   288,   289,   290,     0,   291,     0,   292,   293,   294,
     295,     0,     0,     0,     0,     0,   925,     0,   296,   297,
     298,   299,     0,   930,     0,     0,     0,     0,     0,     0,
     300,   301,     0,   302,   303,     0,     0,   304,   939,     9,
     305,   306,     0,   307,   308,     0,     0,   309,   310,   269,
       0,     0,     0,   311,   189,   190,   191,     0,   193,   194,
     195,   196,   197,   312,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   209,   210,   211,     0,     0,   214,
     215,   216,   217,     0,     0,   313,   314,   270,   271,     0,
       0,     0,   272,     0,     0,   273,     0,   315,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   317,   318,     0,     0,   700,     0,     0,   276,
       0,    43,     0,     0,     0,     0,     0,     0,   319,   320,
     321,   322,   323,   324,   325,   326,   327,    44,   328,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   329,   330,   331,     0,   332,     0,     0,
     333,   334,     0,     0,     0,     0,     0,     0,     0,     0,
     335,     0,   336,   337,   338,   162,   339,   287,     0,     0,
       0,     0,     3,     0,   288,   289,   290,     0,   291,     0,
     292,   293,   294,   295,     0,     0,     0,     0,     0,     0,
       0,   296,   297,   298,   299,     0,     0,     0,     0,     0,
       0,     0,     0,   300,   301,     0,   302,   303,     0,     0,
     304,     0,     9,   305,   306,     0,   307,   308,     0,     0,
     309,   310,     0,     0,     0,     0,   311,   189,   190,   191,
       0,   193,   194,   195,   196,   197,   312,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   209,   210,   211,
       0,     0,   214,   215,   216,   217,     0,     0,   313,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,   316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,   318,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,   266,     0,     0,
       0,   319,   320,   321,   322,   323,   324,   325,   326,   327,
      44,   328,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   267,   268,     0,     0,     0,   329,   330,   331,     0,
     332,     0,     0,   333,   334,     0,     0,     0,   292,   293,
     294,   295,     0,   335,     0,   336,   337,   338,   162,   836,
     297,     0,     0,     0,     0,     0,   266,     0,     0,     0,
       0,     0,   301,     0,     0,   303,     0,     0,   304,     0,
       0,   305,     0,   597,     0,   308,     0,     0,     0,   269,
     267,   268,     0,     0,     0,   189,   190,   191,     0,   193,
     194,   195,   196,   197,   312,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   209,   210,   211,     0,     0,
     214,   215,   216,   217,     0,     0,   313,   270,   271,     0,
       0,     0,   272,     0,   701,   273,     0,     0,   315,   316,
       0,     0,     0,     0,     0,     0,     0,     0,   269,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   598,   328,
       0,     0,     0,     0,     0,     0,   270,   271,     0,     0,
       0,   272,     0,     0,   599,   330,   331,     0,   332,     0,
       0,   333,   334,   292,   293,   294,   295,     0,     0,   274,
       0,   600,     0,   336,   337,   297,   162,     0,   276,     0,
       0,   266,     0,     0,     0,     0,     0,   301,     0,     0,
     303,     0,     0,   304,     0,     0,   305,     0,     0,     0,
     308,     0,     0,     0,     0,   267,   268,     0,     0,     0,
     189,   190,   191,     0,   193,   194,   195,   196,   197,   312,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,   316,     0,     0,     0,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   598,   328,     0,     0,     0,   267,   268,
       0,   270,   271,     0,     0,     0,   272,     0,     0,   599,
     330,   331,     0,   332,     0,     0,   333,   334,   292,   293,
     294,   295,     0,     0,   274,     0,   610,     0,   336,   337,
     297,   162,     0,   276,     0,     0,   406,     0,     0,     0,
       0,     0,   301,     0,     0,   303,     0,     0,   304,     0,
       0,   305,     0,     0,     0,   308,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,     0,   193,
     194,   195,   196,   197,   312,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   209,   210,   211,     0,     0,
     214,   215,   216,   217,   270,   271,   313,     0,     0,   272,
       0,   702,   273,     0,     0,     0,     0,     0,   315,   316,
       0,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,    43,   266,     0,     0,     0,     0,     0,   319,
     320,   321,   322,   323,   324,   325,   326,   327,    44,   328,
       0,     0,     0,     0,     0,     0,     0,   267,   268,     0,
       0,   407,     0,     0,   329,   330,   331,     0,   332,     0,
       0,   333,   334,   292,   293,   294,   295,     0,     0,   408,
       0,   335,     0,   336,   337,   297,   162,     0,   409,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
     303,     0,     0,   304,     0,     0,   305,     0,     0,     0,
     308,     0,     0,     0,   384,   269,     0,     0,     0,     0,
     189,   190,   191,   385,   193,   194,   195,   196,   197,   312,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,     0,
       0,   313,     0,   544,   271,     0,     0,     0,   272,     0,
       0,   273,     0,   315,   316,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   274,     0,   317,   318,
       0,     0,   545,     0,     0,   276,     0,    43,     0,   266,
       0,     0,     0,     0,   319,   320,   321,   322,   323,   324,
     325,   326,   327,    44,   328,     0,     0,     0,     0,     0,
       0,     0,     0,   267,   268,     0,     0,     0,     0,   329,
     330,   331,     0,   332,     0,     0,   333,   334,     0,     0,
     292,   293,   294,   295,     0,     0,   335,     0,   336,   337,
     431,   162,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,   303,     0,     0,
     304,     0,     0,   305,     0,     0,     0,   308,     0,     0,
       0,   269,     0,     0,     0,     0,     0,   189,   190,   191,
       0,   193,   194,   195,   196,   197,   312,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   209,   210,   211,
       0,     0,   214,   215,   216,   217,     0,     0,   313,   270,
     271,     0,     0,     0,   272,     0,   705,   273,     0,     0,
     315,   316,     0,   266,     0,     0,     0,     0,     0,   449,
       0,     0,   274,     0,     0,   317,   318,     0,     0,     0,
       0,   276,     0,     0,    43,     0,     0,   267,   268,     0,
       0,   319,   320,   321,   322,   323,   324,   325,   326,   327,
      44,   328,     0,     0,     0,     0,     0,   292,   293,   294,
     295,     0,     0,     0,     0,     0,   329,   330,   331,   297,
     332,     0,     0,   333,   334,     0,     0,     0,     0,     0,
       0,   301,     0,   335,   303,   336,   337,   304,   162,     0,
     305,     0,     0,     0,   308,   269,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,     0,   193,   194,
     195,   196,   197,   312,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   209,   210,   211,     0,     0,   214,
     215,   216,   217,   270,   271,   313,     0,     0,   272,     0,
     707,   273,     0,     0,     0,     0,     0,   315,   316,     0,
     266,     0,     0,     0,     0,     0,   274,     0,     0,     0,
       0,     0,   317,   318,     0,   276,     0,     0,     0,     0,
       0,    43,     0,     0,   267,   268,     0,     0,   319,   320,
     321,   322,   323,   324,   325,   326,   327,    44,   328,     0,
       0,     0,     0,     0,   292,   293,   294,   295,     0,     0,
       0,     0,     0,   329,   330,   331,   297,   332,     0,     0,
     333,   334,     0,     0,     0,     0,     0,     0,   301,     0,
     335,   303,   336,   337,   304,   162,     0,   305,     0,     0,
       0,   308,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,     0,   193,   194,   195,   196,   197,
     312,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   209,   210,   211,     0,     0,   214,   215,   216,   217,
     270,   271,   313,     0,     0,   272,     0,   709,   273,     0,
       0,     0,     0,     0,   315,   316,     0,     0,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,    43,   266,
       0,     0,     0,     0,     0,   319,   320,   321,   322,   323,
     324,   325,   326,   327,    44,   328,     0,     0,     0,     0,
       0,     0,     0,   267,   268,     0,     0,     0,     0,     0,
     329,   330,   331,     0,   332,     0,     0,   333,   334,   292,
     293,   294,   295,     0,     0,   625,     0,   335,   626,   336,
     337,   297,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,   303,     0,     0,   304,
       0,     0,   305,     0,     0,     0,   308,     0,     0,     0,
     419,   269,     0,     0,     0,     0,   189,   190,   191,     0,
     193,   194,   195,   196,   197,   312,   199,   200,   201,   202,
     203,   204,   205,   206,   207,     0,   209,   210,   211,     0,
       0,   214,   215,   216,   217,     0,     0,   313,     0,   716,
     271,     0,     0,     0,   272,     0,     0,   273,     0,   315,
     316,     0,   266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,   652,   653,     0,     0,     0,     0,
       0,   276,     0,    43,     0,     0,   267,   268,     0,     0,
     319,   320,   321,   322,   323,   324,   325,   326,   327,    44,
     328,     0,     0,     0,     0,     0,   292,   293,   294,   295,
       0,     0,     0,     0,     0,   329,   330,   331,   297,   332,
       0,     0,   333,   334,     0,     0,     0,     0,     0,     0,
     301,     0,   335,   303,   336,   337,   304,   162,     0,   305,
       0,     0,     0,   308,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   312,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,   270,   271,   313,     0,     0,   272,     0,   731,
     273,     0,     0,     0,     0,     0,   315,   316,     0,   266,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,   671,   672,     0,   276,     0,     0,     0,     0,     0,
      43,     0,     0,   267,   268,     0,     0,   319,   320,   321,
     322,   323,   324,   325,   326,   327,    44,   328,     0,     0,
       0,     0,     0,   292,   293,   294,   295,   721,     0,     0,
       0,     0,   329,   330,   331,   297,   332,     0,     0,   333,
     334,     0,     0,     0,     0,     0,     0,   301,     0,   335,
     303,   336,   337,   304,   162,     0,   305,     0,     0,     0,
     308,   269,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,     0,   193,   194,   195,   196,   197,   312,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,   270,
     271,   313,     0,     0,   272,     0,   812,   273,     0,     0,
       0,     0,     0,   315,   316,     0,   266,     0,     0,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,    43,     0,     0,
     267,   268,     0,     0,   319,   320,   321,   322,   323,   324,
     325,   326,   327,    44,   328,     0,     0,     0,     0,     0,
     292,   293,   294,   295,     0,     0,     0,   722,     0,   329,
     330,   331,   297,   332,     0,     0,   333,   334,     0,     0,
       0,     0,     0,     0,   301,     0,   335,   303,   336,   337,
     304,   162,     0,   305,     0,     0,     0,   308,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   190,   191,
       0,   193,   194,   195,   196,   197,   312,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   209,   210,   211,
       0,     0,   214,   215,   216,   217,   270,   271,   313,     0,
       0,   272,     0,   813,   273,     0,     0,     0,     0,     0,
     315,   316,     0,   266,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,    43,     0,     0,   267,   268,     0,
       0,   319,   320,   321,   322,   323,   324,   325,   326,   327,
      44,   328,     0,     0,     0,     0,     0,   292,   293,   294,
     295,     0,     0,     0,     0,     0,   329,   330,   331,   297,
     332,     0,     0,   333,   334,     0,     0,     0,     0,     0,
       0,   301,   410,   335,   303,   336,   337,   304,   162,     0,
     305,     0,     0,     0,   308,   269,     0,   445,     0,     0,
       0,     0,     0,     0,   189,   190,   191,     0,   193,   194,
     195,   196,   197,   312,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   209,   210,   211,     0,     0,   214,
     215,   216,   217,   270,   271,   313,     0,     0,   272,     0,
     814,   273,     0,     0,     0,     0,     0,   315,   316,     0,
     266,     0,     0,     0,     0,     0,   274,     0,     0,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,    43,     0,     0,   267,   268,     0,     0,   319,   320,
     321,   322,   323,   324,   325,   326,   327,    44,   328,     0,
       0,     0,     0,     0,   292,   293,   294,   295,     0,     0,
       0,   674,     0,   329,   330,   331,   297,   332,     0,     0,
     333,   334,     0,     0,     0,     0,     0,     0,   301,     0,
     335,   303,   336,   337,   304,   162,     0,   305,     0,     0,
       0,   308,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,     0,   193,   194,   195,   196,   197,
     312,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   209,   210,   211,     0,     0,   214,   215,   216,   217,
     270,   271,   313,     0,     0,   272,     0,   856,   273,     0,
       0,     0,     0,     0,   315,   316,     0,   266,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,    43,     0,
       0,   267,   268,     0,     0,   319,   320,   321,   322,   323,
     324,   325,   326,   327,    44,   328,     0,     0,     0,     0,
       0,   292,   293,   294,   295,     0,     0,     0,     0,     0,
     329,   330,   331,   297,   332,     0,     0,   333,   334,     0,
       0,     0,     0,     0,     0,   301,     0,   335,   303,   336,
     337,   304,   162,     0,   305,     0,     0,     0,   308,   269,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,     0,   193,   194,   195,   196,   197,   312,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   209,   210,
     211,     0,     0,   214,   215,   216,   217,   270,   271,   313,
       0,     0,   272,     0,   877,   273,     0,     0,     0,     0,
       0,   315,   316,     0,     0,     0,     0,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,   266,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   319,   320,   321,   322,   323,   324,   325,   326,
     327,    44,   328,     0,     0,     0,   267,   268,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   329,   330,   331,
       0,   332,     0,     0,   333,   334,   292,   293,   294,   295,
       0,     0,     0,     0,   335,   689,   336,   337,   297,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,   303,     0,     0,   304,     0,     0,   305,
       0,     0,     0,   308,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   312,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,   270,   271,   313,     0,     0,   272,     0,   911,
     273,     0,     0,     0,     0,     0,   315,   316,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,   266,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   319,   320,   321,
     322,   323,   324,   325,   326,   327,    44,   328,     0,     0,
       0,   267,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   329,   330,   331,     0,   332,     0,     0,   333,
     334,   292,   293,   294,   295,     0,     0,     0,     0,   335,
     719,   336,   337,   297,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,     0,     0,   303,     0,
       0,   304,     0,     0,   305,     0,     0,     0,   308,   269,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,     0,   193,   194,   195,   196,   197,   312,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   209,   210,
     211,     0,     0,   214,   215,   216,   217,   270,   271,   313,
       0,     0,   272,     0,   913,   273,     0,     0,     0,     0,
       0,   315,   316,     0,     0,     0,     0,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,   266,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   319,   320,   321,   322,   323,   324,   325,   326,
     327,    44,   328,     0,     0,     0,   267,   268,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   329,   330,   331,
       0,   332,     0,     0,   333,   334,   292,   293,   294,   295,
       0,     0,     0,     0,   335,   737,   336,   337,   297,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,   303,     0,     0,   304,     0,     0,   305,
       0,     0,     0,   308,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   312,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,   270,   271,   313,     0,     0,   272,     0,   942,
     273,     0,     0,     0,     0,     0,   315,   316,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,   266,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   319,   320,   321,
     322,   323,   324,   325,   326,   327,    44,   328,     0,     0,
       0,   267,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   329,   330,   331,     0,   332,     0,     0,   333,
     334,   292,   293,   294,   295,     0,     0,     0,     0,   335,
     746,   336,   337,   297,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,     0,     0,   303,     0,
       0,   304,     0,     0,   305,     0,     0,     0,   308,   269,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,     0,   193,   194,   195,   196,   197,   312,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   209,   210,
     211,     0,     0,   214,   215,   216,   217,   270,   271,   313,
       0,     0,   272,     0,   947,   273,     0,     0,     0,     0,
       0,   315,   316,     0,     0,     0,     0,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   319,   320,   321,   322,   323,   324,   325,   326,
     327,    44,   328,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   329,   330,   331,
       0,   332,     0,     0,   333,   334,   292,   293,   294,   295,
     891,     0,     0,     0,   335,   849,   336,   337,   297,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,   303,     0,     0,   304,     0,     0,   305,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   312,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,     0,     0,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   319,   320,   321,
     322,   323,   324,   325,   326,   327,    44,   328,     0,     0,
       0,     0,     0,   292,   293,   294,   295,     0,     0,     0,
       0,     0,   329,   330,   331,   297,   332,     0,     0,   333,
     334,     0,     0,     0,     0,     0,     0,   301,     0,   335,
     303,   336,   337,   304,   162,     0,   305,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,     0,   193,   194,   195,   196,   197,   312,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,   316,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   319,   320,   321,   322,   323,   324,
     325,   326,   327,    44,   328,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   329,
     330,   331,     0,   332,     0,     0,   333,   334,   292,   293,
     294,   295,     0,     0,     0,     0,   335,   897,   336,   337,
     297,   162,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,     0,     0,   303,     0,     0,   304,     0,
       0,   305,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,     0,   193,
     194,   195,   196,   197,   312,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   209,   210,   211,     0,     0,
     214,   215,   216,   217,     0,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,   316,
       0,     0,     0,     0,     0,     0,     0,   909,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   319,
     320,   321,   322,   323,   324,   325,   326,   327,    44,   328,
       0,     0,     0,     0,     0,   292,   293,   294,   295,   929,
       0,     0,     0,     0,   329,   330,   331,   297,   332,     0,
       0,   333,   334,     0,     0,     0,     0,     0,     0,   301,
       0,   335,   303,   336,   337,   304,   162,     0,   305,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,   190,   191,     0,   193,   194,   195,   196,
     197,   312,   199,   200,   201,   202,   203,   204,   205,   206,
     207,     0,   209,   210,   211,     0,     0,   214,   215,   216,
     217,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,   316,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,     0,     0,   319,   320,   321,   322,
     323,   324,   325,   326,   327,    44,   328,     0,     0,     0,
       0,     0,   292,   293,   294,   295,     0,     0,     0,     0,
       0,   329,   330,   331,   297,   332,     0,     0,   333,   334,
       0,     0,     0,     0,     0,     0,   301,     0,   335,   303,
     336,   337,   304,   162,     0,   305,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
     190,   191,     0,   193,   194,   195,   196,   197,   312,   199,
     200,   201,   202,   203,   204,   205,   206,   207,     0,   209,
     210,   211,     0,     0,   214,   215,   216,   217,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,   316,     0,     0,     0,   486,   487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,   319,   320,   321,   322,   323,   324,   325,
     326,   327,    44,   328,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   329,   330,
     331,     0,   332,     0,     0,   333,   334,     0,     0,     0,
       0,     0,     0,     0,     0,   335,     0,   336,   337,     0,
     162,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     579,   508,   509,   510,   511,   486,   487,     0,   512,   513,
     514,   515,   516,   517,     0,     0,   518,   519,   520,   521,
     522,   523,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
     525,     0,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,     0,     0,     0,     0,     0,     0,     0,     0,
     536,     0,   537,     0,     0,     0,     0,     0,   594,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   579,   508,
     509,   510,   511,   486,   487,     0,   512,   513,   514,   515,
     516,   517,     0,     0,   518,   519,   520,   521,   522,   523,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,   525,     0,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,     0,     0,     0,     0,     0,   698,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   579,   508,   509,   510,
     511,   486,   487,     0,   512,   513,   514,   515,   516,   517,
       0,     0,   518,   519,   520,   521,   522,   523,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,   525,     0,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,     0,
       0,     0,     0,     0,   724,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   579,   508,   509,   510,   511,   486,
     487,     0,   512,   513,   514,   515,   516,   517,     0,     0,
     518,   519,   520,   521,   522,   523,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,   525,     0,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,   536,     0,   537,     0,     0,     0,
       0,     0,   732,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   579,   508,   509,   510,   511,   486,   487,     0,
     512,   513,   514,   515,   516,   517,     0,     0,   518,   519,
     520,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,     0,     0,     0,     0,     0,
     790,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,     0,     0,     0,   512,     0,
     514,   515,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,     0,     0,     0,     0,     0,     0,   486,   487,
     536,     0,   537,   496,   497,   498,   499,   500,   918,     0,
     503,   504,   505,   506,     0,   508,   509,   510,   511,     0,
       0,     0,   512,     0,   514,   515,     0,     0,     0,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,   536,     0,   537,   486,   487,   620,
     496,   497,   498,   499,   500,     0,     0,   503,   504,   505,
     506,     0,   508,   509,   510,   511,     0,     0,     0,   512,
       0,   514,   515,     0,     0,     0,     0,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,     0,     0,     0,     0,     0,     0,     0,
       0,   536,     0,   537,   486,   487,   726,     0,     0,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,     0,     0,     0,   512,     0,
     514,   515,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,     0,     0,     0,     0,     0,     0,     0,     0,
     536,     0,   537,   486,   487,   729,   496,   497,   498,   499,
     500,     0,     0,   503,   504,   505,   506,     0,   508,   509,
     510,   511,     0,     0,     0,   512,     0,   514,   515,     0,
       0,     0,     0,   518,   519,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,   536,     0,   537,
     486,   487,   811,     0,     0,   496,   497,   498,   499,   500,
       0,     0,   503,   504,   505,   506,     0,   508,   509,   510,
     511,     0,     0,     0,   512,     0,   514,   515,     0,     0,
       0,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,   486,
     487,   915,   496,   497,   498,   499,   500,     0,     0,   503,
     504,   505,   506,     0,   508,   509,   510,   511,     0,     0,
       0,   512,     0,   514,   515,     0,     0,     0,     0,   518,
     519,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,   486,   487,   917,     0,
       0,   496,   497,   498,   499,   500,     0,     0,   503,   504,
     505,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,   486,   487,   938,   496,   497,
     498,   499,   500,     0,     0,   503,   504,   505,   506,     0,
     508,   509,   510,   511,     0,     0,     0,   512,     0,   514,
     515,     0,     0,     0,     0,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,   536,
       0,   537,   568,   486,   487,     0,     0,   496,   497,   498,
     499,   500,     0,     0,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,     0,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,   778,     0,     0,     0,   496,   497,   498,   499,   500,
     486,   487,   503,   504,   505,   506,     0,   508,   509,   510,
     511,     0,     0,     0,   512,     0,   514,   515,     0,     0,
       0,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,     0,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,   784,
       0,     0,     0,     0,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   579,   508,   509,   510,   511,  -228,   486,
     487,   512,   513,   514,   515,   516,   517,     0,     0,   518,
     519,   520,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   486,   487,     0,
     512,   513,   514,   515,   516,   517,     0,     0,   518,   519,
     520,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   524,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,     0,     0,     0,   512,     0,
     514,   515,     0,     0,   613,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,     0,     0,     0,   486,   487,     0,     0,     0,
     536,     0,   537,   496,   497,   498,   499,   500,     0,     0,
     503,   504,   505,   506,     0,   508,   509,   510,   511,     0,
       0,     0,   512,     0,   514,   515,     0,     0,     0,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   782,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,     0,     0,     0,   486,
     487,     0,     0,     0,   536,     0,   537,   496,   497,   498,
     499,   500,     0,     0,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   832,     0,     0,     0,   525,     0,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
       0,     0,     0,   486,   487,     0,     0,     0,   536,     0,
     537,   496,   497,   498,   499,   500,     0,     0,   503,   504,
     505,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,     0,     0,     0,   486,   487,     0,
       0,     0,   536,     0,   537,   496,   497,   498,   499,   500,
       0,     0,   503,   504,   505,   506,     0,   508,   509,   510,
     511,   486,   487,     0,   512,     0,   514,   515,     0,     0,
       0,     0,   518,     0,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,   486,   487,     0,   512,     0,
     514,   515,     0,   496,   497,   498,   499,   500,     0,     0,
     503,   504,   505,   506,     0,   508,   509,   510,   511,     0,
       0,     0,   512,     0,   514,   515,     0,     0,     0,     0,
       0,     0,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,     0,     0,     0,     0,     0,     0,     0,     0,
     536,     0,   537,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,   534,   535,     0,     0,     0,   486,
     487,     0,     0,     0,   536,     0,   537,   496,   497,     0,
       0,   500,     0,     0,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,   525,     0,
     526,   527,   528,   529,   530,     0,     0,     0,   534,   185,
       0,     0,     0,     0,     0,   186,     0,     0,   536,     0,
     537,   496,   497,   498,   499,   500,     0,     0,   503,     0,
     187,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,   188,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   529,   530,   531,
     532,   533,   534,   535,   459,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,     0,   185,     0,     0,     0,
       0,     0,   186,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,   188,     0,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,     0,     0,     0,     0,     0,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   187,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   188,    44,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,     0,     0,     0,     0,     0,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,   187,     0,     0,     0,     0,
       0,   319,   320,   321,     0,     0,     0,     0,     0,   188,
      44,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
       0,     0,     0,     0,   186,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   188,    44,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,     0,     0,     0,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   136,   137,
     138,   139,    43,     0,   140,   141,   142,   143,   144,   145,
       0,     0,     0,     0,     0,     0,     0,     0,   562,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,   156,   157
};

static const yytype_int16 yycheck[] =
{
      13,   110,   283,   239,    82,   279,   234,   398,   281,   400,
     321,   402,   296,   538,   460,   317,   318,    74,   557,   603,
     559,   167,   561,    19,    18,     0,     7,   611,    14,    15,
     314,     6,    49,   110,    21,    19,   118,    20,    21,    19,
      49,   115,   116,   625,    46,   144,   113,   127,   113,   574,
     575,   135,   152,   281,    29,   122,    31,   122,    33,   113,
      35,   128,    75,   291,    39,   144,    53,   113,   122,   144,
      32,    46,   138,   113,    34,   151,   122,    52,   177,   182,
     128,   181,   144,   151,    32,   152,   112,   152,    33,   115,
     116,   113,   157,   151,   178,    70,   144,   110,   152,   181,
     122,   175,   182,   387,   183,   181,   152,   151,    56,    57,
     158,   151,   152,   181,   154,   154,    91,   157,   176,   102,
     103,   104,   105,    14,    15,   181,   159,   183,   237,   112,
     152,   114,   115,   116,   117,   144,   245,   181,   121,   423,
     123,   124,   144,   182,   104,   105,   430,   173,   181,   175,
     144,   164,   138,   737,   135,   151,   142,   739,   144,   145,
     237,   151,   144,    58,   181,   449,   114,   151,   245,   278,
     128,   151,    20,    21,   144,   232,   163,   160,   161,   162,
     163,   164,   484,   128,   144,   294,   173,   144,   178,   144,
     173,   177,   175,   152,   175,   157,   154,   151,   144,   144,
     175,   278,   183,   128,   152,   153,   181,   144,   733,   157,
     319,   320,   160,   175,   365,   361,   144,   294,   154,   144,
     745,   299,   184,   159,   237,   151,   183,   175,   183,   457,
     381,   459,   245,   311,   773,   819,   184,   183,   151,   630,
     154,   177,   319,   320,   151,   829,   183,   138,   151,   151,
     775,   142,   178,   144,   145,   183,   104,   105,   159,   151,
     146,   147,   546,   176,   112,   278,   114,   115,   116,   117,
     151,   178,   151,   121,   151,   178,   178,   151,   144,   159,
     181,   294,   391,   392,   393,   394,   178,   144,   397,   570,
     399,   737,   401,   176,   403,   597,   835,   176,   181,   176,
     746,   181,   176,   614,   159,   178,   319,   320,   181,    65,
      20,    21,   421,    69,   391,   392,   393,   394,   154,   332,
     397,   144,   399,   159,   401,   173,   181,   175,   144,   154,
      86,    87,    88,    89,   159,   154,   181,    11,   183,   448,
     159,   177,    20,    21,   421,    42,    43,   425,    22,    23,
     151,   128,   177,   154,     4,     5,   157,     7,   177,   144,
     438,   179,   180,   177,   182,   756,   146,   147,   148,   182,
     138,   448,   144,   144,   144,   182,   152,   182,   391,   392,
     393,   394,   144,    34,   397,    34,   399,   177,   401,   182,
     403,   154,   102,   103,   104,   105,   106,   144,   476,   109,
     110,   111,   112,   146,   114,   115,   116,   117,   421,    53,
     144,   121,   144,   123,   124,   182,   429,   154,   182,   129,
     158,   694,   158,   158,   177,   158,   104,   105,   158,   158,
     158,   158,    20,    21,   112,   448,   114,   115,   116,   117,
     158,   715,   158,   121,   144,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   181,   154,   181,   144,   472,
      32,   181,   135,   173,   177,   175,   694,   158,   177,   158,
     177,   158,   138,    20,    21,   158,   585,   586,   180,   588,
      36,   177,   160,   161,   162,   163,   164,   181,   181,   181,
     175,   181,   118,   181,   178,   173,   176,   175,    13,   176,
     154,   138,   135,   182,   582,   180,   144,     7,   585,   586,
     144,   588,   144,   144,    42,   151,   104,   105,   181,   176,
     144,   757,   151,   183,   112,   146,   539,   115,   116,   117,
     146,   158,   177,   121,   144,     1,   144,   159,   144,   159,
     821,   159,   144,   158,   183,   146,    20,    21,   176,   144,
     144,   176,   144,   176,   144,   102,   103,   104,   105,   106,
     177,   144,   109,   110,   111,   112,   178,   114,   115,   116,
     117,   176,   585,   586,   121,   588,   123,   124,   181,   176,
      20,    21,   129,   130,   131,   173,   159,   175,   181,   176,
     176,   700,   176,   158,   176,   176,   159,   176,   183,   176,
      49,   176,   711,   183,   713,   158,   153,   178,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   176,   176,
     464,    55,    99,   700,   178,    58,   173,   178,   175,    32,
     104,   105,   879,    56,   711,   182,   250,    75,   112,   539,
     114,   115,   116,   117,   854,   788,   755,   121,     1,   623,
     845,   692,   713,    56,    57,   285,   381,    39,   731,   404,
     383,   182,   102,   103,   104,   105,   106,    68,   460,   109,
     110,   111,   112,   744,   114,   115,   116,   117,   755,   396,
     758,   121,   904,   123,   124,   294,    -1,   700,   162,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   711,   173,
     713,   175,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   239,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,    56,    57,   807,
     253,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,   848,
      -1,    -1,   755,    -1,    -1,    -1,    -1,    -1,    -1,   152,
     153,   274,   861,    -1,   157,    -1,    -1,   160,    -1,    -1,
     838,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,   847,
      -1,   848,   175,   296,    -1,    -1,    -1,   300,   181,   302,
      -1,   184,    -1,    -1,   861,   114,    -1,   310,    -1,    -1,
      -1,   314,   315,   316,    -1,    -1,    -1,    -1,   321,    -1,
      20,    21,    -1,    -1,    -1,    -1,   329,   330,   331,    -1,
     333,   334,   335,    -1,    -1,    -1,    -1,    -1,    -1,   928,
      -1,   344,    -1,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,   160,    -1,    -1,   943,   848,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   861,    -1,
      -1,   928,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,    -1,   943,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,   906,   114,   115,   116,   117,    -1,    -1,
     423,   121,    -1,   123,   124,    -1,    -1,   430,    -1,    -1,
      -1,   924,    -1,    -1,   437,   928,    -1,    -1,   441,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   449,    -1,    -1,    -1,
     943,    -1,    -1,    -1,    -1,   458,    -1,   460,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,    -1,    -1,   500,   501,   502,
     503,   504,   505,    -1,   507,   508,    -1,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,    -1,   537,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   546,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   576,    -1,    -1,   579,   580,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   599,   600,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   610,    -1,    -1,
     613,   614,    -1,    -1,   617,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   652,
     653,   121,   122,   123,   124,   125,   126,    -1,    -1,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,   671,   672,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,   181,   152,   153,   717,    -1,    -1,   157,    -1,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   735,   736,    -1,   175,    -1,   177,    -1,   742,
      -1,   744,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   757,    -1,    -1,    -1,   761,    -1,
      -1,    -1,   765,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   782,
      -1,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    32,    -1,   862,
     181,   864,   865,   866,   867,   868,    -1,    -1,   871,   872,
      -1,   874,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     883,    56,    57,    -1,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,   909,    -1,    25,    26,
      27,    28,    -1,   916,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    41,    -1,    -1,    44,   931,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    54,    55,   114,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    93,   152,   153,    -1,
      -1,    -1,   157,    -1,    -1,   160,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   119,   120,    -1,    -1,   181,    -1,    -1,   184,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,   183,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    32,    -1,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,   177,    -1,   179,   180,   181,   182,   183,
      26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    49,    -1,    51,    -1,    -1,    -1,   114,
      56,    57,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,   160,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,   152,   153,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,    14,    15,    16,    17,    -1,    -1,   175,
      -1,   177,    -1,   179,   180,    26,   182,    -1,   184,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    56,    57,
      -1,   152,   153,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,    14,    15,
      16,    17,    -1,    -1,   175,    -1,   177,    -1,   179,   180,
      26,   182,    -1,   184,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,   152,   153,    92,    -1,    -1,   157,
      -1,   159,   160,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,
      -1,    -1,   128,    32,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,    14,    15,    16,    17,    -1,    -1,   175,
      -1,   177,    -1,   179,   180,    26,   182,    -1,   184,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      61,    62,    63,   122,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,   152,   153,    -1,    -1,    -1,   157,    -1,
      -1,   160,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,   175,    -1,   119,   120,
      -1,    -1,   181,    -1,    -1,   184,    -1,   128,    -1,    32,
      -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,   177,    -1,   179,   180,
     181,   182,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,   152,
     153,    -1,    -1,    -1,   157,    -1,   159,   160,    -1,    -1,
     104,   105,    -1,    32,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,   175,    -1,    -1,   119,   120,    -1,    -1,    -1,
      -1,   184,    -1,    -1,   128,    -1,    -1,    56,    57,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,    26,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   177,    41,   179,   180,    44,   182,    -1,
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
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,   128,    32,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,    14,
      15,    16,    17,    -1,    -1,   175,    -1,   177,   178,   179,
     180,    26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,   152,
     153,    -1,    -1,    -1,   157,    -1,    -1,   160,    -1,   104,
     105,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   128,    -1,    -1,    56,    57,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    -1,   160,   161,   162,    26,   164,
      -1,    -1,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   177,    41,   179,   180,    44,   182,    -1,    47,
      -1,    -1,    -1,    51,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   152,   153,    92,    -1,    -1,   157,    -1,   159,
     160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,   119,   120,    -1,   184,    -1,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    56,    57,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   160,   161,   162,    26,   164,    -1,    -1,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   177,
      41,   179,   180,    44,   182,    -1,    47,    -1,    -1,    -1,
      51,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,   152,
     153,    92,    -1,    -1,   157,    -1,   159,   160,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,
      56,    57,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,   158,    -1,   160,
     161,   162,    26,   164,    -1,    -1,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   177,    41,   179,   180,
      44,   182,    -1,    47,    -1,    -1,    -1,    51,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,   152,   153,    92,    -1,
      -1,   157,    -1,   159,   160,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    32,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    56,    57,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,    26,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    38,   176,   177,    41,   179,   180,    44,   182,    -1,
      47,    -1,    -1,    -1,    51,   114,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,   152,   153,    92,    -1,    -1,   157,    -1,
     159,   160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    56,    57,    -1,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    21,    -1,   160,   161,   162,    26,   164,    -1,    -1,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     177,    41,   179,   180,    44,   182,    -1,    47,    -1,    -1,
      -1,    51,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
     152,   153,    92,    -1,    -1,   157,    -1,   159,   160,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    56,    57,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    26,   164,    -1,    -1,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   177,    41,   179,
     180,    44,   182,    -1,    47,    -1,    -1,    -1,    51,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,   152,   153,    92,
      -1,    -1,   157,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,    32,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,    26,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   152,   153,    92,    -1,    -1,   157,    -1,   159,
     160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    32,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,    26,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,   152,   153,    92,
      -1,    -1,   157,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,    32,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,    26,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   152,   153,    92,    -1,    -1,   157,    -1,   159,
     160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    32,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,    26,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,   152,   153,    92,
      -1,    -1,   157,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   177,   178,   179,   180,    26,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
      26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   160,   161,   162,    26,   164,    -1,
      -1,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   177,    41,   179,   180,    44,   182,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   160,   161,   162,    26,   164,    -1,    -1,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   177,    41,
     179,   180,    44,   182,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,
     162,    -1,   164,    -1,    -1,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,    -1,
     182,    94,    95,    96,    97,    98,    99,   100,   101,   102,
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
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     173,    -1,   175,   102,   103,   104,   105,   106,   181,    -1,
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
      -1,   175,   176,    20,    21,    -1,    -1,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
     175,   176,    -1,    -1,    -1,   102,   103,   104,   105,   106,
      20,    21,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,   176,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    20,
      21,   121,   122,   123,   124,   125,   126,    -1,    -1,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    20,    21,    -1,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,   127,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     173,    -1,   175,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   173,    -1,   175,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   173,    -1,
     175,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   173,    -1,   175,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    20,    21,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    20,    21,    -1,   121,    -1,
     123,   124,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   173,    -1,   175,   102,   103,    -1,
      -1,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,   153,    -1,
     155,   156,   157,   158,   159,    -1,    -1,    -1,   163,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,   173,    -1,
     175,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      45,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   158,   159,   160,
     161,   162,   163,   164,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,   144,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    45,    -1,    -1,    -1,    -1,
      -1,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    59,
     144,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,   144,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   128,    -1,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168
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
     177,   221,   182,   227,   154,   303,   307,   294,   144,   293,
     308,   146,   188,    14,    15,   138,   142,   144,   189,   213,
     214,   199,   182,   144,   183,    24,    30,    45,    59,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     231,   309,   311,   312,   314,   317,   144,   313,   144,   287,
     288,   178,   135,   178,   278,   283,   284,   154,   222,   229,
     231,   182,   183,   293,   151,   154,   157,   292,   146,   147,
     151,   178,   298,   152,   181,   158,   158,   158,   158,   177,
     158,   158,   158,   158,   158,   158,    32,    56,    57,   114,
     152,   153,   157,   160,   175,   181,   184,   181,   154,   181,
     216,    46,   290,   308,   178,   181,   317,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    25,    26,    27,    28,
      37,    38,    40,    41,    44,    47,    48,    50,    51,    54,
      55,    60,    70,    92,    93,   104,   105,   119,   120,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   145,   160,
     161,   162,   164,   167,   168,   177,   179,   180,   181,   183,
     191,   193,   203,   204,   207,   208,   209,   210,   211,   212,
     227,   228,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   248,   257,   261,   262,   263,   264,
     265,   266,   267,   272,   275,   309,   346,   350,   355,   357,
     359,   281,   144,   317,   113,   122,   152,   291,   213,   183,
     275,   320,   322,   324,   318,   144,   315,   326,   328,   330,
     332,   334,   336,   338,   340,    32,    32,   157,   175,   184,
     176,   275,   144,   183,   317,   183,   287,   176,   290,   113,
     152,   154,   157,   289,   283,   275,   308,   317,   346,   177,
     113,   181,   230,   263,   275,   227,   275,   177,   275,   158,
     158,   177,   181,   158,   138,    54,   275,   227,   158,   113,
     230,   275,   275,   275,   180,   261,   261,    12,   317,    12,
     317,   275,   352,   356,   190,   275,   275,   275,   231,   275,
     275,   275,   180,    36,   181,   181,   275,   144,   177,   181,
     181,   181,   293,   220,   118,   181,    20,    21,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   121,   122,   123,   124,   125,   126,   129,   130,
     131,   132,   133,   134,   152,   153,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   173,   175,   177,   175,
     183,   220,   278,   280,   152,   181,   291,   230,   275,   181,
     317,   317,   317,   317,   178,   313,   317,   221,   317,   221,
     317,   221,   144,   285,   286,   317,   288,   176,   176,   176,
     317,   230,   275,   227,    19,   177,   231,   230,   275,   113,
     152,   181,    13,   275,   227,   242,   244,   275,   246,   154,
     138,   317,   230,   275,   181,   180,   308,    49,   144,   160,
     177,   275,   347,   348,   349,   351,   352,   353,   354,   308,
     177,   348,   354,   127,   181,   183,   147,   148,   188,   194,
     178,   158,   231,   182,   227,   175,   178,   255,   275,   135,
     260,   261,    18,   144,   144,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   144,   119,   120,   275,   275,   144,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   119,   120,   275,    21,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   144,   273,   275,   178,
     255,   219,     7,    42,   279,   181,   230,   275,   181,   159,
     181,   159,   159,   159,   181,   159,   222,   159,   222,   159,
     222,   154,   159,   181,   159,   181,   152,   289,   255,   178,
     255,    18,   158,   275,   181,   227,   178,   317,   317,   178,
     317,   159,   181,    19,   261,   122,   289,   178,   151,   181,
     349,   176,   151,   176,   181,    19,   178,   349,   183,   275,
     352,   183,   275,   146,   192,   268,   270,   229,    11,    22,
      23,   205,   206,   144,   347,   151,   178,   113,   122,   152,
     157,   258,   259,   221,   158,   177,   275,   275,   176,   275,
     275,   144,   154,     1,   176,   178,   176,    43,   279,   290,
     181,   321,   317,   325,   319,   316,   327,   159,   331,   159,
     335,   159,   317,   339,   285,   341,   275,   181,   178,   158,
     144,   178,   159,   159,   159,   260,   255,   275,   275,   348,
     349,   144,   347,   176,   176,   275,   176,   353,   255,   348,
     183,   176,   149,   146,   317,   221,   183,   227,   275,   176,
     275,   144,   144,   144,   144,   151,   176,   222,   276,   178,
     255,   275,   274,   181,   282,   226,   159,   329,   333,   337,
     227,   249,   159,   181,   243,   245,   247,   177,   181,   349,
     176,   122,   289,   176,   181,   349,   176,   159,   222,   227,
     178,   258,   176,   127,   227,   256,   317,   178,   225,   323,
     317,    18,   275,   144,   275,   275,   275,   178,   275,   275,
     176,   176,   275,   275,   275,   183,   269,   159,   206,   113,
     275,   159,   181,   159,   158,   178,   159,   178,   181,   358,
     176,   358,   176,   231,   271,   275,   277,   250,   251,    18,
     275,    49,   176,   183,   231,   178,   317,   158,   178,   275,
     176,   176,   159,   253,   252,   317,   178,   159,   254,   178
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
       4,     0,     3,     5,     4,     1,     2,     4,     5,     7,
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
#line 3440 "ds_parser.cpp"
        break;

    case 188: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3446 "ds_parser.cpp"
        break;

    case 189: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3452 "ds_parser.cpp"
        break;

    case 190: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3458 "ds_parser.cpp"
        break;

    case 191: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3464 "ds_parser.cpp"
        break;

    case 193: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3470 "ds_parser.cpp"
        break;

    case 197: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3476 "ds_parser.cpp"
        break;

    case 203: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3482 "ds_parser.cpp"
        break;

    case 204: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3488 "ds_parser.cpp"
        break;

    case 206: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3494 "ds_parser.cpp"
        break;

    case 208: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3500 "ds_parser.cpp"
        break;

    case 209: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3506 "ds_parser.cpp"
        break;

    case 210: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3512 "ds_parser.cpp"
        break;

    case 211: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3518 "ds_parser.cpp"
        break;

    case 212: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3524 "ds_parser.cpp"
        break;

    case 213: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3530 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3536 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3542 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3548 "ds_parser.cpp"
        break;

    case 217: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3554 "ds_parser.cpp"
        break;

    case 218: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3560 "ds_parser.cpp"
        break;

    case 219: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3566 "ds_parser.cpp"
        break;

    case 220: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3572 "ds_parser.cpp"
        break;

    case 221: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3578 "ds_parser.cpp"
        break;

    case 222: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3584 "ds_parser.cpp"
        break;

    case 223: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3590 "ds_parser.cpp"
        break;

    case 227: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3596 "ds_parser.cpp"
        break;

    case 228: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3602 "ds_parser.cpp"
        break;

    case 229: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3608 "ds_parser.cpp"
        break;

    case 230: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3614 "ds_parser.cpp"
        break;

    case 231: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3620 "ds_parser.cpp"
        break;

    case 232: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3626 "ds_parser.cpp"
        break;

    case 233: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3632 "ds_parser.cpp"
        break;

    case 234: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3638 "ds_parser.cpp"
        break;

    case 235: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3644 "ds_parser.cpp"
        break;

    case 236: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3650 "ds_parser.cpp"
        break;

    case 237: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3656 "ds_parser.cpp"
        break;

    case 238: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3662 "ds_parser.cpp"
        break;

    case 240: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3668 "ds_parser.cpp"
        break;

    case 241: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3674 "ds_parser.cpp"
        break;

    case 248: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3680 "ds_parser.cpp"
        break;

    case 255: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3686 "ds_parser.cpp"
        break;

    case 256: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3692 "ds_parser.cpp"
        break;

    case 258: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3698 "ds_parser.cpp"
        break;

    case 259: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3704 "ds_parser.cpp"
        break;

    case 260: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3710 "ds_parser.cpp"
        break;

    case 261: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3716 "ds_parser.cpp"
        break;

    case 262: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3722 "ds_parser.cpp"
        break;

    case 263: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3728 "ds_parser.cpp"
        break;

    case 264: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3734 "ds_parser.cpp"
        break;

    case 265: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3740 "ds_parser.cpp"
        break;

    case 266: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3746 "ds_parser.cpp"
        break;

    case 267: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3752 "ds_parser.cpp"
        break;

    case 272: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3758 "ds_parser.cpp"
        break;

    case 275: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3764 "ds_parser.cpp"
        break;

    case 278: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3770 "ds_parser.cpp"
        break;

    case 280: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3776 "ds_parser.cpp"
        break;

    case 281: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3782 "ds_parser.cpp"
        break;

    case 283: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3788 "ds_parser.cpp"
        break;

    case 284: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3794 "ds_parser.cpp"
        break;

    case 285: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3800 "ds_parser.cpp"
        break;

    case 286: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3806 "ds_parser.cpp"
        break;

    case 287: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3812 "ds_parser.cpp"
        break;

    case 288: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3818 "ds_parser.cpp"
        break;

    case 290: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3824 "ds_parser.cpp"
        break;

    case 293: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3830 "ds_parser.cpp"
        break;

    case 294: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3836 "ds_parser.cpp"
        break;

    case 298: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3842 "ds_parser.cpp"
        break;

    case 303: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3848 "ds_parser.cpp"
        break;

    case 308: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3854 "ds_parser.cpp"
        break;

    case 311: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3860 "ds_parser.cpp"
        break;

    case 312: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3866 "ds_parser.cpp"
        break;

    case 313: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3872 "ds_parser.cpp"
        break;

    case 314: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3878 "ds_parser.cpp"
        break;

    case 317: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3884 "ds_parser.cpp"
        break;

    case 346: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3890 "ds_parser.cpp"
        break;

    case 347: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3896 "ds_parser.cpp"
        break;

    case 348: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3902 "ds_parser.cpp"
        break;

    case 349: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3908 "ds_parser.cpp"
        break;

    case 350: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3914 "ds_parser.cpp"
        break;

    case 351: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3920 "ds_parser.cpp"
        break;

    case 352: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3926 "ds_parser.cpp"
        break;

    case 353: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3932 "ds_parser.cpp"
        break;

    case 354: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3938 "ds_parser.cpp"
        break;

    case 355: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3944 "ds_parser.cpp"
        break;

    case 356: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3950 "ds_parser.cpp"
        break;

    case 357: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3956 "ds_parser.cpp"
        break;

    case 358: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3962 "ds_parser.cpp"
        break;

    case 359: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3968 "ds_parser.cpp"
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
#line 4271 "ds_parser.cpp"
    break;

  case 16:
#line 473 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4277 "ds_parser.cpp"
    break;

  case 17:
#line 474 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4283 "ds_parser.cpp"
    break;

  case 18:
#line 478 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4289 "ds_parser.cpp"
    break;

  case 19:
#line 482 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4299 "ds_parser.cpp"
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
#line 4313 "ds_parser.cpp"
    break;

  case 21:
#line 496 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4323 "ds_parser.cpp"
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
#line 4341 "ds_parser.cpp"
    break;

  case 23:
#line 520 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4351 "ds_parser.cpp"
    break;

  case 24:
#line 525 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4361 "ds_parser.cpp"
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
#line 4388 "ds_parser.cpp"
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
#line 4400 "ds_parser.cpp"
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
#line 4424 "ds_parser.cpp"
    break;

  case 29:
#line 589 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4432 "ds_parser.cpp"
    break;

  case 30:
#line 592 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4443 "ds_parser.cpp"
    break;

  case 31:
#line 598 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4454 "ds_parser.cpp"
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
#line 4480 "ds_parser.cpp"
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
#line 4505 "ds_parser.cpp"
    break;

  case 34:
#line 651 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4511 "ds_parser.cpp"
    break;

  case 35:
#line 652 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4517 "ds_parser.cpp"
    break;

  case 39:
#line 665 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4525 "ds_parser.cpp"
    break;

  case 40:
#line 668 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4533 "ds_parser.cpp"
    break;

  case 41:
#line 674 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4541 "ds_parser.cpp"
    break;

  case 42:
#line 680 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4549 "ds_parser.cpp"
    break;

  case 43:
#line 683 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4557 "ds_parser.cpp"
    break;

  case 44:
#line 689 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4563 "ds_parser.cpp"
    break;

  case 45:
#line 690 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4569 "ds_parser.cpp"
    break;

  case 46:
#line 694 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4575 "ds_parser.cpp"
    break;

  case 47:
#line 695 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4581 "ds_parser.cpp"
    break;

  case 48:
#line 696 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4592 "ds_parser.cpp"
    break;

  case 49:
#line 705 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4598 "ds_parser.cpp"
    break;

  case 50:
#line 706 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4604 "ds_parser.cpp"
    break;

  case 51:
#line 710 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4615 "ds_parser.cpp"
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
#line 4633 "ds_parser.cpp"
    break;

  case 53:
#line 735 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4643 "ds_parser.cpp"
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
#line 4655 "ds_parser.cpp"
    break;

  case 55:
#line 753 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4666 "ds_parser.cpp"
    break;

  case 56:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4672 "ds_parser.cpp"
    break;

  case 57:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4678 "ds_parser.cpp"
    break;

  case 58:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4684 "ds_parser.cpp"
    break;

  case 59:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4690 "ds_parser.cpp"
    break;

  case 60:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4696 "ds_parser.cpp"
    break;

  case 61:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4702 "ds_parser.cpp"
    break;

  case 62:
#line 771 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4712 "ds_parser.cpp"
    break;

  case 63:
#line 776 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4722 "ds_parser.cpp"
    break;

  case 64:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4728 "ds_parser.cpp"
    break;

  case 65:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4734 "ds_parser.cpp"
    break;

  case 66:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4740 "ds_parser.cpp"
    break;

  case 67:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4746 "ds_parser.cpp"
    break;

  case 68:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4752 "ds_parser.cpp"
    break;

  case 69:
#line 789 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4758 "ds_parser.cpp"
    break;

  case 70:
#line 790 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4764 "ds_parser.cpp"
    break;

  case 71:
#line 791 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4772 "ds_parser.cpp"
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
#line 4790 "ds_parser.cpp"
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
#line 4808 "ds_parser.cpp"
    break;

  case 74:
#line 826 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4814 "ds_parser.cpp"
    break;

  case 75:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4820 "ds_parser.cpp"
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
#line 4832 "ds_parser.cpp"
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
#line 4846 "ds_parser.cpp"
    break;

  case 78:
#line 850 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4855 "ds_parser.cpp"
    break;

  case 79:
#line 854 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4864 "ds_parser.cpp"
    break;

  case 80:
#line 861 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4870 "ds_parser.cpp"
    break;

  case 81:
#line 862 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4876 "ds_parser.cpp"
    break;

  case 82:
#line 866 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4882 "ds_parser.cpp"
    break;

  case 83:
#line 867 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4888 "ds_parser.cpp"
    break;

  case 84:
#line 868 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4894 "ds_parser.cpp"
    break;

  case 85:
#line 872 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4904 "ds_parser.cpp"
    break;

  case 86:
#line 877 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4913 "ds_parser.cpp"
    break;

  case 87:
#line 884 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4922 "ds_parser.cpp"
    break;

  case 88:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4928 "ds_parser.cpp"
    break;

  case 89:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4934 "ds_parser.cpp"
    break;

  case 90:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4940 "ds_parser.cpp"
    break;

  case 91:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4946 "ds_parser.cpp"
    break;

  case 92:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4952 "ds_parser.cpp"
    break;

  case 93:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4958 "ds_parser.cpp"
    break;

  case 94:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4964 "ds_parser.cpp"
    break;

  case 95:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4970 "ds_parser.cpp"
    break;

  case 96:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4976 "ds_parser.cpp"
    break;

  case 97:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4982 "ds_parser.cpp"
    break;

  case 98:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4988 "ds_parser.cpp"
    break;

  case 99:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4994 "ds_parser.cpp"
    break;

  case 100:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5000 "ds_parser.cpp"
    break;

  case 101:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5006 "ds_parser.cpp"
    break;

  case 102:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5012 "ds_parser.cpp"
    break;

  case 103:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5018 "ds_parser.cpp"
    break;

  case 104:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5024 "ds_parser.cpp"
    break;

  case 105:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5030 "ds_parser.cpp"
    break;

  case 106:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5036 "ds_parser.cpp"
    break;

  case 107:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5042 "ds_parser.cpp"
    break;

  case 108:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5048 "ds_parser.cpp"
    break;

  case 109:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5054 "ds_parser.cpp"
    break;

  case 110:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5060 "ds_parser.cpp"
    break;

  case 111:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5066 "ds_parser.cpp"
    break;

  case 112:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5072 "ds_parser.cpp"
    break;

  case 113:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5078 "ds_parser.cpp"
    break;

  case 114:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5084 "ds_parser.cpp"
    break;

  case 115:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5090 "ds_parser.cpp"
    break;

  case 116:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5096 "ds_parser.cpp"
    break;

  case 117:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5102 "ds_parser.cpp"
    break;

  case 118:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5108 "ds_parser.cpp"
    break;

  case 119:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5114 "ds_parser.cpp"
    break;

  case 120:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5120 "ds_parser.cpp"
    break;

  case 121:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5126 "ds_parser.cpp"
    break;

  case 122:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5132 "ds_parser.cpp"
    break;

  case 123:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5138 "ds_parser.cpp"
    break;

  case 124:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5144 "ds_parser.cpp"
    break;

  case 125:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5150 "ds_parser.cpp"
    break;

  case 126:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5156 "ds_parser.cpp"
    break;

  case 127:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5162 "ds_parser.cpp"
    break;

  case 128:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5168 "ds_parser.cpp"
    break;

  case 129:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5174 "ds_parser.cpp"
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
#line 5198 "ds_parser.cpp"
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
#line 5238 "ds_parser.cpp"
    break;

  case 132:
#line 993 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5247 "ds_parser.cpp"
    break;

  case 133:
#line 1000 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5256 "ds_parser.cpp"
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
#line 5269 "ds_parser.cpp"
    break;

  case 135:
#line 1015 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5275 "ds_parser.cpp"
    break;

  case 136:
#line 1016 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5281 "ds_parser.cpp"
    break;

  case 137:
#line 1017 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5287 "ds_parser.cpp"
    break;

  case 138:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5293 "ds_parser.cpp"
    break;

  case 139:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5299 "ds_parser.cpp"
    break;

  case 140:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5305 "ds_parser.cpp"
    break;

  case 141:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5311 "ds_parser.cpp"
    break;

  case 142:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5317 "ds_parser.cpp"
    break;

  case 143:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5323 "ds_parser.cpp"
    break;

  case 144:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5329 "ds_parser.cpp"
    break;

  case 145:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5335 "ds_parser.cpp"
    break;

  case 146:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5341 "ds_parser.cpp"
    break;

  case 147:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5347 "ds_parser.cpp"
    break;

  case 148:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5353 "ds_parser.cpp"
    break;

  case 149:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5359 "ds_parser.cpp"
    break;

  case 150:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5365 "ds_parser.cpp"
    break;

  case 151:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5371 "ds_parser.cpp"
    break;

  case 152:
#line 1032 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5377 "ds_parser.cpp"
    break;

  case 153:
#line 1033 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5383 "ds_parser.cpp"
    break;

  case 154:
#line 1037 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5393 "ds_parser.cpp"
    break;

  case 155:
#line 1042 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5404 "ds_parser.cpp"
    break;

  case 156:
#line 1048 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5412 "ds_parser.cpp"
    break;

  case 157:
#line 1054 "ds_parser.ypp"
    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( pipeCall->rtti_isVar() ) {
            // a += b <| c
            auto pVar = (ExprVar *) pipeCall;
            auto pCall = g_Program->makeCall(pVar->at,pVar->name);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            if ( !(yyvsp[-2].pExpression)->swap_tail(pVar,pCall) ) {
                delete pVar;
                (yyval.pExpression) = pCall;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        } else if ( pipeCall->rtti_isMakeStruct() ) {
            auto pMS = (ExprMakeStruct *) pipeCall;
            if ( pMS->block ) {
                das_yyerror("can't pipe into [[ make structure ]]. it already has where closure",
                    tokAt((yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into function call or [[ make structure ]]",
                tokAt((yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 5451 "ds_parser.cpp"
    break;

  case 158:
#line 1088 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5459 "ds_parser.cpp"
    break;

  case 159:
#line 1091 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5467 "ds_parser.cpp"
    break;

  case 160:
#line 1097 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5473 "ds_parser.cpp"
    break;

  case 161:
#line 1098 "ds_parser.ypp"
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
#line 5489 "ds_parser.cpp"
    break;

  case 162:
#line 1109 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5495 "ds_parser.cpp"
    break;

  case 163:
#line 1113 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5503 "ds_parser.cpp"
    break;

  case 164:
#line 1119 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5511 "ds_parser.cpp"
    break;

  case 165:
#line 1122 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5519 "ds_parser.cpp"
    break;

  case 166:
#line 1125 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5528 "ds_parser.cpp"
    break;

  case 167:
#line 1129 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5536 "ds_parser.cpp"
    break;

  case 168:
#line 1135 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5542 "ds_parser.cpp"
    break;

  case 169:
#line 1139 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5548 "ds_parser.cpp"
    break;

  case 170:
#line 1143 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5556 "ds_parser.cpp"
    break;

  case 171:
#line 1146 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5564 "ds_parser.cpp"
    break;

  case 172:
#line 1149 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5574 "ds_parser.cpp"
    break;

  case 173:
#line 1154 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5582 "ds_parser.cpp"
    break;

  case 174:
#line 1157 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5592 "ds_parser.cpp"
    break;

  case 175:
#line 1165 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5600 "ds_parser.cpp"
    break;

  case 176:
#line 1168 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5610 "ds_parser.cpp"
    break;

  case 177:
#line 1173 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5618 "ds_parser.cpp"
    break;

  case 178:
#line 1176 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5628 "ds_parser.cpp"
    break;

  case 179:
#line 1184 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5636 "ds_parser.cpp"
    break;

  case 180:
#line 1190 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5642 "ds_parser.cpp"
    break;

  case 181:
#line 1191 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5648 "ds_parser.cpp"
    break;

  case 182:
#line 1195 "ds_parser.ypp"
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
#line 5684 "ds_parser.cpp"
    break;

  case 183:
#line 1229 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5690 "ds_parser.cpp"
    break;

  case 184:
#line 1229 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5696 "ds_parser.cpp"
    break;

  case 185:
#line 1229 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5704 "ds_parser.cpp"
    break;

  case 186:
#line 1232 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5710 "ds_parser.cpp"
    break;

  case 187:
#line 1232 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5716 "ds_parser.cpp"
    break;

  case 188:
#line 1232 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5726 "ds_parser.cpp"
    break;

  case 189:
#line 1237 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5732 "ds_parser.cpp"
    break;

  case 190:
#line 1237 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5738 "ds_parser.cpp"
    break;

  case 191:
#line 1237 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5748 "ds_parser.cpp"
    break;

  case 192:
#line 1245 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5757 "ds_parser.cpp"
    break;

  case 193:
#line 1249 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5767 "ds_parser.cpp"
    break;

  case 194:
#line 1254 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5778 "ds_parser.cpp"
    break;

  case 195:
#line 1260 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5784 "ds_parser.cpp"
    break;

  case 196:
#line 1260 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5790 "ds_parser.cpp"
    break;

  case 197:
#line 1260 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5799 "ds_parser.cpp"
    break;

  case 198:
#line 1264 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5805 "ds_parser.cpp"
    break;

  case 199:
#line 1264 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5811 "ds_parser.cpp"
    break;

  case 200:
#line 1264 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5821 "ds_parser.cpp"
    break;

  case 201:
#line 1269 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5827 "ds_parser.cpp"
    break;

  case 202:
#line 1269 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5833 "ds_parser.cpp"
    break;

  case 203:
#line 1269 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5844 "ds_parser.cpp"
    break;

  case 204:
#line 1278 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5852 "ds_parser.cpp"
    break;

  case 205:
#line 1281 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5860 "ds_parser.cpp"
    break;

  case 206:
#line 1287 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5866 "ds_parser.cpp"
    break;

  case 207:
#line 1288 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5878 "ds_parser.cpp"
    break;

  case 208:
#line 1295 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5891 "ds_parser.cpp"
    break;

  case 209:
#line 1306 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5897 "ds_parser.cpp"
    break;

  case 210:
#line 1307 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5903 "ds_parser.cpp"
    break;

  case 211:
#line 1308 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5909 "ds_parser.cpp"
    break;

  case 212:
#line 1312 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5915 "ds_parser.cpp"
    break;

  case 213:
#line 1313 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5921 "ds_parser.cpp"
    break;

  case 214:
#line 1314 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5927 "ds_parser.cpp"
    break;

  case 215:
#line 1315 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5933 "ds_parser.cpp"
    break;

  case 216:
#line 1319 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5943 "ds_parser.cpp"
    break;

  case 217:
#line 1324 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5953 "ds_parser.cpp"
    break;

  case 218:
#line 1332 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5959 "ds_parser.cpp"
    break;

  case 219:
#line 1333 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5965 "ds_parser.cpp"
    break;

  case 220:
#line 1337 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5975 "ds_parser.cpp"
    break;

  case 221:
#line 1343 "ds_parser.ypp"
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
#line 6040 "ds_parser.cpp"
    break;

  case 222:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6046 "ds_parser.cpp"
    break;

  case 223:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6052 "ds_parser.cpp"
    break;

  case 224:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6058 "ds_parser.cpp"
    break;

  case 225:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6064 "ds_parser.cpp"
    break;

  case 226:
#line 1410 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6070 "ds_parser.cpp"
    break;

  case 227:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6076 "ds_parser.cpp"
    break;

  case 228:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6082 "ds_parser.cpp"
    break;

  case 229:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6088 "ds_parser.cpp"
    break;

  case 230:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6094 "ds_parser.cpp"
    break;

  case 231:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6100 "ds_parser.cpp"
    break;

  case 232:
#line 1419 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6106 "ds_parser.cpp"
    break;

  case 233:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6112 "ds_parser.cpp"
    break;

  case 234:
#line 1421 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6118 "ds_parser.cpp"
    break;

  case 235:
#line 1422 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6124 "ds_parser.cpp"
    break;

  case 236:
#line 1423 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6130 "ds_parser.cpp"
    break;

  case 237:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6136 "ds_parser.cpp"
    break;

  case 238:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6142 "ds_parser.cpp"
    break;

  case 239:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6148 "ds_parser.cpp"
    break;

  case 240:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6154 "ds_parser.cpp"
    break;

  case 241:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6160 "ds_parser.cpp"
    break;

  case 242:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6166 "ds_parser.cpp"
    break;

  case 243:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6172 "ds_parser.cpp"
    break;

  case 244:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6178 "ds_parser.cpp"
    break;

  case 245:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6184 "ds_parser.cpp"
    break;

  case 246:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6190 "ds_parser.cpp"
    break;

  case 247:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6196 "ds_parser.cpp"
    break;

  case 248:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6202 "ds_parser.cpp"
    break;

  case 249:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6208 "ds_parser.cpp"
    break;

  case 250:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6214 "ds_parser.cpp"
    break;

  case 251:
#line 1444 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6226 "ds_parser.cpp"
    break;

  case 252:
#line 1455 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6236 "ds_parser.cpp"
    break;

  case 253:
#line 1460 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6248 "ds_parser.cpp"
    break;

  case 254:
#line 1470 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6257 "ds_parser.cpp"
    break;

  case 255:
#line 1474 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6263 "ds_parser.cpp"
    break;

  case 256:
#line 1474 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6269 "ds_parser.cpp"
    break;

  case 257:
#line 1474 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6281 "ds_parser.cpp"
    break;

  case 258:
#line 1481 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6287 "ds_parser.cpp"
    break;

  case 259:
#line 1481 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6293 "ds_parser.cpp"
    break;

  case 260:
#line 1481 "ds_parser.ypp"
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
#line 6309 "ds_parser.cpp"
    break;

  case 261:
#line 1495 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6318 "ds_parser.cpp"
    break;

  case 262:
#line 1499 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6324 "ds_parser.cpp"
    break;

  case 263:
#line 1499 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6330 "ds_parser.cpp"
    break;

  case 264:
#line 1499 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6339 "ds_parser.cpp"
    break;

  case 265:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6345 "ds_parser.cpp"
    break;

  case 266:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6351 "ds_parser.cpp"
    break;

  case 267:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6357 "ds_parser.cpp"
    break;

  case 268:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6363 "ds_parser.cpp"
    break;

  case 269:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6369 "ds_parser.cpp"
    break;

  case 270:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6375 "ds_parser.cpp"
    break;

  case 271:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6381 "ds_parser.cpp"
    break;

  case 272:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6387 "ds_parser.cpp"
    break;

  case 273:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6393 "ds_parser.cpp"
    break;

  case 274:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6399 "ds_parser.cpp"
    break;

  case 275:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6405 "ds_parser.cpp"
    break;

  case 276:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6411 "ds_parser.cpp"
    break;

  case 277:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6417 "ds_parser.cpp"
    break;

  case 278:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6423 "ds_parser.cpp"
    break;

  case 279:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6429 "ds_parser.cpp"
    break;

  case 280:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6435 "ds_parser.cpp"
    break;

  case 281:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6441 "ds_parser.cpp"
    break;

  case 282:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6447 "ds_parser.cpp"
    break;

  case 283:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 284:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 285:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 286:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 287:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 288:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 289:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 290:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 291:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 292:
#line 1533 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 293:
#line 1534 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 294:
#line 1535 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 295:
#line 1536 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 296:
#line 1537 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 297:
#line 1538 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 298:
#line 1539 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 299:
#line 1540 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 300:
#line 1541 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 301:
#line 1542 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 302:
#line 1543 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 303:
#line 1544 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6573 "ds_parser.cpp"
    break;

  case 304:
#line 1545 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 305:
#line 1546 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 306:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6591 "ds_parser.cpp"
    break;

  case 307:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6597 "ds_parser.cpp"
    break;

  case 308:
#line 1549 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6606 "ds_parser.cpp"
    break;

  case 309:
#line 1553 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6615 "ds_parser.cpp"
    break;

  case 310:
#line 1557 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6623 "ds_parser.cpp"
    break;

  case 311:
#line 1560 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6631 "ds_parser.cpp"
    break;

  case 312:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6637 "ds_parser.cpp"
    break;

  case 313:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6643 "ds_parser.cpp"
    break;

  case 314:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6649 "ds_parser.cpp"
    break;

  case 315:
#line 1566 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6663 "ds_parser.cpp"
    break;

  case 316:
#line 1575 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6677 "ds_parser.cpp"
    break;

  case 317:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6683 "ds_parser.cpp"
    break;

  case 318:
#line 1585 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6691 "ds_parser.cpp"
    break;

  case 319:
#line 1588 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6697 "ds_parser.cpp"
    break;

  case 320:
#line 1588 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6703 "ds_parser.cpp"
    break;

  case 321:
#line 1588 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6711 "ds_parser.cpp"
    break;

  case 322:
#line 1591 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6720 "ds_parser.cpp"
    break;

  case 323:
#line 1595 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6729 "ds_parser.cpp"
    break;

  case 324:
#line 1599 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6738 "ds_parser.cpp"
    break;

  case 325:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6744 "ds_parser.cpp"
    break;

  case 326:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6750 "ds_parser.cpp"
    break;

  case 327:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6756 "ds_parser.cpp"
    break;

  case 328:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6762 "ds_parser.cpp"
    break;

  case 329:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6768 "ds_parser.cpp"
    break;

  case 330:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6774 "ds_parser.cpp"
    break;

  case 331:
#line 1609 "ds_parser.ypp"
    {
        if ( (yyvsp[-2].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-2].pExpression);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( (yyvsp[-2].pExpression)->rtti_isVar() ) {
            auto pVar = (ExprVar *) (yyvsp[-2].pExpression);
            auto pCall = g_Program->makeCall(pVar->at,pVar->name);
            delete pVar;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = pCall;
        } else {
            das_yyerror("can only lpipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 6795 "ds_parser.cpp"
    break;

  case 332:
#line 1625 "ds_parser.ypp"
    {
        if ( (yyvsp[0].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[0].pExpression);
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = (yyvsp[0].pExpression);
        } else if ( (yyvsp[0].pExpression)->rtti_isVar() ) {
            auto pVar = (ExprVar *) (yyvsp[0].pExpression);
            auto pCall = g_Program->makeCall(pVar->at,pVar->name);
            delete pVar;
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = pCall;
        } else {
            das_yyerror("can only lpipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    }
#line 6816 "ds_parser.cpp"
    break;

  case 333:
#line 1641 "ds_parser.ypp"
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
#line 6873 "ds_parser.cpp"
    break;

  case 334:
#line 1696 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6879 "ds_parser.cpp"
    break;

  case 335:
#line 1697 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6885 "ds_parser.cpp"
    break;

  case 336:
#line 1701 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6891 "ds_parser.cpp"
    break;

  case 337:
#line 1702 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6897 "ds_parser.cpp"
    break;

  case 338:
#line 1706 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6907 "ds_parser.cpp"
    break;

  case 339:
#line 1714 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6915 "ds_parser.cpp"
    break;

  case 340:
#line 1717 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6924 "ds_parser.cpp"
    break;

  case 341:
#line 1722 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6930 "ds_parser.cpp"
    break;

  case 342:
#line 1722 "ds_parser.ypp"
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
#line 6979 "ds_parser.cpp"
    break;

  case 343:
#line 1766 "ds_parser.ypp"
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
#line 7040 "ds_parser.cpp"
    break;

  case 344:
#line 1822 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7051 "ds_parser.cpp"
    break;

  case 345:
#line 1831 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7061 "ds_parser.cpp"
    break;

  case 346:
#line 1836 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7071 "ds_parser.cpp"
    break;

  case 347:
#line 1844 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7077 "ds_parser.cpp"
    break;

  case 348:
#line 1845 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7083 "ds_parser.cpp"
    break;

  case 349:
#line 1849 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7091 "ds_parser.cpp"
    break;

  case 350:
#line 1852 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7102 "ds_parser.cpp"
    break;

  case 351:
#line 1861 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7108 "ds_parser.cpp"
    break;

  case 352:
#line 1862 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7114 "ds_parser.cpp"
    break;

  case 353:
#line 1866 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7125 "ds_parser.cpp"
    break;

  case 354:
#line 1875 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7131 "ds_parser.cpp"
    break;

  case 355:
#line 1876 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7137 "ds_parser.cpp"
    break;

  case 356:
#line 1881 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7143 "ds_parser.cpp"
    break;

  case 357:
#line 1882 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7149 "ds_parser.cpp"
    break;

  case 358:
#line 1886 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7160 "ds_parser.cpp"
    break;

  case 359:
#line 1892 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7171 "ds_parser.cpp"
    break;

  case 360:
#line 1898 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7179 "ds_parser.cpp"
    break;

  case 361:
#line 1901 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7188 "ds_parser.cpp"
    break;

  case 362:
#line 1905 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7199 "ds_parser.cpp"
    break;

  case 363:
#line 1911 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7210 "ds_parser.cpp"
    break;

  case 364:
#line 1920 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7216 "ds_parser.cpp"
    break;

  case 365:
#line 1921 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7222 "ds_parser.cpp"
    break;

  case 366:
#line 1922 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7228 "ds_parser.cpp"
    break;

  case 367:
#line 1926 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7234 "ds_parser.cpp"
    break;

  case 368:
#line 1927 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7240 "ds_parser.cpp"
    break;

  case 369:
#line 1931 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7248 "ds_parser.cpp"
    break;

  case 370:
#line 1934 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7258 "ds_parser.cpp"
    break;

  case 371:
#line 1939 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7268 "ds_parser.cpp"
    break;

  case 372:
#line 1944 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7281 "ds_parser.cpp"
    break;

  case 373:
#line 1952 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7294 "ds_parser.cpp"
    break;

  case 374:
#line 1963 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7302 "ds_parser.cpp"
    break;

  case 375:
#line 1966 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7311 "ds_parser.cpp"
    break;

  case 376:
#line 1973 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7317 "ds_parser.cpp"
    break;

  case 377:
#line 1974 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7323 "ds_parser.cpp"
    break;

  case 378:
#line 1978 "ds_parser.ypp"
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
#line 7355 "ds_parser.cpp"
    break;

  case 379:
#line 2005 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7361 "ds_parser.cpp"
    break;

  case 380:
#line 2005 "ds_parser.ypp"
    {
        auto pDecl = (yyvsp[0].pVarDecl);
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
                if ( (yyvsp[-3].b) ) {
                    pVar->type->constant = true;
                } else {
                    pVar->type->removeConstant = true;
                }
                pVar->global_shared = (yyvsp[-2].b);
                if ( !g_Program->addVariable(pVar) )
                    das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                        CompilationError::global_variable_already_declared);
            }
        }
    }
#line 7391 "ds_parser.cpp"
    break;

  case 381:
#line 2033 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7399 "ds_parser.cpp"
    break;

  case 382:
#line 2036 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7413 "ds_parser.cpp"
    break;

  case 383:
#line 2045 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7427 "ds_parser.cpp"
    break;

  case 384:
#line 2058 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7441 "ds_parser.cpp"
    break;

  case 388:
#line 2079 "ds_parser.ypp"
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
#line 7457 "ds_parser.cpp"
    break;

  case 389:
#line 2090 "ds_parser.ypp"
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
#line 7474 "ds_parser.cpp"
    break;

  case 390:
#line 2105 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7480 "ds_parser.cpp"
    break;

  case 391:
#line 2106 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7486 "ds_parser.cpp"
    break;

  case 392:
#line 2110 "ds_parser.ypp"
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
#line 7525 "ds_parser.cpp"
    break;

  case 393:
#line 2147 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7531 "ds_parser.cpp"
    break;

  case 394:
#line 2148 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7537 "ds_parser.cpp"
    break;

  case 395:
#line 2152 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7543 "ds_parser.cpp"
    break;

  case 396:
#line 2152 "ds_parser.ypp"
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
#line 7629 "ds_parser.cpp"
    break;

  case 397:
#line 2236 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7641 "ds_parser.cpp"
    break;

  case 398:
#line 2243 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7652 "ds_parser.cpp"
    break;

  case 399:
#line 2252 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7658 "ds_parser.cpp"
    break;

  case 400:
#line 2253 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7664 "ds_parser.cpp"
    break;

  case 401:
#line 2254 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7670 "ds_parser.cpp"
    break;

  case 402:
#line 2255 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7676 "ds_parser.cpp"
    break;

  case 403:
#line 2256 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7682 "ds_parser.cpp"
    break;

  case 404:
#line 2257 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7688 "ds_parser.cpp"
    break;

  case 405:
#line 2258 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7694 "ds_parser.cpp"
    break;

  case 406:
#line 2259 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7700 "ds_parser.cpp"
    break;

  case 407:
#line 2260 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7706 "ds_parser.cpp"
    break;

  case 408:
#line 2261 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7712 "ds_parser.cpp"
    break;

  case 409:
#line 2262 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7718 "ds_parser.cpp"
    break;

  case 410:
#line 2263 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7724 "ds_parser.cpp"
    break;

  case 411:
#line 2264 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7730 "ds_parser.cpp"
    break;

  case 412:
#line 2265 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7736 "ds_parser.cpp"
    break;

  case 413:
#line 2266 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7742 "ds_parser.cpp"
    break;

  case 414:
#line 2267 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7748 "ds_parser.cpp"
    break;

  case 415:
#line 2268 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7754 "ds_parser.cpp"
    break;

  case 416:
#line 2269 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7760 "ds_parser.cpp"
    break;

  case 417:
#line 2270 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7766 "ds_parser.cpp"
    break;

  case 418:
#line 2271 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7772 "ds_parser.cpp"
    break;

  case 419:
#line 2272 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7778 "ds_parser.cpp"
    break;

  case 420:
#line 2273 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7784 "ds_parser.cpp"
    break;

  case 421:
#line 2274 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7790 "ds_parser.cpp"
    break;

  case 422:
#line 2275 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7796 "ds_parser.cpp"
    break;

  case 423:
#line 2276 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7802 "ds_parser.cpp"
    break;

  case 424:
#line 2280 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7808 "ds_parser.cpp"
    break;

  case 425:
#line 2281 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7814 "ds_parser.cpp"
    break;

  case 426:
#line 2282 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7820 "ds_parser.cpp"
    break;

  case 427:
#line 2283 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7826 "ds_parser.cpp"
    break;

  case 428:
#line 2284 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7832 "ds_parser.cpp"
    break;

  case 429:
#line 2285 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7838 "ds_parser.cpp"
    break;

  case 430:
#line 2289 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7851 "ds_parser.cpp"
    break;

  case 431:
#line 2300 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7860 "ds_parser.cpp"
    break;

  case 432:
#line 2304 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7872 "ds_parser.cpp"
    break;

  case 433:
#line 2314 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7884 "ds_parser.cpp"
    break;

  case 434:
#line 2321 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7895 "ds_parser.cpp"
    break;

  case 435:
#line 2330 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7901 "ds_parser.cpp"
    break;

  case 436:
#line 2330 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7907 "ds_parser.cpp"
    break;

  case 437:
#line 2330 "ds_parser.ypp"
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
#line 7922 "ds_parser.cpp"
    break;

  case 438:
#line 2343 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7928 "ds_parser.cpp"
    break;

  case 439:
#line 2344 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7934 "ds_parser.cpp"
    break;

  case 440:
#line 2345 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7940 "ds_parser.cpp"
    break;

  case 441:
#line 2346 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7946 "ds_parser.cpp"
    break;

  case 442:
#line 2347 "ds_parser.ypp"
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
#line 7965 "ds_parser.cpp"
    break;

  case 443:
#line 2361 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7976 "ds_parser.cpp"
    break;

  case 444:
#line 2367 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7985 "ds_parser.cpp"
    break;

  case 445:
#line 2371 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7994 "ds_parser.cpp"
    break;

  case 446:
#line 2375 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8004 "ds_parser.cpp"
    break;

  case 447:
#line 2380 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8014 "ds_parser.cpp"
    break;

  case 448:
#line 2385 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8024 "ds_parser.cpp"
    break;

  case 449:
#line 2390 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8034 "ds_parser.cpp"
    break;

  case 450:
#line 2395 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8043 "ds_parser.cpp"
    break;

  case 451:
#line 2399 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8052 "ds_parser.cpp"
    break;

  case 452:
#line 2403 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8062 "ds_parser.cpp"
    break;

  case 453:
#line 2408 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8071 "ds_parser.cpp"
    break;

  case 454:
#line 2412 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8081 "ds_parser.cpp"
    break;

  case 455:
#line 2417 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8087 "ds_parser.cpp"
    break;

  case 456:
#line 2417 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8093 "ds_parser.cpp"
    break;

  case 457:
#line 2417 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8104 "ds_parser.cpp"
    break;

  case 458:
#line 2423 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8116 "ds_parser.cpp"
    break;

  case 459:
#line 2430 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8122 "ds_parser.cpp"
    break;

  case 460:
#line 2430 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8128 "ds_parser.cpp"
    break;

  case 461:
#line 2430 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8138 "ds_parser.cpp"
    break;

  case 462:
#line 2435 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8144 "ds_parser.cpp"
    break;

  case 463:
#line 2435 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8150 "ds_parser.cpp"
    break;

  case 464:
#line 2435 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8161 "ds_parser.cpp"
    break;

  case 465:
#line 2441 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8167 "ds_parser.cpp"
    break;

  case 466:
#line 2441 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8173 "ds_parser.cpp"
    break;

  case 467:
#line 2441 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8183 "ds_parser.cpp"
    break;

  case 468:
#line 2446 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8192 "ds_parser.cpp"
    break;

  case 469:
#line 2450 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8198 "ds_parser.cpp"
    break;

  case 470:
#line 2450 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8204 "ds_parser.cpp"
    break;

  case 471:
#line 2450 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8214 "ds_parser.cpp"
    break;

  case 472:
#line 2455 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8220 "ds_parser.cpp"
    break;

  case 473:
#line 2455 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8226 "ds_parser.cpp"
    break;

  case 474:
#line 2455 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8240 "ds_parser.cpp"
    break;

  case 475:
#line 2464 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8249 "ds_parser.cpp"
    break;

  case 476:
#line 2468 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8255 "ds_parser.cpp"
    break;

  case 477:
#line 2468 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8261 "ds_parser.cpp"
    break;

  case 478:
#line 2468 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8271 "ds_parser.cpp"
    break;

  case 479:
#line 2473 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8277 "ds_parser.cpp"
    break;

  case 480:
#line 2473 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8283 "ds_parser.cpp"
    break;

  case 481:
#line 2473 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8297 "ds_parser.cpp"
    break;

  case 482:
#line 2482 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8306 "ds_parser.cpp"
    break;

  case 483:
#line 2486 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8312 "ds_parser.cpp"
    break;

  case 484:
#line 2486 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8318 "ds_parser.cpp"
    break;

  case 485:
#line 2486 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8328 "ds_parser.cpp"
    break;

  case 486:
#line 2491 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8334 "ds_parser.cpp"
    break;

  case 487:
#line 2491 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8340 "ds_parser.cpp"
    break;

  case 488:
#line 2491 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8354 "ds_parser.cpp"
    break;

  case 489:
#line 2500 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8360 "ds_parser.cpp"
    break;

  case 490:
#line 2500 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8366 "ds_parser.cpp"
    break;

  case 491:
#line 2500 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8377 "ds_parser.cpp"
    break;

  case 492:
#line 2506 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8383 "ds_parser.cpp"
    break;

  case 493:
#line 2506 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8389 "ds_parser.cpp"
    break;

  case 494:
#line 2506 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8400 "ds_parser.cpp"
    break;

  case 495:
#line 2515 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8406 "ds_parser.cpp"
    break;

  case 496:
#line 2515 "ds_parser.ypp"
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
#line 8423 "ds_parser.cpp"
    break;

  case 497:
#line 2530 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8429 "ds_parser.cpp"
    break;

  case 498:
#line 2530 "ds_parser.ypp"
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
#line 8450 "ds_parser.cpp"
    break;

  case 499:
#line 2550 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8456 "ds_parser.cpp"
    break;

  case 500:
#line 2551 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8462 "ds_parser.cpp"
    break;

  case 501:
#line 2552 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8468 "ds_parser.cpp"
    break;

  case 502:
#line 2553 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8474 "ds_parser.cpp"
    break;

  case 503:
#line 2557 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8486 "ds_parser.cpp"
    break;

  case 504:
#line 2564 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8498 "ds_parser.cpp"
    break;

  case 505:
#line 2571 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8509 "ds_parser.cpp"
    break;

  case 506:
#line 2577 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8520 "ds_parser.cpp"
    break;

  case 507:
#line 2586 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8530 "ds_parser.cpp"
    break;

  case 508:
#line 2591 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8539 "ds_parser.cpp"
    break;

  case 509:
#line 2598 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8545 "ds_parser.cpp"
    break;

  case 510:
#line 2599 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8551 "ds_parser.cpp"
    break;

  case 511:
#line 2603 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8562 "ds_parser.cpp"
    break;

  case 512:
#line 2609 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8574 "ds_parser.cpp"
    break;

  case 513:
#line 2616 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8587 "ds_parser.cpp"
    break;

  case 514:
#line 2624 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8599 "ds_parser.cpp"
    break;

  case 515:
#line 2631 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8612 "ds_parser.cpp"
    break;

  case 516:
#line 2639 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8626 "ds_parser.cpp"
    break;

  case 517:
#line 2651 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8634 "ds_parser.cpp"
    break;

  case 518:
#line 2654 "ds_parser.ypp"
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
#line 8650 "ds_parser.cpp"
    break;

  case 519:
#line 2668 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8661 "ds_parser.cpp"
    break;

  case 520:
#line 2677 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8667 "ds_parser.cpp"
    break;

  case 521:
#line 2678 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8673 "ds_parser.cpp"
    break;

  case 522:
#line 2682 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8683 "ds_parser.cpp"
    break;

  case 523:
#line 2687 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8692 "ds_parser.cpp"
    break;

  case 524:
#line 2694 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8702 "ds_parser.cpp"
    break;

  case 525:
#line 2699 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8715 "ds_parser.cpp"
    break;

  case 526:
#line 2710 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8725 "ds_parser.cpp"
    break;

  case 527:
#line 2715 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8734 "ds_parser.cpp"
    break;

  case 528:
#line 2722 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8748 "ds_parser.cpp"
    break;

  case 529:
#line 2734 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8754 "ds_parser.cpp"
    break;

  case 530:
#line 2735 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8760 "ds_parser.cpp"
    break;

  case 531:
#line 2739 "ds_parser.ypp"
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
#line 8783 "ds_parser.cpp"
    break;

  case 532:
#line 2757 "ds_parser.ypp"
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
#line 8806 "ds_parser.cpp"
    break;


#line 8810 "ds_parser.cpp"

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
#line 2777 "ds_parser.ypp"


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
