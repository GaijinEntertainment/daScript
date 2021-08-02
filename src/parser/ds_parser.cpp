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
#define YYLAST   7302

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  186
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  177
/* YYNRULES -- Number of rules.  */
#define YYNRULES  542
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  951

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
       0,   450,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   466,   467,   468,   472,   473,
     477,   494,   495,   499,   503,   508,   517,   525,   541,   546,
     554,   554,   584,   606,   610,   613,   619,   628,   649,   672,
     673,   677,   681,   682,   686,   689,   695,   701,   704,   710,
     711,   715,   716,   717,   726,   727,   731,   740,   756,   764,
     774,   783,   784,   785,   786,   787,   788,   792,   797,   805,
     806,   807,   808,   809,   810,   811,   812,   818,   832,   849,
     850,   851,   855,   863,   876,   880,   887,   888,   892,   893,
     894,   898,   901,   908,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     928,   929,   930,   931,   932,   933,   934,   935,   936,   937,
     938,   939,   940,   941,   942,   943,   944,   945,   946,   947,
     948,   949,   950,   951,   952,   953,   957,   979,   980,   981,
     985,  1024,  1031,  1035,  1046,  1047,  1048,  1049,  1050,  1051,
    1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,  1063,  1064,  1068,  1073,  1079,  1085,  1119,  1122,  1128,
    1129,  1140,  1144,  1150,  1153,  1156,  1160,  1166,  1170,  1174,
    1177,  1180,  1185,  1188,  1196,  1199,  1204,  1207,  1215,  1221,
    1222,  1226,  1260,  1260,  1260,  1263,  1263,  1263,  1268,  1268,
    1268,  1276,  1276,  1276,  1282,  1292,  1303,  1318,  1321,  1327,
    1328,  1335,  1346,  1347,  1348,  1352,  1353,  1354,  1355,  1359,
    1364,  1372,  1373,  1377,  1382,  1446,  1447,  1448,  1449,  1450,
    1451,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,
    1477,  1478,  1479,  1480,  1484,  1495,  1500,  1510,  1514,  1514,
    1514,  1521,  1521,  1521,  1535,  1539,  1539,  1539,  1546,  1547,
    1548,  1549,  1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,
    1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,
    1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,
    1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,
    1588,  1589,  1593,  1597,  1600,  1603,  1604,  1605,  1606,  1615,
    1624,  1625,  1628,  1628,  1628,  1631,  1635,  1639,  1643,  1644,
    1645,  1646,  1647,  1648,  1649,  1650,  1666,  1682,  1740,  1741,
    1745,  1746,  1750,  1758,  1761,  1766,  1765,  1810,  1866,  1875,
    1880,  1888,  1889,  1893,  1896,  1905,  1906,  1910,  1919,  1920,
    1925,  1926,  1930,  1936,  1942,  1945,  1949,  1955,  1964,  1965,
    1966,  1970,  1971,  1975,  1978,  1983,  1988,  1996,  2007,  2010,
    2017,  2018,  2022,  2023,  2024,  2028,  2056,  2056,  2086,  2089,
    2098,  2111,  2123,  2124,  2128,  2132,  2143,  2158,  2159,  2163,
    2200,  2201,  2205,  2206,  2207,  2212,  2211,  2296,  2303,  2312,
    2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,  2321,  2322,
    2323,  2324,  2325,  2326,  2327,  2328,  2329,  2330,  2331,  2332,
    2333,  2334,  2335,  2336,  2340,  2341,  2342,  2343,  2344,  2345,
    2349,  2360,  2364,  2374,  2381,  2390,  2390,  2390,  2403,  2404,
    2405,  2406,  2407,  2421,  2427,  2431,  2435,  2440,  2445,  2450,
    2455,  2459,  2463,  2468,  2472,  2477,  2477,  2477,  2483,  2490,
    2490,  2490,  2495,  2495,  2495,  2501,  2501,  2501,  2506,  2510,
    2510,  2510,  2515,  2515,  2515,  2524,  2528,  2528,  2528,  2533,
    2533,  2533,  2542,  2546,  2546,  2546,  2551,  2551,  2551,  2560,
    2560,  2560,  2566,  2566,  2566,  2575,  2575,  2590,  2590,  2610,
    2611,  2612,  2613,  2617,  2624,  2631,  2637,  2646,  2651,  2658,
    2659,  2663,  2669,  2676,  2684,  2691,  2699,  2711,  2714,  2728,
    2737,  2738,  2742,  2747,  2754,  2759,  2769,  2774,  2781,  2793,
    2794,  2798,  2816
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
  "optional_public_or_private_module", "module_name", "module_declaration",
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
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@2", "$@3", "$@4", "$@5",
  "$@6", "$@7", "expr_type_decl", "$@8", "$@9", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@10", "$@11",
  "$@12", "$@13", "expr_field", "$@14", "$@15", "expr", "$@16", "$@17",
  "optional_field_annotation", "optional_override",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@18", "function_argument_declaration", "function_argument_list",
  "tuple_type", "tuple_type_list", "variant_type", "variant_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@19", "enum_list",
  "single_alias", "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "class_or_struct",
  "optional_public_or_private_structure", "structure_declaration", "$@20",
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

#define YYPACT_NINF -595

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-595)))

