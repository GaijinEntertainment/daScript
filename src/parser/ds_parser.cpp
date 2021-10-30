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
#  define DAS_YYDEBUG 0
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
    STRING_CHARACTER_ESC = 403,
    END_STRING = 404,
    BEGIN_STRING_EXPR = 405,
    END_STRING_EXPR = 406,
    END_OF_READ = 407,
    UNARY_MINUS = 408,
    UNARY_PLUS = 409,
    PRE_INC = 410,
    PRE_DEC = 411,
    POST_INC = 412,
    POST_DEC = 413,
    DEREF = 414
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

#line 409 "ds_parser.cpp"

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
#define YYLAST   7729

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  180
/* YYNRULES -- Number of rules.  */
#define YYNRULES  562
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  983

#define YYUNDEFTOK  2
#define YYMAXUTOK   414

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
       2,     2,     2,   170,     2,   186,   181,   166,   159,     2,
     179,   180,   164,   163,   153,   162,   176,   165,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   156,   183,
     160,   154,   161,   155,   182,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   177,     2,   178,   158,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   184,   157,   185,   169,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   151,   152,   167,   168,
     171,   172,   173,   174,   175
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   454,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   470,   471,   472,   476,   477,
     481,   498,   499,   500,   501,   505,   509,   514,   523,   531,
     547,   552,   560,   560,   590,   612,   616,   619,   623,   629,
     638,   659,   682,   683,   687,   691,   692,   696,   699,   705,
     711,   714,   720,   721,   725,   726,   727,   736,   737,   741,
     750,   766,   774,   784,   793,   794,   795,   796,   797,   798,
     802,   807,   815,   816,   817,   821,   822,   823,   824,   825,
     826,   827,   828,   834,   848,   865,   866,   867,   871,   879,
     892,   896,   903,   904,   908,   909,   910,   914,   917,   924,
     928,   929,   930,   931,   932,   933,   934,   935,   936,   937,
     938,   939,   940,   941,   942,   943,   944,   945,   946,   947,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,   961,   962,   963,   964,   965,   966,   967,
     968,   969,   970,   971,   972,   973,   977,   999,  1000,  1001,
    1005,  1044,  1051,  1055,  1066,  1067,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1088,  1093,  1099,  1105,  1139,  1142,  1148,
    1149,  1160,  1164,  1170,  1173,  1176,  1180,  1186,  1190,  1194,
    1197,  1200,  1205,  1208,  1216,  1219,  1224,  1227,  1235,  1241,
    1242,  1246,  1280,  1280,  1280,  1283,  1283,  1283,  1288,  1288,
    1288,  1296,  1296,  1296,  1302,  1312,  1323,  1338,  1341,  1347,
    1348,  1355,  1366,  1367,  1368,  1372,  1373,  1374,  1375,  1379,
    1384,  1392,  1393,  1397,  1402,  1466,  1467,  1468,  1469,  1470,
    1471,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,
    1497,  1498,  1499,  1500,  1504,  1515,  1520,  1530,  1534,  1534,
    1534,  1541,  1541,  1541,  1555,  1559,  1563,  1563,  1563,  1570,
    1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,
    1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1612,  1613,  1614,  1615,  1616,  1620,  1624,  1627,  1630,
    1631,  1632,  1633,  1642,  1651,  1652,  1655,  1655,  1655,  1658,
    1662,  1666,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
    1693,  1709,  1767,  1768,  1772,  1773,  1777,  1778,  1782,  1790,
    1793,  1798,  1797,  1842,  1902,  1911,  1916,  1924,  1925,  1929,
    1932,  1941,  1942,  1946,  1955,  1956,  1961,  1962,  1966,  1972,
    1978,  1981,  1985,  1991,  2000,  2001,  2002,  2006,  2007,  2011,
    2014,  2019,  2024,  2032,  2043,  2046,  2054,  2055,  2059,  2060,
    2061,  2065,  2093,  2093,  2127,  2130,  2139,  2152,  2164,  2165,
    2169,  2170,  2174,  2175,  2176,  2180,  2208,  2240,  2241,  2245,
    2283,  2284,  2288,  2289,  2290,  2295,  2294,  2379,  2386,  2395,
    2396,  2397,  2398,  2399,  2400,  2401,  2402,  2403,  2404,  2405,
    2406,  2407,  2408,  2409,  2410,  2411,  2412,  2413,  2414,  2415,
    2416,  2417,  2418,  2419,  2423,  2424,  2425,  2426,  2427,  2428,
    2432,  2443,  2447,  2457,  2464,  2473,  2473,  2473,  2486,  2487,
    2488,  2489,  2490,  2504,  2510,  2514,  2518,  2523,  2528,  2533,
    2538,  2542,  2546,  2551,  2555,  2560,  2560,  2560,  2566,  2573,
    2573,  2573,  2578,  2578,  2578,  2584,  2584,  2584,  2589,  2593,
    2593,  2593,  2598,  2598,  2598,  2607,  2611,  2611,  2611,  2616,
    2616,  2616,  2625,  2629,  2629,  2629,  2634,  2634,  2634,  2643,
    2643,  2643,  2649,  2649,  2649,  2658,  2658,  2673,  2673,  2693,
    2694,  2695,  2696,  2700,  2707,  2714,  2720,  2729,  2734,  2741,
    2742,  2746,  2752,  2759,  2767,  2774,  2782,  2794,  2797,  2811,
    2820,  2821,  2825,  2830,  2837,  2842,  2852,  2857,  2864,  2876,
    2877,  2881,  2899
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
  "STRING_CHARACTER", "STRING_CHARACTER_ESC", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"", "','", "'='", "'?'",
  "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'",
  "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
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
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
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
  "optional_field_annotation", "optional_override", "optional_constant",
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
     405,   406,   407,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   408,   409,   126,
      33,   410,   411,   412,   413,   414,    46,    91,    93,    40,
      41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -598

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-598)))

