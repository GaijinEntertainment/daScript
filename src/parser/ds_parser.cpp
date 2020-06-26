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
    DAS_SCOPE = 294,
    DAS_TYPEDEF = 295,
    DAS_WITH = 296,
    DAS_CAST = 297,
    DAS_OVERRIDE = 298,
    DAS_ABSTRACT = 299,
    DAS_UPCAST = 300,
    DAS_ITERATOR = 301,
    DAS_VAR = 302,
    DAS_ADDR = 303,
    DAS_CONTINUE = 304,
    DAS_WHERE = 305,
    DAS_PASS = 306,
    DAS_REINTERPRET = 307,
    DAS_MODULE = 308,
    DAS_PUBLIC = 309,
    DAS_LABEL = 310,
    DAS_GOTO = 311,
    DAS_IMPLICIT = 312,
    DAS_EXPLICIT = 313,
    DAS_SHARED = 314,
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
    RPIPE = 376,
    CLONEEQU = 377,
    ROTL = 378,
    ROTR = 379,
    ROTLEQU = 380,
    ROTREQU = 381,
    MAPTO = 382,
    COLCOL = 383,
    BRABRAB = 384,
    BRACBRB = 385,
    CBRCBRB = 386,
    INTEGER = 387,
    LONG_INTEGER = 388,
    UNSIGNED_INTEGER = 389,
    UNSIGNED_LONG_INTEGER = 390,
    FLOAT = 391,
    DOUBLE = 392,
    NAME = 393,
    BEGIN_STRING = 394,
    STRING_CHARACTER = 395,
    END_STRING = 396,
    BEGIN_STRING_EXPR = 397,
    END_STRING_EXPR = 398,
    END_OF_READ = 399,
    UNARY_MINUS = 400,
    UNARY_PLUS = 401,
    PRE_INC = 402,
    PRE_DEC = 403,
    POST_INC = 404,
    POST_DEC = 405,
    DEREF = 406
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

#line 401 "ds_parser.cpp"

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
#define YYLAST   7215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  176
/* YYNRULES -- Number of rules.  */
#define YYNRULES  518
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  925

#define YYUNDEFTOK  2
#define YYMAXUTOK   406

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
       2,     2,     2,   162,     2,   178,   173,   158,   151,     2,
     171,   172,   156,   155,   145,   154,   167,   157,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   148,   175,
     152,   146,   153,   147,   174,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   169,     2,   170,   150,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   176,   149,   177,   161,     2,     2,     2,
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
     159,   160,   163,   164,   165,   166,   168
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   433,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,   445,   449,   461,   462,   466,   470,
     475,   484,   492,   508,   513,   521,   521,   551,   573,   577,
     580,   586,   595,   616,   639,   640,   644,   648,   649,   653,
     656,   662,   668,   671,   677,   678,   682,   683,   684,   693,
     694,   698,   707,   723,   731,   741,   750,   751,   752,   753,
     754,   755,   759,   764,   772,   773,   774,   775,   776,   777,
     778,   779,   785,   798,   814,   815,   819,   826,   838,   842,
     849,   850,   854,   855,   856,   860,   865,   872,   876,   877,
     878,   879,   880,   881,   882,   883,   884,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   915,   936,   974,   981,   985,   996,
     997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,
    1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1018,  1023,
    1029,  1035,  1056,  1062,  1063,  1074,  1078,  1084,  1087,  1090,
    1094,  1100,  1104,  1108,  1111,  1114,  1119,  1122,  1130,  1133,
    1138,  1141,  1149,  1155,  1156,  1160,  1161,  1165,  1200,  1200,
    1200,  1203,  1203,  1203,  1208,  1208,  1208,  1216,  1220,  1225,
    1231,  1231,  1231,  1235,  1235,  1235,  1240,  1240,  1240,  1249,
    1252,  1258,  1259,  1266,  1277,  1278,  1279,  1283,  1284,  1285,
    1286,  1290,  1295,  1303,  1304,  1308,  1313,  1377,  1378,  1379,
    1380,  1381,  1382,  1386,  1387,  1388,  1389,  1390,  1391,  1392,
    1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1401,  1405,
    1406,  1410,  1421,  1426,  1436,  1440,  1440,  1440,  1447,  1447,
    1447,  1461,  1465,  1465,  1465,  1473,  1474,  1475,  1476,  1477,
    1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1517,  1521,  1524,  1527,
    1528,  1529,  1530,  1539,  1548,  1549,  1552,  1552,  1552,  1555,
    1559,  1563,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1583,
    1593,  1648,  1649,  1653,  1654,  1658,  1666,  1669,  1674,  1673,
    1718,  1776,  1781,  1789,  1790,  1794,  1797,  1806,  1807,  1811,
    1820,  1821,  1826,  1827,  1831,  1837,  1843,  1846,  1850,  1856,
    1865,  1866,  1867,  1871,  1872,  1876,  1879,  1884,  1889,  1897,
    1908,  1911,  1918,  1919,  1923,  1950,  1950,  1962,  1965,  1974,
    1987,  1999,  2000,  2004,  2008,  2019,  2034,  2035,  2039,  2076,
    2077,  2081,  2081,  2165,  2172,  2181,  2182,  2183,  2184,  2185,
    2186,  2187,  2188,  2189,  2190,  2191,  2192,  2193,  2194,  2195,
    2196,  2197,  2198,  2199,  2200,  2201,  2202,  2203,  2204,  2205,
    2209,  2210,  2211,  2212,  2213,  2214,  2218,  2229,  2233,  2243,
    2250,  2259,  2259,  2259,  2272,  2273,  2274,  2275,  2276,  2290,
    2296,  2300,  2304,  2309,  2314,  2319,  2324,  2328,  2332,  2337,
    2341,  2346,  2346,  2346,  2352,  2359,  2359,  2359,  2364,  2364,
    2364,  2370,  2370,  2370,  2375,  2379,  2379,  2379,  2384,  2384,
    2384,  2393,  2397,  2397,  2397,  2402,  2402,  2402,  2411,  2415,
    2415,  2415,  2420,  2420,  2420,  2429,  2429,  2429,  2435,  2435,
    2435,  2444,  2444,  2459,  2459,  2479,  2480,  2481,  2482,  2486,
    2493,  2500,  2506,  2515,  2520,  2527,  2528,  2532,  2538,  2545,
    2553,  2560,  2568,  2580,  2583,  2597,  2606,  2607,  2611,  2616,
    2623,  2628,  2639,  2644,  2651,  2663,  2664,  2668,  2686
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
  "\"with\"", "\"cast\"", "\"override\"", "\"abstract\"", "\"upcast\"",
  "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"", "\"where\"",
  "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"",
  "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
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
  "expression_try_catch", "let_scope", "kwd_let", "expression_let",
  "expr_cast", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "expr_type_info",
  "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@14", "$@15",
  "$@16", "$@17", "expr_field", "$@18", "$@19", "expr", "$@20", "$@21",
  "optional_field_annotation", "optional_override",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@22", "function_argument_declaration", "function_argument_list",
  "tuple_type", "tuple_type_list", "variant_type", "variant_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared", "global_let",
  "$@23", "enum_list", "single_alias", "alias_list", "alias_declaration",
  "enum_declaration", "optional_structure_parent", "structure_name",
  "class_or_struct", "structure_declaration", "$@24",
  "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_type_declaration",
  "$@25", "$@26", "type_declaration", "$@27", "$@28", "$@29", "$@30",
  "$@31", "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39",
  "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48",
  "$@49", "$@50", "variant_alias_declaration", "$@51",
  "bitfield_alias_declaration", "$@52", "make_decl", "make_struct_fields",
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
     395,   396,   397,   398,   399,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   400,
     401,   126,    33,   402,   403,   404,   405,    46,   406,    91,
      93,    40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -573

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-573)))

