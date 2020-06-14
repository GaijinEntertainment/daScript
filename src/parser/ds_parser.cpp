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
    RPIPE = 375,
    CLONEEQU = 376,
    ROTL = 377,
    ROTR = 378,
    ROTLEQU = 379,
    ROTREQU = 380,
    MAPTO = 381,
    BRABRAB = 382,
    BRACBRB = 383,
    CBRCBRB = 384,
    INTEGER = 385,
    LONG_INTEGER = 386,
    UNSIGNED_INTEGER = 387,
    UNSIGNED_LONG_INTEGER = 388,
    FLOAT = 389,
    DOUBLE = 390,
    NAME = 391,
    BEGIN_STRING = 392,
    STRING_CHARACTER = 393,
    END_STRING = 394,
    BEGIN_STRING_EXPR = 395,
    END_STRING_EXPR = 396,
    END_OF_READ = 397,
    UNARY_MINUS = 398,
    UNARY_PLUS = 399,
    PRE_INC = 400,
    PRE_DEC = 401,
    POST_INC = 402,
    POST_DEC = 403,
    DEREF = 404,
    COLCOL = 405
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

#line 398 "ds_parser.cpp"

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
#define YYLAST   7111

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  173
/* YYNRULES -- Number of rules.  */
#define YYNRULES  509
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  907

#define YYUNDEFTOK  2
#define YYMAXUTOK   405

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
       2,     2,     2,   160,     2,   177,   171,   156,   149,     2,
     169,   170,   154,   153,   143,   152,   165,   155,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   146,   174,
     150,   144,   151,   145,   172,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   167,     2,   168,   148,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   175,   147,   176,   159,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   142,   157,   158,
     161,   162,   163,   164,   166,   173
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   423,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   439,   451,   452,   456,   460,
     465,   474,   482,   498,   503,   511,   511,   541,   563,   567,
     570,   576,   585,   606,   629,   630,   634,   638,   639,   643,
     646,   652,   658,   661,   667,   668,   672,   673,   674,   683,
     684,   688,   697,   713,   721,   731,   740,   741,   742,   743,
     744,   745,   749,   754,   762,   763,   764,   765,   766,   767,
     768,   769,   775,   788,   804,   805,   809,   816,   828,   832,
     839,   840,   844,   845,   846,   850,   855,   862,   866,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     878,   879,   880,   881,   882,   883,   884,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     898,   899,   900,   901,   905,   926,   964,   971,   975,   986,
     987,   988,   989,   990,   991,   992,   993,   994,   995,   996,
     997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1008,  1013,
    1019,  1025,  1046,  1052,  1053,  1064,  1068,  1074,  1077,  1080,
    1084,  1090,  1094,  1098,  1101,  1104,  1109,  1112,  1120,  1123,
    1128,  1131,  1139,  1145,  1146,  1150,  1151,  1155,  1190,  1190,
    1190,  1193,  1193,  1193,  1198,  1198,  1198,  1206,  1210,  1215,
    1221,  1221,  1221,  1225,  1225,  1225,  1230,  1230,  1230,  1239,
    1242,  1248,  1249,  1256,  1267,  1268,  1269,  1273,  1278,  1333,
    1334,  1335,  1336,  1337,  1338,  1342,  1343,  1344,  1345,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,
    1357,  1361,  1362,  1366,  1377,  1382,  1392,  1396,  1396,  1396,
    1403,  1403,  1403,  1417,  1421,  1421,  1421,  1429,  1430,  1431,
    1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,
    1442,  1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,
    1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,  1473,  1477,
    1480,  1483,  1484,  1485,  1486,  1491,  1496,  1497,  1500,  1500,
    1500,  1503,  1507,  1511,  1515,  1516,  1517,  1518,  1519,  1520,
    1521,  1531,  1541,  1596,  1597,  1601,  1602,  1606,  1614,  1617,
    1622,  1621,  1666,  1722,  1727,  1735,  1736,  1740,  1743,  1752,
    1753,  1757,  1766,  1767,  1772,  1773,  1777,  1783,  1789,  1792,
    1796,  1802,  1811,  1812,  1813,  1817,  1818,  1822,  1825,  1830,
    1835,  1843,  1854,  1857,  1864,  1865,  1869,  1896,  1896,  1908,
    1911,  1920,  1933,  1945,  1946,  1950,  1954,  1965,  1980,  1981,
    1985,  2022,  2023,  2027,  2027,  2111,  2118,  2127,  2128,  2129,
    2130,  2131,  2132,  2133,  2134,  2135,  2136,  2137,  2138,  2139,
    2140,  2141,  2142,  2143,  2144,  2145,  2146,  2147,  2148,  2149,
    2150,  2151,  2155,  2156,  2157,  2158,  2159,  2160,  2164,  2175,
    2179,  2189,  2196,  2205,  2205,  2205,  2218,  2219,  2220,  2221,
    2222,  2236,  2242,  2246,  2251,  2256,  2261,  2266,  2270,  2274,
    2279,  2283,  2288,  2288,  2288,  2294,  2301,  2301,  2301,  2306,
    2306,  2306,  2312,  2312,  2312,  2317,  2321,  2321,  2321,  2326,
    2326,  2326,  2335,  2339,  2339,  2339,  2344,  2344,  2344,  2353,
    2357,  2357,  2357,  2362,  2362,  2362,  2371,  2371,  2371,  2377,
    2377,  2377,  2386,  2386,  2401,  2401,  2421,  2422,  2423,  2424,
    2428,  2435,  2442,  2448,  2457,  2462,  2469,  2470,  2474,  2480,
    2487,  2495,  2502,  2510,  2522,  2525,  2539,  2548,  2549,  2553,
    2558,  2565,  2570,  2581,  2586,  2593,  2605,  2606,  2610,  2628
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
  "\"goto\"", "\"implicit\"", "\"shared\"", "\"smart_ptr\"", "\"unsafe\"",
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"",
  "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"",
  "\"float4\"", "\"range\"", "\"urange\"", "\"block\"", "\"int64\"",
  "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"",
  "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"",
  "\"generator\"", "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"",
  "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"", "','", "'='", "'?'",
  "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'",
  "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "'.'", "DEREF", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "COLCOL", "';'", "'{'", "'}'", "'#'", "$accept", "program",
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
  "block_or_simple_block", "block_or_lambda", "expr_block",
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
     395,   396,   397,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   398,   399,   126,
      33,   400,   401,   402,   403,    46,   404,    91,    93,    40,
      41,    36,    64,   405,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -570

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-570)))

