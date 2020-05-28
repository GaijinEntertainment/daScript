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

    __forceinline string inThisModule ( const string & name ) { return "_::" + name; }

#line 104 "ds_parser.cpp"

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

#line 203 "ds_parser.cpp"

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
    DAS_TBOOL = 315,
    DAS_TVOID = 316,
    DAS_TSTRING = 317,
    DAS_TAUTO = 318,
    DAS_TINT = 319,
    DAS_TINT2 = 320,
    DAS_TINT3 = 321,
    DAS_TINT4 = 322,
    DAS_TUINT = 323,
    DAS_TBITFIELD = 324,
    DAS_TUINT2 = 325,
    DAS_TUINT3 = 326,
    DAS_TUINT4 = 327,
    DAS_TFLOAT = 328,
    DAS_TFLOAT2 = 329,
    DAS_TFLOAT3 = 330,
    DAS_TFLOAT4 = 331,
    DAS_TRANGE = 332,
    DAS_TURANGE = 333,
    DAS_TBLOCK = 334,
    DAS_TINT64 = 335,
    DAS_TUINT64 = 336,
    DAS_TDOUBLE = 337,
    DAS_TFUNCTION = 338,
    DAS_TLAMBDA = 339,
    DAS_TINT8 = 340,
    DAS_TUINT8 = 341,
    DAS_TINT16 = 342,
    DAS_TUINT16 = 343,
    DAS_TTUPLE = 344,
    DAS_TVARIANT = 345,
    DAS_GENERATOR = 346,
    DAS_YIELD = 347,
    ADDEQU = 348,
    SUBEQU = 349,
    DIVEQU = 350,
    MULEQU = 351,
    MODEQU = 352,
    ANDEQU = 353,
    OREQU = 354,
    XOREQU = 355,
    SHL = 356,
    SHR = 357,
    ADDADD = 358,
    SUBSUB = 359,
    LEEQU = 360,
    SHLEQU = 361,
    SHREQU = 362,
    GREQU = 363,
    EQUEQU = 364,
    NOTEQU = 365,
    RARROW = 366,
    LARROW = 367,
    QQ = 368,
    QDOT = 369,
    QBRA = 370,
    LPIPE = 371,
    LBPIPE = 372,
    LAPIPE = 373,
    RPIPE = 374,
    CLONEEQU = 375,
    ROTL = 376,
    ROTR = 377,
    ROTLEQU = 378,
    ROTREQU = 379,
    MAPTO = 380,
    BRABRAB = 381,
    BRACBRB = 382,
    CBRCBRB = 383,
    INTEGER = 384,
    LONG_INTEGER = 385,
    UNSIGNED_INTEGER = 386,
    UNSIGNED_LONG_INTEGER = 387,
    FLOAT = 388,
    DOUBLE = 389,
    NAME = 390,
    BEGIN_STRING = 391,
    STRING_CHARACTER = 392,
    END_STRING = 393,
    BEGIN_STRING_EXPR = 394,
    END_STRING_EXPR = 395,
    UNARY_MINUS = 396,
    UNARY_PLUS = 397,
    PRE_INC = 398,
    PRE_DEC = 399,
    POST_INC = 400,
    POST_DEC = 401,
    DEREF = 402,
    COLCOL = 403
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 88 "ds_parser.ypp"

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

#line 390 "ds_parser.cpp"

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
#define YYLAST   6912

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  176
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  166
/* YYNRULES -- Number of rules.  */
#define YYNRULES  496
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  889

#define YYUNDEFTOK  2
#define YYMAXUTOK   403

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
       2,     2,     2,   158,     2,   175,   169,   154,   147,     2,
     167,   168,   152,   151,   141,   150,   163,   153,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   144,   172,
     148,   142,   149,   143,   170,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   165,     2,   166,   146,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   173,   145,   174,   157,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   155,   156,   159,   160,
     161,   162,   164,   171
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   412,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   428,   440,   441,   445,   449,
     454,   463,   471,   487,   509,   513,   516,   522,   531,   552,
     575,   576,   580,   584,   585,   589,   592,   598,   604,   607,
     613,   614,   618,   619,   620,   629,   630,   634,   643,   659,
     669,   678,   679,   680,   681,   682,   683,   687,   692,   700,
     701,   702,   703,   704,   705,   706,   707,   713,   726,   742,
     743,   747,   754,   766,   770,   777,   778,   782,   783,   784,
     788,   793,   800,   804,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,   835,   836,   837,   838,   839,   843,
     864,   902,   909,   913,   924,   925,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   945,   950,   956,   962,   983,   989,   990,  1001,
    1005,  1011,  1014,  1017,  1021,  1027,  1031,  1035,  1038,  1041,
    1046,  1049,  1057,  1060,  1065,  1068,  1076,  1082,  1083,  1087,
    1088,  1092,  1126,  1126,  1126,  1129,  1129,  1129,  1134,  1134,
    1134,  1142,  1146,  1151,  1157,  1157,  1157,  1161,  1161,  1161,
    1166,  1166,  1166,  1175,  1178,  1184,  1185,  1192,  1203,  1204,
    1208,  1213,  1268,  1269,  1270,  1271,  1272,  1273,  1277,  1278,
    1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,
    1289,  1290,  1291,  1292,  1296,  1297,  1301,  1312,  1317,  1327,
    1331,  1331,  1331,  1338,  1338,  1338,  1352,  1353,  1354,  1355,
    1356,  1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,
    1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,
    1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1395,  1399,  1402,  1405,
    1406,  1407,  1408,  1413,  1418,  1419,  1422,  1422,  1422,  1425,
    1429,  1433,  1437,  1438,  1439,  1440,  1441,  1442,  1443,  1453,
    1463,  1518,  1519,  1523,  1524,  1528,  1536,  1539,  1544,  1543,
    1588,  1644,  1649,  1657,  1658,  1662,  1665,  1674,  1675,  1679,
    1688,  1689,  1694,  1695,  1699,  1705,  1711,  1714,  1718,  1724,
    1733,  1734,  1735,  1739,  1740,  1744,  1747,  1752,  1757,  1765,
    1776,  1779,  1786,  1787,  1791,  1818,  1818,  1830,  1833,  1842,
    1855,  1867,  1868,  1872,  1876,  1887,  1902,  1903,  1907,  1944,
    1945,  1949,  1949,  2033,  2040,  2049,  2050,  2051,  2052,  2053,
    2054,  2055,  2056,  2057,  2058,  2059,  2060,  2061,  2062,  2063,
    2064,  2065,  2066,  2067,  2068,  2069,  2070,  2071,  2072,  2073,
    2077,  2078,  2079,  2080,  2081,  2082,  2086,  2097,  2101,  2111,
    2118,  2127,  2127,  2127,  2140,  2141,  2142,  2143,  2144,  2158,
    2164,  2168,  2173,  2178,  2183,  2188,  2192,  2196,  2201,  2205,
    2210,  2210,  2210,  2216,  2223,  2223,  2223,  2228,  2228,  2228,
    2234,  2234,  2234,  2239,  2243,  2243,  2243,  2248,  2248,  2248,
    2257,  2261,  2261,  2261,  2266,  2266,  2266,  2275,  2279,  2279,
    2279,  2284,  2284,  2284,  2293,  2293,  2293,  2299,  2299,  2299,
    2308,  2308,  2323,  2323,  2343,  2344,  2345,  2346,  2350,  2357,
    2364,  2370,  2379,  2384,  2391,  2392,  2396,  2402,  2409,  2416,
    2424,  2436,  2439,  2453,  2462,  2463,  2467,  2472,  2479,  2484,
    2495,  2500,  2507,  2519,  2520,  2524,  2542
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
  "\"goto\"", "\"implicit\"", "\"shared\"", "\"smart_ptr\"", "\"bool\"",
  "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"",
  "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "\"end of the string\"", "\"{\"", "\"}\"", "','",
  "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'",
  "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "DEREF", "'['",
  "']'", "'('", "')'", "'$'", "'@'", "COLCOL", "';'", "'{'", "'}'", "'#'",
  "$accept", "program", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_if_then_else", "expression_for_loop",
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
  "expression_try_catch", "let_scope", "kwd_let", "expression_let",
  "expr_cast", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "expr_type_info",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "expr_list",
  "block_or_simple_block", "block_or_lambda", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@13", "$@14",
  "$@15", "$@16", "expr", "$@17", "$@18", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@19",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "$@20", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "class_or_struct",
  "structure_declaration", "$@21", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@22", "$@23", "type_declaration", "$@24",
  "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "variant_alias_declaration",
  "$@48", "bitfield_alias_declaration", "$@49", "make_decl",
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
     395,    44,    61,    63,    58,   124,    94,    38,    60,    62,
      45,    43,    42,    47,    37,   396,   397,   126,    33,   398,
     399,   400,   401,    46,   402,    91,    93,    40,    41,    36,
      64,   403,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -575

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-575)))

