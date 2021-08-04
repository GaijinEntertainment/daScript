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
#define YYLAST   7413

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  186
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  178
/* YYNRULES -- Number of rules.  */
#define YYNRULES  545
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  954

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
       0,   451,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   467,   468,   469,   473,   474,
     478,   495,   496,   500,   504,   509,   518,   526,   542,   547,
     555,   555,   585,   607,   611,   614,   620,   629,   650,   673,
     674,   678,   682,   683,   687,   690,   696,   702,   705,   711,
     712,   716,   717,   718,   727,   728,   732,   741,   757,   765,
     775,   784,   785,   786,   787,   788,   789,   793,   798,   806,
     807,   808,   809,   810,   811,   812,   813,   819,   833,   850,
     851,   852,   856,   864,   877,   881,   888,   889,   893,   894,
     895,   899,   902,   909,   913,   914,   915,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   932,   933,   934,   935,   936,   937,   938,
     939,   940,   941,   942,   943,   944,   945,   946,   947,   948,
     949,   950,   951,   952,   953,   954,   958,   980,   981,   982,
     986,  1025,  1032,  1036,  1047,  1048,  1049,  1050,  1051,  1052,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,
    1063,  1064,  1065,  1069,  1074,  1080,  1086,  1120,  1123,  1129,
    1130,  1141,  1145,  1151,  1154,  1157,  1161,  1167,  1171,  1175,
    1178,  1181,  1186,  1189,  1197,  1200,  1205,  1208,  1216,  1222,
    1223,  1227,  1261,  1261,  1261,  1264,  1264,  1264,  1269,  1269,
    1269,  1277,  1277,  1277,  1283,  1293,  1304,  1319,  1322,  1328,
    1329,  1336,  1347,  1348,  1349,  1353,  1354,  1355,  1356,  1360,
    1365,  1373,  1374,  1378,  1383,  1447,  1448,  1449,  1450,  1451,
    1452,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,  1464,
    1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,
    1478,  1479,  1480,  1481,  1485,  1496,  1501,  1511,  1515,  1515,
    1515,  1522,  1522,  1522,  1536,  1540,  1540,  1540,  1547,  1548,
    1549,  1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,
    1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,
    1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,
    1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1590,  1594,  1598,  1601,  1604,  1605,  1606,  1607,  1616,
    1625,  1626,  1629,  1629,  1629,  1632,  1636,  1640,  1644,  1645,
    1646,  1647,  1648,  1649,  1650,  1651,  1667,  1683,  1741,  1742,
    1746,  1747,  1751,  1759,  1762,  1767,  1766,  1811,  1867,  1876,
    1881,  1889,  1890,  1894,  1897,  1906,  1907,  1911,  1920,  1921,
    1926,  1927,  1931,  1937,  1943,  1946,  1950,  1956,  1965,  1966,
    1967,  1971,  1972,  1976,  1979,  1984,  1989,  1997,  2008,  2011,
    2018,  2019,  2023,  2024,  2025,  2029,  2057,  2057,  2087,  2090,
    2099,  2112,  2124,  2125,  2129,  2133,  2134,  2135,  2139,  2167,
    2199,  2200,  2204,  2241,  2242,  2246,  2247,  2248,  2253,  2252,
    2337,  2344,  2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,
    2361,  2362,  2363,  2364,  2365,  2366,  2367,  2368,  2369,  2370,
    2371,  2372,  2373,  2374,  2375,  2376,  2377,  2381,  2382,  2383,
    2384,  2385,  2386,  2390,  2401,  2405,  2415,  2422,  2431,  2431,
    2431,  2444,  2445,  2446,  2447,  2448,  2462,  2468,  2472,  2476,
    2481,  2486,  2491,  2496,  2500,  2504,  2509,  2513,  2518,  2518,
    2518,  2524,  2531,  2531,  2531,  2536,  2536,  2536,  2542,  2542,
    2542,  2547,  2551,  2551,  2551,  2556,  2556,  2556,  2565,  2569,
    2569,  2569,  2574,  2574,  2574,  2583,  2587,  2587,  2587,  2592,
    2592,  2592,  2601,  2601,  2601,  2607,  2607,  2607,  2616,  2616,
    2631,  2631,  2651,  2652,  2653,  2654,  2658,  2665,  2672,  2678,
    2687,  2692,  2699,  2700,  2704,  2710,  2717,  2725,  2732,  2740,
    2752,  2755,  2769,  2778,  2779,  2783,  2788,  2795,  2800,  2810,
    2815,  2822,  2834,  2835,  2839,  2857
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
  "single_alias", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_declaration",
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

#define YYPACT_NINF -598

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-598)))

