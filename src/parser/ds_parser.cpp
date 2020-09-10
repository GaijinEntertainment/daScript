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
    DAS_TCONTEXT = 349,
    ADDEQU = 350,
    SUBEQU = 351,
    DIVEQU = 352,
    MULEQU = 353,
    MODEQU = 354,
    ANDEQU = 355,
    OREQU = 356,
    XOREQU = 357,
    SHL = 358,
    SHR = 359,
    ADDADD = 360,
    SUBSUB = 361,
    LEEQU = 362,
    SHLEQU = 363,
    SHREQU = 364,
    GREQU = 365,
    EQUEQU = 366,
    NOTEQU = 367,
    RARROW = 368,
    LARROW = 369,
    QQ = 370,
    QDOT = 371,
    QBRA = 372,
    LPIPE = 373,
    LBPIPE = 374,
    LAPIPE = 375,
    LFPIPE = 376,
    RPIPE = 377,
    CLONEEQU = 378,
    ROTL = 379,
    ROTR = 380,
    ROTLEQU = 381,
    ROTREQU = 382,
    MAPTO = 383,
    COLCOL = 384,
    ANDAND = 385,
    OROR = 386,
    XORXOR = 387,
    ANDANDEQU = 388,
    OROREQU = 389,
    XORXOREQU = 390,
    BRABRAB = 391,
    BRACBRB = 392,
    CBRCBRB = 393,
    INTEGER = 394,
    LONG_INTEGER = 395,
    UNSIGNED_INTEGER = 396,
    UNSIGNED_LONG_INTEGER = 397,
    FLOAT = 398,
    DOUBLE = 399,
    NAME = 400,
    BEGIN_STRING = 401,
    STRING_CHARACTER = 402,
    END_STRING = 403,
    BEGIN_STRING_EXPR = 404,
    END_STRING_EXPR = 405,
    END_OF_READ = 406,
    UNARY_MINUS = 407,
    UNARY_PLUS = 408,
    PRE_INC = 409,
    PRE_DEC = 410,
    POST_INC = 411,
    POST_DEC = 412,
    DEREF = 413
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

#line 408 "ds_parser.cpp"

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
#define YYLAST   7739

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  186
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  532
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  947