#define YYTABLE_NINF -497

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -595,    33,  -595,  -595,   -89,   -76,   -30,   -25,   -19,  -595,
     -99,  -595,  -595,    10,  -595,  -595,  -595,  -595,  -595,   395,
    -595,   110,  -595,  -595,  -595,  -595,  -595,  -595,    29,  -595,
      45,    47,    62,  -595,  -595,    20,  -595,   -18,    84,  -595,
    -595,   110,   100,   103,  -595,  -595,   137,   165,   123,  -595,
      46,  -595,  -595,  -595,   221,   253,  -595,   288,    13,   -89,
     204,   -76,   210,  -595,   213,   243,  -595,   245,  -595,   193,
    -595,   -88,   315,   183,   187,  -595,   260,   -89,    10,  -595,
    -595,  -595,     6,   207,  -595,  -595,  -595,   270,  -595,  -595,
     209,  -595,  -595,  -595,  -595,  -595,   282,   130,  -595,  -595,
    -595,  -595,   377,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,   248,   -61,  7046,  -595,  -595,  -595,  -595,  -595,   287,
     291,  -595,    38,  -595,  7133,   399,   403,  -595,   262,  -595,
    -595,   286,  -595,  -595,   299,  -595,   273,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,    40,  -595,  -595,   -83,  -595,   290,
     292,   293,   294,  -595,  -595,  -595,   267,  -595,  -595,  -595,
    -595,  -595,   295,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,   296,  -595,  -595,  -595,   297,   300,  -595,  -595,
    -595,  -595,   306,   310,  -595,  -595,  -595,  -595,  -595,   281,
    -595,   264,   303,  -595,   265,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,   -84,   316,
    1196,   -24,  -595,   305,   -54,  -595,  -595,   246,  -595,  -595,
     130,  -595,   -44,  4343,  -595,  -595,  -595,  -595,  -595,   325,
    -595,   170,   189,   201,  -595,  -595,  -595,  -595,  -595,  -595,
     418,  -595,  -595,    50,  2717,  -595,  -595,   -38,  7046,   -27,
     -89,  -595,    -9,  -595,  -108,  7046,  -595,  -595,  4343,  -595,
    -595,   299,  -595,  -595,  6960,   298,   314,  1851,  -595,  -595,
     207,  4343,   307,  4343,   324,   327,   319,  -595,   311,   335,
     336,  2855,   207,  -595,   341,  2009,  4343,  4343,   233,   233,
    6788,  6874,  4343,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    4343,  4343,  4343,   -24,  4343,  4343,  4343,  -595,   320,  -595,
     438,  -595,  -595,   322,   326,  4343,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,   -92,   328,  -595,   329,   332,  -595,
    -595,  -595,   299,  -595,  -595,  -595,  -595,   330,  -595,  -595,
     -72,  -595,  -595,  -595,  -595,  -595,  6060,   331,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,   357,  7046,  -595,
     -56,  -595,  -595,  5026,  7046,  7046,  7046,  7046,   337,   287,
    7046,   262,  7046,   262,  7046,   262,  7132,   291,  -595,  -595,
    -595,   340,  -595,  -595,  5722,  -595,  -595,  3831,  -595,  -595,
     106,   299,  -595,   -65,  -595,   371,  3831,  1012,    15,   866,
    -595,   -24,  -595,  2147,  -595,  -595,   401,  4432,   505,  6352,
    4343,  1012,  -595,  -595,  4343,  -595,  -595,   366,   383,  6352,
    -595,  7046,  2147,  -595,  4531,   440,   440,   342,  -595,  -595,
     299,   920,   299,  1539,  6127,  -595,   145,   277,   440,   440,
     -41,  -595,   440,   440,  5174,    96,   344,  -595,  -595,  1012,
    -595,  2285,  -595,  -595,  -595,  -595,   388,   233,  -595,     8,
     384,  4343,  4343,  4343,  4343,  4343,  4343,  4343,  4343,  4343,
    4343,  -595,  -595,  4343,  4343,  4343,  4343,  4343,  4343,   386,
    2441,  4343,   387,  4343,  4343,  4343,  4343,  4343,  4343,  4343,
    4343,  4343,  4343,  4343,  4343,  4343,  4343,  2579,  2993,  4343,
    4343,  4343,  4343,  4343,  4343,  4343,  4343,  4343,  4343,   396,
    4343,  3131,    12,  -595,   411,  -595,  -595,  -595,  2147,  -595,
    1482,  1633,  1793,  1950,  -595,    77,  2085,   316,  2223,   316,
    2379,   316,   -49,  -595,    78,  3831,   118,  -595,  -595,   353,
    -595,  -595,  -595,  7046,  -595,  2147,  -595,  -595,  4343,  3287,
    3443,  7046,  -595,  4630,  4343,  4343,  -595,   207,  5251,  -595,
    7046,  7046,  5331,  7046,  -595,  -595,  2517,  -595,  4729,  -595,
    -595,    19,   233,   -31,  1695,  3581,  6127,   381,    -5,   363,
     391,  -595,  -595,   154,    26,  3737,    -5,   194,  4343,  4343,
     360,  -595,  4343,   400,   404,  -595,   225,  -595,  -595,   317,
     407,  -595,    75,  6352,   -64,   262,  -595,   389,  -595,  -595,
    6352,  6352,  6352,  6352,  6352,  6352,  6352,  6352,    88,    88,
     374,  6352,  6352,   374,   247,   247,   376,  4343,  4343,  6352,
      11,  -595,  5797,   -62,   -62,  6352,    88,    88,  6352,  6352,
    6498,  6419,   554,  6352,  6352,  6352,  4343,  4343,  6352,   414,
    6206,  6525,  6604,   751,   374,   374,    57,    57,    11,    11,
      11,  -595,   560,  5872,  -595,    82,    10,  -595,   556,   524,
     397,   418,  -595,  2147,  -595,  4828,  -595,  7046,  -595,  -595,
    -595,   422,  -595,   410,  -595,   413,  -595,   421,  7046,  -595,
    7132,  -595,   291,  -595,  2655,  -595,  5960,   -50,  -595,    92,
     426,  5408,  2793,  -595,  -595,  -595,  2931,  3069,  -595,  3225,
     388,  -595,  4343,  -595,  4343,  4343,    -7,   437,   407,   406,
     408,  4343,   409,  4343,  4343,   407,   405,   415,  6352,  -595,
    -595,  6273,  -595,   441,  7046,   262,  1380,   207,  -595,  -595,
    4343,  -595,    27,   125,  4343,  -595,   439,   446,   449,   450,
    -595,   144,   316,  -595,  3893,  6671,  6352,  -595,  6671,  6352,
    -595,  4343,  -595,  -595,  -595,   146,   385,  -595,   299,  -595,
    -595,  4927,  -595,  -595,  3381,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  3831,  -595,  -595,  -595,   418,  4343,
     207,  -595,   132,  -595,  -595,  -595,  -595,  -595,   419,   -33,
    6352,  6352,    -5,   423,   117,   381,   424,  -595,  6352,  -595,
    -595,   -21,    -5,   425,  -595,  -595,  -595,  3519,   316,  -595,
    -595,  1012,   427,  6352,  -595,  -595,  -595,  -595,   -64,   430,
     -80,  7046,  -595,   105,  6352,  -595,   428,  -595,   221,  -595,
    -595,  -595,  -595,  -595,  -595,  6352,  -595,  4343,   459,  -595,
    4343,  4343,  4343,  4049,  4343,   431,   435,  4343,  4343,  -595,
    4343,   436,  -595,  -595,   454,   317,  -595,  -595,  -595,  4205,
    -595,  -595,  3675,  -595,  -595,   221,  -595,  -595,  5488,   461,
     374,   374,   374,  -595,  5565,  5100,   445,  -595,  6352,  6352,
    5100,   448,   -24,  -595,  -595,  4343,  6352,  -595,   444,  -595,
    4343,  -595,   574,   451,  -595,   443,  -595,  -595,   -24,  6352,
    -595,  -595,  5645,  4343,   453,   458,  -595,  -595,  6352,  -595,
    -595
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    86,     1,   189,     0,     0,     0,     0,     0,   190,
       0,   507,   505,     0,    14,     3,    10,     9,     8,     0,
       7,   380,     6,    11,     5,     4,    12,    13,    75,    77,
      32,    44,    41,    42,    34,    39,    33,     0,     0,    19,
      18,   380,     0,     0,    80,    81,     0,   169,    82,    84,
       0,    79,   401,   400,   137,   402,   381,   382,     0,     0,
       0,     0,     0,    40,     0,     0,    37,     0,   388,     0,
     392,     0,    15,     0,     0,   171,     0,     0,     0,    87,
     139,   138,     0,     0,   136,   404,   403,     0,   384,   383,
     386,    73,    74,    71,    72,    70,     0,     0,    69,    78,
      45,    43,    39,    36,    35,   434,   437,   435,   438,   436,
     439,     0,     0,     0,   394,   393,    16,    17,    20,     0,
       0,   170,     0,    85,     0,     0,     0,    93,    88,   163,
     141,   397,   405,   378,     0,    21,     0,    65,    66,    63,
      64,    62,    61,    67,     0,    38,   388,     0,   395,     0,
       0,     0,     0,   409,   429,   410,   441,   411,   415,   416,
     417,   418,   433,   422,   423,   424,   425,   426,   427,   428,
     430,   431,   478,   414,   421,   432,   485,   492,   412,   419,
     413,   420,     0,     0,   440,   448,   451,   449,   450,     0,
     443,     0,     0,   358,     0,    83,    96,    97,    99,    98,
     100,   101,   102,   103,   128,   129,   126,   127,   119,   130,
     131,   120,   117,   118,   132,   133,   134,   135,   107,   108,
     109,   104,   105,   106,   122,   123,   121,   115,   116,   111,
     110,   112,   113,   114,    95,    94,   124,   125,   338,    91,
       0,     0,   399,     0,     0,   407,   387,   371,    22,    23,
       0,    76,     0,     0,   389,   469,   472,   475,   465,     0,
     445,   479,   486,   493,   499,   502,   456,   461,   455,   468,
       0,   464,   458,     0,     0,   391,   460,     0,     0,     0,
       0,    89,     0,   351,     0,     0,   140,   165,     0,    54,
      55,     0,   274,   275,     0,     0,     0,     0,   268,   177,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   227,   226,   228,   229,   230,    24,
       0,     0,     0,     0,     0,     0,     0,   212,   213,   144,
     142,   272,   271,     0,     0,     0,   158,   153,   151,   150,
     152,   223,   164,   145,   269,     0,   331,     0,     0,   156,
     157,   159,     0,   149,   330,   329,   328,    86,   334,   270,
       0,   146,   333,   332,   310,   276,   231,     0,   273,   509,
     510,   511,   512,   398,   343,   385,   379,     0,     0,   372,
       0,    68,   396,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    88,     0,    88,     0,     0,   463,   457,
     459,     0,   462,   453,     0,   444,   508,   357,   506,   359,
       0,     0,   349,   362,    90,   338,    92,     0,     0,   173,
     176,     0,   201,     0,   179,   182,     0,   231,     0,   172,
       0,     0,   192,   195,     0,   162,   198,     0,     0,    48,
      58,     0,     0,   186,   231,   302,   303,   213,   167,   168,
       0,   519,     0,     0,     0,   536,     0,     0,   280,   279,
     315,    30,   278,   277,     0,   214,     0,   160,   161,     0,
     337,     0,   148,   154,   155,   191,   221,     0,   147,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,   338,   408,     0,   369,   370,   368,     0,   390,
       0,     0,     0,     0,   442,     0,     0,    91,     0,    91,
       0,    91,   169,   355,     0,   353,     0,   454,   452,     0,
     350,   361,   360,     0,   363,     0,   352,    59,     0,     0,
       0,     0,   183,   231,     0,     0,   180,     0,     0,    60,
       0,     0,     0,     0,    46,    47,     0,   187,   231,   184,
     214,     0,     0,   169,     0,     0,   527,   517,   519,     0,
     530,   531,   532,     0,     0,     0,   519,     0,     0,     0,
       0,    27,     0,    25,     0,   306,   258,   257,   163,    51,
       0,   311,     0,   207,     0,    88,   166,     0,   325,   326,
     241,   242,   244,   243,   245,   235,   236,   237,   281,   282,
     294,   246,   247,   295,   292,   293,     0,     0,     0,   233,
     320,   309,     0,   335,   336,   234,   283,   284,   248,   249,
     299,   300,   301,   238,   239,   240,     0,     0,   232,     0,
       0,   297,   298,   296,   290,   291,   286,   285,   287,   288,
     289,   264,     0,     0,   313,     0,     0,   406,     0,   340,
       0,   368,   373,     0,   377,   231,   470,     0,   476,   466,
     446,     0,   480,     0,   487,     0,   494,     0,     0,   500,
       0,   503,     0,   339,   364,   367,   366,     0,   174,     0,
       0,     0,     0,   181,   188,   316,     0,     0,   317,     0,
     221,   185,     0,   520,     0,     0,   519,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   529,   537,
     538,     0,    28,    31,     0,    88,     0,     0,    49,    50,
       0,    56,     0,     0,     0,   312,     0,     0,     0,     0,
     219,     0,    91,   322,     0,   252,   253,   308,   250,   251,
     327,     0,   266,   307,   314,     0,   340,   341,     0,   344,
     375,   231,   376,   471,     0,   477,   467,   447,   481,   483,
     488,   490,   495,   497,   354,   501,   356,   504,   360,     0,
       0,   175,     0,   204,   202,   193,   196,   199,     0,     0,
     514,   513,   519,     0,     0,   518,     0,   522,   528,   534,
     533,     0,   519,     0,   535,    26,    29,     0,    91,   143,
      52,     0,     0,   208,   217,   218,   216,   215,     0,     0,
       0,     0,   255,     0,   321,   267,    87,   345,   137,   342,
     374,   473,   484,   491,   498,   365,    57,     0,     0,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
       0,     0,   525,   259,     0,    51,   254,   220,   222,     0,
     209,   224,     0,   256,   348,   137,   347,   474,     0,     0,
     194,   197,   200,   318,     0,   539,     0,   523,   516,   515,
     539,     0,     0,   262,    53,     0,   210,   323,     0,   205,
       0,   319,     0,     0,   524,     0,   526,   260,     0,   211,
     324,   346,     0,     0,     0,     0,   263,   206,   540,   541,
     542
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -595,  -595,  -595,  -595,  -595,   164,   581,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,   539,  -595,  -595,   582,
    -595,  -595,  -595,  -253,  -595,  -595,  -595,  -595,  -595,  -595,
     394,  -595,   586,   -67,  -595,   568,   -48,  -343,  -388,  -543,
    -595,  -595,  -595,  -256,  -218,   -82,  -595,    23,  -286,   -13,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,   651,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -518,  -595,  -595,  -205,  -595,   -86,  -298,  -595,  -238,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
     302,  -595,  -595,   114,  -134,  -595,  -595,  -595,   238,  -595,
     -52,  -595,  -274,   266,  -415,  -412,   131,  -595,  -219,  -595,
     633,  -595,  -595,  -595,   531,   609,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -279,  -109,  -595,  -595,  -595,
     283,  -595,  -595,  -595,   -78,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,   393,  -594,  -456,  -586,  -595,  -595,  -316,
     -63,   218,  -595,  -595,  -595,  -232,  -595
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   118,    41,    15,   136,   142,   467,   341,   763,
     342,   624,    16,    17,    35,    36,    66,    18,    32,    33,
     343,   344,   770,   771,   345,   346,   347,   348,   349,   350,
     143,   144,    29,    30,    48,    49,    50,    19,   239,   286,
     128,    20,    82,    83,    84,   351,   352,   240,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     590,   880,   591,   881,   593,   882,   365,   581,   879,   366,
     632,   901,   367,   780,   781,   635,   368,   369,   436,   371,
     372,   373,   374,   764,   922,   765,   938,   375,   692,   865,
     633,   861,   940,   282,   798,   700,   542,   905,   283,   284,
     563,   564,   193,   194,   745,   422,   548,   390,   246,   244,
      57,    90,    22,   134,   112,    70,    71,    23,    24,   242,
     132,    55,    87,    25,   243,   247,   377,   111,   186,   187,
     191,   188,   399,   807,   565,   397,   806,   394,   803,   395,
     907,   396,   805,   400,   808,   401,   872,   402,   810,   403,
     873,   404,   812,   405,   874,   406,   815,   407,   817,    26,
      43,    27,    42,   378,   607,   608,   609,   379,   610,   611,
     612,   613,   380,   466,   381,   933,   382
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      51,   130,   370,   423,   185,   419,   465,   616,   575,   570,
     122,   435,   428,   557,   713,   559,   715,   561,   717,   -86,
     458,   459,   749,   695,   486,   386,   637,    91,    92,   453,
     756,   489,   490,     2,   578,   189,   773,   421,   742,     3,
     124,    62,   602,    44,   602,   754,    39,   487,   899,   571,
     776,   509,   280,   480,   512,   513,    28,    69,   545,   777,
     727,   729,     4,    31,     5,    51,     6,   546,     7,    45,
     253,   424,     8,    63,   425,   512,   513,   489,   490,     9,
      76,    40,   409,   571,   147,    10,   481,   387,   572,   778,
     573,   245,   744,   574,   779,   281,   114,   547,    76,   254,
     184,   147,   774,   129,    11,    46,   718,   415,   489,   490,
     488,   125,   126,   539,   540,    34,   501,   502,   192,   774,
      37,    47,   572,   148,   509,    12,   511,   512,   513,   514,
     385,   774,   820,   515,   539,   540,   245,    67,   772,    46,
     392,   571,   423,   485,   137,   138,   416,   582,   280,   884,
     744,   127,    93,   638,   835,    47,    94,   418,    95,    96,
     833,   890,   501,   502,    38,    68,   597,   387,    56,   185,
     509,   387,   511,   512,   513,   514,   185,   748,   387,   515,
     572,   601,    58,   614,    64,   185,   539,   540,   696,   636,
      59,    97,   250,   501,   502,    65,   697,    59,    78,   698,
     417,   509,    60,   511,   512,   513,   514,   426,   410,    13,
     515,   185,   185,   420,    61,    14,   429,   195,   438,   251,
     536,   537,   538,    79,   829,    46,   411,   774,   383,    69,
     450,   571,   539,   540,   774,   412,   841,   710,   719,   860,
     887,    47,   461,   463,   774,    73,   885,   782,    74,   534,
     535,   536,   537,   538,   775,   626,   891,   774,    59,   711,
     720,   794,   704,   539,   540,   184,   863,   489,   490,   139,
     572,   821,   184,   140,    80,   141,    96,   747,   721,   185,
      81,   184,    75,   569,   903,   185,   185,   185,   185,   725,
     832,   185,   877,   185,    76,   185,   858,   185,    78,   842,
     722,    77,   852,   759,   743,   894,    85,   184,   184,   819,
     544,   105,    86,   266,   878,   106,   550,   551,   552,   553,
     471,   859,   556,   866,   558,  -482,   560,   619,   767,   620,
    -482,   752,   107,   108,   109,   110,   753,   267,   268,   768,
     769,    88,   185,   100,  -489,   577,   113,    89,  -482,  -489,
     499,   500,   501,   502,   503,   102,  -496,   506,   103,   589,
     509,  -496,   511,   512,   513,   514,   119,  -489,   116,   515,
     120,   517,   518,   596,   117,   184,   753,   848,   757,  -496,
    -261,   184,   184,   184,   184,  -261,   869,   184,   104,   184,
     129,   184,   133,   184,   489,   490,   269,   629,   387,    52,
      53,   388,    54,  -261,   389,   121,   532,   533,   534,   535,
     536,   537,   538,   337,   457,   131,   129,   800,   580,   888,
     248,   249,   539,   540,   135,   621,   622,   797,   867,   135,
      63,   146,   190,   236,   270,   271,   192,   237,   184,   272,
     238,   241,   273,   266,   245,   259,   277,   279,   419,   255,
     408,   256,   257,   258,   260,   261,   262,   274,   278,   263,
     489,   490,   627,   275,   185,   264,   276,   267,   268,   265,
     398,   285,   185,   432,   476,   447,   431,   499,   500,   501,
     502,   185,   185,   442,   185,   440,   443,   509,   384,   511,
     512,   513,   514,   445,   446,   724,   515,   444,   517,   518,
     451,   475,   543,   732,   477,   734,    13,   280,   478,   541,
     482,   483,   736,   737,   484,   739,   554,   567,   587,   423,
     487,   594,   595,   600,   634,   545,   269,   628,   370,   639,
     723,   656,   661,   747,   546,   534,   535,   536,   537,   538,
     750,   691,   376,   751,   760,   501,   502,   248,   783,   539,
     540,   762,   772,   509,   784,   393,   512,   513,   514,   790,
     184,   792,   515,   796,   701,   271,   797,   415,   184,   272,
     809,   822,   273,   811,   489,   490,   414,   184,   184,   799,
     184,   813,   834,   836,   854,   837,   839,   274,   846,   843,
     427,   855,   844,   702,   856,   857,   276,   883,   185,   437,
     886,   889,   892,   439,   909,   441,   896,   898,   916,   185,
     904,   185,   917,   449,   923,   539,   540,   454,   455,   456,
     921,   930,   934,   943,   464,   936,   941,   945,   944,   804,
     949,   623,   468,   469,   470,   950,   472,   473,   474,    98,
     814,   145,   924,   101,   391,    99,   123,   479,   795,   928,
     906,   766,    21,   897,   828,   185,   699,   499,   500,   501,
     502,   503,   868,   576,   506,   507,   508,   509,   816,   511,
     512,   513,   514,   566,    72,   703,   515,   252,   517,   518,
     115,   617,   555,    51,   521,   850,   847,   430,   935,     0,
     840,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,   184,     0,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   539,
     540,     0,     0,     0,     0,   583,     0,     0,   876,     0,
       0,     0,   588,     0,     0,     0,   592,     0,     0,     0,
       0,   184,   185,     0,   598,     0,     0,     0,     0,     0,
       0,     0,     0,   606,     0,   606,     0,     0,     0,   895,
       0,   489,   490,     0,     0,     0,     0,     0,   900,     0,
       0,     0,     0,   902,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,     0,     0,   650,   651,   652,   653,   654,
     655,     0,   659,   660,     0,   662,   663,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   678,
     680,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,     0,   693,     0,     0,     0,     0,     0,   184,     0,
     705,     0,     0,     0,   499,   500,   501,   502,   503,     0,
       0,   506,   507,   508,   509,     0,   511,   512,   513,   514,
       0,     0,     0,   515,     0,   517,   518,   726,     0,     0,
       0,     0,   731,     0,     0,     0,   659,   678,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,     0,     0,   468,   474,     0,   937,
     532,   533,   534,   535,   536,   537,   538,   474,     0,     0,
     758,   464,   267,   268,   761,   946,   539,   540,     0,     0,
       0,     0,     0,     0,   292,   293,   294,   295,   296,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,     0,   266,     0,     0,     0,     0,     0,   302,   785,
     786,   304,     0,     0,   305,     0,     0,   306,     0,   602,
       0,   309,     0,     0,     0,     0,   267,   268,   788,   789,
       0,   269,   153,   154,   155,     0,   157,   158,   159,   160,
     161,   313,   163,   164,   165,   166,   167,   168,   169,   170,
     171,     0,   173,   174,   175,   801,     0,   178,   179,   180,
     181,     0,     0,   314,     0,     0,     0,     0,     0,   270,
     271,     0,     0,     0,   272,   316,   317,   273,     0,     0,
       0,     0,   489,   490,     0,   269,     0,     0,     0,     0,
       0,     0,   274,     0,   579,     0,   830,   831,     0,    46,
       0,   276,     0,   838,     0,   606,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   603,   329,     0,   376,     0,
       0,     0,   851,   270,   271,     0,   853,     0,   272,     0,
       0,   604,   331,   332,     0,   333,     0,     0,   334,   335,
       0,     0,     0,   864,     0,     0,   274,     0,   605,     0,
     337,   338,     0,   129,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   499,   500,   501,   502,   503,
       0,   875,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,   908,
       0,     0,   910,   911,   912,   914,   915,   539,   540,   918,
     919,     0,   920,     0,     0,   129,     0,   287,     0,     0,
       0,   926,     3,     0,   288,   289,   290,     0,   291,     0,
     292,   293,   294,   295,   296,     0,     0,     0,     0,     0,
       0,   297,   298,   299,   300,     0,     0,   939,     0,     0,
       0,     0,   942,   301,   302,     0,   303,   304,     0,     0,
     305,     0,     9,   306,   307,   948,   308,   309,     0,     0,
     310,   311,     0,     0,     0,     0,     0,   312,   153,   154,
     155,     0,   157,   158,   159,   160,   161,   313,   163,   164,
     165,   166,   167,   168,   169,   170,   171,     0,   173,   174,
     175,     0,     0,   178,   179,   180,   181,     0,     0,   314,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   318,   319,     0,     0,
       0,     0,     0,     0,     0,    46,     0,     0,     0,     0,
       0,     0,   320,   321,   322,   323,   324,   325,   326,   327,
     328,    47,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,   331,   332,
       0,   333,     0,     0,   334,   335,     0,     0,     0,     0,
       0,     0,     0,     0,   336,     0,   337,   338,   339,   129,
     340,   287,     0,     0,     0,     0,     3,     0,   288,   289,
     290,     0,   291,     0,   292,   293,   294,   295,   296,     0,
       0,     0,     0,     0,     0,   297,   298,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,   301,   302,     0,
     303,   304,     0,     0,   305,     0,     9,   306,   307,     0,
     308,   309,     0,     0,   310,   311,     0,     0,     0,     0,
       0,   312,   153,   154,   155,     0,   157,   158,   159,   160,
     161,   313,   163,   164,   165,   166,   167,   168,   169,   170,
     171,     0,   173,   174,   175,     0,     0,   178,   179,   180,
     181,     0,     0,   314,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   316,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,   319,     0,     0,     0,     0,     0,     0,     0,    46,
       0,     0,     0,     0,   266,     0,   320,   321,   322,   323,
     324,   325,   326,   327,   328,    47,   329,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   268,
       0,   330,   331,   332,     0,   333,     0,     0,   334,   335,
       0,     0,     0,   292,   293,   294,   295,   296,   336,     0,
     337,   338,   339,   129,   849,   298,     0,     0,     0,     0,
       0,   266,     0,     0,     0,     0,     0,   302,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,     0,
     309,     0,     0,     0,     0,   267,   268,   269,     0,     0,
       0,   153,   154,   155,     0,   157,   158,   159,   160,   161,
     313,   163,   164,   165,   166,   167,   168,   169,   170,   171,
       0,   173,   174,   175,     0,     0,   178,   179,   180,   181,
       0,     0,   314,     0,     0,   270,   271,     0,     0,     0,
     272,     0,   706,   273,   316,   317,     0,     0,     0,     0,
       0,     0,     0,     0,   269,     0,     0,     0,   274,     0,
       0,     0,     0,     0,     0,   266,     0,   276,    46,     0,
       0,     0,     0,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   603,   329,     0,     0,     0,   267,
     268,     0,   270,   271,     0,     0,     0,   272,     0,     0,
     604,   331,   332,     0,   333,     0,     0,   334,   335,   292,
     293,   294,   295,   296,     0,   274,     0,   615,     0,   337,
     338,   298,   129,     0,   276,     0,     0,   409,     0,     0,
       0,     0,     0,   302,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,     0,   309,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   153,   154,   155,
       0,   157,   158,   159,   160,   161,   313,   163,   164,   165,
     166,   167,   168,   169,   170,   171,     0,   173,   174,   175,
       0,     0,   178,   179,   180,   181,   270,   271,   314,     0,
       0,   272,     0,     0,   273,     0,     0,     0,     0,     0,
     316,   317,     0,     0,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,   707,     0,     0,   276,     0,
       0,     0,     0,     0,    46,   266,     0,     0,     0,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      47,   329,     0,     0,     0,     0,     0,     0,     0,   267,
     268,     0,     0,   410,     0,     0,   330,   331,   332,     0,
     333,     0,     0,   334,   335,   292,   293,   294,   295,   296,
       0,   411,     0,   336,     0,   337,   338,   298,   129,     0,
     412,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,     0,   309,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,   153,   154,   155,     0,   157,   158,   159,
     160,   161,   313,   163,   164,   165,   166,   167,   168,   169,
     170,   171,     0,   173,   174,   175,     0,     0,   178,   179,
     180,   181,     0,     0,   314,     0,   270,   271,     0,     0,
       0,   272,     0,   708,   273,     0,   316,   317,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,   274,
       0,   318,   319,     0,     0,     0,     0,     0,   276,     0,
      46,     0,   266,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    47,   329,     0,     0,
       0,     0,     0,     0,     0,     0,   267,   268,     0,     0,
       0,     0,   330,   331,   332,     0,   333,     0,     0,   334,
     335,     0,     0,   292,   293,   294,   295,   296,     0,   336,
       0,   337,   338,   434,   129,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,     0,
     309,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,   153,   154,   155,     0,   157,   158,   159,   160,   161,
     313,   163,   164,   165,   166,   167,   168,   169,   170,   171,
       0,   173,   174,   175,     0,     0,   178,   179,   180,   181,
       0,     0,   314,   270,   271,     0,     0,     0,   272,     0,
     709,   273,     0,     0,   316,   317,     0,   266,     0,     0,
       0,     0,     0,   452,     0,     0,   274,     0,     0,   318,
     319,     0,     0,     0,     0,   276,     0,     0,    46,     0,
       0,   267,   268,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    47,   329,     0,     0,     0,     0,
       0,   292,   293,   294,   295,   296,     0,     0,     0,     0,
     330,   331,   332,   298,   333,     0,     0,   334,   335,     0,
       0,     0,     0,     0,     0,   302,     0,   336,   304,   337,
     338,   305,   129,     0,   306,     0,     0,     0,   309,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,   155,     0,   157,   158,   159,   160,   161,   313,   163,
     164,   165,   166,   167,   168,   169,   170,   171,     0,   173,
     174,   175,     0,     0,   178,   179,   180,   181,   270,   271,
     314,     0,     0,   272,     0,   712,   273,     0,     0,     0,
       0,     0,   316,   317,     0,   266,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,   318,   319,     0,
     276,     0,     0,     0,     0,     0,    46,     0,     0,   267,
     268,     0,     0,   320,   321,   322,   323,   324,   325,   326,
     327,   328,    47,   329,     0,     0,     0,     0,     0,   292,
     293,   294,   295,   296,     0,     0,     0,     0,   330,   331,
     332,   298,   333,     0,     0,   334,   335,     0,     0,     0,
       0,     0,     0,   302,     0,   336,   304,   337,   338,   305,
     129,     0,   306,     0,     0,     0,   309,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   153,   154,   155,
       0,   157,   158,   159,   160,   161,   313,   163,   164,   165,
     166,   167,   168,   169,   170,   171,     0,   173,   174,   175,
       0,     0,   178,   179,   180,   181,   270,   271,   314,     0,
       0,   272,     0,   714,   273,     0,     0,     0,     0,     0,
     316,   317,     0,     0,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,   266,     0,     0,    46,     0,     0,     0,     0,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      47,   329,     0,     0,     0,   267,   268,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   330,   331,   332,     0,
     333,     0,     0,   334,   335,   292,   293,   294,   295,   296,
       0,   630,     0,   336,   631,   337,   338,   298,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,     0,   309,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   153,   154,   155,     0,   157,   158,   159,
     160,   161,   313,   163,   164,   165,   166,   167,   168,   169,
     170,   171,     0,   173,   174,   175,     0,     0,   178,   179,
     180,   181,   270,   271,   314,     0,     0,   272,     0,   716,
     273,     0,     0,     0,     0,     0,   316,   317,     0,   266,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,   657,   658,     0,   276,     0,     0,     0,     0,     0,
      46,     0,     0,   267,   268,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    47,   329,     0,     0,
       0,     0,     0,   292,   293,   294,   295,   296,     0,     0,
       0,     0,   330,   331,   332,   298,   333,     0,     0,   334,
     335,     0,     0,     0,     0,     0,     0,   302,     0,   336,
     304,   337,   338,   305,   129,     0,   306,     0,     0,     0,
     309,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   153,   154,   155,     0,   157,   158,   159,   160,   161,
     313,   163,   164,   165,   166,   167,   168,   169,   170,   171,
       0,   173,   174,   175,     0,     0,   178,   179,   180,   181,
     270,   271,   314,     0,     0,   272,     0,   740,   273,     0,
       0,     0,     0,     0,   316,   317,     0,   266,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,   676,
     677,     0,   276,     0,     0,     0,     0,     0,    46,     0,
       0,   267,   268,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    47,   329,     0,     0,     0,     0,
       0,   292,   293,   294,   295,   296,     0,     0,     0,     0,
     330,   331,   332,   298,   333,     0,     0,   334,   335,     0,
       0,     0,     0,     0,     0,   302,     0,   336,   304,   337,
     338,   305,   129,     0,   306,     0,     0,     0,   309,   571,
     269,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,   155,     0,   157,   158,   159,   160,   161,   313,   163,
     164,   165,   166,   167,   168,   169,   170,   171,     0,   173,
     174,   175,     0,     0,   178,   179,   180,   181,   818,   271,
     314,     0,     0,   272,     0,     0,   273,     0,     0,     0,
       0,     0,   316,   317,     0,   266,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,    46,     0,     0,   267,
     268,     0,     0,   320,   321,   322,   323,   324,   325,   326,
     327,   328,    47,   329,     0,     0,     0,     0,     0,   292,
     293,   294,   295,   296,     0,     0,     0,     0,   330,   331,
     332,   298,   333,     0,     0,   334,   335,     0,     0,     0,
       0,     0,     0,   302,   413,   336,   304,   337,   338,   305,
     129,     0,   306,     0,     0,     0,   309,     0,   269,   448,
       0,     0,     0,     0,     0,     0,     0,   153,   154,   155,
       0,   157,   158,   159,   160,   161,   313,   163,   164,   165,
     166,   167,   168,   169,   170,   171,     0,   173,   174,   175,
       0,     0,   178,   179,   180,   181,   270,   271,   314,     0,
       0,   272,     0,   824,   273,     0,     0,     0,     0,     0,
     316,   317,     0,   266,     0,     0,     0,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,    46,     0,     0,   267,   268,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      47,   329,     0,     0,     0,     0,     0,   292,   293,   294,
     295,   296,     0,     0,   679,     0,   330,   331,   332,   298,
     333,     0,     0,   334,   335,     0,     0,     0,     0,     0,
       0,   302,     0,   336,   304,   337,   338,   305,   129,     0,
     306,     0,     0,     0,   309,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   153,   154,   155,     0,   157,
     158,   159,   160,   161,   313,   163,   164,   165,   166,   167,
     168,   169,   170,   171,     0,   173,   174,   175,     0,     0,
     178,   179,   180,   181,   270,   271,   314,     0,     0,   272,
       0,   825,   273,     0,     0,     0,     0,     0,   316,   317,
       0,   266,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,    46,     0,     0,   267,   268,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    47,   329,
       0,     0,     0,     0,     0,   292,   293,   294,   295,   296,
       0,     0,     0,     0,   330,   331,   332,   298,   333,     0,
       0,   334,   335,     0,     0,     0,     0,     0,     0,   302,
       0,   336,   304,   337,   338,   305,   129,     0,   306,     0,
       0,     0,   309,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   153,   154,   155,     0,   157,   158,   159,
     160,   161,   313,   163,   164,   165,   166,   167,   168,   169,
     170,   171,     0,   173,   174,   175,     0,     0,   178,   179,
     180,   181,   270,   271,   314,     0,     0,   272,     0,   826,
     273,     0,     0,     0,     0,     0,   316,   317,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,   266,     0,     0,
      46,     0,     0,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    47,   329,     0,     0,
       0,   267,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   331,   332,     0,   333,     0,     0,   334,
     335,   292,   293,   294,   295,   296,     0,     0,     0,   336,
     694,   337,   338,   298,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,     0,   309,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,   155,     0,   157,   158,   159,   160,   161,   313,   163,
     164,   165,   166,   167,   168,   169,   170,   171,     0,   173,
     174,   175,     0,     0,   178,   179,   180,   181,   270,   271,
     314,     0,     0,   272,     0,   827,   273,     0,     0,     0,
       0,     0,   316,   317,     0,     0,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,   266,     0,     0,    46,     0,     0,     0,
       0,     0,     0,   320,   321,   322,   323,   324,   325,   326,
     327,   328,    47,   329,     0,     0,     0,   267,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   330,   331,
     332,     0,   333,     0,     0,   334,   335,   292,   293,   294,
     295,   296,     0,     0,     0,   336,   728,   337,   338,   298,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,     0,   309,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   153,   154,   155,     0,   157,
     158,   159,   160,   161,   313,   163,   164,   165,   166,   167,
     168,   169,   170,   171,     0,   173,   174,   175,     0,     0,
     178,   179,   180,   181,   270,   271,   314,     0,     0,   272,
       0,   871,   273,     0,     0,     0,     0,     0,   316,   317,
       0,   266,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,    46,     0,     0,   267,   268,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    47,   329,
       0,     0,     0,     0,     0,   292,   293,   294,   295,   296,
       0,     0,   730,     0,   330,   331,   332,   298,   333,     0,
       0,   334,   335,     0,     0,     0,     0,     0,     0,   302,
       0,   336,   304,   337,   338,   305,   129,     0,   306,     0,
       0,     0,   309,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   153,   154,   155,     0,   157,   158,   159,
     160,   161,   313,   163,   164,   165,   166,   167,   168,   169,
     170,   171,     0,   173,   174,   175,     0,     0,   178,   179,
     180,   181,   270,   271,   314,     0,     0,   272,     0,   893,
     273,     0,     0,     0,     0,     0,   316,   317,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,   266,     0,     0,
      46,     0,     0,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   325,   326,   327,   328,    47,   329,     0,     0,
       0,   267,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   331,   332,     0,   333,     0,     0,   334,
     335,   292,   293,   294,   295,   296,     0,     0,     0,   336,
     746,   337,   338,   298,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,     0,   309,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,   155,     0,   157,   158,   159,   160,   161,   313,   163,
     164,   165,   166,   167,   168,   169,   170,   171,     0,   173,
     174,   175,     0,     0,   178,   179,   180,   181,   270,   271,
     314,     0,     0,   272,     0,   927,   273,     0,     0,     0,
       0,     0,   316,   317,     0,     0,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,   266,     0,     0,    46,     0,     0,     0,
       0,     0,     0,   320,   321,   322,   323,   324,   325,   326,
     327,   328,    47,   329,     0,     0,     0,   267,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   330,   331,
     332,     0,   333,     0,     0,   334,   335,   292,   293,   294,
     295,   296,     0,     0,     0,   336,   755,   337,   338,   298,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,     0,   309,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   153,   154,   155,     0,   157,
     158,   159,   160,   161,   313,   163,   164,   165,   166,   167,
     168,   169,   170,   171,     0,   173,   174,   175,     0,     0,
     178,   179,   180,   181,   270,   271,   314,     0,     0,   272,
       0,     0,   273,     0,     0,     0,     0,     0,   316,   317,
       0,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    47,   329,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,   331,   332,     0,   333,     0,
       0,   334,   335,   292,   293,   294,   295,   296,     0,     0,
       0,   336,   862,   337,   338,   298,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   153,   154,   155,     0,   157,   158,   159,   160,   161,
     313,   163,   164,   165,   166,   167,   168,   169,   170,   171,
       0,   173,   174,   175,     0,     0,   178,   179,   180,   181,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    47,   329,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,   331,   332,     0,   333,     0,     0,   334,   335,   292,
     293,   294,   295,   296,     0,     0,     0,   336,   913,   337,
     338,   298,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   153,   154,   155,
       0,   157,   158,   159,   160,   161,   313,   163,   164,   165,
     166,   167,   168,   169,   170,   171,     0,   173,   174,   175,
       0,     0,   178,   179,   180,   181,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,   317,     0,     0,     0,     0,     0,     0,     0,   925,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    46,     0,     0,     0,     0,     0,
       0,   320,   321,   322,   323,   324,   325,   326,   327,   328,
      47,   329,     0,     0,     0,     0,     0,   292,   293,   294,
     295,   296,     0,     0,     0,     0,   330,   331,   332,   298,
     333,     0,     0,   334,   335,     0,     0,     0,     0,     0,
       0,   302,     0,   336,   304,   337,   338,   305,   129,     0,
     306,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   153,   154,   155,     0,   157,
     158,   159,   160,   161,   313,   163,   164,   165,   166,   167,
     168,   169,   170,   171,     0,   173,   174,   175,     0,     0,
     178,   179,   180,   181,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,   317,
       0,     0,   489,   490,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   325,   326,   327,   328,    47,   329,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,   331,   332,     0,   333,     0,
       0,   334,   335,     0,     0,     0,     0,     0,     0,     0,
       0,   336,     0,   337,   338,     0,   129,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   584,   511,   512,   513,
     514,   489,   490,     0,   515,   516,   517,   518,   519,   520,
       0,     0,   521,   522,   523,   524,   525,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   585,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   539,   540,     0,
       0,     0,     0,     0,   586,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   584,   511,   512,   513,   514,
     489,   490,     0,   515,   516,   517,   518,   519,   520,     0,
       0,   521,   522,   523,   524,   525,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   585,   528,     0,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   540,     0,     0,
       0,     0,     0,   599,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   584,   511,   512,   513,   514,   489,
     490,     0,   515,   516,   517,   518,   519,   520,     0,     0,
     521,   522,   523,   524,   525,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   585,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   539,   540,     0,     0,     0,
       0,     0,   733,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   584,   511,   512,   513,   514,   489,   490,
       0,   515,   516,   517,   518,   519,   520,     0,     0,   521,
     522,   523,   524,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   585,   528,     0,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   539,   540,     0,     0,     0,     0,
       0,   741,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   584,   511,   512,   513,   514,   489,   490,     0,
     515,   516,   517,   518,   519,   520,     0,     0,   521,   522,
     523,   524,   525,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   585,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   540,     0,     0,     0,     0,     0,
     802,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   584,   511,   512,   513,   514,   489,   490,     0,   515,
     516,   517,   518,   519,   520,     0,     0,   521,   522,   523,
     524,   525,   526,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     585,   528,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   539,   540,     0,     0,     0,     0,     0,   870,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     489,   490,     0,     0,     0,     0,     0,     0,     0,   499,
     500,   501,   502,   503,     0,     0,   506,   507,   508,   509,
       0,   511,   512,   513,   514,     0,     0,     0,   515,     0,
     517,   518,     0,     0,     0,     0,   521,   522,   523,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,     0,     0,   489,   490,     0,     0,     0,     0,
       0,   539,   540,   499,   500,   501,   502,   503,   549,     0,
     506,   507,   508,   509,     0,   511,   512,   513,   514,     0,
       0,     0,   515,     0,   517,   518,     0,     0,     0,     0,
     521,   522,   523,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,     0,
       0,   489,   490,     0,     0,   539,   540,   499,   500,   501,
     502,   503,   932,     0,   506,   507,   508,   509,     0,   511,
     512,   513,   514,     0,     0,     0,   515,     0,   517,   518,
       0,     0,     0,     0,   521,   522,   523,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   539,
     540,   489,   490,   625,   499,   500,   501,   502,   503,     0,
       0,   506,   507,   508,   509,     0,   511,   512,   513,   514,
       0,     0,     0,   515,     0,   517,   518,     0,     0,     0,
       0,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   540,   489,   490,
     735,     0,     0,     0,   499,   500,   501,   502,   503,     0,
       0,   506,   507,   508,   509,     0,   511,   512,   513,   514,
       0,     0,     0,   515,     0,   517,   518,     0,     0,     0,
       0,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   540,   489,   490,
     738,   499,   500,   501,   502,   503,     0,     0,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,     0,     0,     0,     0,   521,   522,
     523,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   540,   489,   490,   823,     0,     0,
       0,   499,   500,   501,   502,   503,     0,     0,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,     0,     0,     0,     0,   521,   522,
     523,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   540,   489,   490,   929,   499,   500,
     501,   502,   503,     0,     0,   506,   507,   508,   509,     0,
     511,   512,   513,   514,     0,     0,     0,   515,     0,   517,
     518,     0,     0,     0,     0,   521,   522,   523,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     539,   540,   489,   490,   931,     0,     0,     0,   499,   500,
     501,   502,   503,     0,     0,   506,   507,   508,   509,     0,
     511,   512,   513,   514,     0,     0,     0,   515,     0,   517,
     518,     0,     0,     0,     0,   521,   522,   523,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,     0,     0,     0,     0,     0,     0,   489,   490,     0,
     539,   540,     0,     0,   947,   499,   500,   501,   502,   503,
       0,     0,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,   489,   490,     0,     0,     0,   539,   540,   568,
     499,   500,   501,   502,   503,     0,     0,   506,   507,   508,
     509,     0,   511,   512,   513,   514,     0,     0,     0,   515,
       0,   517,   518,     0,     0,     0,     0,   521,   522,   523,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   528,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   539,   540,   787,   499,   500,   501,   502,   503,
     489,   490,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   539,   540,   793,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   584,   511,   512,   513,   514,  -231,
     489,   490,   515,   516,   517,   518,   519,   520,     0,     0,
     521,   522,   523,   524,   525,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   585,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   539,   540,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   489,   490,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,     0,
       0,     0,   515,   516,   517,   518,   519,   520,     0,     0,
     521,   522,   523,   524,   525,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   489,   490,     0,     0,
     499,   500,   501,   502,   503,   539,   540,   506,   507,   508,
     509,     0,   511,   512,   513,   514,     0,     0,     0,   515,
       0,   517,   518,     0,     0,   618,     0,   521,   522,   523,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   528,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   489,   490,     0,     0,     0,     0,     0,
       0,     0,   539,   540,     0,     0,     0,     0,     0,   499,
     500,   501,   502,   503,     0,     0,   506,   507,   508,   509,
       0,   511,   512,   513,   514,     0,     0,     0,   515,     0,
     517,   518,     0,     0,     0,     0,   521,   522,   523,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,   791,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   489,   490,     0,     0,   499,   500,   501,   502,
     503,   539,   540,   506,   507,   508,   509,     0,   511,   512,
     513,   514,     0,     0,     0,   515,     0,   517,   518,     0,
       0,     0,     0,   521,   522,   523,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   845,     0,     0,     0,   528,     0,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   489,
     490,     0,     0,     0,     0,     0,     0,     0,   539,   540,
       0,     0,     0,     0,     0,   499,   500,   501,   502,   503,
       0,     0,   506,   507,   508,   509,     0,   511,   512,   513,
     514,     0,     0,     0,   515,     0,   517,   518,     0,     0,
       0,     0,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   489,   490,
       0,     0,   499,   500,   501,   502,   503,   539,   540,   506,
     507,   508,   509,     0,   511,   512,   513,   514,     0,     0,
       0,   515,     0,   517,   518,   489,   490,     0,     0,   521,
       0,   523,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   539,   540,     0,     0,     0,     0,
       0,   499,   500,   501,   502,   503,     0,     0,   506,   507,
     508,   509,     0,   511,   512,   513,   514,     0,     0,     0,
     515,     0,   517,   518,   489,   490,     0,     0,   499,   500,
     501,   502,   503,     0,     0,   506,   507,   508,   509,     0,
     511,   512,   513,   514,     0,     0,     0,   515,     0,   517,
     518,     0,     0,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   540,     0,     0,     0,     0,     0,
       0,     0,   530,   531,   532,   533,   534,   535,   536,   537,
     538,   489,   490,     0,     0,     0,     0,     0,     0,     0,
     539,   540,     0,     0,     0,     0,     0,   499,   500,   501,
     502,   503,     0,     0,   506,   507,   508,   509,     0,   511,
     512,   513,   514,     0,     0,     0,   515,     0,   517,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,   532,   533,   534,   535,   536,   537,   538,
       0,     0,     0,     0,   499,   500,     0,     0,   503,   539,
     540,   506,   507,   508,   509,     0,   511,   512,   513,   514,
       0,     0,     0,   515,     0,   517,   518,     0,     0,     0,
     460,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,   149,     0,     0,     0,     0,     0,   150,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   530,   531,
     532,   533,     0,   151,     0,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   540,   152,     0,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
       0,     0,     0,     0,   150,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,   152,     0,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,     0,     0,
     150,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
     152,     0,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,     0,     0,   150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
       0,   151,     0,     0,     0,     0,   320,   321,   322,     0,
       0,     0,     0,     0,     0,    47,   152,     0,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,     0,     0,     0,
       0,     0,   150,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,     0,   151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,   152,     0,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,     0,     0,     0,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   214,   215,   216,
     217,    46,     0,   218,   219,   220,   221,   222,   223,     0,
       0,     0,     0,     0,     0,     0,     0,   562,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,     0,
       0,   234,   235
};

