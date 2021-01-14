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
#define YYLAST   7294

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  185
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  172
/* YYNRULES -- Number of rules.  */
#define YYNRULES  527
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  936

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
       0,   443,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   459,   474,   475,   479,   483,
     488,   497,   505,   521,   526,   534,   534,   564,   586,   590,
     593,   599,   608,   629,   652,   653,   657,   661,   662,   666,
     669,   675,   681,   684,   690,   691,   695,   696,   697,   706,
     707,   711,   720,   736,   744,   754,   763,   764,   765,   766,
     767,   768,   772,   777,   785,   786,   787,   788,   789,   790,
     791,   792,   798,   811,   827,   828,   832,   839,   851,   855,
     862,   863,   867,   868,   869,   873,   878,   885,   889,   890,
     891,   892,   893,   894,   895,   896,   897,   898,   899,   900,
     901,   902,   903,   904,   905,   906,   907,   908,   909,   910,
     911,   912,   913,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,   929,   930,
     934,   956,   994,  1001,  1005,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1038,  1043,  1049,  1055,  1089,  1092,
    1098,  1099,  1110,  1114,  1120,  1123,  1126,  1130,  1136,  1140,
    1144,  1147,  1150,  1155,  1158,  1166,  1169,  1174,  1177,  1185,
    1191,  1192,  1196,  1230,  1230,  1230,  1233,  1233,  1233,  1238,
    1238,  1238,  1246,  1246,  1246,  1252,  1262,  1273,  1288,  1291,
    1297,  1298,  1305,  1316,  1317,  1318,  1322,  1323,  1324,  1325,
    1329,  1334,  1342,  1343,  1347,  1352,  1416,  1417,  1418,  1419,
    1420,  1421,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,
    1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1447,  1448,  1449,  1450,  1454,  1465,  1470,  1480,  1484,
    1484,  1484,  1491,  1491,  1491,  1505,  1509,  1509,  1509,  1516,
    1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,
    1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,  1535,  1536,
    1537,  1538,  1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,
    1547,  1548,  1549,  1550,  1551,  1552,  1553,  1554,  1555,  1556,
    1557,  1558,  1559,  1563,  1567,  1570,  1573,  1574,  1575,  1576,
    1585,  1594,  1595,  1598,  1598,  1598,  1601,  1605,  1609,  1613,
    1614,  1615,  1616,  1617,  1618,  1619,  1620,  1636,  1652,  1707,
    1708,  1712,  1713,  1717,  1725,  1728,  1733,  1732,  1777,  1833,
    1842,  1847,  1855,  1856,  1860,  1863,  1872,  1873,  1877,  1886,
    1887,  1892,  1893,  1897,  1903,  1909,  1912,  1916,  1922,  1931,
    1932,  1933,  1937,  1938,  1942,  1945,  1950,  1955,  1963,  1974,
    1977,  1984,  1985,  1989,  2016,  2016,  2044,  2047,  2056,  2069,
    2081,  2082,  2086,  2090,  2101,  2116,  2117,  2121,  2158,  2159,
    2163,  2163,  2247,  2254,  2263,  2264,  2265,  2266,  2267,  2268,
    2269,  2270,  2271,  2272,  2273,  2274,  2275,  2276,  2277,  2278,
    2279,  2280,  2281,  2282,  2283,  2284,  2285,  2286,  2287,  2291,
    2292,  2293,  2294,  2295,  2296,  2300,  2311,  2315,  2325,  2332,
    2341,  2341,  2341,  2354,  2355,  2356,  2357,  2358,  2372,  2378,
    2382,  2386,  2391,  2396,  2401,  2406,  2410,  2414,  2419,  2423,
    2428,  2428,  2428,  2434,  2441,  2441,  2441,  2446,  2446,  2446,
    2452,  2452,  2452,  2457,  2461,  2461,  2461,  2466,  2466,  2466,
    2475,  2479,  2479,  2479,  2484,  2484,  2484,  2493,  2497,  2497,
    2497,  2502,  2502,  2502,  2511,  2511,  2511,  2517,  2517,  2517,
    2526,  2526,  2541,  2541,  2561,  2562,  2563,  2564,  2568,  2575,
    2582,  2588,  2597,  2602,  2609,  2610,  2614,  2620,  2627,  2635,
    2642,  2650,  2662,  2665,  2679,  2688,  2689,  2693,  2698,  2705,
    2710,  2720,  2725,  2732,  2744,  2745,  2749,  2767
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
  "optional_shared", "global_let", "$@19", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "class_or_struct",
  "structure_declaration", "$@20", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@21", "$@22", "type_declaration", "$@23",
  "$@24", "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32",
  "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41",
  "$@42", "$@43", "$@44", "$@45", "$@46", "variant_alias_declaration",
  "$@47", "bitfield_alias_declaration", "$@48", "make_decl",
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

#define YYPACT_NINF -603

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-603)))

