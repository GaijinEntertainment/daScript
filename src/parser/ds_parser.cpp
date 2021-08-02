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
    DAS_PRIVATE = 314,
    DAS_SMART_PTR = 315,
    DAS_UNSAFE = 316,
    DAS_TBOOL = 317,
    DAS_TVOID = 318,
    DAS_TSTRING = 319,
    DAS_TAUTO = 320,
    DAS_TINT = 321,
    DAS_TINT2 = 322,
    DAS_TINT3 = 323,
    DAS_TINT4 = 324,
    DAS_TUINT = 325,
    DAS_TBITFIELD = 326,
    DAS_TUINT2 = 327,
    DAS_TUINT3 = 328,
    DAS_TUINT4 = 329,
    DAS_TFLOAT = 330,
    DAS_TFLOAT2 = 331,
    DAS_TFLOAT3 = 332,
    DAS_TFLOAT4 = 333,
    DAS_TRANGE = 334,
    DAS_TURANGE = 335,
    DAS_TBLOCK = 336,
    DAS_TINT64 = 337,
    DAS_TUINT64 = 338,
    DAS_TDOUBLE = 339,
    DAS_TFUNCTION = 340,
    DAS_TLAMBDA = 341,
    DAS_TINT8 = 342,
    DAS_TUINT8 = 343,
    DAS_TINT16 = 344,
    DAS_TUINT16 = 345,
    DAS_TTUPLE = 346,
    DAS_TVARIANT = 347,
    DAS_GENERATOR = 348,
    DAS_YIELD = 349,
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
#define YYLAST   7425

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  186
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  536
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  944

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
     178,   179,   163,   162,   152,   161,   175,   164,     2,     2,
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
     171,   172,   173,   174
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   448,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   464,   465,   466,   470,   471,
     475,   492,   493,   497,   501,   506,   515,   523,   539,   544,
     552,   552,   582,   604,   608,   611,   617,   626,   647,   670,
     671,   675,   679,   680,   684,   687,   693,   699,   702,   708,
     709,   713,   714,   715,   724,   725,   729,   738,   754,   762,
     772,   781,   782,   783,   784,   785,   786,   790,   795,   803,
     804,   805,   806,   807,   808,   809,   810,   816,   830,   847,
     848,   849,   853,   861,   874,   878,   885,   886,   890,   891,
     892,   896,   899,   906,   910,   911,   912,   913,   914,   915,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
     926,   927,   928,   929,   930,   931,   932,   933,   934,   935,
     936,   937,   938,   939,   940,   941,   942,   943,   944,   945,
     946,   947,   948,   949,   950,   951,   955,   977,  1015,  1022,
    1026,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,
    1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,
    1059,  1064,  1070,  1076,  1110,  1113,  1119,  1120,  1131,  1135,
    1141,  1144,  1147,  1151,  1157,  1161,  1165,  1168,  1171,  1176,
    1179,  1187,  1190,  1195,  1198,  1206,  1212,  1213,  1217,  1251,
    1251,  1251,  1254,  1254,  1254,  1259,  1259,  1259,  1267,  1267,
    1267,  1273,  1283,  1294,  1309,  1312,  1318,  1319,  1326,  1337,
    1338,  1339,  1343,  1344,  1345,  1346,  1350,  1355,  1363,  1364,
    1368,  1373,  1437,  1438,  1439,  1440,  1441,  1442,  1446,  1447,
    1448,  1449,  1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,
    1458,  1459,  1460,  1461,  1462,  1463,  1464,  1468,  1469,  1470,
    1471,  1475,  1486,  1491,  1501,  1505,  1505,  1505,  1512,  1512,
    1512,  1526,  1530,  1530,  1530,  1537,  1538,  1539,  1540,  1541,
    1542,  1543,  1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,
    1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,
    1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,
    1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1584,
    1588,  1591,  1594,  1595,  1596,  1597,  1606,  1615,  1616,  1619,
    1619,  1619,  1622,  1626,  1630,  1634,  1635,  1636,  1637,  1638,
    1639,  1640,  1641,  1657,  1673,  1731,  1732,  1736,  1737,  1741,
    1749,  1752,  1757,  1756,  1801,  1857,  1866,  1871,  1879,  1880,
    1884,  1887,  1896,  1897,  1901,  1910,  1911,  1916,  1917,  1921,
    1927,  1933,  1936,  1940,  1946,  1955,  1956,  1957,  1961,  1962,
    1966,  1969,  1974,  1979,  1987,  1998,  2001,  2008,  2009,  2013,
    2014,  2015,  2019,  2047,  2047,  2077,  2080,  2089,  2102,  2114,
    2115,  2119,  2123,  2134,  2149,  2150,  2154,  2191,  2192,  2196,
    2196,  2280,  2287,  2296,  2297,  2298,  2299,  2300,  2301,  2302,
    2303,  2304,  2305,  2306,  2307,  2308,  2309,  2310,  2311,  2312,
    2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,  2324,  2325,
    2326,  2327,  2328,  2329,  2333,  2344,  2348,  2358,  2365,  2374,
    2374,  2374,  2387,  2388,  2389,  2390,  2391,  2405,  2411,  2415,
    2419,  2424,  2429,  2434,  2439,  2443,  2447,  2452,  2456,  2461,
    2461,  2461,  2467,  2474,  2474,  2474,  2479,  2479,  2479,  2485,
    2485,  2485,  2490,  2494,  2494,  2494,  2499,  2499,  2499,  2508,
    2512,  2512,  2512,  2517,  2517,  2517,  2526,  2530,  2530,  2530,
    2535,  2535,  2535,  2544,  2544,  2544,  2550,  2550,  2550,  2559,
    2559,  2574,  2574,  2594,  2595,  2596,  2597,  2601,  2608,  2615,
    2621,  2630,  2635,  2642,  2643,  2647,  2653,  2660,  2668,  2675,
    2683,  2695,  2698,  2712,  2721,  2722,  2726,  2731,  2738,  2743,
    2753,  2758,  2765,  2777,  2778,  2782,  2800
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
  "\"implicit\"", "\"explicit\"", "\"shared\"", "\"private\"",
  "\"smart_ptr\"", "\"unsafe\"", "\"bool\"", "\"void\"", "\"string\"",
  "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
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
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "optional_public_or_private", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_if_then_else", "expression_for_loop", "expression_unsafe",
  "expression_while_loop", "expression_with", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "function_declaration_header", "function_declaration",
  "expression_block", "expression_any", "expressions", "expr_pipe",
  "name_in_namespace", "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_yield",
  "expression_try_catch", "kwd_let", "expression_let", "expr_cast", "$@2",
  "$@3", "$@4", "$@5", "$@6", "$@7", "expr_type_decl", "$@8", "$@9",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_expr", "$@10", "$@11", "$@12", "$@13", "expr_field", "$@14",
  "$@15", "expr", "$@16", "$@17", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@18",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "optional_shared_or_private", "global_let", "$@19",
  "enum_list", "single_alias", "alias_list", "alias_declaration",
  "enum_declaration", "optional_structure_parent", "structure_name",
  "class_or_struct", "structure_declaration", "$@20",
  "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_type_declaration",
  "$@21", "$@22", "type_declaration", "$@23", "$@24", "$@25", "$@26",
  "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34", "$@35",
  "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "variant_alias_declaration", "$@47",
  "bitfield_alias_declaration", "$@48", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_any_tuple", "make_dim", "make_dim_decl",
  "make_table", "make_table_decl", "array_comprehension_where",
  "array_comprehension", YY_NULLPTR
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
     409,   410,   411,   412,   413,    46,    91,    93,    40,    41,
      36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -611

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-611)))