#define YYTABLE_NINF -473

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -573,    23,  -573,  -573,   -59,   -30,    -6,     0,  -104,  -573,
      57,  -573,  -573,    -7,  -573,  -573,  -573,  -573,  -573,   337,
    -573,   195,  -573,  -573,  -573,  -573,  -573,  -573,    72,  -573,
     111,   110,   131,  -573,  -573,    15,  -573,   -91,   139,  -573,
     148,   150,  -573,   172,   189,   147,  -573,   -42,  -573,  -573,
    -573,     1,   185,  -573,   151,    16,   -59,   194,   -30,   190,
    -573,   193,   205,  -573,   250,  -573,   187,  -573,   -73,   169,
     170,  -573,   209,   -59,    -7,  -573,  7053,   318,   320,  -573,
     186,   180,  -573,   210,  -573,  -573,   359,  -573,  -573,  -573,
    -573,  -573,   221,    52,  -573,  -573,  -573,  -573,   308,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,   188,   -67,  6978,
    -573,  -573,   225,   231,  -573,   -81,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,   -80,   222,  -573,  -573,    58,  -573,   196,
     -64,   199,  -573,    30,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,     2,  -573,  -573,   -95,  -573,   224,   227,   228,   229,
    -573,  -573,  -573,   207,  -573,  -573,  -573,  -573,  -573,   230,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,   232,
    -573,  -573,  -573,   233,   241,  -573,  -573,  -573,  -573,   244,
     246,  -573,  -573,  -573,  -573,  -573,   713,  -573,   208,   258,
    -573,   234,  -573,   -59,  -573,    -9,  -573,   -52,  6978,  -573,
    1294,  -573,  -573,  -573,  -573,  -573,   174,  -573,  -573,  -573,
      52,  -573,   -63,  4661,  -573,  -573,  -573,  -573,  -573,   272,
    -573,   -29,   112,   159,  -573,  -573,  -573,  -573,  -573,  -573,
     381,  -573,  -573,    -5,  2624,  -573,  -573,   -61,  6978,   -60,
     -37,   276,  -573,   102,  -573,   286,  4856,  -573,  4661,  -573,
    -573,   276,  -573,  -573,  6901,   245,  2070,  -573,  -573,   180,
    4661,   247,  4661,   265,   269,   252,  -573,   249,   273,   296,
    2751,   180,  -573,   281,  2221,  4661,  4661,   177,  6743,  6824,
    4661,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  4661,  4661,
    4661,    58,  4661,  4661,  4661,  -573,   260,  -573,   407,  -573,
    -573,   271,   274,  4661,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,   -83,   275,  -573,   277,   279,  -573,  -573,  -573,
     425,  -573,  -573,  -573,   278,  -573,  -573,   -78,  -573,  -573,
    -573,  -573,  -573,  5927,   285,  -573,  -573,  -573,  -573,  -573,
       6,   310,  6978,  -573,   -21,  -573,  -573,   498,  6978,  6978,
    6978,  6978,   283,   225,  6978,   186,  6978,   186,  6978,   186,
    7055,   231,  -573,  -573,  -573,   287,  -573,  -573,  5600,  -573,
    -573,  4856,  -573,  -573,   288,  -573,  -573,  -573,  6978,  -573,
    2878,  -573,   391,    13,   614,  -573,    58,  2878,  -573,  -573,
     340,   938,   449,  6205,  4661,   391,  -573,  -573,  4661,  -573,
    -573,   316,   333,  6205,  -573,  6978,  2878,  -573,  1097,   469,
     469,   292,  -573,   276,  1623,   276,  1772,  5994,  -573,    37,
     103,   469,   469,   -74,  -573,   469,   469,   683,    81,   291,
    -573,  -573,   391,  -573,  2348,  -573,  -573,  -573,   435,   276,
     344,   177,  -573,    22,   338,  4661,  4661,  4661,  4661,  4661,
    4661,  4661,  4661,  4661,  4661,  -573,  -573,  4661,  4661,  4661,
    4661,  4661,  4661,   339,  3005,  4661,   346,  4661,  4661,  4661,
    4661,  4661,  4661,  4661,  4661,  3132,  3259,  4661,  4661,  4661,
    4661,  4661,  4661,  4661,  4661,  4661,  4661,   347,  4661,  3386,
    -573,   468,   443,   303,  -573,    54,  -573,  -573,  -573,  2878,
    -573,  1116,   965,  1920,  2442,  -573,   -35,  3480,   222,  3629,
     222,  3778,   222,   -66,  -573,    67,  4856,    82,  -573,  -573,
    -573,  3927,  -573,  5820,  -573,  4661,  3535,  2497,  -573,  4743,
    4661,  4661,  -573,   180,  5180,  -573,  6978,  6978,  5250,  6978,
    -573,  -573,  4076,  -573,  4842,  -573,  -573,    14,   177,   -41,
    1921,  3684,  5994,   343,   -25,   321,   348,  -573,  -573,   136,
      18,  3833,   -25,    49,  4661,  4661,   315,  -573,  4661,   360,
     370,  -573,   161,  -573,  -573,   216,   361,  -573,    34,  6205,
    -573,  -573,    43,   186,  -573,   364,  -573,  -573,  6205,  6205,
    6205,  6205,  6205,  6205,  6205,  6205,   314,   314,   851,  6205,
    6205,   851,   284,   284,   342,  4661,  6205,   146,  -573,  5669,
     -69,   -69,  6205,   314,   314,  6205,  6205,  4661,  6205,   373,
    6066,  6272,  6344,  6411,   851,   851,  4765,  4765,   146,   146,
     146,  -573,   516,  5739,  -573,    59,   226,  -573,   276,  -573,
     381,  -573,  2878,  -573,  4941,  -573,  6978,  -573,  -573,  -573,
     382,  -573,   368,  -573,   369,  -573,   372,  6978,  -573,  7055,
    -573,   231,   381,  4661,   -92,  -573,    62,   371,   388,  5320,
    -573,  -573,  -573,  4378,  6462,  -573,  6499,   344,  -573,  4661,
    -573,  4661,  4661,   -11,   389,   361,   358,   362,  4661,   363,
    4661,  4661,   361,   352,   365,  6205,  -573,  -573,  6133,  -573,
     390,  6978,   186,  1471,   180,  -573,  -573,  4661,  -573,    31,
     -36,  4661,  -573,   393,   396,   398,   399,  -573,   -19,   222,
    -573,  3982,  6472,  -573,  6472,  -573,  4661,  -573,  -573,  -573,
    -573,     1,  -573,  -573,  5040,  -573,  -573,  6513,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  4856,  -573,  -573,
    -573,  6205,   180,  -573,  -573,    93,  -573,  -573,  -573,  -573,
     379,   -86,  6205,  6205,   -25,   385,    71,   343,   387,  -573,
    6205,  -573,  -573,   -85,   -25,   392,  -573,  -573,  -573,  6541,
     222,  -573,  -573,   391,   367,  6205,  -573,  -573,  -573,  -573,
      43,   394,   -77,  6978,  -573,    65,  6205,  -573,     1,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  6978,  4131,   413,  4661,
    4661,  4661,  4258,  4661,   395,   401,  4661,  4661,  -573,  4661,
     375,  -573,  -573,   406,   216,  -573,  -573,  -573,  4407,  -573,
    -573,  6554,  -573,   386,  -573,  6575,   411,  5390,   415,   851,
     851,   851,  -573,  5460,  5107,   402,  -573,  6205,  6205,  5107,
     403,    58,  -573,  -573,  4661,  6205,  -573,  -573,  -573,  -573,
    -573,  4534,  -573,   519,   408,  -573,   400,  -573,  -573,    58,
    6205,  -573,   404,  6978,   414,  5530,  4661,   409,   410,  -573,
    -573,  6605,  -573,  -573,  6205,  -573,  -573,  -573,  6978,   412,
    6615,  -573,  -573,   416,  -573
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   175,     0,     0,     0,     0,     0,   176,
       0,   483,   481,     0,    14,     3,    10,     9,     8,     0,
       7,   362,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    75,     0,   153,    76,    78,     0,    74,   380,
     379,     0,     0,   363,   365,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   367,     0,   371,     0,     0,
       0,   155,     0,     0,     0,    81,     0,     0,     0,    87,
      82,     0,   124,   376,   381,   360,     0,    68,    69,    66,
      67,    65,     0,     0,    64,    73,    40,    38,    34,    31,
      30,   410,   413,   411,   414,   412,   415,     0,     0,     0,
     373,   372,     0,     0,   154,     0,    79,    90,    91,    93,
      92,    94,    95,    96,    97,   116,   117,   114,   115,   107,
     118,   119,   108,   105,   106,   120,   121,   122,   123,   110,
     111,   109,   103,   104,    99,    98,   100,   101,   102,    89,
      88,   112,   113,   321,    85,   148,   126,     0,   378,     0,
       0,     0,    16,     0,    60,    61,    58,    59,    57,    56,
      62,     0,    33,   367,     0,   374,     0,     0,     0,     0,
     385,   405,   386,   417,   387,   391,   392,   393,   394,   409,
     398,   399,   400,   401,   402,   403,   404,   406,   407,   454,
     390,   397,   408,   461,   468,   388,   395,   389,   396,     0,
       0,   416,   424,   427,   425,   426,     0,   419,     0,     0,
     340,     0,    77,     0,    83,     0,   333,     0,     0,   125,
       0,   377,   326,   383,   364,   361,   353,   366,    17,    18,
       0,    71,     0,     0,   368,   445,   448,   451,   441,     0,
     421,   455,   462,   469,   475,   478,   432,   437,   431,   444,
       0,   440,   434,     0,     0,   370,   436,     0,     0,     0,
       0,     0,   331,   344,    84,   321,    86,   150,     0,    49,
      50,     0,   261,   262,     0,     0,     0,   255,   161,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
       0,   217,   219,   218,   220,   221,   222,    19,     0,     0,
       0,     0,     0,     0,     0,   204,   205,   129,   127,   259,
     258,     0,     0,     0,   143,   138,   136,   135,   137,   215,
     149,   130,   256,     0,   314,     0,     0,   141,   142,   144,
     173,   134,   313,   312,    80,   317,   257,     0,   131,   316,
     315,   294,   263,   223,     0,   260,   485,   486,   487,   488,
     321,     0,     0,   354,     0,    63,   375,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,    82,     0,    82,
       0,     0,   439,   433,   435,     0,   438,   429,     0,   420,
     484,   339,   482,   341,     0,   332,   343,   342,     0,   345,
       0,   334,     0,     0,   157,   160,     0,     0,   163,   166,
       0,   223,     0,   156,     0,     0,   178,   181,     0,   147,
     184,     0,     0,    43,    53,     0,     0,   170,   223,   286,
     287,   205,   152,     0,   495,     0,     0,     0,   512,     0,
       0,   267,   266,   299,    25,   265,   264,     0,   206,     0,
     145,   146,     0,   320,     0,   133,   139,   140,     0,     0,
     213,     0,   132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   288,   289,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
     382,     0,   323,     0,   384,     0,   351,   352,   350,     0,
     369,     0,     0,     0,     0,   418,     0,     0,    85,     0,
      85,     0,    85,   153,   337,     0,   335,     0,   430,   428,
     322,   346,   349,   348,    54,     0,     0,     0,   167,   223,
       0,     0,   164,     0,     0,    55,     0,     0,     0,     0,
      41,    42,     0,   171,   223,   168,   206,     0,     0,   153,
       0,     0,   503,   493,   495,     0,   506,   507,   508,     0,
       0,     0,   495,     0,     0,     0,     0,    22,     0,    20,
       0,   290,   245,   244,   148,    46,     0,   295,     0,   199,
     174,   177,     0,    82,   151,     0,   309,   310,   230,   231,
     233,   232,   234,   227,   228,   229,   268,   269,   281,   235,
     236,   282,   279,   280,     0,     0,   225,   304,   293,     0,
     318,   319,   226,   270,   271,   237,   238,     0,   224,     0,
       0,   284,   285,   283,   277,   278,   273,   272,   274,   275,
     276,   251,     0,     0,   297,     0,   323,   324,     0,   327,
     350,   355,     0,   359,   223,   446,     0,   452,   442,   422,
       0,   456,     0,   463,     0,   470,     0,     0,   476,     0,
     479,     0,   342,     0,     0,   158,     0,     0,     0,     0,
     165,   172,   300,     0,     0,   301,     0,   213,   169,     0,
     496,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,   513,   514,     0,    23,
      26,     0,    82,     0,     0,    44,    45,     0,    51,     0,
       0,     0,   296,     0,     0,     0,     0,   211,     0,    85,
     306,     0,   240,   292,   239,   311,     0,   253,   291,   298,
     328,     0,   325,   357,   223,   358,   447,     0,   453,   443,
     423,   457,   459,   464,   466,   471,   473,   336,   477,   338,
     480,   347,     0,   159,   190,     0,   187,   179,   182,   185,
       0,     0,   490,   489,   495,     0,     0,   494,     0,   498,
     504,   510,   509,     0,   495,     0,   511,    21,    24,     0,
      85,   128,    47,     0,     0,   200,   209,   210,   208,   207,
       0,     0,     0,     0,   242,     0,   305,   254,     0,   330,
     356,   449,   460,   467,   474,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
       0,   501,   246,     0,    46,   241,   212,   214,     0,   201,
     216,     0,   243,     0,   450,     0,     0,     0,     0,   180,
     183,   186,   302,     0,   515,     0,   499,   492,   491,   515,
       0,     0,   249,    48,     0,   202,   307,   329,   191,   193,
     188,     0,   303,     0,     0,   500,     0,   502,   247,     0,
     203,   308,     0,     0,     0,     0,     0,     0,     0,   250,
     192,     0,   196,   189,   516,   517,   518,   194,     0,     0,
       0,   195,   197,     0,   198
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -573,  -573,  -573,   141,   537,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,   495,  -573,  -573,   536,  -573,  -573,
    -573,  -259,  -573,  -573,  -573,  -573,  -573,  -573,   356,  -573,
     541,   -62,  -573,   524,  -573,  -286,  -377,  -521,  -573,  -573,
    -229,  -155,   -76,  -573,     3,  -280,   -13,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,   611,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -510,  -573,  -573,  -203,  -573,   -89,  -291,  -573,  -228,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,   434,  -573,  -573,   253,   -46,  -573,  -573,  -573,   349,
    -573,   -58,  -573,  -265,   235,  -272,  -264,   104,  -573,   158,
    -573,  -573,  -573,  -573,   455,   562,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -145,  -106,  -573,  -573,  -573,   251,
    -573,  -573,  -573,   -87,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,   351,  -572,  -432,  -564,  -573,  -573,  -292,   -88,
     191,  -573,  -573,  -573,  -246,  -573
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   450,   329,   730,   330,   600,
      16,    17,    35,    36,    63,    18,    32,    33,   331,   332,
     737,   738,   333,   334,   335,   336,   337,   338,   170,   171,
      29,    30,    45,    46,    47,    19,   154,   229,    80,    20,
      81,    82,   339,   340,   230,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   469,   350,   351,   352,   566,   839,
     567,   840,   569,   841,   353,   836,   902,   903,   919,   918,
     923,   608,   860,   354,   747,   748,   613,   355,   356,   420,
     358,   359,   360,   361,   731,   881,   732,   899,   362,   662,
     827,   609,   823,   901,   225,   668,   523,   370,   828,   226,
     227,   544,   545,   220,   221,   712,   272,   529,   374,   235,
     160,    54,    22,    86,   108,    67,    68,    23,    24,   158,
      84,    52,    25,   159,   273,   364,   107,   213,   214,   218,
     215,   383,   770,   546,   381,   769,   378,   766,   379,   864,
     380,   768,   384,   771,   385,   832,   386,   773,   387,   833,
     388,   775,   389,   834,   390,   778,   391,   780,    26,    41,
      27,    40,   365,   583,   584,   585,   366,   586,   587,   588,
     589,   367,   449,   368,   894,   369
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   410,   357,   212,   403,   156,   419,   405,   538,   665,
     540,   115,   542,   -80,   592,   236,   442,   682,   448,   684,
     716,   686,   216,     2,   437,   578,    42,   393,   723,     3,
      87,    88,   555,   709,   740,    76,    59,   721,   271,   578,
     615,   471,   496,   497,   493,   694,   696,   496,   497,   223,
     858,   243,     4,   741,     5,   463,     6,    64,     7,   741,
     741,    48,    72,     8,    56,    66,   164,   165,   470,    60,
       9,   174,    38,   406,   233,   174,    10,   399,   219,    28,
     244,   711,   687,   782,   521,    65,   256,    72,   464,   843,
     849,   222,   224,   526,    11,   518,   211,   472,   517,   155,
     518,   527,    31,    74,   110,   407,    77,    78,    56,   714,
     175,   257,   258,   234,   376,    12,   400,   402,   679,  -458,
     274,    43,   212,   275,  -458,   528,   820,   739,    75,   233,
     552,    44,    34,   404,   814,   223,   413,   558,    37,    79,
     680,   276,  -458,   797,   231,   406,   394,   240,    89,   795,
     715,   821,    90,   711,    91,    92,   573,   743,   371,   371,
     616,   270,   212,   371,   395,   744,   473,   474,   526,   259,
     238,   239,    61,   396,   241,    13,   527,   407,   212,   741,
     614,   401,    62,   520,   166,   406,    43,    93,   167,   745,
     168,    92,    13,   846,   746,    39,    44,   414,    14,   791,
     670,   261,   212,   212,   741,   262,   742,   741,   263,    43,
     741,   803,   595,   422,   596,   211,   406,   407,    55,    44,
     688,   444,   446,   264,   720,   434,   724,   734,   822,   671,
     844,   759,   266,   602,   783,   690,   749,   862,   735,   736,
     850,   825,   689,   162,   597,   598,   837,   371,   407,   673,
     408,   485,   486,   409,    53,   211,    56,   691,    57,   493,
    -465,   495,   496,   497,   498,  -465,   212,   499,   838,   667,
     760,   211,   212,   212,   212,   212,    58,    66,   212,   693,
     212,   794,   212,  -465,   212,   525,    69,   710,    70,   853,
     804,   531,   532,   533,   534,   211,   211,   537,   577,   539,
     590,   541,   212,   726,   473,   474,   719,  -472,   454,  -248,
      71,   720,  -472,   517,  -248,   518,   101,    72,    73,   371,
     102,   551,   372,    83,   236,   373,    96,    85,    98,   212,
    -472,    99,  -248,   109,   473,   474,   554,   103,   104,   105,
     106,    49,    50,   100,    51,   112,   113,   114,   572,   565,
     325,   441,   151,   155,   152,   810,   155,   153,   157,   211,
     161,   162,    60,   217,   173,   211,   211,   211,   211,   219,
     228,   211,   232,   211,   237,   211,   245,   211,   249,   246,
     247,   248,   250,   267,   251,   252,   605,   483,   484,   485,
     486,   487,   763,   253,   490,   211,   254,   493,   255,   495,
     496,   497,   498,   557,   762,   499,   268,   501,   502,   269,
     382,   473,   474,   392,   233,   223,   416,   426,   424,   485,
     486,   427,   211,   428,   429,   430,   403,   493,   431,   495,
     496,   497,   498,   435,   458,   499,   510,   511,   512,   513,
     514,   515,   516,   459,   468,   603,   460,    13,   524,   461,
     465,   517,   466,   518,   467,   535,   519,   548,   550,   471,
     212,   212,   563,   212,   570,   571,   576,   604,   512,   513,
     514,   515,   516,   612,   610,   666,   617,   634,   669,   703,
     704,   517,   706,   518,   638,   661,   667,   701,   714,   473,
     474,   717,   727,   718,   483,   484,   485,   486,   487,   739,
     238,   490,   491,   492,   493,   357,   495,   496,   497,   498,
     729,   755,   499,   751,   501,   502,   750,   757,   473,   474,
     399,   772,   774,   784,   847,   776,   785,   796,   798,   805,
     808,   816,   799,   801,   817,   806,   818,   819,   506,   855,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     842,   868,   880,   211,   211,   845,   211,   848,   517,   882,
     518,   887,   851,   889,   857,   875,   912,   155,   891,   906,
     212,   876,   895,   897,   485,   486,   910,   908,   907,   915,
     916,   212,   493,   212,   921,   496,   497,   498,   924,   767,
     499,   599,    94,   172,    97,   883,   375,    95,   116,   863,
     777,   483,   484,   485,   486,   487,   829,   733,   490,   491,
     492,   493,    21,   495,   496,   497,   498,   856,   790,   499,
     761,   501,   502,   522,   411,   212,   547,   611,   242,   672,
     111,   779,   802,   896,   536,   415,   517,   593,   518,     0,
       0,     0,     0,     0,   809,   506,   256,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,     0,   812,     0,
       0,     0,     0,   211,   363,   517,     0,   518,     0,     0,
       0,   257,   258,   530,   211,     0,   211,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,   473,   474,     0,   835,     0,     0,     0,
       0,     0,   412,     0,     0,     0,     0,   212,   211,     0,
     421,     0,     0,     0,   423,     0,   425,     0,     0,   259,
     212,     0,     0,     0,   433,     0,   861,   854,   438,   439,
     440,     0,     0,     0,   447,   256,   859,     0,     0,   865,
       0,     0,   451,   452,   453,     0,   455,   456,   457,     0,
     260,   261,     0,     0,     0,   262,     0,   462,   263,     0,
     257,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,   556,   483,   484,   485,   486,
     487,     0,   266,   490,   491,   492,   493,   212,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,     0,     0,
     211,     0,   212,     0,     0,     0,   911,     0,     0,     0,
       0,     0,     0,   211,     0,     0,     0,     0,   259,     0,
     506,   920,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,     0,   553,     0,     0,     0,     0,     0,
     517,   559,   518,     0,     0,   601,     0,     0,   564,   260,
     261,     0,   568,     0,   262,     0,     0,   263,   898,     0,
     574,   473,   474,     0,     0,     0,     0,     0,   582,     0,
     582,     0,   264,     0,     0,     0,   909,     0,   265,     0,
     211,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   211,     0,     0,     0,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   627,     0,
       0,   628,   629,   630,   631,   632,   633,     0,   636,   637,
       0,   639,   640,   641,   642,   643,   644,   645,   646,   648,
     650,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,   663,     0,   483,   484,   485,   486,   473,   474,
       0,     0,     0,   674,   493,     0,   495,   496,   497,   498,
       0,     0,   499,     0,   501,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   699,     0,     0,   636,   648,     0,   256,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   516,
       0,     0,     0,     0,   451,   457,     0,     0,   517,     0,
     518,     0,   257,   258,     0,   457,     0,     0,   725,   447,
       0,     0,   728,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   560,   495,   496,   497,   498,     0,     0,   499,
     500,   501,   502,   503,   504,     0,     0,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   754,     0,     0,   561,   506,     0,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,     0,     0,     0,
       0,     0,     0,     0,     0,   517,   764,   518,     0,     0,
       0,   260,   261,   562,     0,     0,   262,   473,   474,   263,
       0,     0,     0,     0,     0,     0,     0,   781,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
     676,     0,     0,   266,     0,   792,   793,     0,   256,     0,
       0,     0,   800,     0,   582,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,   813,     0,   257,   258,   815,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     826,     0,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   560,   495,   496,   497,   498,     0,     0,   499,   500,
     501,   502,   503,   504,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   561,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,     0,     0,     0,     0,
       0,     0,   260,   261,   517,     0,   518,   262,     0,   675,
     263,   867,   575,   869,   870,   871,   873,   874,     0,     0,
     877,   878,     0,   879,     0,   264,     0,     0,     0,     0,
       0,     0,   885,     0,   266,   277,     0,     0,     0,     0,
       3,     0,   278,   279,   280,     0,   281,     0,   282,   283,
     284,   285,     0,     0,     0,     0,     0,     0,   900,   286,
     287,   288,   289,     0,     0,   905,     0,     0,     0,     0,
       0,   290,   291,     0,     0,   292,   293,     0,     0,   294,
     914,     9,   295,   296,     0,   297,   298,     0,     0,   299,
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
       0,     0,     0,     0,     0,   324,     0,   325,   326,   327,
     155,   328,   277,     0,     0,     0,     0,     3,     0,   278,
     279,   280,     0,   281,     0,   282,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,   286,   287,   288,   289,
       0,     0,     0,     0,     0,     0,     0,     0,   290,   291,
       0,     0,   292,   293,     0,     0,   294,     0,     9,   295,
     296,     0,   297,   298,     0,     0,   299,   300,     0,     0,
       0,     0,   301,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,   304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,     0,     0,   282,   283,   284,
     285,     0,   324,     0,   325,   326,   327,   155,   811,   287,
       0,     0,     0,     0,     0,   256,     0,     0,     0,     0,
       0,   291,     0,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,   578,     0,   298,     0,     0,     0,     0,
     257,   258,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   579,   317,     0,     0,     0,     0,     0,     0,   260,
     261,     0,     0,     0,   262,     0,     0,   580,   319,   320,
       0,   321,     0,     0,   322,   323,   282,   283,   284,   285,
       0,     0,   264,     0,   581,     0,   325,   326,   287,   155,
       0,   266,     0,     0,   256,     0,     0,     0,     0,     0,
     291,     0,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,     0,   257,
     258,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     579,   317,     0,     0,     0,     0,     0,     0,   260,   261,
       0,     0,     0,   262,     0,     0,   580,   319,   320,     0,
     321,     0,     0,   322,   323,   282,   283,   284,   285,     0,
       0,   264,     0,   591,     0,   325,   326,   287,   155,     0,
     266,     0,   256,   393,     0,     0,     0,     0,     0,   291,
       0,     0,     0,   293,     0,     0,   294,     0,     0,   295,
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
       0,   262,   394,   677,   263,   318,   319,   320,     0,   321,
       0,     0,   322,   323,   282,   283,   284,   285,     0,   264,
     395,     0,   324,     0,   325,   326,   287,   155,   266,   396,
       0,     0,     0,     0,     0,     0,     0,     0,   291,     0,
       0,     0,   293,     0,     0,   294,     0,     0,   295,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   302,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   306,     0,     0,     0,
       0,     0,     0,     0,   417,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,    43,   308,
     309,   310,   311,   312,   313,   314,   315,   316,    44,   317,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,   319,   320,     0,   321,     0,
       0,   322,   323,     0,     0,   282,   283,   284,   285,     0,
       0,   324,     0,   325,   326,   418,   155,   287,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
       0,     0,     0,   293,     0,     0,   294,     0,     0,   295,
       0,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   287,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,     0,   291,     0,     0,     0,
     293,     0,   324,   294,   325,   326,   295,   155,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    44,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     258,     0,   318,   319,   320,     0,   321,     0,     0,   322,
     323,   282,   283,   284,   285,   697,     0,   606,     0,   324,
     607,   325,   326,   287,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,   293,
       0,     0,   294,     0,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   260,   261,
     303,     0,     0,   262,     0,   678,   263,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,   282,   283,
     284,   285,     0,     0,     0,     0,     0,     0,     0,   698,
     287,   318,   319,   320,     0,   321,     0,     0,   322,   323,
       0,     0,   291,     0,     0,     0,   293,     0,   324,   294,
     325,   326,   295,   155,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,   282,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   287,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,     0,   291,
       0,     0,     0,   293,   397,   324,   294,   325,   326,   295,
     155,     0,     0,   298,     0,     0,   432,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   287,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,     0,   291,     0,     0,     0,
     293,     0,   324,   294,   325,   326,   295,   155,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    44,   317,     0,   282,
     283,   284,   285,     0,     0,     0,     0,     0,     0,     0,
       0,   287,   318,   319,   320,     0,   321,     0,     0,   322,
     323,     0,     0,   291,     0,     0,     0,   293,     0,   324,
     294,   325,   326,   295,   155,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,   282,   283,   284,   285,
       0,     0,     0,     0,     0,     0,     0,     0,   287,   318,
     319,   320,     0,   321,     0,     0,   322,   323,     0,     0,
     291,     0,     0,     0,   293,     0,   324,   294,   325,   326,
     295,   155,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   647,     0,     0,     0,     0,     0,     0,     0,
      43,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      44,   317,     0,   282,   283,   284,   285,     0,     0,     0,
     649,     0,     0,     0,     0,   287,   318,   319,   320,     0,
     321,     0,     0,   322,   323,     0,     0,   291,     0,     0,
       0,   293,     0,   324,   294,   325,   326,   295,   155,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    44,   317,     0,
     282,   283,   284,   285,     0,     0,     0,     0,     0,     0,
       0,     0,   287,   318,   319,   320,     0,   321,     0,     0,
     322,   323,     0,     0,   291,     0,     0,     0,   293,     0,
     324,   294,   325,   326,   295,   155,     0,     0,   298,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   302,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,    43,   308,   309,   310,   311,   312,
     313,   314,   315,   316,    44,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,   258,     0,
     318,   319,   320,     0,   321,     0,     0,   322,   323,   282,
     283,   284,   285,     0,     0,     0,     0,   324,   664,   325,
     326,   287,   155,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,   260,   261,   303,     0,
       0,   262,     0,   681,   263,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,   256,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   257,   258,     0,   318,
     319,   320,     0,   321,     0,     0,   322,   323,   282,   283,
     284,   285,     0,     0,     0,     0,   324,   695,   325,   326,
     287,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,   259,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,   260,   261,   303,     0,     0,
     262,     0,   683,   263,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,     0,
     256,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   257,   258,     0,   318,   319,
     320,     0,   321,     0,     0,   322,   323,   282,   283,   284,
     285,     0,     0,     0,     0,   324,   713,   325,   326,   287,
     155,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,   260,   261,   303,     0,     0,   262,
       0,   685,   263,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,   256,
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    44,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   257,   258,     0,   318,   319,   320,
       0,   321,     0,     0,   322,   323,   282,   283,   284,   285,
       0,     0,     0,     0,   324,   722,   325,   326,   287,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     291,     0,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,   406,   259,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,   692,   261,   303,     0,     0,   262,     0,
       0,   263,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,     0,   256,     0,
      43,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      44,   317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,   258,     0,   318,   319,   320,     0,
     321,     0,     0,   322,   323,   282,   283,   284,   285,   866,
       0,     0,     0,   324,   824,   325,   326,   287,   155,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
       0,     0,     0,   293,     0,     0,   294,     0,     0,   295,
       0,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,   260,   261,   303,     0,     0,   262,     0,   707,
     263,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,     0,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   287,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,     0,   291,     0,     0,     0,
     293,     0,   324,   294,   325,   326,   295,   155,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    44,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   318,   319,   320,     0,   321,     0,     0,   322,
     323,   282,   283,   284,   285,     0,     0,     0,     0,   324,
     872,   325,   326,   287,   155,   257,   258,     0,     0,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,   293,
       0,     0,   294,     0,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,   259,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,   884,     0,     0,   260,   261,     0,     0,     0,   262,
       0,   787,   263,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,   264,   282,   283,
     284,   285,   904,     0,     0,     0,   266,     0,     0,     0,
     287,   318,   319,   320,     0,   321,     0,     0,   322,   323,
       0,     0,   291,     0,     0,     0,   293,     0,   324,   294,
     325,   326,   295,   155,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,   282,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   287,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,     0,   291,
       0,     0,     0,   293,     0,   324,   294,   325,   326,   295,
     155,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,   473,   474,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   473,   474,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,   325,   326,     0,   155,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   560,   495,   496,
     497,   498,   473,   474,   499,   500,   501,   502,   503,   504,
     485,   486,     0,     0,     0,     0,     0,     0,   493,     0,
     495,   496,   497,   498,     0,     0,   499,     0,   256,   561,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,   257,   258,     0,     0,     0,   700,     0,
       0,   514,   515,   516,     0,     0,     0,     0,     0,     0,
       0,     0,   517,     0,   518,     0,     0,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   560,   495,   496,   497,
     498,   473,   474,   499,   500,   501,   502,   503,   504,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   506,
       0,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,     0,   260,   261,     0,     0,     0,   262,     0,   517,
     263,   518,     0,     0,     0,     0,     0,   708,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   560,   495,   496,   497,   498,
     473,   474,   499,   500,   501,   502,   503,   504,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   506,     0,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
       0,     0,     0,     0,     0,     0,     0,     0,   517,     0,
     518,     0,     0,     0,     0,     0,   765,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,   474,     0,
       0,     0,     0,     0,     0,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   560,   495,   496,   497,   498,     0,
       0,   499,   500,   501,   502,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
     483,   484,   485,   486,   487,   830,     0,   490,   491,   492,
     493,     0,   495,   496,   497,   498,     0,     0,   499,     0,
     501,   502,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,     0,     0,     0,     0,
     473,   474,     0,     0,   517,     0,   518,     0,     0,     0,
       0,     0,   893,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   702,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   705,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   786,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   890,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   892,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   473,
     474,     0,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   913,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   473,
     474,     0,     0,     0,     0,     0,     0,   517,     0,   518,
     549,     0,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,     0,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   518,   753,
     473,   474,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,     0,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   518,   758,
       0,     0,     0,     0,     0,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   560,   495,   496,   497,   498,  -223,
       0,   499,   500,   501,   502,   503,   504,   473,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
       0,     0,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,   474,     0,     0,     0,     0,
       0,     0,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,     0,     0,   499,   500,
     501,   502,   503,   504,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   505,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   473,   474,     0,     0,
       0,     0,     0,     0,   517,     0,   518,   483,   484,   485,
     486,   487,     0,     0,   490,   491,   492,   493,     0,   495,
     496,   497,   498,     0,     0,   499,     0,   501,   502,     0,
       0,   594,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   473,   474,     0,     0,     0,     0,     0,
       0,   517,     0,   518,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   487,     0,     0,   490,   491,   492,   493,
       0,   495,   496,   497,   498,     0,     0,   499,     0,   501,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   756,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   473,   474,     0,     0,     0,
       0,     0,     0,   517,     0,   518,   483,   484,   485,   486,
     487,     0,     0,   490,   491,   492,   493,     0,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,     0,     0,     0,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   473,   474,     0,     0,     0,     0,     0,     0,
     517,     0,   518,     0,     0,     0,     0,     0,   483,   484,
     485,   486,   487,     0,     0,   490,   491,   492,   493,     0,
     495,   496,   497,   498,     0,     0,   499,     0,   501,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,     0,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   473,   474,     0,     0,     0,     0,
       0,     0,   517,     0,   518,   483,   484,   485,   486,   487,
       0,     0,   490,   491,   492,   493,     0,   495,   496,   497,
     498,     0,     0,   499,     0,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   473,   474,     0,     0,     0,     0,     0,     0,   517,
       0,   518,     0,     0,     0,     0,     0,   483,   484,   485,
     486,   487,     0,     0,   490,   491,   492,   493,     0,   495,
     496,   497,   498,     0,     0,   499,     0,   501,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,   474,   256,   509,   510,   511,   512,   513,
     514,   515,   516,     0,     0,     0,     0,     0,     0,     0,
       0,   517,     0,   518,   483,   484,   485,   486,   487,   257,
     258,   490,   491,   492,   493,     0,   495,   496,   497,   498,
       0,   256,   499,     0,   501,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   257,   258,     0,     0,
       0,     0,     0,   510,   511,   512,   513,   514,   515,   516,
     257,   258,     0,   256,     0,   483,   484,   259,   517,   487,
     518,     0,   490,   491,   492,   493,   256,   495,   496,   497,
     498,     0,     0,   499,     0,   501,   502,     0,   257,   258,
       0,     0,     0,     0,     0,     0,     0,   256,   260,   261,
       0,   257,   258,   262,   259,   788,   263,     0,     0,   506,
       0,   507,   508,   509,   510,   511,     0,     0,   259,   515,
       0,   264,   257,   258,     0,     0,     0,   256,     0,   517,
     266,   518,     0,     0,     0,   260,   261,   256,     0,     0,
     262,     0,   789,   263,     0,     0,   259,     0,     0,   260,
     261,     0,   257,   258,   262,     0,   831,   263,   264,   259,
       0,     0,   257,   258,     0,     0,     0,   266,     0,     0,
       0,     0,   264,     0,     0,     0,     0,   260,   261,     0,
     259,   266,   262,     0,   852,   263,     0,     0,     0,     0,
     260,   261,     0,     0,     0,   262,     0,   886,   263,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     259,   260,   261,   264,     0,     0,   262,     0,   888,   263,
     259,     0,   266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,   260,   261,   266,     0,   443,   262,     0,   917,   263,
       0,   260,   261,     0,     0,     0,   262,   176,   922,   263,
       0,     0,     0,   177,   264,     0,     0,     0,     0,     0,
       0,     0,     0,   266,   264,     0,     0,     0,     0,   178,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,     0,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   445,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   176,     0,
       0,     0,     0,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,     0,     0,   179,     0,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,   179,    44,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,     0,     0,    43,
     308,   309,   310,     0,     0,     0,     0,     0,   179,    44,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,   179,    44,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   135,   136,   137,   138,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,     0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      13,   273,   230,   109,   269,    81,   286,   271,   385,   519,
     387,    73,   389,     7,   446,   160,   307,   538,   310,   540,
     584,   542,   109,     0,   304,    50,    33,    32,   592,     6,
      14,    15,    19,    19,   606,    34,    21,    19,    47,    50,
      18,   119,   116,   117,   113,   555,   556,   116,   117,   129,
     127,   146,    29,   145,    31,   138,    33,   148,    35,   145,
     145,    74,   128,    40,   145,   138,    14,    15,   354,    54,
      47,   138,   176,   114,   138,   138,    53,   138,   138,   138,
     175,   122,   148,   175,   370,   176,    32,   128,   171,   175,
     175,   172,   172,   114,    71,   169,   109,   175,   167,   176,
     169,   122,   132,   145,   177,   146,   105,   106,   145,   145,
     177,    57,    58,   177,   177,    92,   177,   177,   153,   148,
     172,   128,   228,   175,   153,   146,   145,   138,   170,   138,
     410,   138,   138,   170,   170,   129,   281,   417,   138,   138,
     175,   228,   171,   715,   157,   114,   151,   145,   132,   713,
     175,   170,   136,   122,   138,   139,   436,   114,   145,   145,
     138,   223,   268,   145,   169,   122,    20,    21,   114,   115,
     140,   141,   157,   178,   172,   169,   122,   146,   284,   145,
     471,   268,   167,   177,   132,   114,   128,   171,   136,   146,
     138,   139,   169,   122,   151,   138,   138,   284,   175,   709,
     146,   147,   308,   309,   145,   151,   172,   145,   154,   128,
     145,   721,   175,   289,   177,   228,   114,   146,   146,   138,
     153,   308,   309,   169,   175,   301,   177,    11,   749,   175,
     794,   172,   178,   152,   172,   153,   613,   172,    22,    23,
     804,   751,   175,   140,   141,   142,   153,   145,   146,   529,
     148,   105,   106,   151,    59,   268,   145,   175,   148,   113,
     148,   115,   116,   117,   118,   153,   372,   121,   175,    43,
      44,   284,   378,   379,   380,   381,   145,   138,   384,   551,
     386,   713,   388,   171,   390,   372,   138,   578,   138,   810,
     722,   378,   379,   380,   381,   308,   309,   384,   443,   386,
     445,   388,   408,   595,    20,    21,   170,   148,   321,   148,
     138,   175,   153,   167,   153,   169,    66,   128,   171,   145,
      70,   408,   148,   138,   469,   151,   132,   176,   138,   435,
     171,   138,   171,   146,    20,    21,   412,    87,    88,    89,
      90,     4,     5,   138,     7,   176,   176,   138,   435,   425,
     173,   174,    34,   176,    34,   732,   176,   171,   148,   372,
       1,   140,    54,   138,   176,   378,   379,   380,   381,   138,
     148,   384,   176,   386,   175,   388,   152,   390,   171,   152,
     152,   152,   152,   175,   152,   152,   462,   103,   104,   105,
     106,   107,   672,   152,   110,   408,   152,   113,   152,   115,
     116,   117,   118,   416,   668,   121,   148,   123,   124,   175,
     138,    20,    21,    32,   138,   129,   171,   152,   171,   105,
     106,   152,   435,   171,   175,   152,   691,   113,   132,   115,
     116,   117,   118,   152,   174,   121,   152,   153,   154,   155,
     156,   157,   158,    36,    19,   458,   175,   169,   138,   175,
     175,   167,   175,   169,   175,   172,   171,   170,   170,   119,
     566,   567,    13,   569,   148,   132,   174,   176,   154,   155,
     156,   157,   158,   129,    39,     7,   138,   138,   175,   566,
     567,   167,   569,   169,   138,   138,    43,   563,   145,    20,
      21,   170,   177,   145,   103,   104,   105,   106,   107,   138,
     140,   110,   111,   112,   113,   733,   115,   116,   117,   118,
     140,   138,   121,   171,   123,   124,   152,     1,    20,    21,
     138,   153,   153,   152,   796,   153,   138,   138,   170,   177,
     140,   138,   170,   170,   138,   170,   138,   138,   147,   172,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     171,   138,   177,   566,   567,   170,   569,   170,   167,   153,
     169,   175,   170,   152,   170,   170,   152,   176,   153,    50,
     676,   170,   170,   170,   105,   106,   172,   177,   170,   170,
     170,   687,   113,   689,   172,   116,   117,   118,   172,   676,
     121,   450,    55,    98,    58,   854,   240,    56,    74,   828,
     687,   103,   104,   105,   106,   107,   761,   604,   110,   111,
     112,   113,     1,   115,   116,   117,   118,   820,   707,   121,
     666,   123,   124,   370,   275,   731,   391,   469,   173,   525,
      68,   689,   720,   879,   383,   284,   167,   446,   169,    -1,
      -1,    -1,    -1,    -1,   731,   147,    32,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,   734,    -1,
      -1,    -1,    -1,   676,   230,   167,    -1,   169,    -1,    -1,
      -1,    57,    58,   175,   687,    -1,   689,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   264,    -1,
      -1,    -1,    -1,    20,    21,    -1,   782,    -1,    -1,    -1,
      -1,    -1,   278,    -1,    -1,    -1,    -1,   823,   731,    -1,
     286,    -1,    -1,    -1,   290,    -1,   292,    -1,    -1,   115,
     836,    -1,    -1,    -1,   300,    -1,   823,   813,   304,   305,
     306,    -1,    -1,    -1,   310,    32,   822,    -1,    -1,   836,
      -1,    -1,   318,   319,   320,    -1,   322,   323,   324,    -1,
     146,   147,    -1,    -1,    -1,   151,    -1,   333,   154,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   171,   103,   104,   105,   106,
     107,    -1,   178,   110,   111,   112,   113,   903,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
     823,    -1,   918,    -1,    -1,    -1,   903,    -1,    -1,    -1,
      -1,    -1,    -1,   836,    -1,    -1,    -1,    -1,   115,    -1,
     147,   918,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,   410,    -1,    -1,    -1,    -1,    -1,
     167,   417,   169,    -1,    -1,   172,    -1,    -1,   424,   146,
     147,    -1,   428,    -1,   151,    -1,    -1,   154,   881,    -1,
     436,    20,    21,    -1,    -1,    -1,    -1,    -1,   444,    -1,
     446,    -1,   169,    -1,    -1,    -1,   899,    -1,   175,    -1,
     903,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   918,    -1,    -1,    -1,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    -1,
      -1,   487,   488,   489,   490,   491,   492,    -1,   494,   495,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,    -1,   518,    -1,   103,   104,   105,   106,    20,    21,
      -1,    -1,    -1,   529,   113,    -1,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   557,    -1,    -1,   560,   561,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,
      -1,    -1,    -1,    -1,   580,   581,    -1,    -1,   167,    -1,
     169,    -1,    57,    58,    -1,   591,    -1,    -1,   594,   595,
      -1,    -1,   598,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,   125,   126,    -1,    -1,    -1,    -1,   635,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   647,    -1,    -1,   146,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,   672,   169,    -1,    -1,
      -1,   146,   147,   175,    -1,    -1,   151,    20,    21,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   693,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,   178,    -1,   711,   712,    -1,    32,    -1,
      -1,    -1,   718,    -1,   720,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   733,    -1,    -1,
      -1,   737,    -1,    57,    58,   741,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     756,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   167,    -1,   169,   151,    -1,   153,
     154,   837,   175,   839,   840,   841,   842,   843,    -1,    -1,
     846,   847,    -1,   849,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,   858,    -1,   178,     1,    -1,    -1,    -1,    -1,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,   884,    25,
      26,    27,    28,    -1,    -1,   891,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    41,    42,    -1,    -1,    45,
     906,    47,    48,    49,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,   173,   174,   175,
     176,   177,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    -1,    41,    42,    -1,    -1,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,   171,    -1,   173,   174,   175,   176,   177,    26,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
      57,    58,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,    -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,
      -1,   158,    -1,    -1,   161,   162,    14,    15,    16,    17,
      -1,    -1,   169,    -1,   171,    -1,   173,   174,    26,   176,
      -1,   178,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      58,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
      -1,    -1,    -1,   151,    -1,    -1,   154,   155,   156,    -1,
     158,    -1,    -1,   161,   162,    14,    15,    16,    17,    -1,
      -1,   169,    -1,   171,    -1,   173,   174,    26,   176,    -1,
     178,    -1,    32,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    57,    58,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,
      -1,   151,   151,   153,   154,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    14,    15,    16,    17,    -1,   169,
     169,    -1,   171,    -1,   173,   174,    26,   176,   178,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,   156,    -1,   158,    -1,
      -1,   161,   162,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   171,    -1,   173,   174,   175,   176,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,   171,    45,   173,   174,    48,   176,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    -1,   154,   155,   156,    -1,   158,    -1,    -1,   161,
     162,    14,    15,    16,    17,    18,    -1,   169,    -1,   171,
     172,   173,   174,    26,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   146,   147,
      93,    -1,    -1,   151,    -1,   153,   154,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      26,   154,   155,   156,    -1,   158,    -1,    -1,   161,   162,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,   171,    45,
     173,   174,    48,   176,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,    38,
      -1,    -1,    -1,    42,   170,   171,    45,   173,   174,    48,
     176,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,   171,    45,   173,   174,    48,   176,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,   154,   155,   156,    -1,   158,    -1,    -1,   161,
     162,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,   171,
      45,   173,   174,    48,   176,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   154,
     155,   156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,   171,    45,   173,   174,
      48,   176,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,   154,   155,   156,    -1,
     158,    -1,    -1,   161,   162,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,   171,    45,   173,   174,    48,   176,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,   154,   155,   156,    -1,   158,    -1,    -1,
     161,   162,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
     171,    45,   173,   174,    48,   176,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,
     154,   155,   156,    -1,   158,    -1,    -1,   161,   162,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,   171,   172,   173,
     174,    26,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   146,   147,    93,    -1,
      -1,   151,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    32,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,   154,
     155,   156,    -1,   158,    -1,    -1,   161,   162,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,   171,   172,   173,   174,
      26,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,   146,   147,    93,    -1,    -1,
     151,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      32,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    -1,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,   171,   172,   173,   174,    26,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   146,   147,    93,    -1,    -1,   151,
      -1,   153,   154,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    32,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    -1,   154,   155,   156,
      -1,   158,    -1,    -1,   161,   162,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   171,   172,   173,   174,    26,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   146,   147,    93,    -1,    -1,   151,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    32,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    -1,   154,   155,   156,    -1,
     158,    -1,    -1,   161,   162,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   171,   172,   173,   174,    26,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   146,   147,    93,    -1,    -1,   151,    -1,   153,
     154,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,   171,    45,   173,   174,    48,   176,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,   154,   155,   156,    -1,   158,    -1,    -1,   161,
     162,    14,    15,    16,    17,    -1,    -1,    -1,    -1,   171,
     172,   173,   174,    26,   176,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,   115,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,   146,   147,    -1,    -1,    -1,   151,
      -1,   153,   154,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   169,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      26,   154,   155,   156,    -1,   158,    -1,    -1,   161,   162,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,   171,    45,
     173,   174,    48,   176,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,   171,    45,   173,   174,    48,
     176,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,   173,   174,    -1,   176,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    20,    21,   121,   122,   123,   124,   125,   126,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     115,   116,   117,   118,    -1,    -1,   121,    -1,    32,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,    57,    58,    -1,    -1,    -1,   175,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,   125,   126,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,   146,   147,    -1,    -1,    -1,   151,    -1,   167,
     154,   169,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     103,   104,   105,   106,   107,   175,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,   167,    -1,   169,    -1,    -1,    -1,
      -1,    -1,   175,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      20,    21,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,   125,   126,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    32,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,   103,   104,   105,   106,   107,    57,
      58,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    32,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
      57,    58,    -1,    32,    -1,   103,   104,   115,   167,   107,
     169,    -1,   110,   111,   112,   113,    32,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,   124,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   146,   147,
      -1,    57,    58,   151,   115,   153,   154,    -1,    -1,   147,
      -1,   149,   150,   151,   152,   153,    -1,    -1,   115,   157,
      -1,   169,    57,    58,    -1,    -1,    -1,    32,    -1,   167,
     178,   169,    -1,    -1,    -1,   146,   147,    32,    -1,    -1,
     151,    -1,   153,   154,    -1,    -1,   115,    -1,    -1,   146,
     147,    -1,    57,    58,   151,    -1,   153,   154,   169,   115,
      -1,    -1,    57,    58,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,   146,   147,    -1,
     115,   178,   151,    -1,   153,   154,    -1,    -1,    -1,    -1,
     146,   147,    -1,    -1,    -1,   151,    -1,   153,   154,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
     115,   146,   147,   169,    -1,    -1,   151,    -1,   153,   154,
     115,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   178,    -1,    12,   151,    -1,   153,   154,
      -1,   146,   147,    -1,    -1,    -1,   151,    24,   153,   154,
      -1,    -1,    -1,    30,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   169,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,   138,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    60,   138,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,   138,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,   161,   162
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   180,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    71,    92,   169,   175,   181,   189,   190,   194,   214,
     218,   234,   291,   296,   297,   301,   337,   339,   138,   209,
     210,   132,   195,   196,   138,   191,   192,   138,   176,   138,
     340,   338,    33,   128,   138,   211,   212,   213,   225,     4,
       5,     7,   300,    59,   290,   146,   145,   148,   145,    21,
      54,   157,   167,   193,   148,   176,   138,   294,   295,   138,
     138,   138,   128,   171,   145,   170,    34,   105,   106,   138,
     217,   219,   220,   138,   299,   176,   292,    14,    15,   132,
     136,   138,   139,   171,   183,   209,   132,   196,   138,   138,
     138,    66,    70,    87,    88,    89,    90,   305,   293,   146,
     177,   294,   176,   176,   138,   210,   212,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   123,   124,   125,   126,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   161,
     162,    34,    34,   171,   215,   176,   221,   148,   298,   302,
     289,     1,   140,   182,    14,    15,   132,   136,   138,   183,
     207,   208,   193,   176,   138,   177,    24,    30,    46,    60,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,   225,   304,   306,   307,   309,   312,   138,   308,   138,
     282,   283,   172,   129,   172,   273,   278,   279,   148,   216,
     223,   225,   176,   138,   177,   288,   303,   175,   140,   141,
     145,   172,   293,   146,   175,   152,   152,   152,   152,   171,
     152,   152,   152,   152,   152,   152,    32,    57,    58,   115,
     146,   147,   151,   154,   169,   175,   178,   175,   148,   175,
     210,    47,   285,   303,   172,   175,   312,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    25,    26,    27,    28,
      37,    38,    41,    42,    45,    48,    49,    51,    52,    55,
      56,    61,    71,    93,    94,   105,   106,   120,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   139,   154,   155,
     156,   158,   161,   162,   171,   173,   174,   175,   177,   185,
     187,   197,   198,   201,   202,   203,   204,   205,   206,   221,
     222,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     234,   235,   236,   243,   252,   256,   257,   258,   259,   260,
     261,   262,   267,   270,   304,   341,   345,   350,   352,   354,
     276,   145,   148,   151,   287,   207,   177,   270,   315,   317,
     319,   313,   138,   310,   321,   323,   325,   327,   329,   331,
     333,   335,    32,    32,   151,   169,   178,   170,   270,   138,
     177,   312,   177,   282,   170,   285,   114,   146,   148,   151,
     284,   278,   270,   303,   312,   341,   171,   114,   175,   224,
     258,   270,   221,   270,   171,   270,   152,   152,   171,   175,
     152,   132,    55,   270,   221,   152,   114,   224,   270,   270,
     270,   174,   256,    12,   312,    12,   312,   270,   347,   351,
     184,   270,   270,   270,   225,   270,   270,   270,   174,    36,
     175,   175,   270,   138,   171,   175,   175,   175,    19,   233,
     214,   119,   175,    20,    21,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     122,   123,   124,   125,   126,   146,   147,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   167,   169,   171,
     177,   214,   273,   275,   138,   312,   114,   122,   146,   286,
     175,   312,   312,   312,   312,   172,   308,   312,   215,   312,
     215,   312,   215,   138,   280,   281,   312,   283,   170,   170,
     170,   312,   224,   270,   221,    19,   171,   225,   224,   270,
     114,   146,   175,    13,   270,   221,   237,   239,   270,   241,
     148,   132,   312,   224,   270,   175,   174,   303,    50,   138,
     154,   171,   270,   342,   343,   344,   346,   347,   348,   349,
     303,   171,   343,   349,   127,   175,   177,   141,   142,   182,
     188,   172,   152,   225,   176,   221,   169,   172,   250,   270,
      39,   288,   129,   255,   256,    18,   138,   138,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   138,   120,   270,   270,   138,   270,
     270,   270,   270,   270,   270,   270,   270,   120,   270,    21,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   138,   268,   270,   172,   250,     7,    43,   274,   175,
     146,   175,   286,   224,   270,   153,   175,   153,   153,   153,
     175,   153,   216,   153,   216,   153,   216,   148,   153,   175,
     153,   175,   146,   284,   250,   172,   250,    18,   152,   270,
     175,   221,   172,   312,   312,   172,   312,   153,   175,    19,
     256,   122,   284,   172,   145,   175,   344,   170,   145,   170,
     175,    19,   172,   344,   177,   270,   347,   177,   270,   140,
     186,   263,   265,   223,    11,    22,    23,   199,   200,   138,
     342,   145,   172,   114,   122,   146,   151,   253,   254,   215,
     152,   171,   270,   170,   270,   138,   148,     1,   170,   172,
      44,   274,   285,   224,   270,   175,   316,   312,   320,   314,
     311,   322,   153,   326,   153,   330,   153,   312,   334,   280,
     336,   270,   175,   172,   152,   138,   172,   153,   153,   153,
     255,   250,   270,   270,   343,   344,   138,   342,   170,   170,
     270,   170,   348,   250,   343,   177,   170,   143,   140,   312,
     215,   177,   221,   270,   170,   270,   138,   138,   138,   138,
     145,   170,   216,   271,   172,   250,   270,   269,   277,   220,
     175,   153,   324,   328,   332,   221,   244,   153,   175,   238,
     240,   242,   171,   175,   344,   170,   122,   284,   170,   175,
     344,   170,   153,   216,   221,   172,   253,   170,   127,   221,
     251,   312,   172,   219,   318,   312,    18,   270,   138,   270,
     270,   270,   172,   270,   270,   170,   170,   270,   270,   270,
     177,   264,   153,   200,   114,   270,   153,   175,   153,   152,
     172,   153,   172,   175,   353,   170,   353,   170,   225,   266,
     270,   272,   245,   246,    18,   270,    50,   170,   177,   225,
     172,   312,   152,   172,   270,   170,   170,   153,   248,   247,
     312,   172,   153,   249,   172
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   179,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   183,   184,
     184,   184,   185,   186,   186,   188,   187,   189,   190,   191,
     191,   191,   192,   192,   193,   193,   194,   195,   195,   196,
     196,   197,   198,   198,   199,   199,   200,   200,   200,   201,
     201,   202,   203,   204,   205,   206,   207,   207,   207,   207,
     207,   207,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   211,   211,   212,   212,   213,   213,
     214,   214,   215,   215,   215,   216,   216,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   218,   219,   220,   221,   221,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   223,   223,
     223,   224,   224,   225,   225,   225,   226,   227,   227,   227,
     227,   228,   229,   230,   230,   230,   230,   230,   231,   231,
     231,   231,   232,   233,   233,   234,   234,   235,   237,   238,
     236,   239,   240,   236,   241,   242,   236,   243,   243,   243,
     244,   245,   243,   246,   247,   243,   248,   249,   243,   250,
     250,   251,   251,   251,   252,   252,   252,   253,   253,   253,
     253,   254,   254,   255,   255,   256,   256,   257,   257,   257,
     257,   257,   257,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   259,
     259,   260,   261,   261,   262,   263,   264,   262,   265,   266,
     262,   267,   268,   269,   267,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   271,   272,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   273,   273,   274,   274,   275,   276,   276,   277,   276,
     276,   278,   278,   279,   279,   280,   280,   281,   281,   282,
     283,   283,   284,   284,   285,   285,   285,   285,   285,   285,
     286,   286,   286,   287,   287,   288,   288,   288,   288,   288,
     289,   289,   290,   290,   291,   292,   291,   293,   293,   293,
     294,   295,   295,   296,   297,   297,   298,   298,   299,   300,
     300,   302,   301,   303,   303,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     305,   305,   305,   305,   305,   305,   306,   307,   307,   308,
     308,   310,   311,   309,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   313,   314,   312,   312,   315,   316,   312,   317,   318,
     312,   319,   320,   312,   312,   321,   322,   312,   323,   324,
     312,   312,   325,   326,   312,   327,   328,   312,   312,   329,
     330,   312,   331,   332,   312,   333,   334,   312,   335,   336,
     312,   338,   337,   340,   339,   341,   341,   341,   341,   342,
     342,   342,   342,   343,   343,   344,   344,   345,   345,   345,
     345,   345,   345,   346,   346,   347,   348,   348,   349,   349,
     350,   350,   351,   351,   352,   353,   353,   354,   354
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
       2,     3,     4,     0,     2,     1,     1,     3,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     5,     8,    10,
       0,     0,    10,     0,     0,    13,     0,     0,    15,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     1,     3,     0,     4,     1,     6,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     6,     5,     6,     3,     0,     0,     8,     0,     0,
       9,     3,     0,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
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
    case 138: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3327 "ds_parser.cpp"
        break;

    case 182: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3333 "ds_parser.cpp"
        break;

    case 183: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3339 "ds_parser.cpp"
        break;

    case 184: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3345 "ds_parser.cpp"
        break;

    case 185: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3351 "ds_parser.cpp"
        break;

    case 187: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3357 "ds_parser.cpp"
        break;

    case 191: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3363 "ds_parser.cpp"
        break;

    case 197: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3369 "ds_parser.cpp"
        break;

    case 198: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3375 "ds_parser.cpp"
        break;

    case 200: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3381 "ds_parser.cpp"
        break;

    case 202: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3387 "ds_parser.cpp"
        break;

    case 203: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3393 "ds_parser.cpp"
        break;

    case 204: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3399 "ds_parser.cpp"
        break;

    case 205: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3405 "ds_parser.cpp"
        break;

    case 206: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3411 "ds_parser.cpp"
        break;

    case 207: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3417 "ds_parser.cpp"
        break;

    case 208: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3423 "ds_parser.cpp"
        break;

    case 209: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3429 "ds_parser.cpp"
        break;

    case 210: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3435 "ds_parser.cpp"
        break;

    case 211: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3441 "ds_parser.cpp"
        break;

    case 212: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3447 "ds_parser.cpp"
        break;

    case 213: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3453 "ds_parser.cpp"
        break;

    case 214: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3459 "ds_parser.cpp"
        break;

    case 215: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3465 "ds_parser.cpp"
        break;

    case 216: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3471 "ds_parser.cpp"
        break;

    case 217: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3477 "ds_parser.cpp"
        break;

    case 221: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3483 "ds_parser.cpp"
        break;

    case 222: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3489 "ds_parser.cpp"
        break;

    case 223: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3495 "ds_parser.cpp"
        break;

    case 224: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 225: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3507 "ds_parser.cpp"
        break;

    case 226: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3513 "ds_parser.cpp"
        break;

    case 227: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3519 "ds_parser.cpp"
        break;

    case 228: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3525 "ds_parser.cpp"
        break;

    case 229: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3531 "ds_parser.cpp"
        break;

    case 230: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3537 "ds_parser.cpp"
        break;

    case 231: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3543 "ds_parser.cpp"
        break;

    case 232: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3549 "ds_parser.cpp"
        break;

    case 235: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3555 "ds_parser.cpp"
        break;

    case 236: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3561 "ds_parser.cpp"
        break;

    case 243: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3567 "ds_parser.cpp"
        break;

    case 250: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3573 "ds_parser.cpp"
        break;

    case 251: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3579 "ds_parser.cpp"
        break;

    case 253: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3585 "ds_parser.cpp"
        break;

    case 254: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3591 "ds_parser.cpp"
        break;

    case 255: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3597 "ds_parser.cpp"
        break;

    case 256: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3603 "ds_parser.cpp"
        break;

    case 257: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3609 "ds_parser.cpp"
        break;

    case 258: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3615 "ds_parser.cpp"
        break;

    case 259: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3621 "ds_parser.cpp"
        break;

    case 260: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3627 "ds_parser.cpp"
        break;

    case 261: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3633 "ds_parser.cpp"
        break;

    case 262: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3639 "ds_parser.cpp"
        break;

    case 267: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3645 "ds_parser.cpp"
        break;

    case 270: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 273: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3657 "ds_parser.cpp"
        break;

    case 275: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3663 "ds_parser.cpp"
        break;

    case 276: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3669 "ds_parser.cpp"
        break;

    case 278: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3675 "ds_parser.cpp"
        break;

    case 279: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3681 "ds_parser.cpp"
        break;

    case 280: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3687 "ds_parser.cpp"
        break;

    case 281: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3693 "ds_parser.cpp"
        break;

    case 282: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3699 "ds_parser.cpp"
        break;

    case 283: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3705 "ds_parser.cpp"
        break;

    case 285: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3711 "ds_parser.cpp"
        break;

    case 288: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3717 "ds_parser.cpp"
        break;

    case 289: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3723 "ds_parser.cpp"
        break;

    case 293: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3729 "ds_parser.cpp"
        break;

    case 298: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3735 "ds_parser.cpp"
        break;

    case 303: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3741 "ds_parser.cpp"
        break;

    case 306: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3747 "ds_parser.cpp"
        break;

    case 307: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3753 "ds_parser.cpp"
        break;

    case 308: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3759 "ds_parser.cpp"
        break;

    case 309: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3765 "ds_parser.cpp"
        break;

    case 312: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3771 "ds_parser.cpp"
        break;

    case 341: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3777 "ds_parser.cpp"
        break;

    case 342: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3783 "ds_parser.cpp"
        break;

    case 343: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3789 "ds_parser.cpp"
        break;

    case 344: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3795 "ds_parser.cpp"
        break;

    case 345: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3801 "ds_parser.cpp"
        break;

    case 346: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3807 "ds_parser.cpp"
        break;

    case 347: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3813 "ds_parser.cpp"
        break;

    case 348: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3819 "ds_parser.cpp"
        break;

    case 349: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3825 "ds_parser.cpp"
        break;

    case 350: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3831 "ds_parser.cpp"
        break;

    case 351: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3837 "ds_parser.cpp"
        break;

    case 352: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3843 "ds_parser.cpp"
        break;

    case 353: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3849 "ds_parser.cpp"
        break;

    case 354: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3855 "ds_parser.cpp"
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
#line 449 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4155 "ds_parser.cpp"
    break;

  case 16:
