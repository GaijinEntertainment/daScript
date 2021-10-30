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
#define YYLAST   7598

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  180
/* YYNRULES -- Number of rules.  */
#define YYNRULES  564
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  985

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
     968,   969,   970,   971,   972,   973,   974,   975,   979,  1001,
    1002,  1003,  1007,  1046,  1053,  1057,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1090,  1095,  1101,  1107,  1141,
    1144,  1150,  1151,  1162,  1166,  1172,  1175,  1178,  1182,  1188,
    1192,  1196,  1199,  1202,  1207,  1210,  1218,  1221,  1226,  1229,
    1237,  1243,  1244,  1248,  1282,  1282,  1282,  1285,  1285,  1285,
    1290,  1290,  1290,  1298,  1298,  1298,  1304,  1314,  1325,  1340,
    1343,  1349,  1350,  1357,  1368,  1369,  1370,  1374,  1375,  1376,
    1377,  1381,  1386,  1394,  1395,  1399,  1404,  1468,  1469,  1470,
    1471,  1472,  1473,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,
    1494,  1495,  1499,  1500,  1501,  1502,  1506,  1517,  1522,  1532,
    1536,  1536,  1536,  1543,  1543,  1543,  1557,  1561,  1565,  1565,
    1565,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,
    1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1622,  1626,
    1629,  1632,  1633,  1634,  1635,  1644,  1653,  1654,  1657,  1657,
    1657,  1660,  1664,  1668,  1672,  1673,  1674,  1675,  1676,  1677,
    1678,  1679,  1695,  1711,  1769,  1770,  1774,  1775,  1779,  1780,
    1784,  1792,  1795,  1800,  1799,  1844,  1904,  1913,  1918,  1926,
    1927,  1931,  1934,  1943,  1944,  1948,  1957,  1958,  1963,  1964,
    1968,  1974,  1980,  1983,  1987,  1993,  2002,  2003,  2004,  2008,
    2009,  2013,  2016,  2021,  2026,  2034,  2045,  2048,  2056,  2057,
    2061,  2062,  2063,  2067,  2095,  2095,  2129,  2132,  2141,  2154,
    2166,  2167,  2171,  2172,  2176,  2177,  2178,  2182,  2210,  2242,
    2243,  2247,  2285,  2286,  2290,  2291,  2292,  2297,  2296,  2381,
    2388,  2397,  2398,  2399,  2400,  2401,  2402,  2403,  2404,  2405,
    2406,  2407,  2408,  2409,  2410,  2411,  2412,  2413,  2414,  2415,
    2416,  2417,  2418,  2419,  2420,  2421,  2425,  2426,  2427,  2428,
    2429,  2430,  2434,  2445,  2449,  2459,  2466,  2475,  2475,  2475,
    2488,  2489,  2490,  2491,  2492,  2506,  2512,  2516,  2520,  2525,
    2530,  2535,  2540,  2544,  2548,  2553,  2557,  2562,  2562,  2562,
    2568,  2575,  2575,  2575,  2580,  2580,  2580,  2586,  2586,  2586,
    2591,  2595,  2595,  2595,  2600,  2600,  2600,  2609,  2613,  2613,
    2613,  2618,  2618,  2618,  2627,  2631,  2631,  2631,  2636,  2636,
    2636,  2645,  2645,  2645,  2651,  2651,  2651,  2660,  2660,  2675,
    2675,  2695,  2696,  2697,  2698,  2702,  2709,  2716,  2722,  2731,
    2736,  2743,  2744,  2748,  2754,  2761,  2769,  2776,  2784,  2796,
    2799,  2813,  2822,  2823,  2827,  2832,  2839,  2844,  2854,  2859,
    2866,  2878,  2879,  2883,  2901
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

#define YYPACT_NINF -605

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-605)))