#define YYTABLE_NINF -452

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -575,    20,  -575,  -575,   -60,   -42,    27,    35,   -95,  -575,
      44,  -575,  -575,   -16,  -575,  -575,  -575,  -575,  -575,   330,
    -575,    64,  -575,  -575,  -575,  -575,  -575,  -575,    65,  -575,
      45,     9,   101,  -575,  -575,    11,  -575,   -87,    93,  -575,
     113,   118,  -575,    91,   137,    77,  -575,   -32,  -575,  -575,
    -575,    12,   163,  -575,   135,    21,   -60,   191,   -42,   201,
    -575,   208,   217,  -575,   394,  -575,   207,  -575,   -76,   182,
     193,   229,  -575,   -60,   -16,  -575,  6754,   334,   338,  -575,
     211,   210,  -575,   236,  -575,  -575,   380,  -575,  -575,  -575,
    -575,  -575,   247,    13,  -575,  -575,  -575,  -575,   331,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,   215,   -72,  5850,
    -575,  -575,   256,   267,  -575,    53,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,   -85,   259,  -575,  -575,   -81,  -575,   232,
     -63,   237,  -575,   220,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,    70,  -575,  -575,    47,  -575,   260,   262,   265,   266,
    -575,  -575,  -575,   250,  -575,  -575,  -575,  -575,  -575,   270,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,   271,
    -575,  -575,  -575,   272,   273,  -575,  -575,  -575,  -575,   274,
     275,  -575,  -575,  -575,  -575,  -575,   198,  -575,   252,   281,
    -575,   254,  -575,   -60,  -575,    -5,  -575,    55,  5850,  -575,
    1233,  -575,  -575,  -575,  -575,  -575,   166,  -575,  -575,  -575,
      13,  -575,   -56,  4552,  -575,  -575,  -575,  -575,  -575,   292,
    -575,   102,   112,   147,  -575,  -575,  -575,  -575,  -575,   397,
    -575,  -575,    -2,  2539,  -575,  -575,   -51,  5850,   -50,   -29,
     300,  -575,    40,  -575,   310,  4372,  -575,  4552,  -575,  -575,
     300,  -575,  -575,  5773,   276,  1994,  -575,  -575,   210,  4552,
     277,  4552,   297,   305,   280,  -575,   268,   307,   327,  2669,
    -575,   309,  2133,  4552,  4552,   181,   448,   940,  4552,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  4552,  4552,  4552,  4552,
    4552,  4552,  -575,   289,  -575,   425,  -575,   291,   294,  4552,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,   -64,   295,  -575,
     298,   301,  -575,  -575,  -575,   446,  -575,  -575,  -575,   311,
    -575,  -575,   -79,  -575,  -575,  -575,  -575,  6250,   316,  -575,
    -575,  -575,  -575,  -575,     6,   342,  5850,  -575,    63,  -575,
    -575,  1038,  5850,  5850,  5850,  5850,   317,   256,  5850,   211,
    5850,   211,  5850,   211,  5927,   267,  -575,  -575,  -575,   318,
    -575,  -575,  5921,  -575,  -575,  4372,  -575,  -575,   320,  -575,
    -575,  -575,  5850,  -575,  2799,  -575,   933,    18,    82,  -575,
     -81,  2799,  -575,  -575,   370,  4633,   475,  6520,  4552,   933,
    -575,  -575,  4552,  -575,  -575,   345,   361,  6520,  5850,  2799,
    -575,  4730,    87,    87,  -575,  -575,   300,  1556,   300,  1702,
    6315,  -575,   -47,   164,    87,    87,   -45,    87,    87,  5255,
     -67,   319,  -575,  -575,   933,  -575,  2263,  -575,  -575,  -575,
     452,   300,   211,   181,  -575,    34,   360,  4552,  4552,  4552,
    4552,  4552,  4552,  4552,  4552,  4552,  4552,  -575,  -575,  4552,
    4552,  4552,  4552,  4552,  4552,   362,  2929,  4552,   365,  4552,
    4552,  4552,  4552,  4552,  4552,  4552,  4552,  3059,  3198,  4552,
    4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,   366,
    4552,  3328,  -575,   489,   459,   367,  -575,    -7,  -575,  -575,
    -575,  2799,  -575,   687,   667,  1847,  2083,  -575,   -73,  2489,
     259,  2619,   259,  2749,   259,    68,  -575,   109,  4372,   151,
    -575,  -575,  -575,   299,  -575,  6145,  -575,  4552,  3467,  2409,
    -575,  4827,  4552,  4552,  -575,   210,  5323,  -575,  5850,  5850,
    5391,  5850,  -575,  -575,  2879,  -575,  4924,  -575,    26,   181,
     -54,  1848,  3606,  6315,   363,   -11,   337,   399,  -575,  -575,
     -92,    37,  3745,   -11,   172,  4552,  4552,   368,  -575,  4552,
     369,  -575,   175,  -575,  -575,   293,   406,  -575,    73,  6520,
    -575,  -575,   259,  -575,   395,  -575,  -575,  6520,  6520,  6520,
    6520,  6520,  6520,  6520,  6520,   636,   636,   478,  6520,  6520,
     478,  6720,  6720,   377,  4552,  6520,   214,  -575,  5998,   -71,
     -71,  6520,   636,   636,  6520,  6520,  4552,  6520,   412,  6385,
    6585,   824,  6655,   478,   478,   653,   653,   214,   214,   214,
    -575,  6068,  -575,    92,   332,  -575,   300,  -575,   397,  -575,
    2799,  -575,  5021,  -575,  5850,  -575,  -575,  -575,   414,  -575,
     401,  -575,   402,  -575,   403,  5850,  -575,  5927,  -575,   267,
     397,  4552,   -77,  -575,    95,   405,   419,  5459,  -575,  -575,
    -575,  3009,  3056,  -575,  3278,   388,  -575,  4552,  -575,  4552,
    4552,   406,   421,   406,   392,   393,  4552,   396,  4552,  4552,
     406,   386,   398,  6520,  -575,  -575,  6450,  5850,   211,  1407,
     210,  -575,  -575,  4552,  -575,    84,    79,  4552,  -575,   -91,
    -575,  3884,   285,  -575,   285,  -575,  4552,  -575,  -575,  -575,
      12,  -575,  -575,  5118,  -575,  -575,  3325,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  4372,  -575,  -575,  -575,
    6520,   210,  -575,  -575,   160,  -575,  -575,  -575,  -575,  4023,
     -44,  6520,  6520,   -11,    98,   363,   400,  -575,  6520,  -575,
    -575,   -41,   -11,   407,  -575,  -575,  3556,   259,  -575,  -575,
     933,   404,  6520,  4162,  -575,  -575,  5850,  -575,    96,  6520,
      12,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  5850,  4292,
     426,  4552,  4552,  4552,  -575,  5527,  4552,   410,  4552,  4552,
    -575,  4552,   389,  -575,  -575,   416,   293,  -575,  4552,  6520,
    3695,  -575,   415,  -575,  3834,   423,  5595,   418,   478,   478,
     478,  -575,  5183,   411,  6520,  6520,  5183,   420,   -81,  -575,
    -575,  6520,  -575,  -575,  -575,  -575,  -575,  4422,   519,   422,
    -575,   424,  -575,  -575,   -81,  -575,   427,  5850,   430,  5663,
    4552,   435,   436,  -575,  -575,  3973,  -575,  -575,  6520,  -575,
    -575,  -575,  5850,   428,  4112,  -575,  -575,   437,  -575
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    75,     1,   169,     0,     0,     0,     0,     0,   170,
       0,   462,   460,     0,    14,     3,    10,     9,     8,     0,
       7,   342,     6,    11,     5,     4,    12,    13,    65,    67,
      23,    35,    32,    33,    25,    30,    24,     0,     0,    15,
       0,     0,    70,   147,     0,    71,    73,     0,    69,   360,
     359,     0,     0,   343,   345,     0,     0,     0,     0,     0,
      31,     0,     0,    28,     0,   347,     0,   351,     0,     0,
       0,     0,   149,     0,     0,    76,     0,     0,     0,    82,
      77,     0,   119,   356,   361,   340,     0,    63,    64,    61,
      62,    60,     0,     0,    59,    68,    36,    34,    30,    27,
      26,   390,   393,   391,   394,   392,   395,     0,     0,     0,
     353,   352,     0,     0,   148,     0,    74,    85,    86,    88,
      87,    89,    90,    91,    92,   111,   112,   109,   110,   102,
     113,   114,   103,   100,   101,   115,   116,   117,   118,   105,
     106,   104,    98,    99,    94,    93,    95,    96,    97,    84,
      83,   107,   108,   301,    80,   142,   121,     0,   358,     0,
       0,     0,    16,     0,    55,    56,    53,    54,    52,    51,
      57,     0,    29,   347,     0,   354,     0,     0,     0,     0,
     365,   385,   366,   397,   367,   371,   372,   373,   374,   389,
     378,   379,   380,   381,   382,   383,   384,   386,   387,   433,
     370,   377,   388,   440,   447,   368,   375,   369,   376,     0,
       0,   396,   404,   407,   405,   406,     0,   399,     0,     0,
     320,     0,    72,     0,    78,     0,   313,     0,     0,   120,
       0,   357,   306,   363,   344,   341,   333,   346,    17,    18,
       0,    66,     0,     0,   348,   424,   427,   430,   420,     0,
     401,   434,   441,   448,   454,   457,   411,   416,   423,     0,
     419,   413,     0,     0,   350,   415,     0,     0,     0,     0,
       0,   311,   324,    79,   301,    81,   144,     0,    45,    46,
       0,   241,   242,     0,     0,     0,   236,   155,     0,     0,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
     389,     0,     0,     0,     0,     0,     0,     0,     0,   202,
     204,   203,   205,   206,   207,    19,     0,     0,     0,     0,
       0,     0,   198,   199,   124,   122,   239,     0,     0,     0,
     137,   132,   130,   131,   200,   143,   125,   237,     0,   294,
       0,     0,   135,   136,   138,   167,   129,   293,   292,    75,
     297,   238,     0,   126,   296,   295,   274,   208,     0,   240,
     464,   465,   466,   467,   301,     0,     0,   334,     0,    58,
     355,     0,     0,     0,     0,     0,     0,     0,     0,    77,
       0,    77,     0,    77,     0,     0,   418,   412,   414,     0,
     417,   409,     0,   400,   463,   319,   461,   321,     0,   312,
     323,   322,     0,   325,     0,   314,     0,     0,   151,   154,
       0,     0,   157,   160,     0,   208,     0,   150,     0,     0,
     172,   175,     0,   141,   178,     0,     0,    39,     0,     0,
     164,   208,   265,   266,   199,   146,     0,   474,     0,     0,
       0,   490,     0,     0,   246,   245,   279,   244,   243,     0,
       0,     0,   139,   140,     0,   300,     0,   128,   133,   134,
       0,     0,    77,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   362,     0,   303,     0,   364,     0,   331,   332,
     330,     0,   349,     0,     0,     0,     0,   398,     0,     0,
      80,     0,    80,     0,    80,   147,   317,     0,   315,     0,
     410,   408,   302,   326,   329,   328,    49,     0,     0,     0,
     161,   208,     0,     0,   158,     0,     0,    50,     0,     0,
       0,     0,    37,    38,     0,   165,   208,   162,     0,     0,
     147,     0,     0,   481,   472,   474,     0,   484,   485,   486,
       0,     0,     0,   474,     0,     0,     0,     0,    22,     0,
      20,   269,   230,   229,   142,    42,     0,   275,     0,   193,
     168,   171,    80,   145,     0,   289,   290,   215,   216,   218,
     217,   219,   212,   213,   214,   247,   248,   260,   220,   221,
     261,   258,   259,     0,     0,   210,   284,   273,     0,   298,
     299,   211,   249,   250,   222,   223,     0,   209,     0,     0,
     263,   264,   262,   256,   257,   252,   251,   253,   254,   255,
     272,     0,   277,     0,   303,   304,     0,   307,   330,   335,
       0,   339,   208,   425,     0,   431,   421,   402,     0,   435,
       0,   442,     0,   449,     0,     0,   455,     0,   458,     0,
     322,     0,     0,   152,     0,     0,     0,     0,   159,   166,
     280,     0,     0,   281,     0,     0,   163,     0,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,   491,   492,     0,     0,    77,     0,
       0,    40,    41,     0,    47,     0,     0,     0,   276,     0,
     286,     0,   225,   271,   224,   291,     0,   270,   278,   308,
       0,   305,   337,   208,   338,   426,     0,   432,   422,   403,
     436,   438,   443,   445,   450,   452,   316,   456,   318,   459,
     327,     0,   153,   184,     0,   181,   173,   176,   179,     0,
       0,   469,   468,   474,     0,   473,     0,   477,   482,   488,
     487,     0,   474,     0,   489,    21,     0,    80,   123,    43,
       0,     0,   194,     0,   195,   201,     0,   227,     0,   285,
       0,   310,   336,   428,   439,   446,   453,    48,     0,     0,
       0,     0,     0,     0,   282,     0,     0,     0,     0,     0,
     476,     0,     0,   479,   231,     0,    42,   226,     0,   196,
       0,   228,     0,   429,     0,     0,     0,     0,   174,   177,
     180,   283,   493,     0,   471,   470,   493,     0,     0,   234,
      44,   197,   287,   309,   185,   187,   182,     0,     0,     0,
     478,     0,   480,   232,     0,   288,     0,     0,     0,     0,
       0,     0,     0,   235,   186,     0,   190,   183,   494,   495,
     496,   188,     0,     0,     0,   189,   191,     0,   192
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -575,  -575,  -575,   132,   529,  -575,  -575,  -575,  -575,  -575,
    -575,   492,  -575,  -575,   545,  -575,  -575,  -575,  -222,  -575,
    -575,  -575,  -575,  -575,   371,  -575,   550,   -57,  -575,   533,
    -575,  -287,  -371,  -501,  -575,  -575,  -192,  -131,   -80,  -575,
      19,  -278,   -13,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,   611,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -500,  -575,  -575,
    -291,  -575,  -226,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,   409,  -575,  -575,   251,   -40,  -575,  -575,  -575,   343,
    -575,   -61,  -575,  -263,   233,  -269,  -264,   103,  -575,   161,
    -575,  -575,  -575,  -575,   450,   553,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -139,  -107,  -575,  -575,  -575,   248,
    -575,  -575,  -575,   -90,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,   341,  -574,  -421,  -560,  -575,  -575,  -299,   -82,
     194,  -575,  -575,  -575,  -212,  -575
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   443,   326,    16,    17,    35,
      36,    63,    18,    32,    33,   327,   328,   723,   724,   329,
     330,   331,   332,   333,   170,   171,    29,    30,    45,    46,
      47,    19,   154,   229,    80,    20,    81,    82,   334,   335,
     230,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     461,   345,   346,   347,   558,   811,   559,   812,   561,   813,
     348,   808,   866,   867,   883,   882,   887,   598,   795,   349,
     350,   351,   414,   353,   354,   355,   356,   717,   848,   718,
     864,   599,   796,   865,   225,   656,   515,   364,   800,   226,
     227,   536,   537,   220,   221,   700,   271,   521,   368,   235,
     160,    54,    22,    86,   108,    67,    68,    23,    24,   158,
      84,    52,    25,   159,   272,   358,   107,   213,   214,   218,
     215,   377,   749,   538,   375,   748,   372,   745,   373,   833,
     374,   747,   378,   750,   379,   804,   380,   752,   381,   805,
     382,   754,   383,   806,   384,   757,   385,   759,    26,    41,
      27,    40,   359,   574,   575,   576,   360,   577,   578,   579,
     580,   361,   442,   362,   859,   363
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   156,   212,   404,   352,   397,   399,   413,   530,   441,
     532,   653,   534,   -75,   435,   704,   115,    42,   583,   216,
       2,   236,   726,   711,   430,   256,     3,   164,   165,   670,
     387,   672,    59,   674,   793,    87,    88,   547,   463,   569,
     485,   223,   270,   488,   489,   697,    76,   682,   684,     4,
     257,     5,   604,     6,    43,     7,   709,    64,   400,    66,
       8,    48,   462,   174,   727,    60,   699,     9,    43,   488,
     489,   455,   233,    10,   707,    28,   667,   513,    38,   174,
     708,   592,   155,   224,   393,   219,    65,    31,   401,    11,
      44,   602,   509,   464,   510,   761,   211,   727,   110,   668,
     727,   729,   175,   456,    44,   518,   258,   465,   466,    74,
      12,   234,    56,   519,   256,    77,    78,    71,   370,    43,
     510,   212,    53,   394,   396,   586,   544,   587,   816,   775,
     233,   821,   223,   550,    75,   658,   260,   398,   275,   257,
     261,   407,   166,   262,   231,   388,   167,    79,   168,    92,
      89,   565,   400,    57,    90,    44,    91,    92,   263,   365,
     212,   703,    34,   389,    61,   659,   269,   365,   265,   605,
      37,    13,   603,   390,    62,   518,   212,   395,   365,    39,
     512,   365,   401,   519,   402,    13,    56,   403,    93,   243,
     477,   478,    14,   408,    56,   258,   400,   770,   485,   212,
     212,   488,   489,   490,   699,   520,   491,    55,   416,   781,
     400,   240,   675,   817,   727,   211,   437,   439,   818,   244,
     702,   222,   822,   273,   259,   260,   401,   274,    66,   261,
     256,   798,   262,   727,   465,   466,   727,   727,   241,    71,
     401,   728,    58,   661,    73,   791,  -437,   263,    69,   548,
     509,  -437,   510,    70,   211,   257,  -444,   265,   676,   212,
     738,  -444,    71,   762,   831,   212,   212,   212,   212,  -437,
     211,   212,    72,   212,   681,   212,   517,   212,   698,  -444,
     773,   677,   523,   524,   525,   526,   825,   714,   529,   782,
     531,  -451,   533,   211,   211,   212,  -451,   568,    83,   581,
     678,   162,   588,   589,   720,   465,   466,   365,    85,   809,
     366,   258,   543,   367,  -451,   721,   722,   477,   478,  -233,
      96,   212,   236,   679,  -233,   485,   546,   487,   488,   489,
     490,   256,   810,   491,    49,    50,    98,    51,   564,   557,
     259,   260,  -233,    99,   708,   261,   712,   787,   262,   109,
     322,   434,   100,   211,   155,   112,   257,   238,   239,   211,
     211,   211,   211,   263,   114,   211,   113,   211,   151,   211,
     264,   211,   152,   265,   595,   655,   739,   509,   153,   510,
     157,   161,   742,   155,   162,    60,   475,   476,   173,   211,
     479,   217,   741,   482,   483,   484,   485,   549,   487,   488,
     489,   490,   219,   228,   491,   232,   493,   494,   245,   237,
     246,   400,   258,   247,   248,   211,   397,   249,   250,   251,
     252,   253,   254,   255,   266,   267,   268,   376,   498,   386,
     499,   500,   501,   502,   503,   233,   223,   593,   507,   508,
     423,   680,   260,   410,   418,   420,   261,   422,   509,   262,
     510,   212,   212,   421,   212,   424,   425,   428,   101,   450,
     436,   451,   102,   452,   263,   460,   453,   457,   691,   692,
     458,   694,   176,   459,   265,   689,    13,   516,   177,   103,
     104,   105,   106,   511,   540,   527,   542,   463,   555,   562,
     563,   600,   594,   352,   178,   606,   654,   623,   465,   466,
     627,   650,   655,   705,   702,   819,   238,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   657,
     706,   725,   715,   730,   731,   211,   211,   735,   211,   393,
     751,   753,   755,   763,   764,   769,   774,   212,   776,   777,
     783,   837,   779,   847,   784,   849,   820,   857,   212,   870,
     212,   855,   827,   823,   746,   590,   843,   860,   876,   475,
     476,   477,   478,    43,    94,   756,   862,   853,   871,   485,
     172,   487,   488,   489,   490,   874,   885,   491,   872,   493,
     494,   879,   880,    97,   850,   888,    95,   116,   832,   801,
     212,   369,    21,   719,   740,   514,   758,   405,   539,    44,
     660,   111,   601,   242,   409,   528,   780,   786,   504,   505,
     506,   507,   508,   584,   861,     0,     0,     0,     0,   357,
     789,   509,     0,   510,     0,     0,     0,     0,     0,   794,
       0,   211,   371,     0,     0,     0,   465,   466,     0,     0,
       0,     0,   211,     0,   211,     0,     0,     0,     0,     0,
       0,     0,   392,   465,   466,     0,     0,     0,     0,     0,
       0,   807,     0,     0,     0,     0,   406,     0,     0,   212,
       0,     0,     0,     0,   415,     0,     0,     0,   417,   256,
     419,   212,     0,     0,   211,     0,   830,     0,   427,     0,
     826,   431,   432,   433,     0,     0,     0,   440,   834,   256,
       0,     0,     0,     0,   257,   444,   445,   446,   447,   448,
     449,     0,     0,     0,     0,     0,     0,     0,   454,   477,
     478,     0,     0,     0,   257,     0,     0,   485,     0,   487,
     488,   489,   490,     0,     0,   491,   477,   478,     0,     0,
     212,     0,     0,     0,   485,     0,   487,   488,   489,   490,
       0,     0,   491,     0,     0,   212,     0,   875,     0,     0,
     258,     0,     0,   211,     0,     0,   504,   505,   506,   507,
     508,     0,   884,     0,     0,   211,     0,     0,     0,   509,
     258,   510,     0,     0,     0,   506,   507,   508,     0,   259,
     260,     0,     0,   545,   261,     0,   509,   262,   510,     0,
     551,     0,     0,     0,     0,     0,     0,   556,     0,   259,
     260,   560,   263,     0,   261,   863,   663,   262,   566,   664,
       0,     0,   265,     0,   465,   466,   573,     0,   573,     0,
       0,   873,   263,     0,   211,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,     0,   211,
       0,     0,     0,     0,     0,     0,   607,   608,   609,   610,
     611,   612,   613,   614,   615,   616,     0,     0,   617,   618,
     619,   620,   621,   622,     0,   625,   626,     0,   628,   629,
     630,   631,   632,   633,   634,   635,   637,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,   649,     0,   651,
       0,     0,     0,     0,     0,   475,   476,   477,   478,   479,
     662,     0,   482,   483,   484,   485,     0,   487,   488,   489,
     490,     0,     0,   491,     0,   493,   494,     0,     0,     0,
       0,     0,   438,   465,   466,     0,     0,     0,   687,     0,
       0,   625,   637,     0,   176,     0,     0,     0,     0,     0,
     177,   501,   502,   503,   504,   505,   506,   507,   508,     0,
     444,   449,     0,     0,     0,     0,   178,   509,     0,   510,
       0,   449,     0,     0,   713,   440,     0,     0,   716,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,     0,   732,   475,   476,   477,   478,   479,     0,
       0,   482,   483,   484,   485,   734,   487,   488,   489,   490,
       0,     0,   491,     0,   493,   494,     0,     0,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   743,
       0,     0,     0,     0,     0,    43,   498,     0,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,     0,     0,
     760,     0,     0,     0,     0,     0,   509,     0,   510,     0,
       0,     0,     0,     0,     0,     0,   155,     0,   771,   772,
       0,    44,     0,     0,     0,   778,     0,   573,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   357,     0,
       0,     0,   790,     0,     0,     0,   792,     0,     0,   475,
     476,   477,   478,   479,     0,   799,   482,   483,   484,   485,
       0,   487,   488,   489,   490,     0,     0,   491,     0,   493,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   815,     0,
       0,   498,     0,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,     0,     0,     0,     0,     0,     0,     0,
       0,   509,   829,   510,     0,     0,     0,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,   836,     0,
     838,   839,   840,     0,     0,   842,     0,   844,   845,     0,
     846,     0,     0,     0,   276,     0,     0,   851,     0,     3,
       0,   277,   278,   279,     0,   280,     0,   281,   282,   283,
     284,     0,     0,     0,     0,     0,     0,     0,   285,   286,
     287,   288,     0,     0,     0,     0,   869,     0,     0,     0,
     289,   290,     0,     0,   291,   292,     0,     0,   293,   878,
       9,   294,   295,     0,   296,   297,     0,     0,   298,   299,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   300,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   301,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,    43,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,   317,   318,     0,     0,     0,     0,
     319,   320,     0,     0,     0,     0,     0,     0,     0,     0,
     321,     0,   322,   323,    44,   324,   155,   325,   276,     0,
       0,     0,     0,     3,     0,   277,   278,   279,     0,   280,
       0,   281,   282,   283,   284,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   287,   288,     0,     0,     0,     0,
       0,     0,     0,     0,   289,   290,     0,     0,   291,   292,
       0,     0,   293,     0,     9,   294,   295,     0,   296,   297,
       0,     0,   298,   299,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   300,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   301,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,     0,     0,     0,     0,
       0,     0,     0,   306,   307,   308,   309,   310,   311,   312,
     313,   314,    43,   315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,   317,   318,
       0,     0,     0,     0,   319,   320,     0,     0,     0,     0,
     281,   282,   283,   284,   321,     0,   322,   323,    44,   324,
     155,   788,   286,     0,     0,     0,     0,     0,   256,     0,
       0,     0,     0,     0,   290,     0,     0,     0,   292,     0,
       0,   293,     0,     0,   294,     0,   569,     0,   297,     0,
       0,     0,     0,   257,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   300,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
     304,     0,     0,     0,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   570,   315,     0,     0,     0,     0,     0,   259,   260,
       0,     0,     0,   261,     0,     0,   571,   317,   318,     0,
       0,     0,     0,   319,   320,     0,   281,   282,   283,   284,
       0,   263,     0,   572,     0,   322,   323,    44,   286,   155,
       0,   265,     0,     0,   256,     0,     0,     0,     0,     0,
     290,     0,     0,     0,   292,     0,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,   257,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   570,   315,     0,
       0,     0,     0,     0,   259,   260,     0,     0,     0,   261,
       0,     0,   571,   317,   318,     0,     0,     0,     0,   319,
     320,     0,   281,   282,   283,   284,     0,   263,     0,   582,
       0,   322,   323,    44,   286,   155,     0,   265,     0,   256,
     387,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,   257,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   300,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,   307,   308,   309,   310,   311,
     312,   313,   314,    43,   315,     0,     0,     0,     0,   259,
     260,     0,     0,     0,   261,   388,   665,   262,   316,   317,
     318,     0,     0,     0,     0,   319,   320,     0,   281,   282,
     283,   284,   263,   389,     0,   321,     0,   322,   323,    44,
     286,   155,   265,   390,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,   292,     0,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   300,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,   304,     0,
       0,     0,     0,     0,     0,     0,   411,     0,     0,     0,
       0,     0,   305,     0,     0,   256,     0,     0,     0,     0,
     306,   307,   308,   309,   310,   311,   312,   313,   314,    43,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,     0,     0,     0,   316,   317,   318,   281,   282,   283,
     284,   319,   320,     0,     0,     0,     0,     0,     0,   286,
       0,   321,     0,   322,   323,    44,   412,   155,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   258,   184,   185,   186,
     187,   188,   300,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   301,   259,   260,     0,     0,     0,
     261,     0,   666,   262,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,   263,     0,
       0,   305,     0,     0,     0,     0,     0,     0,   265,   306,
     307,   308,   309,   310,   311,   312,   313,   314,    43,   315,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,     0,     0,   316,   317,   318,     0,     0,     0,   286,
     319,   320,     0,     0,     0,     0,     0,     0,     0,     0,
     321,   290,   322,   323,    44,   292,   155,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   300,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,    43,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,   317,   318,     0,     0,     0,     0,
     319,   320,     0,   281,   282,   283,   284,   685,   596,     0,
     321,   597,   322,   323,    44,   286,   155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   290,     0,     0,
       0,   292,     0,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,   257,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,   686,     0,   316,
     317,   318,     0,     0,     0,   286,   319,   320,     0,     0,
       0,     0,     0,     0,     0,     0,   321,   290,   322,   323,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,   258,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,   259,   260,     0,     0,     0,   261,     0,   669,   262,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,   257,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   316,
     317,   318,     0,     0,     0,   286,   319,   320,     0,     0,
       0,     0,     0,     0,     0,   391,   321,   290,   322,   323,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,   426,     0,     0,     0,     0,   180,
     181,   182,   258,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,   259,   260,     0,     0,     0,   261,     0,   671,   262,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,   257,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   316,
     317,   318,     0,     0,     0,   286,   319,   320,     0,     0,
       0,     0,     0,     0,     0,     0,   321,   290,   322,   323,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,   258,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,   259,   260,     0,     0,     0,   261,     0,   673,   262,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,   263,     0,     0,   305,     0,     0,
       0,     0,     0,     0,   265,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,   257,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   316,
     317,   318,     0,     0,     0,   286,   319,   320,     0,     0,
       0,     0,     0,     0,     0,     0,   321,   290,   322,   323,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,   258,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,   259,   260,     0,     0,     0,   261,     0,   695,   262,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,   256,     0,     0,   263,     0,     0,   624,     0,     0,
       0,     0,     0,     0,   265,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,   257,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   316,
     317,   318,     0,     0,     0,   286,   319,   320,   256,     0,
       0,     0,     0,     0,     0,     0,   321,   290,   322,   323,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,   257,     0,     0,     0,     0,     0,   180,
     181,   182,   258,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,   259,   260,     0,     0,     0,   261,     0,   766,   262,
       0,     0,   303,   304,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,   263,     0,     0,   636,     0,     0,
       0,     0,     0,     0,   265,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,     0,     0,   259,   260,
       0,     0,     0,   261,     0,   767,   262,     0,     0,   316,
     317,   318,   281,   282,   283,   284,   319,   320,     0,   638,
       0,   263,     0,     0,   286,     0,   321,     0,   322,   323,
      44,   265,   155,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   300,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
     256,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,   307,   308,   309,   310,   311,
     312,   313,   314,    43,   315,   257,     0,     0,     0,     0,
       0,     0,   281,   282,   283,   284,     0,     0,   316,   317,
     318,     0,     0,     0,   286,   319,   320,   256,     0,     0,
       0,     0,     0,     0,     0,   321,   290,   322,   323,    44,
     292,   155,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,   257,     0,     0,     0,     0,     0,   180,   181,
     182,   258,   184,   185,   186,   187,   188,   300,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   301,
     259,   260,     0,     0,     0,   261,     0,   768,   262,     0,
       0,   303,   304,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,   306,   307,   308,   309,   310,   311,
     312,   313,   314,    43,   315,     0,     0,   259,   260,     0,
       0,     0,   261,     0,   803,   262,     0,     0,   316,   317,
     318,   281,   282,   283,   284,   319,   320,     0,     0,     0,
     263,     0,     0,   286,     0,   321,   652,   322,   323,    44,
     265,   155,     0,     0,     0,   290,     0,     0,     0,   292,
       0,     0,   293,     0,     0,   294,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   300,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,     0,     0,   306,   307,   308,   309,   310,   311,   312,
     313,   314,    43,   315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,   316,   317,   318,
     281,   282,   283,   284,   319,   320,     0,     0,     0,     0,
       0,     0,   286,     0,   321,   683,   322,   323,    44,     0,
     155,     0,     0,     0,   290,     0,     0,     0,   292,     0,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,   258,
     184,   185,   186,   187,   188,   300,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   301,   259,   260,
       0,     0,     0,   261,     0,   824,   262,     0,     0,   303,
     304,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   256,     0,     0,
       0,   265,   306,   307,   308,   309,   310,   311,   312,   313,
     314,    43,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,     0,     0,     0,   316,   317,   318,   281,
     282,   283,   284,   319,   320,     0,     0,     0,     0,     0,
       0,   286,     0,   321,   701,   322,   323,    44,     0,   155,
       0,     0,     0,   290,     0,     0,     0,   292,     0,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,   258,   184,
     185,   186,   187,   188,   300,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   301,   259,   260,     0,
       0,     0,   261,     0,   852,   262,     0,     0,   303,   304,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   256,     0,     0,     0,
     265,   306,   307,   308,   309,   310,   311,   312,   313,   314,
      43,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,     0,     0,     0,   316,   317,   318,   281,   282,
     283,   284,   319,   320,     0,     0,     0,     0,     0,     0,
     286,     0,   321,   710,   322,   323,    44,     0,   155,     0,
       0,     0,   290,     0,     0,     0,   292,     0,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,   258,   184,   185,
     186,   187,   188,   300,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   301,   259,   260,     0,     0,
       0,   261,     0,   854,   262,     0,     0,   303,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,   256,     0,     0,     0,   265,
     306,   307,   308,   309,   310,   311,   312,   313,   314,    43,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,     0,     0,     0,   316,   317,   318,   281,   282,   283,
     284,   319,   320,     0,     0,     0,     0,     0,     0,   286,
       0,   321,   797,   322,   323,    44,     0,   155,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   258,   184,   185,   186,
     187,   188,   300,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   301,   259,   260,     0,     0,     0,
     261,     0,   881,   262,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   256,     0,     0,     0,   265,   306,
     307,   308,   309,   310,   311,   312,   313,   314,    43,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
       0,     0,     0,   316,   317,   318,   281,   282,   283,   284,
     319,   320,     0,     0,     0,     0,     0,     0,   286,     0,
     321,   814,   322,   323,    44,     0,   155,     0,     0,     0,
     290,     0,     0,     0,   292,     0,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,   258,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,   259,   260,     0,     0,     0,   261,
       0,   886,   262,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,   828,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     835,     0,   316,   317,   318,     0,     0,     0,   286,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,   321,
     290,   322,   323,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,   256,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,   257,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     868,     0,   316,   317,   318,     0,     0,     0,   286,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,   321,
     290,   322,   323,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,   258,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,   259,   260,     0,     0,     0,   261,
       0,     0,   262,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
       0,     0,   316,   317,   318,     0,     0,     0,   286,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,   321,
     290,   322,   323,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,   303,   304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,   317,   318,     0,     0,     0,     0,   319,
     320,     0,     0,     0,     0,     0,     0,     0,     0,   321,
       0,   322,   323,    44,     0,   155,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   552,   487,   488,   489,   490,
     465,   466,   491,   492,   493,   494,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,   498,     0,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,   510,     0,
       0,     0,     0,     0,     0,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   552,   487,   488,   489,   490,   465,   466,   491,
     492,   493,   494,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   553,   498,     0,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,     0,     0,     0,     0,     0,
       0,     0,     0,   509,     0,   510,     0,     0,     0,     0,
       0,     0,   567,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   552,
     487,   488,   489,   490,   465,   466,   491,   492,   493,   494,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   553,
     498,     0,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
     509,     0,   510,     0,     0,     0,     0,     0,     0,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   552,   487,   488,   489,
     490,   465,   466,   491,   492,   493,   494,   495,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   553,   498,     0,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,   509,     0,   510,
       0,     0,     0,     0,     0,     0,   696,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   552,   487,   488,   489,   490,   465,   466,
     491,   492,   493,   494,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   553,   498,     0,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,   509,     0,   510,     0,     0,     0,
       0,     0,     0,   744,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
       0,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     552,   487,   488,   489,   490,     0,     0,   491,   492,   493,
     494,   495,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     553,   498,     0,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,     0,     0,   465,   466,     0,     0,     0,
       0,   509,     0,   510,   475,   476,   477,   478,   479,     0,
     802,   482,   483,   484,   485,     0,   487,   488,   489,   490,
       0,     0,   491,     0,   493,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   498,     0,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,     0,     0,
       0,     0,     0,   465,   466,     0,   509,     0,   510,     0,
       0,     0,     0,     0,     0,   858,   475,   476,   477,   478,
     479,     0,     0,   482,   483,   484,   485,     0,   487,   488,
     489,   490,     0,     0,   491,     0,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   498,     0,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
       0,   465,   466,     0,     0,     0,     0,     0,   509,     0,
     510,     0,     0,   591,   475,   476,   477,   478,   479,     0,
       0,   482,   483,   484,   485,     0,   487,   488,   489,   490,
       0,     0,   491,     0,   493,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   498,     0,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,     0,   465,
     466,     0,     0,     0,     0,     0,   509,     0,   510,     0,
       0,   690,   475,   476,   477,   478,   479,     0,     0,   482,
     483,   484,   485,     0,   487,   488,   489,   490,     0,     0,
     491,     0,   493,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   498,     0,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,     0,   465,   466,     0,
       0,     0,     0,     0,   509,     0,   510,     0,     0,   693,
     475,   476,   477,   478,   479,     0,     0,   482,   483,   484,
     485,     0,   487,   488,   489,   490,     0,     0,   491,     0,
     493,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,     0,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,     0,   465,   466,     0,     0,     0,
       0,     0,   509,     0,   510,     0,     0,   765,   475,   476,
     477,   478,   479,     0,     0,   482,   483,   484,   485,     0,
     487,   488,   489,   490,     0,     0,   491,     0,   493,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     498,     0,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,     0,   465,   466,     0,     0,     0,     0,     0,
     509,     0,   510,     0,     0,   841,   475,   476,   477,   478,
     479,     0,     0,   482,   483,   484,   485,     0,   487,   488,
     489,   490,     0,     0,   491,     0,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   498,     0,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,   509,     0,
     510,     0,     0,   856,   475,   476,   477,   478,   479,     0,
       0,   482,   483,   484,   485,     0,   487,   488,   489,   490,
       0,     0,   491,     0,   493,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,   177,     0,     0,   498,     0,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,     0,   178,
       0,     0,     0,     0,     0,     0,   509,     0,   510,     0,
       0,   877,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,   306,
     307,   308,     0,     0,     0,     0,     0,     0,    43,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   465,   466,     0,    44,     0,     0,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   465,   466,
       0,    44,   475,   476,   477,   478,   479,     0,     0,   482,
     483,   484,   485,     0,   487,   488,   489,   490,     0,     0,
     491,     0,   493,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,     0,   498,     0,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,   509,     0,   510,   541,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,    44,   475,
     476,   477,   478,   479,     0,     0,   482,   483,   484,   485,
       0,   487,   488,   489,   490,     0,     0,   491,     0,   493,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   498,     0,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,     0,     0,     0,     0,     0,     0,     0,
       0,   509,     0,   510,   733,   465,   466,     0,     0,   475,
     476,   477,   478,   479,     0,     0,   482,   483,   484,   485,
       0,   487,   488,   489,   490,     0,     0,   491,     0,   493,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   498,     0,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,     0,     0,     0,     0,     0,     0,     0,
       0,   509,     0,   510,   737,     0,     0,     0,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   552,   487,   488,
     489,   490,  -208,     0,   491,   492,   493,   494,   495,   496,
     465,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   553,   498,     0,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,   509,     0,
     510,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   465,   466,     0,     0,     0,
       0,     0,     0,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,     0,     0,   491,
     492,   493,   494,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,     0,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   465,   466,     0,     0,     0,
       0,     0,     0,   509,     0,   510,   475,   476,   477,   478,
     479,     0,     0,   482,   483,   484,   485,     0,   487,   488,
     489,   490,     0,     0,   491,     0,   493,   494,     0,     0,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   498,     0,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     465,   466,     0,     0,     0,     0,     0,     0,   509,     0,
     510,     0,     0,     0,     0,     0,   475,   476,   477,   478,
     479,     0,     0,   482,   483,   484,   485,     0,   487,   488,
     489,   490,     0,     0,   491,     0,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   498,   736,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     465,   466,     0,     0,     0,     0,     0,     0,   509,     0,
     510,   475,   476,   477,   478,   479,     0,     0,   482,   483,
     484,   485,     0,   487,   488,   489,   490,     0,     0,   491,
       0,   493,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     785,     0,     0,   498,     0,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   465,   466,     0,     0,     0,
       0,     0,     0,   509,     0,   510,     0,     0,     0,     0,
       0,   475,   476,   477,   478,   479,     0,     0,   482,   483,
     484,   485,     0,   487,   488,   489,   490,     0,     0,   491,
       0,   493,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   498,     0,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   465,   466,     0,     0,     0,
       0,     0,     0,   509,     0,   510,   475,   476,   477,   478,
     479,     0,     0,   482,   483,   484,   485,     0,   487,   488,
     489,   490,     0,     0,   491,     0,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     465,   466,     0,     0,     0,     0,     0,     0,   509,     0,
     510,     0,     0,     0,     0,     0,   475,   476,   477,   478,
     479,     0,     0,   482,   483,   484,   485,     0,   487,   488,
     489,   490,     0,     0,   491,     0,   493,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,   505,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,   509,     0,
     510,   475,   476,   477,   478,   479,     0,     0,   482,     0,
       0,   485,     0,   487,   488,   489,   490,     0,     0,   491,
       0,   493,   494,     0,     0,     0,     0,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,     0,     0,     0,   502,   503,
     504,   505,   506,   507,   508,   135,   136,   137,   138,     0,
       0,     0,     0,   509,     0,   510,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
       0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      13,    81,   109,   272,   230,   268,   270,   285,   379,   308,
     381,   511,   383,     7,   305,   575,    73,    33,   439,   109,
       0,   160,   596,   583,   302,    32,     6,    14,    15,   530,
      32,   532,    21,   534,   125,    14,    15,    19,   117,    50,
     111,   126,    47,   114,   115,    19,    34,   547,   548,    29,
      57,    31,    18,    33,   135,    35,    19,   144,   112,   135,
      40,    74,   349,   135,   141,    54,   120,    47,   135,   114,
     115,   135,   135,    53,   166,   135,   149,   364,   173,   135,
     172,   148,   173,   168,   135,   135,   173,   129,   142,    69,
     171,   462,   163,   172,   165,   172,   109,   141,   174,   172,
     141,   602,   174,   167,   171,   112,   113,    20,    21,   141,
      90,   174,   141,   120,    32,   103,   104,   171,   174,   135,
     165,   228,    58,   174,   174,   172,   404,   174,   172,   703,
     135,   172,   126,   411,   166,   142,   143,   166,   228,    57,
     147,   280,   129,   150,   157,   147,   133,   135,   135,   136,
     129,   429,   112,   144,   133,   171,   135,   136,   165,   141,
     267,   172,   135,   165,   153,   172,   223,   141,   175,   135,
     135,   165,   463,   175,   163,   112,   283,   267,   141,   135,
     174,   141,   142,   120,   144,   165,   141,   147,   167,   142,
     103,   104,   172,   283,   141,   113,   112,   697,   111,   306,
     307,   114,   115,   116,   120,   142,   119,   142,   288,   709,
     112,   141,   144,   773,   141,   228,   306,   307,   120,   172,
     141,   168,   782,   168,   142,   143,   142,   172,   135,   147,
      32,   731,   150,   141,    20,    21,   141,   141,   168,   171,
     142,   168,   141,   521,   167,   166,   144,   165,   135,   167,
     163,   149,   165,   135,   267,    57,   144,   175,   149,   366,
     168,   149,   171,   168,   168,   372,   373,   374,   375,   167,
     283,   378,   135,   380,   543,   382,   366,   384,   569,   167,
     701,   172,   372,   373,   374,   375,   787,   586,   378,   710,
     380,   144,   382,   306,   307,   402,   149,   436,   135,   438,
     149,   137,   138,   139,    11,    20,    21,   141,   173,   149,
     144,   113,   402,   147,   167,    22,    23,   103,   104,   144,
     129,   428,   461,   172,   149,   111,   406,   113,   114,   115,
     116,    32,   172,   119,     4,     5,   135,     7,   428,   419,
     142,   143,   167,   135,   172,   147,   174,   718,   150,   142,
     169,   170,   135,   366,   173,   173,    57,   137,   138,   372,
     373,   374,   375,   165,   135,   378,   173,   380,    34,   382,
     172,   384,    34,   175,   454,    43,    44,   163,   167,   165,
     144,     1,   660,   173,   137,    54,   101,   102,   173,   402,
     105,   135,   656,   108,   109,   110,   111,   410,   113,   114,
     115,   116,   135,   144,   119,   173,   121,   122,   148,   172,
     148,   112,   113,   148,   148,   428,   679,   167,   148,   148,
     148,   148,   148,   148,   172,   144,   172,   135,   143,    32,
     145,   146,   147,   148,   149,   135,   126,   450,   153,   154,
     172,   142,   143,   167,   167,   148,   147,   167,   163,   150,
     165,   558,   559,   148,   561,   148,   129,   148,    64,   170,
      12,    36,    68,   172,   165,    19,   172,   172,   558,   559,
     172,   561,    24,   172,   175,   555,   165,   135,    30,    85,
      86,    87,    88,   167,   166,   168,   166,   117,    13,   144,
     129,    39,   173,   719,    46,   135,     7,   135,    20,    21,
     135,   135,    43,   166,   141,   774,   137,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,   172,
     141,   135,   174,   148,   167,   558,   559,   135,   561,   135,
     149,   149,   149,   148,   135,   167,   135,   664,   166,   166,
     174,   135,   166,   174,   166,   149,   166,   149,   675,    50,
     677,   148,   168,   166,   664,   443,   166,   166,   148,   101,
     102,   103,   104,   135,    55,   675,   166,   172,   166,   111,
      98,   113,   114,   115,   116,   168,   168,   119,   174,   121,
     122,   166,   166,    58,   826,   168,    56,    74,   800,   740,
     717,   240,     1,   594,   654,   364,   677,   274,   385,   171,
     517,    68,   461,   173,   283,   377,   708,   717,   150,   151,
     152,   153,   154,   439,   846,    -1,    -1,    -1,    -1,   230,
     720,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   729,
      -1,   664,   243,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   675,    -1,   677,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   263,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   761,    -1,    -1,    -1,    -1,   277,    -1,    -1,   796,
      -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,   289,    32,
     291,   808,    -1,    -1,   717,    -1,   796,    -1,   299,    -1,
     790,   302,   303,   304,    -1,    -1,    -1,   308,   808,    32,
      -1,    -1,    -1,    -1,    57,   316,   317,   318,   319,   320,
     321,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   329,   103,
     104,    -1,    -1,    -1,    57,    -1,    -1,   111,    -1,   113,
     114,   115,   116,    -1,    -1,   119,   103,   104,    -1,    -1,
     867,    -1,    -1,    -1,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,    -1,   882,    -1,   867,    -1,    -1,
     113,    -1,    -1,   796,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   882,    -1,    -1,   808,    -1,    -1,    -1,   163,
     113,   165,    -1,    -1,    -1,   152,   153,   154,    -1,   142,
     143,    -1,    -1,   404,   147,    -1,   163,   150,   165,    -1,
     411,    -1,    -1,    -1,    -1,    -1,    -1,   418,    -1,   142,
     143,   422,   165,    -1,   147,   848,   149,   150,   429,   172,
      -1,    -1,   175,    -1,    20,    21,   437,    -1,   439,    -1,
      -1,   864,   165,    -1,   867,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,   882,
      -1,    -1,    -1,    -1,    -1,    -1,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,    -1,    -1,   479,   480,
     481,   482,   483,   484,    -1,   486,   487,    -1,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,    -1,   510,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,   105,
     521,    -1,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,
      -1,    -1,    12,    20,    21,    -1,    -1,    -1,   549,    -1,
      -1,   552,   553,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
     571,   572,    -1,    -1,    -1,    -1,    46,   163,    -1,   165,
      -1,   582,    -1,    -1,   585,   586,    -1,    -1,   589,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    -1,   624,   101,   102,   103,   104,   105,    -1,
      -1,   108,   109,   110,   111,   636,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   660,
      -1,    -1,    -1,    -1,    -1,   135,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
     681,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   699,   700,
      -1,   171,    -1,    -1,    -1,   706,    -1,   708,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   719,    -1,
      -1,    -1,   723,    -1,    -1,    -1,   727,    -1,    -1,   101,
     102,   103,   104,   105,    -1,   736,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   769,    -1,
      -1,   143,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,   793,   165,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   809,    -1,
     811,   812,   813,    -1,    -1,   816,    -1,   818,   819,    -1,
     821,    -1,    -1,    -1,     1,    -1,    -1,   828,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,   857,    -1,    -1,    -1,
      37,    38,    -1,    -1,    41,    42,    -1,    -1,    45,   870,
      47,    48,    49,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,     1,    -1,
      -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,
      -1,    -1,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    -1,    -1,    60,    61,    62,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,
      -1,    -1,    -1,    -1,   157,   158,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,   167,    -1,   169,   170,   171,   172,
     173,   174,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    60,    61,    62,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,    -1,    -1,   147,    -1,    -1,   150,   151,   152,    -1,
      -1,    -1,    -1,   157,   158,    -1,    14,    15,    16,    17,
      -1,   165,    -1,   167,    -1,   169,   170,   171,    26,   173,
      -1,   175,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,   157,
     158,    -1,    14,    15,    16,    17,    -1,   165,    -1,   167,
      -1,   169,   170,   171,    26,   173,    -1,   175,    -1,    32,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    57,    -1,    -1,    -1,    60,    61,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,   142,
     143,    -1,    -1,    -1,   147,   147,   149,   150,   150,   151,
     152,    -1,    -1,    -1,    -1,   157,   158,    -1,    14,    15,
      16,    17,   165,   165,    -1,   167,    -1,   169,   170,   171,
      26,   173,   175,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      86,    87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,   150,   151,   152,    14,    15,    16,
      17,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   167,    -1,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,   113,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    86,
      87,    88,    -1,    -1,    91,   142,   143,    -1,    -1,    -1,
     147,    -1,   149,   150,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,   165,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,   175,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,   150,   151,   152,    -1,    -1,    -1,    26,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    38,   169,   170,   171,    42,   173,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,
     157,   158,    -1,    14,    15,    16,    17,    18,   165,    -1,
     167,   168,   169,   170,   171,    26,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,   148,    -1,   150,
     151,   152,    -1,    -1,    -1,    26,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    38,   169,   170,
     171,    42,   173,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,   113,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,   142,   143,    -1,    -1,    -1,   147,    -1,   149,   150,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   150,
     151,   152,    -1,    -1,    -1,    26,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,   167,    38,   169,   170,
     171,    42,   173,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,
      61,    62,   113,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,   142,   143,    -1,    -1,    -1,   147,    -1,   149,   150,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   150,
     151,   152,    -1,    -1,    -1,    26,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    38,   169,   170,
     171,    42,   173,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,   113,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,   142,   143,    -1,    -1,    -1,   147,    -1,   149,   150,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,   165,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   150,
     151,   152,    -1,    -1,    -1,    26,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    38,   169,   170,
     171,    42,   173,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,   113,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,   142,   143,    -1,    -1,    -1,   147,    -1,   149,   150,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,   165,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   150,
     151,   152,    -1,    -1,    -1,    26,   157,   158,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    38,   169,   170,
     171,    42,   173,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    57,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,   113,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,   142,   143,    -1,    -1,    -1,   147,    -1,   149,   150,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   165,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,   142,   143,
      -1,    -1,    -1,   147,    -1,   149,   150,    -1,    -1,   150,
     151,   152,    14,    15,    16,    17,   157,   158,    -1,    21,
      -1,   165,    -1,    -1,    26,    -1,   167,    -1,   169,   170,
     171,   175,   173,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,   150,   151,
     152,    -1,    -1,    -1,    26,   157,   158,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    38,   169,   170,   171,
      42,   173,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    57,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,   113,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,
     142,   143,    -1,    -1,    -1,   147,    -1,   149,   150,    -1,
      -1,   103,   104,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    -1,    -1,   142,   143,    -1,
      -1,    -1,   147,    -1,   149,   150,    -1,    -1,   150,   151,
     152,    14,    15,    16,    17,   157,   158,    -1,    -1,    -1,
     165,    -1,    -1,    26,    -1,   167,   168,   169,   170,   171,
     175,   173,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,   150,   151,   152,
      14,    15,    16,    17,   157,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   167,   168,   169,   170,   171,    -1,
     173,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,   113,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    86,    87,    88,    -1,    -1,    91,   142,   143,
      -1,    -1,    -1,   147,    -1,   149,   150,    -1,    -1,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   175,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,   150,   151,   152,    14,
      15,    16,    17,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   167,   168,   169,   170,   171,    -1,   173,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,   113,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    86,    87,    88,    -1,    -1,    91,   142,   143,    -1,
      -1,    -1,   147,    -1,   149,   150,    -1,    -1,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     175,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,   150,   151,   152,    14,    15,
      16,    17,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   167,   168,   169,   170,   171,    -1,   173,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,   113,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      86,    87,    88,    -1,    -1,    91,   142,   143,    -1,    -1,
      -1,   147,    -1,   149,   150,    -1,    -1,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   175,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,   150,   151,   152,    14,    15,    16,
      17,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   167,   168,   169,   170,   171,    -1,   173,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,   113,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    86,
      87,    88,    -1,    -1,    91,   142,   143,    -1,    -1,    -1,
     147,    -1,   149,   150,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   175,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,   150,   151,   152,    14,    15,    16,    17,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     167,   168,   169,   170,   171,    -1,   173,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,   113,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,   142,   143,    -1,    -1,    -1,   147,
      -1,   149,   150,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   150,   151,   152,    -1,    -1,    -1,    26,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      38,   169,   170,   171,    42,   173,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   150,   151,   152,    -1,    -1,    -1,    26,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      38,   169,   170,   171,    42,   173,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,   113,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,   142,   143,    -1,    -1,    -1,   147,
      -1,    -1,   150,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   150,   151,   152,    -1,    -1,    -1,    26,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      38,   169,   170,   171,    42,   173,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,    -1,   173,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      20,    21,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    20,    21,   119,   120,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    20,    21,   119,   120,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    20,    21,
     119,   120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    -1,    -1,   119,   120,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,   163,    -1,   165,   101,   102,   103,   104,   105,    -1,
     172,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,   172,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,   168,   101,   102,   103,   104,   105,    -1,
      -1,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,   168,   101,   102,   103,   104,   105,    -1,    -1,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,   168,
     101,   102,   103,   104,   105,    -1,    -1,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   165,    -1,    -1,   168,   101,   102,
     103,   104,   105,    -1,    -1,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,   168,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,   168,   101,   102,   103,   104,   105,    -1,
      -1,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,   168,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,   135,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    20,    21,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    20,    21,
      -1,   171,   101,   102,   103,   104,   105,    -1,    -1,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,   166,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   101,
     102,   103,   104,   105,    -1,    -1,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,   165,   166,    20,    21,    -1,    -1,   101,
     102,   103,   104,   105,    -1,    -1,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,   165,   166,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,   123,   124,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
     120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,   101,   102,   103,   104,   105,    -1,    -1,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,   105,    -1,    -1,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,   101,   102,   103,   104,   105,    -1,    -1,   108,    -1,
      -1,   111,    -1,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,   157,   158
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   177,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    69,    90,   165,   172,   178,   183,   184,   188,   207,
     211,   227,   278,   283,   284,   288,   324,   326,   135,   202,
     203,   129,   189,   190,   135,   185,   186,   135,   173,   135,
     327,   325,    33,   135,   171,   204,   205,   206,   218,     4,
       5,     7,   287,    58,   277,   142,   141,   144,   141,    21,
      54,   153,   163,   187,   144,   173,   135,   281,   282,   135,
     135,   171,   135,   167,   141,   166,    34,   103,   104,   135,
     210,   212,   213,   135,   286,   173,   279,    14,    15,   129,
     133,   135,   136,   167,   180,   202,   129,   190,   135,   135,
     135,    64,    68,    85,    86,    87,    88,   292,   280,   142,
     174,   281,   173,   173,   135,   203,   205,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   121,   122,   123,   124,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   157,
     158,    34,    34,   167,   208,   173,   214,   144,   285,   289,
     276,     1,   137,   179,    14,    15,   129,   133,   135,   180,
     200,   201,   187,   173,   135,   174,    24,    30,    46,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   218,   291,   293,   294,   296,   299,   135,   295,   135,
     269,   270,   168,   126,   168,   260,   265,   266,   144,   209,
     216,   218,   173,   135,   174,   275,   290,   172,   137,   138,
     141,   168,   280,   142,   172,   148,   148,   148,   148,   167,
     148,   148,   148,   148,   148,   148,    32,    57,   113,   142,
     143,   147,   150,   165,   172,   175,   172,   144,   172,   203,
      47,   272,   290,   168,   172,   299,     1,     8,     9,    10,
      12,    14,    15,    16,    17,    25,    26,    27,    28,    37,
      38,    41,    42,    45,    48,    49,    51,    52,    55,    56,
      69,    91,    92,   103,   104,   118,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   136,   150,   151,   152,   157,
     158,   167,   169,   170,   172,   174,   182,   191,   192,   195,
     196,   197,   198,   199,   214,   215,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   227,   228,   229,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   257,   291,   328,
     332,   337,   339,   341,   263,   141,   144,   147,   274,   200,
     174,   257,   302,   304,   306,   300,   135,   297,   308,   310,
     312,   314,   316,   318,   320,   322,    32,    32,   147,   165,
     175,   166,   257,   135,   174,   299,   174,   269,   166,   272,
     112,   142,   144,   147,   271,   265,   257,   290,   299,   328,
     167,   112,   172,   217,   248,   257,   214,   257,   167,   257,
     148,   148,   167,   172,   148,   129,    55,   257,   148,   112,
     217,   257,   257,   257,   170,   246,    12,   299,    12,   299,
     257,   334,   338,   181,   257,   257,   257,   257,   257,   257,
     170,    36,   172,   172,   257,   135,   167,   172,   172,   172,
      19,   226,   207,   117,   172,    20,    21,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   119,   120,   121,   122,   123,   124,   142,   143,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   163,
     165,   167,   174,   207,   260,   262,   135,   299,   112,   120,
     142,   273,   172,   299,   299,   299,   299,   168,   295,   299,
     208,   299,   208,   299,   208,   135,   267,   268,   299,   270,
     166,   166,   166,   299,   217,   257,   214,    19,   167,   218,
     217,   257,   112,   142,   172,    13,   257,   214,   230,   232,
     257,   234,   144,   129,   299,   217,   257,   172,   290,    50,
     135,   150,   167,   257,   329,   330,   331,   333,   334,   335,
     336,   290,   167,   330,   336,   125,   172,   174,   138,   139,
     179,   168,   148,   218,   173,   214,   165,   168,   243,   257,
      39,   275,   208,   246,    18,   135,   135,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   135,   118,   257,   257,   135,   257,   257,
     257,   257,   257,   257,   257,   257,   118,   257,    21,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     135,   257,   168,   243,     7,    43,   261,   172,   142,   172,
     273,   217,   257,   149,   172,   149,   149,   149,   172,   149,
     209,   149,   209,   149,   209,   144,   149,   172,   149,   172,
     142,   271,   243,   168,   243,    18,   148,   257,   172,   214,
     168,   299,   299,   168,   299,   149,   172,    19,   246,   120,
     271,   168,   141,   172,   331,   166,   141,   166,   172,    19,
     168,   331,   174,   257,   334,   174,   257,   253,   255,   216,
      11,    22,    23,   193,   194,   135,   329,   141,   168,   209,
     148,   167,   257,   166,   257,   135,   144,   166,   168,    44,
     261,   272,   217,   257,   172,   303,   299,   307,   301,   298,
     309,   149,   313,   149,   317,   149,   299,   321,   267,   323,
     257,   172,   168,   148,   135,   168,   149,   149,   149,   167,
     243,   257,   257,   330,   135,   329,   166,   166,   257,   166,
     335,   243,   330,   174,   166,   140,   299,   208,   174,   214,
     257,   166,   257,   125,   214,   244,   258,   168,   243,   257,
     264,   213,   172,   149,   311,   315,   319,   214,   237,   149,
     172,   231,   233,   235,   168,   257,   172,   331,   120,   271,
     166,   172,   331,   166,   149,   209,   214,   168,   112,   257,
     299,   168,   212,   305,   299,    18,   257,   135,   257,   257,
     257,   168,   257,   166,   257,   257,   257,   174,   254,   149,
     194,   257,   149,   172,   149,   148,   168,   149,   172,   340,
     166,   340,   166,   218,   256,   259,   238,   239,    18,   257,
      50,   166,   174,   218,   168,   299,   148,   168,   257,   166,
     166,   149,   241,   240,   299,   168,   149,   242,   168
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   176,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   178,   179,   179,   180,   181,
     181,   181,   182,   183,   184,   185,   185,   185,   186,   186,
     187,   187,   188,   189,   189,   190,   190,   191,   192,   192,
     193,   193,   194,   194,   194,   195,   195,   196,   197,   198,
     199,   200,   200,   200,   200,   200,   200,   201,   201,   202,
     202,   202,   202,   202,   202,   202,   202,   203,   203,   204,
     204,   205,   205,   206,   206,   207,   207,   208,   208,   208,
     209,   209,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   211,
     212,   213,   214,   214,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   216,   216,   217,   217,   218,   218,   218,
     219,   220,   220,   220,   220,   221,   222,   223,   223,   223,
     223,   223,   224,   224,   224,   224,   225,   226,   226,   227,
     227,   228,   230,   231,   229,   232,   233,   229,   234,   235,
     229,   236,   236,   236,   237,   238,   236,   239,   240,   236,
     241,   242,   236,   243,   243,   244,   244,   244,   245,   245,
     246,   246,   247,   247,   247,   247,   247,   247,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   249,   249,   250,   251,   251,   252,
     253,   254,   252,   255,   256,   252,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   258,   259,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   260,   260,   261,   261,   262,   263,   263,   264,   263,
     263,   265,   265,   266,   266,   267,   267,   268,   268,   269,
     270,   270,   271,   271,   272,   272,   272,   272,   272,   272,
     273,   273,   273,   274,   274,   275,   275,   275,   275,   275,
     276,   276,   277,   277,   278,   279,   278,   280,   280,   280,
     281,   282,   282,   283,   284,   284,   285,   285,   286,   287,
     287,   289,   288,   290,   290,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     292,   292,   292,   292,   292,   292,   293,   294,   294,   295,
     295,   297,   298,   296,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     300,   301,   299,   299,   302,   303,   299,   304,   305,   299,
     306,   307,   299,   299,   308,   309,   299,   310,   311,   299,
     299,   312,   313,   299,   314,   315,   299,   299,   316,   317,
     299,   318,   319,   299,   320,   321,   299,   322,   323,   299,
     325,   324,   327,   326,   328,   328,   328,   328,   329,   329,
     329,   329,   330,   330,   331,   331,   332,   332,   332,   332,
     332,   333,   333,   334,   335,   335,   336,   336,   337,   337,
     338,   338,   339,   340,   340,   341,   341
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
       3,     2,     3,     7,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     2,     1,     3,     2,
       2,     2,     4,     5,     2,     1,     1,     2,     3,     4,
       2,     3,     3,     4,     2,     3,     4,     0,     2,     1,
       1,     3,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     5,     8,    10,     0,     0,    10,     0,     0,    13,
       0,     0,    15,     1,     3,     1,     2,     3,     1,     1,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     4,     6,     5,     6,     3,
       0,     0,     8,     0,     0,     9,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     4,     3,     3,     1,     3,     4,     3,     4,     2,
       4,     4,     6,     7,     3,     5,     0,     0,     8,     3,
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
       4,     2,     3,     2,     3,     2,     2,     3,     3,     2,
       0,     0,     6,     2,     0,     0,     6,     0,     0,     8,
       0,     0,     6,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     0,     0,     6,     0,     0,     6,
       0,     7,     0,     7,     1,     1,     1,     1,     3,     3,
       5,     5,     1,     3,     0,     2,     6,     5,     8,     6,
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
    case 135: /* "name"  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3232 "ds_parser.cpp"
        break;

    case 179: /* character_sequence  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3238 "ds_parser.cpp"
        break;

    case 180: /* string_constant  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3244 "ds_parser.cpp"
        break;

    case 181: /* string_builder_body  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3250 "ds_parser.cpp"
        break;

    case 182: /* string_builder  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3256 "ds_parser.cpp"
        break;

    case 185: /* require_module_name  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3262 "ds_parser.cpp"
        break;

    case 191: /* expression_label  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3268 "ds_parser.cpp"
        break;

    case 192: /* expression_goto  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3274 "ds_parser.cpp"
        break;

    case 194: /* expression_else  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3280 "ds_parser.cpp"
        break;

    case 196: /* expression_if_then_else  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3286 "ds_parser.cpp"
        break;

    case 197: /* expression_for_loop  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3292 "ds_parser.cpp"
        break;

    case 198: /* expression_while_loop  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3298 "ds_parser.cpp"
        break;

    case 199: /* expression_with  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3304 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument_value  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3310 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument_value_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3316 "ds_parser.cpp"
        break;

    case 202: /* annotation_argument  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3322 "ds_parser.cpp"
        break;

    case 203: /* annotation_argument_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3328 "ds_parser.cpp"
        break;

    case 204: /* annotation_declaration_name  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3334 "ds_parser.cpp"
        break;

    case 205: /* annotation_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3340 "ds_parser.cpp"
        break;

    case 206: /* annotation_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3346 "ds_parser.cpp"
        break;

    case 207: /* optional_annotation_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3352 "ds_parser.cpp"
        break;

    case 208: /* optional_function_argument_list  */