static const yytype_int16 yycheck[] =
{
      13,    83,   240,   282,   113,   279,   322,   463,   423,   421,
      77,   297,   291,   401,   557,   403,   559,   405,   561,     7,
     318,   319,   608,   541,   367,   244,    18,    14,    15,   315,
     616,    20,    21,     0,    19,   113,   630,    46,    19,     6,
      34,    21,    49,    33,    49,    19,   145,   119,   128,   114,
     114,   113,   136,   145,   116,   117,   145,   145,   114,   123,
     578,   579,    29,   139,    31,    78,    33,   123,    35,    59,
     153,   179,    39,    53,   182,   116,   117,    20,    21,    46,
     129,   180,    32,   114,   145,    52,   178,   152,   153,   153,
     155,   145,   123,   158,   158,   179,   184,   153,   129,   182,
     113,   145,   152,   183,    71,   129,   155,   145,    20,    21,
     182,   105,   106,   175,   176,   145,   105,   106,   145,   152,
     145,   145,   153,   184,   113,    92,   115,   116,   117,   118,
     184,   152,   182,   122,   175,   176,   145,   155,   145,   129,
     184,   114,   421,   362,    14,    15,   184,   433,   136,   182,
     123,   145,   139,   145,   748,   145,   143,   184,   145,   146,
     746,   182,   105,   106,   183,   183,   452,   152,    58,   278,
     113,   152,   115,   116,   117,   118,   285,   182,   152,   122,
     153,   460,   153,   462,   164,   294,   175,   176,   176,   487,
     152,   178,   152,   105,   106,   175,   184,   152,   152,   542,
     278,   113,   155,   115,   116,   117,   118,   285,   158,   176,
     122,   320,   321,   280,   152,   182,   294,   179,   300,   179,
     163,   164,   165,   177,   742,   129,   176,   152,   241,   145,
     312,   114,   175,   176,   152,   185,   754,   160,   160,   782,
     123,   145,   320,   321,   152,   145,   832,   635,   145,   161,
     162,   163,   164,   165,   179,   159,   842,   152,   152,   182,
     182,   179,   548,   175,   176,   278,   784,    20,    21,   139,
     153,   179,   285,   143,    53,   145,   146,   152,   160,   388,
      59,   294,   145,   177,   179,   394,   395,   396,   397,   575,
     746,   400,   160,   402,   129,   404,   152,   406,   152,   755,
     182,   178,   177,   619,   602,   848,    53,   320,   321,   724,
     388,    66,    59,    32,   182,    70,   394,   395,   396,   397,
     333,   177,   400,   177,   402,   155,   404,   182,    11,   184,
     160,   177,    87,    88,    89,    90,   182,    56,    57,    22,
      23,    53,   451,   139,   155,   427,   153,    59,   178,   160,
     103,   104,   105,   106,   107,   145,   155,   110,   145,   441,
     113,   160,   115,   116,   117,   118,   183,   178,    53,   122,
     183,   124,   125,   451,    59,   388,   182,   765,   184,   178,
     155,   394,   395,   396,   397,   160,   798,   400,   145,   402,
     183,   404,   183,   406,    20,    21,   115,   479,   152,     4,
       5,   155,     7,   178,   158,   145,   159,   160,   161,   162,
     163,   164,   165,   180,   181,   145,   183,   703,   431,   834,
     147,   148,   175,   176,   147,   148,   149,    42,    43,   147,
      53,   183,   145,    34,   153,   154,   145,    34,   451,   158,
     178,   155,   161,    32,   145,   178,   182,   182,   722,   159,
      32,   159,   159,   159,   159,   159,   159,   176,   155,   159,
      20,    21,   475,   182,   573,   159,   185,    56,    57,   159,
     145,   155,   581,   159,    36,   139,   178,   103,   104,   105,
     106,   590,   591,   159,   593,   178,   159,   113,   183,   115,
     116,   117,   118,   182,   159,   573,   122,   178,   124,   125,
     159,   181,   145,   581,   182,   587,   176,   136,   182,   178,
     182,   182,   590,   591,   182,   593,   179,   177,    13,   798,
     119,   155,   139,   181,   136,   114,   115,   183,   766,   145,
     177,   145,   145,   152,   123,   161,   162,   163,   164,   165,
     177,   145,   240,   152,   184,   105,   106,   147,   159,   175,
     176,   147,   145,   113,   178,   253,   116,   117,   118,   145,
     573,     1,   122,     7,   153,   154,    42,   145,   581,   158,
     160,   145,   161,   160,    20,    21,   274,   590,   591,   182,
     593,   160,   145,   177,   145,   177,   177,   176,   147,   184,
     288,   145,   177,   182,   145,   145,   185,   178,   707,   297,
     177,   177,   177,   301,   145,   303,   179,   177,   177,   718,
     182,   720,   177,   311,   160,   175,   176,   315,   316,   317,
     184,   160,   177,    49,   322,   177,   182,   184,   177,   707,
     177,   467,   330,   331,   332,   177,   334,   335,   336,    58,
     718,   102,   895,    61,   250,    59,    78,   345,   696,   905,
     868,   628,     1,   858,   740,   764,   542,   103,   104,   105,
     106,   107,   796,   425,   110,   111,   112,   113,   720,   115,
     116,   117,   118,   407,    41,   544,   122,   146,   124,   125,
      71,   463,   399,   696,   130,   767,   764,   294,   920,    -1,
     753,    -1,    -1,    -1,   707,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   718,    -1,   720,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,   433,    -1,    -1,   820,    -1,
      -1,    -1,   440,    -1,    -1,    -1,   444,    -1,    -1,    -1,
      -1,   764,   861,    -1,   452,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   461,    -1,   463,    -1,    -1,    -1,   851,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   860,    -1,
      -1,    -1,    -1,   861,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,    -1,    -1,   503,   504,   505,   506,   507,
     508,    -1,   510,   511,    -1,   513,   514,   515,   516,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,    -1,   540,    -1,    -1,    -1,    -1,    -1,   861,    -1,
     548,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,   575,    -1,    -1,
      -1,    -1,   580,    -1,    -1,    -1,   584,   585,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   604,   605,    -1,   922,
     159,   160,   161,   162,   163,   164,   165,   615,    -1,    -1,
     618,   619,    56,    57,   622,   938,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,   657,
     658,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    49,
      -1,    51,    -1,    -1,    -1,    -1,    56,    57,   676,   677,
      -1,   115,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,   703,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   153,
     154,    -1,    -1,    -1,   158,   105,   106,   161,    -1,    -1,
      -1,    -1,    20,    21,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,   178,    -1,   744,   745,    -1,   129,
      -1,   185,    -1,   751,    -1,   753,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,   766,    -1,
      -1,    -1,   770,   153,   154,    -1,   774,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,   169,
      -1,    -1,    -1,   791,    -1,    -1,   176,    -1,   178,    -1,
     180,   181,    -1,   183,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,   819,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,   877,
      -1,    -1,   880,   881,   882,   883,   884,   175,   176,   887,
     888,    -1,   890,    -1,    -1,   183,    -1,     1,    -1,    -1,
      -1,   899,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,   925,    -1,    -1,
      -1,    -1,   930,    37,    38,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,   943,    50,    51,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
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
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
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
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   153,   154,    93,    -1,
      -1,   158,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,   129,    32,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    14,    15,    16,    17,    18,
      -1,   176,    -1,   178,    -1,   180,   181,    26,   183,    -1,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,   153,   154,    -1,    -1,
      -1,   158,    -1,   160,   161,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,   176,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,   185,    -1,
     129,    -1,    32,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    -1,    -1,    14,    15,    16,    17,    18,    -1,   178,
      -1,   180,   181,   182,   183,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,   153,   154,    -1,    -1,    -1,   158,    -1,
     160,   161,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,   176,    -1,    -1,   120,
     121,    -1,    -1,    -1,    -1,   185,    -1,    -1,   129,    -1,
      -1,    56,    57,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     161,   162,   163,    26,   165,    -1,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   178,    41,   180,
     181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    32,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    14,    15,    16,    17,    18,
      -1,   176,    -1,   178,   179,   180,   181,    26,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   153,   154,    93,    -1,    -1,   158,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,   120,   121,    -1,   185,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   161,   162,   163,    26,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,
      41,   180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,
      51,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     153,   154,    93,    -1,    -1,   158,    -1,   160,   161,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,   120,
     121,    -1,   185,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    56,    57,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     161,   162,   163,    26,   165,    -1,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   178,    41,   180,
     181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   153,   154,
      93,    -1,    -1,   158,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,
      57,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,   161,   162,
     163,    26,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    38,   177,   178,    41,   180,   181,    44,
     183,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   153,   154,    93,    -1,
      -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    21,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   178,    41,   180,   181,    44,   183,    -1,
      47,    -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   153,   154,    93,    -1,    -1,   158,
      -1,   160,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
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
     169,    14,    15,    16,    17,    18,    -1,    -1,    -1,   178,
     179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   153,   154,
      93,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    32,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   178,   179,   180,   181,    26,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   153,   154,    93,    -1,    -1,   158,
      -1,   160,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,   159,    -1,   161,   162,   163,    26,   165,    -1,
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
     169,    14,    15,    16,    17,    18,    -1,    -1,    -1,   178,
     179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   153,   154,
      93,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    32,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   178,   179,   180,   181,    26,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   153,   154,    93,    -1,    -1,   158,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
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
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      20,    21,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,
      -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    20,
      21,    -1,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    20,    21,
      -1,   122,   123,   124,   125,   126,   127,    -1,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
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
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,   175,   176,   103,   104,   105,   106,   107,   182,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,   175,   176,   103,   104,   105,
     106,   107,   182,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,    20,    21,   179,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    20,    21,
     179,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
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
     165,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     175,   176,    -1,    -1,   179,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,   175,   176,   177,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,   177,   103,   104,   105,   106,   107,
      20,    21,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,   177,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      20,    21,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    20,    21,    -1,    -1,
     103,   104,   105,   106,   107,   175,   176,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,   128,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    20,    21,    -1,    -1,   103,   104,   105,   106,
     107,   175,   176,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    20,    21,
      -1,    -1,   103,   104,   105,   106,   107,   175,   176,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    20,    21,    -1,    -1,   130,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    20,    21,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,   107,   175,
     176,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      12,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,    -1,    45,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    45,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   129,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   187,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    71,    92,   176,   182,   190,   198,   199,   203,   223,
     227,   243,   298,   303,   304,   309,   345,   347,   145,   218,
     219,   139,   204,   205,   145,   200,   201,   145,   183,   145,
     180,   189,   348,   346,    33,    59,   129,   145,   220,   221,
     222,   235,     4,     5,     7,   307,    58,   296,   153,   152,
     155,   152,    21,    53,   164,   175,   202,   155,   183,   145,
     301,   302,   296,   145,   145,   145,   129,   178,   152,   177,
      53,    59,   228,   229,   230,    53,    59,   308,    53,    59,
     297,    14,    15,   139,   143,   145,   146,   178,   192,   218,
     139,   205,   145,   145,   145,    66,    70,    87,    88,    89,
      90,   313,   300,   153,   184,   301,    53,    59,   188,   183,
     183,   145,   219,   221,    34,   105,   106,   145,   226,   183,
     231,   145,   306,   183,   299,   147,   191,    14,    15,   139,
     143,   145,   192,   216,   217,   202,   183,   145,   184,    24,
      30,    45,    60,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   235,   312,   314,   315,   317,   320,
     145,   316,   145,   288,   289,   179,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   124,   125,   126,   127,   130,   131,
     132,   133,   134,   135,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   168,   169,    34,    34,   178,   224,
     233,   155,   305,   310,   295,   145,   294,   311,   147,   148,
     152,   179,   300,   153,   182,   159,   159,   159,   159,   178,
     159,   159,   159,   159,   159,   159,    32,    56,    57,   115,
     153,   154,   158,   161,   176,   182,   185,   182,   155,   182,
     136,   179,   279,   284,   285,   155,   225,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    25,    26,    27,
      28,    37,    38,    40,    41,    44,    47,    48,    50,    51,
      54,    55,    61,    71,    93,    94,   105,   106,   120,   121,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   146,
     161,   162,   163,   165,   168,   169,   178,   180,   181,   182,
     184,   194,   196,   206,   207,   210,   211,   212,   213,   214,
     215,   231,   232,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   252,   255,   258,   262,   263,
     264,   265,   266,   267,   268,   273,   276,   312,   349,   353,
     358,   360,   362,   235,   183,   184,   294,   152,   155,   158,
     293,   216,   184,   276,   323,   325,   327,   321,   145,   318,
     329,   331,   333,   335,   337,   339,   341,   343,    32,    32,
     158,   176,   185,   177,   276,   145,   184,   320,   184,   288,
     219,    46,   291,   311,   179,   182,   320,   276,   311,   320,
     349,   178,   159,   114,   182,   234,   264,   276,   231,   276,
     178,   276,   159,   159,   178,   182,   159,   139,    54,   276,
     231,   159,   114,   234,   276,   276,   276,   181,   262,   262,
      12,   320,    12,   320,   276,   355,   359,   193,   276,   276,
     276,   235,   276,   276,   276,   181,    36,   182,   182,   276,
     145,   178,   182,   182,   182,   294,   223,   119,   182,    20,
      21,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   122,   123,   124,   125,   126,
     127,   130,   131,   132,   133,   134,   135,   153,   154,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   175,
     176,   178,   282,   145,   320,   114,   123,   153,   292,   182,
     320,   320,   320,   320,   179,   316,   320,   224,   320,   224,
     320,   224,   145,   286,   287,   320,   289,   177,   177,   177,
     291,   114,   153,   155,   158,   290,   284,   231,    19,   178,
     235,   253,   234,   276,   114,   153,   182,    13,   276,   231,
     246,   248,   276,   250,   155,   139,   320,   234,   276,   182,
     181,   311,    49,   145,   161,   178,   276,   350,   351,   352,
     354,   355,   356,   357,   311,   178,   351,   357,   128,   182,
     184,   148,   149,   191,   197,   179,   159,   235,   183,   231,
     176,   179,   256,   276,   136,   261,   262,    18,   145,   145,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   145,   120,   121,   276,
     276,   145,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   120,   121,   276,    21,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   145,   274,   276,   179,   256,   176,   184,   223,   279,
     281,   153,   182,   292,   234,   276,   160,   182,   160,   160,
     160,   182,   160,   225,   160,   225,   160,   225,   155,   160,
     182,   160,   182,   177,   320,   234,   276,   256,   179,   256,
     159,   276,   320,   182,   231,   179,   320,   320,   179,   320,
     160,   182,    19,   262,   123,   290,   179,   152,   182,   352,
     177,   152,   177,   182,    19,   179,   352,   184,   276,   355,
     184,   276,   147,   195,   269,   271,   233,    11,    22,    23,
     208,   209,   145,   350,   152,   179,   114,   123,   153,   158,
     259,   260,   224,   159,   178,   276,   276,   177,   276,   276,
     145,   155,     1,   177,   179,   222,     7,    42,   280,   182,
     234,   276,   182,   324,   320,   328,   322,   319,   330,   160,
     334,   160,   338,   160,   320,   342,   286,   344,   153,   290,
     182,   179,   145,   179,   160,   160,   160,   160,   261,   256,
     276,   276,   351,   352,   145,   350,   177,   177,   276,   177,
     356,   256,   351,   184,   177,   150,   147,   320,   224,   184,
     231,   276,   177,   276,   145,   145,   145,   145,   152,   177,
     225,   277,   179,   256,   276,   275,   177,    43,   280,   291,
     182,   160,   332,   336,   340,   276,   231,   160,   182,   254,
     247,   249,   251,   178,   182,   352,   177,   123,   290,   177,
     182,   352,   177,   160,   225,   231,   179,   259,   177,   128,
     231,   257,   320,   179,   182,   283,   230,   326,   276,   145,
     276,   276,   276,   179,   276,   276,   177,   177,   276,   276,
     276,   184,   270,   160,   209,   114,   276,   160,   229,   179,
     160,   179,   182,   361,   177,   361,   177,   235,   272,   276,
     278,   182,   276,    49,   177,   184,   235,   179,   276,   177,
     177
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
     226,   226,   226,   226,   226,   226,   227,   228,   228,   228,
     229,   230,   231,   231,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   233,   233,   233,   234,   234,   234,   235,
     235,   235,   236,   237,   237,   237,   237,   238,   239,   240,
     240,   240,   240,   240,   241,   241,   241,   241,   242,   243,
     243,   244,   246,   247,   245,   248,   249,   245,   250,   251,
     245,   253,   254,   252,   255,   255,   255,   256,   256,   257,
     257,   257,   258,   258,   258,   259,   259,   259,   259,   260,
     260,   261,   261,   262,   262,   263,   263,   263,   263,   263,
     263,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     265,   265,   265,   265,   266,   267,   267,   268,   269,   270,
     268,   271,   272,   268,   273,   274,   275,   273,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   277,   278,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   279,   279,
     280,   280,   281,   282,   282,   283,   282,   282,   282,   284,
     284,   285,   285,   286,   286,   287,   287,   288,   289,   289,
     290,   290,   291,   291,   291,   291,   291,   291,   292,   292,
     292,   293,   293,   294,   294,   294,   294,   294,   295,   295,
     296,   296,   297,   297,   297,   298,   299,   298,   300,   300,
     300,   301,   302,   302,   303,   304,   304,   305,   305,   306,
     307,   307,   308,   308,   308,   310,   309,   311,   311,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   313,   313,   313,   313,   313,   313,
     314,   315,   315,   316,   316,   318,   319,   317,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   321,   322,   320,   320,   323,
     324,   320,   325,   326,   320,   327,   328,   320,   320,   329,
     330,   320,   331,   332,   320,   320,   333,   334,   320,   335,
     336,   320,   320,   337,   338,   320,   339,   340,   320,   341,
     342,   320,   343,   344,   320,   346,   345,   348,   347,   349,
     349,   349,   349,   350,   350,   350,   350,   351,   351,   352,
     352,   353,   353,   353,   353,   353,   353,   354,   354,   355,
     356,   356,   357,   357,   358,   358,   359,   359,   360,   361,
     361,   362,   362
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
       2,     2,     2,     2,     2,     2,     3,     0,     1,     1,
       4,     2,     3,     7,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     3,     2,     2,     1,
       3,     2,     2,     2,     4,     5,     2,     1,     1,     2,
       3,     4,     2,     3,     3,     4,     2,     3,     4,     1,
       1,     2,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     0,     0,     6,     5,     8,    10,     1,     3,     1,
       2,     3,     1,     1,     2,     2,     2,     2,     2,     1,
       3,     0,     4,     1,     6,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     4,     4,     6,     5,     6,     3,     0,     0,
       8,     0,     0,     9,     3,     0,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     3,     4,     4,     3,
       1,     3,     4,     3,     4,     2,     4,     4,     7,     8,
       3,     5,     0,     0,     8,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     2,     0,     4,
       0,     1,     3,     0,     3,     0,     7,     5,     5,     2,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     4,     6,     5,     5,     4,     0,     2,
       0,     1,     0,     1,     1,     6,     0,     5,     0,     3,
       5,     4,     1,     2,     4,     5,     7,     0,     2,     2,
       1,     1,     0,     1,     1,     0,     8,     1,     3,     1,
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3375 "ds_parser.cpp"
        break;

    case 189: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3381 "ds_parser.cpp"
        break;

    case 191: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3387 "ds_parser.cpp"
        break;

    case 192: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3393 "ds_parser.cpp"
        break;

    case 193: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3399 "ds_parser.cpp"
        break;

    case 194: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3405 "ds_parser.cpp"
        break;

    case 196: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3411 "ds_parser.cpp"
        break;

    case 200: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3417 "ds_parser.cpp"
        break;

    case 206: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3423 "ds_parser.cpp"
        break;

    case 207: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3429 "ds_parser.cpp"
        break;

    case 209: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3435 "ds_parser.cpp"
        break;

    case 211: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3441 "ds_parser.cpp"
        break;

    case 212: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3447 "ds_parser.cpp"
        break;

    case 213: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3453 "ds_parser.cpp"
        break;

    case 214: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3459 "ds_parser.cpp"
        break;

    case 215: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3465 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3471 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3477 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3483 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3489 "ds_parser.cpp"
        break;

    case 220: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3495 "ds_parser.cpp"
        break;

    case 221: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3501 "ds_parser.cpp"
        break;

    case 222: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3507 "ds_parser.cpp"
        break;

    case 223: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3513 "ds_parser.cpp"
        break;

    case 224: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3519 "ds_parser.cpp"
        break;

    case 225: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3525 "ds_parser.cpp"
        break;

    case 226: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3531 "ds_parser.cpp"
        break;

    case 231: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3537 "ds_parser.cpp"
        break;

    case 232: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3543 "ds_parser.cpp"
        break;

    case 233: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3549 "ds_parser.cpp"
        break;

    case 234: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3555 "ds_parser.cpp"
        break;

    case 235: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3561 "ds_parser.cpp"
        break;

    case 236: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3567 "ds_parser.cpp"
        break;

    case 237: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3573 "ds_parser.cpp"
        break;

    case 238: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3579 "ds_parser.cpp"
        break;

    case 239: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3585 "ds_parser.cpp"
        break;

    case 240: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3591 "ds_parser.cpp"
        break;

    case 241: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3597 "ds_parser.cpp"
        break;

    case 242: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3603 "ds_parser.cpp"
        break;

    case 244: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3609 "ds_parser.cpp"
        break;

    case 245: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3615 "ds_parser.cpp"
        break;

    case 252: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3621 "ds_parser.cpp"
        break;

    case 255: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3627 "ds_parser.cpp"
        break;

    case 256: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3633 "ds_parser.cpp"
        break;

    case 257: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3639 "ds_parser.cpp"
        break;

    case 259: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3645 "ds_parser.cpp"
        break;

    case 260: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3651 "ds_parser.cpp"
        break;

    case 261: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3657 "ds_parser.cpp"
        break;

    case 262: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 263: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3669 "ds_parser.cpp"
        break;

    case 264: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 265: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 266: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 267: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 268: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 273: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
        break;

    case 276: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3711 "ds_parser.cpp"
        break;

    case 279: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3717 "ds_parser.cpp"
        break;

    case 281: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3723 "ds_parser.cpp"
        break;

    case 282: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3729 "ds_parser.cpp"
        break;

    case 284: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3735 "ds_parser.cpp"
        break;

    case 285: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3741 "ds_parser.cpp"
        break;

    case 286: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3747 "ds_parser.cpp"
        break;

    case 287: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3753 "ds_parser.cpp"
        break;

    case 288: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3759 "ds_parser.cpp"
        break;

    case 289: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3765 "ds_parser.cpp"
        break;

    case 291: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3771 "ds_parser.cpp"
        break;

    case 294: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3777 "ds_parser.cpp"
        break;

    case 295: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3783 "ds_parser.cpp"
        break;

    case 300: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3789 "ds_parser.cpp"
        break;

    case 305: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3795 "ds_parser.cpp"
        break;

    case 311: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3801 "ds_parser.cpp"
        break;

    case 314: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3807 "ds_parser.cpp"
        break;

    case 315: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3813 "ds_parser.cpp"
        break;

    case 316: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3819 "ds_parser.cpp"
        break;

    case 317: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3825 "ds_parser.cpp"
        break;

    case 320: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3831 "ds_parser.cpp"
        break;

    case 349: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3837 "ds_parser.cpp"
        break;

    case 350: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3843 "ds_parser.cpp"
        break;

    case 351: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3849 "ds_parser.cpp"
        break;

    case 352: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3855 "ds_parser.cpp"
        break;

    case 353: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3861 "ds_parser.cpp"
        break;

    case 354: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3867 "ds_parser.cpp"
        break;

    case 355: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3873 "ds_parser.cpp"
        break;

    case 356: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3879 "ds_parser.cpp"
        break;

    case 357: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3885 "ds_parser.cpp"
        break;

    case 358: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3891 "ds_parser.cpp"
        break;

    case 359: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3897 "ds_parser.cpp"
        break;

    case 360: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3903 "ds_parser.cpp"
        break;

    case 361: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3909 "ds_parser.cpp"
        break;

    case 362: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3915 "ds_parser.cpp"
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
#line 466 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4207 "ds_parser.cpp"
    break;

  case 16:
#line 467 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4213 "ds_parser.cpp"
    break;

  case 17:
#line 468 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4219 "ds_parser.cpp"
    break;

  case 18:
#line 472 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4225 "ds_parser.cpp"
    break;

  case 19:
#line 473 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4231 "ds_parser.cpp"
    break;

  case 20:
#line 477 "ds_parser.ypp"
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
#line 4250 "ds_parser.cpp"
    break;

  case 21:
#line 494 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4256 "ds_parser.cpp"
    break;

  case 22:
#line 495 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4262 "ds_parser.cpp"
    break;

  case 23:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4268 "ds_parser.cpp"
    break;

  case 24:
#line 503 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4278 "ds_parser.cpp"
    break;

  case 25:
#line 508 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4292 "ds_parser.cpp"
    break;

  case 26:
#line 517 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4302 "ds_parser.cpp"
    break;

  case 27:
#line 525 "ds_parser.ypp"
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
#line 4320 "ds_parser.cpp"
    break;

  case 28:
#line 541 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4330 "ds_parser.cpp"
    break;

  case 29:
#line 546 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4340 "ds_parser.cpp"
    break;

  case 30:
#line 554 "ds_parser.ypp"
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
#line 4367 "ds_parser.cpp"
    break;

  case 31:
#line 575 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4379 "ds_parser.cpp"
    break;

  case 32:
#line 584 "ds_parser.ypp"
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
#line 4403 "ds_parser.cpp"
    break;

  case 34:
#line 610 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4411 "ds_parser.cpp"
    break;

  case 35:
#line 613 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4422 "ds_parser.cpp"
    break;

  case 36:
#line 619 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4433 "ds_parser.cpp"
    break;

  case 37:
#line 628 "ds_parser.ypp"
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
#line 4459 "ds_parser.cpp"
    break;

  case 38:
#line 649 "ds_parser.ypp"
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
#line 4484 "ds_parser.cpp"
    break;

  case 39:
#line 672 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4490 "ds_parser.cpp"
    break;

  case 40:
#line 673 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4496 "ds_parser.cpp"
    break;

  case 44:
#line 686 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4504 "ds_parser.cpp"
    break;

  case 45:
#line 689 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4512 "ds_parser.cpp"
    break;

  case 46:
#line 695 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4520 "ds_parser.cpp"
    break;

  case 47:
#line 701 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4528 "ds_parser.cpp"
    break;

  case 48:
#line 704 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4536 "ds_parser.cpp"
    break;

  case 49:
#line 710 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4542 "ds_parser.cpp"
    break;

  case 50:
#line 711 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4548 "ds_parser.cpp"
    break;

  case 51:
#line 715 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4554 "ds_parser.cpp"
    break;

  case 52:
#line 716 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4560 "ds_parser.cpp"
    break;

  case 53:
#line 717 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4571 "ds_parser.cpp"
    break;

  case 54:
#line 726 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4577 "ds_parser.cpp"
    break;

  case 55:
#line 727 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4583 "ds_parser.cpp"
    break;

  case 56:
#line 731 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4594 "ds_parser.cpp"
    break;

  case 57:
#line 740 "ds_parser.ypp"
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
#line 4612 "ds_parser.cpp"
    break;

  case 58:
#line 756 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4622 "ds_parser.cpp"
    break;

  case 59:
#line 764 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4634 "ds_parser.cpp"
    break;

  case 60:
#line 774 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4645 "ds_parser.cpp"
    break;

  case 61:
#line 783 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4651 "ds_parser.cpp"
    break;

  case 62:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4657 "ds_parser.cpp"
    break;

  case 63:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4663 "ds_parser.cpp"
    break;

  case 64:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4669 "ds_parser.cpp"
    break;

  case 65:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4675 "ds_parser.cpp"
    break;

  case 66:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4681 "ds_parser.cpp"
    break;

  case 67:
#line 792 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4691 "ds_parser.cpp"
    break;

  case 68:
#line 797 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4701 "ds_parser.cpp"
    break;

  case 69:
#line 805 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4707 "ds_parser.cpp"
    break;

  case 70:
#line 806 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4713 "ds_parser.cpp"
    break;

  case 71:
#line 807 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4719 "ds_parser.cpp"
    break;

  case 72:
#line 808 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4725 "ds_parser.cpp"
    break;

  case 73:
#line 809 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4731 "ds_parser.cpp"
    break;

  case 74:
#line 810 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4737 "ds_parser.cpp"
    break;

  case 75:
#line 811 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4743 "ds_parser.cpp"
    break;

  case 76:
#line 812 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4751 "ds_parser.cpp"
    break;

  case 77:
#line 818 "ds_parser.ypp"
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
#line 4770 "ds_parser.cpp"
    break;

  case 78:
#line 832 "ds_parser.ypp"
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
#line 4789 "ds_parser.cpp"
    break;

  case 79:
#line 849 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4795 "ds_parser.cpp"
    break;

  case 80:
#line 850 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4801 "ds_parser.cpp"
    break;

  case 81:
#line 851 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4807 "ds_parser.cpp"
    break;

  case 82:
#line 855 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4820 "ds_parser.cpp"
    break;

  case 83:
#line 863 "ds_parser.ypp"
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
#line 4835 "ds_parser.cpp"
    break;

  case 84:
#line 876 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4844 "ds_parser.cpp"
    break;

  case 85:
#line 880 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4853 "ds_parser.cpp"
    break;

  case 86:
#line 887 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4859 "ds_parser.cpp"
    break;

  case 87:
#line 888 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4865 "ds_parser.cpp"
    break;

  case 88:
#line 892 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4871 "ds_parser.cpp"
    break;

  case 89:
#line 893 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4877 "ds_parser.cpp"
    break;

  case 90:
#line 894 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4883 "ds_parser.cpp"
    break;

  case 91:
#line 898 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4891 "ds_parser.cpp"
    break;

  case 92:
#line 901 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4900 "ds_parser.cpp"
    break;

  case 93:
#line 908 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4909 "ds_parser.cpp"
    break;

  case 94:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4915 "ds_parser.cpp"
    break;

  case 95:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4921 "ds_parser.cpp"
    break;

  case 96:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4927 "ds_parser.cpp"
    break;

  case 97:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4933 "ds_parser.cpp"
    break;

  case 98:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4939 "ds_parser.cpp"
    break;

  case 99:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4945 "ds_parser.cpp"
    break;

  case 100:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4951 "ds_parser.cpp"
    break;

  case 101:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4957 "ds_parser.cpp"
    break;

  case 102:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4963 "ds_parser.cpp"
    break;

  case 103:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4969 "ds_parser.cpp"
    break;

  case 104:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4975 "ds_parser.cpp"
    break;

  case 105:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4981 "ds_parser.cpp"
    break;

  case 106:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 4987 "ds_parser.cpp"
    break;

  case 107:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 4993 "ds_parser.cpp"
    break;

  case 108:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 4999 "ds_parser.cpp"
    break;

  case 109:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5005 "ds_parser.cpp"
    break;

  case 110:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5011 "ds_parser.cpp"
    break;

  case 111:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5017 "ds_parser.cpp"
    break;

  case 112:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5023 "ds_parser.cpp"
    break;

  case 113:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5029 "ds_parser.cpp"
    break;

  case 114:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5035 "ds_parser.cpp"
    break;

  case 115:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5041 "ds_parser.cpp"
    break;

  case 116:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5047 "ds_parser.cpp"
    break;

  case 117:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5053 "ds_parser.cpp"
    break;

  case 118:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5059 "ds_parser.cpp"
    break;

  case 119:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5065 "ds_parser.cpp"
    break;

  case 120:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5071 "ds_parser.cpp"
    break;

  case 121:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5077 "ds_parser.cpp"
    break;

  case 122:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5083 "ds_parser.cpp"
    break;

  case 123:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5089 "ds_parser.cpp"
    break;

  case 124:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5095 "ds_parser.cpp"
    break;

  case 125:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5101 "ds_parser.cpp"
    break;

  case 126:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5107 "ds_parser.cpp"
    break;

  case 127:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5113 "ds_parser.cpp"
    break;

  case 128:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5119 "ds_parser.cpp"
    break;

  case 129:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5125 "ds_parser.cpp"
    break;

  case 130:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5131 "ds_parser.cpp"
    break;

  case 131:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5137 "ds_parser.cpp"
    break;

  case 132:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5143 "ds_parser.cpp"
    break;

  case 133:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5149 "ds_parser.cpp"
    break;

  case 134:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5155 "ds_parser.cpp"
    break;

  case 135:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5161 "ds_parser.cpp"
    break;

  case 136:
#line 957 "ds_parser.ypp"
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
#line 5185 "ds_parser.cpp"
    break;

  case 137:
#line 979 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5191 "ds_parser.cpp"
    break;

  case 138:
#line 980 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5197 "ds_parser.cpp"
    break;

  case 139:
#line 981 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5203 "ds_parser.cpp"
    break;

  case 140:
#line 985 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-2]));
        pFunction->name = *(yyvsp[-2].s);
        pFunction->result = TypeDeclPtr((yyvsp[0].pTypeDecl));
        pFunction->privateFunction = !(yyvsp[-3].b);
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
#line 5244 "ds_parser.cpp"
    break;

  case 141:
#line 1024 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5253 "ds_parser.cpp"
    break;

  case 142:
#line 1031 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5262 "ds_parser.cpp"
    break;

  case 143:
#line 1035 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5275 "ds_parser.cpp"
    break;

  case 144:
#line 1046 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5281 "ds_parser.cpp"
    break;

  case 145:
#line 1047 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5287 "ds_parser.cpp"
    break;

  case 146:
#line 1048 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5293 "ds_parser.cpp"
    break;

  case 147:
#line 1049 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5299 "ds_parser.cpp"
    break;

  case 148:
#line 1050 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5305 "ds_parser.cpp"
    break;

  case 149:
#line 1051 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5311 "ds_parser.cpp"
    break;

  case 150:
#line 1052 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5317 "ds_parser.cpp"
    break;

  case 151:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5323 "ds_parser.cpp"
    break;

  case 152:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5329 "ds_parser.cpp"
    break;

  case 153:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5335 "ds_parser.cpp"
    break;

  case 154:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5341 "ds_parser.cpp"
    break;

  case 155:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5347 "ds_parser.cpp"
    break;

  case 156:
#line 1058 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5353 "ds_parser.cpp"
    break;

  case 157:
#line 1059 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5359 "ds_parser.cpp"
    break;

  case 158:
#line 1060 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5365 "ds_parser.cpp"
    break;

  case 159:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5371 "ds_parser.cpp"
    break;

  case 160:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5377 "ds_parser.cpp"
    break;

  case 161:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5383 "ds_parser.cpp"
    break;

  case 162:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5389 "ds_parser.cpp"
    break;

  case 163:
#line 1068 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5399 "ds_parser.cpp"
    break;

  case 164:
#line 1073 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5410 "ds_parser.cpp"
    break;

  case 165:
#line 1079 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5418 "ds_parser.cpp"
    break;

  case 166:
#line 1085 "ds_parser.ypp"
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
#line 5457 "ds_parser.cpp"
    break;

  case 167:
#line 1119 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5465 "ds_parser.cpp"
    break;

  case 168:
#line 1122 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5473 "ds_parser.cpp"
    break;

  case 169:
#line 1128 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5479 "ds_parser.cpp"
    break;

  case 170:
#line 1129 "ds_parser.ypp"
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
#line 5495 "ds_parser.cpp"
    break;

  case 171:
#line 1140 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5501 "ds_parser.cpp"
    break;

  case 172:
#line 1144 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5509 "ds_parser.cpp"
    break;

  case 173:
#line 1150 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5517 "ds_parser.cpp"
    break;

  case 174:
#line 1153 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5525 "ds_parser.cpp"
    break;

  case 175:
#line 1156 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5534 "ds_parser.cpp"
    break;

  case 176:
#line 1160 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5542 "ds_parser.cpp"
    break;

  case 177:
#line 1166 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5548 "ds_parser.cpp"
    break;

  case 178:
#line 1170 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5554 "ds_parser.cpp"
    break;

  case 179:
#line 1174 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5562 "ds_parser.cpp"
    break;

  case 180:
#line 1177 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5570 "ds_parser.cpp"
    break;

  case 181:
#line 1180 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5580 "ds_parser.cpp"
    break;

  case 182:
#line 1185 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5588 "ds_parser.cpp"
    break;

  case 183:
#line 1188 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5598 "ds_parser.cpp"
    break;

  case 184:
#line 1196 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5606 "ds_parser.cpp"
    break;

  case 185:
#line 1199 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5616 "ds_parser.cpp"
    break;

  case 186:
#line 1204 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5624 "ds_parser.cpp"
    break;

  case 187:
#line 1207 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5634 "ds_parser.cpp"
    break;

  case 188:
#line 1215 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5642 "ds_parser.cpp"
    break;

  case 189:
#line 1221 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5648 "ds_parser.cpp"
    break;

  case 190:
#line 1222 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5654 "ds_parser.cpp"
    break;

  case 191:
#line 1226 "ds_parser.ypp"
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
#line 5690 "ds_parser.cpp"
    break;

  case 192:
#line 1260 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5696 "ds_parser.cpp"
    break;

  case 193:
#line 1260 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5702 "ds_parser.cpp"
    break;

  case 194:
#line 1260 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5710 "ds_parser.cpp"
    break;

  case 195:
#line 1263 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5716 "ds_parser.cpp"
    break;

  case 196:
#line 1263 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5722 "ds_parser.cpp"
    break;

  case 197:
#line 1263 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5732 "ds_parser.cpp"
    break;

  case 198:
#line 1268 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5738 "ds_parser.cpp"
    break;

  case 199:
#line 1268 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5744 "ds_parser.cpp"
    break;

  case 200:
#line 1268 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5754 "ds_parser.cpp"
    break;

  case 201:
#line 1276 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5760 "ds_parser.cpp"
    break;

  case 202:
#line 1276 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5766 "ds_parser.cpp"
    break;

  case 203:
#line 1276 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5774 "ds_parser.cpp"
    break;

  case 204:
#line 1282 "ds_parser.ypp"
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
#line 5789 "ds_parser.cpp"
    break;

  case 205:
#line 1292 "ds_parser.ypp"
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
#line 5805 "ds_parser.cpp"
    break;

  case 206:
#line 1303 "ds_parser.ypp"
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
#line 5822 "ds_parser.cpp"
    break;

  case 207:
#line 1318 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5830 "ds_parser.cpp"
    break;

  case 208:
#line 1321 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5838 "ds_parser.cpp"
    break;

  case 209:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5844 "ds_parser.cpp"
    break;

  case 210:
#line 1328 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5856 "ds_parser.cpp"
    break;

  case 211:
#line 1335 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5869 "ds_parser.cpp"
    break;

  case 212:
#line 1346 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5875 "ds_parser.cpp"
    break;

  case 213:
#line 1347 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5881 "ds_parser.cpp"
    break;

  case 214:
#line 1348 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5887 "ds_parser.cpp"
    break;

  case 215:
#line 1352 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5893 "ds_parser.cpp"
    break;

  case 216:
#line 1353 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5899 "ds_parser.cpp"
    break;

  case 217:
#line 1354 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5905 "ds_parser.cpp"
    break;

  case 218:
#line 1355 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5911 "ds_parser.cpp"
    break;

  case 219:
#line 1359 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5921 "ds_parser.cpp"
    break;

  case 220:
#line 1364 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5931 "ds_parser.cpp"
    break;

  case 221:
#line 1372 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5937 "ds_parser.cpp"
    break;

  case 222:
#line 1373 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5943 "ds_parser.cpp"
    break;

  case 223:
#line 1377 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5953 "ds_parser.cpp"
    break;

  case 224:
#line 1383 "ds_parser.ypp"
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
#line 6018 "ds_parser.cpp"
    break;

  case 225:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6024 "ds_parser.cpp"
    break;

  case 226:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6030 "ds_parser.cpp"
    break;

  case 227:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6036 "ds_parser.cpp"
    break;

  case 228:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6042 "ds_parser.cpp"
    break;

  case 229:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6048 "ds_parser.cpp"
    break;

  case 230:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6054 "ds_parser.cpp"
    break;

  case 231:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6060 "ds_parser.cpp"
    break;

  case 232:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6066 "ds_parser.cpp"
    break;

  case 233:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6072 "ds_parser.cpp"
    break;

  case 234:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6078 "ds_parser.cpp"
    break;

  case 235:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6084 "ds_parser.cpp"
    break;

  case 236:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6090 "ds_parser.cpp"
    break;

  case 237:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6096 "ds_parser.cpp"
    break;

  case 238:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6102 "ds_parser.cpp"
    break;

  case 239:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6108 "ds_parser.cpp"
    break;

  case 240:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6114 "ds_parser.cpp"
    break;

  case 241:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6120 "ds_parser.cpp"
    break;

  case 242:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6126 "ds_parser.cpp"
    break;

  case 243:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6132 "ds_parser.cpp"
    break;

  case 244:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6138 "ds_parser.cpp"
    break;

  case 245:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6144 "ds_parser.cpp"
    break;

  case 246:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6150 "ds_parser.cpp"
    break;

  case 247:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6156 "ds_parser.cpp"
    break;

  case 248:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6162 "ds_parser.cpp"
    break;

  case 249:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6168 "ds_parser.cpp"
    break;

  case 250:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6174 "ds_parser.cpp"
    break;

  case 251:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6180 "ds_parser.cpp"
    break;

  case 252:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6186 "ds_parser.cpp"
    break;

  case 253:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6192 "ds_parser.cpp"
    break;

  case 254:
#line 1484 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6204 "ds_parser.cpp"
    break;

  case 255:
#line 1495 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6214 "ds_parser.cpp"
    break;

  case 256:
#line 1500 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6226 "ds_parser.cpp"
    break;

  case 257:
#line 1510 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6235 "ds_parser.cpp"
    break;

  case 258:
#line 1514 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6241 "ds_parser.cpp"
    break;

  case 259:
#line 1514 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6247 "ds_parser.cpp"
    break;

  case 260:
#line 1514 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6259 "ds_parser.cpp"
    break;

  case 261:
#line 1521 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6265 "ds_parser.cpp"
    break;

  case 262:
#line 1521 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6271 "ds_parser.cpp"
    break;

  case 263:
#line 1521 "ds_parser.ypp"
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
#line 6287 "ds_parser.cpp"
    break;

  case 264:
#line 1535 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6296 "ds_parser.cpp"
    break;

  case 265:
#line 1539 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6302 "ds_parser.cpp"
    break;

  case 266:
#line 1539 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6308 "ds_parser.cpp"
    break;

  case 267:
#line 1539 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6317 "ds_parser.cpp"
    break;

  case 268:
#line 1546 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6323 "ds_parser.cpp"
    break;

  case 269:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6329 "ds_parser.cpp"
    break;

  case 270:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6335 "ds_parser.cpp"
    break;

  case 271:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6341 "ds_parser.cpp"
    break;

  case 272:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6347 "ds_parser.cpp"
    break;

  case 273:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6353 "ds_parser.cpp"
    break;

  case 274:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6359 "ds_parser.cpp"
    break;

  case 275:
#line 1553 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6365 "ds_parser.cpp"
    break;

  case 276:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6371 "ds_parser.cpp"
    break;

  case 277:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6377 "ds_parser.cpp"
    break;

  case 278:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6383 "ds_parser.cpp"
    break;

  case 279:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6389 "ds_parser.cpp"
    break;

  case 280:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6395 "ds_parser.cpp"
    break;

  case 281:
#line 1559 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6401 "ds_parser.cpp"
    break;

  case 282:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6407 "ds_parser.cpp"
    break;

  case 283:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6413 "ds_parser.cpp"
    break;

  case 284:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6419 "ds_parser.cpp"
    break;

  case 285:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6425 "ds_parser.cpp"
    break;

  case 286:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6431 "ds_parser.cpp"
    break;

  case 287:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6437 "ds_parser.cpp"
    break;

  case 288:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6443 "ds_parser.cpp"
    break;

  case 289:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6449 "ds_parser.cpp"
    break;

  case 290:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6455 "ds_parser.cpp"
    break;

  case 291:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6461 "ds_parser.cpp"
    break;

  case 292:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6467 "ds_parser.cpp"
    break;

  case 293:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6473 "ds_parser.cpp"
    break;

  case 294:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6479 "ds_parser.cpp"
    break;

  case 295:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6485 "ds_parser.cpp"
    break;

  case 296:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6491 "ds_parser.cpp"
    break;

  case 297:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6497 "ds_parser.cpp"
    break;

  case 298:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6503 "ds_parser.cpp"
    break;

  case 299:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6509 "ds_parser.cpp"
    break;

  case 300:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6515 "ds_parser.cpp"
    break;

  case 301:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6521 "ds_parser.cpp"
    break;

  case 302:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6527 "ds_parser.cpp"
    break;

  case 303:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6533 "ds_parser.cpp"
    break;

  case 304:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6539 "ds_parser.cpp"
    break;

  case 305:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6545 "ds_parser.cpp"
    break;

  case 306:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6551 "ds_parser.cpp"
    break;

  case 307:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6557 "ds_parser.cpp"
    break;

  case 308:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6563 "ds_parser.cpp"
    break;

  case 309:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6569 "ds_parser.cpp"
    break;

  case 310:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6575 "ds_parser.cpp"
    break;

  case 311:
#line 1589 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6584 "ds_parser.cpp"
    break;

  case 312:
#line 1593 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6593 "ds_parser.cpp"
    break;

  case 313:
#line 1597 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6601 "ds_parser.cpp"
    break;

  case 314:
#line 1600 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6609 "ds_parser.cpp"
    break;

  case 315:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 316:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6621 "ds_parser.cpp"
    break;

  case 317:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6627 "ds_parser.cpp"
    break;

  case 318:
#line 1606 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6641 "ds_parser.cpp"
    break;

  case 319:
#line 1615 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6655 "ds_parser.cpp"
    break;

  case 320:
#line 1624 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6661 "ds_parser.cpp"
    break;

  case 321:
#line 1625 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6669 "ds_parser.cpp"
    break;

  case 322:
#line 1628 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6675 "ds_parser.cpp"
    break;

  case 323:
#line 1628 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6681 "ds_parser.cpp"
    break;

  case 324:
#line 1628 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6689 "ds_parser.cpp"
    break;

  case 325:
#line 1631 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6698 "ds_parser.cpp"
    break;

  case 326:
#line 1635 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6707 "ds_parser.cpp"
    break;

  case 327:
#line 1639 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6716 "ds_parser.cpp"
    break;

  case 328:
#line 1643 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6722 "ds_parser.cpp"
    break;

  case 329:
#line 1644 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6728 "ds_parser.cpp"
    break;

  case 330:
#line 1645 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6734 "ds_parser.cpp"
    break;

  case 331:
#line 1646 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6740 "ds_parser.cpp"
    break;

  case 332:
#line 1647 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6746 "ds_parser.cpp"
    break;

  case 333:
#line 1648 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6752 "ds_parser.cpp"
    break;

  case 334:
#line 1649 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6758 "ds_parser.cpp"
    break;

  case 335:
#line 1650 "ds_parser.ypp"
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
#line 6779 "ds_parser.cpp"
    break;

  case 336:
#line 1666 "ds_parser.ypp"
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
#line 6800 "ds_parser.cpp"
    break;

  case 337:
#line 1682 "ds_parser.ypp"
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
#line 6860 "ds_parser.cpp"
    break;

  case 338:
#line 1740 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6866 "ds_parser.cpp"
    break;

  case 339:
#line 1741 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6872 "ds_parser.cpp"
    break;

  case 340:
#line 1745 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6878 "ds_parser.cpp"
    break;

  case 341:
#line 1746 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6884 "ds_parser.cpp"
    break;

  case 342:
#line 1750 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6894 "ds_parser.cpp"
    break;

  case 343:
#line 1758 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6902 "ds_parser.cpp"
    break;

  case 344:
#line 1761 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6911 "ds_parser.cpp"
    break;

  case 345:
#line 1766 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6917 "ds_parser.cpp"
    break;

  case 346:
#line 1766 "ds_parser.ypp"
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
#line 6966 "ds_parser.cpp"
    break;

  case 347:
#line 1810 "ds_parser.ypp"
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
#line 7027 "ds_parser.cpp"
    break;

  case 348:
#line 1866 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7038 "ds_parser.cpp"
    break;

  case 349:
#line 1875 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7048 "ds_parser.cpp"
    break;

  case 350:
#line 1880 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7058 "ds_parser.cpp"
    break;

  case 351:
#line 1888 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7064 "ds_parser.cpp"
    break;

  case 352:
#line 1889 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7070 "ds_parser.cpp"
    break;

  case 353:
#line 1893 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7078 "ds_parser.cpp"
    break;

  case 354:
#line 1896 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7089 "ds_parser.cpp"
    break;

  case 355:
#line 1905 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7095 "ds_parser.cpp"
    break;

  case 356:
#line 1906 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7101 "ds_parser.cpp"
    break;

  case 357:
#line 1910 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7112 "ds_parser.cpp"
    break;

  case 358:
#line 1919 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7118 "ds_parser.cpp"
    break;

  case 359:
#line 1920 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7124 "ds_parser.cpp"
    break;

  case 360:
#line 1925 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7130 "ds_parser.cpp"
    break;

  case 361:
#line 1926 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7136 "ds_parser.cpp"
    break;

  case 362:
#line 1930 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7147 "ds_parser.cpp"
    break;

  case 363:
#line 1936 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7158 "ds_parser.cpp"
    break;

  case 364:
#line 1942 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7166 "ds_parser.cpp"
    break;

  case 365:
#line 1945 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7175 "ds_parser.cpp"
    break;

  case 366:
#line 1949 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7186 "ds_parser.cpp"
    break;

  case 367:
#line 1955 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7197 "ds_parser.cpp"
    break;

  case 368:
#line 1964 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7203 "ds_parser.cpp"
    break;

  case 369:
#line 1965 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7209 "ds_parser.cpp"
    break;

  case 370:
#line 1966 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7215 "ds_parser.cpp"
    break;

  case 371:
#line 1970 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7221 "ds_parser.cpp"
    break;

  case 372:
#line 1971 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7227 "ds_parser.cpp"
    break;

  case 373:
#line 1975 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7235 "ds_parser.cpp"
    break;

  case 374:
#line 1978 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7245 "ds_parser.cpp"
    break;

  case 375:
#line 1983 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7255 "ds_parser.cpp"
    break;

  case 376:
#line 1988 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7268 "ds_parser.cpp"
    break;

  case 377:
#line 1996 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7281 "ds_parser.cpp"
    break;

  case 378:
#line 2007 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7289 "ds_parser.cpp"
    break;

  case 379:
#line 2010 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7298 "ds_parser.cpp"
    break;

  case 380:
#line 2017 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7304 "ds_parser.cpp"
    break;

  case 381:
#line 2018 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7310 "ds_parser.cpp"
    break;

  case 382:
#line 2022 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7316 "ds_parser.cpp"
    break;

  case 383:
#line 2023 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7322 "ds_parser.cpp"
    break;

  case 384:
#line 2024 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7328 "ds_parser.cpp"
    break;

  case 385:
#line 2028 "ds_parser.ypp"
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
                    if ( (yyvsp[-5].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pVar->global_shared = (yyvsp[-4].b);
                    pVar->private_variable = !(yyvsp[-3].b);
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7361 "ds_parser.cpp"
    break;

  case 386:
#line 2056 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7367 "ds_parser.cpp"
    break;

  case 387:
#line 2056 "ds_parser.ypp"
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
                if ( (yyvsp[-4].b) ) {
                    pVar->type->constant = true;
                } else {
                    pVar->type->removeConstant = true;
                }
                pVar->global_shared = (yyvsp[-3].b);
                pVar->private_variable = !(yyvsp[-2].b);
                if ( !g_Program->addVariable(pVar) )
                    das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }
#line 7399 "ds_parser.cpp"
    break;

  case 388:
#line 2086 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7407 "ds_parser.cpp"
    break;

  case 389:
#line 2089 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7421 "ds_parser.cpp"
    break;

  case 390:
#line 2098 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7435 "ds_parser.cpp"
    break;

  case 391:
#line 2111 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7449 "ds_parser.cpp"
    break;

  case 395:
#line 2132 "ds_parser.ypp"
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
#line 7465 "ds_parser.cpp"
    break;

  case 396:
#line 2143 "ds_parser.ypp"
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
#line 7482 "ds_parser.cpp"
    break;

  case 397:
#line 2158 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7488 "ds_parser.cpp"
    break;

  case 398:
#line 2159 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7494 "ds_parser.cpp"
    break;

  case 399:
#line 2163 "ds_parser.ypp"
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
#line 7533 "ds_parser.cpp"
    break;

  case 400:
#line 2200 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7539 "ds_parser.cpp"
    break;

  case 401:
#line 2201 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7545 "ds_parser.cpp"
    break;

  case 402:
#line 2205 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7551 "ds_parser.cpp"
    break;

  case 403:
#line 2206 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7557 "ds_parser.cpp"
    break;

  case 404:
#line 2207 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7563 "ds_parser.cpp"
    break;

  case 405:
#line 2212 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7569 "ds_parser.cpp"
    break;

  case 406:
#line 2212 "ds_parser.ypp"
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
            if ( (yyvsp[-7].faList) ) {
                for ( auto pA : *(yyvsp[-7].faList) ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isStructureAnnotation() ) {
                            auto ann = static_pointer_cast<StructureAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->touch(pStruct, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                                das_yyerror("can't 'touch' with structure annotation\n" + err,
                                    tokAt((yylsp[-6])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-7].faList)->size()!=1 ) {
                                das_yyerror("structures are only allowed one structure type annotation", tokAt((yylsp[-6])),
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror("handled structure is already defined "+pStruct->name,tokAt((yylsp[-6])),
                                        CompilationError::structure_already_declared);
                                } else {
                                    pStruct->module->removeStructure(pStruct);
                                }
                            }
                        }
                    }
                }
                swap ( pStruct->annotations, *(yyvsp[-7].faList) );
                delete (yyvsp[-7].faList);
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        g_thisStructure = nullptr;
    }