#define YYTABLE_NINF -464

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -570,    29,  -570,  -570,   -62,   -35,     3,    10,   -39,  -570,
      43,  -570,  -570,    12,  -570,  -570,  -570,  -570,  -570,   255,
    -570,   128,  -570,  -570,  -570,  -570,  -570,  -570,    44,  -570,
      48,    47,   132,  -570,  -570,    18,  -570,   -87,   142,  -570,
     148,   152,  -570,   120,   164,   137,  -570,    87,  -570,  -570,
    -570,   121,   173,  -570,   160,    13,   -62,   216,   -35,   219,
    -570,   233,   235,  -570,   262,  -570,   210,  -570,  -110,   198,
     207,   247,  -570,   -62,    12,  -570,  6951,   351,   353,  -570,
     220,   213,  -570,   251,  -570,  -570,   397,  -570,  -570,  -570,
    -570,  -570,   265,   184,  -570,  -570,  -570,  -570,   352,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,   230,   -89,  5928,
    -570,  -570,   271,   273,  -570,   -46,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,   -84,   268,  -570,  -570,   -90,  -570,   240,
     -73,   243,  -570,   -54,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,    66,  -570,  -570,    -3,  -570,   269,   270,   272,   275,
    -570,  -570,  -570,   249,  -570,  -570,  -570,  -570,  -570,   277,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,   282,
    -570,  -570,  -570,   283,   284,  -570,  -570,  -570,  -570,   285,
     286,  -570,  -570,  -570,  -570,  -570,    61,  -570,   263,   292,
    -570,   266,  -570,   -62,  -570,    -7,  -570,  -103,  5928,  -570,
    1245,  -570,  -570,  -570,  -570,  -570,   176,  -570,  -570,  -570,
     184,  -570,   -71,  4684,  -570,  -570,  -570,  -570,  -570,   293,
    -570,   117,   187,   206,  -570,  -570,  -570,  -570,  -570,   407,
    -570,  -570,    -9,  2578,  -570,  -570,   -66,  5928,   -57,    99,
     305,  -570,   107,  -570,   315,  5994,  -570,  4684,  -570,  -570,
     305,  -570,  -570,  5852,   274,  2016,  -570,  -570,   213,  4684,
     276,  4684,   297,   298,   280,  -570,   281,   303,   324,  2710,
     213,  -570,   307,  2166,  4684,  4684,   224,   458,   942,  4684,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  4684,  4684,  4684,
     -90,  4684,  4684,  4684,  -570,   299,  -570,   422,  -570,  -570,
     290,   291,  4684,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,   -68,   294,  -570,   295,   301,  -570,  -570,  -570,   453,
    -570,  -570,  -570,   306,  -570,  -570,   -76,  -570,  -570,  -570,
    -570,  -570,  6324,   308,  -570,  -570,  -570,  -570,  -570,     5,
     338,  5928,  -570,   -32,  -570,  -570,  1043,  5928,  5928,  5928,
    5928,   309,   271,  5928,   220,  5928,   220,  5928,   220,  6004,
     273,  -570,  -570,  -570,   310,  -570,  -570,  6000,  -570,  -570,
    5994,  -570,  -570,   316,  -570,  -570,  -570,  5928,  -570,  2842,
    -570,   935,    14,    45,  -570,   -90,  2842,  -570,  -570,   358,
    4766,   470,  6598,  4684,   935,  -570,  -570,  4684,  -570,  -570,
     340,   357,  6598,  -570,  5928,  2842,  -570,  4864,   460,   460,
     317,  -570,   305,  1572,   305,  1720,  6390,  -570,   115,   222,
     460,   460,   -67,  -570,   460,   460,   636,   -56,   319,  -570,
    -570,   935,  -570,  2298,  -570,  -570,  -570,   456,   305,   220,
     224,  -570,    33,   356,  4684,  4684,  4684,  4684,  4684,  4684,
    4684,  4684,  4684,  4684,  -570,  -570,  4684,  4684,  4684,  4684,
    4684,  4684,   360,  2974,  4684,   361,  4684,  4684,  4684,  4684,
    4684,  4684,  4684,  4684,  3106,  3268,  4684,  4684,  4684,  4684,
    4684,  4684,  4684,  4684,  4684,  4684,   362,  4684,  3400,  -570,
     492,   457,   327,  -570,  1060,  -570,  -570,  -570,  2842,  -570,
     279,  1666,  1814,  1987,  -570,  -101,  2392,   268,  3103,   268,
    3214,   268,    58,  -570,     0,  5994,   139,  -570,  -570,  -570,
    3399,  -570,  6218,  -570,  4684,  3548,  2446,  -570,  4962,  4684,
    4684,  -570,   213,  5395,  -570,  5928,  5928,  5464,  5928,  -570,
    -570,  3547,  -570,  5060,  -570,  -570,    17,   224,   -69,  1868,
    3696,  6390,   365,   -20,   335,   366,  -570,  -570,    84,    19,
    3844,   -20,   150,  4684,  4684,   329,  -570,  4684,   372,   373,
    -570,   217,  -570,  -570,   218,   376,  -570,    78,  6598,  -570,
    -570,   268,  -570,   363,  -570,  -570,  6598,  6598,  6598,  6598,
    6598,  6598,  6598,  6598,   225,   225,    11,  6598,  6598,    11,
    6721,  6721,   345,  4684,  6598,   446,  -570,  6076,   -59,   -59,
    6598,   225,   225,  6598,  6598,  4684,  6598,   379,  6461,   486,
     824,  6664,    11,    11,   296,   296,   446,   446,   446,  -570,
     515,  6143,  -570,    91,   300,  -570,   305,  -570,   407,  -570,
    2842,  -570,  5158,  -570,  5928,  -570,  -570,  -570,   418,  -570,
     405,  -570,   406,  -570,   408,  5928,  -570,  6004,  -570,   273,
     407,  4684,   -65,  -570,    94,   417,   433,  5533,  -570,  -570,
    -570,  3695,  3843,  -570,  3991,   401,  -570,  4684,  -570,  4684,
    4684,   -25,   435,   376,   410,   414,  4684,   415,  4684,  4684,
     376,   398,   416,  6598,  -570,  -570,  6527,  -570,   447,  5928,
     220,  1421,   213,  -570,  -570,  4684,  -570,    74,   101,  4684,
    -570,   -85,  -570,  3992,  6781,  -570,  6781,  -570,  4684,  -570,
    -570,  -570,  -570,   121,  -570,  -570,  5256,  -570,  -570,  4139,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  5994,
    -570,  -570,  -570,  6598,   213,  -570,  -570,   141,  -570,  -570,
    -570,  -570,  4140,   -36,  6598,  6598,   -20,   419,   103,   365,
     425,  -570,  6598,  -570,  -570,   -22,   -20,   436,  -570,  -570,
    -570,  4681,   268,  -570,  -570,   935,   437,  6598,  4288,  -570,
    -570,  5928,  -570,    96,  6598,  -570,   121,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  5928,  4420,   450,  4684,  4684,  4684,
    -570,  5602,  4684,   444,   448,  4684,  4684,  -570,  4684,   429,
    -570,  -570,   463,   218,  -570,  4684,  6598,  6800,  -570,   399,
    -570,  6813,   465,  5671,   466,    11,    11,    11,  -570,  5322,
     451,  -570,  6598,  6598,  5322,   452,   -90,  -570,  -570,  6598,
    -570,  -570,  -570,  -570,  -570,  4552,   568,   454,  -570,   445,
    -570,  -570,   -90,  -570,   459,  5928,   474,  5740,  4684,   462,
     464,  -570,  -570,  6824,  -570,  -570,  6598,  -570,  -570,  -570,
    5928,   473,  6867,  -570,  -570,   475,  -570
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   175,     0,     0,     0,     0,     0,   176,
       0,   474,   472,     0,    14,     3,    10,     9,     8,     0,
       7,   354,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    75,   153,     0,    76,    78,     0,    74,   372,
     371,     0,     0,   355,   357,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   359,     0,   363,     0,     0,
       0,     0,   155,     0,     0,    81,     0,     0,     0,    87,
      82,     0,   124,   368,   373,   352,     0,    68,    69,    66,
      67,    65,     0,     0,    64,    73,    40,    38,    34,    31,
      30,   402,   405,   403,   406,   404,   407,     0,     0,     0,
     365,   364,     0,     0,   154,     0,    79,    90,    91,    93,
      92,    94,    95,    96,    97,   116,   117,   114,   115,   107,
     118,   119,   108,   105,   106,   120,   121,   122,   123,   110,
     111,   109,   103,   104,    99,    98,   100,   101,   102,    89,
      88,   112,   113,   313,    85,   148,   126,     0,   370,     0,
       0,     0,    16,     0,    60,    61,    58,    59,    57,    56,
      62,     0,    33,   359,     0,   366,     0,     0,     0,     0,
     377,   397,   378,   409,   379,   383,   384,   385,   386,   401,
     390,   391,   392,   393,   394,   395,   396,   398,   399,   445,
     382,   389,   400,   452,   459,   380,   387,   381,   388,     0,
       0,   408,   416,   419,   417,   418,     0,   411,     0,     0,
     332,     0,    77,     0,    83,     0,   325,     0,     0,   125,
       0,   369,   318,   375,   356,   353,   345,   358,    17,    18,
       0,    71,     0,     0,   360,   436,   439,   442,   432,     0,
     413,   446,   453,   460,   466,   469,   423,   428,   435,     0,
     431,   425,     0,     0,   362,   427,     0,     0,     0,     0,
       0,   323,   336,    84,   313,    86,   150,     0,    49,    50,
       0,   253,   254,     0,     0,     0,   247,   161,     0,     0,
       0,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
     209,   211,   210,   212,   213,   214,    19,     0,     0,     0,
       0,     0,     0,     0,   204,   205,   129,   127,   251,   250,
       0,     0,     0,   143,   138,   136,   135,   137,   207,   149,
     130,   248,     0,   306,     0,     0,   141,   142,   144,   173,
     134,   305,   304,    80,   309,   249,     0,   131,   308,   307,
     286,   255,   215,     0,   252,   476,   477,   478,   479,   313,
       0,     0,   346,     0,    63,   367,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,    82,     0,    82,     0,
       0,   430,   424,   426,     0,   429,   421,     0,   412,   475,
     331,   473,   333,     0,   324,   335,   334,     0,   337,     0,
     326,     0,     0,   157,   160,     0,     0,   163,   166,     0,
     215,     0,   156,     0,     0,   178,   181,     0,   147,   184,
       0,     0,    43,    53,     0,     0,   170,   215,   278,   279,
     205,   152,     0,   486,     0,     0,     0,   503,     0,     0,
     259,   258,   291,    25,   257,   256,     0,   206,     0,   145,
     146,     0,   312,     0,   133,   139,   140,     0,     0,    82,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   280,   281,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,     0,     0,   374,
       0,   315,     0,   376,     0,   343,   344,   342,     0,   361,
       0,     0,     0,     0,   410,     0,     0,    85,     0,    85,
       0,    85,   153,   329,     0,   327,     0,   422,   420,   314,
     338,   341,   340,    54,     0,     0,     0,   167,   215,     0,
       0,   164,     0,     0,    55,     0,     0,     0,     0,    41,
      42,     0,   171,   215,   168,   206,     0,     0,   153,     0,
       0,   494,   484,   486,     0,   497,   498,   499,     0,     0,
       0,   486,     0,     0,     0,     0,    22,     0,    20,     0,
     282,   237,   236,   148,    46,     0,   287,     0,   199,   174,
     177,    85,   151,     0,   301,   302,   222,   223,   225,   224,
     226,   219,   220,   221,   260,   261,   273,   227,   228,   274,
     271,   272,     0,     0,   217,   296,   285,     0,   310,   311,
     218,   262,   263,   229,   230,     0,   216,     0,     0,   276,
     277,   275,   269,   270,   265,   264,   266,   267,   268,   243,
       0,     0,   289,     0,   315,   316,     0,   319,   342,   347,
       0,   351,   215,   437,     0,   443,   433,   414,     0,   447,
       0,   454,     0,   461,     0,     0,   467,     0,   470,     0,
     334,     0,     0,   158,     0,     0,     0,     0,   165,   172,
     292,     0,     0,   293,     0,     0,   169,     0,   487,     0,
       0,   486,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   504,   505,     0,    23,    26,     0,
      82,     0,     0,    44,    45,     0,    51,     0,     0,     0,
     288,     0,   298,     0,   232,   284,   231,   303,     0,   245,
     283,   290,   320,     0,   317,   349,   215,   350,   438,     0,
     444,   434,   415,   448,   450,   455,   457,   462,   464,   328,
     468,   330,   471,   339,     0,   159,   190,     0,   187,   179,
     182,   185,     0,     0,   481,   480,   486,     0,     0,   485,
       0,   489,   495,   501,   500,     0,   486,     0,   502,    21,
      24,     0,    85,   128,    47,     0,     0,   200,     0,   201,
     208,     0,   234,     0,   297,   246,     0,   322,   348,   440,
     451,   458,   465,    52,     0,     0,     0,     0,     0,     0,
     294,     0,     0,     0,     0,     0,     0,   488,     0,     0,
     492,   238,     0,    46,   233,     0,   202,     0,   235,     0,
     441,     0,     0,     0,     0,   180,   183,   186,   295,   506,
       0,   490,   483,   482,   506,     0,     0,   241,    48,   203,
     299,   321,   191,   193,   188,     0,     0,     0,   491,     0,
     493,   239,     0,   300,     0,     0,     0,     0,     0,     0,
       0,   242,   192,     0,   196,   189,   507,   508,   509,   194,
       0,     0,     0,   195,   197,     0,   198
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -570,  -570,  -570,   177,   573,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,   546,  -570,  -570,   589,  -570,  -570,
    -570,  -195,  -570,  -570,  -570,  -570,  -570,  -570,   409,  -570,
     594,   -53,  -570,   578,  -570,  -169,  -377,  -520,  -570,  -570,
    -158,   -94,   -77,  -570,    57,  -279,   -13,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,   661,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -500,  -570,  -570,  -290,  -570,  -229,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,   403,  -570,
    -570,   302,    -1,  -570,  -570,  -570,   391,  -570,   -19,  -570,
    -263,   287,  -270,  -262,   143,  -570,   201,  -570,  -570,  -570,
    -570,   497,   605,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -145,  -106,  -570,  -570,  -570,   304,  -570,  -570,  -570,
     -75,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,   392,
    -568,  -435,  -569,  -570,  -570,  -296,   -42,   234,  -570,  -570,
    -570,  -186,  -570
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   449,   328,   728,   329,   599,
      16,    17,    35,    36,    63,    18,    32,    33,   330,   331,
     735,   736,   332,   333,   334,   335,   336,   337,   170,   171,
      29,    30,    45,    46,    47,    19,   154,   229,    80,    20,
      81,    82,   338,   339,   230,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   468,   349,   350,   351,   565,   827,
     566,   828,   568,   829,   352,   824,   884,   885,   901,   900,
     905,   607,   810,   353,   354,   355,   419,   357,   358,   359,
     360,   729,   866,   730,   882,   361,   660,   815,   608,   811,
     883,   225,   666,   522,   369,   816,   226,   227,   543,   544,
     220,   221,   710,   271,   528,   373,   235,   160,    54,    22,
      86,   108,    67,    68,    23,    24,   158,    84,    52,    25,
     159,   272,   363,   107,   213,   214,   218,   215,   382,   762,
     545,   380,   761,   377,   758,   378,   850,   379,   760,   383,
     763,   384,   820,   385,   765,   386,   821,   387,   767,   388,
     822,   389,   770,   390,   772,    26,    41,    27,    40,   364,
     582,   583,   584,   365,   585,   586,   587,   588,   366,   448,
     367,   877,   368
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   356,   409,   212,   156,   402,   418,   537,   404,   539,
     591,   541,   -80,   447,   714,   236,   441,   680,   663,   682,
     115,   684,   721,   392,   436,   577,    66,    87,    88,     2,
     577,   472,   473,   554,   216,     3,   707,   738,   719,    59,
     270,   808,   470,   223,   405,    42,    43,   174,   495,   496,
     677,   613,   709,   492,   692,   694,   495,   496,     4,    64,
       5,    48,     6,   233,     7,   174,   110,   273,   462,     8,
     398,   274,    60,   678,    28,   406,     9,   256,   739,   219,
      43,   525,    10,    44,   238,   239,   224,   175,    65,   526,
     155,   741,   611,   256,   601,    31,   211,    56,   471,    11,
     517,   463,   257,   234,    71,   375,   516,   739,   517,   774,
     399,   737,   527,   482,   483,   484,   485,    44,   257,   401,
      12,   739,   212,   492,   222,   494,   495,   496,   497,   233,
     551,   498,   223,   500,   501,   412,    38,   557,   832,    34,
     393,   243,   787,    89,   231,   789,    37,    90,    43,    91,
      92,   686,   838,   275,   713,    76,   572,   370,   394,   258,
     370,   212,   370,   511,   512,   513,   514,   515,   395,   614,
     269,   244,    13,    61,   687,   258,   516,   212,   517,    39,
     612,   519,    93,    62,   469,    44,    53,   405,    55,   259,
     260,    56,   400,    57,   261,   709,    13,   262,   164,   165,
     520,   212,   212,    14,   685,   259,   260,   783,   413,   240,
     261,   421,   263,   262,   555,   211,   405,   833,   406,   795,
     405,   739,   265,   433,   835,    77,    78,   839,   263,   732,
      74,    71,   443,   445,   739,   264,   241,   739,   265,   739,
     733,   734,    56,   813,   712,   472,   473,   406,   740,   671,
     370,   406,   717,   407,   211,    75,   408,    79,   718,    49,
      50,   751,    51,  -449,   775,   212,   848,   403,  -449,   806,
     211,   212,   212,   212,   212,    58,   786,   212,    66,   212,
     691,   212,   842,   212,    69,   796,  -449,   708,    70,   594,
     688,   595,   825,    71,   211,   211,   524,   576,   724,   589,
      72,   212,   530,   531,   532,   533,    73,   453,   536,    83,
     538,   256,   540,   689,   166,   826,   472,   473,   167,   370,
     168,    92,   371,   236,   718,   372,   722,   101,   212,   484,
     485,   102,   550,  -456,   553,    85,   257,   492,  -456,   494,
     495,   496,   497,   665,   752,   498,    96,   564,   103,   104,
     105,   106,  -463,   802,   109,    98,  -456,  -463,   211,   571,
     162,   596,   597,  -240,   211,   211,   211,   211,  -240,    99,
     211,   100,   211,   112,   211,  -463,   211,   511,   512,   513,
     514,   515,   113,   114,   604,   151,  -240,   152,   155,   153,
     516,   755,   517,   258,   211,   324,   440,   157,   161,   155,
     484,   485,   556,   162,   754,   173,    60,   217,   492,   219,
     494,   495,   496,   497,   228,   232,   498,   237,   249,   245,
     246,   211,   247,   259,   260,   248,   402,   250,   261,   381,
     673,   262,   251,   252,   253,   254,   255,   266,   267,   391,
     268,   233,   223,   415,   602,   423,   263,   425,   426,   427,
     513,   514,   515,   429,   430,   428,   265,   434,   458,   212,
     212,   516,   212,   517,   459,   460,   472,   473,   464,   465,
     442,   457,   467,    13,   523,   466,   470,   518,   547,   534,
     472,   473,   176,   562,   549,   699,   569,   570,   177,   575,
     701,   702,   615,   704,   603,   609,   632,   636,   659,   664,
     665,   667,   356,   715,   178,   725,   472,   473,   712,   716,
     238,   727,   737,   742,   743,   747,   749,   179,   836,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     484,   485,   211,   211,   398,   211,   764,   766,   492,   768,
     494,   495,   496,   497,   484,   485,   498,   776,   212,   777,
     782,   788,   492,   871,   797,   495,   496,   497,   790,   212,
     498,   212,   791,   793,   798,   800,   854,   834,   482,   483,
     484,   485,   486,   837,    43,   489,   490,   491,   492,   759,
     494,   495,   496,   497,   840,   865,   498,   844,   500,   501,
     769,   516,   860,   517,   867,   873,   861,   875,   888,   878,
     880,   890,   889,   212,   894,   516,   598,   517,    94,   892,
     897,    44,   898,   362,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   903,   172,   906,   376,    97,   868,   374,
      95,   516,   116,   517,   801,   804,   472,   473,   849,   817,
     731,   211,    21,   753,   809,   410,   397,   670,   771,   610,
     242,   521,   211,   111,   211,   414,   794,   546,   879,   592,
     411,     0,     0,     0,     0,     0,   535,     0,   420,     0,
       0,     0,   422,     0,   424,     0,     0,   823,     0,     0,
       0,     0,   432,     0,     0,   212,   437,   438,   439,     0,
       0,     0,   446,     0,     0,     0,   211,     0,   212,     0,
     450,   451,   452,     0,   454,   455,   456,     0,   843,     0,
       0,     0,     0,     0,     0,   461,   847,     0,   482,   483,
     484,   485,   486,     0,     0,   489,   490,   491,   492,   851,
     494,   495,   496,   497,     0,     0,   498,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
       0,   505,     0,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,     0,   212,     0,     0,     0,   211,     0,
       0,   516,     0,   517,     0,     0,   600,     0,     0,     0,
     893,   211,   552,     0,     0,     0,     0,     0,     0,   558,
       0,     0,     0,     0,     0,   902,   563,     0,     0,     0,
     567,     0,     0,     0,     0,     0,     0,     0,   573,     0,
       0,     0,     0,     0,   472,   473,   581,     0,   581,     0,
       0,     0,     0,   881,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   891,
       0,     0,   211,     0,     0,     0,     0,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   211,     0,   626,
     627,   628,   629,   630,   631,     0,   634,   635,     0,   637,
     638,   639,   640,   641,   642,   643,   644,   646,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,   658,     0,
     661,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,   672,     0,   489,   490,   491,   492,     0,   494,   495,
     496,   497,     0,     0,   498,     0,   500,   501,     0,     0,
       0,     0,     0,     0,   444,   472,   473,     0,     0,   697,
       0,     0,   634,   646,     0,     0,   176,     0,     0,     0,
       0,     0,   177,   508,   509,   510,   511,   512,   513,   514,
     515,     0,   450,   456,     0,     0,     0,     0,   178,   516,
       0,   517,     0,   456,     0,     0,   723,   446,     0,     0,
     726,   179,     0,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,     0,   744,   482,   483,   484,
     485,   486,     0,     0,   489,   490,   491,   492,   746,   494,
     495,   496,   497,     0,     0,   498,     0,   500,   501,     0,
       0,     0,     0,   472,   473,     0,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,    43,     0,
     505,     0,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   256,     0,   773,     0,     0,     0,     0,     0,
     516,     0,   517,     0,     0,     0,     0,     0,     0,     0,
     155,     0,   784,   785,     0,    44,     0,   257,     0,   792,
       0,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   362,     0,     0,     0,   805,     0,
       0,     0,   807,     0,     0,   482,   483,   484,   485,   486,
       0,   814,   489,   490,   491,   492,     0,   494,   495,   496,
     497,     0,     0,   498,     0,   500,   501,     0,     0,     0,
       0,     0,     0,   525,   258,     0,     0,     0,     0,     0,
       0,   526,     0,     0,     0,   831,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,     0,     0,   668,   260,     0,     0,   516,   261,
     517,   846,   262,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,   853,     0,
     855,   856,   857,     0,   669,   859,     0,   265,   862,   863,
       0,   864,     0,     0,     0,     0,   276,     0,   869,     0,
       0,     3,     0,   277,   278,   279,     0,   280,     0,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
     285,   286,   287,   288,     0,     0,     0,     0,   887,     0,
       0,     0,   289,   290,     0,     0,   291,   292,     0,     0,
     293,   896,     9,   294,   295,     0,   296,   297,     0,     0,
     298,   299,     0,     0,     0,   300,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   302,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,     0,     0,     0,     0,     0,
       0,     0,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    43,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,   318,   319,
       0,   320,     0,     0,   321,   322,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,   324,   325,    44,   326,
     155,   327,   276,     0,     0,     0,     0,     3,     0,   277,
     278,   279,     0,   280,     0,   281,   282,   283,   284,     0,
       0,     0,     0,     0,     0,     0,   285,   286,   287,   288,
       0,     0,     0,     0,     0,     0,     0,     0,   289,   290,
       0,     0,   291,   292,     0,     0,   293,     0,     9,   294,
     295,     0,   296,   297,     0,     0,   298,   299,     0,     0,
       0,   300,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   301,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   302,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,   305,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     306,     0,     0,     0,     0,     0,     0,     0,   307,   308,
     309,   310,   311,   312,   313,   314,   315,    43,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,   318,   319,     0,   320,     0,     0,
     321,   322,     0,     0,     0,     0,   281,   282,   283,   284,
     323,     0,   324,   325,    44,   326,   155,   803,   286,     0,
       0,     0,     0,     0,   256,     0,     0,     0,     0,     0,
     290,     0,     0,     0,   292,     0,     0,   293,     0,     0,
     294,     0,   577,     0,   297,     0,     0,     0,     0,   257,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   301,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   578,   316,
       0,     0,     0,     0,     0,     0,   259,   260,     0,     0,
       0,   261,     0,   257,   579,   318,   319,     0,   320,     0,
       0,   321,   322,     0,   281,   282,   283,   284,     0,   263,
       0,   580,     0,   324,   325,    44,   286,   155,     0,   265,
       0,     0,   256,     0,     0,     0,     0,     0,   290,     0,
       0,     0,   292,     0,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,     0,   257,     0,     0,
     258,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     301,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
     259,   260,   302,     0,     0,   261,     0,     0,   262,     0,
       0,     0,     0,     0,   304,   305,     0,     0,     0,     0,
       0,     0,     0,   263,   258,     0,     0,     0,     0,     0,
     674,     0,     0,   265,     0,     0,   256,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   578,   316,     0,     0,
       0,     0,     0,     0,   259,   260,     0,     0,     0,   261,
       0,   257,   579,   318,   319,     0,   320,     0,     0,   321,
     322,     0,   281,   282,   283,   284,     0,   263,     0,   590,
       0,   324,   325,    44,   286,   155,     0,   265,     0,     0,
     392,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,   258,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   301,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   259,   260,
     302,     0,     0,   261,     0,   675,   262,     0,     0,     0,
       0,     0,   304,   305,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,   307,   308,   309,   310,   311,
     312,   313,   314,   315,    43,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   393,     0,   256,
     317,   318,   319,     0,   320,     0,     0,   321,   322,     0,
     281,   282,   283,   284,     0,   394,     0,   323,     0,   324,
     325,    44,   286,   155,   257,   395,     0,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,   292,     0,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   301,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,   258,   205,   206,   207,   208,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   305,     0,     0,     0,     0,     0,     0,     0,   416,
       0,   259,   260,     0,     0,   306,   261,     0,   676,   262,
       0,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    43,   316,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,     0,     0,   317,   318,
     319,     0,   320,     0,     0,   321,   322,     0,     0,     0,
     281,   282,   283,   284,     0,   323,     0,   324,   325,    44,
     417,   155,   286,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,   292,     0,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   301,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   305,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    43,   316,     0,     0,     0,     0,     0,     0,
       0,     0,   281,   282,   283,   284,     0,     0,   317,   318,
     319,     0,   320,     0,   286,   321,   322,     0,     0,     0,
       0,     0,     0,     0,     0,   323,   290,   324,   325,    44,
     292,   155,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   301,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,   307,   308,   309,   310,   311,
     312,   313,   314,   315,    43,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     317,   318,   319,     0,   320,     0,     0,   321,   322,     0,
     281,   282,   283,   284,   695,   605,     0,   323,   606,   324,
     325,    44,   286,   155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,   292,     0,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,   258,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   301,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,   259,   260,   302,     0,
       0,   261,     0,   679,   262,     0,     0,     0,     0,     0,
     304,   305,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    43,   316,     0,     0,     0,     0,     0,     0,
       0,     0,   281,   282,   283,   284,   696,     0,   317,   318,
     319,     0,   320,     0,   286,   321,   322,     0,     0,     0,
       0,     0,     0,     0,     0,   323,   290,   324,   325,    44,
     292,   155,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   301,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,   308,   309,   310,   311,
     312,   313,   314,   315,    43,   316,     0,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   284,     0,     0,
     317,   318,   319,     0,   320,     0,   286,   321,   322,     0,
       0,     0,     0,     0,     0,     0,   396,   323,   290,   324,
     325,    44,   292,   155,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,   431,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     301,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,   308,   309,
     310,   311,   312,   313,   314,   315,    43,   316,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
       0,     0,   317,   318,   319,     0,   320,     0,   286,   321,
     322,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     290,   324,   325,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   301,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   306,     0,     0,     0,     0,     0,     0,     0,   307,
     308,   309,   310,   311,   312,   313,   314,   315,    43,   316,
       0,     0,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   284,     0,     0,   317,   318,   319,     0,   320,     0,
     286,   321,   322,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   290,   324,   325,    44,   292,   155,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   301,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,   305,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   633,     0,     0,     0,     0,     0,     0,
       0,   307,   308,   309,   310,   311,   312,   313,   314,   315,
      43,   316,     0,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,     0,     0,   317,   318,   319,     0,
     320,     0,   286,   321,   322,   256,     0,     0,     0,     0,
       0,     0,     0,   323,   290,   324,   325,    44,   292,   155,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
     257,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   301,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   305,     0,     0,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,   645,     0,     0,     0,     0,
       0,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    43,   316,     0,     0,   256,   259,   260,     0,
       0,     0,   261,     0,   681,   262,     0,     0,   317,   318,
     319,     0,   320,     0,     0,   321,   322,     0,     0,     0,
     263,   257,     0,     0,     0,   323,     0,   324,   325,    44,
     265,   155,   281,   282,   283,   284,     0,     0,     0,   647,
       0,     0,     0,     0,   286,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,   258,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   301,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   259,   260,
     302,     0,     0,   261,     0,   683,   262,     0,     0,     0,
       0,     0,   304,   305,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,   307,   308,   309,   310,   311,
     312,   313,   314,   315,    43,   316,     0,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   284,     0,     0,
     317,   318,   319,     0,   320,     0,   286,   321,   322,     0,
       0,   256,     0,     0,     0,     0,     0,   323,   290,   324,
     325,    44,   292,   155,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,   257,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     301,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,     0,     0,     0,     0,
       0,     0,   405,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,   308,   309,
     310,   311,   312,   313,   314,   315,    43,   316,     0,     0,
       0,     0,     0,   690,   260,     0,     0,     0,   261,     0,
       0,   262,   317,   318,   319,     0,   320,     0,     0,   321,
     322,     0,   281,   282,   283,   284,   263,     0,     0,   323,
     662,   324,   325,    44,   286,   155,   265,     0,     0,   256,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,   257,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   301,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,     0,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,   308,   309,   310,   311,
     312,   313,   314,   315,    43,   316,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,   261,     0,   705,   262,
     317,   318,   319,     0,   320,     0,     0,   321,   322,     0,
     281,   282,   283,   284,   263,     0,     0,   323,   693,   324,
     325,    44,   286,   155,   265,     0,     0,   256,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,   292,     0,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
       0,     0,   257,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   301,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   305,     0,     0,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    43,   316,     0,     0,     0,     0,     0,   259,
     260,     0,     0,     0,   261,     0,   779,   262,   317,   318,
     319,     0,   320,     0,     0,   321,   322,     0,   281,   282,
     283,   284,   263,     0,     0,   323,   711,   324,   325,    44,
     286,   155,   265,     0,     0,   256,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,   292,     0,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,     0,     0,
     257,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   301,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,   305,
       0,     0,     0,     0,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,   308,   309,   310,   311,   312,   313,   314,   315,
      43,   316,     0,     0,     0,     0,     0,   259,   260,     0,
       0,     0,   261,     0,   780,   262,   317,   318,   319,     0,
     320,     0,     0,   321,   322,     0,   281,   282,   283,   284,
     263,     0,     0,   323,   720,   324,   325,    44,   286,   155,
     265,     0,     0,   256,     0,     0,     0,     0,     0,     0,
     290,     0,     0,     0,   292,     0,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,   257,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   301,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,     0,     0,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
     308,   309,   310,   311,   312,   313,   314,   315,    43,   316,
       0,     0,     0,     0,     0,   259,   260,     0,     0,     0,
     261,     0,   781,   262,   317,   318,   319,     0,   320,     0,
       0,   321,   322,     0,   281,   282,   283,   284,   263,     0,
       0,   323,   812,   324,   325,    44,   286,   155,   265,     0,
       0,   256,     0,     0,     0,     0,     0,     0,   290,     0,
       0,     0,   292,     0,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,   257,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     301,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,     0,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,   308,   309,
     310,   311,   312,   313,   314,   315,    43,   316,     0,     0,
       0,     0,     0,   259,   260,     0,     0,     0,   261,     0,
     819,   262,   317,   318,   319,     0,   320,     0,     0,   321,
     322,     0,   281,   282,   283,   284,   263,     0,     0,   323,
     830,   324,   325,    44,   286,   155,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   301,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,     0,     0,     0,     0,     0,     0,
       0,   845,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,   308,   309,   310,   311,
     312,   313,   314,   315,    43,   316,     0,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   284,   852,     0,
     317,   318,   319,     0,   320,     0,   286,   321,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   323,   290,   324,
     325,    44,   292,   155,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     301,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,   308,   309,
     310,   311,   312,   313,   314,   315,    43,   316,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     886,     0,   317,   318,   319,     0,   320,     0,   286,   321,
     322,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     290,   324,   325,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   301,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   302,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
     308,   309,   310,   311,   312,   313,   314,   315,    43,   316,
       0,     0,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   284,     0,     0,   317,   318,   319,     0,   320,     0,
     286,   321,   322,   256,     0,     0,     0,     0,     0,     0,
       0,   323,   290,   324,   325,    44,   292,   155,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,   257,     0,
       0,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   301,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   472,   473,   304,   305,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,   308,   309,   310,   311,   312,   313,   314,   315,
      43,   316,     0,     0,     0,   259,   260,     0,     0,     0,
     261,     0,   841,   262,     0,     0,   317,   318,   319,     0,
     320,     0,     0,   321,   322,     0,     0,     0,   263,     0,
       0,     0,     0,   323,     0,   324,   325,    44,   265,   155,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   559,
     494,   495,   496,   497,   472,   473,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,   505,     0,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,   517,     0,     0,     0,     0,     0,     0,
     561,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   559,   494,   495,
     496,   497,   472,   473,   498,   499,   500,   501,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   560,   505,
       0,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,   516,
       0,   517,     0,     0,     0,     0,     0,     0,   574,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   559,   494,   495,   496,   497,
     472,   473,   498,   499,   500,   501,   502,   503,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   560,   505,     0,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,     0,
       0,     0,     0,     0,     0,     0,     0,   516,     0,   517,
       0,     0,     0,     0,     0,     0,   698,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   559,   494,   495,   496,   497,   472,   473,
     498,   499,   500,   501,   502,   503,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   560,   505,     0,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,     0,     0,     0,
       0,     0,     0,     0,     0,   516,     0,   517,     0,     0,
       0,     0,     0,     0,   706,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   559,   494,   495,   496,   497,   472,   473,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   560,   505,     0,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,   516,     0,   517,     0,     0,     0,     0,
       0,     0,   757,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   472,   473,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   559,
     494,   495,   496,   497,     0,     0,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     560,   505,     0,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,     0,     0,   472,   473,     0,     0,     0,
       0,   516,     0,   517,   482,   483,   484,   485,   486,     0,
     818,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   505,     0,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,     0,
       0,     0,     0,     0,   472,   473,     0,   516,     0,   517,
       0,     0,     0,     0,     0,     0,   876,   482,   483,   484,
     485,   486,     0,     0,   489,   490,   491,   492,     0,   494,
     495,   496,   497,     0,     0,   498,     0,   500,   501,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     505,     0,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,     0,   472,   473,     0,     0,     0,     0,     0,
     516,     0,   517,     0,     0,   700,   482,   483,   484,   485,
     486,     0,     0,   489,   490,   491,   492,     0,   494,   495,
     496,   497,     0,     0,   498,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,     0,   472,   473,     0,     0,     0,     0,     0,   516,
       0,   517,     0,     0,   703,   482,   483,   484,   485,   486,
       0,     0,   489,   490,   491,   492,     0,   494,   495,   496,
     497,     0,     0,   498,     0,   500,   501,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,   472,   473,     0,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   778,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   505,     0,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,     0,
     472,   473,     0,     0,     0,     0,     0,   516,     0,   517,
       0,     0,   858,   482,   483,   484,   485,   486,     0,     0,
     489,   490,   491,   492,     0,   494,   495,   496,   497,     0,
       0,   498,     0,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   505,     0,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,   517,     0,
       0,   874,   482,   483,   484,   485,   486,     0,     0,   489,
     490,   491,   492,     0,   494,   495,   496,   497,     0,     0,
     498,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,   177,     0,     0,   505,     0,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,     0,   178,     0,
       0,     0,     0,     0,     0,   516,     0,   517,     0,     0,
     895,   179,     0,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,     0,     0,   307,
     308,   309,     0,     0,     0,     0,     0,   179,    43,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     472,   473,     0,     0,     0,    44,   256,     0,   176,     0,
       0,     0,     0,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,    43,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   472,   473,     0,     0,
       0,    44,   482,   483,   484,   485,   486,     0,   258,   489,
     490,   491,   492,     0,   494,   495,   496,   497,     0,     0,
     498,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   260,
     542,     0,     0,   261,     0,   505,   262,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,     0,     0,     0,
       0,   263,     0,   472,   473,   516,     0,   517,   548,     0,
       0,   265,     0,     0,     0,     0,     0,    44,   482,   483,
     484,   485,   486,     0,     0,   489,   490,   491,   492,     0,
     494,   495,   496,   497,     0,     0,   498,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   505,     0,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,     0,     0,     0,     0,     0,   472,   473,
       0,   516,     0,   517,   745,   482,   483,   484,   485,   486,
       0,     0,   489,   490,   491,   492,     0,   494,   495,   496,
     497,     0,     0,   498,     0,   500,   501,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
     517,   750,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   559,   494,   495,   496,   497,  -215,     0,   498,   499,
     500,   501,   502,   503,   472,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   560,   505,     0,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,   516,     0,   517,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,   473,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,     0,     0,   498,   499,   500,   501,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   472,   473,     0,     0,     0,     0,     0,     0,   516,
       0,   517,   482,   483,   484,   485,   486,     0,     0,   489,
     490,   491,   492,     0,   494,   495,   496,   497,     0,     0,
     498,     0,   500,   501,     0,     0,   593,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,     0,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   472,   473,     0,
       0,     0,     0,     0,     0,   516,     0,   517,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   486,     0,     0,
     489,   490,   491,   492,     0,   494,   495,   496,   497,     0,
       0,   498,     0,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   505,   748,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   472,   473,
       0,     0,     0,     0,     0,     0,   516,     0,   517,   482,
     483,   484,   485,   486,     0,     0,   489,   490,   491,   492,
       0,   494,   495,   496,   497,     0,     0,   498,     0,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   799,     0,
       0,     0,   505,     0,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   472,   473,     0,     0,     0,     0,
       0,     0,   516,     0,   517,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,     0,   489,   490,   491,
     492,     0,   494,   495,   496,   497,     0,     0,   498,     0,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   472,   473,   505,     0,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,   516,     0,   517,   482,   483,   484,   485,
     486,     0,     0,   489,   490,   491,   492,     0,   494,   495,
     496,   497,     0,     0,   498,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   472,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   509,   510,   511,   512,   513,   514,
     515,     0,     0,   482,   483,   484,   485,   486,     0,   516,
     489,   517,   256,   492,     0,   494,   495,   496,   497,     0,
       0,   498,     0,   500,   501,   256,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,   509,   510,   511,   512,   513,   514,   515,     0,     0,
       0,   257,     0,   482,   483,     0,   516,   486,   517,     0,
     489,   490,   491,   492,     0,   494,   495,   496,   497,   256,
       0,   498,     0,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   257,     0,   505,   258,   506,   507,
     508,   509,   510,     0,     0,     0,   514,     0,   258,     0,
       0,     0,     0,     0,   259,   260,   516,     0,   517,   261,
       0,   870,   262,     0,     0,     0,     0,   259,   260,     0,
       0,     0,   261,     0,   872,   262,     0,   263,   259,   260,
       0,     0,     0,   261,     0,   899,   262,   265,     0,     0,
     263,   258,     0,     0,     0,     0,     0,     0,     0,     0,
     265,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,   261,     0,   904,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   136,   137,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,     0,     0,
     149,   150
};

