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
#line 59 "ds_parser.ypp"

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

#line 102 "ds_parser.cpp"

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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = false );
    void runFunctionAnnotations ( Function * func, AnnotationList * annL, const LineInfo & at );

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

    extern Structure * g_thisStructure;

#line 201 "ds_parser.cpp"

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
    DAS_SHARED = 312,
    DAS_SMART_PTR = 313,
    DAS_TBOOL = 314,
    DAS_TVOID = 315,
    DAS_TSTRING = 316,
    DAS_TAUTO = 317,
    DAS_TINT = 318,
    DAS_TINT2 = 319,
    DAS_TINT3 = 320,
    DAS_TINT4 = 321,
    DAS_TUINT = 322,
    DAS_TBITFIELD = 323,
    DAS_TUINT2 = 324,
    DAS_TUINT3 = 325,
    DAS_TUINT4 = 326,
    DAS_TFLOAT = 327,
    DAS_TFLOAT2 = 328,
    DAS_TFLOAT3 = 329,
    DAS_TFLOAT4 = 330,
    DAS_TRANGE = 331,
    DAS_TURANGE = 332,
    DAS_TBLOCK = 333,
    DAS_TINT64 = 334,
    DAS_TUINT64 = 335,
    DAS_TDOUBLE = 336,
    DAS_TFUNCTION = 337,
    DAS_TLAMBDA = 338,
    DAS_TINT8 = 339,
    DAS_TUINT8 = 340,
    DAS_TINT16 = 341,
    DAS_TUINT16 = 342,
    DAS_TTUPLE = 343,
    DAS_TVARIANT = 344,
    DAS_GENERATOR = 345,
    DAS_YIELD = 346,
    ADDEQU = 347,
    SUBEQU = 348,
    DIVEQU = 349,
    MULEQU = 350,
    MODEQU = 351,
    ANDEQU = 352,
    OREQU = 353,
    XOREQU = 354,
    SHL = 355,
    SHR = 356,
    ADDADD = 357,
    SUBSUB = 358,
    LEEQU = 359,
    SHLEQU = 360,
    SHREQU = 361,
    GREQU = 362,
    EQUEQU = 363,
    NOTEQU = 364,
    RARROW = 365,
    LARROW = 366,
    QQ = 367,
    QDOT = 368,
    QBRA = 369,
    LPIPE = 370,
    LBPIPE = 371,
    LAPIPE = 372,
    RPIPE = 373,
    CLONEEQU = 374,
    ROTL = 375,
    ROTR = 376,
    ROTLEQU = 377,
    ROTREQU = 378,
    MAPTO = 379,
    BRABRAB = 380,
    BRACBRB = 381,
    CBRCBRB = 382,
    INTEGER = 383,
    LONG_INTEGER = 384,
    UNSIGNED_INTEGER = 385,
    UNSIGNED_LONG_INTEGER = 386,
    FLOAT = 387,
    DOUBLE = 388,
    NAME = 389,
    BEGIN_STRING = 390,
    STRING_CHARACTER = 391,
    END_STRING = 392,
    BEGIN_STRING_EXPR = 393,
    END_STRING_EXPR = 394,
    UNARY_MINUS = 395,
    UNARY_PLUS = 396,
    PRE_INC = 397,
    PRE_DEC = 398,
    POST_INC = 399,
    POST_DEC = 400,
    DEREF = 401,
    COLCOL = 402
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 86 "ds_parser.ypp"

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

#line 387 "ds_parser.cpp"

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
#define YYLAST   6862

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  163
/* YYNRULES -- Number of rules.  */
#define YYNRULES  489
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  873

#define YYUNDEFTOK  2
#define YYMAXUTOK   402

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
       2,     2,     2,   157,     2,   174,   168,   153,   146,     2,
     166,   167,   151,   150,   140,   149,   162,   152,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   143,   171,
     147,   141,   148,   142,   169,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   164,     2,   165,   145,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   172,   144,   173,   156,     2,     2,     2,
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
     135,   136,   137,   138,   139,   154,   155,   158,   159,   160,
     161,   163,   170
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   406,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   422,   434,   435,   439,   443,
     448,   457,   465,   481,   503,   507,   510,   516,   525,   546,
     569,   570,   574,   578,   579,   583,   586,   592,   598,   601,
     607,   608,   612,   613,   614,   623,   624,   628,   637,   652,
     662,   671,   672,   673,   674,   675,   676,   680,   685,   693,
     694,   695,   696,   697,   698,   699,   700,   706,   719,   735,
     736,   740,   747,   759,   763,   770,   771,   775,   776,   777,
     781,   786,   793,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,   830,   831,   832,   836,
     857,   897,   900,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     927,   931,   936,   942,   948,   959,   965,   966,   977,   981,
     987,   990,   993,   997,  1003,  1007,  1011,  1014,  1017,  1022,
    1025,  1033,  1036,  1041,  1044,  1052,  1058,  1059,  1063,  1064,
    1068,  1102,  1102,  1102,  1105,  1105,  1105,  1110,  1110,  1110,
    1118,  1122,  1127,  1133,  1133,  1133,  1137,  1137,  1137,  1142,
    1142,  1142,  1151,  1154,  1160,  1161,  1168,  1179,  1180,  1184,
    1189,  1244,  1245,  1246,  1247,  1248,  1249,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,  1272,  1273,  1277,  1288,  1293,  1303,  1307,
    1307,  1307,  1314,  1314,  1314,  1328,  1329,  1330,  1331,  1332,
    1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,  1342,
    1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
    1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,  1361,  1362,
    1363,  1364,  1365,  1366,  1367,  1371,  1375,  1378,  1381,  1382,
    1383,  1384,  1389,  1394,  1395,  1398,  1398,  1398,  1401,  1405,
    1409,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1429,  1439,
    1494,  1495,  1499,  1500,  1504,  1512,  1515,  1519,  1572,  1577,
    1585,  1586,  1590,  1593,  1602,  1603,  1607,  1616,  1617,  1622,
    1623,  1627,  1633,  1639,  1642,  1646,  1652,  1661,  1662,  1663,
    1667,  1668,  1672,  1675,  1680,  1685,  1693,  1704,  1707,  1714,
    1715,  1719,  1746,  1746,  1758,  1761,  1770,  1783,  1795,  1796,
    1800,  1804,  1815,  1830,  1831,  1835,  1875,  1876,  1880,  1880,
    1964,  1971,  1980,  1981,  1982,  1983,  1984,  1985,  1986,  1987,
    1988,  1989,  1990,  1991,  1992,  1993,  1994,  1995,  1996,  1997,
    1998,  1999,  2000,  2001,  2002,  2003,  2004,  2008,  2009,  2010,
    2011,  2012,  2013,  2017,  2028,  2032,  2042,  2049,  2058,  2058,
    2058,  2071,  2072,  2073,  2074,  2075,  2089,  2095,  2099,  2104,
    2109,  2114,  2119,  2123,  2127,  2132,  2136,  2141,  2141,  2141,
    2147,  2154,  2154,  2154,  2159,  2159,  2159,  2165,  2165,  2165,
    2170,  2174,  2174,  2174,  2179,  2179,  2179,  2188,  2192,  2192,
    2192,  2197,  2197,  2197,  2206,  2210,  2210,  2210,  2215,  2215,
    2215,  2224,  2224,  2224,  2230,  2230,  2230,  2239,  2239,  2254,
    2254,  2274,  2275,  2276,  2277,  2281,  2288,  2297,  2302,  2309,
    2314,  2320,  2326,  2333,  2344,  2347,  2361,  2370,  2371,  2375,
    2380,  2387,  2392,  2403,  2408,  2415,  2427,  2428,  2432,  2449
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
  "\"with\"", "\"cast\"", "\"override\"", "\"upcast\"", "\"iterator\"",
  "\"var\"", "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"",
  "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"", "\"goto\"",
  "\"implicit\"", "\"shared\"", "\"smart_ptr\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"",
  "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"",
  "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS",
  "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'",
  "DEREF", "'['", "']'", "'('", "')'", "'$'", "'@'", "COLCOL", "';'",
  "'{'", "'}'", "'#'", "$accept", "program", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "expression_with", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration", "function_declaration",
  "expression_block", "expression_any", "expressions", "expr_pipe",
  "name_in_namespace", "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_yield",
  "expression_try_catch", "let_scope", "kwd_let", "expression_let",
  "expr_cast", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "expr_type_info",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "expr_list",
  "block_or_simple_block", "block_or_lambda", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@13", "$@14",
  "$@15", "$@16", "expr", "$@17", "$@18", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
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
  "make_struct_fields", "make_struct_dim", "make_struct_decl",
  "make_tuple", "make_map_tuple", "make_any_tuple", "make_dim",
  "make_dim_decl", "make_table", "make_table_decl",
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
      44,    61,    63,    58,   124,    94,    38,    60,    62,    45,
      43,    42,    47,    37,   395,   396,   126,    33,   397,   398,
     399,   400,    46,   401,    91,    93,    40,    41,    36,    64,
     402,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -514

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-514)))