#line 119 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3358 "ds_parser.cpp"
        break;

    case 209: /* optional_function_type  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3364 "ds_parser.cpp"
        break;

    case 210: /* function_name  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3370 "ds_parser.cpp"
        break;

    case 214: /* expression_block  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3376 "ds_parser.cpp"
        break;

    case 215: /* expression_any  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3382 "ds_parser.cpp"
        break;

    case 216: /* expressions  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3388 "ds_parser.cpp"
        break;

    case 217: /* expr_pipe  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3394 "ds_parser.cpp"
        break;

    case 218: /* name_in_namespace  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3400 "ds_parser.cpp"
        break;

    case 219: /* expression_delete  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3406 "ds_parser.cpp"
        break;

    case 220: /* expr_new  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3412 "ds_parser.cpp"
        break;

    case 221: /* expression_break  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3418 "ds_parser.cpp"
        break;

    case 222: /* expression_continue  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3424 "ds_parser.cpp"
        break;

    case 223: /* expression_return  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3430 "ds_parser.cpp"
        break;

    case 224: /* expression_yield  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3436 "ds_parser.cpp"
        break;

    case 225: /* expression_try_catch  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3442 "ds_parser.cpp"
        break;

    case 228: /* expression_let  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3448 "ds_parser.cpp"
        break;

    case 229: /* expr_cast  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3454 "ds_parser.cpp"
        break;

    case 236: /* expr_type_info  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3460 "ds_parser.cpp"
        break;

    case 243: /* expr_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3466 "ds_parser.cpp"
        break;

    case 244: /* block_or_simple_block  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3472 "ds_parser.cpp"
        break;

    case 246: /* expr_block  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3478 "ds_parser.cpp"
        break;

    case 247: /* expr_numeric_const  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3484 "ds_parser.cpp"
        break;

    case 248: /* expr_assign  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3490 "ds_parser.cpp"
        break;

    case 249: /* expr_assign_pipe  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3496 "ds_parser.cpp"
        break;

    case 250: /* expr_named_call  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3502 "ds_parser.cpp"
        break;

    case 251: /* expr_method_call  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3508 "ds_parser.cpp"
        break;

    case 252: /* func_addr_expr  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3514 "ds_parser.cpp"
        break;

    case 257: /* expr  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3520 "ds_parser.cpp"
        break;

    case 260: /* optional_field_annotation  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3526 "ds_parser.cpp"
        break;

    case 262: /* structure_variable_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3532 "ds_parser.cpp"
        break;

    case 263: /* struct_variable_declaration_list  */