#define YYTABLE_NINF -491

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -611,    30,  -611,  -611,   -68,   -48,   -24,    20,     0,  -611,
     -96,  -611,  -611,     7,  -611,  -611,  -611,  -611,  -611,   260,
    -611,   248,  -611,  -611,  -611,  -611,  -611,  -611,    39,  -611,
      63,    72,   179,  -611,  -611,    18,  -611,   -43,   113,  -611,
    -611,   281,   231,   234,  -611,  -611,   235,   270,   206,  -611,
      92,  -611,  -611,  -611,     1,   240,  -611,  -611,   224,    17,
     -68,   276,   -48,   271,  -611,   272,   277,  -611,   263,  -611,
     274,  -611,   -91,  -611,   311,   241,   242,  -611,   284,   -68,
       7,  -611,  7256,   396,   399,  -611,   257,   253,  -611,   283,
    -611,  -611,   294,  -611,  -611,  -611,  -611,  -611,   309,   199,
    -611,  -611,  -611,  -611,   398,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,   275,   -88,  7169,  -611,  -611,  -611,  -611,
    -611,   312,   315,  -611,   -47,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,   -94,   308,
    -611,  -611,   -37,  -611,   285,   -87,  -611,  -611,   104,  -611,
     175,  -611,  -611,  -611,  -611,  -611,  -611,  -611,     2,  -611,
    -611,    54,  -611,   305,   310,   314,   316,  -611,  -611,  -611,
     296,  -611,  -611,  -611,  -611,  -611,   317,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,   319,  -611,  -611,  -611,
     321,   326,  -611,  -611,  -611,  -611,   331,   332,  -611,  -611,
    -611,  -611,  -611,   483,  -611,   322,   348,  -611,   323,  -611,
     -68,  -611,   -32,  -611,   177,  7169,  -611,  1186,  -611,  -611,
    -611,  -611,   347,  7169,  -611,   -44,  -611,  -611,   199,  -611,
     -65,  4489,  -611,  -611,  -611,  -611,  -611,   361,  -611,   157,
     158,   159,  -611,  -611,  -611,  -611,  -611,  -611,   475,  -611,
    -611,    62,  2863,  -611,  -611,    55,  7169,    57,    97,   294,
    -611,   102,  -611,   373,   899,  -611,  4489,  -611,  -611,   294,
    -611,  -611,  7083,   336,   351,  1997,  -611,  -611,   253,  4489,
     338,  4489,   358,   359,   341,  -611,   340,   367,   386,  3001,
     253,  -611,   375,  2155,  4489,  4489,   228,   228,  6911,  6997,
    4489,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  4489,  4489,
    4489,   -37,  4489,  4489,  4489,  -611,   354,  -611,   502,  -611,
    -611,   362,   363,  4489,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,   -83,   364,  -611,   371,   372,  -611,  -611,  -611,
     294,  -611,  -611,  -611,  -611,   360,  -611,  -611,   -72,  -611,
    -611,  -611,  -611,  -611,  6107,   365,  -611,  -611,  -611,  -611,
    -611,    14,  -611,   252,  -611,  -611,  -611,  2293,  -611,  -611,
    5073,  7169,  7169,  7169,  7169,   376,   312,  7169,   257,  7169,
     257,  7169,   257,  7255,   315,  -611,  -611,  -611,   382,  -611,
    -611,  5769,  -611,  -611,   899,  -611,  -611,   383,  -611,  -611,
    -611,  7169,  -611,  2293,  -611,   817,     9,    43,  -611,   -37,
    -611,  2293,  -611,  -611,   433,   992,   551,  6399,  4489,   817,
    -611,  -611,  4489,  -611,  -611,   411,   430,  6399,  -611,  7169,
    2293,  -611,  4578,   445,   445,   389,  -611,  -611,   294,  1529,
     294,  1685,  6174,  -611,   213,   255,   445,   445,   -61,  -611,
     445,   445,  5221,   -81,   388,  -611,  -611,   817,  -611,  2431,
    -611,  -611,  -611,  -611,   438,   228,  -611,     8,   431,  4489,
    4489,  4489,  4489,  4489,  4489,  4489,  4489,  4489,  4489,  -611,
    -611,  4489,  4489,  4489,  4489,  4489,  4489,   432,  2587,  4489,
     434,  4489,  4489,  4489,  4489,  4489,  4489,  4489,  4489,  4489,
    4489,  4489,  4489,  4489,  4489,  2725,  3139,  4489,  4489,  4489,
    4489,  4489,  4489,  4489,  4489,  4489,  4489,   435,  4489,  3277,
       7,  -611,   571,   539,   401,   475,  -611,  2293,  -611,  4677,
    -611,   500,   516,   533,   691,  -611,    86,   843,   308,   869,
     308,  1128,   308,    95,  -611,   106,   899,   161,  -611,  -611,
    -611,  1623,  -611,  6007,  -611,  4489,  3433,  3589,  7169,  -611,
    4776,  4489,  4489,  -611,   253,  5298,  -611,  7169,  7169,  5378,
    7169,  -611,  -611,  1472,  -611,  4875,  -611,  -611,    22,   228,
     -42,  1841,  3727,  6174,   436,    -4,   409,   440,  -611,  -611,
    -109,    27,  3883,    -4,   230,  4489,  4489,   410,  -611,  4489,
     446,   449,  -611,   205,  -611,  -611,   370,   454,  -611,    47,
    6399,   -70,   257,  -611,   441,  -611,  -611,  6399,  6399,  6399,
    6399,  6399,  6399,  6399,  6399,   366,   366,  6785,  6399,  6399,
    6785,  6757,  6757,   424,  4489,  4489,  6399,   165,  -611,  5844,
     -27,   -27,  6399,   366,   366,  6399,  6399,   337,  6466,  6545,
    6399,  6399,  6399,  4489,  4489,  6399,   458,  6253,  6573,  6651,
    6679,  6785,  6785,   256,   256,   165,   165,   165,  -611,   603,
    5919,  -611,    51,   109,   304,  -611,   294,  -611,  -611,  4974,
    -611,  -611,  7169,  -611,  -611,  -611,   461,  -611,   447,  -611,
     448,  -611,   453,  7169,  -611,  7255,  -611,   315,   475,  4489,
     -78,  -611,    93,   472,  5455,  1779,  -611,  -611,  -611,  1939,
    2096,  -611,  2231,   438,  -611,  4489,  -611,  4489,  4489,   -12,
     473,   454,   442,   450,  4489,   455,  4489,  4489,   454,   439,
     456,  6399,  -611,  -611,  6320,  -611,   487,  7169,   257,  1370,
     253,  -611,  -611,  4489,  -611,     4,   127,  4489,  -611,   477,
     493,   495,   497,  -611,   133,   308,  -611,  4039,    13,  6399,
    -611,    13,  6399,  -611,  4489,  -611,  -611,  -611,   463,  -611,
       1,  -611,  -611,  -611,  2369,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,   899,  -611,  -611,  -611,  6399,   253,
    -611,   195,  -611,  -611,  -611,  -611,  -611,   465,    28,  6399,
    6399,    -4,   470,    41,   436,   474,  -611,  6399,  -611,  -611,
      53,    -4,   478,  -611,  -611,  -611,  2525,   308,  -611,  -611,
     817,   471,  6399,  -611,  -611,  -611,  -611,   -70,   480,   -85,
    7169,  -611,    96,  6399,  -611,  -611,     1,  -611,  -611,  -611,
    -611,  -611,  -611,  4489,   504,  -611,  4489,  4489,  4489,  4195,
    4489,   485,   486,  4489,  4489,  -611,  4489,   468,  -611,  -611,
     506,   370,  -611,  -611,  -611,  4351,  -611,  -611,  2663,  -611,
     482,  -611,  5535,   507,  6785,  6785,  6785,  -611,  5612,  5147,
     494,  -611,  6399,  6399,  5147,   496,   -37,  -611,  -611,  4489,
    6399,  -611,  -611,  -611,  4489,  -611,   623,   498,  -611,   505,
    -611,  -611,   -37,  6399,  -611,  5692,  4489,   501,   503,  -611,
    -611,  6399,  -611,  -611
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    86,     1,   186,     0,     0,     0,     0,     0,   187,
       0,   501,   499,     0,    14,     3,    10,     9,     8,     0,
       7,   379,     6,    11,     5,     4,    12,    13,    75,    77,
      32,    44,    41,    42,    34,    39,    33,     0,     0,    19,
      18,   377,     0,     0,    80,    81,     0,   166,    82,    84,
       0,    79,   398,   397,     0,     0,   380,   381,   383,     0,
       0,     0,     0,     0,    40,     0,     0,    37,     0,   385,
       0,   389,     0,   378,    15,     0,     0,   168,     0,     0,
       0,    87,     0,     0,     0,    93,    88,     0,   136,   394,
     399,   375,     0,    73,    74,    71,    72,    70,     0,     0,
      69,    78,    45,    43,    39,    36,    35,   428,   431,   429,
     432,   430,   433,     0,     0,     0,   391,   390,    16,    17,
      20,     0,     0,   167,     0,    85,    96,    97,    99,    98,
     100,   101,   102,   103,   128,   129,   126,   127,   119,   130,
     131,   120,   117,   118,   132,   133,   134,   135,   107,   108,
     109,   104,   105,   106,   122,   123,   121,   115,   116,   111,
     110,   112,   113,   114,    95,    94,   124,   125,   335,    91,
     160,   138,     0,   396,     0,     0,   401,   384,   368,    21,
       0,    65,    66,    63,    64,    62,    61,    67,     0,    38,
     385,     0,   392,     0,     0,     0,     0,   403,   423,   404,
     435,   405,   409,   410,   411,   412,   427,   416,   417,   418,
     419,   420,   421,   422,   424,   425,   472,   408,   415,   426,
     479,   486,   406,   413,   407,   414,     0,     0,   434,   442,
     445,   443,   444,     0,   437,     0,     0,   355,     0,    83,
       0,    89,     0,   348,     0,     0,   137,     0,   395,   340,
     382,   376,     0,     0,   369,     0,    22,    23,     0,    76,
       0,     0,   386,   463,   466,   469,   459,     0,   439,   473,
     480,   487,   493,   496,   450,   455,   449,   462,     0,   458,
     452,     0,     0,   388,   454,     0,     0,     0,     0,     0,
     346,   359,    90,   335,    92,   162,     0,    54,    55,     0,
     271,   272,     0,     0,     0,     0,   265,   174,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   222,   224,   223,   225,   226,   227,    24,     0,     0,
       0,     0,     0,     0,     0,   209,   210,   141,   139,   269,
     268,     0,     0,     0,   155,   150,   148,   147,   149,   220,
     161,   142,   266,     0,   328,     0,     0,   153,   154,   156,
       0,   146,   327,   326,   325,    86,   331,   267,     0,   143,
     330,   329,   307,   273,   228,     0,   270,   503,   504,   505,
     506,   335,   402,     0,   366,   367,   365,     0,    68,   393,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      88,     0,    88,     0,     0,   457,   451,   453,     0,   456,
     447,     0,   438,   502,   354,   500,   356,     0,   347,   358,
     357,     0,   360,     0,   349,     0,     0,   170,   173,     0,
     198,     0,   176,   179,     0,   228,     0,   169,     0,     0,
     189,   192,     0,   159,   195,     0,     0,    48,    58,     0,
       0,   183,   228,   299,   300,   210,   164,   165,     0,   513,
       0,     0,     0,   530,     0,     0,   277,   276,   312,    30,
     275,   274,     0,   211,     0,   157,   158,     0,   334,     0,
     145,   151,   152,   188,   218,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   301,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,   400,     0,   337,     0,   365,   370,     0,   374,   228,
     387,     0,     0,     0,     0,   436,     0,     0,    91,     0,
      91,     0,    91,   166,   352,     0,   350,     0,   448,   446,
     336,   361,   364,   363,    59,     0,     0,     0,     0,   180,
     228,     0,     0,   177,     0,     0,    60,     0,     0,     0,
       0,    46,    47,     0,   184,   228,   181,   211,     0,     0,
     166,     0,     0,   521,   511,   513,     0,   524,   525,   526,
       0,     0,     0,   513,     0,     0,     0,     0,    27,     0,
      25,     0,   303,   255,   254,   160,    51,     0,   308,     0,
     204,     0,    88,   163,     0,   322,   323,   238,   239,   241,
     240,   242,   232,   233,   234,   278,   279,   291,   243,   244,
     292,   289,   290,     0,     0,     0,   230,   317,   306,     0,
     332,   333,   231,   280,   281,   245,   246,   296,   297,   298,
     235,   236,   237,     0,     0,   229,     0,     0,   294,   295,
     293,   287,   288,   283,   282,   284,   285,   286,   261,     0,
       0,   310,     0,     0,   337,   338,     0,   341,   372,   228,
     373,   464,     0,   470,   460,   440,     0,   474,     0,   481,
       0,   488,     0,     0,   494,     0,   497,     0,   357,     0,
       0,   171,     0,     0,     0,     0,   178,   185,   313,     0,
       0,   314,     0,   218,   182,     0,   514,     0,     0,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   531,   532,     0,    28,    31,     0,    88,     0,
       0,    49,    50,     0,    56,     0,     0,     0,   309,     0,
       0,     0,     0,   216,     0,    91,   319,     0,   249,   250,
     305,   247,   248,   324,     0,   263,   304,   311,    87,   342,
       0,   339,   371,   465,     0,   471,   461,   441,   475,   477,
     482,   484,   489,   491,   351,   495,   353,   498,   362,     0,
     172,     0,   201,   199,   190,   193,   196,     0,     0,   508,
     507,   513,     0,     0,   512,     0,   516,   522,   528,   527,
       0,   513,     0,   529,    26,    29,     0,    91,   140,    52,
       0,     0,   205,   214,   215,   213,   212,     0,     0,     0,
       0,   252,     0,   318,   264,   345,     0,   344,   467,   478,
     485,   492,    57,     0,     0,   200,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   515,     0,     0,   519,   256,
       0,    51,   251,   217,   219,     0,   206,   221,     0,   253,
       0,   468,     0,     0,   191,   194,   197,   315,     0,   533,
       0,   517,   510,   509,   533,     0,     0,   259,    53,     0,
     207,   320,   343,   202,     0,   316,     0,     0,   518,     0,
     520,   257,     0,   208,   321,     0,     0,     0,     0,   260,
     203,   534,   535,   536
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -611,  -611,  -611,  -611,  -611,   204,   622,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,   578,  -611,  -611,   626,
    -611,  -611,  -611,  -196,  -611,  -611,  -611,  -611,  -611,  -611,
     444,  -611,   636,   -56,  -611,   617,   150,   -80,  -400,  -548,
    -611,  -611,  -163,   -95,   -86,  -611,    69,  -294,   -13,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,   705,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -534,
    -611,  -611,  -150,  -611,   -35,  -308,  -611,  -245,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,   286,
    -611,  -611,   320,    10,  -611,  -611,  -611,   420,  -611,   -10,
    -611,  -282,   302,  -284,  -283,   324,  -611,  -162,  -611,  -611,
    -611,  -611,  -611,   530,   649,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -239,  -111,  -611,  -611,  -611,   318,  -611,
    -611,  -611,   -77,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,   423,  -610,  -455,  -598,  -611,  -611,  -321,   -34,   258,
    -611,  -611,  -611,  -188,  -611
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   120,    41,    15,   180,   186,   475,   349,   766,
     350,   631,    16,    17,    35,    36,    67,    18,    32,    33,
     351,   352,   773,   774,   353,   354,   355,   356,   357,   358,
     187,   188,    29,    30,    48,    49,    50,    19,   169,   246,
      86,    20,    87,    88,   359,   360,   247,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   597,
     876,   598,   877,   600,   878,   373,   588,   875,   374,   639,
     897,   375,   783,   784,   642,   376,   377,   444,   379,   380,
     381,   382,   767,   916,   768,   932,   383,   699,   864,   640,
     860,   934,   242,   706,   554,   391,   866,   243,   244,   574,
     575,   237,   238,   748,   290,   397,   255,   177,   175,    74,
      58,    22,    92,   114,    71,    72,    23,    24,   173,    90,
      55,    25,   174,   178,   385,   113,   230,   231,   235,   232,
     406,   807,   576,   404,   806,   401,   803,   402,   901,   403,
     805,   407,   808,   408,   869,   409,   810,   410,   870,   411,
     812,   412,   871,   413,   815,   414,   817,    26,    43,    27,
      42,   386,   614,   615,   616,   387,   617,   618,   619,   620,
     388,   474,   389,   927,   390
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      51,   171,   378,   291,   229,   426,   428,   433,   568,   473,
     570,   443,   572,   251,   289,   702,   623,   752,   466,   467,
     718,   -86,   720,   124,   722,   759,   644,   776,   585,   461,
       2,    93,    94,   497,   498,    82,     3,   609,   233,    63,
      44,   745,   240,   895,   779,   609,   757,   495,    46,    39,
     291,   730,   732,   780,    70,   520,   521,   191,   176,     4,
     436,     5,   488,     6,    47,     7,    45,    51,   755,     8,
     394,    64,   429,   756,   777,   274,     9,    28,   633,   395,
     191,   747,    10,   781,    40,   241,   517,    78,   782,   520,
     521,    31,    46,   116,   416,   489,   192,   250,   170,   275,
     276,    11,   228,   558,   819,    60,    83,    84,    47,   396,
     496,   430,    68,   176,   547,   548,   507,   508,   429,   399,
     511,    34,    12,   514,   515,   516,   517,   747,   519,   520,
     521,   522,   239,   775,   229,   523,    46,   525,   526,   582,
      69,   834,   229,   529,   530,   531,    85,   589,   547,   548,
     240,   832,    47,   645,   258,   429,    95,   430,   277,   248,
      96,   252,    97,    98,   883,    37,   604,   536,   294,   537,
     538,   539,   540,   541,   252,   229,   393,   545,   751,   252,
     777,   259,    65,    38,   288,   497,   498,   643,   547,   548,
     550,   229,    59,    66,   430,    99,   278,   279,   551,   777,
     422,   280,   236,   777,   281,   777,    13,   261,   493,   424,
     880,   828,    14,   181,   182,    60,   429,   229,   229,   282,
     417,   586,   446,   840,    78,   437,   778,    61,   284,   608,
     797,   621,   228,   881,   458,   886,   262,   859,   418,   423,
     228,   425,   785,   887,    80,   777,   715,   419,   777,    60,
     723,   469,   471,   862,   252,   430,   252,   431,    70,   253,
     432,    80,   254,   708,    52,    53,   724,    54,   716,    81,
     509,   510,   820,   228,   427,   899,   497,   498,   517,   750,
     519,   520,   521,   522,   274,   857,   798,   523,   725,   228,
     229,   229,   229,   229,   831,   494,   229,   729,   229,   890,
     229,   746,   229,   841,   851,   762,    56,    57,   275,   276,
     858,   552,  -476,  -483,  -490,   228,   228,  -476,  -483,  -490,
     229,   726,   256,   257,   561,   562,   563,   564,   479,   107,
     567,    62,   569,   108,   571,  -476,  -483,  -490,   183,    73,
     547,   548,   184,   727,   185,    98,   705,   799,   229,   584,
     109,   110,   111,   112,   581,   873,   292,   497,   498,   293,
    -258,   509,   510,   596,   118,  -258,   394,   277,   847,   517,
     119,   519,   520,   521,   522,   395,    75,   874,   523,    76,
      77,   770,   603,  -258,    79,    89,   497,   498,   228,   228,
     228,   228,   771,   772,   228,   626,   228,   627,   228,    78,
     228,   636,   179,   628,   629,   555,   279,    91,   345,   465,
     280,   170,   756,   281,   760,   102,   104,   105,   228,   544,
     545,   546,   106,   801,   121,   122,   587,   115,   282,   123,
     166,   547,   548,   167,   556,   168,   170,   284,   172,   176,
     507,   508,   509,   510,   511,   426,   228,   514,   515,   516,
     517,    64,   519,   520,   521,   522,   179,   234,   190,   523,
     236,   525,   526,   245,   263,   497,   498,   291,   249,   264,
     634,   509,   510,   265,   267,   266,   268,   229,   269,   517,
     270,   519,   520,   521,   522,   271,   229,   229,   523,   229,
     272,   273,   392,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   286,   285,   287,   405,   415,   737,   240,
     440,   735,   547,   548,   439,   274,   448,   450,   451,   452,
     739,   740,   453,   742,   378,   455,   454,   542,   543,   544,
     545,   546,   274,   384,   459,   483,    13,    51,   484,   275,
     276,   547,   548,   549,   485,   486,   490,   400,   274,   884,
     509,   510,   495,   491,   492,   565,   275,   276,   517,   578,
     580,   520,   521,   522,   594,   274,   601,   523,   421,   602,
     607,   635,   275,   276,   641,   228,   646,   663,   704,   668,
     698,   705,   435,   707,   228,   228,   753,   228,   750,   275,
     276,   445,   754,   256,   763,   447,   765,   449,   277,   775,
     786,   229,   787,   793,   795,   457,   422,   809,   811,   462,
     463,   464,   229,   813,   229,   277,   472,   821,   833,   835,
     547,   548,   853,   842,   476,   477,   478,   836,   480,   481,
     482,   277,   838,   843,   845,   804,   278,   279,   854,   487,
     855,   280,   856,   879,   281,   865,   814,   882,   277,   903,
     892,   885,   915,   278,   279,   888,   229,   894,   280,   282,
     711,   281,   910,   911,   922,   283,   917,   924,   284,   278,
     279,   928,   936,   930,   280,   937,   282,   281,   942,   630,
     943,   100,   189,   559,   849,   284,   278,   279,   103,   938,
     846,   280,   282,   713,   281,   918,   101,   125,   712,   228,
     703,   284,   398,   900,   769,   867,    21,   893,   827,   282,
     228,   553,   228,   434,   800,   816,   577,   557,   284,   583,
     260,   117,   839,   274,   566,   438,   929,   590,     0,   624,
       0,     0,     0,   872,   595,     0,     0,     0,   599,     0,
       0,     0,     0,     0,     0,     0,   605,   275,   276,   229,
       0,     0,     0,     0,   228,   613,     0,   613,     0,     0,
       0,     0,     0,     0,   891,     0,     0,     0,     0,     0,
       0,     0,     0,   896,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   898,     0,   647,   648,   649,   650,   651,
     652,   653,   654,   655,   656,     0,     0,   657,   658,   659,
     660,   661,   662,     0,   666,   667,   277,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   679,   680,   681,
     682,   685,   687,   688,   689,   690,   691,   692,   693,   694,
     695,   696,   697,     0,   700,     0,     0,   497,   498,     0,
       0,     0,     0,   709,   278,   279,     0,   228,     0,   280,
       0,   714,   281,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   282,     0,     0,
       0,     0,     0,   734,     0,   274,   284,   666,   685,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   476,   482,   275,
     276,   274,     0,   931,     0,     0,     0,     0,   482,     0,
       0,   761,   472,     0,     0,   764,     0,     0,     0,   939,
     507,   508,   509,   510,   511,   275,   276,   514,   515,   516,
     517,   274,   519,   520,   521,   522,     0,     0,     0,   523,
       0,   525,   526,     0,     0,     0,     0,   529,   530,   531,
     788,   789,     0,     0,     0,   275,   276,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   791,
     792,   536,     0,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,     0,   277,     0,     0,     0,     0,     0,
       0,     0,   547,   548,     0,     0,   278,   279,     0,     0,
     170,   280,     0,   717,   281,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   277,   818,     0,     0,     0,   282,
       0,     0,   278,   279,     0,     0,     0,   280,   284,   719,
     281,     0,     0,   829,   830,     0,     0,     0,     0,     0,
     837,     0,   613,     0,     0,   282,     0,     0,     0,     0,
       0,     0,   278,   279,   284,   384,     0,   280,     0,   850,
     281,     0,     0,   852,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,     0,
     863,     0,     0,     0,   284,     0,     0,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   591,   519,   520,   521,
     522,     0,     0,     0,   523,   524,   525,   526,   527,   528,
       0,     0,   529,   530,   531,   532,   533,   534,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   592,   536,     0,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,     0,   902,
     274,     0,   904,   905,   906,   908,   909,   547,   548,   912,
     913,     0,   914,     0,   593,     0,     0,     0,     0,     0,
       0,   920,     0,     0,   275,   276,     0,   295,     0,     0,
       0,     0,     3,     0,   296,   297,   298,     0,   299,     0,
     300,   301,   302,   303,   304,   933,     0,     0,     0,     0,
     935,   305,   306,   307,   308,     0,     0,     0,     0,     0,
       0,     0,   941,   309,   310,     0,   311,   312,     0,     0,
     313,     0,     9,   314,   315,     0,   316,   317,     0,     0,
     318,   319,     0,   277,     0,     0,     0,   320,   197,   198,
     199,     0,   201,   202,   203,   204,   205,   321,   207,   208,
     209,   210,   211,   212,   213,   214,   215,     0,   217,   218,
     219,     0,     0,   222,   223,   224,   225,     0,     0,   322,
     323,   278,   279,     0,     0,     0,   280,     0,   721,   281,
       0,   324,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   282,     0,   326,   327,     0,     0,
       0,     0,     0,   284,     0,    46,     0,     0,     0,     0,
       0,     0,   328,   329,   330,   331,   332,   333,   334,   335,
     336,    47,   337,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,   340,
       0,   341,     0,     0,   342,   343,     0,     0,     0,     0,
       0,     0,     0,     0,   344,     0,   345,   346,   347,   170,
     348,   295,     0,     0,     0,     0,     3,     0,   296,   297,
     298,     0,   299,     0,   300,   301,   302,   303,   304,     0,
       0,     0,     0,     0,     0,   305,   306,   307,   308,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   310,     0,
     311,   312,     0,     0,   313,     0,     9,   314,   315,     0,
     316,   317,     0,     0,   318,   319,     0,     0,     0,     0,
       0,   320,   197,   198,   199,     0,   201,   202,   203,   204,
     205,   321,   207,   208,   209,   210,   211,   212,   213,   214,
     215,     0,   217,   218,   219,     0,     0,   222,   223,   224,
     225,     0,     0,   322,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,     0,     0,     0,     0,     0,     0,     0,    46,
       0,     0,     0,     0,   274,     0,   328,   329,   330,   331,
     332,   333,   334,   335,   336,    47,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   275,   276,
       0,   338,   339,   340,     0,   341,     0,     0,   342,   343,
       0,     0,     0,   300,   301,   302,   303,   304,   344,     0,
     345,   346,   347,   170,   848,   306,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,   310,     0,     0,
     312,     0,     0,   313,     0,     0,   314,     0,   609,     0,
     317,     0,     0,     0,     0,   275,   276,   277,     0,     0,
       0,   197,   198,   199,     0,   201,   202,   203,   204,   205,
     321,   207,   208,   209,   210,   211,   212,   213,   214,   215,
       0,   217,   218,   219,     0,     0,   222,   223,   224,   225,
       0,     0,   322,     0,     0,   278,   279,     0,     0,     0,
     280,     0,   743,   281,   324,   325,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,   282,     0,
       0,     0,     0,     0,     0,   274,     0,   284,    46,     0,
       0,     0,     0,     0,     0,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   610,   337,     0,     0,     0,   275,
     276,     0,   278,   279,     0,     0,     0,   280,     0,     0,
     611,   339,   340,     0,   341,     0,     0,   342,   343,   300,
     301,   302,   303,   304,     0,   282,     0,   612,     0,   345,
     346,   306,   170,     0,   284,     0,     0,   274,     0,     0,
       0,     0,     0,   310,     0,     0,   312,     0,     0,   313,
       0,     0,   314,     0,     0,     0,   317,   429,   277,     0,
       0,   275,   276,     0,     0,     0,     0,   197,   198,   199,
       0,   201,   202,   203,   204,   205,   321,   207,   208,   209,
     210,   211,   212,   213,   214,   215,     0,   217,   218,   219,
       0,     0,   222,   223,   224,   225,   728,   279,   322,     0,
       0,   280,     0,     0,   281,     0,     0,     0,     0,     0,
     324,   325,     0,     0,     0,     0,     0,     0,     0,   282,
     277,     0,     0,     0,     0,     0,     0,     0,   284,     0,
       0,   274,     0,     0,    46,     0,     0,     0,     0,     0,
       0,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     610,   337,     0,     0,     0,   275,   276,     0,   278,   279,
       0,     0,     0,   280,     0,     0,   611,   339,   340,     0,
     341,     0,     0,   342,   343,   300,   301,   302,   303,   304,
       0,   282,     0,   622,     0,   345,   346,   306,   170,     0,
     284,     0,     0,   416,     0,     0,     0,     0,     0,   310,
       0,     0,   312,     0,     0,   313,     0,     0,   314,     0,
       0,     0,   317,     0,   277,     0,     0,     0,     0,     0,
       0,     0,     0,   197,   198,   199,     0,   201,   202,   203,
     204,   205,   321,   207,   208,   209,   210,   211,   212,   213,
     214,   215,     0,   217,   218,   219,     0,     0,   222,   223,
     224,   225,   278,   279,   322,     0,     0,   280,     0,   823,
     281,     0,     0,     0,     0,     0,   324,   325,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,     0,
       0,     0,     0,     0,   284,     0,     0,     0,     0,     0,
      46,   274,     0,     0,     0,     0,     0,   328,   329,   330,
     331,   332,   333,   334,   335,   336,    47,   337,     0,     0,
       0,     0,     0,     0,     0,   275,   276,     0,     0,   417,
       0,     0,   338,   339,   340,     0,   341,     0,     0,   342,
     343,   300,   301,   302,   303,   304,     0,   418,     0,   344,
       0,   345,   346,   306,   170,     0,   419,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,     0,   312,     0,
       0,   313,     0,     0,   314,     0,     0,     0,   317,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,   197,
     198,   199,     0,   201,   202,   203,   204,   205,   321,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,   217,
     218,   219,     0,     0,   222,   223,   224,   225,     0,     0,
     322,     0,   278,   279,     0,     0,     0,   280,     0,   824,
     281,     0,   324,   325,     0,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,   282,     0,   326,   327,     0,
       0,     0,     0,     0,   284,     0,    46,     0,   274,     0,
       0,     0,     0,   328,   329,   330,   331,   332,   333,   334,
     335,   336,    47,   337,     0,     0,     0,     0,     0,     0,
       0,     0,   275,   276,     0,     0,     0,     0,   338,   339,
     340,     0,   341,     0,     0,   342,   343,     0,     0,   300,
     301,   302,   303,   304,     0,   344,     0,   345,   346,   442,
     170,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,     0,     0,   312,     0,     0,   313,
       0,     0,   314,     0,     0,     0,   317,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,   197,   198,   199,
       0,   201,   202,   203,   204,   205,   321,   207,   208,   209,
     210,   211,   212,   213,   214,   215,     0,   217,   218,   219,
       0,     0,   222,   223,   224,   225,     0,     0,   322,   278,
     279,     0,     0,     0,   280,     0,   825,   281,     0,     0,
     324,   325,     0,   274,     0,     0,     0,     0,     0,   460,
       0,     0,   282,     0,     0,   326,   327,     0,     0,     0,
       0,   284,     0,     0,    46,     0,     0,   275,   276,     0,
       0,   328,   329,   330,   331,   332,   333,   334,   335,   336,
      47,   337,     0,     0,     0,     0,     0,   300,   301,   302,
     303,   304,     0,     0,     0,     0,   338,   339,   340,   306,
     341,     0,     0,   342,   343,     0,     0,     0,     0,     0,
       0,   310,     0,   344,   312,   345,   346,   313,   170,     0,
     314,     0,     0,     0,   317,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,   197,   198,   199,     0,   201,
     202,   203,   204,   205,   321,   207,   208,   209,   210,   211,
     212,   213,   214,   215,     0,   217,   218,   219,     0,     0,
     222,   223,   224,   225,   278,   279,   322,     0,     0,   280,
       0,   826,   281,     0,     0,     0,     0,     0,   324,   325,
       0,   274,     0,     0,     0,     0,     0,   282,     0,     0,
       0,     0,     0,   326,   327,     0,   284,     0,     0,     0,
       0,     0,    46,     0,     0,   275,   276,     0,     0,   328,
     329,   330,   331,   332,   333,   334,   335,   336,    47,   337,
       0,     0,     0,     0,     0,   300,   301,   302,   303,   304,
       0,     0,     0,     0,   338,   339,   340,   306,   341,     0,
       0,   342,   343,     0,     0,     0,     0,     0,     0,   310,
       0,   344,   312,   345,   346,   313,   170,     0,   314,     0,
       0,     0,   317,     0,   277,     0,     0,     0,     0,     0,
       0,     0,     0,   197,   198,   199,     0,   201,   202,   203,
     204,   205,   321,   207,   208,   209,   210,   211,   212,   213,
     214,   215,     0,   217,   218,   219,     0,     0,   222,   223,
     224,   225,   278,   279,   322,     0,     0,   280,     0,   868,
     281,     0,     0,     0,     0,     0,   324,   325,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,     0,
       0,     0,     0,     0,   284,     0,     0,   274,     0,     0,
      46,     0,     0,     0,     0,     0,     0,   328,   329,   330,
     331,   332,   333,   334,   335,   336,    47,   337,     0,     0,
       0,   275,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,   340,     0,   341,     0,     0,   342,
     343,   300,   301,   302,   303,   304,     0,   637,     0,   344,
     638,   345,   346,   306,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,     0,   312,     0,
       0,   313,     0,     0,   314,     0,     0,     0,   317,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     198,   199,     0,   201,   202,   203,   204,   205,   321,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,   217,
     218,   219,     0,     0,   222,   223,   224,   225,   278,   279,
     322,     0,     0,   280,     0,   889,   281,     0,     0,     0,
       0,     0,   324,   325,     0,   274,     0,     0,     0,     0,
       0,   282,     0,     0,     0,     0,     0,   664,   665,     0,
     284,     0,     0,     0,     0,     0,    46,     0,     0,   275,
     276,     0,     0,   328,   329,   330,   331,   332,   333,   334,
     335,   336,    47,   337,     0,     0,     0,     0,     0,   300,
     301,   302,   303,   304,     0,     0,     0,     0,   338,   339,
     340,   306,   341,     0,     0,   342,   343,     0,     0,     0,
       0,     0,     0,   310,     0,   344,   312,   345,   346,   313,
     170,     0,   314,     0,     0,     0,   317,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,   197,   198,   199,
       0,   201,   202,   203,   204,   205,   321,   207,   208,   209,
     210,   211,   212,   213,   214,   215,     0,   217,   218,   219,
       0,     0,   222,   223,   224,   225,   278,   279,   322,     0,
       0,   280,     0,   921,   281,     0,     0,     0,     0,     0,
     324,   325,     0,     0,     0,     0,     0,     0,     0,   282,
       0,     0,     0,     0,     0,   683,   684,     0,   284,     0,
       0,     0,     0,     0,    46,     0,     0,     0,     0,     0,
       0,   328,   329,   330,   331,   332,   333,   334,   335,   336,
      47,   337,     0,     0,     0,     0,     0,   300,   301,   302,
     303,   304,     0,     0,     0,     0,   338,   339,   340,   306,
     341,     0,     0,   342,   343,     0,     0,     0,     0,     0,
       0,   310,     0,   344,   312,   345,   346,   313,   170,     0,
     314,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,   198,   199,     0,   201,
     202,   203,   204,   205,   321,   207,   208,   209,   210,   211,
     212,   213,   214,   215,     0,   217,   218,   219,     0,     0,
     222,   223,   224,   225,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,   328,
     329,   330,   331,   332,   333,   334,   335,   336,    47,   337,
       0,     0,     0,     0,     0,   300,   301,   302,   303,   304,
       0,     0,     0,     0,   338,   339,   340,   306,   341,     0,
       0,   342,   343,     0,     0,     0,     0,     0,     0,   310,
     420,   344,   312,   345,   346,   313,   170,     0,   314,     0,
       0,     0,   317,     0,     0,   456,     0,     0,     0,     0,
       0,     0,     0,   197,   198,   199,     0,   201,   202,   203,
     204,   205,   321,   207,   208,   209,   210,   211,   212,   213,
     214,   215,     0,   217,   218,   219,     0,     0,   222,   223,
     224,   225,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   325,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,   328,   329,   330,
     331,   332,   333,   334,   335,   336,    47,   337,     0,     0,
       0,     0,     0,   300,   301,   302,   303,   304,     0,     0,
     686,     0,   338,   339,   340,   306,   341,     0,     0,   342,
     343,     0,     0,     0,     0,     0,     0,   310,     0,   344,
     312,   345,   346,   313,   170,     0,   314,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   197,   198,   199,     0,   201,   202,   203,   204,   205,
     321,   207,   208,   209,   210,   211,   212,   213,   214,   215,
       0,   217,   218,   219,     0,     0,   222,   223,   224,   225,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,   328,   329,   330,   331,   332,
     333,   334,   335,   336,    47,   337,     0,     0,     0,     0,
       0,   300,   301,   302,   303,   304,     0,     0,     0,     0,
     338,   339,   340,   306,   341,     0,     0,   342,   343,     0,
       0,     0,     0,     0,     0,   310,     0,   344,   312,   345,
     346,   313,   170,     0,   314,     0,     0,     0,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     198,   199,     0,   201,   202,   203,   204,   205,   321,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,   217,
     218,   219,     0,     0,   222,   223,   224,   225,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
       0,     0,     0,   328,   329,   330,   331,   332,   333,   334,
     335,   336,    47,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
     340,     0,   341,     0,     0,   342,   343,   300,   301,   302,
     303,   304,     0,     0,     0,   344,   701,   345,   346,   306,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,   312,     0,     0,   313,     0,     0,
     314,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,   198,   199,     0,   201,
     202,   203,   204,   205,   321,   207,   208,   209,   210,   211,
     212,   213,   214,   215,     0,   217,   218,   219,     0,     0,
     222,   223,   224,   225,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,   328,
     329,   330,   331,   332,   333,   334,   335,   336,    47,   337,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,   339,   340,     0,   341,     0,
       0,   342,   343,   300,   301,   302,   303,   304,     0,     0,
       0,   344,   731,   345,   346,   306,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,     0,     0,
     312,     0,     0,   313,     0,     0,   314,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   197,   198,   199,     0,   201,   202,   203,   204,   205,
     321,   207,   208,   209,   210,   211,   212,   213,   214,   215,
       0,   217,   218,   219,     0,     0,   222,   223,   224,   225,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,   328,   329,   330,   331,   332,
     333,   334,   335,   336,    47,   337,     0,     0,     0,     0,
       0,   300,   301,   302,   303,   304,     0,     0,   733,     0,
     338,   339,   340,   306,   341,     0,     0,   342,   343,     0,
       0,     0,     0,     0,     0,   310,     0,   344,   312,   345,
     346,   313,   170,     0,   314,     0,     0,     0,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     198,   199,     0,   201,   202,   203,   204,   205,   321,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,   217,
     218,   219,     0,     0,   222,   223,   224,   225,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
       0,     0,     0,   328,   329,   330,   331,   332,   333,   334,
     335,   336,    47,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,   339,
     340,     0,   341,     0,     0,   342,   343,   300,   301,   302,
     303,   304,     0,     0,     0,   344,   749,   345,   346,   306,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,   312,     0,     0,   313,     0,     0,
     314,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,   198,   199,     0,   201,
     202,   203,   204,   205,   321,   207,   208,   209,   210,   211,
     212,   213,   214,   215,     0,   217,   218,   219,     0,     0,
     222,   223,   224,   225,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,   328,
     329,   330,   331,   332,   333,   334,   335,   336,    47,   337,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,   339,   340,     0,   341,     0,
       0,   342,   343,   300,   301,   302,   303,   304,     0,     0,
       0,   344,   758,   345,   346,   306,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,     0,     0,
     312,     0,     0,   313,     0,     0,   314,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   197,   198,   199,     0,   201,   202,   203,   204,   205,
     321,   207,   208,   209,   210,   211,   212,   213,   214,   215,
       0,   217,   218,   219,     0,     0,   222,   223,   224,   225,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,   328,   329,   330,   331,   332,
     333,   334,   335,   336,    47,   337,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,   340,     0,   341,     0,     0,   342,   343,   300,
     301,   302,   303,   304,     0,     0,     0,   344,   861,   345,
     346,   306,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,     0,     0,   312,     0,     0,   313,
       0,     0,   314,     0,     0,     0,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   197,   198,   199,
       0,   201,   202,   203,   204,   205,   321,   207,   208,   209,
     210,   211,   212,   213,   214,   215,     0,   217,   218,   219,
       0,     0,   222,   223,   224,   225,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   325,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    46,     0,     0,     0,     0,     0,
       0,   328,   329,   330,   331,   332,   333,   334,   335,   336,
      47,   337,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   339,   340,     0,
     341,     0,     0,   342,   343,   300,   301,   302,   303,   304,
       0,     0,     0,   344,   907,   345,   346,   306,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,   312,     0,     0,   313,     0,     0,   314,     0,
       0,     0,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,   198,   199,     0,   201,   202,   203,
     204,   205,   321,   207,   208,   209,   210,   211,   212,   213,
     214,   215,     0,   217,   218,   219,     0,     0,   222,   223,
     224,   225,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   325,     0,     0,
       0,     0,     0,     0,     0,   919,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,   328,   329,   330,
     331,   332,   333,   334,   335,   336,    47,   337,     0,     0,
       0,     0,     0,   300,   301,   302,   303,   304,     0,     0,
       0,     0,   338,   339,   340,   306,   341,     0,     0,   342,
     343,     0,     0,     0,     0,     0,     0,   310,     0,   344,
     312,   345,   346,   313,   170,     0,   314,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   197,   198,   199,     0,   201,   202,   203,   204,   205,
     321,   207,   208,   209,   210,   211,   212,   213,   214,   215,
       0,   217,   218,   219,     0,     0,   222,   223,   224,   225,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   325,     0,     0,   497,   498,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,   328,   329,   330,   331,   332,
     333,   334,   335,   336,    47,   337,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,   340,     0,   341,     0,     0,   342,   343,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,   345,
     346,     0,   170,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   517,   591,   519,   520,   521,   522,   497,   498,     0,
     523,   524,   525,   526,   527,   528,     0,     0,   529,   530,
     531,   532,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   592,   536,     0,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   547,   548,     0,     0,     0,     0,     0,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   591,   519,   520,   521,   522,   497,   498,     0,   523,
     524,   525,   526,   527,   528,     0,     0,   529,   530,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     592,   536,     0,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   547,   548,     0,     0,     0,     0,     0,   710,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   517,
     591,   519,   520,   521,   522,   497,   498,     0,   523,   524,
     525,   526,   527,   528,     0,     0,   529,   530,   531,   532,
     533,   534,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   592,
     536,     0,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   547,   548,     0,     0,     0,     0,     0,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   591,
     519,   520,   521,   522,   497,   498,     0,   523,   524,   525,
     526,   527,   528,     0,     0,   529,   530,   531,   532,   533,
     534,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   592,   536,
       0,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     547,   548,     0,     0,     0,     0,     0,   744,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   591,   519,
     520,   521,   522,   497,   498,     0,   523,   524,   525,   526,
     527,   528,     0,     0,   529,   530,   531,   532,   533,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   592,   536,     0,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   547,
     548,     0,     0,     0,     0,     0,   802,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   497,   498,     0,
       0,     0,     0,     0,     0,     0,   507,   508,   509,   510,
     511,     0,     0,   514,   515,   516,   517,     0,   519,   520,
     521,   522,     0,     0,     0,   523,     0,   525,   526,     0,
       0,     0,     0,   529,   530,   531,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   536,     0,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,     0,
       0,   497,   498,     0,     0,     0,     0,     0,   547,   548,
     507,   508,   509,   510,   511,   560,     0,   514,   515,   516,
     517,     0,   519,   520,   521,   522,     0,     0,     0,   523,
       0,   525,   526,     0,     0,     0,     0,   529,   530,   531,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   536,     0,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,     0,     0,     0,     0,     0,   497,   498,
       0,     0,   547,   548,   507,   508,   509,   510,   511,   926,
       0,   514,   515,   516,   517,     0,   519,   520,   521,   522,
       0,     0,     0,   523,     0,   525,   526,     0,     0,     0,
       0,   529,   530,   531,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   536,     0,   537,   538,   539,
     540,   541,   542,   543,   544,   545,   546,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   547,   548,   497,   498,
     632,   507,   508,   509,   510,   511,     0,     0,   514,   515,
     516,   517,     0,   519,   520,   521,   522,     0,     0,     0,
     523,     0,   525,   526,     0,     0,     0,     0,   529,   530,
     531,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   547,   548,   497,   498,   738,     0,     0,
       0,   507,   508,   509,   510,   511,     0,     0,   514,   515,
     516,   517,     0,   519,   520,   521,   522,     0,     0,     0,
     523,     0,   525,   526,     0,     0,     0,     0,   529,   530,
     531,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   547,   548,   497,   498,   741,   507,   508,
     509,   510,   511,     0,     0,   514,   515,   516,   517,     0,
     519,   520,   521,   522,     0,     0,     0,   523,     0,   525,
     526,     0,     0,     0,     0,   529,   530,   531,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   536,
       0,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     547,   548,   497,   498,   822,     0,     0,     0,   507,   508,
     509,   510,   511,     0,     0,   514,   515,   516,   517,     0,
     519,   520,   521,   522,     0,     0,     0,   523,     0,   525,
     526,     0,     0,     0,     0,   529,   530,   531,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   536,
       0,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     547,   548,   497,   498,   923,   507,   508,   509,   510,   511,
       0,     0,   514,   515,   516,   517,     0,   519,   520,   521,
     522,     0,     0,     0,   523,     0,   525,   526,     0,     0,
       0,     0,   529,   530,   531,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   547,   548,   497,
     498,   925,     0,     0,     0,   507,   508,   509,   510,   511,
       0,     0,   514,   515,   516,   517,     0,   519,   520,   521,
     522,     0,     0,     0,   523,     0,   525,   526,     0,     0,
       0,     0,   529,   530,   531,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,     0,     0,
       0,     0,     0,     0,   497,   498,     0,   547,   548,     0,
       0,   940,   507,   508,   509,   510,   511,     0,     0,   514,
     515,   516,   517,     0,   519,   520,   521,   522,     0,     0,
       0,   523,     0,   525,   526,     0,     0,     0,     0,   529,
     530,   531,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,     0,     0,     0,     0,   497,
     498,     0,     0,     0,   547,   548,   579,   507,   508,   509,
     510,   511,     0,     0,   514,   515,   516,   517,     0,   519,
     520,   521,   522,     0,     0,     0,   523,     0,   525,   526,
       0,     0,     0,     0,   529,   530,   531,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   547,
     548,   790,   507,   508,   509,   510,   511,   497,   498,   514,
     515,   516,   517,     0,   519,   520,   521,   522,     0,     0,
       0,   523,     0,   525,   526,     0,     0,     0,     0,   529,
     530,   531,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   547,   548,   796,     0,     0,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   591,   519,   520,   521,   522,  -228,   497,   498,   523,
     524,   525,   526,   527,   528,     0,     0,   529,   530,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     592,   536,     0,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   547,   548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,     0,     0,     0,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,     0,     0,     0,   523,
     524,   525,   526,   527,   528,     0,     0,   529,   530,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,     0,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   497,   498,     0,     0,   507,   508,   509,
     510,   511,   547,   548,   514,   515,   516,   517,     0,   519,
     520,   521,   522,     0,     0,     0,   523,     0,   525,   526,
       0,     0,   625,     0,   529,   530,   531,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     497,   498,     0,     0,     0,     0,     0,     0,     0,   547,
     548,     0,     0,     0,     0,     0,   507,   508,   509,   510,
     511,     0,     0,   514,   515,   516,   517,     0,   519,   520,
     521,   522,     0,     0,     0,   523,     0,   525,   526,     0,
       0,     0,     0,   529,   530,   531,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   536,   794,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   497,
     498,     0,     0,   507,   508,   509,   510,   511,   547,   548,
     514,   515,   516,   517,     0,   519,   520,   521,   522,     0,
       0,     0,   523,     0,   525,   526,     0,     0,     0,     0,
     529,   530,   531,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     844,     0,     0,     0,   536,     0,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   497,   498,     0,     0,
       0,     0,     0,     0,     0,   547,   548,     0,     0,     0,
       0,     0,   507,   508,   509,   510,   511,     0,     0,   514,
     515,   516,   517,     0,   519,   520,   521,   522,     0,     0,
       0,   523,     0,   525,   526,     0,     0,     0,     0,   529,
     530,   531,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   497,   498,     0,     0,   507,
     508,   509,   510,   511,   547,   548,   514,   515,   516,   517,
       0,   519,   520,   521,   522,     0,     0,     0,   523,     0,
     525,   526,     0,   497,   498,     0,   529,     0,   531,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   547,   548,     0,     0,     0,     0,     0,   507,   508,
     509,   510,   511,     0,     0,   514,   515,   516,   517,     0,
     519,   520,   521,   522,     0,     0,     0,   523,     0,   525,
     526,   497,   498,     0,     0,   529,   507,   508,   509,   510,
     511,     0,     0,   514,   515,   516,   517,     0,   519,   520,
     521,   522,     0,     0,     0,   523,     0,   525,   526,   497,
     498,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     547,   548,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,   540,   541,   542,   543,   544,   545,   546,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   547,   548,
       0,     0,     0,     0,   507,   508,   509,   510,   511,     0,
       0,   514,   515,   516,   517,     0,   519,   520,   521,   522,
       0,     0,     0,   523,     0,   525,   526,   497,   498,     0,
       0,     0,   507,   508,   509,   510,   511,     0,     0,   514,
     515,   516,   517,     0,   519,   520,   521,   522,     0,     0,
       0,   523,     0,   525,   526,   497,   498,     0,     0,   539,
     540,   541,   542,   543,   544,   545,   546,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   547,   548,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   540,   541,
     542,   543,   544,   545,   546,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   547,   548,     0,     0,     0,     0,
     507,   508,   509,   510,   511,     0,     0,   514,     0,     0,
     517,     0,   519,   520,   521,   522,     0,     0,     0,   523,
       0,   525,   526,     0,     0,     0,     0,     0,   507,   508,
     509,   510,     0,     0,     0,     0,     0,     0,   517,     0,
     519,   520,   521,   522,     0,     0,     0,   523,     0,   525,
     526,     0,     0,     0,     0,     0,   540,   541,   542,   543,
     544,   545,   546,   468,     0,     0,     0,     0,     0,     0,
       0,     0,   547,   548,     0,   193,     0,     0,     0,     0,
       0,   194,     0,     0,     0,     0,   542,   543,   544,   545,
     546,     0,     0,     0,     0,     0,   195,     0,     0,     0,
     547,   548,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   196,     0,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,     0,     0,     0,     0,     0,   470,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   193,     0,     0,     0,     0,     0,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,   195,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,   196,     0,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   193,     0,     0,
       0,     0,     0,   194,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,   195,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,   196,     0,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,     0,     0,     0,   194,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,   195,     0,     0,     0,     0,   328,
     329,   330,     0,     0,     0,     0,     0,     0,    47,   196,
       0,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   193,
       0,     0,     0,     0,     0,   194,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
     195,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,   196,     0,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,     0,     0,
       0,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   145,   146,   147,    46,     0,   148,   149,   150,   151,
     152,   153,     0,     0,     0,     0,     0,     0,     0,     0,
     573,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,     0,   164,   165
};