#define YYTABLE_NINF -519

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -605,    43,  -605,  -605,    19,   -56,   -78,   -43,  -605,   -81,
    -605,  -605,   166,  -605,  -605,  -605,  -605,  -605,   255,  -605,
      91,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
     -76,  -605,    54,     2,    61,  -605,  -605,   -78,     7,  -605,
     121,   139,  -605,  -605,  -605,    91,   147,   175,  -605,  -605,
     194,   184,   152,  -605,   168,  -605,  -605,  -605,    26,    93,
     149,  -605,   357,    16,    19,   205,   -56,   171,   226,  -605,
     274,   298,  -605,  7367,   215,   -95,   358,   248,   273,  -605,
     314,    19,   166,  -605,  -605,  -605,     6,   276,  -605,  -605,
    -605,   317,  -605,  -605,   318,  -605,  -605,   280,  -605,  -605,
    -605,  -605,  -605,   291,    90,  -605,  -605,  -605,  -605,   398,
    -605,  -605,   305,   307,   308,   320,  -605,  -605,  -605,   290,
    -605,  -605,  -605,  -605,  -605,   321,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,   322,  -605,  -605,  -605,   323,
     324,  -605,  -605,  -605,  -605,   327,   328,  -605,  -605,  -605,
    -605,  -605,    12,  -605,  -605,   283,  -605,  -605,  -605,   332,
     344,  -605,   -17,  -605,  7069,   461,   462,  -605,   329,  -605,
    -605,   132,   345,  -605,  -605,   377,  -605,  -605,   253,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,    97,  -605,  -605,  -605,
    -605,  -605,   369,  -605,   218,   233,   234,  -605,  -605,  -605,
    -605,  -605,  -605,   483,  -605,  -605,    10,  2969,  -605,  -605,
    -605,   333,   364,  -605,   339,  -605,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,   346,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,   347,  -605,  -605,   -82,   373,  1281,   316,  -605,   -64,
    -605,   348,   -91,    19,   390,  -605,  -605,  -605,    90,  -605,
    7367,  7367,  7367,  7367,   356,   332,  7367,   329,  7367,   329,
    7367,   329,  7453,   344,  -605,  -605,  -605,   362,  -605,  -605,
    -605,  7281,   379,   381,  -605,   380,   396,   401,   383,   406,
    -605,   408,  4606,  4606,  7053,  7195,  4606,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  4606,  4606,  4606,   -64,  4606,  4606,
    -605,  4606,  -605,   388,  -605,  -605,  -605,     9,  -605,  -605,
    -605,  -605,   395,  -605,  -605,  -605,  -605,  -605,  -605,  5839,
     394,  -605,  -605,  -605,  -605,  -605,   -72,  7367,     8,  -605,
    -605,  -605,    -7,  -605,   -36,  7367,  -605,  -605,  4606,  -605,
    -605,   390,  2097,  -605,   276,  4606,  4606,  -605,   391,   436,
    3108,   276,  2256,   263,   263,  -605,   540,   400,   403,  4606,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,   405,   410,   411,
    -605,  -605,  -605,   390,  -605,   -87,  -605,  6333,  -605,  -605,
    -605,  -605,  -605,  -605,   393,    55,  -605,  -605,  -605,   390,
     173,  -605,  -605,   232,  -605,   181,   210,   838,   910,  -605,
    -103,  1566,   373,  1877,   373,  2038,   373,   153,  -605,   180,
      12,   209,  -605,    69,  -605,   -64,  -605,  4606,  -605,  -605,
    4606,  -605,  7367,   421,   421,   390,  1626,   390,  1783,  6401,
    -605,    21,   287,   421,   421,   -54,  -605,   421,   421,   775,
       5,  -605,  2395,   442,    41,   434,  4606,  4606,  -605,  -605,
    4606,  4606,  4606,  4606,   446,  4606,   451,  4606,  4606,  4606,
    4606,  4606,  4606,  4606,  4606,  3247,  4606,  4606,  4606,  4606,
    4606,  4606,  4606,  4606,  4606,  4606,   -70,  4606,  -605,  3386,
    -605,  -605,    12,  -605,  -605,   390,  -605,   -11,  -605,   377,
      12,   387,    22,  2552,  -605,  -605,   481,  4696,   568,  6627,
     387,  -605,   445,   463,  6627,  -605,  2552,  -605,  4795,   422,
    -605,  -605,   419,  -605,  -605,   387,  -605,  -605,  -605,  -605,
     263,  -605,  4606,  4606,  4606,  4606,  4606,  4606,  4606,  4606,
    4606,  4606,  2691,  4606,  4606,  4606,  4606,  4606,  4606,  2830,
    -605,    68,  -605,    20,  -605,   427,   464,  7367,  -605,   -30,
    -605,  7367,  -605,  -605,  -605,   466,  -605,   447,  -605,   452,
    -605,   453,  7367,  -605,  7453,  -605,   344,  3543,  3700,  7367,
    5365,  7367,  7367,  5443,  7367,  2196,    29,   263,   -37,  1940,
    3839,  6401,   454,   -15,   428,   465,  -605,  -605,  -112,    32,
    3996,   -15,   265,  4606,  4606,   430,  -605,  4606,   306,   469,
    -605,   267,  -605,   476,  -605,   134,  6627,    80,   329,   470,
    -605,  -605,   309,   309,    15,    15,  6986,  6986,   444,   244,
    -605,  5917,   -61,   -61,   309,   309,  6802,  6695,  6774,   479,
    6480,   534,  6880,  6908,    15,    15,   607,   607,   244,   244,
     244,   484,  4606,  -605,   486,  4606,   631,  5993,  -605,   138,
    -605,  -605,  -605,  7367,  -605,  2552,  -605,  -605,  4606,  -605,
    4894,  4606,  4606,  -605,   276,  -605,  -605,  -605,  -605,  4993,
    -605,  -605,  -605,   407,  -605,  6627,  6627,  6627,  6627,  6627,
    6627,  6627,  6627,  6627,  6627,  4606,  4606,  6627,  6627,  6627,
    6627,  6627,  6627,  6627,  4606,  4606,  6627,    71,  4606,  -605,
     166,  -605,   626,   592,   471,  -605,  -605,   647,  -605,  -605,
    -605,  2552,  -605,  2332,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,    12,  -605,  -605,  -605,  -605,   144,   490,
    5523,  2489,  -605,  2628,  2767,  -605,  2906,   442,  4606,  -605,
    4606,  4606,   -16,   491,   476,   475,   477,  4606,   482,  4606,
    4606,   476,   472,   485,  6627,  -605,  -605,  6548,  -605,   501,
    7367,   329,   118,   174,  4606,  -605,   516,   517,   519,   520,
    -605,   195,   373,  -605,  4153,  -605,  -605,  4606,  -605,  6069,
    -605,  6145,  -605,  -605,  -605,   528,  -605,  6233,   -96,  -605,
    -605,  -605,  1466,   276,  -605,  -605,  4606,  -605,  1087,  6627,
    1087,  6627,  -605,   987,   202,   413,  -605,   390,  -605,   483,
    -605,  2552,  -605,  5092,  -605,  -605,  -605,  -605,  -605,   216,
    -605,  -605,  -605,  -605,  -605,   487,    96,  6627,  6627,   -15,
     489,   131,   454,   492,  -605,  6627,  -605,  -605,   100,   -15,
     496,  -605,  -605,  -605,  3045,   373,   488,  6627,  -605,  -605,
    -605,  -605,    80,   497,   -75,  7367,  -605,   154,  6627,  -605,
    -605,  -605,   483,  4606,   276,  -605,  -605,   387,  -605,   494,
    -605,   637,  -605,  -605,  5191,  -605,  -605,  4606,   533,  -605,
    4606,  4606,  4606,  4310,  4606,   502,   503,  4606,  4606,  -605,
    4606,   521,  -605,  -605,   541,  -605,  -605,  -605,  4467,  -605,
    -605,  3184,  -605,  6627,  -605,   407,  -605,    26,  -605,    26,
    -605,  5601,   546,    15,    15,    15,  -605,  5681,  5290,   530,
    -605,  6627,  6627,  5290,   531,   -64,  -605,  4606,  6627,  -605,
    -605,   532,  -605,  -605,  4606,  -605,   667,   539,  -605,   536,
    -605,  -605,   -64,  6627,  -605,  -605,  5759,  4606,   548,   549,
    -605,  -605,  6627,  -605,  -605
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   201,     0,     0,     0,     0,   202,     0,
     529,   527,     0,    14,     3,    10,     9,     8,     0,     7,
     398,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   413,    19,    18,   398,     0,     0,    86,    87,
       0,   181,    88,    90,     0,    85,   423,   422,   149,   414,
     424,   399,   400,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   183,
       0,     0,     0,    93,   151,   150,     0,     0,   148,   416,
     415,     0,   426,   425,     0,   402,   401,   404,    79,    80,
      77,    78,    76,     0,     0,    75,    84,    48,    46,    42,
      39,    38,     0,     0,     0,     0,   431,   451,   432,   463,
     433,   437,   438,   439,   440,   455,   444,   445,   446,   447,
     448,   449,   450,   452,   453,   500,   436,   443,   454,   507,
     514,   434,   441,   435,   442,     0,     0,   462,   470,   473,
     471,   472,   409,   410,   412,     0,    16,    17,    20,     0,
       0,   182,     0,    91,     0,     0,     0,    99,    94,   175,
     153,     0,   419,   427,   396,   354,    21,    22,     0,    68,
      69,    66,    67,    65,    64,    70,     0,    41,   491,   494,
     497,   487,     0,   467,   501,   508,   515,   521,   524,   478,
     483,   477,   490,     0,   486,   480,     0,     0,   482,   411,
     465,     0,     0,   376,     0,    89,   147,   102,   103,   105,
     104,   106,   107,   108,   109,   134,   135,   132,   133,   125,
     136,   137,   126,   123,   124,   145,     0,   146,   138,   139,
     140,   141,   113,   114,   115,   110,   111,   112,   128,   129,
     127,   121,   122,   117,   116,   118,   119,   120,   101,   100,
     144,     0,   130,   131,   354,    97,     0,     0,   406,     0,
     421,     0,   354,     0,     0,    23,    24,    25,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    94,     0,    94,
       0,    94,     0,     0,   485,   479,   481,     0,   484,   287,
     288,     0,     0,     0,   281,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,     0,   237,   239,   238,
     240,   241,   242,    26,     0,     0,     0,     0,     0,     0,
     475,     0,   224,   225,   285,   284,   235,   282,   347,   346,
     345,   344,    92,   350,   283,   349,   348,   326,   289,     0,
       0,   286,   531,   532,   533,   534,     0,     0,     0,   143,
     142,    95,     0,   369,     0,     0,   152,   177,     0,    57,
      58,     0,     0,   189,     0,     0,     0,   190,     0,     0,
       0,     0,     0,     0,     0,   156,   154,     0,     0,     0,
     170,   165,   163,   162,   164,   176,   157,     0,     0,     0,
     168,   169,   171,     0,   161,     0,   158,   243,   456,   459,
     457,   460,   458,   461,     0,     0,   420,   361,   403,     0,
       0,   429,   405,   389,    71,     0,     0,     0,     0,   464,
       0,     0,    97,     0,    97,     0,    97,   181,   373,     0,
     371,     0,   476,   185,   188,     0,   213,     0,   204,   207,
       0,   210,     0,   315,   316,     0,   541,     0,     0,     0,
     558,     0,     0,   293,   292,   331,    32,   291,   290,     0,
     226,   353,     0,   233,     0,     0,     0,     0,   317,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   278,     0,   474,     0,
     466,   530,   375,   528,   377,     0,   367,   380,    96,   354,
      98,     0,     0,     0,   191,   194,     0,   243,     0,   184,
       0,   174,     0,     0,    51,    61,     0,   198,   243,   225,
     179,   180,     0,   172,   173,     0,   160,   166,   167,   203,
       0,   159,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,     0,   417,   354,   397,     0,     0,     0,   390,     0,
     492,     0,   498,   488,   468,     0,   502,     0,   509,     0,
     516,     0,     0,   522,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,   549,   539,   541,     0,   552,   553,   554,     0,     0,
       0,   541,     0,     0,     0,     0,    29,     0,    27,     0,
     319,   270,   269,     0,   327,     0,   219,     0,    94,     0,
     341,   342,   294,   295,   307,   308,   305,   306,     0,   336,
     324,     0,   351,   352,   296,   297,   312,   313,   314,     0,
       0,   310,   311,   309,   303,   304,   299,   298,   300,   301,
     302,     0,     0,   276,     0,     0,     0,     0,   329,     0,
     368,   379,   378,     0,   381,     0,   370,    62,     0,   195,
     243,     0,     0,   192,     0,    63,    49,    50,   199,   243,
     196,   226,   175,    54,   178,   253,   254,   256,   255,   257,
     247,   248,   249,   258,   259,     0,     0,   245,   246,   260,
     261,   250,   251,   252,     0,     0,   244,     0,     0,   407,
       0,   428,     0,   356,     0,   355,   430,     0,   387,   388,
     386,     0,   493,     0,   499,   489,   469,   503,   505,   510,
     512,   517,   519,   372,   523,   374,   526,   186,     0,     0,
       0,     0,   332,     0,     0,   333,     0,   233,     0,   542,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   551,   559,   560,     0,    30,    33,
       0,    94,     0,     0,     0,   328,     0,     0,     0,     0,
     231,     0,    97,   338,     0,   322,   343,     0,   325,     0,
     277,     0,   279,   320,   330,   382,   385,   384,     0,   193,
     200,   197,     0,     0,    52,    53,     0,    59,   264,   265,
     262,   263,   418,     0,     0,   356,   357,     0,   362,   386,
     391,     0,   395,   243,   495,   506,   513,   520,   187,     0,
     216,   214,   205,   208,   211,     0,     0,   536,   535,   541,
       0,     0,   540,     0,   544,   550,   556,   555,     0,   541,
       0,   557,    28,    31,     0,    97,     0,   220,   229,   230,
     228,   227,     0,     0,     0,     0,   267,     0,   337,   323,
     321,   280,   378,     0,     0,   155,    55,     0,   408,    93,
     363,   358,   360,   393,   243,   394,   496,     0,     0,   215,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   543,
       0,     0,   547,   271,     0,   266,   232,   234,     0,   221,
     236,     0,   268,   383,    60,    54,   366,   149,   359,   149,
     392,     0,     0,   206,   209,   212,   334,     0,   561,     0,
     545,   538,   537,   561,     0,     0,   274,     0,   222,   339,
      56,     0,   365,   217,     0,   335,     0,     0,   546,     0,
     548,   272,     0,   223,   340,   364,     0,     0,     0,     0,
     275,   218,   562,   563,   564
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -605,  -605,  -605,  -605,  -605,   268,   668,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,   696,  -605,   625,  -605,  -605,   671,
    -605,  -605,  -605,  -197,  -605,  -605,  -605,  -605,  -605,  -605,
     467,  -605,  -605,   675,   -55,  -605,   659,    13,  -321,  -284,
    -426,  -605,  -605,  -605,  -193,  -192,   -69,  -605,    44,  -363,
     -12,  -605,  -605,  -605,  -605,  -605,  -605,  -605,   747,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,  -487,  -605,  -605,  -132,  -605,   -14,  -360,  -605,  -265,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,   360,  -605,  -605,  -164,   -84,  -605,  -605,  -605,  -605,
     236,  -605,   158,  -605,  -338,   473,  -505,  -499,    23,  -605,
     -35,  -605,   711,  -605,  -605,  -605,   187,   186,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -358,   -71,
    -605,  -605,  -605,   474,  -605,  -605,  -605,    18,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,  -605,
    -605,  -605,  -605,  -605,  -605,  -605,   457,  -604,  -444,  -596,
    -605,  -605,  -301,   -10,   310,  -605,  -605,  -605,  -190,  -605
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   158,    45,    14,   178,   184,   462,   334,   789,
     335,   629,    15,    16,    38,    39,    72,    17,    34,    35,
     387,   388,   826,   827,   389,   390,   391,   392,   393,   394,
     185,   186,    30,    31,    32,    52,    53,    54,    18,   265,
     366,   168,    19,    86,    87,    88,   336,   395,   266,   396,
     337,   397,   338,   398,   399,   400,   401,   402,   403,   404,
     339,   601,   910,   602,   911,   604,   912,   340,   599,   909,
     341,   635,   930,   342,   800,   801,   638,   343,   344,   526,
     406,   345,   346,   347,   790,   955,   791,   972,   348,   676,
     891,   636,   885,   974,   362,   837,   939,   734,   573,   937,
     363,   364,   438,   439,   213,   214,   771,   516,   741,   579,
     422,   272,    62,    97,    21,   175,   415,    42,    75,    22,
      91,    23,   270,   173,    60,    94,    24,   271,   423,   350,
     414,   149,   150,   211,   151,   285,   746,   440,   283,   745,
     280,   742,   281,   906,   282,   744,   286,   747,   287,   845,
     288,   749,   289,   846,   290,   751,   291,   847,   292,   754,
     293,   756,    25,    47,    26,    46,   351,   612,   613,   614,
     352,   615,   616,   617,   618,   353,   461,   354,   967,   355
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   405,   148,   432,   517,   434,   587,   436,   589,   525,
     591,   274,   685,   522,   621,   460,   680,   775,   170,   537,
     514,   473,   679,   540,   541,   782,   162,   -92,    68,   793,
      98,    99,   550,   607,   607,   474,   475,    27,    28,   515,
     164,   688,   295,     2,   199,   273,   671,   672,   768,     3,
      40,   780,   484,   928,   273,   486,   487,   794,   584,   639,
      69,   147,   486,   487,    43,    50,   778,    36,   200,   201,
      55,   779,     4,   510,     5,   673,     6,   681,    63,    84,
     585,    51,     7,    33,   738,    85,   770,   894,    37,     8,
     154,   152,    80,   739,   418,     9,   551,   606,   361,   619,
      44,   199,    40,   681,   179,   180,   674,   675,   419,   169,
     758,   165,   166,   511,    10,   506,   507,   682,   476,   477,
     478,   479,   506,   507,   740,   200,   201,   202,   484,   792,
     485,   486,   487,   488,    50,    11,    64,   489,   421,   490,
     491,    41,   576,   682,   518,   683,    89,   519,   684,    61,
      51,   167,    90,   212,   471,   100,   273,   517,    65,   101,
     689,   102,   103,   215,    29,   631,   203,   204,   774,   296,
     862,   205,    70,   698,   206,   576,   860,   501,   502,   503,
     504,   505,   576,    71,   202,   576,   640,   297,   472,   207,
     704,   506,   507,   513,   796,   104,   298,   730,   208,    48,
     571,   818,    92,   797,   624,   731,   625,    64,    93,   148,
     148,   148,   148,   199,    66,   148,   571,   148,   420,   148,
      12,   148,   728,   203,   204,    49,    13,    74,   205,   181,
     148,   206,   681,   182,   798,   183,   103,   200,   201,   799,
     572,   770,   199,   148,   148,   681,   207,   769,   597,   794,
     278,   729,   732,   794,   917,   208,   832,   416,   514,    56,
      57,   155,    58,   915,   474,   475,   200,   201,   147,   147,
     147,   147,   682,   921,   147,    73,   147,   279,   147,   914,
     147,   856,    80,   920,    40,   682,   148,   794,   267,   147,
      59,   794,    77,   868,   148,    50,   202,   794,   425,   426,
     427,   428,   147,   147,   431,   528,   433,   794,   435,   592,
     893,    51,   535,    80,   795,   466,   268,   887,   814,   443,
      78,    82,   816,   785,   848,   202,    64,   773,   859,   474,
     475,    81,   456,   458,   932,   203,   204,   869,   902,    79,
     205,   593,   580,   206,   107,   147,    83,    71,   882,   478,
     479,   575,   876,   147,   802,    82,   918,   484,   207,   485,
     486,   487,   488,   594,   203,   204,   489,   208,   549,   205,
     595,   109,   206,   883,  -504,   512,   884,   907,   842,  -504,
     899,   148,   408,   520,   574,   576,   409,   207,   577,  -511,
    -518,   578,   596,   581,  -511,  -518,   208,  -504,   153,   908,
     275,   276,   277,   410,   411,   412,   413,   474,   475,   733,
      95,   156,  -511,  -518,   478,   479,    96,   157,   823,   110,
     506,   507,   484,  -273,   485,   486,   487,   488,  -273,   824,
     825,   489,   159,   598,   176,   177,   626,   627,   176,   177,
     147,   474,   475,   111,   332,   539,  -273,   169,   779,   924,
     783,    69,   687,   275,   276,   836,   900,   160,   632,   161,
     169,   695,   171,   172,   174,   188,   209,   189,   190,   192,
     605,   501,   502,   503,   504,   505,   703,   210,   903,   517,
     191,   193,   194,   195,   196,   506,   507,   197,   198,   212,
     476,   477,   478,   479,   480,   262,   263,   481,   482,   483,
     484,   269,   485,   486,   487,   488,   148,   875,   264,   489,
     148,   490,   491,   273,   284,   294,   356,   492,   493,   494,
     357,   148,   358,   148,   359,   360,   478,   479,   148,   365,
     148,   148,   417,   148,   484,   421,   429,   486,   487,   488,
     442,   446,   495,   489,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   474,   475,   448,   405,   445,   447,
     199,   449,   450,   506,   507,   147,   451,   349,   452,   147,
     470,   169,    12,   509,   531,   532,   542,   570,   637,   641,
     147,   694,   147,   543,   200,   201,   544,   147,   546,   147,
     147,   648,   147,   547,   548,   737,   650,   506,   507,   743,
     550,   696,   697,   702,   701,   735,   776,   773,   748,   736,
     753,   510,   148,   750,   752,   786,   788,   761,   777,   763,
     764,   792,   766,   804,   806,   820,   407,   474,   475,   808,
     803,   810,   812,   835,   836,   849,   861,   476,   477,   478,
     479,   480,   681,   202,   481,   482,   483,   484,   873,   485,
     486,   487,   488,   863,   838,   864,   489,   870,   490,   491,
     866,   878,   879,   871,   880,   881,   913,   916,   925,   938,
     919,   147,   453,   454,   922,   927,   459,   936,   942,   199,
     949,   950,   892,   204,   463,   464,   465,   205,   467,   468,
     206,   469,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   815,   956,   200,   201,   207,   954,   964,   968,   970,
     506,   507,   478,   479,   208,   975,   977,   978,    55,   148,
     484,   979,   485,   486,   487,   488,   983,   984,   521,   489,
     628,   105,   527,    67,   187,   529,   530,   108,   960,   106,
     534,   163,   538,   834,   961,   424,   822,   962,    20,   545,
     926,   901,   755,   855,   896,   686,    76,   727,   444,   430,
     841,   738,   202,   969,     0,     0,   441,     0,   622,   867,
     739,   503,   504,   505,     0,     0,     0,     0,   147,     0,
       0,     0,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,   839,   204,     0,     0,     0,   205,   600,   874,   206,
     603,     0,     0,     0,   148,   929,   611,     0,   611,     0,
       0,     0,     0,     0,   207,   934,     0,     0,   935,     0,
     840,     0,     0,   208,     0,     0,   642,   643,     0,     0,
     644,   645,   646,   647,     0,   649,     0,   651,   652,   653,
     654,   655,   656,   657,   658,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,     0,   677,     0,     0,
     199,     0,     0,   147,     0,     0,     0,     0,   476,   477,
     478,   479,   480,   690,     0,   481,   482,   483,   484,     0,
     485,   486,   487,   488,   200,   201,   699,   489,     0,   490,
     491,     0,     0,   931,     0,   492,   493,   494,     0,     0,
       0,     0,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   714,   717,   718,   719,   720,   721,   722,   723,   726,
     495,     0,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   199,   971,     0,     0,     0,     0,     0,     0,
       0,   506,   507,   202,     0,   630,     0,     0,   760,     0,
     980,     0,     0,     0,     0,     0,   200,   201,     0,   463,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,   784,   459,     0,     0,   787,     0,     0,
       0,     0,   203,   204,     0,     0,     0,   205,     0,   582,
     206,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,   207,     0,     0,     0,     0,
       0,     0,     0,     0,   208,   202,     0,     0,     0,     0,
       0,     0,   809,     0,     0,   811,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,     0,     0,     0,     0,
       0,   717,   726,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   203,   204,     0,     0,     0,   205,
       0,   583,   206,     0,     0,   828,   829,     0,     0,     0,
       0,     0,     0,     0,   830,   831,     0,   207,   833,     0,
     476,   477,   478,   479,   480,     0,   208,   481,   482,   483,
     484,   843,   485,   486,   487,   488,     0,   474,   475,   489,
       0,   490,   491,     0,     0,     0,     0,   492,   493,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     857,   858,     0,     0,     0,     0,     0,   865,     0,   611,
       0,     0,   495,     0,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   877,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,     0,     0,   888,     0,     0,
     898,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,   897,     0,     0,     0,
     476,   477,     0,     0,   480,     0,     0,   481,   482,   483,
     484,   904,   485,   486,   487,   488,     0,     0,     0,   489,
       0,   490,   491,     0,     0,     0,     0,   492,   493,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,   496,   497,   498,   499,   500,     0,
       0,     0,   504,   933,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,     0,     0,   941,     0,     0,
     943,   944,   945,   947,   948,     0,     0,   951,   952,     0,
     953,     0,   367,     0,     0,     0,     0,     3,   958,   368,
     369,   370,     0,   371,     0,   299,   300,   301,   302,   303,
       0,     0,     0,     0,     0,     0,   372,   304,   373,   374,
       0,     0,     0,     0,     0,     0,     0,   973,   375,   305,
       0,   376,   306,     0,   976,   307,     0,     8,   308,   377,
       0,   378,   309,     0,     0,   379,   380,   982,     0,     0,
       0,     0,   381,   116,   117,   118,     0,   120,   121,   122,
     123,   124,   310,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   136,   137,   138,     0,     0,   141,   142,
     143,   144,     0,     0,   311,   382,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,   384,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,     0,     0,
     331,     0,   332,   333,   385,   169,   386,   367,     0,     0,
       0,     0,     3,     0,   368,   369,   370,     0,   371,     0,
     299,   300,   301,   302,   303,     0,     0,     0,     0,     0,
       0,   372,   304,   373,   374,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   305,     0,   376,   306,     0,     0,
     307,     0,     8,   308,   377,     0,   378,   309,     0,     0,
     379,   380,     0,     0,     0,     0,     0,   381,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   310,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,     0,     0,   311,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   384,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,   199,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   200,   201,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,     0,
     299,   300,   301,   302,   303,   331,     0,   332,   333,   385,
     169,   895,   304,     0,     0,     0,     0,     0,   199,     0,
       0,     0,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,     0,   308,     0,   607,     0,   309,     0,     0,
       0,   202,   200,   201,     0,     0,     0,     0,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   310,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,     0,     0,   311,
     203,   204,     0,     0,     0,   205,     0,   586,   206,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,   202,     0,   207,     0,     0,     0,     0,     0,     0,
       0,     0,   208,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   608,   323,     0,     0,     0,     0,     0,     0,     0,
     203,   204,     0,     0,     0,   205,     0,     0,   609,   325,
     326,     0,   327,     0,     0,   328,   329,   299,   300,   301,
     302,   303,     0,   207,     0,   610,     0,   332,   333,   304,
     169,     0,   208,     0,     0,   199,     0,     0,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
     308,     0,     0,     0,   309,     0,     0,     0,     0,   200,
     201,     0,     0,     0,     0,   116,   117,   118,     0,   120,
     121,   122,   123,   124,   310,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,   136,   137,   138,     0,     0,
     141,   142,   143,   144,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,   202,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   199,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   608,   323,
       0,     0,     0,   200,   201,     0,     0,   203,   204,     0,
       0,     0,   205,     0,     0,   609,   325,   326,     0,   327,
       0,     0,   328,   329,   299,   300,   301,   302,   303,     0,
     207,     0,   620,     0,   332,   333,   304,   169,     0,   208,
       0,     0,   295,     0,     0,     0,     0,     0,   305,     0,
       0,   306,     0,     0,   307,     0,     0,   308,     0,     0,
       0,   309,   202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,     0,   120,   121,   122,   123,
     124,   310,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   136,   137,   138,     0,     0,   141,   142,   143,
     144,   203,   204,   311,     0,     0,   205,     0,   588,   206,
       0,     0,     0,     0,     0,   312,   313,     0,     0,     0,
       0,     0,     0,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,   208,     0,     0,     0,     0,     0,    50,
     199,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,     0,   200,   201,     0,     0,     0,   296,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,   299,   300,   301,   302,   303,     0,   297,     0,   331,
       0,   332,   333,   304,   169,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,   308,     0,     0,     0,   309,     0,
       0,     0,     0,   202,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   310,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     311,     0,   203,   204,     0,     0,     0,   205,     0,   590,
     206,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,   523,     0,     0,     0,   207,     0,   383,   384,     0,
       0,     0,     0,     0,   208,     0,    50,     0,   199,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   200,   201,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,     0,     0,
     299,   300,   301,   302,   303,     0,   331,     0,   332,   333,
     524,   169,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,     0,   308,     0,     0,     0,   309,     0,     0,
       0,   202,     0,     0,     0,     0,     0,     0,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   310,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,     0,     0,   311,
     203,   204,     0,     0,     0,   205,     0,   767,   206,     0,
       0,   312,   313,     0,   199,     0,     0,     0,     0,     0,
     536,     0,     0,   207,     0,     0,   383,   384,     0,     0,
       0,     0,   208,     0,     0,    50,     0,     0,   200,   201,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,   299,
     300,   301,   302,   303,     0,     0,     0,     0,   324,   325,
     326,   304,   327,     0,     0,   328,   329,     0,     0,     0,
       0,     0,     0,   305,     0,   331,   306,   332,   333,   307,
     169,     0,   308,     0,     0,     0,   309,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,   117,   118,
       0,   120,   121,   122,   123,   124,   310,   126,   127,   128,
     129,   130,   131,   132,   133,   134,     0,   136,   137,   138,
       0,     0,   141,   142,   143,   144,   203,   204,   311,     0,
       0,   205,     0,   844,   206,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,   207,
       0,     0,     0,     0,     0,     0,     0,     0,   208,     0,
       0,   199,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,     0,     0,   200,   201,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,   299,   300,   301,   302,
     303,     0,   633,     0,   331,   634,   332,   333,   304,   169,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,   306,     0,     0,   307,     0,     0,   308,
       0,     0,     0,   309,   202,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,     0,   120,   121,
     122,   123,   124,   310,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,   136,   137,   138,     0,     0,   141,
     142,   143,   144,   203,   204,   311,     0,     0,   205,     0,
     851,   206,     0,     0,     0,     0,     0,   312,   313,     0,
     199,     0,     0,     0,     0,     0,   207,     0,     0,     0,
       0,     0,   383,   384,     0,   208,     0,     0,     0,     0,
       0,    50,     0,     0,   200,   201,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,     0,     0,     0,     0,   299,   300,   301,   302,   303,
       0,     0,     0,     0,   324,   325,   326,   304,   327,     0,
       0,   328,   329,     0,     0,     0,     0,     0,     0,   305,
       0,   331,   306,   332,   333,   307,   169,     0,   308,     0,
       0,     0,   309,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,     0,   120,   121,   122,
     123,   124,   310,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   136,   137,   138,     0,     0,   141,   142,
     143,   144,   203,   204,   311,     0,     0,   205,     0,   852,
     206,     0,     0,     0,     0,     0,   312,   313,     0,   199,
       0,     0,     0,     0,     0,   207,     0,     0,     0,     0,
       0,   715,   716,     0,   208,     0,     0,     0,     0,     0,
      50,     0,     0,   200,   201,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,     0,     0,   299,   300,   301,   302,   303,     0,
       0,     0,     0,   324,   325,   326,   304,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   305,     0,
     331,   306,   332,   333,   307,   169,     0,   308,     0,     0,
       0,   309,   202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,     0,   120,   121,   122,   123,
     124,   310,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   136,   137,   138,     0,     0,   141,   142,   143,
     144,   203,   204,   311,     0,     0,   205,     0,   853,   206,
       0,     0,     0,     0,     0,   312,   313,     0,   199,     0,
       0,     0,     0,     0,   207,     0,     0,     0,     0,     0,
     724,   725,     0,   208,     0,     0,     0,     0,     0,    50,
       0,     0,   200,   201,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,   299,   300,   301,   302,   303,     0,     0,
       0,     0,   324,   325,   326,   304,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,   305,     0,   331,
     306,   332,   333,   307,   169,     0,   308,     0,     0,     0,
     309,   202,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,     0,   120,   121,   122,   123,   124,
     310,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,   136,   137,   138,     0,     0,   141,   142,   143,   144,
     203,   204,   311,     0,     0,   205,     0,   854,   206,     0,
       0,     0,     0,     0,   312,   313,     0,   199,     0,     0,
       0,     0,     0,   207,     0,     0,     0,     0,     0,     0,
       0,     0,   208,     0,     0,     0,     0,     0,    50,     0,
       0,   200,   201,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,   299,   300,   301,   302,   303,     0,     0,     0,
       0,   324,   325,   326,   304,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   305,   330,   331,   306,
     332,   333,   307,   169,     0,   308,     0,     0,     0,   309,
     202,     0,   533,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,     0,   120,   121,   122,   123,   124,   310,
     126,   127,   128,   129,   130,   131,   132,   133,   134,     0,
     136,   137,   138,     0,     0,   141,   142,   143,   144,   203,
     204,   311,     0,     0,   205,     0,   923,   206,     0,     0,
       0,     0,     0,   312,   313,     0,   199,     0,     0,     0,
       0,     0,   207,     0,     0,     0,     0,     0,     0,     0,
       0,   208,     0,     0,     0,     0,     0,    50,     0,     0,
     200,   201,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,     0,     0,     0,     0,
       0,   299,   300,   301,   302,   303,     0,     0,   659,     0,
     324,   325,   326,   304,   327,     0,     0,   328,   329,     0,
       0,     0,     0,     0,     0,   305,     0,   331,   306,   332,
     333,   307,   169,     0,   308,     0,     0,     0,   309,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   310,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,   203,   204,
     311,     0,     0,   205,     0,   959,   206,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
     299,   300,   301,   302,   303,     0,     0,     0,     0,   324,
     325,   326,   304,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,   305,     0,   331,   306,   332,   333,
     307,   169,     0,   308,     0,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   310,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,   299,   300,   301,
     302,   303,     0,     0,     0,   331,   678,   332,   333,   304,
     169,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
     308,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,     0,   120,
     121,   122,   123,   124,   310,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,   136,   137,   138,     0,     0,
     141,   142,   143,   144,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   299,   300,   301,   302,   303,     0,
       0,     0,   331,   757,   332,   333,   304,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,     0,
       0,   306,     0,     0,   307,     0,     0,   308,     0,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,     0,   120,   121,   122,   123,
     124,   310,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   136,   137,   138,     0,     0,   141,   142,   143,
     144,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,   299,   300,   301,   302,   303,     0,     0,
     759,     0,   324,   325,   326,   304,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,   305,     0,   331,
     306,   332,   333,   307,   169,     0,   308,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,     0,   120,   121,   122,   123,   124,
     310,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,   136,   137,   138,     0,     0,   141,   142,   143,   144,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     299,   300,   301,   302,   303,     0,     0,     0,   331,   772,
     332,   333,   304,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,     0,   308,     0,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   310,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,   299,   300,   301,
     302,   303,     0,     0,     0,   331,   781,   332,   333,   304,
     169,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
     308,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,     0,   120,
     121,   122,   123,   124,   310,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,   136,   137,   138,     0,     0,
     141,   142,   143,   144,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   299,   300,   301,   302,   303,     0,
       0,     0,   331,   886,   332,   333,   304,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,     0,
       0,   306,     0,     0,   307,     0,     0,   308,     0,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,     0,   120,   121,   122,   123,
     124,   310,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   136,   137,   138,     0,     0,   141,   142,   143,
     144,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,   299,   300,   301,   302,   303,     0,     0,     0,   331,
     946,   332,   333,   304,   169,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,   308,     0,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,     0,   120,   121,   122,   123,   124,   310,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,   136,
     137,   138,     0,     0,   141,   142,   143,   144,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,   957,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
     299,   300,   301,   302,   303,     0,     0,     0,     0,   324,
     325,   326,   304,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,   305,     0,   331,   306,   332,   333,
     307,   169,     0,   308,     0,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,     0,   120,   121,   122,   123,   124,   310,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   136,   137,
     138,     0,     0,   141,   142,   143,   144,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,     0,
       0,     0,     0,     0,     0,   331,     0,   332,   333,     0,
     169,   552,   553,   554,   555,   556,   557,   558,   559,   476,
     477,   478,   479,   480,   560,   561,   481,   482,   483,   484,
     691,   485,   486,   487,   488,   474,   475,     0,   489,   563,
     490,   491,   564,   565,     0,     0,   492,   493,   494,   566,
     567,   568,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     692,   495,     0,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,   693,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     552,   553,   554,   555,   556,   557,   558,   559,   476,   477,
     478,   479,   480,   560,   561,   481,   482,   483,   484,   691,
     485,   486,   487,   488,   474,   475,     0,   489,   563,   490,
     491,   564,   565,     0,     0,   492,   493,   494,   566,   567,
     568,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   692,
     495,     0,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,     0,     0,     0,     0,     0,   700,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   552,
     553,   554,   555,   556,   557,   558,   559,   476,   477,   478,
     479,   480,   560,   561,   481,   482,   483,   484,   691,   485,
     486,   487,   488,   474,   475,     0,   489,   563,   490,   491,
     564,   565,     0,     0,   492,   493,   494,   566,   567,   568,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   692,   495,
       0,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,   819,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   552,   553,
     554,   555,   556,   557,   558,   559,   476,   477,   478,   479,
     480,   560,   561,   481,   482,   483,   484,   691,   485,   486,
     487,   488,   474,   475,     0,   489,   563,   490,   491,   564,
     565,     0,     0,   492,   493,   494,   566,   567,   568,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   692,   495,     0,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   506,
     507,     0,     0,     0,     0,     0,   821,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   552,   553,   554,
     555,   556,   557,   558,   559,   476,   477,   478,   479,   480,
     560,   561,   481,   482,   483,   484,   691,   485,   486,   487,
     488,   474,   475,     0,   489,   563,   490,   491,   564,   565,
       0,     0,   492,   493,   494,   566,   567,   568,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   692,   495,     0,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,     0,     0,   905,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   552,   553,   554,   555,
     556,   557,   558,   559,   476,   477,   478,   479,   480,   560,
     561,   481,   482,   483,   484,   691,   485,   486,   487,   488,
     474,   475,     0,   489,   563,   490,   491,   564,   565,     0,
       0,   492,   493,   494,   566,   567,   568,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   692,   495,     0,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,     0,   940,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,   476,   477,   478,   479,   480,     0,     0,
     481,   482,   483,   484,     0,   485,   486,   487,   488,     0,
       0,     0,   489,     0,   490,   491,     0,     0,     0,     0,
     492,   493,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,     0,     0,     0,
       0,     0,     0,   474,   475,     0,   506,   507,   476,   477,
     478,   479,   480,   966,     0,   481,   482,   483,   484,     0,
     485,   486,   487,   488,     0,     0,     0,   489,     0,   490,
     491,     0,     0,     0,     0,   492,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,     0,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,   474,   475,   762,   476,   477,   478,   479,
     480,     0,     0,   481,   482,   483,   484,     0,   485,   486,
     487,   488,     0,     0,     0,   489,     0,   490,   491,     0,
       0,     0,     0,   492,   493,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   506,
     507,   474,   475,   765,     0,     0,   476,   477,   478,   479,
     480,     0,     0,   481,   482,   483,   484,     0,   485,   486,
     487,   488,     0,     0,     0,   489,     0,   490,   491,     0,
       0,     0,     0,   492,   493,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   506,
     507,   474,   475,   850,   476,   477,   478,   479,   480,     0,
       0,   481,   482,   483,   484,     0,   485,   486,   487,   488,
       0,     0,     0,   489,     0,   490,   491,     0,     0,     0,
       0,   492,   493,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,   474,
     475,   963,     0,     0,   476,   477,   478,   479,   480,     0,
       0,   481,   482,   483,   484,     0,   485,   486,   487,   488,
       0,     0,     0,   489,     0,   490,   491,     0,     0,     0,
       0,   492,   493,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,   474,
     475,   965,   476,   477,   478,   479,   480,     0,     0,   481,
     482,   483,   484,     0,   485,   486,   487,   488,     0,     0,
       0,   489,     0,   490,   491,     0,     0,     0,     0,   492,
     493,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,     0,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,   507,   474,   475,   981,
       0,     0,   476,   477,   478,   479,   480,     0,     0,   481,
     482,   483,   484,     0,   485,   486,   487,   488,     0,     0,
       0,   489,     0,   490,   491,     0,     0,     0,     0,   492,
     493,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,     0,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,     0,     0,     0,     0,
       0,     0,     0,   474,   475,   506,   507,   508,     0,     0,
     476,   477,   478,   479,   480,     0,     0,   481,   482,   483,
     484,     0,   485,   486,   487,   488,     0,     0,     0,   489,
       0,   490,   491,     0,     0,     0,     0,   492,   493,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,     0,     0,     0,     0,     0,   474,
     475,     0,     0,   506,   507,   805,   476,   477,   478,   479,
     480,     0,     0,   481,   482,   483,   484,     0,   485,   486,
     487,   488,     0,     0,     0,   489,     0,   490,   491,     0,
       0,     0,     0,   492,   493,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
       0,     0,     0,     0,     0,   474,   475,     0,     0,   506,
     507,   813,   476,   477,   478,   479,   480,     0,     0,   481,
     482,   483,   484,     0,   485,   486,   487,   488,     0,     0,
       0,   489,     0,   490,   491,     0,     0,     0,     0,   492,
     493,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,     0,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,   507,   889,   476,   477,
     478,   479,   480,   474,   475,   481,   482,   483,   484,     0,
     485,   486,   487,   488,     0,     0,     0,   489,     0,   490,
     491,     0,     0,     0,     0,   492,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,     0,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,   890,     0,     0,     0,     0,   552,   553,
     554,   555,   556,   557,   558,   559,   476,   477,   478,   479,
     480,   560,   561,   481,   482,   483,   484,   691,   485,   486,
     487,   488,  -243,   474,   475,   489,   563,   490,   491,   564,
     565,     0,     0,   492,   493,   494,   566,   567,   568,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   692,   495,     0,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,     0,     0,     0,     0,     0,   552,   553,
     554,   555,   556,   557,   558,   559,   476,   477,   478,   479,
     480,   560,   561,   481,   482,   483,   484,   562,   485,   486,
     487,   488,     0,     0,     0,   489,   563,   490,   491,   564,
     565,     0,     0,   492,   493,   494,   566,   567,   568,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   569,   495,     0,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     474,   475,     0,     0,   476,   477,   478,   479,   480,   506,
     507,   481,   482,   483,   484,     0,   485,   486,   487,   488,
       0,     0,     0,   489,     0,   490,   491,     0,     0,   623,
       0,   492,   493,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   474,   475,
       0,     0,     0,     0,     0,     0,     0,   506,   507,     0,
       0,     0,     0,   476,   477,   478,   479,   480,     0,     0,
     481,   482,   483,   484,     0,   485,   486,   487,   488,     0,
       0,     0,   489,     0,   490,   491,     0,     0,     0,     0,
     492,   493,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,   807,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   474,   475,     0,
       0,   476,   477,   478,   479,   480,   506,   507,   481,   482,
     483,   484,     0,   485,   486,   487,   488,     0,     0,     0,
     489,     0,   490,   491,     0,     0,     0,     0,   492,   493,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   872,
       0,     0,     0,   495,     0,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   474,   475,     0,     0,     0,
       0,     0,     0,     0,   506,   507,     0,     0,     0,     0,
     476,   477,   478,   479,   480,     0,     0,   481,   482,   483,
     484,     0,   485,   486,   487,   488,     0,     0,     0,   489,
       0,   490,   491,     0,     0,     0,     0,   492,   493,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   474,   475,     0,     0,   476,   477,
     478,   479,   480,   506,   507,   481,   482,   483,   484,     0,
     485,   486,   487,   488,     0,     0,     0,   489,     0,   490,
     491,     0,   474,   475,     0,   492,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,     0,     0,     0,     0,   476,   477,   478,
     479,   480,     0,     0,   481,   482,   483,   484,     0,   485,
     486,   487,   488,     0,     0,     0,   489,     0,   490,   491,
     474,   475,     0,     0,   492,   476,   477,   478,   479,   480,
       0,     0,   481,   482,   483,   484,     0,   485,   486,   487,
     488,     0,     0,     0,   489,     0,   490,   491,   474,   475,
       0,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,   476,   477,   478,   479,   480,     0,     0,
     481,   482,   483,   484,     0,   485,   486,   487,   488,     0,
       0,     0,   489,     0,   490,   491,   474,   475,     0,     0,
       0,   476,   477,   478,   479,   480,     0,     0,   481,   482,
     483,   484,     0,   485,   486,   487,   488,     0,     0,     0,
     489,     0,   490,   491,     0,     0,     0,     0,     0,   498,
     499,   500,   501,   502,   503,   504,   505,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,   499,   500,
     501,   502,   503,   504,   505,     0,     0,   112,     0,     0,
       0,     0,     0,   113,   506,   507,     0,     0,     0,   476,
     477,   478,   479,   480,     0,     0,   481,     0,   114,   484,
       0,   485,   486,   487,   488,     0,   216,     0,   489,     0,
     490,   491,     0,   115,     0,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   499,   500,   501,   502,
     503,   504,   505,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,    50,     0,     0,   235,   236,     0,     0,     0,
       0,     0,   237,   238,   239,   240,   241,     0,    51,   242,
     243,   244,   245,   246,   247,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
       0,     0,     0,     0,     0,   113,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,     0,     0,   258,   259,
     114,     0,     0,     0,     0,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,   115,     0,   116,   117,   118,
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
     143,   144,   145,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,   114,     0,     0,     0,     0,   314,   315,   316,
       0,     0,     0,     0,     0,     0,    51,   115,     0,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   115,     0,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437
};