#define YYTABLE_NINF -500

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -598,    33,  -598,  -598,   -84,     1,   -70,    78,  -598,    -8,
    -598,  -598,     9,  -598,  -598,  -598,  -598,  -598,   237,  -598,
     128,  -598,  -598,  -598,  -598,  -598,  -598,   117,  -598,   136,
     145,   139,  -598,  -598,    20,  -598,   157,  -598,  -598,   128,
     159,   187,  -598,  -598,   214,   200,   182,  -598,    68,  -598,
    -598,  -598,    80,   277,   322,  -598,   333,    21,   -84,   225,
       1,   231,  -598,   239,   246,  -598,   240,  -598,   -90,   350,
     211,   213,  -598,   259,   -84,     9,  -598,  -598,  -598,     0,
     223,  -598,  -598,  -598,   262,  -598,  -598,   265,  -598,  -598,
     230,  -598,  -598,  -598,  -598,  -598,   275,    85,  -598,  -598,
    -598,  -598,   370,  -598,  -598,  1258,  -598,  -598,  -598,  -598,
    -598,   279,   280,  -598,   -98,  -598,  7244,   392,   399,  -598,
     257,  -598,  -598,   -36,   282,  -598,  -598,   293,  -598,   104,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,    44,  -598,   283,
     284,   285,   286,  -598,  -598,  -598,   263,  -598,  -598,  -598,
    -598,  -598,   287,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,   288,  -598,  -598,  -598,   289,   290,  -598,  -598,
    -598,  -598,   291,   294,  -598,  -598,  -598,  -598,  -598,    58,
    -598,   272,   300,  -598,   274,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,   -92,   310,
    1384,    54,  -598,   -69,  -598,   292,   -71,  -598,  -598,   250,
    -598,  -598,    85,  -598,  -598,  -598,  -598,  -598,   315,  -598,
     114,   184,   185,  -598,  -598,  -598,  -598,  -598,  -598,   434,
    -598,  -598,    50,  2905,  -598,  -598,   -56,  1258,   -54,   -84,
    -598,    -9,  -598,   -50,  1258,  -598,  -598,  4531,  -598,  -598,
     293,  -598,  -598,  7157,   295,   308,  2039,  -598,  -598,   223,
    4531,   298,  4531,   311,   312,   302,  -598,   299,   313,   330,
    3043,   223,  -598,   324,  2197,  4531,  4531,   191,   191,  6985,
    7071,  4531,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  4531,
    4531,  4531,   -69,  4531,  4531,  4531,  -598,   303,  -598,   449,
    -598,  -598,   304,   306,  4531,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,   -86,   317,  -598,   318,   320,  -598,  -598,
    -598,   293,  -598,  -598,  -598,  -598,   319,  -598,  -598,   -73,
    -598,  -598,  -598,  -598,  -598,  6174,   309,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,   321,   -22,
    -598,  -598,  -598,  -598,   351,  1258,  -598,   -43,  -598,  1258,
    1258,  1258,  1258,   328,   279,  1258,   257,  1258,   257,  1258,
     257,  7243,   280,  -598,  -598,  -598,   326,  -598,  -598,  5836,
    -598,  -598,  3707,  -598,  -598,   106,   293,  -598,   -57,  -598,
     376,  3707,   488,    -1,   743,  -598,   -69,  -598,  2335,  -598,
    -598,   395,  4620,   502,  6466,  4531,   488,  -598,  -598,  4531,
    -598,  -598,   361,   378,  6466,  -598,  1258,  2335,  -598,  4719,
      63,    63,   337,  -598,  -598,   293,  1131,   293,  1727,  6241,
    -598,  -117,   269,    63,    63,   -64,  -598,    63,    63,  5288,
      48,   336,  -598,  -598,   488,  -598,  2473,  -598,  -598,  -598,
    -598,   384,   191,  -598,    25,   379,  4531,  4531,  4531,  4531,
    4531,  4531,  4531,  4531,  4531,  4531,  -598,  -598,  4531,  4531,
    4531,  4531,  4531,  4531,   380,  2629,  4531,   381,  4531,  4531,
    4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,
    4531,  4531,  2767,  3181,  4531,  4531,  4531,  4531,  4531,  4531,
    4531,  4531,  4531,  4531,   382,  4531,  3319,  -598,    22,  -598,
      16,  -598,   383,  -598,  -598,  -598,  2335,  1069,  1326,  1670,
    1821,  -598,  -104,  1981,   310,  2138,   310,  2273,   310,   121,
    -598,   149,  3707,   173,  -598,  -598,   352,  -598,  -598,  -598,
    1258,  -598,  2335,  -598,  -598,  4531,  3475,  3631,  1258,  -598,
    4818,  4531,  4531,  -598,   223,  5365,  -598,  1258,  1258,  5445,
    1258,  -598,  -598,  2411,  -598,  4917,  -598,  -598,     5,   191,
     -26,  1883,  3769,  6241,   371,   -23,   353,   387,  -598,  -598,
     179,    11,  3925,   -23,    19,  4531,  4531,   344,  -598,  4531,
     385,   386,  -598,   210,  -598,  -598,    47,   390,  -598,    46,
    6466,   -37,   257,  -598,   372,  -598,  -598,  6466,  6466,  6466,
    6466,  6466,  6466,  6466,  6466,   236,   236,   162,  6466,  6466,
     162,   594,   594,   362,  4531,  4531,  6466,   346,  -598,  5911,
     -68,   -68,  6466,   236,   236,  6466,  6466,  6640,  6533,  6612,
    6466,  6466,  6466,  4531,  4531,  6466,   397,  6320,   933,  6718,
    6864,   162,   162,   314,   314,   346,   346,   346,  -598,   542,
    5986,  -598,    75,   -10,  4531,  -598,     9,  -598,   538,   504,
     365,   434,  -598,  2335,  -598,  5016,  -598,  1258,  -598,  -598,
    -598,   403,  -598,   389,  -598,   391,  -598,   393,  1258,  -598,
    7243,  -598,   280,  -598,  2567,  -598,  6074,   -34,  -598,    84,
     405,  5522,  2705,  -598,  -598,  -598,  2843,  2981,  -598,  3119,
     384,  -598,  4531,  -598,  4531,  4531,   -11,   407,   390,   377,
     394,  4531,   396,  4531,  4531,   390,   373,   400,  6466,  -598,
    -598,  6387,  -598,   408,  1258,   257,  1568,   223,  -598,  -598,
    4531,  -598,     3,   108,  4531,  -598,   411,   415,   416,   417,
    -598,   129,   310,  -598,  4081,  6788,  6466,  -598,  6788,  6466,
    -598,  4531,  -598,  -598,  -598,  -598,   712,   130,   305,  -598,
     293,  -598,  -598,  5115,  -598,  -598,  3257,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  3707,  -598,  -598,  -598,
     434,  4531,   223,  -598,   186,  -598,  -598,  -598,  -598,  -598,
     401,     6,  6466,  6466,   -23,   404,     8,   371,   406,  -598,
    6466,  -598,  -598,    66,   -23,   409,  -598,  -598,  -598,  3413,
     310,  -598,  -598,   488,   388,  6466,  -598,  -598,  -598,  -598,
     -37,   410,   -81,  1258,  -598,    94,  6466,  -598,  -598,   398,
    -598,    80,  -598,  -598,  -598,  -598,  -598,  -598,  6466,  -598,
    4531,   418,  -598,  4531,  4531,  4531,  4237,  4531,   412,   413,
    4531,  4531,  -598,  4531,   423,  -598,  -598,   414,    47,  -598,
    -598,  -598,  4393,  -598,  -598,  3569,  -598,  -598,    80,  -598,
    -598,  5602,   422,   162,   162,   162,  -598,  5679,  5214,   419,
    -598,  6466,  6466,  5214,   420,   -69,  -598,  -598,  4531,  6466,
    -598,   402,  -598,  4531,  -598,   515,   425,  -598,   424,  -598,
    -598,   -69,  6466,  -598,  -598,  5759,  4531,   432,   439,  -598,
    -598,  6466,  -598,  -598
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    86,     1,   189,     0,     0,     0,     0,   190,     0,
     510,   508,     0,    14,     3,    10,     9,     8,     0,     7,
     380,     6,    11,     5,     4,    12,    13,    75,    77,    32,
      44,    41,    42,    34,    39,    33,     0,    19,    18,   380,
       0,     0,    80,    81,     0,   169,    82,    84,     0,    79,
     404,   403,   137,   395,   405,   381,   382,     0,     0,     0,
       0,     0,    40,     0,     0,    37,     0,   392,     0,    15,
       0,     0,   171,     0,     0,     0,    87,   139,   138,     0,
       0,   136,   397,   396,     0,   407,   406,     0,   384,   383,
     386,    73,    74,    71,    72,    70,     0,     0,    69,    78,
      45,    43,    39,    36,    35,     0,   394,   393,    16,    17,
      20,     0,     0,   170,     0,    85,     0,     0,     0,    93,
      88,   163,   141,     0,   400,   408,   378,     0,    21,     0,
      65,    66,    63,    64,    62,    61,    67,     0,    38,     0,
       0,     0,     0,   412,   432,   413,   444,   414,   418,   419,
     420,   421,   436,   425,   426,   427,   428,   429,   430,   431,
     433,   434,   481,   417,   424,   435,   488,   495,   415,   422,
     416,   423,     0,     0,   443,   451,   454,   452,   453,     0,
     446,     0,     0,   358,     0,    83,    96,    97,    99,    98,
     100,   101,   102,   103,   128,   129,   126,   127,   119,   130,
     131,   120,   117,   118,   132,   133,   134,   135,   107,   108,
     109,   104,   105,   106,   122,   123,   121,   115,   116,   111,
     110,   112,   113,   114,    95,    94,   124,   125,   338,    91,
       0,     0,   388,     0,   402,     0,     0,   410,   387,   371,
      22,    23,     0,    76,   472,   475,   478,   468,     0,   448,
     482,   489,   496,   502,   505,   459,   464,   458,   471,     0,
     467,   461,     0,     0,   391,   463,     0,     0,     0,     0,
      89,     0,   351,     0,     0,   140,   165,     0,    54,    55,
       0,   274,   275,     0,     0,     0,     0,   268,   177,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,   227,   226,   228,   229,   230,    24,     0,
       0,     0,     0,     0,     0,     0,   212,   213,   144,   142,
     272,   271,     0,     0,     0,   158,   153,   151,   150,   152,
     223,   164,   145,   269,     0,   331,     0,     0,   156,   157,
     159,     0,   149,   330,   329,   328,    86,   334,   270,     0,
     146,   333,   332,   310,   276,   231,     0,   273,   512,   513,
     514,   515,   437,   440,   438,   441,   439,   442,     0,     0,
     401,   343,   385,   379,     0,     0,   372,     0,    68,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    88,     0,
      88,     0,     0,   466,   460,   462,     0,   465,   456,     0,
     447,   511,   357,   509,   359,     0,     0,   349,   362,    90,
     338,    92,     0,     0,   173,   176,     0,   201,     0,   179,
     182,     0,   231,     0,   172,     0,     0,   192,   195,     0,
     162,   198,     0,     0,    48,    58,     0,     0,   186,   231,
     302,   303,   213,   167,   168,     0,   522,     0,     0,     0,
     539,     0,     0,   280,   279,   315,    30,   278,   277,     0,
     214,     0,   160,   161,     0,   337,     0,   148,   154,   155,
     191,   221,     0,   147,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,     0,   388,     0,   398,
     338,   411,     0,   369,   370,   368,     0,     0,     0,     0,
       0,   445,     0,     0,    91,     0,    91,     0,    91,   169,
     355,     0,   353,     0,   457,   455,     0,   350,   361,   360,
       0,   363,     0,   352,    59,     0,     0,     0,     0,   183,
     231,     0,     0,   180,     0,     0,    60,     0,     0,     0,
       0,    46,    47,     0,   187,   231,   184,   214,     0,     0,
     169,     0,     0,   530,   520,   522,     0,   533,   534,   535,
       0,     0,     0,   522,     0,     0,     0,     0,    27,     0,
      25,     0,   306,   258,   257,   163,    51,     0,   311,     0,
     207,     0,    88,   166,     0,   325,   326,   241,   242,   244,
     243,   245,   235,   236,   237,   281,   282,   294,   246,   247,
     295,   292,   293,     0,     0,     0,   233,   320,   309,     0,
     335,   336,   234,   283,   284,   248,   249,   299,   300,   301,
     238,   239,   240,     0,     0,   232,     0,     0,   297,   298,
     296,   290,   291,   286,   285,   287,   288,   289,   264,     0,
       0,   313,     0,     0,     0,   389,     0,   409,     0,   340,
       0,   368,   373,     0,   377,   231,   473,     0,   479,   469,
     449,     0,   483,     0,   490,     0,   497,     0,     0,   503,
       0,   506,     0,   339,   364,   367,   366,     0,   174,     0,
       0,     0,     0,   181,   188,   316,     0,     0,   317,     0,
     221,   185,     0,   523,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   532,   540,
     541,     0,    28,    31,     0,    88,     0,     0,    49,    50,
       0,    56,     0,     0,     0,   312,     0,     0,     0,     0,
     219,     0,    91,   322,     0,   252,   253,   308,   250,   251,
     327,     0,   266,   307,   314,   399,     0,     0,   340,   341,
       0,   344,   375,   231,   376,   474,     0,   480,   470,   450,
     484,   486,   491,   493,   498,   500,   354,   504,   356,   507,
     360,     0,     0,   175,     0,   204,   202,   193,   196,   199,
       0,     0,   517,   516,   522,     0,     0,   521,     0,   525,
     531,   537,   536,     0,   522,     0,   538,    26,    29,     0,
      91,   143,    52,     0,     0,   208,   217,   218,   216,   215,
       0,     0,     0,     0,   255,     0,   321,   267,   390,    87,
     345,   137,   342,   374,   476,   487,   494,   501,   365,    57,
       0,     0,   203,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   524,     0,     0,   528,   259,     0,    51,   254,
     220,   222,     0,   209,   224,     0,   256,   348,   137,   347,
     477,     0,     0,   194,   197,   200,   318,     0,   542,     0,
     526,   519,   518,   542,     0,     0,   262,    53,     0,   210,
     323,     0,   205,     0,   319,     0,     0,   527,     0,   529,
     260,     0,   211,   324,   346,     0,     0,     0,     0,   263,
     206,   543,   544,   545
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -598,  -598,  -598,  -598,  -598,   107,   513,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,   470,  -598,  -598,   525,
    -598,  -598,  -598,  -310,  -598,  -598,  -598,  -598,  -598,  -598,
     375,  -598,   563,   -67,  -598,   548,   -65,  -335,  -383,  -529,
    -598,  -598,  -598,  -283,  -245,   -79,  -598,     2,  -282,   -12,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,   627,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -525,  -598,  -598,  -231,  -598,  -108,  -288,  -598,  -228,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
     487,  -598,  -598,    93,  -164,  -598,  -598,  -598,   215,  -598,
     -83,  -598,  -265,   234,  -408,  -410,    96,  -598,  -196,  -598,
     600,  -598,  -598,  -598,   103,   575,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -266,   -96,  -598,  -598,
    -598,   260,  -598,  -598,  -598,   -77,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,   374,  -595,  -427,  -597,  -598,  -598,
    -299,   -97,   197,  -598,  -598,  -598,  -264,  -598
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   110,    39,    14,   129,   135,   462,   330,   763,
     331,   621,    15,    16,    34,    35,    65,    17,    31,    32,
     332,   333,   770,   771,   334,   335,   336,   337,   338,   339,
     136,   137,    28,    29,    46,    47,    48,    18,   229,   275,
     120,    19,    79,    80,    81,   340,   341,   230,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     587,   883,   588,   884,   590,   885,   354,   578,   882,   355,
     629,   904,   356,   780,   781,   632,   357,   358,   431,   360,
     361,   362,   363,   764,   925,   765,   941,   364,   689,   867,
     630,   863,   943,   271,   800,   700,   540,   908,   272,   273,
     560,   561,   183,   184,   745,   417,   546,   387,   238,   236,
      56,    90,    21,   127,   379,    67,    68,    22,    84,    23,
     234,   125,    54,    87,    24,   235,   239,   366,   378,   176,
     177,   181,   178,   394,   809,   562,   392,   808,   389,   805,
     390,   910,   391,   807,   395,   810,   396,   875,   397,   812,
     398,   876,   399,   814,   400,   877,   401,   817,   402,   819,
      25,    41,    26,    40,   367,   604,   605,   606,   368,   607,
     608,   609,   610,   369,   461,   370,   936,   371
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,   122,   359,   414,   430,   418,   567,   114,   749,   175,
     572,   692,   460,   554,   423,   556,   756,   558,   575,   453,
     454,   481,   448,   -86,   742,   713,   599,   715,   179,   717,
     754,   613,   773,     2,   116,    91,    92,   416,   599,     3,
     383,    61,    42,   634,   269,   504,   482,   902,   507,   508,
     727,   729,   507,   508,    58,    66,   710,   568,   767,   475,
      44,    27,     4,    49,     5,   616,     6,   617,    43,   768,
     769,   543,     7,    62,   237,    33,    45,   776,   711,     8,
     544,   185,   404,   484,   485,     9,   777,   270,   568,   410,
     255,   182,   476,   174,   106,   384,   569,   744,   570,   130,
     131,   571,   121,    73,    10,   117,   118,   534,   535,   483,
     545,   534,   535,   382,   256,   257,   778,   568,   774,   231,
     372,   779,   568,   538,   373,    11,   744,   569,   411,   419,
     413,   890,   420,    77,   772,   538,   237,    37,    44,    78,
      30,   374,   375,   376,   377,   119,   579,   232,   822,   835,
     418,   384,   269,   837,    45,   480,   569,   384,   774,   748,
      93,   569,   539,   384,    94,   594,    95,    96,   496,   497,
     635,   175,    38,   258,   795,   694,   504,    44,   175,   507,
     508,   509,   484,   485,    63,   510,    55,   175,   887,   598,
     412,   611,   696,    45,   633,    64,   242,   421,   774,    97,
     697,   753,   415,   757,   695,   698,   424,   623,   405,    12,
     433,   259,   260,   175,   175,    13,   261,   831,   774,   262,
      75,   380,   445,   243,   132,   775,   406,   774,   133,   843,
     134,    96,   456,   458,   263,   407,   774,   888,   534,   535,
     264,    50,    51,   265,    52,    76,   774,   894,   893,   782,
      73,   240,   241,   862,   794,   174,   484,   485,    58,   865,
     747,    36,   174,   823,   704,   494,   495,   496,   497,  -485,
      57,   174,    53,   906,  -485,   504,   718,   506,   507,   508,
     509,   860,    75,   566,   510,   854,   512,   513,    58,   175,
     725,    60,  -485,   175,   175,   175,   175,   174,   174,   175,
      59,   175,    66,   175,    70,   175,   861,   869,   542,   719,
     466,   743,   547,   548,   549,   550,   821,   759,   553,   834,
     555,   897,   557,   529,   530,   531,   532,   533,   844,    73,
      82,   720,    71,   721,   484,   485,    83,   534,   535,  -492,
    -499,   496,   497,   574,  -492,  -499,   880,   799,   870,   504,
     175,   506,   507,   508,   509,   722,   752,   586,   510,    72,
      74,   753,  -492,  -499,   100,  -261,   484,   485,   881,   593,
    -261,   326,   452,   174,   121,    85,   102,   174,   174,   174,
     174,    86,   850,   174,   103,   174,    88,   174,  -261,   174,
     872,   104,    89,   105,   111,   626,   112,   529,   530,   531,
     532,   533,   384,   108,   113,   385,   121,   123,   386,   109,
     124,   534,   535,   126,   577,   255,   128,   618,   619,   496,
     497,   802,   128,    62,   180,   182,   226,   504,   891,   506,
     507,   508,   509,   227,   174,   228,   510,   233,   237,   256,
     257,   248,   244,   245,   246,   247,   249,   250,   251,   252,
     253,   496,   497,   254,   266,   267,   268,   414,   624,   504,
     393,   506,   507,   508,   509,   274,   403,   427,   510,   442,
     437,   438,   441,   426,   175,   381,   435,   531,   532,   533,
     439,   440,   175,   446,   470,   471,   472,   536,   473,   534,
     535,   175,   175,   724,   175,    12,   541,   543,   258,   477,
     478,   732,   479,   564,   537,   734,   544,   551,   484,   485,
     736,   737,   269,   739,   482,   584,   591,   592,   597,   625,
     631,   534,   535,   747,   636,   653,   658,   688,   760,   723,
     750,   783,   240,   762,   418,   772,   701,   260,   359,   751,
     784,   261,   790,   792,   262,   798,   799,   801,   410,   811,
     824,   813,   836,   815,   838,   848,   856,   845,   174,   263,
     857,   858,   859,   912,   946,   702,   174,   899,   265,   620,
      98,   839,   138,   841,   926,   174,   174,   846,   174,   886,
     907,   889,   933,   892,   944,   101,   895,   901,   927,   919,
     920,   494,   495,   496,   497,   498,   937,   939,   501,   502,
     503,   504,   947,   506,   507,   508,   509,   924,   948,   952,
     510,   175,   512,   513,   484,   485,   953,   388,   516,   517,
     518,    99,   175,   115,   175,   931,   909,   766,    20,   900,
     806,   797,   830,   699,   871,   573,   563,   818,   703,    69,
     693,   816,   523,   107,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   552,   614,   842,   425,     0,   938,
       0,     0,     0,   534,   535,     0,     0,     0,   175,     0,
       0,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,     0,     0,   849,   852,     0,
       0,     0,     0,     0,     0,   174,     0,   494,   495,   496,
     497,   498,     0,     0,   501,     0,   174,   504,   174,   506,
     507,   508,   509,     0,     0,     0,   510,   365,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   879,     0,     0,     0,     0,     0,     0,
     409,     0,   174,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,   422,     0,     0,   175,     0,   534,
     535,     0,     0,   432,   898,   255,     0,   434,     0,   436,
       0,     0,     0,   903,     0,     0,   905,   444,     0,     0,
       0,   449,   450,   451,     0,     0,     0,     0,   459,   256,
     257,     0,     0,     0,     0,     0,   463,   464,   465,     0,
     467,   468,   469,     0,     0,   494,   495,   496,   497,   498,
       0,   474,   501,   502,   503,   504,     0,   506,   507,   508,
     509,     0,     0,     0,   510,     0,   512,   513,     0,     0,
       0,     0,   516,   517,   518,     0,     0,     0,     0,     0,
       0,   174,     0,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,     0,     0,     0,   868,     0,   259,   260,     0,     0,
       0,   261,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,   940,     0,   580,     0,     0,     0,   263,
       0,   576,   585,     0,     0,     0,   589,     0,   265,   949,
       0,     0,     0,     0,   595,     0,     0,     0,     0,     0,
       0,     0,     0,   603,     0,   603,     0,     0,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,     0,     0,   647,   648,   649,   650,   651,
     652,     0,   656,   657,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   675,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,     0,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   705,     0,     0,   494,   495,   496,   497,
     498,     0,     0,   501,   502,   503,   504,     0,   506,   507,
     508,   509,     0,     0,     0,   510,     0,   512,   513,   726,
       0,     0,     0,     0,   731,     0,     0,     0,   656,   675,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   469,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   469,
       0,   255,   758,   459,     0,     0,   761,     0,   534,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,   257,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,   786,     0,     0,   281,   282,   283,   284,   285,
       0,     0,     0,     0,     0,     0,     0,   287,     0,     0,
     788,   789,     0,   255,     0,     0,     0,     0,     0,   291,
       0,     0,   293,     0,     0,   294,     0,     0,   295,     0,
     599,   796,   298,     0,   258,     0,     0,   256,   257,     0,
     803,     0,     0,   143,   144,   145,     0,   147,   148,   149,
     150,   151,   302,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,   163,   164,   165,     0,     0,   168,   169,
     170,   171,   259,   260,   303,     0,     0,   261,     0,   706,
     262,   832,   833,     0,     0,     0,   305,   306,   840,     0,
     603,     0,     0,     0,     0,   263,   258,     0,     0,     0,
       0,     0,     0,   365,   265,     0,     0,   853,     0,     0,
      44,   855,     0,     0,     0,     0,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   600,   318,   866,     0,
       0,     0,   139,     0,   259,   260,     0,     0,   140,   261,
       0,     0,   601,   320,   321,     0,   322,     0,     0,   323,
     324,     0,     0,   141,     0,     0,     0,   263,   878,   602,
       0,   326,   327,     0,   121,     0,   265,     0,   142,     0,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,     0,     0,     0,     0,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,   911,     0,     0,
     913,   914,   915,   917,   918,     0,     0,   921,   922,     0,
     923,     0,   256,   257,     0,   276,     0,    44,     0,   929,
       3,     0,   277,   278,   279,     0,   280,     0,   281,   282,
     283,   284,   285,    45,     0,     0,     0,     0,     0,   286,
     287,   288,   289,     0,     0,   942,     0,     0,     0,     0,
     945,   290,   291,     0,   292,   293,     0,     0,   294,     0,
       8,   295,   296,   951,   297,   298,     0,     0,   299,   300,
       0,   258,     0,     0,     0,   301,   143,   144,   145,     0,
     147,   148,   149,   150,   151,   302,   153,   154,   155,   156,
     157,   158,   159,   160,   161,     0,   163,   164,   165,     0,
       0,   168,   169,   170,   171,     0,     0,   303,   304,   259,
     260,     0,     0,     0,   261,     0,     0,   262,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   263,     0,   307,   308,     0,     0,   707,     0,
       0,   265,     0,    44,     0,     0,     0,     0,     0,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,    45,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,   320,   321,     0,   322,
       0,     0,   323,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   325,     0,   326,   327,   328,   121,   329,   276,
       0,     0,     0,     0,     3,     0,   277,   278,   279,     0,
     280,     0,   281,   282,   283,   284,   285,     0,     0,     0,
       0,     0,     0,   286,   287,   288,   289,     0,     0,     0,
       0,     0,     0,     0,     0,   290,   291,     0,   292,   293,
       0,     0,   294,     0,     8,   295,   296,     0,   297,   298,
       0,     0,   299,   300,     0,     0,     0,     0,     0,   301,
     143,   144,   145,     0,   147,   148,   149,   150,   151,   302,
     153,   154,   155,   156,   157,   158,   159,   160,   161,     0,
     163,   164,   165,     0,     0,   168,   169,   170,   171,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,   308,
       0,     0,     0,     0,     0,     0,     0,    44,     0,     0,
       0,     0,   255,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,    45,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,   257,     0,   319,
     320,   321,     0,   322,     0,     0,   323,   324,     0,     0,
       0,   281,   282,   283,   284,   285,   325,     0,   326,   327,
     328,   121,   851,   287,     0,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,   291,     0,     0,   293,     0,
       0,   294,     0,     0,   295,     0,     0,     0,   298,     0,
       0,     0,     0,   256,   257,   258,     0,     0,     0,   143,
     144,   145,     0,   147,   148,   149,   150,   151,   302,   153,
     154,   155,   156,   157,   158,   159,   160,   161,     0,   163,
     164,   165,     0,     0,   168,   169,   170,   171,     0,     0,
     303,     0,     0,   259,   260,     0,     0,     0,   261,     0,
     708,   262,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,   255,     0,   265,    44,     0,     0,     0,
       0,     0,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   600,   318,     0,     0,     0,   256,   257,     0,
     259,   260,     0,     0,     0,   261,     0,     0,   601,   320,
     321,     0,   322,     0,     0,   323,   324,   281,   282,   283,
     284,   285,     0,   263,     0,   612,     0,   326,   327,   287,
     121,     0,   265,     0,     0,   404,     0,     0,     0,     0,
       0,   291,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   147,
     148,   149,   150,   151,   302,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     0,   163,   164,   165,     0,     0,
     168,   169,   170,   171,   259,   260,   303,     0,     0,   261,
       0,   709,   262,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,    44,   255,     0,     0,     0,     0,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,    45,   318,
       0,     0,     0,     0,     0,     0,     0,   256,   257,     0,
       0,   405,     0,     0,   319,   320,   321,     0,   322,     0,
       0,   323,   324,   281,   282,   283,   284,   285,     0,   406,
       0,   325,     0,   326,   327,   287,   121,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   291,     0,     0,
     293,     0,     0,   294,     0,     0,   295,     0,     0,     0,
     298,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,   143,   144,   145,     0,   147,   148,   149,   150,   151,
     302,   153,   154,   155,   156,   157,   158,   159,   160,   161,
       0,   163,   164,   165,     0,     0,   168,   169,   170,   171,
       0,     0,   303,     0,   259,   260,     0,     0,     0,   261,
       0,   712,   262,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   428,     0,     0,     0,   263,     0,   307,
     308,     0,     0,     0,     0,     0,   265,     0,    44,     0,
     255,     0,     0,     0,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,    45,   318,     0,     0,     0,     0,
       0,     0,     0,     0,   256,   257,     0,     0,     0,     0,
     319,   320,   321,     0,   322,     0,     0,   323,   324,     0,
       0,   281,   282,   283,   284,   285,     0,   325,     0,   326,
     327,   429,   121,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,     0,     0,   293,     0,
       0,   294,     0,     0,   295,     0,     0,     0,   298,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   147,   148,   149,   150,   151,   302,   153,
     154,   155,   156,   157,   158,   159,   160,   161,     0,   163,
     164,   165,     0,     0,   168,   169,   170,   171,     0,     0,
     303,   259,   260,     0,     0,     0,   261,     0,   714,   262,
       0,     0,   305,   306,     0,   255,     0,     0,     0,     0,
       0,   447,     0,     0,   263,     0,     0,   307,   308,     0,
       0,     0,     0,   265,     0,     0,    44,     0,     0,   256,
     257,     0,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,    45,   318,     0,     0,     0,     0,     0,   281,
     282,   283,   284,   285,     0,     0,     0,     0,   319,   320,
     321,   287,   322,     0,     0,   323,   324,     0,     0,     0,
       0,     0,     0,   291,     0,   325,   293,   326,   327,   294,
     121,     0,   295,     0,     0,     0,   298,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   147,   148,   149,   150,   151,   302,   153,   154,   155,
     156,   157,   158,   159,   160,   161,     0,   163,   164,   165,
       0,     0,   168,   169,   170,   171,   259,   260,   303,     0,
       0,   261,     0,   716,   262,     0,     0,     0,     0,     0,
     305,   306,     0,   255,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,   307,   308,     0,   265,     0,
       0,     0,     0,     0,    44,     0,     0,   256,   257,     0,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
      45,   318,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   285,     0,     0,     0,     0,   319,   320,   321,   287,
     322,     0,     0,   323,   324,     0,     0,     0,     0,     0,
       0,   291,     0,   325,   293,   326,   327,   294,   121,     0,
     295,     0,     0,     0,   298,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   147,
     148,   149,   150,   151,   302,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     0,   163,   164,   165,     0,     0,
     168,   169,   170,   171,   259,   260,   303,     0,     0,   261,
       0,   740,   262,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,   255,
       0,     0,    44,     0,     0,     0,     0,     0,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,    45,   318,
       0,     0,     0,   256,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,   320,   321,     0,   322,     0,
       0,   323,   324,   281,   282,   283,   284,   285,     0,   627,
       0,   325,   628,   326,   327,   287,   121,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,     0,     0,
     293,     0,     0,   294,     0,     0,   295,     0,     0,     0,
     298,   568,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   144,   145,     0,   147,   148,   149,   150,   151,
     302,   153,   154,   155,   156,   157,   158,   159,   160,   161,
       0,   163,   164,   165,     0,     0,   168,   169,   170,   171,
     820,   260,   303,     0,     0,   261,     0,     0,   262,     0,
       0,     0,     0,     0,   305,   306,     0,   255,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,   654,
     655,     0,   265,     0,     0,     0,     0,     0,    44,     0,
       0,   256,   257,     0,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,    45,   318,     0,     0,     0,     0,
       0,   281,   282,   283,   284,   285,     0,     0,     0,     0,
     319,   320,   321,   287,   322,     0,     0,   323,   324,     0,
       0,     0,     0,     0,     0,   291,     0,   325,   293,   326,
     327,   294,   121,     0,   295,     0,     0,     0,   298,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   147,   148,   149,   150,   151,   302,   153,
     154,   155,   156,   157,   158,   159,   160,   161,     0,   163,
     164,   165,     0,     0,   168,   169,   170,   171,   259,   260,
     303,     0,     0,   261,     0,   826,   262,     0,     0,     0,
       0,     0,   305,   306,     0,   255,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   673,   674,     0,
     265,     0,     0,     0,     0,     0,    44,     0,     0,   256,
     257,     0,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,    45,   318,     0,     0,     0,     0,     0,   281,
     282,   283,   284,   285,     0,     0,     0,     0,   319,   320,
     321,   287,   322,     0,     0,   323,   324,     0,     0,     0,
       0,     0,     0,   291,     0,   325,   293,   326,   327,   294,
     121,     0,   295,     0,     0,     0,   298,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   147,   148,   149,   150,   151,   302,   153,   154,   155,
     156,   157,   158,   159,   160,   161,     0,   163,   164,   165,
       0,     0,   168,   169,   170,   171,   259,   260,   303,     0,
       0,   261,     0,   827,   262,     0,     0,     0,     0,     0,
     305,   306,     0,   255,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,    44,     0,     0,   256,   257,     0,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
      45,   318,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   285,     0,     0,     0,     0,   319,   320,   321,   287,
     322,     0,     0,   323,   324,     0,     0,     0,     0,     0,
       0,   291,   408,   325,   293,   326,   327,   294,   121,     0,
     295,     0,     0,     0,   298,     0,   258,   443,     0,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   147,
     148,   149,   150,   151,   302,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     0,   163,   164,   165,     0,     0,
     168,   169,   170,   171,   259,   260,   303,     0,     0,   261,
       0,   828,   262,     0,     0,     0,     0,     0,   305,   306,
       0,   255,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,    44,     0,     0,   256,   257,     0,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,    45,   318,
       0,     0,     0,     0,     0,   281,   282,   283,   284,   285,
       0,     0,   676,     0,   319,   320,   321,   287,   322,     0,
       0,   323,   324,     0,     0,     0,     0,     0,     0,   291,
       0,   325,   293,   326,   327,   294,   121,     0,   295,     0,
       0,     0,   298,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   147,   148,   149,
     150,   151,   302,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,   163,   164,   165,     0,     0,   168,   169,
     170,   171,   259,   260,   303,     0,     0,   261,     0,   829,
     262,     0,     0,     0,     0,     0,   305,   306,     0,   255,
       0,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,     0,     0,     0,     0,
      44,     0,     0,   256,   257,     0,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,    45,   318,     0,     0,
       0,     0,     0,   281,   282,   283,   284,   285,     0,     0,
       0,     0,   319,   320,   321,   287,   322,     0,     0,   323,
     324,     0,     0,     0,     0,     0,     0,   291,     0,   325,
     293,   326,   327,   294,   121,     0,   295,     0,     0,     0,
     298,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   144,   145,     0,   147,   148,   149,   150,   151,
     302,   153,   154,   155,   156,   157,   158,   159,   160,   161,
       0,   163,   164,   165,     0,     0,   168,   169,   170,   171,
     259,   260,   303,     0,     0,   261,     0,   874,   262,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,   255,     0,     0,    44,     0,
       0,     0,     0,     0,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,    45,   318,     0,     0,     0,   256,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,   320,   321,     0,   322,     0,     0,   323,   324,   281,
     282,   283,   284,   285,     0,     0,     0,   325,   691,   326,
     327,   287,   121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   291,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   147,   148,   149,   150,   151,   302,   153,   154,   155,
     156,   157,   158,   159,   160,   161,     0,   163,   164,   165,
       0,     0,   168,   169,   170,   171,   259,   260,   303,     0,
       0,   261,     0,   896,   262,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
       0,   255,     0,     0,    44,     0,     0,     0,     0,     0,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
      45,   318,     0,     0,     0,   256,   257,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,   320,   321,     0,
     322,     0,     0,   323,   324,   281,   282,   283,   284,   285,
       0,     0,     0,   325,   728,   326,   327,   287,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
       0,     0,   293,     0,     0,   294,     0,     0,   295,     0,
       0,     0,   298,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   147,   148,   149,
     150,   151,   302,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,   163,   164,   165,     0,     0,   168,   169,
     170,   171,   259,   260,   303,     0,     0,   261,     0,   930,
     262,     0,     0,     0,     0,     0,   305,   306,     0,   255,
       0,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,     0,     0,     0,     0,
      44,     0,     0,   256,   257,     0,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,    45,   318,     0,     0,
       0,     0,     0,   281,   282,   283,   284,   285,     0,     0,
     730,     0,   319,   320,   321,   287,   322,     0,     0,   323,
     324,     0,     0,     0,     0,     0,     0,   291,     0,   325,
     293,   326,   327,   294,   121,     0,   295,     0,     0,     0,
     298,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   144,   145,     0,   147,   148,   149,   150,   151,
     302,   153,   154,   155,   156,   157,   158,   159,   160,   161,
       0,   163,   164,   165,     0,     0,   168,   169,   170,   171,
     259,   260,   303,     0,     0,   261,     0,     0,   262,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,    44,     0,
       0,     0,     0,     0,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,    45,   318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,   320,   321,     0,   322,     0,     0,   323,   324,   281,
     282,   283,   284,   285,     0,     0,     0,   325,   746,   326,
     327,   287,   121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   291,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   147,   148,   149,   150,   151,   302,   153,   154,   155,
     156,   157,   158,   159,   160,   161,     0,   163,   164,   165,
       0,     0,   168,   169,   170,   171,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,     0,     0,     0,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
      45,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,   320,   321,     0,
     322,     0,     0,   323,   324,   281,   282,   283,   284,   285,
       0,     0,     0,   325,   755,   326,   327,   287,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
       0,     0,   293,     0,     0,   294,     0,     0,   295,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   147,   148,   149,
     150,   151,   302,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,   163,   164,   165,     0,     0,   168,   169,
     170,   171,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,    45,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,   320,   321,     0,   322,     0,     0,   323,
     324,   281,   282,   283,   284,   285,     0,     0,     0,   325,
     864,   326,   327,   287,   121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,     0,     0,   293,     0,
       0,   294,     0,     0,   295,     0,     0,     0,   298,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   147,   148,   149,   150,   151,   302,   153,
     154,   155,   156,   157,   158,   159,   160,   161,     0,   163,
     164,   165,     0,     0,   168,   169,   170,   171,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,     0,     0,     0,
       0,     0,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,    45,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,   320,
     321,     0,   322,     0,     0,   323,   324,   281,   282,   283,
     284,   285,     0,     0,     0,   325,   916,   326,   327,   287,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   291,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   147,
     148,   149,   150,   151,   302,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     0,   163,   164,   165,     0,     0,
     168,   169,   170,   171,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   928,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,     0,     0,     0,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,    45,   318,
       0,     0,     0,     0,     0,   281,   282,   283,   284,   285,
       0,     0,     0,     0,   319,   320,   321,   287,   322,     0,
       0,   323,   324,     0,     0,     0,     0,     0,     0,   291,
       0,   325,   293,   326,   327,   294,   121,     0,   295,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   147,   148,   149,
     150,   151,   302,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,   163,   164,   165,     0,     0,   168,   169,
     170,   171,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,    45,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,   320,   321,     0,   322,     0,     0,   323,
     324,     0,     0,     0,     0,     0,     0,     0,     0,   325,
       0,   326,   327,     0,   121,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   581,   506,   507,   508,   509,   484,
     485,     0,   510,   511,   512,   513,   514,   515,     0,     0,
     516,   517,   518,   519,   520,   521,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   582,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,     0,
       0,     0,   583,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   581,   506,   507,   508,   509,   484,   485,
       0,   510,   511,   512,   513,   514,   515,     0,     0,   516,
     517,   518,   519,   520,   521,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   582,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,   596,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   581,   506,   507,   508,   509,   484,   485,     0,
     510,   511,   512,   513,   514,   515,     0,     0,   516,   517,
     518,   519,   520,   521,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   582,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,     0,     0,     0,
     733,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   581,   506,   507,   508,   509,   484,   485,     0,   510,
     511,   512,   513,   514,   515,     0,     0,   516,   517,   518,
     519,   520,   521,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     582,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,   741,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     581,   506,   507,   508,   509,   484,   485,     0,   510,   511,
     512,   513,   514,   515,     0,     0,   516,   517,   518,   519,
     520,   521,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   582,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,     0,     0,     0,   804,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   581,
     506,   507,   508,   509,   484,   485,     0,   510,   511,   512,
     513,   514,   515,     0,     0,   516,   517,   518,   519,   520,
     521,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   582,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,     0,     0,     0,   873,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,     0,     0,   501,   502,   503,   504,     0,   506,
     507,   508,   509,     0,     0,     0,   510,     0,   512,   513,
       0,     0,     0,     0,   516,   517,   518,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,   484,   485,     0,     0,   534,
     535,   494,   495,   496,   497,   498,   935,     0,   501,   502,
     503,   504,     0,   506,   507,   508,   509,     0,     0,     0,
     510,     0,   512,   513,     0,     0,     0,     0,   516,   517,
     518,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,   484,   485,   622,   494,   495,
     496,   497,   498,     0,     0,   501,   502,   503,   504,     0,
     506,   507,   508,   509,     0,     0,     0,   510,     0,   512,
     513,     0,     0,     0,     0,   516,   517,   518,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   484,   485,   735,     0,     0,     0,   494,   495,
     496,   497,   498,     0,     0,   501,   502,   503,   504,     0,
     506,   507,   508,   509,     0,     0,     0,   510,     0,   512,
     513,     0,     0,     0,     0,   516,   517,   518,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   484,   485,   738,   494,   495,   496,   497,   498,
       0,     0,   501,   502,   503,   504,     0,   506,   507,   508,
     509,     0,     0,     0,   510,     0,   512,   513,     0,     0,
       0,     0,   516,   517,   518,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,   484,
     485,   825,     0,     0,     0,   494,   495,   496,   497,   498,
       0,     0,   501,   502,   503,   504,     0,   506,   507,   508,
     509,     0,     0,     0,   510,     0,   512,   513,     0,     0,
       0,     0,   516,   517,   518,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,   484,
     485,   932,   494,   495,   496,   497,   498,     0,     0,   501,
     502,   503,   504,     0,   506,   507,   508,   509,     0,     0,
       0,   510,     0,   512,   513,     0,     0,     0,     0,   516,
     517,   518,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   484,   485,   934,     0,
       0,     0,   494,   495,   496,   497,   498,     0,     0,   501,
     502,   503,   504,     0,   506,   507,   508,   509,     0,     0,
       0,   510,     0,   512,   513,     0,     0,     0,     0,   516,
     517,   518,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,   484,   485,     0,   534,   535,     0,     0,   950,   494,
     495,   496,   497,   498,     0,     0,   501,   502,   503,   504,
       0,   506,   507,   508,   509,     0,     0,     0,   510,     0,
     512,   513,     0,     0,     0,     0,   516,   517,   518,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,   484,   485,     0,     0,
       0,   534,   535,   565,   494,   495,   496,   497,   498,     0,
       0,   501,   502,   503,   504,     0,   506,   507,   508,   509,
       0,     0,     0,   510,     0,   512,   513,     0,     0,     0,
       0,   516,   517,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   787,   494,
     495,   496,   497,   498,   484,   485,   501,   502,   503,   504,
       0,   506,   507,   508,   509,     0,     0,     0,   510,     0,
     512,   513,     0,     0,     0,     0,   516,   517,   518,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,   793,     0,     0,     0,     0,     0,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   581,   506,
     507,   508,   509,  -231,   484,   485,   510,   511,   512,   513,
     514,   515,     0,     0,   516,   517,   518,   519,   520,   521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   582,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   484,   485,     0,     0,     0,     0,     0,     0,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,     0,     0,   516,   517,   518,   519,   520,   521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   522,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     484,   485,     0,     0,   494,   495,   496,   497,   498,   534,
     535,   501,   502,   503,   504,     0,   506,   507,   508,   509,
       0,     0,     0,   510,     0,   512,   513,     0,     0,   615,
       0,   516,   517,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   484,   485,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,     0,     0,
     501,   502,   503,   504,     0,   506,   507,   508,   509,     0,
       0,     0,   510,     0,   512,   513,     0,     0,     0,     0,
     516,   517,   518,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   791,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   484,   485,     0,     0,
     494,   495,   496,   497,   498,   534,   535,   501,   502,   503,
     504,     0,   506,   507,   508,   509,     0,     0,     0,   510,
       0,   512,   513,     0,     0,     0,     0,   516,   517,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   847,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,   484,   485,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,     0,     0,   501,   502,   503,   504,
       0,   506,   507,   508,   509,     0,     0,     0,   510,     0,
     512,   513,     0,     0,     0,     0,   516,   517,   518,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   484,   485,     0,     0,   494,   495,   496,   497,
     498,   534,   535,   501,   502,   503,   504,     0,   506,   507,
     508,   509,     0,     0,     0,   510,     0,   512,   513,     0,
     484,   485,     0,   516,     0,   518,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
       0,     0,   501,   502,   503,   504,     0,   506,   507,   508,
     509,     0,     0,     0,   510,     0,   512,   513,   484,   485,
       0,     0,   516,   494,   495,   496,   497,   498,     0,     0,
     501,   502,   503,   504,     0,   506,   507,   508,   509,     0,
       0,     0,   510,     0,   512,   513,     0,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,     0,     0,     0,     0,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,   484,   485,
       0,     0,     0,     0,     0,   534,   535,     0,     0,     0,
       0,   494,   495,   496,   497,   498,     0,     0,   501,   502,
     503,   504,     0,   506,   507,   508,   509,     0,     0,     0,
     510,     0,   512,   513,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,   531,   532,   533,   484,   485,     0,     0,     0,     0,
       0,   494,   495,   534,   535,   498,     0,     0,   501,   502,
     503,   504,     0,   506,   507,   508,   509,     0,     0,     0,
     510,     0,   512,   513,     0,     0,     0,     0,   516,   517,
     518,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,     0,
       0,     0,   532,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,   494,   495,   496,
     497,   498,     0,     0,   501,   502,   503,   504,     0,   506,
     507,   508,   509,     0,     0,     0,   510,     0,   512,   513,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,   140,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
     141,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,     0,     0,   142,     0,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,     0,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   139,     0,     0,     0,     0,
       0,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      45,   142,     0,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   139,     0,     0,     0,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,   142,     0,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,     0,   141,     0,
       0,     0,     0,   309,   310,   311,     0,     0,     0,     0,
       0,     0,    45,   142,     0,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,     0,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   204,   205,
     206,   207,    44,     0,   208,   209,   210,   211,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
       0,     0,   224,   225
};