#line 7655 "ds_parser.cpp"
    break;

  case 407:
#line 2296 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7667 "ds_parser.cpp"
    break;

  case 408:
#line 2303 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7678 "ds_parser.cpp"
    break;

  case 409:
#line 2312 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7684 "ds_parser.cpp"
    break;

  case 410:
#line 2313 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7690 "ds_parser.cpp"
    break;

  case 411:
#line 2314 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7696 "ds_parser.cpp"
    break;

  case 412:
#line 2315 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7702 "ds_parser.cpp"
    break;

  case 413:
#line 2316 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7708 "ds_parser.cpp"
    break;

  case 414:
#line 2317 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7714 "ds_parser.cpp"
    break;

  case 415:
#line 2318 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7720 "ds_parser.cpp"
    break;

  case 416:
#line 2319 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7726 "ds_parser.cpp"
    break;

  case 417:
#line 2320 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7732 "ds_parser.cpp"
    break;

  case 418:
#line 2321 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7738 "ds_parser.cpp"
    break;

  case 419:
#line 2322 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7744 "ds_parser.cpp"
    break;

  case 420:
#line 2323 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7750 "ds_parser.cpp"
    break;

  case 421:
#line 2324 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7756 "ds_parser.cpp"
    break;

  case 422:
#line 2325 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7762 "ds_parser.cpp"
    break;

  case 423:
#line 2326 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7768 "ds_parser.cpp"
    break;

  case 424:
#line 2327 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7774 "ds_parser.cpp"
    break;

  case 425:
#line 2328 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7780 "ds_parser.cpp"
    break;

  case 426:
#line 2329 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7786 "ds_parser.cpp"
    break;

  case 427:
#line 2330 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7792 "ds_parser.cpp"
    break;

  case 428:
#line 2331 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7798 "ds_parser.cpp"
    break;

  case 429:
#line 2332 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7804 "ds_parser.cpp"
    break;

  case 430:
#line 2333 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7810 "ds_parser.cpp"
    break;

  case 431:
#line 2334 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7816 "ds_parser.cpp"
    break;

  case 432:
#line 2335 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7822 "ds_parser.cpp"
    break;

  case 433:
#line 2336 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7828 "ds_parser.cpp"
    break;

  case 434:
#line 2340 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7834 "ds_parser.cpp"
    break;

  case 435:
#line 2341 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7840 "ds_parser.cpp"
    break;

  case 436:
#line 2342 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7846 "ds_parser.cpp"
    break;

  case 437:
#line 2343 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7852 "ds_parser.cpp"
    break;

  case 438:
#line 2344 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7858 "ds_parser.cpp"
    break;

  case 439:
#line 2345 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7864 "ds_parser.cpp"
    break;

  case 440:
#line 2349 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7877 "ds_parser.cpp"
    break;

  case 441:
#line 2360 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7886 "ds_parser.cpp"
    break;

  case 442:
#line 2364 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7898 "ds_parser.cpp"
    break;

  case 443:
#line 2374 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7910 "ds_parser.cpp"
    break;

  case 444:
#line 2381 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7921 "ds_parser.cpp"
    break;

  case 445:
#line 2390 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7927 "ds_parser.cpp"
    break;

  case 446:
#line 2390 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7933 "ds_parser.cpp"
    break;

  case 447:
#line 2390 "ds_parser.ypp"
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
#line 7948 "ds_parser.cpp"
    break;

  case 448:
#line 2403 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7954 "ds_parser.cpp"
    break;

  case 449:
#line 2404 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7960 "ds_parser.cpp"
    break;

  case 450:
#line 2405 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7966 "ds_parser.cpp"
    break;

  case 451:
#line 2406 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7972 "ds_parser.cpp"
    break;

  case 452:
#line 2407 "ds_parser.ypp"
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
#line 7991 "ds_parser.cpp"
    break;

  case 453:
#line 2421 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8002 "ds_parser.cpp"
    break;

  case 454:
#line 2427 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8011 "ds_parser.cpp"
    break;

  case 455:
#line 2431 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8020 "ds_parser.cpp"
    break;

  case 456:
#line 2435 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8030 "ds_parser.cpp"
    break;

  case 457:
#line 2440 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8040 "ds_parser.cpp"
    break;

  case 458:
#line 2445 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8050 "ds_parser.cpp"
    break;

  case 459:
#line 2450 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8060 "ds_parser.cpp"
    break;

  case 460:
#line 2455 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8069 "ds_parser.cpp"
    break;

  case 461:
#line 2459 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8078 "ds_parser.cpp"
    break;

  case 462:
#line 2463 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8088 "ds_parser.cpp"
    break;

  case 463:
#line 2468 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8097 "ds_parser.cpp"
    break;

  case 464:
#line 2472 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8107 "ds_parser.cpp"
    break;

  case 465:
#line 2477 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8113 "ds_parser.cpp"
    break;

  case 466:
#line 2477 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8119 "ds_parser.cpp"
    break;

  case 467:
#line 2477 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8130 "ds_parser.cpp"
    break;

  case 468:
#line 2483 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8142 "ds_parser.cpp"
    break;

  case 469:
#line 2490 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8148 "ds_parser.cpp"
    break;

  case 470:
#line 2490 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8154 "ds_parser.cpp"
    break;

  case 471:
#line 2490 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8164 "ds_parser.cpp"
    break;

  case 472:
#line 2495 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8170 "ds_parser.cpp"
    break;

  case 473:
#line 2495 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8176 "ds_parser.cpp"
    break;

  case 474:
#line 2495 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8187 "ds_parser.cpp"
    break;

  case 475:
#line 2501 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8193 "ds_parser.cpp"
    break;

  case 476:
#line 2501 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8199 "ds_parser.cpp"
    break;

  case 477:
#line 2501 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8209 "ds_parser.cpp"
    break;

  case 478:
#line 2506 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8218 "ds_parser.cpp"
    break;

  case 479:
#line 2510 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8224 "ds_parser.cpp"
    break;

  case 480:
#line 2510 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8230 "ds_parser.cpp"
    break;

  case 481:
#line 2510 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8240 "ds_parser.cpp"
    break;

  case 482:
#line 2515 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8246 "ds_parser.cpp"
    break;

  case 483:
#line 2515 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8252 "ds_parser.cpp"
    break;

  case 484:
#line 2515 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8266 "ds_parser.cpp"
    break;

  case 485:
#line 2524 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8275 "ds_parser.cpp"
    break;

  case 486:
#line 2528 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8281 "ds_parser.cpp"
    break;

  case 487:
#line 2528 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8287 "ds_parser.cpp"
    break;

  case 488:
#line 2528 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8297 "ds_parser.cpp"
    break;

  case 489:
#line 2533 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8303 "ds_parser.cpp"
    break;

  case 490:
#line 2533 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8309 "ds_parser.cpp"
    break;

  case 491:
#line 2533 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8323 "ds_parser.cpp"
    break;

  case 492:
#line 2542 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8332 "ds_parser.cpp"
    break;

  case 493:
#line 2546 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8338 "ds_parser.cpp"
    break;

  case 494:
#line 2546 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8344 "ds_parser.cpp"
    break;

  case 495:
#line 2546 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8354 "ds_parser.cpp"
    break;

  case 496:
#line 2551 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8360 "ds_parser.cpp"
    break;

  case 497:
#line 2551 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8366 "ds_parser.cpp"
    break;

  case 498:
#line 2551 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8380 "ds_parser.cpp"
    break;

  case 499:
#line 2560 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8386 "ds_parser.cpp"
    break;

  case 500:
#line 2560 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8392 "ds_parser.cpp"
    break;

  case 501:
#line 2560 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8403 "ds_parser.cpp"
    break;

  case 502:
#line 2566 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8409 "ds_parser.cpp"
    break;

  case 503:
#line 2566 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8415 "ds_parser.cpp"
    break;

  case 504:
#line 2566 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8426 "ds_parser.cpp"
    break;

  case 505:
#line 2575 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8432 "ds_parser.cpp"
    break;

  case 506:
#line 2575 "ds_parser.ypp"
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
#line 8449 "ds_parser.cpp"
    break;

  case 507:
#line 2590 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8455 "ds_parser.cpp"
    break;

  case 508:
#line 2590 "ds_parser.ypp"
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
#line 8476 "ds_parser.cpp"
    break;

  case 509:
#line 2610 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8482 "ds_parser.cpp"
    break;

  case 510:
#line 2611 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8488 "ds_parser.cpp"
    break;

  case 511:
#line 2612 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8494 "ds_parser.cpp"
    break;

  case 512:
#line 2613 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8500 "ds_parser.cpp"
    break;

  case 513:
#line 2617 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8512 "ds_parser.cpp"
    break;

  case 514:
#line 2624 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8524 "ds_parser.cpp"
    break;

  case 515:
#line 2631 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8535 "ds_parser.cpp"
    break;

  case 516:
#line 2637 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8546 "ds_parser.cpp"
    break;

  case 517:
#line 2646 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8556 "ds_parser.cpp"
    break;

  case 518:
#line 2651 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8565 "ds_parser.cpp"
    break;

  case 519:
#line 2658 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8571 "ds_parser.cpp"
    break;

  case 520:
#line 2659 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8577 "ds_parser.cpp"
    break;

  case 521:
#line 2663 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8588 "ds_parser.cpp"
    break;

  case 522:
#line 2669 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8600 "ds_parser.cpp"
    break;

  case 523:
#line 2676 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8613 "ds_parser.cpp"
    break;

  case 524:
#line 2684 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8625 "ds_parser.cpp"
    break;

  case 525:
#line 2691 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8638 "ds_parser.cpp"
    break;

  case 526:
#line 2699 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8652 "ds_parser.cpp"
    break;

  case 527:
#line 2711 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8660 "ds_parser.cpp"
    break;

  case 528:
#line 2714 "ds_parser.ypp"
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
#line 8676 "ds_parser.cpp"
    break;

  case 529:
#line 2728 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8687 "ds_parser.cpp"
    break;

  case 530:
#line 2737 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8693 "ds_parser.cpp"
    break;

  case 531:
#line 2738 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8699 "ds_parser.cpp"
    break;

  case 532:
#line 2742 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8709 "ds_parser.cpp"
    break;

  case 533:
#line 2747 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8718 "ds_parser.cpp"
    break;

  case 534:
#line 2754 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8728 "ds_parser.cpp"
    break;

  case 535:
#line 2759 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8740 "ds_parser.cpp"
    break;

  case 536:
#line 2769 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8750 "ds_parser.cpp"
    break;

  case 537:
#line 2774 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8759 "ds_parser.cpp"
    break;

  case 538:
#line 2781 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8773 "ds_parser.cpp"
    break;

  case 539:
#line 2793 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8779 "ds_parser.cpp"
    break;

  case 540:
#line 2794 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8785 "ds_parser.cpp"
    break;

  case 541:
#line 2798 "ds_parser.ypp"
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
#line 8808 "ds_parser.cpp"
    break;

  case 542:
#line 2816 "ds_parser.ypp"
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
#line 8831 "ds_parser.cpp"
    break;


#line 8835 "ds_parser.cpp"

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
#line 2836 "ds_parser.ypp"


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