#define YYTABLE_NINF -517

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -598,    29,  -598,  -598,    22,   -62,   150,     1,  -598,   -63,
    -598,  -598,    -5,  -598,  -598,  -598,  -598,  -598,   222,  -598,
      93,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
      37,  -598,    47,   104,    98,  -598,  -598,   150,    12,  -598,
     134,   145,  -598,  -598,  -598,    93,   160,   169,  -598,  -598,
     173,   148,   177,  -598,    58,  -598,  -598,  -598,    95,   330,
     367,  -598,   384,    16,    22,   248,   -62,   171,   273,  -598,
     303,   312,  -598,  7465,   239,   -65,   399,   316,   319,  -598,
     351,    22,    -5,  -598,  -598,  -598,     5,   325,  -598,  -598,
    -598,   353,  -598,  -598,   365,  -598,  -598,   328,  -598,  -598,
    -598,  -598,  -598,   123,    75,  -598,  -598,  -598,  -598,   460,
    -598,  -598,   354,   355,   356,   357,  -598,  -598,  -598,   339,
    -598,  -598,  -598,  -598,  -598,   360,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,   364,  -598,  -598,  -598,   370,
     372,  -598,  -598,  -598,  -598,   373,   374,  -598,  -598,  -598,
    -598,  -598,  2490,  -598,  -598,   342,  -598,  -598,  -598,   378,
     382,  -598,    45,  -598,  7552,   503,   504,  -598,   361,  -598,
    -598,    68,   383,  -598,  -598,   405,  -598,  -598,    55,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,   105,  -598,  -598,  -598,
    -598,  -598,   397,  -598,   205,   232,   234,  -598,  -598,  -598,
    -598,  -598,  -598,   511,  -598,  -598,    15,  2970,  -598,  -598,
    -598,   362,   388,  -598,   366,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,   376,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,   377,
    -598,  -598,   -88,   392,  1282,   340,  -598,   263,  -598,   368,
     -81,    22,   406,  -598,  -598,  -598,    75,  -598,  7465,  7465,
    7465,  7465,   381,   378,  7465,   361,  7465,   361,  7465,   361,
    7551,   382,  -598,  -598,  -598,   380,  -598,  -598,  -598,  7379,
     371,   393,  -598,   385,   396,   402,   386,   409,  -598,   411,
    4607,  4607,  7204,  7293,  4607,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  4607,  4607,  4607,   263,  4607,  4607,  -598,  4607,
    -598,   390,  -598,  -598,  -598,   -16,  -598,  -598,  -598,  -598,
     389,  -598,  -598,  -598,  -598,  -598,  -598,  5993,   394,  -598,
    -598,  -598,  -598,  -598,   -40,  7465,   -38,  -598,  -598,  -598,
      -3,  -598,   287,  7465,  -598,  -598,  4607,  -598,  -598,   406,
    2098,  -598,   325,  4607,  4607,  -598,   391,   436,  3109,   325,
    2257,  -108,  -108,  -598,   540,   398,   400,  4607,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,   401,   403,   408,  -598,  -598,
    -598,   406,  -598,   -68,  -598,  6485,  -598,  -598,  -598,  -598,
    -598,  -598,   395,   -17,  -598,  -598,  -598,   406,   181,  -598,
    -598,   313,  -598,    10,   208,   479,   490,  -598,   -90,   569,
     392,   603,   392,   623,   392,   133,  -598,   -25,  2490,   185,
    -598,   842,  -598,   263,  -598,  4607,  -598,  -598,  4607,  -598,
    7465,   132,   132,   406,  1627,   406,  1784,  6553,  -598,   288,
     333,   132,   132,   -33,  -598,   132,   132,  5441,   182,  -598,
    2396,   441,    41,   437,  4607,  4607,  -598,  -598,  4607,  4607,
    4607,  4607,   444,  4607,   447,  4607,  4607,  4607,  4607,  4607,
    4607,  4607,  4607,  3248,  4607,  4607,  4607,  4607,  4607,  4607,
    4607,  4607,  4607,  4607,   -60,  4607,  -598,  3387,  -598,  -598,
    2490,  -598,  -598,   406,  -598,   218,  -598,   405,  2490,   988,
      13,  2553,  -598,  -598,   476,  4697,   583,  6779,   988,  -598,
     446,   459,  6779,  -598,  2553,  -598,  4796,   421,  -598,  -598,
     422,  -598,  -598,   988,  -598,  -598,  -598,  -598,  -108,  -598,
    4607,  4607,  4607,  4607,  4607,  4607,  4607,  4607,  4607,  4607,
    2692,  4607,  4607,  4607,  4607,  4607,  4607,  2831,  -598,    92,
    -598,    20,  -598,   431,   466,  7465,  -598,   -51,  -598,  7465,
    -598,  -598,  -598,   467,  -598,   452,  -598,   453,  -598,   455,
    7465,  -598,  7551,  -598,   382,  3544,  3701,  7465,  5519,  7465,
    7465,  5599,  7465,   910,    25,  -108,   -35,  1941,  3840,  6553,
     468,   -13,   451,   471,  -598,  -598,   277,    31,  3997,   -13,
     314,  4607,  4607,   434,  -598,  4607,   217,   483,  -598,   240,
    -598,   487,  -598,   127,  6779,   -45,   361,   477,  -598,  -598,
      17,    17,   299,   299,  7138,  7138,   457,   223,  -598,  6069,
     -64,   -64,    17,    17,   329,  6847,  6926,   494,  6632,  6954,
    7032,  7060,   299,   299,   117,   117,   223,   223,   223,   497,
    4607,  -598,   498,  4607,   645,  6145,  -598,   168,  -598,  -598,
    -598,  7465,  -598,  2553,  -598,  -598,  4607,  -598,  4895,  4607,
    4607,  -598,   325,  -598,  -598,  -598,  -598,  4994,  -598,  -598,
    -598,   375,  -598,  6779,  6779,  6779,  6779,  6779,  6779,  6779,
    6779,  6779,  6779,  4607,  4607,  6779,  6779,  6779,  6779,  6779,
    6779,  6779,  4607,  4607,  6779,     4,  4607,  -598,    -5,  -598,
     640,   606,   470,  -598,  -598,   445,  -598,  -598,  -598,  2553,
    -598,  1027,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  2490,  -598,  -598,  -598,  -598,   172,   505,  5677,  1065,
    -598,  1079,  1567,  -598,  1878,   441,  4607,  -598,  4607,  4607,
      -4,   509,   487,   480,   484,  4607,   485,  4607,  4607,   487,
     472,   486,  6779,  -598,  -598,  6700,  -598,   514,  7465,   361,
     -28,   191,  4607,  -598,   521,   523,   525,   526,  -598,   202,
     392,  -598,  4154,  -598,  -598,  4607,  -598,  6221,  -598,  6297,
    -598,  -598,  -598,  2039,  -598,  6385,   -52,  -598,  -598,  -598,
    1467,   325,  -598,  -598,  4607,  -598,   778,  6779,   778,  6779,
    -598,  5291,   204,   442,  -598,   406,  -598,   511,  -598,  2553,
    -598,  5093,  -598,  -598,  -598,  -598,  -598,   192,  -598,  -598,
    -598,  -598,  -598,   493,    78,  6779,  6779,   -13,   500,   -27,
     468,   510,  -598,  6779,  -598,  -598,   103,   -13,   513,  -598,
    -598,  -598,  2197,   392,   495,  6779,  -598,  -598,  -598,  -598,
     -45,   515,   -82,  7465,  -598,   180,  6779,  -598,  -598,  -598,
     511,  4607,   325,  -598,  -598,   988,  -598,   499,  -598,   649,
    -598,  -598,  5192,  -598,  -598,  4607,   538,  -598,  4607,  4607,
    4607,  4311,  4607,   516,   517,  4607,  4607,  -598,  4607,   512,
    -598,  -598,   535,  -598,  -598,  -598,  4468,  -598,  -598,  2333,
    -598,  6779,  -598,   375,  -598,    95,  -598,    95,  -598,  5757,
     537,   299,   299,   299,  -598,  5835,  5366,   522,  -598,  6779,
    6779,  5366,   524,   263,  -598,  4607,  6779,  -598,  -598,   518,
    -598,  -598,  4607,  -598,   654,   527,  -598,   519,  -598,  -598,
     263,  6779,  -598,  -598,  5915,  4607,   528,   529,  -598,  -598,
    6779,  -598,  -598
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   199,     0,     0,     0,     0,   200,     0,
     527,   525,     0,    14,     3,    10,     9,     8,     0,     7,
     396,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   411,    19,    18,   396,     0,     0,    86,    87,
       0,   179,    88,    90,     0,    85,   421,   420,   147,   412,
     422,   397,   398,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   181,
       0,     0,     0,    93,   149,   148,     0,     0,   146,   414,
     413,     0,   424,   423,     0,   400,   399,   402,    79,    80,
      77,    78,    76,     0,     0,    75,    84,    48,    46,    42,
      39,    38,     0,     0,     0,     0,   429,   449,   430,   461,
     431,   435,   436,   437,   438,   453,   442,   443,   444,   445,
     446,   447,   448,   450,   451,   498,   434,   441,   452,   505,
     512,   432,   439,   433,   440,     0,     0,   460,   468,   471,
     469,   470,   407,   408,   410,     0,    16,    17,    20,     0,
       0,   180,     0,    91,     0,     0,     0,    99,    94,   173,
     151,     0,   417,   425,   394,   352,    21,    22,     0,    68,
      69,    66,    67,    65,    64,    70,     0,    41,   489,   492,
     495,   485,     0,   465,   499,   506,   513,   519,   522,   476,
     481,   475,   488,     0,   484,   478,     0,     0,   480,   409,
     463,     0,     0,   374,     0,    89,   102,   103,   105,   104,
     106,   107,   108,   109,   134,   135,   132,   133,   125,   136,
     137,   126,   123,   124,   145,     0,   138,   139,   140,   141,
     113,   114,   115,   110,   111,   112,   128,   129,   127,   121,
     122,   117,   116,   118,   119,   120,   101,   100,   144,     0,
     130,   131,   352,    97,     0,     0,   404,     0,   419,     0,
     352,     0,     0,    23,    24,    25,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    94,     0,    94,     0,    94,
       0,     0,   483,   477,   479,     0,   482,   285,   286,     0,
       0,     0,   279,     0,     0,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,   235,   237,   236,   238,   239,
     240,    26,     0,     0,     0,     0,     0,     0,   473,     0,
     222,   223,   283,   282,   233,   280,   345,   344,   343,   342,
      92,   348,   281,   347,   346,   324,   287,     0,     0,   284,
     529,   530,   531,   532,     0,     0,     0,   143,   142,    95,
       0,   367,     0,     0,   150,   175,     0,    57,    58,     0,
       0,   187,     0,     0,     0,   188,     0,     0,     0,     0,
       0,     0,     0,   154,   152,     0,     0,     0,   168,   163,
     161,   160,   162,   174,   155,     0,     0,     0,   166,   167,
     169,     0,   159,     0,   156,   241,   454,   457,   455,   458,
     456,   459,     0,     0,   418,   359,   401,     0,     0,   427,
     403,   387,    71,     0,     0,     0,     0,   462,     0,     0,
      97,     0,    97,     0,    97,   179,   371,     0,   369,     0,
     474,   183,   186,     0,   211,     0,   202,   205,     0,   208,
       0,   313,   314,     0,   539,     0,     0,     0,   556,     0,
       0,   291,   290,   329,    32,   289,   288,     0,   224,   351,
       0,   231,     0,     0,     0,     0,   315,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,   472,     0,   464,   528,
     373,   526,   375,     0,   365,   378,    96,   352,    98,     0,
       0,     0,   189,   192,     0,   241,     0,   182,     0,   172,
       0,     0,    51,    61,     0,   196,   241,   223,   177,   178,
       0,   170,   171,     0,   158,   164,   165,   201,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,     0,
     415,   352,   395,     0,     0,     0,   388,     0,   490,     0,
     496,   486,   466,     0,   500,     0,   507,     0,   514,     0,
       0,   520,     0,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   179,     0,     0,   547,
     537,   539,     0,   550,   551,   552,     0,     0,     0,   539,
       0,     0,     0,     0,    29,     0,    27,     0,   317,   268,
     267,     0,   325,     0,   217,     0,    94,     0,   339,   340,
     292,   293,   305,   306,   303,   304,     0,   334,   322,     0,
     349,   350,   294,   295,   310,   311,   312,     0,     0,   308,
     309,   307,   301,   302,   297,   296,   298,   299,   300,     0,
       0,   274,     0,     0,     0,     0,   327,     0,   366,   377,
     376,     0,   379,     0,   368,    62,     0,   193,   241,     0,
       0,   190,     0,    63,    49,    50,   197,   241,   194,   224,
     173,    54,   176,   251,   252,   254,   253,   255,   245,   246,
     247,   256,   257,     0,     0,   243,   244,   258,   259,   248,
     249,   250,     0,     0,   242,     0,     0,   405,     0,   426,
       0,   354,     0,   353,   428,     0,   385,   386,   384,     0,
     491,     0,   497,   487,   467,   501,   503,   508,   510,   515,
     517,   370,   521,   372,   524,   184,     0,     0,     0,     0,
     330,     0,     0,   331,     0,   231,     0,   540,     0,     0,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   549,   557,   558,     0,    30,    33,     0,    94,
       0,     0,     0,   326,     0,     0,     0,     0,   229,     0,
      97,   336,     0,   320,   341,     0,   323,     0,   275,     0,
     277,   318,   328,   380,   383,   382,     0,   191,   198,   195,
       0,     0,    52,    53,     0,    59,   262,   263,   260,   261,
     416,     0,     0,   354,   355,     0,   360,   384,   389,     0,
     393,   241,   493,   504,   511,   518,   185,     0,   214,   212,
     203,   206,   209,     0,     0,   534,   533,   539,     0,     0,
     538,     0,   542,   548,   554,   553,     0,   539,     0,   555,
      28,    31,     0,    97,     0,   218,   227,   228,   226,   225,
       0,     0,     0,     0,   265,     0,   335,   321,   319,   278,
     376,     0,     0,   153,    55,     0,   406,    93,   361,   356,
     358,   391,   241,   392,   494,     0,     0,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,     0,     0,
     545,   269,     0,   264,   230,   232,     0,   219,   234,     0,
     266,   381,    60,    54,   364,   147,   357,   147,   390,     0,
       0,   204,   207,   210,   332,     0,   559,     0,   543,   536,
     535,   559,     0,     0,   272,     0,   220,   337,    56,     0,
     363,   215,     0,   333,     0,     0,   544,     0,   546,   270,
       0,   221,   338,   362,     0,     0,     0,     0,   273,   216,
     560,   561,   562
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -598,  -598,  -598,  -598,  -598,   249,   647,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,   671,  -598,   602,  -598,  -598,   646,
    -598,  -598,  -598,  -220,  -598,  -598,  -598,  -598,  -598,  -598,
     438,  -598,  -598,   651,   -55,  -598,   637,    -8,  -318,  -282,
    -424,  -598,  -598,  -598,  -214,  -215,   -69,  -598,    26,  -361,
     -12,  -598,  -598,  -598,  -598,  -598,  -598,  -598,   724,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -487,  -598,  -598,  -153,  -598,   -31,  -358,  -598,  -263,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,   363,  -598,  -598,  -164,  -101,  -598,  -598,  -598,  -598,
     225,  -598,   143,  -598,  -335,   448,  -501,  -498,     9,  -598,
       8,  -598,   695,  -598,  -598,  -598,   179,    23,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -356,   -71,
    -598,  -598,  -598,   462,  -598,  -598,  -598,    18,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,  -598,
    -598,  -598,  -598,  -598,  -598,  -598,   449,  -597,  -444,  -594,
    -598,  -598,  -298,   -26,   293,  -598,  -598,  -598,  -198,  -598
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   158,    45,    14,   178,   184,   460,   332,   787,
     333,   627,    15,    16,    38,    39,    72,    17,    34,    35,
     385,   386,   824,   825,   387,   388,   389,   390,   391,   392,
     185,   186,    30,    31,    32,    52,    53,    54,    18,   263,
     364,   168,    19,    86,    87,    88,   334,   393,   264,   394,
     335,   395,   336,   396,   397,   398,   399,   400,   401,   402,
     337,   599,   908,   600,   909,   602,   910,   338,   597,   907,
     339,   633,   928,   340,   798,   799,   636,   341,   342,   524,
     404,   343,   344,   345,   788,   953,   789,   970,   346,   674,
     889,   634,   883,   972,   360,   835,   937,   732,   571,   935,
     361,   362,   436,   437,   213,   214,   769,   514,   739,   577,
     420,   270,    62,    97,    21,   175,   413,    42,    75,    22,
      91,    23,   268,   173,    60,    94,    24,   269,   421,   348,
     412,   149,   150,   211,   151,   283,   744,   438,   281,   743,
     278,   740,   279,   904,   280,   742,   284,   745,   285,   843,
     286,   747,   287,   844,   288,   749,   289,   845,   290,   752,
     291,   754,    25,    47,    26,    46,   349,   610,   611,   612,
     350,   613,   614,   615,   616,   351,   459,   352,   965,   353
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   403,   148,   430,   515,   432,   585,   434,   587,   523,
     589,   272,   619,   520,   683,   678,   458,   773,   170,   535,
     677,   512,   471,   538,   539,   780,   162,   -92,    48,     2,
      98,    99,   686,    68,   791,     3,   605,   472,   473,   164,
      27,    28,   199,   513,   766,   605,   926,   293,   271,   482,
     778,   548,   484,   485,    49,   271,   669,   670,     4,   637,
       5,   147,     6,   736,    74,    69,   200,   201,     7,   794,
      55,   582,   737,   330,   537,     8,   169,    33,   795,   679,
      40,     9,    43,   484,   485,   671,   679,   679,   768,   179,
     180,   152,   359,   583,    80,   768,   915,   604,   155,   617,
      10,   792,   169,   738,   416,   508,   417,   212,   756,   796,
     165,   166,   504,   505,   797,   549,   672,   673,    44,   680,
     154,    11,   476,   477,    50,   202,   680,   680,   569,   469,
     482,   892,   483,   484,   485,   486,   591,   472,   473,   487,
      51,   790,   419,   504,   505,   509,    40,   511,    84,   569,
     167,    61,   472,   473,    85,   100,   271,   515,   592,   101,
     687,   102,   103,   470,   203,   204,   574,    29,   570,   205,
     772,   578,   206,   696,   294,   860,   858,    70,   574,   499,
     500,   501,   502,   503,   574,    41,   638,   207,    71,   830,
     702,    63,   295,   504,   505,   104,   208,   728,    64,   816,
      64,   296,   273,   274,   275,   729,    12,   148,   148,   148,
     148,    82,    13,   148,   181,   148,   418,   148,   182,   148,
     183,   103,   476,   477,   265,   215,    56,    57,   148,    58,
     482,   792,   483,   484,   485,   486,    83,   476,   477,   487,
     199,   148,   148,   472,   473,   482,   726,   767,   484,   485,
     486,    66,   266,   730,   487,   414,   792,    59,   276,   512,
      65,   912,    80,   913,   200,   201,   147,   147,   147,   147,
     176,   177,   147,   919,   147,   727,   147,    80,   147,   854,
     792,   501,   502,   503,   148,   277,   918,   147,    73,   590,
      40,   866,   148,   504,   505,    36,   423,   424,   425,   426,
     147,   147,   429,   526,   431,    77,   433,   793,   504,   505,
     533,    50,   891,   464,    78,   885,    37,   441,    79,   472,
     473,   792,   814,   202,   783,   792,   857,    51,   476,   477,
     454,   456,   679,   792,    64,   867,   482,   900,   483,   484,
     485,   486,   629,   147,   771,   487,   593,    71,   812,   472,
     473,   147,   846,   905,   800,   880,    81,    82,   916,   573,
     930,  -502,   203,   204,   273,   274,  -502,   205,   594,   874,
     206,   574,   680,   510,   681,   906,   882,   682,   840,   148,
     881,   518,   897,    89,  -502,   207,   821,   107,  -509,    90,
    -516,   579,    50,  -509,   208,  -516,  -271,   822,   823,   504,
     505,  -271,   474,   475,   476,   477,   406,   731,    51,   547,
     407,  -509,   482,  -516,   483,   484,   485,   486,   109,  -271,
      92,   487,   153,   488,   489,   572,    93,   408,   409,   410,
     411,   596,   474,   475,   476,   477,   478,    95,   147,   479,
     480,   481,   482,    96,   483,   484,   485,   486,   110,   922,
     685,   487,   156,   488,   489,   776,   630,   111,   157,   693,
     777,   499,   500,   501,   502,   503,   574,   516,   603,   575,
     517,   622,   576,   623,   701,   504,   505,   199,   901,   515,
     176,   177,   624,   625,   834,   898,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   161,   777,   171,   781,
     159,   200,   201,   160,   148,   504,   505,   873,   148,   169,
     172,   199,   174,    69,   188,   189,   190,   191,   192,   148,
     193,   148,   199,   210,   194,   209,   148,   212,   148,   148,
     195,   148,   196,   197,   198,   200,   201,   260,   261,   267,
     262,   271,   282,   292,   355,   354,   200,   201,   363,   356,
     443,   419,   415,   444,   357,   358,   446,   403,   440,   736,
     202,   427,   447,   147,   445,   448,    12,   147,   737,   449,
     347,   450,   468,   507,   529,   530,   540,   635,   147,   568,
     147,   541,   639,   542,   544,   147,   545,   147,   147,   646,
     147,   546,   648,   735,   202,   548,   692,   741,   695,   837,
     204,   199,   694,   699,   205,   202,   700,   206,   751,   733,
     148,   734,   508,   746,   748,   759,   750,   761,   762,   784,
     764,   771,   207,   818,   775,   200,   201,   405,   838,   774,
     786,   208,   790,   203,   204,   199,   802,   801,   205,   804,
     580,   206,   806,   808,   203,   204,   810,   833,   834,   205,
     847,   581,   206,   836,   859,   199,   207,   868,   861,   200,
     201,   871,   862,   864,   869,   208,   876,   207,   877,   147,
     878,   879,   911,   451,   452,   923,   208,   457,   914,   200,
     201,   936,   934,   940,   202,   461,   462,   463,   917,   465,
     466,   920,   467,   925,   947,   948,   954,   952,   962,   813,
     966,   973,   968,   975,   977,   976,   981,   982,    67,   626,
     105,   187,   108,   958,   422,   106,    55,   148,   202,   163,
     832,   959,   960,   203,   204,    20,   820,   924,   205,   519,
     584,   206,   899,   525,   853,   753,   527,   528,   202,   439,
      76,   532,   684,   536,   839,   428,   207,   725,   442,   620,
     543,   865,   894,   967,     0,   208,     0,   203,   204,     0,
       0,     0,   205,     0,   586,   206,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   203,   204,     0,
     207,     0,   205,     0,   588,   206,     0,     0,     0,   208,
       0,     0,     0,     0,     0,     0,     0,     0,   472,   473,
     207,     0,     0,     0,     0,     0,   872,     0,   598,   208,
       0,   601,   148,   927,     0,     0,     0,   609,     0,   609,
       0,     0,     0,   932,     0,     0,   933,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   640,   641,     0,
       0,   642,   643,   644,   645,     0,   647,     0,   649,   650,
     651,   652,   653,   654,   655,   656,   658,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,   675,     0,
       0,   147,     0,     0,   199,     0,     0,     0,     0,     0,
       0,   474,   475,     0,   688,   478,     0,     0,   479,   480,
     481,   482,     0,   483,   484,   485,   486,   697,   200,   201,
     487,   929,   488,   489,     0,     0,     0,     0,   490,   491,
     492,     0,     0,   703,   704,   705,   706,   707,   708,   709,
     710,   711,   712,   715,   716,   717,   718,   719,   720,   721,
     724,     0,     0,   493,     0,   494,   495,   496,   497,   498,
       0,   969,   199,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,   202,   978,   758,
       0,     0,     0,     0,     0,     0,   200,   201,     0,     0,
     461,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,   782,   457,     0,     0,   785,     0,
       0,     0,     0,     0,     0,     0,   203,   204,     0,     0,
       0,   205,     0,     0,   206,     0,     0,     0,   472,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,   595,     0,     0,     0,   202,     0,     0,   208,     0,
       0,     0,     0,   807,     0,     0,   809,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   815,     0,     0,     0,
       0,     0,   715,   724,     0,     0,     0,     0,     0,   199,
       0,     0,     0,     0,   203,   204,     0,     0,     0,   205,
       0,   765,   206,     0,     0,     0,   826,   827,     0,     0,
       0,     0,     0,   200,   201,   828,   829,   207,     0,   831,
       0,   474,   475,   476,   477,   478,   208,   199,   479,   480,
     481,   482,   841,   483,   484,   485,   486,     0,     0,     0,
     487,   199,   488,   489,     0,     0,     0,     0,   490,   491,
     492,   200,   201,     0,     0,     0,     0,     0,     0,     0,
       0,   855,   856,     0,     0,   200,   201,     0,   863,     0,
     609,     0,   202,   493,     0,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   875,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,   886,     0,
       0,     0,   169,     0,     0,     0,     0,     0,     0,     0,
     202,   203,   204,   405,     0,     0,   205,   895,   842,   206,
       0,     0,     0,     0,   202,     0,     0,     0,     0,     0,
       0,     0,   902,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,   208,     0,     0,     0,     0,     0,   203,
     204,     0,     0,     0,   205,     0,   849,   206,     0,     0,
       0,     0,     0,   203,   204,     0,     0,     0,   205,     0,
     850,   206,   207,     0,     0,     0,     0,     0,     0,     0,
       0,   208,     0,     0,   931,     0,   207,     0,     0,     0,
       0,     0,     0,     0,     0,   208,     0,     0,   939,     0,
       0,   941,   942,   943,   945,   946,     0,     0,   949,   950,
       0,   951,     0,   365,     0,     0,     0,     0,     3,   956,
     366,   367,   368,     0,   369,     0,   297,   298,   299,   300,
     301,     0,     0,     0,     0,     0,     0,   370,   302,   371,
     372,     0,     0,     0,     0,     0,     0,     0,   971,   373,
     303,     0,   374,   304,     0,   974,   305,     0,     8,   306,
     375,     0,   376,   307,     0,     0,   377,   378,   980,     0,
       0,     0,     0,   379,   116,   117,   118,     0,   120,   121,
     122,   123,   124,   308,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,   136,   137,   138,     0,     0,   141,
     142,   143,   144,     0,     0,   309,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,   311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   312,   313,
     314,   315,   316,   317,   318,   319,   320,    51,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,   323,   324,     0,   325,     0,
       0,   326,   327,     0,     0,     0,     0,     0,     0,     0,
       0,   329,     0,   330,   331,   383,   169,   384,   365,     0,
       0,     0,     0,     3,     0,   366,   367,   368,     0,   369,
       0,   297,   298,   299,   300,   301,     0,     0,     0,     0,
       0,     0,   370,   302,   371,   372,     0,     0,     0,     0,
       0,     0,     0,     0,   373,   303,     0,   374,   304,     0,
       0,   305,     0,     8,   306,   375,     0,   376,   307,     0,
       0,   377,   378,     0,     0,     0,     0,     0,   379,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   308,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     309,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,   199,
       0,     0,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,    51,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   200,   201,     0,     0,     0,     0,   322,
     323,   324,     0,   325,     0,     0,   326,   327,     0,     0,
       0,   297,   298,   299,   300,   301,   329,     0,   330,   331,
     383,   169,   893,   302,     0,     0,     0,     0,     0,   199,
       0,     0,     0,     0,     0,   303,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,   605,     0,   307,     0,
       0,     0,   202,   200,   201,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   308,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     309,   203,   204,     0,     0,     0,   205,     0,   851,   206,
       0,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,     0,   202,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,   208,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   606,   321,     0,     0,     0,     0,     0,     0,
       0,   203,   204,     0,     0,     0,   205,     0,     0,   607,
     323,   324,     0,   325,     0,     0,   326,   327,   297,   298,
     299,   300,   301,     0,   207,     0,   608,     0,   330,   331,
     302,   169,     0,   208,     0,     0,   199,     0,     0,     0,
       0,     0,   303,     0,     0,   304,     0,     0,   305,     0,
       0,   306,     0,     0,     0,   307,     0,     0,     0,     0,
     200,   201,     0,     0,     0,     0,   116,   117,   118,     0,
     120,   121,   122,   123,   124,   308,   126,   127,   128,   129,
     130,   131,   132,   133,   134,     0,   136,   137,   138,     0,
       0,   141,   142,   143,   144,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     311,     0,     0,     0,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     199,     0,     0,    50,     0,     0,     0,     0,     0,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   606,
     321,     0,     0,     0,   200,   201,     0,     0,   203,   204,
       0,     0,     0,   205,     0,     0,   607,   323,   324,     0,
     325,     0,     0,   326,   327,   297,   298,   299,   300,   301,
       0,   207,     0,   618,     0,   330,   331,   302,   169,     0,
     208,     0,     0,   293,     0,     0,     0,     0,     0,   303,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,     0,   307,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,     0,   120,   121,   122,
     123,   124,   308,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   136,   137,   138,     0,     0,   141,   142,
     143,   144,   203,   204,   309,     0,     0,   205,     0,   852,
     206,     0,     0,     0,     0,     0,   310,   311,     0,     0,
       0,     0,     0,     0,     0,   207,     0,     0,     0,     0,
       0,     0,     0,     0,   208,     0,     0,     0,     0,     0,
      50,   199,     0,     0,     0,     0,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    51,   321,     0,     0,
       0,     0,     0,     0,     0,   200,   201,     0,     0,     0,
     294,     0,     0,   322,   323,   324,     0,   325,     0,     0,
     326,   327,   297,   298,   299,   300,   301,     0,   295,     0,
     329,     0,   330,   331,   302,   169,     0,   296,     0,     0,
       0,     0,     0,     0,     0,     0,   303,     0,     0,   304,
       0,     0,   305,     0,     0,   306,     0,     0,     0,   307,
       0,     0,     0,   679,   202,     0,     0,     0,     0,     0,
     116,   117,   118,     0,   120,   121,   122,   123,   124,   308,
     126,   127,   128,   129,   130,   131,   132,   133,   134,     0,
     136,   137,   138,     0,     0,   141,   142,   143,   144,     0,
       0,   309,     0,   890,   204,     0,     0,     0,   205,     0,
       0,   206,     0,   310,   311,     0,     0,     0,     0,     0,
       0,     0,   521,     0,     0,     0,   207,     0,   381,   382,
       0,     0,     0,     0,     0,   208,     0,    50,     0,   199,
       0,     0,     0,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,    51,   321,     0,     0,     0,     0,     0,
       0,     0,     0,   200,   201,     0,     0,     0,     0,     0,
     322,   323,   324,     0,   325,     0,     0,   326,   327,     0,
       0,   297,   298,   299,   300,   301,     0,   329,     0,   330,
     331,   522,   169,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,     0,   307,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   308,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     309,   203,   204,     0,     0,     0,   205,     0,   921,   206,
       0,     0,   310,   311,     0,   199,     0,     0,     0,     0,
       0,   534,     0,     0,   207,     0,     0,   381,   382,     0,
       0,     0,     0,   208,     0,     0,    50,     0,     0,   200,
     201,     0,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,    51,   321,     0,     0,     0,     0,     0,     0,
     297,   298,   299,   300,   301,     0,     0,     0,     0,   322,
     323,   324,   302,   325,     0,     0,   326,   327,     0,     0,
       0,     0,     0,     0,   303,     0,   329,   304,   330,   331,
     305,   169,     0,   306,     0,     0,     0,   307,   202,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   308,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,   203,   204,   309,
       0,     0,   205,     0,   957,   206,     0,     0,     0,     0,
       0,   310,   311,     0,     0,     0,     0,     0,     0,     0,
     207,     0,     0,     0,     0,     0,     0,     0,     0,   208,
       0,     0,   199,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   312,   313,   314,   315,   316,   317,   318,   319,
     320,    51,   321,     0,     0,     0,   200,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,   323,
     324,     0,   325,     0,     0,   326,   327,   297,   298,   299,
     300,   301,     0,   631,     0,   329,   632,   330,   331,   302,
     169,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,     0,   307,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,     0,   120,
     121,   122,   123,   124,   308,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,   136,   137,   138,     0,     0,
     141,   142,   143,   144,   203,   204,   309,     0,     0,   205,
       0,     0,   206,     0,     0,     0,     0,     0,   310,   311,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
       0,     0,     0,   381,   382,     0,   208,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   312,
     313,   314,   315,   316,   317,   318,   319,   320,    51,   321,
       0,     0,     0,     0,     0,     0,   297,   298,   299,   300,
     301,     0,     0,     0,     0,   322,   323,   324,   302,   325,
       0,     0,   326,   327,     0,     0,     0,     0,     0,     0,
     303,     0,   329,   304,   330,   331,   305,   169,     0,   306,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,     0,   120,   121,
     122,   123,   124,   308,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,   136,   137,   138,     0,     0,   141,
     142,   143,   144,     0,     0,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,   311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   713,   714,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   312,   313,
     314,   315,   316,   317,   318,   319,   320,    51,   321,     0,
       0,     0,     0,     0,     0,   297,   298,   299,   300,   301,
       0,     0,     0,     0,   322,   323,   324,   302,   325,     0,
       0,   326,   327,     0,     0,     0,     0,     0,     0,   303,
       0,   329,   304,   330,   331,   305,   169,     0,   306,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,     0,   120,   121,   122,
     123,   124,   308,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   136,   137,   138,     0,     0,   141,   142,
     143,   144,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   722,   723,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    51,   321,     0,     0,
       0,     0,     0,     0,   297,   298,   299,   300,   301,     0,
       0,     0,     0,   322,   323,   324,   302,   325,     0,     0,
     326,   327,     0,     0,     0,     0,     0,     0,   303,     0,
     329,   304,   330,   331,   305,   169,     0,   306,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,     0,   120,   121,   122,   123,
     124,   308,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   136,   137,   138,     0,     0,   141,   142,   143,
     144,     0,     0,   309,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,    51,   321,     0,     0,     0,
       0,     0,     0,   297,   298,   299,   300,   301,     0,     0,
       0,     0,   322,   323,   324,   302,   325,     0,     0,   326,
     327,     0,     0,     0,     0,     0,     0,   303,   328,   329,
     304,   330,   331,   305,   169,     0,   306,     0,     0,     0,
     307,     0,     0,   531,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,     0,   120,   121,   122,   123,   124,
     308,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,   136,   137,   138,     0,     0,   141,   142,   143,   144,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   310,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   312,   313,   314,   315,   316,
     317,   318,   319,   320,    51,   321,     0,     0,     0,     0,
       0,     0,   297,   298,   299,   300,   301,     0,     0,   657,
       0,   322,   323,   324,   302,   325,     0,     0,   326,   327,
       0,     0,     0,     0,     0,     0,   303,     0,   329,   304,
     330,   331,   305,   169,     0,   306,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,     0,   120,   121,   122,   123,   124,   308,
     126,   127,   128,   129,   130,   131,   132,   133,   134,     0,
     136,   137,   138,     0,     0,   141,   142,   143,   144,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,    51,   321,     0,     0,     0,     0,     0,
       0,   297,   298,   299,   300,   301,     0,     0,     0,     0,
     322,   323,   324,   302,   325,     0,     0,   326,   327,     0,
       0,     0,     0,     0,     0,   303,     0,   329,   304,   330,
     331,   305,   169,     0,   306,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   308,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,    51,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     323,   324,     0,   325,     0,     0,   326,   327,   297,   298,
     299,   300,   301,     0,     0,     0,   329,   676,   330,   331,
     302,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,     0,     0,   304,     0,     0,   305,     0,
       0,   306,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,   117,   118,     0,
     120,   121,   122,   123,   124,   308,   126,   127,   128,   129,
     130,   131,   132,   133,   134,     0,   136,   137,   138,     0,
       0,   141,   142,   143,   144,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,    51,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,   323,   324,     0,
     325,     0,     0,   326,   327,   297,   298,   299,   300,   301,
       0,     0,     0,   329,   755,   330,   331,   302,   169,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,     0,   120,   121,   122,
     123,   124,   308,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   136,   137,   138,     0,     0,   141,   142,
     143,   144,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    51,   321,     0,     0,
       0,     0,     0,     0,   297,   298,   299,   300,   301,     0,
       0,   757,     0,   322,   323,   324,   302,   325,     0,     0,
     326,   327,     0,     0,     0,     0,     0,     0,   303,     0,
     329,   304,   330,   331,   305,   169,     0,   306,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,     0,   120,   121,   122,   123,
     124,   308,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   136,   137,   138,     0,     0,   141,   142,   143,
     144,     0,     0,   309,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,    51,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,   323,   324,     0,   325,     0,     0,   326,
     327,   297,   298,   299,   300,   301,     0,     0,     0,   329,
     770,   330,   331,   302,   169,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   308,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,    51,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     323,   324,     0,   325,     0,     0,   326,   327,   297,   298,
     299,   300,   301,     0,     0,     0,   329,   779,   330,   331,
     302,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,     0,     0,   304,     0,     0,   305,     0,
       0,   306,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,   117,   118,     0,
     120,   121,   122,   123,   124,   308,   126,   127,   128,   129,
     130,   131,   132,   133,   134,     0,   136,   137,   138,     0,
       0,   141,   142,   143,   144,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,    51,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,   323,   324,     0,
     325,     0,     0,   326,   327,   297,   298,   299,   300,   301,
       0,     0,     0,   329,   884,   330,   331,   302,   169,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,     0,   120,   121,   122,
     123,   124,   308,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   136,   137,   138,     0,     0,   141,   142,
     143,   144,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    51,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,   323,   324,     0,   325,     0,     0,
     326,   327,   297,   298,   299,   300,   301,     0,     0,     0,
     329,   944,   330,   331,   302,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,     0,     0,   304,
       0,     0,   305,     0,     0,   306,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,     0,   120,   121,   122,   123,   124,   308,
     126,   127,   128,   129,   130,   131,   132,   133,   134,     0,
     136,   137,   138,     0,     0,   141,   142,   143,   144,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,   311,     0,     0,     0,     0,     0,
       0,     0,   955,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,    51,   321,     0,     0,     0,     0,     0,
       0,   297,   298,   299,   300,   301,     0,     0,     0,     0,
     322,   323,   324,   302,   325,     0,     0,   326,   327,     0,
       0,     0,     0,     0,     0,   303,     0,   329,   304,   330,
     331,   305,   169,     0,   306,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   308,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   311,     0,     0,     0,   472,   473,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,    51,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     323,   324,     0,   325,     0,     0,   326,   327,     0,     0,
       0,     0,     0,     0,     0,     0,   329,     0,   330,   331,
       0,   169,   550,   551,   552,   553,   554,   555,   556,   557,
     474,   475,   476,   477,   478,   558,   559,   479,   480,   481,
     482,   689,   483,   484,   485,   486,   472,   473,     0,   487,
     561,   488,   489,   562,   563,     0,     0,   490,   491,   492,
     564,   565,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   690,   493,     0,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,     0,     0,     0,     0,     0,
     691,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,   551,   552,   553,   554,   555,   556,   557,   474,
     475,   476,   477,   478,   558,   559,   479,   480,   481,   482,
     689,   483,   484,   485,   486,   472,   473,     0,   487,   561,
     488,   489,   562,   563,     0,     0,   490,   491,   492,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     690,   493,     0,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,   698,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     550,   551,   552,   553,   554,   555,   556,   557,   474,   475,
     476,   477,   478,   558,   559,   479,   480,   481,   482,   689,
     483,   484,   485,   486,   472,   473,     0,   487,   561,   488,
     489,   562,   563,     0,     0,   490,   491,   492,   564,   565,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   690,
     493,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   504,   505,     0,     0,     0,     0,     0,   817,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
     551,   552,   553,   554,   555,   556,   557,   474,   475,   476,
     477,   478,   558,   559,   479,   480,   481,   482,   689,   483,
     484,   485,   486,   472,   473,     0,   487,   561,   488,   489,
     562,   563,     0,     0,   490,   491,   492,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   690,   493,
       0,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,     0,     0,   819,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   550,   551,
     552,   553,   554,   555,   556,   557,   474,   475,   476,   477,
     478,   558,   559,   479,   480,   481,   482,   689,   483,   484,
     485,   486,   472,   473,     0,   487,   561,   488,   489,   562,
     563,     0,     0,   490,   491,   492,   564,   565,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   690,   493,     0,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   504,
     505,     0,     0,     0,     0,     0,   903,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,   551,   552,
     553,   554,   555,   556,   557,   474,   475,   476,   477,   478,
     558,   559,   479,   480,   481,   482,   689,   483,   484,   485,
     486,   472,   473,     0,   487,   561,   488,   489,   562,   563,
       0,     0,   490,   491,   492,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   690,   493,     0,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,   938,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   472,   473,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
       0,   479,   480,   481,   482,     0,   483,   484,   485,   486,
       0,     0,     0,   487,     0,   488,   489,     0,     0,     0,
       0,   490,   491,   492,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,     0,     0,
       0,   472,   473,     0,     0,     0,     0,   504,   505,   474,
     475,   476,   477,   478,   896,     0,   479,   480,   481,   482,
       0,   483,   484,   485,   486,     0,     0,     0,   487,     0,
     488,   489,     0,     0,     0,     0,   490,   491,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   493,     0,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,     0,     0,     0,     0,     0,     0,   472,
     473,     0,   504,   505,   474,   475,   476,   477,   478,   964,
       0,   479,   480,   481,   482,     0,   483,   484,   485,   486,
       0,     0,     0,   487,     0,   488,   489,     0,     0,     0,
       0,   490,   491,   492,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   472,
     473,   628,   474,   475,   476,   477,   478,     0,     0,   479,
     480,   481,   482,     0,   483,   484,   485,   486,     0,     0,
       0,   487,     0,   488,   489,     0,     0,     0,     0,   490,
     491,   492,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   472,   473,   760,
       0,     0,   474,   475,   476,   477,   478,     0,     0,   479,
     480,   481,   482,     0,   483,   484,   485,   486,     0,     0,
       0,   487,     0,   488,   489,     0,     0,     0,     0,   490,
     491,   492,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   472,   473,   763,
     474,   475,   476,   477,   478,     0,     0,   479,   480,   481,
     482,     0,   483,   484,   485,   486,     0,     0,     0,   487,
       0,   488,   489,     0,     0,     0,     0,   490,   491,   492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   493,     0,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   472,   473,   848,     0,     0,
     474,   475,   476,   477,   478,     0,     0,   479,   480,   481,
     482,     0,   483,   484,   485,   486,     0,     0,     0,   487,
       0,   488,   489,     0,     0,     0,     0,   490,   491,   492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   493,     0,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   472,   473,   961,   474,   475,
     476,   477,   478,     0,     0,   479,   480,   481,   482,     0,
     483,   484,   485,   486,     0,     0,     0,   487,     0,   488,
     489,     0,     0,     0,     0,   490,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     493,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   504,   505,   472,   473,   963,     0,     0,   474,   475,
     476,   477,   478,     0,     0,   479,   480,   481,   482,     0,
     483,   484,   485,   486,     0,     0,     0,   487,     0,   488,
     489,     0,     0,     0,     0,   490,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     493,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,   472,
     473,   504,   505,     0,     0,   979,   474,   475,   476,   477,
     478,     0,     0,   479,   480,   481,   482,     0,   483,   484,
     485,   486,     0,     0,     0,   487,     0,   488,   489,     0,
       0,     0,     0,   490,   491,   492,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   493,     0,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
       0,     0,     0,     0,     0,   472,   473,     0,     0,   504,
     505,   506,   474,   475,   476,   477,   478,     0,     0,   479,
     480,   481,   482,     0,   483,   484,   485,   486,     0,     0,
       0,   487,     0,   488,   489,     0,     0,     0,     0,   490,
     491,   492,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,     0,     0,     0,     0,
       0,   472,   473,     0,     0,   504,   505,   803,   474,   475,
     476,   477,   478,     0,     0,   479,   480,   481,   482,     0,
     483,   484,   485,   486,     0,     0,     0,   487,     0,   488,
     489,     0,     0,     0,     0,   490,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     493,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,   472,   473,     0,
       0,   504,   505,   811,   474,   475,   476,   477,   478,     0,
       0,   479,   480,   481,   482,     0,   483,   484,   485,   486,
       0,     0,     0,   487,     0,   488,   489,     0,     0,     0,
       0,   490,   491,   492,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   887,
     474,   475,   476,   477,   478,   472,   473,   479,   480,   481,
     482,     0,   483,   484,   485,   486,     0,     0,     0,   487,
       0,   488,   489,     0,     0,     0,     0,   490,   491,   492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   493,     0,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   888,     0,     0,     0,     0,
     550,   551,   552,   553,   554,   555,   556,   557,   474,   475,
     476,   477,   478,   558,   559,   479,   480,   481,   482,   689,
     483,   484,   485,   486,  -241,   472,   473,   487,   561,   488,
     489,   562,   563,     0,     0,   490,   491,   492,   564,   565,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   690,
     493,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   504,   505,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   472,   473,     0,     0,     0,     0,     0,
     550,   551,   552,   553,   554,   555,   556,   557,   474,   475,
     476,   477,   478,   558,   559,   479,   480,   481,   482,   560,
     483,   484,   485,   486,     0,     0,     0,   487,   561,   488,
     489,   562,   563,     0,     0,   490,   491,   492,   564,   565,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     493,     0,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   472,   473,     0,     0,   474,   475,   476,   477,
     478,   504,   505,   479,   480,   481,   482,     0,   483,   484,
     485,   486,     0,     0,     0,   487,     0,   488,   489,     0,
       0,   621,     0,   490,   491,   492,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   493,     0,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     472,   473,     0,     0,     0,     0,     0,     0,     0,   504,
     505,     0,     0,     0,     0,   474,   475,   476,   477,   478,
       0,     0,   479,   480,   481,   482,     0,   483,   484,   485,
     486,     0,     0,     0,   487,     0,   488,   489,     0,     0,
       0,     0,   490,   491,   492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   493,   805,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   472,
     473,     0,     0,   474,   475,   476,   477,   478,   504,   505,
     479,   480,   481,   482,     0,   483,   484,   485,   486,     0,
       0,     0,   487,     0,   488,   489,     0,     0,     0,     0,
     490,   491,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   870,     0,     0,     0,   493,     0,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   472,   473,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,     0,   479,
     480,   481,   482,     0,   483,   484,   485,   486,     0,     0,
       0,   487,     0,   488,   489,     0,     0,     0,     0,   490,
     491,   492,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   472,   473,     0,     0,
     474,   475,   476,   477,   478,   504,   505,   479,   480,   481,
     482,     0,   483,   484,   485,   486,     0,     0,     0,   487,
       0,   488,   489,     0,   472,   473,     0,   490,     0,   492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,     0,     0,     0,     0,   474,
     475,   476,   477,   478,     0,     0,   479,   480,   481,   482,
       0,   483,   484,   485,   486,     0,     0,     0,   487,     0,
     488,   489,   472,   473,     0,     0,   490,   474,   475,   476,
     477,   478,     0,     0,   479,   480,   481,   482,     0,   483,
     484,   485,   486,     0,     0,     0,   487,     0,   488,   489,
     472,   473,     0,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,   500,   501,   502,
     503,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,   474,   475,   476,   477,   478,
       0,     0,   479,   480,   481,   482,     0,   483,   484,   485,
     486,     0,     0,     0,   487,     0,   488,   489,   472,   473,
       0,     0,     0,   474,   475,   476,   477,   478,     0,     0,
     479,   480,   481,   482,     0,   483,   484,   485,   486,     0,
       0,     0,   487,     0,   488,   489,     0,     0,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,   453,     0,     0,     0,
     497,   498,   499,   500,   501,   502,   503,     0,   112,     0,
       0,     0,     0,     0,   113,     0,   504,   505,     0,     0,
       0,   474,   475,   476,   477,   478,     0,     0,   479,   114,
       0,   482,     0,   483,   484,   485,   486,     0,     0,     0,
     487,     0,   488,   489,   115,     0,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,     0,   497,   498,
     499,   500,   501,   502,   503,   455,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,   112,     0,     0,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,   115,     0,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,     0,     0,     0,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,   115,
       0,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
       0,     0,     0,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     114,     0,     0,     0,     0,   312,   313,   314,     0,     0,
       0,     0,     0,     0,    51,   115,     0,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,     0,     0,     0,     0,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,   115,     0,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,     0,     0,     0,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,     0,     0,     0,   234,   235,
       0,     0,     0,     0,     0,     0,   236,   237,   238,   239,
      50,     0,   240,   241,   242,   243,   244,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,     0,
       0,   256,   257,     0,     0,     0,     0,     0,   258,   259
};