#define YYUNDEFTOK  2
#define YYMAXUTOK   413

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
       2,     2,     2,   169,     2,   185,   180,   165,   158,     2,
     178,   179,   163,   162,   152,   161,   174,   164,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   155,   182,
     159,   153,   160,   154,   181,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   176,     2,   177,   157,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   183,   156,   184,   168,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   151,   166,   167,   170,
     171,   172,   173,   175
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   443,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   459,   471,   472,   476,   480,
     485,   494,   502,   518,   523,   531,   531,   561,   583,   587,
     590,   596,   605,   626,   649,   650,   654,   658,   659,   663,
     666,   672,   678,   681,   687,   688,   692,   693,   694,   703,
     704,   708,   717,   733,   741,   751,   760,   761,   762,   763,
     764,   765,   769,   774,   782,   783,   784,   785,   786,   787,
     788,   789,   795,   808,   824,   825,   829,   836,   848,   852,
     859,   860,   864,   865,   866,   870,   875,   882,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     931,   953,   991,   998,  1002,  1013,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,  1035,  1040,  1046,  1052,  1073,  1076,
    1082,  1083,  1094,  1098,  1104,  1107,  1110,  1114,  1120,  1124,
    1128,  1131,  1134,  1139,  1142,  1150,  1153,  1158,  1161,  1169,
    1175,  1176,  1180,  1214,  1214,  1214,  1217,  1217,  1217,  1222,
    1222,  1222,  1230,  1234,  1239,  1245,  1245,  1245,  1249,  1249,
    1249,  1254,  1254,  1254,  1263,  1266,  1272,  1273,  1280,  1291,
    1292,  1293,  1297,  1298,  1299,  1300,  1304,  1309,  1317,  1318,
    1322,  1327,  1391,  1392,  1393,  1394,  1395,  1396,  1400,  1401,
    1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,
    1412,  1413,  1414,  1415,  1416,  1417,  1418,  1422,  1423,  1424,
    1425,  1429,  1440,  1445,  1455,  1459,  1459,  1459,  1466,  1466,
    1466,  1480,  1484,  1484,  1484,  1492,  1493,  1494,  1495,  1496,
    1497,  1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,
    1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,
    1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,
    1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,  1535,  1539,
    1543,  1546,  1549,  1550,  1551,  1552,  1561,  1570,  1571,  1574,
    1574,  1574,  1577,  1581,  1585,  1589,  1590,  1591,  1592,  1593,
    1594,  1595,  1605,  1615,  1670,  1671,  1675,  1676,  1680,  1688,
    1691,  1696,  1695,  1740,  1799,  1804,  1812,  1813,  1817,  1820,
    1829,  1830,  1834,  1843,  1844,  1849,  1850,  1854,  1860,  1866,
    1869,  1873,  1879,  1888,  1889,  1890,  1894,  1895,  1899,  1902,
    1907,  1912,  1920,  1931,  1934,  1941,  1942,  1946,  1973,  1973,
    1985,  1988,  1997,  2010,  2022,  2023,  2027,  2031,  2042,  2057,
    2058,  2062,  2099,  2100,  2104,  2104,  2188,  2195,  2204,  2205,
    2206,  2207,  2208,  2209,  2210,  2211,  2212,  2213,  2214,  2215,
    2216,  2217,  2218,  2219,  2220,  2221,  2222,  2223,  2224,  2225,
    2226,  2227,  2228,  2232,  2233,  2234,  2235,  2236,  2237,  2241,
    2252,  2256,  2266,  2273,  2282,  2282,  2282,  2295,  2296,  2297,
    2298,  2299,  2300,  2314,  2320,  2324,  2328,  2333,  2338,  2343,
    2348,  2352,  2356,  2361,  2365,  2370,  2370,  2370,  2376,  2383,
    2383,  2383,  2388,  2388,  2388,  2394,  2394,  2394,  2399,  2403,
    2403,  2403,  2408,  2408,  2408,  2417,  2421,  2421,  2421,  2426,
    2426,  2426,  2435,  2439,  2439,  2439,  2444,  2444,  2444,  2453,
    2453,  2453,  2459,  2459,  2459,  2468,  2468,  2483,  2483,  2503,
    2504,  2505,  2506,  2510,  2517,  2524,  2530,  2539,  2544,  2551,
    2552,  2556,  2562,  2569,  2577,  2584,  2592,  2604,  2607,  2621,
    2630,  2631,  2635,  2640,  2647,  2652,  2663,  2668,  2675,  2687,
    2688,  2692,  2710
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
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"",
  "\"__context\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"",
  "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"",
  "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"",
  "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
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
     405,   406,    44,    61,    63,    58,   124,    94,    38,    60,
      62,    45,    43,    42,    47,    37,   407,   408,   126,    33,
     409,   410,   411,   412,    46,   413,    91,    93,    40,    41,
      36,    64,    59,   123,   125,    35
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
    -585,    23,  -585,  -585,   -26,   -74,   -23,    37,    14,  -585,
      56,  -585,  -585,    30,  -585,  -585,  -585,  -585,  -585,   315,
    -585,   151,  -585,  -585,  -585,  -585,  -585,  -585,    68,  -585,
      81,    85,    90,  -585,  -585,     6,  -585,    10,   106,  -585,
     115,   126,  -585,   130,   156,   113,  -585,    50,  -585,  -585,
    -585,     7,   199,  -585,   162,    17,   -26,   208,   -74,   212,
    -585,   213,   214,  -585,   180,  -585,   207,  -585,   -92,   178,
     179,  -585,   218,   -26,    30,  -585,  7570,   330,   335,  -585,
     192,   188,  -585,   217,  -585,  -585,   376,  -585,  -585,  -585,
    -585,  -585,   231,    69,  -585,  -585,  -585,  -585,   327,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,   201,   -73,  7484,
    -585,  -585,   237,   241,  -585,   -51,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,   -93,
     232,  -585,  -585,   -72,  -585,   205,   -42,   210,  -585,   166,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,   -32,  -585,  -585,
     -56,  -585,   230,   234,   240,   242,  -585,  -585,  -585,   222,
    -585,  -585,  -585,  -585,  -585,   243,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,   245,  -585,  -585,  -585,   246,
     249,  -585,  -585,  -585,  -585,   251,   255,  -585,  -585,  -585,
    -585,  -585,  -585,   874,  -585,   235,   260,  -585,   236,  -585,
     -26,  -585,    -2,  -585,   153,  7484,  -585,  1332,  -585,  -585,
    -585,  -585,  -585,   163,  -585,  -585,  -585,    69,  -585,   -40,
    4753,  -585,  -585,  -585,  -585,  -585,   274,  -585,    92,    95,
     101,  -585,  -585,  -585,  -585,  -585,  -585,   358,  -585,  -585,
     -12,  2989,  -585,  -585,   -39,  7484,   -29,    67,   276,  -585,
     -64,  -585,   286,  3714,  -585,  4753,  -585,  -585,   276,  -585,
    -585,  7399,   248,   510,  -585,  -585,   188,  4753,   250,  4753,
     268,   272,   254,  -585,   252,   277,   294,  3127,   188,  -585,
     278,  2143,  4753,  4753,   150,   150,  7229,  7314,  4753,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  4753,  4753,  4753,   -72,
    4753,  4753,  4753,  -585,   257,  -585,   399,  -585,  -585,   259,
     261,  4753,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
     -60,   262,  -585,   263,   264,  -585,  -585,  -585,   276,  -585,
    -585,  -585,   266,  -585,  -585,   -82,  -585,  -585,  -585,  -585,
    -585,  6371,   269,  -585,  -585,  -585,  -585,  -585,     5,   308,
    7484,  -585,   -44,  -585,  -585,  5337,  7484,  7484,  7484,  7484,
     280,   237,  7484,   192,  7484,   192,  7484,   192,  7569,   241,
    -585,  -585,  -585,   287,  -585,  -585,  6033,  -585,  -585,  3714,
    -585,  -585,   288,  -585,  -585,  -585,  7484,  -585,  2281,  -585,
     963,    -3,   989,  -585,   -72,  2281,  -585,  -585,   320,  1133,
     453,  6692,  4753,   963,  -585,  -585,  4753,  -585,  -585,   313,
     332,  6692,  -585,  7484,  2281,  -585,  4842,   307,   307,   291,
    -585,  -585,   276,  1675,   276,  1831,  6470,  -585,   152,   194,
     307,   307,   -68,  -585,   307,   307,  5485,    58,   290,  -585,
    -585,   963,  -585,  2419,  -585,  -585,  -585,  -585,   340,   150,
    -585,    27,   333,  4753,  4753,  4753,  4753,  4753,  4753,  4753,
    4753,  4753,  4753,  -585,  -585,  4753,  4753,  4753,  4753,  4753,
    4753,   334,  2575,  4753,   337,  4753,  4753,  4753,  4753,  4753,
    4753,  4753,  4753,  4753,  4753,  4753,  4753,  4753,  4753,  2713,
    3265,  4753,  4753,  4753,  4753,  4753,  4753,  4753,  4753,  4753,
    4753,   339,  4753,  3403,  -585,   473,   443,   304,  -585,   612,
    -585,  -585,  -585,  2281,  -585,  1015,  1169,  1269,  1612,  -585,
    -105,  1768,   232,  1924,   232,  1986,   232,   -53,  -585,   -61,
    3714,    83,  -585,  -585,  -585,  2085,  -585,  6271,  -585,  4753,
    3559,  2851,  -585,  4941,  4753,  4753,  -585,   188,  5562,  -585,
    7484,  7484,  5642,  7484,  -585,  -585,  2218,  -585,  5040,  -585,
    -585,     9,   150,   -63,  1987,  3715,  6470,   336,   -25,   310,
     338,  -585,  -585,   107,    16,  3871,   -25,   170,  4753,  4753,
     305,  -585,  4753,   344,   348,  -585,   117,  -585,  -585,   301,
     347,  -585,    34,  6692,   -43,   192,  -585,   341,  -585,  -585,
    6692,  6692,  6692,  6692,  6692,  6692,  6692,  6692,   233,   233,
    2585,  6692,  6692,  2585,  7087,  7087,   321,  4753,  4753,  6692,
      18,  -585,  6108,   -49,   -49,  6692,   233,   233,  6692,  6692,
     345,  6766,  6840,  6692,  6692,  6692,  4753,  4753,  6692,   353,
    6544,  6914,   696,  7062,  2585,  2585,    61,    61,    18,    18,
      18,  -585,   511,  6183,  -585,    44,   283,  -585,   276,  -585,
     358,  -585,  2281,  -585,  5139,  -585,  7484,  -585,  -585,  -585,
     366,  -585,   354,  -585,   355,  -585,   356,  7484,  -585,  7569,
    -585,   241,   358,  4753,   -78,  -585,    79,   359,   368,  5719,
    -585,  -585,  -585,  2356,  2788,  -585,  2926,   340,  -585,  4753,
    -585,  4753,  4753,    -7,   372,   347,   346,   351,  4753,   352,
    4753,  4753,   347,   349,   357,  6692,  -585,  -585,  6618,  -585,
     375,  7484,   192,  1516,   188,  -585,  -585,  4753,  -585,   -36,
      86,  4753,  -585,   385,   386,   387,   390,  -585,    87,   232,
    -585,  4027,  6988,  6692,  -585,  6988,  6692,  -585,  4753,  -585,
    -585,  -585,  -585,     7,  -585,  -585,  5238,  -585,  -585,  3064,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  3714,
    -585,  -585,  -585,  6692,   188,  -585,  -585,   116,  -585,  -585,
    -585,  -585,   360,     2,  6692,  6692,   -25,   362,   -16,   336,
     364,  -585,  6692,  -585,  -585,    38,   -25,   365,  -585,  -585,
    -585,  3202,   232,  -585,  -585,   963,   367,  6692,  -585,  -585,
    -585,  -585,   -43,   370,   -88,  7484,  -585,    80,  6692,  -585,
       7,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  7484,  4183,
     392,  4753,  4753,  4753,  4321,  4753,   373,   378,  4753,  4753,
    -585,  4753,   361,  -585,  -585,   383,   301,  -585,  -585,  -585,
    4477,  -585,  -585,  3340,  -585,   371,  -585,  3496,   393,  5799,
     384,  2585,  2585,  2585,  -585,  5876,  5411,   379,  -585,  6692,
    6692,  5411,   381,   -72,  -585,  -585,  4753,  6692,  -585,  -585,
    -585,  -585,  -585,  4615,  -585,   500,   382,  -585,   380,  -585,
    -585,   -72,  6692,  -585,   395,  7484,   401,  5956,  4753,   388,
     389,  -585,  -585,  3558,  -585,  -585,  6692,  -585,  -585,  -585,
    7484,   415,  3652,  -585,  -585,   416,  -585
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   180,     0,     0,     0,     0,     0,   181,
       0,   497,   495,     0,    14,     3,    10,     9,     8,     0,
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
     415,   416,   417,   419,   420,   468,   403,   410,   421,   475,
     482,   401,   408,   402,   409,     0,     0,   441,   429,   437,
     440,   438,   439,     0,   432,     0,     0,   353,     0,    77,
       0,    83,     0,   346,     0,     0,   131,     0,   390,   339,
     396,   377,   374,   366,   379,    17,    18,     0,    71,     0,
       0,   381,   459,   462,   465,   455,     0,   434,   469,   476,
     483,   489,   492,   446,   451,   445,   458,     0,   454,   448,
       0,     0,   383,   450,     0,     0,     0,     0,     0,   344,
     357,    84,   334,    86,   156,     0,    49,    50,     0,   271,
     272,     0,     0,     0,   265,   168,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   222,
     224,   223,   225,   226,   227,    19,     0,     0,     0,     0,
       0,     0,     0,   209,   210,   135,   133,   269,   268,     0,
       0,     0,   149,   144,   142,   141,   143,   220,   155,   136,
     266,     0,   327,     0,     0,   147,   148,   150,     0,   140,
     326,   325,    80,   330,   267,     0,   137,   329,   328,   307,
     273,   228,     0,   270,   499,   500,   501,   502,   334,     0,
       0,   367,     0,    63,   388,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,    82,     0,    82,     0,     0,
     453,   447,   449,     0,   452,   443,     0,   433,   498,   352,
     496,   354,     0,   345,   356,   355,     0,   358,     0,   347,
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
       0,   262,     0,     0,   395,     0,   336,     0,   397,     0,
     364,   365,   363,     0,   382,     0,     0,     0,     0,   431,
       0,     0,    85,     0,    85,     0,    85,   160,   350,     0,
     348,     0,   444,   442,   335,   359,   362,   361,    54,     0,
       0,     0,   174,   228,     0,     0,   171,     0,     0,    55,
       0,     0,     0,     0,    41,    42,     0,   178,   228,   175,
     211,     0,     0,   160,     0,     0,   517,   507,   509,     0,
     520,   521,   522,     0,     0,     0,   509,     0,     0,     0,
       0,    22,     0,    20,     0,   303,   255,   254,   154,    46,
       0,   308,     0,   204,     0,    82,   157,     0,   322,   323,
     238,   239,   241,   240,   242,   232,   233,   234,   278,   279,
     291,   243,   244,   292,   289,   290,     0,     0,     0,   230,
     317,   306,     0,   331,   332,   231,   280,   281,   245,   246,
     296,   297,   298,   235,   236,   237,     0,     0,   229,     0,
       0,   294,   295,   293,   287,   288,   283,   282,   284,   285,
     286,   261,     0,     0,   310,     0,   336,   337,     0,   340,
     363,   368,     0,   372,   228,   460,     0,   466,   456,   435,
       0,   470,     0,   477,     0,   484,     0,     0,   490,     0,
     493,     0,   355,     0,     0,   165,     0,     0,     0,     0,
     172,   179,   313,     0,     0,   314,     0,   218,   176,     0,
     510,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   519,   527,   528,     0,    23,
      26,     0,    82,     0,     0,    44,    45,     0,    51,     0,
       0,     0,   309,     0,     0,     0,     0,   216,     0,    85,
     319,     0,   249,   250,   305,   247,   248,   324,     0,   263,
     304,   311,   341,     0,   338,   370,   228,   371,   461,     0,
     467,   457,   436,   471,   473,   478,   480,   485,   487,   349,
     491,   351,   494,   360,     0,   166,   195,     0,   192,   184,
     187,   190,     0,     0,   504,   503,   509,     0,     0,   508,
       0,   512,   518,   524,   523,     0,   509,     0,   525,    21,
      24,     0,    85,   134,    47,     0,     0,   205,   214,   215,
     213,   212,     0,     0,     0,     0,   252,     0,   318,   264,
       0,   343,   369,   463,   474,   481,   488,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     511,     0,     0,   515,   256,     0,    46,   251,   217,   219,
       0,   206,   221,     0,   253,     0,   464,     0,     0,     0,
       0,   185,   188,   191,   315,     0,   529,     0,   513,   506,
     505,   529,     0,     0,   259,    48,     0,   207,   320,   342,
     196,   198,   193,     0,   316,     0,     0,   514,     0,   516,
     257,     0,   208,   321,     0,     0,     0,     0,     0,     0,
       0,   260,   197,     0,   201,   194,   530,   531,   532,   199,
       0,     0,     0,   200,   202,     0,   203
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -585,  -585,  -585,   103,   508,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,   471,  -585,  -585,   542,  -585,  -585,
    -585,  -270,  -585,  -585,  -585,  -585,  -585,  -585,   363,  -585,
     548,   -59,  -585,   531,  -585,  -171,  -384,  -535,  -585,  -585,
    -243,  -175,   -80,  -585,    -6,  -286,   -13,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,   610,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -523,  -585,  -585,  -229,  -585,  -113,  -281,  -585,  -233,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
     449,  -585,  -585,   239,   -67,  -585,  -585,  -585,   343,  -585,
     -91,  -585,  -271,   223,  -278,  -272,    82,  -585,   265,  -585,
    -585,  -585,  -585,   447,   559,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -158,  -106,  -585,  -585,  -585,   238,  -585,
    -585,  -585,   -87,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,  -585,
    -585,   342,  -584,  -425,  -580,  -585,  -585,  -303,  -112,   177,
    -585,  -585,  -585,  -267,  -585
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   169,   175,   459,   337,   750,   338,   614,
      16,    17,    35,    36,    63,    18,    32,    33,   339,   340,
     757,   758,   341,   342,   343,   344,   345,   346,   176,   177,
      29,    30,    45,    46,    47,    19,   160,   236,    80,    20,
      81,    82,   347,   348,   237,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   580,   861,   581,
     862,   583,   863,   361,   858,   924,   925,   941,   940,   945,
     622,   882,   362,   767,   768,   625,   363,   364,   428,   366,
     367,   368,   369,   751,   903,   752,   921,   370,   682,   849,
     623,   845,   923,   232,   688,   537,   378,   850,   233,   234,
     558,   559,   227,   228,   732,   279,   543,   382,   242,   166,
      54,    22,    86,   108,    67,    68,    23,    24,   164,    84,
      52,    25,   165,   280,   372,   107,   220,   221,   225,   222,
     391,   792,   560,   389,   791,   386,   788,   387,   886,   388,
     790,   392,   793,   393,   854,   394,   795,   395,   855,   396,
     797,   397,   856,   398,   800,   399,   802,    26,    41,    27,
      40,   373,   597,   598,   599,   374,   600,   601,   602,   603,
     375,   458,   376,   916,   377
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   162,   418,   219,   365,   411,   413,   427,   243,   552,
     685,   554,   -80,   556,   115,   457,   569,   702,   736,   704,
     401,   706,   223,     2,   592,   445,   743,    59,   729,     3,
     606,    87,    88,   450,   451,   741,   760,   479,   481,   482,
     880,    76,   592,   230,   278,   627,   714,   716,   504,   505,
     414,   414,     4,    66,     5,   699,     6,    43,     7,    60,
     731,    48,     8,    42,   501,    31,    72,   504,   505,     9,
     540,   763,   180,    44,   761,    10,    72,   700,   414,   541,
     764,   481,   482,   170,   171,   472,   231,   731,   379,   415,
     415,   416,   110,    11,   417,   161,   218,   250,   414,   708,
     480,    56,   707,   240,   804,   180,   407,   868,   532,   542,
     765,   181,    77,    78,    12,   766,   226,   415,   473,    28,
     247,   709,    34,   493,   494,   531,   251,   532,   229,   219,
     421,   501,   566,   503,   504,   505,   506,   415,   759,   572,
     507,   230,   241,   240,   384,   408,   402,   248,   283,   379,
     238,   819,    79,   817,   761,   410,    89,   735,   587,    43,
      90,   379,    91,    92,   403,    64,   493,   494,   379,   219,
      61,   277,   628,   404,   501,    44,   503,   504,   505,   506,
      62,    13,    37,   507,   865,   219,   761,    43,   409,   534,
     761,   478,   531,    65,   532,    93,   761,    38,   626,    13,
     243,    39,    74,    44,   422,    14,   813,   535,   172,    53,
     219,   219,   173,   762,   174,    92,   430,   616,   825,    56,
     871,    55,   218,   781,   528,   529,   530,    75,   442,   453,
     455,   761,   761,    56,   844,   531,   866,   532,   734,   842,
      57,   769,    58,   710,   412,   101,   872,  -472,   847,   102,
    -479,    66,  -472,   481,   482,  -479,  -486,   693,   805,   884,
      69,  -486,   218,   836,   843,   711,   103,   104,   105,   106,
    -472,    70,  -258,  -479,   219,    71,   859,  -258,   218,  -486,
     219,   219,   219,   219,   739,    72,   219,   713,   219,   740,
     219,    73,   219,   539,   591,  -258,   604,   875,   860,   545,
     546,   547,   548,   218,   218,   551,   746,   553,   816,   555,
     219,   730,   754,   245,   246,   379,   463,   826,   380,    49,
      50,   381,    51,   755,   756,   687,   782,   481,   482,   565,
     333,   449,   281,   161,   609,   282,   610,   219,   493,   494,
     568,   168,   611,   612,    83,    85,   501,    96,   503,   504,
     505,   506,   740,   579,   744,   507,   586,    98,    99,   100,
     109,   112,   113,   114,   157,   481,   482,   218,   832,   158,
     159,   161,   163,   218,   218,   218,   218,   167,   168,   218,
      60,   218,   224,   218,   179,   218,   226,   235,   239,   252,
     400,   619,   244,   253,   526,   527,   528,   529,   530,   254,
     256,   255,   257,   218,   258,   259,   785,   531,   260,   532,
     261,   571,   493,   494,   262,   275,   784,   274,   276,   390,
     501,   240,   230,   504,   505,   506,   424,   434,   432,   507,
     218,   435,   436,   439,   437,   468,   438,   443,   467,   479,
     411,   469,    13,   470,   474,   475,   476,   533,   491,   492,
     493,   494,   495,   538,   617,   498,   499,   500,   501,   549,
     503,   504,   505,   506,   562,   564,   577,   507,   584,   509,
     510,   585,   590,   618,   219,   219,   624,   219,   629,   646,
     686,   531,   651,   532,   681,   687,   689,   737,   734,   747,
     738,   245,   759,   723,   724,   749,   726,   721,   777,   771,
     770,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   407,   779,   807,   794,   796,   798,   818,   806,   531,
     365,   532,   830,   820,   289,   290,   291,   292,   821,   823,
     838,   839,   840,   827,   828,   841,   294,   890,   864,   867,
     869,   870,   873,   904,   913,   902,   877,   879,   298,   928,
     897,   300,   911,   909,   301,   898,   917,   302,   919,   929,
     934,   305,   613,    94,   930,   937,   938,   218,   218,   178,
     218,   186,   187,   188,   932,   190,   191,   192,   193,   194,
     309,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     219,   206,   207,   208,   943,   946,   211,   212,   213,   214,
      97,   219,   310,   219,    95,   116,   905,   885,   851,   789,
     383,    21,   753,   878,   812,   312,   313,   536,   801,   783,
     799,   692,   561,   477,   425,   419,   249,   111,   824,   550,
     314,   315,   607,   423,   918,     0,     0,     0,     0,    43,
       0,     0,     0,     0,   263,   219,   316,   317,   318,   319,
     320,   321,   322,   323,   324,    44,   325,     0,     0,     0,
       0,     0,     0,     0,   831,     0,     0,     0,   264,   265,
       0,   326,   327,   328,   834,   329,     0,     0,   330,   331,
       0,     0,     0,   218,     0,     0,   371,     0,   332,     0,
     333,   334,   426,   161,   218,     0,   218,     0,     0,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   481,   482,     0,     0,
     406,     0,     0,     0,   857,     0,   540,   266,     0,     0,
       0,     0,     0,     0,   420,   541,     0,     0,   218,   219,
       0,     0,   429,     0,     0,     0,   431,     0,   433,     0,
       0,     0,   219,     0,     0,   876,   441,     0,   883,     0,
     446,   447,   448,     0,   881,   690,   268,   456,     0,     0,
     269,   887,     0,   270,     0,   460,   461,   462,     0,   464,
     465,   466,     0,     0,     0,     0,     0,     0,   271,     0,
     471,     0,     0,     0,   691,     0,     0,   273,     0,   491,
     492,   493,   494,   495,     0,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,     0,     0,     0,   507,   219,
     509,   510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   218,     0,   219,     0,     0,     0,   933,     0,
       0,     0,     0,     0,     0,   218,     0,     0,     0,     0,
       0,     0,     0,   942,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,   567,     0,     0,
     531,     0,   532,     0,   573,     0,     0,     0,     0,     0,
       0,   578,     0,     0,     0,   582,     0,     0,     0,     0,
     920,     0,     0,   588,     0,     0,     0,     0,     0,     0,
       0,     0,   596,     0,   596,     0,   263,     0,   931,     0,
       0,     0,   218,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   218,     0,     0,
     264,   265,   630,   631,   632,   633,   634,   635,   636,   637,
     638,   639,     0,     0,   640,   641,   642,   643,   644,   645,
       0,   649,   650,     0,   652,   653,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,   664,   665,   668,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
       0,   683,     0,   481,   482,     0,     0,     0,     0,   266,
       0,     0,   694,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     719,   263,     0,   649,   668,     0,     0,   267,   268,     0,
       0,     0,   269,     0,     0,   270,     0,     0,     0,     0,
       0,     0,     0,   460,   466,   264,   265,   263,     0,     0,
     271,     0,     0,     0,   466,     0,   272,   745,   456,   273,
       0,   748,     0,     0,     0,     0,   491,   492,   493,   494,
     495,   264,   265,   498,   499,   500,   501,     0,   503,   504,
     505,   506,     0,     0,     0,   507,     0,   509,   510,     0,
       0,     0,     0,   513,   514,   515,   772,   773,     0,     0,
       0,     0,     0,     0,   266,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   775,   776,   520,     0,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,     0,
     266,     0,     0,     0,     0,     0,     0,   531,     0,   532,
       0,   786,   267,   268,     0,     0,   161,   269,     0,     0,
     270,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,   803,     0,     0,   271,     0,   570,   267,   268,
       0,     0,     0,   269,   273,   695,   270,     0,     0,     0,
     814,   815,     0,     0,     0,     0,     0,   822,     0,   596,
       0,   271,     0,     0,     0,     0,     0,     0,     0,     0,
     273,   263,   371,     0,     0,     0,   835,     0,     0,     0,
     837,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,   265,   848,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   574,   503,   504,
     505,   506,     0,     0,     0,   507,   508,   509,   510,   511,
     512,     0,     0,   513,   514,   515,   516,   517,   518,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,   575,   520,     0,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,     0,
       0,   263,     0,     0,     0,     0,     0,   531,   889,   532,
     891,   892,   893,   895,   896,   576,     0,   899,   900,     0,
     901,     0,   267,   268,     0,   264,   265,   269,     0,   907,
     270,     0,     0,   284,     0,     0,     0,     0,     3,     0,
     285,   286,   287,     0,   288,   271,   289,   290,   291,   292,
       0,   696,     0,     0,   273,   922,     0,   293,   294,   295,
     296,     0,   927,     0,     0,     0,     0,     0,     0,   297,
     298,     0,   299,   300,     0,     0,   301,   936,     9,   302,
     303,     0,   304,   305,   266,     0,   306,   307,     0,     0,
       0,     0,   308,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   309,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,   267,   268,   310,   311,     0,   269,     0,   697,
     270,     0,     0,     0,     0,     0,     0,   312,   313,     0,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,     0,   314,   315,   273,     0,     0,     0,     0,     0,
       0,    43,     0,     0,     0,     0,     0,     0,   316,   317,
     318,   319,   320,   321,   322,   323,   324,    44,   325,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327,   328,     0,   329,     0,     0,
     330,   331,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,   333,   334,   335,   161,   336,   284,     0,     0,
       0,     0,     3,     0,   285,   286,   287,     0,   288,     0,
     289,   290,   291,   292,     0,     0,     0,     0,     0,     0,
       0,   293,   294,   295,   296,     0,     0,     0,     0,     0,
       0,     0,     0,   297,   298,     0,   299,   300,     0,     0,
     301,     0,     9,   302,   303,     0,   304,   305,     0,     0,
     306,   307,     0,     0,     0,     0,   308,   186,   187,   188,
       0,   190,   191,   192,   193,   194,   309,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   206,   207,   208,
       0,     0,   211,   212,   213,   214,     0,     0,   310,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,   315,     0,     0,
       0,     0,     0,     0,   263,    43,     0,     0,     0,     0,
       0,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,    44,   325,     0,     0,     0,     0,     0,   264,   265,
       0,     0,     0,     0,     0,     0,     0,   326,   327,   328,
       0,   329,     0,     0,   330,   331,     0,     0,     0,   289,
     290,   291,   292,     0,   332,     0,   333,   334,   335,   161,
     833,   294,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,   298,     0,     0,   300,     0,     0,   301,
       0,     0,   302,     0,   592,     0,   305,   266,     0,     0,
       0,   264,   265,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   309,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,   267,   268,   310,     0,     0,
     269,     0,   698,   270,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,   271,     0,
     266,     0,     0,     0,     0,     0,     0,   273,     0,     0,
     263,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     593,   325,     0,     0,   264,   265,     0,     0,   267,   268,
       0,     0,     0,   269,     0,     0,   594,   327,   328,     0,
     329,     0,     0,   330,   331,   289,   290,   291,   292,     0,
       0,   271,     0,   595,     0,   333,   334,   294,   161,     0,
     273,     0,     0,   263,     0,     0,     0,     0,     0,   298,
       0,     0,   300,     0,     0,   301,     0,     0,   302,     0,
       0,     0,   305,   266,     0,     0,     0,   264,   265,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   309,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,   267,   268,   310,     0,     0,   269,     0,   701,   270,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,   271,     0,   266,     0,     0,     0,
       0,     0,     0,   273,     0,     0,   263,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   593,   325,     0,     0,
     264,   265,     0,     0,   267,   268,     0,     0,     0,   269,
       0,     0,   594,   327,   328,     0,   329,     0,     0,   330,
     331,   289,   290,   291,   292,     0,     0,   271,     0,   605,
       0,   333,   334,   294,   161,     0,   273,     0,   263,   401,
       0,     0,     0,     0,     0,   298,     0,     0,   300,     0,
       0,   301,     0,     0,   302,     0,     0,     0,   305,   266,
       0,     0,   264,   265,     0,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   309,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,   267,   268,   310,
       0,     0,   269,     0,   703,   270,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
     271,   266,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,    43,   263,     0,     0,
       0,     0,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,    44,   325,     0,     0,     0,     0,     0,   267,
     268,   264,   265,     0,   269,   402,   705,   270,   326,   327,
     328,     0,   329,     0,     0,   330,   331,   289,   290,   291,
     292,     0,   271,   403,     0,   332,     0,   333,   334,   294,
     161,   273,   404,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,   300,     0,     0,   301,     0,     0,
     302,     0,     0,     0,   305,     0,     0,     0,     0,   414,
     266,     0,     0,     0,   186,   187,   188,     0,   190,   191,
     192,   193,   194,   309,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   206,   207,   208,     0,     0,   211,
     212,   213,   214,     0,     0,   310,     0,     0,   712,   268,
       0,     0,     0,   269,     0,     0,   270,     0,   312,   313,
     263,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,   271,     0,   314,   315,     0,     0,     0,     0,     0,
     273,     0,    43,     0,   264,   265,     0,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,     0,     0,     0,   289,   290,   291,   292,     0,
       0,     0,     0,     0,   326,   327,   328,   294,   329,     0,
       0,   330,   331,     0,     0,     0,     0,     0,     0,   298,
       0,   332,   300,   333,   334,   301,   161,     0,   302,     0,
       0,     0,   305,   266,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   309,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,   267,   268,   310,     0,     0,   269,     0,   727,   270,
       0,     0,     0,     0,     0,     0,   312,   313,   263,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
       0,   314,   315,   273,     0,     0,     0,     0,     0,     0,
      43,     0,   264,   265,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,    44,   325,     0,     0,
       0,     0,     0,   289,   290,   291,   292,     0,     0,     0,
       0,     0,   326,   327,   328,   294,   329,     0,     0,   330,
     331,     0,     0,     0,     0,     0,     0,   298,     0,   332,
     300,   333,   334,   301,   161,     0,   302,     0,     0,     0,
     305,   266,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   187,   188,     0,   190,   191,   192,   193,   194,   309,
     196,   197,   198,   199,   200,   201,   202,   203,   204,     0,
     206,   207,   208,     0,     0,   211,   212,   213,   214,   267,
     268,   310,     0,     0,   269,     0,   809,   270,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,   328,     0,   329,     0,     0,   330,   331,   289,
     290,   291,   292,     0,     0,   620,     0,   332,   621,   333,
     334,   294,   161,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   300,     0,     0,   301,
       0,     0,   302,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   309,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,   491,   492,
     493,   494,     0,     0,     0,   647,   648,     0,   501,     0,
     503,   504,   505,   506,    43,     0,     0,   507,     0,   509,
     510,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,   325,     0,     0,     0,     0,     0,   289,   290,   291,
     292,     0,     0,     0,     0,     0,   326,   327,   328,   294,
     329,     0,     0,   330,   331,     0,   526,   527,   528,   529,
     530,   298,     0,   332,   300,   333,   334,   301,   161,   531,
     302,   532,     0,     0,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,   188,     0,   190,   191,
     192,   193,   194,   309,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   206,   207,   208,     0,     0,   211,
     212,   213,   214,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   666,   667,     0,     0,     0,     0,     0,
       0,     0,    43,     0,   264,   265,     0,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,     0,     0,     0,   289,   290,   291,   292,   717,
       0,     0,     0,     0,   326,   327,   328,   294,   329,     0,
       0,   330,   331,     0,     0,     0,     0,     0,     0,   298,
       0,   332,   300,   333,   334,   301,   161,     0,   302,     0,
       0,     0,   305,   266,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   309,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,   267,   268,   310,     0,     0,   269,     0,   810,   270,
       0,     0,     0,     0,     0,     0,   312,   313,   263,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
       0,     0,     0,   273,     0,     0,     0,     0,     0,     0,
      43,     0,   264,   265,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,    44,   325,     0,     0,
       0,     0,     0,   289,   290,   291,   292,     0,     0,     0,
     718,     0,   326,   327,   328,   294,   329,     0,     0,   330,
     331,     0,     0,     0,     0,     0,     0,   298,     0,   332,
     300,   333,   334,   301,   161,     0,   302,     0,     0,     0,
     305,   266,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   187,   188,     0,   190,   191,   192,   193,   194,   309,
     196,   197,   198,   199,   200,   201,   202,   203,   204,     0,
     206,   207,   208,     0,     0,   211,   212,   213,   214,   267,
     268,   310,     0,     0,   269,     0,   811,   270,     0,     0,
       0,     0,     0,     0,   312,   313,   263,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,    43,     0,
     264,   265,     0,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,     0,     0,
       0,   289,   290,   291,   292,     0,     0,     0,     0,     0,
     326,   327,   328,   294,   329,     0,     0,   330,   331,     0,
       0,     0,     0,     0,     0,   298,   405,   332,   300,   333,
     334,   301,   161,     0,   302,     0,     0,     0,   305,   266,
       0,   440,     0,     0,     0,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   309,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,   267,   268,   310,
       0,     0,   269,     0,   853,   270,     0,     0,     0,     0,
       0,     0,   312,   313,   263,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,    43,     0,   264,   265,
       0,     0,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,    44,   325,     0,     0,     0,     0,     0,   289,
     290,   291,   292,     0,     0,     0,   669,     0,   326,   327,
     328,   294,   329,     0,     0,   330,   331,     0,     0,     0,
       0,     0,     0,   298,     0,   332,   300,   333,   334,   301,
     161,     0,   302,     0,     0,     0,   305,   266,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   309,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,   267,   268,   310,     0,     0,
     269,     0,   874,   270,     0,     0,     0,     0,     0,     0,
     312,   313,   263,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,    43,     0,   264,   265,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,   325,     0,     0,     0,     0,     0,   289,   290,   291,
     292,     0,     0,     0,     0,     0,   326,   327,   328,   294,
     329,     0,     0,   330,   331,     0,     0,     0,     0,     0,
       0,   298,     0,   332,   300,   333,   334,   301,   161,     0,
     302,     0,     0,     0,   305,   266,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,   188,     0,   190,   191,
     192,   193,   194,   309,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   206,   207,   208,     0,     0,   211,
     212,   213,   214,   267,   268,   310,     0,     0,   269,     0,
     908,   270,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,   271,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,   263,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,   264,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   327,   328,     0,   329,     0,
       0,   330,   331,   289,   290,   291,   292,     0,     0,     0,
       0,   332,   684,   333,   334,   294,   161,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,   298,     0,     0,
     300,     0,     0,   301,     0,     0,   302,     0,     0,     0,
     305,   266,     0,     0,   264,   265,     0,     0,     0,     0,
     186,   187,   188,     0,   190,   191,   192,   193,   194,   309,
     196,   197,   198,   199,   200,   201,   202,   203,   204,     0,
     206,   207,   208,     0,     0,   211,   212,   213,   214,   267,
     268,   310,     0,     0,   269,     0,   910,   270,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,   271,   266,     0,     0,     0,     0,     0,     0,
       0,   273,     0,     0,   263,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   316,   317,   318,   319,   320,
     321,   322,   323,   324,    44,   325,     0,     0,   264,   265,
       0,   267,   268,     0,     0,     0,   269,     0,   939,   270,
     326,   327,   328,     0,   329,     0,     0,   330,   331,   289,
     290,   291,   292,     0,   271,     0,     0,   332,   715,   333,
     334,   294,   161,   273,     0,     0,   263,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   300,     0,     0,   301,
       0,     0,   302,     0,     0,     0,   305,   266,     0,     0,
     264,   265,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   309,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,   267,   268,   310,     0,     0,
     269,     0,   944,   270,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,   271,   266,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,   325,     0,     0,     0,     0,     0,   267,   268,     0,
       0,     0,   269,     0,     0,   270,   326,   327,   328,     0,
     329,     0,     0,   330,   331,   289,   290,   291,   292,     0,
     271,     0,     0,   332,   733,   333,   334,   294,   161,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,   300,     0,     0,   301,     0,     0,   302,     0,
       0,     0,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   309,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,     0,     0,   310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,    44,   325,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,   328,     0,   329,     0,     0,   330,
     331,   289,   290,   291,   292,     0,     0,     0,     0,   332,
     742,   333,   334,   294,   161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,   300,     0,
       0,   301,     0,     0,   302,     0,     0,     0,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   309,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,    44,   325,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,   327,
     328,     0,   329,     0,     0,   330,   331,   289,   290,   291,
     292,   888,     0,     0,     0,   332,   846,   333,   334,   294,
     161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,   300,     0,     0,   301,     0,     0,
     302,     0,     0,     0,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,   188,     0,   190,   191,
     192,   193,   194,   309,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   206,   207,   208,     0,     0,   211,
     212,   213,   214,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,     0,     0,     0,   289,   290,   291,   292,     0,
       0,     0,     0,     0,   326,   327,   328,   294,   329,     0,
       0,   330,   331,     0,     0,     0,     0,     0,     0,   298,
       0,   332,   300,   333,   334,   301,   161,     0,   302,     0,
       0,     0,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   309,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,     0,     0,   310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,    44,   325,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,   328,     0,   329,     0,     0,   330,
     331,   289,   290,   291,   292,     0,     0,     0,     0,   332,
     894,   333,   334,   294,   161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,   300,     0,
       0,   301,     0,     0,   302,     0,     0,     0,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   186,   187,
     188,     0,   190,   191,   192,   193,   194,   309,   196,   197,
     198,   199,   200,   201,   202,   203,   204,     0,   206,   207,
     208,     0,     0,   211,   212,   213,   214,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,   906,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,    44,   325,     0,     0,     0,     0,     0,   289,
     290,   291,   292,   926,     0,     0,     0,     0,   326,   327,
     328,   294,   329,     0,     0,   330,   331,     0,     0,     0,
       0,     0,     0,   298,     0,   332,   300,   333,   334,   301,
     161,     0,   302,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   309,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
       0,   211,   212,   213,   214,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,   325,     0,     0,     0,     0,     0,   289,   290,   291,
     292,     0,     0,     0,     0,     0,   326,   327,   328,   294,
     329,     0,     0,   330,   331,     0,     0,     0,     0,     0,
       0,   298,     0,   332,   300,   333,   334,   301,   161,     0,
     302,     0,     0,     0,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,   188,     0,   190,   191,
     192,   193,   194,   309,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   206,   207,   208,     0,     0,   211,
     212,   213,   214,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,   481,   482,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    44,   325,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   327,   328,     0,   329,     0,
       0,   330,   331,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,   333,   334,     0,   161,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   574,   503,   504,   505,
     506,   481,   482,     0,   507,   508,   509,   510,   511,   512,
       0,     0,   513,   514,   515,   516,   517,   518,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,     0,
       0,     0,     0,     0,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   574,   503,   504,   505,   506,
     481,   482,     0,   507,   508,   509,   510,   511,   512,     0,
       0,   513,   514,   515,   516,   517,   518,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   575,   520,     0,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,     0,     0,   531,     0,   532,     0,     0,
       0,     0,     0,   720,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   574,   503,   504,   505,   506,   481,
     482,     0,   507,   508,   509,   510,   511,   512,     0,     0,
     513,   514,   515,   516,   517,   518,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,   520,     0,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,     0,     0,
       0,     0,     0,     0,   531,     0,   532,     0,     0,     0,
       0,     0,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   574,   503,   504,   505,   506,   481,   482,
       0,   507,   508,   509,   510,   511,   512,     0,     0,   513,
     514,   515,   516,   517,   518,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   575,   520,     0,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,     0,     0,   531,     0,   532,     0,     0,     0,     0,
       0,   787,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   574,   503,   504,   505,   506,   481,   482,     0,
     507,   508,   509,   510,   511,   512,     0,     0,   513,   514,
     515,   516,   517,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   575,   520,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,     0,     0,     0,     0,     0,
     852,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,   494,   495,     0,     0,   498,   499,   500,
     501,     0,   503,   504,   505,   506,     0,     0,     0,   507,
       0,   509,   510,     0,     0,     0,     0,   513,   514,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   520,     0,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,     0,     0,   481,   482,     0,     0,     0,
       0,   531,     0,   532,   491,   492,   493,   494,   495,   544,
       0,   498,   499,   500,   501,     0,   503,   504,   505,   506,
       0,     0,     0,   507,     0,   509,   510,     0,     0,     0,
       0,   513,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,     0,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,     0,     0,     0,
       0,     0,   481,   482,     0,   531,     0,   532,   491,   492,
     493,   494,   495,   915,     0,   498,   499,   500,   501,     0,
     503,   504,   505,   506,     0,     0,     0,   507,     0,   509,
     510,     0,     0,     0,     0,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   520,
       0,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,     0,     0,   531,
       0,   532,   481,   482,   615,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
       0,     0,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,   481,
     482,   722,     0,     0,     0,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
       0,     0,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,   481,
     482,   725,   491,   492,   493,   494,   495,     0,     0,   498,
     499,   500,   501,     0,   503,   504,   505,   506,     0,     0,
       0,   507,     0,   509,   510,     0,     0,     0,     0,   513,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,     0,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,     0,     0,   531,     0,   532,   481,   482,   808,     0,
       0,     0,   491,   492,   493,   494,   495,     0,     0,   498,
     499,   500,   501,     0,   503,   504,   505,   506,     0,     0,
       0,   507,     0,   509,   510,     0,     0,     0,     0,   513,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,     0,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,     0,     0,   531,     0,   532,   481,   482,   912,   491,
     492,   493,   494,   495,     0,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,     0,     0,     0,   507,     0,
     509,   510,     0,     0,     0,     0,   513,   514,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,     0,     0,     0,
     531,     0,   532,   481,   482,   914,     0,     0,     0,   491,
     492,   493,   494,   495,     0,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,     0,     0,     0,   507,     0,
     509,   510,     0,     0,     0,     0,   513,   514,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,     0,   481,   482,
     531,     0,   532,     0,     0,   935,   491,   492,   493,   494,
     495,     0,     0,   498,   499,   500,   501,     0,   503,   504,
     505,   506,     0,     0,     0,   507,     0,   509,   510,     0,
       0,     0,     0,   513,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,     0,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,     0,
       0,     0,     0,   481,   482,     0,     0,   531,     0,   532,
     563,   491,   492,   493,   494,   495,     0,     0,   498,   499,
     500,   501,     0,   503,   504,   505,   506,     0,     0,     0,
     507,     0,   509,   510,     0,     0,     0,     0,   513,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   774,   491,   492,   493,   494,
     495,   481,   482,   498,   499,   500,   501,     0,   503,   504,
     505,   506,     0,     0,     0,   507,     0,   509,   510,     0,
       0,     0,     0,   513,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,     0,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,     0,     0,   531,     0,   532,
     780,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   574,   503,   504,   505,   506,
    -228,   481,   482,   507,   508,   509,   510,   511,   512,     0,
       0,   513,   514,   515,   516,   517,   518,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   575,   520,     0,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,     0,     0,   531,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     481,   482,     0,   507,   508,   509,   510,   511,   512,     0,
       0,   513,   514,   515,   516,   517,   518,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,   520,     0,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,     0,     0,   531,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   481,   482,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,     0,
     498,   499,   500,   501,     0,   503,   504,   505,   506,     0,
       0,     0,   507,     0,   509,   510,     0,     0,   608,     0,
     513,   514,   515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,   481,   482,
       0,     0,     0,     0,   531,     0,   532,   491,   492,   493,
     494,   495,     0,     0,   498,   499,   500,   501,     0,   503,
     504,   505,   506,     0,     0,     0,   507,     0,   509,   510,
       0,     0,     0,     0,   513,   514,   515,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   520,   778,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,   481,   482,     0,     0,     0,     0,   531,     0,
     532,   491,   492,   493,   494,   495,     0,     0,   498,   499,
     500,   501,     0,   503,   504,   505,   506,     0,     0,     0,
     507,     0,   509,   510,     0,     0,     0,     0,   513,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   829,     0,
       0,     0,   520,     0,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,     0,     0,   481,   482,     0,     0,
       0,     0,   531,     0,   532,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
       0,     0,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
     481,   482,     0,     0,     0,     0,   531,     0,   532,   491,
     492,   493,   494,   495,     0,     0,   498,   499,   500,   501,
       0,   503,   504,   505,   506,     0,     0,     0,   507,     0,
     509,   510,     0,     0,     0,     0,   513,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,   481,   482,     0,     0,     0,     0,
     531,     0,   532,   491,   492,   493,   494,   495,     0,     0,
     498,   499,   500,   501,     0,   503,   504,   505,   506,     0,
       0,     0,   507,     0,   509,   510,     0,     0,     0,     0,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,   481,   482,
       0,     0,     0,     0,   531,     0,   532,   491,   492,   493,
     494,   495,     0,     0,   498,   499,   500,   501,     0,   503,
     504,   505,   506,     0,     0,     0,   507,     0,   509,   510,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   522,   523,   524,   525,   526,   527,   528,   529,   530,
       0,     0,   481,   482,     0,     0,     0,     0,   531,     0,
     532,   491,   492,     0,     0,   495,     0,     0,   498,   499,
     500,   501,     0,   503,   504,   505,   506,   481,   482,     0,
     507,     0,   509,   510,     0,     0,     0,     0,   513,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,   521,   522,   523,   524,   525,     0,
       0,     0,   529,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   491,   492,   493,   494,   495,
       0,     0,   498,   499,   500,   501,     0,   503,   504,   505,
     506,     0,     0,     0,   507,     0,   509,   510,     0,     0,
     491,   492,   493,   494,   495,     0,     0,   498,     0,     0,
     501,     0,   503,   504,   505,   506,     0,     0,     0,   507,
       0,   509,   510,     0,     0,     0,     0,     0,     0,     0,
       0,   524,   525,   526,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,     0,
       0,   452,     0,     0,     0,     0,   524,   525,   526,   527,
     528,   529,   530,   182,     0,     0,     0,     0,     0,   183,
       0,   531,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,   217,     0,     0,   454,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   182,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   185,    44,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     0,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,     0,     0,     0,     0,     0,   183,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,    44,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,   217,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   182,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,   184,
       0,     0,     0,     0,     0,   316,   317,   318,     0,     0,
       0,     0,     0,   185,    44,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,     0,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,     0,     0,     0,     0,     0,   183,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,    44,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,   217,     0,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,    43,     0,
     139,   140,   141,   142,   143,   144,     0,     0,     0,     0,
       0,     0,     0,     0,   557,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,     0,     0,   155,   156
};