#define YYTABLE_NINF -449

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -514,    13,  -514,  -514,   -20,   -11,   -10,    -7,   -37,  -514,
      62,  -514,  -514,    -8,  -514,  -514,  -514,  -514,  -514,   163,
    -514,   101,  -514,  -514,  -514,  -514,  -514,  -514,    79,  -514,
     108,    86,   137,  -514,  -514,    46,  -514,    50,   133,  -514,
     150,   156,  -514,   122,   168,   149,  -514,   -55,  -514,  -514,
    -514,    72,   169,  -514,   151,    12,   -20,   200,   -11,   205,
    -514,   208,   218,  -514,   250,  -514,   216,  -514,   -94,   192,
     223,   226,  -514,   -20,    -8,  -514,  6705,   356,   368,  -514,
     239,  -514,   265,  -514,  -514,   426,  -514,  -514,  -514,  -514,
    -514,   296,    89,  -514,  -514,  -514,  -514,   382,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,   264,   -83,  5695,  -514,
    -514,   303,   308,  -514,   -48,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,   -84,   300,   -82,  -514,   276,   -80,   278,  -514,
      74,  -514,  -514,  -514,  -514,  -514,  -514,  -514,    67,  -514,
    -514,   -71,  -514,   306,   307,   312,   314,  -514,  -514,  -514,
     299,  -514,  -514,  -514,  -514,  -514,   320,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,   321,  -514,  -514,  -514,
     323,   327,  -514,  -514,  -514,  -514,   329,   330,  -514,  -514,
    -514,  -514,  -514,    45,  -514,   311,   335,  -514,   316,  -514,
     -20,  -514,   -18,  -514,   143,  5695,   313,  -514,  -514,  -514,
    -514,  -514,   251,  -514,  -514,  -514,    89,  -514,   -76,  4291,
    -514,  -514,  -514,  -514,  -514,   345,  -514,   152,   195,   207,
    -514,  -514,  -514,  -514,  -514,   457,  -514,  -514,    -3,  2293,
    -514,  -514,   -68,  5695,   -65,   121,   365,  -514,   119,  -514,
     379,  3716,  -514,  -514,    17,   371,  5695,  -514,    54,  -514,
    -514,  -514,  -514,  5618,   341,  -514,   342,   359,   362,   344,
     364,  -514,   366,  4291,  4291,   821,  4419,  4291,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  4291,  4291,  4291,  4291,  4291,
    4291,  -514,   343,  -514,  -514,   -70,  -514,  -514,  -514,   350,
    -514,  -514,  -514,  -514,  -514,  4962,   349,  -514,  -514,  -514,
    -514,  -514,  5695,  5695,  5695,  5695,   351,   303,  5695,   239,
    5695,   239,  5695,   239,  5772,   308,  -514,  -514,  -514,   352,
    -514,  -514,  5766,  -514,  -514,  3716,  -514,  -514,   354,  -514,
    -514,  -514,  5695,  -514,  2422,  -514,   996,  -514,   509,   478,
     353,  -514,   292,  -514,  -514,  -514,  2422,   298,  -514,   -82,
    4291,  -514,  -514,  4291,  -514,  5695,    35,    35,   365,  1317,
     365,  1462,  6158,  -514,   -64,   262,    35,    35,   -77,    35,
      35,  5099,   -58,  -514,  1890,   239,     5,   388,  4291,  4291,
    -514,  -514,  4291,  4291,  4291,  4291,   389,  4291,   392,  4291,
    4291,  4291,  4291,  4291,  2560,  4291,  4291,  4291,  4291,  4291,
    4291,  4291,  4291,  4291,  4291,   393,  4291,  -514,  2689,   309,
     127,   413,   593,  -514,   -99,   803,   300,   820,   300,  1606,
     300,     9,  -514,   -98,  3716,   109,  -514,  -514,  -514,  2419,
     224,  -514,   412,  5989,  -514,  4291,  -514,  -514,   365,  1752,
    -514,   313,  4291,  4291,  -514,   358,   402,  2827,  2035,  -514,
     495,   361,   363,  4291,  -514,  -514,  -514,  -514,  -514,  -514,
     367,   369,   370,  -514,  -514,  -514,   514,  -514,   -85,  -514,
    6093,   478,  -514,   365,  -514,   457,  -514,  2422,  -514,  4417,
    2956,  2164,  5168,  5695,  5695,  5236,  5695,  1840,    14,   -79,
    1607,   372,  3094,  6158,   395,  -108,   396,  -514,  -514,   161,
      24,  3232,    30,   213,  4291,  4291,   373,  -514,  4291,   403,
    -514,   235,  -514,   408,  -514,    73,  6361,   300,   397,  -514,
    -514,   711,   711,  6671,  6671,  6616,  6616,   377,    18,  -514,
    5843,   -39,   -39,   711,   711,   411,  6227,   267,  6426,  6495,
    6671,  6671,   194,   194,    18,    18,    18,  -514,  5912,  -514,
      78,  -514,  5695,  -514,  -514,  -514,   415,  -514,   404,  -514,
     409,  -514,   410,  5695,  -514,  5772,  -514,   308,   457,  4291,
    -514,  -514,   224,  4291,  4291,  4291,  4291,  4291,  4291,  4291,
    4291,  4291,  4291,  4291,  4291,  4291,  4291,  4291,   527,    26,
    2422,  -514,  -514,  4513,   537,  6361,   527,  -514,   417,   428,
    6361,  2422,  -514,  4609,   394,  -514,  -514,   527,  -514,  -514,
    -514,   525,   365,  -514,  3370,  3499,    72,  -514,  -514,  4705,
    -514,  -514,   112,   421,   437,  5304,  -514,  2421,  2510,  -514,
    2686,   406,  4291,  4291,  -514,   408,   440,   414,   408,  4291,
     416,  4291,  4291,   408,   418,   419,  6361,  -514,  -514,  6292,
    5695,   239,    91,   134,  4291,  -514,   -90,  -514,  3628,  -514,
    -514,  4291,  -514,  -514,  -514,  2777,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  3716,  -514,  -514,  -514,  6361,
    -514,  6361,  6361,  6361,  6361,  6361,  6361,  6361,  6361,  6361,
    6361,  6361,  6361,  6361,  6361,  6361,  -514,  4291,  -514,  4801,
    -514,   313,  -514,  -514,  -514,  -514,  4897,  -514,  -514,   340,
    -514,  -514,  4291,  4291,  -514,  -514,  -514,  -514,   118,  -514,
    -514,  -514,  -514,  3766,   -93,  6361,   188,    91,  -514,   395,
    6361,  -514,  -514,   -56,   257,  -514,  -514,  -514,  2953,   300,
     423,  6361,  3904,  -514,  -514,  5695,  -514,   114,  6361,  -514,
    -514,  -514,  -514,   -42,  -514,  -514,  -514,  1169,   313,  -514,
    -514,  4291,  -514,  6560,  6560,  5695,  4033,   441,  4291,  4291,
    4291,  -514,  5372,  4291,   420,  4291,  4291,   427,  -514,   438,
    -514,  4291,  6361,  3044,  -514,  -514,   313,  -514,  -514,   527,
    3182,   444,  5440,   445,  6671,  6671,  6671,  -514,  5034,  -514,
    6361,  5034,  -514,   -82,  -514,  6361,  -514,  -514,   340,  -514,
    -514,  -514,  4162,   539,   429,   424,  -514,   -82,  -514,  -514,
     431,  5695,   456,  5508,  4291,   459,   461,  -514,  -514,  3320,
    -514,  -514,  6361,  -514,  -514,  -514,  5695,   465,  3625,  -514,
    -514,   466,  -514
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    75,     1,   168,     0,     0,     0,     0,     0,   169,
       0,   459,   457,     0,    14,     3,    10,     9,     8,     0,
       7,   339,     6,    11,     5,     4,    12,    13,    65,    67,
      23,    35,    32,    33,    25,    30,    24,     0,     0,    15,
       0,     0,    70,   146,     0,    71,    73,     0,    69,   357,
     356,     0,     0,   340,   342,     0,     0,     0,     0,     0,
      31,     0,     0,    28,     0,   344,     0,   348,     0,     0,
       0,     0,   148,     0,     0,    76,     0,     0,     0,    82,
      77,   119,   353,   358,   337,     0,    63,    64,    61,    62,
      60,     0,     0,    59,    68,    36,    34,    30,    27,    26,
     387,   390,   388,   391,   389,   392,     0,     0,     0,   350,
     349,     0,     0,   147,     0,    74,    85,    86,    88,    87,
      89,    90,    91,    92,   111,   112,   109,   110,   102,   113,
     114,   103,   100,   101,   115,   116,   117,   118,   105,   106,
     104,    98,    99,    94,    93,    95,    96,    97,    84,    83,
     107,   108,   300,    80,     0,   355,     0,     0,     0,    16,
       0,    55,    56,    53,    54,    52,    51,    57,     0,    29,
     344,     0,   351,     0,     0,     0,     0,   362,   382,   363,
     394,   364,   368,   369,   370,   371,   386,   375,   376,   377,
     378,   379,   380,   381,   383,   384,   430,   367,   374,   385,
     437,   444,   365,   372,   366,   373,     0,     0,   393,   401,
     404,   402,   403,     0,   396,     0,     0,   317,     0,    72,
       0,    78,     0,   310,     0,     0,     0,   354,   305,   360,
     341,   338,   330,   343,    17,    18,     0,    66,     0,     0,
     345,   421,   424,   427,   417,     0,   398,   431,   438,   445,
     451,   454,   408,   413,   420,     0,   416,   410,     0,     0,
     347,   412,     0,     0,     0,     0,     0,   308,   321,    79,
     300,    81,   141,   120,   300,     0,     0,   331,     0,    58,
     352,   240,   241,     0,     0,   235,     0,     0,     0,     0,
       0,   386,     0,     0,     0,     0,     0,     0,   201,   203,
     202,   204,   205,   206,    19,     0,     0,     0,     0,     0,
       0,   197,   198,   238,   199,   236,   293,   292,   291,    75,
     296,   237,   295,   294,   273,     0,     0,   239,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,    77,
       0,    77,     0,    77,     0,     0,   415,   409,   411,     0,
     414,   406,     0,   397,   460,   316,   458,   318,     0,   309,
     320,   319,     0,   322,     0,   311,     0,   359,     0,   302,
       0,   361,     0,   328,   329,   327,     0,   150,   153,     0,
       0,   171,   174,     0,   177,     0,   264,   265,     0,     0,
       0,     0,     0,   483,     0,     0,   245,   244,   278,   243,
     242,     0,     0,   299,     0,    77,     0,     0,     0,     0,
     266,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,   395,     0,     0,    80,     0,    80,     0,
      80,   146,   314,     0,   312,     0,   407,   405,   301,   323,
       0,   326,     0,   325,   143,     0,    45,    46,     0,     0,
     154,     0,     0,     0,   155,     0,     0,     0,     0,   123,
     121,     0,     0,     0,   136,   131,   129,   130,   142,   124,
       0,     0,     0,   134,   135,   137,   166,   128,     0,   125,
     207,   302,   303,     0,   306,   327,   332,     0,   336,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
       0,     0,     0,   474,   467,     0,   477,   478,   479,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,    20,
     268,   229,   228,     0,   274,     0,   192,    80,     0,   288,
     289,   246,   247,   259,   260,   257,   258,     0,   283,   272,
       0,   297,   298,   248,   249,     0,     0,   262,   263,   261,
     255,   256,   251,   250,   252,   253,   254,   271,     0,   276,
       0,   422,     0,   428,   418,   399,     0,   432,     0,   439,
       0,   446,     0,     0,   452,     0,   455,     0,   319,     0,
     198,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,   159,   207,     0,   149,     0,   140,     0,     0,
      39,     0,   163,   207,     0,   138,   139,     0,   127,   132,
     133,     0,     0,   126,     0,     0,     0,   304,   334,   207,
     335,   151,     0,     0,     0,     0,   279,     0,     0,   280,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   476,   484,   485,     0,
       0,    77,     0,     0,     0,   275,     0,   285,     0,   270,
     290,     0,   269,   277,   423,     0,   429,   419,   400,   433,
     435,   440,   442,   447,   449,   313,   453,   315,   456,   324,
     144,   214,   215,   217,   216,   218,   211,   212,   213,   219,
     220,   209,   210,   221,   222,   208,    49,     0,   160,   207,
     157,     0,    50,    37,    38,   164,   207,   161,   141,    42,
     167,   170,     0,     0,   307,   333,   152,   183,     0,   180,
     172,   175,   178,     0,     0,   465,     0,     0,   469,   468,
     475,   481,   480,     0,     0,   472,   482,    21,     0,    80,
       0,   193,     0,   194,   200,     0,   226,     0,   284,   425,
     436,   443,   450,     0,   158,   165,   162,     0,     0,    40,
      41,     0,    47,   224,   223,     0,     0,     0,     0,     0,
       0,   281,     0,     0,     0,     0,     0,     0,   230,     0,
     225,     0,   195,     0,   227,   426,     0,   122,    43,     0,
       0,     0,     0,     0,   173,   176,   179,   282,   486,   471,
     466,   486,   473,     0,   233,   196,   286,    48,    42,   184,
     186,   181,     0,     0,     0,     0,   231,     0,   287,    44,
       0,     0,     0,     0,     0,     0,     0,   234,   185,     0,
     189,   182,   487,   488,   489,   187,     0,     0,     0,   188,
     190,     0,   191
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -514,  -514,  -514,   210,   583,  -514,  -514,  -514,  -514,  -514,
    -514,   546,  -514,  -514,   586,  -514,  -514,  -514,  -192,  -514,
    -514,  -514,  -514,  -514,   422,  -514,   595,   -57,  -514,   580,
    -514,  -239,  -337,  -436,  -514,  -514,    11,  -166,  -514,   -75,
    -356,   -13,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
     654,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,  -416,  -514,  -514,  -442,
    -514,  -363,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
     187,  -514,  -514,   387,   165,  -514,  -514,   425,  -514,    87,
    -514,  -257,   317,  -259,  -245,   315,  -514,    39,  -514,  -514,
    -514,  -514,   513,   617,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -152,  -107,  -514,  -514,  -514,   355,  -514,  -514,
    -514,   -91,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
    -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,  -514,
     407,  -513,  -380,  -514,  -514,  -282,    15,   302,  -514,  -514,
    -514,  -135,  -514
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   160,   166,   395,   313,    16,    17,    35,
      36,    63,    18,    32,    33,   481,   482,   791,   792,   483,
     484,   485,   486,   487,   167,   168,    29,    30,    45,    46,
      47,    19,   153,   226,    80,    20,    81,   314,   488,   366,
     489,   315,   490,   316,   491,   492,   493,   494,   495,   642,
     496,   497,   317,   513,   798,   514,   799,   516,   800,   318,
     795,   850,   851,   867,   866,   871,   545,   774,   319,   320,
     321,   462,   499,   322,   323,   324,   680,   833,   681,   847,
     546,   775,   848,   222,   503,   370,   274,   223,   224,   452,
     453,   217,   218,   663,   267,   376,   278,   231,   157,    54,
      22,    85,   107,    67,    68,    23,    24,   155,    83,    52,
      25,   156,   268,   326,   106,   210,   211,   215,   212,   337,
     698,   454,   335,   697,   332,   694,   333,   815,   334,   696,
     338,   699,   339,   780,   340,   701,   341,   781,   342,   703,
     343,   782,   344,   706,   345,   708,    26,    41,    27,    40,
     327,   524,   525,   328,   526,   527,   528,   529,   329,   394,
     330,   844,   331
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   209,   446,   498,   448,   232,   450,   357,   461,   364,
     588,   532,   590,     2,   592,   393,   114,   213,   601,     3,
     508,   359,   580,   548,   -75,    42,    86,    87,   266,   347,
     683,   602,   360,   662,   772,   368,   418,   419,   406,   407,
      66,   220,     4,   672,     5,   727,     6,   684,     7,   585,
     594,   171,    43,     8,   229,   406,   407,   667,   171,     9,
     273,    48,   361,   668,   403,    10,   353,    59,   547,   216,
     239,   416,   586,   595,   418,   419,    43,   252,   803,   109,
     405,    11,   272,   221,   684,    74,   643,   436,    44,   541,
     172,    71,    56,   230,   652,   208,   404,   280,   684,    60,
     240,   253,    12,   161,   162,   354,    76,   535,   356,   536,
      75,   686,    44,   622,    28,   806,   229,    31,   209,   219,
     410,   411,   632,   435,    34,   436,    43,    37,   416,   816,
     417,   418,   419,   420,   271,    38,   421,   410,   411,   549,
      88,   227,   220,   348,    89,   416,    90,    91,   418,   419,
     420,   648,   593,   421,   275,   759,   209,   254,    53,   252,
     710,   349,    44,   265,   275,   373,   275,    49,    50,   209,
      51,   350,   355,   374,    77,    78,   209,    13,    92,    71,
     435,    13,   436,   253,    14,   372,   255,   256,   209,   209,
     367,   257,   377,    64,   258,   375,    39,   435,    61,   436,
     599,   668,   360,   674,   389,   391,    79,   236,    62,   259,
     234,   235,   208,   684,   406,   407,   260,   163,   684,   261,
      55,   164,    65,   165,    91,   209,   209,   209,   209,    57,
     360,   209,   361,   209,   237,   209,   518,   209,   530,   254,
     685,   439,   440,   441,   442,   693,   754,   445,    56,   447,
     208,   449,   684,   677,   684,   209,   763,   596,   647,   275,
     361,    56,   362,   208,   728,   363,   796,    66,   255,   256,
     208,   459,   777,   257,   666,   735,   258,    58,   209,   746,
     597,   814,   208,   208,    69,   756,   358,   406,   407,   797,
      70,   259,    71,   764,   517,  -434,   410,   411,   582,   770,
    -434,   261,    72,    82,   416,   624,   417,   418,   419,   420,
     269,   783,   421,   100,   270,    73,   619,   101,  -434,   208,
     208,   208,   208,    84,   252,   208,   670,   208,    95,   208,
     252,   208,   671,   809,   102,   103,   104,   105,  -441,    97,
     357,   252,    98,  -441,   769,   432,   433,   434,   253,   208,
    -448,   788,    99,   804,   253,  -448,   435,   108,   436,   668,
     113,  -441,   789,   790,   111,   253,   511,   408,   409,   410,
     411,   412,   208,  -448,   413,   414,   415,   416,  -232,   417,
     418,   419,   420,  -232,   671,   421,   675,   422,   423,   542,
     150,   275,   311,   600,   276,   112,   272,   277,   159,   537,
     538,  -232,   151,   373,   254,   152,   209,   209,   154,   209,
     254,   374,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   254,   657,   658,   498,   660,   325,   158,   668,   435,
     807,   436,   159,   505,   256,    60,   170,   214,   257,   255,
     256,   258,   216,   225,   257,   252,   352,   258,   228,   233,
     255,   256,   726,   241,   242,   257,   259,   581,   258,   243,
     732,   244,   259,   506,   510,   245,   261,   246,   247,   253,
     248,   739,   261,   259,   249,   209,   250,   251,   263,   336,
     386,   387,   262,   261,   392,   272,   209,   264,   209,   346,
     232,   695,   396,   397,   398,   399,   400,   401,   805,   229,
     208,   208,   705,   208,   220,   371,   381,   379,   380,   382,
     383,   384,   402,   385,    13,   438,   501,   456,   443,   458,
     773,   502,   550,   557,   504,   254,   559,   577,   602,   627,
     628,   634,   635,   641,   636,   666,   669,   664,   638,   234,
     639,   640,   682,   688,   687,   690,   678,   406,   407,   353,
     731,   463,   700,   500,   255,   256,   734,   702,   704,   257,
     733,   583,   258,   509,   740,   785,   738,   512,   747,   208,
     515,   748,   753,   209,   757,   823,   523,   259,   523,   758,
     208,   761,   208,   765,   766,   829,   834,   261,   854,   768,
     810,   840,   832,   842,   855,   551,   552,   856,   858,   553,
     554,   555,   556,   860,   558,   539,   560,   561,   562,   563,
     564,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   818,   578,   863,   252,   864,   408,   409,   410,
     411,   412,   869,   872,   413,   414,   415,   416,    93,   417,
     418,   419,   420,   169,    96,   421,   849,   422,   423,   253,
     837,    94,   618,   838,   115,    21,   623,   744,   279,   625,
     626,   369,   455,   787,   630,   633,   646,   208,   209,   424,
     637,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   741,   707,   238,   813,   110,   762,   507,   209,   435,
     378,   436,   444,   533,   649,   365,   845,     0,   655,   272,
       0,     0,     0,     0,   820,   254,     0,   396,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
       0,   676,   392,     0,     0,   679,     0,     0,     0,     0,
       0,   406,   407,     0,   255,   256,     0,     0,     0,   257,
       0,   584,   258,     0,   209,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   209,
     859,     0,   208,     0,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,   868,     0,     0,     0,     0,
       0,     0,   208,     0,     0,     0,   709,     0,     0,     0,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,     0,     0,   729,     0,     0,
       0,     0,     0,   410,   411,     0,     0,     0,   736,     0,
     846,   416,     0,   417,   418,   419,   420,     0,     0,   421,
       0,   721,   725,   388,   857,   252,     0,     0,   208,     0,
       0,     0,     0,     0,     0,   173,     0,     0,     0,     0,
     755,   174,   252,   208,     0,     0,   760,     0,   523,   253,
     430,   431,   432,   433,   434,     0,   175,     0,     0,     0,
       0,   771,     0,   435,     0,   436,   253,     0,   778,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,     0,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   793,
     794,     0,   254,     0,     0,     0,     0,     0,     0,     0,
     802,     0,     0,     0,   255,   256,     0,     0,     0,   257,
       0,   587,   258,     0,     0,    43,     0,     0,     0,   812,
       0,   255,   256,     0,     0,     0,   257,   259,   589,   258,
       0,     0,     0,     0,   500,     0,     0,   261,   819,     0,
       0,     0,     0,   822,   259,   824,   825,   826,     0,     0,
     828,    44,   830,   831,   261,     0,     0,   464,   835,     0,
       0,     0,     3,     0,   465,   466,   467,     0,   468,     0,
     281,   282,   283,   284,     0,     0,     0,     0,     0,     0,
       0,   469,   285,   470,   471,     0,     0,     0,     0,   853,
       0,     0,     0,   472,   286,     0,     0,   473,   287,     0,
     288,   862,     9,   289,   474,     0,   475,   290,     0,     0,
     476,   477,     0,     0,     0,   177,   178,   179,     0,   181,
     182,   183,   184,   185,   291,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   292,   478,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   293,   294,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,     0,
       0,   295,   296,   297,   298,   299,   300,   301,   302,   303,
      43,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   306,   307,     0,     0,
       0,     0,   308,   309,     0,     0,     0,     0,     0,     0,
       0,     0,   310,     0,   311,   312,    44,   479,   272,   480,
     464,     0,     0,     0,     0,     3,     0,   465,   466,   467,
       0,   468,     0,   281,   282,   283,   284,     0,     0,     0,
       0,     0,     0,     0,   469,   285,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,   472,   286,     0,     0,
     473,   287,     0,   288,     0,     9,   289,   474,     0,   475,
     290,     0,     0,   476,   477,     0,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   291,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   292,
     478,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   294,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,   295,   296,   297,   298,   299,   300,
     301,   302,   303,    43,   304,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
     307,     0,     0,     0,     0,   308,   309,     0,     0,     0,
       0,   281,   282,   283,   284,   310,     0,   311,   312,    44,
     479,   272,   817,   285,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,     0,   286,     0,     0,     0,   287,
       0,   288,     0,     0,   289,     0,     0,     0,   290,     0,
       0,     0,     0,   253,     0,     0,   177,   178,   179,     0,
     181,   182,   183,   184,   185,   291,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   293,
     294,     0,     0,     0,     0,     0,     0,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   519,   304,     0,     0,     0,     0,     0,   255,   256,
       0,     0,     0,   257,     0,     0,   520,   306,   307,     0,
       0,     0,     0,   308,   309,     0,   281,   282,   283,   284,
       0,   259,   521,   522,     0,   311,   312,    44,   285,   272,
       0,   261,     0,     0,   252,     0,     0,     0,     0,     0,
     286,     0,     0,     0,   287,     0,   288,     0,     0,   289,
       0,     0,     0,   290,     0,     0,     0,     0,   253,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     291,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   294,     0,     0,     0,     0,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   519,   304,     0,     0,
       0,     0,     0,   255,   256,     0,     0,     0,   257,     0,
       0,   520,   306,   307,     0,     0,     0,     0,   308,   309,
       0,   281,   282,   283,   284,     0,   259,     0,   531,     0,
     311,   312,    44,   285,   272,     0,   261,     0,   252,   347,
       0,     0,     0,     0,     0,   286,     0,     0,     0,   287,
       0,   288,     0,     0,   289,     0,     0,     0,   290,     0,
       0,     0,   253,     0,     0,     0,   177,   178,   179,     0,
     181,   182,   183,   184,   185,   291,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   293,
     294,     0,     0,     0,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,   296,   297,   298,   299,   300,   301,   302,
     303,    43,   304,     0,     0,     0,     0,   255,   256,     0,
       0,     0,   257,   348,   591,   258,   305,   306,   307,     0,
       0,     0,     0,   308,   309,     0,   281,   282,   283,   284,
     259,   349,     0,   310,     0,   311,   312,    44,   285,   272,
     261,   350,     0,     0,     0,     0,     0,     0,     0,     0,
     286,     0,     0,     0,   287,     0,   288,     0,     0,   289,
       0,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     291,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   294,     0,     0,     0,     0,
       0,     0,     0,   620,     0,     0,     0,     0,     0,   460,
       0,     0,   252,     0,     0,     0,     0,   295,   296,   297,
     298,   299,   300,   301,   302,   303,    43,   304,     0,     0,
       0,     0,     0,     0,     0,     0,   253,     0,     0,     0,
       0,   305,   306,   307,   281,   282,   283,   284,   308,   309,
       0,     0,     0,     0,     0,     0,   285,     0,   310,     0,
     311,   312,    44,   621,   272,     0,     0,     0,   286,     0,
       0,     0,   287,     0,   288,     0,     0,   289,     0,     0,
       0,   290,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   254,   181,   182,   183,   184,   185,   291,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     292,   255,   256,     0,     0,     0,   257,     0,   661,   258,
       0,     0,   293,   294,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   295,   296,   297,   298,   299,
     300,   301,   302,   303,    43,   304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,   307,     0,     0,     0,     0,   308,   309,     0,   281,
     282,   283,   284,     0,   543,     0,   310,   544,   311,   312,
      44,   285,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   286,     0,     0,     0,   287,     0,   288,
       0,     0,   289,     0,     0,     0,   290,     0,     0,     0,
       0,     0,     0,     0,   177,   178,   179,     0,   181,   182,
     183,   184,   185,   291,   187,   188,   189,   190,   191,   192,
     193,   194,   195,     0,   197,   198,   199,     0,     0,   202,
     203,   204,   205,     0,     0,   292,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   293,   294,     0,
       0,     0,     0,     0,     0,     0,   631,     0,     0,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     295,   296,   297,   298,   299,   300,   301,   302,   303,    43,
     304,     0,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   284,   653,     0,   305,   306,   307,     0,     0,     0,
     285,   308,   309,     0,     0,     0,     0,     0,     0,     0,
       0,   310,   286,   311,   312,    44,   287,   272,   288,     0,
       0,   289,     0,     0,     0,   290,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   291,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,   294,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
     296,   297,   298,   299,   300,   301,   302,   303,    43,   304,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   654,     0,   305,   306,   307,     0,     0,     0,   285,
     308,   309,     0,     0,     0,     0,     0,     0,     0,     0,
     310,   286,   311,   312,    44,   287,   272,   288,     0,     0,
     289,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,     0,   177,   178,   179,     0,   181,   182,   183,   184,
     185,   291,   187,   188,   189,   190,   191,   192,   193,   194,
     195,     0,   197,   198,   199,     0,     0,   202,   203,   204,
     205,     0,     0,   292,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   293,   294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   296,
     297,   298,   299,   300,   301,   302,   303,    43,   304,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
       0,     0,   305,   306,   307,     0,     0,     0,   285,   308,
     309,   252,     0,   252,     0,     0,     0,     0,   351,   310,
     286,   311,   312,    44,   287,   272,   288,     0,     0,   289,
       0,     0,     0,   290,     0,   253,     0,   253,     0,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     291,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   294,     0,     0,     0,     0,
     360,   254,     0,   254,     0,     0,     0,     0,     0,   460,
       0,     0,   252,     0,     0,     0,     0,   295,   296,   297,
     298,   299,   300,   301,   302,   303,    43,   304,     0,     0,
     598,   256,   255,   256,     0,   257,   253,   257,   258,   750,
     258,   305,   306,   307,   281,   282,   283,   284,   308,   309,
       0,   565,     0,   259,     0,   259,   285,     0,   310,     0,
     311,   312,    44,   261,   272,   261,     0,     0,   286,     0,
       0,     0,   287,     0,   288,     0,     0,   289,     0,     0,
       0,   290,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   254,   181,   182,   183,   184,   185,   291,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     292,   255,   256,     0,     0,     0,   257,     0,   751,   258,
       0,     0,   293,   294,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   295,   296,   297,   298,   299,
     300,   301,   302,   303,    43,   304,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   305,
     306,   307,     0,     0,     0,   285,   308,   309,   252,     0,
       0,     0,     0,     0,     0,     0,   310,   286,   311,   312,
      44,   287,   272,   288,     0,     0,   289,     0,     0,     0,
     290,     0,   253,     0,     0,     0,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   291,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   292,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   294,     0,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,   295,   296,   297,   298,   299,   300,
     301,   302,   303,    43,   304,     0,     0,   255,   256,     0,
       0,     0,   257,   253,   752,   258,     0,     0,   305,   306,
     307,   281,   282,   283,   284,   308,   309,     0,     0,     0,
     259,     0,     0,   285,     0,   310,   579,   311,   312,    44,
     261,   272,     0,     0,     0,   286,     0,     0,     0,   287,
       0,   288,     0,     0,   289,     0,     0,     0,   290,     0,
       0,   629,     0,     0,     0,     0,   177,   178,   179,   254,
     181,   182,   183,   184,   185,   291,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   292,   255,   256,
       0,     0,     0,   257,     0,   779,   258,     0,     0,   293,
     294,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,   295,   296,   297,   298,   299,   300,   301,   302,
     303,    43,   304,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,     0,     0,   305,   306,   307,     0,
       0,     0,   285,   308,   309,   252,     0,     0,     0,     0,
       0,     0,     0,   310,   286,   311,   312,    44,   287,   272,
     288,     0,     0,   289,     0,     0,     0,   290,     0,   253,
       0,     0,     0,     0,     0,   177,   178,   179,     0,   181,
     182,   183,   184,   185,   291,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   293,   294,
       0,     0,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
       0,   295,   296,   297,   298,   299,   300,   301,   302,   303,
      43,   304,     0,     0,   255,   256,     0,     0,     0,   257,
     253,   808,   258,     0,     0,   305,   306,   307,   281,   282,
     283,   284,   308,   309,     0,     0,     0,   259,     0,     0,
     285,     0,   310,   651,   311,   312,    44,   261,   272,     0,
       0,     0,   286,     0,     0,     0,   287,     0,   288,     0,
       0,   289,     0,     0,     0,   290,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,   254,   181,   182,   183,
     184,   185,   291,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   292,   255,   256,     0,     0,     0,
     257,     0,   836,   258,     0,     0,   293,   294,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   261,   295,
     296,   297,   298,   299,   300,   301,   302,   303,    43,   304,
       0,     0,     0,     0,     0,     0,     0,     0,   253,     0,
       0,     0,     0,   305,   306,   307,   281,   282,   283,   284,
     308,   309,     0,     0,     0,     0,     0,     0,   285,     0,
     310,   665,   311,   312,    44,     0,   272,     0,     0,     0,
     286,     0,     0,     0,   287,     0,   288,     0,     0,   289,
       0,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   254,   181,   182,   183,   184,   185,
     291,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   292,   255,   256,     0,     0,     0,   257,     0,
     839,   258,     0,     0,   293,   294,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   261,   295,   296,   297,
     298,   299,   300,   301,   302,   303,    43,   304,     0,     0,
       0,     0,     0,     0,     0,     0,   253,     0,     0,     0,
       0,   305,   306,   307,   281,   282,   283,   284,   308,   309,
       0,     0,     0,     0,     0,     0,   285,     0,   310,   673,
     311,   312,    44,     0,   272,     0,     0,     0,   286,     0,
       0,     0,   287,     0,   288,     0,     0,   289,     0,     0,
       0,   290,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   254,   181,   182,   183,   184,   185,   291,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     292,   255,   256,     0,     0,     0,   257,     0,   865,   258,
       0,     0,   293,   294,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,   742,     0,     0,
       0,     0,     0,     0,   261,   295,   296,   297,   298,   299,
     300,   301,   302,   303,    43,   304,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   305,
     306,   307,     0,     0,     0,   285,   308,   309,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   286,   311,   312,
      44,   287,   272,   288,     0,     0,   289,     0,     0,     0,
     290,     0,     0,     0,     0,     0,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   291,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   292,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   294,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   743,     0,     0,     0,
       0,     0,     0,     0,   295,   296,   297,   298,   299,   300,
     301,   302,   303,    43,   304,     0,     0,     0,     0,     0,
       0,     0,   281,   282,   283,   284,     0,     0,   305,   306,
     307,     0,     0,     0,   285,   308,   309,   252,     0,     0,
       0,     0,     0,     0,     0,   310,   286,   311,   312,    44,
     287,   272,   288,     0,     0,   289,     0,     0,     0,   290,
       0,   253,     0,     0,     0,     0,     0,   177,   178,   179,
       0,   181,   182,   183,   184,   185,   291,   187,   188,   189,
     190,   191,   192,   193,   194,   195,     0,   197,   198,   199,
       0,     0,   202,   203,   204,   205,     0,     0,   292,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     293,   294,     0,     0,     0,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,   295,   296,   297,   298,   299,   300,   301,
     302,   303,    43,   304,     0,     0,   255,   256,     0,     0,
       0,   257,   253,   870,   258,     0,     0,   305,   306,   307,
     281,   282,   283,   284,   308,   309,     0,     0,     0,   259,
       0,     0,   285,     0,   310,   776,   311,   312,    44,   261,
     272,     0,     0,     0,   286,     0,     0,     0,   287,     0,
     288,     0,     0,   289,     0,     0,     0,   290,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,   254,   181,
     182,   183,   184,   185,   291,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   292,   255,   256,     0,
       0,     0,   257,     0,     0,   258,     0,     0,   293,   294,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   295,   296,   297,   298,   299,   300,   301,   302,   303,
      43,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   306,   307,   281,   282,
     283,   284,   308,   309,     0,     0,     0,     0,     0,     0,
     285,     0,   310,   801,   311,   312,    44,     0,   272,     0,
       0,     0,   286,     0,     0,     0,   287,     0,   288,     0,
       0,   289,     0,     0,     0,   290,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   291,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,   294,     0,     0,
       0,     0,     0,     0,     0,   811,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
     296,   297,   298,   299,   300,   301,   302,   303,    43,   304,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   821,     0,   305,   306,   307,     0,     0,     0,   285,
     308,   309,     0,     0,     0,     0,     0,     0,     0,     0,
     310,   286,   311,   312,    44,   287,   272,   288,     0,     0,
     289,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,     0,   177,   178,   179,     0,   181,   182,   183,   184,
     185,   291,   187,   188,   189,   190,   191,   192,   193,   194,
     195,     0,   197,   198,   199,     0,     0,   202,   203,   204,
     205,     0,     0,   292,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   293,   294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   296,
     297,   298,   299,   300,   301,   302,   303,    43,   304,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     852,     0,   305,   306,   307,     0,     0,     0,   285,   308,
     309,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     286,   311,   312,    44,   287,   272,   288,     0,     0,   289,
       0,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     291,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   294,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   295,   296,   297,
     298,   299,   300,   301,   302,   303,    43,   304,     0,     0,
       0,     0,     0,     0,     0,   281,   282,   283,   284,     0,
       0,   305,   306,   307,     0,     0,     0,   285,   308,   309,
       0,     0,     0,     0,     0,     0,     0,     0,   310,   286,
     311,   312,    44,   287,   272,   288,     0,     0,   289,     0,
       0,     0,   290,     0,     0,     0,     0,     0,     0,     0,
     177,   178,   179,     0,   181,   182,   183,   184,   185,   291,
     187,   188,   189,   190,   191,   192,   193,   194,   195,     0,
     197,   198,   199,     0,     0,   202,   203,   204,   205,     0,
       0,   292,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,   294,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,   296,   297,   298,
     299,   300,   301,   302,   303,    43,   304,     0,     0,     0,
       0,   390,     0,     0,     0,     0,     0,   406,   407,     0,
     305,   306,   307,   173,     0,     0,     0,   308,   309,   174,
       0,     0,     0,     0,     0,     0,     0,   310,     0,   311,
     312,    44,     0,   272,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   603,
     604,   605,   606,   607,   608,   609,   610,   408,   409,   410,
     411,   412,   611,   612,   413,   414,   415,   416,   613,   417,
     418,   419,   420,   406,   407,   421,   614,   422,   423,   615,
     616,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,   617,   424,
       0,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,     0,     0,     0,     0,     0,     0,     0,     0,   435,
       0,   436,     0,     0,     0,     0,     0,     0,   650,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   603,   604,   605,   606,   607,
     608,   609,   610,   408,   409,   410,   411,   412,   611,   612,
     413,   414,   415,   416,   613,   417,   418,   419,   420,   406,
     407,   421,   614,   422,   423,   615,   616,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   617,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,   436,     0,     0,
       0,     0,     0,     0,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   603,   604,   605,   606,   607,   608,   609,   610,   408,
     409,   410,   411,   412,   611,   612,   413,   414,   415,   416,
     613,   417,   418,   419,   420,   406,   407,   421,   614,   422,
     423,   615,   616,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     617,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,   436,     0,     0,     0,     0,     0,     0,
     737,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   603,   604,   605,
     606,   607,   608,   609,   610,   408,   409,   410,   411,   412,
     611,   612,   413,   414,   415,   416,   613,   417,   418,   419,
     420,   406,   407,   421,   614,   422,   423,   615,   616,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   617,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,   436,
       0,     0,     0,     0,     0,     0,   745,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   603,   604,   605,   606,   607,   608,   609,
     610,   408,   409,   410,   411,   412,   611,   612,   413,   414,
     415,   416,   613,   417,   418,   419,   420,   406,   407,   421,
     614,   422,   423,   615,   616,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   617,   424,     0,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,   436,     0,     0,     0,     0,
       0,     0,   784,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   407,     0,     0,     0,     0,     0,   603,
     604,   605,   606,   607,   608,   609,   610,   408,   409,   410,
     411,   412,   611,   612,   413,   414,   415,   416,   613,   417,
     418,   419,   420,     0,     0,   421,   614,   422,   423,   615,
     616,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   617,   424,
       0,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,     0,     0,     0,   406,   407,     0,     0,     0,   435,
       0,   436,   408,   409,   410,   411,   412,     0,   786,   413,
     414,   415,   416,     0,   417,   418,   419,   420,     0,     0,
     421,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,     0,     0,     0,   406,
     407,     0,     0,     0,   435,     0,   436,     0,     0,     0,
       0,     0,     0,   437,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,   416,     0,   417,   418,   419,   420,
       0,     0,   421,     0,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   406,   407,
       0,     0,     0,     0,     0,     0,   435,     0,   436,   408,
     409,   410,   411,   412,     0,   843,   413,   414,   415,   416,
       0,   417,   418,   419,   420,     0,     0,   421,     0,   422,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,     0,     0,     0,   406,   407,     0,     0,
       0,   435,     0,   436,     0,     0,   540,     0,   408,   409,
     410,   411,   412,     0,     0,   413,   414,   415,   416,     0,
     417,   418,   419,   420,     0,     0,   421,     0,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     424,     0,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,     0,     0,   406,   407,     0,     0,     0,     0,
     435,     0,   436,     0,     0,   656,   408,   409,   410,   411,
     412,     0,     0,   413,   414,   415,   416,     0,   417,   418,
     419,   420,     0,     0,   421,     0,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
       0,     0,   406,   407,     0,     0,     0,     0,   435,     0,
     436,     0,     0,   659,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,   416,     0,   417,   418,   419,   420,
       0,     0,   421,     0,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,     0,     0,
     406,   407,     0,     0,     0,     0,   435,     0,   436,     0,
       0,   749,   408,   409,   410,   411,   412,     0,     0,   413,
     414,   415,   416,     0,   417,   418,   419,   420,     0,     0,
     421,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,     0,     0,   406,   407,
       0,     0,     0,     0,   435,     0,   436,     0,     0,   827,
     408,   409,   410,   411,   412,     0,     0,   413,   414,   415,
     416,     0,   417,   418,   419,   420,     0,     0,   421,     0,
     422,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,     0,
       0,     0,   435,     0,   436,     0,     0,   841,   408,   409,
     410,   411,   412,     0,     0,   413,   414,   415,   416,     0,
     417,   418,   419,   420,     0,     0,   421,     0,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,     0,     0,     0,     0,     0,   174,     0,
     424,     0,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,     0,   175,     0,     0,     0,     0,     0,     0,
     435,     0,   436,     0,     0,   861,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
       0,     0,     0,     0,     0,   174,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,     0,     0,   295,   296,   297,     0,     0,     0,     0,
       0,     0,    43,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,     0,   406,   407,    44,     0,
       0,     0,     0,     0,     0,     0,   173,     0,     0,     0,
       0,     0,   174,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,     0,   406,   407,    44,   408,   409,   410,   411,
     412,     0,     0,   413,   414,   415,   416,     0,   417,   418,
     419,   420,     0,     0,   421,     0,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,     0,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,     0,     0,     0,   435,     0,
     436,   457,   406,   407,     0,     0,     0,     0,     0,     0,
       0,     0,    44,   408,   409,   410,   411,   412,     0,     0,
     413,   414,   415,   416,     0,   417,   418,   419,   420,     0,
       0,   421,     0,   422,   423,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,   436,   689,   406,
     407,     0,   408,   409,   410,   411,   412,     0,     0,   413,
     414,   415,   416,     0,   417,   418,   419,   420,     0,     0,
     421,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,     0,     0,     0,   435,     0,   436,   692,     0,     0,
       0,   603,   604,   605,   606,   607,   608,   609,   610,   408,
     409,   410,   411,   412,   611,   612,   413,   414,   415,   416,
     613,   417,   418,   419,   420,  -207,     0,   421,   614,   422,
     423,   615,   616,   406,   407,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     617,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,     0,     0,     0,     0,   603,   604,   605,   606,   607,
     608,   609,   610,   408,   409,   410,   411,   412,   611,   612,
     413,   414,   415,   416,   644,   417,   418,   419,   420,     0,
       0,   421,   614,   422,   423,   615,   616,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   645,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   406,   407,     0,
       0,     0,     0,     0,     0,   435,     0,   436,   408,   409,
     410,   411,   412,     0,     0,   413,   414,   415,   416,     0,
     417,   418,   419,   420,     0,     0,   421,     0,   422,   423,
       0,     0,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     424,     0,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   406,   407,     0,     0,     0,     0,     0,     0,
     435,     0,   436,     0,     0,     0,     0,   408,   409,   410,
     411,   412,     0,     0,   413,   414,   415,   416,     0,   417,
     418,   419,   420,     0,     0,   421,     0,   422,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   424,
     691,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   406,   407,     0,     0,     0,     0,     0,     0,   435,
       0,   436,   408,   409,   410,   411,   412,     0,     0,   413,
     414,   415,   416,     0,   417,   418,   419,   420,     0,     0,
     421,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   767,     0,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   406,   407,     0,     0,
       0,     0,     0,     0,   435,     0,   436,     0,     0,     0,
       0,   408,   409,   410,   411,   412,     0,     0,   413,   414,
     415,   416,     0,   417,   418,   419,   420,     0,     0,   421,
       0,   422,   423,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   406,   407,     0,     0,     0,
       0,     0,     0,   435,     0,   436,   408,   409,   410,   411,
     412,     0,     0,   413,   414,   415,   416,     0,   417,   418,
     419,   420,     0,     0,   421,     0,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   427,   428,   429,   430,   431,   432,   433,   434,
     406,   407,     0,     0,     0,     0,     0,     0,   435,     0,
     436,     0,     0,     0,     0,   408,   409,   410,   411,   412,
       0,     0,   413,   414,   415,   416,     0,   417,   418,   419,
     420,     0,     0,   421,     0,   422,   423,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,     0,
       0,     0,   428,   429,   430,   431,   432,   433,   434,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,   436,
     408,   409,     0,     0,   412,     0,     0,   413,   414,   415,
     416,     0,   417,   418,   419,   420,     0,     0,   421,     0,
     422,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,   425,   426,   427,   428,   429,     0,
       0,     0,   433,   434,     0,     0,   408,   409,   410,   411,
     412,     0,   435,   413,   436,     0,   416,     0,   417,   418,
     419,   420,     0,     0,   421,     0,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,   433,   434,
       0,   408,   409,   410,   411,     0,     0,     0,   435,     0,
     436,   416,     0,   417,   418,   419,   420,     0,     0,   421,
       0,   422,   423,     0,     0,     0,     0,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,     0,     0,     0,     0,
     430,   431,   432,   433,   434,   134,   135,   136,   137,     0,
       0,     0,     0,   435,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,     0,
       0,   148,   149
};

