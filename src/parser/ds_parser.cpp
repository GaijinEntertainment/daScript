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
#define YYLAST   7450

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  186
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  173
/* YYNRULES -- Number of rules.  */
#define YYNRULES  531
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  939

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
       0,   445,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   461,   476,   477,   481,   485,
     490,   499,   507,   523,   528,   536,   536,   566,   588,   592,
     595,   601,   610,   631,   654,   655,   659,   663,   664,   668,
     671,   677,   683,   686,   692,   693,   697,   698,   699,   708,
     709,   713,   722,   738,   746,   756,   765,   766,   767,   768,
     769,   770,   774,   779,   787,   788,   789,   790,   791,   792,
     793,   794,   800,   813,   829,   830,   831,   835,   842,   854,
     858,   865,   866,   870,   871,   872,   876,   881,   888,   892,
     893,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   909,   910,   911,   912,
     913,   914,   915,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,   926,   927,   928,   929,   930,   931,   932,
     933,   937,   959,   997,  1004,  1008,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,  1034,  1035,  1036,  1037,  1041,  1046,  1052,  1058,  1092,
    1095,  1101,  1102,  1113,  1117,  1123,  1126,  1129,  1133,  1139,
    1143,  1147,  1150,  1153,  1158,  1161,  1169,  1172,  1177,  1180,
    1188,  1194,  1195,  1199,  1233,  1233,  1233,  1236,  1236,  1236,
    1241,  1241,  1241,  1249,  1249,  1249,  1255,  1265,  1276,  1291,
    1294,  1300,  1301,  1308,  1319,  1320,  1321,  1325,  1326,  1327,
    1328,  1332,  1337,  1345,  1346,  1350,  1355,  1419,  1420,  1421,
    1422,  1423,  1424,  1428,  1429,  1430,  1431,  1432,  1433,  1434,
    1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,
    1445,  1446,  1450,  1451,  1452,  1453,  1457,  1468,  1473,  1483,
    1487,  1487,  1487,  1494,  1494,  1494,  1508,  1512,  1512,  1512,
    1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,
    1529,  1530,  1531,  1532,  1533,  1534,  1535,  1536,  1537,  1538,
    1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,  1547,  1548,
    1549,  1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,
    1559,  1560,  1561,  1562,  1566,  1570,  1573,  1576,  1577,  1578,
    1579,  1588,  1597,  1598,  1601,  1601,  1601,  1604,  1608,  1612,
    1616,  1617,  1618,  1619,  1620,  1621,  1622,  1623,  1639,  1655,
    1710,  1711,  1715,  1716,  1720,  1728,  1731,  1736,  1735,  1780,
    1836,  1845,  1850,  1858,  1859,  1863,  1866,  1875,  1876,  1880,
    1889,  1890,  1895,  1896,  1900,  1906,  1912,  1915,  1919,  1925,
    1934,  1935,  1936,  1940,  1941,  1945,  1948,  1953,  1958,  1966,
    1977,  1980,  1987,  1988,  1992,  1993,  1994,  1998,  2026,  2026,
    2056,  2059,  2068,  2081,  2093,  2094,  2098,  2102,  2113,  2128,
    2129,  2133,  2170,  2171,  2175,  2175,  2259,  2266,  2275,  2276,
    2277,  2278,  2279,  2280,  2281,  2282,  2283,  2284,  2285,  2286,
    2287,  2288,  2289,  2290,  2291,  2292,  2293,  2294,  2295,  2296,
    2297,  2298,  2299,  2303,  2304,  2305,  2306,  2307,  2308,  2312,
    2323,  2327,  2337,  2344,  2353,  2353,  2353,  2366,  2367,  2368,
    2369,  2370,  2384,  2390,  2394,  2398,  2403,  2408,  2413,  2418,
    2422,  2426,  2431,  2435,  2440,  2440,  2440,  2446,  2453,  2453,
    2453,  2458,  2458,  2458,  2464,  2464,  2464,  2469,  2473,  2473,
    2473,  2478,  2478,  2478,  2487,  2491,  2491,  2491,  2496,  2496,
    2496,  2505,  2509,  2509,  2509,  2514,  2514,  2514,  2523,  2523,
    2523,  2529,  2529,  2529,  2538,  2538,  2553,  2553,  2573,  2574,
    2575,  2576,  2580,  2587,  2594,  2600,  2609,  2614,  2621,  2622,
    2626,  2632,  2639,  2647,  2654,  2662,  2674,  2677,  2691,  2700,
    2701,  2705,  2710,  2717,  2722,  2732,  2737,  2744,  2756,  2757,
    2761,  2779
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
     409,   410,   411,   412,    46,   413,    91,    93,    40,    41,
      36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -609

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-609)))