static const yytype_int16 yycheck[] =
{
      13,    81,   280,   109,   237,   276,   278,   293,   166,   393,
     533,   395,     7,   397,    73,   318,    19,   552,   598,   554,
      32,   556,   109,     0,    49,   311,   606,    21,    19,     6,
     455,    14,    15,   314,   315,    19,   620,   119,    20,    21,
     128,    34,    49,   136,    46,    18,   569,   570,   116,   117,
     114,   114,    29,   145,    31,   160,    33,   129,    35,    53,
     123,    74,    39,    33,   113,   139,   129,   116,   117,    46,
     114,   114,   145,   145,   152,    52,   129,   182,   114,   123,
     123,    20,    21,    14,    15,   145,   179,   123,   152,   153,
     153,   155,   184,    70,   158,   183,   109,   153,   114,   160,
     182,   152,   155,   145,   182,   145,   145,   123,   176,   153,
     153,   184,   105,   106,    91,   158,   145,   153,   178,   145,
     152,   182,   145,   105,   106,   174,   182,   176,   179,   235,
     288,   113,   418,   115,   116,   117,   118,   153,   145,   425,
     122,   136,   184,   145,   184,   184,   158,   179,   235,   152,
     163,   735,   145,   733,   152,   184,   139,   182,   444,   129,
     143,   152,   145,   146,   176,   155,   105,   106,   152,   275,
     164,   230,   145,   185,   113,   145,   115,   116,   117,   118,
     174,   176,   145,   122,   182,   291,   152,   129,   275,   184,
     152,   362,   174,   183,   176,   178,   152,   183,   479,   176,
     358,   145,   152,   145,   291,   182,   729,   378,   139,    58,
     316,   317,   143,   179,   145,   146,   296,   159,   741,   152,
     182,   153,   235,   179,   163,   164,   165,   177,   308,   316,
     317,   152,   152,   152,   769,   174,   816,   176,   152,   152,
     155,   625,   152,   160,   177,    65,   826,   155,   771,    69,
     155,   145,   160,    20,    21,   160,   155,   543,   179,   179,
     145,   160,   275,   177,   177,   182,    86,    87,    88,    89,
     178,   145,   155,   178,   380,   145,   160,   160,   291,   178,
     386,   387,   388,   389,   177,   129,   392,   565,   394,   182,
     396,   178,   398,   380,   452,   178,   454,   832,   182,   386,
     387,   388,   389,   316,   317,   392,   609,   394,   733,   396,
     416,   592,    11,   147,   148,   152,   329,   742,   155,     4,
       5,   158,     7,    22,    23,    42,    43,    20,    21,   416,
     180,   181,   179,   183,   182,   182,   184,   443,   105,   106,
     420,   147,   148,   149,   145,   183,   113,   139,   115,   116,
     117,   118,   182,   433,   184,   122,   443,   145,   145,   145,
     153,   183,   183,   145,    34,    20,    21,   380,   752,    34,
     178,   183,   155,   386,   387,   388,   389,     1,   147,   392,
      53,   394,   145,   396,   183,   398,   145,   155,   183,   159,
      32,   471,   182,   159,   161,   162,   163,   164,   165,   159,
     178,   159,   159,   416,   159,   159,   692,   174,   159,   176,
     159,   424,   105,   106,   159,   155,   688,   182,   182,   145,
     113,   145,   136,   116,   117,   118,   178,   159,   178,   122,
     443,   159,   178,   139,   182,    36,   159,   159,   181,   119,
     711,   182,   176,   182,   182,   182,   182,   178,   103,   104,
     105,   106,   107,   145,   467,   110,   111,   112,   113,   179,
     115,   116,   117,   118,   177,   177,    13,   122,   155,   124,
     125,   139,   181,   183,   580,   581,   136,   583,   145,   145,
       7,   174,   145,   176,   145,    42,   182,   177,   152,   184,
     152,   147,   145,   580,   581,   147,   583,   577,   145,   178,
     159,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   145,     1,   145,   160,   160,   160,   145,   159,   174,
     753,   176,   147,   177,    14,    15,    16,    17,   177,   177,
     145,   145,   145,   184,   177,   145,    26,   145,   178,   177,
     818,   177,   177,   160,   160,   184,   179,   177,    38,    49,
     177,    41,   159,   182,    44,   177,   177,    47,   177,   177,
     159,    51,   459,    55,   184,   177,   177,   580,   581,    98,
     583,    61,    62,    63,   179,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
     696,    81,    82,    83,   179,   179,    86,    87,    88,    89,
      58,   707,    92,   709,    56,    74,   876,   850,   783,   696,
     247,     1,   618,   842,   727,   105,   106,   378,   709,   686,
     707,   539,   399,   358,   114,   282,   179,    68,   740,   391,
     120,   121,   455,   291,   901,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    32,   751,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   751,    -1,    -1,    -1,    56,    57,
      -1,   161,   162,   163,   754,   165,    -1,    -1,   168,   169,
      -1,    -1,    -1,   696,    -1,    -1,   237,    -1,   178,    -1,
     180,   181,   182,   183,   707,    -1,   709,    -1,    -1,   250,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
     271,    -1,    -1,    -1,   804,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,   285,   123,    -1,    -1,   751,   845,
      -1,    -1,   293,    -1,    -1,    -1,   297,    -1,   299,    -1,
      -1,    -1,   858,    -1,    -1,   835,   307,    -1,   845,    -1,
     311,   312,   313,    -1,   844,   153,   154,   318,    -1,    -1,
     158,   858,    -1,   161,    -1,   326,   327,   328,    -1,   330,
     331,   332,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
     341,    -1,    -1,    -1,   182,    -1,    -1,   185,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,   925,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   845,    -1,   940,    -1,    -1,    -1,   925,    -1,
      -1,    -1,    -1,    -1,    -1,   858,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   940,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,   418,    -1,    -1,
     174,    -1,   176,    -1,   425,    -1,    -1,    -1,    -1,    -1,
      -1,   432,    -1,    -1,    -1,   436,    -1,    -1,    -1,    -1,
     903,    -1,    -1,   444,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   453,    -1,   455,    -1,    32,    -1,   921,    -1,
      -1,    -1,   925,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   940,    -1,    -1,
      56,    57,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,    -1,    -1,   495,   496,   497,   498,   499,   500,
      -1,   502,   503,    -1,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
      -1,   532,    -1,    20,    21,    -1,    -1,    -1,    -1,   115,
      -1,    -1,   543,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     571,    32,    -1,   574,   575,    -1,    -1,   153,   154,    -1,
      -1,    -1,   158,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   594,   595,    56,    57,    32,    -1,    -1,
     176,    -1,    -1,    -1,   605,    -1,   182,   608,   609,   185,
      -1,   612,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    56,    57,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,   647,   648,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   666,   667,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
      -1,   692,   153,   154,    -1,    -1,   183,   158,    -1,    -1,
     161,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   713,    -1,    -1,   176,    -1,   178,   153,   154,
      -1,    -1,    -1,   158,   185,   160,   161,    -1,    -1,    -1,
     731,   732,    -1,    -1,    -1,    -1,    -1,   738,    -1,   740,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    32,   753,    -1,    -1,    -1,   757,    -1,    -1,    -1,
     761,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,   778,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   174,   859,   176,
     861,   862,   863,   864,   865,   182,    -1,   868,   869,    -1,
     871,    -1,   153,   154,    -1,    56,    57,   158,    -1,   880,
     161,    -1,    -1,     1,    -1,    -1,    -1,    -1,     6,    -1,
       8,     9,    10,    -1,    12,   176,    14,    15,    16,    17,
      -1,   182,    -1,    -1,   185,   906,    -1,    25,    26,    27,
      28,    -1,   913,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    -1,    -1,    44,   928,    46,    47,
      48,    -1,    50,    51,   115,    -1,    54,    55,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   153,   154,    92,    93,    -1,   158,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,   180,   181,   182,   183,   184,     1,    -1,    -1,
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
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,
      -1,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,   178,    -1,   180,   181,   182,   183,
     184,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    49,    -1,    51,   115,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   153,   154,    92,    -1,    -1,
     158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      32,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    56,    57,    -1,    -1,   153,   154,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    14,    15,    16,    17,    -1,
      -1,   176,    -1,   178,    -1,   180,   181,    26,   183,    -1,
     185,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,   153,   154,    92,    -1,    -1,   158,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    32,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      56,    57,    -1,    -1,   153,   154,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    14,    15,    16,    17,    -1,    -1,   176,    -1,   178,
      -1,   180,   181,    26,   183,    -1,   185,    -1,    32,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,   115,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,   153,   154,    92,
      -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    32,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,   153,
     154,    56,    57,    -1,   158,   158,   160,   161,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,    14,    15,    16,
      17,    -1,   176,   176,    -1,   178,    -1,   180,   181,    26,
     183,   185,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,   114,
     115,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,   153,   154,
      -1,    -1,    -1,   158,    -1,    -1,   161,    -1,   105,   106,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   176,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
     185,    -1,   129,    -1,    56,    57,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,   153,   154,    92,    -1,    -1,   158,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    32,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,   185,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    56,    57,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    26,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,
      41,   180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,   153,
     154,    92,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    14,
      15,    16,    17,    -1,    -1,   176,    -1,   178,   179,   180,
     181,    26,   183,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,   120,   121,    -1,   113,    -1,
     115,   116,   117,   118,   129,    -1,    -1,   122,    -1,   124,
     125,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,   161,   162,   163,   164,
     165,    38,    -1,   178,    41,   180,   181,    44,   183,   174,
      47,   176,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    56,    57,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,   153,   154,    92,    -1,    -1,   158,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    32,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    56,    57,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
     159,    -1,   161,   162,   163,    26,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,
      41,   180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,   153,
     154,    92,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    32,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      56,    57,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    26,   165,    -1,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    38,   177,   178,    41,   180,
     181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,   115,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,   153,   154,    92,
      -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    32,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    56,    57,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    21,    -1,   161,   162,
     163,    26,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   178,    41,   180,   181,    44,
     183,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   153,   154,    92,    -1,    -1,
     158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    32,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    56,    57,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   178,    41,   180,   181,    44,   183,    -1,
      47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,   153,   154,    92,    -1,    -1,   158,    -1,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    32,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,
      -1,   168,   169,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    26,   183,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,   153,
     154,    92,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,    32,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    56,    57,
      -1,   153,   154,    -1,    -1,    -1,   158,    -1,   160,   161,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    14,
      15,    16,    17,    -1,   176,    -1,    -1,   178,   179,   180,
     181,    26,   183,   185,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   153,   154,    92,    -1,    -1,
     158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,   176,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,
      -1,    -1,   158,    -1,    -1,   161,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    14,    15,    16,    17,    -1,
     176,    -1,    -1,   178,   179,   180,   181,    26,   183,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    14,    15,    16,    17,    -1,    -1,    -1,    -1,   178,
     179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   178,   179,   180,   181,    26,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    14,    15,    16,    17,    -1,    -1,    -1,    -1,   178,
     179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,   161,   162,
     163,    26,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   178,    41,   180,   181,    44,
     183,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   178,    41,   180,   181,    44,   183,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,   180,   181,    -1,   183,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    20,    21,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,
      -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      20,    21,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,
      -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    20,
      21,    -1,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    20,    21,
      -1,   122,   123,   124,   125,   126,   127,    -1,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,    -1,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    20,    21,    -1,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,   174,    -1,   176,   103,   104,   105,   106,   107,   182,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   174,    -1,   176,   103,   104,
     105,   106,   107,   182,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,   176,    20,    21,   179,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    20,
      21,   179,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    20,
      21,   179,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,    20,    21,   179,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,    20,    21,   179,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,    20,    21,   179,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     174,    -1,   176,    -1,    -1,   179,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,   174,    -1,   176,
     177,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   103,   104,   105,   106,
     107,    20,    21,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    20,    21,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      20,    21,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,   128,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   174,    -1,   176,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   174,    -1,
     176,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   174,    -1,   176,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   174,    -1,   176,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     174,    -1,   176,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   174,    -1,   176,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   174,    -1,
     176,   103,   104,    -1,    -1,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    20,    21,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   159,   160,   161,   162,
     163,   164,   165,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,   174,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    94,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,   145,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,   145,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    45,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    59,   145,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,   145,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    94,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   129,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,   168,   169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   187,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    70,    91,   176,   182,   188,   196,   197,   201,   221,
     225,   240,   297,   302,   303,   307,   343,   345,   145,   216,
     217,   139,   202,   203,   145,   198,   199,   145,   183,   145,
     346,   344,    33,   129,   145,   218,   219,   220,   232,     4,
       5,     7,   306,    58,   296,   153,   152,   155,   152,    21,
      53,   164,   174,   200,   155,   183,   145,   300,   301,   145,
     145,   145,   129,   178,   152,   177,    34,   105,   106,   145,
     224,   226,   227,   145,   305,   183,   298,    14,    15,   139,
     143,   145,   146,   178,   190,   216,   139,   203,   145,   145,
     145,    65,    69,    86,    87,    88,    89,   311,   299,   153,
     184,   300,   183,   183,   145,   217,   219,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   124,   125,   126,   127,   130,
     131,   132,   133,   134,   135,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   168,   169,    34,    34,   178,
     222,   183,   228,   155,   304,   308,   295,     1,   147,   189,
      14,    15,   139,   143,   145,   190,   214,   215,   200,   183,
     145,   184,    24,    30,    45,    59,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    94,   232,   310,
     312,   313,   315,   318,   145,   314,   145,   288,   289,   179,
     136,   179,   279,   284,   285,   155,   223,   230,   232,   183,
     145,   184,   294,   309,   182,   147,   148,   152,   179,   299,
     153,   182,   159,   159,   159,   159,   178,   159,   159,   159,
     159,   159,   159,    32,    56,    57,   115,   153,   154,   158,
     161,   176,   182,   185,   182,   155,   182,   217,    46,   291,
     309,   179,   182,   318,     1,     8,     9,    10,    12,    14,
      15,    16,    17,    25,    26,    27,    28,    37,    38,    40,
      41,    44,    47,    48,    50,    51,    54,    55,    60,    70,
      92,    93,   105,   106,   120,   121,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   146,   161,   162,   163,   165,
     168,   169,   178,   180,   181,   182,   184,   192,   194,   204,
     205,   208,   209,   210,   211,   212,   213,   228,   229,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   249,   258,   262,   263,   264,   265,   266,   267,   268,
     273,   276,   310,   347,   351,   356,   358,   360,   282,   152,
     155,   158,   293,   214,   184,   276,   321,   323,   325,   319,
     145,   316,   327,   329,   331,   333,   335,   337,   339,   341,
      32,    32,   158,   176,   185,   177,   276,   145,   184,   318,
     184,   288,   177,   291,   114,   153,   155,   158,   290,   284,
     276,   309,   318,   347,   178,   114,   182,   231,   264,   276,
     228,   276,   178,   276,   159,   159,   178,   182,   159,   139,
      54,   276,   228,   159,   114,   231,   276,   276,   276,   181,
     262,   262,    12,   318,    12,   318,   276,   353,   357,   191,
     276,   276,   276,   232,   276,   276,   276,   181,    36,   182,
     182,   276,   145,   178,   182,   182,   182,   294,   221,   119,
     182,    20,    21,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   122,   123,   124,
     125,   126,   127,   130,   131,   132,   133,   134,   135,   153,
     154,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   174,   176,   178,   184,   221,   279,   281,   145,   318,
     114,   123,   153,   292,   182,   318,   318,   318,   318,   179,
     314,   318,   222,   318,   222,   318,   222,   145,   286,   287,
     318,   289,   177,   177,   177,   318,   231,   276,   228,    19,
     178,   232,   231,   276,   114,   153,   182,    13,   276,   228,
     243,   245,   276,   247,   155,   139,   318,   231,   276,   182,
     181,   309,    49,   145,   161,   178,   276,   348,   349,   350,
     352,   353,   354,   355,   309,   178,   349,   355,   128,   182,
     184,   148,   149,   189,   195,   179,   159,   232,   183,   228,
     176,   179,   256,   276,   136,   261,   262,    18,   145,   145,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   145,   120,   121,   276,
     276,   145,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   120,   121,   276,    21,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   145,   274,   276,   179,   256,     7,    42,   280,   182,
     153,   182,   292,   231,   276,   160,   182,   160,   160,   160,
     182,   160,   223,   160,   223,   160,   223,   155,   160,   182,
     160,   182,   153,   290,   256,   179,   256,    18,   159,   276,
     182,   228,   179,   318,   318,   179,   318,   160,   182,    19,
     262,   123,   290,   179,   152,   182,   350,   177,   152,   177,
     182,    19,   179,   350,   184,   276,   353,   184,   276,   147,
     193,   269,   271,   230,    11,    22,    23,   206,   207,   145,
     348,   152,   179,   114,   123,   153,   158,   259,   260,   222,
     159,   178,   276,   276,   177,   276,   276,   145,   155,     1,
     177,   179,    43,   280,   291,   231,   276,   182,   322,   318,
     326,   320,   317,   328,   160,   332,   160,   336,   160,   318,
     340,   286,   342,   276,   182,   179,   159,   145,   179,   160,
     160,   160,   261,   256,   276,   276,   349,   350,   145,   348,
     177,   177,   276,   177,   354,   256,   349,   184,   177,   150,
     147,   318,   222,   184,   228,   276,   177,   276,   145,   145,
     145,   145,   152,   177,   223,   277,   179,   256,   276,   275,
     283,   227,   182,   160,   330,   334,   338,   228,   250,   160,
     182,   244,   246,   248,   178,   182,   350,   177,   123,   290,
     177,   182,   350,   177,   160,   223,   228,   179,   259,   177,
     128,   228,   257,   318,   179,   226,   324,   318,    18,   276,
     145,   276,   276,   276,   179,   276,   276,   177,   177,   276,
     276,   276,   184,   270,   160,   207,   114,   276,   160,   182,
     160,   159,   179,   160,   179,   182,   359,   177,   359,   177,
     232,   272,   276,   278,   251,   252,    18,   276,    49,   177,
     184,   232,   179,   318,   159,   179,   276,   177,   177,   160,
     254,   253,   318,   179,   160,   255,   179
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   186,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   188,   189,   189,   190,   191,
     191,   191,   192,   193,   193,   195,   194,   196,   197,   198,
     198,   198,   199,   199,   200,   200,   201,   202,   202,   203,
     203,   204,   205,   205,   206,   206,   207,   207,   207,   208,
     208,   209,   210,   211,   212,   213,   214,   214,   214,   214,
     214,   214,   215,   215,   216,   216,   216,   216,   216,   216,
     216,   216,   217,   217,   218,   218,   219,   219,   220,   220,
     221,   221,   222,   222,   222,   223,   223,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     225,   226,   227,   228,   228,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   230,   230,   230,   231,   231,   231,
     232,   232,   232,   233,   234,   234,   234,   234,   235,   236,
     237,   237,   237,   237,   237,   238,   238,   238,   238,   239,
     240,   240,   241,   243,   244,   242,   245,   246,   242,   247,
     248,   242,   249,   249,   249,   250,   251,   249,   252,   253,
     249,   254,   255,   249,   256,   256,   257,   257,   257,   258,
     258,   258,   259,   259,   259,   259,   260,   260,   261,   261,
     262,   262,   263,   263,   263,   263,   263,   263,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   265,   265,   265,
     265,   266,   267,   267,   268,   269,   270,   268,   271,   272,
     268,   273,   274,   275,   273,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   277,
     278,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   279,   279,   280,   280,   281,   282,
     282,   283,   282,   282,   284,   284,   285,   285,   286,   286,
     287,   287,   288,   289,   289,   290,   290,   291,   291,   291,
     291,   291,   291,   292,   292,   292,   293,   293,   294,   294,
     294,   294,   294,   295,   295,   296,   296,   297,   298,   297,
     299,   299,   299,   300,   301,   301,   302,   303,   303,   304,
     304,   305,   306,   306,   308,   307,   309,   309,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   311,   311,   311,   311,   311,   311,   312,
     313,   313,   314,   314,   316,   317,   315,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   319,   320,   318,   318,   321,
     322,   318,   323,   324,   318,   325,   326,   318,   318,   327,
     328,   318,   329,   330,   318,   318,   331,   332,   318,   333,
     334,   318,   318,   335,   336,   318,   337,   338,   318,   339,
     340,   318,   341,   342,   318,   344,   343,   346,   345,   347,
     347,   347,   347,   348,   348,   348,   348,   349,   349,   350,
     350,   351,   351,   351,   351,   351,   351,   352,   352,   353,
     354,   354,   355,   355,   356,   356,   357,   357,   358,   359,
     359,   360,   360
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3453 "ds_parser.cpp"
        break;

    case 189: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3459 "ds_parser.cpp"
        break;

    case 190: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3465 "ds_parser.cpp"
        break;

    case 191: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3471 "ds_parser.cpp"
        break;

    case 192: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3477 "ds_parser.cpp"
        break;

    case 194: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3483 "ds_parser.cpp"
        break;

    case 198: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3489 "ds_parser.cpp"
        break;

    case 204: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3495 "ds_parser.cpp"
        break;

    case 205: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 207: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3507 "ds_parser.cpp"
        break;

    case 209: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3513 "ds_parser.cpp"
        break;

    case 210: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3519 "ds_parser.cpp"
        break;

    case 211: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3525 "ds_parser.cpp"
        break;

    case 212: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3531 "ds_parser.cpp"
        break;

    case 213: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3537 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3543 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3549 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3555 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3561 "ds_parser.cpp"
        break;

    case 218: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3567 "ds_parser.cpp"
        break;

    case 219: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3573 "ds_parser.cpp"
        break;

    case 220: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3579 "ds_parser.cpp"
        break;

    case 221: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3585 "ds_parser.cpp"
        break;

    case 222: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3591 "ds_parser.cpp"
        break;

    case 223: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3597 "ds_parser.cpp"
        break;

    case 224: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3603 "ds_parser.cpp"
        break;

    case 228: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3609 "ds_parser.cpp"
        break;

    case 229: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3615 "ds_parser.cpp"
        break;

    case 230: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3621 "ds_parser.cpp"
        break;

    case 231: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3627 "ds_parser.cpp"
        break;

    case 232: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3633 "ds_parser.cpp"
        break;

    case 233: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3639 "ds_parser.cpp"
        break;

    case 234: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3645 "ds_parser.cpp"
        break;

    case 235: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 236: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3657 "ds_parser.cpp"
        break;

    case 237: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 238: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3669 "ds_parser.cpp"
        break;

    case 239: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 241: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 242: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 249: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 256: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 257: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
        break;

    case 259: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3711 "ds_parser.cpp"
        break;

    case 260: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3717 "ds_parser.cpp"
        break;

    case 261: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3723 "ds_parser.cpp"
        break;

    case 262: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3729 "ds_parser.cpp"
        break;

    case 263: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3735 "ds_parser.cpp"
        break;

    case 264: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3741 "ds_parser.cpp"
        break;

    case 265: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3747 "ds_parser.cpp"
        break;

    case 266: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3753 "ds_parser.cpp"
        break;

    case 267: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3759 "ds_parser.cpp"
        break;

    case 268: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3765 "ds_parser.cpp"
        break;

    case 273: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3771 "ds_parser.cpp"
        break;

    case 276: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3777 "ds_parser.cpp"
        break;

    case 279: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3783 "ds_parser.cpp"
        break;

    case 281: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3789 "ds_parser.cpp"
        break;

    case 282: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3795 "ds_parser.cpp"
        break;

    case 284: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3801 "ds_parser.cpp"
        break;

    case 285: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3807 "ds_parser.cpp"
        break;

    case 286: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3813 "ds_parser.cpp"
        break;

    case 287: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3819 "ds_parser.cpp"
        break;

    case 288: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3825 "ds_parser.cpp"
        break;

    case 289: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3831 "ds_parser.cpp"
        break;

    case 291: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3837 "ds_parser.cpp"
        break;

    case 294: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3843 "ds_parser.cpp"
        break;

    case 295: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3849 "ds_parser.cpp"
        break;

    case 299: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3855 "ds_parser.cpp"
        break;

    case 304: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3861 "ds_parser.cpp"
        break;

    case 309: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3867 "ds_parser.cpp"
        break;

    case 312: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3873 "ds_parser.cpp"
        break;

    case 313: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3879 "ds_parser.cpp"
        break;

    case 314: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3885 "ds_parser.cpp"
        break;

    case 315: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3891 "ds_parser.cpp"
        break;

    case 318: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3897 "ds_parser.cpp"
        break;

    case 347: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3903 "ds_parser.cpp"
        break;

    case 348: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3909 "ds_parser.cpp"
        break;

    case 349: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3915 "ds_parser.cpp"
        break;

    case 350: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3921 "ds_parser.cpp"
        break;

    case 351: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3927 "ds_parser.cpp"
        break;

    case 352: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3933 "ds_parser.cpp"
        break;

    case 353: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3939 "ds_parser.cpp"
        break;

    case 354: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3945 "ds_parser.cpp"
        break;

    case 355: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3951 "ds_parser.cpp"
        break;

    case 356: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3957 "ds_parser.cpp"
        break;

    case 357: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3963 "ds_parser.cpp"
        break;

    case 358: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3969 "ds_parser.cpp"
        break;

    case 359: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3975 "ds_parser.cpp"
        break;

    case 360: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3981 "ds_parser.cpp"
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
#line 459 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4281 "ds_parser.cpp"
    break;

  case 16:
#line 471 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4287 "ds_parser.cpp"
    break;

  case 17:
#line 472 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4293 "ds_parser.cpp"
    break;

  case 18:
#line 476 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4299 "ds_parser.cpp"
    break;

  case 19:
#line 480 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4309 "ds_parser.cpp"
    break;

  case 20:
#line 485 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4323 "ds_parser.cpp"
    break;

  case 21:
#line 494 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4333 "ds_parser.cpp"
    break;

  case 22:
#line 502 "ds_parser.ypp"
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
#line 4351 "ds_parser.cpp"
    break;

  case 23:
#line 518 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4361 "ds_parser.cpp"
    break;

  case 24:
#line 523 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4371 "ds_parser.cpp"
    break;

  case 25:
#line 531 "ds_parser.ypp"
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
#line 4398 "ds_parser.cpp"
    break;

  case 26:
#line 552 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4410 "ds_parser.cpp"
    break;

  case 27:
#line 561 "ds_parser.ypp"
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
#line 4434 "ds_parser.cpp"
    break;

  case 29:
#line 587 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4442 "ds_parser.cpp"
    break;

  case 30:
#line 590 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4453 "ds_parser.cpp"
    break;

  case 31:
#line 596 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4464 "ds_parser.cpp"
    break;

  case 32:
#line 605 "ds_parser.ypp"
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
#line 4490 "ds_parser.cpp"
    break;

  case 33:
#line 626 "ds_parser.ypp"
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
#line 4515 "ds_parser.cpp"
    break;

  case 34:
#line 649 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4521 "ds_parser.cpp"
    break;

  case 35:
#line 650 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4527 "ds_parser.cpp"
    break;

  case 39:
#line 663 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4535 "ds_parser.cpp"
    break;

  case 40:
#line 666 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4543 "ds_parser.cpp"
    break;

  case 41:
#line 672 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4551 "ds_parser.cpp"
    break;

  case 42:
#line 678 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4559 "ds_parser.cpp"
    break;

  case 43:
#line 681 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4567 "ds_parser.cpp"
    break;

  case 44:
#line 687 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4573 "ds_parser.cpp"
    break;

  case 45:
#line 688 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4579 "ds_parser.cpp"
    break;

  case 46:
#line 692 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4585 "ds_parser.cpp"
    break;

  case 47:
#line 693 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4591 "ds_parser.cpp"
    break;

  case 48:
#line 694 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4602 "ds_parser.cpp"
    break;

  case 49:
#line 703 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4608 "ds_parser.cpp"
    break;

  case 50:
#line 704 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4614 "ds_parser.cpp"
    break;

  case 51:
#line 708 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4625 "ds_parser.cpp"
    break;

  case 52:
#line 717 "ds_parser.ypp"
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
#line 4643 "ds_parser.cpp"
    break;

  case 53:
#line 733 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4653 "ds_parser.cpp"
    break;

  case 54:
#line 741 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4665 "ds_parser.cpp"
    break;

  case 55:
#line 751 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4676 "ds_parser.cpp"
    break;

  case 56:
#line 760 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4682 "ds_parser.cpp"
    break;

  case 57:
#line 761 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4688 "ds_parser.cpp"
    break;

  case 58:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4694 "ds_parser.cpp"
    break;

  case 59:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4700 "ds_parser.cpp"
    break;

  case 60:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4706 "ds_parser.cpp"
    break;

  case 61:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4712 "ds_parser.cpp"
    break;

  case 62:
#line 769 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4722 "ds_parser.cpp"
    break;

  case 63:
#line 774 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4732 "ds_parser.cpp"
    break;

  case 64:
#line 782 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4738 "ds_parser.cpp"
    break;

  case 65:
#line 783 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4744 "ds_parser.cpp"
    break;

  case 66:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4750 "ds_parser.cpp"
    break;

  case 67:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4756 "ds_parser.cpp"
    break;

  case 68:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4762 "ds_parser.cpp"
    break;

  case 69:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4768 "ds_parser.cpp"
    break;

  case 70:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4774 "ds_parser.cpp"
    break;

  case 71:
#line 789 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4782 "ds_parser.cpp"
    break;

  case 72:
#line 795 "ds_parser.ypp"
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
#line 4800 "ds_parser.cpp"
    break;

  case 73:
#line 808 "ds_parser.ypp"
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
#line 4818 "ds_parser.cpp"
    break;

  case 74:
#line 824 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4824 "ds_parser.cpp"
    break;

  case 75:
#line 825 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4830 "ds_parser.cpp"
    break;

  case 76:
#line 829 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4842 "ds_parser.cpp"
    break;

  case 77:
#line 836 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4856 "ds_parser.cpp"
    break;

  case 78:
#line 848 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4865 "ds_parser.cpp"
    break;

  case 79:
#line 852 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4874 "ds_parser.cpp"
    break;

  case 80:
#line 859 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4880 "ds_parser.cpp"
    break;

  case 81:
#line 860 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4886 "ds_parser.cpp"
    break;

  case 82:
#line 864 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4892 "ds_parser.cpp"
    break;

  case 83:
#line 865 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4898 "ds_parser.cpp"
    break;

  case 84:
#line 866 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4904 "ds_parser.cpp"
    break;

  case 85:
#line 870 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4914 "ds_parser.cpp"
    break;

  case 86:
#line 875 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4923 "ds_parser.cpp"
    break;

  case 87:
#line 882 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4932 "ds_parser.cpp"
    break;

  case 88:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4938 "ds_parser.cpp"
    break;

  case 89:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4944 "ds_parser.cpp"
    break;

  case 90:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4950 "ds_parser.cpp"
    break;

  case 91:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4956 "ds_parser.cpp"
    break;

  case 92:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4962 "ds_parser.cpp"
    break;

  case 93:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4968 "ds_parser.cpp"
    break;

  case 94:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4974 "ds_parser.cpp"
    break;

  case 95:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4980 "ds_parser.cpp"
    break;

  case 96:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4986 "ds_parser.cpp"
    break;

  case 97:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4992 "ds_parser.cpp"
    break;

  case 98:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4998 "ds_parser.cpp"
    break;

  case 99:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5004 "ds_parser.cpp"
    break;

  case 100:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5010 "ds_parser.cpp"
    break;

  case 101:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5016 "ds_parser.cpp"
    break;

  case 102:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5022 "ds_parser.cpp"
    break;

  case 103:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5028 "ds_parser.cpp"
    break;

  case 104:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5034 "ds_parser.cpp"
    break;

  case 105:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5040 "ds_parser.cpp"
    break;

  case 106:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5046 "ds_parser.cpp"
    break;

  case 107:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5052 "ds_parser.cpp"
    break;

  case 108:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5058 "ds_parser.cpp"
    break;

  case 109:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5064 "ds_parser.cpp"
    break;

  case 110:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5070 "ds_parser.cpp"
    break;

  case 111:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5076 "ds_parser.cpp"
    break;

  case 112:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5082 "ds_parser.cpp"
    break;

  case 113:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5088 "ds_parser.cpp"
    break;

  case 114:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5094 "ds_parser.cpp"
    break;

  case 115:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5100 "ds_parser.cpp"
    break;

  case 116:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5106 "ds_parser.cpp"
    break;

  case 117:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5112 "ds_parser.cpp"
    break;

  case 118:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5118 "ds_parser.cpp"
    break;

  case 119:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5124 "ds_parser.cpp"
    break;

  case 120:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5130 "ds_parser.cpp"
    break;

  case 121:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5136 "ds_parser.cpp"
    break;

  case 122:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5142 "ds_parser.cpp"
    break;

  case 123:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5148 "ds_parser.cpp"
    break;

  case 124:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5154 "ds_parser.cpp"
    break;

  case 125:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5160 "ds_parser.cpp"
    break;

  case 126:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5166 "ds_parser.cpp"
    break;

  case 127:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5172 "ds_parser.cpp"
    break;

  case 128:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5178 "ds_parser.cpp"
    break;

  case 129:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5184 "ds_parser.cpp"
    break;

  case 130:
#line 931 "ds_parser.ypp"
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
#line 5208 "ds_parser.cpp"
    break;

  case 131:
#line 953 "ds_parser.ypp"
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
#line 5248 "ds_parser.cpp"
    break;

  case 132:
#line 991 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5257 "ds_parser.cpp"
    break;

  case 133:
#line 998 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5266 "ds_parser.cpp"
    break;

  case 134:
#line 1002 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5279 "ds_parser.cpp"
    break;

  case 135:
#line 1013 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5285 "ds_parser.cpp"
    break;

  case 136:
#line 1014 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5291 "ds_parser.cpp"
    break;

  case 137:
#line 1015 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5297 "ds_parser.cpp"
    break;

  case 138:
#line 1016 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5303 "ds_parser.cpp"
    break;

  case 139:
#line 1017 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5309 "ds_parser.cpp"
    break;

  case 140:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5315 "ds_parser.cpp"
    break;

  case 141:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5321 "ds_parser.cpp"
    break;

  case 142:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5327 "ds_parser.cpp"
    break;

  case 143:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5333 "ds_parser.cpp"
    break;

  case 144:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5339 "ds_parser.cpp"
    break;

  case 145:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5345 "ds_parser.cpp"
    break;

  case 146:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5351 "ds_parser.cpp"
    break;

  case 147:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5357 "ds_parser.cpp"
    break;

  case 148:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5363 "ds_parser.cpp"
    break;

  case 149:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5369 "ds_parser.cpp"
    break;

  case 150:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5375 "ds_parser.cpp"
    break;

  case 151:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5381 "ds_parser.cpp"
    break;

  case 152:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5387 "ds_parser.cpp"
    break;

  case 153:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5393 "ds_parser.cpp"
    break;

  case 154:
#line 1035 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5403 "ds_parser.cpp"
    break;

  case 155:
#line 1040 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5414 "ds_parser.cpp"
    break;

  case 156:
#line 1046 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5422 "ds_parser.cpp"
    break;

  case 157:
#line 1052 "ds_parser.ypp"
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
#line 5448 "ds_parser.cpp"
    break;

  case 158:
#line 1073 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5456 "ds_parser.cpp"
    break;

  case 159:
#line 1076 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5464 "ds_parser.cpp"
    break;

  case 160:
#line 1082 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5470 "ds_parser.cpp"
    break;

  case 161:
#line 1083 "ds_parser.ypp"
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
#line 5486 "ds_parser.cpp"
    break;

  case 162:
#line 1094 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5492 "ds_parser.cpp"
    break;

  case 163:
#line 1098 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5500 "ds_parser.cpp"
    break;

  case 164:
#line 1104 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5508 "ds_parser.cpp"
    break;

  case 165:
#line 1107 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5516 "ds_parser.cpp"
    break;

  case 166:
#line 1110 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5525 "ds_parser.cpp"
    break;

  case 167:
#line 1114 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5533 "ds_parser.cpp"
    break;

  case 168:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5539 "ds_parser.cpp"
    break;

  case 169:
#line 1124 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5545 "ds_parser.cpp"
    break;

  case 170:
#line 1128 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5553 "ds_parser.cpp"
    break;

  case 171:
#line 1131 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5561 "ds_parser.cpp"
    break;

  case 172:
#line 1134 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5571 "ds_parser.cpp"
    break;

  case 173:
#line 1139 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5579 "ds_parser.cpp"
    break;

  case 174:
#line 1142 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5589 "ds_parser.cpp"
    break;

  case 175:
#line 1150 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5597 "ds_parser.cpp"
    break;

  case 176:
#line 1153 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5607 "ds_parser.cpp"
    break;

  case 177:
#line 1158 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5615 "ds_parser.cpp"
    break;

  case 178:
#line 1161 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5625 "ds_parser.cpp"
    break;

  case 179:
#line 1169 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5633 "ds_parser.cpp"
    break;

  case 180:
#line 1175 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5639 "ds_parser.cpp"
    break;

  case 181:
#line 1176 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5645 "ds_parser.cpp"
    break;

  case 182:
#line 1180 "ds_parser.ypp"
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
#line 5681 "ds_parser.cpp"
    break;

  case 183:
#line 1214 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5687 "ds_parser.cpp"
    break;

  case 184:
#line 1214 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5693 "ds_parser.cpp"
    break;

  case 185:
#line 1214 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5701 "ds_parser.cpp"
    break;

  case 186:
#line 1217 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5707 "ds_parser.cpp"
    break;

  case 187:
#line 1217 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5713 "ds_parser.cpp"
    break;

  case 188:
#line 1217 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5723 "ds_parser.cpp"
    break;

  case 189:
#line 1222 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5729 "ds_parser.cpp"
    break;

  case 190:
#line 1222 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5735 "ds_parser.cpp"
    break;

  case 191:
#line 1222 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5745 "ds_parser.cpp"
    break;

  case 192:
#line 1230 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5754 "ds_parser.cpp"
    break;

  case 193:
#line 1234 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5764 "ds_parser.cpp"
    break;

  case 194:
#line 1239 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5775 "ds_parser.cpp"
    break;

  case 195:
#line 1245 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5781 "ds_parser.cpp"
    break;

  case 196:
#line 1245 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5787 "ds_parser.cpp"
    break;

  case 197:
#line 1245 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5796 "ds_parser.cpp"
    break;

  case 198:
#line 1249 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5802 "ds_parser.cpp"
    break;

  case 199:
#line 1249 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5808 "ds_parser.cpp"
    break;

  case 200:
#line 1249 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5818 "ds_parser.cpp"
    break;

  case 201:
#line 1254 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5824 "ds_parser.cpp"
    break;

  case 202:
#line 1254 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5830 "ds_parser.cpp"
    break;

  case 203:
#line 1254 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5841 "ds_parser.cpp"
    break;

  case 204:
#line 1263 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5849 "ds_parser.cpp"
    break;

  case 205:
#line 1266 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5857 "ds_parser.cpp"
    break;

  case 206:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5863 "ds_parser.cpp"
    break;

  case 207:
#line 1273 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5875 "ds_parser.cpp"
    break;

  case 208:
#line 1280 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5888 "ds_parser.cpp"
    break;

  case 209:
#line 1291 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5894 "ds_parser.cpp"
    break;

  case 210:
#line 1292 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5900 "ds_parser.cpp"
    break;

  case 211:
#line 1293 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5906 "ds_parser.cpp"
    break;

  case 212:
#line 1297 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5912 "ds_parser.cpp"
    break;

  case 213:
#line 1298 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5918 "ds_parser.cpp"
    break;

  case 214:
#line 1299 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5924 "ds_parser.cpp"
    break;

  case 215:
#line 1300 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5930 "ds_parser.cpp"
    break;

  case 216:
#line 1304 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5940 "ds_parser.cpp"
    break;

  case 217:
#line 1309 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5950 "ds_parser.cpp"
    break;

  case 218:
#line 1317 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5956 "ds_parser.cpp"
    break;

  case 219:
#line 1318 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5962 "ds_parser.cpp"
    break;

  case 220:
#line 1322 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5972 "ds_parser.cpp"
    break;

  case 221:
#line 1328 "ds_parser.ypp"
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
#line 6037 "ds_parser.cpp"
    break;

  case 222:
#line 1391 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6043 "ds_parser.cpp"
    break;

  case 223:
#line 1392 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6049 "ds_parser.cpp"
    break;

  case 224:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6055 "ds_parser.cpp"
    break;

  case 225:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6061 "ds_parser.cpp"
    break;

  case 226:
#line 1395 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6067 "ds_parser.cpp"
    break;

  case 227:
#line 1396 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6073 "ds_parser.cpp"
    break;

  case 228:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6079 "ds_parser.cpp"
    break;

  case 229:
#line 1401 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6085 "ds_parser.cpp"
    break;

  case 230:
#line 1402 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6091 "ds_parser.cpp"
    break;

  case 231:
#line 1403 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6097 "ds_parser.cpp"
    break;

  case 232:
#line 1404 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6103 "ds_parser.cpp"
    break;

  case 233:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6109 "ds_parser.cpp"
    break;

  case 234:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6115 "ds_parser.cpp"
    break;

  case 235:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6121 "ds_parser.cpp"
    break;

  case 236:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6127 "ds_parser.cpp"
    break;

  case 237:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6133 "ds_parser.cpp"
    break;

  case 238:
#line 1410 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6139 "ds_parser.cpp"
    break;

  case 239:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6145 "ds_parser.cpp"
    break;

  case 240:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6151 "ds_parser.cpp"
    break;

  case 241:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6157 "ds_parser.cpp"
    break;

  case 242:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6163 "ds_parser.cpp"
    break;

  case 243:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6169 "ds_parser.cpp"
    break;

  case 244:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6175 "ds_parser.cpp"
    break;

  case 245:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6181 "ds_parser.cpp"
    break;

  case 246:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6187 "ds_parser.cpp"
    break;

  case 247:
#line 1422 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6193 "ds_parser.cpp"
    break;

  case 248:
#line 1423 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6199 "ds_parser.cpp"
    break;

  case 249:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6205 "ds_parser.cpp"
    break;

  case 250:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6211 "ds_parser.cpp"
    break;

  case 251:
#line 1429 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6223 "ds_parser.cpp"
    break;

  case 252:
#line 1440 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6233 "ds_parser.cpp"
    break;

  case 253:
#line 1445 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6245 "ds_parser.cpp"
    break;

  case 254:
#line 1455 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6254 "ds_parser.cpp"
    break;

  case 255:
#line 1459 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6260 "ds_parser.cpp"
    break;

  case 256:
#line 1459 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6266 "ds_parser.cpp"
    break;

  case 257:
#line 1459 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6278 "ds_parser.cpp"
    break;

  case 258:
#line 1466 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6284 "ds_parser.cpp"
    break;

  case 259:
#line 1466 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6290 "ds_parser.cpp"
    break;

  case 260:
#line 1466 "ds_parser.ypp"
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
#line 6306 "ds_parser.cpp"
    break;

  case 261:
#line 1480 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6315 "ds_parser.cpp"
    break;

  case 262:
#line 1484 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6321 "ds_parser.cpp"
    break;

  case 263:
#line 1484 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6327 "ds_parser.cpp"
    break;

  case 264:
#line 1484 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6337 "ds_parser.cpp"
    break;

  case 265:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6343 "ds_parser.cpp"
    break;

  case 266:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6349 "ds_parser.cpp"
    break;

  case 267:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6355 "ds_parser.cpp"
    break;

  case 268:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6361 "ds_parser.cpp"
    break;

  case 269:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6367 "ds_parser.cpp"
    break;

  case 270:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6373 "ds_parser.cpp"
    break;

  case 271:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6379 "ds_parser.cpp"
    break;

  case 272:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6385 "ds_parser.cpp"
    break;

  case 273:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6391 "ds_parser.cpp"
    break;

  case 274:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6397 "ds_parser.cpp"
    break;

  case 275:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6403 "ds_parser.cpp"
    break;

  case 276:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6409 "ds_parser.cpp"
    break;

  case 277:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6415 "ds_parser.cpp"
    break;

  case 278:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6421 "ds_parser.cpp"
    break;

  case 279:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6427 "ds_parser.cpp"
    break;

  case 280:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6433 "ds_parser.cpp"
    break;

  case 281:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6439 "ds_parser.cpp"
    break;

  case 282:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6445 "ds_parser.cpp"
    break;

  case 283:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6451 "ds_parser.cpp"
    break;

  case 284:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6457 "ds_parser.cpp"
    break;

  case 285:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6463 "ds_parser.cpp"
    break;

  case 286:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6469 "ds_parser.cpp"
    break;

  case 287:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6475 "ds_parser.cpp"
    break;

  case 288:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6481 "ds_parser.cpp"
    break;

  case 289:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6487 "ds_parser.cpp"
    break;

  case 290:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6493 "ds_parser.cpp"
    break;

  case 291:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6499 "ds_parser.cpp"
    break;

  case 292:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6505 "ds_parser.cpp"
    break;

  case 293:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6511 "ds_parser.cpp"
    break;

  case 294:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6517 "ds_parser.cpp"
    break;

  case 295:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6523 "ds_parser.cpp"
    break;

  case 296:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6529 "ds_parser.cpp"
    break;

  case 297:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6535 "ds_parser.cpp"
    break;

  case 298:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6541 "ds_parser.cpp"
    break;

  case 299:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6547 "ds_parser.cpp"
    break;

  case 300:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6553 "ds_parser.cpp"
    break;

  case 301:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6559 "ds_parser.cpp"
    break;

  case 302:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6565 "ds_parser.cpp"
    break;

  case 303:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6571 "ds_parser.cpp"
    break;

  case 304:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6577 "ds_parser.cpp"
    break;

  case 305:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6583 "ds_parser.cpp"
    break;

  case 306:
#line 1533 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6589 "ds_parser.cpp"
    break;

  case 307:
#line 1534 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6595 "ds_parser.cpp"
    break;

  case 308:
#line 1535 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6604 "ds_parser.cpp"
    break;

  case 309:
#line 1539 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6613 "ds_parser.cpp"
    break;

  case 310:
#line 1543 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6621 "ds_parser.cpp"
    break;

  case 311:
#line 1546 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6629 "ds_parser.cpp"
    break;

  case 312:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6635 "ds_parser.cpp"
    break;

  case 313:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6641 "ds_parser.cpp"
    break;

  case 314:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6647 "ds_parser.cpp"
    break;

  case 315:
#line 1552 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6661 "ds_parser.cpp"
    break;

  case 316:
#line 1561 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6675 "ds_parser.cpp"
    break;

  case 317:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6681 "ds_parser.cpp"
    break;

  case 318:
#line 1571 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6689 "ds_parser.cpp"
    break;

  case 319:
#line 1574 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6695 "ds_parser.cpp"
    break;

  case 320:
#line 1574 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6701 "ds_parser.cpp"
    break;

  case 321:
#line 1574 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6709 "ds_parser.cpp"
    break;

  case 322:
#line 1577 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6718 "ds_parser.cpp"
    break;

  case 323:
#line 1581 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6727 "ds_parser.cpp"
    break;

  case 324:
#line 1585 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6736 "ds_parser.cpp"
    break;

  case 325:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6742 "ds_parser.cpp"
    break;

  case 326:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6748 "ds_parser.cpp"
    break;

  case 327:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6754 "ds_parser.cpp"
    break;

  case 328:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6760 "ds_parser.cpp"
    break;

  case 329:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6766 "ds_parser.cpp"
    break;

  case 330:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6772 "ds_parser.cpp"
    break;

  case 331:
#line 1595 "ds_parser.ypp"
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
#line 6787 "ds_parser.cpp"
    break;

  case 332:
#line 1605 "ds_parser.ypp"
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
#line 6802 "ds_parser.cpp"
    break;

  case 333:
#line 1615 "ds_parser.ypp"
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
#line 6859 "ds_parser.cpp"
    break;

  case 334:
#line 1670 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6865 "ds_parser.cpp"
    break;

  case 335:
#line 1671 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6871 "ds_parser.cpp"
    break;

  case 336:
#line 1675 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6877 "ds_parser.cpp"
    break;

  case 337:
#line 1676 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6883 "ds_parser.cpp"
    break;

  case 338:
#line 1680 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6893 "ds_parser.cpp"
    break;

  case 339:
#line 1688 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6901 "ds_parser.cpp"
    break;

  case 340:
#line 1691 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6910 "ds_parser.cpp"
    break;

  case 341:
#line 1696 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6916 "ds_parser.cpp"
    break;

  case 342:
#line 1696 "ds_parser.ypp"
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
#line 6965 "ds_parser.cpp"
    break;

  case 343:
#line 1740 "ds_parser.ypp"
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
#line 7026 "ds_parser.cpp"
    break;

  case 344:
#line 1799 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7036 "ds_parser.cpp"
    break;

  case 345:
#line 1804 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7046 "ds_parser.cpp"
    break;

  case 346:
#line 1812 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7052 "ds_parser.cpp"
    break;

  case 347:
#line 1813 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7058 "ds_parser.cpp"
    break;

  case 348:
#line 1817 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7066 "ds_parser.cpp"
    break;

  case 349:
#line 1820 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7077 "ds_parser.cpp"
    break;

  case 350:
#line 1829 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7083 "ds_parser.cpp"
    break;

  case 351:
#line 1830 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7089 "ds_parser.cpp"
    break;

  case 352:
#line 1834 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7100 "ds_parser.cpp"
    break;

  case 353:
#line 1843 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7106 "ds_parser.cpp"
    break;

  case 354:
#line 1844 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7112 "ds_parser.cpp"
    break;

  case 355:
#line 1849 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7118 "ds_parser.cpp"
    break;

  case 356:
#line 1850 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7124 "ds_parser.cpp"
    break;

  case 357:
#line 1854 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7135 "ds_parser.cpp"
    break;

  case 358:
#line 1860 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7146 "ds_parser.cpp"
    break;

  case 359:
#line 1866 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7154 "ds_parser.cpp"
    break;

  case 360:
#line 1869 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7163 "ds_parser.cpp"
    break;

  case 361:
#line 1873 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7174 "ds_parser.cpp"
    break;

  case 362:
#line 1879 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7185 "ds_parser.cpp"
    break;

  case 363:
#line 1888 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7191 "ds_parser.cpp"
    break;

  case 364:
#line 1889 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7197 "ds_parser.cpp"
    break;

  case 365:
#line 1890 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7203 "ds_parser.cpp"
    break;

  case 366:
#line 1894 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7209 "ds_parser.cpp"
    break;

  case 367:
#line 1895 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7215 "ds_parser.cpp"
    break;

  case 368:
#line 1899 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7223 "ds_parser.cpp"
    break;

  case 369:
#line 1902 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7233 "ds_parser.cpp"
    break;

  case 370:
#line 1907 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7243 "ds_parser.cpp"
    break;

  case 371:
#line 1912 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7256 "ds_parser.cpp"
    break;

  case 372:
#line 1920 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7269 "ds_parser.cpp"
    break;

  case 373:
#line 1931 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7277 "ds_parser.cpp"
    break;

  case 374:
#line 1934 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7286 "ds_parser.cpp"
    break;

  case 375:
#line 1941 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7292 "ds_parser.cpp"
    break;

  case 376:
#line 1942 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7298 "ds_parser.cpp"
    break;

  case 377:
#line 1946 "ds_parser.ypp"
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
#line 7330 "ds_parser.cpp"
    break;

  case 378:
#line 1973 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7339 "ds_parser.cpp"
    break;

  case 379:
#line 1976 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7350 "ds_parser.cpp"
    break;

  case 380:
#line 1985 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7358 "ds_parser.cpp"
    break;

  case 381:
#line 1988 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7372 "ds_parser.cpp"
    break;

  case 382:
#line 1997 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7386 "ds_parser.cpp"
    break;

  case 383:
#line 2010 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7400 "ds_parser.cpp"
    break;

  case 387:
#line 2031 "ds_parser.ypp"
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
#line 7416 "ds_parser.cpp"
    break;

  case 388:
#line 2042 "ds_parser.ypp"
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
#line 7433 "ds_parser.cpp"
    break;

  case 389:
#line 2057 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7439 "ds_parser.cpp"
    break;

  case 390:
#line 2058 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7445 "ds_parser.cpp"
    break;

  case 391:
#line 2062 "ds_parser.ypp"
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
#line 7484 "ds_parser.cpp"
    break;

  case 392:
#line 2099 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7490 "ds_parser.cpp"
    break;

  case 393:
#line 2100 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7496 "ds_parser.cpp"
    break;

  case 394:
#line 2104 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7502 "ds_parser.cpp"
    break;

  case 395:
#line 2104 "ds_parser.ypp"
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
#line 7588 "ds_parser.cpp"
    break;

  case 396:
#line 2188 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7600 "ds_parser.cpp"
    break;

  case 397:
#line 2195 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7611 "ds_parser.cpp"
    break;

  case 398:
#line 2204 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7617 "ds_parser.cpp"
    break;

  case 399:
#line 2205 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7623 "ds_parser.cpp"
    break;

  case 400:
#line 2206 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7629 "ds_parser.cpp"
    break;

  case 401:
#line 2207 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7635 "ds_parser.cpp"
    break;

  case 402:
#line 2208 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7641 "ds_parser.cpp"
    break;

  case 403:
#line 2209 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7647 "ds_parser.cpp"
    break;

  case 404:
#line 2210 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7653 "ds_parser.cpp"
    break;

  case 405:
#line 2211 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7659 "ds_parser.cpp"
    break;

  case 406:
#line 2212 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7665 "ds_parser.cpp"
    break;

  case 407:
#line 2213 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7671 "ds_parser.cpp"
    break;

  case 408:
#line 2214 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7677 "ds_parser.cpp"
    break;

  case 409:
#line 2215 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7683 "ds_parser.cpp"
    break;

  case 410:
#line 2216 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7689 "ds_parser.cpp"
    break;

  case 411:
#line 2217 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7695 "ds_parser.cpp"
    break;

  case 412:
#line 2218 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7701 "ds_parser.cpp"
    break;

  case 413:
#line 2219 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7707 "ds_parser.cpp"
    break;

  case 414:
#line 2220 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7713 "ds_parser.cpp"
    break;

  case 415:
#line 2221 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7719 "ds_parser.cpp"
    break;

  case 416:
#line 2222 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7725 "ds_parser.cpp"
    break;

  case 417:
#line 2223 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7731 "ds_parser.cpp"
    break;

  case 418:
#line 2224 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7737 "ds_parser.cpp"
    break;

  case 419:
#line 2225 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7743 "ds_parser.cpp"
    break;

  case 420:
#line 2226 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7749 "ds_parser.cpp"
    break;

  case 421:
#line 2227 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7755 "ds_parser.cpp"
    break;

  case 422:
#line 2228 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7761 "ds_parser.cpp"
    break;

  case 423:
#line 2232 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7767 "ds_parser.cpp"
    break;

  case 424:
#line 2233 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7773 "ds_parser.cpp"
    break;

  case 425:
#line 2234 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7779 "ds_parser.cpp"
    break;

  case 426:
#line 2235 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7785 "ds_parser.cpp"
    break;

  case 427:
#line 2236 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7791 "ds_parser.cpp"
    break;

  case 428:
#line 2237 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7797 "ds_parser.cpp"
    break;

  case 429:
#line 2241 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7810 "ds_parser.cpp"
    break;

  case 430:
#line 2252 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7819 "ds_parser.cpp"
    break;

  case 431:
#line 2256 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7831 "ds_parser.cpp"
    break;

  case 432:
#line 2266 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7843 "ds_parser.cpp"
    break;

  case 433:
#line 2273 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7854 "ds_parser.cpp"
    break;

  case 434:
#line 2282 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7860 "ds_parser.cpp"
    break;

  case 435:
#line 2282 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7866 "ds_parser.cpp"
    break;

  case 436:
#line 2282 "ds_parser.ypp"
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
#line 7881 "ds_parser.cpp"
    break;

  case 437:
#line 2295 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7887 "ds_parser.cpp"
    break;

  case 438:
#line 2296 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7893 "ds_parser.cpp"
    break;

  case 439:
#line 2297 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7899 "ds_parser.cpp"
    break;

  case 440:
#line 2298 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7905 "ds_parser.cpp"
    break;

  case 441:
#line 2299 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl(Type::fakeContext); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7911 "ds_parser.cpp"
    break;

  case 442:
#line 2300 "ds_parser.ypp"
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
#line 7930 "ds_parser.cpp"
    break;

  case 443:
#line 2314 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7941 "ds_parser.cpp"
    break;

  case 444:
#line 2320 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7950 "ds_parser.cpp"
    break;

  case 445:
#line 2324 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7959 "ds_parser.cpp"
    break;

  case 446:
#line 2328 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7969 "ds_parser.cpp"
    break;

  case 447:
#line 2333 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7979 "ds_parser.cpp"
    break;

  case 448:
#line 2338 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7989 "ds_parser.cpp"
    break;

  case 449:
#line 2343 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7999 "ds_parser.cpp"
    break;

  case 450:
#line 2348 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8008 "ds_parser.cpp"
    break;

  case 451:
#line 2352 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8017 "ds_parser.cpp"
    break;

  case 452:
#line 2356 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8027 "ds_parser.cpp"
    break;

  case 453:
#line 2361 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8036 "ds_parser.cpp"
    break;

  case 454:
#line 2365 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8046 "ds_parser.cpp"
    break;

  case 455:
#line 2370 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8052 "ds_parser.cpp"
    break;

  case 456:
#line 2370 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8058 "ds_parser.cpp"
    break;

  case 457:
#line 2370 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8069 "ds_parser.cpp"
    break;

  case 458:
#line 2376 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8081 "ds_parser.cpp"
    break;

  case 459:
#line 2383 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8087 "ds_parser.cpp"
    break;

  case 460:
#line 2383 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8093 "ds_parser.cpp"
    break;

  case 461:
#line 2383 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8103 "ds_parser.cpp"
    break;

  case 462:
#line 2388 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8109 "ds_parser.cpp"
    break;

  case 463:
#line 2388 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8115 "ds_parser.cpp"
    break;

  case 464:
#line 2388 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8126 "ds_parser.cpp"
    break;

  case 465:
#line 2394 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8132 "ds_parser.cpp"
    break;

  case 466:
#line 2394 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8138 "ds_parser.cpp"
    break;

  case 467:
#line 2394 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8148 "ds_parser.cpp"
    break;

  case 468:
#line 2399 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8157 "ds_parser.cpp"
    break;

  case 469:
#line 2403 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8163 "ds_parser.cpp"
    break;

  case 470:
#line 2403 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8169 "ds_parser.cpp"
    break;

  case 471:
#line 2403 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8179 "ds_parser.cpp"
    break;

  case 472:
#line 2408 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8185 "ds_parser.cpp"
    break;

  case 473:
#line 2408 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8191 "ds_parser.cpp"
    break;

  case 474:
#line 2408 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8205 "ds_parser.cpp"
    break;

  case 475:
#line 2417 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8214 "ds_parser.cpp"
    break;

  case 476:
#line 2421 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8220 "ds_parser.cpp"
    break;

  case 477:
#line 2421 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8226 "ds_parser.cpp"
    break;

  case 478:
#line 2421 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8236 "ds_parser.cpp"
    break;

  case 479:
#line 2426 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8242 "ds_parser.cpp"
    break;

  case 480:
#line 2426 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8248 "ds_parser.cpp"
    break;

  case 481:
#line 2426 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8262 "ds_parser.cpp"
    break;

  case 482:
#line 2435 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8271 "ds_parser.cpp"
    break;

  case 483:
#line 2439 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8277 "ds_parser.cpp"
    break;

  case 484:
#line 2439 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8283 "ds_parser.cpp"
    break;

  case 485:
#line 2439 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8293 "ds_parser.cpp"
    break;

  case 486:
#line 2444 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8299 "ds_parser.cpp"
    break;

  case 487:
#line 2444 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8305 "ds_parser.cpp"
    break;

  case 488:
#line 2444 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8319 "ds_parser.cpp"
    break;

  case 489:
#line 2453 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8325 "ds_parser.cpp"
    break;

  case 490:
#line 2453 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8331 "ds_parser.cpp"
    break;

  case 491:
#line 2453 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8342 "ds_parser.cpp"
    break;

  case 492:
#line 2459 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8348 "ds_parser.cpp"
    break;

  case 493:
#line 2459 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8354 "ds_parser.cpp"
    break;

  case 494:
#line 2459 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8365 "ds_parser.cpp"
    break;

  case 495:
#line 2468 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8371 "ds_parser.cpp"
    break;

  case 496:
#line 2468 "ds_parser.ypp"
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
#line 8388 "ds_parser.cpp"
    break;

  case 497:
#line 2483 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8394 "ds_parser.cpp"
    break;

  case 498:
#line 2483 "ds_parser.ypp"
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
#line 8415 "ds_parser.cpp"
    break;

  case 499:
#line 2503 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8421 "ds_parser.cpp"
    break;

  case 500:
#line 2504 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8427 "ds_parser.cpp"
    break;

  case 501:
#line 2505 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8433 "ds_parser.cpp"
    break;

  case 502:
#line 2506 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8439 "ds_parser.cpp"
    break;

  case 503:
#line 2510 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8451 "ds_parser.cpp"
    break;

  case 504:
#line 2517 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8463 "ds_parser.cpp"
    break;

  case 505:
#line 2524 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8474 "ds_parser.cpp"
    break;

  case 506:
#line 2530 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8485 "ds_parser.cpp"
    break;

  case 507:
#line 2539 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8495 "ds_parser.cpp"
    break;

  case 508:
#line 2544 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8504 "ds_parser.cpp"
    break;

  case 509:
#line 2551 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8510 "ds_parser.cpp"
    break;

  case 510:
#line 2552 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8516 "ds_parser.cpp"
    break;

  case 511:
#line 2556 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8527 "ds_parser.cpp"
    break;

  case 512:
#line 2562 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8539 "ds_parser.cpp"
    break;

  case 513:
#line 2569 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8552 "ds_parser.cpp"
    break;

  case 514:
#line 2577 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8564 "ds_parser.cpp"
    break;

  case 515:
#line 2584 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8577 "ds_parser.cpp"
    break;

  case 516:
#line 2592 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8591 "ds_parser.cpp"
    break;

  case 517:
#line 2604 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8599 "ds_parser.cpp"
    break;

  case 518:
#line 2607 "ds_parser.ypp"
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
#line 8615 "ds_parser.cpp"
    break;

  case 519:
#line 2621 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8626 "ds_parser.cpp"
    break;

  case 520:
#line 2630 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8632 "ds_parser.cpp"
    break;

  case 521:
#line 2631 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8638 "ds_parser.cpp"
    break;

  case 522:
#line 2635 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8648 "ds_parser.cpp"
    break;

  case 523:
#line 2640 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8657 "ds_parser.cpp"
    break;

  case 524:
#line 2647 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8667 "ds_parser.cpp"
    break;

  case 525:
#line 2652 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8680 "ds_parser.cpp"
    break;

  case 526:
#line 2663 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8690 "ds_parser.cpp"
    break;

  case 527:
#line 2668 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8699 "ds_parser.cpp"
    break;

  case 528:
#line 2675 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8713 "ds_parser.cpp"
    break;

  case 529:
#line 2687 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8719 "ds_parser.cpp"
    break;

  case 530:
#line 2688 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8725 "ds_parser.cpp"
    break;

  case 531:
#line 2692 "ds_parser.ypp"
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
#line 8748 "ds_parser.cpp"
    break;

  case 532:
#line 2710 "ds_parser.ypp"
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
#line 8771 "ds_parser.cpp"
    break;


#line 8775 "ds_parser.cpp"

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
#line 2730 "ds_parser.ypp"


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