static const yytype_int16 yycheck[] =
{
      13,   108,   339,   366,   341,   157,   343,   264,   364,   268,
     446,   391,   448,     0,   450,   297,    73,   108,   460,     6,
     376,   266,   438,    18,     7,    33,    14,    15,    46,    32,
     543,   116,   111,    19,   124,   274,   113,   114,    20,    21,
     134,   125,    29,    19,    31,    19,    33,   140,    35,   148,
     148,   134,   134,    40,   134,    20,    21,   165,   134,    46,
     226,    74,   141,   171,   134,    52,   134,    21,   405,   134,
     141,   110,   171,   171,   113,   114,   134,    32,   171,   173,
     319,    68,   172,   167,   140,   140,   171,   164,   170,   147,
     173,   170,   140,   173,   510,   108,   166,   173,   140,    53,
     171,    56,    89,    14,    15,   173,    34,   171,   173,   173,
     165,   547,   170,   469,   134,   171,   134,   128,   225,   167,
     102,   103,   478,   162,   134,   164,   134,   134,   110,   171,
     112,   113,   114,   115,   225,   172,   118,   102,   103,   134,
     128,   154,   125,   146,   132,   110,   134,   135,   113,   114,
     115,   507,   143,   118,   140,   668,   263,   112,    57,    32,
     602,   164,   170,   220,   140,   111,   140,     4,     5,   276,
       7,   174,   263,   119,   102,   103,   283,   164,   166,   170,
     162,   164,   164,    56,   171,   276,   141,   142,   295,   296,
     173,   146,   283,   143,   149,   141,   134,   162,   152,   164,
     459,   171,   111,   173,   295,   296,   134,   140,   162,   164,
     136,   137,   225,   140,    20,    21,   171,   128,   140,   174,
     141,   132,   172,   134,   135,   332,   333,   334,   335,   143,
     111,   338,   141,   340,   167,   342,   388,   344,   390,   112,
     167,   332,   333,   334,   335,   167,   662,   338,   140,   340,
     263,   342,   140,   535,   140,   362,   672,   148,   503,   140,
     141,   140,   143,   276,   620,   146,   148,   134,   141,   142,
     283,   362,   688,   146,   140,   631,   149,   140,   385,   167,
     171,   167,   295,   296,   134,   665,   165,    20,    21,   171,
     134,   164,   170,   673,   385,   143,   102,   103,   171,   165,
     148,   174,   134,   134,   110,   471,   112,   113,   114,   115,
     167,   727,   118,    63,   171,   166,   468,    67,   166,   332,
     333,   334,   335,   172,    32,   338,   165,   340,   128,   342,
      32,   344,   171,   769,    84,    85,    86,    87,   143,   134,
     597,    32,   134,   148,   681,   151,   152,   153,    56,   362,
     143,    11,   134,   165,    56,   148,   162,   141,   164,   171,
     134,   166,    22,    23,   172,    56,   379,   100,   101,   102,
     103,   104,   385,   166,   107,   108,   109,   110,   143,   112,
     113,   114,   115,   148,   171,   118,   173,   120,   121,   402,
      34,   140,   168,   169,   143,   172,   172,   146,   136,   137,
     138,   166,    34,   111,   112,   166,   513,   514,   143,   516,
     112,   119,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   112,   513,   514,   787,   516,   239,     1,   171,   162,
     173,   164,   136,   141,   142,    53,   172,   134,   146,   141,
     142,   149,   134,   143,   146,    32,   259,   149,   172,   171,
     141,   142,   618,   147,   147,   146,   164,   148,   149,   147,
     626,   147,   164,   171,   166,   166,   174,   147,   147,    56,
     147,   637,   174,   164,   147,   582,   147,   147,   143,   134,
     293,   294,   171,   174,   297,   172,   593,   171,   595,    32,
     642,   582,   305,   306,   307,   308,   309,   310,   757,   134,
     513,   514,   593,   516,   125,   134,   147,   166,   166,   147,
     166,   147,   169,   147,   164,   166,     7,   165,   167,   165,
     686,    43,   134,   134,   171,   112,   134,   134,   116,   171,
     128,    36,   171,    19,   171,   140,   140,   165,   171,   136,
     171,   171,   134,   166,   147,   134,   173,    20,    21,   134,
      13,   364,   148,   366,   141,   142,   128,   148,   148,   146,
     143,   148,   149,   376,    39,   731,   172,   380,   147,   582,
     383,   134,   166,   680,   134,   134,   389,   164,   391,   165,
     593,   165,   595,   165,   165,   165,   148,   174,    49,   680,
     167,   147,   165,   148,   165,   408,   409,   173,   167,   412,
     413,   414,   415,   147,   417,   395,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   788,   436,   165,    32,   165,   100,   101,   102,
     103,   104,   167,   167,   107,   108,   109,   110,    55,   112,
     113,   114,   115,    97,    58,   118,   838,   120,   121,    56,
     816,    56,   465,   819,    74,     1,   469,   646,   236,   472,
     473,   274,   345,   738,   477,   478,   501,   680,   775,   142,
     483,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   642,   595,   170,   775,    68,   671,   372,   795,   162,
     283,   164,   337,   391,   507,   270,   831,    -1,   511,   172,
      -1,    -1,    -1,    -1,   795,   112,    -1,   520,    -1,   522,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   531,    -1,
      -1,   534,   535,    -1,    -1,   538,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,   141,   142,    -1,    -1,    -1,   146,
      -1,   148,   149,    -1,   851,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   866,
     851,    -1,   775,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   866,    -1,    -1,    -1,    -1,
      -1,    -1,   795,    -1,    -1,    -1,   599,    -1,    -1,    -1,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   615,   616,   617,    -1,    -1,   620,    -1,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,   631,    -1,
     833,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   644,   645,    12,   847,    32,    -1,    -1,   851,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
     663,    30,    32,   866,    -1,    -1,   669,    -1,   671,    56,
     149,   150,   151,   152,   153,    -1,    45,    -1,    -1,    -1,
      -1,   684,    -1,   162,    -1,   164,    56,    -1,   691,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   742,
     743,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     753,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,
      -1,   148,   149,    -1,    -1,   134,    -1,    -1,    -1,   772,
      -1,   141,   142,    -1,    -1,    -1,   146,   164,   148,   149,
      -1,    -1,    -1,    -1,   787,    -1,    -1,   174,   791,    -1,
      -1,    -1,    -1,   796,   164,   798,   799,   800,    -1,    -1,
     803,   170,   805,   806,   174,    -1,    -1,     1,   811,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,   842,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,    -1,
      44,   854,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    -1,    -1,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,    -1,    -1,
      -1,    -1,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   168,   169,   170,   171,   172,   173,
       1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      41,    42,    -1,    44,    -1,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,    -1,    -1,    -1,    -1,   156,   157,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,   166,    -1,   168,   169,   170,
     171,   172,   173,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,   149,   150,   151,    -1,
      -1,    -1,    -1,   156,   157,    -1,    14,    15,    16,    17,
      -1,   164,   165,   166,    -1,   168,   169,   170,    26,   172,
      -1,   174,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    -1,   156,   157,
      -1,    14,    15,    16,    17,    -1,   164,    -1,   166,    -1,
     168,   169,   170,    26,   172,    -1,   174,    -1,    32,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,   146,   146,   148,   149,   149,   150,   151,    -1,
      -1,    -1,    -1,   156,   157,    -1,    14,    15,    16,    17,
     164,   164,    -1,   166,    -1,   168,   169,   170,    26,   172,
     174,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,   149,   150,   151,    14,    15,    16,    17,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   166,    -1,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,   112,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,   141,   142,    -1,    -1,    -1,   146,    -1,   148,   149,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,    -1,    -1,    -1,    -1,   156,   157,    -1,    14,
      15,    16,    17,    -1,   164,    -1,   166,   167,   168,   169,
     170,    26,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,   149,   150,   151,    -1,    -1,    -1,
      26,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    38,   168,   169,   170,    42,   172,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,   147,    -1,   149,   150,   151,    -1,    -1,    -1,    26,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    38,   168,   169,   170,    42,   172,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,
     157,    32,    -1,    32,    -1,    -1,    -1,    -1,   165,   166,
      38,   168,   169,   170,    42,   172,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    56,    -1,    56,    -1,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
     111,   112,    -1,   112,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
     141,   142,   141,   142,    -1,   146,    56,   146,   149,   148,
     149,   149,   150,   151,    14,    15,    16,    17,   156,   157,
      -1,    21,    -1,   164,    -1,   164,    26,    -1,   166,    -1,
     168,   169,   170,   174,   172,   174,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,   112,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,   141,   142,    -1,    -1,    -1,   146,    -1,   148,   149,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   149,
     150,   151,    -1,    -1,    -1,    26,   156,   157,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    38,   168,   169,
     170,    42,   172,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    56,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,   141,   142,    -1,
      -1,    -1,   146,    56,   148,   149,    -1,    -1,   149,   150,
     151,    14,    15,    16,    17,   156,   157,    -1,    -1,    -1,
     164,    -1,    -1,    26,    -1,   166,   167,   168,   169,   170,
     174,   172,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    59,    60,    61,   112,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,   141,   142,
      -1,    -1,    -1,   146,    -1,   148,   149,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,   149,   150,   151,    -1,
      -1,    -1,    26,   156,   157,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    38,   168,   169,   170,    42,   172,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,
      56,   148,   149,    -1,    -1,   149,   150,   151,    14,    15,
      16,    17,   156,   157,    -1,    -1,    -1,   164,    -1,    -1,
      26,    -1,   166,   167,   168,   169,   170,   174,   172,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,   112,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,   141,   142,    -1,    -1,    -1,
     146,    -1,   148,   149,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   174,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,   149,   150,   151,    14,    15,    16,    17,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     166,   167,   168,   169,   170,    -1,   172,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,   112,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,    -1,
     148,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   174,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,   149,   150,   151,    14,    15,    16,    17,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   166,   167,
     168,   169,   170,    -1,   172,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,   112,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,   141,   142,    -1,    -1,    -1,   146,    -1,   148,   149,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   149,
     150,   151,    -1,    -1,    -1,    26,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    38,   168,   169,
     170,    42,   172,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,   149,   150,
     151,    -1,    -1,    -1,    26,   156,   157,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    38,   168,   169,   170,
      42,   172,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,   141,   142,    -1,    -1,
      -1,   146,    56,   148,   149,    -1,    -1,   149,   150,   151,
      14,    15,    16,    17,   156,   157,    -1,    -1,    -1,   164,
      -1,    -1,    26,    -1,   166,   167,   168,   169,   170,   174,
     172,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,   112,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,   149,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,    14,    15,
      16,    17,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   166,   167,   168,   169,   170,    -1,   172,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,   149,   150,   151,    -1,    -1,    -1,    26,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    38,   168,   169,   170,    42,   172,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      38,   168,   169,   170,    42,   172,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    38,
     168,   169,   170,    42,   172,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     149,   150,   151,    24,    -1,    -1,    -1,   156,   157,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,
     169,   170,    -1,   172,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    20,    21,   118,   119,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,   171,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    20,    21,   118,   119,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    20,    21,   118,   119,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    20,    21,   118,
     119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   162,
      -1,   164,   100,   101,   102,   103,   104,    -1,   171,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   100,   101,   102,   103,   104,    -1,
      -1,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   100,
     101,   102,   103,   104,    -1,   171,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,   162,    -1,   164,    -1,    -1,   167,    -1,   100,   101,
     102,   103,   104,    -1,    -1,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     162,    -1,   164,    -1,    -1,   167,   100,   101,   102,   103,
     104,    -1,    -1,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   162,    -1,
     164,    -1,    -1,   167,   100,   101,   102,   103,   104,    -1,
      -1,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,
      -1,   167,   100,   101,   102,   103,   104,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,   167,
     100,   101,   102,   103,   104,    -1,    -1,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,    -1,    -1,   167,   100,   101,
     102,   103,   104,    -1,    -1,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,    -1,    -1,   167,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    20,    21,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    20,    21,   170,   100,   101,   102,   103,
     104,    -1,    -1,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,   142,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,   100,   101,   102,   103,   104,    -1,    -1,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,    20,
      21,    -1,   100,   101,   102,   103,   104,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,   122,   123,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,   119,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   100,   101,
     102,   103,   104,    -1,    -1,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,    -1,    -1,    -1,    -1,   100,   101,   102,
     103,   104,    -1,    -1,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,   164,   100,   101,   102,   103,   104,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,    -1,    -1,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,
      -1,   100,   101,   102,   103,   104,    -1,    -1,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,   164,   100,   101,   102,   103,
     104,    -1,    -1,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,   151,   152,   153,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,    -1,    -1,    -1,    -1,   100,   101,   102,   103,   104,
      -1,    -1,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,
     100,   101,    -1,    -1,   104,    -1,    -1,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,   145,   146,   147,   148,    -1,
      -1,    -1,   152,   153,    -1,    -1,   100,   101,   102,   103,
     104,    -1,   162,   107,   164,    -1,   110,    -1,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,   100,   101,   102,   103,    -1,    -1,    -1,   162,    -1,
     164,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,   156,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   176,     0,     6,    29,    31,    33,    35,    40,    46,
      52,    68,    89,   164,   171,   177,   182,   183,   187,   206,
     210,   225,   275,   280,   281,   285,   321,   323,   134,   201,
     202,   128,   188,   189,   134,   184,   185,   134,   172,   134,
     324,   322,    33,   134,   170,   203,   204,   205,   216,     4,
       5,     7,   284,    57,   274,   141,   140,   143,   140,    21,
      53,   152,   162,   186,   143,   172,   134,   278,   279,   134,
     134,   170,   134,   166,   140,   165,    34,   102,   103,   134,
     209,   211,   134,   283,   172,   276,    14,    15,   128,   132,
     134,   135,   166,   179,   201,   128,   189,   134,   134,   134,
      63,    67,    84,    85,    86,    87,   289,   277,   141,   173,
     278,   172,   172,   134,   202,   204,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   120,   121,   122,   123,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   156,   157,
      34,    34,   166,   207,   143,   282,   286,   273,     1,   136,
     178,    14,    15,   128,   132,   134,   179,   199,   200,   186,
     172,   134,   173,    24,    30,    45,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   216,   288,
     290,   291,   293,   296,   134,   292,   134,   266,   267,   167,
     125,   167,   258,   262,   263,   143,   208,   216,   172,   134,
     173,   272,   287,   171,   136,   137,   140,   167,   277,   141,
     171,   147,   147,   147,   147,   166,   147,   147,   147,   147,
     147,   147,    32,    56,   112,   141,   142,   146,   149,   164,
     171,   174,   171,   143,   171,   202,    46,   269,   287,   167,
     171,   296,   172,   212,   261,   140,   143,   146,   271,   199,
     173,    14,    15,    16,    17,    26,    38,    42,    44,    47,
      51,    68,    90,   102,   103,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   135,   149,   150,   151,   156,   157,
     166,   168,   169,   181,   212,   216,   218,   227,   234,   243,
     244,   245,   248,   249,   250,   255,   288,   325,   328,   333,
     335,   337,   299,   301,   303,   297,   134,   294,   305,   307,
     309,   311,   313,   315,   317,   319,    32,    32,   146,   164,
     174,   165,   255,   134,   173,   296,   173,   266,   165,   269,
     111,   141,   143,   146,   268,   262,   214,   173,   206,   258,
     260,   134,   296,   111,   119,   141,   270,   296,   325,   166,
     166,   147,   147,   166,   147,   147,   255,   255,    12,   296,
      12,   296,   255,   330,   334,   180,   255,   255,   255,   255,
     255,   255,   169,   134,   166,   206,    20,    21,   100,   101,
     102,   103,   104,   107,   108,   109,   110,   112,   113,   114,
     115,   118,   120,   121,   142,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   162,   164,   171,   166,   296,
     296,   296,   296,   167,   292,   296,   207,   296,   207,   296,
     207,   134,   264,   265,   296,   267,   165,   165,   165,   296,
     117,   215,   246,   255,     1,     8,     9,    10,    12,    25,
      27,    28,    37,    41,    48,    50,    54,    55,    91,   171,
     173,   190,   191,   194,   195,   196,   197,   198,   213,   215,
     217,   219,   220,   221,   222,   223,   225,   226,   246,   247,
     255,     7,    43,   259,   171,   141,   171,   270,   215,   255,
     166,   216,   255,   228,   230,   255,   232,   296,   287,   134,
     149,   165,   166,   255,   326,   327,   329,   330,   331,   332,
     287,   166,   327,   332,   124,   171,   173,   137,   138,   178,
     167,   147,   216,   164,   167,   241,   255,   207,    18,   134,
     134,   255,   255,   255,   255,   255,   255,   134,   255,   134,
     255,   255,   255,   255,   255,    21,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   134,   255,   167,
     241,   148,   171,   148,   148,   148,   171,   148,   208,   148,
     208,   148,   208,   143,   148,   171,   148,   171,   141,   268,
     169,   244,   116,    92,    93,    94,    95,    96,    97,    98,
      99,   105,   106,   111,   119,   122,   123,   141,   255,   287,
     111,   171,   215,   255,   212,   255,   255,   171,   128,    54,
     255,   111,   215,   255,    36,   171,   171,   255,   171,   171,
     171,    19,   224,   171,   111,   141,   259,   269,   215,   255,
     171,   167,   241,    18,   147,   255,   167,   296,   296,   167,
     296,   148,    19,   268,   165,   167,   140,   165,   171,   140,
     165,   171,    19,   167,   173,   173,   255,   330,   173,   255,
     251,   253,   134,   326,   140,   167,   208,   147,   166,   165,
     134,   143,   165,   167,   300,   296,   304,   298,   295,   306,
     148,   310,   148,   314,   148,   296,   318,   264,   320,   255,
     244,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   212,    19,   215,   255,
     171,    13,   212,   143,   128,   215,   255,   171,   172,   212,
      39,   272,   117,   117,   211,   171,   167,   147,   134,   167,
     148,   148,   148,   166,   241,   255,   327,   134,   165,   326,
     255,   165,   331,   241,   327,   165,   165,   139,   296,   207,
     165,   255,   124,   212,   242,   256,   167,   241,   255,   148,
     308,   312,   316,   241,   171,   212,   171,   214,    11,    22,
      23,   192,   193,   255,   255,   235,   148,   171,   229,   231,
     233,   167,   255,   171,   165,   268,   171,   173,   148,   208,
     167,   111,   255,   296,   167,   302,   171,   173,   212,   255,
     296,    18,   255,   134,   255,   255,   255,   167,   255,   165,
     255,   255,   165,   252,   148,   255,   148,   212,   212,   148,
     147,   167,   148,   171,   336,   336,   216,   254,   257,   193,
     236,   237,    18,   255,    49,   165,   173,   216,   167,   296,
     147,   167,   255,   165,   165,   148,   239,   238,   296,   167,
     148,   240,   167
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   175,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   177,   178,   178,   179,   180,
     180,   180,   181,   182,   183,   184,   184,   184,   185,   185,
     186,   186,   187,   188,   188,   189,   189,   190,   191,   191,
     192,   192,   193,   193,   193,   194,   194,   195,   196,   197,
     198,   199,   199,   199,   199,   199,   199,   200,   200,   201,
     201,   201,   201,   201,   201,   201,   201,   202,   202,   203,
     203,   204,   204,   205,   205,   206,   206,   207,   207,   207,
     208,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   210,
     211,   212,   212,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   214,   214,   215,   215,   216,   216,   216,   217,
     218,   218,   218,   218,   219,   220,   221,   221,   221,   221,
     221,   222,   222,   222,   222,   223,   224,   224,   225,   225,
     226,   228,   229,   227,   230,   231,   227,   232,   233,   227,
     234,   234,   234,   235,   236,   234,   237,   238,   234,   239,
     240,   234,   241,   241,   242,   242,   242,   243,   243,   244,
     244,   245,   245,   245,   245,   245,   245,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   247,   247,   248,   249,   249,   250,   251,
     252,   250,   253,   254,   250,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   256,   257,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     258,   258,   259,   259,   260,   261,   261,   261,   262,   262,
     263,   263,   264,   264,   265,   265,   266,   267,   267,   268,
     268,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     271,   271,   272,   272,   272,   272,   272,   273,   273,   274,
     274,   275,   276,   275,   277,   277,   277,   278,   279,   279,
     280,   281,   281,   282,   282,   283,   284,   284,   286,   285,
     287,   287,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   289,   289,   289,
     289,   289,   289,   290,   291,   291,   292,   292,   294,   295,
     293,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   297,   298,   296,
     296,   299,   300,   296,   301,   302,   296,   303,   304,   296,
     296,   305,   306,   296,   307,   308,   296,   296,   309,   310,
     296,   311,   312,   296,   296,   313,   314,   296,   315,   316,
     296,   317,   318,   296,   319,   320,   296,   322,   321,   324,
     323,   325,   325,   325,   325,   326,   326,   327,   327,   328,
     328,   328,   328,   328,   329,   329,   330,   331,   331,   332,
     332,   333,   333,   334,   334,   335,   336,   336,   337,   337
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     0,
       2,     4,     3,     2,     2,     1,     3,     3,     2,     4,
       0,     1,     2,     1,     3,     1,     3,     3,     3,     2,
       1,     1,     0,     2,     4,     1,     1,     4,     6,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     4,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       4,     3,     7,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     2,     2,
       2,     0,     2,     2,     3,     2,     1,     3,     2,     2,
       2,     4,     5,     2,     1,     1,     2,     3,     4,     2,
       3,     3,     4,     2,     3,     4,     0,     2,     1,     1,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       5,     8,    10,     0,     0,    10,     0,     0,    13,     0,
       0,    15,     1,     3,     1,     2,     3,     1,     1,     1,
       5,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     6,     5,     6,     3,     0,
       0,     8,     0,     0,     9,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       4,     3,     3,     1,     3,     4,     3,     4,     2,     4,
       4,     6,     7,     3,     5,     0,     0,     8,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     4,     0,     1,     3,     0,     3,     5,     2,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       1,     1,     2,     3,     5,     3,     3,     1,     1,     1,
       0,     1,     4,     6,     5,     5,     4,     0,     2,     0,
       1,     5,     0,     5,     0,     3,     5,     4,     1,     2,
       4,     5,     7,     0,     2,     2,     1,     1,     0,     7,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     3,     0,     0,
       6,     1,     1,     1,     1,     4,     3,     4,     2,     3,
       2,     3,     2,     2,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     8,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     0,     7,     0,
       7,     1,     1,     1,     1,     3,     5,     1,     3,     5,
       4,     7,     5,     7,     1,     3,     3,     1,     1,     1,
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
    case 134: /* "name"  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3211 "ds_parser.cpp"
        break;

    case 178: /* character_sequence  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3217 "ds_parser.cpp"
        break;

    case 179: /* string_constant  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3223 "ds_parser.cpp"
        break;

    case 180: /* string_builder_body  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3229 "ds_parser.cpp"
        break;

    case 181: /* string_builder  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3235 "ds_parser.cpp"
        break;

    case 184: /* require_module_name  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3241 "ds_parser.cpp"
        break;

    case 190: /* expression_label  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3247 "ds_parser.cpp"
        break;

    case 191: /* expression_goto  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3253 "ds_parser.cpp"
        break;

    case 193: /* expression_else  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3259 "ds_parser.cpp"
        break;

    case 195: /* expression_if_then_else  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3265 "ds_parser.cpp"
        break;

    case 196: /* expression_for_loop  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3271 "ds_parser.cpp"
        break;

    case 197: /* expression_while_loop  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3277 "ds_parser.cpp"
        break;

    case 198: /* expression_with  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3283 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3289 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument_value_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3295 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3301 "ds_parser.cpp"
        break;

    case 202: /* annotation_argument_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3307 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration_name  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3313 "ds_parser.cpp"
        break;

    case 204: /* annotation_declaration  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3319 "ds_parser.cpp"
        break;

    case 205: /* annotation_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3325 "ds_parser.cpp"
        break;

    case 206: /* optional_annotation_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3331 "ds_parser.cpp"
        break;

    case 207: /* optional_function_argument_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3337 "ds_parser.cpp"
        break;

    case 208: /* optional_function_type  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3343 "ds_parser.cpp"
        break;

    case 209: /* function_name  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3349 "ds_parser.cpp"
        break;

    case 212: /* expression_block  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3355 "ds_parser.cpp"
        break;

    case 213: /* expression_any  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3361 "ds_parser.cpp"
        break;

    case 214: /* expressions  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3367 "ds_parser.cpp"
        break;

    case 215: /* expr_pipe  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3373 "ds_parser.cpp"
        break;

    case 216: /* name_in_namespace  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3379 "ds_parser.cpp"
        break;

    case 217: /* expression_delete  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3385 "ds_parser.cpp"
        break;

    case 218: /* expr_new  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3391 "ds_parser.cpp"
        break;

    case 219: /* expression_break  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3397 "ds_parser.cpp"
        break;

    case 220: /* expression_continue  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3403 "ds_parser.cpp"
        break;

    case 221: /* expression_return  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3409 "ds_parser.cpp"
        break;

    case 222: /* expression_yield  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3415 "ds_parser.cpp"
        break;

    case 223: /* expression_try_catch  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3421 "ds_parser.cpp"
        break;

    case 226: /* expression_let  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3427 "ds_parser.cpp"
        break;

    case 227: /* expr_cast  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3433 "ds_parser.cpp"
        break;

    case 234: /* expr_type_info  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3439 "ds_parser.cpp"
        break;

    case 241: /* expr_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3445 "ds_parser.cpp"
        break;

    case 242: /* block_or_simple_block  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3451 "ds_parser.cpp"
        break;

    case 244: /* expr_block  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3457 "ds_parser.cpp"
        break;

    case 245: /* expr_numeric_const  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3463 "ds_parser.cpp"
        break;

    case 246: /* expr_assign  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3469 "ds_parser.cpp"
        break;

    case 247: /* expr_assign_pipe  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3475 "ds_parser.cpp"
        break;

    case 248: /* expr_named_call  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3481 "ds_parser.cpp"
        break;

    case 249: /* expr_method_call  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3487 "ds_parser.cpp"
        break;

    case 250: /* func_addr_expr  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3493 "ds_parser.cpp"
        break;

    case 255: /* expr  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3499 "ds_parser.cpp"
        break;

    case 258: /* optional_field_annotation  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3505 "ds_parser.cpp"
        break;

    case 260: /* structure_variable_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3511 "ds_parser.cpp"
        break;

    case 261: /* struct_variable_declaration_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3517 "ds_parser.cpp"
        break;

    case 262: /* function_argument_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3523 "ds_parser.cpp"
        break;

    case 263: /* function_argument_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3529 "ds_parser.cpp"
        break;

    case 264: /* tuple_type  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3535 "ds_parser.cpp"
        break;

    case 265: /* tuple_type_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3541 "ds_parser.cpp"
        break;

    case 266: /* variant_type  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3547 "ds_parser.cpp"
        break;

    case 267: /* variant_type_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3553 "ds_parser.cpp"
        break;

    case 269: /* variable_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3559 "ds_parser.cpp"
        break;

    case 272: /* let_variable_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3565 "ds_parser.cpp"
        break;

    case 273: /* global_variable_declaration_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3571 "ds_parser.cpp"
        break;

    case 277: /* enum_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3577 "ds_parser.cpp"
        break;

    case 282: /* optional_structure_parent  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3583 "ds_parser.cpp"
        break;

    case 287: /* variable_name_with_pos_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3589 "ds_parser.cpp"
        break;

    case 290: /* structure_type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3595 "ds_parser.cpp"
        break;

    case 291: /* auto_type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3601 "ds_parser.cpp"
        break;

    case 292: /* bitfield_bits  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3607 "ds_parser.cpp"
        break;

    case 293: /* bitfield_type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3613 "ds_parser.cpp"
        break;

    case 296: /* type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3619 "ds_parser.cpp"
        break;

    case 325: /* make_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3625 "ds_parser.cpp"
        break;

    case 326: /* make_struct_fields  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3631 "ds_parser.cpp"
        break;

    case 327: /* make_struct_dim  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3637 "ds_parser.cpp"
        break;

    case 328: /* make_struct_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3643 "ds_parser.cpp"
        break;

    case 329: /* make_tuple  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3649 "ds_parser.cpp"
        break;

    case 330: /* make_map_tuple  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3655 "ds_parser.cpp"
        break;

    case 331: /* make_any_tuple  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3661 "ds_parser.cpp"
        break;

    case 332: /* make_dim  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3667 "ds_parser.cpp"
        break;

    case 333: /* make_dim_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3673 "ds_parser.cpp"
        break;

    case 334: /* make_table  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3679 "ds_parser.cpp"
        break;

    case 335: /* make_table_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3685 "ds_parser.cpp"
        break;

    case 336: /* array_comprehension_where  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3691 "ds_parser.cpp"
        break;

    case 337: /* array_comprehension  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3697 "ds_parser.cpp"
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
#line 422 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3997 "ds_parser.cpp"
    break;

  case 16:
#line 434 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4003 "ds_parser.cpp"
    break;

  case 17:
#line 435 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4009 "ds_parser.cpp"
    break;

  case 18:
#line 439 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4015 "ds_parser.cpp"
    break;

  case 19:
#line 443 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4025 "ds_parser.cpp"
    break;

  case 20:
#line 448 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4039 "ds_parser.cpp"
    break;

  case 21:
#line 457 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4049 "ds_parser.cpp"
    break;

  case 22:
#line 465 "ds_parser.ypp"
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
#line 4067 "ds_parser.cpp"
    break;

  case 23:
#line 481 "ds_parser.ypp"
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
#line 4091 "ds_parser.cpp"
    break;

  case 25:
#line 507 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4099 "ds_parser.cpp"
    break;

  case 26:
#line 510 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4110 "ds_parser.cpp"
    break;

  case 27:
#line 516 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4121 "ds_parser.cpp"
    break;

  case 28:
#line 525 "ds_parser.ypp"
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
#line 4147 "ds_parser.cpp"
    break;

  case 29:
#line 546 "ds_parser.ypp"
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
#line 4172 "ds_parser.cpp"
    break;

  case 30:
#line 569 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4178 "ds_parser.cpp"
    break;

  case 31:
#line 570 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4184 "ds_parser.cpp"
    break;

  case 35:
#line 583 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4192 "ds_parser.cpp"
    break;

  case 36:
#line 586 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4200 "ds_parser.cpp"
    break;

  case 37:
#line 592 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4208 "ds_parser.cpp"
    break;

  case 38:
#line 598 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4216 "ds_parser.cpp"
    break;

  case 39:
#line 601 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4224 "ds_parser.cpp"
    break;

  case 40:
#line 607 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4230 "ds_parser.cpp"
    break;

  case 41:
#line 608 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4236 "ds_parser.cpp"
    break;

  case 42:
#line 612 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4242 "ds_parser.cpp"
    break;

  case 43:
#line 613 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4248 "ds_parser.cpp"
    break;

  case 44:
#line 614 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4259 "ds_parser.cpp"
    break;

  case 45:
#line 623 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4265 "ds_parser.cpp"
    break;

  case 46:
#line 624 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4271 "ds_parser.cpp"
    break;

  case 47:
#line 628 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4282 "ds_parser.cpp"
    break;

  case 48:
#line 637 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
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
#line 4299 "ds_parser.cpp"
    break;

  case 49:
#line 652 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4311 "ds_parser.cpp"
    break;

  case 50:
#line 662 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4322 "ds_parser.cpp"
    break;

  case 51:
#line 671 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4328 "ds_parser.cpp"
    break;

  case 52:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4334 "ds_parser.cpp"
    break;

  case 53:
#line 673 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4340 "ds_parser.cpp"
    break;

  case 54:
#line 674 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4346 "ds_parser.cpp"
    break;

  case 55:
#line 675 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4352 "ds_parser.cpp"
    break;

  case 56:
#line 676 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4358 "ds_parser.cpp"
    break;

  case 57:
#line 680 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4368 "ds_parser.cpp"
    break;

  case 58:
#line 685 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4378 "ds_parser.cpp"
    break;

  case 59:
#line 693 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4384 "ds_parser.cpp"
    break;

  case 60:
#line 694 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4390 "ds_parser.cpp"
    break;

  case 61:
#line 695 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4396 "ds_parser.cpp"
    break;

  case 62:
#line 696 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4402 "ds_parser.cpp"
    break;

  case 63:
#line 697 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4408 "ds_parser.cpp"
    break;

  case 64:
#line 698 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4414 "ds_parser.cpp"
    break;

  case 65:
#line 699 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4420 "ds_parser.cpp"
    break;

  case 66:
#line 700 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4428 "ds_parser.cpp"
    break;

  case 67:
#line 706 "ds_parser.ypp"
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
#line 4446 "ds_parser.cpp"
    break;

  case 68:
#line 719 "ds_parser.ypp"
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
#line 4464 "ds_parser.cpp"
    break;

  case 69:
#line 735 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4470 "ds_parser.cpp"
    break;

  case 70:
#line 736 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4476 "ds_parser.cpp"
    break;

  case 71:
#line 740 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4488 "ds_parser.cpp"
    break;

  case 72:
#line 747 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4502 "ds_parser.cpp"
    break;

  case 73:
#line 759 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4511 "ds_parser.cpp"
    break;

  case 74:
#line 763 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4520 "ds_parser.cpp"
    break;

  case 75:
#line 770 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4526 "ds_parser.cpp"
    break;

  case 76:
#line 771 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4532 "ds_parser.cpp"
    break;

  case 77:
#line 775 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4538 "ds_parser.cpp"
    break;

  case 78:
#line 776 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4544 "ds_parser.cpp"
    break;

  case 79:
#line 777 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4550 "ds_parser.cpp"
    break;

  case 80:
#line 781 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4560 "ds_parser.cpp"
    break;

  case 81:
#line 786 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4569 "ds_parser.cpp"
    break;

  case 82:
#line 793 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4578 "ds_parser.cpp"
    break;

  case 83:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4584 "ds_parser.cpp"
    break;

  case 84:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4590 "ds_parser.cpp"
    break;

  case 85:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4596 "ds_parser.cpp"
    break;

  case 86:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4602 "ds_parser.cpp"
    break;

  case 87:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4608 "ds_parser.cpp"
    break;

  case 88:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4614 "ds_parser.cpp"
    break;

  case 89:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4620 "ds_parser.cpp"
    break;

  case 90:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4626 "ds_parser.cpp"
    break;

  case 91:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4632 "ds_parser.cpp"
    break;

  case 92:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4638 "ds_parser.cpp"
    break;

  case 93:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4644 "ds_parser.cpp"
    break;

  case 94:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4650 "ds_parser.cpp"
    break;

  case 95:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4656 "ds_parser.cpp"
    break;

  case 96:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4662 "ds_parser.cpp"
    break;

  case 97:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4668 "ds_parser.cpp"
    break;

  case 98:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4674 "ds_parser.cpp"
    break;

  case 99:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4680 "ds_parser.cpp"
    break;

  case 100:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4686 "ds_parser.cpp"
    break;

  case 101:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4692 "ds_parser.cpp"
    break;

  case 102:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4698 "ds_parser.cpp"
    break;

  case 103:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4704 "ds_parser.cpp"
    break;

  case 104:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4710 "ds_parser.cpp"
    break;

  case 105:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4716 "ds_parser.cpp"
    break;

  case 106:
#line 820 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4722 "ds_parser.cpp"
    break;

  case 107:
#line 821 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4728 "ds_parser.cpp"
    break;

  case 108:
#line 822 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4734 "ds_parser.cpp"
    break;

  case 109:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4740 "ds_parser.cpp"
    break;

  case 110:
#line 824 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4746 "ds_parser.cpp"
    break;

  case 111:
#line 825 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4752 "ds_parser.cpp"
    break;

  case 112:
#line 826 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4758 "ds_parser.cpp"
    break;

  case 113:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4764 "ds_parser.cpp"
    break;

  case 114:
#line 828 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4770 "ds_parser.cpp"
    break;

  case 115:
#line 829 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4776 "ds_parser.cpp"
    break;

  case 116:
#line 830 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4782 "ds_parser.cpp"
    break;

  case 117:
#line 831 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4788 "ds_parser.cpp"
    break;

  case 118:
#line 832 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4794 "ds_parser.cpp"
    break;

  case 119:
#line 836 "ds_parser.ypp"
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
#line 4817 "ds_parser.cpp"
    break;

  case 120:
#line 858 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-3]));
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
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
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
        delete (yyvsp[-3].s);
        (yyval.pFuncDecl) = pFunction.orphan();
    }
#line 4858 "ds_parser.cpp"
    break;

  case 121:
#line 897 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4866 "ds_parser.cpp"
    break;

  case 122:
#line 900 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4878 "ds_parser.cpp"
    break;

  case 123:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4884 "ds_parser.cpp"
    break;

  case 124:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4890 "ds_parser.cpp"
    break;

  case 125:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4896 "ds_parser.cpp"
    break;

  case 126:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4902 "ds_parser.cpp"
    break;

  case 127:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4908 "ds_parser.cpp"
    break;

  case 128:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4914 "ds_parser.cpp"
    break;

  case 129:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4920 "ds_parser.cpp"
    break;

  case 130:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4926 "ds_parser.cpp"
    break;

  case 131:
#line 918 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4932 "ds_parser.cpp"
    break;

  case 132:
#line 919 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4938 "ds_parser.cpp"
    break;

  case 133:
#line 920 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4944 "ds_parser.cpp"
    break;

  case 134:
#line 921 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4950 "ds_parser.cpp"
    break;

  case 135:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4956 "ds_parser.cpp"
    break;

  case 136:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4962 "ds_parser.cpp"
    break;

  case 137:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4968 "ds_parser.cpp"
    break;

  case 138:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4974 "ds_parser.cpp"
    break;

  case 139:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4980 "ds_parser.cpp"
    break;

  case 140:
#line 927 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4986 "ds_parser.cpp"
    break;

  case 141:
#line 931 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4996 "ds_parser.cpp"
    break;

  case 142:
#line 936 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5007 "ds_parser.cpp"
    break;

  case 143:
#line 942 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5015 "ds_parser.cpp"
    break;

  case 144:
#line 948 "ds_parser.ypp"
    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 5031 "ds_parser.cpp"
    break;

  case 145:
#line 959 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5039 "ds_parser.cpp"
    break;

  case 146:
#line 965 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5045 "ds_parser.cpp"
    break;

  case 147:
#line 966 "ds_parser.ypp"
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
#line 5061 "ds_parser.cpp"
    break;

  case 148:
#line 977 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5067 "ds_parser.cpp"
    break;

  case 149:
#line 981 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5075 "ds_parser.cpp"
    break;

  case 150:
#line 987 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5083 "ds_parser.cpp"
    break;

  case 151:
#line 990 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5091 "ds_parser.cpp"
    break;

  case 152:
#line 993 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5100 "ds_parser.cpp"
    break;

  case 153:
#line 997 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5108 "ds_parser.cpp"
    break;

  case 154:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5114 "ds_parser.cpp"
    break;

  case 155:
#line 1007 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5120 "ds_parser.cpp"
    break;

  case 156:
#line 1011 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5128 "ds_parser.cpp"
    break;

  case 157:
#line 1014 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5136 "ds_parser.cpp"
    break;

  case 158:
#line 1017 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5146 "ds_parser.cpp"
    break;

  case 159:
#line 1022 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5154 "ds_parser.cpp"
    break;

  case 160:
#line 1025 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5164 "ds_parser.cpp"
    break;

  case 161:
#line 1033 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5172 "ds_parser.cpp"
    break;

  case 162:
#line 1036 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5182 "ds_parser.cpp"
    break;

  case 163:
#line 1041 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5190 "ds_parser.cpp"
    break;

  case 164:
#line 1044 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5200 "ds_parser.cpp"
    break;

  case 165:
#line 1052 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5208 "ds_parser.cpp"
    break;

  case 166:
#line 1058 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5214 "ds_parser.cpp"
    break;

  case 167:
#line 1059 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5220 "ds_parser.cpp"
    break;

  case 168:
#line 1063 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5226 "ds_parser.cpp"
    break;

  case 169:
#line 1064 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5232 "ds_parser.cpp"
    break;

  case 170:
#line 1068 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
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
#line 5268 "ds_parser.cpp"
    break;

  case 171:
#line 1102 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5274 "ds_parser.cpp"
    break;

  case 172:
#line 1102 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5280 "ds_parser.cpp"
    break;

  case 173:
#line 1102 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5288 "ds_parser.cpp"
    break;

  case 174:
#line 1105 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5294 "ds_parser.cpp"
    break;

  case 175:
#line 1105 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5300 "ds_parser.cpp"
    break;

  case 176:
#line 1105 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5310 "ds_parser.cpp"
    break;

  case 177:
#line 1110 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5316 "ds_parser.cpp"
    break;

  case 178:
#line 1110 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5322 "ds_parser.cpp"
    break;

  case 179:
#line 1110 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5332 "ds_parser.cpp"
    break;

  case 180:
#line 1118 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5341 "ds_parser.cpp"
    break;

  case 181:
#line 1122 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5351 "ds_parser.cpp"
    break;

  case 182:
#line 1127 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5362 "ds_parser.cpp"
    break;

  case 183:
#line 1133 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5368 "ds_parser.cpp"
    break;

  case 184:
#line 1133 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5374 "ds_parser.cpp"
    break;

  case 185:
#line 1133 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5383 "ds_parser.cpp"
    break;

  case 186:
#line 1137 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5389 "ds_parser.cpp"
    break;

  case 187:
#line 1137 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5395 "ds_parser.cpp"
    break;

  case 188:
#line 1137 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5405 "ds_parser.cpp"
    break;

  case 189:
#line 1142 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5411 "ds_parser.cpp"
    break;

  case 190:
#line 1142 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5417 "ds_parser.cpp"
    break;

  case 191:
#line 1142 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5428 "ds_parser.cpp"
    break;

  case 192:
#line 1151 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5436 "ds_parser.cpp"
    break;

  case 193:
#line 1154 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5444 "ds_parser.cpp"
    break;

  case 194:
#line 1160 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5450 "ds_parser.cpp"
    break;

  case 195:
#line 1161 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5462 "ds_parser.cpp"
    break;

  case 196:
#line 1168 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5475 "ds_parser.cpp"
    break;

  case 197:
#line 1179 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5481 "ds_parser.cpp"
    break;

  case 198:
#line 1180 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5487 "ds_parser.cpp"
    break;

  case 199:
#line 1184 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5497 "ds_parser.cpp"
    break;

  case 200:
#line 1190 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
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
#line 5553 "ds_parser.cpp"
    break;

  case 201:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5559 "ds_parser.cpp"
    break;

  case 202:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5565 "ds_parser.cpp"
    break;

  case 203:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5571 "ds_parser.cpp"
    break;

  case 204:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5577 "ds_parser.cpp"
    break;

  case 205:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5583 "ds_parser.cpp"
    break;

  case 206:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5589 "ds_parser.cpp"
    break;

  case 207:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5595 "ds_parser.cpp"
    break;

  case 208:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5601 "ds_parser.cpp"
    break;

  case 209:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5607 "ds_parser.cpp"
    break;

  case 210:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5613 "ds_parser.cpp"
    break;

  case 211:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5619 "ds_parser.cpp"
    break;

  case 212:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5625 "ds_parser.cpp"
    break;

  case 213:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5631 "ds_parser.cpp"
    break;

  case 214:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5637 "ds_parser.cpp"
    break;

  case 215:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5643 "ds_parser.cpp"
    break;

  case 216:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5649 "ds_parser.cpp"
    break;

  case 217:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5655 "ds_parser.cpp"
    break;

  case 218:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5661 "ds_parser.cpp"
    break;

  case 219:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5667 "ds_parser.cpp"
    break;

  case 220:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5673 "ds_parser.cpp"
    break;

  case 221:
#line 1267 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5679 "ds_parser.cpp"
    break;

  case 222:
#line 1268 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5685 "ds_parser.cpp"
    break;

  case 223:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5691 "ds_parser.cpp"
    break;

  case 224:
#line 1273 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5697 "ds_parser.cpp"
    break;

  case 225:
#line 1277 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5709 "ds_parser.cpp"
    break;

  case 226:
#line 1288 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5719 "ds_parser.cpp"
    break;

  case 227:
#line 1293 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5731 "ds_parser.cpp"
    break;

  case 228:
#line 1303 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5740 "ds_parser.cpp"
    break;

  case 229:
#line 1307 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5746 "ds_parser.cpp"
    break;

  case 230:
#line 1307 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5752 "ds_parser.cpp"
    break;

  case 231:
#line 1307 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 5764 "ds_parser.cpp"
    break;

  case 232:
#line 1314 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5770 "ds_parser.cpp"
    break;

  case 233:
#line 1314 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5776 "ds_parser.cpp"
    break;

  case 234:
#line 1314 "ds_parser.ypp"
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
#line 5792 "ds_parser.cpp"
    break;

  case 235:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5798 "ds_parser.cpp"
    break;

  case 236:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5804 "ds_parser.cpp"
    break;

  case 237:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5810 "ds_parser.cpp"
    break;

  case 238:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5816 "ds_parser.cpp"
    break;

  case 239:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5822 "ds_parser.cpp"
    break;

  case 240:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5828 "ds_parser.cpp"
    break;

  case 241:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5834 "ds_parser.cpp"
    break;

  case 242:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5840 "ds_parser.cpp"
    break;

  case 243:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5846 "ds_parser.cpp"
    break;

  case 244:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5852 "ds_parser.cpp"
    break;

  case 245:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5858 "ds_parser.cpp"
    break;

  case 246:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5864 "ds_parser.cpp"
    break;

  case 247:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5870 "ds_parser.cpp"
    break;

  case 248:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5876 "ds_parser.cpp"
    break;

  case 249:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5882 "ds_parser.cpp"
    break;

  case 250:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5888 "ds_parser.cpp"
    break;

  case 251:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5894 "ds_parser.cpp"
    break;

  case 252:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5900 "ds_parser.cpp"
    break;

  case 253:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5906 "ds_parser.cpp"
    break;

  case 254:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5912 "ds_parser.cpp"
    break;

  case 255:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5918 "ds_parser.cpp"
    break;

  case 256:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5924 "ds_parser.cpp"
    break;

  case 257:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5930 "ds_parser.cpp"
    break;

  case 258:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5936 "ds_parser.cpp"
    break;

  case 259:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5942 "ds_parser.cpp"
    break;

  case 260:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5948 "ds_parser.cpp"
    break;

  case 261:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5954 "ds_parser.cpp"
    break;

  case 262:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5960 "ds_parser.cpp"
    break;

  case 263:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5966 "ds_parser.cpp"
    break;

  case 264:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5972 "ds_parser.cpp"
    break;

  case 265:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5978 "ds_parser.cpp"
    break;

  case 266:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5984 "ds_parser.cpp"
    break;

  case 267:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5990 "ds_parser.cpp"
    break;

  case 268:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5996 "ds_parser.cpp"
    break;

  case 269:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6002 "ds_parser.cpp"
    break;

  case 270:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6008 "ds_parser.cpp"
    break;

  case 271:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6014 "ds_parser.cpp"
    break;

  case 272:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6020 "ds_parser.cpp"
    break;

  case 273:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6026 "ds_parser.cpp"
    break;

  case 274:
#line 1367 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6035 "ds_parser.cpp"
    break;

  case 275:
#line 1371 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6044 "ds_parser.cpp"
    break;

  case 276:
#line 1375 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6052 "ds_parser.cpp"
    break;

  case 277:
#line 1378 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6060 "ds_parser.cpp"
    break;

  case 278:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6066 "ds_parser.cpp"
    break;

  case 279:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6072 "ds_parser.cpp"
    break;

  case 280:
#line 1383 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6078 "ds_parser.cpp"
    break;

  case 281:
#line 1384 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6088 "ds_parser.cpp"
    break;

  case 282:
#line 1389 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6098 "ds_parser.cpp"
    break;

  case 283:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6104 "ds_parser.cpp"
    break;

  case 284:
#line 1395 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6112 "ds_parser.cpp"
    break;

  case 285:
#line 1398 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6118 "ds_parser.cpp"
    break;

  case 286:
#line 1398 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6124 "ds_parser.cpp"
    break;

  case 287:
#line 1398 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6132 "ds_parser.cpp"
    break;

  case 288:
#line 1401 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6141 "ds_parser.cpp"
    break;

  case 289:
#line 1405 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6150 "ds_parser.cpp"
    break;

  case 290:
#line 1409 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6159 "ds_parser.cpp"
    break;

  case 291:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6165 "ds_parser.cpp"
    break;

  case 292:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6171 "ds_parser.cpp"
    break;

  case 293:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6177 "ds_parser.cpp"
    break;

  case 294:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6183 "ds_parser.cpp"
    break;

  case 295:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6189 "ds_parser.cpp"
    break;

  case 296:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6195 "ds_parser.cpp"
    break;

  case 297:
#line 1419 "ds_parser.ypp"
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
#line 6210 "ds_parser.cpp"
    break;

  case 298:
#line 1429 "ds_parser.ypp"
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
#line 6225 "ds_parser.cpp"
    break;

  case 299:
#line 1439 "ds_parser.ypp"
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
#line 6282 "ds_parser.cpp"
    break;

  case 300:
#line 1494 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6288 "ds_parser.cpp"
    break;

  case 301:
#line 1495 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6294 "ds_parser.cpp"
    break;

  case 302:
#line 1499 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6300 "ds_parser.cpp"
    break;

  case 303:
#line 1500 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6306 "ds_parser.cpp"
    break;

  case 304:
#line 1504 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6316 "ds_parser.cpp"
    break;

  case 305:
#line 1512 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6324 "ds_parser.cpp"
    break;

  case 306:
#line 1515 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6333 "ds_parser.cpp"
    break;

  case 307:
#line 1519 "ds_parser.ypp"
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt((yylsp[-2]),(yylsp[0]));
        if ( !g_thisStructure->isClass ) {
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
                Expression * finit = new ExprAddr((yyvsp[0].pFuncDecl)->at, "_::" + (yyvsp[0].pFuncDecl)->name);
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
#line 6388 "ds_parser.cpp"
    break;

  case 308:
#line 1572 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6398 "ds_parser.cpp"
    break;

  case 309:
#line 1577 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6408 "ds_parser.cpp"
    break;

  case 310:
#line 1585 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6414 "ds_parser.cpp"
    break;

  case 311:
#line 1586 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6420 "ds_parser.cpp"
    break;

  case 312:
#line 1590 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6428 "ds_parser.cpp"
    break;

  case 313:
#line 1593 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6439 "ds_parser.cpp"
    break;

  case 314:
#line 1602 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6445 "ds_parser.cpp"
    break;

  case 315:
#line 1603 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6451 "ds_parser.cpp"
    break;

  case 316:
#line 1607 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6462 "ds_parser.cpp"
    break;

  case 317:
#line 1616 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6468 "ds_parser.cpp"
    break;

  case 318:
#line 1617 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6474 "ds_parser.cpp"
    break;

  case 319:
#line 1622 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6480 "ds_parser.cpp"
    break;

  case 320:
#line 1623 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6486 "ds_parser.cpp"
    break;

  case 321:
#line 1627 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6497 "ds_parser.cpp"
    break;

  case 322:
#line 1633 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6508 "ds_parser.cpp"
    break;

  case 323:
#line 1639 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6516 "ds_parser.cpp"
    break;

  case 324:
#line 1642 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6525 "ds_parser.cpp"
    break;

  case 325:
#line 1646 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6536 "ds_parser.cpp"
    break;

  case 326:
#line 1652 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6547 "ds_parser.cpp"
    break;

  case 327:
#line 1661 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6553 "ds_parser.cpp"
    break;

  case 328:
#line 1662 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6559 "ds_parser.cpp"
    break;

  case 329:
#line 1663 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6565 "ds_parser.cpp"
    break;

  case 330:
#line 1667 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6571 "ds_parser.cpp"
    break;

  case 331:
#line 1668 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6577 "ds_parser.cpp"
    break;

  case 332:
#line 1672 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6585 "ds_parser.cpp"
    break;

  case 333:
#line 1675 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6595 "ds_parser.cpp"
    break;

  case 334:
#line 1680 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6605 "ds_parser.cpp"
    break;

  case 335:
#line 1685 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6618 "ds_parser.cpp"
    break;

  case 336:
#line 1693 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6631 "ds_parser.cpp"
    break;

  case 337:
#line 1704 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6639 "ds_parser.cpp"
    break;

  case 338:
#line 1707 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6648 "ds_parser.cpp"
    break;

  case 339:
#line 1714 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6654 "ds_parser.cpp"
    break;

  case 340:
#line 1715 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6660 "ds_parser.cpp"
    break;

  case 341:
#line 1719 "ds_parser.ypp"
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
#line 6692 "ds_parser.cpp"
    break;

  case 342:
#line 1746 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6701 "ds_parser.cpp"
    break;

  case 343:
#line 1749 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6712 "ds_parser.cpp"
    break;

  case 344:
#line 1758 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6720 "ds_parser.cpp"
    break;

  case 345:
#line 1761 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6734 "ds_parser.cpp"
    break;

  case 346:
#line 1770 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6748 "ds_parser.cpp"
    break;

  case 347:
#line 1783 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6762 "ds_parser.cpp"
    break;

  case 351:
#line 1804 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-4]));
        pEnum->name = *(yyvsp[-3].s);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6778 "ds_parser.cpp"
    break;

  case 352:
#line 1815 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-5].s),tokAt((yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-6]));
        pEnum->name = *(yyvsp[-5].s);
        pEnum->baseType = (yyvsp[-3].type);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-5].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-5].s);
    }
#line 6795 "ds_parser.cpp"
    break;

  case 353:
#line 1830 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6801 "ds_parser.cpp"
    break;

  case 354:
#line 1831 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6807 "ds_parser.cpp"
    break;

  case 355:
#line 1835 "ds_parser.ypp"
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
            } else if ( structs.size()==0 ) {
                das_yyerror("parent structure not found"+*(yyvsp[0].s),tokAt((yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = g_Program->describeCandidates(structs);
                das_yyerror("too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt((yylsp[0])),
                    CompilationError::structure_not_found);

            }
            delete (yyvsp[0].s);
        } else {
            pStruct = make_smart<Structure>(*(yyvsp[-1].s));
        }
        if ( pStruct ) {
            if ( !g_Program->addStructure(pStruct) ) {
                das_yyerror("structure is already defined "+*(yyvsp[-1].s),tokAt((yylsp[-1])),
                    CompilationError::structure_already_declared);
                (yyval.pStructure) = nullptr;
            } else {
                (yyval.pStructure) = pStruct.get();
                g_thisStructure = pStruct.get();
            }
        } else {
            (yyval.pStructure) = nullptr;
        }
        delete (yyvsp[-1].s);
    }
#line 6849 "ds_parser.cpp"
    break;

  case 356:
#line 1875 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6855 "ds_parser.cpp"
    break;

  case 357:
#line 1876 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6861 "ds_parser.cpp"
    break;

  case 358:
#line 1880 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 6867 "ds_parser.cpp"
    break;

  case 359:
#line 1880 "ds_parser.ypp"
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
#line 6953 "ds_parser.cpp"
    break;

  case 360:
#line 1964 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6965 "ds_parser.cpp"
    break;

  case 361:
#line 1971 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 6976 "ds_parser.cpp"
    break;

  case 362:
#line 1980 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6982 "ds_parser.cpp"
    break;

  case 363:
#line 1981 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6988 "ds_parser.cpp"
    break;

  case 364:
#line 1982 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6994 "ds_parser.cpp"
    break;

  case 365:
#line 1983 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7000 "ds_parser.cpp"
    break;

  case 366:
#line 1984 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7006 "ds_parser.cpp"
    break;

  case 367:
#line 1985 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7012 "ds_parser.cpp"
    break;

  case 368:
#line 1986 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7018 "ds_parser.cpp"
    break;

  case 369:
#line 1987 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7024 "ds_parser.cpp"
    break;

  case 370:
#line 1988 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7030 "ds_parser.cpp"
    break;

  case 371:
#line 1989 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7036 "ds_parser.cpp"
    break;

  case 372:
#line 1990 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7042 "ds_parser.cpp"
    break;

  case 373:
#line 1991 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7048 "ds_parser.cpp"
    break;

  case 374:
#line 1992 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7054 "ds_parser.cpp"
    break;

  case 375:
#line 1993 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7060 "ds_parser.cpp"
    break;

  case 376:
#line 1994 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7066 "ds_parser.cpp"
    break;

  case 377:
#line 1995 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7072 "ds_parser.cpp"
    break;

  case 378:
#line 1996 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7078 "ds_parser.cpp"
    break;

  case 379:
#line 1997 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7084 "ds_parser.cpp"
    break;

  case 380:
#line 1998 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7090 "ds_parser.cpp"
    break;

  case 381:
#line 1999 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7096 "ds_parser.cpp"
    break;

  case 382:
#line 2000 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7102 "ds_parser.cpp"
    break;

  case 383:
#line 2001 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7108 "ds_parser.cpp"
    break;

  case 384:
#line 2002 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7114 "ds_parser.cpp"
    break;

  case 385:
#line 2003 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7120 "ds_parser.cpp"
    break;

  case 386:
#line 2004 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7126 "ds_parser.cpp"
    break;

  case 387:
#line 2008 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7132 "ds_parser.cpp"
    break;

  case 388:
#line 2009 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7138 "ds_parser.cpp"
    break;

  case 389:
#line 2010 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7144 "ds_parser.cpp"
    break;

  case 390:
#line 2011 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7150 "ds_parser.cpp"
    break;

  case 391:
#line 2012 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7156 "ds_parser.cpp"
    break;

  case 392:
#line 2013 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7162 "ds_parser.cpp"
    break;

  case 393:
#line 2017 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7175 "ds_parser.cpp"
    break;

  case 394:
#line 2028 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7184 "ds_parser.cpp"
    break;

  case 395:
#line 2032 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7196 "ds_parser.cpp"
    break;

  case 396:
#line 2042 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7208 "ds_parser.cpp"
    break;

  case 397:
#line 2049 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7219 "ds_parser.cpp"
    break;

  case 398:
#line 2058 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7225 "ds_parser.cpp"
    break;

  case 399:
#line 2058 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7231 "ds_parser.cpp"
    break;

  case 400:
#line 2058 "ds_parser.ypp"
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
#line 7246 "ds_parser.cpp"
    break;

  case 401:
#line 2071 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7252 "ds_parser.cpp"
    break;

  case 402:
#line 2072 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7258 "ds_parser.cpp"
    break;

  case 403:
#line 2073 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7264 "ds_parser.cpp"
    break;

  case 404:
#line 2074 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7270 "ds_parser.cpp"
    break;

  case 405:
#line 2075 "ds_parser.ypp"
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
#line 7289 "ds_parser.cpp"
    break;

  case 406:
#line 2089 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7300 "ds_parser.cpp"
    break;

  case 407:
#line 2095 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7309 "ds_parser.cpp"
    break;

  case 408:
#line 2099 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7319 "ds_parser.cpp"
    break;

  case 409:
#line 2104 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7329 "ds_parser.cpp"
    break;

  case 410:
#line 2109 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7339 "ds_parser.cpp"
    break;

  case 411:
#line 2114 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7349 "ds_parser.cpp"
    break;

  case 412:
#line 2119 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7358 "ds_parser.cpp"
    break;

  case 413:
#line 2123 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7367 "ds_parser.cpp"
    break;

  case 414:
#line 2127 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7377 "ds_parser.cpp"
    break;

  case 415:
#line 2132 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7386 "ds_parser.cpp"
    break;

  case 416:
#line 2136 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7396 "ds_parser.cpp"
    break;

  case 417:
#line 2141 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7402 "ds_parser.cpp"
    break;

  case 418:
#line 2141 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7408 "ds_parser.cpp"
    break;

  case 419:
#line 2141 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7419 "ds_parser.cpp"
    break;

  case 420:
#line 2147 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7431 "ds_parser.cpp"
    break;

  case 421:
#line 2154 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7437 "ds_parser.cpp"
    break;

  case 422:
#line 2154 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7443 "ds_parser.cpp"
    break;

  case 423:
#line 2154 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7453 "ds_parser.cpp"
    break;

  case 424:
#line 2159 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7459 "ds_parser.cpp"
    break;

  case 425:
#line 2159 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7465 "ds_parser.cpp"
    break;

  case 426:
#line 2159 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7476 "ds_parser.cpp"
    break;

  case 427:
#line 2165 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7482 "ds_parser.cpp"
    break;

  case 428:
#line 2165 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7488 "ds_parser.cpp"
    break;

  case 429:
#line 2165 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7498 "ds_parser.cpp"
    break;

  case 430:
#line 2170 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7507 "ds_parser.cpp"
    break;

  case 431:
#line 2174 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7513 "ds_parser.cpp"
    break;

  case 432:
#line 2174 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7519 "ds_parser.cpp"
    break;

  case 433:
#line 2174 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7529 "ds_parser.cpp"
    break;

  case 434:
#line 2179 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7535 "ds_parser.cpp"
    break;

  case 435:
#line 2179 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7541 "ds_parser.cpp"
    break;

  case 436:
#line 2179 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7555 "ds_parser.cpp"
    break;

  case 437:
#line 2188 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7564 "ds_parser.cpp"
    break;

  case 438:
#line 2192 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7570 "ds_parser.cpp"
    break;

  case 439:
#line 2192 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7576 "ds_parser.cpp"
    break;

  case 440:
#line 2192 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7586 "ds_parser.cpp"
    break;

  case 441:
#line 2197 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7592 "ds_parser.cpp"
    break;

  case 442:
#line 2197 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7598 "ds_parser.cpp"
    break;

  case 443:
#line 2197 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7612 "ds_parser.cpp"
    break;

  case 444:
#line 2206 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7621 "ds_parser.cpp"
    break;

  case 445:
#line 2210 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7627 "ds_parser.cpp"
    break;

  case 446:
#line 2210 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7633 "ds_parser.cpp"
    break;

  case 447:
#line 2210 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7643 "ds_parser.cpp"
    break;

  case 448:
#line 2215 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7649 "ds_parser.cpp"
    break;

  case 449:
#line 2215 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7655 "ds_parser.cpp"
    break;

  case 450:
#line 2215 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7669 "ds_parser.cpp"
    break;

  case 451:
#line 2224 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7675 "ds_parser.cpp"
    break;

  case 452:
#line 2224 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7681 "ds_parser.cpp"
    break;

  case 453:
#line 2224 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7692 "ds_parser.cpp"
    break;

  case 454:
#line 2230 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7698 "ds_parser.cpp"
    break;

  case 455:
#line 2230 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7704 "ds_parser.cpp"
    break;

  case 456:
#line 2230 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7715 "ds_parser.cpp"
    break;

  case 457:
#line 2239 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7721 "ds_parser.cpp"
    break;

  case 458:
#line 2239 "ds_parser.ypp"
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
#line 7738 "ds_parser.cpp"
    break;

  case 459:
#line 2254 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7744 "ds_parser.cpp"
    break;

  case 460:
#line 2254 "ds_parser.ypp"
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
#line 7765 "ds_parser.cpp"
    break;

  case 461:
#line 2274 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7771 "ds_parser.cpp"
    break;

  case 462:
#line 2275 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7777 "ds_parser.cpp"
    break;

  case 463:
#line 2276 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7783 "ds_parser.cpp"
    break;

  case 464:
#line 2277 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7789 "ds_parser.cpp"
    break;

  case 465:
#line 2281 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7801 "ds_parser.cpp"
    break;

  case 466:
#line 2288 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7812 "ds_parser.cpp"
    break;

  case 467:
#line 2297 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7822 "ds_parser.cpp"
    break;

  case 468:
#line 2302 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7831 "ds_parser.cpp"
    break;

  case 469:
#line 2309 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7841 "ds_parser.cpp"
    break;

  case 470:
#line 2314 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7852 "ds_parser.cpp"
    break;

  case 471:
#line 2320 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7863 "ds_parser.cpp"
    break;

  case 472:
#line 2326 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7875 "ds_parser.cpp"
    break;

  case 473:
#line 2333 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7888 "ds_parser.cpp"
    break;

  case 474:
#line 2344 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7896 "ds_parser.cpp"
    break;

  case 475:
#line 2347 "ds_parser.ypp"
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
#line 7912 "ds_parser.cpp"
    break;

  case 476:
#line 2361 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7923 "ds_parser.cpp"
    break;

  case 477:
#line 2370 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7929 "ds_parser.cpp"
    break;

  case 478:
#line 2371 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7935 "ds_parser.cpp"
    break;

  case 479:
#line 2375 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7945 "ds_parser.cpp"
    break;

  case 480:
#line 2380 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7954 "ds_parser.cpp"
    break;

  case 481:
#line 2387 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7964 "ds_parser.cpp"
    break;

  case 482:
#line 2392 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7977 "ds_parser.cpp"
    break;

  case 483:
#line 2403 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7987 "ds_parser.cpp"
    break;

  case 484:
#line 2408 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7996 "ds_parser.cpp"
    break;

  case 485:
#line 2415 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8010 "ds_parser.cpp"
    break;

  case 486:
#line 2427 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8016 "ds_parser.cpp"
    break;

  case 487:
#line 2428 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8022 "ds_parser.cpp"
    break;

  case 488:
#line 2432 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
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
#line 8044 "ds_parser.cpp"
    break;

  case 489:
#line 2449 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
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
#line 8066 "ds_parser.cpp"
    break;


#line 8070 "ds_parser.cpp"

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
#line 2468 "ds_parser.ypp"


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