#define YYTABLE_NINF -486

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -609,    25,  -609,  -609,   -88,   -46,   -18,    -5,  -122,  -609,
      32,  -609,  -609,    69,  -609,  -609,  -609,  -609,  -609,   333,
    -609,   151,  -609,  -609,  -609,  -609,  -609,  -609,    43,  -609,
      76,    66,    93,  -609,  -609,    20,  -609,   -31,   144,   242,
     172,   183,  -609,  -609,   187,   212,   170,  -609,    56,  -609,
    -609,  -609,    44,   205,  -609,  -609,   175,    12,   -88,   226,
     -46,   221,  -609,   224,   225,  -609,   191,  -609,   218,  -609,
     -90,  -609,  -609,   189,   192,  -609,   231,   -88,    69,  -609,
    7281,   345,   346,  -609,   204,   207,  -609,   232,  -609,  -609,
     243,  -609,  -609,  -609,  -609,  -609,   245,    60,  -609,  -609,
    -609,  -609,   341,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,   214,   -83,  7194,  -609,  -609,   254,   255,  -609,    -6,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,   -91,   248,  -609,  -609,   113,  -609,   223,
     -66,  -609,  -609,   184,  -609,   115,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,    23,  -609,  -609,   -39,  -609,   253,   256,
     264,   265,  -609,  -609,  -609,   236,  -609,  -609,  -609,  -609,
    -609,   268,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,   273,  -609,  -609,  -609,   274,   275,  -609,  -609,  -609,
    -609,   277,   283,  -609,  -609,  -609,  -609,  -609,   220,  -609,
     219,   289,  -609,   222,  -609,   -88,  -609,    -4,  -609,   122,
    7194,  -609,  1181,  -609,  -609,  -609,  -609,   305,  7194,  -609,
     -34,  -609,  -609,    60,  -609,   -63,  4484,  -609,  -609,  -609,
    -609,  -609,   310,  -609,    96,   104,   148,  -609,  -609,  -609,
    -609,  -609,  -609,   424,  -609,  -609,     7,  2858,  -609,  -609,
     -58,  7194,   -53,    73,   243,  -609,   -42,  -609,   321,  3210,
    -609,  4484,  -609,  -609,   243,  -609,  -609,  7108,   281,   303,
    1992,  -609,  -609,   207,  4484,   285,  4484,   312,   320,   291,
    -609,   282,   324,   347,  2996,   207,  -609,   326,  2150,  4484,
    4484,   164,   164,  6936,  7022,  4484,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  4484,  4484,  4484,   113,  4484,  4484,  4484,
    -609,   299,  -609,   452,  -609,  -609,   309,   317,  4484,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,   -79,   319,  -609,
     322,   325,  -609,  -609,  -609,   243,  -609,  -609,  -609,  -609,
     327,  -609,  -609,   -75,  -609,  -609,  -609,  -609,  -609,  6127,
     330,  -609,  -609,  -609,  -609,  -609,    11,  -609,   864,  -609,
    -609,  -609,  2288,  -609,  -609,   961,  7194,  7194,  7194,  7194,
     323,   254,  7194,   204,  7194,   204,  7194,   204,  7280,   255,
    -609,  -609,  -609,   332,  -609,  -609,  5789,  -609,  -609,  3210,
    -609,  -609,   334,  -609,  -609,  -609,  7194,  -609,  2288,  -609,
     812,     9,   512,  -609,   113,  -609,  2288,  -609,  -609,   394,
    4573,   501,  6448,  4484,   812,  -609,  -609,  4484,  -609,  -609,
     362,   380,  6448,  -609,  7194,  2288,  -609,  4672,    63,    63,
     339,  -609,  -609,   243,  1524,   243,  1680,  6226,  -609,    91,
     206,    63,    63,   -68,  -609,    63,    63,  5241,    19,   338,
    -609,  -609,   812,  -609,  2426,  -609,  -609,  -609,  -609,   386,
     164,  -609,    14,   378,  4484,  4484,  4484,  4484,  4484,  4484,
    4484,  4484,  4484,  4484,  -609,  -609,  4484,  4484,  4484,  4484,
    4484,  4484,   379,  2582,  4484,   381,  4484,  4484,  4484,  4484,
    4484,  4484,  4484,  4484,  4484,  4484,  4484,  4484,  4484,  4484,
    2720,  3134,  4484,  4484,  4484,  4484,  4484,  4484,  4484,  4484,
    4484,  4484,   382,  4484,  3272,    69,  -609,   522,   488,   349,
     424,  -609,  2288,  -609,  4771,  -609,   441,   581,   987,  1123,
    -609,   110,  1467,   248,  1618,   248,  1774,   248,    71,  -609,
     123,  3210,   142,  -609,  -609,  -609,  1934,  -609,  6027,  -609,
    4484,  3428,  3584,  7194,  -609,  4870,  4484,  4484,  -609,   207,
    5318,  -609,  7194,  7194,  5398,  7194,  -609,  -609,  2091,  -609,
    4969,  -609,  -609,    10,   164,   -38,  1836,  3722,  6226,   383,
     -16,   356,   384,  -609,  -609,   136,    15,  3878,   -16,   132,
    4484,  4484,   350,  -609,  4484,   390,   391,  -609,   152,  -609,
    -609,   249,   395,  -609,    45,  6448,   -55,   204,  -609,   388,
    -609,  -609,  6448,  6448,  6448,  6448,  6448,  6448,  6448,  6448,
     246,   246,   313,  6448,  6448,   313,   539,   539,   361,  4484,
    4484,  6448,    17,  -609,  5864,   -70,   -70,  6448,   246,   246,
    6448,  6448,  6670,  6522,  6596,  6448,  6448,  6448,  4484,  4484,
    6448,   396,  6300,  6695,  6769,  6794,   313,   313,   835,   835,
      17,    17,    17,  -609,   542,  5939,  -609,    64,    77,    27,
    -609,   243,  -609,  -609,  5068,  -609,  -609,  7194,  -609,  -609,
    -609,   400,  -609,   389,  -609,   392,  -609,   393,  7194,  -609,
    7280,  -609,   255,   424,  4484,   -85,  -609,    65,   403,  5475,
    2226,  -609,  -609,  -609,  2364,  2520,  -609,  2658,   386,  -609,
    4484,  -609,  4484,  4484,    -9,   405,   395,   374,   377,  4484,
     385,  4484,  4484,   395,   371,   387,  6448,  -609,  -609,  6374,
    -609,   410,  7194,   204,  1365,   207,  -609,  -609,  4484,  -609,
     -33,    78,  4484,  -609,   413,   416,   420,   421,  -609,    88,
     248,  -609,  4034,   336,  6448,  -609,   336,  6448,  -609,  4484,
    -609,  -609,  -609,   399,  -609,    44,  -609,  -609,  -609,  2796,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  3210,
    -609,  -609,  -609,  6448,   207,  -609,   149,  -609,  -609,  -609,
    -609,  -609,   397,   -40,  6448,  6448,   -16,   401,   -28,   383,
     406,  -609,  6448,  -609,  -609,    36,   -16,   407,  -609,  -609,
    -609,  2934,   248,  -609,  -609,   812,   408,  6448,  -609,  -609,
    -609,  -609,   -55,   411,   -78,  7194,  -609,    70,  6448,  -609,
    -609,    44,  -609,  -609,  -609,  -609,  -609,  -609,  4484,   422,
    -609,  4484,  4484,  4484,  4190,  4484,   412,   414,  4484,  4484,
    -609,  4484,   409,  -609,  -609,   425,   249,  -609,  -609,  -609,
    4346,  -609,  -609,  3072,  -609,   415,  -609,  5555,   438,   313,
     313,   313,  -609,  5632,  5167,   426,  -609,  6448,  6448,  5167,
     431,   113,  -609,  -609,  4484,  6448,  -609,  -609,  -609,  4484,
    -609,   523,   433,  -609,   428,  -609,  -609,   113,  6448,  -609,
    5712,  4484,   437,   439,  -609,  -609,  6448,  -609,  -609
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    81,     1,   181,     0,     0,     0,     0,     0,   182,
       0,   496,   494,     0,    14,     3,    10,     9,     8,     0,
       7,   374,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,   372,
       0,     0,    75,    76,     0,   161,    77,    79,     0,    74,
     393,   392,     0,     0,   375,   376,   378,     0,     0,     0,
       0,     0,    35,     0,     0,    32,     0,   380,     0,   384,
       0,   373,    15,     0,     0,   163,     0,     0,     0,    82,
       0,     0,     0,    88,    83,     0,   131,   389,   394,   370,
       0,    68,    69,    66,    67,    65,     0,     0,    64,    73,
      40,    38,    34,    31,    30,   423,   426,   424,   427,   425,
     428,     0,     0,     0,   386,   385,     0,     0,   162,     0,
      80,    91,    92,    94,    93,    95,    96,    97,    98,   123,
     124,   121,   122,   114,   125,   126,   115,   112,   113,   127,
     128,   129,   130,   102,   103,   104,    99,   100,   101,   117,
     118,   116,   110,   111,   106,   105,   107,   108,   109,    90,
      89,   119,   120,   330,    86,   155,   133,     0,   391,     0,
       0,   396,   379,   363,    16,     0,    60,    61,    58,    59,
      57,    56,    62,     0,    33,   380,     0,   387,     0,     0,
       0,     0,   398,   418,   399,   430,   400,   404,   405,   406,
     407,   422,   411,   412,   413,   414,   415,   416,   417,   419,
     420,   467,   403,   410,   421,   474,   481,   401,   408,   402,
     409,     0,     0,   429,   437,   440,   438,   439,     0,   432,
       0,     0,   350,     0,    78,     0,    84,     0,   343,     0,
       0,   132,     0,   390,   335,   377,   371,     0,     0,   364,
       0,    17,    18,     0,    71,     0,     0,   381,   458,   461,
     464,   454,     0,   434,   468,   475,   482,   488,   491,   445,
     450,   444,   457,     0,   453,   447,     0,     0,   383,   449,
       0,     0,     0,     0,     0,   341,   354,    85,   330,    87,
     157,     0,    49,    50,     0,   266,   267,     0,     0,     0,
       0,   260,   169,     0,     0,     0,     0,     0,     0,     0,
     170,     0,     0,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   219,   218,   220,
     221,   222,    19,     0,     0,     0,     0,     0,     0,     0,
     204,   205,   136,   134,   264,   263,     0,     0,     0,   150,
     145,   143,   142,   144,   215,   156,   137,   261,     0,   323,
       0,     0,   148,   149,   151,     0,   141,   322,   321,   320,
      81,   326,   262,     0,   138,   325,   324,   302,   268,   223,
       0,   265,   498,   499,   500,   501,   330,   397,     0,   361,
     362,   360,     0,    63,   388,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,    83,     0,    83,     0,     0,
     452,   446,   448,     0,   451,   442,     0,   433,   497,   349,
     495,   351,     0,   342,   353,   352,     0,   355,     0,   344,
       0,     0,   165,   168,     0,   193,     0,   171,   174,     0,
     223,     0,   164,     0,     0,   184,   187,     0,   154,   190,
       0,     0,    43,    53,     0,     0,   178,   223,   294,   295,
     205,   159,   160,     0,   508,     0,     0,     0,   525,     0,
       0,   272,   271,   307,    25,   270,   269,     0,   206,     0,
     152,   153,     0,   329,     0,   140,   146,   147,   183,   213,
       0,   139,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,     0,     0,     0,   395,     0,   332,     0,
     360,   365,     0,   369,   223,   382,     0,     0,     0,     0,
     431,     0,     0,    86,     0,    86,     0,    86,   161,   347,
       0,   345,     0,   443,   441,   331,   356,   359,   358,    54,
       0,     0,     0,     0,   175,   223,     0,     0,   172,     0,
       0,    55,     0,     0,     0,     0,    41,    42,     0,   179,
     223,   176,   206,     0,     0,   161,     0,     0,   516,   506,
     508,     0,   519,   520,   521,     0,     0,     0,   508,     0,
       0,     0,     0,    22,     0,    20,     0,   298,   250,   249,
     155,    46,     0,   303,     0,   199,     0,    83,   158,     0,
     317,   318,   233,   234,   236,   235,   237,   227,   228,   229,
     273,   274,   286,   238,   239,   287,   284,   285,     0,     0,
       0,   225,   312,   301,     0,   327,   328,   226,   275,   276,
     240,   241,   291,   292,   293,   230,   231,   232,     0,     0,
     224,     0,     0,   289,   290,   288,   282,   283,   278,   277,
     279,   280,   281,   256,     0,     0,   305,     0,     0,   332,
     333,     0,   336,   367,   223,   368,   459,     0,   465,   455,
     435,     0,   469,     0,   476,     0,   483,     0,     0,   489,
       0,   492,     0,   352,     0,     0,   166,     0,     0,     0,
       0,   173,   180,   308,     0,     0,   309,     0,   213,   177,
       0,   509,     0,     0,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   526,   527,     0,
      23,    26,     0,    83,     0,     0,    44,    45,     0,    51,
       0,     0,     0,   304,     0,     0,     0,     0,   211,     0,
      86,   314,     0,   244,   245,   300,   242,   243,   319,     0,
     258,   299,   306,    82,   337,     0,   334,   366,   460,     0,
     466,   456,   436,   470,   472,   477,   479,   484,   486,   346,
     490,   348,   493,   357,     0,   167,     0,   196,   194,   185,
     188,   191,     0,     0,   503,   502,   508,     0,     0,   507,
       0,   511,   517,   523,   522,     0,   508,     0,   524,    21,
      24,     0,    86,   135,    47,     0,     0,   200,   209,   210,
     208,   207,     0,     0,     0,     0,   247,     0,   313,   259,
     340,     0,   339,   462,   473,   480,   487,    52,     0,     0,
     195,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,     0,     0,   514,   251,     0,    46,   246,   212,   214,
       0,   201,   216,     0,   248,     0,   463,     0,     0,   186,
     189,   192,   310,     0,   528,     0,   512,   505,   504,   528,
       0,     0,   254,    48,     0,   202,   315,   338,   197,     0,
     311,     0,     0,   513,     0,   515,   252,     0,   203,   316,
       0,     0,     0,     0,   255,   198,   529,   530,   531
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -609,  -609,  -609,   101,   516,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,   472,  -609,  -609,   555,  -609,  -609,
    -609,  -310,  -609,  -609,  -609,  -609,  -609,  -609,   365,  -609,
     564,   -61,  -609,   550,    84,  -226,  -396,  -544,  -609,  -609,
    -230,  -163,   -84,  -609,     3,  -283,   -13,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,   634,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -529,  -609,  -609,
    -216,  -609,   -99,  -308,  -609,  -239,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,   286,  -609,  -609,
     261,   -59,  -609,  -609,  -609,   360,  -609,   -67,  -609,  -277,
     241,  -282,  -276,   270,  -609,  -150,  -609,  -609,  -609,  -609,
    -609,   474,   590,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -231,  -111,  -609,  -609,  -609,   266,  -609,  -609,  -609,
     -77,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,
    -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,  -609,   372,
    -608,  -454,  -588,  -609,  -609,  -315,   -89,   202,  -609,  -609,
    -609,  -238,  -609
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   175,   181,   470,   344,   761,   345,   626,
      16,    17,    35,    36,    65,    18,    32,    33,   346,   347,
     768,   769,   348,   349,   350,   351,   352,   353,   182,   183,
      29,    30,    46,    47,    48,    19,   164,   241,    84,    20,
      85,    86,   354,   355,   242,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   592,   871,   593,
     872,   595,   873,   368,   583,   870,   369,   634,   892,   370,
     778,   779,   637,   371,   372,   439,   374,   375,   376,   377,
     762,   911,   763,   927,   378,   694,   859,   635,   855,   929,
     237,   701,   549,   386,   861,   238,   239,   569,   570,   232,
     233,   743,   285,   392,   250,   172,   170,    72,    56,    22,
      90,   112,    69,    70,    23,    24,   168,    88,    53,    25,
     169,   173,   380,   111,   225,   226,   230,   227,   401,   802,
     571,   399,   801,   396,   798,   397,   896,   398,   800,   402,
     803,   403,   864,   404,   805,   405,   865,   406,   807,   407,
     866,   408,   810,   409,   812,    26,    41,    27,    40,   381,
     609,   610,   611,   382,   612,   613,   614,   615,   383,   469,
     384,   922,   385
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,   166,   224,   373,   428,   421,   286,   563,   423,   565,
     468,   567,   618,   461,   462,   697,   119,   438,   -81,   713,
     246,   715,   747,   717,   771,     2,    91,    92,   580,   740,
     754,     3,   639,   604,   752,   456,   228,   492,   493,   411,
     604,    61,   284,   512,   490,   235,   515,   516,   515,   516,
     890,   725,   727,   286,     4,    68,     5,    28,     6,   774,
       7,    38,   186,   431,     8,    49,   483,   772,   775,   700,
     794,     9,   424,    62,   176,   177,   424,    10,    80,   171,
     389,   424,   186,   492,   493,   742,   424,   417,   236,   390,
     742,    76,   231,    31,   114,   878,    11,   814,   776,   484,
     223,   187,    42,   777,   542,   165,   543,   491,   543,   553,
     247,   425,   772,   426,   256,   425,   427,    12,   245,   391,
     425,   394,   504,   505,    66,   425,   418,    34,    43,   224,
     512,   420,   514,   515,   516,   517,   770,   224,   829,   518,
      37,   171,   875,   257,   489,   577,    58,   235,    44,    81,
      82,    93,    67,   584,   243,    94,   827,    95,    96,   640,
     547,   247,   247,   289,    45,   412,   746,   247,   504,   505,
     224,   388,   599,   234,   283,   253,   512,    39,   628,   515,
     516,   517,   638,   413,    63,   518,   224,   545,   772,    83,
      97,   542,   414,   543,    64,   546,    57,   772,    44,   178,
      76,    13,   254,   179,   419,   180,    96,    14,    78,    54,
      55,   823,   224,   224,    45,   488,   772,   772,   881,   441,
     432,    59,   772,   835,   773,    58,   718,   223,    58,    78,
     745,   453,   603,    79,   616,   223,   854,   542,   876,   543,
     852,   780,    44,   792,   815,    60,   464,   466,   882,   894,
     422,  -471,   269,   857,   793,   846,  -471,   105,    45,  -478,
     765,   106,   251,   252,  -478,   853,   492,   493,   223,   703,
     710,   766,   767,   621,  -471,   622,   270,   271,   107,   108,
     109,   110,  -478,   719,   223,   224,   224,   224,   224,    68,
     826,   224,   711,   224,   724,   224,   741,   224,   885,   836,
      71,   287,   721,  -485,   288,   720,   757,  -253,  -485,   868,
     223,   223,  -253,   750,   751,   224,   755,    73,   751,   556,
     557,   558,   559,   474,   722,   562,  -485,   564,    74,   566,
    -253,   869,    75,   492,   493,   272,   247,    50,    51,   248,
      52,    76,   249,   224,   340,   460,   579,   165,    77,   576,
      87,   504,   505,   174,   623,   624,   492,   493,    89,   512,
     591,   514,   515,   516,   517,   100,   102,   842,   518,   103,
     104,   113,   116,   273,   274,   117,   118,   598,   275,   161,
     162,   276,   163,   223,   223,   223,   223,   167,   171,   223,
     165,   223,   174,   223,    62,   223,   277,   185,   631,   229,
     231,   280,   278,   240,   282,   279,   244,   537,   538,   539,
     540,   541,   258,   223,   262,   259,   502,   503,   504,   505,
     542,   582,   543,   260,   261,   796,   512,   263,   514,   515,
     516,   517,   264,   265,   266,   518,   267,   520,   521,   502,
     503,   223,   268,   506,   281,   421,   509,   510,   511,   512,
     387,   514,   515,   516,   517,   400,   410,   235,   518,   434,
     520,   521,   435,   443,   448,   629,   524,   525,   526,   447,
     286,   445,   224,   269,   537,   538,   539,   540,   541,   446,
     478,   224,   224,   449,   224,   454,   450,   542,   479,   543,
     531,   480,   532,   533,   534,   535,   536,   270,   271,   481,
     540,   485,   560,    13,   486,   732,   730,   487,   544,   573,
     542,   575,   543,   490,   589,   734,   735,   596,   737,   597,
     602,   630,   636,   641,   658,   373,   663,   693,   379,   699,
     700,   702,    49,   748,   758,   745,   749,   251,   760,   782,
     770,   788,   395,   790,   269,   417,   879,   781,   816,   804,
     828,   830,   806,   808,   831,   837,   272,   840,   848,   492,
     493,   849,   833,   416,   838,   850,   851,   898,   270,   271,
     223,   625,   931,    98,   184,   874,   913,   430,   877,   223,
     223,   860,   223,   880,   883,   912,   440,   887,   889,   905,
     442,   906,   444,   910,   273,   274,   224,   917,   919,   275,
     452,   706,   276,   923,   457,   458,   459,   224,   925,   224,
     932,   467,   933,   269,   937,   101,   938,   277,   393,   471,
     472,   473,    99,   475,   476,   477,   279,   272,   120,   698,
     799,   895,   862,   764,   482,    21,   888,   270,   271,   822,
     795,   809,   502,   503,   504,   505,   506,   548,   429,   509,
     572,   224,   512,   811,   514,   515,   516,   517,   552,   255,
     115,   518,   834,   520,   521,   273,   274,   561,   619,   433,
     275,   924,     0,   276,     0,     0,     0,     0,   554,     0,
       0,   844,     0,     0,     0,   841,     0,     0,   277,     0,
     581,     0,     0,     0,   223,     0,   272,   279,   535,   536,
     537,   538,   539,   540,   541,   223,     0,   223,     0,     0,
       0,     0,     0,   542,   578,   543,     0,     0,     0,     0,
       0,     0,   585,     0,     0,     0,     0,     0,     0,   590,
     867,     0,     0,   594,   273,   274,     0,     0,     0,   275,
       0,   600,   276,     0,   224,     0,     0,     0,     0,   223,
     608,     0,   608,     0,     0,     0,     0,   277,     0,     0,
       0,   886,     0,   707,     0,     0,   279,     0,     0,     0,
     891,     0,     0,     0,     0,     0,     0,     0,   893,     0,
     642,   643,   644,   645,   646,   647,   648,   649,   650,   651,
       0,     0,   652,   653,   654,   655,   656,   657,     0,   661,
     662,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   680,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,   692,     0,   695,
       0,     0,   492,   493,     0,     0,     0,     0,   704,     0,
       0,     0,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   492,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,     0,
       0,     0,   661,   680,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   471,   477,     0,     0,   269,     0,   926,     0,
       0,     0,     0,   477,     0,     0,   756,   467,     0,     0,
     759,     0,     0,     0,   934,   502,   503,   504,   505,   506,
     270,   271,   509,   510,   511,   512,     0,   514,   515,   516,
     517,     0,     0,     0,   518,     0,   520,   521,     0,     0,
     504,   505,   524,   525,   526,   783,   784,     0,   512,     0,
     514,   515,   516,   517,     0,     0,     0,   518,     0,     0,
       0,     0,     0,     0,   786,   787,   531,     0,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   389,   272,
       0,   492,   493,     0,     0,     0,   542,   390,   543,     0,
       0,     0,     0,     0,     0,   165,     0,     0,   539,   540,
     541,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     813,   543,     0,     0,     0,     0,     0,   550,   274,   269,
       0,     0,   275,     0,     0,   276,     0,     0,   824,   825,
       0,     0,     0,     0,     0,   832,     0,   608,     0,     0,
     277,     0,     0,   270,   271,     0,   551,     0,     0,   279,
     379,     0,     0,     0,   845,     0,     0,     0,   847,     0,
       0,     0,     0,     0,   502,   503,   504,   505,   506,     0,
       0,   509,   510,   511,   512,   858,   514,   515,   516,   517,
       0,     0,     0,   518,     0,   520,   521,     0,     0,     0,
       0,   524,   525,   526,     0,     0,     0,     0,     0,     0,
       0,     0,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   531,     0,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,   543,     0,     0,
     273,   274,     0,   555,     0,   275,     0,   708,   276,     0,
       0,     0,     0,     0,   897,   269,     0,   899,   900,   901,
     903,   904,     0,   277,   907,   908,     0,   909,     0,     0,
       0,     0,   279,     0,     0,     0,   915,     0,     0,   270,
     271,     0,   290,     0,     0,     0,     0,     3,     0,   291,
     292,   293,     0,   294,     0,   295,   296,   297,   298,   299,
     928,     0,     0,     0,     0,   930,   300,   301,   302,   303,
       0,     0,     0,     0,     0,     0,     0,   936,   304,   305,
       0,   306,   307,     0,     0,   308,     0,     9,   309,   310,
       0,   311,   312,     0,     0,   313,   314,     0,   272,     0,
       0,     0,   315,   192,   193,   194,     0,   196,   197,   198,
     199,   200,   316,   202,   203,   204,   205,   206,   207,   208,
     209,   210,     0,   212,   213,   214,     0,     0,   217,   218,
     219,   220,     0,     0,   317,   318,   273,   274,     0,     0,
       0,   275,     0,   709,   276,     0,   319,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   321,   322,     0,     0,     0,     0,     0,   279,     0,
      44,     0,     0,     0,     0,     0,     0,   323,   324,   325,
     326,   327,   328,   329,   330,   331,    45,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   333,   334,   335,     0,   336,     0,     0,   337,
     338,     0,     0,     0,     0,     0,     0,     0,     0,   339,
       0,   340,   341,   342,   165,   343,   290,     0,     0,     0,
       0,     3,     0,   291,   292,   293,     0,   294,     0,   295,
     296,   297,   298,   299,     0,     0,     0,     0,     0,     0,
     300,   301,   302,   303,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,     0,   306,   307,     0,     0,   308,
       0,     9,   309,   310,     0,   311,   312,     0,     0,   313,
     314,     0,     0,     0,     0,     0,   315,   192,   193,   194,
       0,   196,   197,   198,   199,   200,   316,   202,   203,   204,
     205,   206,   207,   208,   209,   210,     0,   212,   213,   214,
       0,     0,   217,   218,   219,   220,     0,     0,   317,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,   322,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,     0,     0,   269,
       0,   323,   324,   325,   326,   327,   328,   329,   330,   331,
      45,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   271,     0,   333,   334,   335,     0,
     336,     0,     0,   337,   338,     0,     0,     0,   295,   296,
     297,   298,   299,   339,     0,   340,   341,   342,   165,   843,
     301,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,   305,     0,     0,   307,     0,     0,   308,     0,
       0,   309,     0,   604,     0,   312,     0,     0,     0,     0,
     270,   271,   272,     0,     0,     0,   192,   193,   194,     0,
     196,   197,   198,   199,   200,   316,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,   213,   214,     0,
       0,   217,   218,   219,   220,     0,     0,   317,     0,     0,
     273,   274,     0,     0,     0,   275,     0,   712,   276,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,   272,
       0,     0,     0,   277,     0,     0,     0,     0,     0,     0,
     269,     0,   279,    44,     0,     0,     0,     0,     0,     0,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   605,
     332,     0,     0,     0,   270,   271,     0,   273,   274,     0,
       0,     0,   275,     0,     0,   606,   334,   335,     0,   336,
       0,     0,   337,   338,   295,   296,   297,   298,   299,     0,
     277,     0,   607,     0,   340,   341,   301,   165,     0,   279,
       0,     0,   269,     0,     0,     0,     0,     0,   305,     0,
       0,   307,     0,     0,   308,     0,     0,   309,     0,     0,
       0,   312,     0,   272,     0,     0,   270,   271,     0,     0,
       0,     0,   192,   193,   194,     0,   196,   197,   198,   199,
     200,   316,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,   212,   213,   214,     0,     0,   217,   218,   219,
     220,   273,   274,   317,     0,     0,   275,     0,   714,   276,
       0,     0,     0,     0,     0,   319,   320,     0,     0,     0,
       0,     0,     0,     0,   277,   272,     0,     0,     0,     0,
       0,     0,     0,   279,     0,     0,   269,     0,     0,    44,
       0,     0,     0,     0,     0,     0,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   605,   332,     0,     0,     0,
     270,   271,     0,   273,   274,     0,     0,     0,   275,     0,
       0,   606,   334,   335,     0,   336,     0,     0,   337,   338,
     295,   296,   297,   298,   299,     0,   277,     0,   617,     0,
     340,   341,   301,   165,     0,   279,     0,     0,   411,     0,
       0,     0,     0,     0,   305,     0,     0,   307,     0,     0,
     308,     0,     0,   309,     0,     0,     0,   312,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,   192,   193,
     194,     0,   196,   197,   198,   199,   200,   316,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,   212,   213,
     214,     0,     0,   217,   218,   219,   220,   273,   274,   317,
       0,     0,   275,     0,   716,   276,     0,     0,     0,     0,
       0,   319,   320,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,   279,
       0,     0,     0,     0,     0,    44,   269,     0,     0,     0,
       0,     0,   323,   324,   325,   326,   327,   328,   329,   330,
     331,    45,   332,     0,     0,     0,     0,     0,     0,     0,
     270,   271,     0,     0,   412,     0,     0,   333,   334,   335,
       0,   336,     0,     0,   337,   338,   295,   296,   297,   298,
     299,     0,   413,     0,   339,     0,   340,   341,   301,   165,
       0,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,   307,     0,     0,   308,     0,     0,   309,
       0,     0,     0,   312,     0,     0,     0,     0,   424,   272,
       0,     0,     0,     0,   192,   193,   194,     0,   196,   197,
     198,   199,   200,   316,   202,   203,   204,   205,   206,   207,
     208,   209,   210,     0,   212,   213,   214,     0,     0,   217,
     218,   219,   220,     0,     0,   317,     0,   723,   274,     0,
       0,     0,   275,     0,     0,   276,     0,   319,   320,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
     277,     0,   321,   322,     0,     0,     0,     0,     0,   279,
       0,    44,     0,   269,     0,     0,     0,     0,   323,   324,
     325,   326,   327,   328,   329,   330,   331,    45,   332,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,     0,
       0,     0,     0,   333,   334,   335,     0,   336,     0,     0,
     337,   338,     0,     0,   295,   296,   297,   298,   299,     0,
     339,     0,   340,   341,   437,   165,   301,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,     0,
       0,   307,     0,     0,   308,     0,     0,   309,     0,     0,
       0,   312,     0,     0,     0,     0,   272,     0,     0,     0,
       0,     0,   192,   193,   194,     0,   196,   197,   198,   199,
     200,   316,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,   212,   213,   214,     0,     0,   217,   218,   219,
     220,     0,     0,   317,   273,   274,     0,     0,     0,   275,
       0,   738,   276,     0,     0,   319,   320,     0,   269,     0,
       0,     0,     0,     0,   455,     0,     0,   277,     0,     0,
     321,   322,     0,     0,     0,     0,   279,     0,     0,    44,
       0,     0,   270,   271,     0,     0,   323,   324,   325,   326,
     327,   328,   329,   330,   331,    45,   332,     0,     0,     0,
       0,     0,   295,   296,   297,   298,   299,     0,     0,     0,
       0,   333,   334,   335,   301,   336,     0,     0,   337,   338,
       0,     0,     0,     0,     0,     0,   305,     0,   339,   307,
     340,   341,   308,   165,     0,   309,     0,     0,     0,   312,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
     192,   193,   194,     0,   196,   197,   198,   199,   200,   316,
     202,   203,   204,   205,   206,   207,   208,   209,   210,     0,
     212,   213,   214,     0,     0,   217,   218,   219,   220,   273,
     274,   317,     0,     0,   275,     0,   818,   276,     0,     0,
       0,     0,     0,   319,   320,     0,   269,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,   321,   322,
       0,   279,     0,     0,     0,     0,     0,    44,     0,     0,
     270,   271,     0,     0,   323,   324,   325,   326,   327,   328,
     329,   330,   331,    45,   332,     0,     0,     0,     0,     0,
     295,   296,   297,   298,   299,     0,     0,     0,     0,   333,
     334,   335,   301,   336,     0,     0,   337,   338,     0,     0,
       0,     0,     0,     0,   305,     0,   339,   307,   340,   341,
     308,   165,     0,   309,     0,     0,     0,   312,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,   192,   193,
     194,     0,   196,   197,   198,   199,   200,   316,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,   212,   213,
     214,     0,     0,   217,   218,   219,   220,   273,   274,   317,
       0,     0,   275,     0,   819,   276,     0,     0,     0,     0,
       0,   319,   320,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,   279,
       0,     0,   269,     0,     0,    44,     0,     0,     0,     0,
       0,     0,   323,   324,   325,   326,   327,   328,   329,   330,
     331,    45,   332,     0,     0,     0,   270,   271,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   333,   334,   335,
       0,   336,     0,     0,   337,   338,   295,   296,   297,   298,
     299,     0,   632,     0,   339,   633,   340,   341,   301,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,   307,     0,     0,   308,     0,     0,   309,
       0,     0,     0,   312,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,   192,   193,   194,     0,   196,   197,
     198,   199,   200,   316,   202,   203,   204,   205,   206,   207,
     208,   209,   210,     0,   212,   213,   214,     0,     0,   217,
     218,   219,   220,   273,   274,   317,     0,     0,   275,     0,
     820,   276,     0,     0,     0,     0,     0,   319,   320,     0,
     269,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,   659,   660,     0,   279,     0,     0,     0,     0,
       0,    44,     0,     0,   270,   271,     0,     0,   323,   324,
     325,   326,   327,   328,   329,   330,   331,    45,   332,     0,
       0,     0,     0,     0,   295,   296,   297,   298,   299,     0,
       0,     0,     0,   333,   334,   335,   301,   336,     0,     0,
     337,   338,     0,     0,     0,     0,     0,     0,   305,     0,
     339,   307,   340,   341,   308,   165,     0,   309,     0,     0,
       0,   312,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,   192,   193,   194,     0,   196,   197,   198,   199,
     200,   316,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,   212,   213,   214,     0,     0,   217,   218,   219,
     220,   273,   274,   317,     0,     0,   275,     0,   821,   276,
       0,     0,     0,     0,     0,   319,   320,     0,   269,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
     678,   679,     0,   279,     0,     0,     0,     0,     0,    44,
       0,     0,   270,   271,     0,     0,   323,   324,   325,   326,
     327,   328,   329,   330,   331,    45,   332,     0,     0,     0,
       0,     0,   295,   296,   297,   298,   299,     0,     0,     0,
       0,   333,   334,   335,   301,   336,     0,     0,   337,   338,
       0,     0,     0,     0,     0,     0,   305,     0,   339,   307,
     340,   341,   308,   165,     0,   309,     0,     0,     0,   312,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
     192,   193,   194,     0,   196,   197,   198,   199,   200,   316,
     202,   203,   204,   205,   206,   207,   208,   209,   210,     0,
     212,   213,   214,     0,     0,   217,   218,   219,   220,   273,
     274,   317,     0,     0,   275,     0,   863,   276,     0,     0,
       0,     0,     0,   319,   320,     0,   269,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,   279,     0,     0,     0,     0,     0,    44,     0,     0,
     270,   271,     0,     0,   323,   324,   325,   326,   327,   328,
     329,   330,   331,    45,   332,     0,     0,     0,     0,     0,
     295,   296,   297,   298,   299,     0,     0,     0,     0,   333,
     334,   335,   301,   336,     0,     0,   337,   338,     0,     0,
       0,     0,     0,     0,   305,   415,   339,   307,   340,   341,
     308,   165,     0,   309,     0,     0,     0,   312,     0,   272,
     451,     0,     0,     0,     0,     0,     0,     0,   192,   193,
     194,     0,   196,   197,   198,   199,   200,   316,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,   212,   213,
     214,     0,     0,   217,   218,   219,   220,   273,   274,   317,
       0,     0,   275,     0,   884,   276,     0,     0,     0,     0,
       0,   319,   320,     0,   269,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,   279,
       0,     0,     0,     0,     0,    44,     0,     0,   270,   271,
       0,     0,   323,   324,   325,   326,   327,   328,   329,   330,
     331,    45,   332,     0,     0,     0,     0,     0,   295,   296,
     297,   298,   299,     0,     0,   681,     0,   333,   334,   335,
     301,   336,     0,     0,   337,   338,     0,     0,     0,     0,
       0,     0,   305,     0,   339,   307,   340,   341,   308,   165,
       0,   309,     0,     0,     0,   312,     0,   272,     0,     0,
       0,     0,     0,     0,     0,     0,   192,   193,   194,     0,
     196,   197,   198,   199,   200,   316,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,   213,   214,     0,
       0,   217,   218,   219,   220,   273,   274,   317,     0,     0,
     275,     0,   916,   276,     0,     0,     0,     0,     0,   319,
     320,     0,   269,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,   279,     0,     0,
       0,     0,     0,    44,     0,     0,   270,   271,     0,     0,
     323,   324,   325,   326,   327,   328,   329,   330,   331,    45,
     332,     0,     0,     0,     0,     0,   295,   296,   297,   298,
     299,     0,     0,     0,     0,   333,   334,   335,   301,   336,
       0,     0,   337,   338,     0,     0,     0,     0,     0,     0,
     305,     0,   339,   307,   340,   341,   308,   165,     0,   309,
       0,     0,     0,   312,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,   192,   193,   194,     0,   196,   197,
     198,   199,   200,   316,   202,   203,   204,   205,   206,   207,
     208,   209,   210,     0,   212,   213,   214,     0,     0,   217,
     218,   219,   220,   273,   274,   317,     0,     0,   275,     0,
       0,   276,     0,     0,     0,     0,     0,   319,   320,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,   279,     0,     0,     0,     0,
       0,    44,     0,     0,     0,     0,     0,     0,   323,   324,
     325,   326,   327,   328,   329,   330,   331,    45,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,   334,   335,     0,   336,     0,     0,
     337,   338,   295,   296,   297,   298,   299,     0,     0,     0,
     339,   696,   340,   341,   301,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,     0,     0,   307,
       0,     0,   308,     0,     0,   309,     0,     0,     0,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,   193,   194,     0,   196,   197,   198,   199,   200,   316,
     202,   203,   204,   205,   206,   207,   208,   209,   210,     0,
     212,   213,   214,     0,     0,   217,   218,   219,   220,     0,
       0,   317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,     0,     0,
       0,     0,     0,     0,   323,   324,   325,   326,   327,   328,
     329,   330,   331,    45,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   333,
     334,   335,     0,   336,     0,     0,   337,   338,   295,   296,
     297,   298,   299,     0,     0,     0,   339,   726,   340,   341,
     301,   165,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,     0,     0,   307,     0,     0,   308,     0,
       0,   309,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   192,   193,   194,     0,
     196,   197,   198,   199,   200,   316,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,   213,   214,     0,
       0,   217,   218,   219,   220,     0,     0,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
     323,   324,   325,   326,   327,   328,   329,   330,   331,    45,
     332,     0,     0,     0,     0,     0,   295,   296,   297,   298,
     299,     0,     0,   728,     0,   333,   334,   335,   301,   336,
       0,     0,   337,   338,     0,     0,     0,     0,     0,     0,
     305,     0,   339,   307,   340,   341,   308,   165,     0,   309,
       0,     0,     0,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   192,   193,   194,     0,   196,   197,
     198,   199,   200,   316,   202,   203,   204,   205,   206,   207,
     208,   209,   210,     0,   212,   213,   214,     0,     0,   217,
     218,   219,   220,     0,     0,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,   320,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,     0,     0,     0,     0,     0,     0,   323,   324,
     325,   326,   327,   328,   329,   330,   331,    45,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,   334,   335,     0,   336,     0,     0,
     337,   338,   295,   296,   297,   298,   299,     0,     0,     0,
     339,   744,   340,   341,   301,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,     0,     0,   307,
       0,     0,   308,     0,     0,   309,     0,     0,     0,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,   193,   194,     0,   196,   197,   198,   199,   200,   316,
     202,   203,   204,   205,   206,   207,   208,   209,   210,     0,
     212,   213,   214,     0,     0,   217,   218,   219,   220,     0,
       0,   317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,     0,     0,
       0,     0,     0,     0,   323,   324,   325,   326,   327,   328,
     329,   330,   331,    45,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   333,
     334,   335,     0,   336,     0,     0,   337,   338,   295,   296,
     297,   298,   299,     0,     0,     0,   339,   753,   340,   341,
     301,   165,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,     0,     0,   307,     0,     0,   308,     0,
       0,   309,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   192,   193,   194,     0,
     196,   197,   198,   199,   200,   316,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,   213,   214,     0,
       0,   217,   218,   219,   220,     0,     0,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
     323,   324,   325,   326,   327,   328,   329,   330,   331,    45,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,   334,   335,     0,   336,
       0,     0,   337,   338,   295,   296,   297,   298,   299,     0,
       0,     0,   339,   856,   340,   341,   301,   165,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,     0,
       0,   307,     0,     0,   308,     0,     0,   309,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   192,   193,   194,     0,   196,   197,   198,   199,
     200,   316,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,   212,   213,   214,     0,     0,   217,   218,   219,
     220,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,   320,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,     0,     0,     0,     0,     0,   323,   324,   325,   326,
     327,   328,   329,   330,   331,    45,   332,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,   334,   335,     0,   336,     0,     0,   337,   338,
     295,   296,   297,   298,   299,     0,     0,     0,   339,   902,
     340,   341,   301,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,     0,     0,   307,     0,     0,
     308,     0,     0,   309,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   192,   193,
     194,     0,   196,   197,   198,   199,   200,   316,   202,   203,
     204,   205,   206,   207,   208,   209,   210,     0,   212,   213,
     214,     0,     0,   217,   218,   219,   220,     0,     0,   317,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,   320,     0,     0,     0,     0,     0,     0,     0,
     914,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,     0,     0,     0,
       0,     0,   323,   324,   325,   326,   327,   328,   329,   330,
     331,    45,   332,     0,     0,     0,     0,     0,   295,   296,
     297,   298,   299,     0,     0,     0,     0,   333,   334,   335,
     301,   336,     0,     0,   337,   338,     0,     0,     0,     0,
       0,     0,   305,     0,   339,   307,   340,   341,   308,   165,
       0,   309,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   192,   193,   194,     0,
     196,   197,   198,   199,   200,   316,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   212,   213,   214,     0,
       0,   217,   218,   219,   220,     0,     0,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
     320,     0,     0,   492,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
     323,   324,   325,   326,   327,   328,   329,   330,   331,    45,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,   334,   335,     0,   336,
       0,     0,   337,   338,     0,     0,     0,     0,     0,     0,
       0,     0,   339,     0,   340,   341,     0,   165,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   586,   514,   515,
     516,   517,   492,   493,     0,   518,   519,   520,   521,   522,
     523,     0,     0,   524,   525,   526,   527,   528,   529,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   587,   531,     0,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,     0,
       0,     0,     0,     0,     0,     0,     0,   542,     0,   543,
       0,     0,     0,     0,     0,   588,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   586,   514,   515,   516,
     517,   492,   493,     0,   518,   519,   520,   521,   522,   523,
       0,     0,   524,   525,   526,   527,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   587,   531,     0,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,   543,     0,
       0,     0,     0,     0,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,   586,   514,   515,   516,   517,
     492,   493,     0,   518,   519,   520,   521,   522,   523,     0,
       0,   524,   525,   526,   527,   528,   529,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   587,   531,     0,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,   543,     0,     0,
       0,     0,     0,   705,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   586,   514,   515,   516,   517,   492,
     493,     0,   518,   519,   520,   521,   522,   523,     0,     0,
     524,   525,   526,   527,   528,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   587,   531,     0,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,     0,     0,     0,     0,
       0,     0,     0,     0,   542,     0,   543,     0,     0,     0,
       0,     0,   731,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   586,   514,   515,   516,   517,   492,   493,
       0,   518,   519,   520,   521,   522,   523,     0,     0,   524,
     525,   526,   527,   528,   529,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   587,   531,     0,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,   543,     0,     0,     0,     0,
       0,   739,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   586,   514,   515,   516,   517,   492,   493,     0,
     518,   519,   520,   521,   522,   523,     0,     0,   524,   525,
     526,   527,   528,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   587,   531,     0,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,     0,     0,     0,     0,     0,     0,
       0,     0,   542,     0,   543,     0,     0,     0,     0,     0,
     797,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   492,   493,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,   505,   506,     0,     0,   509,   510,   511,
     512,     0,   514,   515,   516,   517,     0,     0,     0,   518,
       0,   520,   521,     0,     0,     0,     0,   524,   525,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   531,     0,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,     0,     0,     0,     0,     0,   492,   493,
       0,   542,     0,   543,   502,   503,   504,   505,   506,   921,
       0,   509,   510,   511,   512,     0,   514,   515,   516,   517,
       0,     0,     0,   518,     0,   520,   521,     0,     0,     0,
       0,   524,   525,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   531,     0,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,   543,   492,   493,
     627,   502,   503,   504,   505,   506,     0,     0,   509,   510,
     511,   512,     0,   514,   515,   516,   517,     0,     0,     0,
     518,     0,   520,   521,     0,     0,     0,     0,   524,   525,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,     0,     0,     0,     0,     0,     0,
       0,     0,   542,     0,   543,   492,   493,   733,     0,     0,
       0,   502,   503,   504,   505,   506,     0,     0,   509,   510,
     511,   512,     0,   514,   515,   516,   517,     0,     0,     0,
     518,     0,   520,   521,     0,     0,     0,     0,   524,   525,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,     0,     0,     0,     0,     0,     0,
       0,     0,   542,     0,   543,   492,   493,   736,   502,   503,
     504,   505,   506,     0,     0,   509,   510,   511,   512,     0,
     514,   515,   516,   517,     0,     0,     0,   518,     0,   520,
     521,     0,     0,     0,     0,   524,   525,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   531,
       0,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,   543,   492,   493,   817,     0,     0,     0,   502,   503,
     504,   505,   506,     0,     0,   509,   510,   511,   512,     0,
     514,   515,   516,   517,     0,     0,     0,   518,     0,   520,
     521,     0,     0,     0,     0,   524,   525,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   531,
       0,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,   543,   492,   493,   918,   502,   503,   504,   505,   506,
       0,     0,   509,   510,   511,   512,     0,   514,   515,   516,
     517,     0,     0,     0,   518,     0,   520,   521,     0,     0,
       0,     0,   524,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,   543,   492,
     493,   920,     0,     0,     0,   502,   503,   504,   505,   506,
       0,     0,   509,   510,   511,   512,     0,   514,   515,   516,
     517,     0,     0,     0,   518,     0,   520,   521,     0,     0,
       0,     0,   524,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,     0,     0,
       0,     0,     0,     0,   492,   493,   542,     0,   543,     0,
       0,   935,   502,   503,   504,   505,   506,     0,     0,   509,
     510,   511,   512,     0,   514,   515,   516,   517,     0,     0,
       0,   518,     0,   520,   521,     0,     0,     0,     0,   524,
     525,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   531,     0,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,     0,     0,     0,     0,   492,
     493,     0,     0,   542,     0,   543,   574,   502,   503,   504,
     505,   506,     0,     0,   509,   510,   511,   512,     0,   514,
     515,   516,   517,     0,     0,     0,   518,     0,   520,   521,
       0,     0,     0,     0,   524,   525,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
     543,   785,   502,   503,   504,   505,   506,   492,   493,   509,
     510,   511,   512,     0,   514,   515,   516,   517,     0,     0,
       0,   518,     0,   520,   521,     0,     0,     0,     0,   524,
     525,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   531,     0,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,   543,   791,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   586,   514,   515,   516,   517,  -223,   492,   493,   518,
     519,   520,   521,   522,   523,     0,     0,   524,   525,   526,
     527,   528,   529,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     587,   531,     0,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   492,   493,     0,   518,
     519,   520,   521,   522,   523,     0,     0,   524,   525,   526,
     527,   528,   529,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,     0,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     492,   493,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,   505,   506,     0,     0,   509,   510,   511,   512,
       0,   514,   515,   516,   517,     0,     0,     0,   518,     0,
     520,   521,     0,     0,   620,     0,   524,   525,   526,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,     0,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,     0,     0,   492,   493,     0,     0,     0,     0,
     542,     0,   543,   502,   503,   504,   505,   506,     0,     0,
     509,   510,   511,   512,     0,   514,   515,   516,   517,     0,
       0,     0,   518,     0,   520,   521,     0,     0,     0,     0,
     524,   525,   526,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   531,   789,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,     0,     0,   492,   493,
       0,     0,     0,     0,   542,     0,   543,   502,   503,   504,
     505,   506,     0,     0,   509,   510,   511,   512,     0,   514,
     515,   516,   517,     0,     0,     0,   518,     0,   520,   521,
       0,     0,     0,     0,   524,   525,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   839,     0,     0,     0,   531,     0,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
       0,     0,   492,   493,     0,     0,     0,     0,   542,     0,
     543,   502,   503,   504,   505,   506,     0,     0,   509,   510,
     511,   512,     0,   514,   515,   516,   517,     0,     0,     0,
     518,     0,   520,   521,     0,     0,     0,     0,   524,   525,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,     0,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,     0,     0,   492,   493,     0,     0,
       0,     0,   542,     0,   543,   502,   503,   504,   505,   506,
       0,     0,   509,   510,   511,   512,     0,   514,   515,   516,
     517,     0,     0,     0,   518,     0,   520,   521,     0,     0,
       0,     0,   524,     0,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,     0,     0,
     492,   493,     0,     0,     0,     0,   542,     0,   543,   502,
     503,   504,   505,   506,     0,     0,   509,   510,   511,   512,
       0,   514,   515,   516,   517,   492,   493,     0,   518,     0,
     520,   521,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,   543,   502,   503,   504,   505,   506,     0,     0,
     509,   510,   511,   512,     0,   514,   515,   516,   517,   492,
     493,     0,   518,     0,   520,   521,     0,     0,   502,   503,
     504,   505,   506,     0,     0,   509,   510,   511,   512,     0,
     514,   515,   516,   517,   492,   493,     0,   518,     0,   520,
     521,     0,     0,     0,     0,     0,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,     0,     0,     0,     0,
       0,     0,     0,     0,   542,     0,   543,     0,     0,     0,
       0,     0,   533,   534,   535,   536,   537,   538,   539,   540,
     541,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,   543,   502,   503,   504,   505,   506,     0,     0,   509,
     510,   511,   512,     0,   514,   515,   516,   517,     0,     0,
       0,   518,     0,   520,   521,     0,     0,   502,   503,   504,
     505,   506,     0,     0,   509,   510,   511,   512,     0,   514,
     515,   516,   517,     0,     0,     0,   518,     0,   520,   521,
       0,     0,     0,     0,     0,     0,     0,   534,   535,   536,
     537,   538,   539,   540,   541,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,   543,     0,     0,   463,     0,
       0,     0,     0,   535,   536,   537,   538,   539,   540,   541,
     188,     0,     0,     0,     0,     0,   189,     0,   542,     0,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   465,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,   190,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    45,   191,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   188,     0,     0,     0,     0,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,   191,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   188,     0,
       0,     0,     0,     0,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,     0,   190,
       0,     0,     0,     0,   323,   324,   325,     0,     0,     0,
       0,     0,     0,    45,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   188,     0,     0,     0,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,   190,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
     191,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,     0,     0,     0,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   139,   140,   141,   142,    44,
       0,   143,   144,   145,   146,   147,   148,     0,     0,     0,
       0,     0,     0,     0,     0,   568,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,     0,     0,   159,
     160
};