static const yytype_int16 yycheck[] =
{
      13,    87,   247,   242,   115,   287,   289,   291,   408,   330,
     410,   305,   412,   175,    46,   549,   471,   615,   326,   327,
     568,     7,   570,    79,   572,   623,    18,   637,    19,   323,
       0,    14,    15,    20,    21,    34,     6,    49,   115,    21,
      33,    19,   136,   128,   114,    49,    19,   119,   129,   145,
     289,   585,   586,   123,   145,   116,   117,   145,   145,    29,
     299,    31,   145,    33,   145,    35,    59,    80,   177,    39,
     114,    53,   114,   182,   152,    32,    46,   145,   159,   123,
     145,   123,    52,   153,   180,   179,   113,   129,   158,   116,
     117,   139,   129,   184,    32,   178,   184,   184,   183,    56,
      57,    71,   115,   397,   182,   152,   105,   106,   145,   153,
     182,   153,   155,   145,   175,   176,   103,   104,   114,   184,
     107,   145,    92,   110,   111,   112,   113,   123,   115,   116,
     117,   118,   179,   145,   245,   122,   129,   124,   125,   433,
     183,   751,   253,   130,   131,   132,   145,   441,   175,   176,
     136,   749,   145,   145,   152,   114,   139,   153,   115,   172,
     143,   152,   145,   146,   123,   145,   460,   154,   245,   156,
     157,   158,   159,   160,   152,   286,   253,   164,   182,   152,
     152,   179,   164,   183,   240,    20,    21,   495,   175,   176,
     176,   302,   153,   175,   153,   178,   153,   154,   184,   152,
     145,   158,   145,   152,   161,   152,   176,   153,   370,   286,
     182,   745,   182,    14,    15,   152,   114,   328,   329,   176,
     158,   178,   308,   757,   129,   302,   179,   155,   185,   468,
     179,   470,   245,   831,   320,   182,   182,   785,   176,   184,
     253,   184,   642,   841,   152,   152,   160,   185,   152,   152,
     155,   328,   329,   787,   152,   153,   152,   155,   145,   155,
     158,   152,   158,   557,     4,     5,   160,     7,   182,   177,
     105,   106,   179,   286,   177,   179,    20,    21,   113,   152,
     115,   116,   117,   118,    32,   152,   177,   122,   182,   302,
     401,   402,   403,   404,   749,   375,   407,   581,   409,   847,
     411,   609,   413,   758,   177,   626,    58,    59,    56,    57,
     177,   391,   155,   155,   155,   328,   329,   160,   160,   160,
     431,   160,   147,   148,   401,   402,   403,   404,   341,    66,
     407,   152,   409,    70,   411,   178,   178,   178,   139,    58,
     175,   176,   143,   182,   145,   146,    42,    43,   459,   435,
      87,    88,    89,    90,   431,   160,   179,    20,    21,   182,
     155,   105,   106,   449,    53,   160,   114,   115,   768,   113,
      59,   115,   116,   117,   118,   123,   145,   182,   122,   145,
     145,    11,   459,   178,   178,   145,    20,    21,   401,   402,
     403,   404,    22,    23,   407,   182,   409,   184,   411,   129,
     413,   487,   147,   148,   149,   153,   154,   183,   180,   181,
     158,   183,   182,   161,   184,   139,   145,   145,   431,   163,
     164,   165,   145,   706,   183,   183,   439,   153,   176,   145,
      34,   175,   176,    34,   182,   178,   183,   185,   155,   145,
     103,   104,   105,   106,   107,   727,   459,   110,   111,   112,
     113,    53,   115,   116,   117,   118,   147,   145,   183,   122,
     145,   124,   125,   155,   159,    20,    21,   706,   183,   159,
     483,   105,   106,   159,   178,   159,   159,   588,   159,   113,
     159,   115,   116,   117,   118,   159,   597,   598,   122,   600,
     159,   159,   145,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   155,   182,   182,   145,    32,   594,   136,
     159,   588,   175,   176,   178,    32,   178,   159,   159,   178,
     597,   598,   182,   600,   769,   139,   159,   161,   162,   163,
     164,   165,    32,   247,   159,   181,   176,   550,    36,    56,
      57,   175,   176,   178,   182,   182,   182,   261,    32,   833,
     105,   106,   119,   182,   182,   179,    56,    57,   113,   177,
     177,   116,   117,   118,    13,    32,   155,   122,   282,   139,
     181,   183,    56,    57,   136,   588,   145,   145,     7,   145,
     145,    42,   296,   182,   597,   598,   177,   600,   152,    56,
      57,   305,   152,   147,   184,   309,   147,   311,   115,   145,
     159,   712,   178,   145,     1,   319,   145,   160,   160,   323,
     324,   325,   723,   160,   725,   115,   330,   145,   145,   177,
     175,   176,   145,   184,   338,   339,   340,   177,   342,   343,
     344,   115,   177,   177,   147,   712,   153,   154,   145,   353,
     145,   158,   145,   178,   161,   182,   723,   177,   115,   145,
     179,   177,   184,   153,   154,   177,   767,   177,   158,   176,
     160,   161,   177,   177,   182,   182,   160,   160,   185,   153,
     154,   177,    49,   177,   158,   177,   176,   161,   177,   475,
     177,    59,   104,   397,   770,   185,   153,   154,    62,   184,
     767,   158,   176,   160,   161,   891,    60,    80,   182,   712,
     550,   185,   258,   866,   635,   800,     1,   857,   743,   176,
     723,   391,   725,   293,   704,   725,   414,   393,   185,   433,
     190,    72,   756,    32,   406,   302,   914,   441,    -1,   471,
      -1,    -1,    -1,   819,   448,    -1,    -1,    -1,   452,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   460,    56,    57,   860,
      -1,    -1,    -1,    -1,   767,   469,    -1,   471,    -1,    -1,
      -1,    -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   859,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   860,    -1,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,    -1,    -1,   511,   512,   513,
     514,   515,   516,    -1,   518,   519,   115,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,    -1,   548,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   557,   153,   154,    -1,   860,    -1,   158,
      -1,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,   587,    -1,    32,   185,   591,   592,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   611,   612,    56,
      57,    32,    -1,   916,    -1,    -1,    -1,    -1,   622,    -1,
      -1,   625,   626,    -1,    -1,   629,    -1,    -1,    -1,   932,
     103,   104,   105,   106,   107,    56,    57,   110,   111,   112,
     113,    32,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
     664,   665,    -1,    -1,    -1,    56,    57,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   683,
     684,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    -1,   153,   154,    -1,    -1,
     183,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   115,   729,    -1,    -1,    -1,   176,
      -1,    -1,   153,   154,    -1,    -1,    -1,   158,   185,   160,
     161,    -1,    -1,   747,   748,    -1,    -1,    -1,    -1,    -1,
     754,    -1,   756,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   153,   154,   185,   769,    -1,   158,    -1,   773,
     161,    -1,    -1,   777,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
     794,    -1,    -1,    -1,   185,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,   873,
      32,    -1,   876,   877,   878,   879,   880,   175,   176,   883,
     884,    -1,   886,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,   895,    -1,    -1,    56,    57,    -1,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,   919,    -1,    -1,    -1,    -1,
     924,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   936,    37,    38,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,   115,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      94,   153,   154,    -1,    -1,    -1,   158,    -1,   160,   161,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,   185,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,
      -1,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,   183,
     184,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    41,    -1,    -1,    44,    -1,    46,    47,    48,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    32,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,   169,
      -1,    -1,    -1,    14,    15,    16,    17,    18,   178,    -1,
     180,   181,   182,   183,   184,    26,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,   115,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,   153,   154,    -1,    -1,    -1,
     158,    -1,   160,   161,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   185,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    56,
      57,    -1,   153,   154,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    14,
      15,    16,    17,    18,    -1,   176,    -1,   178,    -1,   180,
     181,    26,   183,    -1,   185,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,   114,   115,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   153,   154,    93,    -1,
      -1,   158,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    32,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    56,    57,    -1,   153,   154,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    14,    15,    16,    17,    18,
      -1,   176,    -1,   178,    -1,   180,   181,    26,   183,    -1,
     185,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   153,   154,    93,    -1,    -1,   158,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
     129,    32,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    14,    15,    16,    17,    18,    -1,   176,    -1,   178,
      -1,   180,   181,    26,   183,    -1,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,   153,   154,    -1,    -1,    -1,   158,    -1,   160,
     161,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,   176,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,   185,    -1,   129,    -1,    32,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   178,    -1,   180,   181,   182,
     183,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,   153,
     154,    -1,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
     105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,   176,    -1,    -1,   120,   121,    -1,    -1,    -1,
      -1,   185,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   178,    41,   180,   181,    44,   183,    -1,
      47,    -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   153,   154,    93,    -1,    -1,   158,
      -1,   160,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,   185,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   153,   154,    93,    -1,    -1,   158,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    32,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    14,    15,    16,    17,    18,    -1,   176,    -1,   178,
     179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   153,   154,
      93,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
     185,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,
      57,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,   161,   162,
     163,    26,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   178,    41,   180,   181,    44,
     183,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   153,   154,    93,    -1,
      -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   185,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   178,    41,   180,   181,    44,   183,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
     177,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      21,    -1,   161,   162,   163,    26,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,
      41,   180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     161,   162,   163,    26,   165,    -1,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   178,    41,   180,
     181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,
      -1,   168,   169,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   178,   179,   180,   181,    26,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,   159,    -1,
     161,   162,   163,    26,   165,    -1,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   178,    41,   180,
     181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,
      -1,   168,   169,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   178,   179,   180,   181,    26,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   178,   179,   180,
     181,    26,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   178,   179,   180,   181,    26,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   161,   162,   163,    26,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,
      41,   180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,   180,
     181,    -1,   183,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    20,    21,    -1,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    20,    21,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    -1,    -1,    -1,    -1,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    20,    21,    -1,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,   176,    -1,    -1,    -1,    -1,    -1,   182,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    20,    21,    -1,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,    -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    20,    21,    -1,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   175,   176,
     103,   104,   105,   106,   107,   182,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,   175,   176,   103,   104,   105,   106,   107,   182,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    20,    21,
     179,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   176,    20,    21,   179,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   176,    20,    21,   179,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,    20,    21,   179,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,    20,    21,   179,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    20,
      21,   179,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,   175,   176,    -1,
      -1,   179,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   175,   176,   177,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,   177,   103,   104,   105,   106,   107,    20,    21,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    20,    21,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    20,    21,    -1,    -1,   103,   104,   105,
     106,   107,   175,   176,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,   128,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    20,
      21,    -1,    -1,   103,   104,   105,   106,   107,   175,   176,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    20,    21,    -1,    -1,   103,
     104,   105,   106,   107,   175,   176,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    20,    21,    -1,   130,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,   176,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    20,    21,    -1,    -1,   130,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,
      21,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    20,    21,    -1,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,   162,
     163,   164,   165,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    45,    -1,    -1,    -1,    -1,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,   145,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   129,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,   168,   169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   187,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    71,    92,   176,   182,   190,   198,   199,   203,   223,
     227,   242,   297,   302,   303,   307,   343,   345,   145,   218,
     219,   139,   204,   205,   145,   200,   201,   145,   183,   145,
     180,   189,   346,   344,    33,    59,   129,   145,   220,   221,
     222,   234,     4,     5,     7,   306,    58,    59,   296,   153,
     152,   155,   152,    21,    53,   164,   175,   202,   155,   183,
     145,   300,   301,    58,   295,   145,   145,   145,   129,   178,
     152,   177,    34,   105,   106,   145,   226,   228,   229,   145,
     305,   183,   298,    14,    15,   139,   143,   145,   146,   178,
     192,   218,   139,   205,   145,   145,   145,    66,    70,    87,
      88,    89,    90,   311,   299,   153,   184,   300,    53,    59,
     188,   183,   183,   145,   219,   221,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   124,   125,   126,   127,   130,   131,
     132,   133,   134,   135,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   168,   169,    34,    34,   178,   224,
     183,   230,   155,   304,   308,   294,   145,   293,   309,   147,
     191,    14,    15,   139,   143,   145,   192,   216,   217,   202,
     183,   145,   184,    24,    30,    45,    60,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,   234,   310,
     312,   313,   315,   318,   145,   314,   145,   287,   288,   179,
     136,   179,   278,   283,   284,   155,   225,   232,   234,   183,
     184,   293,   152,   155,   158,   292,   147,   148,   152,   179,
     299,   153,   182,   159,   159,   159,   159,   178,   159,   159,
     159,   159,   159,   159,    32,    56,    57,   115,   153,   154,
     158,   161,   176,   182,   185,   182,   155,   182,   219,    46,
     290,   309,   179,   182,   318,     1,     8,     9,    10,    12,
      14,    15,    16,    17,    18,    25,    26,    27,    28,    37,
      38,    40,    41,    44,    47,    48,    50,    51,    54,    55,
      61,    71,    93,    94,   105,   106,   120,   121,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   146,   161,   162,
     163,   165,   168,   169,   178,   180,   181,   182,   184,   194,
     196,   206,   207,   210,   211,   212,   213,   214,   215,   230,
     231,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   251,   254,   257,   261,   262,   263,   264,
     265,   266,   267,   272,   275,   310,   347,   351,   356,   358,
     360,   281,   145,   318,   114,   123,   153,   291,   216,   184,
     275,   321,   323,   325,   319,   145,   316,   327,   329,   331,
     333,   335,   337,   339,   341,    32,    32,   158,   176,   185,
     177,   275,   145,   184,   318,   184,   287,   177,   290,   114,
     153,   155,   158,   289,   283,   275,   309,   318,   347,   178,
     159,   114,   182,   233,   263,   275,   230,   275,   178,   275,
     159,   159,   178,   182,   159,   139,    54,   275,   230,   159,
     114,   233,   275,   275,   275,   181,   261,   261,    12,   318,
      12,   318,   275,   353,   357,   193,   275,   275,   275,   234,
     275,   275,   275,   181,    36,   182,   182,   275,   145,   178,
     182,   182,   182,   293,   223,   119,   182,    20,    21,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   122,   123,   124,   125,   126,   127,   130,
     131,   132,   133,   134,   135,   153,   154,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   175,   176,   178,
     176,   184,   223,   278,   280,   153,   182,   291,   233,   275,
     182,   318,   318,   318,   318,   179,   314,   318,   224,   318,
     224,   318,   224,   145,   285,   286,   318,   288,   177,   177,
     177,   318,   233,   275,   230,    19,   178,   234,   252,   233,
     275,   114,   153,   182,    13,   275,   230,   245,   247,   275,
     249,   155,   139,   318,   233,   275,   182,   181,   309,    49,
     145,   161,   178,   275,   348,   349,   350,   352,   353,   354,
     355,   309,   178,   349,   355,   128,   182,   184,   148,   149,
     191,   197,   179,   159,   234,   183,   230,   176,   179,   255,
     275,   136,   260,   261,    18,   145,   145,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   145,   120,   121,   275,   275,   145,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   120,   121,   275,    21,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   145,   273,
     275,   179,   255,   222,     7,    42,   279,   182,   233,   275,
     182,   160,   182,   160,   160,   160,   182,   160,   225,   160,
     225,   160,   225,   155,   160,   182,   160,   182,   153,   289,
     255,   179,   255,   159,   275,   318,   182,   230,   179,   318,
     318,   179,   318,   160,   182,    19,   261,   123,   289,   179,
     152,   182,   350,   177,   152,   177,   182,    19,   179,   350,
     184,   275,   353,   184,   275,   147,   195,   268,   270,   232,
      11,    22,    23,   208,   209,   145,   348,   152,   179,   114,
     123,   153,   158,   258,   259,   224,   159,   178,   275,   275,
     177,   275,   275,   145,   155,     1,   177,   179,   177,    43,
     279,   290,   182,   322,   318,   326,   320,   317,   328,   160,
     332,   160,   336,   160,   318,   340,   285,   342,   275,   182,
     179,   145,   179,   160,   160,   160,   160,   260,   255,   275,
     275,   349,   350,   145,   348,   177,   177,   275,   177,   354,
     255,   349,   184,   177,   150,   147,   318,   224,   184,   230,
     275,   177,   275,   145,   145,   145,   145,   152,   177,   225,
     276,   179,   255,   275,   274,   182,   282,   229,   160,   330,
     334,   338,   230,   160,   182,   253,   246,   248,   250,   178,
     182,   350,   177,   123,   289,   177,   182,   350,   177,   160,
     225,   230,   179,   258,   177,   128,   230,   256,   318,   179,
     228,   324,   275,   145,   275,   275,   275,   179,   275,   275,
     177,   177,   275,   275,   275,   184,   269,   160,   209,   114,
     275,   160,   182,   179,   160,   179,   182,   359,   177,   359,
     177,   234,   271,   275,   277,   275,    49,   177,   184,   234,
     179,   275,   177,   177
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   186,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   188,   188,   188,   189,   189,
     190,   191,   191,   192,   193,   193,   193,   194,   195,   195,
     197,   196,   198,   199,   200,   200,   200,   201,   201,   202,
     202,   203,   204,   204,   205,   205,   206,   207,   207,   208,
     208,   209,   209,   209,   210,   210,   211,   212,   213,   214,
     215,   216,   216,   216,   216,   216,   216,   217,   217,   218,
     218,   218,   218,   218,   218,   218,   218,   219,   219,   220,
     220,   220,   221,   221,   222,   222,   223,   223,   224,   224,
     224,   225,   225,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   227,   228,   229,   230,
     230,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     232,   232,   232,   233,   233,   233,   234,   234,   234,   235,
     236,   236,   236,   236,   237,   238,   239,   239,   239,   239,
     239,   240,   240,   240,   240,   241,   242,   242,   243,   245,
     246,   244,   247,   248,   244,   249,   250,   244,   252,   253,
     251,   254,   254,   254,   255,   255,   256,   256,   256,   257,
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
     275,   275,   275,   275,   275,   278,   278,   279,   279,   280,
     281,   281,   282,   281,   281,   281,   283,   283,   284,   284,
     285,   285,   286,   286,   287,   288,   288,   289,   289,   290,
     290,   290,   290,   290,   290,   291,   291,   291,   292,   292,
     293,   293,   293,   293,   293,   294,   294,   295,   295,   296,
     296,   296,   297,   298,   297,   299,   299,   299,   300,   301,
     301,   302,   303,   303,   304,   304,   305,   306,   306,   308,
     307,   309,   309,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   311,   311,
     311,   311,   311,   311,   312,   313,   313,   314,   314,   316,
     317,   315,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   319,
     320,   318,   318,   321,   322,   318,   323,   324,   318,   325,
     326,   318,   318,   327,   328,   318,   329,   330,   318,   318,
     331,   332,   318,   333,   334,   318,   318,   335,   336,   318,
     337,   338,   318,   339,   340,   318,   341,   342,   318,   344,
     343,   346,   345,   347,   347,   347,   347,   348,   348,   348,
     348,   349,   349,   350,   350,   351,   351,   351,   351,   351,
     351,   352,   352,   353,   354,   354,   355,   355,   356,   356,
     357,   357,   358,   359,   359,   360,   360
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     2,     3,     0,     2,     4,     3,     1,     2,
       0,     4,     2,     2,     1,     3,     3,     2,     4,     0,
       1,     2,     1,     3,     1,     3,     3,     3,     2,     1,
       1,     0,     2,     4,     1,     1,     4,     6,     2,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     1,     4,     1,     3,     0,     3,     0,     2,
       3,     0,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     2,     3,
       7,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       0,     2,     2,     3,     2,     2,     1,     3,     2,     2,
       2,     4,     5,     2,     1,     1,     2,     3,     4,     2,
       3,     3,     4,     2,     3,     4,     1,     1,     2,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     1,     3,     1,     2,     3,     1,
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
       1,     1,     3,     3,     2,     0,     4,     0,     1,     3,
       0,     3,     0,     7,     5,     5,     2,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     1,     1,
       2,     3,     5,     3,     3,     1,     1,     1,     0,     1,
       4,     6,     5,     5,     4,     0,     2,     0,     1,     0,
       1,     1,     5,     0,     4,     0,     3,     5,     4,     1,
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
#line 3392 "ds_parser.cpp"
        break;

    case 189: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3398 "ds_parser.cpp"
        break;

    case 191: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3404 "ds_parser.cpp"
        break;

    case 192: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3410 "ds_parser.cpp"
        break;

    case 193: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3416 "ds_parser.cpp"
        break;

    case 194: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3422 "ds_parser.cpp"
        break;

    case 196: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3428 "ds_parser.cpp"
        break;

    case 200: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3434 "ds_parser.cpp"
        break;

    case 206: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3440 "ds_parser.cpp"
        break;

    case 207: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3446 "ds_parser.cpp"
        break;

    case 209: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3452 "ds_parser.cpp"
        break;

    case 211: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3458 "ds_parser.cpp"
        break;

    case 212: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3464 "ds_parser.cpp"
        break;

    case 213: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3470 "ds_parser.cpp"
        break;

    case 214: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3476 "ds_parser.cpp"
        break;

    case 215: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3482 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3488 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3494 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3500 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3506 "ds_parser.cpp"
        break;

    case 220: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3512 "ds_parser.cpp"
        break;

    case 221: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3518 "ds_parser.cpp"
        break;

    case 222: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3524 "ds_parser.cpp"
        break;

    case 223: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3530 "ds_parser.cpp"
        break;

    case 224: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3536 "ds_parser.cpp"
        break;

    case 225: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3542 "ds_parser.cpp"
        break;

    case 226: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3548 "ds_parser.cpp"
        break;

    case 230: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3554 "ds_parser.cpp"
        break;

    case 231: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3560 "ds_parser.cpp"
        break;

    case 232: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3566 "ds_parser.cpp"
        break;

    case 233: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3572 "ds_parser.cpp"
        break;

    case 234: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3578 "ds_parser.cpp"
        break;

    case 235: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3584 "ds_parser.cpp"
        break;

    case 236: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3590 "ds_parser.cpp"
        break;

    case 237: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3596 "ds_parser.cpp"
        break;

    case 238: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3602 "ds_parser.cpp"
        break;

    case 239: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3608 "ds_parser.cpp"
        break;

    case 240: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3614 "ds_parser.cpp"
        break;

    case 241: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3620 "ds_parser.cpp"
        break;

    case 243: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3626 "ds_parser.cpp"
        break;

    case 244: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3632 "ds_parser.cpp"
        break;

    case 251: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3638 "ds_parser.cpp"
        break;

    case 254: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3644 "ds_parser.cpp"
        break;

    case 255: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3650 "ds_parser.cpp"
        break;

    case 256: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3656 "ds_parser.cpp"
        break;

    case 258: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3662 "ds_parser.cpp"
        break;

    case 259: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3668 "ds_parser.cpp"
        break;

    case 260: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3674 "ds_parser.cpp"
        break;

    case 261: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3680 "ds_parser.cpp"
        break;

    case 262: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3686 "ds_parser.cpp"
        break;

    case 263: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3692 "ds_parser.cpp"
        break;

    case 264: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3698 "ds_parser.cpp"
        break;

    case 265: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3704 "ds_parser.cpp"
        break;

    case 266: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3710 "ds_parser.cpp"
        break;

    case 267: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3716 "ds_parser.cpp"
        break;

    case 272: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3722 "ds_parser.cpp"
        break;

    case 275: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3728 "ds_parser.cpp"
        break;

    case 278: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3734 "ds_parser.cpp"
        break;

    case 280: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3740 "ds_parser.cpp"
        break;

    case 281: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3746 "ds_parser.cpp"
        break;

    case 283: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3752 "ds_parser.cpp"
        break;

    case 284: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3758 "ds_parser.cpp"
        break;

    case 285: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3764 "ds_parser.cpp"
        break;

    case 286: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3770 "ds_parser.cpp"
        break;

    case 287: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3776 "ds_parser.cpp"
        break;

    case 288: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3782 "ds_parser.cpp"
        break;

    case 290: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3788 "ds_parser.cpp"
        break;

    case 293: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3794 "ds_parser.cpp"
        break;

    case 294: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3800 "ds_parser.cpp"
        break;

    case 299: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3806 "ds_parser.cpp"
        break;

    case 304: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3812 "ds_parser.cpp"
        break;

    case 309: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3818 "ds_parser.cpp"
        break;

    case 312: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3824 "ds_parser.cpp"
        break;

    case 313: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3830 "ds_parser.cpp"
        break;

    case 314: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3836 "ds_parser.cpp"
        break;

    case 315: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3842 "ds_parser.cpp"
        break;

    case 318: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3848 "ds_parser.cpp"
        break;

    case 347: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3854 "ds_parser.cpp"
        break;

    case 348: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3860 "ds_parser.cpp"
        break;

    case 349: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3866 "ds_parser.cpp"
        break;

    case 350: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3872 "ds_parser.cpp"
        break;

    case 351: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3878 "ds_parser.cpp"
        break;

    case 352: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3884 "ds_parser.cpp"
        break;

    case 353: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3890 "ds_parser.cpp"
        break;

    case 354: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3896 "ds_parser.cpp"
        break;

    case 355: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3902 "ds_parser.cpp"
        break;

    case 356: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3908 "ds_parser.cpp"
        break;

    case 357: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3914 "ds_parser.cpp"
        break;

    case 358: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3920 "ds_parser.cpp"
        break;

    case 359: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3926 "ds_parser.cpp"
        break;

    case 360: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3932 "ds_parser.cpp"
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
#line 464 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4224 "ds_parser.cpp"
    break;

  case 16:
#line 465 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4230 "ds_parser.cpp"
    break;

  case 17:
#line 466 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4236 "ds_parser.cpp"
    break;

  case 18:
#line 470 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4242 "ds_parser.cpp"
    break;

  case 19:
#line 471 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4248 "ds_parser.cpp"
    break;

  case 20:
#line 475 "ds_parser.ypp"
    {
        g_Program->thisModule->isPublic = (yyvsp[0].b);
        g_Program->thisModule->isModule = true;
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !g_Program->policies.ignore_shared_modules ) {
            g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }
#line 4267 "ds_parser.cpp"
    break;

  case 21:
#line 492 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4273 "ds_parser.cpp"
    break;

  case 22:
#line 493 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4279 "ds_parser.cpp"
    break;

  case 23:
#line 497 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4285 "ds_parser.cpp"
    break;

  case 24:
#line 501 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4295 "ds_parser.cpp"
    break;

  case 25:
#line 506 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4309 "ds_parser.cpp"
    break;

  case 26:
#line 515 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4319 "ds_parser.cpp"
    break;

  case 27:
#line 523 "ds_parser.ypp"
    {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(tokRangeAt((yylsp[-2]),(yylsp[0])),"");
            delete (yyvsp[-1].pExpression);
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_pointer_cast<ExprConstString>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt((yylsp[-2]),(yylsp[0])),sconst->text);
            delete (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
#line 4337 "ds_parser.cpp"
    break;

  case 28:
#line 539 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4347 "ds_parser.cpp"
    break;

  case 29:
#line 544 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4357 "ds_parser.cpp"
    break;

  case 30:
#line 552 "ds_parser.ypp"
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
#line 4384 "ds_parser.cpp"
    break;

  case 31:
#line 573 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4396 "ds_parser.cpp"
    break;

  case 32:
#line 582 "ds_parser.ypp"
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
#line 4420 "ds_parser.cpp"
    break;

  case 34:
#line 608 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4428 "ds_parser.cpp"
    break;

  case 35:
#line 611 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4439 "ds_parser.cpp"
    break;

  case 36:
#line 617 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4450 "ds_parser.cpp"
    break;

  case 37:
#line 626 "ds_parser.ypp"
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
#line 4476 "ds_parser.cpp"
    break;

  case 38:
#line 647 "ds_parser.ypp"
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
#line 4501 "ds_parser.cpp"
    break;

  case 39:
#line 670 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4507 "ds_parser.cpp"
    break;

  case 40:
#line 671 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4513 "ds_parser.cpp"
    break;

  case 44:
#line 684 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4521 "ds_parser.cpp"
    break;

  case 45:
#line 687 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4529 "ds_parser.cpp"
    break;

  case 46:
#line 693 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4537 "ds_parser.cpp"
    break;

  case 47:
#line 699 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4545 "ds_parser.cpp"
    break;

  case 48:
#line 702 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4553 "ds_parser.cpp"
    break;

  case 49:
#line 708 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4559 "ds_parser.cpp"
    break;

  case 50:
#line 709 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4565 "ds_parser.cpp"
    break;

  case 51:
#line 713 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4571 "ds_parser.cpp"
    break;

  case 52:
#line 714 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4577 "ds_parser.cpp"
    break;

  case 53:
#line 715 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4588 "ds_parser.cpp"
    break;

  case 54:
#line 724 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4594 "ds_parser.cpp"
    break;

  case 55:
#line 725 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4600 "ds_parser.cpp"
    break;

  case 56:
#line 729 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4611 "ds_parser.cpp"
    break;

  case 57:
#line 738 "ds_parser.ypp"
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
#line 4629 "ds_parser.cpp"
    break;

  case 58:
#line 754 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4639 "ds_parser.cpp"
    break;

  case 59:
#line 762 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4651 "ds_parser.cpp"
    break;

  case 60:
#line 772 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4662 "ds_parser.cpp"
    break;

  case 61:
#line 781 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4668 "ds_parser.cpp"
    break;

  case 62:
#line 782 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4674 "ds_parser.cpp"
    break;

  case 63:
#line 783 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4680 "ds_parser.cpp"
    break;

  case 64:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4686 "ds_parser.cpp"
    break;

  case 65:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4692 "ds_parser.cpp"
    break;

  case 66:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4698 "ds_parser.cpp"
    break;

  case 67:
#line 790 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4708 "ds_parser.cpp"
    break;

  case 68:
#line 795 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4718 "ds_parser.cpp"
    break;

  case 69:
#line 803 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4724 "ds_parser.cpp"
    break;

  case 70:
#line 804 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4730 "ds_parser.cpp"
    break;

  case 71:
#line 805 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4736 "ds_parser.cpp"
    break;

  case 72:
#line 806 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4742 "ds_parser.cpp"
    break;

  case 73:
#line 807 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4748 "ds_parser.cpp"
    break;

  case 74:
#line 808 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4754 "ds_parser.cpp"
    break;

  case 75:
#line 809 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4760 "ds_parser.cpp"
    break;

  case 76:
#line 810 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4768 "ds_parser.cpp"
    break;

  case 77:
#line 816 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        if ( (yyvsp[0].aa)->type==Type::none ) {
            for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
                sarg.name = (yyvsp[0].aa)->name;
                sarg.at = (yyvsp[0].aa)->at;
                (yyval.aaList)->push_back(sarg);
            }
            delete (yyvsp[0].aa)->aList;
        } else {
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
        }
        delete (yyvsp[0].aa);
    }