static const yytype_int16 yycheck[] =
{
      12,   266,    73,   287,   362,   289,   432,   291,   434,   372,
     436,   175,   517,   371,   458,   316,   515,   613,    87,   382,
     358,   342,   509,   383,   384,   621,    81,     7,    21,   633,
      14,    15,   119,    49,    49,    20,    21,    18,    19,    46,
      34,    19,    32,     0,    32,   136,   116,   117,    19,     6,
     145,    19,   113,   128,   136,   116,   117,   153,   161,    18,
      53,    73,   116,   117,   145,   129,   178,   145,    56,    57,
      82,   183,    29,   145,    31,   145,    33,   114,   154,    53,
     183,   145,    39,   139,   114,    59,   123,   183,   166,    46,
     185,    73,   129,   123,   185,    52,   183,   455,   180,   457,
     181,    32,   145,   114,    14,    15,   176,   177,   272,   184,
     597,   105,   106,   185,    71,   176,   177,   154,   103,   104,
     105,   106,   176,   177,   154,    56,    57,   115,   113,   145,
     115,   116,   117,   118,   129,    92,   153,   122,   145,   124,
     125,   184,   153,   154,   180,   156,    53,   183,   159,    58,
     145,   145,    59,   145,   145,   139,   136,   515,   156,   143,
     523,   145,   146,   180,   145,   160,   154,   155,   183,   159,
     774,   159,   165,   536,   162,   153,   772,   162,   163,   164,
     165,   166,   153,   176,   115,   153,   145,   177,   179,   177,
     550,   176,   177,   185,   114,   179,   186,   177,   186,    33,
     145,   688,    53,   123,   183,   185,   185,   153,    59,   280,
     281,   282,   283,    32,   153,   286,   145,   288,   273,   290,
     177,   292,   154,   154,   155,    59,   183,    41,   159,   139,
     301,   162,   114,   143,   154,   145,   146,    56,    57,   159,
     185,   123,    32,   314,   315,   114,   177,   607,   179,   153,
     153,   183,   573,   153,   123,   186,   185,   269,   596,     4,
       5,    75,     7,   859,    20,    21,    56,    57,   280,   281,
     282,   283,   154,   869,   286,   154,   288,   180,   290,   183,
     292,   768,   129,   183,   145,   154,   357,   153,   156,   301,
      35,   153,   145,   780,   365,   129,   115,   153,   280,   281,
     282,   283,   314,   315,   286,   374,   288,   153,   290,   156,
     815,   145,   381,   129,   180,   327,   184,   804,   180,   301,
     145,   153,   685,   624,   180,   115,   153,   153,   772,    20,
      21,   179,   314,   315,   180,   154,   155,   781,   837,   145,
     159,   161,   161,   162,   139,   357,   178,   176,   153,   105,
     106,   178,   178,   365,   638,   153,   861,   113,   177,   115,
     116,   117,   118,   183,   154,   155,   122,   186,   403,   159,
     161,   145,   162,   178,   156,   357,   802,   161,   741,   161,
     178,   452,    66,   365,   419,   153,    70,   177,   156,   156,
     156,   159,   183,   183,   161,   161,   186,   179,   183,   183,
     147,   148,   149,    87,    88,    89,    90,    20,    21,   573,
      53,    53,   179,   179,   105,   106,    59,    59,    11,   145,
     176,   177,   113,   156,   115,   116,   117,   118,   161,    22,
      23,   122,   184,   445,   147,   148,   149,   150,   147,   148,
     452,    20,    21,   145,   181,   182,   179,   184,   183,   875,
     185,    53,   521,   147,   148,    42,    43,   184,   470,   145,
     184,   530,   145,   145,   184,   160,   183,   160,   160,   179,
     452,   162,   163,   164,   165,   166,   545,   145,   841,   837,
     160,   160,   160,   160,   160,   176,   177,   160,   160,   145,
     103,   104,   105,   106,   107,    34,    34,   110,   111,   112,
     113,   156,   115,   116,   117,   118,   577,   791,   179,   122,
     581,   124,   125,   136,   145,    32,   183,   130,   131,   132,
     156,   592,   183,   594,   178,   178,   105,   106,   599,   156,
     601,   602,   184,   604,   113,   145,   180,   116,   117,   118,
     178,   160,   155,   122,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    20,    21,   160,   822,   179,   179,
      32,   160,   179,   176,   177,   577,   160,   207,   160,   581,
     182,   184,   177,   179,   183,   139,    36,   184,   136,   145,
     592,    13,   594,   183,    56,    57,   183,   599,   183,   601,
     602,   145,   604,   183,   183,   577,   145,   176,   177,   581,
     119,   156,   139,   184,   182,   178,   178,   153,   161,   145,
     592,   145,   683,   161,   161,   185,   147,   599,   153,   601,
     602,   145,   604,   179,   145,   694,   266,    20,    21,   145,
     160,   145,     1,     7,    42,   145,   145,   103,   104,   105,
     106,   107,   114,   115,   110,   111,   112,   113,   147,   115,
     116,   117,   118,   178,   183,   178,   122,   185,   124,   125,
     178,   145,   145,   178,   145,   145,   179,   178,   180,    32,
     178,   683,   312,   313,   178,   178,   316,   183,   145,    32,
     178,   178,   154,   155,   324,   325,   326,   159,   328,   329,
     162,   331,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   683,   161,    56,    57,   177,   185,   161,   178,   178,
     176,   177,   105,   106,   186,   183,    49,   178,   730,   790,
     113,   185,   115,   116,   117,   118,   178,   178,   368,   122,
     462,    63,   372,    37,   109,   375,   376,    66,   935,    64,
     380,    82,   382,   730,   937,   278,   702,   939,     1,   389,
     882,   835,   594,   767,   823,   519,    45,   570,   301,   285,
     737,   114,   115,   953,    -1,    -1,   293,    -1,   458,   779,
     123,   164,   165,   166,    -1,    -1,    -1,    -1,   790,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,   154,   155,    -1,    -1,    -1,   159,   447,   790,   162,
     450,    -1,    -1,    -1,   885,   884,   456,    -1,   458,    -1,
      -1,    -1,    -1,    -1,   177,   894,    -1,    -1,   897,    -1,
     183,    -1,    -1,   186,    -1,    -1,   476,   477,    -1,    -1,
     480,   481,   482,   483,    -1,   485,    -1,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,    -1,   507,    -1,    -1,
      32,    -1,    -1,   885,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,   523,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    56,    57,   536,   122,    -1,   124,
     125,    -1,    -1,   885,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    32,   955,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,   115,    -1,   180,    -1,    -1,   598,    -1,
     972,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,   609,
     610,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     620,    -1,    -1,   623,   624,    -1,    -1,   627,    -1,    -1,
      -1,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   672,    -1,    -1,   675,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   685,    -1,    -1,    -1,    -1,
      -1,   691,   692,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,   715,   716,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   724,   725,    -1,   177,   728,    -1,
     103,   104,   105,   106,   107,    -1,   186,   110,   111,   112,
     113,   741,   115,   116,   117,   118,    -1,    20,    21,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     770,   771,    -1,    -1,    -1,    -1,    -1,   777,    -1,   779,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   794,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,   807,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   822,    -1,    -1,    -1,   826,    -1,    -1,    -1,
     103,   104,    -1,    -1,   107,    -1,    -1,   110,   111,   112,
     113,   841,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,    -1,
      -1,    -1,   165,   893,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,   907,    -1,    -1,
     910,   911,   912,   913,   914,    -1,    -1,   917,   918,    -1,
     920,    -1,     1,    -1,    -1,    -1,    -1,     6,   928,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   957,    37,    38,
      -1,    40,    41,    -1,   964,    44,    -1,    46,    47,    48,
      -1,    50,    51,    -1,    -1,    54,    55,   977,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    32,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   179,    -1,   181,   182,   183,
     184,   185,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    49,    -1,    51,    -1,    -1,
      -1,   115,    56,    57,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,   177,    -1,   179,    -1,   181,   182,    26,
     184,    -1,   186,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    56,    57,    -1,    -1,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    14,    15,    16,    17,    18,    -1,
     177,    -1,   179,    -1,   181,   182,    26,   184,    -1,   186,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   154,   155,    93,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,
      32,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    14,    15,    16,    17,    18,    -1,   177,    -1,   179,
      -1,   181,   182,    26,   184,    -1,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
     162,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,   177,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,   186,    -1,   129,    -1,    32,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   179,    -1,   181,   182,
     183,   184,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,   177,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,   186,    -1,    -1,   129,    -1,    -1,    56,    57,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,   163,
     164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,    44,
     184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   154,   155,    93,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
      -1,    32,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
      -1,   166,    -1,    -1,   169,   170,    14,    15,    16,    17,
      18,    -1,   177,    -1,   179,   180,   181,   182,    26,   184,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   154,   155,    93,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   186,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   179,    41,   181,   182,    44,   184,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   154,   155,    93,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,   120,   121,    -1,   186,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     179,    41,   181,   182,    44,   184,    -1,    47,    -1,    -1,
      -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   154,   155,    93,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   179,
      41,   181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     154,   155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    56,    57,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   162,   163,   164,    26,   166,    -1,    -1,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    38,   178,   179,    41,
     181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,
     115,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   154,
     155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      56,    57,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    21,    -1,
     162,   163,   164,    26,   166,    -1,    -1,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,
     182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   154,   155,
      93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,
     163,   164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,
      44,   184,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   179,   180,   181,   182,    26,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   179,   180,   181,   182,    26,   184,    -1,    -1,
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
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
     160,    -1,   162,   163,   164,    26,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   179,
      41,   181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,
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
      -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,   170,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   179,   180,
     181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   179,   180,   181,   182,    26,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   179,   180,   181,   182,    26,   184,    -1,    -1,
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
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,
     163,   164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,
      44,   184,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,   181,   182,    -1,
     184,    95,    96,    97,    98,    99,   100,   101,   102,   103,
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
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      20,    21,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   176,   177,   103,   104,
     105,   106,   107,   183,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    20,    21,   180,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    20,    21,   180,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    20,    21,   180,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    20,
      21,   180,    -1,    -1,   103,   104,   105,   106,   107,    -1,
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
      -1,    -1,    -1,    20,    21,   176,   177,   178,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,   176,   177,   178,   103,   104,   105,   106,
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
      -1,    -1,    -1,    -1,    -1,   176,   177,   178,   103,   104,
     105,   106,   107,    20,    21,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,   178,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    20,    21,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      20,    21,    -1,    -1,   103,   104,   105,   106,   107,   176,
     177,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,   128,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    20,    21,    -1,
      -1,   103,   104,   105,   106,   107,   176,   177,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    20,    21,    -1,    -1,   103,   104,
     105,   106,   107,   176,   177,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    20,    21,    -1,   130,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      20,    21,    -1,    -1,   130,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    20,    21,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,   176,   177,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,    -1,    45,   113,
      -1,   115,   116,   117,   118,    -1,    37,    -1,   122,    -1,
     124,   125,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   129,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,    -1,   145,   130,
     131,   132,   133,   134,   135,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,   169,   170,
      45,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    62,    63,    64,
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
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    45,    -1,    -1,    -1,    -1,   136,   137,   138,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145
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
     145,   320,   145,   291,   292,   180,    37,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   116,   117,   123,   124,   125,
     126,   127,   130,   131,   132,   133,   134,   135,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   169,   170,
     176,   177,    34,    34,   179,   226,   235,   156,   184,   156,
     309,   314,   298,   136,   281,   147,   148,   149,   153,   180,
     327,   329,   331,   325,   145,   322,   333,   335,   337,   339,
     341,   343,   345,   347,    32,    32,   159,   177,   186,    14,
      15,    16,    17,    18,    26,    38,    41,    44,    47,    51,
      71,    93,   105,   106,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   146,   162,   163,   164,   166,   169,   170,
     178,   179,   181,   182,   195,   197,   233,   237,   239,   247,
     254,   257,   260,   264,   265,   268,   269,   270,   275,   278,
     316,   353,   357,   362,   364,   366,   183,   156,   183,   178,
     178,   180,   281,   287,   288,   156,   227,     1,     8,     9,
      10,    12,    25,    27,    28,    37,    40,    48,    50,    54,
      55,    61,    94,   120,   121,   183,   185,   207,   208,   211,
     212,   213,   214,   215,   216,   234,   236,   238,   240,   241,
     242,   243,   244,   245,   246,   266,   267,   278,    66,    70,
      87,    88,    89,    90,   317,   303,   237,   184,   185,   281,
     221,   145,   297,   315,   217,   324,   324,   324,   324,   180,
     320,   324,   226,   324,   226,   324,   226,   145,   289,   290,
     324,   292,   178,   324,   353,   179,   160,   179,   160,   160,
     179,   160,   160,   278,   278,    12,   324,    12,   324,   278,
     359,   363,   194,   278,   278,   278,   237,   278,   278,   278,
     182,   145,   179,   225,    20,    21,   103,   104,   105,   106,
     107,   110,   111,   112,   113,   115,   116,   117,   118,   122,
     124,   125,   130,   131,   132,   155,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   176,   177,   178,   179,
     145,   185,   324,   185,   291,    46,   294,   315,   180,   183,
     324,   278,   315,   114,   183,   236,   266,   278,   233,   278,
     278,   183,   139,    54,   278,   233,   114,   236,   278,   182,
     264,   264,    36,   183,   183,   278,   183,   183,   183,   297,
     119,   183,    95,    96,    97,    98,    99,   100,   101,   102,
     108,   109,   114,   123,   126,   127,   133,   134,   135,   154,
     184,   145,   185,   285,   297,   178,   153,   156,   159,   296,
     161,   183,   161,   161,   161,   183,   161,   227,   161,   227,
     161,   227,   156,   161,   183,   161,   183,   179,   237,   255,
     278,   248,   250,   278,   252,   324,   315,    49,   145,   162,
     179,   278,   354,   355,   356,   358,   359,   360,   361,   315,
     179,   355,   361,   128,   183,   185,   149,   150,   192,   198,
     180,   160,   237,   177,   180,   258,   278,   136,   263,    18,
     145,   145,   278,   278,   278,   278,   278,   278,   145,   278,
     145,   278,   278,   278,   278,   278,   278,   278,   278,    21,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   116,   117,   145,   176,   177,   276,   278,   180,   258,
     294,   114,   154,   156,   159,   293,   287,   233,    19,   236,
     278,   114,   154,   183,    13,   233,   156,   139,   236,   278,
     183,   182,   184,   233,   264,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   120,   121,   278,   278,   278,
     278,   278,   278,   278,   120,   121,   278,   303,   154,   183,
     177,   185,   225,   281,   284,   178,   145,   324,   114,   123,
     154,   295,   328,   324,   332,   326,   323,   334,   161,   338,
     161,   342,   161,   324,   346,   289,   348,   180,   258,   160,
     278,   324,   180,   324,   324,   180,   324,   161,    19,   264,
     123,   293,   180,   153,   183,   356,   178,   153,   178,   183,
      19,   180,   356,   185,   278,   359,   185,   278,   147,   196,
     271,   273,   145,   354,   153,   180,   114,   123,   154,   159,
     261,   262,   226,   160,   179,   178,   145,   156,   145,   278,
     145,   278,     1,   178,   180,   324,   236,   278,   258,   183,
     233,   183,   235,    11,    22,    23,   209,   210,   278,   278,
     278,   278,   185,   278,   224,     7,    42,   282,   183,   154,
     183,   295,   236,   278,   161,   336,   340,   344,   180,   145,
     180,   161,   161,   161,   161,   263,   258,   278,   278,   355,
     356,   145,   354,   178,   178,   278,   178,   360,   258,   355,
     185,   178,   151,   147,   324,   226,   178,   278,   145,   145,
     145,   145,   153,   178,   227,   279,   180,   258,   278,   178,
     178,   277,   154,   293,   183,   185,   233,   278,   183,   178,
      43,   282,   294,   236,   278,   183,   330,   161,   183,   256,
     249,   251,   253,   179,   183,   356,   178,   123,   293,   178,
     183,   356,   178,   161,   227,   180,   261,   178,   128,   233,
     259,   324,   180,   278,   233,   233,   183,   286,    32,   283,
     183,   278,   145,   278,   278,   278,   180,   278,   278,   178,
     178,   278,   278,   278,   185,   272,   161,   114,   278,   161,
     210,   231,   232,   180,   161,   180,   183,   365,   178,   365,
     178,   237,   274,   278,   280,   183,   278,    49,   178,   185,
     237,   180,   278,   178,   178
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
     228,   228,   228,   228,   228,   228,   228,   228,   229,   230,
     230,   230,   231,   232,   233,   233,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   235,   235,   235,   236,   236,
     236,   237,   237,   237,   238,   239,   239,   239,   239,   240,
     241,   242,   242,   242,   242,   242,   243,   243,   243,   243,
     244,   245,   245,   246,   248,   249,   247,   250,   251,   247,
     252,   253,   247,   255,   256,   254,   257,   257,   257,   258,
     258,   259,   259,   259,   260,   260,   260,   261,   261,   261,
     261,   262,   262,   263,   263,   264,   264,   265,   265,   265,
     265,   265,   265,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   267,   267,   267,   267,   268,   269,   269,   270,
     271,   272,   270,   273,   274,   270,   275,   275,   276,   277,
     275,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   279,   280,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   281,   281,   282,   282,   283,   283,
     284,   285,   285,   286,   285,   285,   285,   287,   287,   288,
     288,   289,   289,   290,   290,   291,   292,   292,   293,   293,
     294,   294,   294,   294,   294,   294,   295,   295,   295,   296,
     296,   297,   297,   297,   297,   297,   298,   298,   299,   299,
     300,   300,   300,   301,   302,   301,   303,   303,   303,   304,
     305,   305,   306,   306,   307,   307,   307,   308,   308,   309,
     309,   310,   311,   311,   312,   312,   312,   314,   313,   315,
     315,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   317,   317,   317,   317,
     317,   317,   318,   319,   319,   320,   320,   322,   323,   321,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   325,   326,   324,
     324,   327,   328,   324,   329,   330,   324,   331,   332,   324,
     324,   333,   334,   324,   335,   336,   324,   324,   337,   338,
     324,   339,   340,   324,   324,   341,   342,   324,   343,   344,
     324,   345,   346,   324,   347,   348,   324,   350,   349,   352,
     351,   353,   353,   353,   353,   354,   354,   354,   354,   355,
     355,   356,   356,   357,   357,   357,   357,   357,   357,   358,
     358,   359,   360,   360,   361,   361,   362,   362,   363,   363,
     364,   365,   365,   366,   366
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
       2,     2,     3,     3,     2,     2,     2,     2,     3,     0,
       1,     1,     4,     2,     3,     7,     1,     1,     1,     2,
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
       0,     0,     8,     0,     0,     9,     3,     4,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     5,     4,     5,     3,     4,     1,     3,     4,     3,
       4,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     0,     4,     0,     1,     0,     1,
       3,     0,     3,     0,     7,     6,     5,     2,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     2,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     4,     6,     5,     5,     4,     0,     3,     0,     1,
       0,     1,     1,     6,     0,     6,     0,     3,     5,     3,
       2,     3,     4,     2,     0,     1,     1,     7,     9,     0,
       2,     2,     1,     1,     0,     1,     1,     0,     8,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3,     0,     0,     6,
       1,     1,     1,     1,     4,     3,     4,     2,     2,     3,
       2,     3,     2,     2,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     8,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     0,     7,     0,
       7,     1,     1,     1,     1,     3,     3,     5,     5,     1,
       3,     0,     2,     6,     5,     7,     8,     6,     8,     1,
       3,     3,     1,     1,     1,     3,     5,     5,     1,     3,
       4,     0,     3,    10,    10
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
#line 3451 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3457 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3463 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3469 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3475 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3481 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3487 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3493 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3499 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3505 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3511 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3517 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3523 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3529 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3535 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3541 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3547 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3553 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3559 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3565 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3571 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3577 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3583 "ds_parser.cpp"
        break;

    case 224: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3589 "ds_parser.cpp"
        break;

    case 225: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3595 "ds_parser.cpp"
        break;

    case 226: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3601 "ds_parser.cpp"
        break;

    case 227: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3607 "ds_parser.cpp"
        break;

    case 228: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3613 "ds_parser.cpp"
        break;

    case 233: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3619 "ds_parser.cpp"
        break;

    case 234: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3625 "ds_parser.cpp"
        break;

    case 235: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3631 "ds_parser.cpp"
        break;

    case 236: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3637 "ds_parser.cpp"
        break;

    case 237: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3643 "ds_parser.cpp"
        break;

    case 238: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3649 "ds_parser.cpp"
        break;

    case 239: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3655 "ds_parser.cpp"
        break;

    case 240: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3661 "ds_parser.cpp"
        break;

    case 241: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3667 "ds_parser.cpp"
        break;

    case 242: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3673 "ds_parser.cpp"
        break;

    case 243: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3679 "ds_parser.cpp"
        break;

    case 244: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3685 "ds_parser.cpp"
        break;

    case 246: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3691 "ds_parser.cpp"
        break;

    case 247: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3697 "ds_parser.cpp"
        break;

    case 254: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3703 "ds_parser.cpp"
        break;

    case 257: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3709 "ds_parser.cpp"
        break;

    case 258: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3715 "ds_parser.cpp"
        break;

    case 259: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3721 "ds_parser.cpp"
        break;

    case 261: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3727 "ds_parser.cpp"
        break;

    case 262: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3733 "ds_parser.cpp"
        break;

    case 263: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3739 "ds_parser.cpp"
        break;

    case 264: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3745 "ds_parser.cpp"
        break;

    case 265: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3751 "ds_parser.cpp"
        break;

    case 266: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3757 "ds_parser.cpp"
        break;

    case 267: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3763 "ds_parser.cpp"
        break;

    case 268: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3769 "ds_parser.cpp"
        break;

    case 269: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3775 "ds_parser.cpp"
        break;

    case 270: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3781 "ds_parser.cpp"
        break;

    case 275: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3787 "ds_parser.cpp"
        break;

    case 278: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3793 "ds_parser.cpp"
        break;

    case 281: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3799 "ds_parser.cpp"
        break;

    case 284: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3805 "ds_parser.cpp"
        break;

    case 285: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3811 "ds_parser.cpp"
        break;

    case 287: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3817 "ds_parser.cpp"
        break;

    case 288: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3823 "ds_parser.cpp"
        break;

    case 289: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3829 "ds_parser.cpp"
        break;

    case 290: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3835 "ds_parser.cpp"
        break;

    case 291: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3841 "ds_parser.cpp"
        break;

    case 292: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3847 "ds_parser.cpp"
        break;

    case 294: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3853 "ds_parser.cpp"
        break;

    case 297: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3859 "ds_parser.cpp"
        break;

    case 298: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3865 "ds_parser.cpp"
        break;

    case 303: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3871 "ds_parser.cpp"
        break;

    case 309: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3877 "ds_parser.cpp"
        break;

    case 315: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3883 "ds_parser.cpp"
        break;

    case 318: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3889 "ds_parser.cpp"
        break;

    case 319: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3895 "ds_parser.cpp"
        break;

    case 320: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3901 "ds_parser.cpp"
        break;

    case 321: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3907 "ds_parser.cpp"
        break;

    case 324: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3913 "ds_parser.cpp"
        break;

    case 353: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3919 "ds_parser.cpp"
        break;

    case 354: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3925 "ds_parser.cpp"
        break;

    case 355: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3931 "ds_parser.cpp"
        break;

    case 356: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3937 "ds_parser.cpp"
        break;

    case 357: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3943 "ds_parser.cpp"
        break;

    case 358: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3949 "ds_parser.cpp"
        break;

    case 359: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3955 "ds_parser.cpp"
        break;

    case 360: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3961 "ds_parser.cpp"
        break;

    case 361: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3967 "ds_parser.cpp"
        break;

    case 362: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3973 "ds_parser.cpp"
        break;

    case 363: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3979 "ds_parser.cpp"
        break;

    case 364: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3985 "ds_parser.cpp"
        break;

    case 365: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3991 "ds_parser.cpp"
        break;

    case 366: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3997 "ds_parser.cpp"
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
#line 4289 "ds_parser.cpp"
    break;

  case 16:
#line 471 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4295 "ds_parser.cpp"
    break;

  case 17:
#line 472 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4301 "ds_parser.cpp"
    break;

  case 18:
#line 476 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4307 "ds_parser.cpp"
    break;

  case 19:
#line 477 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4313 "ds_parser.cpp"
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
#line 4332 "ds_parser.cpp"
    break;

  case 21:
#line 498 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4338 "ds_parser.cpp"
    break;

  case 22:
#line 499 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4344 "ds_parser.cpp"
    break;

  case 23:
#line 500 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4350 "ds_parser.cpp"
    break;

  case 24:
#line 501 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4356 "ds_parser.cpp"
    break;

  case 25:
#line 505 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4362 "ds_parser.cpp"
    break;

  case 26:
#line 509 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4372 "ds_parser.cpp"
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
#line 4386 "ds_parser.cpp"
    break;

  case 28:
#line 523 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4396 "ds_parser.cpp"
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
#line 4414 "ds_parser.cpp"
    break;

  case 30:
#line 547 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4424 "ds_parser.cpp"
    break;

  case 31:
#line 552 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4434 "ds_parser.cpp"
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
#line 4461 "ds_parser.cpp"
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
#line 4473 "ds_parser.cpp"
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
#line 4497 "ds_parser.cpp"
    break;

  case 36:
#line 616 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4505 "ds_parser.cpp"
    break;

  case 37:
#line 619 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4514 "ds_parser.cpp"
    break;

  case 38:
#line 623 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4525 "ds_parser.cpp"
    break;

  case 39:
#line 629 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4536 "ds_parser.cpp"
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
#line 4562 "ds_parser.cpp"
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
#line 4587 "ds_parser.cpp"
    break;

  case 42:
#line 682 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4593 "ds_parser.cpp"
    break;

  case 43:
#line 683 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4599 "ds_parser.cpp"
    break;

  case 47:
#line 696 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4607 "ds_parser.cpp"
    break;

  case 48:
#line 699 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4615 "ds_parser.cpp"
    break;

  case 49:
#line 705 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4623 "ds_parser.cpp"
    break;

  case 50:
#line 711 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4631 "ds_parser.cpp"
    break;

  case 51:
#line 714 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4639 "ds_parser.cpp"
    break;

  case 52:
#line 720 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4645 "ds_parser.cpp"
    break;

  case 53:
#line 721 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4651 "ds_parser.cpp"
    break;

  case 54:
#line 725 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4657 "ds_parser.cpp"
    break;

  case 55:
#line 726 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4663 "ds_parser.cpp"
    break;

  case 56:
#line 727 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4674 "ds_parser.cpp"
    break;

  case 57:
#line 736 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4680 "ds_parser.cpp"
    break;

  case 58:
#line 737 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4686 "ds_parser.cpp"
    break;

  case 59:
#line 741 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4697 "ds_parser.cpp"
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
#line 4715 "ds_parser.cpp"
    break;

  case 61:
#line 766 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4725 "ds_parser.cpp"
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
#line 4737 "ds_parser.cpp"
    break;

  case 63:
#line 784 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4748 "ds_parser.cpp"
    break;

  case 64:
#line 793 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4754 "ds_parser.cpp"
    break;

  case 65:
#line 794 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4760 "ds_parser.cpp"
    break;

  case 66:
#line 795 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4766 "ds_parser.cpp"
    break;

  case 67:
#line 796 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4772 "ds_parser.cpp"
    break;

  case 68:
#line 797 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4778 "ds_parser.cpp"
    break;

  case 69:
#line 798 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4784 "ds_parser.cpp"
    break;

  case 70:
#line 802 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4794 "ds_parser.cpp"
    break;

  case 71:
#line 807 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4804 "ds_parser.cpp"
    break;

  case 72:
#line 815 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4810 "ds_parser.cpp"
    break;

  case 73:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4816 "ds_parser.cpp"
    break;

  case 74:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4822 "ds_parser.cpp"
    break;

  case 75:
#line 821 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4828 "ds_parser.cpp"
    break;

  case 76:
#line 822 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4834 "ds_parser.cpp"
    break;

  case 77:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4840 "ds_parser.cpp"
    break;

  case 78:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4846 "ds_parser.cpp"
    break;

  case 79:
#line 825 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4852 "ds_parser.cpp"
    break;

  case 80:
#line 826 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4858 "ds_parser.cpp"
    break;

  case 81:
#line 827 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4864 "ds_parser.cpp"
    break;

  case 82:
#line 828 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4872 "ds_parser.cpp"
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
#line 4891 "ds_parser.cpp"
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
#line 4910 "ds_parser.cpp"
    break;

  case 85:
#line 865 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4916 "ds_parser.cpp"
    break;

  case 86:
#line 866 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4922 "ds_parser.cpp"
    break;

  case 87:
#line 867 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4928 "ds_parser.cpp"
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
#line 4941 "ds_parser.cpp"
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
#line 4956 "ds_parser.cpp"
    break;

  case 90:
#line 892 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4965 "ds_parser.cpp"
    break;

  case 91:
#line 896 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4974 "ds_parser.cpp"
    break;

  case 92:
#line 903 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4980 "ds_parser.cpp"
    break;

  case 93:
#line 904 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4986 "ds_parser.cpp"
    break;

  case 94:
#line 908 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4992 "ds_parser.cpp"
    break;

  case 95:
#line 909 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4998 "ds_parser.cpp"
    break;

  case 96:
#line 910 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5004 "ds_parser.cpp"
    break;

  case 97:
#line 914 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5012 "ds_parser.cpp"
    break;

  case 98:
#line 917 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5021 "ds_parser.cpp"
    break;

  case 99:
#line 924 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5030 "ds_parser.cpp"
    break;

  case 100:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5036 "ds_parser.cpp"
    break;

  case 101:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5042 "ds_parser.cpp"
    break;

  case 102:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5048 "ds_parser.cpp"
    break;

  case 103:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5054 "ds_parser.cpp"
    break;

  case 104:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5060 "ds_parser.cpp"
    break;

  case 105:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5066 "ds_parser.cpp"
    break;

  case 106:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5072 "ds_parser.cpp"
    break;

  case 107:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5078 "ds_parser.cpp"
    break;

  case 108:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5084 "ds_parser.cpp"
    break;

  case 109:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5090 "ds_parser.cpp"
    break;

  case 110:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5096 "ds_parser.cpp"
    break;

  case 111:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5102 "ds_parser.cpp"
    break;

  case 112:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5108 "ds_parser.cpp"
    break;

  case 113:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5114 "ds_parser.cpp"
    break;

  case 114:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5120 "ds_parser.cpp"
    break;

  case 115:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5126 "ds_parser.cpp"
    break;

  case 116:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5132 "ds_parser.cpp"
    break;

  case 117:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5138 "ds_parser.cpp"
    break;

  case 118:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5144 "ds_parser.cpp"
    break;

  case 119:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5150 "ds_parser.cpp"
    break;

  case 120:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5156 "ds_parser.cpp"
    break;

  case 121:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5162 "ds_parser.cpp"
    break;

  case 122:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5168 "ds_parser.cpp"
    break;

  case 123:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5174 "ds_parser.cpp"
    break;

  case 124:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5180 "ds_parser.cpp"
    break;

  case 125:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5186 "ds_parser.cpp"
    break;

  case 126:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5192 "ds_parser.cpp"
    break;

  case 127:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5198 "ds_parser.cpp"
    break;

  case 128:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5204 "ds_parser.cpp"
    break;

  case 129:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5210 "ds_parser.cpp"
    break;

  case 130:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5216 "ds_parser.cpp"
    break;

  case 131:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5222 "ds_parser.cpp"
    break;

  case 132:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5228 "ds_parser.cpp"
    break;

  case 133:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5234 "ds_parser.cpp"
    break;

  case 134:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5240 "ds_parser.cpp"
    break;

  case 135:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5246 "ds_parser.cpp"
    break;

  case 136:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5252 "ds_parser.cpp"
    break;

  case 137:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5258 "ds_parser.cpp"
    break;

  case 138:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5264 "ds_parser.cpp"
    break;

  case 139:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5270 "ds_parser.cpp"
    break;

  case 140:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5276 "ds_parser.cpp"
    break;

  case 141:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5282 "ds_parser.cpp"
    break;

  case 142:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5288 "ds_parser.cpp"
    break;

  case 143:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5294 "ds_parser.cpp"
    break;

  case 144:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5300 "ds_parser.cpp"
    break;

  case 145:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5306 "ds_parser.cpp"
    break;

  case 146:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5312 "ds_parser.cpp"
    break;

  case 147:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5318 "ds_parser.cpp"
    break;

  case 148:
#line 979 "ds_parser.ypp"
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
#line 5342 "ds_parser.cpp"
    break;

  case 149:
#line 1001 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5348 "ds_parser.cpp"
    break;

  case 150:
#line 1002 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5354 "ds_parser.cpp"
    break;

  case 151:
#line 1003 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5360 "ds_parser.cpp"
    break;

  case 152:
#line 1007 "ds_parser.ypp"
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
#line 5401 "ds_parser.cpp"
    break;

  case 153:
#line 1046 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5410 "ds_parser.cpp"
    break;

  case 154:
#line 1053 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5419 "ds_parser.cpp"
    break;

  case 155:
#line 1057 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5432 "ds_parser.cpp"
    break;

  case 156:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5438 "ds_parser.cpp"
    break;

  case 157:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5444 "ds_parser.cpp"
    break;

  case 158:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5450 "ds_parser.cpp"
    break;

  case 159:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5456 "ds_parser.cpp"
    break;

  case 160:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5462 "ds_parser.cpp"
    break;

  case 161:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5468 "ds_parser.cpp"
    break;

  case 162:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5474 "ds_parser.cpp"
    break;

  case 163:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5480 "ds_parser.cpp"
    break;

  case 164:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5486 "ds_parser.cpp"
    break;

  case 165:
#line 1077 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5492 "ds_parser.cpp"
    break;

  case 166:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5498 "ds_parser.cpp"
    break;

  case 167:
#line 1079 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5504 "ds_parser.cpp"
    break;

  case 168:
#line 1080 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5510 "ds_parser.cpp"
    break;

  case 169:
#line 1081 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5516 "ds_parser.cpp"
    break;

  case 170:
#line 1082 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5522 "ds_parser.cpp"
    break;

  case 171:
#line 1083 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5528 "ds_parser.cpp"
    break;

  case 172:
#line 1084 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5534 "ds_parser.cpp"
    break;

  case 173:
#line 1085 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5540 "ds_parser.cpp"
    break;

  case 174:
#line 1086 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5546 "ds_parser.cpp"
    break;

  case 175:
#line 1090 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5556 "ds_parser.cpp"
    break;

  case 176:
#line 1095 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5567 "ds_parser.cpp"
    break;

  case 177:
#line 1101 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5575 "ds_parser.cpp"
    break;

  case 178:
#line 1107 "ds_parser.ypp"
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
#line 5614 "ds_parser.cpp"
    break;

  case 179:
#line 1141 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5622 "ds_parser.cpp"
    break;

  case 180:
#line 1144 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5630 "ds_parser.cpp"
    break;

  case 181:
#line 1150 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5636 "ds_parser.cpp"
    break;

  case 182:
#line 1151 "ds_parser.ypp"
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
#line 5652 "ds_parser.cpp"
    break;

  case 183:
#line 1162 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5658 "ds_parser.cpp"
    break;

  case 184:
#line 1166 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5666 "ds_parser.cpp"
    break;

  case 185:
#line 1172 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5674 "ds_parser.cpp"
    break;

  case 186:
#line 1175 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5682 "ds_parser.cpp"
    break;

  case 187:
#line 1178 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5691 "ds_parser.cpp"
    break;

  case 188:
#line 1182 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5699 "ds_parser.cpp"
    break;

  case 189:
#line 1188 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5705 "ds_parser.cpp"
    break;

  case 190:
#line 1192 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5711 "ds_parser.cpp"
    break;

  case 191:
#line 1196 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5719 "ds_parser.cpp"
    break;

  case 192:
#line 1199 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5727 "ds_parser.cpp"
    break;

  case 193:
#line 1202 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5737 "ds_parser.cpp"
    break;

  case 194:
#line 1207 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5745 "ds_parser.cpp"
    break;

  case 195:
#line 1210 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5755 "ds_parser.cpp"
    break;

  case 196:
#line 1218 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5763 "ds_parser.cpp"
    break;

  case 197:
#line 1221 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5773 "ds_parser.cpp"
    break;

  case 198:
#line 1226 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5781 "ds_parser.cpp"
    break;

  case 199:
#line 1229 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5791 "ds_parser.cpp"
    break;

  case 200:
#line 1237 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5799 "ds_parser.cpp"
    break;

  case 201:
#line 1243 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5805 "ds_parser.cpp"
    break;

  case 202:
#line 1244 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5811 "ds_parser.cpp"
    break;

  case 203:
#line 1248 "ds_parser.ypp"
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
#line 5847 "ds_parser.cpp"
    break;

  case 204:
#line 1282 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5853 "ds_parser.cpp"
    break;

  case 205:
#line 1282 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5859 "ds_parser.cpp"
    break;

  case 206:
#line 1282 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5867 "ds_parser.cpp"
    break;

  case 207:
#line 1285 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5873 "ds_parser.cpp"
    break;

  case 208:
#line 1285 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5879 "ds_parser.cpp"
    break;

  case 209:
#line 1285 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5889 "ds_parser.cpp"
    break;

  case 210:
#line 1290 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5895 "ds_parser.cpp"
    break;

  case 211:
#line 1290 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5901 "ds_parser.cpp"
    break;

  case 212:
#line 1290 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5911 "ds_parser.cpp"
    break;

  case 213:
#line 1298 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5917 "ds_parser.cpp"
    break;

  case 214:
#line 1298 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5923 "ds_parser.cpp"
    break;

  case 215:
#line 1298 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5931 "ds_parser.cpp"
    break;

  case 216:
#line 1304 "ds_parser.ypp"
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
#line 5946 "ds_parser.cpp"
    break;

  case 217:
#line 1314 "ds_parser.ypp"
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
#line 5962 "ds_parser.cpp"
    break;

  case 218:
#line 1325 "ds_parser.ypp"
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
#line 5979 "ds_parser.cpp"
    break;

  case 219:
#line 1340 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5987 "ds_parser.cpp"
    break;

  case 220:
#line 1343 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5995 "ds_parser.cpp"
    break;

  case 221:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6001 "ds_parser.cpp"
    break;

  case 222:
#line 1350 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6013 "ds_parser.cpp"
    break;

  case 223:
#line 1357 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6026 "ds_parser.cpp"
    break;

  case 224:
#line 1368 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6032 "ds_parser.cpp"
    break;

  case 225:
#line 1369 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6038 "ds_parser.cpp"
    break;

  case 226:
#line 1370 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6044 "ds_parser.cpp"
    break;

  case 227:
#line 1374 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6050 "ds_parser.cpp"
    break;

  case 228:
#line 1375 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6056 "ds_parser.cpp"
    break;

  case 229:
#line 1376 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6062 "ds_parser.cpp"
    break;

  case 230:
#line 1377 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6068 "ds_parser.cpp"
    break;

  case 231:
#line 1381 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6078 "ds_parser.cpp"
    break;

  case 232:
#line 1386 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6088 "ds_parser.cpp"
    break;

  case 233:
#line 1394 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6094 "ds_parser.cpp"
    break;

  case 234:
#line 1395 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6100 "ds_parser.cpp"
    break;

  case 235:
#line 1399 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6110 "ds_parser.cpp"
    break;

  case 236:
#line 1405 "ds_parser.ypp"
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
#line 6175 "ds_parser.cpp"
    break;

  case 237:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6181 "ds_parser.cpp"
    break;

  case 238:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6187 "ds_parser.cpp"
    break;

  case 239:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6193 "ds_parser.cpp"
    break;

  case 240:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6199 "ds_parser.cpp"
    break;

  case 241:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6205 "ds_parser.cpp"
    break;

  case 242:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6211 "ds_parser.cpp"
    break;

  case 243:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6217 "ds_parser.cpp"
    break;

  case 244:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6223 "ds_parser.cpp"
    break;

  case 245:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6229 "ds_parser.cpp"
    break;

  case 246:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6235 "ds_parser.cpp"
    break;

  case 247:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6241 "ds_parser.cpp"
    break;

  case 248:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6247 "ds_parser.cpp"
    break;

  case 249:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6253 "ds_parser.cpp"
    break;

  case 250:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6259 "ds_parser.cpp"
    break;

  case 251:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6265 "ds_parser.cpp"
    break;

  case 252:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6271 "ds_parser.cpp"
    break;

  case 253:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6277 "ds_parser.cpp"
    break;

  case 254:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6283 "ds_parser.cpp"
    break;

  case 255:
#line 1489 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6289 "ds_parser.cpp"
    break;

  case 256:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6295 "ds_parser.cpp"
    break;

  case 257:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6301 "ds_parser.cpp"
    break;

  case 258:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6307 "ds_parser.cpp"
    break;

  case 259:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp"
    break;

  case 260:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6319 "ds_parser.cpp"
    break;

  case 261:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6325 "ds_parser.cpp"
    break;

  case 262:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6331 "ds_parser.cpp"
    break;

  case 263:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp"
    break;

  case 264:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6343 "ds_parser.cpp"
    break;

  case 265:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6349 "ds_parser.cpp"
    break;

  case 266:
#line 1506 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6361 "ds_parser.cpp"
    break;

  case 267:
#line 1517 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6371 "ds_parser.cpp"
    break;

  case 268:
#line 1522 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6383 "ds_parser.cpp"
    break;

  case 269:
#line 1532 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6392 "ds_parser.cpp"
    break;

  case 270:
#line 1536 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6398 "ds_parser.cpp"
    break;

  case 271:
#line 1536 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6404 "ds_parser.cpp"
    break;

  case 272:
#line 1536 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6416 "ds_parser.cpp"
    break;

  case 273:
#line 1543 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6422 "ds_parser.cpp"
    break;

  case 274:
#line 1543 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6428 "ds_parser.cpp"
    break;

  case 275:
#line 1543 "ds_parser.ypp"
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
#line 6444 "ds_parser.cpp"
    break;

  case 276:
#line 1557 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6453 "ds_parser.cpp"
    break;

  case 277:
#line 1561 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6462 "ds_parser.cpp"
    break;

  case 278:
#line 1565 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6468 "ds_parser.cpp"
    break;

  case 279:
#line 1565 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6474 "ds_parser.cpp"
    break;

  case 280:
#line 1565 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6483 "ds_parser.cpp"
    break;

  case 281:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6489 "ds_parser.cpp"
    break;

  case 282:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6495 "ds_parser.cpp"
    break;

  case 283:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6501 "ds_parser.cpp"
    break;

  case 284:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6507 "ds_parser.cpp"
    break;

  case 285:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6513 "ds_parser.cpp"
    break;

  case 286:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6519 "ds_parser.cpp"
    break;

  case 287:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6525 "ds_parser.cpp"
    break;

  case 288:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6531 "ds_parser.cpp"
    break;

  case 289:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6537 "ds_parser.cpp"
    break;

  case 290:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 291:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 292:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 293:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 294:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 295:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6573 "ds_parser.cpp"
    break;

  case 296:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 297:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 298:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6591 "ds_parser.cpp"
    break;

  case 299:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 300:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 301:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 302:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 303:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6621 "ds_parser.cpp"
    break;

  case 304:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6627 "ds_parser.cpp"
    break;

  case 305:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6633 "ds_parser.cpp"
    break;

  case 306:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6639 "ds_parser.cpp"
    break;

  case 307:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6645 "ds_parser.cpp"
    break;

  case 308:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6651 "ds_parser.cpp"
    break;

  case 309:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6657 "ds_parser.cpp"
    break;

  case 310:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6663 "ds_parser.cpp"
    break;

  case 311:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6669 "ds_parser.cpp"
    break;

  case 312:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6675 "ds_parser.cpp"
    break;

  case 313:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6681 "ds_parser.cpp"
    break;

  case 314:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6687 "ds_parser.cpp"
    break;

  case 315:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6693 "ds_parser.cpp"
    break;

  case 316:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6699 "ds_parser.cpp"
    break;

  case 317:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6705 "ds_parser.cpp"
    break;

  case 318:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6711 "ds_parser.cpp"
    break;

  case 319:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6717 "ds_parser.cpp"
    break;

  case 320:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6723 "ds_parser.cpp"
    break;

  case 321:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6729 "ds_parser.cpp"
    break;

  case 322:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6735 "ds_parser.cpp"
    break;

  case 323:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6741 "ds_parser.cpp"
    break;

  case 324:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6747 "ds_parser.cpp"
    break;

  case 325:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6753 "ds_parser.cpp"
    break;

  case 326:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6759 "ds_parser.cpp"
    break;

  case 327:
#line 1618 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6768 "ds_parser.cpp"
    break;

  case 328:
#line 1622 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6777 "ds_parser.cpp"
    break;

  case 329:
#line 1626 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6785 "ds_parser.cpp"
    break;

  case 330:
#line 1629 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6793 "ds_parser.cpp"
    break;

  case 331:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6799 "ds_parser.cpp"
    break;

  case 332:
#line 1633 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6805 "ds_parser.cpp"
    break;

  case 333:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6811 "ds_parser.cpp"
    break;

  case 334:
#line 1635 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6825 "ds_parser.cpp"
    break;

  case 335:
#line 1644 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6839 "ds_parser.cpp"
    break;

  case 336:
#line 1653 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6845 "ds_parser.cpp"
    break;

  case 337:
#line 1654 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6853 "ds_parser.cpp"
    break;

  case 338:
#line 1657 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6859 "ds_parser.cpp"
    break;

  case 339:
#line 1657 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6865 "ds_parser.cpp"
    break;

  case 340:
#line 1657 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6873 "ds_parser.cpp"
    break;

  case 341:
#line 1660 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6882 "ds_parser.cpp"
    break;

  case 342:
#line 1664 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6891 "ds_parser.cpp"
    break;

  case 343:
#line 1668 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6900 "ds_parser.cpp"
    break;

  case 344:
#line 1672 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6906 "ds_parser.cpp"
    break;

  case 345:
#line 1673 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6912 "ds_parser.cpp"
    break;

  case 346:
#line 1674 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6918 "ds_parser.cpp"
    break;

  case 347:
#line 1675 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6924 "ds_parser.cpp"
    break;

  case 348:
#line 1676 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6930 "ds_parser.cpp"
    break;

  case 349:
#line 1677 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6936 "ds_parser.cpp"
    break;

  case 350:
#line 1678 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6942 "ds_parser.cpp"
    break;

  case 351:
#line 1679 "ds_parser.ypp"
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
#line 6963 "ds_parser.cpp"
    break;

  case 352:
#line 1695 "ds_parser.ypp"
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
#line 6984 "ds_parser.cpp"
    break;

  case 353:
#line 1711 "ds_parser.ypp"
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
#line 7044 "ds_parser.cpp"
    break;

  case 354:
#line 1769 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7050 "ds_parser.cpp"
    break;

  case 355:
#line 1770 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7056 "ds_parser.cpp"
    break;

  case 356:
#line 1774 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7062 "ds_parser.cpp"
    break;

  case 357:
#line 1775 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7068 "ds_parser.cpp"
    break;

  case 358:
#line 1779 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7074 "ds_parser.cpp"
    break;

  case 359:
#line 1780 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7080 "ds_parser.cpp"
    break;

  case 360:
#line 1784 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7090 "ds_parser.cpp"
    break;

  case 361:
#line 1792 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7098 "ds_parser.cpp"
    break;

  case 362:
#line 1795 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7107 "ds_parser.cpp"
    break;

  case 363:
#line 1800 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7113 "ds_parser.cpp"
    break;

  case 364:
#line 1800 "ds_parser.ypp"
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
#line 7162 "ds_parser.cpp"
    break;

  case 365:
#line 1844 "ds_parser.ypp"
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
#line 7227 "ds_parser.cpp"
    break;

  case 366:
#line 1904 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7238 "ds_parser.cpp"
    break;

  case 367:
#line 1913 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7248 "ds_parser.cpp"
    break;

  case 368:
#line 1918 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7258 "ds_parser.cpp"
    break;

  case 369:
#line 1926 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7264 "ds_parser.cpp"
    break;

  case 370:
#line 1927 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7270 "ds_parser.cpp"
    break;

  case 371:
#line 1931 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7278 "ds_parser.cpp"
    break;

  case 372:
#line 1934 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7289 "ds_parser.cpp"
    break;

  case 373:
#line 1943 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7295 "ds_parser.cpp"
    break;

  case 374:
#line 1944 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7301 "ds_parser.cpp"
    break;

  case 375:
#line 1948 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7312 "ds_parser.cpp"
    break;

  case 376:
#line 1957 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7318 "ds_parser.cpp"
    break;

  case 377:
#line 1958 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7324 "ds_parser.cpp"
    break;

  case 378:
#line 1963 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7330 "ds_parser.cpp"
    break;

  case 379:
#line 1964 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7336 "ds_parser.cpp"
    break;

  case 380:
#line 1968 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7347 "ds_parser.cpp"
    break;

  case 381:
#line 1974 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7358 "ds_parser.cpp"
    break;

  case 382:
#line 1980 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7366 "ds_parser.cpp"
    break;

  case 383:
#line 1983 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7375 "ds_parser.cpp"
    break;

  case 384:
#line 1987 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7386 "ds_parser.cpp"
    break;

  case 385:
#line 1993 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7397 "ds_parser.cpp"
    break;

  case 386:
#line 2002 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7403 "ds_parser.cpp"
    break;

  case 387:
#line 2003 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7409 "ds_parser.cpp"
    break;

  case 388:
#line 2004 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7415 "ds_parser.cpp"
    break;

  case 389:
#line 2008 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7421 "ds_parser.cpp"
    break;

  case 390:
#line 2009 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7427 "ds_parser.cpp"
    break;

  case 391:
#line 2013 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7435 "ds_parser.cpp"
    break;

  case 392:
#line 2016 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7445 "ds_parser.cpp"
    break;

  case 393:
#line 2021 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7455 "ds_parser.cpp"
    break;

  case 394:
#line 2026 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7468 "ds_parser.cpp"
    break;

  case 395:
#line 2034 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7481 "ds_parser.cpp"
    break;

  case 396:
#line 2045 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7489 "ds_parser.cpp"
    break;

  case 397:
#line 2048 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7499 "ds_parser.cpp"
    break;

  case 398:
#line 2056 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7505 "ds_parser.cpp"
    break;

  case 399:
#line 2057 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7511 "ds_parser.cpp"
    break;

  case 400:
#line 2061 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7517 "ds_parser.cpp"
    break;

  case 401:
#line 2062 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7523 "ds_parser.cpp"
    break;

  case 402:
#line 2063 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7529 "ds_parser.cpp"
    break;

  case 403:
#line 2067 "ds_parser.ypp"
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
#line 7562 "ds_parser.cpp"
    break;

  case 404:
#line 2095 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7568 "ds_parser.cpp"
    break;

  case 405:
#line 2095 "ds_parser.ypp"
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
#line 7604 "ds_parser.cpp"
    break;

  case 406:
#line 2129 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7612 "ds_parser.cpp"
    break;

  case 407:
#line 2132 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7626 "ds_parser.cpp"
    break;

  case 408:
#line 2141 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7640 "ds_parser.cpp"
    break;

  case 409:
#line 2154 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7654 "ds_parser.cpp"
    break;

  case 414:
#line 2176 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7660 "ds_parser.cpp"
    break;

  case 415:
#line 2177 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7666 "ds_parser.cpp"
    break;

  case 416:
#line 2178 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7672 "ds_parser.cpp"
    break;

  case 417:
#line 2182 "ds_parser.ypp"
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
#line 7705 "ds_parser.cpp"
    break;

  case 418:
#line 2210 "ds_parser.ypp"
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
#line 7739 "ds_parser.cpp"
    break;

  case 419:
#line 2242 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7745 "ds_parser.cpp"
    break;

  case 420:
#line 2243 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7751 "ds_parser.cpp"
    break;

  case 421:
#line 2247 "ds_parser.ypp"
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
#line 7791 "ds_parser.cpp"
    break;

  case 422:
#line 2285 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7797 "ds_parser.cpp"
    break;

  case 423:
#line 2286 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7803 "ds_parser.cpp"
    break;

  case 424:
#line 2290 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7809 "ds_parser.cpp"
    break;

  case 425:
#line 2291 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7815 "ds_parser.cpp"
    break;

  case 426:
#line 2292 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7821 "ds_parser.cpp"
    break;

  case 427:
#line 2297 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7827 "ds_parser.cpp"
    break;

  case 428:
#line 2297 "ds_parser.ypp"
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
#line 7913 "ds_parser.cpp"
    break;

  case 429:
#line 2381 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7925 "ds_parser.cpp"
    break;

  case 430:
#line 2388 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7936 "ds_parser.cpp"
    break;

  case 431:
#line 2397 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7942 "ds_parser.cpp"
    break;

  case 432:
#line 2398 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7948 "ds_parser.cpp"
    break;

  case 433:
#line 2399 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7954 "ds_parser.cpp"
    break;

  case 434:
#line 2400 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7960 "ds_parser.cpp"
    break;

  case 435:
#line 2401 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7966 "ds_parser.cpp"
    break;

  case 436:
#line 2402 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7972 "ds_parser.cpp"
    break;

  case 437:
#line 2403 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7978 "ds_parser.cpp"
    break;

  case 438:
#line 2404 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7984 "ds_parser.cpp"
    break;

  case 439:
#line 2405 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7990 "ds_parser.cpp"
    break;

  case 440:
#line 2406 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7996 "ds_parser.cpp"
    break;

  case 441:
#line 2407 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8002 "ds_parser.cpp"
    break;

  case 442:
#line 2408 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8008 "ds_parser.cpp"
    break;

  case 443:
#line 2409 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8014 "ds_parser.cpp"
    break;

  case 444:
#line 2410 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8020 "ds_parser.cpp"
    break;

  case 445:
#line 2411 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8026 "ds_parser.cpp"
    break;

  case 446:
#line 2412 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8032 "ds_parser.cpp"
    break;

  case 447:
#line 2413 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8038 "ds_parser.cpp"
    break;

  case 448:
#line 2414 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8044 "ds_parser.cpp"
    break;

  case 449:
#line 2415 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8050 "ds_parser.cpp"
    break;

  case 450:
#line 2416 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8056 "ds_parser.cpp"
    break;

  case 451:
#line 2417 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8062 "ds_parser.cpp"
    break;

  case 452:
#line 2418 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8068 "ds_parser.cpp"
    break;

  case 453:
#line 2419 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8074 "ds_parser.cpp"
    break;

  case 454:
#line 2420 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8080 "ds_parser.cpp"
    break;

  case 455:
#line 2421 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8086 "ds_parser.cpp"
    break;

  case 456:
#line 2425 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8092 "ds_parser.cpp"
    break;

  case 457:
#line 2426 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8098 "ds_parser.cpp"
    break;

  case 458:
#line 2427 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8104 "ds_parser.cpp"
    break;

  case 459:
#line 2428 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8110 "ds_parser.cpp"
    break;

  case 460:
#line 2429 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8116 "ds_parser.cpp"
    break;

  case 461:
#line 2430 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8122 "ds_parser.cpp"
    break;

  case 462:
#line 2434 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8135 "ds_parser.cpp"
    break;

  case 463:
#line 2445 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8144 "ds_parser.cpp"
    break;

  case 464:
#line 2449 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8156 "ds_parser.cpp"
    break;

  case 465:
#line 2459 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8168 "ds_parser.cpp"
    break;

  case 466:
#line 2466 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8179 "ds_parser.cpp"
    break;

  case 467:
#line 2475 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8185 "ds_parser.cpp"
    break;

  case 468:
#line 2475 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8191 "ds_parser.cpp"
    break;

  case 469:
#line 2475 "ds_parser.ypp"
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
#line 8206 "ds_parser.cpp"
    break;

  case 470:
#line 2488 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8212 "ds_parser.cpp"
    break;

  case 471:
#line 2489 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8218 "ds_parser.cpp"
    break;

  case 472:
#line 2490 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8224 "ds_parser.cpp"
    break;

  case 473:
#line 2491 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8230 "ds_parser.cpp"
    break;

  case 474:
#line 2492 "ds_parser.ypp"
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
#line 8249 "ds_parser.cpp"
    break;

  case 475:
#line 2506 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8260 "ds_parser.cpp"
    break;

  case 476:
#line 2512 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8269 "ds_parser.cpp"
    break;

  case 477:
#line 2516 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8278 "ds_parser.cpp"
    break;

  case 478:
#line 2520 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8288 "ds_parser.cpp"
    break;

  case 479:
#line 2525 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8298 "ds_parser.cpp"
    break;

  case 480:
#line 2530 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8308 "ds_parser.cpp"
    break;

  case 481:
#line 2535 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8318 "ds_parser.cpp"
    break;

  case 482:
#line 2540 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8327 "ds_parser.cpp"
    break;

  case 483:
#line 2544 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8336 "ds_parser.cpp"
    break;

  case 484:
#line 2548 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8346 "ds_parser.cpp"
    break;

  case 485:
#line 2553 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8355 "ds_parser.cpp"
    break;

  case 486:
#line 2557 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8365 "ds_parser.cpp"
    break;

  case 487:
#line 2562 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8371 "ds_parser.cpp"
    break;

  case 488:
#line 2562 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8377 "ds_parser.cpp"
    break;

  case 489:
#line 2562 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8388 "ds_parser.cpp"
    break;

  case 490:
#line 2568 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8400 "ds_parser.cpp"
    break;

  case 491:
#line 2575 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8406 "ds_parser.cpp"
    break;

  case 492:
#line 2575 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8412 "ds_parser.cpp"
    break;

  case 493:
#line 2575 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8422 "ds_parser.cpp"
    break;

  case 494:
#line 2580 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8428 "ds_parser.cpp"
    break;

  case 495:
#line 2580 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8434 "ds_parser.cpp"
    break;

  case 496:
#line 2580 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8445 "ds_parser.cpp"
    break;

  case 497:
#line 2586 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8451 "ds_parser.cpp"
    break;

  case 498:
#line 2586 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8457 "ds_parser.cpp"
    break;

  case 499:
#line 2586 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8467 "ds_parser.cpp"
    break;

  case 500:
#line 2591 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8476 "ds_parser.cpp"
    break;

  case 501:
#line 2595 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8482 "ds_parser.cpp"
    break;

  case 502:
#line 2595 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8488 "ds_parser.cpp"
    break;

  case 503:
#line 2595 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8498 "ds_parser.cpp"
    break;

  case 504:
#line 2600 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8504 "ds_parser.cpp"
    break;

  case 505:
#line 2600 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8510 "ds_parser.cpp"
    break;

  case 506:
#line 2600 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8524 "ds_parser.cpp"
    break;

  case 507:
#line 2609 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8533 "ds_parser.cpp"
    break;

  case 508:
#line 2613 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8539 "ds_parser.cpp"
    break;

  case 509:
#line 2613 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8545 "ds_parser.cpp"
    break;

  case 510:
#line 2613 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8555 "ds_parser.cpp"
    break;

  case 511:
#line 2618 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8561 "ds_parser.cpp"
    break;

  case 512:
#line 2618 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8567 "ds_parser.cpp"
    break;

  case 513:
#line 2618 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8581 "ds_parser.cpp"
    break;

  case 514:
#line 2627 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8590 "ds_parser.cpp"
    break;

  case 515:
#line 2631 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8596 "ds_parser.cpp"
    break;

  case 516:
#line 2631 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8602 "ds_parser.cpp"
    break;

  case 517:
#line 2631 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8612 "ds_parser.cpp"
    break;

  case 518:
#line 2636 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8618 "ds_parser.cpp"
    break;

  case 519:
#line 2636 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8624 "ds_parser.cpp"
    break;

  case 520:
#line 2636 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8638 "ds_parser.cpp"
    break;

  case 521:
#line 2645 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8644 "ds_parser.cpp"
    break;

  case 522:
#line 2645 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8650 "ds_parser.cpp"
    break;

  case 523:
#line 2645 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8661 "ds_parser.cpp"
    break;

  case 524:
#line 2651 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8667 "ds_parser.cpp"
    break;

  case 525:
#line 2651 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8673 "ds_parser.cpp"
    break;

  case 526:
#line 2651 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8684 "ds_parser.cpp"
    break;

  case 527:
#line 2660 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8690 "ds_parser.cpp"
    break;

  case 528:
#line 2660 "ds_parser.ypp"
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
#line 8707 "ds_parser.cpp"
    break;

  case 529:
#line 2675 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8713 "ds_parser.cpp"
    break;

  case 530:
#line 2675 "ds_parser.ypp"
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
#line 8734 "ds_parser.cpp"
    break;

  case 531:
#line 2695 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8740 "ds_parser.cpp"
    break;

  case 532:
#line 2696 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8746 "ds_parser.cpp"
    break;

  case 533:
#line 2697 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8752 "ds_parser.cpp"
    break;

  case 534:
#line 2698 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8758 "ds_parser.cpp"
    break;

  case 535:
#line 2702 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8770 "ds_parser.cpp"
    break;

  case 536:
#line 2709 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8782 "ds_parser.cpp"
    break;

  case 537:
#line 2716 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8793 "ds_parser.cpp"
    break;

  case 538:
#line 2722 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8804 "ds_parser.cpp"
    break;

  case 539:
#line 2731 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8814 "ds_parser.cpp"
    break;

  case 540:
#line 2736 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8823 "ds_parser.cpp"
    break;

  case 541:
#line 2743 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8829 "ds_parser.cpp"
    break;

  case 542:
#line 2744 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8835 "ds_parser.cpp"
    break;

  case 543:
#line 2748 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8846 "ds_parser.cpp"
    break;

  case 544:
#line 2754 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8858 "ds_parser.cpp"
    break;

  case 545:
#line 2761 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8871 "ds_parser.cpp"
    break;

  case 546:
#line 2769 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8883 "ds_parser.cpp"
    break;

  case 547:
#line 2776 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8896 "ds_parser.cpp"
    break;

  case 548:
#line 2784 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8910 "ds_parser.cpp"
    break;

  case 549:
#line 2796 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8918 "ds_parser.cpp"
    break;

  case 550:
#line 2799 "ds_parser.ypp"
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
#line 8934 "ds_parser.cpp"
    break;

  case 551:
#line 2813 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8945 "ds_parser.cpp"
    break;

  case 552:
#line 2822 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8951 "ds_parser.cpp"
    break;

  case 553:
#line 2823 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8957 "ds_parser.cpp"
    break;

  case 554:
#line 2827 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8967 "ds_parser.cpp"
    break;

  case 555:
#line 2832 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8976 "ds_parser.cpp"
    break;

  case 556:
#line 2839 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8986 "ds_parser.cpp"
    break;

  case 557:
#line 2844 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8998 "ds_parser.cpp"
    break;

  case 558:
#line 2854 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9008 "ds_parser.cpp"
    break;

  case 559:
#line 2859 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9017 "ds_parser.cpp"
    break;

  case 560:
#line 2866 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9031 "ds_parser.cpp"
    break;

  case 561:
#line 2878 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9037 "ds_parser.cpp"
    break;

  case 562:
#line 2879 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9043 "ds_parser.cpp"
    break;

  case 563:
#line 2883 "ds_parser.ypp"
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
#line 9066 "ds_parser.cpp"
    break;

  case 564:
#line 2901 "ds_parser.ypp"
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
#line 9089 "ds_parser.cpp"
    break;


#line 9093 "ds_parser.cpp"

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
#line 2921 "ds_parser.ypp"


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