#define YYTABLE_NINF -482

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -603,    26,  -603,  -603,   -22,    42,    31,    60,  -122,  -603,
      72,  -603,  -603,    14,  -603,  -603,  -603,  -603,  -603,   350,
    -603,    67,  -603,  -603,  -603,  -603,  -603,  -603,    73,  -603,
      77,   106,   124,  -603,  -603,    24,  -603,   -84,   132,    67,
     139,   146,  -603,   156,   151,   126,  -603,    70,  -603,  -603,
    -603,    45,   165,  -603,   137,    22,   -22,   190,    42,   198,
    -603,   206,   215,  -603,   248,  -603,   213,  -603,   -88,  -603,
     184,   189,  -603,   231,   -22,    14,  -603,  7126,   343,   344,
    -603,   212,   209,  -603,   245,  -603,  -603,   260,  -603,  -603,
    -603,  -603,  -603,   263,   112,  -603,  -603,  -603,  -603,   360,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,   234,   -78,
    7041,  -603,  -603,   273,   275,  -603,   -77,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
     -91,   274,  -603,  -603,    82,  -603,   247,   -73,  -603,  -603,
     210,  -603,   -31,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
     -57,  -603,  -603,   -89,  -603,   269,   272,   276,   279,  -603,
    -603,  -603,   255,  -603,  -603,  -603,  -603,  -603,   281,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,   282,  -603,
    -603,  -603,   283,   284,  -603,  -603,  -603,  -603,   285,   298,
    -603,  -603,  -603,  -603,  -603,   317,  -603,   252,   304,  -603,
     280,  -603,   -22,  -603,    -5,  -603,    62,  7041,  -603,  1183,
    -603,  -603,  -603,  -603,   313,  7041,  -603,   -20,  -603,  -603,
     112,  -603,   -14,  4464,  -603,  -603,  -603,  -603,  -603,   315,
    -603,   179,   185,   193,  -603,  -603,  -603,  -603,  -603,  -603,
     428,  -603,  -603,    -3,  2849,  -603,  -603,    46,  7041,    47,
      83,   260,  -603,   -71,  -603,   328,  3198,  -603,  4464,  -603,
    -603,   260,  -603,  -603,  6957,   288,   309,  1989,  -603,  -603,
     209,  4464,   299,  4464,   310,   320,   302,  -603,   294,   322,
     345,  2986,   209,  -603,   327,  2146,  4464,  4464,   205,   205,
    6789,  6873,  4464,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    4464,  4464,  4464,    82,  4464,  4464,  4464,  -603,   308,  -603,
     453,  -603,  -603,   314,   316,  4464,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,   -59,   318,  -603,   319,   321,  -603,
    -603,  -603,   260,  -603,  -603,  -603,  -603,   329,  -603,  -603,
     -90,  -603,  -603,  -603,  -603,  -603,  6005,   326,  -603,  -603,
    -603,  -603,  -603,    10,  -603,   188,  -603,  -603,  -603,  2283,
    -603,  -603,   672,  7041,  7041,  7041,  7041,   312,   273,  7041,
     212,  7041,   212,  7041,   212,  7125,   275,  -603,  -603,  -603,
     330,  -603,  -603,  5662,  -603,  -603,  3198,  -603,  -603,   331,
    -603,  -603,  -603,  7041,  -603,  2283,  -603,   291,     2,   379,
    -603,    82,  -603,  2283,  -603,  -603,   373,   958,   481,  6325,
    4464,   291,  -603,  -603,  4464,  -603,  -603,   342,   374,  6325,
    -603,  7041,  2283,  -603,  4553,   157,   157,   333,  -603,  -603,
     260,  1524,   260,  1679,  6103,  -603,   -35,   278,   157,   157,
     -47,  -603,   157,   157,  5117,    40,   332,  -603,  -603,   291,
    -603,  2420,  -603,  -603,  -603,  -603,   376,   205,  -603,    17,
     371,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,
    4464,  -603,  -603,  4464,  4464,  4464,  4464,  4464,  4464,   372,
    2575,  4464,   375,  4464,  4464,  4464,  4464,  4464,  4464,  4464,
    4464,  4464,  4464,  4464,  4464,  4464,  4464,  2712,  3123,  4464,
    4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,   377,
    4464,  3260,    14,  -603,   515,   482,   346,   428,  -603,  2283,
    -603,  4651,  -603,    43,   501,   691,   984,  -603,    99,  1617,
     274,  1772,   274,  1931,   274,    51,  -603,   111,  3198,   123,
    -603,  -603,  -603,  2087,  -603,  5906,  -603,  4464,  3415,  3570,
    7041,  -603,  4749,  4464,  4464,  -603,   209,  5194,  -603,  7041,
    7041,  5273,  7041,  -603,  -603,  2089,  -603,  4847,  -603,  -603,
       4,   205,   -40,  1834,  3707,  6103,   383,   -19,   347,   384,
    -603,  -603,  -123,    12,  3862,   -19,   135,  4464,  4464,   354,
    -603,  4464,   380,   382,  -603,   199,  -603,  -603,   303,   386,
    -603,    -4,  6325,   -46,   212,  -603,   385,  -603,  -603,  6325,
    6325,  6325,  6325,  6325,  6325,  6325,  6325,  1462,  1462,   870,
    6325,  6325,   870,   466,   466,   361,  4464,  4464,  6325,    19,
    -603,  5741,   -66,   -66,  6325,  1462,  1462,  6325,  6325,  6547,
    6399,  6473,  6325,  6325,  6325,  4464,  4464,  6325,   396,  6177,
    6571,  6645,  6669,   870,   870,   596,   596,    19,    19,    19,
    -603,   540,  5819,  -603,     5,    87,   224,  -603,   260,  -603,
    -603,  4945,  -603,  -603,  7041,  -603,  -603,  -603,   398,  -603,
     388,  -603,   389,  -603,   390,  7041,  -603,  7125,  -603,   275,
     428,  4464,   -67,  -603,    37,   400,  5350,  2221,  -603,  -603,
    -603,  2358,  2513,  -603,  2650,   376,  -603,  4464,  -603,  4464,
    4464,    -6,   401,   386,   397,   408,  4464,   409,  4464,  4464,
     386,   367,   410,  6325,  -603,  -603,  6251,  -603,   405,  7041,
     212,  1366,   209,  -603,  -603,  4464,  -603,    -9,    88,  4464,
    -603,   411,   415,   416,   417,  -603,   101,   274,  -603,  4017,
     489,  6325,  -603,   489,  6325,  -603,  4464,  -603,  -603,  -603,
     381,  -603,    45,  -603,  -603,  -603,  2787,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  3198,  -603,  -603,  -603,
    6325,   209,  -603,   125,  -603,  -603,  -603,  -603,  -603,   387,
     -62,  6325,  6325,   -19,   412,    95,   383,   418,  -603,  6325,
    -603,  -603,   -61,   -19,   426,  -603,  -603,  -603,  2924,   274,
    -603,  -603,   291,   419,  6325,  -603,  -603,  -603,  -603,   -46,
     432,   -79,  7041,  -603,    58,  6325,  -603,  -603,    45,  -603,
    -603,  -603,  -603,  -603,  -603,  4464,   422,  -603,  4464,  4464,
    4464,  4172,  4464,   435,   438,  4464,  4464,  -603,  4464,   369,
    -603,  -603,   394,   303,  -603,  -603,  -603,  4327,  -603,  -603,
    3061,  -603,   393,  -603,  5429,   406,   870,   870,   870,  -603,
    5506,  5043,   445,  -603,  6325,  6325,  5043,   446,    82,  -603,
    -603,  4464,  6325,  -603,  -603,  -603,  4464,  -603,   544,   447,
    -603,   448,  -603,  -603,    82,  6325,  -603,  5585,  4464,   457,
     458,  -603,  -603,  6325,  -603,  -603
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   180,     0,     0,     0,     0,     0,   181,
       0,   492,   490,     0,    14,     3,    10,     9,     8,     0,
       7,   371,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,   371,
       0,     0,    75,     0,   160,    76,    78,     0,    74,   389,
     388,     0,     0,   372,   374,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   376,     0,   380,     0,    15,
       0,     0,   162,     0,     0,     0,    81,     0,     0,     0,
      87,    82,     0,   130,   385,   390,   369,     0,    68,    69,
      66,    67,    65,     0,     0,    64,    73,    40,    38,    34,
      31,    30,   419,   422,   420,   423,   421,   424,     0,     0,
       0,   382,   381,     0,     0,   161,     0,    79,    90,    91,
      93,    92,    94,    95,    96,    97,   122,   123,   120,   121,
     113,   124,   125,   114,   111,   112,   126,   127,   128,   129,
     101,   102,   103,    98,    99,   100,   116,   117,   115,   109,
     110,   105,   104,   106,   107,   108,    89,    88,   118,   119,
     329,    85,   154,   132,     0,   387,     0,     0,   392,   375,
     362,    16,     0,    60,    61,    58,    59,    57,    56,    62,
       0,    33,   376,     0,   383,     0,     0,     0,     0,   394,
     414,   395,   426,   396,   400,   401,   402,   403,   418,   407,
     408,   409,   410,   411,   412,   413,   415,   416,   463,   399,
     406,   417,   470,   477,   397,   404,   398,   405,     0,     0,
     425,   433,   436,   434,   435,     0,   428,     0,     0,   349,
       0,    77,     0,    83,     0,   342,     0,     0,   131,     0,
     386,   334,   373,   370,     0,     0,   363,     0,    17,    18,
       0,    71,     0,     0,   377,   454,   457,   460,   450,     0,
     430,   464,   471,   478,   484,   487,   441,   446,   440,   453,
       0,   449,   443,     0,     0,   379,   445,     0,     0,     0,
       0,     0,   340,   353,    84,   329,    86,   156,     0,    49,
      50,     0,   265,   266,     0,     0,     0,     0,   259,   168,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,   418,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   216,   218,   217,   219,   220,   221,    19,
       0,     0,     0,     0,     0,     0,     0,   203,   204,   135,
     133,   263,   262,     0,     0,     0,   149,   144,   142,   141,
     143,   214,   155,   136,   260,     0,   322,     0,     0,   147,
     148,   150,     0,   140,   321,   320,   319,    80,   325,   261,
       0,   137,   324,   323,   301,   267,   222,     0,   264,   494,
     495,   496,   497,   329,   393,     0,   360,   361,   359,     0,
      63,   384,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    82,     0,    82,     0,     0,   448,   442,   444,
       0,   447,   438,     0,   429,   493,   348,   491,   350,     0,
     341,   352,   351,     0,   354,     0,   343,     0,     0,   164,
     167,     0,   192,     0,   170,   173,     0,   222,     0,   163,
       0,     0,   183,   186,     0,   153,   189,     0,     0,    43,
      53,     0,     0,   177,   222,   293,   294,   204,   158,   159,
       0,   504,     0,     0,     0,   521,     0,     0,   271,   270,
     306,    25,   269,   268,     0,   205,     0,   151,   152,     0,
     328,     0,   139,   145,   146,   182,   212,     0,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   295,   296,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   391,     0,   331,     0,   359,   364,     0,
     368,   222,   378,     0,     0,     0,     0,   427,     0,     0,
      85,     0,    85,     0,    85,   160,   346,     0,   344,     0,
     439,   437,   330,   355,   358,   357,    54,     0,     0,     0,
       0,   174,   222,     0,     0,   171,     0,     0,    55,     0,
       0,     0,     0,    41,    42,     0,   178,   222,   175,   205,
       0,     0,   160,     0,     0,   512,   502,   504,     0,   515,
     516,   517,     0,     0,     0,   504,     0,     0,     0,     0,
      22,     0,    20,     0,   297,   249,   248,   154,    46,     0,
     302,     0,   198,     0,    82,   157,     0,   316,   317,   232,
     233,   235,   234,   236,   226,   227,   228,   272,   273,   285,
     237,   238,   286,   283,   284,     0,     0,     0,   224,   311,
     300,     0,   326,   327,   225,   274,   275,   239,   240,   290,
     291,   292,   229,   230,   231,     0,     0,   223,     0,     0,
     288,   289,   287,   281,   282,   277,   276,   278,   279,   280,
     255,     0,     0,   304,     0,     0,   331,   332,     0,   335,
     366,   222,   367,   455,     0,   461,   451,   431,     0,   465,
       0,   472,     0,   479,     0,     0,   485,     0,   488,     0,
     351,     0,     0,   165,     0,     0,     0,     0,   172,   179,
     307,     0,     0,   308,     0,   212,   176,     0,   505,     0,
       0,   504,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   514,   522,   523,     0,    23,    26,     0,
      82,     0,     0,    44,    45,     0,    51,     0,     0,     0,
     303,     0,     0,     0,     0,   210,     0,    85,   313,     0,
     243,   244,   299,   241,   242,   318,     0,   257,   298,   305,
      81,   336,     0,   333,   365,   456,     0,   462,   452,   432,
     466,   468,   473,   475,   480,   482,   345,   486,   347,   489,
     356,     0,   166,     0,   195,   193,   184,   187,   190,     0,
       0,   499,   498,   504,     0,     0,   503,     0,   507,   513,
     519,   518,     0,   504,     0,   520,    21,    24,     0,    85,
     134,    47,     0,     0,   199,   208,   209,   207,   206,     0,
       0,     0,     0,   246,     0,   312,   258,   339,     0,   338,
     458,   469,   476,   483,    52,     0,     0,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,     0,
     510,   250,     0,    46,   245,   211,   213,     0,   200,   215,
       0,   247,     0,   459,     0,     0,   185,   188,   191,   309,
       0,   524,     0,   508,   501,   500,   524,     0,     0,   253,
      48,     0,   201,   314,   337,   196,     0,   310,     0,     0,
     509,     0,   511,   251,     0,   202,   315,     0,     0,     0,
       0,   254,   197,   525,   526,   527
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -603,  -603,  -603,   168,   581,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,   538,  -603,  -603,   580,  -603,  -603,
    -603,  -243,  -603,  -603,  -603,  -603,  -603,  -603,   399,  -603,
     594,   -56,  -603,   582,   113,  -112,  -392,  -540,  -603,  -603,
    -202,  -133,   -81,  -603,    36,  -281,   -13,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,   664,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -526,  -603,  -603,
    -183,  -603,   -68,  -305,  -603,  -236,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,   421,  -603,  -603,
     286,   -28,  -603,  -603,  -603,   392,  -603,   -45,  -603,  -274,
     264,  -279,  -275,   290,  -603,  -148,  -603,   632,  -603,  -603,
     491,   610,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -227,  -108,  -603,  -603,  -603,   292,  -603,  -603,  -603,   -72,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,   395,  -602,
    -452,  -582,  -603,  -603,  -313,   -69,   217,  -603,  -603,  -603,
    -223,  -603
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   172,   178,   467,   341,   758,   342,   623,
      16,    17,    35,    36,    63,    18,    32,    33,   343,   344,
     765,   766,   345,   346,   347,   348,   349,   350,   179,   180,
      29,    30,    45,    46,    47,    19,   161,   238,    81,    20,
      82,    83,   351,   352,   239,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   589,   868,   590,
     869,   592,   870,   365,   580,   867,   366,   631,   889,   367,
     775,   776,   634,   368,   369,   436,   371,   372,   373,   374,
     759,   908,   760,   924,   375,   691,   856,   632,   852,   926,
     234,   698,   546,   383,   858,   235,   236,   566,   567,   229,
     230,   740,   282,   389,   247,   169,   167,    54,    22,    87,
     109,    67,    68,    23,    24,   165,    85,    52,    25,   166,
     170,   377,   108,   222,   223,   227,   224,   398,   799,   568,
     396,   798,   393,   795,   394,   893,   395,   797,   399,   800,
     400,   861,   401,   802,   402,   862,   403,   804,   404,   863,
     405,   807,   406,   809,    26,    41,    27,    40,   378,   606,
     607,   608,   379,   609,   610,   611,   612,   380,   466,   381,
     919,   382
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   163,   221,   370,   425,   418,   420,   283,   560,   465,
     562,   615,   564,   458,   459,   694,   435,   -80,   116,   243,
     710,   577,   712,   737,   714,   744,     2,   768,   487,   408,
     601,   749,     3,   751,   453,   636,    88,    89,   225,   489,
     490,   281,   421,   601,   232,    59,   509,    42,   887,   512,
     513,   722,   724,   747,   283,     4,    66,     5,   748,     6,
      38,     7,    48,   253,   428,     8,   183,   771,   512,   513,
      64,   168,     9,   421,    56,   266,   772,    60,    10,    77,
     244,   422,   739,   423,   769,   480,   424,   233,    73,   769,
     769,   488,   254,   386,   250,   111,    11,   220,    65,   267,
     268,   231,   387,   162,   421,   184,   773,   539,   550,   540,
     242,   774,   422,   739,   811,   248,   249,    12,   481,   872,
     878,   251,    28,   501,   502,    53,   173,   174,   540,   221,
     183,   509,   388,   511,   512,   513,   514,   221,   767,   168,
     515,   826,    43,   422,   574,   232,   618,   769,   619,    78,
      79,   240,   581,   244,   409,   244,   769,   269,    44,   824,
      90,   637,   743,   244,    91,   286,    92,    93,    43,   391,
     221,   596,   410,   385,   770,    34,   280,   489,   490,    73,
      31,   411,   635,   789,    44,   542,   221,    61,   769,    80,
     414,   228,   539,   543,   540,   270,   271,    62,   625,    94,
     272,    13,   703,   273,    37,   715,   416,    14,   421,   769,
      43,   820,   221,   221,   485,   812,    39,   875,   274,   438,
     266,    75,   429,   832,   220,    55,    44,   276,    56,   415,
     417,   450,   220,   600,    56,   613,   891,   851,    75,   742,
     284,   873,   777,   285,   267,   268,    76,   422,   461,   463,
     175,   879,   849,   854,   176,   486,   177,    93,   707,   419,
      57,   501,   502,   790,   843,   220,   697,   791,   700,   509,
     716,   544,   512,   513,   514,    58,    66,   850,   515,    73,
     708,   220,   718,    70,   865,   221,   221,   221,   221,   823,
      71,   221,   717,   221,   721,   221,   738,   221,   833,   882,
      72,   386,   269,    74,   719,   754,   866,   220,   220,    84,
     387,   489,   490,   102,   762,   221,   748,   103,   752,    86,
     471,   553,   554,   555,   556,   763,   764,   559,    97,   561,
     539,   563,   540,  -467,   104,   105,   106,   107,  -467,  -474,
     547,   271,    99,   221,  -474,   272,   576,  -481,   273,   266,
     100,   573,  -481,  -252,    49,    50,  -467,    51,  -252,   101,
     588,   244,  -474,   274,   245,   110,   113,   246,   839,   548,
    -481,   114,   276,   267,   268,   115,  -252,   158,   159,   595,
     220,   220,   220,   220,   337,   457,   220,   162,   220,   160,
     220,   162,   220,   499,   500,   501,   502,   503,   628,   164,
     506,   507,   508,   509,   168,   511,   512,   513,   514,   171,
     220,   266,   515,    60,   517,   518,   182,   226,   579,   228,
     521,   522,   523,   793,   171,   620,   621,   255,   237,   241,
     256,   269,   259,   277,   257,   267,   268,   258,   220,   260,
     261,   262,   263,   264,   528,   418,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   265,   384,   278,   397,
     407,   279,   626,   232,   539,   431,   540,   432,   442,   270,
     271,   283,   221,   162,   272,   445,   440,   273,   443,   444,
     446,   221,   221,   447,   221,   451,   489,   490,   475,   476,
     557,   487,   274,   269,   586,   477,   593,   478,   275,   482,
     483,   276,   484,   541,    13,   729,   570,   572,   727,   489,
     490,   633,   594,   599,   627,   638,   655,   731,   732,   660,
     734,   690,   696,   745,   697,   370,   248,   699,   757,    48,
     767,   270,   271,   266,   742,   746,   272,   755,   779,   273,
     785,   787,   414,   778,   813,   825,   876,   801,   803,   805,
     834,   837,   907,   909,   274,   845,   578,   267,   268,   846,
     847,   848,   857,   276,   871,   916,   895,   220,   499,   500,
     501,   502,   503,   827,   914,   506,   220,   220,   509,   220,
     511,   512,   513,   514,   828,   830,   835,   515,   874,   517,
     518,   499,   500,   928,   877,   503,   221,   884,   506,   507,
     508,   509,   880,   511,   512,   513,   514,   221,   886,   221,
     515,   902,   517,   518,   903,   269,   489,   490,   521,   522,
     523,   920,   922,   929,   532,   533,   534,   535,   536,   537,
     538,   930,   796,   934,   935,   622,    95,   181,    98,   539,
     910,   540,   528,   806,   529,   530,   531,   532,   533,   390,
      96,   221,   537,   270,   271,   695,   892,   117,   272,   859,
     376,   273,   539,   761,   540,    21,   885,   819,   792,   545,
     569,    69,   808,   252,   392,   549,   274,   426,   112,   831,
     616,   841,   704,   921,     0,   276,     0,   838,     0,   430,
     558,   220,   489,   490,     0,   413,     0,     0,     0,     0,
     501,   502,   220,     0,   220,     0,     0,     0,   509,   427,
     511,   512,   513,   514,     0,     0,     0,   515,   437,     0,
       0,     0,   439,   266,   441,     0,     0,     0,     0,     0,
     864,     0,   449,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,   464,   221,     0,   220,   267,   268,     0,
       0,   468,   469,   470,     0,   472,   473,   474,   536,   537,
     538,   883,     0,     0,     0,     0,   479,     0,     0,   539,
     888,   540,     0,     0,   499,   500,   501,   502,   503,     0,
     890,   506,   507,   508,   509,     0,   511,   512,   513,   514,
       0,     0,     0,   515,     0,   517,   518,     0,     0,     0,
       0,   521,   522,   523,     0,   269,     0,     0,     0,     0,
     551,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,     0,     0,   220,
       0,     0,     0,   270,   271,   539,   575,   540,   272,     0,
     705,   273,     0,   552,   582,     0,     0,     0,     0,     0,
       0,   587,     0,     0,     0,   591,   274,     0,     0,     0,
       0,     0,     0,   597,     0,   276,     0,     0,     0,     0,
       0,     0,   605,     0,   605,     0,     0,     0,     0,     0,
     489,   490,     0,     0,     0,   923,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   931,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,     0,     0,   649,   650,   651,   652,   653,   654,
       0,   658,   659,     0,   661,   662,   663,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   677,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,   689,
       0,   692,     0,     0,     0,     0,     0,     0,     0,     0,
     701,     0,   499,   500,   501,   502,     0,     0,   489,   490,
       0,     0,   509,     0,   511,   512,   513,   514,     0,     0,
       0,   515,     0,   517,   518,     0,     0,     0,     0,     0,
     726,     0,     0,     0,   658,   677,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,     0,
       0,     0,     0,     0,   468,   474,     0,     0,     0,     0,
     534,   535,   536,   537,   538,   474,     0,     0,   753,   464,
     267,   268,   756,   539,     0,   540,     0,     0,     0,     0,
       0,     0,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   583,   511,   512,   513,   514,     0,   780,   781,   515,
     516,   517,   518,   519,   520,     0,     0,   521,   522,   523,
     524,   525,   526,     0,     0,     0,   783,   784,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     584,   528,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,     0,     0,     0,     0,     0,     0,     0,
       0,   539,     0,   540,     0,     0,   270,   271,     0,   585,
       0,   272,   810,   706,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
     821,   822,     0,     0,     0,     0,     0,   829,   276,   605,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   376,     0,   287,     0,   842,     0,     0,     3,
     844,   288,   289,   290,     0,   291,     0,   292,   293,   294,
     295,   296,     0,     0,     0,     0,     0,   855,   297,   298,
     299,   300,     0,     0,     0,     0,     0,     0,     0,     0,
     301,   302,     0,   303,   304,     0,     0,   305,     0,     9,
     306,   307,     0,   308,   309,     0,     0,   310,   311,     0,
       0,     0,     0,   312,   189,   190,   191,     0,   193,   194,
     195,   196,   197,   313,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   209,   210,   211,     0,     0,   214,
     215,   216,   217,     0,     0,   314,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   894,   316,   317,   896,
     897,   898,   900,   901,     0,     0,   904,   905,     0,   906,
       0,     0,   318,   319,     0,     0,     0,     0,   912,     0,
       0,    43,     0,     0,     0,     0,     0,     0,   320,   321,
     322,   323,   324,   325,   326,   327,   328,    44,   329,     0,
       0,     0,   925,     0,     0,     0,     0,   927,     0,     0,
       0,     0,     0,   330,   331,   332,     0,   333,     0,   933,
     334,   335,     0,     0,     0,     0,     0,     0,     0,     0,
     336,     0,   337,   338,   339,   162,   340,   287,     0,     0,
       0,     0,     3,     0,   288,   289,   290,     0,   291,     0,
     292,   293,   294,   295,   296,     0,     0,     0,     0,     0,
       0,   297,   298,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,   301,   302,     0,   303,   304,     0,     0,
     305,     0,     9,   306,   307,     0,   308,   309,     0,     0,
     310,   311,     0,     0,     0,     0,   312,   189,   190,   191,
       0,   193,   194,   195,   196,   197,   313,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   209,   210,   211,
       0,     0,   214,   215,   216,   217,     0,     0,   314,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,   317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   489,   490,     0,   318,   319,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      44,   329,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   330,   331,   332,     0,
     333,     0,     0,   334,   335,     0,     0,     0,   292,   293,
     294,   295,   296,   336,     0,   337,   338,   339,   162,   840,
     298,     0,     0,     0,     0,     0,   266,     0,     0,     0,
       0,     0,   302,     0,     0,   304,   501,   502,   305,     0,
       0,   306,     0,   601,   509,   309,   511,   512,   513,   514,
     267,   268,     0,   515,     0,   189,   190,   191,     0,   193,
     194,   195,   196,   197,   313,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   209,   210,   211,     0,     0,
     214,   215,   216,   217,     0,     0,   314,     0,     0,     0,
       0,     0,   534,   535,   536,   537,   538,     0,   316,   317,
       0,     0,     0,     0,     0,   539,     0,   540,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   602,   329,
       0,     0,     0,   267,   268,     0,   270,   271,     0,     0,
       0,   272,     0,     0,   603,   331,   332,     0,   333,     0,
       0,   334,   335,   292,   293,   294,   295,   296,     0,   274,
       0,   604,     0,   337,   338,   298,   162,     0,   276,     0,
       0,   266,     0,     0,     0,     0,     0,   302,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,     0,
     309,   269,     0,     0,     0,   267,   268,     0,     0,     0,
     189,   190,   191,     0,   193,   194,   195,   196,   197,   313,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,   270,
     271,   314,     0,     0,   272,     0,   709,   273,     0,     0,
       0,     0,     0,   316,   317,     0,     0,     0,     0,     0,
       0,     0,   274,   269,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,   266,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   602,   329,     0,     0,     0,   267,   268,
       0,   270,   271,     0,     0,     0,   272,     0,     0,   603,
     331,   332,     0,   333,     0,     0,   334,   335,   292,   293,
     294,   295,   296,     0,   274,     0,   614,     0,   337,   338,
     298,   162,     0,   276,     0,     0,   408,     0,     0,     0,
       0,     0,   302,     0,     0,   304,     0,     0,   305,     0,
       0,   306,     0,     0,     0,   309,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,     0,   193,
     194,   195,   196,   197,   313,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   209,   210,   211,     0,     0,
     214,   215,   216,   217,   270,   271,   314,     0,     0,   272,
       0,   711,   273,     0,     0,     0,     0,     0,   316,   317,
       0,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,    43,   266,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    44,   329,
       0,     0,     0,     0,     0,     0,     0,   267,   268,     0,
       0,   409,     0,     0,   330,   331,   332,     0,   333,     0,
       0,   334,   335,   292,   293,   294,   295,   296,     0,   410,
       0,   336,     0,   337,   338,   298,   162,     0,   411,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,     0,
     309,     0,     0,     0,     0,   269,     0,     0,     0,     0,
     189,   190,   191,     0,   193,   194,   195,   196,   197,   313,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,     0,
       0,   314,     0,   270,   271,     0,     0,     0,   272,     0,
     713,   273,     0,   316,   317,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,   274,     0,   318,   319,
       0,     0,     0,     0,     0,   276,     0,    43,     0,   266,
       0,   266,     0,     0,   320,   321,   322,   323,   324,   325,
     326,   327,   328,    44,   329,     0,     0,     0,     0,     0,
       0,     0,     0,   267,   268,   267,   268,     0,     0,   330,
     331,   332,     0,   333,     0,     0,   334,   335,     0,     0,
     292,   293,   294,   295,   296,     0,   336,     0,   337,   338,
     434,   162,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,     0,   304,     0,     0,
     305,     0,     0,   306,     0,     0,     0,   309,     0,     0,
     421,   269,     0,   269,     0,     0,     0,   189,   190,   191,
       0,   193,   194,   195,   196,   197,   313,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   209,   210,   211,
       0,     0,   214,   215,   216,   217,     0,     0,   314,   720,
     271,   270,   271,     0,   272,     0,   272,   273,   735,   273,
     316,   317,     0,   266,     0,     0,     0,     0,     0,   452,
       0,     0,   274,     0,   274,   318,   319,     0,     0,     0,
       0,   276,     0,   276,    43,     0,     0,   267,   268,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      44,   329,     0,     0,     0,     0,     0,   292,   293,   294,
     295,   296,     0,     0,     0,     0,   330,   331,   332,   298,
     333,     0,     0,   334,   335,     0,     0,     0,     0,     0,
       0,   302,     0,   336,   304,   337,   338,   305,   162,     0,
     306,     0,     0,     0,   309,   269,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,     0,   193,   194,
     195,   196,   197,   313,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   209,   210,   211,     0,     0,   214,
     215,   216,   217,   270,   271,   314,     0,     0,   272,     0,
     815,   273,     0,     0,     0,     0,     0,   316,   317,     0,
     266,     0,     0,     0,     0,     0,   274,     0,     0,     0,
       0,     0,   318,   319,     0,   276,     0,     0,     0,     0,
       0,    43,     0,     0,   267,   268,     0,     0,   320,   321,
     322,   323,   324,   325,   326,   327,   328,    44,   329,     0,
       0,     0,     0,     0,   292,   293,   294,   295,   296,     0,
       0,     0,     0,   330,   331,   332,   298,   333,     0,     0,
     334,   335,     0,     0,     0,     0,     0,     0,   302,     0,
     336,   304,   337,   338,   305,   162,     0,   306,     0,     0,
       0,   309,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,     0,   193,   194,   195,   196,   197,
     313,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   209,   210,   211,     0,     0,   214,   215,   216,   217,
     270,   271,   314,     0,     0,   272,     0,   816,   273,     0,
       0,     0,     0,     0,   316,   317,     0,     0,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,   266,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    44,   329,     0,     0,     0,   267,
     268,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,   331,   332,     0,   333,     0,     0,   334,   335,   292,
     293,   294,   295,   296,     0,   629,     0,   336,   630,   337,
     338,   298,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,     0,   309,   269,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   190,   191,     0,
     193,   194,   195,   196,   197,   313,   199,   200,   201,   202,
     203,   204,   205,   206,   207,     0,   209,   210,   211,     0,
       0,   214,   215,   216,   217,   270,   271,   314,     0,     0,
     272,     0,   817,   273,     0,     0,     0,     0,     0,   316,
     317,     0,   266,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,   656,   657,     0,   276,     0,     0,
       0,     0,     0,    43,     0,     0,   267,   268,     0,     0,
     320,   321,   322,   323,   324,   325,   326,   327,   328,    44,
     329,     0,     0,     0,     0,     0,   292,   293,   294,   295,
     296,     0,     0,     0,     0,   330,   331,   332,   298,   333,
       0,     0,   334,   335,     0,     0,     0,     0,     0,     0,
     302,     0,   336,   304,   337,   338,   305,   162,     0,   306,
       0,     0,     0,   309,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   313,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,   270,   271,   314,     0,     0,   272,     0,   818,
     273,     0,     0,     0,     0,     0,   316,   317,     0,   266,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,   675,   676,     0,   276,     0,     0,     0,     0,     0,
      43,     0,     0,   267,   268,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    44,   329,     0,     0,
       0,     0,     0,   292,   293,   294,   295,   296,     0,     0,
       0,     0,   330,   331,   332,   298,   333,     0,     0,   334,
     335,     0,     0,     0,     0,     0,     0,   302,     0,   336,
     304,   337,   338,   305,   162,     0,   306,     0,     0,     0,
     309,   269,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,     0,   193,   194,   195,   196,   197,   313,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
     209,   210,   211,     0,     0,   214,   215,   216,   217,   270,
     271,   314,     0,     0,   272,     0,   860,   273,     0,     0,
       0,     0,     0,   316,   317,     0,   266,     0,     0,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,    43,     0,     0,
     267,   268,     0,     0,   320,   321,   322,   323,   324,   325,
     326,   327,   328,    44,   329,     0,     0,     0,     0,     0,
     292,   293,   294,   295,   296,     0,     0,     0,     0,   330,
     331,   332,   298,   333,     0,     0,   334,   335,     0,     0,
       0,     0,     0,     0,   302,   412,   336,   304,   337,   338,
     305,   162,     0,   306,     0,     0,     0,   309,   269,     0,
     448,     0,     0,     0,     0,     0,     0,   189,   190,   191,
       0,   193,   194,   195,   196,   197,   313,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   209,   210,   211,
       0,     0,   214,   215,   216,   217,   270,   271,   314,     0,
       0,   272,     0,   881,   273,     0,     0,     0,     0,     0,
     316,   317,     0,   266,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,    43,     0,     0,   267,   268,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      44,   329,     0,     0,     0,     0,     0,   292,   293,   294,
     295,   296,     0,     0,   678,     0,   330,   331,   332,   298,
     333,     0,     0,   334,   335,     0,     0,     0,     0,     0,
       0,   302,     0,   336,   304,   337,   338,   305,   162,     0,
     306,     0,     0,     0,   309,   269,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,     0,   193,   194,
     195,   196,   197,   313,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   209,   210,   211,     0,     0,   214,
     215,   216,   217,   270,   271,   314,     0,     0,   272,     0,
     913,   273,     0,     0,     0,     0,     0,   316,   317,     0,
     266,     0,     0,     0,     0,     0,   274,     0,     0,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,    43,     0,     0,   267,   268,     0,     0,   320,   321,
     322,   323,   324,   325,   326,   327,   328,    44,   329,     0,
       0,     0,     0,     0,   292,   293,   294,   295,   296,     0,
       0,     0,     0,   330,   331,   332,   298,   333,     0,     0,
     334,   335,     0,     0,     0,     0,     0,     0,   302,     0,
     336,   304,   337,   338,   305,   162,     0,   306,     0,     0,
       0,   309,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,     0,   193,   194,   195,   196,   197,
     313,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   209,   210,   211,     0,     0,   214,   215,   216,   217,
     270,   271,   314,     0,     0,   272,     0,     0,   273,     0,
       0,     0,     0,     0,   316,   317,     0,     0,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    44,   329,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,   331,   332,     0,   333,     0,     0,   334,   335,   292,
     293,   294,   295,   296,     0,     0,     0,   336,   693,   337,
     338,   298,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   190,   191,     0,
     193,   194,   195,   196,   197,   313,   199,   200,   201,   202,
     203,   204,   205,   206,   207,     0,   209,   210,   211,     0,
       0,   214,   215,   216,   217,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     320,   321,   322,   323,   324,   325,   326,   327,   328,    44,
     329,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   330,   331,   332,     0,   333,
       0,     0,   334,   335,   292,   293,   294,   295,   296,     0,
       0,     0,   336,   723,   337,   338,   298,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
       0,   304,     0,     0,   305,     0,     0,   306,     0,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,     0,   193,   194,   195,   196,   197,
     313,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   209,   210,   211,     0,     0,   214,   215,   216,   217,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    44,   329,     0,     0,     0,     0,
       0,   292,   293,   294,   295,   296,     0,     0,   725,     0,
     330,   331,   332,   298,   333,     0,     0,   334,   335,     0,
       0,     0,     0,     0,     0,   302,     0,   336,   304,   337,
     338,   305,   162,     0,   306,     0,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,     0,   193,   194,   195,   196,   197,   313,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   209,   210,
     211,     0,     0,   214,   215,   216,   217,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    44,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,   331,   332,
       0,   333,     0,     0,   334,   335,   292,   293,   294,   295,
     296,     0,     0,     0,   336,   741,   337,   338,   298,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,     0,   304,     0,     0,   305,     0,     0,   306,
       0,     0,     0,   309,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   313,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    44,   329,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   331,   332,     0,   333,     0,     0,   334,
     335,   292,   293,   294,   295,   296,     0,     0,     0,   336,
     750,   337,   338,   298,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,     0,   193,   194,   195,   196,   197,   313,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   209,   210,
     211,     0,     0,   214,   215,   216,   217,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    44,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,   331,   332,
       0,   333,     0,     0,   334,   335,   292,   293,   294,   295,
     296,     0,     0,     0,   336,   853,   337,   338,   298,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,     0,   304,     0,     0,   305,     0,     0,   306,
       0,     0,     0,   309,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,     0,   193,   194,   195,
     196,   197,   313,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   209,   210,   211,     0,     0,   214,   215,
     216,   217,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    44,   329,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   331,   332,     0,   333,     0,     0,   334,
     335,   292,   293,   294,   295,   296,     0,     0,     0,   336,
     899,   337,   338,   298,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,     0,   193,   194,   195,   196,   197,   313,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   209,   210,
     211,     0,     0,   214,   215,   216,   217,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   317,     0,     0,     0,     0,     0,     0,     0,
     911,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    44,   329,     0,     0,     0,     0,     0,   292,   293,
     294,   295,   296,     0,     0,     0,     0,   330,   331,   332,
     298,   333,     0,     0,   334,   335,     0,     0,     0,     0,
       0,     0,   302,     0,   336,   304,   337,   338,   305,   162,
       0,   306,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,     0,   193,
     194,   195,   196,   197,   313,   199,   200,   201,   202,   203,
     204,   205,   206,   207,     0,   209,   210,   211,     0,     0,
     214,   215,   216,   217,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,   317,
       0,     0,     0,   489,   490,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    44,   329,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,   331,   332,     0,   333,     0,
       0,   334,   335,     0,     0,     0,     0,     0,     0,     0,
       0,   336,     0,   337,   338,     0,   162,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   583,   511,   512,   513,
     514,   489,   490,     0,   515,   516,   517,   518,   519,   520,
       0,     0,   521,   522,   523,   524,   525,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,     0,     0,     0,     0,   539,     0,   540,     0,
       0,     0,     0,     0,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   583,   511,   512,   513,   514,   489,
     490,     0,   515,   516,   517,   518,   519,   520,     0,     0,
     521,   522,   523,   524,   525,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,     0,
       0,     0,     0,     0,   539,     0,   540,     0,     0,     0,
       0,     0,   702,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   583,   511,   512,   513,   514,   489,   490,     0,
     515,   516,   517,   518,   519,   520,     0,     0,   521,   522,
     523,   524,   525,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,   539,     0,   540,     0,     0,     0,     0,     0,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     583,   511,   512,   513,   514,   489,   490,     0,   515,   516,
     517,   518,   519,   520,     0,     0,   521,   522,   523,   524,
     525,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
     528,     0,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,     0,     0,     0,     0,     0,     0,     0,     0,
     539,     0,   540,     0,     0,     0,     0,     0,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   583,   511,
     512,   513,   514,   489,   490,     0,   515,   516,   517,   518,
     519,   520,     0,     0,   521,   522,   523,   524,   525,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,     0,     0,     0,     0,     0,   539,     0,
     540,     0,     0,     0,     0,     0,   794,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   489,   490,     0,
       0,     0,     0,     0,     0,   499,   500,   501,   502,   503,
       0,     0,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,     0,     0,   489,   490,   539,     0,   540,   499,
     500,   501,   502,   503,   918,     0,   506,   507,   508,   509,
       0,   511,   512,   513,   514,     0,     0,     0,   515,     0,
     517,   518,     0,     0,     0,     0,   521,   522,   523,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,     0,     0,     0,     0,     0,     0,     0,     0,
     539,     0,   540,   489,   490,   624,   499,   500,   501,   502,
     503,     0,     0,   506,   507,   508,   509,     0,   511,   512,
     513,   514,     0,     0,     0,   515,     0,   517,   518,     0,
       0,     0,     0,   521,   522,   523,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   528,     0,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,     0,
       0,     0,     0,     0,     0,     0,     0,   539,     0,   540,
     489,   490,   730,     0,     0,   499,   500,   501,   502,   503,
       0,     0,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,     0,     0,     0,     0,   539,     0,   540,   489,
     490,   733,   499,   500,   501,   502,   503,     0,     0,   506,
     507,   508,   509,     0,   511,   512,   513,   514,     0,     0,
       0,   515,     0,   517,   518,     0,     0,     0,     0,   521,
     522,   523,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,     0,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,     0,     0,     0,     0,     0,
       0,     0,     0,   539,     0,   540,   489,   490,   814,     0,
       0,   499,   500,   501,   502,   503,     0,     0,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,     0,     0,     0,     0,   521,   522,
     523,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,   539,     0,   540,   489,   490,   915,   499,   500,
     501,   502,   503,     0,     0,   506,   507,   508,   509,     0,
     511,   512,   513,   514,     0,     0,     0,   515,     0,   517,
     518,     0,     0,     0,     0,   521,   522,   523,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,   539,
       0,   540,   489,   490,   917,     0,     0,   499,   500,   501,
     502,   503,     0,     0,   506,   507,   508,   509,     0,   511,
     512,   513,   514,     0,     0,     0,   515,     0,   517,   518,
       0,     0,     0,     0,   521,   522,   523,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,     0,     0,     0,     0,     0,   539,     0,
     540,   489,   490,   932,   499,   500,   501,   502,   503,     0,
       0,   506,   507,   508,   509,     0,   511,   512,   513,   514,
       0,     0,     0,   515,     0,   517,   518,     0,     0,     0,
       0,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,     0,     0,     0,
       0,     0,     0,     0,     0,   539,     0,   540,   571,   489,
     490,     0,     0,   499,   500,   501,   502,   503,     0,     0,
     506,   507,   508,   509,     0,   511,   512,   513,   514,     0,
       0,     0,   515,     0,   517,   518,     0,     0,     0,     0,
     521,   522,   523,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,     0,
       0,     0,     0,     0,   539,     0,   540,   782,     0,     0,
       0,   499,   500,   501,   502,   503,   489,   490,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,     0,     0,     0,     0,   521,   522,
     523,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,   539,     0,   540,   788,     0,     0,     0,     0,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   583,
     511,   512,   513,   514,  -222,   489,   490,   515,   516,   517,
     518,   519,   520,     0,     0,   521,   522,   523,   524,   525,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   584,   528,
       0,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,   539,
       0,   540,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   489,   490,     0,   515,   516,   517,   518,
     519,   520,     0,     0,   521,   522,   523,   524,   525,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,     0,     0,     0,     0,     0,   539,     0,
     540,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   489,   490,     0,
       0,     0,     0,     0,     0,   499,   500,   501,   502,   503,
       0,     0,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
     617,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,   489,   490,     0,     0,     0,   539,     0,   540,   499,
     500,   501,   502,   503,     0,     0,   506,   507,   508,   509,
       0,   511,   512,   513,   514,     0,     0,     0,   515,     0,
     517,   518,     0,     0,     0,     0,   521,   522,   523,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,   786,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,     0,     0,     0,   489,   490,     0,     0,     0,
     539,     0,   540,   499,   500,   501,   502,   503,     0,     0,
     506,   507,   508,   509,     0,   511,   512,   513,   514,     0,
       0,     0,   515,     0,   517,   518,     0,     0,     0,     0,
     521,   522,   523,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     836,     0,     0,     0,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,   489,
     490,     0,     0,     0,   539,     0,   540,   499,   500,   501,
     502,   503,     0,     0,   506,   507,   508,   509,     0,   511,
     512,   513,   514,     0,     0,     0,   515,     0,   517,   518,
       0,     0,     0,     0,   521,   522,   523,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,   489,   490,     0,     0,     0,   539,     0,
     540,   499,   500,   501,   502,   503,     0,     0,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,     0,     0,     0,     0,   521,     0,
     523,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,   489,   490,     0,
       0,     0,   539,     0,   540,   499,   500,   501,   502,   503,
       0,     0,   506,   507,   508,   509,     0,   511,   512,   513,
     514,   489,   490,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,     0,     0,     0,     0,   539,     0,   540,   499,
     500,   501,   502,   503,     0,     0,   506,   507,   508,   509,
       0,   511,   512,   513,   514,   489,   490,     0,   515,     0,
     517,   518,     0,   499,   500,   501,   502,   503,     0,     0,
     506,   507,   508,   509,     0,   511,   512,   513,   514,   489,
     490,     0,   515,     0,   517,   518,     0,     0,     0,     0,
       0,     0,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,     0,     0,     0,     0,     0,     0,     0,     0,
     539,     0,   540,     0,     0,     0,     0,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,     0,
       0,     0,     0,     0,   539,     0,   540,   499,   500,   501,
     502,   503,     0,     0,   506,   507,   508,   509,     0,   511,
     512,   513,   514,     0,     0,     0,   515,     0,   517,   518,
       0,   499,   500,   501,   502,   503,     0,     0,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,     0,     0,     0,     0,     0,     0,
       0,   460,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,   185,     0,     0,     0,     0,   539,   186,
     540,     0,     0,     0,     0,     0,     0,   532,   533,   534,
     535,   536,   537,   538,   187,     0,     0,     0,     0,     0,
       0,     0,   539,     0,   540,     0,     0,     0,   188,     0,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,     0,     0,     0,     0,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   185,     0,     0,
       0,     0,     0,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,   187,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   188,    44,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   185,     0,     0,     0,     0,     0,   186,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,   187,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,    44,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   185,     0,     0,     0,     0,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   187,     0,     0,     0,
       0,     0,   320,   321,   322,     0,     0,     0,     0,     0,
     188,    44,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   185,
       0,     0,     0,     0,     0,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   188,    44,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,     0,     0,     0,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
     137,   138,   139,    43,     0,   140,   141,   142,   143,   144,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,     0,     0,   156,   157
};