#line 461 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4161 "ds_parser.cpp"
    break;

  case 17:
#line 462 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4167 "ds_parser.cpp"
    break;

  case 18:
#line 466 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4173 "ds_parser.cpp"
    break;

  case 19:
#line 470 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4183 "ds_parser.cpp"
    break;

  case 20:
#line 475 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4197 "ds_parser.cpp"
    break;

  case 21:
#line 484 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4207 "ds_parser.cpp"
    break;

  case 22:
#line 492 "ds_parser.ypp"
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
#line 4225 "ds_parser.cpp"
    break;

  case 23:
#line 508 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4235 "ds_parser.cpp"
    break;

  case 24:
#line 513 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4245 "ds_parser.cpp"
    break;

  case 25:
#line 521 "ds_parser.ypp"
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
#line 4272 "ds_parser.cpp"
    break;

  case 26:
#line 542 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4284 "ds_parser.cpp"
    break;

  case 27:
#line 551 "ds_parser.ypp"
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
#line 4308 "ds_parser.cpp"
    break;

  case 29:
#line 577 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4316 "ds_parser.cpp"
    break;

  case 30:
#line 580 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4327 "ds_parser.cpp"
    break;

  case 31:
#line 586 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4338 "ds_parser.cpp"
    break;

  case 32:
#line 595 "ds_parser.ypp"
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
#line 4364 "ds_parser.cpp"
    break;

  case 33:
#line 616 "ds_parser.ypp"
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
#line 4389 "ds_parser.cpp"
    break;

  case 34:
#line 639 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4395 "ds_parser.cpp"
    break;

  case 35:
#line 640 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4401 "ds_parser.cpp"
    break;

  case 39:
#line 653 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4409 "ds_parser.cpp"
    break;

  case 40:
#line 656 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4417 "ds_parser.cpp"
    break;

  case 41:
#line 662 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4425 "ds_parser.cpp"
    break;

  case 42:
#line 668 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4433 "ds_parser.cpp"
    break;

  case 43:
#line 671 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4441 "ds_parser.cpp"
    break;

  case 44:
#line 677 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4447 "ds_parser.cpp"
    break;

  case 45:
#line 678 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4453 "ds_parser.cpp"
    break;

  case 46:
#line 682 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4459 "ds_parser.cpp"
    break;

  case 47:
#line 683 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4465 "ds_parser.cpp"
    break;

  case 48:
#line 684 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4476 "ds_parser.cpp"
    break;

  case 49:
#line 693 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4482 "ds_parser.cpp"
    break;

  case 50:
#line 694 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4488 "ds_parser.cpp"
    break;

  case 51:
#line 698 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4499 "ds_parser.cpp"
    break;

  case 52:
#line 707 "ds_parser.ypp"
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
#line 4517 "ds_parser.cpp"
    break;

  case 53:
#line 723 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4527 "ds_parser.cpp"
    break;

  case 54:
#line 731 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4539 "ds_parser.cpp"
    break;

  case 55:
#line 741 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4550 "ds_parser.cpp"
    break;

  case 56:
#line 750 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4556 "ds_parser.cpp"
    break;

  case 57:
#line 751 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4562 "ds_parser.cpp"
    break;

  case 58:
#line 752 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4568 "ds_parser.cpp"
    break;

  case 59:
#line 753 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4574 "ds_parser.cpp"
    break;

  case 60:
#line 754 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4580 "ds_parser.cpp"
    break;

  case 61:
#line 755 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4586 "ds_parser.cpp"
    break;

  case 62:
#line 759 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4596 "ds_parser.cpp"
    break;

  case 63:
#line 764 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4606 "ds_parser.cpp"
    break;

  case 64:
#line 772 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4612 "ds_parser.cpp"
    break;

  case 65:
#line 773 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4618 "ds_parser.cpp"
    break;

  case 66:
#line 774 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4624 "ds_parser.cpp"
    break;

  case 67:
#line 775 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4630 "ds_parser.cpp"
    break;

  case 68:
#line 776 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4636 "ds_parser.cpp"
    break;

  case 69:
#line 777 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4642 "ds_parser.cpp"
    break;

  case 70:
#line 778 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4648 "ds_parser.cpp"
    break;

  case 71:
#line 779 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4656 "ds_parser.cpp"
    break;

  case 72:
#line 785 "ds_parser.ypp"
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
#line 4674 "ds_parser.cpp"
    break;

  case 73:
#line 798 "ds_parser.ypp"
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
#line 4692 "ds_parser.cpp"
    break;

  case 74:
#line 814 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4698 "ds_parser.cpp"
    break;

  case 75:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4704 "ds_parser.cpp"
    break;

  case 76:
#line 819 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4716 "ds_parser.cpp"
    break;

  case 77:
#line 826 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4730 "ds_parser.cpp"
    break;

  case 78:
#line 838 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4739 "ds_parser.cpp"
    break;

  case 79:
#line 842 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4748 "ds_parser.cpp"
    break;

  case 80:
#line 849 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4754 "ds_parser.cpp"
    break;

  case 81:
#line 850 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4760 "ds_parser.cpp"
    break;

  case 82:
#line 854 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4766 "ds_parser.cpp"
    break;

  case 83:
#line 855 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4772 "ds_parser.cpp"
    break;

  case 84:
#line 856 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4778 "ds_parser.cpp"
    break;

  case 85:
#line 860 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4788 "ds_parser.cpp"
    break;

  case 86:
#line 865 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4797 "ds_parser.cpp"
    break;

  case 87:
#line 872 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4806 "ds_parser.cpp"
    break;

  case 88:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4812 "ds_parser.cpp"
    break;

  case 89:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4818 "ds_parser.cpp"
    break;

  case 90:
#line 878 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4824 "ds_parser.cpp"
    break;

  case 91:
#line 879 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4830 "ds_parser.cpp"
    break;

  case 92:
#line 880 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4836 "ds_parser.cpp"
    break;

  case 93:
#line 881 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4842 "ds_parser.cpp"
    break;

  case 94:
#line 882 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4848 "ds_parser.cpp"
    break;

  case 95:
#line 883 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4854 "ds_parser.cpp"
    break;

  case 96:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4860 "ds_parser.cpp"
    break;

  case 97:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4866 "ds_parser.cpp"
    break;

  case 98:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4872 "ds_parser.cpp"
    break;

  case 99:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4878 "ds_parser.cpp"
    break;

  case 100:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4884 "ds_parser.cpp"
    break;

  case 101:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4890 "ds_parser.cpp"
    break;

  case 102:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4896 "ds_parser.cpp"
    break;

  case 103:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4902 "ds_parser.cpp"
    break;

  case 104:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4908 "ds_parser.cpp"
    break;

  case 105:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4914 "ds_parser.cpp"
    break;

  case 106:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4920 "ds_parser.cpp"
    break;

  case 107:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4926 "ds_parser.cpp"
    break;

  case 108:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4932 "ds_parser.cpp"
    break;

  case 109:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4938 "ds_parser.cpp"
    break;

  case 110:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4944 "ds_parser.cpp"
    break;

  case 111:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4950 "ds_parser.cpp"
    break;

  case 112:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4956 "ds_parser.cpp"
    break;

  case 113:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4962 "ds_parser.cpp"
    break;

  case 114:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4968 "ds_parser.cpp"
    break;

  case 115:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4974 "ds_parser.cpp"
    break;

  case 116:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4980 "ds_parser.cpp"
    break;

  case 117:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4986 "ds_parser.cpp"
    break;

  case 118:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4992 "ds_parser.cpp"
    break;

  case 119:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4998 "ds_parser.cpp"
    break;

  case 120:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5004 "ds_parser.cpp"
    break;

  case 121:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5010 "ds_parser.cpp"
    break;

  case 122:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5016 "ds_parser.cpp"
    break;

  case 123:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5022 "ds_parser.cpp"
    break;

  case 124:
#line 915 "ds_parser.ypp"
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
#line 5045 "ds_parser.cpp"
    break;

  case 125:
#line 936 "ds_parser.ypp"
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
#line 5085 "ds_parser.cpp"
    break;

  case 126:
#line 974 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5094 "ds_parser.cpp"
    break;

  case 127:
#line 981 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5103 "ds_parser.cpp"
    break;

  case 128:
#line 985 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5116 "ds_parser.cpp"
    break;

  case 129:
#line 996 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5122 "ds_parser.cpp"
    break;

  case 130:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5128 "ds_parser.cpp"
    break;

  case 131:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5134 "ds_parser.cpp"
    break;

  case 132:
#line 999 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5140 "ds_parser.cpp"
    break;

  case 133:
#line 1000 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5146 "ds_parser.cpp"
    break;

  case 134:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5152 "ds_parser.cpp"
    break;

  case 135:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5158 "ds_parser.cpp"
    break;

  case 136:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5164 "ds_parser.cpp"
    break;

  case 137:
#line 1004 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5170 "ds_parser.cpp"
    break;

  case 138:
#line 1005 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5176 "ds_parser.cpp"
    break;

  case 139:
#line 1006 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5182 "ds_parser.cpp"
    break;

  case 140:
#line 1007 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5188 "ds_parser.cpp"
    break;

  case 141:
#line 1008 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5194 "ds_parser.cpp"
    break;

  case 142:
#line 1009 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5200 "ds_parser.cpp"
    break;

  case 143:
#line 1010 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5206 "ds_parser.cpp"
    break;

  case 144:
#line 1011 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5212 "ds_parser.cpp"
    break;

  case 145:
#line 1012 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5218 "ds_parser.cpp"
    break;

  case 146:
#line 1013 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5224 "ds_parser.cpp"
    break;

  case 147:
#line 1014 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5230 "ds_parser.cpp"
    break;

  case 148:
#line 1018 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5240 "ds_parser.cpp"
    break;

  case 149:
#line 1023 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5251 "ds_parser.cpp"
    break;

  case 150:
#line 1029 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5259 "ds_parser.cpp"
    break;

  case 151:
#line 1035 "ds_parser.ypp"
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
#line 5285 "ds_parser.cpp"
    break;

  case 152:
#line 1056 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5293 "ds_parser.cpp"
    break;

  case 153:
#line 1062 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5299 "ds_parser.cpp"
    break;

  case 154:
#line 1063 "ds_parser.ypp"
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
#line 5315 "ds_parser.cpp"
    break;

  case 155:
#line 1074 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5321 "ds_parser.cpp"
    break;

  case 156:
#line 1078 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5329 "ds_parser.cpp"
    break;

  case 157:
#line 1084 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5337 "ds_parser.cpp"
    break;

  case 158:
#line 1087 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5345 "ds_parser.cpp"
    break;

  case 159:
#line 1090 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5354 "ds_parser.cpp"
    break;

  case 160:
#line 1094 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5362 "ds_parser.cpp"
    break;

  case 161:
#line 1100 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5368 "ds_parser.cpp"
    break;

  case 162:
#line 1104 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5374 "ds_parser.cpp"
    break;

  case 163:
#line 1108 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5382 "ds_parser.cpp"
    break;

  case 164:
#line 1111 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5390 "ds_parser.cpp"
    break;

  case 165:
#line 1114 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5400 "ds_parser.cpp"
    break;

  case 166:
#line 1119 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5408 "ds_parser.cpp"
    break;

  case 167:
#line 1122 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5418 "ds_parser.cpp"
    break;

  case 168:
#line 1130 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5426 "ds_parser.cpp"
    break;

  case 169:
#line 1133 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5436 "ds_parser.cpp"
    break;

  case 170:
#line 1138 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5444 "ds_parser.cpp"
    break;

  case 171:
#line 1141 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5454 "ds_parser.cpp"
    break;

  case 172:
#line 1149 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5462 "ds_parser.cpp"
    break;

  case 173:
#line 1155 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5468 "ds_parser.cpp"
    break;

  case 174:
#line 1156 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5474 "ds_parser.cpp"
    break;

  case 175:
#line 1160 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5480 "ds_parser.cpp"
    break;

  case 176:
#line 1161 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5486 "ds_parser.cpp"
    break;

  case 177:
#line 1165 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
        pLet->atInit = tokAt((yylsp[0]));
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
#line 5523 "ds_parser.cpp"
    break;

  case 178:
#line 1200 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5529 "ds_parser.cpp"
    break;

  case 179:
#line 1200 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5535 "ds_parser.cpp"
    break;

  case 180:
#line 1200 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5543 "ds_parser.cpp"
    break;

  case 181:
#line 1203 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5549 "ds_parser.cpp"
    break;

  case 182:
#line 1203 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5555 "ds_parser.cpp"
    break;

  case 183:
#line 1203 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5565 "ds_parser.cpp"
    break;

  case 184:
#line 1208 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5571 "ds_parser.cpp"
    break;

  case 185:
#line 1208 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5577 "ds_parser.cpp"
    break;

  case 186:
#line 1208 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5587 "ds_parser.cpp"
    break;

  case 187:
#line 1216 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5596 "ds_parser.cpp"
    break;

  case 188:
#line 1220 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5606 "ds_parser.cpp"
    break;

  case 189:
#line 1225 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5617 "ds_parser.cpp"
    break;

  case 190:
#line 1231 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5623 "ds_parser.cpp"
    break;

  case 191:
#line 1231 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5629 "ds_parser.cpp"
    break;

  case 192:
#line 1231 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5638 "ds_parser.cpp"
    break;

  case 193:
#line 1235 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5644 "ds_parser.cpp"
    break;

  case 194:
#line 1235 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5650 "ds_parser.cpp"
    break;

  case 195:
#line 1235 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5660 "ds_parser.cpp"
    break;

  case 196:
#line 1240 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5666 "ds_parser.cpp"
    break;

  case 197:
#line 1240 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5672 "ds_parser.cpp"
    break;

  case 198:
#line 1240 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5683 "ds_parser.cpp"
    break;

  case 199:
#line 1249 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5691 "ds_parser.cpp"
    break;

  case 200:
#line 1252 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5699 "ds_parser.cpp"
    break;

  case 201:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5705 "ds_parser.cpp"
    break;

  case 202:
#line 1259 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5717 "ds_parser.cpp"
    break;

  case 203:
#line 1266 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5730 "ds_parser.cpp"
    break;

  case 204:
#line 1277 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5736 "ds_parser.cpp"
    break;

  case 205:
#line 1278 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5742 "ds_parser.cpp"
    break;

  case 206:
#line 1279 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5748 "ds_parser.cpp"
    break;

  case 207:
#line 1283 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5754 "ds_parser.cpp"
    break;

  case 208:
#line 1284 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5760 "ds_parser.cpp"
    break;

  case 209:
#line 1285 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5766 "ds_parser.cpp"
    break;

  case 210:
#line 1286 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5772 "ds_parser.cpp"
    break;

  case 211:
#line 1290 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5782 "ds_parser.cpp"
    break;

  case 212:
#line 1295 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5792 "ds_parser.cpp"
    break;

  case 213:
#line 1303 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5798 "ds_parser.cpp"
    break;

  case 214:
#line 1304 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5804 "ds_parser.cpp"
    break;

  case 215:
#line 1308 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5814 "ds_parser.cpp"
    break;

  case 216:
#line 1314 "ds_parser.ypp"
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
#line 5879 "ds_parser.cpp"
    break;

  case 217:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5885 "ds_parser.cpp"
    break;

  case 218:
#line 1378 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5891 "ds_parser.cpp"
    break;

  case 219:
#line 1379 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5897 "ds_parser.cpp"
    break;

  case 220:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5903 "ds_parser.cpp"
    break;

  case 221:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5909 "ds_parser.cpp"
    break;

  case 222:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5915 "ds_parser.cpp"
    break;

  case 223:
#line 1386 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5921 "ds_parser.cpp"
    break;

  case 224:
#line 1387 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5927 "ds_parser.cpp"
    break;

  case 225:
#line 1388 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5933 "ds_parser.cpp"
    break;

  case 226:
#line 1389 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5939 "ds_parser.cpp"
    break;

  case 227:
#line 1390 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5945 "ds_parser.cpp"
    break;

  case 228:
#line 1391 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5951 "ds_parser.cpp"
    break;

  case 229:
#line 1392 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5957 "ds_parser.cpp"
    break;

  case 230:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5963 "ds_parser.cpp"
    break;

  case 231:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5969 "ds_parser.cpp"
    break;

  case 232:
#line 1395 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5975 "ds_parser.cpp"
    break;

  case 233:
#line 1396 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5981 "ds_parser.cpp"
    break;

  case 234:
#line 1397 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5987 "ds_parser.cpp"
    break;

  case 235:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5993 "ds_parser.cpp"
    break;

  case 236:
#line 1399 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5999 "ds_parser.cpp"
    break;

  case 237:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6005 "ds_parser.cpp"
    break;

  case 238:
#line 1401 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6011 "ds_parser.cpp"
    break;

  case 239:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6017 "ds_parser.cpp"
    break;

  case 240:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6023 "ds_parser.cpp"
    break;

  case 241:
#line 1410 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6035 "ds_parser.cpp"
    break;

  case 242:
#line 1421 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6045 "ds_parser.cpp"
    break;

  case 243:
#line 1426 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6057 "ds_parser.cpp"
    break;

  case 244:
#line 1436 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6066 "ds_parser.cpp"
    break;

  case 245:
#line 1440 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6072 "ds_parser.cpp"
    break;

  case 246:
#line 1440 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6078 "ds_parser.cpp"
    break;

  case 247:
#line 1440 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6090 "ds_parser.cpp"
    break;

  case 248:
#line 1447 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6096 "ds_parser.cpp"
    break;

  case 249:
#line 1447 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6102 "ds_parser.cpp"
    break;

  case 250:
#line 1447 "ds_parser.ypp"
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
#line 6118 "ds_parser.cpp"
    break;

  case 251:
#line 1461 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6127 "ds_parser.cpp"
    break;

  case 252:
#line 1465 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6133 "ds_parser.cpp"
    break;

  case 253:
#line 1465 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6139 "ds_parser.cpp"
    break;

  case 254:
#line 1465 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6149 "ds_parser.cpp"
    break;

  case 255:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6155 "ds_parser.cpp"
    break;

  case 256:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6161 "ds_parser.cpp"
    break;

  case 257:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6167 "ds_parser.cpp"
    break;

  case 258:
#line 1476 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6173 "ds_parser.cpp"
    break;

  case 259:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6179 "ds_parser.cpp"
    break;

  case 260:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6185 "ds_parser.cpp"
    break;

  case 261:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6191 "ds_parser.cpp"
    break;

  case 262:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6197 "ds_parser.cpp"
    break;

  case 263:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6203 "ds_parser.cpp"
    break;

  case 264:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6209 "ds_parser.cpp"
    break;

  case 265:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6215 "ds_parser.cpp"
    break;

  case 266:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6221 "ds_parser.cpp"
    break;

  case 267:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6227 "ds_parser.cpp"
    break;

  case 268:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6233 "ds_parser.cpp"
    break;

  case 269:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6239 "ds_parser.cpp"
    break;

  case 270:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6245 "ds_parser.cpp"
    break;

  case 271:
#line 1489 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6251 "ds_parser.cpp"
    break;

  case 272:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6257 "ds_parser.cpp"
    break;

  case 273:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6263 "ds_parser.cpp"
    break;

  case 274:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6269 "ds_parser.cpp"
    break;

  case 275:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6275 "ds_parser.cpp"
    break;

  case 276:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6281 "ds_parser.cpp"
    break;

  case 277:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6287 "ds_parser.cpp"
    break;

  case 278:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6293 "ds_parser.cpp"
    break;

  case 279:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6299 "ds_parser.cpp"
    break;

  case 280:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6305 "ds_parser.cpp"
    break;

  case 281:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6311 "ds_parser.cpp"
    break;

  case 282:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6317 "ds_parser.cpp"
    break;

  case 283:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6323 "ds_parser.cpp"
    break;

  case 284:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6329 "ds_parser.cpp"
    break;

  case 285:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6335 "ds_parser.cpp"
    break;

  case 286:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6341 "ds_parser.cpp"
    break;

  case 287:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6347 "ds_parser.cpp"
    break;

  case 288:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6353 "ds_parser.cpp"
    break;

  case 289:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6359 "ds_parser.cpp"
    break;

  case 290:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6365 "ds_parser.cpp"
    break;

  case 291:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6371 "ds_parser.cpp"
    break;

  case 292:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6377 "ds_parser.cpp"
    break;

  case 293:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6383 "ds_parser.cpp"
    break;

  case 294:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6389 "ds_parser.cpp"
    break;

  case 295:
#line 1513 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6398 "ds_parser.cpp"
    break;

  case 296:
#line 1517 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6407 "ds_parser.cpp"
    break;

  case 297:
#line 1521 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6415 "ds_parser.cpp"
    break;

  case 298:
#line 1524 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6423 "ds_parser.cpp"
    break;

  case 299:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6429 "ds_parser.cpp"
    break;

  case 300:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6435 "ds_parser.cpp"
    break;

  case 301:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6441 "ds_parser.cpp"
    break;

  case 302:
#line 1530 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6455 "ds_parser.cpp"
    break;

  case 303:
#line 1539 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6469 "ds_parser.cpp"
    break;

  case 304:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6475 "ds_parser.cpp"
    break;

  case 305:
#line 1549 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6483 "ds_parser.cpp"
    break;

  case 306:
#line 1552 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6489 "ds_parser.cpp"
    break;

  case 307:
#line 1552 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6495 "ds_parser.cpp"
    break;

  case 308:
#line 1552 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6503 "ds_parser.cpp"
    break;

  case 309:
#line 1555 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6512 "ds_parser.cpp"
    break;

  case 310:
#line 1559 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6521 "ds_parser.cpp"
    break;

  case 311:
#line 1563 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6530 "ds_parser.cpp"
    break;

  case 312:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6536 "ds_parser.cpp"
    break;

  case 313:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6542 "ds_parser.cpp"
    break;

  case 314:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6548 "ds_parser.cpp"
    break;

  case 315:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6554 "ds_parser.cpp"
    break;

  case 316:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6560 "ds_parser.cpp"
    break;

  case 317:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6566 "ds_parser.cpp"
    break;

  case 318:
#line 1573 "ds_parser.ypp"
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
#line 6581 "ds_parser.cpp"
    break;

  case 319:
#line 1583 "ds_parser.ypp"
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
#line 6596 "ds_parser.cpp"
    break;

  case 320:
#line 1593 "ds_parser.ypp"
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
#line 6653 "ds_parser.cpp"
    break;

  case 321:
#line 1648 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6659 "ds_parser.cpp"
    break;

  case 322:
#line 1649 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6665 "ds_parser.cpp"
    break;

  case 323:
#line 1653 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6671 "ds_parser.cpp"
    break;

  case 324:
#line 1654 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6677 "ds_parser.cpp"
    break;

  case 325:
#line 1658 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6687 "ds_parser.cpp"
    break;

  case 326:
#line 1666 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6695 "ds_parser.cpp"
    break;

  case 327:
#line 1669 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6704 "ds_parser.cpp"
    break;

  case 328:
#line 1674 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6710 "ds_parser.cpp"
    break;

  case 329:
#line 1674 "ds_parser.ypp"
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
#line 6759 "ds_parser.cpp"
    break;

  case 330:
#line 1718 "ds_parser.ypp"
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
#line 6819 "ds_parser.cpp"
    break;

  case 331:
#line 1776 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6829 "ds_parser.cpp"
    break;

  case 332:
#line 1781 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6839 "ds_parser.cpp"
    break;

  case 333:
#line 1789 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6845 "ds_parser.cpp"
    break;

  case 334:
#line 1790 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6851 "ds_parser.cpp"
    break;

  case 335:
#line 1794 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6859 "ds_parser.cpp"
    break;

  case 336:
#line 1797 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6870 "ds_parser.cpp"
    break;

  case 337:
#line 1806 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6876 "ds_parser.cpp"
    break;

  case 338:
#line 1807 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6882 "ds_parser.cpp"
    break;

  case 339:
#line 1811 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6893 "ds_parser.cpp"
    break;

  case 340:
#line 1820 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6899 "ds_parser.cpp"
    break;

  case 341:
#line 1821 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6905 "ds_parser.cpp"
    break;

  case 342:
#line 1826 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6911 "ds_parser.cpp"
    break;

  case 343:
#line 1827 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6917 "ds_parser.cpp"
    break;

  case 344:
#line 1831 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6928 "ds_parser.cpp"
    break;

  case 345:
#line 1837 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6939 "ds_parser.cpp"
    break;

  case 346:
#line 1843 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6947 "ds_parser.cpp"
    break;

  case 347:
#line 1846 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6956 "ds_parser.cpp"
    break;

  case 348:
#line 1850 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6967 "ds_parser.cpp"
    break;

  case 349:
#line 1856 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6978 "ds_parser.cpp"
    break;

  case 350:
#line 1865 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6984 "ds_parser.cpp"
    break;

  case 351:
#line 1866 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6990 "ds_parser.cpp"
    break;

  case 352:
#line 1867 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6996 "ds_parser.cpp"
    break;

  case 353:
#line 1871 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7002 "ds_parser.cpp"
    break;

  case 354:
#line 1872 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7008 "ds_parser.cpp"
    break;

  case 355:
#line 1876 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7016 "ds_parser.cpp"
    break;

  case 356:
#line 1879 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7026 "ds_parser.cpp"
    break;

  case 357:
#line 1884 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7036 "ds_parser.cpp"
    break;

  case 358:
#line 1889 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7049 "ds_parser.cpp"
    break;

  case 359:
#line 1897 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7062 "ds_parser.cpp"
    break;

  case 360:
#line 1908 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7070 "ds_parser.cpp"
    break;

  case 361:
#line 1911 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7079 "ds_parser.cpp"
    break;

  case 362:
#line 1918 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7085 "ds_parser.cpp"
    break;

  case 363:
#line 1919 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7091 "ds_parser.cpp"
    break;

  case 364:
#line 1923 "ds_parser.ypp"
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
#line 7123 "ds_parser.cpp"
    break;

  case 365:
#line 1950 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7132 "ds_parser.cpp"
    break;

  case 366:
#line 1953 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7143 "ds_parser.cpp"
    break;

  case 367:
#line 1962 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7151 "ds_parser.cpp"
    break;

  case 368:
#line 1965 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7165 "ds_parser.cpp"
    break;

  case 369:
#line 1974 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7179 "ds_parser.cpp"
    break;

  case 370:
#line 1987 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7193 "ds_parser.cpp"
    break;

  case 374:
#line 2008 "ds_parser.ypp"
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
#line 7209 "ds_parser.cpp"
    break;

  case 375:
#line 2019 "ds_parser.ypp"
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
#line 7226 "ds_parser.cpp"
    break;

  case 376:
#line 2034 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7232 "ds_parser.cpp"
    break;

  case 377:
#line 2035 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7238 "ds_parser.cpp"
    break;

  case 378:
#line 2039 "ds_parser.ypp"
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
#line 7277 "ds_parser.cpp"
    break;

  case 379:
#line 2076 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7283 "ds_parser.cpp"
    break;

  case 380:
#line 2077 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7289 "ds_parser.cpp"
    break;

  case 381:
#line 2081 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7295 "ds_parser.cpp"
    break;

  case 382:
#line 2081 "ds_parser.ypp"
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
#line 7381 "ds_parser.cpp"
    break;

  case 383:
#line 2165 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7393 "ds_parser.cpp"
    break;

  case 384:
#line 2172 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7404 "ds_parser.cpp"
    break;

  case 385:
#line 2181 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7410 "ds_parser.cpp"
    break;

  case 386:
#line 2182 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7416 "ds_parser.cpp"
    break;

  case 387:
#line 2183 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7422 "ds_parser.cpp"
    break;

  case 388:
#line 2184 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7428 "ds_parser.cpp"
    break;

  case 389:
#line 2185 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7434 "ds_parser.cpp"
    break;

  case 390:
#line 2186 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7440 "ds_parser.cpp"
    break;

  case 391:
#line 2187 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7446 "ds_parser.cpp"
    break;

  case 392:
#line 2188 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7452 "ds_parser.cpp"
    break;

  case 393:
#line 2189 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7458 "ds_parser.cpp"
    break;

  case 394:
#line 2190 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7464 "ds_parser.cpp"
    break;

  case 395:
#line 2191 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7470 "ds_parser.cpp"
    break;

  case 396:
#line 2192 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7476 "ds_parser.cpp"
    break;

  case 397:
#line 2193 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7482 "ds_parser.cpp"
    break;

  case 398:
#line 2194 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7488 "ds_parser.cpp"
    break;

  case 399:
#line 2195 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7494 "ds_parser.cpp"
    break;

  case 400:
#line 2196 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7500 "ds_parser.cpp"
    break;

  case 401:
#line 2197 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7506 "ds_parser.cpp"
    break;

  case 402:
#line 2198 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7512 "ds_parser.cpp"
    break;

  case 403:
#line 2199 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7518 "ds_parser.cpp"
    break;

  case 404:
#line 2200 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7524 "ds_parser.cpp"
    break;

  case 405:
#line 2201 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7530 "ds_parser.cpp"
    break;

  case 406:
#line 2202 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7536 "ds_parser.cpp"
    break;

  case 407:
#line 2203 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7542 "ds_parser.cpp"
    break;

  case 408:
#line 2204 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7548 "ds_parser.cpp"
    break;

  case 409:
#line 2205 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7554 "ds_parser.cpp"
    break;

  case 410:
#line 2209 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7560 "ds_parser.cpp"
    break;

  case 411:
#line 2210 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7566 "ds_parser.cpp"
    break;

  case 412:
#line 2211 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7572 "ds_parser.cpp"
    break;

  case 413:
#line 2212 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7578 "ds_parser.cpp"
    break;

  case 414:
#line 2213 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7584 "ds_parser.cpp"
    break;

  case 415:
#line 2214 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7590 "ds_parser.cpp"
    break;

  case 416:
#line 2218 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7603 "ds_parser.cpp"
    break;

  case 417:
#line 2229 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7612 "ds_parser.cpp"
    break;

  case 418:
#line 2233 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7624 "ds_parser.cpp"
    break;

  case 419:
#line 2243 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7636 "ds_parser.cpp"
    break;

  case 420:
#line 2250 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7647 "ds_parser.cpp"
    break;

  case 421:
#line 2259 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7653 "ds_parser.cpp"
    break;

  case 422:
#line 2259 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7659 "ds_parser.cpp"
    break;

  case 423:
#line 2259 "ds_parser.ypp"
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
#line 7674 "ds_parser.cpp"
    break;

  case 424:
#line 2272 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7680 "ds_parser.cpp"
    break;

  case 425:
#line 2273 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7686 "ds_parser.cpp"
    break;

  case 426:
#line 2274 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7692 "ds_parser.cpp"
    break;

  case 427:
#line 2275 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7698 "ds_parser.cpp"
    break;

  case 428:
#line 2276 "ds_parser.ypp"
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
#line 7717 "ds_parser.cpp"
    break;

  case 429:
#line 2290 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7728 "ds_parser.cpp"
    break;

  case 430:
#line 2296 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7737 "ds_parser.cpp"
    break;

  case 431:
#line 2300 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7746 "ds_parser.cpp"
    break;

  case 432:
#line 2304 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7756 "ds_parser.cpp"
    break;

  case 433:
#line 2309 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7766 "ds_parser.cpp"
    break;

  case 434:
#line 2314 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7776 "ds_parser.cpp"
    break;

  case 435:
#line 2319 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7786 "ds_parser.cpp"
    break;

  case 436:
#line 2324 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7795 "ds_parser.cpp"
    break;

  case 437:
#line 2328 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7804 "ds_parser.cpp"
    break;

  case 438:
#line 2332 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7814 "ds_parser.cpp"
    break;

  case 439:
#line 2337 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7823 "ds_parser.cpp"
    break;

  case 440:
#line 2341 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7833 "ds_parser.cpp"
    break;

  case 441:
#line 2346 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7839 "ds_parser.cpp"
    break;

  case 442:
#line 2346 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7845 "ds_parser.cpp"
    break;

  case 443:
#line 2346 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7856 "ds_parser.cpp"
    break;

  case 444:
#line 2352 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7868 "ds_parser.cpp"
    break;

  case 445:
#line 2359 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7874 "ds_parser.cpp"
    break;

  case 446:
#line 2359 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7880 "ds_parser.cpp"
    break;

  case 447:
#line 2359 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7890 "ds_parser.cpp"
    break;

  case 448:
#line 2364 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7896 "ds_parser.cpp"
    break;

  case 449:
#line 2364 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7902 "ds_parser.cpp"
    break;

  case 450:
#line 2364 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7913 "ds_parser.cpp"
    break;

  case 451:
#line 2370 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7919 "ds_parser.cpp"
    break;

  case 452:
#line 2370 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7925 "ds_parser.cpp"
    break;

  case 453:
#line 2370 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7935 "ds_parser.cpp"
    break;

  case 454:
#line 2375 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7944 "ds_parser.cpp"
    break;

  case 455:
#line 2379 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7950 "ds_parser.cpp"
    break;

  case 456:
#line 2379 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7956 "ds_parser.cpp"
    break;

  case 457:
#line 2379 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7966 "ds_parser.cpp"
    break;

  case 458:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7972 "ds_parser.cpp"
    break;

  case 459:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7978 "ds_parser.cpp"
    break;

  case 460:
#line 2384 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7992 "ds_parser.cpp"
    break;

  case 461:
#line 2393 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8001 "ds_parser.cpp"
    break;

  case 462:
#line 2397 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8007 "ds_parser.cpp"
    break;

  case 463:
#line 2397 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8013 "ds_parser.cpp"
    break;

  case 464:
#line 2397 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8023 "ds_parser.cpp"
    break;

  case 465:
#line 2402 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8029 "ds_parser.cpp"
    break;

  case 466:
#line 2402 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8035 "ds_parser.cpp"
    break;

  case 467:
#line 2402 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8049 "ds_parser.cpp"
    break;

  case 468:
#line 2411 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8058 "ds_parser.cpp"
    break;

  case 469:
#line 2415 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8064 "ds_parser.cpp"
    break;

  case 470:
#line 2415 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8070 "ds_parser.cpp"
    break;

  case 471:
#line 2415 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8080 "ds_parser.cpp"
    break;

  case 472:
#line 2420 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8086 "ds_parser.cpp"
    break;

  case 473:
#line 2420 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8092 "ds_parser.cpp"
    break;

  case 474:
#line 2420 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8106 "ds_parser.cpp"
    break;

  case 475:
#line 2429 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8112 "ds_parser.cpp"
    break;

  case 476:
#line 2429 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8118 "ds_parser.cpp"
    break;

  case 477:
#line 2429 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8129 "ds_parser.cpp"
    break;

  case 478:
#line 2435 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8135 "ds_parser.cpp"
    break;

  case 479:
#line 2435 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8141 "ds_parser.cpp"
    break;

  case 480:
#line 2435 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8152 "ds_parser.cpp"
    break;

  case 481:
#line 2444 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8158 "ds_parser.cpp"
    break;

  case 482:
#line 2444 "ds_parser.ypp"
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
#line 8175 "ds_parser.cpp"
    break;

  case 483:
#line 2459 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8181 "ds_parser.cpp"
    break;

  case 484:
#line 2459 "ds_parser.ypp"
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
#line 8202 "ds_parser.cpp"
    break;

  case 485:
#line 2479 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8208 "ds_parser.cpp"
    break;

  case 486:
#line 2480 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8214 "ds_parser.cpp"
    break;

  case 487:
#line 2481 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8220 "ds_parser.cpp"
    break;

  case 488:
#line 2482 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8226 "ds_parser.cpp"
    break;

  case 489:
#line 2486 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8238 "ds_parser.cpp"
    break;

  case 490:
#line 2493 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8250 "ds_parser.cpp"
    break;

  case 491:
#line 2500 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8261 "ds_parser.cpp"
    break;

  case 492:
#line 2506 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8272 "ds_parser.cpp"
    break;

  case 493:
#line 2515 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8282 "ds_parser.cpp"
    break;

  case 494:
#line 2520 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8291 "ds_parser.cpp"
    break;

  case 495:
#line 2527 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8297 "ds_parser.cpp"
    break;

  case 496:
#line 2528 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8303 "ds_parser.cpp"
    break;

  case 497:
#line 2532 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8314 "ds_parser.cpp"
    break;

  case 498:
#line 2538 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8326 "ds_parser.cpp"
    break;

  case 499:
#line 2545 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8339 "ds_parser.cpp"
    break;

  case 500:
#line 2553 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8351 "ds_parser.cpp"
    break;

  case 501:
#line 2560 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8364 "ds_parser.cpp"
    break;

  case 502:
#line 2568 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8378 "ds_parser.cpp"
    break;

  case 503:
#line 2580 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8386 "ds_parser.cpp"
    break;

  case 504:
#line 2583 "ds_parser.ypp"
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
#line 8402 "ds_parser.cpp"
    break;

  case 505:
#line 2597 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8413 "ds_parser.cpp"
    break;

  case 506:
#line 2606 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8419 "ds_parser.cpp"
    break;

  case 507:
#line 2607 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8425 "ds_parser.cpp"
    break;

  case 508:
#line 2611 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8435 "ds_parser.cpp"
    break;

  case 509:
#line 2616 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8444 "ds_parser.cpp"
    break;

  case 510:
#line 2623 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8454 "ds_parser.cpp"
    break;

  case 511:
#line 2628 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8467 "ds_parser.cpp"
    break;

  case 512:
#line 2639 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8477 "ds_parser.cpp"
    break;

  case 513:
#line 2644 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8486 "ds_parser.cpp"
    break;

  case 514:
#line 2651 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8500 "ds_parser.cpp"
    break;

  case 515:
#line 2663 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8506 "ds_parser.cpp"
    break;

  case 516:
#line 2664 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8512 "ds_parser.cpp"
    break;

  case 517:
#line 2668 "ds_parser.ypp"
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
#line 8535 "ds_parser.cpp"
    break;

  case 518:
#line 2686 "ds_parser.ypp"
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
#line 8558 "ds_parser.cpp"
    break;


#line 8562 "ds_parser.cpp"

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
#line 2706 "ds_parser.ypp"


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