static const yytype_int16 yycheck[] =
{
      12,    80,   230,   268,   286,   271,   416,    74,   605,   105,
     418,   536,   311,   396,   280,   398,   613,   400,    19,   307,
     308,   356,   304,     7,    19,   554,    49,   556,   105,   558,
      19,   458,   627,     0,    34,    14,    15,    46,    49,     6,
     236,    21,    33,    18,   136,   113,   119,   128,   116,   117,
     575,   576,   116,   117,   152,   145,   160,   114,    11,   145,
     129,   145,    29,    75,    31,   182,    33,   184,    59,    22,
      23,   114,    39,    53,   145,   145,   145,   114,   182,    46,
     123,   179,    32,    20,    21,    52,   123,   179,   114,   145,
      32,   145,   178,   105,   184,   152,   153,   123,   155,    14,
      15,   158,   183,   129,    71,   105,   106,   175,   176,   182,
     153,   175,   176,   184,    56,    57,   153,   114,   152,   155,
      66,   158,   114,   145,    70,    92,   123,   153,   184,   179,
     184,   123,   182,    53,   145,   145,   145,   145,   129,    59,
     139,    87,    88,    89,    90,   145,   428,   183,   182,   746,
     416,   152,   136,   748,   145,   351,   153,   152,   152,   182,
     139,   153,   184,   152,   143,   447,   145,   146,   105,   106,
     145,   267,   180,   115,   184,   153,   113,   129,   274,   116,
     117,   118,    20,    21,   164,   122,    58,   283,   182,   455,
     267,   457,   176,   145,   482,   175,   152,   274,   152,   178,
     184,   182,   269,   184,   182,   540,   283,   159,   158,   176,
     289,   153,   154,   309,   310,   182,   158,   742,   152,   161,
     152,   233,   301,   179,   139,   179,   176,   152,   143,   754,
     145,   146,   309,   310,   176,   185,   152,   834,   175,   176,
     182,     4,     5,   185,     7,   177,   152,   844,   182,   632,
     129,   147,   148,   782,   179,   267,    20,    21,   152,   784,
     152,   183,   274,   179,   546,   103,   104,   105,   106,   155,
     153,   283,    35,   179,   160,   113,   155,   115,   116,   117,
     118,   152,   152,   177,   122,   177,   124,   125,   152,   385,
     572,   152,   178,   389,   390,   391,   392,   309,   310,   395,
     155,   397,   145,   399,   145,   401,   177,   177,   385,   160,
     322,   599,   389,   390,   391,   392,   724,   616,   395,   746,
     397,   850,   399,   161,   162,   163,   164,   165,   755,   129,
      53,   182,   145,   160,    20,    21,    59,   175,   176,   155,
     155,   105,   106,   422,   160,   160,   160,    42,    43,   113,
     446,   115,   116,   117,   118,   182,   177,   436,   122,   145,
     178,   182,   178,   178,   139,   155,    20,    21,   182,   446,
     160,   180,   181,   385,   183,    53,   145,   389,   390,   391,
     392,    59,   765,   395,   145,   397,    53,   399,   178,   401,
     800,   145,    59,   153,   183,   474,   183,   161,   162,   163,
     164,   165,   152,    53,   145,   155,   183,   145,   158,    59,
     145,   175,   176,   183,   426,    32,   147,   148,   149,   105,
     106,   703,   147,    53,   145,   145,    34,   113,   836,   115,
     116,   117,   118,    34,   446,   178,   122,   155,   145,    56,
      57,   178,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   105,   106,   159,   182,   155,   182,   722,   470,   113,
     145,   115,   116,   117,   118,   155,    32,   159,   122,   139,
     159,   159,   159,   178,   570,   183,   178,   163,   164,   165,
     178,   182,   578,   159,   181,    36,   182,   178,   182,   175,
     176,   587,   588,   570,   590,   176,   145,   114,   115,   182,
     182,   578,   182,   177,   183,   584,   123,   179,    20,    21,
     587,   588,   136,   590,   119,    13,   155,   139,   181,   183,
     136,   175,   176,   152,   145,   145,   145,   145,   184,   177,
     177,   159,   147,   147,   800,   145,   153,   154,   766,   152,
     178,   158,   145,     1,   161,     7,    42,   182,   145,   160,
     145,   160,   145,   160,   177,   147,   145,   184,   570,   176,
     145,   145,   145,   145,    49,   182,   578,   179,   185,   462,
      57,   177,   102,   177,   160,   587,   588,   177,   590,   178,
     182,   177,   160,   177,   182,    60,   177,   177,   898,   177,
     177,   103,   104,   105,   106,   107,   177,   177,   110,   111,
     112,   113,   177,   115,   116,   117,   118,   184,   184,   177,
     122,   707,   124,   125,    20,    21,   177,   242,   130,   131,
     132,    58,   718,    75,   720,   908,   871,   625,     1,   860,
     707,   696,   740,   540,   798,   420,   402,   720,   542,    39,
     537,   718,   154,    68,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   394,   458,   753,   283,    -1,   923,
      -1,    -1,    -1,   175,   176,    -1,    -1,    -1,   764,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   696,    -1,    -1,   764,   767,    -1,
      -1,    -1,    -1,    -1,    -1,   707,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,    -1,   718,   113,   720,   115,
     116,   117,   118,    -1,    -1,    -1,   122,   230,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   822,    -1,    -1,    -1,    -1,    -1,    -1,
     263,    -1,   764,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,   277,    -1,    -1,   863,    -1,   175,
     176,    -1,    -1,   286,   853,    32,    -1,   290,    -1,   292,
      -1,    -1,    -1,   862,    -1,    -1,   863,   300,    -1,    -1,
      -1,   304,   305,   306,    -1,    -1,    -1,    -1,   311,    56,
      57,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,    -1,
     323,   324,   325,    -1,    -1,   103,   104,   105,   106,   107,
      -1,   334,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,   863,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      -1,    -1,    -1,    -1,   182,    -1,   153,   154,    -1,    -1,
      -1,   158,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   925,    -1,   428,    -1,    -1,    -1,   176,
      -1,   178,   435,    -1,    -1,    -1,   439,    -1,   185,   941,
      -1,    -1,    -1,    -1,   447,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   456,    -1,   458,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,   498,   499,   500,   501,   502,
     503,    -1,   505,   506,    -1,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,    -1,   535,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   546,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,   572,
      -1,    -1,    -1,    -1,   577,    -1,    -1,    -1,   581,   582,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   601,   602,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   612,
      -1,    32,   615,   616,    -1,    -1,   619,    -1,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   654,   655,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
     673,   674,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      49,   694,    51,    -1,   115,    -1,    -1,    56,    57,    -1,
     703,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   153,   154,    93,    -1,    -1,   158,    -1,   160,
     161,   744,   745,    -1,    -1,    -1,   105,   106,   751,    -1,
     753,    -1,    -1,    -1,    -1,   176,   115,    -1,    -1,    -1,
      -1,    -1,    -1,   766,   185,    -1,    -1,   770,    -1,    -1,
     129,   774,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   791,    -1,
      -1,    -1,    24,    -1,   153,   154,    -1,    -1,    30,   158,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    -1,    -1,    45,    -1,    -1,    -1,   176,   821,   178,
      -1,   180,   181,    -1,   183,    -1,   185,    -1,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   880,    -1,    -1,
     883,   884,   885,   886,   887,    -1,    -1,   890,   891,    -1,
     893,    -1,    56,    57,    -1,     1,    -1,   129,    -1,   902,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,   145,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,   928,    -1,    -1,    -1,    -1,
     933,    37,    38,    -1,    40,    41,    -1,    -1,    44,    -1,
      46,    47,    48,   946,    50,    51,    -1,    -1,    54,    55,
      -1,   115,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    94,   153,
     154,    -1,    -1,    -1,   158,    -1,    -1,   161,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,   120,   121,    -1,    -1,   182,    -1,
      -1,   185,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,
      -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,   180,   181,   182,   183,   184,     1,
      -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,
      -1,    -1,    44,    -1,    46,    47,    48,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    32,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,   161,
     162,   163,    -1,   165,    -1,    -1,   168,   169,    -1,    -1,
      -1,    14,    15,    16,    17,    18,   178,    -1,   180,   181,
     182,   183,   184,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    57,   115,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,   153,   154,    -1,    -1,    -1,   158,    -1,
     160,   161,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,   185,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,
     153,   154,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,    14,    15,    16,
      17,    18,    -1,   176,    -1,   178,    -1,   180,   181,    26,
     183,    -1,   185,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   153,   154,    93,    -1,    -1,   158,
      -1,   160,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      -1,    -1,   129,    32,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,   165,    -1,
      -1,   168,   169,    14,    15,    16,    17,    18,    -1,   176,
      -1,   178,    -1,   180,   181,    26,   183,    -1,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,   153,   154,    -1,    -1,    -1,   158,
      -1,   160,   161,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,   176,    -1,   120,
     121,    -1,    -1,    -1,    -1,    -1,   185,    -1,   129,    -1,
      32,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   178,    -1,   180,
     181,   182,   183,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,   153,   154,    -1,    -1,    -1,   158,    -1,   160,   161,
      -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,   176,    -1,    -1,   120,   121,    -1,
      -1,    -1,    -1,   185,    -1,    -1,   129,    -1,    -1,    56,
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
     105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   185,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    32,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,
      -1,   168,   169,    14,    15,    16,    17,    18,    -1,   176,
      -1,   178,   179,   180,   181,    26,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     153,   154,    93,    -1,    -1,   158,    -1,    -1,   161,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,   120,
     121,    -1,   185,    -1,    -1,    -1,    -1,    -1,   129,    -1,
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
     105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,   161,   162,   163,    26,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    38,   177,   178,    41,   180,   181,    44,   183,    -1,
      47,    -1,    -1,    -1,    51,    -1,   115,    54,    -1,    -1,
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
      -1,    -1,    21,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   153,   154,    93,    -1,    -1,   158,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    32,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,   165,    -1,    -1,   168,   169,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   178,   179,   180,
     181,    26,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,    -1,
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
      -1,    -1,    -1,   178,   179,   180,   181,    26,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   153,   154,    93,    -1,    -1,   158,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
     159,    -1,   161,   162,   163,    26,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,
      41,   180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,
      51,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     153,   154,    93,    -1,    -1,   158,    -1,    -1,   161,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,   129,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    14,    15,    16,    17,    18,    -1,    -1,    -1,   178,
     179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,
      -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   178,    41,   180,   181,    44,   183,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,   180,   181,    -1,   183,    95,    96,    97,    98,    99,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   175,
     176,   103,   104,   105,   106,   107,   182,    -1,   110,   111,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    20,
      21,   179,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,    20,    21,   179,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,   175,   176,    -1,    -1,   179,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,   175,   176,   177,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,   177,   103,
     104,   105,   106,   107,    20,    21,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,   176,   177,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    20,    21,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      20,    21,    -1,    -1,   103,   104,   105,   106,   107,   175,
     176,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,   128,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    20,    21,    -1,    -1,
     103,   104,   105,   106,   107,   175,   176,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    20,    21,    -1,    -1,   103,   104,   105,   106,
     107,   175,   176,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      20,    21,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,
      -1,    -1,   130,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,    20,    21,    -1,    -1,    -1,    -1,
      -1,   103,   104,   175,   176,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   176,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,   162,   163,   164,   165,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   129,    -1,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,   168,   169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   187,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   176,   182,   190,   198,   199,   203,   223,   227,
     243,   298,   303,   305,   310,   346,   348,   145,   218,   219,
     139,   204,   205,   145,   200,   201,   183,   145,   180,   189,
     349,   347,    33,    59,   129,   145,   220,   221,   222,   235,
       4,     5,     7,    35,   308,    58,   296,   153,   152,   155,
     152,    21,    53,   164,   175,   202,   145,   301,   302,   296,
     145,   145,   145,   129,   178,   152,   177,    53,    59,   228,
     229,   230,    53,    59,   304,    53,    59,   309,    53,    59,
     297,    14,    15,   139,   143,   145,   146,   178,   192,   218,
     139,   205,   145,   145,   145,   153,   184,   301,    53,    59,
     188,   183,   183,   145,   219,   221,    34,   105,   106,   145,
     226,   183,   231,   145,   145,   307,   183,   299,   147,   191,
      14,    15,   139,   143,   145,   192,   216,   217,   202,    24,
      30,    45,    60,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   235,   313,   315,   316,   318,   321,
     145,   317,   145,   288,   289,   179,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   124,   125,   126,   127,   130,   131,
     132,   133,   134,   135,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   168,   169,    34,    34,   178,   224,
     233,   155,   183,   155,   306,   311,   295,   145,   294,   312,
     147,   148,   152,   179,   159,   159,   159,   159,   178,   159,
     159,   159,   159,   159,   159,    32,    56,    57,   115,   153,
     154,   158,   161,   176,   182,   185,   182,   155,   182,   136,
     179,   279,   284,   285,   155,   225,     1,     8,     9,    10,
      12,    14,    15,    16,    17,    18,    25,    26,    27,    28,
      37,    38,    40,    41,    44,    47,    48,    50,    51,    54,
      55,    61,    71,    93,    94,   105,   106,   120,   121,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   146,   161,
     162,   163,   165,   168,   169,   178,   180,   181,   182,   184,
     194,   196,   206,   207,   210,   211,   212,   213,   214,   215,
     231,   232,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   252,   255,   258,   262,   263,   264,
     265,   266,   267,   268,   273,   276,   313,   350,   354,   359,
     361,   363,    66,    70,    87,    88,    89,    90,   314,   300,
     235,   183,   184,   294,   152,   155,   158,   293,   216,   324,
     326,   328,   322,   145,   319,   330,   332,   334,   336,   338,
     340,   342,   344,    32,    32,   158,   176,   185,   177,   276,
     145,   184,   321,   184,   288,   219,    46,   291,   312,   179,
     182,   321,   276,   312,   321,   350,   178,   159,   114,   182,
     234,   264,   276,   231,   276,   178,   276,   159,   159,   178,
     182,   159,   139,    54,   276,   231,   159,   114,   234,   276,
     276,   276,   181,   262,   262,    12,   321,    12,   321,   276,
     356,   360,   193,   276,   276,   276,   235,   276,   276,   276,
     181,    36,   182,   182,   276,   145,   178,   182,   182,   182,
     294,   223,   119,   182,    20,    21,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     122,   123,   124,   125,   126,   127,   130,   131,   132,   133,
     134,   135,   153,   154,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   175,   176,   178,   183,   145,   184,
     282,   145,   321,   114,   123,   153,   292,   321,   321,   321,
     321,   179,   317,   321,   224,   321,   224,   321,   224,   145,
     286,   287,   321,   289,   177,   177,   177,   291,   114,   153,
     155,   158,   290,   284,   231,    19,   178,   235,   253,   234,
     276,   114,   153,   182,    13,   276,   231,   246,   248,   276,
     250,   155,   139,   321,   234,   276,   182,   181,   312,    49,
     145,   161,   178,   276,   351,   352,   353,   355,   356,   357,
     358,   312,   178,   352,   358,   128,   182,   184,   148,   149,
     191,   197,   179,   159,   235,   183,   231,   176,   179,   256,
     276,   136,   261,   262,    18,   145,   145,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   145,   120,   121,   276,   276,   145,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   120,   121,   276,    21,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   145,   274,
     276,   179,   256,   300,   153,   182,   176,   184,   223,   279,
     281,   153,   182,   292,   234,   276,   160,   182,   160,   160,
     160,   182,   160,   225,   160,   225,   160,   225,   155,   160,
     182,   160,   182,   177,   321,   234,   276,   256,   179,   256,
     159,   276,   321,   182,   231,   179,   321,   321,   179,   321,
     160,   182,    19,   262,   123,   290,   179,   152,   182,   353,
     177,   152,   177,   182,    19,   179,   353,   184,   276,   356,
     184,   276,   147,   195,   269,   271,   233,    11,    22,    23,
     208,   209,   145,   351,   152,   179,   114,   123,   153,   158,
     259,   260,   224,   159,   178,   276,   276,   177,   276,   276,
     145,   155,     1,   177,   179,   184,   276,   222,     7,    42,
     280,   182,   234,   276,   182,   325,   321,   329,   323,   320,
     331,   160,   335,   160,   339,   160,   321,   343,   286,   345,
     153,   290,   182,   179,   145,   179,   160,   160,   160,   160,
     261,   256,   276,   276,   352,   353,   145,   351,   177,   177,
     276,   177,   357,   256,   352,   184,   177,   150,   147,   321,
     224,   184,   231,   276,   177,   276,   145,   145,   145,   145,
     152,   177,   225,   277,   179,   256,   276,   275,   182,   177,
      43,   280,   291,   182,   160,   333,   337,   341,   276,   231,
     160,   182,   254,   247,   249,   251,   178,   182,   353,   177,
     123,   290,   177,   182,   353,   177,   160,   225,   231,   179,
     259,   177,   128,   231,   257,   321,   179,   182,   283,   230,
     327,   276,   145,   276,   276,   276,   179,   276,   276,   177,
     177,   276,   276,   276,   184,   270,   160,   209,   114,   276,
     160,   229,   179,   160,   179,   182,   362,   177,   362,   177,
     235,   272,   276,   278,   182,   276,    49,   177,   184,   235,
     179,   276,   177,   177
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
     300,   301,   302,   302,   303,   304,   304,   304,   305,   305,
     306,   306,   307,   308,   308,   309,   309,   309,   311,   310,
     312,   312,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   314,   314,   314,
     314,   314,   314,   315,   316,   316,   317,   317,   319,   320,
     318,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   322,   323,
     321,   321,   324,   325,   321,   326,   327,   321,   328,   329,
     321,   321,   330,   331,   321,   332,   333,   321,   321,   334,
     335,   321,   336,   337,   321,   321,   338,   339,   321,   340,
     341,   321,   342,   343,   321,   344,   345,   321,   347,   346,
     349,   348,   350,   350,   350,   350,   351,   351,   351,   351,
     352,   352,   353,   353,   354,   354,   354,   354,   354,   354,
     355,   355,   356,   357,   357,   358,   358,   359,   359,   360,
     360,   361,   362,   362,   363,   363
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
       5,     4,     1,     2,     4,     0,     1,     1,     7,     9,
       0,     2,     2,     1,     1,     0,     1,     1,     0,     8,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     3,     0,     0,
       6,     1,     1,     1,     1,     4,     3,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     2,     0,     0,
       6,     2,     0,     0,     6,     0,     0,     8,     0,     0,
       6,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     0,     0,     6,     0,     0,     6,     0,     7,
       0,     7,     1,     1,     1,     1,     3,     3,     5,     5,
       1,     3,     0,     2,     6,     5,     7,     8,     6,     8,
       1,     3,     3,     1,     1,     1,     3,     5,     5,     1,
       3,     4,     0,     3,    10,    10
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
#line 3398 "ds_parser.cpp"
        break;

    case 189: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3404 "ds_parser.cpp"
        break;

    case 191: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3410 "ds_parser.cpp"
        break;

    case 192: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3416 "ds_parser.cpp"
        break;

    case 193: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3422 "ds_parser.cpp"
        break;

    case 194: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3428 "ds_parser.cpp"
        break;

    case 196: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3434 "ds_parser.cpp"
        break;

    case 200: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3440 "ds_parser.cpp"
        break;

    case 206: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3446 "ds_parser.cpp"
        break;

    case 207: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3452 "ds_parser.cpp"
        break;

    case 209: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3458 "ds_parser.cpp"
        break;

    case 211: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3464 "ds_parser.cpp"
        break;

    case 212: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3470 "ds_parser.cpp"
        break;

    case 213: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3476 "ds_parser.cpp"
        break;

    case 214: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3482 "ds_parser.cpp"
        break;

    case 215: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3488 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3494 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3500 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3506 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3512 "ds_parser.cpp"
        break;

    case 220: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3518 "ds_parser.cpp"
        break;

    case 221: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3524 "ds_parser.cpp"
        break;

    case 222: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3530 "ds_parser.cpp"
        break;

    case 223: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3536 "ds_parser.cpp"
        break;

    case 224: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3542 "ds_parser.cpp"
        break;

    case 225: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3548 "ds_parser.cpp"
        break;

    case 226: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3554 "ds_parser.cpp"
        break;

    case 231: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3560 "ds_parser.cpp"
        break;

    case 232: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3566 "ds_parser.cpp"
        break;

    case 233: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3572 "ds_parser.cpp"
        break;

    case 234: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3578 "ds_parser.cpp"
        break;

    case 235: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3584 "ds_parser.cpp"
        break;

    case 236: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3590 "ds_parser.cpp"
        break;

    case 237: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3596 "ds_parser.cpp"
        break;

    case 238: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3602 "ds_parser.cpp"
        break;

    case 239: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3608 "ds_parser.cpp"
        break;

    case 240: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3614 "ds_parser.cpp"
        break;

    case 241: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3620 "ds_parser.cpp"
        break;

    case 242: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3626 "ds_parser.cpp"
        break;

    case 244: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3632 "ds_parser.cpp"
        break;

    case 245: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3638 "ds_parser.cpp"
        break;

    case 252: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3644 "ds_parser.cpp"
        break;

    case 255: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3650 "ds_parser.cpp"
        break;

    case 256: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3656 "ds_parser.cpp"
        break;

    case 257: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3662 "ds_parser.cpp"
        break;

    case 259: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3668 "ds_parser.cpp"
        break;

    case 260: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3674 "ds_parser.cpp"
        break;

    case 261: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3680 "ds_parser.cpp"
        break;

    case 262: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3686 "ds_parser.cpp"
        break;

    case 263: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3692 "ds_parser.cpp"
        break;

    case 264: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3698 "ds_parser.cpp"
        break;

    case 265: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3704 "ds_parser.cpp"
        break;

    case 266: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3710 "ds_parser.cpp"
        break;

    case 267: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3716 "ds_parser.cpp"
        break;

    case 268: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3722 "ds_parser.cpp"
        break;

    case 273: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3728 "ds_parser.cpp"
        break;

    case 276: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3734 "ds_parser.cpp"
        break;

    case 279: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3740 "ds_parser.cpp"
        break;

    case 281: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3746 "ds_parser.cpp"
        break;

    case 282: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3752 "ds_parser.cpp"
        break;

    case 284: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3758 "ds_parser.cpp"
        break;

    case 285: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3764 "ds_parser.cpp"
        break;

    case 286: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3770 "ds_parser.cpp"
        break;

    case 287: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3776 "ds_parser.cpp"
        break;

    case 288: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3782 "ds_parser.cpp"
        break;

    case 289: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3788 "ds_parser.cpp"
        break;

    case 291: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3794 "ds_parser.cpp"
        break;

    case 294: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3800 "ds_parser.cpp"
        break;

    case 295: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3806 "ds_parser.cpp"
        break;

    case 300: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3812 "ds_parser.cpp"
        break;

    case 306: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3818 "ds_parser.cpp"
        break;

    case 312: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3824 "ds_parser.cpp"
        break;

    case 315: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3830 "ds_parser.cpp"
        break;

    case 316: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3836 "ds_parser.cpp"
        break;

    case 317: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3842 "ds_parser.cpp"
        break;

    case 318: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3848 "ds_parser.cpp"
        break;

    case 321: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3854 "ds_parser.cpp"
        break;

    case 350: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3860 "ds_parser.cpp"
        break;

    case 351: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3866 "ds_parser.cpp"
        break;

    case 352: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3872 "ds_parser.cpp"
        break;

    case 353: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3878 "ds_parser.cpp"
        break;

    case 354: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3884 "ds_parser.cpp"
        break;

    case 355: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3890 "ds_parser.cpp"
        break;

    case 356: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3896 "ds_parser.cpp"
        break;

    case 357: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3902 "ds_parser.cpp"
        break;

    case 358: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3908 "ds_parser.cpp"
        break;

    case 359: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3914 "ds_parser.cpp"
        break;

    case 360: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3920 "ds_parser.cpp"
        break;

    case 361: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3926 "ds_parser.cpp"
        break;

    case 362: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3932 "ds_parser.cpp"
        break;

    case 363: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3938 "ds_parser.cpp"
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
#line 467 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4230 "ds_parser.cpp"
    break;

  case 16:
#line 468 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4236 "ds_parser.cpp"
    break;

  case 17:
#line 469 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4242 "ds_parser.cpp"
    break;

  case 18:
#line 473 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4248 "ds_parser.cpp"
    break;

  case 19:
#line 474 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4254 "ds_parser.cpp"
    break;

  case 20:
#line 478 "ds_parser.ypp"
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
#line 4273 "ds_parser.cpp"
    break;

  case 21:
#line 495 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4279 "ds_parser.cpp"
    break;

  case 22:
#line 496 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4285 "ds_parser.cpp"
    break;

  case 23:
#line 500 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4291 "ds_parser.cpp"
    break;

  case 24:
#line 504 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4301 "ds_parser.cpp"
    break;

  case 25:
#line 509 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4315 "ds_parser.cpp"
    break;

  case 26:
#line 518 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4325 "ds_parser.cpp"
    break;

  case 27:
#line 526 "ds_parser.ypp"
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
#line 4343 "ds_parser.cpp"
    break;

  case 28:
#line 542 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4353 "ds_parser.cpp"
    break;

  case 29:
#line 547 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4363 "ds_parser.cpp"
    break;

  case 30:
#line 555 "ds_parser.ypp"
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
#line 4390 "ds_parser.cpp"
    break;

  case 31:
#line 576 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4402 "ds_parser.cpp"
    break;

  case 32:
#line 585 "ds_parser.ypp"
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
#line 4426 "ds_parser.cpp"
    break;

  case 34:
#line 611 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4434 "ds_parser.cpp"
    break;

  case 35:
#line 614 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4445 "ds_parser.cpp"
    break;

  case 36:
#line 620 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4456 "ds_parser.cpp"
    break;

  case 37:
#line 629 "ds_parser.ypp"
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
#line 4482 "ds_parser.cpp"
    break;

  case 38:
#line 650 "ds_parser.ypp"
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
#line 4507 "ds_parser.cpp"
    break;

  case 39:
#line 673 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4513 "ds_parser.cpp"
    break;

  case 40:
#line 674 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4519 "ds_parser.cpp"
    break;

  case 44:
#line 687 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4527 "ds_parser.cpp"
    break;

  case 45:
#line 690 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4535 "ds_parser.cpp"
    break;

  case 46:
#line 696 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4543 "ds_parser.cpp"
    break;

  case 47:
#line 702 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4551 "ds_parser.cpp"
    break;

  case 48:
#line 705 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4559 "ds_parser.cpp"
    break;

  case 49:
#line 711 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4565 "ds_parser.cpp"
    break;

  case 50:
#line 712 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4571 "ds_parser.cpp"
    break;

  case 51:
#line 716 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4577 "ds_parser.cpp"
    break;

  case 52:
#line 717 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4583 "ds_parser.cpp"
    break;

  case 53:
#line 718 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4594 "ds_parser.cpp"
    break;

  case 54:
#line 727 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4600 "ds_parser.cpp"
    break;

  case 55:
#line 728 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4606 "ds_parser.cpp"
    break;

  case 56:
#line 732 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4617 "ds_parser.cpp"
    break;

  case 57:
#line 741 "ds_parser.ypp"
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
#line 4635 "ds_parser.cpp"
    break;

  case 58:
#line 757 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4645 "ds_parser.cpp"
    break;

  case 59:
#line 765 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4657 "ds_parser.cpp"
    break;

  case 60:
#line 775 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4668 "ds_parser.cpp"
    break;

  case 61:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4674 "ds_parser.cpp"
    break;

  case 62:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4680 "ds_parser.cpp"
    break;

  case 63:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4686 "ds_parser.cpp"
    break;

  case 64:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4692 "ds_parser.cpp"
    break;

  case 65:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4698 "ds_parser.cpp"
    break;

  case 66:
#line 789 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4704 "ds_parser.cpp"
    break;

  case 67:
#line 793 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4714 "ds_parser.cpp"
    break;

  case 68:
#line 798 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4724 "ds_parser.cpp"
    break;

  case 69:
#line 806 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4730 "ds_parser.cpp"
    break;

  case 70:
#line 807 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4736 "ds_parser.cpp"
    break;

  case 71:
#line 808 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4742 "ds_parser.cpp"
    break;

  case 72:
#line 809 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4748 "ds_parser.cpp"
    break;

  case 73:
#line 810 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4754 "ds_parser.cpp"
    break;

  case 74:
#line 811 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4760 "ds_parser.cpp"
    break;

  case 75:
#line 812 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4766 "ds_parser.cpp"
    break;

  case 76:
#line 813 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4774 "ds_parser.cpp"
    break;

  case 77:
#line 819 "ds_parser.ypp"
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
#line 4793 "ds_parser.cpp"
    break;

  case 78:
#line 833 "ds_parser.ypp"
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
#line 4812 "ds_parser.cpp"
    break;

  case 79:
#line 850 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4818 "ds_parser.cpp"
    break;

  case 80:
#line 851 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4824 "ds_parser.cpp"
    break;

  case 81:
#line 852 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4830 "ds_parser.cpp"
    break;

  case 82:
#line 856 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4843 "ds_parser.cpp"
    break;

  case 83:
#line 864 "ds_parser.ypp"
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
#line 4858 "ds_parser.cpp"
    break;

  case 84:
#line 877 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4867 "ds_parser.cpp"
    break;

  case 85:
#line 881 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4876 "ds_parser.cpp"
    break;

  case 86:
#line 888 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4882 "ds_parser.cpp"
    break;

  case 87:
#line 889 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4888 "ds_parser.cpp"
    break;

  case 88:
#line 893 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4894 "ds_parser.cpp"
    break;

  case 89:
#line 894 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4900 "ds_parser.cpp"
    break;

  case 90:
#line 895 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4906 "ds_parser.cpp"
    break;

  case 91:
#line 899 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4914 "ds_parser.cpp"
    break;

  case 92:
#line 902 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4923 "ds_parser.cpp"
    break;

  case 93:
#line 909 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4932 "ds_parser.cpp"
    break;

  case 94:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4938 "ds_parser.cpp"
    break;

  case 95:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4944 "ds_parser.cpp"
    break;

  case 96:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4950 "ds_parser.cpp"
    break;

  case 97:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4956 "ds_parser.cpp"
    break;

  case 98:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4962 "ds_parser.cpp"
    break;

  case 99:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4968 "ds_parser.cpp"
    break;

  case 100:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4974 "ds_parser.cpp"
    break;

  case 101:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4980 "ds_parser.cpp"
    break;

  case 102:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4986 "ds_parser.cpp"
    break;

  case 103:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4992 "ds_parser.cpp"
    break;

  case 104:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4998 "ds_parser.cpp"
    break;

  case 105:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5004 "ds_parser.cpp"
    break;

  case 106:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5010 "ds_parser.cpp"
    break;

  case 107:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5016 "ds_parser.cpp"
    break;

  case 108:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5022 "ds_parser.cpp"
    break;

  case 109:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5028 "ds_parser.cpp"
    break;

  case 110:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5034 "ds_parser.cpp"
    break;

  case 111:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5040 "ds_parser.cpp"
    break;

  case 112:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5046 "ds_parser.cpp"
    break;

  case 113:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5052 "ds_parser.cpp"
    break;

  case 114:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5058 "ds_parser.cpp"
    break;

  case 115:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5064 "ds_parser.cpp"
    break;

  case 116:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5070 "ds_parser.cpp"
    break;

  case 117:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5076 "ds_parser.cpp"
    break;

  case 118:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5082 "ds_parser.cpp"
    break;

  case 119:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5088 "ds_parser.cpp"
    break;

  case 120:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5094 "ds_parser.cpp"
    break;

  case 121:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5100 "ds_parser.cpp"
    break;

  case 122:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5106 "ds_parser.cpp"
    break;

  case 123:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5112 "ds_parser.cpp"
    break;

  case 124:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5118 "ds_parser.cpp"
    break;

  case 125:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5124 "ds_parser.cpp"
    break;

  case 126:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5130 "ds_parser.cpp"
    break;

  case 127:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5136 "ds_parser.cpp"
    break;

  case 128:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5142 "ds_parser.cpp"
    break;

  case 129:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5148 "ds_parser.cpp"
    break;

  case 130:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5154 "ds_parser.cpp"
    break;

  case 131:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5160 "ds_parser.cpp"
    break;

  case 132:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5166 "ds_parser.cpp"
    break;

  case 133:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5172 "ds_parser.cpp"
    break;

  case 134:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5178 "ds_parser.cpp"
    break;

  case 135:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5184 "ds_parser.cpp"
    break;

  case 136:
#line 958 "ds_parser.ypp"
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

  case 137:
#line 980 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5214 "ds_parser.cpp"
    break;

  case 138:
#line 981 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5220 "ds_parser.cpp"
    break;

  case 139:
#line 982 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5226 "ds_parser.cpp"
    break;

  case 140:
#line 986 "ds_parser.ypp"
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
#line 5267 "ds_parser.cpp"
    break;

  case 141:
#line 1025 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5276 "ds_parser.cpp"
    break;

  case 142:
#line 1032 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5285 "ds_parser.cpp"
    break;

  case 143:
#line 1036 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5298 "ds_parser.cpp"
    break;

  case 144:
#line 1047 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5304 "ds_parser.cpp"
    break;

  case 145:
#line 1048 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5310 "ds_parser.cpp"
    break;

  case 146:
#line 1049 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5316 "ds_parser.cpp"
    break;

  case 147:
#line 1050 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5322 "ds_parser.cpp"
    break;

  case 148:
#line 1051 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5328 "ds_parser.cpp"
    break;

  case 149:
#line 1052 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5334 "ds_parser.cpp"
    break;

  case 150:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5340 "ds_parser.cpp"
    break;

  case 151:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5346 "ds_parser.cpp"
    break;

  case 152:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5352 "ds_parser.cpp"
    break;

  case 153:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5358 "ds_parser.cpp"
    break;

  case 154:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5364 "ds_parser.cpp"
    break;

  case 155:
#line 1058 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5370 "ds_parser.cpp"
    break;

  case 156:
#line 1059 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5376 "ds_parser.cpp"
    break;

  case 157:
#line 1060 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5382 "ds_parser.cpp"
    break;

  case 158:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5388 "ds_parser.cpp"
    break;

  case 159:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5394 "ds_parser.cpp"
    break;

  case 160:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5400 "ds_parser.cpp"
    break;

  case 161:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5406 "ds_parser.cpp"
    break;

  case 162:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5412 "ds_parser.cpp"
    break;

  case 163:
#line 1069 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5422 "ds_parser.cpp"
    break;

  case 164:
#line 1074 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5433 "ds_parser.cpp"
    break;

  case 165:
#line 1080 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5441 "ds_parser.cpp"
    break;

  case 166:
#line 1086 "ds_parser.ypp"
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
#line 5480 "ds_parser.cpp"
    break;

  case 167:
#line 1120 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5488 "ds_parser.cpp"
    break;

  case 168:
#line 1123 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5496 "ds_parser.cpp"
    break;

  case 169:
#line 1129 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5502 "ds_parser.cpp"
    break;

  case 170:
#line 1130 "ds_parser.ypp"
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
#line 5518 "ds_parser.cpp"
    break;

  case 171:
#line 1141 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5524 "ds_parser.cpp"
    break;

  case 172:
#line 1145 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5532 "ds_parser.cpp"
    break;

  case 173:
#line 1151 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5540 "ds_parser.cpp"
    break;

  case 174:
#line 1154 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5548 "ds_parser.cpp"
    break;

  case 175:
#line 1157 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5557 "ds_parser.cpp"
    break;

  case 176:
#line 1161 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5565 "ds_parser.cpp"
    break;

  case 177:
#line 1167 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5571 "ds_parser.cpp"
    break;

  case 178:
#line 1171 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5577 "ds_parser.cpp"
    break;

  case 179:
#line 1175 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5585 "ds_parser.cpp"
    break;

  case 180:
#line 1178 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5593 "ds_parser.cpp"
    break;

  case 181:
#line 1181 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5603 "ds_parser.cpp"
    break;

  case 182:
#line 1186 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5611 "ds_parser.cpp"
    break;

  case 183:
#line 1189 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5621 "ds_parser.cpp"
    break;

  case 184:
#line 1197 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5629 "ds_parser.cpp"
    break;

  case 185:
#line 1200 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5639 "ds_parser.cpp"
    break;

  case 186:
#line 1205 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5647 "ds_parser.cpp"
    break;

  case 187:
#line 1208 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5657 "ds_parser.cpp"
    break;

  case 188:
#line 1216 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5665 "ds_parser.cpp"
    break;

  case 189:
#line 1222 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5671 "ds_parser.cpp"
    break;

  case 190:
#line 1223 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5677 "ds_parser.cpp"
    break;

  case 191:
#line 1227 "ds_parser.ypp"
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
#line 5713 "ds_parser.cpp"
    break;

  case 192:
#line 1261 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5719 "ds_parser.cpp"
    break;

  case 193:
#line 1261 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5725 "ds_parser.cpp"
    break;

  case 194:
#line 1261 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5733 "ds_parser.cpp"
    break;

  case 195:
#line 1264 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5739 "ds_parser.cpp"
    break;

  case 196:
#line 1264 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5745 "ds_parser.cpp"
    break;

  case 197:
#line 1264 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5755 "ds_parser.cpp"
    break;

  case 198:
#line 1269 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5761 "ds_parser.cpp"
    break;

  case 199:
#line 1269 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5767 "ds_parser.cpp"
    break;

  case 200:
#line 1269 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5777 "ds_parser.cpp"
    break;

  case 201:
#line 1277 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5783 "ds_parser.cpp"
    break;

  case 202:
#line 1277 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5789 "ds_parser.cpp"
    break;

  case 203:
#line 1277 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5797 "ds_parser.cpp"
    break;

  case 204:
#line 1283 "ds_parser.ypp"
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
#line 5812 "ds_parser.cpp"
    break;

  case 205:
#line 1293 "ds_parser.ypp"
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
#line 5828 "ds_parser.cpp"
    break;

  case 206:
#line 1304 "ds_parser.ypp"
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
#line 5845 "ds_parser.cpp"
    break;

  case 207:
#line 1319 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5853 "ds_parser.cpp"
    break;

  case 208:
#line 1322 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5861 "ds_parser.cpp"
    break;

  case 209:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5867 "ds_parser.cpp"
    break;

  case 210:
#line 1329 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5879 "ds_parser.cpp"
    break;

  case 211:
#line 1336 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5892 "ds_parser.cpp"
    break;

  case 212:
#line 1347 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5898 "ds_parser.cpp"
    break;

  case 213:
#line 1348 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5904 "ds_parser.cpp"
    break;

  case 214:
#line 1349 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5910 "ds_parser.cpp"
    break;

  case 215:
#line 1353 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5916 "ds_parser.cpp"
    break;

  case 216:
#line 1354 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5922 "ds_parser.cpp"
    break;

  case 217:
#line 1355 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5928 "ds_parser.cpp"
    break;

  case 218:
#line 1356 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5934 "ds_parser.cpp"
    break;

  case 219:
#line 1360 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5944 "ds_parser.cpp"
    break;

  case 220:
#line 1365 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5954 "ds_parser.cpp"
    break;

  case 221:
#line 1373 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5960 "ds_parser.cpp"
    break;

  case 222:
#line 1374 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5966 "ds_parser.cpp"
    break;

  case 223:
#line 1378 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5976 "ds_parser.cpp"
    break;

  case 224:
#line 1384 "ds_parser.ypp"
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
#line 6041 "ds_parser.cpp"
    break;

  case 225:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6047 "ds_parser.cpp"
    break;

  case 226:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6053 "ds_parser.cpp"
    break;

  case 227:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6059 "ds_parser.cpp"
    break;

  case 228:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6065 "ds_parser.cpp"
    break;

  case 229:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6071 "ds_parser.cpp"
    break;

  case 230:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6077 "ds_parser.cpp"
    break;

  case 231:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6083 "ds_parser.cpp"
    break;

  case 232:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6089 "ds_parser.cpp"
    break;

  case 233:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6095 "ds_parser.cpp"
    break;

  case 234:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6101 "ds_parser.cpp"
    break;

  case 235:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6107 "ds_parser.cpp"
    break;

  case 236:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6113 "ds_parser.cpp"
    break;

  case 237:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6119 "ds_parser.cpp"
    break;

  case 238:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6125 "ds_parser.cpp"
    break;

  case 239:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6131 "ds_parser.cpp"
    break;

  case 240:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6137 "ds_parser.cpp"
    break;

  case 241:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6143 "ds_parser.cpp"
    break;

  case 242:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6149 "ds_parser.cpp"
    break;

  case 243:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6155 "ds_parser.cpp"
    break;

  case 244:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6161 "ds_parser.cpp"
    break;

  case 245:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6167 "ds_parser.cpp"
    break;

  case 246:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6173 "ds_parser.cpp"
    break;

  case 247:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6179 "ds_parser.cpp"
    break;

  case 248:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6185 "ds_parser.cpp"
    break;

  case 249:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6191 "ds_parser.cpp"
    break;

  case 250:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6197 "ds_parser.cpp"
    break;

  case 251:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6203 "ds_parser.cpp"
    break;

  case 252:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6209 "ds_parser.cpp"
    break;

  case 253:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6215 "ds_parser.cpp"
    break;

  case 254:
#line 1485 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6227 "ds_parser.cpp"
    break;

  case 255:
#line 1496 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6237 "ds_parser.cpp"
    break;

  case 256:
#line 1501 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6249 "ds_parser.cpp"
    break;

  case 257:
#line 1511 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6258 "ds_parser.cpp"
    break;

  case 258:
#line 1515 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6264 "ds_parser.cpp"
    break;

  case 259:
#line 1515 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6270 "ds_parser.cpp"
    break;

  case 260:
#line 1515 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6282 "ds_parser.cpp"
    break;

  case 261:
#line 1522 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6288 "ds_parser.cpp"
    break;

  case 262:
#line 1522 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6294 "ds_parser.cpp"
    break;

  case 263:
#line 1522 "ds_parser.ypp"
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
#line 6310 "ds_parser.cpp"
    break;

  case 264:
#line 1536 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6319 "ds_parser.cpp"
    break;

  case 265:
#line 1540 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6325 "ds_parser.cpp"
    break;

  case 266:
#line 1540 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6331 "ds_parser.cpp"
    break;

  case 267:
#line 1540 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6340 "ds_parser.cpp"
    break;

  case 268:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6346 "ds_parser.cpp"
    break;

  case 269:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6352 "ds_parser.cpp"
    break;

  case 270:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6358 "ds_parser.cpp"
    break;

  case 271:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6364 "ds_parser.cpp"
    break;

  case 272:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6370 "ds_parser.cpp"
    break;

  case 273:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6376 "ds_parser.cpp"
    break;

  case 274:
#line 1553 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6382 "ds_parser.cpp"
    break;

  case 275:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6388 "ds_parser.cpp"
    break;

  case 276:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6394 "ds_parser.cpp"
    break;

  case 277:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6400 "ds_parser.cpp"
    break;

  case 278:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6406 "ds_parser.cpp"
    break;

  case 279:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6412 "ds_parser.cpp"
    break;

  case 280:
#line 1559 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6418 "ds_parser.cpp"
    break;

  case 281:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6424 "ds_parser.cpp"
    break;

  case 282:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6430 "ds_parser.cpp"
    break;

  case 283:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6436 "ds_parser.cpp"
    break;

  case 284:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6442 "ds_parser.cpp"
    break;

  case 285:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6448 "ds_parser.cpp"
    break;

  case 286:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6454 "ds_parser.cpp"
    break;

  case 287:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6460 "ds_parser.cpp"
    break;

  case 288:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6466 "ds_parser.cpp"
    break;

  case 289:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6472 "ds_parser.cpp"
    break;

  case 290:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6478 "ds_parser.cpp"
    break;

  case 291:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6484 "ds_parser.cpp"
    break;

  case 292:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6490 "ds_parser.cpp"
    break;

  case 293:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6496 "ds_parser.cpp"
    break;

  case 294:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6502 "ds_parser.cpp"
    break;

  case 295:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6508 "ds_parser.cpp"
    break;

  case 296:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6514 "ds_parser.cpp"
    break;

  case 297:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6520 "ds_parser.cpp"
    break;

  case 298:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6526 "ds_parser.cpp"
    break;

  case 299:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6532 "ds_parser.cpp"
    break;

  case 300:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6538 "ds_parser.cpp"
    break;

  case 301:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6544 "ds_parser.cpp"
    break;

  case 302:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6550 "ds_parser.cpp"
    break;

  case 303:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6556 "ds_parser.cpp"
    break;

  case 304:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6562 "ds_parser.cpp"
    break;

  case 305:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6568 "ds_parser.cpp"
    break;

  case 306:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6574 "ds_parser.cpp"
    break;

  case 307:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6580 "ds_parser.cpp"
    break;

  case 308:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6586 "ds_parser.cpp"
    break;

  case 309:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6592 "ds_parser.cpp"
    break;

  case 310:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6598 "ds_parser.cpp"
    break;

  case 311:
#line 1590 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6607 "ds_parser.cpp"
    break;

  case 312:
#line 1594 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6616 "ds_parser.cpp"
    break;

  case 313:
#line 1598 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6624 "ds_parser.cpp"
    break;

  case 314:
#line 1601 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6632 "ds_parser.cpp"
    break;

  case 315:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6638 "ds_parser.cpp"
    break;

  case 316:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6644 "ds_parser.cpp"
    break;

  case 317:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6650 "ds_parser.cpp"
    break;

  case 318:
#line 1607 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6664 "ds_parser.cpp"
    break;

  case 319:
#line 1616 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6678 "ds_parser.cpp"
    break;

  case 320:
#line 1625 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6684 "ds_parser.cpp"
    break;

  case 321:
#line 1626 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6692 "ds_parser.cpp"
    break;

  case 322:
#line 1629 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6698 "ds_parser.cpp"
    break;

  case 323:
#line 1629 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6704 "ds_parser.cpp"
    break;

  case 324:
#line 1629 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6712 "ds_parser.cpp"
    break;

  case 325:
#line 1632 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6721 "ds_parser.cpp"
    break;

  case 326:
#line 1636 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6730 "ds_parser.cpp"
    break;

  case 327:
#line 1640 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6739 "ds_parser.cpp"
    break;

  case 328:
#line 1644 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6745 "ds_parser.cpp"
    break;

  case 329:
#line 1645 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6751 "ds_parser.cpp"
    break;

  case 330:
#line 1646 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6757 "ds_parser.cpp"
    break;

  case 331:
#line 1647 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6763 "ds_parser.cpp"
    break;

  case 332:
#line 1648 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6769 "ds_parser.cpp"
    break;

  case 333:
#line 1649 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6775 "ds_parser.cpp"
    break;

  case 334:
#line 1650 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6781 "ds_parser.cpp"
    break;

  case 335:
#line 1651 "ds_parser.ypp"
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
#line 6802 "ds_parser.cpp"
    break;

  case 336:
#line 1667 "ds_parser.ypp"
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
#line 6823 "ds_parser.cpp"
    break;

  case 337:
#line 1683 "ds_parser.ypp"
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
#line 6883 "ds_parser.cpp"
    break;

  case 338:
#line 1741 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6889 "ds_parser.cpp"
    break;

  case 339:
#line 1742 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6895 "ds_parser.cpp"
    break;

  case 340:
#line 1746 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6901 "ds_parser.cpp"
    break;

  case 341:
#line 1747 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6907 "ds_parser.cpp"
    break;

  case 342:
#line 1751 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6917 "ds_parser.cpp"
    break;

  case 343:
#line 1759 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6925 "ds_parser.cpp"
    break;

  case 344:
#line 1762 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6934 "ds_parser.cpp"
    break;

  case 345:
#line 1767 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6940 "ds_parser.cpp"
    break;

  case 346:
#line 1767 "ds_parser.ypp"
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
#line 6989 "ds_parser.cpp"
    break;

  case 347:
#line 1811 "ds_parser.ypp"
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
#line 7050 "ds_parser.cpp"
    break;

  case 348:
#line 1867 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7061 "ds_parser.cpp"
    break;

  case 349:
#line 1876 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7071 "ds_parser.cpp"
    break;

  case 350:
#line 1881 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7081 "ds_parser.cpp"
    break;

  case 351:
#line 1889 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7087 "ds_parser.cpp"
    break;

  case 352:
#line 1890 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7093 "ds_parser.cpp"
    break;

  case 353:
#line 1894 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7101 "ds_parser.cpp"
    break;

  case 354:
#line 1897 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7112 "ds_parser.cpp"
    break;

  case 355:
#line 1906 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7118 "ds_parser.cpp"
    break;

  case 356:
#line 1907 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7124 "ds_parser.cpp"
    break;

  case 357:
#line 1911 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7135 "ds_parser.cpp"
    break;

  case 358:
#line 1920 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7141 "ds_parser.cpp"
    break;

  case 359:
#line 1921 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7147 "ds_parser.cpp"
    break;

  case 360:
#line 1926 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7153 "ds_parser.cpp"
    break;

  case 361:
#line 1927 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7159 "ds_parser.cpp"
    break;

  case 362:
#line 1931 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7170 "ds_parser.cpp"
    break;

  case 363:
#line 1937 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7181 "ds_parser.cpp"
    break;

  case 364:
#line 1943 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7189 "ds_parser.cpp"
    break;

  case 365:
#line 1946 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7198 "ds_parser.cpp"
    break;

  case 366:
#line 1950 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7209 "ds_parser.cpp"
    break;

  case 367:
#line 1956 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7220 "ds_parser.cpp"
    break;

  case 368:
#line 1965 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7226 "ds_parser.cpp"
    break;

  case 369:
#line 1966 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7232 "ds_parser.cpp"
    break;

  case 370:
#line 1967 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7238 "ds_parser.cpp"
    break;

  case 371:
#line 1971 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7244 "ds_parser.cpp"
    break;

  case 372:
#line 1972 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7250 "ds_parser.cpp"
    break;

  case 373:
#line 1976 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7258 "ds_parser.cpp"
    break;

  case 374:
#line 1979 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7268 "ds_parser.cpp"
    break;

  case 375:
#line 1984 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7278 "ds_parser.cpp"
    break;

  case 376:
#line 1989 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7291 "ds_parser.cpp"
    break;

  case 377:
#line 1997 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7304 "ds_parser.cpp"
    break;

  case 378:
#line 2008 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7312 "ds_parser.cpp"
    break;

  case 379:
#line 2011 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7321 "ds_parser.cpp"
    break;

  case 380:
#line 2018 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7327 "ds_parser.cpp"
    break;

  case 381:
#line 2019 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7333 "ds_parser.cpp"
    break;

  case 382:
#line 2023 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7339 "ds_parser.cpp"
    break;

  case 383:
#line 2024 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7345 "ds_parser.cpp"
    break;

  case 384:
#line 2025 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7351 "ds_parser.cpp"
    break;

  case 385:
#line 2029 "ds_parser.ypp"
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
#line 7384 "ds_parser.cpp"
    break;

  case 386:
#line 2057 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7390 "ds_parser.cpp"
    break;

  case 387:
#line 2057 "ds_parser.ypp"
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
#line 7422 "ds_parser.cpp"
    break;

  case 388:
#line 2087 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7430 "ds_parser.cpp"
    break;

  case 389:
#line 2090 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7444 "ds_parser.cpp"
    break;

  case 390:
#line 2099 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7458 "ds_parser.cpp"
    break;

  case 391:
#line 2112 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7472 "ds_parser.cpp"
    break;

  case 395:
#line 2133 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7478 "ds_parser.cpp"
    break;

  case 396:
#line 2134 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7484 "ds_parser.cpp"
    break;

  case 397:
#line 2135 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7490 "ds_parser.cpp"
    break;

  case 398:
#line 2139 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-3]));
        pEnum->name = *(yyvsp[-3].s);
        pEnum->isPrivate = !(yyvsp[-4].b);
        if ( (yyvsp[-6].faList) ) {
            for ( auto pA : *(yyvsp[-6].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isEnumerationAnnotation() ) {
                        auto ann = static_pointer_cast<EnumerationAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->touch(pEnum, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't 'touch' with enumeration annotation\n" + err,
                                tokAt((yylsp[-5])), CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *(yyvsp[-6].faList) );
            delete (yyvsp[-6].faList);
        }
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7523 "ds_parser.cpp"
    break;

  case 399:
#line 2167 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-5].s),tokAt((yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-5]));
        pEnum->name = *(yyvsp[-5].s);
        pEnum->baseType = (yyvsp[-3].type);
        pEnum->isPrivate = !(yyvsp[-6].b);
        if ( (yyvsp[-8].faList) ) {
            for ( auto pA : *(yyvsp[-8].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isEnumerationAnnotation() ) {
                        auto ann = static_pointer_cast<EnumerationAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->touch(pEnum, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't 'touch' with enumeration annotation\n" + err,
                                tokAt((yylsp[-7])), CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *(yyvsp[-8].faList) );
            delete (yyvsp[-8].faList);
        }
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-5].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-5].s);
    }
#line 7557 "ds_parser.cpp"
    break;

  case 400:
#line 2199 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7563 "ds_parser.cpp"
    break;

  case 401:
#line 2200 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7569 "ds_parser.cpp"
    break;

  case 402:
#line 2204 "ds_parser.ypp"
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
#line 7608 "ds_parser.cpp"
    break;

  case 403:
#line 2241 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7614 "ds_parser.cpp"
    break;

  case 404:
#line 2242 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7620 "ds_parser.cpp"
    break;

  case 405:
#line 2246 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7626 "ds_parser.cpp"
    break;

  case 406:
#line 2247 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7632 "ds_parser.cpp"
    break;

  case 407:
#line 2248 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7638 "ds_parser.cpp"
    break;

  case 408:
#line 2253 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7644 "ds_parser.cpp"
    break;

  case 409:
#line 2253 "ds_parser.ypp"
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
#line 7730 "ds_parser.cpp"
    break;

  case 410:
#line 2337 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7742 "ds_parser.cpp"
    break;

  case 411:
#line 2344 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7753 "ds_parser.cpp"
    break;

  case 412:
#line 2353 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7759 "ds_parser.cpp"
    break;

  case 413:
#line 2354 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7765 "ds_parser.cpp"
    break;

  case 414:
#line 2355 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7771 "ds_parser.cpp"
    break;

  case 415:
#line 2356 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7777 "ds_parser.cpp"
    break;

  case 416:
#line 2357 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7783 "ds_parser.cpp"
    break;

  case 417:
#line 2358 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7789 "ds_parser.cpp"
    break;

  case 418:
#line 2359 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7795 "ds_parser.cpp"
    break;

  case 419:
#line 2360 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7801 "ds_parser.cpp"
    break;

  case 420:
#line 2361 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7807 "ds_parser.cpp"
    break;

  case 421:
#line 2362 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7813 "ds_parser.cpp"
    break;

  case 422:
#line 2363 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7819 "ds_parser.cpp"
    break;

  case 423:
#line 2364 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7825 "ds_parser.cpp"
    break;

  case 424:
#line 2365 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7831 "ds_parser.cpp"
    break;

  case 425:
#line 2366 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7837 "ds_parser.cpp"
    break;

  case 426:
#line 2367 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7843 "ds_parser.cpp"
    break;

  case 427:
#line 2368 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7849 "ds_parser.cpp"
    break;

  case 428:
#line 2369 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7855 "ds_parser.cpp"
    break;

  case 429:
#line 2370 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7861 "ds_parser.cpp"
    break;

  case 430:
#line 2371 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7867 "ds_parser.cpp"
    break;

  case 431:
#line 2372 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7873 "ds_parser.cpp"
    break;

  case 432:
#line 2373 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7879 "ds_parser.cpp"
    break;

  case 433:
#line 2374 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7885 "ds_parser.cpp"
    break;

  case 434:
#line 2375 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7891 "ds_parser.cpp"
    break;

  case 435:
#line 2376 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7897 "ds_parser.cpp"
    break;

  case 436:
#line 2377 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7903 "ds_parser.cpp"
    break;

  case 437:
#line 2381 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7909 "ds_parser.cpp"
    break;

  case 438:
#line 2382 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7915 "ds_parser.cpp"
    break;

  case 439:
#line 2383 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7921 "ds_parser.cpp"
    break;

  case 440:
#line 2384 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7927 "ds_parser.cpp"
    break;

  case 441:
#line 2385 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7933 "ds_parser.cpp"
    break;

  case 442:
#line 2386 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7939 "ds_parser.cpp"
    break;

  case 443:
#line 2390 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7952 "ds_parser.cpp"
    break;

  case 444:
#line 2401 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7961 "ds_parser.cpp"
    break;

  case 445:
#line 2405 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7973 "ds_parser.cpp"
    break;

  case 446:
#line 2415 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7985 "ds_parser.cpp"
    break;

  case 447:
#line 2422 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7996 "ds_parser.cpp"
    break;

  case 448:
#line 2431 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8002 "ds_parser.cpp"
    break;

  case 449:
#line 2431 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8008 "ds_parser.cpp"
    break;

  case 450:
#line 2431 "ds_parser.ypp"
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
#line 8023 "ds_parser.cpp"
    break;

  case 451:
#line 2444 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8029 "ds_parser.cpp"
    break;

  case 452:
#line 2445 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8035 "ds_parser.cpp"
    break;

  case 453:
#line 2446 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8041 "ds_parser.cpp"
    break;

  case 454:
#line 2447 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8047 "ds_parser.cpp"
    break;

  case 455:
#line 2448 "ds_parser.ypp"
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
#line 8066 "ds_parser.cpp"
    break;

  case 456:
#line 2462 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8077 "ds_parser.cpp"
    break;

  case 457:
#line 2468 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8086 "ds_parser.cpp"
    break;

  case 458:
#line 2472 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8095 "ds_parser.cpp"
    break;

  case 459:
#line 2476 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8105 "ds_parser.cpp"
    break;

  case 460:
#line 2481 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8115 "ds_parser.cpp"
    break;

  case 461:
#line 2486 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8125 "ds_parser.cpp"
    break;

  case 462:
#line 2491 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8135 "ds_parser.cpp"
    break;

  case 463:
#line 2496 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8144 "ds_parser.cpp"
    break;

  case 464:
#line 2500 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8153 "ds_parser.cpp"
    break;

  case 465:
#line 2504 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8163 "ds_parser.cpp"
    break;

  case 466:
#line 2509 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8172 "ds_parser.cpp"
    break;

  case 467:
#line 2513 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8182 "ds_parser.cpp"
    break;

  case 468:
#line 2518 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8188 "ds_parser.cpp"
    break;

  case 469:
#line 2518 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8194 "ds_parser.cpp"
    break;

  case 470:
#line 2518 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8205 "ds_parser.cpp"
    break;

  case 471:
#line 2524 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8217 "ds_parser.cpp"
    break;

  case 472:
#line 2531 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8223 "ds_parser.cpp"
    break;

  case 473:
#line 2531 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8229 "ds_parser.cpp"
    break;

  case 474:
#line 2531 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8239 "ds_parser.cpp"
    break;

  case 475:
#line 2536 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8245 "ds_parser.cpp"
    break;

  case 476:
#line 2536 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8251 "ds_parser.cpp"
    break;

  case 477:
#line 2536 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8262 "ds_parser.cpp"
    break;

  case 478:
#line 2542 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8268 "ds_parser.cpp"
    break;

  case 479:
#line 2542 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8274 "ds_parser.cpp"
    break;

  case 480:
#line 2542 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8284 "ds_parser.cpp"
    break;

  case 481:
#line 2547 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8293 "ds_parser.cpp"
    break;

  case 482:
#line 2551 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8299 "ds_parser.cpp"
    break;

  case 483:
#line 2551 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8305 "ds_parser.cpp"
    break;

  case 484:
#line 2551 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8315 "ds_parser.cpp"
    break;

  case 485:
#line 2556 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8321 "ds_parser.cpp"
    break;

  case 486:
#line 2556 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8327 "ds_parser.cpp"
    break;

  case 487:
#line 2556 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8341 "ds_parser.cpp"
    break;

  case 488:
#line 2565 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8350 "ds_parser.cpp"
    break;

  case 489:
#line 2569 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8356 "ds_parser.cpp"
    break;

  case 490:
#line 2569 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8362 "ds_parser.cpp"
    break;

  case 491:
#line 2569 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8372 "ds_parser.cpp"
    break;

  case 492:
#line 2574 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8378 "ds_parser.cpp"
    break;

  case 493:
#line 2574 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8384 "ds_parser.cpp"
    break;

  case 494:
#line 2574 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8398 "ds_parser.cpp"
    break;

  case 495:
#line 2583 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8407 "ds_parser.cpp"
    break;

  case 496:
#line 2587 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8413 "ds_parser.cpp"
    break;

  case 497:
#line 2587 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8419 "ds_parser.cpp"
    break;

  case 498:
#line 2587 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8429 "ds_parser.cpp"
    break;

  case 499:
#line 2592 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8435 "ds_parser.cpp"
    break;

  case 500:
#line 2592 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8441 "ds_parser.cpp"
    break;

  case 501:
#line 2592 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8455 "ds_parser.cpp"
    break;

  case 502:
#line 2601 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8461 "ds_parser.cpp"
    break;

  case 503:
#line 2601 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8467 "ds_parser.cpp"
    break;

  case 504:
#line 2601 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8478 "ds_parser.cpp"
    break;

  case 505:
#line 2607 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8484 "ds_parser.cpp"
    break;

  case 506:
#line 2607 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8490 "ds_parser.cpp"
    break;

  case 507:
#line 2607 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8501 "ds_parser.cpp"
    break;

  case 508:
#line 2616 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8507 "ds_parser.cpp"
    break;

  case 509:
#line 2616 "ds_parser.ypp"
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
#line 8524 "ds_parser.cpp"
    break;

  case 510:
#line 2631 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8530 "ds_parser.cpp"
    break;

  case 511:
#line 2631 "ds_parser.ypp"
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
#line 8551 "ds_parser.cpp"
    break;

  case 512:
#line 2651 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8557 "ds_parser.cpp"
    break;

  case 513:
#line 2652 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8563 "ds_parser.cpp"
    break;

  case 514:
#line 2653 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8569 "ds_parser.cpp"
    break;

  case 515:
#line 2654 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8575 "ds_parser.cpp"
    break;

  case 516:
#line 2658 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8587 "ds_parser.cpp"
    break;

  case 517:
#line 2665 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8599 "ds_parser.cpp"
    break;

  case 518:
#line 2672 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8610 "ds_parser.cpp"
    break;

  case 519:
#line 2678 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8621 "ds_parser.cpp"
    break;

  case 520:
#line 2687 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8631 "ds_parser.cpp"
    break;

  case 521:
#line 2692 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8640 "ds_parser.cpp"
    break;

  case 522:
#line 2699 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8646 "ds_parser.cpp"
    break;

  case 523:
#line 2700 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8652 "ds_parser.cpp"
    break;

  case 524:
#line 2704 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8663 "ds_parser.cpp"
    break;

  case 525:
#line 2710 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8675 "ds_parser.cpp"
    break;

  case 526:
#line 2717 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8688 "ds_parser.cpp"
    break;

  case 527:
#line 2725 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8700 "ds_parser.cpp"
    break;

  case 528:
#line 2732 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8713 "ds_parser.cpp"
    break;

  case 529:
#line 2740 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8727 "ds_parser.cpp"
    break;

  case 530:
#line 2752 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8735 "ds_parser.cpp"
    break;

  case 531:
#line 2755 "ds_parser.ypp"
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
#line 8751 "ds_parser.cpp"
    break;

  case 532:
#line 2769 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8762 "ds_parser.cpp"
    break;

  case 533:
#line 2778 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8768 "ds_parser.cpp"
    break;

  case 534:
#line 2779 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8774 "ds_parser.cpp"
    break;

  case 535:
#line 2783 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8784 "ds_parser.cpp"
    break;

  case 536:
#line 2788 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8793 "ds_parser.cpp"
    break;

  case 537:
#line 2795 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8803 "ds_parser.cpp"
    break;

  case 538:
#line 2800 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8815 "ds_parser.cpp"
    break;

  case 539:
#line 2810 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8825 "ds_parser.cpp"
    break;

  case 540:
#line 2815 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8834 "ds_parser.cpp"
    break;

  case 541:
#line 2822 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8848 "ds_parser.cpp"
    break;

  case 542:
#line 2834 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8854 "ds_parser.cpp"
    break;

  case 543:
#line 2835 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8860 "ds_parser.cpp"
    break;

  case 544:
#line 2839 "ds_parser.ypp"
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
#line 8883 "ds_parser.cpp"
    break;

  case 545:
#line 2857 "ds_parser.ypp"
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
#line 8906 "ds_parser.cpp"
    break;


#line 8910 "ds_parser.cpp"

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
#line 2877 "ds_parser.ypp"


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