static const yytype_int16 yycheck[] =
{
      13,    82,   110,   239,   283,   279,   281,   234,   400,   322,
     402,   463,   404,   318,   319,   541,   297,     7,    74,   167,
     560,    19,   562,    19,   564,   607,     0,   629,   118,    32,
      49,    19,     6,   615,   315,    18,    14,    15,   110,    20,
      21,    46,   113,    49,   135,    21,   112,    33,   127,   115,
     116,   577,   578,   176,   281,    29,   144,    31,   181,    33,
     182,    35,    75,   152,   291,    39,   144,   113,   115,   116,
     154,   144,    46,   113,   151,    32,   122,    53,    52,    34,
     151,   152,   122,   154,   151,   144,   157,   178,   128,   151,
     151,   181,   181,   113,   151,   183,    70,   110,   182,    56,
      57,   178,   122,   182,   113,   183,   152,   173,   389,   175,
     183,   157,   152,   122,   181,   146,   147,    91,   177,   181,
     181,   178,   144,   104,   105,    58,    14,    15,   175,   237,
     144,   112,   152,   114,   115,   116,   117,   245,   144,   144,
     121,   743,   128,   152,   425,   135,   181,   151,   183,   104,
     105,   164,   433,   151,   157,   151,   151,   114,   144,   741,
     138,   144,   181,   151,   142,   237,   144,   145,   128,   183,
     278,   452,   175,   245,   178,   144,   232,    20,    21,   128,
     138,   184,   487,   178,   144,   175,   294,   163,   151,   144,
     144,   144,   173,   183,   175,   152,   153,   173,   158,   177,
     157,   175,   159,   160,   144,   154,   278,   181,   113,   151,
     128,   737,   320,   321,   362,   178,   144,   122,   175,   300,
      32,   151,   294,   749,   237,   152,   144,   184,   151,   183,
     183,   312,   245,   460,   151,   462,   178,   777,   151,   151,
     178,   823,   634,   181,    56,    57,   176,   152,   320,   321,
     138,   833,   151,   779,   142,   367,   144,   145,   159,   176,
     154,   104,   105,   176,   176,   278,    42,    43,   549,   112,
     159,   383,   115,   116,   117,   151,   144,   176,   121,   128,
     181,   294,   159,   144,   159,   393,   394,   395,   396,   741,
     144,   399,   181,   401,   573,   403,   601,   405,   750,   839,
     144,   113,   114,   177,   181,   618,   181,   320,   321,   144,
     122,    20,    21,    65,    11,   423,   181,    69,   183,   182,
     333,   393,   394,   395,   396,    22,    23,   399,   138,   401,
     173,   403,   175,   154,    86,    87,    88,    89,   159,   154,
     152,   153,   144,   451,   159,   157,   427,   154,   160,    32,
     144,   423,   159,   154,     4,     5,   177,     7,   159,   144,
     441,   151,   177,   175,   154,   152,   182,   157,   760,   181,
     177,   182,   184,    56,    57,   144,   177,    34,    34,   451,
     393,   394,   395,   396,   179,   180,   399,   182,   401,   177,
     403,   182,   405,   102,   103,   104,   105,   106,   479,   154,
     109,   110,   111,   112,   144,   114,   115,   116,   117,   146,
     423,    32,   121,    53,   123,   124,   182,   144,   431,   144,
     129,   130,   131,   698,   146,   147,   148,   158,   154,   182,
     158,   114,   177,   181,   158,    56,    57,   158,   451,   158,
     158,   158,   158,   158,   153,   719,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   158,   144,   154,   144,
      32,   181,   475,   135,   173,   177,   175,   158,   158,   152,
     153,   698,   580,   182,   157,   181,   177,   160,   158,   177,
     158,   589,   590,   138,   592,   158,    20,    21,   180,    36,
     178,   118,   175,   114,    13,   181,   154,   181,   181,   181,
     181,   184,   181,   177,   175,   586,   176,   176,   580,    20,
      21,   135,   138,   180,   182,   144,   144,   589,   590,   144,
     592,   144,     7,   176,    42,   761,   146,   181,   146,   542,
     144,   152,   153,    32,   151,   151,   157,   183,   177,   160,
     144,     1,   144,   158,   144,   144,   825,   159,   159,   159,
     183,   146,   183,   159,   175,   144,   177,    56,    57,   144,
     144,   144,   181,   184,   177,   159,   144,   580,   102,   103,
     104,   105,   106,   176,   181,   109,   589,   590,   112,   592,
     114,   115,   116,   117,   176,   176,   176,   121,   176,   123,
     124,   102,   103,    49,   176,   106,   704,   178,   109,   110,
     111,   112,   176,   114,   115,   116,   117,   715,   176,   717,
     121,   176,   123,   124,   176,   114,    20,    21,   129,   130,
     131,   176,   176,   176,   158,   159,   160,   161,   162,   163,
     164,   183,   704,   176,   176,   467,    55,    99,    58,   173,
     883,   175,   153,   715,   155,   156,   157,   158,   159,   250,
      56,   759,   163,   152,   153,   542,   858,    75,   157,   792,
     239,   160,   173,   627,   175,     1,   849,   735,   696,   383,
     406,    39,   717,   182,   253,   385,   175,   285,    68,   748,
     463,   762,   181,   906,    -1,   184,    -1,   759,    -1,   294,
     398,   704,    20,    21,    -1,   274,    -1,    -1,    -1,    -1,
     104,   105,   715,    -1,   717,    -1,    -1,    -1,   112,   288,
     114,   115,   116,   117,    -1,    -1,    -1,   121,   297,    -1,
      -1,    -1,   301,    32,   303,    -1,    -1,    -1,    -1,    -1,
     811,    -1,   311,    -1,    -1,    -1,   315,   316,   317,    -1,
      -1,    -1,    -1,   322,   852,    -1,   759,    56,    57,    -1,
      -1,   330,   331,   332,    -1,   334,   335,   336,   162,   163,
     164,   842,    -1,    -1,    -1,    -1,   345,    -1,    -1,   173,
     851,   175,    -1,    -1,   102,   103,   104,   105,   106,    -1,
     852,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,   129,   130,   131,    -1,   114,    -1,    -1,    -1,    -1,
     389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   852,
      -1,    -1,    -1,   152,   153,   173,   425,   175,   157,    -1,
     159,   160,    -1,   181,   433,    -1,    -1,    -1,    -1,    -1,
      -1,   440,    -1,    -1,    -1,   444,   175,    -1,    -1,    -1,
      -1,    -1,    -1,   452,    -1,   184,    -1,    -1,    -1,    -1,
      -1,    -1,   461,    -1,   463,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   908,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   924,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,    -1,    -1,   503,   504,   505,   506,   507,   508,
      -1,   510,   511,    -1,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
      -1,   540,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     549,    -1,   102,   103,   104,   105,    -1,    -1,    20,    21,
      -1,    -1,   112,    -1,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
     579,    -1,    -1,    -1,   583,   584,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   603,   604,    -1,    -1,    -1,    -1,
     160,   161,   162,   163,   164,   614,    -1,    -1,   617,   618,
      56,    57,   621,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   656,   657,   121,
     122,   123,   124,   125,   126,    -1,    -1,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,   675,   676,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,   175,    -1,    -1,   152,   153,    -1,   181,
      -1,   157,   721,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     739,   740,    -1,    -1,    -1,    -1,    -1,   746,   184,   748,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   761,    -1,     1,    -1,   765,    -1,    -1,     6,
     769,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,   786,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    41,    -1,    -1,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   865,   104,   105,   868,
     869,   870,   871,   872,    -1,    -1,   875,   876,    -1,   878,
      -1,    -1,   119,   120,    -1,    -1,    -1,    -1,   887,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,   911,    -1,    -1,    -1,    -1,   916,    -1,    -1,
      -1,    -1,    -1,   160,   161,   162,    -1,   164,    -1,   928,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   179,   180,   181,   182,   183,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    14,    15,
      16,    17,    18,   177,    -1,   179,   180,   181,   182,   183,
      26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,   104,   105,    44,    -1,
      -1,    47,    -1,    49,   112,    51,   114,   115,   116,   117,
      56,    57,    -1,   121,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,   160,   161,   162,   163,   164,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    56,    57,    -1,   152,   153,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,    14,    15,    16,    17,    18,    -1,   175,
      -1,   177,    -1,   179,   180,    26,   182,    -1,   184,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,   114,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,   152,
     153,    92,    -1,    -1,   157,    -1,   159,   160,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,    -1,    32,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    56,    57,
      -1,   152,   153,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,    14,    15,
      16,    17,    18,    -1,   175,    -1,   177,    -1,   179,   180,
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
      -1,   167,   168,    14,    15,    16,    17,    18,    -1,   175,
      -1,   177,    -1,   179,   180,    26,   182,    -1,   184,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,   160,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,   175,    -1,   119,   120,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   128,    -1,    32,
      -1,    32,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    56,    57,    -1,    -1,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   177,    -1,   179,   180,
     181,   182,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
     113,   114,    -1,   114,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,   152,
     153,   152,   153,    -1,   157,    -1,   157,   160,   159,   160,
     104,   105,    -1,    32,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,   175,    -1,   175,   119,   120,    -1,    -1,    -1,
      -1,   184,    -1,   184,   128,    -1,    -1,    56,    57,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,   160,   161,   162,    26,
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
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
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
      -1,    -1,   184,    -1,    -1,    32,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,    14,
      15,    16,    17,    18,    -1,   175,    -1,   177,   178,   179,
     180,    26,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   152,   153,    92,    -1,    -1,
     157,    -1,   159,   160,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    32,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,   119,   120,    -1,   184,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    56,    57,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,   160,   161,   162,    26,   164,
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
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   160,
     161,   162,    26,   164,    -1,    -1,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    38,   176,   177,    41,   179,   180,
      44,   182,    -1,    47,    -1,    -1,    -1,    51,   114,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,   152,   153,    92,    -1,
      -1,   157,    -1,   159,   160,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    32,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    56,    57,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    21,    -1,   160,   161,   162,    26,
     164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   177,    41,   179,   180,    44,   182,    -1,
      47,    -1,    -1,    -1,    51,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,   152,   153,    92,    -1,    -1,   157,    -1,
     159,   160,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    56,    57,    -1,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   160,   161,   162,    26,   164,    -1,    -1,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     177,    41,   179,   180,    44,   182,    -1,    47,    -1,    -1,
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
      15,    16,    17,    18,    -1,    -1,    -1,   177,   178,   179,
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
      -1,    14,    15,    16,    17,    18,    -1,    -1,   158,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    18,    -1,    -1,    -1,   177,
     178,   179,   180,    26,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,    14,    15,    16,    17,    18,    -1,    -1,    -1,   177,
     178,   179,   180,    26,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,   160,   161,   162,
      26,   164,    -1,    -1,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   177,    41,   179,   180,    44,   182,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   179,   180,    -1,   182,    94,    95,    96,
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
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   173,    -1,   175,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    20,    21,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    20,    21,    -1,   121,    -1,
     123,   124,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    20,
      21,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,   175,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
      -1,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,   173,    30,
     175,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
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
     224,   239,   293,   298,   299,   303,   339,   341,   144,   215,
     216,   138,   201,   202,   144,   197,   198,   144,   182,   144,
     342,   340,    33,   128,   144,   217,   218,   219,   231,     4,
       5,     7,   302,    58,   292,   152,   151,   154,   151,    21,
      53,   163,   173,   199,   154,   182,   144,   296,   297,   292,
     144,   144,   144,   128,   177,   151,   176,    34,   104,   105,
     144,   223,   225,   226,   144,   301,   182,   294,    14,    15,
     138,   142,   144,   145,   177,   189,   215,   138,   202,   144,
     144,   144,    65,    69,    86,    87,    88,    89,   307,   295,
     152,   183,   296,   182,   182,   144,   216,   218,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   123,   124,   125,   126,
     129,   130,   131,   132,   133,   134,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   167,   168,    34,    34,
     177,   221,   182,   227,   154,   300,   304,   291,   144,   290,
     305,   146,   188,    14,    15,   138,   142,   144,   189,   213,
     214,   199,   182,   144,   183,    24,    30,    45,    59,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     231,   306,   308,   309,   311,   314,   144,   310,   144,   284,
     285,   178,   135,   178,   275,   280,   281,   154,   222,   229,
     231,   182,   183,   290,   151,   154,   157,   289,   146,   147,
     151,   178,   295,   152,   181,   158,   158,   158,   158,   177,
     158,   158,   158,   158,   158,   158,    32,    56,    57,   114,
     152,   153,   157,   160,   175,   181,   184,   181,   154,   181,
     216,    46,   287,   305,   178,   181,   314,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    25,    26,    27,
      28,    37,    38,    40,    41,    44,    47,    48,    50,    51,
      54,    55,    60,    70,    92,    93,   104,   105,   119,   120,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   145,
     160,   161,   162,   164,   167,   168,   177,   179,   180,   181,
     183,   191,   193,   203,   204,   207,   208,   209,   210,   211,
     212,   227,   228,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   248,   251,   254,   258,   259,
     260,   261,   262,   263,   264,   269,   272,   306,   343,   347,
     352,   354,   356,   278,   144,   314,   113,   122,   152,   288,
     213,   183,   272,   317,   319,   321,   315,   144,   312,   323,
     325,   327,   329,   331,   333,   335,   337,    32,    32,   157,
     175,   184,   176,   272,   144,   183,   314,   183,   284,   176,
     287,   113,   152,   154,   157,   286,   280,   272,   305,   314,
     343,   177,   158,   113,   181,   230,   260,   272,   227,   272,
     177,   272,   158,   158,   177,   181,   158,   138,    54,   272,
     227,   158,   113,   230,   272,   272,   272,   180,   258,   258,
      12,   314,    12,   314,   272,   349,   353,   190,   272,   272,
     272,   231,   272,   272,   272,   180,    36,   181,   181,   272,
     144,   177,   181,   181,   181,   290,   220,   118,   181,    20,
      21,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   121,   122,   123,   124,   125,
     126,   129,   130,   131,   132,   133,   134,   152,   153,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   173,
     175,   177,   175,   183,   220,   275,   277,   152,   181,   288,
     230,   272,   181,   314,   314,   314,   314,   178,   310,   314,
     221,   314,   221,   314,   221,   144,   282,   283,   314,   285,
     176,   176,   176,   314,   230,   272,   227,    19,   177,   231,
     249,   230,   272,   113,   152,   181,    13,   272,   227,   242,
     244,   272,   246,   154,   138,   314,   230,   272,   181,   180,
     305,    49,   144,   160,   177,   272,   344,   345,   346,   348,
     349,   350,   351,   305,   177,   345,   351,   127,   181,   183,
     147,   148,   188,   194,   178,   158,   231,   182,   227,   175,
     178,   252,   272,   135,   257,   258,    18,   144,   144,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   144,   119,   120,   272,   272,
     144,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   119,   120,   272,    21,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     144,   270,   272,   178,   252,   219,     7,    42,   276,   181,
     230,   272,   181,   159,   181,   159,   159,   159,   181,   159,
     222,   159,   222,   159,   222,   154,   159,   181,   159,   181,
     152,   286,   252,   178,   252,   158,   272,   314,   181,   227,
     178,   314,   314,   178,   314,   159,   181,    19,   258,   122,
     286,   178,   151,   181,   346,   176,   151,   176,   181,    19,
     178,   346,   183,   272,   349,   183,   272,   146,   192,   265,
     267,   229,    11,    22,    23,   205,   206,   144,   344,   151,
     178,   113,   122,   152,   157,   255,   256,   221,   158,   177,
     272,   272,   176,   272,   272,   144,   154,     1,   176,   178,
     176,    43,   276,   287,   181,   318,   314,   322,   316,   313,
     324,   159,   328,   159,   332,   159,   314,   336,   282,   338,
     272,   181,   178,   144,   178,   159,   159,   159,   159,   257,
     252,   272,   272,   345,   346,   144,   344,   176,   176,   272,
     176,   350,   252,   345,   183,   176,   149,   146,   314,   221,
     183,   227,   272,   176,   272,   144,   144,   144,   144,   151,
     176,   222,   273,   178,   252,   272,   271,   181,   279,   226,
     159,   326,   330,   334,   227,   159,   181,   250,   243,   245,
     247,   177,   181,   346,   176,   122,   286,   176,   181,   346,
     176,   159,   222,   227,   178,   255,   176,   127,   227,   253,
     314,   178,   225,   320,   272,   144,   272,   272,   272,   178,
     272,   272,   176,   176,   272,   272,   272,   183,   266,   159,
     206,   113,   272,   159,   181,   178,   159,   178,   181,   355,
     176,   355,   176,   231,   268,   272,   274,   272,    49,   176,
     183,   231,   178,   272,   176,   176
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
     247,   241,   249,   250,   248,   251,   251,   251,   252,   252,
     253,   253,   253,   254,   254,   254,   255,   255,   255,   255,
     256,   256,   257,   257,   258,   258,   259,   259,   259,   259,
     259,   259,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   261,   261,   261,   261,   262,   263,   263,   264,   265,
     266,   264,   267,   268,   264,   269,   270,   271,   269,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   273,   274,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   275,
     275,   276,   276,   277,   278,   278,   279,   278,   278,   278,
     280,   280,   281,   281,   282,   282,   283,   283,   284,   285,
     285,   286,   286,   287,   287,   287,   287,   287,   287,   288,
     288,   288,   289,   289,   290,   290,   290,   290,   290,   291,
     291,   292,   292,   293,   294,   293,   295,   295,   295,   296,
     297,   297,   298,   299,   299,   300,   300,   301,   302,   302,
     304,   303,   305,   305,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   308,   309,   309,   310,   310,
     312,   313,   311,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     315,   316,   314,   314,   317,   318,   314,   319,   320,   314,
     321,   322,   314,   314,   323,   324,   314,   325,   326,   314,
     314,   327,   328,   314,   329,   330,   314,   314,   331,   332,
     314,   333,   334,   314,   335,   336,   314,   337,   338,   314,
     340,   339,   342,   341,   343,   343,   343,   343,   344,   344,
     344,   344,   345,   345,   346,   346,   347,   347,   347,   347,
     347,   347,   348,   348,   349,   350,   350,   351,   351,   352,
     352,   353,   353,   354,   355,   355,   356,   356
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
       0,     7,     0,     0,     6,     5,     8,    10,     1,     3,
       1,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       1,     3,     0,     4,     1,     6,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     6,     5,     6,     3,     0,
       0,     8,     0,     0,     9,     3,     0,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     4,
       3,     1,     3,     4,     3,     4,     2,     4,     4,     7,
       8,     3,     5,     0,     0,     8,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       4,     0,     1,     3,     0,     3,     0,     7,     5,     5,
       2,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     1,     1,     2,     3,     5,     3,     3,     1,
       1,     1,     0,     1,     4,     6,     5,     5,     4,     0,
       2,     0,     1,     5,     0,     4,     0,     3,     5,     4,
       1,     2,     4,     5,     7,     0,     2,     2,     1,     1,
       0,     7,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       0,     0,     6,     1,     1,     1,     1,     4,     3,     4,
       2,     2,     3,     2,     3,     2,     2,     3,     3,     2,
       0,     0,     6,     2,     0,     0,     6,     0,     0,     8,
       0,     0,     6,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     0,     0,     6,     0,     0,     6,
       0,     7,     0,     7,     1,     1,     1,     1,     3,     3,
       5,     5,     1,     3,     0,     2,     6,     5,     7,     8,
       6,     8,     1,     3,     3,     1,     1,     1,     3,     5,
       5,     1,     3,     4,     0,     3,    10,    10
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
#line 3358 "ds_parser.cpp"
        break;

    case 188: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3364 "ds_parser.cpp"
        break;

    case 189: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3370 "ds_parser.cpp"
        break;

    case 190: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3376 "ds_parser.cpp"
        break;

    case 191: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3382 "ds_parser.cpp"
        break;

    case 193: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3388 "ds_parser.cpp"
        break;

    case 197: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3394 "ds_parser.cpp"
        break;

    case 203: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3400 "ds_parser.cpp"
        break;

    case 204: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3406 "ds_parser.cpp"
        break;

    case 206: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3412 "ds_parser.cpp"
        break;

    case 208: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3418 "ds_parser.cpp"
        break;

    case 209: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3424 "ds_parser.cpp"
        break;

    case 210: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3430 "ds_parser.cpp"
        break;

    case 211: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3436 "ds_parser.cpp"
        break;

    case 212: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3442 "ds_parser.cpp"
        break;

    case 213: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3448 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3454 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3460 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3466 "ds_parser.cpp"
        break;

    case 217: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3472 "ds_parser.cpp"
        break;

    case 218: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3478 "ds_parser.cpp"
        break;

    case 219: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3484 "ds_parser.cpp"
        break;

    case 220: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3490 "ds_parser.cpp"
        break;

    case 221: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3496 "ds_parser.cpp"
        break;

    case 222: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3502 "ds_parser.cpp"
        break;

    case 223: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3508 "ds_parser.cpp"
        break;

    case 227: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3514 "ds_parser.cpp"
        break;

    case 228: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3520 "ds_parser.cpp"
        break;

    case 229: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3526 "ds_parser.cpp"
        break;

    case 230: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3532 "ds_parser.cpp"
        break;

    case 231: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3538 "ds_parser.cpp"
        break;

    case 232: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3544 "ds_parser.cpp"
        break;

    case 233: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3550 "ds_parser.cpp"
        break;

    case 234: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3556 "ds_parser.cpp"
        break;

    case 235: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3562 "ds_parser.cpp"
        break;

    case 236: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3568 "ds_parser.cpp"
        break;

    case 237: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3574 "ds_parser.cpp"
        break;

    case 238: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3580 "ds_parser.cpp"
        break;

    case 240: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3586 "ds_parser.cpp"
        break;

    case 241: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3592 "ds_parser.cpp"
        break;

    case 248: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3598 "ds_parser.cpp"
        break;

    case 251: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3604 "ds_parser.cpp"
        break;

    case 252: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3610 "ds_parser.cpp"
        break;

    case 253: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3616 "ds_parser.cpp"
        break;

    case 255: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3622 "ds_parser.cpp"
        break;

    case 256: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3628 "ds_parser.cpp"
        break;

    case 257: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3634 "ds_parser.cpp"
        break;

    case 258: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3640 "ds_parser.cpp"
        break;

    case 259: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3646 "ds_parser.cpp"
        break;

    case 260: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3652 "ds_parser.cpp"
        break;

    case 261: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp"
        break;

    case 262: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 263: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 264: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3676 "ds_parser.cpp"
        break;

    case 269: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3682 "ds_parser.cpp"
        break;

    case 272: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3688 "ds_parser.cpp"
        break;

    case 275: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3694 "ds_parser.cpp"
        break;

    case 277: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3700 "ds_parser.cpp"
        break;

    case 278: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3706 "ds_parser.cpp"
        break;

    case 280: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3712 "ds_parser.cpp"
        break;

    case 281: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3718 "ds_parser.cpp"
        break;

    case 282: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3724 "ds_parser.cpp"
        break;

    case 283: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3730 "ds_parser.cpp"
        break;

    case 284: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3736 "ds_parser.cpp"
        break;

    case 285: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3742 "ds_parser.cpp"
        break;

    case 287: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3748 "ds_parser.cpp"
        break;

    case 290: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3754 "ds_parser.cpp"
        break;

    case 291: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3760 "ds_parser.cpp"
        break;

    case 295: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3766 "ds_parser.cpp"
        break;

    case 300: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3772 "ds_parser.cpp"
        break;

    case 305: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3778 "ds_parser.cpp"
        break;

    case 308: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3784 "ds_parser.cpp"
        break;

    case 309: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3790 "ds_parser.cpp"
        break;

    case 310: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3796 "ds_parser.cpp"
        break;

    case 311: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3802 "ds_parser.cpp"
        break;

    case 314: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3808 "ds_parser.cpp"
        break;

    case 343: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3814 "ds_parser.cpp"
        break;

    case 344: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3820 "ds_parser.cpp"
        break;

    case 345: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3826 "ds_parser.cpp"
        break;

    case 346: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3832 "ds_parser.cpp"
        break;

    case 347: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3838 "ds_parser.cpp"
        break;

    case 348: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3844 "ds_parser.cpp"
        break;

    case 349: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3850 "ds_parser.cpp"
        break;

    case 350: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3856 "ds_parser.cpp"
        break;

    case 351: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3862 "ds_parser.cpp"
        break;

    case 352: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3868 "ds_parser.cpp"
        break;

    case 353: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3874 "ds_parser.cpp"
        break;

    case 354: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3880 "ds_parser.cpp"
        break;

    case 355: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3886 "ds_parser.cpp"
        break;

    case 356: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3892 "ds_parser.cpp"
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
#line 4195 "ds_parser.cpp"
    break;

  case 16:
#line 474 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4201 "ds_parser.cpp"
    break;

  case 17:
#line 475 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4207 "ds_parser.cpp"
    break;

  case 18:
#line 479 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4213 "ds_parser.cpp"
    break;

  case 19:
#line 483 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4223 "ds_parser.cpp"
    break;

  case 20:
#line 488 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4237 "ds_parser.cpp"
    break;

  case 21:
#line 497 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4247 "ds_parser.cpp"
    break;

  case 22:
#line 505 "ds_parser.ypp"
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
#line 4265 "ds_parser.cpp"
    break;

  case 23:
#line 521 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4275 "ds_parser.cpp"
    break;

  case 24:
#line 526 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4285 "ds_parser.cpp"
    break;

  case 25:
#line 534 "ds_parser.ypp"
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
#line 4312 "ds_parser.cpp"
    break;

  case 26:
#line 555 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4324 "ds_parser.cpp"
    break;

  case 27:
#line 564 "ds_parser.ypp"
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
#line 4348 "ds_parser.cpp"
    break;

  case 29:
#line 590 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4356 "ds_parser.cpp"
    break;

  case 30:
#line 593 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4367 "ds_parser.cpp"
    break;

  case 31:
#line 599 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4378 "ds_parser.cpp"
    break;

  case 32:
#line 608 "ds_parser.ypp"
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
#line 4404 "ds_parser.cpp"
    break;

  case 33:
#line 629 "ds_parser.ypp"
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
#line 4429 "ds_parser.cpp"
    break;

  case 34:
#line 652 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4435 "ds_parser.cpp"
    break;

  case 35:
#line 653 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4441 "ds_parser.cpp"
    break;

  case 39:
#line 666 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4449 "ds_parser.cpp"
    break;

  case 40:
#line 669 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4457 "ds_parser.cpp"
    break;

  case 41:
#line 675 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4465 "ds_parser.cpp"
    break;

  case 42:
#line 681 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4473 "ds_parser.cpp"
    break;

  case 43:
#line 684 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4481 "ds_parser.cpp"
    break;

  case 44:
#line 690 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4487 "ds_parser.cpp"
    break;

  case 45:
#line 691 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4493 "ds_parser.cpp"
    break;

  case 46:
#line 695 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4499 "ds_parser.cpp"
    break;

  case 47:
#line 696 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4505 "ds_parser.cpp"
    break;

  case 48:
#line 697 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4516 "ds_parser.cpp"
    break;

  case 49:
#line 706 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4522 "ds_parser.cpp"
    break;

  case 50:
#line 707 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4528 "ds_parser.cpp"
    break;

  case 51:
#line 711 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4539 "ds_parser.cpp"
    break;

  case 52:
#line 720 "ds_parser.ypp"
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
#line 4557 "ds_parser.cpp"
    break;

  case 53:
#line 736 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4567 "ds_parser.cpp"
    break;

  case 54:
#line 744 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4579 "ds_parser.cpp"
    break;

  case 55:
#line 754 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4590 "ds_parser.cpp"
    break;

  case 56:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4596 "ds_parser.cpp"
    break;

  case 57:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4602 "ds_parser.cpp"
    break;

  case 58:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4608 "ds_parser.cpp"
    break;

  case 59:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4614 "ds_parser.cpp"
    break;

  case 60:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4620 "ds_parser.cpp"
    break;

  case 61:
#line 768 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4626 "ds_parser.cpp"
    break;

  case 62:
#line 772 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4636 "ds_parser.cpp"
    break;

  case 63:
#line 777 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4646 "ds_parser.cpp"
    break;

  case 64:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4652 "ds_parser.cpp"
    break;

  case 65:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4658 "ds_parser.cpp"
    break;

  case 66:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4664 "ds_parser.cpp"
    break;

  case 67:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4670 "ds_parser.cpp"
    break;

  case 68:
#line 789 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4676 "ds_parser.cpp"
    break;

  case 69:
#line 790 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4682 "ds_parser.cpp"
    break;

  case 70:
#line 791 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4688 "ds_parser.cpp"
    break;

  case 71:
#line 792 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4696 "ds_parser.cpp"
    break;

  case 72:
#line 798 "ds_parser.ypp"
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
#line 4714 "ds_parser.cpp"
    break;

  case 73:
#line 811 "ds_parser.ypp"
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
#line 4732 "ds_parser.cpp"
    break;

  case 74:
#line 827 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4738 "ds_parser.cpp"
    break;

  case 75:
#line 828 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4744 "ds_parser.cpp"
    break;

  case 76:
#line 832 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4756 "ds_parser.cpp"
    break;

  case 77:
#line 839 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4770 "ds_parser.cpp"
    break;

  case 78:
#line 851 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4779 "ds_parser.cpp"
    break;

  case 79:
#line 855 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4788 "ds_parser.cpp"
    break;

  case 80:
#line 862 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4794 "ds_parser.cpp"
    break;

  case 81:
#line 863 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4800 "ds_parser.cpp"
    break;

  case 82:
#line 867 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4806 "ds_parser.cpp"
    break;

  case 83:
#line 868 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4812 "ds_parser.cpp"
    break;

  case 84:
#line 869 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4818 "ds_parser.cpp"
    break;

  case 85:
#line 873 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4828 "ds_parser.cpp"
    break;

  case 86:
#line 878 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4837 "ds_parser.cpp"
    break;

  case 87:
#line 885 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4846 "ds_parser.cpp"
    break;

  case 88:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4852 "ds_parser.cpp"
    break;

  case 89:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4858 "ds_parser.cpp"
    break;

  case 90:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4864 "ds_parser.cpp"
    break;

  case 91:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4870 "ds_parser.cpp"
    break;

  case 92:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4876 "ds_parser.cpp"
    break;

  case 93:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4882 "ds_parser.cpp"
    break;

  case 94:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4888 "ds_parser.cpp"
    break;

  case 95:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4894 "ds_parser.cpp"
    break;

  case 96:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4900 "ds_parser.cpp"
    break;

  case 97:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4906 "ds_parser.cpp"
    break;

  case 98:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4912 "ds_parser.cpp"
    break;

  case 99:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4918 "ds_parser.cpp"
    break;

  case 100:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 4924 "ds_parser.cpp"
    break;

  case 101:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 4930 "ds_parser.cpp"
    break;

  case 102:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 4936 "ds_parser.cpp"
    break;

  case 103:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 4942 "ds_parser.cpp"
    break;

  case 104:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4948 "ds_parser.cpp"
    break;

  case 105:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4954 "ds_parser.cpp"
    break;

  case 106:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4960 "ds_parser.cpp"
    break;

  case 107:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4966 "ds_parser.cpp"
    break;

  case 108:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4972 "ds_parser.cpp"
    break;

  case 109:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4978 "ds_parser.cpp"
    break;

  case 110:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4984 "ds_parser.cpp"
    break;

  case 111:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4990 "ds_parser.cpp"
    break;

  case 112:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4996 "ds_parser.cpp"
    break;

  case 113:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5002 "ds_parser.cpp"
    break;

  case 114:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5008 "ds_parser.cpp"
    break;

  case 115:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5014 "ds_parser.cpp"
    break;

  case 116:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5020 "ds_parser.cpp"
    break;

  case 117:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5026 "ds_parser.cpp"
    break;

  case 118:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5032 "ds_parser.cpp"
    break;

  case 119:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5038 "ds_parser.cpp"
    break;

  case 120:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5044 "ds_parser.cpp"
    break;

  case 121:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5050 "ds_parser.cpp"
    break;

  case 122:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5056 "ds_parser.cpp"
    break;

  case 123:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5062 "ds_parser.cpp"
    break;

  case 124:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5068 "ds_parser.cpp"
    break;

  case 125:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5074 "ds_parser.cpp"
    break;

  case 126:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5080 "ds_parser.cpp"
    break;

  case 127:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5086 "ds_parser.cpp"
    break;

  case 128:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5092 "ds_parser.cpp"
    break;

  case 129:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5098 "ds_parser.cpp"
    break;

  case 130:
#line 934 "ds_parser.ypp"
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
#line 5122 "ds_parser.cpp"
    break;

  case 131:
#line 956 "ds_parser.ypp"
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
#line 5162 "ds_parser.cpp"
    break;

  case 132:
#line 994 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5171 "ds_parser.cpp"
    break;

  case 133:
#line 1001 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5180 "ds_parser.cpp"
    break;

  case 134:
#line 1005 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5193 "ds_parser.cpp"
    break;

  case 135:
#line 1016 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5199 "ds_parser.cpp"
    break;

  case 136:
#line 1017 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5205 "ds_parser.cpp"
    break;

  case 137:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5211 "ds_parser.cpp"
    break;

  case 138:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5217 "ds_parser.cpp"
    break;

  case 139:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5223 "ds_parser.cpp"
    break;

  case 140:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5229 "ds_parser.cpp"
    break;

  case 141:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5235 "ds_parser.cpp"
    break;

  case 142:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5241 "ds_parser.cpp"
    break;

  case 143:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5247 "ds_parser.cpp"
    break;

  case 144:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5253 "ds_parser.cpp"
    break;

  case 145:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5259 "ds_parser.cpp"
    break;

  case 146:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5265 "ds_parser.cpp"
    break;

  case 147:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5271 "ds_parser.cpp"
    break;

  case 148:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5277 "ds_parser.cpp"
    break;

  case 149:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5283 "ds_parser.cpp"
    break;

  case 150:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5289 "ds_parser.cpp"
    break;

  case 151:
#line 1032 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5295 "ds_parser.cpp"
    break;

  case 152:
#line 1033 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5301 "ds_parser.cpp"
    break;

  case 153:
#line 1034 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5307 "ds_parser.cpp"
    break;

  case 154:
#line 1038 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5317 "ds_parser.cpp"
    break;

  case 155:
#line 1043 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5328 "ds_parser.cpp"
    break;

  case 156:
#line 1049 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5336 "ds_parser.cpp"
    break;

  case 157:
#line 1055 "ds_parser.ypp"
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
#line 5375 "ds_parser.cpp"
    break;

  case 158:
#line 1089 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5383 "ds_parser.cpp"
    break;

  case 159:
#line 1092 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5391 "ds_parser.cpp"
    break;

  case 160:
#line 1098 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5397 "ds_parser.cpp"
    break;

  case 161:
#line 1099 "ds_parser.ypp"
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
#line 5413 "ds_parser.cpp"
    break;

  case 162:
#line 1110 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5419 "ds_parser.cpp"
    break;

  case 163:
#line 1114 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5427 "ds_parser.cpp"
    break;

  case 164:
#line 1120 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5435 "ds_parser.cpp"
    break;

  case 165:
#line 1123 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5443 "ds_parser.cpp"
    break;

  case 166:
#line 1126 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5452 "ds_parser.cpp"
    break;

  case 167:
#line 1130 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5460 "ds_parser.cpp"
    break;

  case 168:
#line 1136 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5466 "ds_parser.cpp"
    break;

  case 169:
#line 1140 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5472 "ds_parser.cpp"
    break;

  case 170:
#line 1144 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5480 "ds_parser.cpp"
    break;

  case 171:
#line 1147 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5488 "ds_parser.cpp"
    break;

  case 172:
#line 1150 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5498 "ds_parser.cpp"
    break;

  case 173:
#line 1155 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5506 "ds_parser.cpp"
    break;

  case 174:
#line 1158 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5516 "ds_parser.cpp"
    break;

  case 175:
#line 1166 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5524 "ds_parser.cpp"
    break;

  case 176:
#line 1169 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5534 "ds_parser.cpp"
    break;

  case 177:
#line 1174 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5542 "ds_parser.cpp"
    break;

  case 178:
#line 1177 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5552 "ds_parser.cpp"
    break;

  case 179:
#line 1185 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5560 "ds_parser.cpp"
    break;

  case 180:
#line 1191 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5566 "ds_parser.cpp"
    break;

  case 181:
#line 1192 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5572 "ds_parser.cpp"
    break;

  case 182:
#line 1196 "ds_parser.ypp"
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
#line 5608 "ds_parser.cpp"
    break;

  case 183:
#line 1230 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5614 "ds_parser.cpp"
    break;

  case 184:
#line 1230 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5620 "ds_parser.cpp"
    break;

  case 185:
#line 1230 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5628 "ds_parser.cpp"
    break;

  case 186:
#line 1233 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5634 "ds_parser.cpp"
    break;

  case 187:
#line 1233 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5640 "ds_parser.cpp"
    break;

  case 188:
#line 1233 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5650 "ds_parser.cpp"
    break;

  case 189:
#line 1238 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5656 "ds_parser.cpp"
    break;

  case 190:
#line 1238 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5662 "ds_parser.cpp"
    break;

  case 191:
#line 1238 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5672 "ds_parser.cpp"
    break;

  case 192:
#line 1246 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5678 "ds_parser.cpp"
    break;

  case 193:
#line 1246 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5684 "ds_parser.cpp"
    break;

  case 194:
#line 1246 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5692 "ds_parser.cpp"
    break;

  case 195:
#line 1252 "ds_parser.ypp"
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
#line 5707 "ds_parser.cpp"
    break;

  case 196:
#line 1262 "ds_parser.ypp"
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
#line 5723 "ds_parser.cpp"
    break;

  case 197:
#line 1273 "ds_parser.ypp"
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
#line 5740 "ds_parser.cpp"
    break;

  case 198:
#line 1288 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5748 "ds_parser.cpp"
    break;

  case 199:
#line 1291 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5756 "ds_parser.cpp"
    break;

  case 200:
#line 1297 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5762 "ds_parser.cpp"
    break;

  case 201:
#line 1298 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5774 "ds_parser.cpp"
    break;

  case 202:
#line 1305 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5787 "ds_parser.cpp"
    break;

  case 203:
#line 1316 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5793 "ds_parser.cpp"
    break;

  case 204:
#line 1317 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5799 "ds_parser.cpp"
    break;

  case 205:
#line 1318 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5805 "ds_parser.cpp"
    break;

  case 206:
#line 1322 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5811 "ds_parser.cpp"
    break;

  case 207:
#line 1323 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5817 "ds_parser.cpp"
    break;

  case 208:
#line 1324 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5823 "ds_parser.cpp"
    break;

  case 209:
#line 1325 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5829 "ds_parser.cpp"
    break;

  case 210:
#line 1329 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5839 "ds_parser.cpp"
    break;

  case 211:
#line 1334 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5849 "ds_parser.cpp"
    break;

  case 212:
#line 1342 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5855 "ds_parser.cpp"
    break;

  case 213:
#line 1343 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5861 "ds_parser.cpp"
    break;

  case 214:
#line 1347 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5871 "ds_parser.cpp"
    break;

  case 215:
#line 1353 "ds_parser.ypp"
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
#line 5936 "ds_parser.cpp"
    break;

  case 216:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5942 "ds_parser.cpp"
    break;

  case 217:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5948 "ds_parser.cpp"
    break;

  case 218:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5954 "ds_parser.cpp"
    break;

  case 219:
#line 1419 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5960 "ds_parser.cpp"
    break;

  case 220:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5966 "ds_parser.cpp"
    break;

  case 221:
#line 1421 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5972 "ds_parser.cpp"
    break;

  case 222:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5978 "ds_parser.cpp"
    break;

  case 223:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5984 "ds_parser.cpp"
    break;

  case 224:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5990 "ds_parser.cpp"
    break;

  case 225:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5996 "ds_parser.cpp"
    break;

  case 226:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6002 "ds_parser.cpp"
    break;

  case 227:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6008 "ds_parser.cpp"
    break;

  case 228:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6014 "ds_parser.cpp"
    break;

  case 229:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6020 "ds_parser.cpp"
    break;

  case 230:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6026 "ds_parser.cpp"
    break;

  case 231:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6032 "ds_parser.cpp"
    break;

  case 232:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6038 "ds_parser.cpp"
    break;

  case 233:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6044 "ds_parser.cpp"
    break;

  case 234:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6050 "ds_parser.cpp"
    break;

  case 235:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6056 "ds_parser.cpp"
    break;

  case 236:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6062 "ds_parser.cpp"
    break;

  case 237:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6068 "ds_parser.cpp"
    break;

  case 238:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6074 "ds_parser.cpp"
    break;

  case 239:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6080 "ds_parser.cpp"
    break;

  case 240:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6086 "ds_parser.cpp"
    break;

  case 241:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6092 "ds_parser.cpp"
    break;

  case 242:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6098 "ds_parser.cpp"
    break;

  case 243:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6104 "ds_parser.cpp"
    break;

  case 244:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6110 "ds_parser.cpp"
    break;

  case 245:
#line 1454 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6122 "ds_parser.cpp"
    break;

  case 246:
#line 1465 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6132 "ds_parser.cpp"
    break;

  case 247:
#line 1470 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6144 "ds_parser.cpp"
    break;

  case 248:
#line 1480 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6153 "ds_parser.cpp"
    break;

  case 249:
#line 1484 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6159 "ds_parser.cpp"
    break;

  case 250:
#line 1484 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6165 "ds_parser.cpp"
    break;

  case 251:
#line 1484 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6177 "ds_parser.cpp"
    break;

  case 252:
#line 1491 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6183 "ds_parser.cpp"
    break;

  case 253:
#line 1491 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6189 "ds_parser.cpp"
    break;

  case 254:
#line 1491 "ds_parser.ypp"
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
#line 6205 "ds_parser.cpp"
    break;

  case 255:
#line 1505 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6214 "ds_parser.cpp"
    break;

  case 256:
#line 1509 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6220 "ds_parser.cpp"
    break;

  case 257:
#line 1509 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6226 "ds_parser.cpp"
    break;

  case 258:
#line 1509 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6235 "ds_parser.cpp"
    break;

  case 259:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6241 "ds_parser.cpp"
    break;

  case 260:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6247 "ds_parser.cpp"
    break;

  case 261:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6253 "ds_parser.cpp"
    break;

  case 262:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6259 "ds_parser.cpp"
    break;

  case 263:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6265 "ds_parser.cpp"
    break;

  case 264:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6271 "ds_parser.cpp"
    break;

  case 265:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6277 "ds_parser.cpp"
    break;

  case 266:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6283 "ds_parser.cpp"
    break;

  case 267:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6289 "ds_parser.cpp"
    break;

  case 268:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6295 "ds_parser.cpp"
    break;

  case 269:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6301 "ds_parser.cpp"
    break;

  case 270:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6307 "ds_parser.cpp"
    break;

  case 271:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp"
    break;

  case 272:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6319 "ds_parser.cpp"
    break;

  case 273:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6325 "ds_parser.cpp"
    break;

  case 274:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6331 "ds_parser.cpp"
    break;

  case 275:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp"
    break;

  case 276:
#line 1533 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6343 "ds_parser.cpp"
    break;

  case 277:
#line 1534 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6349 "ds_parser.cpp"
    break;

  case 278:
#line 1535 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6355 "ds_parser.cpp"
    break;

  case 279:
#line 1536 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6361 "ds_parser.cpp"
    break;

  case 280:
#line 1537 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6367 "ds_parser.cpp"
    break;

  case 281:
#line 1538 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6373 "ds_parser.cpp"
    break;

  case 282:
#line 1539 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6379 "ds_parser.cpp"
    break;

  case 283:
#line 1540 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6385 "ds_parser.cpp"
    break;

  case 284:
#line 1541 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6391 "ds_parser.cpp"
    break;

  case 285:
#line 1542 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6397 "ds_parser.cpp"
    break;

  case 286:
#line 1543 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6403 "ds_parser.cpp"
    break;

  case 287:
#line 1544 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6409 "ds_parser.cpp"
    break;

  case 288:
#line 1545 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6415 "ds_parser.cpp"
    break;

  case 289:
#line 1546 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6421 "ds_parser.cpp"
    break;

  case 290:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6427 "ds_parser.cpp"
    break;

  case 291:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6433 "ds_parser.cpp"
    break;

  case 292:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6439 "ds_parser.cpp"
    break;

  case 293:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6445 "ds_parser.cpp"
    break;

  case 294:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6451 "ds_parser.cpp"
    break;

  case 295:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6457 "ds_parser.cpp"
    break;

  case 296:
#line 1553 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6463 "ds_parser.cpp"
    break;

  case 297:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6469 "ds_parser.cpp"
    break;

  case 298:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6475 "ds_parser.cpp"
    break;

  case 299:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6481 "ds_parser.cpp"
    break;

  case 300:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6487 "ds_parser.cpp"
    break;

  case 301:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6493 "ds_parser.cpp"
    break;

  case 302:
#line 1559 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6502 "ds_parser.cpp"
    break;

  case 303:
#line 1563 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6511 "ds_parser.cpp"
    break;

  case 304:
#line 1567 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6519 "ds_parser.cpp"
    break;

  case 305:
#line 1570 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6527 "ds_parser.cpp"
    break;

  case 306:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6533 "ds_parser.cpp"
    break;

  case 307:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6539 "ds_parser.cpp"
    break;

  case 308:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6545 "ds_parser.cpp"
    break;

  case 309:
#line 1576 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6559 "ds_parser.cpp"
    break;

  case 310:
#line 1585 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6573 "ds_parser.cpp"
    break;

  case 311:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 312:
#line 1595 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6587 "ds_parser.cpp"
    break;

  case 313:
#line 1598 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6593 "ds_parser.cpp"
    break;

  case 314:
#line 1598 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6599 "ds_parser.cpp"
    break;

  case 315:
#line 1598 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6607 "ds_parser.cpp"
    break;

  case 316:
#line 1601 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6616 "ds_parser.cpp"
    break;

  case 317:
#line 1605 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6625 "ds_parser.cpp"
    break;

  case 318:
#line 1609 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6634 "ds_parser.cpp"
    break;

  case 319:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6640 "ds_parser.cpp"
    break;

  case 320:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6646 "ds_parser.cpp"
    break;

  case 321:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6652 "ds_parser.cpp"
    break;

  case 322:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6658 "ds_parser.cpp"
    break;

  case 323:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6664 "ds_parser.cpp"
    break;

  case 324:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6670 "ds_parser.cpp"
    break;

  case 325:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6676 "ds_parser.cpp"
    break;

  case 326:
#line 1620 "ds_parser.ypp"
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
#line 6697 "ds_parser.cpp"
    break;

  case 327:
#line 1636 "ds_parser.ypp"
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
#line 6718 "ds_parser.cpp"
    break;

  case 328:
#line 1652 "ds_parser.ypp"
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
#line 6775 "ds_parser.cpp"
    break;

  case 329:
#line 1707 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6781 "ds_parser.cpp"
    break;

  case 330:
#line 1708 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6787 "ds_parser.cpp"
    break;

  case 331:
#line 1712 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6793 "ds_parser.cpp"
    break;

  case 332:
#line 1713 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6799 "ds_parser.cpp"
    break;

  case 333:
#line 1717 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6809 "ds_parser.cpp"
    break;

  case 334:
#line 1725 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6817 "ds_parser.cpp"
    break;

  case 335:
#line 1728 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6826 "ds_parser.cpp"
    break;

  case 336:
#line 1733 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6832 "ds_parser.cpp"
    break;

  case 337:
#line 1733 "ds_parser.ypp"
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
#line 6881 "ds_parser.cpp"
    break;

  case 338:
#line 1777 "ds_parser.ypp"
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
#line 6942 "ds_parser.cpp"
    break;

  case 339:
#line 1833 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 6953 "ds_parser.cpp"
    break;

  case 340:
#line 1842 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6963 "ds_parser.cpp"
    break;

  case 341:
#line 1847 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6973 "ds_parser.cpp"
    break;

  case 342:
#line 1855 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6979 "ds_parser.cpp"
    break;

  case 343:
#line 1856 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6985 "ds_parser.cpp"
    break;

  case 344:
#line 1860 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6993 "ds_parser.cpp"
    break;

  case 345:
#line 1863 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7004 "ds_parser.cpp"
    break;

  case 346:
#line 1872 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7010 "ds_parser.cpp"
    break;

  case 347:
#line 1873 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7016 "ds_parser.cpp"
    break;

  case 348:
#line 1877 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7027 "ds_parser.cpp"
    break;

  case 349:
#line 1886 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7033 "ds_parser.cpp"
    break;

  case 350:
#line 1887 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7039 "ds_parser.cpp"
    break;

  case 351:
#line 1892 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7045 "ds_parser.cpp"
    break;

  case 352:
#line 1893 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7051 "ds_parser.cpp"
    break;

  case 353:
#line 1897 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7062 "ds_parser.cpp"
    break;

  case 354:
#line 1903 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7073 "ds_parser.cpp"
    break;

  case 355:
#line 1909 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7081 "ds_parser.cpp"
    break;

  case 356:
#line 1912 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7090 "ds_parser.cpp"
    break;

  case 357:
#line 1916 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7101 "ds_parser.cpp"
    break;

  case 358:
#line 1922 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7112 "ds_parser.cpp"
    break;

  case 359:
#line 1931 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7118 "ds_parser.cpp"
    break;

  case 360:
#line 1932 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7124 "ds_parser.cpp"
    break;

  case 361:
#line 1933 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7130 "ds_parser.cpp"
    break;

  case 362:
#line 1937 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7136 "ds_parser.cpp"
    break;

  case 363:
#line 1938 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7142 "ds_parser.cpp"
    break;

  case 364:
#line 1942 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7150 "ds_parser.cpp"
    break;

  case 365:
#line 1945 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7160 "ds_parser.cpp"
    break;

  case 366:
#line 1950 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7170 "ds_parser.cpp"
    break;

  case 367:
#line 1955 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7183 "ds_parser.cpp"
    break;

  case 368:
#line 1963 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7196 "ds_parser.cpp"
    break;

  case 369:
#line 1974 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7204 "ds_parser.cpp"
    break;

  case 370:
#line 1977 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7213 "ds_parser.cpp"
    break;

  case 371:
#line 1984 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7219 "ds_parser.cpp"
    break;

  case 372:
#line 1985 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7225 "ds_parser.cpp"
    break;

  case 373:
#line 1989 "ds_parser.ypp"
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
#line 7257 "ds_parser.cpp"
    break;

  case 374:
#line 2016 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7263 "ds_parser.cpp"
    break;

  case 375:
#line 2016 "ds_parser.ypp"
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
#line 7293 "ds_parser.cpp"
    break;

  case 376:
#line 2044 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7301 "ds_parser.cpp"
    break;

  case 377:
#line 2047 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7315 "ds_parser.cpp"
    break;

  case 378:
#line 2056 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7329 "ds_parser.cpp"
    break;

  case 379:
#line 2069 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7343 "ds_parser.cpp"
    break;

  case 383:
#line 2090 "ds_parser.ypp"
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
#line 7359 "ds_parser.cpp"
    break;

  case 384:
#line 2101 "ds_parser.ypp"
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
#line 7376 "ds_parser.cpp"
    break;

  case 385:
#line 2116 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7382 "ds_parser.cpp"
    break;

  case 386:
#line 2117 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7388 "ds_parser.cpp"
    break;

  case 387:
#line 2121 "ds_parser.ypp"
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
#line 7427 "ds_parser.cpp"
    break;

  case 388:
#line 2158 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7433 "ds_parser.cpp"
    break;

  case 389:
#line 2159 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7439 "ds_parser.cpp"
    break;

  case 390:
#line 2163 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7445 "ds_parser.cpp"
    break;

  case 391:
#line 2163 "ds_parser.ypp"
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
#line 7531 "ds_parser.cpp"
    break;

  case 392:
#line 2247 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7543 "ds_parser.cpp"
    break;

  case 393:
#line 2254 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7554 "ds_parser.cpp"
    break;

  case 394:
#line 2263 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7560 "ds_parser.cpp"
    break;

  case 395:
#line 2264 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7566 "ds_parser.cpp"
    break;

  case 396:
#line 2265 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7572 "ds_parser.cpp"
    break;

  case 397:
#line 2266 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7578 "ds_parser.cpp"
    break;

  case 398:
#line 2267 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7584 "ds_parser.cpp"
    break;

  case 399:
#line 2268 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7590 "ds_parser.cpp"
    break;

  case 400:
#line 2269 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7596 "ds_parser.cpp"
    break;

  case 401:
#line 2270 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7602 "ds_parser.cpp"
    break;

  case 402:
#line 2271 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7608 "ds_parser.cpp"
    break;

  case 403:
#line 2272 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7614 "ds_parser.cpp"
    break;

  case 404:
#line 2273 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7620 "ds_parser.cpp"
    break;

  case 405:
#line 2274 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7626 "ds_parser.cpp"
    break;

  case 406:
#line 2275 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7632 "ds_parser.cpp"
    break;

  case 407:
#line 2276 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7638 "ds_parser.cpp"
    break;

  case 408:
#line 2277 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7644 "ds_parser.cpp"
    break;

  case 409:
#line 2278 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7650 "ds_parser.cpp"
    break;

  case 410:
#line 2279 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7656 "ds_parser.cpp"
    break;

  case 411:
#line 2280 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7662 "ds_parser.cpp"
    break;

  case 412:
#line 2281 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7668 "ds_parser.cpp"
    break;

  case 413:
#line 2282 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7674 "ds_parser.cpp"
    break;

  case 414:
#line 2283 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7680 "ds_parser.cpp"
    break;

  case 415:
#line 2284 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7686 "ds_parser.cpp"
    break;

  case 416:
#line 2285 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7692 "ds_parser.cpp"
    break;

  case 417:
#line 2286 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7698 "ds_parser.cpp"
    break;

  case 418:
#line 2287 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7704 "ds_parser.cpp"
    break;

  case 419:
#line 2291 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7710 "ds_parser.cpp"
    break;

  case 420:
#line 2292 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7716 "ds_parser.cpp"
    break;

  case 421:
#line 2293 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7722 "ds_parser.cpp"
    break;

  case 422:
#line 2294 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7728 "ds_parser.cpp"
    break;

  case 423:
#line 2295 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7734 "ds_parser.cpp"
    break;

  case 424:
#line 2296 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7740 "ds_parser.cpp"
    break;

  case 425:
#line 2300 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7753 "ds_parser.cpp"
    break;

  case 426:
#line 2311 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7762 "ds_parser.cpp"
    break;

  case 427:
#line 2315 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7774 "ds_parser.cpp"
    break;

  case 428:
#line 2325 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7786 "ds_parser.cpp"
    break;

  case 429:
#line 2332 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7797 "ds_parser.cpp"
    break;

  case 430:
#line 2341 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7803 "ds_parser.cpp"
    break;

  case 431:
#line 2341 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7809 "ds_parser.cpp"
    break;

  case 432:
#line 2341 "ds_parser.ypp"
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
#line 7824 "ds_parser.cpp"
    break;

  case 433:
#line 2354 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7830 "ds_parser.cpp"
    break;

  case 434:
#line 2355 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7836 "ds_parser.cpp"
    break;

  case 435:
#line 2356 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7842 "ds_parser.cpp"
    break;

  case 436:
#line 2357 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7848 "ds_parser.cpp"
    break;

  case 437:
#line 2358 "ds_parser.ypp"
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
#line 7867 "ds_parser.cpp"
    break;

  case 438:
#line 2372 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7878 "ds_parser.cpp"
    break;

  case 439:
#line 2378 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7887 "ds_parser.cpp"
    break;

  case 440:
#line 2382 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7896 "ds_parser.cpp"
    break;

  case 441:
#line 2386 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7906 "ds_parser.cpp"
    break;

  case 442:
#line 2391 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7916 "ds_parser.cpp"
    break;

  case 443:
#line 2396 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7926 "ds_parser.cpp"
    break;

  case 444:
#line 2401 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7936 "ds_parser.cpp"
    break;

  case 445:
#line 2406 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7945 "ds_parser.cpp"
    break;

  case 446:
#line 2410 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7954 "ds_parser.cpp"
    break;

  case 447:
#line 2414 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7964 "ds_parser.cpp"
    break;

  case 448:
#line 2419 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7973 "ds_parser.cpp"
    break;

  case 449:
#line 2423 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7983 "ds_parser.cpp"
    break;

  case 450:
#line 2428 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7989 "ds_parser.cpp"
    break;

  case 451:
#line 2428 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7995 "ds_parser.cpp"
    break;

  case 452:
#line 2428 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8006 "ds_parser.cpp"
    break;

  case 453:
#line 2434 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8018 "ds_parser.cpp"
    break;

  case 454:
#line 2441 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8024 "ds_parser.cpp"
    break;

  case 455:
#line 2441 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8030 "ds_parser.cpp"
    break;

  case 456:
#line 2441 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8040 "ds_parser.cpp"
    break;

  case 457:
#line 2446 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8046 "ds_parser.cpp"
    break;

  case 458:
#line 2446 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8052 "ds_parser.cpp"
    break;

  case 459:
#line 2446 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8063 "ds_parser.cpp"
    break;

  case 460:
#line 2452 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8069 "ds_parser.cpp"
    break;

  case 461:
#line 2452 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8075 "ds_parser.cpp"
    break;

  case 462:
#line 2452 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8085 "ds_parser.cpp"
    break;

  case 463:
#line 2457 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8094 "ds_parser.cpp"
    break;

  case 464:
#line 2461 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8100 "ds_parser.cpp"
    break;

  case 465:
#line 2461 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8106 "ds_parser.cpp"
    break;

  case 466:
#line 2461 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8116 "ds_parser.cpp"
    break;

  case 467:
#line 2466 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8122 "ds_parser.cpp"
    break;

  case 468:
#line 2466 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8128 "ds_parser.cpp"
    break;

  case 469:
#line 2466 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8142 "ds_parser.cpp"
    break;

  case 470:
#line 2475 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8151 "ds_parser.cpp"
    break;

  case 471:
#line 2479 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8157 "ds_parser.cpp"
    break;

  case 472:
#line 2479 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8163 "ds_parser.cpp"
    break;

  case 473:
#line 2479 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8173 "ds_parser.cpp"
    break;

  case 474:
#line 2484 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8179 "ds_parser.cpp"
    break;

  case 475:
#line 2484 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8185 "ds_parser.cpp"
    break;

  case 476:
#line 2484 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8199 "ds_parser.cpp"
    break;

  case 477:
#line 2493 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8208 "ds_parser.cpp"
    break;

  case 478:
#line 2497 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8214 "ds_parser.cpp"
    break;

  case 479:
#line 2497 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8220 "ds_parser.cpp"
    break;

  case 480:
#line 2497 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8230 "ds_parser.cpp"
    break;

  case 481:
#line 2502 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8236 "ds_parser.cpp"
    break;

  case 482:
#line 2502 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8242 "ds_parser.cpp"
    break;

  case 483:
#line 2502 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8256 "ds_parser.cpp"
    break;

  case 484:
#line 2511 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8262 "ds_parser.cpp"
    break;

  case 485:
#line 2511 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8268 "ds_parser.cpp"
    break;

  case 486:
#line 2511 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8279 "ds_parser.cpp"
    break;

  case 487:
#line 2517 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8285 "ds_parser.cpp"
    break;

  case 488:
#line 2517 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8291 "ds_parser.cpp"
    break;

  case 489:
#line 2517 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8302 "ds_parser.cpp"
    break;

  case 490:
#line 2526 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8308 "ds_parser.cpp"
    break;

  case 491:
#line 2526 "ds_parser.ypp"
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
#line 8325 "ds_parser.cpp"
    break;

  case 492:
#line 2541 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8331 "ds_parser.cpp"
    break;

  case 493:
#line 2541 "ds_parser.ypp"
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
#line 8352 "ds_parser.cpp"
    break;

  case 494:
#line 2561 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8358 "ds_parser.cpp"
    break;

  case 495:
#line 2562 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8364 "ds_parser.cpp"
    break;

  case 496:
#line 2563 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8370 "ds_parser.cpp"
    break;

  case 497:
#line 2564 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8376 "ds_parser.cpp"
    break;

  case 498:
#line 2568 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8388 "ds_parser.cpp"
    break;

  case 499:
#line 2575 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8400 "ds_parser.cpp"
    break;

  case 500:
#line 2582 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8411 "ds_parser.cpp"
    break;

  case 501:
#line 2588 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8422 "ds_parser.cpp"
    break;

  case 502:
#line 2597 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8432 "ds_parser.cpp"
    break;

  case 503:
#line 2602 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8441 "ds_parser.cpp"
    break;

  case 504:
#line 2609 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8447 "ds_parser.cpp"
    break;

  case 505:
#line 2610 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8453 "ds_parser.cpp"
    break;

  case 506:
#line 2614 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8464 "ds_parser.cpp"
    break;

  case 507:
#line 2620 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8476 "ds_parser.cpp"
    break;

  case 508:
#line 2627 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8489 "ds_parser.cpp"
    break;

  case 509:
#line 2635 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8501 "ds_parser.cpp"
    break;

  case 510:
#line 2642 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8514 "ds_parser.cpp"
    break;

  case 511:
#line 2650 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8528 "ds_parser.cpp"
    break;

  case 512:
#line 2662 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8536 "ds_parser.cpp"
    break;

  case 513:
#line 2665 "ds_parser.ypp"
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
#line 8552 "ds_parser.cpp"
    break;

  case 514:
#line 2679 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8563 "ds_parser.cpp"
    break;

  case 515:
#line 2688 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8569 "ds_parser.cpp"
    break;

  case 516:
#line 2689 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8575 "ds_parser.cpp"
    break;

  case 517:
#line 2693 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8585 "ds_parser.cpp"
    break;

  case 518:
#line 2698 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8594 "ds_parser.cpp"
    break;

  case 519:
#line 2705 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8604 "ds_parser.cpp"
    break;

  case 520:
#line 2710 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8616 "ds_parser.cpp"
    break;

  case 521:
#line 2720 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8626 "ds_parser.cpp"
    break;

  case 522:
#line 2725 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8635 "ds_parser.cpp"
    break;

  case 523:
#line 2732 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8649 "ds_parser.cpp"
    break;

  case 524:
#line 2744 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8655 "ds_parser.cpp"
    break;

  case 525:
#line 2745 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8661 "ds_parser.cpp"
    break;

  case 526:
#line 2749 "ds_parser.ypp"
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
#line 8684 "ds_parser.cpp"
    break;

  case 527:
#line 2767 "ds_parser.ypp"
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
#line 8707 "ds_parser.cpp"
    break;


#line 8711 "ds_parser.cpp"

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
#line 2787 "ds_parser.ypp"


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