static const yytype_int16 yycheck[] =
{
      13,    85,   113,   242,   286,   282,   237,   403,   284,   405,
     325,   407,   466,   321,   322,   544,    77,   300,     7,   563,
     170,   565,   610,   567,   632,     0,    14,    15,    19,    19,
     618,     6,    18,    49,    19,   318,   113,    20,    21,    32,
      49,    21,    46,   113,   119,   136,   116,   117,   116,   117,
     128,   580,   581,   284,    29,   145,    31,   145,    33,   114,
      35,   183,   145,   294,    39,    78,   145,   152,   123,    42,
      43,    46,   114,    53,    14,    15,   114,    52,    34,   145,
     114,   114,   145,    20,    21,   123,   114,   145,   179,   123,
     123,   129,   145,   139,   184,   123,    71,   182,   153,   178,
     113,   184,    33,   158,   174,   183,   176,   182,   176,   392,
     152,   153,   152,   155,   153,   153,   158,    92,   184,   153,
     153,   184,   105,   106,   155,   153,   184,   145,    59,   240,
     113,   184,   115,   116,   117,   118,   145,   248,   746,   122,
     145,   145,   182,   182,   370,   428,   152,   136,   129,   105,
     106,   139,   183,   436,   167,   143,   744,   145,   146,   145,
     386,   152,   152,   240,   145,   158,   182,   152,   105,   106,
     281,   248,   455,   179,   235,   152,   113,   145,   159,   116,
     117,   118,   490,   176,   164,   122,   297,   176,   152,   145,
     178,   174,   185,   176,   174,   184,   153,   152,   129,   139,
     129,   176,   179,   143,   281,   145,   146,   182,   152,    58,
      59,   740,   323,   324,   145,   365,   152,   152,   182,   303,
     297,   155,   152,   752,   179,   152,   155,   240,   152,   152,
     152,   315,   463,   177,   465,   248,   780,   174,   826,   176,
     152,   637,   129,   179,   179,   152,   323,   324,   836,   179,
     177,   155,    32,   782,   177,   177,   160,    66,   145,   155,
      11,    70,   147,   148,   160,   177,    20,    21,   281,   552,
     160,    22,    23,   182,   178,   184,    56,    57,    87,    88,
      89,    90,   178,   160,   297,   396,   397,   398,   399,   145,
     744,   402,   182,   404,   576,   406,   604,   408,   842,   753,
      58,   179,   160,   155,   182,   182,   621,   155,   160,   160,
     323,   324,   160,   177,   182,   426,   184,   145,   182,   396,
     397,   398,   399,   336,   182,   402,   178,   404,   145,   406,
     178,   182,   145,    20,    21,   115,   152,     4,     5,   155,
       7,   129,   158,   454,   180,   181,   430,   183,   178,   426,
     145,   105,   106,   147,   148,   149,    20,    21,   183,   113,
     444,   115,   116,   117,   118,   139,   145,   763,   122,   145,
     145,   153,   183,   153,   154,   183,   145,   454,   158,    34,
      34,   161,   178,   396,   397,   398,   399,   155,   145,   402,
     183,   404,   147,   406,    53,   408,   176,   183,   482,   145,
     145,   182,   182,   155,   182,   185,   183,   161,   162,   163,
     164,   165,   159,   426,   178,   159,   103,   104,   105,   106,
     174,   434,   176,   159,   159,   701,   113,   159,   115,   116,
     117,   118,   159,   159,   159,   122,   159,   124,   125,   103,
     104,   454,   159,   107,   155,   722,   110,   111,   112,   113,
     145,   115,   116,   117,   118,   145,    32,   136,   122,   178,
     124,   125,   159,   178,   182,   478,   130,   131,   132,   178,
     701,   159,   583,    32,   161,   162,   163,   164,   165,   159,
     181,   592,   593,   159,   595,   159,   139,   174,    36,   176,
     154,   182,   156,   157,   158,   159,   160,    56,    57,   182,
     164,   182,   179,   176,   182,   589,   583,   182,   178,   177,
     174,   177,   176,   119,    13,   592,   593,   155,   595,   139,
     181,   183,   136,   145,   145,   764,   145,   145,   242,     7,
      42,   182,   545,   177,   184,   152,   152,   147,   147,   178,
     145,   145,   256,     1,    32,   145,   828,   159,   145,   160,
     145,   177,   160,   160,   177,   184,   115,   147,   145,    20,
      21,   145,   177,   277,   177,   145,   145,   145,    56,    57,
     583,   470,    49,    57,   102,   178,   886,   291,   177,   592,
     593,   182,   595,   177,   177,   160,   300,   179,   177,   177,
     304,   177,   306,   184,   153,   154,   707,   182,   160,   158,
     314,   160,   161,   177,   318,   319,   320,   718,   177,   720,
     177,   325,   184,    32,   177,    60,   177,   176,   253,   333,
     334,   335,    58,   337,   338,   339,   185,   115,    78,   545,
     707,   861,   795,   630,   348,     1,   852,    56,    57,   738,
     699,   718,   103,   104,   105,   106,   107,   386,   288,   110,
     409,   762,   113,   720,   115,   116,   117,   118,   388,   185,
      70,   122,   751,   124,   125,   153,   154,   401,   466,   297,
     158,   909,    -1,   161,    -1,    -1,    -1,    -1,   392,    -1,
      -1,   765,    -1,    -1,    -1,   762,    -1,    -1,   176,    -1,
     178,    -1,    -1,    -1,   707,    -1,   115,   185,   159,   160,
     161,   162,   163,   164,   165,   718,    -1,   720,    -1,    -1,
      -1,    -1,    -1,   174,   428,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   436,    -1,    -1,    -1,    -1,    -1,    -1,   443,
     814,    -1,    -1,   447,   153,   154,    -1,    -1,    -1,   158,
      -1,   455,   161,    -1,   855,    -1,    -1,    -1,    -1,   762,
     464,    -1,   466,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,   845,    -1,   182,    -1,    -1,   185,    -1,    -1,    -1,
     854,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
      -1,    -1,   506,   507,   508,   509,   510,   511,    -1,   513,
     514,    -1,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,    -1,   543,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   552,    -1,
      -1,    -1,   855,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   582,    -1,
      -1,    -1,   586,   587,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   606,   607,    -1,    -1,    32,    -1,   911,    -1,
      -1,    -1,    -1,   617,    -1,    -1,   620,   621,    -1,    -1,
     624,    -1,    -1,    -1,   927,   103,   104,   105,   106,   107,
      56,    57,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
     105,   106,   130,   131,   132,   659,   660,    -1,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,   678,   679,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   114,   115,
      -1,    20,    21,    -1,    -1,    -1,   174,   123,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     724,   176,    -1,    -1,    -1,    -1,    -1,   153,   154,    32,
      -1,    -1,   158,    -1,    -1,   161,    -1,    -1,   742,   743,
      -1,    -1,    -1,    -1,    -1,   749,    -1,   751,    -1,    -1,
     176,    -1,    -1,    56,    57,    -1,   182,    -1,    -1,   185,
     764,    -1,    -1,    -1,   768,    -1,    -1,    -1,   772,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,   789,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,
     153,   154,    -1,   182,    -1,   158,    -1,   160,   161,    -1,
      -1,    -1,    -1,    -1,   868,    32,    -1,   871,   872,   873,
     874,   875,    -1,   176,   878,   879,    -1,   881,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,   890,    -1,    -1,    56,
      57,    -1,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
     914,    -1,    -1,    -1,    -1,   919,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   931,    37,    38,
      -1,    40,    41,    -1,    -1,    44,    -1,    46,    47,    48,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,   115,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    94,   153,   154,    -1,    -1,
      -1,   158,    -1,   160,   161,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,   185,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,   180,   181,   182,   183,   184,     1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,    44,
      -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    32,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,   161,   162,   163,    -1,
     165,    -1,    -1,   168,   169,    -1,    -1,    -1,    14,    15,
      16,    17,    18,   178,    -1,   180,   181,   182,   183,   184,
      26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,
      56,    57,   115,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
     153,   154,    -1,    -1,    -1,   158,    -1,   160,   161,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,   185,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    56,    57,    -1,   153,   154,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,   165,
      -1,    -1,   168,   169,    14,    15,    16,    17,    18,    -1,
     176,    -1,   178,    -1,   180,   181,    26,   183,    -1,   185,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,   115,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   153,   154,    93,    -1,    -1,   158,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    32,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      56,    57,    -1,   153,   154,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,   169,
      14,    15,    16,    17,    18,    -1,   176,    -1,   178,    -1,
     180,   181,    26,   183,    -1,   185,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   153,   154,    93,
      -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,    -1,    -1,   129,    32,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,   165,    -1,    -1,   168,   169,    14,    15,    16,    17,
      18,    -1,   176,    -1,   178,    -1,   180,   181,    26,   183,
      -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,   153,   154,    -1,
      -1,    -1,   158,    -1,    -1,   161,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
     176,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,   185,
      -1,   129,    -1,    32,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,
     168,   169,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     178,    -1,   180,   181,   182,   183,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,   153,   154,    -1,    -1,    -1,   158,
      -1,   160,   161,    -1,    -1,   105,   106,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,   176,    -1,    -1,
     120,   121,    -1,    -1,    -1,    -1,   185,    -1,    -1,   129,
      -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   161,   162,   163,    26,   165,    -1,    -1,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,    41,
     180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   153,
     154,    93,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   185,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      56,    57,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   161,
     162,   163,    26,   165,    -1,    -1,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   178,    41,   180,   181,
      44,   183,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   153,   154,    93,
      -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    32,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,
      -1,   165,    -1,    -1,   168,   169,    14,    15,    16,    17,
      18,    -1,   176,    -1,   178,   179,   180,   181,    26,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   153,   154,    93,    -1,    -1,   158,    -1,
     160,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   185,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   161,   162,   163,    26,   165,    -1,    -1,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     178,    41,   180,   181,    44,   183,    -1,    47,    -1,    -1,
      -1,    51,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   153,   154,    93,    -1,    -1,   158,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   185,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   161,   162,   163,    26,   165,    -1,    -1,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   178,    41,
     180,   181,    44,   183,    -1,    47,    -1,    -1,    -1,    51,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   153,
     154,    93,    -1,    -1,   158,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      56,    57,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   161,
     162,   163,    26,   165,    -1,    -1,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    38,   177,   178,    41,   180,   181,
      44,   183,    -1,    47,    -1,    -1,    -1,    51,    -1,   115,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   153,   154,    93,
      -1,    -1,   158,    -1,   160,   161,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    21,    -1,   161,   162,   163,
      26,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   178,    41,   180,   181,    44,   183,
      -1,    47,    -1,    -1,    -1,    51,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,   153,   154,    93,    -1,    -1,
     158,    -1,   160,   161,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,   161,   162,   163,    26,   165,
      -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   178,    41,   180,   181,    44,   183,    -1,    47,
      -1,    -1,    -1,    51,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   153,   154,    93,    -1,    -1,   158,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,
     168,   169,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     178,   179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     162,   163,    -1,   165,    -1,    -1,   168,   169,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   178,   179,   180,   181,
      26,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,   159,    -1,   161,   162,   163,    26,   165,
      -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   178,    41,   180,   181,    44,   183,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,   162,   163,    -1,   165,    -1,    -1,
     168,   169,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     178,   179,   180,   181,    26,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     162,   163,    -1,   165,    -1,    -1,   168,   169,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   178,   179,   180,   181,
      26,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,
      -1,    -1,   168,   169,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   178,   179,   180,   181,    26,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,   162,   163,    -1,   165,    -1,    -1,   168,   169,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   178,   179,
     180,   181,    26,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,   161,   162,   163,
      26,   165,    -1,    -1,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   178,    41,   180,   181,    44,   183,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,   165,
      -1,    -1,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,   180,   181,    -1,   183,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    20,    21,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
      -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
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
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   174,    -1,   176,   103,   104,   105,   106,   107,   182,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    20,    21,
     179,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,    20,    21,   179,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,    20,    21,   179,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,   176,    20,    21,   179,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
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
      -1,    -1,    -1,    -1,    20,    21,   174,    -1,   176,    -1,
      -1,   179,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,   174,    -1,   176,   177,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   103,   104,   105,   106,   107,    20,    21,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,   177,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    20,    21,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    20,    21,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,   128,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     174,    -1,   176,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   174,    -1,   176,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   174,    -1,
     176,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   174,    -1,   176,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   174,    -1,   176,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    20,    21,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    20,
      21,    -1,   122,    -1,   124,   125,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    20,    21,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,   176,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   159,   160,   161,   162,   163,   164,   165,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,   174,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    45,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
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
      90,    91,    92,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   129,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,   168,
     169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   187,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    71,    92,   176,   182,   188,   196,   197,   201,   221,
     225,   240,   295,   300,   301,   305,   341,   343,   145,   216,
     217,   139,   202,   203,   145,   198,   199,   145,   183,   145,
     344,   342,    33,    59,   129,   145,   218,   219,   220,   232,
       4,     5,     7,   304,    58,    59,   294,   153,   152,   155,
     152,    21,    53,   164,   174,   200,   155,   183,   145,   298,
     299,    58,   293,   145,   145,   145,   129,   178,   152,   177,
      34,   105,   106,   145,   224,   226,   227,   145,   303,   183,
     296,    14,    15,   139,   143,   145,   146,   178,   190,   216,
     139,   203,   145,   145,   145,    66,    70,    87,    88,    89,
      90,   309,   297,   153,   184,   298,   183,   183,   145,   217,
     219,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   124,
     125,   126,   127,   130,   131,   132,   133,   134,   135,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   168,
     169,    34,    34,   178,   222,   183,   228,   155,   302,   306,
     292,   145,   291,   307,   147,   189,    14,    15,   139,   143,
     145,   190,   214,   215,   200,   183,   145,   184,    24,    30,
      45,    60,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   232,   308,   310,   311,   313,   316,   145,
     312,   145,   285,   286,   179,   136,   179,   276,   281,   282,
     155,   223,   230,   232,   183,   184,   291,   152,   155,   158,
     290,   147,   148,   152,   179,   297,   153,   182,   159,   159,
     159,   159,   178,   159,   159,   159,   159,   159,   159,    32,
      56,    57,   115,   153,   154,   158,   161,   176,   182,   185,
     182,   155,   182,   217,    46,   288,   307,   179,   182,   316,
       1,     8,     9,    10,    12,    14,    15,    16,    17,    18,
      25,    26,    27,    28,    37,    38,    40,    41,    44,    47,
      48,    50,    51,    54,    55,    61,    71,    93,    94,   105,
     106,   120,   121,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   146,   161,   162,   163,   165,   168,   169,   178,
     180,   181,   182,   184,   192,   194,   204,   205,   208,   209,
     210,   211,   212,   213,   228,   229,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   249,   252,
     255,   259,   260,   261,   262,   263,   264,   265,   270,   273,
     308,   345,   349,   354,   356,   358,   279,   145,   316,   114,
     123,   153,   289,   214,   184,   273,   319,   321,   323,   317,
     145,   314,   325,   327,   329,   331,   333,   335,   337,   339,
      32,    32,   158,   176,   185,   177,   273,   145,   184,   316,
     184,   285,   177,   288,   114,   153,   155,   158,   287,   281,
     273,   307,   316,   345,   178,   159,   114,   182,   231,   261,
     273,   228,   273,   178,   273,   159,   159,   178,   182,   159,
     139,    54,   273,   228,   159,   114,   231,   273,   273,   273,
     181,   259,   259,    12,   316,    12,   316,   273,   351,   355,
     191,   273,   273,   273,   232,   273,   273,   273,   181,    36,
     182,   182,   273,   145,   178,   182,   182,   182,   291,   221,
     119,   182,    20,    21,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   122,   123,
     124,   125,   126,   127,   130,   131,   132,   133,   134,   135,
     153,   154,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   174,   176,   178,   176,   184,   221,   276,   278,
     153,   182,   289,   231,   273,   182,   316,   316,   316,   316,
     179,   312,   316,   222,   316,   222,   316,   222,   145,   283,
     284,   316,   286,   177,   177,   177,   316,   231,   273,   228,
      19,   178,   232,   250,   231,   273,   114,   153,   182,    13,
     273,   228,   243,   245,   273,   247,   155,   139,   316,   231,
     273,   182,   181,   307,    49,   145,   161,   178,   273,   346,
     347,   348,   350,   351,   352,   353,   307,   178,   347,   353,
     128,   182,   184,   148,   149,   189,   195,   179,   159,   232,
     183,   228,   176,   179,   253,   273,   136,   258,   259,    18,
     145,   145,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   145,   120,
     121,   273,   273,   145,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   120,   121,
     273,    21,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   145,   271,   273,   179,   253,   220,     7,
      42,   277,   182,   231,   273,   182,   160,   182,   160,   160,
     160,   182,   160,   223,   160,   223,   160,   223,   155,   160,
     182,   160,   182,   153,   287,   253,   179,   253,   159,   273,
     316,   182,   228,   179,   316,   316,   179,   316,   160,   182,
      19,   259,   123,   287,   179,   152,   182,   348,   177,   152,
     177,   182,    19,   179,   348,   184,   273,   351,   184,   273,
     147,   193,   266,   268,   230,    11,    22,    23,   206,   207,
     145,   346,   152,   179,   114,   123,   153,   158,   256,   257,
     222,   159,   178,   273,   273,   177,   273,   273,   145,   155,
       1,   177,   179,   177,    43,   277,   288,   182,   320,   316,
     324,   318,   315,   326,   160,   330,   160,   334,   160,   316,
     338,   283,   340,   273,   182,   179,   145,   179,   160,   160,
     160,   160,   258,   253,   273,   273,   347,   348,   145,   346,
     177,   177,   273,   177,   352,   253,   347,   184,   177,   150,
     147,   316,   222,   184,   228,   273,   177,   273,   145,   145,
     145,   145,   152,   177,   223,   274,   179,   253,   273,   272,
     182,   280,   227,   160,   328,   332,   336,   228,   160,   182,
     251,   244,   246,   248,   178,   182,   348,   177,   123,   287,
     177,   182,   348,   177,   160,   223,   228,   179,   256,   177,
     128,   228,   254,   316,   179,   226,   322,   273,   145,   273,
     273,   273,   179,   273,   273,   177,   177,   273,   273,   273,
     184,   267,   160,   207,   114,   273,   160,   182,   179,   160,
     179,   182,   357,   177,   357,   177,   232,   269,   273,   275,
     273,    49,   177,   184,   232,   179,   273,   177,   177
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
     216,   216,   217,   217,   218,   218,   218,   219,   219,   220,
     220,   221,   221,   222,   222,   222,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   225,   226,   227,   228,   228,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   230,   230,   230,   231,   231,
     231,   232,   232,   232,   233,   234,   234,   234,   234,   235,
     236,   237,   237,   237,   237,   237,   238,   238,   238,   238,
     239,   240,   240,   241,   243,   244,   242,   245,   246,   242,
     247,   248,   242,   250,   251,   249,   252,   252,   252,   253,
     253,   254,   254,   254,   255,   255,   255,   256,   256,   256,
     256,   257,   257,   258,   258,   259,   259,   260,   260,   260,
     260,   260,   260,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   262,   262,   262,   262,   263,   264,   264,   265,
     266,   267,   265,   268,   269,   265,   270,   271,   272,   270,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   274,   275,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     276,   276,   277,   277,   278,   279,   279,   280,   279,   279,
     279,   281,   281,   282,   282,   283,   283,   284,   284,   285,
     286,   286,   287,   287,   288,   288,   288,   288,   288,   288,
     289,   289,   289,   290,   290,   291,   291,   291,   291,   291,
     292,   292,   293,   293,   294,   294,   294,   295,   296,   295,
     297,   297,   297,   298,   299,   299,   300,   301,   301,   302,
     302,   303,   304,   304,   306,   305,   307,   307,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   309,   309,   309,   309,   309,   309,   310,
     311,   311,   312,   312,   314,   315,   313,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   317,   318,   316,   316,   319,   320,
     316,   321,   322,   316,   323,   324,   316,   316,   325,   326,
     316,   327,   328,   316,   316,   329,   330,   316,   331,   332,
     316,   316,   333,   334,   316,   335,   336,   316,   337,   338,
     316,   339,   340,   316,   342,   341,   344,   343,   345,   345,
     345,   345,   346,   346,   346,   346,   347,   347,   348,   348,
     349,   349,   349,   349,   349,   349,   350,   350,   351,   352,
     352,   353,   353,   354,   354,   355,   355,   356,   357,   357,
     358,   358
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
       1,     5,     1,     3,     1,     1,     1,     1,     4,     1,
       3,     0,     3,     0,     2,     3,     0,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     3,     7,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     0,     2,     2,     3,     2,
       2,     1,     3,     2,     2,     2,     4,     5,     2,     1,
       1,     2,     3,     4,     2,     3,     3,     4,     2,     3,
       4,     1,     1,     2,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     1,     3,     0,     4,     1,     6,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     4,     4,     6,     5,     6,     3,
       0,     0,     8,     0,     0,     9,     3,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       4,     3,     1,     3,     4,     3,     4,     2,     4,     4,
       7,     8,     3,     5,     0,     0,     8,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     4,     0,     1,     3,     0,     3,     0,     7,     5,
       5,     2,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     4,     6,     5,     5,     4,
       0,     2,     0,     1,     0,     1,     1,     5,     0,     4,
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3395 "ds_parser.cpp"
        break;

    case 189: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3401 "ds_parser.cpp"
        break;

    case 190: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3407 "ds_parser.cpp"
        break;

    case 191: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3413 "ds_parser.cpp"
        break;

    case 192: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3419 "ds_parser.cpp"
        break;

    case 194: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3425 "ds_parser.cpp"
        break;

    case 198: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3431 "ds_parser.cpp"
        break;

    case 204: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3437 "ds_parser.cpp"
        break;

    case 205: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3443 "ds_parser.cpp"
        break;

    case 207: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3449 "ds_parser.cpp"
        break;

    case 209: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3455 "ds_parser.cpp"
        break;

    case 210: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3461 "ds_parser.cpp"
        break;

    case 211: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3467 "ds_parser.cpp"
        break;

    case 212: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3473 "ds_parser.cpp"
        break;

    case 213: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3479 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3485 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3491 "ds_parser.cpp"
        break;

    case 216: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3497 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3503 "ds_parser.cpp"
        break;

    case 218: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3509 "ds_parser.cpp"
        break;

    case 219: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3515 "ds_parser.cpp"
        break;

    case 220: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3521 "ds_parser.cpp"
        break;

    case 221: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3527 "ds_parser.cpp"
        break;

    case 222: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3533 "ds_parser.cpp"
        break;

    case 223: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3539 "ds_parser.cpp"
        break;

    case 224: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3545 "ds_parser.cpp"
        break;

    case 228: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3551 "ds_parser.cpp"
        break;

    case 229: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3557 "ds_parser.cpp"
        break;

    case 230: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3563 "ds_parser.cpp"
        break;

    case 231: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3569 "ds_parser.cpp"
        break;

    case 232: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3575 "ds_parser.cpp"
        break;

    case 233: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3581 "ds_parser.cpp"
        break;

    case 234: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3587 "ds_parser.cpp"
        break;

    case 235: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3593 "ds_parser.cpp"
        break;

    case 236: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3599 "ds_parser.cpp"
        break;

    case 237: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3605 "ds_parser.cpp"
        break;

    case 238: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3611 "ds_parser.cpp"
        break;

    case 239: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3617 "ds_parser.cpp"
        break;

    case 241: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3623 "ds_parser.cpp"
        break;

    case 242: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3629 "ds_parser.cpp"
        break;

    case 249: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3635 "ds_parser.cpp"
        break;

    case 252: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3641 "ds_parser.cpp"
        break;

    case 253: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3647 "ds_parser.cpp"
        break;

    case 254: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3653 "ds_parser.cpp"
        break;

    case 256: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3659 "ds_parser.cpp"
        break;

    case 257: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3665 "ds_parser.cpp"
        break;

    case 258: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3671 "ds_parser.cpp"
        break;

    case 259: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3677 "ds_parser.cpp"
        break;

    case 260: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3683 "ds_parser.cpp"
        break;

    case 261: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3689 "ds_parser.cpp"
        break;

    case 262: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3695 "ds_parser.cpp"
        break;

    case 263: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3701 "ds_parser.cpp"
        break;

    case 264: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3707 "ds_parser.cpp"
        break;

    case 265: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3713 "ds_parser.cpp"
        break;

    case 270: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3719 "ds_parser.cpp"
        break;

    case 273: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3725 "ds_parser.cpp"
        break;

    case 276: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3731 "ds_parser.cpp"
        break;

    case 278: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3737 "ds_parser.cpp"
        break;

    case 279: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3743 "ds_parser.cpp"
        break;

    case 281: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3749 "ds_parser.cpp"
        break;

    case 282: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3755 "ds_parser.cpp"
        break;

    case 283: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3761 "ds_parser.cpp"
        break;

    case 284: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3767 "ds_parser.cpp"
        break;

    case 285: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3773 "ds_parser.cpp"
        break;

    case 286: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3779 "ds_parser.cpp"
        break;

    case 288: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3785 "ds_parser.cpp"
        break;

    case 291: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3791 "ds_parser.cpp"
        break;

    case 292: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3797 "ds_parser.cpp"
        break;

    case 297: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3803 "ds_parser.cpp"
        break;

    case 302: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3809 "ds_parser.cpp"
        break;

    case 307: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3815 "ds_parser.cpp"
        break;

    case 310: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3821 "ds_parser.cpp"
        break;

    case 311: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3827 "ds_parser.cpp"
        break;

    case 312: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3833 "ds_parser.cpp"
        break;

    case 313: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3839 "ds_parser.cpp"
        break;

    case 316: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3845 "ds_parser.cpp"
        break;

    case 345: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3851 "ds_parser.cpp"
        break;

    case 346: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3857 "ds_parser.cpp"
        break;

    case 347: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3863 "ds_parser.cpp"
        break;

    case 348: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3869 "ds_parser.cpp"
        break;

    case 349: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3875 "ds_parser.cpp"
        break;

    case 350: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3881 "ds_parser.cpp"
        break;

    case 351: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3887 "ds_parser.cpp"
        break;

    case 352: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3893 "ds_parser.cpp"
        break;

    case 353: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3899 "ds_parser.cpp"
        break;

    case 354: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3905 "ds_parser.cpp"
        break;

    case 355: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3911 "ds_parser.cpp"
        break;

    case 356: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3917 "ds_parser.cpp"
        break;

    case 357: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3923 "ds_parser.cpp"
        break;

    case 358: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3929 "ds_parser.cpp"
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
#line 461 "ds_parser.ypp"
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
#line 4232 "ds_parser.cpp"
    break;

  case 16:
#line 476 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4238 "ds_parser.cpp"
    break;

  case 17:
#line 477 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4244 "ds_parser.cpp"
    break;

  case 18:
#line 481 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4250 "ds_parser.cpp"
    break;

  case 19:
#line 485 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4260 "ds_parser.cpp"
    break;

  case 20:
#line 490 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4274 "ds_parser.cpp"
    break;

  case 21:
#line 499 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4284 "ds_parser.cpp"
    break;

  case 22:
#line 507 "ds_parser.ypp"
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
#line 4302 "ds_parser.cpp"
    break;

  case 23:
#line 523 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4312 "ds_parser.cpp"
    break;

  case 24:
#line 528 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4322 "ds_parser.cpp"
    break;

  case 25:
#line 536 "ds_parser.ypp"
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
#line 4349 "ds_parser.cpp"
    break;

  case 26:
#line 557 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4361 "ds_parser.cpp"
    break;

  case 27:
#line 566 "ds_parser.ypp"
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
#line 4385 "ds_parser.cpp"
    break;

  case 29:
#line 592 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4393 "ds_parser.cpp"
    break;

  case 30:
#line 595 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4404 "ds_parser.cpp"
    break;

  case 31:
#line 601 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4415 "ds_parser.cpp"
    break;

  case 32:
#line 610 "ds_parser.ypp"
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
#line 4441 "ds_parser.cpp"
    break;

  case 33:
#line 631 "ds_parser.ypp"
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
#line 4466 "ds_parser.cpp"
    break;

  case 34:
#line 654 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4472 "ds_parser.cpp"
    break;

  case 35:
#line 655 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4478 "ds_parser.cpp"
    break;

  case 39:
#line 668 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4486 "ds_parser.cpp"
    break;

  case 40:
#line 671 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4494 "ds_parser.cpp"
    break;

  case 41:
#line 677 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4502 "ds_parser.cpp"
    break;

  case 42:
#line 683 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4510 "ds_parser.cpp"
    break;

  case 43:
#line 686 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4518 "ds_parser.cpp"
    break;

  case 44:
#line 692 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4524 "ds_parser.cpp"
    break;

  case 45:
#line 693 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4530 "ds_parser.cpp"
    break;

  case 46:
#line 697 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4536 "ds_parser.cpp"
    break;

  case 47:
#line 698 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4542 "ds_parser.cpp"
    break;

  case 48:
#line 699 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
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
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4576 "ds_parser.cpp"
    break;

  case 52:
#line 722 "ds_parser.ypp"
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
#line 4594 "ds_parser.cpp"
    break;

  case 53:
#line 738 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4604 "ds_parser.cpp"
    break;

  case 54:
#line 746 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4616 "ds_parser.cpp"
    break;

  case 55:
#line 756 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4627 "ds_parser.cpp"
    break;

  case 56:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4633 "ds_parser.cpp"
    break;

  case 57:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4639 "ds_parser.cpp"
    break;

  case 58:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4645 "ds_parser.cpp"
    break;

  case 59:
#line 768 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4651 "ds_parser.cpp"
    break;

  case 60:
#line 769 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4657 "ds_parser.cpp"
    break;

  case 61:
#line 770 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4663 "ds_parser.cpp"
    break;

  case 62:
#line 774 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4673 "ds_parser.cpp"
    break;

  case 63:
#line 779 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4683 "ds_parser.cpp"
    break;

  case 64:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4689 "ds_parser.cpp"
    break;

  case 65:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4695 "ds_parser.cpp"
    break;

  case 66:
#line 789 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4701 "ds_parser.cpp"
    break;

  case 67:
#line 790 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4707 "ds_parser.cpp"
    break;

  case 68:
#line 791 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4713 "ds_parser.cpp"
    break;

  case 69:
#line 792 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4719 "ds_parser.cpp"
    break;

  case 70:
#line 793 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4725 "ds_parser.cpp"
    break;

  case 71:
#line 794 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4733 "ds_parser.cpp"
    break;

  case 72:
#line 800 "ds_parser.ypp"
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
#line 4751 "ds_parser.cpp"
    break;

  case 73:
#line 813 "ds_parser.ypp"
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
#line 4769 "ds_parser.cpp"
    break;

  case 74:
#line 829 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4775 "ds_parser.cpp"
    break;

  case 75:
#line 830 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4781 "ds_parser.cpp"
    break;

  case 76:
#line 831 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4787 "ds_parser.cpp"
    break;

  case 77:
#line 835 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4799 "ds_parser.cpp"
    break;

  case 78:
#line 842 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4813 "ds_parser.cpp"
    break;

  case 79:
#line 854 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4822 "ds_parser.cpp"
    break;

  case 80:
#line 858 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4831 "ds_parser.cpp"
    break;

  case 81:
#line 865 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4837 "ds_parser.cpp"
    break;

  case 82:
#line 866 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4843 "ds_parser.cpp"
    break;

  case 83:
#line 870 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4849 "ds_parser.cpp"
    break;

  case 84:
#line 871 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4855 "ds_parser.cpp"
    break;

  case 85:
#line 872 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4861 "ds_parser.cpp"
    break;

  case 86:
#line 876 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4871 "ds_parser.cpp"
    break;

  case 87:
#line 881 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4880 "ds_parser.cpp"
    break;

  case 88:
#line 888 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4889 "ds_parser.cpp"
    break;

  case 89:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4895 "ds_parser.cpp"
    break;

  case 90:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4901 "ds_parser.cpp"
    break;

  case 91:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4907 "ds_parser.cpp"
    break;

  case 92:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4913 "ds_parser.cpp"
    break;

  case 93:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4919 "ds_parser.cpp"
    break;

  case 94:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4925 "ds_parser.cpp"
    break;

  case 95:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4931 "ds_parser.cpp"
    break;

  case 96:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4937 "ds_parser.cpp"
    break;

  case 97:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4943 "ds_parser.cpp"
    break;

  case 98:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4949 "ds_parser.cpp"
    break;

  case 99:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4955 "ds_parser.cpp"
    break;

  case 100:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4961 "ds_parser.cpp"
    break;

  case 101:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 4967 "ds_parser.cpp"
    break;

  case 102:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 4973 "ds_parser.cpp"
    break;

  case 103:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 4979 "ds_parser.cpp"
    break;

  case 104:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 4985 "ds_parser.cpp"
    break;

  case 105:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4991 "ds_parser.cpp"
    break;

  case 106:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4997 "ds_parser.cpp"
    break;

  case 107:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5003 "ds_parser.cpp"
    break;

  case 108:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5009 "ds_parser.cpp"
    break;

  case 109:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5015 "ds_parser.cpp"
    break;

  case 110:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5021 "ds_parser.cpp"
    break;

  case 111:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5027 "ds_parser.cpp"
    break;

  case 112:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5033 "ds_parser.cpp"
    break;

  case 113:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5039 "ds_parser.cpp"
    break;

  case 114:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5045 "ds_parser.cpp"
    break;

  case 115:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5051 "ds_parser.cpp"
    break;

  case 116:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5057 "ds_parser.cpp"
    break;

  case 117:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5063 "ds_parser.cpp"
    break;

  case 118:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5069 "ds_parser.cpp"
    break;

  case 119:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5075 "ds_parser.cpp"
    break;

  case 120:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5081 "ds_parser.cpp"
    break;

  case 121:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5087 "ds_parser.cpp"
    break;

  case 122:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5093 "ds_parser.cpp"
    break;

  case 123:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5099 "ds_parser.cpp"
    break;

  case 124:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5105 "ds_parser.cpp"
    break;

  case 125:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5111 "ds_parser.cpp"
    break;

  case 126:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5117 "ds_parser.cpp"
    break;

  case 127:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5123 "ds_parser.cpp"
    break;

  case 128:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5129 "ds_parser.cpp"
    break;

  case 129:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5135 "ds_parser.cpp"
    break;

  case 130:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5141 "ds_parser.cpp"
    break;

  case 131:
#line 937 "ds_parser.ypp"
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
#line 5165 "ds_parser.cpp"
    break;

  case 132:
#line 959 "ds_parser.ypp"
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
#line 5205 "ds_parser.cpp"
    break;

  case 133:
#line 997 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5214 "ds_parser.cpp"
    break;

  case 134:
#line 1004 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5223 "ds_parser.cpp"
    break;

  case 135:
#line 1008 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5236 "ds_parser.cpp"
    break;

  case 136:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5242 "ds_parser.cpp"
    break;

  case 137:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5248 "ds_parser.cpp"
    break;

  case 138:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5254 "ds_parser.cpp"
    break;

  case 139:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5260 "ds_parser.cpp"
    break;

  case 140:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5266 "ds_parser.cpp"
    break;

  case 141:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5272 "ds_parser.cpp"
    break;

  case 142:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5278 "ds_parser.cpp"
    break;

  case 143:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5284 "ds_parser.cpp"
    break;

  case 144:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5290 "ds_parser.cpp"
    break;

  case 145:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5296 "ds_parser.cpp"
    break;

  case 146:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5302 "ds_parser.cpp"
    break;

  case 147:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5308 "ds_parser.cpp"
    break;

  case 148:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5314 "ds_parser.cpp"
    break;

  case 149:
#line 1032 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5320 "ds_parser.cpp"
    break;

  case 150:
#line 1033 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5326 "ds_parser.cpp"
    break;

  case 151:
#line 1034 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5332 "ds_parser.cpp"
    break;

  case 152:
#line 1035 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5338 "ds_parser.cpp"
    break;

  case 153:
#line 1036 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5344 "ds_parser.cpp"
    break;

  case 154:
#line 1037 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5350 "ds_parser.cpp"
    break;

  case 155:
#line 1041 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5360 "ds_parser.cpp"
    break;

  case 156:
#line 1046 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5371 "ds_parser.cpp"
    break;

  case 157:
#line 1052 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5379 "ds_parser.cpp"
    break;

  case 158:
#line 1058 "ds_parser.ypp"
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
#line 5418 "ds_parser.cpp"
    break;

  case 159:
#line 1092 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5426 "ds_parser.cpp"
    break;

  case 160:
#line 1095 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5434 "ds_parser.cpp"
    break;

  case 161:
#line 1101 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5440 "ds_parser.cpp"
    break;

  case 162:
#line 1102 "ds_parser.ypp"
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
#line 5456 "ds_parser.cpp"
    break;

  case 163:
#line 1113 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5462 "ds_parser.cpp"
    break;

  case 164:
#line 1117 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5470 "ds_parser.cpp"
    break;

  case 165:
#line 1123 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5478 "ds_parser.cpp"
    break;

  case 166:
#line 1126 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5486 "ds_parser.cpp"
    break;

  case 167:
#line 1129 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5495 "ds_parser.cpp"
    break;

  case 168:
#line 1133 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5503 "ds_parser.cpp"
    break;

  case 169:
#line 1139 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5509 "ds_parser.cpp"
    break;

  case 170:
#line 1143 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5515 "ds_parser.cpp"
    break;

  case 171:
#line 1147 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5523 "ds_parser.cpp"
    break;

  case 172:
#line 1150 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5531 "ds_parser.cpp"
    break;

  case 173:
#line 1153 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5541 "ds_parser.cpp"
    break;

  case 174:
#line 1158 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5549 "ds_parser.cpp"
    break;

  case 175:
#line 1161 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5559 "ds_parser.cpp"
    break;

  case 176:
#line 1169 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5567 "ds_parser.cpp"
    break;

  case 177:
#line 1172 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5577 "ds_parser.cpp"
    break;

  case 178:
#line 1177 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5585 "ds_parser.cpp"
    break;

  case 179:
#line 1180 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5595 "ds_parser.cpp"
    break;

  case 180:
#line 1188 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5603 "ds_parser.cpp"
    break;

  case 181:
#line 1194 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5609 "ds_parser.cpp"
    break;

  case 182:
#line 1195 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5615 "ds_parser.cpp"
    break;

  case 183:
#line 1199 "ds_parser.ypp"
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
#line 5651 "ds_parser.cpp"
    break;

  case 184:
#line 1233 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5657 "ds_parser.cpp"
    break;

  case 185:
#line 1233 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5663 "ds_parser.cpp"
    break;

  case 186:
#line 1233 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5671 "ds_parser.cpp"
    break;

  case 187:
#line 1236 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5677 "ds_parser.cpp"
    break;

  case 188:
#line 1236 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5683 "ds_parser.cpp"
    break;

  case 189:
#line 1236 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5693 "ds_parser.cpp"
    break;

  case 190:
#line 1241 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5699 "ds_parser.cpp"
    break;

  case 191:
#line 1241 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5705 "ds_parser.cpp"
    break;

  case 192:
#line 1241 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5715 "ds_parser.cpp"
    break;

  case 193:
#line 1249 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5721 "ds_parser.cpp"
    break;

  case 194:
#line 1249 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5727 "ds_parser.cpp"
    break;

  case 195:
#line 1249 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5735 "ds_parser.cpp"
    break;

  case 196:
#line 1255 "ds_parser.ypp"
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
#line 5750 "ds_parser.cpp"
    break;

  case 197:
#line 1265 "ds_parser.ypp"
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
#line 5766 "ds_parser.cpp"
    break;

  case 198:
#line 1276 "ds_parser.ypp"
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
#line 5783 "ds_parser.cpp"
    break;

  case 199:
#line 1291 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5791 "ds_parser.cpp"
    break;

  case 200:
#line 1294 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5799 "ds_parser.cpp"
    break;

  case 201:
#line 1300 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5805 "ds_parser.cpp"
    break;

  case 202:
#line 1301 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5817 "ds_parser.cpp"
    break;

  case 203:
#line 1308 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5830 "ds_parser.cpp"
    break;

  case 204:
#line 1319 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5836 "ds_parser.cpp"
    break;

  case 205:
#line 1320 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5842 "ds_parser.cpp"
    break;

  case 206:
#line 1321 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5848 "ds_parser.cpp"
    break;

  case 207:
#line 1325 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5854 "ds_parser.cpp"
    break;

  case 208:
#line 1326 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5860 "ds_parser.cpp"
    break;

  case 209:
#line 1327 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5866 "ds_parser.cpp"
    break;

  case 210:
#line 1328 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5872 "ds_parser.cpp"
    break;

  case 211:
#line 1332 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5882 "ds_parser.cpp"
    break;

  case 212:
#line 1337 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5892 "ds_parser.cpp"
    break;

  case 213:
#line 1345 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5898 "ds_parser.cpp"
    break;

  case 214:
#line 1346 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5904 "ds_parser.cpp"
    break;

  case 215:
#line 1350 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5914 "ds_parser.cpp"
    break;

  case 216:
#line 1356 "ds_parser.ypp"
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
#line 5979 "ds_parser.cpp"
    break;

  case 217:
#line 1419 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5985 "ds_parser.cpp"
    break;

  case 218:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5991 "ds_parser.cpp"
    break;

  case 219:
#line 1421 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5997 "ds_parser.cpp"
    break;

  case 220:
#line 1422 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6003 "ds_parser.cpp"
    break;

  case 221:
#line 1423 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6009 "ds_parser.cpp"
    break;

  case 222:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6015 "ds_parser.cpp"
    break;

  case 223:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6021 "ds_parser.cpp"
    break;

  case 224:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6027 "ds_parser.cpp"
    break;

  case 225:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6033 "ds_parser.cpp"
    break;

  case 226:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6039 "ds_parser.cpp"
    break;

  case 227:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6045 "ds_parser.cpp"
    break;

  case 228:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6051 "ds_parser.cpp"
    break;

  case 229:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6057 "ds_parser.cpp"
    break;

  case 230:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6063 "ds_parser.cpp"
    break;

  case 231:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6069 "ds_parser.cpp"
    break;

  case 232:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6075 "ds_parser.cpp"
    break;

  case 233:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6081 "ds_parser.cpp"
    break;

  case 234:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6087 "ds_parser.cpp"
    break;

  case 235:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6093 "ds_parser.cpp"
    break;

  case 236:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6099 "ds_parser.cpp"
    break;

  case 237:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6105 "ds_parser.cpp"
    break;

  case 238:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6111 "ds_parser.cpp"
    break;

  case 239:
#line 1444 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6117 "ds_parser.cpp"
    break;

  case 240:
#line 1445 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6123 "ds_parser.cpp"
    break;

  case 241:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6129 "ds_parser.cpp"
    break;

  case 242:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6135 "ds_parser.cpp"
    break;

  case 243:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6141 "ds_parser.cpp"
    break;

  case 244:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6147 "ds_parser.cpp"
    break;

  case 245:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6153 "ds_parser.cpp"
    break;

  case 246:
#line 1457 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6165 "ds_parser.cpp"
    break;

  case 247:
#line 1468 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6175 "ds_parser.cpp"
    break;

  case 248:
#line 1473 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6187 "ds_parser.cpp"
    break;

  case 249:
#line 1483 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6196 "ds_parser.cpp"
    break;

  case 250:
#line 1487 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6202 "ds_parser.cpp"
    break;

  case 251:
#line 1487 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6208 "ds_parser.cpp"
    break;

  case 252:
#line 1487 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6220 "ds_parser.cpp"
    break;

  case 253:
#line 1494 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6226 "ds_parser.cpp"
    break;

  case 254:
#line 1494 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6232 "ds_parser.cpp"
    break;

  case 255:
#line 1494 "ds_parser.ypp"
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
#line 6248 "ds_parser.cpp"
    break;

  case 256:
#line 1508 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6257 "ds_parser.cpp"
    break;

  case 257:
#line 1512 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6263 "ds_parser.cpp"
    break;

  case 258:
#line 1512 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6269 "ds_parser.cpp"
    break;

  case 259:
#line 1512 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6278 "ds_parser.cpp"
    break;

  case 260:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6284 "ds_parser.cpp"
    break;

  case 261:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6290 "ds_parser.cpp"
    break;

  case 262:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6296 "ds_parser.cpp"
    break;

  case 263:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6302 "ds_parser.cpp"
    break;

  case 264:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6308 "ds_parser.cpp"
    break;

  case 265:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6314 "ds_parser.cpp"
    break;

  case 266:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6320 "ds_parser.cpp"
    break;

  case 267:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6326 "ds_parser.cpp"
    break;

  case 268:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6332 "ds_parser.cpp"
    break;

  case 269:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6338 "ds_parser.cpp"
    break;

  case 270:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6344 "ds_parser.cpp"
    break;

  case 271:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6350 "ds_parser.cpp"
    break;

  case 272:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6356 "ds_parser.cpp"
    break;

  case 273:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6362 "ds_parser.cpp"
    break;

  case 274:
#line 1533 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6368 "ds_parser.cpp"
    break;

  case 275:
#line 1534 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6374 "ds_parser.cpp"
    break;

  case 276:
#line 1535 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6380 "ds_parser.cpp"
    break;

  case 277:
#line 1536 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6386 "ds_parser.cpp"
    break;

  case 278:
#line 1537 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6392 "ds_parser.cpp"
    break;

  case 279:
#line 1538 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6398 "ds_parser.cpp"
    break;

  case 280:
#line 1539 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6404 "ds_parser.cpp"
    break;

  case 281:
#line 1540 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6410 "ds_parser.cpp"
    break;

  case 282:
#line 1541 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6416 "ds_parser.cpp"
    break;

  case 283:
#line 1542 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6422 "ds_parser.cpp"
    break;

  case 284:
#line 1543 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6428 "ds_parser.cpp"
    break;

  case 285:
#line 1544 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6434 "ds_parser.cpp"
    break;

  case 286:
#line 1545 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6440 "ds_parser.cpp"
    break;

  case 287:
#line 1546 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6446 "ds_parser.cpp"
    break;

  case 288:
#line 1547 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6452 "ds_parser.cpp"
    break;

  case 289:
#line 1548 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6458 "ds_parser.cpp"
    break;

  case 290:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6464 "ds_parser.cpp"
    break;

  case 291:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6470 "ds_parser.cpp"
    break;

  case 292:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6476 "ds_parser.cpp"
    break;

  case 293:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6482 "ds_parser.cpp"
    break;

  case 294:
#line 1553 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6488 "ds_parser.cpp"
    break;

  case 295:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6494 "ds_parser.cpp"
    break;

  case 296:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6500 "ds_parser.cpp"
    break;

  case 297:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6506 "ds_parser.cpp"
    break;

  case 298:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6512 "ds_parser.cpp"
    break;

  case 299:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6518 "ds_parser.cpp"
    break;

  case 300:
#line 1559 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6524 "ds_parser.cpp"
    break;

  case 301:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6530 "ds_parser.cpp"
    break;

  case 302:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6536 "ds_parser.cpp"
    break;

  case 303:
#line 1562 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6545 "ds_parser.cpp"
    break;

  case 304:
#line 1566 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6554 "ds_parser.cpp"
    break;

  case 305:
#line 1570 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6562 "ds_parser.cpp"
    break;

  case 306:
#line 1573 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6570 "ds_parser.cpp"
    break;

  case 307:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6576 "ds_parser.cpp"
    break;

  case 308:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6582 "ds_parser.cpp"
    break;

  case 309:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6588 "ds_parser.cpp"
    break;

  case 310:
#line 1579 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6602 "ds_parser.cpp"
    break;

  case 311:
#line 1588 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6616 "ds_parser.cpp"
    break;

  case 312:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6622 "ds_parser.cpp"
    break;

  case 313:
#line 1598 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6630 "ds_parser.cpp"
    break;

  case 314:
#line 1601 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6636 "ds_parser.cpp"
    break;

  case 315:
#line 1601 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6642 "ds_parser.cpp"
    break;

  case 316:
#line 1601 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6650 "ds_parser.cpp"
    break;

  case 317:
#line 1604 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6659 "ds_parser.cpp"
    break;

  case 318:
#line 1608 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6668 "ds_parser.cpp"
    break;

  case 319:
#line 1612 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6677 "ds_parser.cpp"
    break;

  case 320:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6683 "ds_parser.cpp"
    break;

  case 321:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6689 "ds_parser.cpp"
    break;

  case 322:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6695 "ds_parser.cpp"
    break;

  case 323:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6701 "ds_parser.cpp"
    break;

  case 324:
#line 1620 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6707 "ds_parser.cpp"
    break;

  case 325:
#line 1621 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6713 "ds_parser.cpp"
    break;

  case 326:
#line 1622 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6719 "ds_parser.cpp"
    break;

  case 327:
#line 1623 "ds_parser.ypp"
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
#line 6740 "ds_parser.cpp"
    break;

  case 328:
#line 1639 "ds_parser.ypp"
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
#line 6761 "ds_parser.cpp"
    break;

  case 329:
#line 1655 "ds_parser.ypp"
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
#line 6818 "ds_parser.cpp"
    break;

  case 330:
#line 1710 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6824 "ds_parser.cpp"
    break;

  case 331:
#line 1711 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6830 "ds_parser.cpp"
    break;

  case 332:
#line 1715 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6836 "ds_parser.cpp"
    break;

  case 333:
#line 1716 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6842 "ds_parser.cpp"
    break;

  case 334:
#line 1720 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6852 "ds_parser.cpp"
    break;

  case 335:
#line 1728 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6860 "ds_parser.cpp"
    break;

  case 336:
#line 1731 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6869 "ds_parser.cpp"
    break;

  case 337:
#line 1736 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6875 "ds_parser.cpp"
    break;

  case 338:
#line 1736 "ds_parser.ypp"
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
#line 6924 "ds_parser.cpp"
    break;

  case 339:
#line 1780 "ds_parser.ypp"
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
#line 6985 "ds_parser.cpp"
    break;

  case 340:
#line 1836 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 6996 "ds_parser.cpp"
    break;

  case 341:
#line 1845 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7006 "ds_parser.cpp"
    break;

  case 342:
#line 1850 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7016 "ds_parser.cpp"
    break;

  case 343:
#line 1858 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7022 "ds_parser.cpp"
    break;

  case 344:
#line 1859 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7028 "ds_parser.cpp"
    break;

  case 345:
#line 1863 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7036 "ds_parser.cpp"
    break;

  case 346:
#line 1866 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7047 "ds_parser.cpp"
    break;

  case 347:
#line 1875 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7053 "ds_parser.cpp"
    break;

  case 348:
#line 1876 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7059 "ds_parser.cpp"
    break;

  case 349:
#line 1880 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7070 "ds_parser.cpp"
    break;

  case 350:
#line 1889 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7076 "ds_parser.cpp"
    break;

  case 351:
#line 1890 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7082 "ds_parser.cpp"
    break;

  case 352:
#line 1895 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7088 "ds_parser.cpp"
    break;

  case 353:
#line 1896 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7094 "ds_parser.cpp"
    break;

  case 354:
#line 1900 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7105 "ds_parser.cpp"
    break;

  case 355:
#line 1906 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7116 "ds_parser.cpp"
    break;

  case 356:
#line 1912 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7124 "ds_parser.cpp"
    break;

  case 357:
#line 1915 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7133 "ds_parser.cpp"
    break;

  case 358:
#line 1919 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7144 "ds_parser.cpp"
    break;

  case 359:
#line 1925 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7155 "ds_parser.cpp"
    break;

  case 360:
#line 1934 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7161 "ds_parser.cpp"
    break;

  case 361:
#line 1935 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7167 "ds_parser.cpp"
    break;

  case 362:
#line 1936 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7173 "ds_parser.cpp"
    break;

  case 363:
#line 1940 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7179 "ds_parser.cpp"
    break;

  case 364:
#line 1941 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7185 "ds_parser.cpp"
    break;

  case 365:
#line 1945 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7193 "ds_parser.cpp"
    break;

  case 366:
#line 1948 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7203 "ds_parser.cpp"
    break;

  case 367:
#line 1953 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7213 "ds_parser.cpp"
    break;

  case 368:
#line 1958 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7226 "ds_parser.cpp"
    break;

  case 369:
#line 1966 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7239 "ds_parser.cpp"
    break;

  case 370:
#line 1977 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7247 "ds_parser.cpp"
    break;

  case 371:
#line 1980 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7256 "ds_parser.cpp"
    break;

  case 372:
#line 1987 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7262 "ds_parser.cpp"
    break;

  case 373:
#line 1988 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7268 "ds_parser.cpp"
    break;

  case 374:
#line 1992 "ds_parser.ypp"
    { (yyval.i) = 0; }
#line 7274 "ds_parser.cpp"
    break;

  case 375:
#line 1993 "ds_parser.ypp"
    { (yyval.i) = 1; }
#line 7280 "ds_parser.cpp"
    break;

  case 376:
#line 1994 "ds_parser.ypp"
    { (yyval.i) = 2; }
#line 7286 "ds_parser.cpp"
    break;

  case 377:
#line 1998 "ds_parser.ypp"
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
#line 7319 "ds_parser.cpp"
    break;

  case 378:
#line 2026 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7325 "ds_parser.cpp"
    break;

  case 379:
#line 2026 "ds_parser.ypp"
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
#line 7357 "ds_parser.cpp"
    break;

  case 380:
#line 2056 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7365 "ds_parser.cpp"
    break;

  case 381:
#line 2059 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7379 "ds_parser.cpp"
    break;

  case 382:
#line 2068 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7393 "ds_parser.cpp"
    break;

  case 383:
#line 2081 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7407 "ds_parser.cpp"
    break;

  case 387:
#line 2102 "ds_parser.ypp"
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
#line 7423 "ds_parser.cpp"
    break;

  case 388:
#line 2113 "ds_parser.ypp"
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
#line 7440 "ds_parser.cpp"
    break;

  case 389:
#line 2128 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7446 "ds_parser.cpp"
    break;

  case 390:
#line 2129 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7452 "ds_parser.cpp"
    break;

  case 391:
#line 2133 "ds_parser.ypp"
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
#line 7491 "ds_parser.cpp"
    break;

  case 392:
#line 2170 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7497 "ds_parser.cpp"
    break;

  case 393:
#line 2171 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7503 "ds_parser.cpp"
    break;

  case 394:
#line 2175 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7509 "ds_parser.cpp"
    break;

  case 395:
#line 2175 "ds_parser.ypp"
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
#line 7595 "ds_parser.cpp"
    break;

  case 396:
#line 2259 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7607 "ds_parser.cpp"
    break;

  case 397:
#line 2266 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7618 "ds_parser.cpp"
    break;

  case 398:
#line 2275 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7624 "ds_parser.cpp"
    break;

  case 399:
#line 2276 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7630 "ds_parser.cpp"
    break;

  case 400:
#line 2277 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7636 "ds_parser.cpp"
    break;

  case 401:
#line 2278 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7642 "ds_parser.cpp"
    break;

  case 402:
#line 2279 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7648 "ds_parser.cpp"
    break;

  case 403:
#line 2280 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7654 "ds_parser.cpp"
    break;

  case 404:
#line 2281 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7660 "ds_parser.cpp"
    break;

  case 405:
#line 2282 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7666 "ds_parser.cpp"
    break;

  case 406:
#line 2283 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7672 "ds_parser.cpp"
    break;

  case 407:
#line 2284 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7678 "ds_parser.cpp"
    break;

  case 408:
#line 2285 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7684 "ds_parser.cpp"
    break;

  case 409:
#line 2286 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7690 "ds_parser.cpp"
    break;

  case 410:
#line 2287 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7696 "ds_parser.cpp"
    break;

  case 411:
#line 2288 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7702 "ds_parser.cpp"
    break;

  case 412:
#line 2289 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7708 "ds_parser.cpp"
    break;

  case 413:
#line 2290 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7714 "ds_parser.cpp"
    break;

  case 414:
#line 2291 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7720 "ds_parser.cpp"
    break;

  case 415:
#line 2292 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7726 "ds_parser.cpp"
    break;

  case 416:
#line 2293 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7732 "ds_parser.cpp"
    break;

  case 417:
#line 2294 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7738 "ds_parser.cpp"
    break;

  case 418:
#line 2295 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7744 "ds_parser.cpp"
    break;

  case 419:
#line 2296 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7750 "ds_parser.cpp"
    break;

  case 420:
#line 2297 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7756 "ds_parser.cpp"
    break;

  case 421:
#line 2298 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7762 "ds_parser.cpp"
    break;

  case 422:
#line 2299 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7768 "ds_parser.cpp"
    break;

  case 423:
#line 2303 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7774 "ds_parser.cpp"
    break;

  case 424:
#line 2304 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7780 "ds_parser.cpp"
    break;

  case 425:
#line 2305 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7786 "ds_parser.cpp"
    break;

  case 426:
#line 2306 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7792 "ds_parser.cpp"
    break;

  case 427:
#line 2307 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7798 "ds_parser.cpp"
    break;

  case 428:
#line 2308 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7804 "ds_parser.cpp"
    break;

  case 429:
#line 2312 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7817 "ds_parser.cpp"
    break;

  case 430:
#line 2323 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7826 "ds_parser.cpp"
    break;

  case 431:
#line 2327 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7838 "ds_parser.cpp"
    break;

  case 432:
#line 2337 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7850 "ds_parser.cpp"
    break;

  case 433:
#line 2344 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7861 "ds_parser.cpp"
    break;

  case 434:
#line 2353 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7867 "ds_parser.cpp"
    break;

  case 435:
#line 2353 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7873 "ds_parser.cpp"
    break;

  case 436:
#line 2353 "ds_parser.ypp"
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
#line 7888 "ds_parser.cpp"
    break;

  case 437:
#line 2366 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7894 "ds_parser.cpp"
    break;

  case 438:
#line 2367 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7900 "ds_parser.cpp"
    break;

  case 439:
#line 2368 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7906 "ds_parser.cpp"
    break;

  case 440:
#line 2369 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7912 "ds_parser.cpp"
    break;

  case 441:
#line 2370 "ds_parser.ypp"
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
#line 7931 "ds_parser.cpp"
    break;

  case 442:
#line 2384 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7942 "ds_parser.cpp"
    break;

  case 443:
#line 2390 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7951 "ds_parser.cpp"
    break;

  case 444:
#line 2394 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7960 "ds_parser.cpp"
    break;

  case 445:
#line 2398 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7970 "ds_parser.cpp"
    break;

  case 446:
#line 2403 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7980 "ds_parser.cpp"
    break;

  case 447:
#line 2408 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7990 "ds_parser.cpp"
    break;

  case 448:
#line 2413 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8000 "ds_parser.cpp"
    break;

  case 449:
#line 2418 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8009 "ds_parser.cpp"
    break;

  case 450:
#line 2422 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8018 "ds_parser.cpp"
    break;

  case 451:
#line 2426 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8028 "ds_parser.cpp"
    break;

  case 452:
#line 2431 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8037 "ds_parser.cpp"
    break;

  case 453:
#line 2435 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8047 "ds_parser.cpp"
    break;

  case 454:
#line 2440 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8053 "ds_parser.cpp"
    break;

  case 455:
#line 2440 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8059 "ds_parser.cpp"
    break;

  case 456:
#line 2440 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8070 "ds_parser.cpp"
    break;

  case 457:
#line 2446 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8082 "ds_parser.cpp"
    break;

  case 458:
#line 2453 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8088 "ds_parser.cpp"
    break;

  case 459:
#line 2453 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8094 "ds_parser.cpp"
    break;

  case 460:
#line 2453 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8104 "ds_parser.cpp"
    break;

  case 461:
#line 2458 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8110 "ds_parser.cpp"
    break;

  case 462:
#line 2458 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8116 "ds_parser.cpp"
    break;

  case 463:
#line 2458 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8127 "ds_parser.cpp"
    break;

  case 464:
#line 2464 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8133 "ds_parser.cpp"
    break;

  case 465:
#line 2464 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8139 "ds_parser.cpp"
    break;

  case 466:
#line 2464 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8149 "ds_parser.cpp"
    break;

  case 467:
#line 2469 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8158 "ds_parser.cpp"
    break;

  case 468:
#line 2473 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8164 "ds_parser.cpp"
    break;

  case 469:
#line 2473 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8170 "ds_parser.cpp"
    break;

  case 470:
#line 2473 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8180 "ds_parser.cpp"
    break;

  case 471:
#line 2478 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8186 "ds_parser.cpp"
    break;

  case 472:
#line 2478 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8192 "ds_parser.cpp"
    break;

  case 473:
#line 2478 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8206 "ds_parser.cpp"
    break;

  case 474:
#line 2487 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8215 "ds_parser.cpp"
    break;

  case 475:
#line 2491 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8221 "ds_parser.cpp"
    break;

  case 476:
#line 2491 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8227 "ds_parser.cpp"
    break;

  case 477:
#line 2491 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8237 "ds_parser.cpp"
    break;

  case 478:
#line 2496 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8243 "ds_parser.cpp"
    break;

  case 479:
#line 2496 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8249 "ds_parser.cpp"
    break;

  case 480:
#line 2496 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8263 "ds_parser.cpp"
    break;

  case 481:
#line 2505 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8272 "ds_parser.cpp"
    break;

  case 482:
#line 2509 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8278 "ds_parser.cpp"
    break;

  case 483:
#line 2509 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8284 "ds_parser.cpp"
    break;

  case 484:
#line 2509 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8294 "ds_parser.cpp"
    break;

  case 485:
#line 2514 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8300 "ds_parser.cpp"
    break;

  case 486:
#line 2514 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8306 "ds_parser.cpp"
    break;

  case 487:
#line 2514 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8320 "ds_parser.cpp"
    break;

  case 488:
#line 2523 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8326 "ds_parser.cpp"
    break;

  case 489:
#line 2523 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8332 "ds_parser.cpp"
    break;

  case 490:
#line 2523 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8343 "ds_parser.cpp"
    break;

  case 491:
#line 2529 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8349 "ds_parser.cpp"
    break;

  case 492:
#line 2529 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8355 "ds_parser.cpp"
    break;

  case 493:
#line 2529 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8366 "ds_parser.cpp"
    break;

  case 494:
#line 2538 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8372 "ds_parser.cpp"
    break;

  case 495:
#line 2538 "ds_parser.ypp"
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
#line 8389 "ds_parser.cpp"
    break;

  case 496:
#line 2553 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8395 "ds_parser.cpp"
    break;

  case 497:
#line 2553 "ds_parser.ypp"
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
#line 8416 "ds_parser.cpp"
    break;

  case 498:
#line 2573 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8422 "ds_parser.cpp"
    break;

  case 499:
#line 2574 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8428 "ds_parser.cpp"
    break;

  case 500:
#line 2575 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8434 "ds_parser.cpp"
    break;

  case 501:
#line 2576 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8440 "ds_parser.cpp"
    break;

  case 502:
#line 2580 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8452 "ds_parser.cpp"
    break;

  case 503:
#line 2587 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8464 "ds_parser.cpp"
    break;

  case 504:
#line 2594 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8475 "ds_parser.cpp"
    break;

  case 505:
#line 2600 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8486 "ds_parser.cpp"
    break;

  case 506:
#line 2609 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8496 "ds_parser.cpp"
    break;

  case 507:
#line 2614 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8505 "ds_parser.cpp"
    break;

  case 508:
#line 2621 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8511 "ds_parser.cpp"
    break;

  case 509:
#line 2622 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8517 "ds_parser.cpp"
    break;

  case 510:
#line 2626 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8528 "ds_parser.cpp"
    break;

  case 511:
#line 2632 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8540 "ds_parser.cpp"
    break;

  case 512:
#line 2639 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8553 "ds_parser.cpp"
    break;

  case 513:
#line 2647 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8565 "ds_parser.cpp"
    break;

  case 514:
#line 2654 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8578 "ds_parser.cpp"
    break;

  case 515:
#line 2662 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8592 "ds_parser.cpp"
    break;

  case 516:
#line 2674 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8600 "ds_parser.cpp"
    break;

  case 517:
#line 2677 "ds_parser.ypp"
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
#line 8616 "ds_parser.cpp"
    break;

  case 518:
#line 2691 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8627 "ds_parser.cpp"
    break;

  case 519:
#line 2700 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8633 "ds_parser.cpp"
    break;

  case 520:
#line 2701 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8639 "ds_parser.cpp"
    break;

  case 521:
#line 2705 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8649 "ds_parser.cpp"
    break;

  case 522:
#line 2710 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8658 "ds_parser.cpp"
    break;

  case 523:
#line 2717 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8668 "ds_parser.cpp"
    break;

  case 524:
#line 2722 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8680 "ds_parser.cpp"
    break;

  case 525:
#line 2732 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8690 "ds_parser.cpp"
    break;

  case 526:
#line 2737 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8699 "ds_parser.cpp"
    break;

  case 527:
#line 2744 "ds_parser.ypp"
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

  case 528:
#line 2756 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8719 "ds_parser.cpp"
    break;

  case 529:
#line 2757 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8725 "ds_parser.cpp"
    break;

  case 530:
#line 2761 "ds_parser.ypp"
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

  case 531:
#line 2779 "ds_parser.ypp"
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
#line 2799 "ds_parser.ypp"


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