#line 119 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3538 "ds_parser.cpp"
        break;

    case 265: /* function_argument_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3544 "ds_parser.cpp"
        break;

    case 266: /* function_argument_list  */
#line 119 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3550 "ds_parser.cpp"
        break;

    case 267: /* tuple_type  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3556 "ds_parser.cpp"
        break;

    case 268: /* tuple_type_list  */
#line 119 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3562 "ds_parser.cpp"
        break;

    case 269: /* variant_type  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3568 "ds_parser.cpp"
        break;

    case 270: /* variant_type_list  */
#line 119 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3574 "ds_parser.cpp"
        break;

    case 272: /* variable_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3580 "ds_parser.cpp"
        break;

    case 275: /* let_variable_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3586 "ds_parser.cpp"
        break;

    case 276: /* global_variable_declaration_list  */
#line 119 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3592 "ds_parser.cpp"
        break;

    case 280: /* enum_list  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3598 "ds_parser.cpp"
        break;

    case 285: /* optional_structure_parent  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3604 "ds_parser.cpp"
        break;

    case 290: /* variable_name_with_pos_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3610 "ds_parser.cpp"
        break;

    case 293: /* structure_type_declaration  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3616 "ds_parser.cpp"
        break;

    case 294: /* auto_type_declaration  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3622 "ds_parser.cpp"
        break;

    case 295: /* bitfield_bits  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3628 "ds_parser.cpp"
        break;

    case 296: /* bitfield_type_declaration  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3634 "ds_parser.cpp"
        break;

    case 299: /* type_declaration  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3640 "ds_parser.cpp"
        break;

    case 328: /* make_decl  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3646 "ds_parser.cpp"
        break;

    case 329: /* make_struct_fields  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3652 "ds_parser.cpp"
        break;

    case 330: /* make_struct_dim  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp"
        break;

    case 331: /* optional_block  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 332: /* make_struct_decl  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 333: /* make_tuple  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3676 "ds_parser.cpp"
        break;

    case 334: /* make_map_tuple  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3682 "ds_parser.cpp"
        break;

    case 335: /* make_any_tuple  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3688 "ds_parser.cpp"
        break;

    case 336: /* make_dim  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3694 "ds_parser.cpp"
        break;

    case 337: /* make_dim_decl  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3700 "ds_parser.cpp"
        break;

    case 338: /* make_table  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3706 "ds_parser.cpp"
        break;

    case 339: /* make_table_decl  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3712 "ds_parser.cpp"
        break;

    case 340: /* array_comprehension_where  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3718 "ds_parser.cpp"
        break;

    case 341: /* array_comprehension  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3724 "ds_parser.cpp"
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
#line 428 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4024 "ds_parser.cpp"
    break;

  case 16:
#line 440 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4030 "ds_parser.cpp"
    break;

  case 17:
#line 441 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4036 "ds_parser.cpp"
    break;

  case 18:
#line 445 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4042 "ds_parser.cpp"
    break;

  case 19:
#line 449 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4052 "ds_parser.cpp"
    break;

  case 20:
#line 454 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4066 "ds_parser.cpp"
    break;

  case 21:
#line 463 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4076 "ds_parser.cpp"
    break;

  case 22:
#line 471 "ds_parser.ypp"
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
#line 4094 "ds_parser.cpp"
    break;

  case 23:
#line 487 "ds_parser.ypp"
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
#line 4118 "ds_parser.cpp"
    break;

  case 25:
#line 513 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4126 "ds_parser.cpp"
    break;

  case 26:
#line 516 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4137 "ds_parser.cpp"
    break;

  case 27:
#line 522 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4148 "ds_parser.cpp"
    break;

  case 28:
#line 531 "ds_parser.ypp"
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
#line 4174 "ds_parser.cpp"
    break;

  case 29:
#line 552 "ds_parser.ypp"
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
#line 4199 "ds_parser.cpp"
    break;

  case 30:
#line 575 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4205 "ds_parser.cpp"
    break;

  case 31:
#line 576 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4211 "ds_parser.cpp"
    break;

  case 35:
#line 589 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4219 "ds_parser.cpp"
    break;

  case 36:
#line 592 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4227 "ds_parser.cpp"
    break;

  case 37:
#line 598 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4235 "ds_parser.cpp"
    break;

  case 38:
#line 604 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4243 "ds_parser.cpp"
    break;

  case 39:
#line 607 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4251 "ds_parser.cpp"
    break;

  case 40:
#line 613 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4257 "ds_parser.cpp"
    break;

  case 41:
#line 614 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4263 "ds_parser.cpp"
    break;

  case 42:
#line 618 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4269 "ds_parser.cpp"
    break;

  case 43:
#line 619 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4275 "ds_parser.cpp"
    break;

  case 44:
#line 620 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4286 "ds_parser.cpp"
    break;

  case 45:
#line 629 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4292 "ds_parser.cpp"
    break;

  case 46:
#line 630 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4298 "ds_parser.cpp"
    break;

  case 47:
#line 634 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4309 "ds_parser.cpp"
    break;

  case 48:
#line 643 "ds_parser.ypp"
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
#line 4327 "ds_parser.cpp"
    break;

  case 49:
#line 659 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4339 "ds_parser.cpp"
    break;

  case 50:
#line 669 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4350 "ds_parser.cpp"
    break;

  case 51:
#line 678 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4356 "ds_parser.cpp"
    break;

  case 52:
#line 679 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4362 "ds_parser.cpp"
    break;

  case 53:
#line 680 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4368 "ds_parser.cpp"
    break;

  case 54:
#line 681 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4374 "ds_parser.cpp"
    break;

  case 55:
#line 682 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4380 "ds_parser.cpp"
    break;

  case 56:
#line 683 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4386 "ds_parser.cpp"
    break;

  case 57:
#line 687 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4396 "ds_parser.cpp"
    break;

  case 58:
#line 692 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4406 "ds_parser.cpp"
    break;

  case 59:
#line 700 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4412 "ds_parser.cpp"
    break;

  case 60:
#line 701 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4418 "ds_parser.cpp"
    break;

  case 61:
#line 702 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4424 "ds_parser.cpp"
    break;

  case 62:
#line 703 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4430 "ds_parser.cpp"
    break;

  case 63:
#line 704 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4436 "ds_parser.cpp"
    break;

  case 64:
#line 705 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4442 "ds_parser.cpp"
    break;

  case 65:
#line 706 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4448 "ds_parser.cpp"
    break;

  case 66:
#line 707 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4456 "ds_parser.cpp"
    break;

  case 67:
#line 713 "ds_parser.ypp"
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
#line 4474 "ds_parser.cpp"
    break;

  case 68:
#line 726 "ds_parser.ypp"
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
#line 4492 "ds_parser.cpp"
    break;

  case 69:
#line 742 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4498 "ds_parser.cpp"
    break;

  case 70:
#line 743 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4504 "ds_parser.cpp"
    break;

  case 71:
#line 747 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4516 "ds_parser.cpp"
    break;

  case 72:
#line 754 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4530 "ds_parser.cpp"
    break;

  case 73:
#line 766 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4539 "ds_parser.cpp"
    break;

  case 74:
#line 770 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4548 "ds_parser.cpp"
    break;

  case 75:
#line 777 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4554 "ds_parser.cpp"
    break;

  case 76:
#line 778 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4560 "ds_parser.cpp"
    break;

  case 77:
#line 782 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4566 "ds_parser.cpp"
    break;

  case 78:
#line 783 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4572 "ds_parser.cpp"
    break;

  case 79:
#line 784 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4578 "ds_parser.cpp"
    break;

  case 80:
#line 788 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4588 "ds_parser.cpp"
    break;

  case 81:
#line 793 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4597 "ds_parser.cpp"
    break;

  case 82:
#line 800 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4606 "ds_parser.cpp"
    break;

  case 83:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4612 "ds_parser.cpp"
    break;

  case 84:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4618 "ds_parser.cpp"
    break;

  case 85:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4624 "ds_parser.cpp"
    break;

  case 86:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4630 "ds_parser.cpp"
    break;

  case 87:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4636 "ds_parser.cpp"
    break;

  case 88:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4642 "ds_parser.cpp"
    break;

  case 89:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4648 "ds_parser.cpp"
    break;

  case 90:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4654 "ds_parser.cpp"
    break;

  case 91:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4660 "ds_parser.cpp"
    break;

  case 92:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4666 "ds_parser.cpp"
    break;

  case 93:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4672 "ds_parser.cpp"
    break;

  case 94:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4678 "ds_parser.cpp"
    break;

  case 95:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4684 "ds_parser.cpp"
    break;

  case 96:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4690 "ds_parser.cpp"
    break;

  case 97:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4696 "ds_parser.cpp"
    break;

  case 98:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4702 "ds_parser.cpp"
    break;

  case 99:
#line 820 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4708 "ds_parser.cpp"
    break;

  case 100:
#line 821 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4714 "ds_parser.cpp"
    break;

  case 101:
#line 822 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4720 "ds_parser.cpp"
    break;

  case 102:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4726 "ds_parser.cpp"
    break;

  case 103:
#line 824 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4732 "ds_parser.cpp"
    break;

  case 104:
#line 825 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4738 "ds_parser.cpp"
    break;

  case 105:
#line 826 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4744 "ds_parser.cpp"
    break;

  case 106:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4750 "ds_parser.cpp"
    break;

  case 107:
#line 828 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4756 "ds_parser.cpp"
    break;

  case 108:
#line 829 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4762 "ds_parser.cpp"
    break;

  case 109:
#line 830 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4768 "ds_parser.cpp"
    break;

  case 110:
#line 831 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4774 "ds_parser.cpp"
    break;

  case 111:
#line 832 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4780 "ds_parser.cpp"
    break;

  case 112:
#line 833 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4786 "ds_parser.cpp"
    break;

  case 113:
#line 834 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4792 "ds_parser.cpp"
    break;

  case 114:
#line 835 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4798 "ds_parser.cpp"
    break;

  case 115:
#line 836 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4804 "ds_parser.cpp"
    break;

  case 116:
#line 837 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4810 "ds_parser.cpp"
    break;

  case 117:
#line 838 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4816 "ds_parser.cpp"
    break;

  case 118:
#line 839 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4822 "ds_parser.cpp"
    break;

  case 119:
#line 843 "ds_parser.ypp"
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
#line 4845 "ds_parser.cpp"
    break;

  case 120:
#line 864 "ds_parser.ypp"
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
#line 4885 "ds_parser.cpp"
    break;

  case 121:
#line 902 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 4894 "ds_parser.cpp"
    break;

  case 122:
#line 909 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 4903 "ds_parser.cpp"
    break;

  case 123:
#line 913 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 4916 "ds_parser.cpp"
    break;

  case 124:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4922 "ds_parser.cpp"
    break;

  case 125:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4928 "ds_parser.cpp"
    break;

  case 126:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4934 "ds_parser.cpp"
    break;

  case 127:
#line 927 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4940 "ds_parser.cpp"
    break;

  case 128:
#line 928 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4946 "ds_parser.cpp"
    break;

  case 129:
#line 929 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4952 "ds_parser.cpp"
    break;

  case 130:
#line 930 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4958 "ds_parser.cpp"
    break;

  case 131:
#line 931 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4964 "ds_parser.cpp"
    break;

  case 132:
#line 932 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4970 "ds_parser.cpp"
    break;

  case 133:
#line 933 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4976 "ds_parser.cpp"
    break;

  case 134:
#line 934 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4982 "ds_parser.cpp"
    break;

  case 135:
#line 935 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4988 "ds_parser.cpp"
    break;

  case 136:
#line 936 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4994 "ds_parser.cpp"
    break;

  case 137:
#line 937 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5000 "ds_parser.cpp"
    break;

  case 138:
#line 938 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5006 "ds_parser.cpp"
    break;

  case 139:
#line 939 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5012 "ds_parser.cpp"
    break;

  case 140:
#line 940 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5018 "ds_parser.cpp"
    break;

  case 141:
#line 941 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5024 "ds_parser.cpp"
    break;

  case 142:
#line 945 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5034 "ds_parser.cpp"
    break;

  case 143:
#line 950 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5045 "ds_parser.cpp"
    break;

  case 144:
#line 956 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5053 "ds_parser.cpp"
    break;

  case 145:
#line 962 "ds_parser.ypp"
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
#line 5079 "ds_parser.cpp"
    break;

  case 146:
#line 983 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5087 "ds_parser.cpp"
    break;

  case 147:
#line 989 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5093 "ds_parser.cpp"
    break;

  case 148:
#line 990 "ds_parser.ypp"
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
#line 5109 "ds_parser.cpp"
    break;

  case 149:
#line 1001 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5115 "ds_parser.cpp"
    break;

  case 150:
#line 1005 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5123 "ds_parser.cpp"
    break;

  case 151:
#line 1011 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5131 "ds_parser.cpp"
    break;

  case 152:
#line 1014 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5139 "ds_parser.cpp"
    break;

  case 153:
#line 1017 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5148 "ds_parser.cpp"
    break;

  case 154:
#line 1021 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5156 "ds_parser.cpp"
    break;

  case 155:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5162 "ds_parser.cpp"
    break;

  case 156:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5168 "ds_parser.cpp"
    break;

  case 157:
#line 1035 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5176 "ds_parser.cpp"
    break;

  case 158:
#line 1038 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5184 "ds_parser.cpp"
    break;

  case 159:
#line 1041 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5194 "ds_parser.cpp"
    break;

  case 160:
#line 1046 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5202 "ds_parser.cpp"
    break;

  case 161:
#line 1049 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5212 "ds_parser.cpp"
    break;

  case 162:
#line 1057 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5220 "ds_parser.cpp"
    break;

  case 163:
#line 1060 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5230 "ds_parser.cpp"
    break;

  case 164:
#line 1065 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5238 "ds_parser.cpp"
    break;

  case 165:
#line 1068 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5248 "ds_parser.cpp"
    break;

  case 166:
#line 1076 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5256 "ds_parser.cpp"
    break;

  case 167:
#line 1082 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5262 "ds_parser.cpp"
    break;

  case 168:
#line 1083 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5268 "ds_parser.cpp"
    break;

  case 169:
#line 1087 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5274 "ds_parser.cpp"
    break;

  case 170:
#line 1088 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5280 "ds_parser.cpp"
    break;

  case 171:
#line 1092 "ds_parser.ypp"
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
#line 5316 "ds_parser.cpp"
    break;

  case 172:
#line 1126 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5322 "ds_parser.cpp"
    break;

  case 173:
#line 1126 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5328 "ds_parser.cpp"
    break;

  case 174:
#line 1126 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5336 "ds_parser.cpp"
    break;

  case 175:
#line 1129 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5342 "ds_parser.cpp"
    break;

  case 176:
#line 1129 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5348 "ds_parser.cpp"
    break;

  case 177:
#line 1129 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5358 "ds_parser.cpp"
    break;

  case 178:
#line 1134 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5364 "ds_parser.cpp"
    break;

  case 179:
#line 1134 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5370 "ds_parser.cpp"
    break;

  case 180:
#line 1134 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5380 "ds_parser.cpp"
    break;

  case 181:
#line 1142 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5389 "ds_parser.cpp"
    break;

  case 182:
#line 1146 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5399 "ds_parser.cpp"
    break;

  case 183:
#line 1151 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5410 "ds_parser.cpp"
    break;

  case 184:
#line 1157 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5416 "ds_parser.cpp"
    break;

  case 185:
#line 1157 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5422 "ds_parser.cpp"
    break;

  case 186:
#line 1157 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5431 "ds_parser.cpp"
    break;

  case 187:
#line 1161 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5437 "ds_parser.cpp"
    break;

  case 188:
#line 1161 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5443 "ds_parser.cpp"
    break;

  case 189:
#line 1161 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5453 "ds_parser.cpp"
    break;

  case 190:
#line 1166 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5459 "ds_parser.cpp"
    break;

  case 191:
#line 1166 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5465 "ds_parser.cpp"
    break;

  case 192:
#line 1166 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5476 "ds_parser.cpp"
    break;

  case 193:
#line 1175 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5484 "ds_parser.cpp"
    break;

  case 194:
#line 1178 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5492 "ds_parser.cpp"
    break;

  case 195:
#line 1184 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5498 "ds_parser.cpp"
    break;

  case 196:
#line 1185 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5510 "ds_parser.cpp"
    break;

  case 197:
#line 1192 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5523 "ds_parser.cpp"
    break;

  case 198:
#line 1203 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5529 "ds_parser.cpp"
    break;

  case 199:
#line 1204 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5535 "ds_parser.cpp"
    break;

  case 200:
#line 1208 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5545 "ds_parser.cpp"
    break;

  case 201:
#line 1214 "ds_parser.ypp"
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
#line 5601 "ds_parser.cpp"
    break;

  case 202:
#line 1268 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5607 "ds_parser.cpp"
    break;

  case 203:
#line 1269 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5613 "ds_parser.cpp"
    break;

  case 204:
#line 1270 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5619 "ds_parser.cpp"
    break;

  case 205:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5625 "ds_parser.cpp"
    break;

  case 206:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5631 "ds_parser.cpp"
    break;

  case 207:
#line 1273 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5637 "ds_parser.cpp"
    break;

  case 208:
#line 1277 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5643 "ds_parser.cpp"
    break;

  case 209:
#line 1278 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5649 "ds_parser.cpp"
    break;

  case 210:
#line 1279 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5655 "ds_parser.cpp"
    break;

  case 211:
#line 1280 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5661 "ds_parser.cpp"
    break;

  case 212:
#line 1281 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5667 "ds_parser.cpp"
    break;

  case 213:
#line 1282 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5673 "ds_parser.cpp"
    break;

  case 214:
#line 1283 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5679 "ds_parser.cpp"
    break;

  case 215:
#line 1284 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5685 "ds_parser.cpp"
    break;

  case 216:
#line 1285 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5691 "ds_parser.cpp"
    break;

  case 217:
#line 1286 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5697 "ds_parser.cpp"
    break;

  case 218:
#line 1287 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5703 "ds_parser.cpp"
    break;

  case 219:
#line 1288 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5709 "ds_parser.cpp"
    break;

  case 220:
#line 1289 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5715 "ds_parser.cpp"
    break;

  case 221:
#line 1290 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5721 "ds_parser.cpp"
    break;

  case 222:
#line 1291 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5727 "ds_parser.cpp"
    break;

  case 223:
#line 1292 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5733 "ds_parser.cpp"
    break;

  case 224:
#line 1296 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5739 "ds_parser.cpp"
    break;

  case 225:
#line 1297 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5745 "ds_parser.cpp"
    break;

  case 226:
#line 1301 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5757 "ds_parser.cpp"
    break;

  case 227:
#line 1312 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5767 "ds_parser.cpp"
    break;

  case 228:
#line 1317 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5779 "ds_parser.cpp"
    break;

  case 229:
#line 1327 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5788 "ds_parser.cpp"
    break;

  case 230:
#line 1331 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5794 "ds_parser.cpp"
    break;

  case 231:
#line 1331 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5800 "ds_parser.cpp"
    break;

  case 232:
#line 1331 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 5812 "ds_parser.cpp"
    break;

  case 233:
#line 1338 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5818 "ds_parser.cpp"
    break;

  case 234:
#line 1338 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5824 "ds_parser.cpp"
    break;

  case 235:
#line 1338 "ds_parser.ypp"
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
#line 5840 "ds_parser.cpp"
    break;

  case 236:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5846 "ds_parser.cpp"
    break;

  case 237:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5852 "ds_parser.cpp"
    break;

  case 238:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5858 "ds_parser.cpp"
    break;

  case 239:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5864 "ds_parser.cpp"
    break;

  case 240:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5870 "ds_parser.cpp"
    break;

  case 241:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5876 "ds_parser.cpp"
    break;

  case 242:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5882 "ds_parser.cpp"
    break;

  case 243:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5888 "ds_parser.cpp"
    break;

  case 244:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5894 "ds_parser.cpp"
    break;

  case 245:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5900 "ds_parser.cpp"
    break;

  case 246:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5906 "ds_parser.cpp"
    break;

  case 247:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5912 "ds_parser.cpp"
    break;

  case 248:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5918 "ds_parser.cpp"
    break;

  case 249:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5924 "ds_parser.cpp"
    break;

  case 250:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5930 "ds_parser.cpp"
    break;

  case 251:
#line 1367 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5936 "ds_parser.cpp"
    break;

  case 252:
#line 1368 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5942 "ds_parser.cpp"
    break;

  case 253:
#line 1369 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5948 "ds_parser.cpp"
    break;

  case 254:
#line 1370 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5954 "ds_parser.cpp"
    break;

  case 255:
#line 1371 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5960 "ds_parser.cpp"
    break;

  case 256:
#line 1372 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5966 "ds_parser.cpp"
    break;

  case 257:
#line 1373 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5972 "ds_parser.cpp"
    break;

  case 258:
#line 1374 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5978 "ds_parser.cpp"
    break;

  case 259:
#line 1375 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5984 "ds_parser.cpp"
    break;

  case 260:
#line 1376 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5990 "ds_parser.cpp"
    break;

  case 261:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5996 "ds_parser.cpp"
    break;

  case 262:
#line 1378 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6002 "ds_parser.cpp"
    break;

  case 263:
#line 1379 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6008 "ds_parser.cpp"
    break;

  case 264:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6014 "ds_parser.cpp"
    break;

  case 265:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6020 "ds_parser.cpp"
    break;

  case 266:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6026 "ds_parser.cpp"
    break;

  case 267:
#line 1383 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6032 "ds_parser.cpp"
    break;

  case 268:
#line 1384 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6038 "ds_parser.cpp"
    break;

  case 269:
#line 1385 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6044 "ds_parser.cpp"
    break;

  case 270:
#line 1386 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6050 "ds_parser.cpp"
    break;

  case 271:
#line 1387 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6056 "ds_parser.cpp"
    break;

  case 272:
#line 1388 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6062 "ds_parser.cpp"
    break;

  case 273:
#line 1389 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6068 "ds_parser.cpp"
    break;

  case 274:
#line 1390 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6074 "ds_parser.cpp"
    break;

  case 275:
#line 1391 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6083 "ds_parser.cpp"
    break;

  case 276:
#line 1395 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6092 "ds_parser.cpp"
    break;

  case 277:
#line 1399 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6100 "ds_parser.cpp"
    break;

  case 278:
#line 1402 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6108 "ds_parser.cpp"
    break;

  case 279:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6114 "ds_parser.cpp"
    break;

  case 280:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6120 "ds_parser.cpp"
    break;

  case 281:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6126 "ds_parser.cpp"
    break;

  case 282:
#line 1408 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6136 "ds_parser.cpp"
    break;

  case 283:
#line 1413 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6146 "ds_parser.cpp"
    break;

  case 284:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6152 "ds_parser.cpp"
    break;

  case 285:
#line 1419 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6160 "ds_parser.cpp"
    break;

  case 286:
#line 1422 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6166 "ds_parser.cpp"
    break;

  case 287:
#line 1422 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6172 "ds_parser.cpp"
    break;

  case 288:
#line 1422 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6180 "ds_parser.cpp"
    break;

  case 289:
#line 1425 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6189 "ds_parser.cpp"
    break;

  case 290:
#line 1429 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6198 "ds_parser.cpp"
    break;

  case 291:
#line 1433 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6207 "ds_parser.cpp"
    break;

  case 292:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6213 "ds_parser.cpp"
    break;

  case 293:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6219 "ds_parser.cpp"
    break;

  case 294:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6225 "ds_parser.cpp"
    break;

  case 295:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6231 "ds_parser.cpp"
    break;

  case 296:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6237 "ds_parser.cpp"
    break;

  case 297:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6243 "ds_parser.cpp"
    break;

  case 298:
#line 1443 "ds_parser.ypp"
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
#line 6258 "ds_parser.cpp"
    break;

  case 299:
#line 1453 "ds_parser.ypp"
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
#line 6273 "ds_parser.cpp"
    break;

  case 300:
#line 1463 "ds_parser.ypp"
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
#line 6330 "ds_parser.cpp"
    break;

  case 301:
#line 1518 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6336 "ds_parser.cpp"
    break;

  case 302:
#line 1519 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6342 "ds_parser.cpp"
    break;

  case 303:
#line 1523 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6348 "ds_parser.cpp"
    break;

  case 304:
#line 1524 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6354 "ds_parser.cpp"
    break;

  case 305:
#line 1528 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6364 "ds_parser.cpp"
    break;

  case 306:
#line 1536 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6372 "ds_parser.cpp"
    break;

  case 307:
#line 1539 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6381 "ds_parser.cpp"
    break;

  case 308:
#line 1544 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6387 "ds_parser.cpp"
    break;

  case 309:
#line 1544 "ds_parser.ypp"
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
#line 6436 "ds_parser.cpp"
    break;

  case 310:
#line 1588 "ds_parser.ypp"
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
#line 6494 "ds_parser.cpp"
    break;

  case 311:
#line 1644 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6504 "ds_parser.cpp"
    break;

  case 312:
#line 1649 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6514 "ds_parser.cpp"
    break;

  case 313:
#line 1657 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6520 "ds_parser.cpp"
    break;

  case 314:
#line 1658 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6526 "ds_parser.cpp"
    break;

  case 315:
#line 1662 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6534 "ds_parser.cpp"
    break;

  case 316:
#line 1665 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6545 "ds_parser.cpp"
    break;

  case 317:
#line 1674 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6551 "ds_parser.cpp"
    break;

  case 318:
#line 1675 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6557 "ds_parser.cpp"
    break;

  case 319:
#line 1679 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6568 "ds_parser.cpp"
    break;

  case 320:
#line 1688 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6574 "ds_parser.cpp"
    break;

  case 321:
#line 1689 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6580 "ds_parser.cpp"
    break;

  case 322:
#line 1694 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6586 "ds_parser.cpp"
    break;

  case 323:
#line 1695 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6592 "ds_parser.cpp"
    break;

  case 324:
#line 1699 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6603 "ds_parser.cpp"
    break;

  case 325:
#line 1705 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6614 "ds_parser.cpp"
    break;

  case 326:
#line 1711 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6622 "ds_parser.cpp"
    break;

  case 327:
#line 1714 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6631 "ds_parser.cpp"
    break;

  case 328:
#line 1718 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6642 "ds_parser.cpp"
    break;

  case 329:
#line 1724 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6653 "ds_parser.cpp"
    break;

  case 330:
#line 1733 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6659 "ds_parser.cpp"
    break;

  case 331:
#line 1734 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6665 "ds_parser.cpp"
    break;

  case 332:
#line 1735 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6671 "ds_parser.cpp"
    break;

  case 333:
#line 1739 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6677 "ds_parser.cpp"
    break;

  case 334:
#line 1740 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6683 "ds_parser.cpp"
    break;

  case 335:
#line 1744 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6691 "ds_parser.cpp"
    break;

  case 336:
#line 1747 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6701 "ds_parser.cpp"
    break;

  case 337:
#line 1752 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6711 "ds_parser.cpp"
    break;

  case 338:
#line 1757 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6724 "ds_parser.cpp"
    break;

  case 339:
#line 1765 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6737 "ds_parser.cpp"
    break;

  case 340:
#line 1776 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6745 "ds_parser.cpp"
    break;

  case 341:
#line 1779 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6754 "ds_parser.cpp"
    break;

  case 342:
#line 1786 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6760 "ds_parser.cpp"
    break;

  case 343:
#line 1787 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6766 "ds_parser.cpp"
    break;

  case 344:
#line 1791 "ds_parser.ypp"
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
#line 6798 "ds_parser.cpp"
    break;

  case 345:
#line 1818 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6807 "ds_parser.cpp"
    break;

  case 346:
#line 1821 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6818 "ds_parser.cpp"
    break;

  case 347:
#line 1830 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6826 "ds_parser.cpp"
    break;

  case 348:
#line 1833 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6840 "ds_parser.cpp"
    break;

  case 349:
#line 1842 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6854 "ds_parser.cpp"
    break;

  case 350:
#line 1855 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6868 "ds_parser.cpp"
    break;

  case 354:
#line 1876 "ds_parser.ypp"
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
#line 6884 "ds_parser.cpp"
    break;

  case 355:
#line 1887 "ds_parser.ypp"
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
#line 6901 "ds_parser.cpp"
    break;

  case 356:
#line 1902 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6907 "ds_parser.cpp"
    break;

  case 357:
#line 1903 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6913 "ds_parser.cpp"
    break;

  case 358:
#line 1907 "ds_parser.ypp"
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
#line 6952 "ds_parser.cpp"
    break;

  case 359:
#line 1944 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6958 "ds_parser.cpp"
    break;

  case 360:
#line 1945 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6964 "ds_parser.cpp"
    break;

  case 361:
#line 1949 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 6970 "ds_parser.cpp"
    break;

  case 362:
#line 1949 "ds_parser.ypp"
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
#line 7056 "ds_parser.cpp"
    break;

  case 363:
#line 2033 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7068 "ds_parser.cpp"
    break;

  case 364:
#line 2040 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7079 "ds_parser.cpp"
    break;

  case 365:
#line 2049 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7085 "ds_parser.cpp"
    break;

  case 366:
#line 2050 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7091 "ds_parser.cpp"
    break;

  case 367:
#line 2051 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7097 "ds_parser.cpp"
    break;

  case 368:
#line 2052 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7103 "ds_parser.cpp"
    break;

  case 369:
#line 2053 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7109 "ds_parser.cpp"
    break;

  case 370:
#line 2054 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7115 "ds_parser.cpp"
    break;

  case 371:
#line 2055 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7121 "ds_parser.cpp"
    break;

  case 372:
#line 2056 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7127 "ds_parser.cpp"
    break;

  case 373:
#line 2057 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7133 "ds_parser.cpp"
    break;

  case 374:
#line 2058 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7139 "ds_parser.cpp"
    break;

  case 375:
#line 2059 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7145 "ds_parser.cpp"
    break;

  case 376:
#line 2060 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7151 "ds_parser.cpp"
    break;

  case 377:
#line 2061 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7157 "ds_parser.cpp"
    break;

  case 378:
#line 2062 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7163 "ds_parser.cpp"
    break;

  case 379:
#line 2063 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7169 "ds_parser.cpp"
    break;

  case 380:
#line 2064 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7175 "ds_parser.cpp"
    break;

  case 381:
#line 2065 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7181 "ds_parser.cpp"
    break;

  case 382:
#line 2066 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7187 "ds_parser.cpp"
    break;

  case 383:
#line 2067 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7193 "ds_parser.cpp"
    break;

  case 384:
#line 2068 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7199 "ds_parser.cpp"
    break;

  case 385:
#line 2069 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7205 "ds_parser.cpp"
    break;

  case 386:
#line 2070 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7211 "ds_parser.cpp"
    break;

  case 387:
#line 2071 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7217 "ds_parser.cpp"
    break;

  case 388:
#line 2072 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7223 "ds_parser.cpp"
    break;

  case 389:
#line 2073 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7229 "ds_parser.cpp"
    break;

  case 390:
#line 2077 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7235 "ds_parser.cpp"
    break;

  case 391:
#line 2078 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7241 "ds_parser.cpp"
    break;

  case 392:
#line 2079 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7247 "ds_parser.cpp"
    break;

  case 393:
#line 2080 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7253 "ds_parser.cpp"
    break;

  case 394:
#line 2081 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7259 "ds_parser.cpp"
    break;

  case 395:
#line 2082 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7265 "ds_parser.cpp"
    break;

  case 396:
#line 2086 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7278 "ds_parser.cpp"
    break;

  case 397:
#line 2097 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7287 "ds_parser.cpp"
    break;

  case 398:
#line 2101 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7299 "ds_parser.cpp"
    break;

  case 399:
#line 2111 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7311 "ds_parser.cpp"
    break;

  case 400:
#line 2118 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7322 "ds_parser.cpp"
    break;

  case 401:
#line 2127 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7328 "ds_parser.cpp"
    break;

  case 402:
#line 2127 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7334 "ds_parser.cpp"
    break;

  case 403:
#line 2127 "ds_parser.ypp"
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
#line 7349 "ds_parser.cpp"
    break;

  case 404:
#line 2140 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7355 "ds_parser.cpp"
    break;

  case 405:
#line 2141 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7361 "ds_parser.cpp"
    break;

  case 406:
#line 2142 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7367 "ds_parser.cpp"
    break;

  case 407:
#line 2143 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7373 "ds_parser.cpp"
    break;

  case 408:
#line 2144 "ds_parser.ypp"
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
#line 7392 "ds_parser.cpp"
    break;

  case 409:
#line 2158 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7403 "ds_parser.cpp"
    break;

  case 410:
#line 2164 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7412 "ds_parser.cpp"
    break;

  case 411:
#line 2168 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7422 "ds_parser.cpp"
    break;

  case 412:
#line 2173 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7432 "ds_parser.cpp"
    break;

  case 413:
#line 2178 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7442 "ds_parser.cpp"
    break;

  case 414:
#line 2183 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7452 "ds_parser.cpp"
    break;

  case 415:
#line 2188 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7461 "ds_parser.cpp"
    break;

  case 416:
#line 2192 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7470 "ds_parser.cpp"
    break;

  case 417:
#line 2196 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7480 "ds_parser.cpp"
    break;

  case 418:
#line 2201 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7489 "ds_parser.cpp"
    break;

  case 419:
#line 2205 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7499 "ds_parser.cpp"
    break;

  case 420:
#line 2210 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7505 "ds_parser.cpp"
    break;

  case 421:
#line 2210 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7511 "ds_parser.cpp"
    break;

  case 422:
#line 2210 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7522 "ds_parser.cpp"
    break;

  case 423:
#line 2216 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7534 "ds_parser.cpp"
    break;

  case 424:
#line 2223 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7540 "ds_parser.cpp"
    break;

  case 425:
#line 2223 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7546 "ds_parser.cpp"
    break;

  case 426:
#line 2223 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7556 "ds_parser.cpp"
    break;

  case 427:
#line 2228 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7562 "ds_parser.cpp"
    break;

  case 428:
#line 2228 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7568 "ds_parser.cpp"
    break;

  case 429:
#line 2228 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7579 "ds_parser.cpp"
    break;

  case 430:
#line 2234 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7585 "ds_parser.cpp"
    break;

  case 431:
#line 2234 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7591 "ds_parser.cpp"
    break;

  case 432:
#line 2234 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7601 "ds_parser.cpp"
    break;

  case 433:
#line 2239 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7610 "ds_parser.cpp"
    break;

  case 434:
#line 2243 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7616 "ds_parser.cpp"
    break;

  case 435:
#line 2243 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7622 "ds_parser.cpp"
    break;

  case 436:
#line 2243 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7632 "ds_parser.cpp"
    break;

  case 437:
#line 2248 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7638 "ds_parser.cpp"
    break;

  case 438:
#line 2248 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7644 "ds_parser.cpp"
    break;

  case 439:
#line 2248 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7658 "ds_parser.cpp"
    break;

  case 440:
#line 2257 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7667 "ds_parser.cpp"
    break;

  case 441:
#line 2261 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7673 "ds_parser.cpp"
    break;

  case 442:
#line 2261 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7679 "ds_parser.cpp"
    break;

  case 443:
#line 2261 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7689 "ds_parser.cpp"
    break;

  case 444:
#line 2266 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7695 "ds_parser.cpp"
    break;

  case 445:
#line 2266 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7701 "ds_parser.cpp"
    break;

  case 446:
#line 2266 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7715 "ds_parser.cpp"
    break;

  case 447:
#line 2275 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7724 "ds_parser.cpp"
    break;

  case 448:
#line 2279 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7730 "ds_parser.cpp"
    break;

  case 449:
#line 2279 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7736 "ds_parser.cpp"
    break;

  case 450:
#line 2279 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7746 "ds_parser.cpp"
    break;

  case 451:
#line 2284 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7752 "ds_parser.cpp"
    break;

  case 452:
#line 2284 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7758 "ds_parser.cpp"
    break;

  case 453:
#line 2284 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7772 "ds_parser.cpp"
    break;

  case 454:
#line 2293 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7778 "ds_parser.cpp"
    break;

  case 455:
#line 2293 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7784 "ds_parser.cpp"
    break;

  case 456:
#line 2293 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7795 "ds_parser.cpp"
    break;

  case 457:
#line 2299 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7801 "ds_parser.cpp"
    break;

  case 458:
#line 2299 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7807 "ds_parser.cpp"
    break;

  case 459:
#line 2299 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7818 "ds_parser.cpp"
    break;

  case 460:
#line 2308 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7824 "ds_parser.cpp"
    break;

  case 461:
#line 2308 "ds_parser.ypp"
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
#line 7841 "ds_parser.cpp"
    break;

  case 462:
#line 2323 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7847 "ds_parser.cpp"
    break;

  case 463:
#line 2323 "ds_parser.ypp"
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
#line 7868 "ds_parser.cpp"
    break;

  case 464:
#line 2343 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7874 "ds_parser.cpp"
    break;

  case 465:
#line 2344 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7880 "ds_parser.cpp"
    break;

  case 466:
#line 2345 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7886 "ds_parser.cpp"
    break;

  case 467:
#line 2346 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7892 "ds_parser.cpp"
    break;

  case 468:
#line 2350 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7904 "ds_parser.cpp"
    break;

  case 469:
#line 2357 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7916 "ds_parser.cpp"
    break;

  case 470:
#line 2364 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7927 "ds_parser.cpp"
    break;

  case 471:
#line 2370 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7938 "ds_parser.cpp"
    break;

  case 472:
#line 2379 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7948 "ds_parser.cpp"
    break;

  case 473:
#line 2384 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7957 "ds_parser.cpp"
    break;

  case 474:
#line 2391 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7963 "ds_parser.cpp"
    break;

  case 475:
#line 2392 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7969 "ds_parser.cpp"
    break;

  case 476:
#line 2396 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 7980 "ds_parser.cpp"
    break;

  case 477:
#line 2402 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 7992 "ds_parser.cpp"
    break;

  case 478:
#line 2409 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8004 "ds_parser.cpp"
    break;

  case 479:
#line 2416 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8017 "ds_parser.cpp"
    break;

  case 480:
#line 2424 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8031 "ds_parser.cpp"
    break;

  case 481:
#line 2436 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8039 "ds_parser.cpp"
    break;

  case 482:
#line 2439 "ds_parser.ypp"
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
#line 8055 "ds_parser.cpp"
    break;

  case 483:
#line 2453 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8066 "ds_parser.cpp"
    break;

  case 484:
#line 2462 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8072 "ds_parser.cpp"
    break;

  case 485:
#line 2463 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8078 "ds_parser.cpp"
    break;

  case 486:
#line 2467 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8088 "ds_parser.cpp"
    break;

  case 487:
#line 2472 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8097 "ds_parser.cpp"
    break;

  case 488:
#line 2479 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8107 "ds_parser.cpp"
    break;

  case 489:
#line 2484 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8120 "ds_parser.cpp"
    break;

  case 490:
#line 2495 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8130 "ds_parser.cpp"
    break;

  case 491:
#line 2500 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8139 "ds_parser.cpp"
    break;

  case 492:
#line 2507 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8153 "ds_parser.cpp"
    break;

  case 493:
#line 2519 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8159 "ds_parser.cpp"
    break;

  case 494:
#line 2520 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8165 "ds_parser.cpp"
    break;

  case 495:
#line 2524 "ds_parser.ypp"
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
#line 8188 "ds_parser.cpp"
    break;

  case 496:
#line 2542 "ds_parser.ypp"
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
#line 8211 "ds_parser.cpp"
    break;


#line 8215 "ds_parser.cpp"

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
#line 2562 "ds_parser.ypp"


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