#line 4787 "ds_parser.cpp"
    break;

  case 78:
#line 830 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            if ( (yyvsp[0].aa)->type==Type::none ) {
                for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
                    sarg.name = (yyvsp[0].aa)->name;
                    sarg.at = (yyvsp[0].aa)->at;
                    (yyval.aaList)->push_back(sarg);
                }
                delete (yyvsp[0].aa)->aList;
            } else {
                (yyval.aaList)->push_back(*(yyvsp[0].aa));
            }
            delete (yyvsp[0].aa);
    }
#line 4806 "ds_parser.cpp"
    break;

  case 79:
#line 847 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4812 "ds_parser.cpp"
    break;

  case 80:
#line 848 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4818 "ds_parser.cpp"
    break;

  case 81:
#line 849 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4824 "ds_parser.cpp"
    break;

  case 82:
#line 853 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4837 "ds_parser.cpp"
    break;

  case 83:
#line 861 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[-3]));
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4852 "ds_parser.cpp"
    break;

  case 84:
#line 874 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4861 "ds_parser.cpp"
    break;

  case 85:
#line 878 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4870 "ds_parser.cpp"
    break;

  case 86:
#line 885 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4876 "ds_parser.cpp"
    break;

  case 87:
#line 886 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4882 "ds_parser.cpp"
    break;

  case 88:
#line 890 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4888 "ds_parser.cpp"
    break;

  case 89:
#line 891 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4894 "ds_parser.cpp"
    break;

  case 90:
#line 892 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4900 "ds_parser.cpp"
    break;

  case 91:
#line 896 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4908 "ds_parser.cpp"
    break;

  case 92:
#line 899 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4917 "ds_parser.cpp"
    break;

  case 93:
#line 906 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4926 "ds_parser.cpp"
    break;

  case 94:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4932 "ds_parser.cpp"
    break;

  case 95:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4938 "ds_parser.cpp"
    break;

  case 96:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4944 "ds_parser.cpp"
    break;

  case 97:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4950 "ds_parser.cpp"
    break;

  case 98:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4956 "ds_parser.cpp"
    break;

  case 99:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4962 "ds_parser.cpp"
    break;

  case 100:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4968 "ds_parser.cpp"
    break;

  case 101:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4974 "ds_parser.cpp"
    break;

  case 102:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4980 "ds_parser.cpp"
    break;

  case 103:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4986 "ds_parser.cpp"
    break;

  case 104:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4992 "ds_parser.cpp"
    break;

  case 105:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4998 "ds_parser.cpp"
    break;

  case 106:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5004 "ds_parser.cpp"
    break;

  case 107:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5010 "ds_parser.cpp"
    break;

  case 108:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5016 "ds_parser.cpp"
    break;

  case 109:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5022 "ds_parser.cpp"
    break;

  case 110:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5028 "ds_parser.cpp"
    break;

  case 111:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5034 "ds_parser.cpp"
    break;

  case 112:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5040 "ds_parser.cpp"
    break;

  case 113:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5046 "ds_parser.cpp"
    break;

  case 114:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5052 "ds_parser.cpp"
    break;

  case 115:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5058 "ds_parser.cpp"
    break;

  case 116:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5064 "ds_parser.cpp"
    break;

  case 117:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5070 "ds_parser.cpp"
    break;

  case 118:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5076 "ds_parser.cpp"
    break;

  case 119:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5082 "ds_parser.cpp"
    break;

  case 120:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5088 "ds_parser.cpp"
    break;

  case 121:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5094 "ds_parser.cpp"
    break;

  case 122:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5100 "ds_parser.cpp"
    break;

  case 123:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5106 "ds_parser.cpp"
    break;

  case 124:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5112 "ds_parser.cpp"
    break;

  case 125:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5118 "ds_parser.cpp"
    break;

  case 126:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5124 "ds_parser.cpp"
    break;

  case 127:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5130 "ds_parser.cpp"
    break;

  case 128:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5136 "ds_parser.cpp"
    break;

  case 129:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5142 "ds_parser.cpp"
    break;

  case 130:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5148 "ds_parser.cpp"
    break;

  case 131:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5154 "ds_parser.cpp"
    break;

  case 132:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5160 "ds_parser.cpp"
    break;

  case 133:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5166 "ds_parser.cpp"
    break;

  case 134:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5172 "ds_parser.cpp"
    break;

  case 135:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5178 "ds_parser.cpp"
    break;

  case 136:
#line 955 "ds_parser.ypp"
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
#line 5202 "ds_parser.cpp"
    break;

  case 137:
#line 977 "ds_parser.ypp"
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
#line 5242 "ds_parser.cpp"
    break;

  case 138:
#line 1015 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5251 "ds_parser.cpp"
    break;

  case 139:
#line 1022 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5260 "ds_parser.cpp"
    break;

  case 140:
#line 1026 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5273 "ds_parser.cpp"
    break;

  case 141:
#line 1037 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5279 "ds_parser.cpp"
    break;

  case 142:
#line 1038 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5285 "ds_parser.cpp"
    break;

  case 143:
#line 1039 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5291 "ds_parser.cpp"
    break;

  case 144:
#line 1040 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5297 "ds_parser.cpp"
    break;

  case 145:
#line 1041 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5303 "ds_parser.cpp"
    break;

  case 146:
#line 1042 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5309 "ds_parser.cpp"
    break;

  case 147:
#line 1043 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5315 "ds_parser.cpp"
    break;

  case 148:
#line 1044 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5321 "ds_parser.cpp"
    break;

  case 149:
#line 1045 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5327 "ds_parser.cpp"
    break;

  case 150:
#line 1046 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5333 "ds_parser.cpp"
    break;

  case 151:
#line 1047 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5339 "ds_parser.cpp"
    break;

  case 152:
#line 1048 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5345 "ds_parser.cpp"
    break;

  case 153:
#line 1049 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5351 "ds_parser.cpp"
    break;

  case 154:
#line 1050 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5357 "ds_parser.cpp"
    break;

  case 155:
#line 1051 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5363 "ds_parser.cpp"
    break;

  case 156:
#line 1052 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5369 "ds_parser.cpp"
    break;

  case 157:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5375 "ds_parser.cpp"
    break;

  case 158:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5381 "ds_parser.cpp"
    break;

  case 159:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5387 "ds_parser.cpp"
    break;

  case 160:
#line 1059 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5397 "ds_parser.cpp"
    break;

  case 161:
#line 1064 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5408 "ds_parser.cpp"
    break;

  case 162:
#line 1070 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5416 "ds_parser.cpp"
    break;

  case 163:
#line 1076 "ds_parser.ypp"
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
#line 5455 "ds_parser.cpp"
    break;

  case 164:
#line 1110 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5463 "ds_parser.cpp"
    break;

  case 165:
#line 1113 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5471 "ds_parser.cpp"
    break;

  case 166:
#line 1119 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5477 "ds_parser.cpp"
    break;

  case 167:
#line 1120 "ds_parser.ypp"
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
#line 5493 "ds_parser.cpp"
    break;

  case 168:
#line 1131 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5499 "ds_parser.cpp"
    break;

  case 169:
#line 1135 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5507 "ds_parser.cpp"
    break;

  case 170:
#line 1141 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5515 "ds_parser.cpp"
    break;

  case 171:
#line 1144 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5523 "ds_parser.cpp"
    break;

  case 172:
#line 1147 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5532 "ds_parser.cpp"
    break;

  case 173:
#line 1151 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5540 "ds_parser.cpp"
    break;

  case 174:
#line 1157 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5546 "ds_parser.cpp"
    break;

  case 175:
#line 1161 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5552 "ds_parser.cpp"
    break;

  case 176:
#line 1165 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5560 "ds_parser.cpp"
    break;

  case 177:
#line 1168 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5568 "ds_parser.cpp"
    break;

  case 178:
#line 1171 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5578 "ds_parser.cpp"
    break;

  case 179:
#line 1176 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5586 "ds_parser.cpp"
    break;

  case 180:
#line 1179 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5596 "ds_parser.cpp"
    break;

  case 181:
#line 1187 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5604 "ds_parser.cpp"
    break;

  case 182:
#line 1190 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5614 "ds_parser.cpp"
    break;

  case 183:
#line 1195 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5622 "ds_parser.cpp"
    break;

  case 184:
#line 1198 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5632 "ds_parser.cpp"
    break;

  case 185:
#line 1206 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5640 "ds_parser.cpp"
    break;

  case 186:
#line 1212 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5646 "ds_parser.cpp"
    break;

  case 187:
#line 1213 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5652 "ds_parser.cpp"
    break;

  case 188:
#line 1217 "ds_parser.ypp"
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
#line 5688 "ds_parser.cpp"
    break;

  case 189:
#line 1251 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5694 "ds_parser.cpp"
    break;

  case 190:
#line 1251 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5700 "ds_parser.cpp"
    break;

  case 191:
#line 1251 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5708 "ds_parser.cpp"
    break;

  case 192:
#line 1254 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5714 "ds_parser.cpp"
    break;

  case 193:
#line 1254 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5720 "ds_parser.cpp"
    break;

  case 194:
#line 1254 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5730 "ds_parser.cpp"
    break;

  case 195:
#line 1259 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5736 "ds_parser.cpp"
    break;

  case 196:
#line 1259 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5742 "ds_parser.cpp"
    break;

  case 197:
#line 1259 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5752 "ds_parser.cpp"
    break;

  case 198:
#line 1267 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5758 "ds_parser.cpp"
    break;

  case 199:
#line 1267 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5764 "ds_parser.cpp"
    break;

  case 200:
#line 1267 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5772 "ds_parser.cpp"
    break;

  case 201:
#line 1273 "ds_parser.ypp"
    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-2].s);
    }
#line 5787 "ds_parser.cpp"
    break;

  case 202:
#line 1283 "ds_parser.ypp"
    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5803 "ds_parser.cpp"
    break;

  case 203:
#line 1294 "ds_parser.ypp"
    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5820 "ds_parser.cpp"
    break;

  case 204:
#line 1309 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5828 "ds_parser.cpp"
    break;

  case 205:
#line 1312 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5836 "ds_parser.cpp"
    break;

  case 206:
#line 1318 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5842 "ds_parser.cpp"
    break;

  case 207:
#line 1319 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5854 "ds_parser.cpp"
    break;

  case 208:
#line 1326 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5867 "ds_parser.cpp"
    break;

  case 209:
#line 1337 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5873 "ds_parser.cpp"
    break;

  case 210:
#line 1338 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5879 "ds_parser.cpp"
    break;

  case 211:
#line 1339 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5885 "ds_parser.cpp"
    break;

  case 212:
#line 1343 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5891 "ds_parser.cpp"
    break;

  case 213:
#line 1344 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5897 "ds_parser.cpp"
    break;

  case 214:
#line 1345 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5903 "ds_parser.cpp"
    break;

  case 215:
#line 1346 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5909 "ds_parser.cpp"
    break;

  case 216:
#line 1350 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5919 "ds_parser.cpp"
    break;

  case 217:
#line 1355 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5929 "ds_parser.cpp"
    break;

  case 218:
#line 1363 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5935 "ds_parser.cpp"
    break;

  case 219:
#line 1364 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5941 "ds_parser.cpp"
    break;

  case 220:
#line 1368 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5951 "ds_parser.cpp"
    break;

  case 221:
#line 1374 "ds_parser.ypp"
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
#line 6016 "ds_parser.cpp"
    break;

  case 222:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6022 "ds_parser.cpp"
    break;

  case 223:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6028 "ds_parser.cpp"
    break;

  case 224:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6034 "ds_parser.cpp"
    break;

  case 225:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6040 "ds_parser.cpp"
    break;

  case 226:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6046 "ds_parser.cpp"
    break;

  case 227:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6052 "ds_parser.cpp"
    break;

  case 228:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6058 "ds_parser.cpp"
    break;

  case 229:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6064 "ds_parser.cpp"
    break;

  case 230:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6070 "ds_parser.cpp"
    break;

  case 231:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6076 "ds_parser.cpp"
    break;

  case 232:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6082 "ds_parser.cpp"
    break;

  case 233:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6088 "ds_parser.cpp"
    break;

  case 234:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6094 "ds_parser.cpp"
    break;

  case 235:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6100 "ds_parser.cpp"
    break;

  case 236:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6106 "ds_parser.cpp"
    break;

  case 237:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6112 "ds_parser.cpp"
    break;

  case 238:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6118 "ds_parser.cpp"
    break;

  case 239:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6124 "ds_parser.cpp"
    break;

  case 240:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6130 "ds_parser.cpp"
    break;

  case 241:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6136 "ds_parser.cpp"
    break;

  case 242:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6142 "ds_parser.cpp"
    break;

  case 243:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6148 "ds_parser.cpp"
    break;

  case 244:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6154 "ds_parser.cpp"
    break;

  case 245:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6160 "ds_parser.cpp"
    break;

  case 246:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6166 "ds_parser.cpp"
    break;

  case 247:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6172 "ds_parser.cpp"
    break;

  case 248:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6178 "ds_parser.cpp"
    break;

  case 249:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6184 "ds_parser.cpp"
    break;

  case 250:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6190 "ds_parser.cpp"
    break;

  case 251:
#line 1475 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6202 "ds_parser.cpp"
    break;

  case 252:
#line 1486 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6212 "ds_parser.cpp"
    break;

  case 253:
#line 1491 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6224 "ds_parser.cpp"
    break;

  case 254:
#line 1501 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6233 "ds_parser.cpp"
    break;

  case 255:
#line 1505 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6239 "ds_parser.cpp"
    break;

  case 256:
#line 1505 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6245 "ds_parser.cpp"
    break;

  case 257:
#line 1505 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6257 "ds_parser.cpp"
    break;

  case 258:
#line 1512 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6263 "ds_parser.cpp"
    break;

  case 259:
#line 1512 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6269 "ds_parser.cpp"
    break;

  case 260:
#line 1512 "ds_parser.ypp"
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
#line 6285 "ds_parser.cpp"
    break;

  case 261:
#line 1526 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6294 "ds_parser.cpp"
    break;

  case 262:
#line 1530 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6300 "ds_parser.cpp"
    break;

  case 263:
#line 1530 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6306 "ds_parser.cpp"
    break;

  case 264:
#line 1530 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6315 "ds_parser.cpp"
    break;

  case 265:
#line 1537 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6321 "ds_parser.cpp"
    break;

  case 266:
#line 1538 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6327 "ds_parser.cpp"
    break;

  case 267:
#line 1539 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6333 "ds_parser.cpp"
    break;

  case 268:
#line 1540 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6339 "ds_parser.cpp"
    break;

  case 269:
#line 1541 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6345 "ds_parser.cpp"
    break;

  case 270:
#line 1542 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6351 "ds_parser.cpp"
    break;

  case 271:
#line 1543 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6357 "ds_parser.cpp"
    break;

  case 272:
#line 1544 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6363 "ds_parser.cpp"
    break;

  case 273:
#line 1545 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6369 "ds_parser.cpp"
    break;

  case 274:
#line 1546 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6375 "ds_parser.cpp"
    break;

  case 275:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6381 "ds_parser.cpp"
    break;

  case 276:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6387 "ds_parser.cpp"
    break;

  case 277:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6393 "ds_parser.cpp"
    break;

  case 278:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6399 "ds_parser.cpp"
    break;

  case 279:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6405 "ds_parser.cpp"
    break;

  case 280:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6411 "ds_parser.cpp"
    break;

  case 281:
#line 1553 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6417 "ds_parser.cpp"
    break;

  case 282:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6423 "ds_parser.cpp"
    break;

  case 283:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6429 "ds_parser.cpp"
    break;

  case 284:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6435 "ds_parser.cpp"
    break;

  case 285:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6441 "ds_parser.cpp"
    break;

  case 286:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6447 "ds_parser.cpp"
    break;

  case 287:
#line 1559 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 288:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 289:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 290:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 291:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 292:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 293:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 294:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 295:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 296:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 297:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 298:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 299:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 300:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 301:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 302:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 303:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6549 "ds_parser.cpp"
    break;

  case 304:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 305:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 306:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6567 "ds_parser.cpp"
    break;

  case 307:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6573 "ds_parser.cpp"
    break;

  case 308:
#line 1580 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6582 "ds_parser.cpp"
    break;

  case 309:
#line 1584 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6591 "ds_parser.cpp"
    break;

  case 310:
#line 1588 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6599 "ds_parser.cpp"
    break;

  case 311:
#line 1591 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6607 "ds_parser.cpp"
    break;

  case 312:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6613 "ds_parser.cpp"
    break;

  case 313:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6619 "ds_parser.cpp"
    break;

  case 314:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6625 "ds_parser.cpp"
    break;

  case 315:
#line 1597 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6639 "ds_parser.cpp"
    break;

  case 316:
#line 1606 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6653 "ds_parser.cpp"
    break;

  case 317:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6659 "ds_parser.cpp"
    break;

  case 318:
#line 1616 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6667 "ds_parser.cpp"
    break;

  case 319:
#line 1619 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6673 "ds_parser.cpp"
    break;

  case 320:
#line 1619 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6679 "ds_parser.cpp"
    break;

  case 321:
#line 1619 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6687 "ds_parser.cpp"
    break;

  case 322:
#line 1622 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6696 "ds_parser.cpp"
    break;

  case 323:
#line 1626 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6705 "ds_parser.cpp"
    break;

  case 324:
#line 1630 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6714 "ds_parser.cpp"
    break;

  case 325:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6720 "ds_parser.cpp"
    break;

  case 326:
#line 1635 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6726 "ds_parser.cpp"
    break;

  case 327:
#line 1636 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6732 "ds_parser.cpp"
    break;

  case 328:
#line 1637 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6738 "ds_parser.cpp"
    break;

  case 329:
#line 1638 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6744 "ds_parser.cpp"
    break;

  case 330:
#line 1639 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6750 "ds_parser.cpp"
    break;

  case 331:
#line 1640 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6756 "ds_parser.cpp"
    break;

  case 332:
#line 1641 "ds_parser.ypp"
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
#line 6777 "ds_parser.cpp"
    break;

  case 333:
#line 1657 "ds_parser.ypp"
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
#line 6798 "ds_parser.cpp"
    break;

  case 334:
#line 1673 "ds_parser.ypp"
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
            } else {
                das_yyerror("expecting enumeration or bitfield " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                    CompilationError::syntax_error);
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
#line 6858 "ds_parser.cpp"
    break;

  case 335:
#line 1731 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6864 "ds_parser.cpp"
    break;

  case 336:
#line 1732 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6870 "ds_parser.cpp"
    break;

  case 337:
#line 1736 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6876 "ds_parser.cpp"
    break;

  case 338:
#line 1737 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6882 "ds_parser.cpp"
    break;

  case 339:
#line 1741 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6892 "ds_parser.cpp"
    break;

  case 340:
#line 1749 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6900 "ds_parser.cpp"
    break;

  case 341:
#line 1752 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6909 "ds_parser.cpp"
    break;

  case 342:
#line 1757 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6915 "ds_parser.cpp"
    break;

  case 343:
#line 1757 "ds_parser.ypp"
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
#line 6964 "ds_parser.cpp"
    break;

  case 344:
#line 1801 "ds_parser.ypp"
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
#line 7025 "ds_parser.cpp"
    break;

  case 345:
#line 1857 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7036 "ds_parser.cpp"
    break;

  case 346:
#line 1866 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7046 "ds_parser.cpp"
    break;

  case 347:
#line 1871 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7056 "ds_parser.cpp"
    break;

  case 348:
#line 1879 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7062 "ds_parser.cpp"
    break;

  case 349:
#line 1880 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7068 "ds_parser.cpp"
    break;

  case 350:
#line 1884 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7076 "ds_parser.cpp"
    break;

  case 351:
#line 1887 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7087 "ds_parser.cpp"
    break;

  case 352:
#line 1896 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7093 "ds_parser.cpp"
    break;

  case 353:
#line 1897 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7099 "ds_parser.cpp"
    break;

  case 354:
#line 1901 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7110 "ds_parser.cpp"
    break;

  case 355:
#line 1910 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7116 "ds_parser.cpp"
    break;

  case 356:
#line 1911 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7122 "ds_parser.cpp"
    break;

  case 357:
#line 1916 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7128 "ds_parser.cpp"
    break;

  case 358:
#line 1917 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7134 "ds_parser.cpp"
    break;

  case 359:
#line 1921 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7145 "ds_parser.cpp"
    break;

  case 360:
#line 1927 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7156 "ds_parser.cpp"
    break;

  case 361:
#line 1933 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7164 "ds_parser.cpp"
    break;

  case 362:
#line 1936 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7173 "ds_parser.cpp"
    break;

  case 363:
#line 1940 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7184 "ds_parser.cpp"
    break;

  case 364:
#line 1946 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7195 "ds_parser.cpp"
    break;

  case 365:
#line 1955 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7201 "ds_parser.cpp"
    break;

  case 366:
#line 1956 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7207 "ds_parser.cpp"
    break;

  case 367:
#line 1957 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7213 "ds_parser.cpp"
    break;

  case 368:
#line 1961 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7219 "ds_parser.cpp"
    break;

  case 369:
#line 1962 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7225 "ds_parser.cpp"
    break;

  case 370:
#line 1966 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7233 "ds_parser.cpp"
    break;

  case 371:
#line 1969 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7243 "ds_parser.cpp"
    break;

  case 372:
#line 1974 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7253 "ds_parser.cpp"
    break;

  case 373:
#line 1979 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7266 "ds_parser.cpp"
    break;

  case 374:
#line 1987 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7279 "ds_parser.cpp"
    break;

  case 375:
#line 1998 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7287 "ds_parser.cpp"
    break;

  case 376:
#line 2001 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7296 "ds_parser.cpp"
    break;

  case 377:
#line 2008 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7302 "ds_parser.cpp"
    break;

  case 378:
#line 2009 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7308 "ds_parser.cpp"
    break;

  case 379:
#line 2013 "ds_parser.ypp"
    { (yyval.i) = 0; }
#line 7314 "ds_parser.cpp"
    break;

  case 380:
#line 2014 "ds_parser.ypp"
    { (yyval.i) = 1; }
#line 7320 "ds_parser.cpp"
    break;

  case 381:
#line 2015 "ds_parser.ypp"
    { (yyval.i) = 2; }
#line 7326 "ds_parser.cpp"
    break;

  case 382:
#line 2019 "ds_parser.ypp"
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
                    pVar->global_shared = ((yyvsp[-3].i)==1);
                    pVar->private_variable = ((yyvsp[-3].i)==2);
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7359 "ds_parser.cpp"
    break;

  case 383:
#line 2047 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7365 "ds_parser.cpp"
    break;

  case 384:
#line 2047 "ds_parser.ypp"
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
                pVar->global_shared = ((yyvsp[-2].i)==1);
                pVar->private_variable = ((yyvsp[-2].i)==2);
                if ( !g_Program->addVariable(pVar) )
                    das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }
#line 7397 "ds_parser.cpp"
    break;

  case 385:
#line 2077 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7405 "ds_parser.cpp"
    break;

  case 386:
#line 2080 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7419 "ds_parser.cpp"
    break;

  case 387:
#line 2089 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7433 "ds_parser.cpp"
    break;

  case 388:
#line 2102 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7447 "ds_parser.cpp"
    break;

  case 392:
#line 2123 "ds_parser.ypp"
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
#line 7463 "ds_parser.cpp"
    break;

  case 393:
#line 2134 "ds_parser.ypp"
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
#line 7480 "ds_parser.cpp"
    break;

  case 394:
#line 2149 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7486 "ds_parser.cpp"
    break;

  case 395:
#line 2150 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7492 "ds_parser.cpp"
    break;

  case 396:
#line 2154 "ds_parser.ypp"
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
#line 7531 "ds_parser.cpp"
    break;

  case 397:
#line 2191 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7537 "ds_parser.cpp"
    break;

  case 398:
#line 2192 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7543 "ds_parser.cpp"
    break;

  case 399:
#line 2196 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7549 "ds_parser.cpp"
    break;

  case 400:
#line 2196 "ds_parser.ypp"
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
#line 7635 "ds_parser.cpp"
    break;

  case 401:
#line 2280 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7647 "ds_parser.cpp"
    break;

  case 402:
#line 2287 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7658 "ds_parser.cpp"
    break;

  case 403:
#line 2296 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7664 "ds_parser.cpp"
    break;

  case 404:
#line 2297 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7670 "ds_parser.cpp"
    break;

  case 405:
#line 2298 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7676 "ds_parser.cpp"
    break;

  case 406:
#line 2299 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7682 "ds_parser.cpp"
    break;

  case 407:
#line 2300 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7688 "ds_parser.cpp"
    break;

  case 408:
#line 2301 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7694 "ds_parser.cpp"
    break;

  case 409:
#line 2302 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7700 "ds_parser.cpp"
    break;

  case 410:
#line 2303 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7706 "ds_parser.cpp"
    break;

  case 411:
#line 2304 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7712 "ds_parser.cpp"
    break;

  case 412:
#line 2305 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7718 "ds_parser.cpp"
    break;

  case 413:
#line 2306 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7724 "ds_parser.cpp"
    break;

  case 414:
#line 2307 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7730 "ds_parser.cpp"
    break;

  case 415:
#line 2308 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7736 "ds_parser.cpp"
    break;

  case 416:
#line 2309 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7742 "ds_parser.cpp"
    break;

  case 417:
#line 2310 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7748 "ds_parser.cpp"
    break;

  case 418:
#line 2311 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7754 "ds_parser.cpp"
    break;

  case 419:
#line 2312 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7760 "ds_parser.cpp"
    break;

  case 420:
#line 2313 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7766 "ds_parser.cpp"
    break;

  case 421:
#line 2314 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7772 "ds_parser.cpp"
    break;

  case 422:
#line 2315 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7778 "ds_parser.cpp"
    break;

  case 423:
#line 2316 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7784 "ds_parser.cpp"
    break;

  case 424:
#line 2317 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7790 "ds_parser.cpp"
    break;

  case 425:
#line 2318 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7796 "ds_parser.cpp"
    break;

  case 426:
#line 2319 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7802 "ds_parser.cpp"
    break;

  case 427:
#line 2320 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7808 "ds_parser.cpp"
    break;

  case 428:
#line 2324 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7814 "ds_parser.cpp"
    break;

  case 429:
#line 2325 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7820 "ds_parser.cpp"
    break;

  case 430:
#line 2326 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7826 "ds_parser.cpp"
    break;

  case 431:
#line 2327 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7832 "ds_parser.cpp"
    break;

  case 432:
#line 2328 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7838 "ds_parser.cpp"
    break;

  case 433:
#line 2329 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7844 "ds_parser.cpp"
    break;

  case 434:
#line 2333 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7857 "ds_parser.cpp"
    break;

  case 435:
#line 2344 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7866 "ds_parser.cpp"
    break;

  case 436:
#line 2348 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7878 "ds_parser.cpp"
    break;

  case 437:
#line 2358 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7890 "ds_parser.cpp"
    break;

  case 438:
#line 2365 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7901 "ds_parser.cpp"
    break;

  case 439:
#line 2374 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7907 "ds_parser.cpp"
    break;

  case 440:
#line 2374 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7913 "ds_parser.cpp"
    break;

  case 441:
#line 2374 "ds_parser.ypp"
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
#line 7928 "ds_parser.cpp"
    break;

  case 442:
#line 2387 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7934 "ds_parser.cpp"
    break;

  case 443:
#line 2388 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7940 "ds_parser.cpp"
    break;

  case 444:
#line 2389 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7946 "ds_parser.cpp"
    break;

  case 445:
#line 2390 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7952 "ds_parser.cpp"
    break;

  case 446:
#line 2391 "ds_parser.ypp"
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
#line 7971 "ds_parser.cpp"
    break;

  case 447:
#line 2405 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7982 "ds_parser.cpp"
    break;

  case 448:
#line 2411 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7991 "ds_parser.cpp"
    break;

  case 449:
#line 2415 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8000 "ds_parser.cpp"
    break;

  case 450:
#line 2419 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8010 "ds_parser.cpp"
    break;

  case 451:
#line 2424 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8020 "ds_parser.cpp"
    break;

  case 452:
#line 2429 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8030 "ds_parser.cpp"
    break;

  case 453:
#line 2434 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8040 "ds_parser.cpp"
    break;

  case 454:
#line 2439 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8049 "ds_parser.cpp"
    break;

  case 455:
#line 2443 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8058 "ds_parser.cpp"
    break;

  case 456:
#line 2447 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8068 "ds_parser.cpp"
    break;

  case 457:
#line 2452 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8077 "ds_parser.cpp"
    break;

  case 458:
#line 2456 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8087 "ds_parser.cpp"
    break;

  case 459:
#line 2461 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8093 "ds_parser.cpp"
    break;

  case 460:
#line 2461 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8099 "ds_parser.cpp"
    break;

  case 461:
#line 2461 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8110 "ds_parser.cpp"
    break;

  case 462:
#line 2467 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8122 "ds_parser.cpp"
    break;

  case 463:
#line 2474 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8128 "ds_parser.cpp"
    break;

  case 464:
#line 2474 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8134 "ds_parser.cpp"
    break;

  case 465:
#line 2474 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8144 "ds_parser.cpp"
    break;

  case 466:
#line 2479 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8150 "ds_parser.cpp"
    break;

  case 467:
#line 2479 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8156 "ds_parser.cpp"
    break;

  case 468:
#line 2479 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8167 "ds_parser.cpp"
    break;

  case 469:
#line 2485 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8173 "ds_parser.cpp"
    break;

  case 470:
#line 2485 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8179 "ds_parser.cpp"
    break;

  case 471:
#line 2485 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8189 "ds_parser.cpp"
    break;

  case 472:
#line 2490 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8198 "ds_parser.cpp"
    break;

  case 473:
#line 2494 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8204 "ds_parser.cpp"
    break;

  case 474:
#line 2494 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8210 "ds_parser.cpp"
    break;

  case 475:
#line 2494 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8220 "ds_parser.cpp"
    break;

  case 476:
#line 2499 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8226 "ds_parser.cpp"
    break;

  case 477:
#line 2499 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8232 "ds_parser.cpp"
    break;

  case 478:
#line 2499 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8246 "ds_parser.cpp"
    break;

  case 479:
#line 2508 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8255 "ds_parser.cpp"
    break;

  case 480:
#line 2512 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8261 "ds_parser.cpp"
    break;

  case 481:
#line 2512 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8267 "ds_parser.cpp"
    break;

  case 482:
#line 2512 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8277 "ds_parser.cpp"
    break;

  case 483:
#line 2517 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8283 "ds_parser.cpp"
    break;

  case 484:
#line 2517 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8289 "ds_parser.cpp"
    break;

  case 485:
#line 2517 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8303 "ds_parser.cpp"
    break;

  case 486:
#line 2526 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8312 "ds_parser.cpp"
    break;

  case 487:
#line 2530 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8318 "ds_parser.cpp"
    break;

  case 488:
#line 2530 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8324 "ds_parser.cpp"
    break;

  case 489:
#line 2530 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8334 "ds_parser.cpp"
    break;

  case 490:
#line 2535 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8340 "ds_parser.cpp"
    break;

  case 491:
#line 2535 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8346 "ds_parser.cpp"
    break;

  case 492:
#line 2535 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8360 "ds_parser.cpp"
    break;

  case 493:
#line 2544 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8366 "ds_parser.cpp"
    break;

  case 494:
#line 2544 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8372 "ds_parser.cpp"
    break;

  case 495:
#line 2544 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8383 "ds_parser.cpp"
    break;

  case 496:
#line 2550 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8389 "ds_parser.cpp"
    break;

  case 497:
#line 2550 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8395 "ds_parser.cpp"
    break;

  case 498:
#line 2550 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8406 "ds_parser.cpp"
    break;

  case 499:
#line 2559 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8412 "ds_parser.cpp"
    break;

  case 500:
#line 2559 "ds_parser.ypp"
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
#line 8429 "ds_parser.cpp"
    break;

  case 501:
#line 2574 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8435 "ds_parser.cpp"
    break;

  case 502:
#line 2574 "ds_parser.ypp"
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
#line 8456 "ds_parser.cpp"
    break;

  case 503:
#line 2594 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8462 "ds_parser.cpp"
    break;

  case 504:
#line 2595 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8468 "ds_parser.cpp"
    break;

  case 505:
#line 2596 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8474 "ds_parser.cpp"
    break;

  case 506:
#line 2597 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8480 "ds_parser.cpp"
    break;

  case 507:
#line 2601 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8492 "ds_parser.cpp"
    break;

  case 508:
#line 2608 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8504 "ds_parser.cpp"
    break;

  case 509:
#line 2615 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8515 "ds_parser.cpp"
    break;

  case 510:
#line 2621 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8526 "ds_parser.cpp"
    break;

  case 511:
#line 2630 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8536 "ds_parser.cpp"
    break;

  case 512:
#line 2635 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8545 "ds_parser.cpp"
    break;

  case 513:
#line 2642 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8551 "ds_parser.cpp"
    break;

  case 514:
#line 2643 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8557 "ds_parser.cpp"
    break;

  case 515:
#line 2647 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8568 "ds_parser.cpp"
    break;

  case 516:
#line 2653 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8580 "ds_parser.cpp"
    break;

  case 517:
#line 2660 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8593 "ds_parser.cpp"
    break;

  case 518:
#line 2668 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8605 "ds_parser.cpp"
    break;

  case 519:
#line 2675 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8618 "ds_parser.cpp"
    break;

  case 520:
#line 2683 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8632 "ds_parser.cpp"
    break;

  case 521:
#line 2695 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8640 "ds_parser.cpp"
    break;

  case 522:
#line 2698 "ds_parser.ypp"
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
#line 8656 "ds_parser.cpp"
    break;

  case 523:
#line 2712 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8667 "ds_parser.cpp"
    break;

  case 524:
#line 2721 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8673 "ds_parser.cpp"
    break;

  case 525:
#line 2722 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8679 "ds_parser.cpp"
    break;

  case 526:
#line 2726 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8689 "ds_parser.cpp"
    break;

  case 527:
#line 2731 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8698 "ds_parser.cpp"
    break;

  case 528:
#line 2738 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8708 "ds_parser.cpp"
    break;

  case 529:
#line 2743 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8720 "ds_parser.cpp"
    break;

  case 530:
#line 2753 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8730 "ds_parser.cpp"
    break;

  case 531:
#line 2758 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8739 "ds_parser.cpp"
    break;

  case 532:
#line 2765 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8753 "ds_parser.cpp"
    break;

  case 533:
#line 2777 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8759 "ds_parser.cpp"
    break;

  case 534:
#line 2778 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8765 "ds_parser.cpp"
    break;

  case 535:
#line 2782 "ds_parser.ypp"
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
#line 8788 "ds_parser.cpp"
    break;

  case 536:
#line 2800 "ds_parser.ypp"
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
#line 8811 "ds_parser.cpp"
    break;


#line 8815 "ds_parser.cpp"

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
#line 2820 "ds_parser.ypp"


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
        for ( const auto & pA : *annL ) {
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
        for ( const auto & pA : *annL ) {
            func->annotations.push_back(pA);
        }
        delete annL;
    }
}