static const yytype_int16 yycheck[] =
{
      13,   230,   272,   109,    81,   268,   285,   384,   270,   386,
     445,   388,     7,   309,   583,   160,   306,   537,   518,   539,
      73,   541,   591,    32,   303,    50,   136,    14,    15,     0,
      50,    20,    21,    19,   109,     6,    19,   605,    19,    21,
      47,   126,   118,   127,   113,    33,   136,   136,   115,   116,
     151,    18,   121,   112,   554,   555,   115,   116,    29,   146,
      31,    74,    33,   136,    35,   136,   176,   170,   136,    40,
     136,   174,    54,   174,   136,   144,    47,    32,   143,   136,
     136,   113,    53,   173,   138,   139,   170,   176,   175,   121,
     175,   611,   469,    32,   150,   130,   109,   143,   174,    70,
     167,   169,    57,   176,   173,   176,   165,   143,   167,   174,
     176,   136,   144,   102,   103,   104,   105,   173,    57,   176,
      91,   143,   228,   112,   170,   114,   115,   116,   117,   136,
     409,   120,   127,   122,   123,   280,   175,   416,   174,   136,
     149,   144,   711,   130,   157,   713,   136,   134,   136,   136,
     137,   151,   174,   228,   174,    34,   435,   143,   167,   114,
     143,   267,   143,   152,   153,   154,   155,   156,   177,   136,
     223,   174,   167,   155,   174,   114,   165,   283,   167,   136,
     470,   176,   169,   165,   353,   173,    58,   113,   144,   144,
     145,   143,   267,   146,   149,   121,   167,   152,    14,    15,
     369,   307,   308,   174,   146,   144,   145,   707,   283,   143,
     149,   288,   167,   152,   169,   228,   113,   786,   144,   719,
     113,   143,   177,   300,   121,   104,   105,   796,   167,    11,
     143,   173,   307,   308,   143,   174,   170,   143,   177,   143,
      22,    23,   143,   743,   143,    20,    21,   144,   170,   528,
     143,   144,   168,   146,   267,   168,   149,   136,   174,     4,
       5,   170,     7,   146,   170,   371,   170,   168,   151,   168,
     283,   377,   378,   379,   380,   143,   711,   383,   136,   385,
     550,   387,   802,   389,   136,   720,   169,   577,   136,   174,
     151,   176,   151,   173,   307,   308,   371,   442,   594,   444,
     136,   407,   377,   378,   379,   380,   169,   320,   383,   136,
     385,    32,   387,   174,   130,   174,    20,    21,   134,   143,
     136,   137,   146,   468,   174,   149,   176,    65,   434,   104,
     105,    69,   407,   146,   411,   175,    57,   112,   151,   114,
     115,   116,   117,    43,    44,   120,   130,   424,    86,    87,
      88,    89,   146,   730,   144,   136,   169,   151,   371,   434,
     138,   139,   140,   146,   377,   378,   379,   380,   151,   136,
     383,   136,   385,   175,   387,   169,   389,   152,   153,   154,
     155,   156,   175,   136,   461,    34,   169,    34,   175,   169,
     165,   670,   167,   114,   407,   171,   172,   146,     1,   175,
     104,   105,   415,   138,   666,   175,    54,   136,   112,   136,
     114,   115,   116,   117,   146,   175,   120,   174,   169,   150,
     150,   434,   150,   144,   145,   150,   689,   150,   149,   136,
     151,   152,   150,   150,   150,   150,   150,   174,   146,    32,
     174,   136,   127,   169,   457,   169,   167,   150,   150,   169,
     154,   155,   156,   150,   130,   174,   177,   150,    36,   565,
     566,   165,   568,   167,   174,   174,    20,    21,   174,   174,
      12,   172,    19,   167,   136,   174,   118,   169,   168,   170,
      20,    21,    24,    13,   168,   562,   146,   130,    30,   172,
     565,   566,   136,   568,   175,    39,   136,   136,   136,     7,
      43,   174,   731,   168,    46,   176,    20,    21,   143,   143,
     138,   138,   136,   150,   169,   136,     1,    59,   788,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     104,   105,   565,   566,   136,   568,   151,   151,   112,   151,
     114,   115,   116,   117,   104,   105,   120,   150,   674,   136,
     169,   136,   112,   174,   176,   115,   116,   117,   168,   685,
     120,   687,   168,   168,   168,   138,   136,   168,   102,   103,
     104,   105,   106,   168,   136,   109,   110,   111,   112,   674,
     114,   115,   116,   117,   168,   176,   120,   170,   122,   123,
     685,   165,   168,   167,   151,   150,   168,   151,    50,   168,
     168,   176,   168,   729,   150,   165,   449,   167,    55,   170,
     168,   173,   168,   230,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   170,    98,   170,   243,    58,   843,   240,
      56,   165,    74,   167,   729,   732,    20,    21,   816,   753,
     603,   674,     1,   664,   741,   274,   263,   524,   687,   468,
     173,   369,   685,    68,   687,   283,   718,   390,   864,   445,
     277,    -1,    -1,    -1,    -1,    -1,   382,    -1,   285,    -1,
      -1,    -1,   289,    -1,   291,    -1,    -1,   774,    -1,    -1,
      -1,    -1,   299,    -1,    -1,   811,   303,   304,   305,    -1,
      -1,    -1,   309,    -1,    -1,    -1,   729,    -1,   824,    -1,
     317,   318,   319,    -1,   321,   322,   323,    -1,   805,    -1,
      -1,    -1,    -1,    -1,    -1,   332,   811,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,   110,   111,   112,   824,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   885,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    -1,   900,    -1,    -1,    -1,   811,    -1,
      -1,   165,    -1,   167,    -1,    -1,   170,    -1,    -1,    -1,
     885,   824,   409,    -1,    -1,    -1,    -1,    -1,    -1,   416,
      -1,    -1,    -1,    -1,    -1,   900,   423,    -1,    -1,    -1,
     427,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   435,    -1,
      -1,    -1,    -1,    -1,    20,    21,   443,    -1,   445,    -1,
      -1,    -1,    -1,   866,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   882,
      -1,    -1,   885,    -1,    -1,    -1,    -1,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   900,    -1,   486,
     487,   488,   489,   490,   491,    -1,   493,   494,    -1,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,    -1,
     517,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
     106,   528,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    20,    21,    -1,    -1,   556,
      -1,    -1,   559,   560,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,   149,   150,   151,   152,   153,   154,   155,
     156,    -1,   579,   580,    -1,    -1,    -1,    -1,    46,   165,
      -1,   167,    -1,   590,    -1,    -1,   593,   594,    -1,    -1,
     597,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    -1,   633,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,   645,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   670,    -1,    -1,    -1,    -1,   136,    -1,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    32,    -1,   691,    -1,    -1,    -1,    -1,    -1,
     165,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   709,   710,    -1,   173,    -1,    57,    -1,   716,
      -1,   718,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   731,    -1,    -1,    -1,   735,    -1,
      -1,    -1,   739,    -1,    -1,   102,   103,   104,   105,   106,
      -1,   748,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   782,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,   165,   149,
     167,   808,   152,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   825,    -1,
     827,   828,   829,    -1,   174,   832,    -1,   177,   835,   836,
      -1,   838,    -1,    -1,    -1,    -1,     1,    -1,   845,    -1,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,   875,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    42,    -1,    -1,
      45,   888,    47,    48,    49,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,   154,
      -1,   156,    -1,    -1,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,
     175,   176,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    -1,    41,    42,    -1,    -1,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,   153,   154,    -1,   156,    -1,    -1,
     159,   160,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
     169,    -1,   171,   172,   173,   174,   175,   176,    26,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,
      -1,   149,    -1,    57,   152,   153,   154,    -1,   156,    -1,
      -1,   159,   160,    -1,    14,    15,    16,    17,    -1,   167,
      -1,   169,    -1,   171,   172,   173,    26,   175,    -1,   177,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    -1,    -1,
     114,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
     144,   145,    92,    -1,    -1,   149,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   114,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,   177,    -1,    -1,    32,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,   149,
      -1,    57,   152,   153,   154,    -1,   156,    -1,    -1,   159,
     160,    -1,    14,    15,    16,    17,    -1,   167,    -1,   169,
      -1,   171,   172,   173,    26,   175,    -1,   177,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   144,   145,
      92,    -1,    -1,   149,    -1,   151,   152,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    32,
     152,   153,   154,    -1,   156,    -1,    -1,   159,   160,    -1,
      14,    15,    16,    17,    -1,   167,    -1,   169,    -1,   171,
     172,   173,    26,   175,    57,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,   114,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,   144,   145,    -1,    -1,   119,   149,    -1,   151,   152,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,   169,    -1,   171,   172,   173,
     174,   175,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,   152,   153,
     154,    -1,   156,    -1,    26,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    38,   171,   172,   173,
      42,   175,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
     152,   153,   154,    -1,   156,    -1,    -1,   159,   160,    -1,
      14,    15,    16,    17,    18,   167,    -1,   169,   170,   171,
     172,   173,    26,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,   144,   145,    92,    -1,
      -1,   149,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,   150,    -1,   152,   153,
     154,    -1,   156,    -1,    26,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    38,   171,   172,   173,
      42,   175,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
     152,   153,   154,    -1,   156,    -1,    26,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,   169,    38,   171,
     172,   173,    42,   175,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   152,   153,   154,    -1,   156,    -1,    26,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      38,   171,   172,   173,    42,   175,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   152,   153,   154,    -1,   156,    -1,
      26,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    38,   171,   172,   173,    42,   175,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,   152,   153,   154,    -1,
     156,    -1,    26,   159,   160,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    38,   171,   172,   173,    42,   175,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    32,   144,   145,    -1,
      -1,    -1,   149,    -1,   151,   152,    -1,    -1,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,    -1,    -1,    -1,
     167,    57,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
     177,   175,    14,    15,    16,    17,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   144,   145,
      92,    -1,    -1,   149,    -1,   151,   152,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
     152,   153,   154,    -1,   156,    -1,    26,   159,   160,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   169,    38,   171,
     172,   173,    42,   175,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,   144,   145,    -1,    -1,    -1,   149,    -1,
      -1,   152,   152,   153,   154,    -1,   156,    -1,    -1,   159,
     160,    -1,    14,    15,    16,    17,   167,    -1,    -1,   169,
     170,   171,   172,   173,    26,   175,   177,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,   144,   145,    -1,    -1,    -1,   149,    -1,   151,   152,
     152,   153,   154,    -1,   156,    -1,    -1,   159,   160,    -1,
      14,    15,    16,    17,   167,    -1,    -1,   169,   170,   171,
     172,   173,    26,   175,   177,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,    -1,    -1,   149,    -1,   151,   152,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,    -1,    14,    15,
      16,    17,   167,    -1,    -1,   169,   170,   171,   172,   173,
      26,   175,   177,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,
      -1,    -1,   149,    -1,   151,   152,   152,   153,   154,    -1,
     156,    -1,    -1,   159,   160,    -1,    14,    15,    16,    17,
     167,    -1,    -1,   169,   170,   171,   172,   173,    26,   175,
     177,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,
     149,    -1,   151,   152,   152,   153,   154,    -1,   156,    -1,
      -1,   159,   160,    -1,    14,    15,    16,    17,   167,    -1,
      -1,   169,   170,   171,   172,   173,    26,   175,   177,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,   144,   145,    -1,    -1,    -1,   149,    -1,
     151,   152,   152,   153,   154,    -1,   156,    -1,    -1,   159,
     160,    -1,    14,    15,    16,    17,   167,    -1,    -1,   169,
     170,   171,   172,   173,    26,   175,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     152,   153,   154,    -1,   156,    -1,    26,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    38,   171,
     172,   173,    42,   175,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   152,   153,   154,    -1,   156,    -1,    26,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      38,   171,   172,   173,    42,   175,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   152,   153,   154,    -1,   156,    -1,
      26,   159,   160,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    38,   171,   172,   173,    42,   175,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   104,   105,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,
     149,    -1,   151,   152,    -1,    -1,   152,   153,   154,    -1,
     156,    -1,    -1,   159,   160,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,   169,    -1,   171,   172,   173,   177,   175,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,   165,    -1,   167,   102,   103,   104,   105,   106,    -1,
     174,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,   165,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     165,    -1,   167,    -1,    -1,   170,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   165,
      -1,   167,    -1,    -1,   170,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   165,    -1,
     167,    -1,    -1,   170,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   165,    -1,   167,
      -1,    -1,   170,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,   167,    -1,
      -1,   170,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,   167,    -1,    -1,
     170,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    59,   136,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      20,    21,    -1,    -1,    -1,   173,    32,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,   136,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    20,    21,    -1,    -1,
      -1,   173,   102,   103,   104,   105,   106,    -1,   114,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     136,    -1,    -1,   149,    -1,   145,   152,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,    -1,
      -1,   167,    -1,    20,    21,   165,    -1,   167,   168,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,   173,   102,   103,
     104,   105,   106,    -1,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   165,    -1,   167,   168,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
     167,   168,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,   167,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,   167,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,   167,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,   167,    -1,    -1,    -1,    -1,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   167,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   102,   103,   104,   105,   106,    -1,   165,
     109,   167,    32,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
      -1,    57,    -1,   102,   103,    -1,   165,   106,   167,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    32,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,   145,   114,   147,   148,
     149,   150,   151,    -1,    -1,    -1,   155,    -1,   114,    -1,
      -1,    -1,    -1,    -1,   144,   145,   165,    -1,   167,   149,
      -1,   151,   152,    -1,    -1,    -1,    -1,   144,   145,    -1,
      -1,    -1,   149,    -1,   151,   152,    -1,   167,   144,   145,
      -1,    -1,    -1,   149,    -1,   151,   152,   177,    -1,    -1,
     167,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,    -1,    -1,    -1,   149,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   179,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    70,    91,   167,   174,   180,   188,   189,   193,   213,
     217,   233,   287,   292,   293,   297,   333,   335,   136,   208,
     209,   130,   194,   195,   136,   190,   191,   136,   175,   136,
     336,   334,    33,   136,   173,   210,   211,   212,   224,     4,
       5,     7,   296,    58,   286,   144,   143,   146,   143,    21,
      54,   155,   165,   192,   146,   175,   136,   290,   291,   136,
     136,   173,   136,   169,   143,   168,    34,   104,   105,   136,
     216,   218,   219,   136,   295,   175,   288,    14,    15,   130,
     134,   136,   137,   169,   182,   208,   130,   195,   136,   136,
     136,    65,    69,    86,    87,    88,    89,   301,   289,   144,
     176,   290,   175,   175,   136,   209,   211,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   122,   123,   124,   125,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   159,
     160,    34,    34,   169,   214,   175,   220,   146,   294,   298,
     285,     1,   138,   181,    14,    15,   130,   134,   136,   182,
     206,   207,   192,   175,   136,   176,    24,    30,    46,    59,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   224,   300,   302,   303,   305,   308,   136,   304,   136,
     278,   279,   170,   127,   170,   269,   274,   275,   146,   215,
     222,   224,   175,   136,   176,   284,   299,   174,   138,   139,
     143,   170,   289,   144,   174,   150,   150,   150,   150,   169,
     150,   150,   150,   150,   150,   150,    32,    57,   114,   144,
     145,   149,   152,   167,   174,   177,   174,   146,   174,   209,
      47,   281,   299,   170,   174,   308,     1,     8,     9,    10,
      12,    14,    15,    16,    17,    25,    26,    27,    28,    37,
      38,    41,    42,    45,    48,    49,    51,    52,    55,    56,
      60,    70,    92,    93,   104,   105,   119,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   137,   152,   153,   154,
     156,   159,   160,   169,   171,   172,   174,   176,   184,   186,
     196,   197,   200,   201,   202,   203,   204,   205,   220,   221,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   233,
     234,   235,   242,   251,   252,   253,   254,   255,   256,   257,
     258,   263,   266,   300,   337,   341,   346,   348,   350,   272,
     143,   146,   149,   283,   206,   176,   266,   311,   313,   315,
     309,   136,   306,   317,   319,   321,   323,   325,   327,   329,
     331,    32,    32,   149,   167,   177,   168,   266,   136,   176,
     308,   176,   278,   168,   281,   113,   144,   146,   149,   280,
     274,   266,   299,   308,   337,   169,   113,   174,   223,   254,
     266,   220,   266,   169,   266,   150,   150,   169,   174,   150,
     130,    55,   266,   220,   150,   113,   223,   266,   266,   266,
     172,   252,    12,   308,    12,   308,   266,   343,   347,   183,
     266,   266,   266,   224,   266,   266,   266,   172,    36,   174,
     174,   266,   136,   169,   174,   174,   174,    19,   232,   213,
     118,   174,    20,    21,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   120,   121,
     122,   123,   124,   125,   144,   145,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   165,   167,   169,   176,
     213,   269,   271,   136,   308,   113,   121,   144,   282,   174,
     308,   308,   308,   308,   170,   304,   308,   214,   308,   214,
     308,   214,   136,   276,   277,   308,   279,   168,   168,   168,
     308,   223,   266,   220,    19,   169,   224,   223,   266,   113,
     144,   174,    13,   266,   220,   236,   238,   266,   240,   146,
     130,   308,   223,   266,   174,   172,   299,    50,   136,   152,
     169,   266,   338,   339,   340,   342,   343,   344,   345,   299,
     169,   339,   345,   126,   174,   176,   139,   140,   181,   187,
     170,   150,   224,   175,   220,   167,   170,   249,   266,    39,
     284,   214,   252,    18,   136,   136,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   136,   119,   266,   266,   136,   266,   266,   266,
     266,   266,   266,   266,   266,   119,   266,    21,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   136,
     264,   266,   170,   249,     7,    43,   270,   174,   144,   174,
     282,   223,   266,   151,   174,   151,   151,   151,   174,   151,
     215,   151,   215,   151,   215,   146,   151,   174,   151,   174,
     144,   280,   249,   170,   249,    18,   150,   266,   174,   220,
     170,   308,   308,   170,   308,   151,   174,    19,   252,   121,
     280,   170,   143,   174,   340,   168,   143,   168,   174,    19,
     170,   340,   176,   266,   343,   176,   266,   138,   185,   259,
     261,   222,    11,    22,    23,   198,   199,   136,   338,   143,
     170,   215,   150,   169,   266,   168,   266,   136,   146,     1,
     168,   170,    44,   270,   281,   223,   266,   174,   312,   308,
     316,   310,   307,   318,   151,   322,   151,   326,   151,   308,
     330,   276,   332,   266,   174,   170,   150,   136,   170,   151,
     151,   151,   169,   249,   266,   266,   339,   340,   136,   338,
     168,   168,   266,   168,   344,   249,   339,   176,   168,   141,
     138,   308,   214,   176,   220,   266,   168,   266,   126,   220,
     250,   267,   170,   249,   266,   265,   273,   219,   174,   151,
     320,   324,   328,   220,   243,   151,   174,   237,   239,   241,
     170,   266,   174,   340,   168,   121,   280,   168,   174,   340,
     168,   151,   215,   220,   170,   113,   266,   308,   170,   218,
     314,   308,    18,   266,   136,   266,   266,   266,   170,   266,
     168,   168,   266,   266,   266,   176,   260,   151,   199,   266,
     151,   174,   151,   150,   170,   151,   174,   349,   168,   349,
     168,   224,   262,   268,   244,   245,    18,   266,    50,   168,
     176,   224,   170,   308,   150,   170,   266,   168,   168,   151,
     247,   246,   308,   170,   151,   248,   170
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   178,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   180,   181,   181,   182,   183,
     183,   183,   184,   185,   185,   187,   186,   188,   189,   190,
     190,   190,   191,   191,   192,   192,   193,   194,   194,   195,
     195,   196,   197,   197,   198,   198,   199,   199,   199,   200,
     200,   201,   202,   203,   204,   205,   206,   206,   206,   206,
     206,   206,   207,   207,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   213,   214,   214,   214,   215,   215,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   217,   218,   219,   220,   220,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     222,   223,   223,   224,   224,   224,   225,   226,   226,   226,
     226,   227,   228,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   231,   232,   232,   233,   233,   234,   236,   237,
     235,   238,   239,   235,   240,   241,   235,   242,   242,   242,
     243,   244,   242,   245,   246,   242,   247,   248,   242,   249,
     249,   250,   250,   250,   251,   251,   251,   252,   252,   253,
     253,   253,   253,   253,   253,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   255,   255,   256,   257,   257,   258,   259,   260,   258,
     261,   262,   258,   263,   264,   265,   263,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   267,   268,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   269,   269,   270,   270,   271,   272,   272,
     273,   272,   272,   274,   274,   275,   275,   276,   276,   277,
     277,   278,   279,   279,   280,   280,   281,   281,   281,   281,
     281,   281,   282,   282,   282,   283,   283,   284,   284,   284,
     284,   284,   285,   285,   286,   286,   287,   288,   287,   289,
     289,   289,   290,   291,   291,   292,   293,   293,   294,   294,
     295,   296,   296,   298,   297,   299,   299,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   301,   301,   301,   301,   301,   301,   302,   303,
     303,   304,   304,   306,   307,   305,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   309,   310,   308,   308,   311,   312,   308,   313,
     314,   308,   315,   316,   308,   308,   317,   318,   308,   319,
     320,   308,   308,   321,   322,   308,   323,   324,   308,   308,
     325,   326,   308,   327,   328,   308,   329,   330,   308,   331,
     332,   308,   334,   333,   336,   335,   337,   337,   337,   337,
     338,   338,   338,   338,   339,   339,   340,   340,   341,   341,
     341,   341,   341,   341,   342,   342,   343,   344,   344,   345,
     345,   346,   346,   347,   347,   348,   349,   349,   350,   350
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
       3,     1,     2,     3,     1,     1,     2,     1,     5,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     6,     5,     6,     3,     0,     0,     8,
       0,     0,     9,     3,     0,     0,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     4,     3,     1,     3,     4,     3,
       4,     2,     4,     4,     6,     7,     3,     5,     0,     0,
       8,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     0,     4,     0,     1,     3,     0,     3,
       0,     7,     5,     2,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     1,     1,     2,     3,     5,
       3,     3,     1,     1,     1,     0,     1,     4,     6,     5,
       5,     4,     0,     2,     0,     1,     5,     0,     5,     0,
       3,     5,     4,     1,     2,     4,     5,     7,     0,     2,
       2,     1,     1,     0,     7,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     3,     0,     0,     6,     1,     1,     1,     1,
       4,     3,     4,     2,     3,     2,     3,     2,     2,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     8,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     0,     7,     0,     7,     1,     1,     1,     1,
       3,     3,     5,     5,     1,     3,     0,     2,     6,     5,
       7,     8,     6,     8,     1,     3,     3,     1,     1,     1,
       3,     5,     5,     1,     3,     4,     0,     3,    10,    10
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
    case 136: /* "name"  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3294 "ds_parser.cpp"
        break;

    case 181: /* character_sequence  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3300 "ds_parser.cpp"
        break;

    case 182: /* string_constant  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3306 "ds_parser.cpp"
        break;

    case 183: /* string_builder_body  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp"
        break;

    case 184: /* string_builder  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp"
        break;

    case 186: /* expr_reader  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp"
        break;

    case 190: /* require_module_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3330 "ds_parser.cpp"
        break;

    case 196: /* expression_label  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp"
        break;

    case 197: /* expression_goto  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp"
        break;

    case 199: /* expression_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp"
        break;

    case 201: /* expression_if_then_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3354 "ds_parser.cpp"
        break;

    case 202: /* expression_for_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3360 "ds_parser.cpp"
        break;

    case 203: /* expression_unsafe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3366 "ds_parser.cpp"
        break;

    case 204: /* expression_while_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3372 "ds_parser.cpp"
        break;

    case 205: /* expression_with  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3378 "ds_parser.cpp"
        break;

    case 206: /* annotation_argument_value  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3384 "ds_parser.cpp"
        break;

    case 207: /* annotation_argument_value_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3390 "ds_parser.cpp"
        break;

    case 208: /* annotation_argument  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3396 "ds_parser.cpp"
        break;

    case 209: /* annotation_argument_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3402 "ds_parser.cpp"
        break;

    case 210: /* annotation_declaration_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3408 "ds_parser.cpp"
        break;

    case 211: /* annotation_declaration  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3414 "ds_parser.cpp"
        break;

    case 212: /* annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3420 "ds_parser.cpp"
        break;

    case 213: /* optional_annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3426 "ds_parser.cpp"
        break;

    case 214: /* optional_function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3432 "ds_parser.cpp"
        break;

    case 215: /* optional_function_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3438 "ds_parser.cpp"
        break;

    case 216: /* function_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3444 "ds_parser.cpp"
        break;

    case 220: /* expression_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3450 "ds_parser.cpp"
        break;

    case 221: /* expression_any  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3456 "ds_parser.cpp"
        break;

    case 222: /* expressions  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3462 "ds_parser.cpp"
        break;

    case 223: /* expr_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3468 "ds_parser.cpp"
        break;

    case 224: /* name_in_namespace  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3474 "ds_parser.cpp"
        break;

    case 225: /* expression_delete  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3480 "ds_parser.cpp"
        break;

    case 226: /* expr_new  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3486 "ds_parser.cpp"
        break;

    case 227: /* expression_break  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3492 "ds_parser.cpp"
        break;

    case 228: /* expression_continue  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3498 "ds_parser.cpp"
        break;

    case 229: /* expression_return  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3504 "ds_parser.cpp"
        break;

    case 230: /* expression_yield  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp"
        break;

    case 231: /* expression_try_catch  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp"
        break;

    case 234: /* expression_let  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp"
        break;

    case 235: /* expr_cast  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 242: /* expr_type_info  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp"
        break;

    case 249: /* expr_list  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp"
        break;

    case 250: /* block_or_simple_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3546 "ds_parser.cpp"
        break;

    case 252: /* expr_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3552 "ds_parser.cpp"
        break;

    case 253: /* expr_numeric_const  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3558 "ds_parser.cpp"
        break;

    case 254: /* expr_assign  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3564 "ds_parser.cpp"
        break;

    case 255: /* expr_assign_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3570 "ds_parser.cpp"
        break;

    case 256: /* expr_named_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3576 "ds_parser.cpp"
        break;

    case 257: /* expr_method_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3582 "ds_parser.cpp"
        break;

    case 258: /* func_addr_expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3588 "ds_parser.cpp"
        break;

    case 263: /* expr_field  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3594 "ds_parser.cpp"
        break;

    case 266: /* expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3600 "ds_parser.cpp"
        break;

    case 269: /* optional_field_annotation  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3606 "ds_parser.cpp"
        break;

    case 271: /* structure_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3612 "ds_parser.cpp"
        break;

    case 272: /* struct_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3618 "ds_parser.cpp"
        break;

    case 274: /* function_argument_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3624 "ds_parser.cpp"
        break;

    case 275: /* function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3630 "ds_parser.cpp"
        break;

    case 276: /* tuple_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3636 "ds_parser.cpp"
        break;

    case 277: /* tuple_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3642 "ds_parser.cpp"
        break;

    case 278: /* variant_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3648 "ds_parser.cpp"
        break;

    case 279: /* variant_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3654 "ds_parser.cpp"
        break;

    case 281: /* variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3660 "ds_parser.cpp"
        break;

    case 284: /* let_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3666 "ds_parser.cpp"
        break;

    case 285: /* global_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3672 "ds_parser.cpp"
        break;

    case 289: /* enum_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3678 "ds_parser.cpp"
        break;

    case 294: /* optional_structure_parent  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3684 "ds_parser.cpp"
        break;

    case 299: /* variable_name_with_pos_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3690 "ds_parser.cpp"
        break;

    case 302: /* structure_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3696 "ds_parser.cpp"
        break;

    case 303: /* auto_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3702 "ds_parser.cpp"
        break;

    case 304: /* bitfield_bits  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3708 "ds_parser.cpp"
        break;

    case 305: /* bitfield_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3714 "ds_parser.cpp"
        break;

    case 308: /* type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3720 "ds_parser.cpp"
        break;

    case 337: /* make_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3726 "ds_parser.cpp"
        break;

    case 338: /* make_struct_fields  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3732 "ds_parser.cpp"
        break;

    case 339: /* make_struct_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3738 "ds_parser.cpp"
        break;

    case 340: /* optional_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3744 "ds_parser.cpp"
        break;

    case 341: /* make_struct_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3750 "ds_parser.cpp"
        break;

    case 342: /* make_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3756 "ds_parser.cpp"
        break;

    case 343: /* make_map_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3762 "ds_parser.cpp"
        break;

    case 344: /* make_any_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3768 "ds_parser.cpp"
        break;

    case 345: /* make_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3774 "ds_parser.cpp"
        break;

    case 346: /* make_dim_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3780 "ds_parser.cpp"
        break;

    case 347: /* make_table  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3786 "ds_parser.cpp"
        break;

    case 348: /* make_table_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3792 "ds_parser.cpp"
        break;

    case 349: /* array_comprehension_where  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3798 "ds_parser.cpp"
        break;

    case 350: /* array_comprehension  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3804 "ds_parser.cpp"
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
#line 439 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4104 "ds_parser.cpp"
    break;

  case 16:
#line 451 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4110 "ds_parser.cpp"
    break;

  case 17:
#line 452 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4116 "ds_parser.cpp"
    break;

  case 18:
#line 456 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4122 "ds_parser.cpp"
    break;

  case 19:
#line 460 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4132 "ds_parser.cpp"
    break;

  case 20:
#line 465 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4146 "ds_parser.cpp"
    break;

  case 21:
#line 474 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4156 "ds_parser.cpp"
    break;

  case 22:
#line 482 "ds_parser.ypp"
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
#line 4174 "ds_parser.cpp"
    break;

  case 23:
#line 498 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4184 "ds_parser.cpp"
    break;

  case 24:
#line 503 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4194 "ds_parser.cpp"
    break;

  case 25:
#line 511 "ds_parser.ypp"
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
#line 4221 "ds_parser.cpp"
    break;

  case 26:
#line 532 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4233 "ds_parser.cpp"
    break;

  case 27:
#line 541 "ds_parser.ypp"
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
#line 4257 "ds_parser.cpp"
    break;

  case 29:
#line 567 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4265 "ds_parser.cpp"
    break;

  case 30:
#line 570 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4276 "ds_parser.cpp"
    break;

  case 31:
#line 576 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4287 "ds_parser.cpp"
    break;

  case 32:
#line 585 "ds_parser.ypp"
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
#line 4313 "ds_parser.cpp"
    break;

  case 33:
#line 606 "ds_parser.ypp"
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
#line 4338 "ds_parser.cpp"
    break;

  case 34:
#line 629 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4344 "ds_parser.cpp"
    break;

  case 35:
#line 630 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4350 "ds_parser.cpp"
    break;

  case 39:
#line 643 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4358 "ds_parser.cpp"
    break;

  case 40:
#line 646 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4366 "ds_parser.cpp"
    break;

  case 41:
#line 652 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4374 "ds_parser.cpp"
    break;

  case 42:
#line 658 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4382 "ds_parser.cpp"
    break;

  case 43:
#line 661 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4390 "ds_parser.cpp"
    break;

  case 44:
#line 667 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4396 "ds_parser.cpp"
    break;

  case 45:
#line 668 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4402 "ds_parser.cpp"
    break;

  case 46:
#line 672 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4408 "ds_parser.cpp"
    break;

  case 47:
#line 673 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4414 "ds_parser.cpp"
    break;

  case 48:
#line 674 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4425 "ds_parser.cpp"
    break;

  case 49:
#line 683 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4431 "ds_parser.cpp"
    break;

  case 50:
#line 684 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4437 "ds_parser.cpp"
    break;

  case 51:
#line 688 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4448 "ds_parser.cpp"
    break;

  case 52:
#line 697 "ds_parser.ypp"
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
#line 4466 "ds_parser.cpp"
    break;

  case 53:
#line 713 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4476 "ds_parser.cpp"
    break;

  case 54:
#line 721 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4488 "ds_parser.cpp"
    break;

  case 55:
#line 731 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4499 "ds_parser.cpp"
    break;

  case 56:
#line 740 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4505 "ds_parser.cpp"
    break;

  case 57:
#line 741 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4511 "ds_parser.cpp"
    break;

  case 58:
#line 742 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4517 "ds_parser.cpp"
    break;

  case 59:
#line 743 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4523 "ds_parser.cpp"
    break;

  case 60:
#line 744 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4529 "ds_parser.cpp"
    break;

  case 61:
#line 745 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4535 "ds_parser.cpp"
    break;

  case 62:
#line 749 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4545 "ds_parser.cpp"
    break;

  case 63:
#line 754 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4555 "ds_parser.cpp"
    break;

  case 64:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4561 "ds_parser.cpp"
    break;

  case 65:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4567 "ds_parser.cpp"
    break;

  case 66:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4573 "ds_parser.cpp"
    break;

  case 67:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4579 "ds_parser.cpp"
    break;

  case 68:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4585 "ds_parser.cpp"
    break;

  case 69:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4591 "ds_parser.cpp"
    break;

  case 70:
#line 768 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4597 "ds_parser.cpp"
    break;

  case 71:
#line 769 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4605 "ds_parser.cpp"
    break;

  case 72:
#line 775 "ds_parser.ypp"
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
#line 4623 "ds_parser.cpp"
    break;

  case 73:
#line 788 "ds_parser.ypp"
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
#line 4641 "ds_parser.cpp"
    break;

  case 74:
#line 804 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4647 "ds_parser.cpp"
    break;

  case 75:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4653 "ds_parser.cpp"
    break;

  case 76:
#line 809 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4665 "ds_parser.cpp"
    break;

  case 77:
#line 816 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4679 "ds_parser.cpp"
    break;

  case 78:
#line 828 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4688 "ds_parser.cpp"
    break;

  case 79:
#line 832 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4697 "ds_parser.cpp"
    break;

  case 80:
#line 839 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4703 "ds_parser.cpp"
    break;

  case 81:
#line 840 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4709 "ds_parser.cpp"
    break;

  case 82:
#line 844 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4715 "ds_parser.cpp"
    break;

  case 83:
#line 845 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4721 "ds_parser.cpp"
    break;

  case 84:
#line 846 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4727 "ds_parser.cpp"
    break;

  case 85:
#line 850 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4737 "ds_parser.cpp"
    break;

  case 86:
#line 855 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4746 "ds_parser.cpp"
    break;

  case 87:
#line 862 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4755 "ds_parser.cpp"
    break;

  case 88:
#line 866 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4761 "ds_parser.cpp"
    break;

  case 89:
#line 867 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4767 "ds_parser.cpp"
    break;

  case 90:
#line 868 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4773 "ds_parser.cpp"
    break;

  case 91:
#line 869 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4779 "ds_parser.cpp"
    break;

  case 92:
#line 870 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4785 "ds_parser.cpp"
    break;

  case 93:
#line 871 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4791 "ds_parser.cpp"
    break;

  case 94:
#line 872 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4797 "ds_parser.cpp"
    break;

  case 95:
#line 873 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4803 "ds_parser.cpp"
    break;

  case 96:
#line 874 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4809 "ds_parser.cpp"
    break;

  case 97:
#line 875 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4815 "ds_parser.cpp"
    break;

  case 98:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4821 "ds_parser.cpp"
    break;

  case 99:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4827 "ds_parser.cpp"
    break;

  case 100:
#line 878 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4833 "ds_parser.cpp"
    break;

  case 101:
#line 879 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4839 "ds_parser.cpp"
    break;

  case 102:
#line 880 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4845 "ds_parser.cpp"
    break;

  case 103:
#line 881 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4851 "ds_parser.cpp"
    break;

  case 104:
#line 882 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4857 "ds_parser.cpp"
    break;

  case 105:
#line 883 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4863 "ds_parser.cpp"
    break;

  case 106:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4869 "ds_parser.cpp"
    break;

  case 107:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4875 "ds_parser.cpp"
    break;

  case 108:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4881 "ds_parser.cpp"
    break;

  case 109:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4887 "ds_parser.cpp"
    break;

  case 110:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4893 "ds_parser.cpp"
    break;

  case 111:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4899 "ds_parser.cpp"
    break;

  case 112:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4905 "ds_parser.cpp"
    break;

  case 113:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4911 "ds_parser.cpp"
    break;

  case 114:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4917 "ds_parser.cpp"
    break;

  case 115:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4923 "ds_parser.cpp"
    break;

  case 116:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4929 "ds_parser.cpp"
    break;

  case 117:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4935 "ds_parser.cpp"
    break;

  case 118:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4941 "ds_parser.cpp"
    break;

  case 119:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4947 "ds_parser.cpp"
    break;

  case 120:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4953 "ds_parser.cpp"
    break;

  case 121:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4959 "ds_parser.cpp"
    break;

  case 122:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4965 "ds_parser.cpp"
    break;

  case 123:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4971 "ds_parser.cpp"
    break;

  case 124:
#line 905 "ds_parser.ypp"
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
#line 4994 "ds_parser.cpp"
    break;

  case 125:
#line 926 "ds_parser.ypp"
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
#line 5034 "ds_parser.cpp"
    break;

  case 126:
#line 964 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5043 "ds_parser.cpp"
    break;

  case 127:
#line 971 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5052 "ds_parser.cpp"
    break;

  case 128:
#line 975 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5065 "ds_parser.cpp"
    break;

  case 129:
#line 986 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5071 "ds_parser.cpp"
    break;

  case 130:
#line 987 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5077 "ds_parser.cpp"
    break;

  case 131:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5083 "ds_parser.cpp"
    break;

  case 132:
#line 989 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5089 "ds_parser.cpp"
    break;

  case 133:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5095 "ds_parser.cpp"
    break;

  case 134:
#line 991 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5101 "ds_parser.cpp"
    break;

  case 135:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5107 "ds_parser.cpp"
    break;

  case 136:
#line 993 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5113 "ds_parser.cpp"
    break;

  case 137:
#line 994 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5119 "ds_parser.cpp"
    break;

  case 138:
#line 995 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5125 "ds_parser.cpp"
    break;

  case 139:
#line 996 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5131 "ds_parser.cpp"
    break;

  case 140:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5137 "ds_parser.cpp"
    break;

  case 141:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5143 "ds_parser.cpp"
    break;

  case 142:
#line 999 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5149 "ds_parser.cpp"
    break;

  case 143:
#line 1000 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5155 "ds_parser.cpp"
    break;

  case 144:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5161 "ds_parser.cpp"
    break;

  case 145:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5167 "ds_parser.cpp"
    break;

  case 146:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5173 "ds_parser.cpp"
    break;

  case 147:
#line 1004 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5179 "ds_parser.cpp"
    break;

  case 148:
#line 1008 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5189 "ds_parser.cpp"
    break;

  case 149:
#line 1013 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5200 "ds_parser.cpp"
    break;

  case 150:
#line 1019 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5208 "ds_parser.cpp"
    break;

  case 151:
#line 1025 "ds_parser.ypp"
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
#line 5234 "ds_parser.cpp"
    break;

  case 152:
#line 1046 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5242 "ds_parser.cpp"
    break;

  case 153:
#line 1052 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5248 "ds_parser.cpp"
    break;

  case 154:
#line 1053 "ds_parser.ypp"
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
#line 5264 "ds_parser.cpp"
    break;

  case 155:
#line 1064 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5270 "ds_parser.cpp"
    break;

  case 156:
#line 1068 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5278 "ds_parser.cpp"
    break;

  case 157:
#line 1074 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5286 "ds_parser.cpp"
    break;

  case 158:
#line 1077 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5294 "ds_parser.cpp"
    break;

  case 159:
#line 1080 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5303 "ds_parser.cpp"
    break;

  case 160:
#line 1084 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5311 "ds_parser.cpp"
    break;

  case 161:
#line 1090 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5317 "ds_parser.cpp"
    break;

  case 162:
#line 1094 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5323 "ds_parser.cpp"
    break;

  case 163:
#line 1098 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5331 "ds_parser.cpp"
    break;

  case 164:
#line 1101 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5339 "ds_parser.cpp"
    break;

  case 165:
#line 1104 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5349 "ds_parser.cpp"
    break;

  case 166:
#line 1109 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5357 "ds_parser.cpp"
    break;

  case 167:
#line 1112 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5367 "ds_parser.cpp"
    break;

  case 168:
#line 1120 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5375 "ds_parser.cpp"
    break;

  case 169:
#line 1123 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5385 "ds_parser.cpp"
    break;

  case 170:
#line 1128 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5393 "ds_parser.cpp"
    break;

  case 171:
#line 1131 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5403 "ds_parser.cpp"
    break;

  case 172:
#line 1139 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5411 "ds_parser.cpp"
    break;

  case 173:
#line 1145 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5417 "ds_parser.cpp"
    break;

  case 174:
#line 1146 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5423 "ds_parser.cpp"
    break;

  case 175:
#line 1150 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5429 "ds_parser.cpp"
    break;

  case 176:
#line 1151 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5435 "ds_parser.cpp"
    break;

  case 177:
#line 1155 "ds_parser.ypp"
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
#line 5472 "ds_parser.cpp"
    break;

  case 178:
#line 1190 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5478 "ds_parser.cpp"
    break;

  case 179:
#line 1190 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5484 "ds_parser.cpp"
    break;

  case 180:
#line 1190 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5492 "ds_parser.cpp"
    break;

  case 181:
#line 1193 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5498 "ds_parser.cpp"
    break;

  case 182:
#line 1193 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5504 "ds_parser.cpp"
    break;

  case 183:
#line 1193 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5514 "ds_parser.cpp"
    break;

  case 184:
#line 1198 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5520 "ds_parser.cpp"
    break;

  case 185:
#line 1198 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5526 "ds_parser.cpp"
    break;

  case 186:
#line 1198 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5536 "ds_parser.cpp"
    break;

  case 187:
#line 1206 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5545 "ds_parser.cpp"
    break;

  case 188:
#line 1210 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5555 "ds_parser.cpp"
    break;

  case 189:
#line 1215 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5566 "ds_parser.cpp"
    break;

  case 190:
#line 1221 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5572 "ds_parser.cpp"
    break;

  case 191:
#line 1221 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5578 "ds_parser.cpp"
    break;

  case 192:
#line 1221 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5587 "ds_parser.cpp"
    break;

  case 193:
#line 1225 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5593 "ds_parser.cpp"
    break;

  case 194:
#line 1225 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5599 "ds_parser.cpp"
    break;

  case 195:
#line 1225 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5609 "ds_parser.cpp"
    break;

  case 196:
#line 1230 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5615 "ds_parser.cpp"
    break;

  case 197:
#line 1230 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5621 "ds_parser.cpp"
    break;

  case 198:
#line 1230 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5632 "ds_parser.cpp"
    break;

  case 199:
#line 1239 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5640 "ds_parser.cpp"
    break;

  case 200:
#line 1242 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5648 "ds_parser.cpp"
    break;

  case 201:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5654 "ds_parser.cpp"
    break;

  case 202:
#line 1249 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5666 "ds_parser.cpp"
    break;

  case 203:
#line 1256 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5679 "ds_parser.cpp"
    break;

  case 204:
#line 1267 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5685 "ds_parser.cpp"
    break;

  case 205:
#line 1268 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5691 "ds_parser.cpp"
    break;

  case 206:
#line 1269 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5697 "ds_parser.cpp"
    break;

  case 207:
#line 1273 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5707 "ds_parser.cpp"
    break;

  case 208:
#line 1279 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].i)==1, (yyvsp[-4].i)==2);
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
        if ( (yyvsp[-3].faList) ) {
            for ( auto pA : *(yyvsp[-3].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(closure, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't apply annotation\n" + err, tokAt((yylsp[-3])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("blocks are only allowed function annotations", tokAt((yylsp[-3])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-3].faList) );
            delete (yyvsp[-3].faList);
        }
    }
#line 5763 "ds_parser.cpp"
    break;

  case 209:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5769 "ds_parser.cpp"
    break;

  case 210:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5775 "ds_parser.cpp"
    break;

  case 211:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5781 "ds_parser.cpp"
    break;

  case 212:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5787 "ds_parser.cpp"
    break;

  case 213:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5793 "ds_parser.cpp"
    break;

  case 214:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5799 "ds_parser.cpp"
    break;

  case 215:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5805 "ds_parser.cpp"
    break;

  case 216:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5811 "ds_parser.cpp"
    break;

  case 217:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5817 "ds_parser.cpp"
    break;

  case 218:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5823 "ds_parser.cpp"
    break;

  case 219:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5829 "ds_parser.cpp"
    break;

  case 220:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5835 "ds_parser.cpp"
    break;

  case 221:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5841 "ds_parser.cpp"
    break;

  case 222:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5847 "ds_parser.cpp"
    break;

  case 223:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5853 "ds_parser.cpp"
    break;

  case 224:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5859 "ds_parser.cpp"
    break;

  case 225:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5865 "ds_parser.cpp"
    break;

  case 226:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5871 "ds_parser.cpp"
    break;

  case 227:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5877 "ds_parser.cpp"
    break;

  case 228:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5883 "ds_parser.cpp"
    break;

  case 229:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5889 "ds_parser.cpp"
    break;

  case 230:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5895 "ds_parser.cpp"
    break;

  case 231:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5901 "ds_parser.cpp"
    break;

  case 232:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5907 "ds_parser.cpp"
    break;

  case 233:
#line 1366 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5919 "ds_parser.cpp"
    break;

  case 234:
#line 1377 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5929 "ds_parser.cpp"
    break;

  case 235:
#line 1382 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5941 "ds_parser.cpp"
    break;

  case 236:
#line 1392 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5950 "ds_parser.cpp"
    break;

  case 237:
#line 1396 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5956 "ds_parser.cpp"
    break;

  case 238:
#line 1396 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5962 "ds_parser.cpp"
    break;

  case 239:
#line 1396 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 5974 "ds_parser.cpp"
    break;

  case 240:
#line 1403 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5980 "ds_parser.cpp"
    break;

  case 241:
#line 1403 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5986 "ds_parser.cpp"
    break;

  case 242:
#line 1403 "ds_parser.ypp"
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
#line 6002 "ds_parser.cpp"
    break;

  case 243:
#line 1417 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6011 "ds_parser.cpp"
    break;

  case 244:
#line 1421 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6017 "ds_parser.cpp"
    break;

  case 245:
#line 1421 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6023 "ds_parser.cpp"
    break;

  case 246:
#line 1421 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6033 "ds_parser.cpp"
    break;

  case 247:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6039 "ds_parser.cpp"
    break;

  case 248:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6045 "ds_parser.cpp"
    break;

  case 249:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6051 "ds_parser.cpp"
    break;

  case 250:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6057 "ds_parser.cpp"
    break;

  case 251:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6063 "ds_parser.cpp"
    break;

  case 252:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6069 "ds_parser.cpp"
    break;

  case 253:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6075 "ds_parser.cpp"
    break;

  case 254:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6081 "ds_parser.cpp"
    break;

  case 255:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6087 "ds_parser.cpp"
    break;

  case 256:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6093 "ds_parser.cpp"
    break;

  case 257:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6099 "ds_parser.cpp"
    break;

  case 258:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6105 "ds_parser.cpp"
    break;

  case 259:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6111 "ds_parser.cpp"
    break;

  case 260:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6117 "ds_parser.cpp"
    break;

  case 261:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6123 "ds_parser.cpp"
    break;

  case 262:
#line 1444 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6129 "ds_parser.cpp"
    break;

  case 263:
#line 1445 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6135 "ds_parser.cpp"
    break;

  case 264:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6141 "ds_parser.cpp"
    break;

  case 265:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6147 "ds_parser.cpp"
    break;

  case 266:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6153 "ds_parser.cpp"
    break;

  case 267:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6159 "ds_parser.cpp"
    break;

  case 268:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6165 "ds_parser.cpp"
    break;

  case 269:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6171 "ds_parser.cpp"
    break;

  case 270:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6177 "ds_parser.cpp"
    break;

  case 271:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6183 "ds_parser.cpp"
    break;

  case 272:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6189 "ds_parser.cpp"
    break;

  case 273:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6195 "ds_parser.cpp"
    break;

  case 274:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6201 "ds_parser.cpp"
    break;

  case 275:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6207 "ds_parser.cpp"
    break;

  case 276:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6213 "ds_parser.cpp"
    break;

  case 277:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6219 "ds_parser.cpp"
    break;

  case 278:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6225 "ds_parser.cpp"
    break;

  case 279:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6231 "ds_parser.cpp"
    break;

  case 280:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6237 "ds_parser.cpp"
    break;

  case 281:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6243 "ds_parser.cpp"
    break;

  case 282:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6249 "ds_parser.cpp"
    break;

  case 283:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6255 "ds_parser.cpp"
    break;

  case 284:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6261 "ds_parser.cpp"
    break;

  case 285:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6267 "ds_parser.cpp"
    break;

  case 286:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6273 "ds_parser.cpp"
    break;

  case 287:
#line 1469 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6282 "ds_parser.cpp"
    break;

  case 288:
#line 1473 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6291 "ds_parser.cpp"
    break;

  case 289:
#line 1477 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6299 "ds_parser.cpp"
    break;

  case 290:
#line 1480 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6307 "ds_parser.cpp"
    break;

  case 291:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp"
    break;

  case 292:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6319 "ds_parser.cpp"
    break;

  case 293:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6325 "ds_parser.cpp"
    break;

  case 294:
#line 1486 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6335 "ds_parser.cpp"
    break;

  case 295:
#line 1491 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6345 "ds_parser.cpp"
    break;

  case 296:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6351 "ds_parser.cpp"
    break;

  case 297:
#line 1497 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6359 "ds_parser.cpp"
    break;

  case 298:
#line 1500 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6365 "ds_parser.cpp"
    break;

  case 299:
#line 1500 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6371 "ds_parser.cpp"
    break;

  case 300:
#line 1500 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6379 "ds_parser.cpp"
    break;

  case 301:
#line 1503 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6388 "ds_parser.cpp"
    break;

  case 302:
#line 1507 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6397 "ds_parser.cpp"
    break;

  case 303:
#line 1511 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6406 "ds_parser.cpp"
    break;

  case 304:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6412 "ds_parser.cpp"
    break;

  case 305:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6418 "ds_parser.cpp"
    break;

  case 306:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6424 "ds_parser.cpp"
    break;

  case 307:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6430 "ds_parser.cpp"
    break;

  case 308:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6436 "ds_parser.cpp"
    break;

  case 309:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6442 "ds_parser.cpp"
    break;

  case 310:
#line 1521 "ds_parser.ypp"
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
#line 6457 "ds_parser.cpp"
    break;

  case 311:
#line 1531 "ds_parser.ypp"
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
#line 6472 "ds_parser.cpp"
    break;

  case 312:
#line 1541 "ds_parser.ypp"
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
#line 6529 "ds_parser.cpp"
    break;

  case 313:
#line 1596 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6535 "ds_parser.cpp"
    break;

  case 314:
#line 1597 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6541 "ds_parser.cpp"
    break;

  case 315:
#line 1601 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6547 "ds_parser.cpp"
    break;

  case 316:
#line 1602 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6553 "ds_parser.cpp"
    break;

  case 317:
#line 1606 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6563 "ds_parser.cpp"
    break;

  case 318:
#line 1614 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6571 "ds_parser.cpp"
    break;

  case 319:
#line 1617 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6580 "ds_parser.cpp"
    break;

  case 320:
#line 1622 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6586 "ds_parser.cpp"
    break;

  case 321:
#line 1622 "ds_parser.ypp"
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
#line 6635 "ds_parser.cpp"
    break;

  case 322:
#line 1666 "ds_parser.ypp"
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
                modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure);
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
                }
                modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure);
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
#line 6693 "ds_parser.cpp"
    break;

  case 323:
#line 1722 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6703 "ds_parser.cpp"
    break;

  case 324:
#line 1727 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6713 "ds_parser.cpp"
    break;

  case 325:
#line 1735 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6719 "ds_parser.cpp"
    break;

  case 326:
#line 1736 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6725 "ds_parser.cpp"
    break;

  case 327:
#line 1740 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6733 "ds_parser.cpp"
    break;

  case 328:
#line 1743 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6744 "ds_parser.cpp"
    break;

  case 329:
#line 1752 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6750 "ds_parser.cpp"
    break;

  case 330:
#line 1753 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6756 "ds_parser.cpp"
    break;

  case 331:
#line 1757 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6767 "ds_parser.cpp"
    break;

  case 332:
#line 1766 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6773 "ds_parser.cpp"
    break;

  case 333:
#line 1767 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6779 "ds_parser.cpp"
    break;

  case 334:
#line 1772 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6785 "ds_parser.cpp"
    break;

  case 335:
#line 1773 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6791 "ds_parser.cpp"
    break;

  case 336:
#line 1777 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6802 "ds_parser.cpp"
    break;

  case 337:
#line 1783 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6813 "ds_parser.cpp"
    break;

  case 338:
#line 1789 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6821 "ds_parser.cpp"
    break;

  case 339:
#line 1792 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6830 "ds_parser.cpp"
    break;

  case 340:
#line 1796 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6841 "ds_parser.cpp"
    break;

  case 341:
#line 1802 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6852 "ds_parser.cpp"
    break;

  case 342:
#line 1811 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6858 "ds_parser.cpp"
    break;

  case 343:
#line 1812 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6864 "ds_parser.cpp"
    break;

  case 344:
#line 1813 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6870 "ds_parser.cpp"
    break;

  case 345:
#line 1817 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6876 "ds_parser.cpp"
    break;

  case 346:
#line 1818 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6882 "ds_parser.cpp"
    break;

  case 347:
#line 1822 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6890 "ds_parser.cpp"
    break;

  case 348:
#line 1825 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6900 "ds_parser.cpp"
    break;

  case 349:
#line 1830 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6910 "ds_parser.cpp"
    break;

  case 350:
#line 1835 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6923 "ds_parser.cpp"
    break;

  case 351:
#line 1843 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6936 "ds_parser.cpp"
    break;

  case 352:
#line 1854 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6944 "ds_parser.cpp"
    break;

  case 353:
#line 1857 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6953 "ds_parser.cpp"
    break;

  case 354:
#line 1864 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6959 "ds_parser.cpp"
    break;

  case 355:
#line 1865 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6965 "ds_parser.cpp"
    break;

  case 356:
#line 1869 "ds_parser.ypp"
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
#line 6997 "ds_parser.cpp"
    break;

  case 357:
#line 1896 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7006 "ds_parser.cpp"
    break;

  case 358:
#line 1899 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7017 "ds_parser.cpp"
    break;

  case 359:
#line 1908 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7025 "ds_parser.cpp"
    break;

  case 360:
#line 1911 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7039 "ds_parser.cpp"
    break;

  case 361:
#line 1920 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7053 "ds_parser.cpp"
    break;

  case 362:
#line 1933 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7067 "ds_parser.cpp"
    break;

  case 366:
#line 1954 "ds_parser.ypp"
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
#line 7083 "ds_parser.cpp"
    break;

  case 367:
#line 1965 "ds_parser.ypp"
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
#line 7100 "ds_parser.cpp"
    break;

  case 368:
#line 1980 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7106 "ds_parser.cpp"
    break;

  case 369:
#line 1981 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7112 "ds_parser.cpp"
    break;

  case 370:
#line 1985 "ds_parser.ypp"
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
#line 7151 "ds_parser.cpp"
    break;

  case 371:
#line 2022 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7157 "ds_parser.cpp"
    break;

  case 372:
#line 2023 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7163 "ds_parser.cpp"
    break;

  case 373:
#line 2027 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7169 "ds_parser.cpp"
    break;

  case 374:
#line 2027 "ds_parser.ypp"
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
#line 7255 "ds_parser.cpp"
    break;

  case 375:
#line 2111 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7267 "ds_parser.cpp"
    break;

  case 376:
#line 2118 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7278 "ds_parser.cpp"
    break;

  case 377:
#line 2127 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7284 "ds_parser.cpp"
    break;

  case 378:
#line 2128 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7290 "ds_parser.cpp"
    break;

  case 379:
#line 2129 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7296 "ds_parser.cpp"
    break;

  case 380:
#line 2130 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7302 "ds_parser.cpp"
    break;

  case 381:
#line 2131 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7308 "ds_parser.cpp"
    break;

  case 382:
#line 2132 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7314 "ds_parser.cpp"
    break;

  case 383:
#line 2133 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7320 "ds_parser.cpp"
    break;

  case 384:
#line 2134 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7326 "ds_parser.cpp"
    break;

  case 385:
#line 2135 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7332 "ds_parser.cpp"
    break;

  case 386:
#line 2136 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7338 "ds_parser.cpp"
    break;

  case 387:
#line 2137 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7344 "ds_parser.cpp"
    break;

  case 388:
#line 2138 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7350 "ds_parser.cpp"
    break;

  case 389:
#line 2139 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7356 "ds_parser.cpp"
    break;

  case 390:
#line 2140 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7362 "ds_parser.cpp"
    break;

  case 391:
#line 2141 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7368 "ds_parser.cpp"
    break;

  case 392:
#line 2142 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7374 "ds_parser.cpp"
    break;

  case 393:
#line 2143 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7380 "ds_parser.cpp"
    break;

  case 394:
#line 2144 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7386 "ds_parser.cpp"
    break;

  case 395:
#line 2145 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7392 "ds_parser.cpp"
    break;

  case 396:
#line 2146 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7398 "ds_parser.cpp"
    break;

  case 397:
#line 2147 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7404 "ds_parser.cpp"
    break;

  case 398:
#line 2148 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7410 "ds_parser.cpp"
    break;

  case 399:
#line 2149 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7416 "ds_parser.cpp"
    break;

  case 400:
#line 2150 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7422 "ds_parser.cpp"
    break;

  case 401:
#line 2151 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7428 "ds_parser.cpp"
    break;

  case 402:
#line 2155 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7434 "ds_parser.cpp"
    break;

  case 403:
#line 2156 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7440 "ds_parser.cpp"
    break;

  case 404:
#line 2157 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7446 "ds_parser.cpp"
    break;

  case 405:
#line 2158 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7452 "ds_parser.cpp"
    break;

  case 406:
#line 2159 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7458 "ds_parser.cpp"
    break;

  case 407:
#line 2160 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7464 "ds_parser.cpp"
    break;

  case 408:
#line 2164 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7477 "ds_parser.cpp"
    break;

  case 409:
#line 2175 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7486 "ds_parser.cpp"
    break;

  case 410:
#line 2179 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7498 "ds_parser.cpp"
    break;

  case 411:
#line 2189 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7510 "ds_parser.cpp"
    break;

  case 412:
#line 2196 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7521 "ds_parser.cpp"
    break;

  case 413:
#line 2205 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7527 "ds_parser.cpp"
    break;

  case 414:
#line 2205 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7533 "ds_parser.cpp"
    break;

  case 415:
#line 2205 "ds_parser.ypp"
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
#line 7548 "ds_parser.cpp"
    break;

  case 416:
#line 2218 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7554 "ds_parser.cpp"
    break;

  case 417:
#line 2219 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7560 "ds_parser.cpp"
    break;

  case 418:
#line 2220 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7566 "ds_parser.cpp"
    break;

  case 419:
#line 2221 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7572 "ds_parser.cpp"
    break;

  case 420:
#line 2222 "ds_parser.ypp"
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
#line 7591 "ds_parser.cpp"
    break;

  case 421:
#line 2236 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7602 "ds_parser.cpp"
    break;

  case 422:
#line 2242 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7611 "ds_parser.cpp"
    break;

  case 423:
#line 2246 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7621 "ds_parser.cpp"
    break;

  case 424:
#line 2251 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7631 "ds_parser.cpp"
    break;

  case 425:
#line 2256 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7641 "ds_parser.cpp"
    break;

  case 426:
#line 2261 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7651 "ds_parser.cpp"
    break;

  case 427:
#line 2266 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7660 "ds_parser.cpp"
    break;

  case 428:
#line 2270 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7669 "ds_parser.cpp"
    break;

  case 429:
#line 2274 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7679 "ds_parser.cpp"
    break;

  case 430:
#line 2279 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7688 "ds_parser.cpp"
    break;

  case 431:
#line 2283 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7698 "ds_parser.cpp"
    break;

  case 432:
#line 2288 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7704 "ds_parser.cpp"
    break;

  case 433:
#line 2288 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7710 "ds_parser.cpp"
    break;

  case 434:
#line 2288 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7721 "ds_parser.cpp"
    break;

  case 435:
#line 2294 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7733 "ds_parser.cpp"
    break;

  case 436:
#line 2301 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7739 "ds_parser.cpp"
    break;

  case 437:
#line 2301 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7745 "ds_parser.cpp"
    break;

  case 438:
#line 2301 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7755 "ds_parser.cpp"
    break;

  case 439:
#line 2306 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7761 "ds_parser.cpp"
    break;

  case 440:
#line 2306 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7767 "ds_parser.cpp"
    break;

  case 441:
#line 2306 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7778 "ds_parser.cpp"
    break;

  case 442:
#line 2312 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7784 "ds_parser.cpp"
    break;

  case 443:
#line 2312 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7790 "ds_parser.cpp"
    break;

  case 444:
#line 2312 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7800 "ds_parser.cpp"
    break;

  case 445:
#line 2317 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7809 "ds_parser.cpp"
    break;

  case 446:
#line 2321 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7815 "ds_parser.cpp"
    break;

  case 447:
#line 2321 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7821 "ds_parser.cpp"
    break;

  case 448:
#line 2321 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7831 "ds_parser.cpp"
    break;

  case 449:
#line 2326 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7837 "ds_parser.cpp"
    break;

  case 450:
#line 2326 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7843 "ds_parser.cpp"
    break;

  case 451:
#line 2326 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7857 "ds_parser.cpp"
    break;

  case 452:
#line 2335 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7866 "ds_parser.cpp"
    break;

  case 453:
#line 2339 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7872 "ds_parser.cpp"
    break;

  case 454:
#line 2339 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7878 "ds_parser.cpp"
    break;

  case 455:
#line 2339 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7888 "ds_parser.cpp"
    break;

  case 456:
#line 2344 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7894 "ds_parser.cpp"
    break;

  case 457:
#line 2344 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7900 "ds_parser.cpp"
    break;

  case 458:
#line 2344 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7914 "ds_parser.cpp"
    break;

  case 459:
#line 2353 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7923 "ds_parser.cpp"
    break;

  case 460:
#line 2357 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7929 "ds_parser.cpp"
    break;

  case 461:
#line 2357 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7935 "ds_parser.cpp"
    break;

  case 462:
#line 2357 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7945 "ds_parser.cpp"
    break;

  case 463:
#line 2362 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7951 "ds_parser.cpp"
    break;

  case 464:
#line 2362 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7957 "ds_parser.cpp"
    break;

  case 465:
#line 2362 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7971 "ds_parser.cpp"
    break;

  case 466:
#line 2371 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7977 "ds_parser.cpp"
    break;

  case 467:
#line 2371 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7983 "ds_parser.cpp"
    break;

  case 468:
#line 2371 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7994 "ds_parser.cpp"
    break;

  case 469:
#line 2377 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8000 "ds_parser.cpp"
    break;

  case 470:
#line 2377 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8006 "ds_parser.cpp"
    break;

  case 471:
#line 2377 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8017 "ds_parser.cpp"
    break;

  case 472:
#line 2386 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8023 "ds_parser.cpp"
    break;

  case 473:
#line 2386 "ds_parser.ypp"
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
#line 8040 "ds_parser.cpp"
    break;

  case 474:
#line 2401 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8046 "ds_parser.cpp"
    break;

  case 475:
#line 2401 "ds_parser.ypp"
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
#line 8067 "ds_parser.cpp"
    break;

  case 476:
#line 2421 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8073 "ds_parser.cpp"
    break;

  case 477:
#line 2422 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8079 "ds_parser.cpp"
    break;

  case 478:
#line 2423 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8085 "ds_parser.cpp"
    break;

  case 479:
#line 2424 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8091 "ds_parser.cpp"
    break;

  case 480:
#line 2428 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8103 "ds_parser.cpp"
    break;

  case 481:
#line 2435 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8115 "ds_parser.cpp"
    break;

  case 482:
#line 2442 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8126 "ds_parser.cpp"
    break;

  case 483:
#line 2448 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8137 "ds_parser.cpp"
    break;

  case 484:
#line 2457 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8147 "ds_parser.cpp"
    break;

  case 485:
#line 2462 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8156 "ds_parser.cpp"
    break;

  case 486:
#line 2469 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8162 "ds_parser.cpp"
    break;

  case 487:
#line 2470 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8168 "ds_parser.cpp"
    break;

  case 488:
#line 2474 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8179 "ds_parser.cpp"
    break;

  case 489:
#line 2480 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8191 "ds_parser.cpp"
    break;

  case 490:
#line 2487 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8204 "ds_parser.cpp"
    break;

  case 491:
#line 2495 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8216 "ds_parser.cpp"
    break;

  case 492:
#line 2502 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8229 "ds_parser.cpp"
    break;

  case 493:
#line 2510 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8243 "ds_parser.cpp"
    break;

  case 494:
#line 2522 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8251 "ds_parser.cpp"
    break;

  case 495:
#line 2525 "ds_parser.ypp"
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
#line 8267 "ds_parser.cpp"
    break;

  case 496:
#line 2539 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8278 "ds_parser.cpp"
    break;

  case 497:
#line 2548 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8284 "ds_parser.cpp"
    break;

  case 498:
#line 2549 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8290 "ds_parser.cpp"
    break;

  case 499:
#line 2553 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8300 "ds_parser.cpp"
    break;

  case 500:
#line 2558 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8309 "ds_parser.cpp"
    break;

  case 501:
#line 2565 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8319 "ds_parser.cpp"
    break;

  case 502:
#line 2570 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8332 "ds_parser.cpp"
    break;

  case 503:
#line 2581 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8342 "ds_parser.cpp"
    break;

  case 504:
#line 2586 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8351 "ds_parser.cpp"
    break;

  case 505:
#line 2593 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8365 "ds_parser.cpp"
    break;

  case 506:
#line 2605 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8371 "ds_parser.cpp"
    break;

  case 507:
#line 2606 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8377 "ds_parser.cpp"
    break;

  case 508:
#line 2610 "ds_parser.ypp"
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
#line 8400 "ds_parser.cpp"
    break;

  case 509:
#line 2628 "ds_parser.ypp"
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
#line 8423 "ds_parser.cpp"
    break;


#line 8427 "ds_parser.cpp"

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
#line 2648 "ds_parser.ypp"


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