static const yytype_int16 yycheck[] =
{
      12,   264,    73,   285,   360,   287,   430,   289,   432,   370,
     434,   175,   456,   369,   515,   513,   314,   611,    87,   380,
     507,   356,   340,   381,   382,   619,    81,     7,    33,     0,
      14,    15,    19,    21,   631,     6,    49,    20,    21,    34,
      18,    19,    32,    46,    19,    49,   128,    32,   136,   113,
      19,   119,   116,   117,    59,   136,   116,   117,    29,    18,
      31,    73,    33,   114,    41,    53,    56,    57,    39,   114,
      82,   161,   123,   181,   182,    46,   184,   139,   123,   114,
     145,    52,   145,   116,   117,   145,   114,   114,   123,    14,
      15,    73,   180,   183,   129,   123,   123,   453,    75,   455,
      71,   153,   184,   154,   185,   145,   270,   145,   595,   154,
     105,   106,   176,   177,   159,   183,   176,   177,   181,   154,
     185,    92,   105,   106,   129,   115,   154,   154,   145,   145,
     113,   183,   115,   116,   117,   118,   161,    20,    21,   122,
     145,   145,   145,   176,   177,   185,   145,   185,    53,   145,
     145,    58,    20,    21,    59,   139,   136,   513,   183,   143,
     521,   145,   146,   179,   154,   155,   153,   145,   185,   159,
     183,   161,   162,   534,   159,   772,   770,   165,   153,   162,
     163,   164,   165,   166,   153,   184,   145,   177,   176,   185,
     548,   154,   177,   176,   177,   179,   186,   177,   153,   686,
     153,   186,   147,   148,   149,   185,   177,   278,   279,   280,
     281,   153,   183,   284,   139,   286,   271,   288,   143,   290,
     145,   146,   105,   106,   156,   180,     4,     5,   299,     7,
     113,   153,   115,   116,   117,   118,   178,   105,   106,   122,
      32,   312,   313,    20,    21,   113,   154,   605,   116,   117,
     118,   153,   184,   571,   122,   267,   153,    35,   153,   594,
     156,   183,   129,   857,    56,    57,   278,   279,   280,   281,
     147,   148,   284,   867,   286,   183,   288,   129,   290,   766,
     153,   164,   165,   166,   355,   180,   183,   299,   154,   156,
     145,   778,   363,   176,   177,   145,   278,   279,   280,   281,
     312,   313,   284,   372,   286,   145,   288,   180,   176,   177,
     379,   129,   813,   325,   145,   802,   166,   299,   145,    20,
      21,   153,   683,   115,   622,   153,   770,   145,   105,   106,
     312,   313,   114,   153,   153,   779,   113,   835,   115,   116,
     117,   118,   160,   355,   153,   122,   161,   176,   180,    20,
      21,   363,   180,   161,   636,   153,   179,   153,   859,   178,
     180,   156,   154,   155,   147,   148,   161,   159,   183,   178,
     162,   153,   154,   355,   156,   183,   800,   159,   739,   450,
     178,   363,   178,    53,   179,   177,    11,   139,   156,    59,
     156,   183,   129,   161,   186,   161,   156,    22,    23,   176,
     177,   161,   103,   104,   105,   106,    66,   571,   145,   401,
      70,   179,   113,   179,   115,   116,   117,   118,   145,   179,
      53,   122,   183,   124,   125,   417,    59,    87,    88,    89,
      90,   443,   103,   104,   105,   106,   107,    53,   450,   110,
     111,   112,   113,    59,   115,   116,   117,   118,   145,   873,
     519,   122,    53,   124,   125,   178,   468,   145,    59,   528,
     183,   162,   163,   164,   165,   166,   153,   180,   450,   156,
     183,   183,   159,   185,   543,   176,   177,    32,   839,   835,
     147,   148,   149,   150,    42,    43,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   145,   183,   145,   185,
     184,    56,    57,   184,   575,   176,   177,   789,   579,   184,
     145,    32,   184,    53,   160,   160,   160,   160,   179,   590,
     160,   592,    32,   145,   160,   183,   597,   145,   599,   600,
     160,   602,   160,   160,   160,    56,    57,    34,    34,   156,
     179,   136,   145,    32,   156,   183,    56,    57,   156,   183,
     179,   145,   184,   160,   178,   178,   160,   820,   178,   114,
     115,   180,   160,   575,   179,   179,   177,   579,   123,   160,
     207,   160,   182,   179,   183,   139,    36,   136,   590,   184,
     592,   183,   145,   183,   183,   597,   183,   599,   600,   145,
     602,   183,   145,   575,   115,   119,    13,   579,   139,   154,
     155,    32,   156,   182,   159,   115,   184,   162,   590,   178,
     681,   145,   145,   161,   161,   597,   161,   599,   600,   185,
     602,   153,   177,   692,   153,    56,    57,   264,   183,   178,
     147,   186,   145,   154,   155,    32,   179,   160,   159,   145,
     161,   162,   145,   145,   154,   155,     1,     7,    42,   159,
     145,   161,   162,   183,   145,    32,   177,   185,   178,    56,
      57,   147,   178,   178,   178,   186,   145,   177,   145,   681,
     145,   145,   179,   310,   311,   180,   186,   314,   178,    56,
      57,    32,   183,   145,   115,   322,   323,   324,   178,   326,
     327,   178,   329,   178,   178,   178,   161,   185,   161,   681,
     178,   183,   178,    49,   185,   178,   178,   178,    37,   460,
      63,   109,    66,   933,   276,    64,   728,   788,   115,    82,
     728,   935,   937,   154,   155,     1,   700,   880,   159,   366,
     161,   162,   833,   370,   765,   592,   373,   374,   115,   291,
      45,   378,   517,   380,   735,   283,   177,   568,   299,   456,
     387,   777,   821,   951,    -1,   186,    -1,   154,   155,    -1,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   788,   154,   155,    -1,
     177,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     177,    -1,    -1,    -1,    -1,    -1,   788,    -1,   445,   186,
      -1,   448,   883,   882,    -1,    -1,    -1,   454,    -1,   456,
      -1,    -1,    -1,   892,    -1,    -1,   895,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   474,   475,    -1,
      -1,   478,   479,   480,   481,    -1,   483,    -1,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,    -1,   505,    -1,
      -1,   883,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,   521,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,   534,    56,    57,
     122,   883,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
      -1,   953,    32,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,   115,   970,   596,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
     607,   608,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   618,    -1,    -1,   621,   622,    -1,    -1,   625,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,
      -1,   159,    -1,    -1,   162,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,   179,    -1,    -1,    -1,   115,    -1,    -1,   186,    -1,
      -1,    -1,    -1,   670,    -1,    -1,   673,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,    -1,
      -1,    -1,   689,   690,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,   713,   714,    -1,    -1,
      -1,    -1,    -1,    56,    57,   722,   723,   177,    -1,   726,
      -1,   103,   104,   105,   106,   107,   186,    32,   110,   111,
     112,   113,   739,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    32,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   768,   769,    -1,    -1,    56,    57,    -1,   775,    -1,
     777,    -1,   115,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   792,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,   805,    -1,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   154,   155,   820,    -1,    -1,   159,   824,   161,   162,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   839,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,
     161,   162,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,   891,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,   905,    -1,
      -1,   908,   909,   910,   911,   912,    -1,    -1,   915,   916,
      -1,   918,    -1,     1,    -1,    -1,    -1,    -1,     6,   926,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   955,    37,
      38,    -1,    40,    41,    -1,   962,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    54,    55,   975,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,   181,   182,   183,   184,   185,     1,    -1,
      -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,    -1,
      -1,    44,    -1,    46,    47,    48,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    32,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    14,    15,    16,    17,    18,   179,    -1,   181,   182,
     183,   184,   185,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    49,    -1,    51,    -1,
      -1,    -1,   115,    56,    57,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,   154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,   177,    -1,   179,    -1,   181,   182,
      26,   184,    -1,   186,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    56,    57,    -1,    -1,   154,   155,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,   177,    -1,   179,    -1,   181,   182,    26,   184,    -1,
     186,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   154,   155,    93,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,
     129,    32,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,   177,    -1,
     179,    -1,   181,   182,    26,   184,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,
      -1,   162,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,   177,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,   186,    -1,   129,    -1,    32,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   179,    -1,   181,
     182,   183,   184,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,   154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,   177,    -1,    -1,   120,   121,    -1,
      -1,    -1,    -1,   186,    -1,    -1,   129,    -1,    -1,    56,
      57,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,
     163,   164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,
      44,   184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   154,   155,    93,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    32,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,   177,    -1,   179,   180,   181,   182,    26,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   154,   155,    93,    -1,    -1,   159,
      -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,   186,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,
      -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   179,    41,   181,   182,    44,   184,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   179,    41,   181,   182,    44,   184,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     179,    41,   181,   182,    44,   184,    -1,    47,    -1,    -1,
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
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    38,   178,   179,
      41,   181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,   162,   163,   164,    26,   166,    -1,    -1,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,
     181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,
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
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     162,   163,   164,    26,   166,    -1,    -1,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,
     182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   179,   180,   181,   182,
      26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   179,   180,   181,   182,    26,   184,    -1,
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
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,   160,    -1,   162,   163,   164,    26,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     179,    41,   181,   182,    44,   184,    -1,    47,    -1,    -1,
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
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    14,    15,    16,    17,    18,    -1,    -1,    -1,   179,
     180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   179,   180,   181,   182,
      26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   179,   180,   181,   182,    26,   184,    -1,
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
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     179,   180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     162,   163,   164,    26,   166,    -1,    -1,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,
     182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   181,   182,
      -1,   184,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    20,    21,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    20,    21,    -1,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    20,    21,    -1,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    20,    21,    -1,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    20,    21,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    20,    21,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   176,   177,   103,
     104,   105,   106,   107,   183,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   176,   177,   103,   104,   105,   106,   107,   183,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    20,
      21,   180,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    20,    21,   180,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    20,    21,   180,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    20,    21,   180,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    20,    21,   180,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    20,    21,   180,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,   176,   177,    -1,    -1,   180,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   176,
     177,   178,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,   176,   177,   178,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,   176,   177,   178,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,   178,
     103,   104,   105,   106,   107,    20,    21,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,   178,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    20,    21,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    20,    21,    -1,    -1,   103,   104,   105,   106,
     107,   176,   177,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,   128,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    20,
      21,    -1,    -1,   103,   104,   105,   106,   107,   176,   177,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    20,    21,    -1,    -1,
     103,   104,   105,   106,   107,   176,   177,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    20,    21,    -1,   130,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    20,    21,    -1,    -1,   130,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      20,    21,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
     160,   161,   162,   163,   164,   165,   166,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,   176,   177,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,    45,
      -1,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,   160,   161,
     162,   163,   164,   165,   166,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      45,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     129,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,   176,   177
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   188,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   177,   183,   191,   199,   200,   204,   225,   229,
     245,   301,   306,   308,   313,   349,   351,    18,    19,   145,
     219,   220,   221,   139,   205,   206,   145,   166,   201,   202,
     145,   184,   304,   145,   181,   190,   352,   350,    33,    59,
     129,   145,   222,   223,   224,   237,     4,     5,     7,    35,
     311,    58,   299,   154,   153,   156,   153,   201,    21,    53,
     165,   176,   203,   154,   304,   305,   299,   145,   145,   145,
     129,   179,   153,   178,    53,    59,   230,   231,   232,    53,
      59,   307,    53,    59,   312,    53,    59,   300,    14,    15,
     139,   143,   145,   146,   179,   193,   220,   139,   206,   145,
     145,   145,    24,    30,    45,    60,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,   237,   316,   318,
     319,   321,   324,   183,   185,   304,    53,    59,   189,   184,
     184,   145,   221,   223,    34,   105,   106,   145,   228,   184,
     233,   145,   145,   310,   184,   302,   147,   148,   192,    14,
      15,   139,   143,   145,   193,   217,   218,   203,   160,   160,
     160,   160,   179,   160,   160,   160,   160,   160,   160,    32,
      56,    57,   115,   154,   155,   159,   162,   177,   186,   183,
     145,   320,   145,   291,   292,   180,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   116,   117,   124,   125,   126,   127,
     130,   131,   132,   133,   134,   135,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   169,   170,   176,   177,
      34,    34,   179,   226,   235,   156,   184,   156,   309,   314,
     298,   136,   281,   147,   148,   149,   153,   180,   327,   329,
     331,   325,   145,   322,   333,   335,   337,   339,   341,   343,
     345,   347,    32,    32,   159,   177,   186,    14,    15,    16,
      17,    18,    26,    38,    41,    44,    47,    51,    71,    93,
     105,   106,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   146,   162,   163,   164,   166,   169,   170,   178,   179,
     181,   182,   195,   197,   233,   237,   239,   247,   254,   257,
     260,   264,   265,   268,   269,   270,   275,   278,   316,   353,
     357,   362,   364,   366,   183,   156,   183,   178,   178,   180,
     281,   287,   288,   156,   227,     1,     8,     9,    10,    12,
      25,    27,    28,    37,    40,    48,    50,    54,    55,    61,
      94,   120,   121,   183,   185,   207,   208,   211,   212,   213,
     214,   215,   216,   234,   236,   238,   240,   241,   242,   243,
     244,   245,   246,   266,   267,   278,    66,    70,    87,    88,
      89,    90,   317,   303,   237,   184,   185,   281,   221,   145,
     297,   315,   217,   324,   324,   324,   324,   180,   320,   324,
     226,   324,   226,   324,   226,   145,   289,   290,   324,   292,
     178,   324,   353,   179,   160,   179,   160,   160,   179,   160,
     160,   278,   278,    12,   324,    12,   324,   278,   359,   363,
     194,   278,   278,   278,   237,   278,   278,   278,   182,   145,
     179,   225,    20,    21,   103,   104,   105,   106,   107,   110,
     111,   112,   113,   115,   116,   117,   118,   122,   124,   125,
     130,   131,   132,   155,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   176,   177,   178,   179,   145,   185,
     324,   185,   291,    46,   294,   315,   180,   183,   324,   278,
     315,   114,   183,   236,   266,   278,   233,   278,   278,   183,
     139,    54,   278,   233,   114,   236,   278,   182,   264,   264,
      36,   183,   183,   278,   183,   183,   183,   297,   119,   183,
      95,    96,    97,    98,    99,   100,   101,   102,   108,   109,
     114,   123,   126,   127,   133,   134,   135,   154,   184,   145,
     185,   285,   297,   178,   153,   156,   159,   296,   161,   183,
     161,   161,   161,   183,   161,   227,   161,   227,   161,   227,
     156,   161,   183,   161,   183,   179,   237,   255,   278,   248,
     250,   278,   252,   324,   315,    49,   145,   162,   179,   278,
     354,   355,   356,   358,   359,   360,   361,   315,   179,   355,
     361,   128,   183,   185,   149,   150,   192,   198,   180,   160,
     237,   177,   180,   258,   278,   136,   263,    18,   145,   145,
     278,   278,   278,   278,   278,   278,   145,   278,   145,   278,
     278,   278,   278,   278,   278,   278,   278,    21,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   116,
     117,   145,   176,   177,   276,   278,   180,   258,   294,   114,
     154,   156,   159,   293,   287,   233,    19,   236,   278,   114,
     154,   183,    13,   233,   156,   139,   236,   278,   183,   182,
     184,   233,   264,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   120,   121,   278,   278,   278,   278,   278,
     278,   278,   120,   121,   278,   303,   154,   183,   177,   185,
     225,   281,   284,   178,   145,   324,   114,   123,   154,   295,
     328,   324,   332,   326,   323,   334,   161,   338,   161,   342,
     161,   324,   346,   289,   348,   180,   258,   160,   278,   324,
     180,   324,   324,   180,   324,   161,    19,   264,   123,   293,
     180,   153,   183,   356,   178,   153,   178,   183,    19,   180,
     356,   185,   278,   359,   185,   278,   147,   196,   271,   273,
     145,   354,   153,   180,   114,   123,   154,   159,   261,   262,
     226,   160,   179,   178,   145,   156,   145,   278,   145,   278,
       1,   178,   180,   324,   236,   278,   258,   183,   233,   183,
     235,    11,    22,    23,   209,   210,   278,   278,   278,   278,
     185,   278,   224,     7,    42,   282,   183,   154,   183,   295,
     236,   278,   161,   336,   340,   344,   180,   145,   180,   161,
     161,   161,   161,   263,   258,   278,   278,   355,   356,   145,
     354,   178,   178,   278,   178,   360,   258,   355,   185,   178,
     151,   147,   324,   226,   178,   278,   145,   145,   145,   145,
     153,   178,   227,   279,   180,   258,   278,   178,   178,   277,
     154,   293,   183,   185,   233,   278,   183,   178,    43,   282,
     294,   236,   278,   183,   330,   161,   183,   256,   249,   251,
     253,   179,   183,   356,   178,   123,   293,   178,   183,   356,
     178,   161,   227,   180,   261,   178,   128,   233,   259,   324,
     180,   278,   233,   233,   183,   286,    32,   283,   183,   278,
     145,   278,   278,   278,   180,   278,   278,   178,   178,   278,
     278,   278,   185,   272,   161,   114,   278,   161,   210,   231,
     232,   180,   161,   180,   183,   365,   178,   365,   178,   237,
     274,   278,   280,   183,   278,    49,   178,   185,   237,   180,
     278,   178,   178
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   187,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   189,   190,   190,
     191,   192,   192,   192,   192,   193,   194,   194,   194,   195,
     196,   196,   198,   197,   199,   200,   201,   201,   201,   201,
     202,   202,   203,   203,   204,   205,   205,   206,   206,   207,
     208,   208,   209,   209,   210,   210,   210,   211,   211,   212,
     213,   214,   215,   216,   217,   217,   217,   217,   217,   217,
     218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
     220,   220,   220,   221,   221,   222,   222,   222,   223,   223,
     224,   224,   225,   225,   226,   226,   226,   227,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   229,   230,   230,   230,
     231,   232,   233,   233,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   235,   235,   235,   236,   236,   236,   237,
     237,   237,   238,   239,   239,   239,   239,   240,   241,   242,
     242,   242,   242,   242,   243,   243,   243,   243,   244,   245,
     245,   246,   248,   249,   247,   250,   251,   247,   252,   253,
     247,   255,   256,   254,   257,   257,   257,   258,   258,   259,
     259,   259,   260,   260,   260,   261,   261,   261,   261,   262,
     262,   263,   263,   264,   264,   265,   265,   265,   265,   265,
     265,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     267,   267,   267,   267,   268,   269,   269,   270,   271,   272,
     270,   273,   274,   270,   275,   275,   276,   277,   275,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   279,   280,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   281,   281,   282,   282,   283,   283,   284,   285,
     285,   286,   285,   285,   285,   287,   287,   288,   288,   289,
     289,   290,   290,   291,   292,   292,   293,   293,   294,   294,
     294,   294,   294,   294,   295,   295,   295,   296,   296,   297,
     297,   297,   297,   297,   298,   298,   299,   299,   300,   300,
     300,   301,   302,   301,   303,   303,   303,   304,   305,   305,
     306,   306,   307,   307,   307,   308,   308,   309,   309,   310,
     311,   311,   312,   312,   312,   314,   313,   315,   315,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   317,   317,   317,   317,   317,   317,
     318,   319,   319,   320,   320,   322,   323,   321,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   325,   326,   324,   324,   327,
     328,   324,   329,   330,   324,   331,   332,   324,   324,   333,
     334,   324,   335,   336,   324,   324,   337,   338,   324,   339,
     340,   324,   324,   341,   342,   324,   343,   344,   324,   345,
     346,   324,   347,   348,   324,   350,   349,   352,   351,   353,
     353,   353,   353,   354,   354,   354,   354,   355,   355,   356,
     356,   357,   357,   357,   357,   357,   357,   358,   358,   359,
     360,   360,   361,   361,   362,   362,   363,   363,   364,   365,
     365,   366,   366
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     2,     3,     3,
       2,     4,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     1,     1,     0,     2,     4,     1,     1,     4,
       6,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     5,     1,     3,     1,     1,     1,     1,     4,
       1,     3,     0,     3,     0,     2,     3,     0,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     2,     2,     3,     0,     1,     1,
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
       8,     0,     0,     9,     3,     4,     0,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     5,
       4,     5,     3,     4,     1,     3,     4,     3,     4,     2,
       4,     4,     7,     8,     3,     5,     0,     0,     8,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     2,     0,     4,     0,     1,     0,     1,     3,     0,
       3,     0,     7,     6,     5,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     3,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     3,     5,     3,     2,     3,
       4,     2,     0,     1,     1,     7,     9,     0,     2,     2,
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
#line 3477 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3483 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3489 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3495 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3507 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3513 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3519 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3525 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3531 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3537 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3543 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3549 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3555 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3561 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3567 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3573 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3579 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3585 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3591 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3597 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3603 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3609 "ds_parser.cpp"
        break;

    case 224: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3615 "ds_parser.cpp"
        break;

    case 225: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3621 "ds_parser.cpp"
        break;

    case 226: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3627 "ds_parser.cpp"
        break;

    case 227: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3633 "ds_parser.cpp"
        break;

    case 228: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3639 "ds_parser.cpp"
        break;

    case 233: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3645 "ds_parser.cpp"
        break;

    case 234: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 235: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3657 "ds_parser.cpp"
        break;

    case 236: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 237: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3669 "ds_parser.cpp"
        break;

    case 238: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 239: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 240: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 241: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 242: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 243: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
        break;

    case 244: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3711 "ds_parser.cpp"
        break;

    case 246: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3717 "ds_parser.cpp"
        break;

    case 247: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3723 "ds_parser.cpp"
        break;

    case 254: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3729 "ds_parser.cpp"
        break;

    case 257: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3735 "ds_parser.cpp"
        break;

    case 258: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3741 "ds_parser.cpp"
        break;

    case 259: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3747 "ds_parser.cpp"
        break;

    case 261: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3753 "ds_parser.cpp"
        break;

    case 262: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3759 "ds_parser.cpp"
        break;

    case 263: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3765 "ds_parser.cpp"
        break;

    case 264: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3771 "ds_parser.cpp"
        break;

    case 265: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3777 "ds_parser.cpp"
        break;

    case 266: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3783 "ds_parser.cpp"
        break;

    case 267: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3789 "ds_parser.cpp"
        break;

    case 268: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3795 "ds_parser.cpp"
        break;

    case 269: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3801 "ds_parser.cpp"
        break;

    case 270: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3807 "ds_parser.cpp"
        break;

    case 275: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3813 "ds_parser.cpp"
        break;

    case 278: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3819 "ds_parser.cpp"
        break;

    case 281: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3825 "ds_parser.cpp"
        break;

    case 284: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3831 "ds_parser.cpp"
        break;

    case 285: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3837 "ds_parser.cpp"
        break;

    case 287: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3843 "ds_parser.cpp"
        break;

    case 288: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3849 "ds_parser.cpp"
        break;

    case 289: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3855 "ds_parser.cpp"
        break;

    case 290: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3861 "ds_parser.cpp"
        break;

    case 291: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3867 "ds_parser.cpp"
        break;

    case 292: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3873 "ds_parser.cpp"
        break;

    case 294: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3879 "ds_parser.cpp"
        break;

    case 297: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3885 "ds_parser.cpp"
        break;

    case 298: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3891 "ds_parser.cpp"
        break;

    case 303: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3897 "ds_parser.cpp"
        break;

    case 309: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3903 "ds_parser.cpp"
        break;

    case 315: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3909 "ds_parser.cpp"
        break;

    case 318: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3915 "ds_parser.cpp"
        break;

    case 319: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3921 "ds_parser.cpp"
        break;

    case 320: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3927 "ds_parser.cpp"
        break;

    case 321: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3933 "ds_parser.cpp"
        break;

    case 324: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3939 "ds_parser.cpp"
        break;

    case 353: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3945 "ds_parser.cpp"
        break;

    case 354: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3951 "ds_parser.cpp"
        break;

    case 355: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3957 "ds_parser.cpp"
        break;

    case 356: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3963 "ds_parser.cpp"
        break;

    case 357: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3969 "ds_parser.cpp"
        break;

    case 358: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3975 "ds_parser.cpp"
        break;

    case 359: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3981 "ds_parser.cpp"
        break;

    case 360: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3987 "ds_parser.cpp"
        break;

    case 361: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3993 "ds_parser.cpp"
        break;

    case 362: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3999 "ds_parser.cpp"
        break;

    case 363: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4005 "ds_parser.cpp"
        break;

    case 364: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4011 "ds_parser.cpp"
        break;

    case 365: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4017 "ds_parser.cpp"
        break;

    case 366: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4023 "ds_parser.cpp"
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
#line 470 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4315 "ds_parser.cpp"
    break;

  case 16:
#line 471 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4321 "ds_parser.cpp"
    break;

  case 17:
#line 472 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4327 "ds_parser.cpp"
    break;

  case 18:
#line 476 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4333 "ds_parser.cpp"
    break;

  case 19:
#line 477 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4339 "ds_parser.cpp"
    break;

  case 20:
#line 481 "ds_parser.ypp"
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
#line 4358 "ds_parser.cpp"
    break;

  case 21:
#line 498 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4364 "ds_parser.cpp"
    break;

  case 22:
#line 499 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4370 "ds_parser.cpp"
    break;

  case 23:
#line 500 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4376 "ds_parser.cpp"
    break;

  case 24:
#line 501 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4382 "ds_parser.cpp"
    break;

  case 25:
#line 505 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4388 "ds_parser.cpp"
    break;

  case 26:
#line 509 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4398 "ds_parser.cpp"
    break;

  case 27:
#line 514 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4412 "ds_parser.cpp"
    break;

  case 28:
#line 523 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4422 "ds_parser.cpp"
    break;

  case 29:
#line 531 "ds_parser.ypp"
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
#line 4440 "ds_parser.cpp"
    break;

  case 30:
#line 547 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4450 "ds_parser.cpp"
    break;

  case 31:
#line 552 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4460 "ds_parser.cpp"
    break;

  case 32:
#line 560 "ds_parser.ypp"
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
#line 4487 "ds_parser.cpp"
    break;

  case 33:
#line 581 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4499 "ds_parser.cpp"
    break;

  case 34:
#line 590 "ds_parser.ypp"
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
#line 4523 "ds_parser.cpp"
    break;

  case 36:
#line 616 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4531 "ds_parser.cpp"
    break;

  case 37:
#line 619 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4540 "ds_parser.cpp"
    break;

  case 38:
#line 623 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4551 "ds_parser.cpp"
    break;

  case 39:
#line 629 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4562 "ds_parser.cpp"
    break;

  case 40:
#line 638 "ds_parser.ypp"
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
#line 4588 "ds_parser.cpp"
    break;

  case 41:
#line 659 "ds_parser.ypp"
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
#line 4613 "ds_parser.cpp"
    break;

  case 42:
#line 682 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4619 "ds_parser.cpp"
    break;

  case 43:
#line 683 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4625 "ds_parser.cpp"
    break;

  case 47:
#line 696 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4633 "ds_parser.cpp"
    break;

  case 48:
#line 699 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4641 "ds_parser.cpp"
    break;

  case 49:
#line 705 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4649 "ds_parser.cpp"
    break;

  case 50:
#line 711 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4657 "ds_parser.cpp"
    break;

  case 51:
#line 714 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4665 "ds_parser.cpp"
    break;

  case 52:
#line 720 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4671 "ds_parser.cpp"
    break;

  case 53:
#line 721 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4677 "ds_parser.cpp"
    break;

  case 54:
#line 725 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4683 "ds_parser.cpp"
    break;

  case 55:
#line 726 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4689 "ds_parser.cpp"
    break;

  case 56:
#line 727 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4700 "ds_parser.cpp"
    break;

  case 57:
#line 736 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4706 "ds_parser.cpp"
    break;

  case 58:
#line 737 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4712 "ds_parser.cpp"
    break;

  case 59:
#line 741 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4723 "ds_parser.cpp"
    break;

  case 60:
#line 750 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->visibility = tokAt((yylsp[0]));
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
#line 4741 "ds_parser.cpp"
    break;

  case 61:
#line 766 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4751 "ds_parser.cpp"
    break;

  case 62:
#line 774 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4763 "ds_parser.cpp"
    break;

  case 63:
#line 784 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4774 "ds_parser.cpp"
    break;

  case 64:
#line 793 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4780 "ds_parser.cpp"
    break;

  case 65:
#line 794 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4786 "ds_parser.cpp"
    break;

  case 66:
#line 795 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4792 "ds_parser.cpp"
    break;

  case 67:
#line 796 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4798 "ds_parser.cpp"
    break;

  case 68:
#line 797 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4804 "ds_parser.cpp"
    break;

  case 69:
#line 798 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4810 "ds_parser.cpp"
    break;

  case 70:
#line 802 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4820 "ds_parser.cpp"
    break;

  case 71:
#line 807 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4830 "ds_parser.cpp"
    break;

  case 72:
#line 815 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4836 "ds_parser.cpp"
    break;

  case 73:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4842 "ds_parser.cpp"
    break;

  case 74:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4848 "ds_parser.cpp"
    break;

  case 75:
#line 821 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4854 "ds_parser.cpp"
    break;

  case 76:
#line 822 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4860 "ds_parser.cpp"
    break;

  case 77:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4866 "ds_parser.cpp"
    break;

  case 78:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4872 "ds_parser.cpp"
    break;

  case 79:
#line 825 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4878 "ds_parser.cpp"
    break;

  case 80:
#line 826 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4884 "ds_parser.cpp"
    break;

  case 81:
#line 827 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4890 "ds_parser.cpp"
    break;

  case 82:
#line 828 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4898 "ds_parser.cpp"
    break;

  case 83:
#line 834 "ds_parser.ypp"
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
#line 4917 "ds_parser.cpp"
    break;

  case 84:
#line 848 "ds_parser.ypp"
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
#line 4936 "ds_parser.cpp"
    break;

  case 85:
#line 865 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4942 "ds_parser.cpp"
    break;

  case 86:
#line 866 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4948 "ds_parser.cpp"
    break;

  case 87:
#line 867 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4954 "ds_parser.cpp"
    break;

  case 88:
#line 871 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4967 "ds_parser.cpp"
    break;

  case 89:
#line 879 "ds_parser.ypp"
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
#line 4982 "ds_parser.cpp"
    break;

  case 90:
#line 892 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4991 "ds_parser.cpp"
    break;

  case 91:
#line 896 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5000 "ds_parser.cpp"
    break;

  case 92:
#line 903 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 5006 "ds_parser.cpp"
    break;

  case 93:
#line 904 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 5012 "ds_parser.cpp"
    break;

  case 94:
#line 908 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5018 "ds_parser.cpp"
    break;

  case 95:
#line 909 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5024 "ds_parser.cpp"
    break;

  case 96:
#line 910 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5030 "ds_parser.cpp"
    break;

  case 97:
#line 914 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5038 "ds_parser.cpp"
    break;

  case 98:
#line 917 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5047 "ds_parser.cpp"
    break;

  case 99:
#line 924 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5056 "ds_parser.cpp"
    break;

  case 100:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5062 "ds_parser.cpp"
    break;

  case 101:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5068 "ds_parser.cpp"
    break;

  case 102:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5074 "ds_parser.cpp"
    break;

  case 103:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5080 "ds_parser.cpp"
    break;

  case 104:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5086 "ds_parser.cpp"
    break;

  case 105:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5092 "ds_parser.cpp"
    break;

  case 106:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5098 "ds_parser.cpp"
    break;

  case 107:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5104 "ds_parser.cpp"
    break;

  case 108:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5110 "ds_parser.cpp"
    break;

  case 109:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5116 "ds_parser.cpp"
    break;

  case 110:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5122 "ds_parser.cpp"
    break;

  case 111:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5128 "ds_parser.cpp"
    break;

  case 112:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5134 "ds_parser.cpp"
    break;

  case 113:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5140 "ds_parser.cpp"
    break;

  case 114:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5146 "ds_parser.cpp"
    break;

  case 115:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5152 "ds_parser.cpp"
    break;

  case 116:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5158 "ds_parser.cpp"
    break;

  case 117:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5164 "ds_parser.cpp"
    break;

  case 118:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5170 "ds_parser.cpp"
    break;

  case 119:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5176 "ds_parser.cpp"
    break;

  case 120:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5182 "ds_parser.cpp"
    break;

  case 121:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5188 "ds_parser.cpp"
    break;

  case 122:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5194 "ds_parser.cpp"
    break;

  case 123:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5200 "ds_parser.cpp"
    break;

  case 124:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5206 "ds_parser.cpp"
    break;

  case 125:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5212 "ds_parser.cpp"
    break;

  case 126:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5218 "ds_parser.cpp"
    break;

  case 127:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5224 "ds_parser.cpp"
    break;

  case 128:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5230 "ds_parser.cpp"
    break;

  case 129:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5236 "ds_parser.cpp"
    break;

  case 130:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5242 "ds_parser.cpp"
    break;

  case 131:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5248 "ds_parser.cpp"
    break;

  case 132:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5254 "ds_parser.cpp"
    break;

  case 133:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5260 "ds_parser.cpp"
    break;

  case 134:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5266 "ds_parser.cpp"
    break;

  case 135:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5272 "ds_parser.cpp"
    break;

  case 136:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5278 "ds_parser.cpp"
    break;

  case 137:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5284 "ds_parser.cpp"
    break;

  case 138:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5290 "ds_parser.cpp"
    break;

  case 139:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5296 "ds_parser.cpp"
    break;

  case 140:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5302 "ds_parser.cpp"
    break;

  case 141:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5308 "ds_parser.cpp"
    break;

  case 142:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5314 "ds_parser.cpp"
    break;

  case 143:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5320 "ds_parser.cpp"
    break;

  case 144:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5326 "ds_parser.cpp"
    break;

  case 145:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5332 "ds_parser.cpp"
    break;

  case 146:
#line 977 "ds_parser.ypp"
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
#line 5356 "ds_parser.cpp"
    break;

  case 147:
#line 999 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5362 "ds_parser.cpp"
    break;

  case 148:
#line 1000 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5368 "ds_parser.cpp"
    break;

  case 149:
#line 1001 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5374 "ds_parser.cpp"
    break;

  case 150:
#line 1005 "ds_parser.ypp"
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
#line 5415 "ds_parser.cpp"
    break;

  case 151:
#line 1044 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5424 "ds_parser.cpp"
    break;

  case 152:
#line 1051 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5433 "ds_parser.cpp"
    break;

  case 153:
#line 1055 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5446 "ds_parser.cpp"
    break;

  case 154:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5452 "ds_parser.cpp"
    break;

  case 155:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5458 "ds_parser.cpp"
    break;

  case 156:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5464 "ds_parser.cpp"
    break;

  case 157:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5470 "ds_parser.cpp"
    break;

  case 158:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5476 "ds_parser.cpp"
    break;

  case 159:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5482 "ds_parser.cpp"
    break;

  case 160:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5488 "ds_parser.cpp"
    break;

  case 161:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5494 "ds_parser.cpp"
    break;

  case 162:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5500 "ds_parser.cpp"
    break;

  case 163:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5506 "ds_parser.cpp"
    break;

  case 164:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5512 "ds_parser.cpp"
    break;

  case 165:
#line 1077 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5518 "ds_parser.cpp"
    break;

  case 166:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5524 "ds_parser.cpp"
    break;

  case 167:
#line 1079 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5530 "ds_parser.cpp"
    break;

  case 168:
#line 1080 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5536 "ds_parser.cpp"
    break;

  case 169:
#line 1081 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5542 "ds_parser.cpp"
    break;

  case 170:
#line 1082 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5548 "ds_parser.cpp"
    break;

  case 171:
#line 1083 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5554 "ds_parser.cpp"
    break;

  case 172:
#line 1084 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5560 "ds_parser.cpp"
    break;

  case 173:
#line 1088 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5570 "ds_parser.cpp"
    break;

  case 174:
#line 1093 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5581 "ds_parser.cpp"
    break;

  case 175:
#line 1099 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5589 "ds_parser.cpp"
    break;

  case 176:
#line 1105 "ds_parser.ypp"
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
#line 5628 "ds_parser.cpp"
    break;

  case 177:
#line 1139 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5636 "ds_parser.cpp"
    break;

  case 178:
#line 1142 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5644 "ds_parser.cpp"
    break;

  case 179:
#line 1148 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5650 "ds_parser.cpp"
    break;

  case 180:
#line 1149 "ds_parser.ypp"
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
#line 5666 "ds_parser.cpp"
    break;

  case 181:
#line 1160 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5672 "ds_parser.cpp"
    break;

  case 182:
#line 1164 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5680 "ds_parser.cpp"
    break;

  case 183:
#line 1170 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5688 "ds_parser.cpp"
    break;

  case 184:
#line 1173 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5696 "ds_parser.cpp"
    break;

  case 185:
#line 1176 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5705 "ds_parser.cpp"
    break;

  case 186:
#line 1180 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5713 "ds_parser.cpp"
    break;

  case 187:
#line 1186 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5719 "ds_parser.cpp"
    break;

  case 188:
#line 1190 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5725 "ds_parser.cpp"
    break;

  case 189:
#line 1194 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5733 "ds_parser.cpp"
    break;

  case 190:
#line 1197 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5741 "ds_parser.cpp"
    break;

  case 191:
#line 1200 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5751 "ds_parser.cpp"
    break;

  case 192:
#line 1205 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5759 "ds_parser.cpp"
    break;

  case 193:
#line 1208 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5769 "ds_parser.cpp"
    break;

  case 194:
#line 1216 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5777 "ds_parser.cpp"
    break;

  case 195:
#line 1219 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5787 "ds_parser.cpp"
    break;

  case 196:
#line 1224 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5795 "ds_parser.cpp"
    break;

  case 197:
#line 1227 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5805 "ds_parser.cpp"
    break;

  case 198:
#line 1235 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5813 "ds_parser.cpp"
    break;

  case 199:
#line 1241 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5819 "ds_parser.cpp"
    break;

  case 200:
#line 1242 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5825 "ds_parser.cpp"
    break;

  case 201:
#line 1246 "ds_parser.ypp"
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
#line 5861 "ds_parser.cpp"
    break;

  case 202:
#line 1280 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5867 "ds_parser.cpp"
    break;

  case 203:
#line 1280 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5873 "ds_parser.cpp"
    break;

  case 204:
#line 1280 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5881 "ds_parser.cpp"
    break;

  case 205:
#line 1283 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5887 "ds_parser.cpp"
    break;

  case 206:
#line 1283 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5893 "ds_parser.cpp"
    break;

  case 207:
#line 1283 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5903 "ds_parser.cpp"
    break;

  case 208:
#line 1288 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5909 "ds_parser.cpp"
    break;

  case 209:
#line 1288 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5915 "ds_parser.cpp"
    break;

  case 210:
#line 1288 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5925 "ds_parser.cpp"
    break;

  case 211:
#line 1296 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5931 "ds_parser.cpp"
    break;

  case 212:
#line 1296 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5937 "ds_parser.cpp"
    break;

  case 213:
#line 1296 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5945 "ds_parser.cpp"
    break;

  case 214:
#line 1302 "ds_parser.ypp"
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
#line 5960 "ds_parser.cpp"
    break;

  case 215:
#line 1312 "ds_parser.ypp"
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
#line 5976 "ds_parser.cpp"
    break;

  case 216:
#line 1323 "ds_parser.ypp"
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
#line 5993 "ds_parser.cpp"
    break;

  case 217:
#line 1338 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6001 "ds_parser.cpp"
    break;

  case 218:
#line 1341 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6009 "ds_parser.cpp"
    break;

  case 219:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6015 "ds_parser.cpp"
    break;

  case 220:
#line 1348 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6027 "ds_parser.cpp"
    break;

  case 221:
#line 1355 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6040 "ds_parser.cpp"
    break;

  case 222:
#line 1366 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6046 "ds_parser.cpp"
    break;

  case 223:
#line 1367 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6052 "ds_parser.cpp"
    break;

  case 224:
#line 1368 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6058 "ds_parser.cpp"
    break;

  case 225:
#line 1372 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6064 "ds_parser.cpp"
    break;

  case 226:
#line 1373 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6070 "ds_parser.cpp"
    break;

  case 227:
#line 1374 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6076 "ds_parser.cpp"
    break;

  case 228:
#line 1375 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6082 "ds_parser.cpp"
    break;

  case 229:
#line 1379 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6092 "ds_parser.cpp"
    break;

  case 230:
#line 1384 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6102 "ds_parser.cpp"
    break;

  case 231:
#line 1392 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6108 "ds_parser.cpp"
    break;

  case 232:
#line 1393 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6114 "ds_parser.cpp"
    break;

  case 233:
#line 1397 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6124 "ds_parser.cpp"
    break;

  case 234:
#line 1403 "ds_parser.ypp"
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
#line 6189 "ds_parser.cpp"
    break;

  case 235:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6195 "ds_parser.cpp"
    break;

  case 236:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6201 "ds_parser.cpp"
    break;

  case 237:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6207 "ds_parser.cpp"
    break;

  case 238:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6213 "ds_parser.cpp"
    break;

  case 239:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6219 "ds_parser.cpp"
    break;

  case 240:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6225 "ds_parser.cpp"
    break;

  case 241:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6231 "ds_parser.cpp"
    break;

  case 242:
#line 1476 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6237 "ds_parser.cpp"
    break;

  case 243:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6243 "ds_parser.cpp"
    break;

  case 244:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6249 "ds_parser.cpp"
    break;

  case 245:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6255 "ds_parser.cpp"
    break;

  case 246:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6261 "ds_parser.cpp"
    break;

  case 247:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6267 "ds_parser.cpp"
    break;

  case 248:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6273 "ds_parser.cpp"
    break;

  case 249:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6279 "ds_parser.cpp"
    break;

  case 250:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6285 "ds_parser.cpp"
    break;

  case 251:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6291 "ds_parser.cpp"
    break;

  case 252:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6297 "ds_parser.cpp"
    break;

  case 253:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6303 "ds_parser.cpp"
    break;

  case 254:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6309 "ds_parser.cpp"
    break;

  case 255:
#line 1489 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6315 "ds_parser.cpp"
    break;

  case 256:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6321 "ds_parser.cpp"
    break;

  case 257:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6327 "ds_parser.cpp"
    break;

  case 258:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6333 "ds_parser.cpp"
    break;

  case 259:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6339 "ds_parser.cpp"
    break;

  case 260:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6345 "ds_parser.cpp"
    break;

  case 261:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6351 "ds_parser.cpp"
    break;

  case 262:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6357 "ds_parser.cpp"
    break;

  case 263:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6363 "ds_parser.cpp"
    break;

  case 264:
#line 1504 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6375 "ds_parser.cpp"
    break;

  case 265:
#line 1515 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6385 "ds_parser.cpp"
    break;

  case 266:
#line 1520 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6397 "ds_parser.cpp"
    break;

  case 267:
#line 1530 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6406 "ds_parser.cpp"
    break;

  case 268:
#line 1534 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6412 "ds_parser.cpp"
    break;

  case 269:
#line 1534 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6418 "ds_parser.cpp"
    break;

  case 270:
#line 1534 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6430 "ds_parser.cpp"
    break;

  case 271:
#line 1541 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6436 "ds_parser.cpp"
    break;

  case 272:
#line 1541 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6442 "ds_parser.cpp"
    break;

  case 273:
#line 1541 "ds_parser.ypp"
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
#line 6458 "ds_parser.cpp"
    break;

  case 274:
#line 1555 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6467 "ds_parser.cpp"
    break;

  case 275:
#line 1559 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6476 "ds_parser.cpp"
    break;

  case 276:
#line 1563 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6482 "ds_parser.cpp"
    break;

  case 277:
#line 1563 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6488 "ds_parser.cpp"
    break;

  case 278:
#line 1563 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6497 "ds_parser.cpp"
    break;

  case 279:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6503 "ds_parser.cpp"
    break;

  case 280:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6509 "ds_parser.cpp"
    break;

  case 281:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6515 "ds_parser.cpp"
    break;

  case 282:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6521 "ds_parser.cpp"
    break;

  case 283:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6527 "ds_parser.cpp"
    break;

  case 284:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6533 "ds_parser.cpp"
    break;

  case 285:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6539 "ds_parser.cpp"
    break;

  case 286:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6545 "ds_parser.cpp"
    break;

  case 287:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6551 "ds_parser.cpp"
    break;

  case 288:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6557 "ds_parser.cpp"
    break;

  case 289:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6563 "ds_parser.cpp"
    break;

  case 290:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6569 "ds_parser.cpp"
    break;

  case 291:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6575 "ds_parser.cpp"
    break;

  case 292:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6581 "ds_parser.cpp"
    break;

  case 293:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6587 "ds_parser.cpp"
    break;

  case 294:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6593 "ds_parser.cpp"
    break;

  case 295:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6599 "ds_parser.cpp"
    break;

  case 296:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6605 "ds_parser.cpp"
    break;

  case 297:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6611 "ds_parser.cpp"
    break;

  case 298:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6617 "ds_parser.cpp"
    break;

  case 299:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6623 "ds_parser.cpp"
    break;

  case 300:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6629 "ds_parser.cpp"
    break;

  case 301:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6635 "ds_parser.cpp"
    break;

  case 302:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6641 "ds_parser.cpp"
    break;

  case 303:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6647 "ds_parser.cpp"
    break;

  case 304:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6653 "ds_parser.cpp"
    break;

  case 305:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6659 "ds_parser.cpp"
    break;

  case 306:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6665 "ds_parser.cpp"
    break;

  case 307:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6671 "ds_parser.cpp"
    break;

  case 308:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6677 "ds_parser.cpp"
    break;

  case 309:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6683 "ds_parser.cpp"
    break;

  case 310:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6689 "ds_parser.cpp"
    break;

  case 311:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6695 "ds_parser.cpp"
    break;

  case 312:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6701 "ds_parser.cpp"
    break;

  case 313:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6707 "ds_parser.cpp"
    break;

  case 314:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6713 "ds_parser.cpp"
    break;

  case 315:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6719 "ds_parser.cpp"
    break;

  case 316:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6725 "ds_parser.cpp"
    break;

  case 317:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6731 "ds_parser.cpp"
    break;

  case 318:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6737 "ds_parser.cpp"
    break;

  case 319:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6743 "ds_parser.cpp"
    break;

  case 320:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6749 "ds_parser.cpp"
    break;

  case 321:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6755 "ds_parser.cpp"
    break;

  case 322:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6761 "ds_parser.cpp"
    break;

  case 323:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6767 "ds_parser.cpp"
    break;

  case 324:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6773 "ds_parser.cpp"
    break;

  case 325:
#line 1616 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6782 "ds_parser.cpp"
    break;

  case 326:
#line 1620 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6791 "ds_parser.cpp"
    break;

  case 327:
#line 1624 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6799 "ds_parser.cpp"
    break;

  case 328:
#line 1627 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6807 "ds_parser.cpp"
    break;

  case 329:
#line 1630 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6813 "ds_parser.cpp"
    break;

  case 330:
#line 1631 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6819 "ds_parser.cpp"
    break;

  case 331:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6825 "ds_parser.cpp"
    break;

  case 332:
#line 1633 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6839 "ds_parser.cpp"
    break;

  case 333:
#line 1642 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6853 "ds_parser.cpp"
    break;

  case 334:
#line 1651 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6859 "ds_parser.cpp"
    break;

  case 335:
#line 1652 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6867 "ds_parser.cpp"
    break;

  case 336:
#line 1655 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6873 "ds_parser.cpp"
    break;

  case 337:
#line 1655 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6879 "ds_parser.cpp"
    break;

  case 338:
#line 1655 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6887 "ds_parser.cpp"
    break;

  case 339:
#line 1658 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6896 "ds_parser.cpp"
    break;

  case 340:
#line 1662 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6905 "ds_parser.cpp"
    break;

  case 341:
#line 1666 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6914 "ds_parser.cpp"
    break;

  case 342:
#line 1670 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6920 "ds_parser.cpp"
    break;

  case 343:
#line 1671 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6926 "ds_parser.cpp"
    break;

  case 344:
#line 1672 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6932 "ds_parser.cpp"
    break;

  case 345:
#line 1673 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6938 "ds_parser.cpp"
    break;

  case 346:
#line 1674 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6944 "ds_parser.cpp"
    break;

  case 347:
#line 1675 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6950 "ds_parser.cpp"
    break;

  case 348:
#line 1676 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6956 "ds_parser.cpp"
    break;

  case 349:
#line 1677 "ds_parser.ypp"
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
#line 6977 "ds_parser.cpp"
    break;

  case 350:
#line 1693 "ds_parser.ypp"
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
#line 6998 "ds_parser.cpp"
    break;

  case 351:
#line 1709 "ds_parser.ypp"
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
#line 7058 "ds_parser.cpp"
    break;

  case 352:
#line 1767 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7064 "ds_parser.cpp"
    break;

  case 353:
#line 1768 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7070 "ds_parser.cpp"
    break;

  case 354:
#line 1772 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7076 "ds_parser.cpp"
    break;

  case 355:
#line 1773 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7082 "ds_parser.cpp"
    break;

  case 356:
#line 1777 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7088 "ds_parser.cpp"
    break;

  case 357:
#line 1778 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7094 "ds_parser.cpp"
    break;

  case 358:
#line 1782 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7104 "ds_parser.cpp"
    break;

  case 359:
#line 1790 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7112 "ds_parser.cpp"
    break;

  case 360:
#line 1793 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7121 "ds_parser.cpp"
    break;

  case 361:
#line 1798 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7127 "ds_parser.cpp"
    break;

  case 362:
#line 1798 "ds_parser.ypp"
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
#line 7176 "ds_parser.cpp"
    break;

  case 363:
#line 1842 "ds_parser.ypp"
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt((yylsp[-3]),(yylsp[0]));
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
                if ( (yyvsp[-2].b) ) {
                    finit = new ExprCast((yyvsp[0].pFuncDecl)->at, finit, make_smart<TypeDecl>(Type::autoinfer));
                }
                VariableDeclaration * decl = new VariableDeclaration(
                    vars,
                    new TypeDecl(Type::autoinfer),
                    finit
                );
                decl->override = (yyvsp[-2].b);
                (yyvsp[-5].pVarDeclList)->push_back(decl);
                modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure, false, (yyvsp[-1].b));
            } else {
                if ( (yyvsp[-2].b) ) {
                    das_yyerror("can't override initializer or finalizer " + (yyvsp[0].pFuncDecl)->getMangledName(),
                        (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[-1].b) ) {
                    das_yyerror("can't have constant initializer or finalizer " + (yyvsp[0].pFuncDecl)->getMangledName(),
                        (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[0].pFuncDecl)->name!="finalize" ) {
                    auto ctr = makeClassConstructor(g_thisStructure, (yyvsp[0].pFuncDecl));
                    if ( !g_Program->addFunction(ctr) ) {
                        das_yyerror("intializer is already defined " + ctr->getMangledName(),
                            ctr->at, CompilationError::function_already_declared);
                    }
                    (yyvsp[0].pFuncDecl)->name = g_thisStructure->name + "`" + g_thisStructure->name;
                    modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure, false, false);
                } else {
                    modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure, true, false);
                }
            }
            assignDefaultArguments((yyvsp[0].pFuncDecl));
            runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-4].faList), tokAt((yylsp[-4])));
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[0].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
#line 7241 "ds_parser.cpp"
    break;

  case 364:
#line 1902 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7252 "ds_parser.cpp"
    break;

  case 365:
#line 1911 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7262 "ds_parser.cpp"
    break;

  case 366:
#line 1916 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7272 "ds_parser.cpp"
    break;

  case 367:
#line 1924 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7278 "ds_parser.cpp"
    break;

  case 368:
#line 1925 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7284 "ds_parser.cpp"
    break;

  case 369:
#line 1929 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7292 "ds_parser.cpp"
    break;

  case 370:
#line 1932 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7303 "ds_parser.cpp"
    break;

  case 371:
#line 1941 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7309 "ds_parser.cpp"
    break;

  case 372:
#line 1942 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7315 "ds_parser.cpp"
    break;

  case 373:
#line 1946 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7326 "ds_parser.cpp"
    break;

  case 374:
#line 1955 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7332 "ds_parser.cpp"
    break;

  case 375:
#line 1956 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7338 "ds_parser.cpp"
    break;

  case 376:
#line 1961 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7344 "ds_parser.cpp"
    break;

  case 377:
#line 1962 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7350 "ds_parser.cpp"
    break;

  case 378:
#line 1966 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7361 "ds_parser.cpp"
    break;

  case 379:
#line 1972 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7372 "ds_parser.cpp"
    break;

  case 380:
#line 1978 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7380 "ds_parser.cpp"
    break;

  case 381:
#line 1981 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7389 "ds_parser.cpp"
    break;

  case 382:
#line 1985 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7400 "ds_parser.cpp"
    break;

  case 383:
#line 1991 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7411 "ds_parser.cpp"
    break;

  case 384:
#line 2000 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7417 "ds_parser.cpp"
    break;

  case 385:
#line 2001 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7423 "ds_parser.cpp"
    break;

  case 386:
#line 2002 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7429 "ds_parser.cpp"
    break;

  case 387:
#line 2006 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7435 "ds_parser.cpp"
    break;

  case 388:
#line 2007 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7441 "ds_parser.cpp"
    break;

  case 389:
#line 2011 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7449 "ds_parser.cpp"
    break;

  case 390:
#line 2014 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7459 "ds_parser.cpp"
    break;

  case 391:
#line 2019 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7469 "ds_parser.cpp"
    break;

  case 392:
#line 2024 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7482 "ds_parser.cpp"
    break;

  case 393:
#line 2032 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7495 "ds_parser.cpp"
    break;

  case 394:
#line 2043 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7503 "ds_parser.cpp"
    break;

  case 395:
#line 2046 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7513 "ds_parser.cpp"
    break;

  case 396:
#line 2054 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7519 "ds_parser.cpp"
    break;

  case 397:
#line 2055 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7525 "ds_parser.cpp"
    break;

  case 398:
#line 2059 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7531 "ds_parser.cpp"
    break;

  case 399:
#line 2060 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7537 "ds_parser.cpp"
    break;

  case 400:
#line 2061 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7543 "ds_parser.cpp"
    break;

  case 401:
#line 2065 "ds_parser.ypp"
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
#line 7576 "ds_parser.cpp"
    break;

  case 402:
#line 2093 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7582 "ds_parser.cpp"
    break;

  case 403:
#line 2093 "ds_parser.ypp"
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
                if ( (yyvsp[-5].b) ) {
                    pVar->type->constant = true;
                } else {
                    pVar->type->removeConstant = true;
                }
                pVar->global_shared = (yyvsp[-4].b);
                pVar->private_variable = !(yyvsp[-3].b);
                if ( (yyvsp[-1].aaList) ) {
                    pVar->annotation = move(*(yyvsp[-1].aaList));
                    delete (yyvsp[-1].aaList);
                }
                if ( !g_Program->addVariable(pVar) )
                    das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }
#line 7618 "ds_parser.cpp"
    break;

  case 404:
#line 2127 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7626 "ds_parser.cpp"
    break;

  case 405:
#line 2130 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7640 "ds_parser.cpp"
    break;

  case 406:
#line 2139 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7654 "ds_parser.cpp"
    break;

  case 407:
#line 2152 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7668 "ds_parser.cpp"
    break;

  case 412:
#line 2174 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7674 "ds_parser.cpp"
    break;

  case 413:
#line 2175 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7680 "ds_parser.cpp"
    break;

  case 414:
#line 2176 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7686 "ds_parser.cpp"
    break;

  case 415:
#line 2180 "ds_parser.ypp"
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
#line 7719 "ds_parser.cpp"
    break;

  case 416:
#line 2208 "ds_parser.ypp"
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
#line 7753 "ds_parser.cpp"
    break;

  case 417:
#line 2240 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7759 "ds_parser.cpp"
    break;

  case 418:
#line 2241 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7765 "ds_parser.cpp"
    break;

  case 419:
#line 2245 "ds_parser.ypp"
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
                pStruct->macroInterface = pStruct->parent && pStruct->parent->macroInterface;
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
#line 7805 "ds_parser.cpp"
    break;

  case 420:
#line 2283 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7811 "ds_parser.cpp"
    break;

  case 421:
#line 2284 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7817 "ds_parser.cpp"
    break;

  case 422:
#line 2288 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7823 "ds_parser.cpp"
    break;

  case 423:
#line 2289 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7829 "ds_parser.cpp"
    break;

  case 424:
#line 2290 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7835 "ds_parser.cpp"
    break;

  case 425:
#line 2295 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7841 "ds_parser.cpp"
    break;

  case 426:
#line 2295 "ds_parser.ypp"
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
#line 7927 "ds_parser.cpp"
    break;

  case 427:
#line 2379 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7939 "ds_parser.cpp"
    break;

  case 428:
#line 2386 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7950 "ds_parser.cpp"
    break;

  case 429:
#line 2395 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7956 "ds_parser.cpp"
    break;

  case 430:
#line 2396 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7962 "ds_parser.cpp"
    break;

  case 431:
#line 2397 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7968 "ds_parser.cpp"
    break;

  case 432:
#line 2398 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7974 "ds_parser.cpp"
    break;

  case 433:
#line 2399 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7980 "ds_parser.cpp"
    break;

  case 434:
#line 2400 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7986 "ds_parser.cpp"
    break;

  case 435:
#line 2401 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7992 "ds_parser.cpp"
    break;

  case 436:
#line 2402 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7998 "ds_parser.cpp"
    break;

  case 437:
#line 2403 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8004 "ds_parser.cpp"
    break;

  case 438:
#line 2404 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8010 "ds_parser.cpp"
    break;

  case 439:
#line 2405 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8016 "ds_parser.cpp"
    break;

  case 440:
#line 2406 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8022 "ds_parser.cpp"
    break;

  case 441:
#line 2407 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8028 "ds_parser.cpp"
    break;

  case 442:
#line 2408 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8034 "ds_parser.cpp"
    break;

  case 443:
#line 2409 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8040 "ds_parser.cpp"
    break;

  case 444:
#line 2410 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8046 "ds_parser.cpp"
    break;

  case 445:
#line 2411 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8052 "ds_parser.cpp"
    break;

  case 446:
#line 2412 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8058 "ds_parser.cpp"
    break;

  case 447:
#line 2413 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8064 "ds_parser.cpp"
    break;

  case 448:
#line 2414 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8070 "ds_parser.cpp"
    break;

  case 449:
#line 2415 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8076 "ds_parser.cpp"
    break;

  case 450:
#line 2416 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8082 "ds_parser.cpp"
    break;

  case 451:
#line 2417 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8088 "ds_parser.cpp"
    break;

  case 452:
#line 2418 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8094 "ds_parser.cpp"
    break;

  case 453:
#line 2419 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8100 "ds_parser.cpp"
    break;

  case 454:
#line 2423 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8106 "ds_parser.cpp"
    break;

  case 455:
#line 2424 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8112 "ds_parser.cpp"
    break;

  case 456:
#line 2425 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8118 "ds_parser.cpp"
    break;

  case 457:
#line 2426 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8124 "ds_parser.cpp"
    break;

  case 458:
#line 2427 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8130 "ds_parser.cpp"
    break;

  case 459:
#line 2428 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8136 "ds_parser.cpp"
    break;

  case 460:
#line 2432 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8149 "ds_parser.cpp"
    break;

  case 461:
#line 2443 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8158 "ds_parser.cpp"
    break;

  case 462:
#line 2447 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8170 "ds_parser.cpp"
    break;

  case 463:
#line 2457 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8182 "ds_parser.cpp"
    break;

  case 464:
#line 2464 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8193 "ds_parser.cpp"
    break;

  case 465:
#line 2473 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8199 "ds_parser.cpp"
    break;

  case 466:
#line 2473 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8205 "ds_parser.cpp"
    break;

  case 467:
#line 2473 "ds_parser.ypp"
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
#line 8220 "ds_parser.cpp"
    break;

  case 468:
#line 2486 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8226 "ds_parser.cpp"
    break;

  case 469:
#line 2487 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8232 "ds_parser.cpp"
    break;

  case 470:
#line 2488 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8238 "ds_parser.cpp"
    break;

  case 471:
#line 2489 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8244 "ds_parser.cpp"
    break;

  case 472:
#line 2490 "ds_parser.ypp"
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
#line 8263 "ds_parser.cpp"
    break;

  case 473:
#line 2504 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8274 "ds_parser.cpp"
    break;

  case 474:
#line 2510 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8283 "ds_parser.cpp"
    break;

  case 475:
#line 2514 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8292 "ds_parser.cpp"
    break;

  case 476:
#line 2518 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8302 "ds_parser.cpp"
    break;

  case 477:
#line 2523 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8312 "ds_parser.cpp"
    break;

  case 478:
#line 2528 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8322 "ds_parser.cpp"
    break;

  case 479:
#line 2533 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8332 "ds_parser.cpp"
    break;

  case 480:
#line 2538 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8341 "ds_parser.cpp"
    break;

  case 481:
#line 2542 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8350 "ds_parser.cpp"
    break;

  case 482:
#line 2546 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8360 "ds_parser.cpp"
    break;

  case 483:
#line 2551 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8369 "ds_parser.cpp"
    break;

  case 484:
#line 2555 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8379 "ds_parser.cpp"
    break;

  case 485:
#line 2560 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8385 "ds_parser.cpp"
    break;

  case 486:
#line 2560 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8391 "ds_parser.cpp"
    break;

  case 487:
#line 2560 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8402 "ds_parser.cpp"
    break;

  case 488:
#line 2566 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8414 "ds_parser.cpp"
    break;

  case 489:
#line 2573 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8420 "ds_parser.cpp"
    break;

  case 490:
#line 2573 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8426 "ds_parser.cpp"
    break;

  case 491:
#line 2573 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8436 "ds_parser.cpp"
    break;

  case 492:
#line 2578 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8442 "ds_parser.cpp"
    break;

  case 493:
#line 2578 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8448 "ds_parser.cpp"
    break;

  case 494:
#line 2578 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8459 "ds_parser.cpp"
    break;

  case 495:
#line 2584 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8465 "ds_parser.cpp"
    break;

  case 496:
#line 2584 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8471 "ds_parser.cpp"
    break;

  case 497:
#line 2584 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8481 "ds_parser.cpp"
    break;

  case 498:
#line 2589 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8490 "ds_parser.cpp"
    break;

  case 499:
#line 2593 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8496 "ds_parser.cpp"
    break;

  case 500:
#line 2593 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8502 "ds_parser.cpp"
    break;

  case 501:
#line 2593 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8512 "ds_parser.cpp"
    break;

  case 502:
#line 2598 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8518 "ds_parser.cpp"
    break;

  case 503:
#line 2598 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8524 "ds_parser.cpp"
    break;

  case 504:
#line 2598 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8538 "ds_parser.cpp"
    break;

  case 505:
#line 2607 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8547 "ds_parser.cpp"
    break;

  case 506:
#line 2611 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8553 "ds_parser.cpp"
    break;

  case 507:
#line 2611 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8559 "ds_parser.cpp"
    break;

  case 508:
#line 2611 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8569 "ds_parser.cpp"
    break;

  case 509:
#line 2616 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8575 "ds_parser.cpp"
    break;

  case 510:
#line 2616 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8581 "ds_parser.cpp"
    break;

  case 511:
#line 2616 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8595 "ds_parser.cpp"
    break;

  case 512:
#line 2625 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8604 "ds_parser.cpp"
    break;

  case 513:
#line 2629 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8610 "ds_parser.cpp"
    break;

  case 514:
#line 2629 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8616 "ds_parser.cpp"
    break;

  case 515:
#line 2629 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8626 "ds_parser.cpp"
    break;

  case 516:
#line 2634 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8632 "ds_parser.cpp"
    break;

  case 517:
#line 2634 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8638 "ds_parser.cpp"
    break;

  case 518:
#line 2634 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8652 "ds_parser.cpp"
    break;

  case 519:
#line 2643 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8658 "ds_parser.cpp"
    break;

  case 520:
#line 2643 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8664 "ds_parser.cpp"
    break;

  case 521:
#line 2643 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8675 "ds_parser.cpp"
    break;

  case 522:
#line 2649 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8681 "ds_parser.cpp"
    break;

  case 523:
#line 2649 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8687 "ds_parser.cpp"
    break;

  case 524:
#line 2649 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8698 "ds_parser.cpp"
    break;

  case 525:
#line 2658 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8704 "ds_parser.cpp"
    break;

  case 526:
#line 2658 "ds_parser.ypp"
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
#line 8721 "ds_parser.cpp"
    break;

  case 527:
#line 2673 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8727 "ds_parser.cpp"
    break;

  case 528:
#line 2673 "ds_parser.ypp"
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
#line 8748 "ds_parser.cpp"
    break;

  case 529:
#line 2693 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8754 "ds_parser.cpp"
    break;

  case 530:
#line 2694 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8760 "ds_parser.cpp"
    break;

  case 531:
#line 2695 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8766 "ds_parser.cpp"
    break;

  case 532:
#line 2696 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8772 "ds_parser.cpp"
    break;

  case 533:
#line 2700 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8784 "ds_parser.cpp"
    break;

  case 534:
#line 2707 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8796 "ds_parser.cpp"
    break;

  case 535:
#line 2714 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8807 "ds_parser.cpp"
    break;

  case 536:
#line 2720 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8818 "ds_parser.cpp"
    break;

  case 537:
#line 2729 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8828 "ds_parser.cpp"
    break;

  case 538:
#line 2734 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8837 "ds_parser.cpp"
    break;

  case 539:
#line 2741 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8843 "ds_parser.cpp"
    break;

  case 540:
#line 2742 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8849 "ds_parser.cpp"
    break;

  case 541:
#line 2746 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8860 "ds_parser.cpp"
    break;

  case 542:
#line 2752 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8872 "ds_parser.cpp"
    break;

  case 543:
#line 2759 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8885 "ds_parser.cpp"
    break;

  case 544:
#line 2767 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8897 "ds_parser.cpp"
    break;

  case 545:
#line 2774 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8910 "ds_parser.cpp"
    break;

  case 546:
#line 2782 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8924 "ds_parser.cpp"
    break;

  case 547:
#line 2794 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8932 "ds_parser.cpp"
    break;

  case 548:
#line 2797 "ds_parser.ypp"
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
#line 8948 "ds_parser.cpp"
    break;

  case 549:
#line 2811 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8959 "ds_parser.cpp"
    break;

  case 550:
#line 2820 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8965 "ds_parser.cpp"
    break;

  case 551:
#line 2821 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8971 "ds_parser.cpp"
    break;

  case 552:
#line 2825 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8981 "ds_parser.cpp"
    break;

  case 553:
#line 2830 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8990 "ds_parser.cpp"
    break;

  case 554:
#line 2837 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9000 "ds_parser.cpp"
    break;

  case 555:
#line 2842 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9012 "ds_parser.cpp"
    break;

  case 556:
#line 2852 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9022 "ds_parser.cpp"
    break;

  case 557:
#line 2857 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9031 "ds_parser.cpp"
    break;

  case 558:
#line 2864 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9045 "ds_parser.cpp"
    break;

  case 559:
#line 2876 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9051 "ds_parser.cpp"
    break;

  case 560:
#line 2877 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9057 "ds_parser.cpp"
    break;

  case 561:
#line 2881 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->visibility = tokRangeAt((yylsp[-3]),(yylsp[0]));
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
#line 9080 "ds_parser.cpp"
    break;

  case 562:
#line 2899 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->visibility = tokRangeAt((yylsp[-3]),(yylsp[0]));
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
#line 9103 "ds_parser.cpp"
    break;


#line 9107 "ds_parser.cpp"

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
#line 2919 "ds_parser.ypp"


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
