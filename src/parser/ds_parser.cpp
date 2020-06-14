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
#define YYLAST   7112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  170
/* YYNRULES -- Number of rules.  */
#define YYNRULES  505
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  903

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
       0,   422,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   438,   450,   451,   455,   459,
     464,   473,   481,   497,   502,   510,   510,   540,   562,   566,
     569,   575,   584,   605,   628,   629,   633,   637,   638,   642,
     645,   651,   657,   660,   666,   667,   671,   672,   673,   682,
     683,   687,   696,   712,   720,   730,   739,   740,   741,   742,
     743,   744,   748,   753,   761,   762,   763,   764,   765,   766,
     767,   768,   774,   787,   803,   804,   808,   815,   827,   831,
     838,   839,   843,   844,   845,   849,   854,   861,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     897,   898,   899,   900,   904,   925,   963,   970,   974,   985,
     986,   987,   988,   989,   990,   991,   992,   993,   994,   995,
     996,   997,   998,   999,  1000,  1001,  1002,  1003,  1007,  1012,
    1018,  1024,  1045,  1051,  1052,  1063,  1067,  1073,  1076,  1079,
    1083,  1089,  1093,  1097,  1100,  1103,  1108,  1111,  1119,  1122,
    1127,  1130,  1138,  1144,  1145,  1149,  1150,  1154,  1189,  1189,
    1189,  1192,  1192,  1192,  1197,  1197,  1197,  1205,  1209,  1214,
    1220,  1220,  1220,  1224,  1224,  1224,  1229,  1229,  1229,  1238,
    1241,  1247,  1248,  1255,  1266,  1267,  1268,  1272,  1277,  1332,
    1333,  1334,  1335,  1336,  1337,  1341,  1342,  1343,  1344,  1345,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,
    1356,  1360,  1361,  1365,  1376,  1381,  1391,  1395,  1395,  1395,
    1402,  1402,  1402,  1416,  1417,  1418,  1419,  1420,  1421,  1422,
    1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,
    1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,
    1453,  1454,  1455,  1456,  1460,  1464,  1467,  1470,  1471,  1472,
    1473,  1478,  1483,  1484,  1487,  1487,  1487,  1490,  1494,  1498,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1518,  1528,  1583,
    1584,  1588,  1589,  1593,  1601,  1604,  1609,  1608,  1653,  1709,
    1714,  1722,  1723,  1727,  1730,  1739,  1740,  1744,  1753,  1754,
    1759,  1760,  1764,  1770,  1776,  1779,  1783,  1789,  1798,  1799,
    1800,  1804,  1805,  1809,  1812,  1817,  1822,  1830,  1841,  1844,
    1851,  1852,  1856,  1883,  1883,  1895,  1898,  1907,  1920,  1932,
    1933,  1937,  1941,  1952,  1967,  1968,  1972,  2009,  2010,  2014,
    2014,  2098,  2105,  2114,  2115,  2116,  2117,  2118,  2119,  2120,
    2121,  2122,  2123,  2124,  2125,  2126,  2127,  2128,  2129,  2130,
    2131,  2132,  2133,  2134,  2135,  2136,  2137,  2138,  2142,  2143,
    2144,  2145,  2146,  2147,  2151,  2162,  2166,  2176,  2183,  2192,
    2192,  2192,  2205,  2206,  2207,  2208,  2209,  2223,  2229,  2233,
    2238,  2243,  2248,  2253,  2257,  2261,  2266,  2270,  2275,  2275,
    2275,  2281,  2288,  2288,  2288,  2293,  2293,  2293,  2299,  2299,
    2299,  2304,  2308,  2308,  2308,  2313,  2313,  2313,  2322,  2326,
    2326,  2326,  2331,  2331,  2331,  2340,  2344,  2344,  2344,  2349,
    2349,  2349,  2358,  2358,  2358,  2364,  2364,  2364,  2373,  2373,
    2388,  2388,  2408,  2409,  2410,  2411,  2415,  2422,  2429,  2435,
    2444,  2449,  2456,  2457,  2461,  2467,  2474,  2482,  2489,  2497,
    2509,  2512,  2526,  2535,  2536,  2540,  2545,  2552,  2557,  2568,
    2573,  2580,  2592,  2593,  2597,  2615
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
  "$@16", "$@17", "expr", "$@18", "$@19", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@20",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "$@21", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "class_or_struct",
  "structure_declaration", "$@22", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@23", "$@24", "type_declaration", "$@25",
  "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34",
  "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43",
  "$@44", "$@45", "$@46", "$@47", "$@48", "variant_alias_declaration",
  "$@49", "bitfield_alias_declaration", "$@50", "make_decl",
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
     395,   396,   397,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   398,   399,   126,
      33,   400,   401,   402,   403,    46,   404,    91,    93,    40,
      41,    36,    64,   405,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -568

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-568)))

#define YYTABLE_NINF -460

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -568,    22,  -568,  -568,   -66,   -47,   -41,   -32,     7,  -568,
     -21,  -568,  -568,    12,  -568,  -568,  -568,  -568,  -568,   325,
    -568,    74,  -568,  -568,  -568,  -568,  -568,  -568,    43,  -568,
      54,    58,   107,  -568,  -568,     6,  -568,   -49,   121,  -568,
     124,   152,  -568,   126,   157,   133,  -568,    40,  -568,  -568,
    -568,    -3,   174,  -568,   139,    15,   -66,   185,   -47,   198,
    -568,   222,   223,  -568,   341,  -568,   218,  -568,   -94,   192,
     193,   234,  -568,   -66,    12,  -568,  6952,   340,   342,  -568,
     211,   207,  -568,   242,  -568,  -568,   390,  -568,  -568,  -568,
    -568,  -568,   265,   106,  -568,  -568,  -568,  -568,   350,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,   232,   -71,  5838,
    -568,  -568,   275,   278,  -568,   -72,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,   -86,   276,  -568,  -568,   -84,  -568,   248,
     -70,   247,  -568,    28,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,    75,  -568,  -568,   -67,  -568,   282,   284,   285,   290,
    -568,  -568,  -568,   257,  -568,  -568,  -568,  -568,  -568,   292,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   294,
    -568,  -568,  -568,   295,   297,  -568,  -568,  -568,  -568,   302,
     303,  -568,  -568,  -568,  -568,  -568,   175,  -568,   251,   309,
    -568,   287,  -568,   -66,  -568,   -27,  -568,   138,  5838,  -568,
    1260,  -568,  -568,  -568,  -568,  -568,   256,  -568,  -568,  -568,
     106,  -568,   -64,  4549,  -568,  -568,  -568,  -568,  -568,   322,
    -568,   210,   226,   243,  -568,  -568,  -568,  -568,  -568,   431,
    -568,  -568,    -8,  2443,  -568,  -568,   -62,  5838,   -60,    76,
     328,  -568,    68,  -568,   338,   173,  -568,  4549,  -568,  -568,
     328,  -568,  -568,  5762,   298,   455,  -568,  -568,   207,  4549,
     299,  4549,   316,   323,   305,  -568,   301,   326,   347,  2575,
     207,  -568,   329,  2031,  4549,  4549,    87,   958,  4680,  4549,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  4549,  4549,  4549,
     -84,  4549,  4549,  4549,  -568,   306,  -568,   444,  -568,  -568,
     310,   311,  4549,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,   -82,   321,  -568,   324,   330,  -568,  -568,  -568,   464,
    -568,  -568,  -568,   339,  -568,  -568,   -80,  -568,  -568,  -568,
    -568,  6234,   327,  -568,  -568,  -568,  -568,  -568,    19,   369,
    5838,  -568,    78,  -568,  -568,   842,  5838,  5838,  5838,  5838,
     343,   275,  5838,   211,  5838,   211,  5838,   211,  5914,   278,
    -568,  -568,  -568,   344,  -568,  -568,  5910,  -568,  -568,   173,
    -568,  -568,   351,  -568,  -568,  -568,  5838,  -568,  2707,  -568,
     953,    14,   264,  -568,   -84,  2707,  -568,  -568,   391,  4678,
     495,  6508,  4549,   953,  -568,  -568,  4549,  -568,  -568,   364,
     381,  6508,  -568,  5838,  2707,  -568,  4776,    23,    23,   363,
    -568,   328,  1587,   328,  1735,  6300,  -568,    93,   279,    23,
      23,   -57,  -568,    23,    23,  5234,   -51,   365,  -568,  -568,
     953,  -568,  2163,  -568,  -568,  -568,   476,   328,   211,    87,
    -568,    17,   403,  4549,  4549,  4549,  4549,  4549,  4549,  4549,
    4549,  4549,  4549,  -568,  -568,  4549,  4549,  4549,  4549,  4549,
    4549,   409,  2839,  4549,   410,  4549,  4549,  4549,  4549,  4549,
    4549,  4549,  4549,  2971,  3133,  4549,  4549,  4549,  4549,  4549,
    4549,  4549,  4549,  4549,  4549,   414,  4549,  3265,  -568,   546,
     512,   382,  -568,    79,  -568,  -568,  -568,  2707,  -568,   673,
    1055,  1681,  1829,  -568,    -1,  2257,   276,  2968,   276,  3079,
     276,   -83,  -568,   110,   173,   117,  -568,  -568,  -568,  3264,
    -568,  6128,  -568,  4549,  3413,  2311,  -568,  4874,  4549,  4549,
    -568,   207,  5305,  -568,  5838,  5838,  5374,  5838,  -568,  -568,
    3412,  -568,  4972,  -568,  -568,    20,    87,   -65,  1883,  3561,
    6300,   415,   -16,   389,   418,  -568,  -568,   -14,    27,  3709,
     -16,   165,  4549,  4549,   386,  -568,  4549,   425,   426,  -568,
     246,  -568,  -568,   315,   429,  -568,    77,  6508,  -568,  -568,
     276,  -568,   416,  -568,  -568,  6508,  6508,  6508,  6508,  6508,
    6508,  6508,  6508,   231,   231,   334,  6508,  6508,   334,  6711,
    6711,   400,  4549,  6508,    60,  -568,  5986,   -48,   -48,  6508,
     231,   231,  6508,  6508,  4549,  6508,   434,  6371,  6574,   647,
    6645,   334,   334,   528,   528,    60,    60,    60,  -568,  6053,
    -568,    96,    81,  -568,   328,  -568,   431,  -568,  2707,  -568,
    5070,  -568,  5838,  -568,  -568,  -568,   435,  -568,   421,  -568,
     424,  -568,   428,  5838,  -568,  5914,  -568,   278,   431,  4549,
     -96,  -568,   105,   423,   440,  5443,  -568,  -568,  -568,  3560,
    3708,  -568,  3856,   408,  -568,  4549,  -568,  4549,  4549,   -18,
     445,   429,   427,   430,  4549,   432,  4549,  4549,   429,   417,
     433,  6508,  -568,  -568,  6437,  -568,   456,  5838,   211,  1436,
     207,  -568,  -568,  4549,  -568,    85,    95,  4549,  -568,   -89,
    -568,  3857,  6771,  -568,  6771,  -568,  4549,  -568,  -568,  -568,
      -3,  -568,  -568,  5168,  -568,  -568,  4004,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,   173,  -568,  -568,  -568,
    6508,   207,  -568,  -568,   149,  -568,  -568,  -568,  -568,  4005,
     -43,  6508,  6508,   -16,   436,   120,   415,   437,  -568,  6508,
    -568,  -568,   -40,   -16,   438,  -568,  -568,  -568,  6790,   276,
    -568,  -568,   953,   442,  6508,  4153,  -568,  -568,  5838,  -568,
     112,  6508,    -3,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    5838,  4285,   460,  4549,  4549,  4549,  -568,  5512,  4549,   448,
     449,  4549,  4549,  -568,  4549,   443,  -568,  -568,   446,   315,
    -568,  4549,  6508,  6803,  -568,   439,  -568,  6814,   452,  5581,
     467,   334,   334,   334,  -568,  1061,   453,  -568,  6508,  6508,
    1061,   457,   -84,  -568,  -568,  6508,  -568,  -568,  -568,  -568,
    -568,  4417,   553,   463,  -568,   447,  -568,  -568,   -84,  -568,
     450,  5838,   484,  5650,  4549,   468,   469,  -568,  -568,  6857,
    -568,  -568,  6508,  -568,  -568,  -568,  5838,   465,  6868,  -568,
    -568,   471,  -568
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   175,     0,     0,     0,     0,     0,   176,
       0,   470,   468,     0,    14,     3,    10,     9,     8,     0,
       7,   350,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    75,   153,     0,    76,    78,     0,    74,   368,
     367,     0,     0,   351,   353,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   355,     0,   359,     0,     0,
       0,     0,   155,     0,     0,    81,     0,     0,     0,    87,
      82,     0,   124,   364,   369,   348,     0,    68,    69,    66,
      67,    65,     0,     0,    64,    73,    40,    38,    34,    31,
      30,   398,   401,   399,   402,   400,   403,     0,     0,     0,
     361,   360,     0,     0,   154,     0,    79,    90,    91,    93,
      92,    94,    95,    96,    97,   116,   117,   114,   115,   107,
     118,   119,   108,   105,   106,   120,   121,   122,   123,   110,
     111,   109,   103,   104,    99,    98,   100,   101,   102,    89,
      88,   112,   113,   309,    85,   148,   126,     0,   366,     0,
       0,     0,    16,     0,    60,    61,    58,    59,    57,    56,
      62,     0,    33,   355,     0,   362,     0,     0,     0,     0,
     373,   393,   374,   405,   375,   379,   380,   381,   382,   397,
     386,   387,   388,   389,   390,   391,   392,   394,   395,   441,
     378,   385,   396,   448,   455,   376,   383,   377,   384,     0,
       0,   404,   412,   415,   413,   414,     0,   407,     0,     0,
     328,     0,    77,     0,    83,     0,   321,     0,     0,   125,
       0,   365,   314,   371,   352,   349,   341,   354,    17,    18,
       0,    71,     0,     0,   356,   432,   435,   438,   428,     0,
     409,   442,   449,   456,   462,   465,   419,   424,   431,     0,
     427,   421,     0,     0,   358,   423,     0,     0,     0,     0,
       0,   319,   332,    84,   309,    86,   150,     0,    49,    50,
       0,   249,   250,     0,     0,     0,   243,   161,     0,     0,
       0,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
     209,   211,   210,   212,   213,   214,    19,     0,     0,     0,
       0,     0,     0,     0,   204,   205,   129,   127,   247,   246,
       0,     0,     0,   143,   138,   136,   135,   137,   207,   149,
     130,   244,     0,   302,     0,     0,   141,   142,   144,   173,
     134,   301,   300,    80,   305,   245,     0,   131,   304,   303,
     282,   215,     0,   248,   472,   473,   474,   475,   309,     0,
       0,   342,     0,    63,   363,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,    82,     0,    82,     0,     0,
     426,   420,   422,     0,   425,   417,     0,   408,   471,   327,
     469,   329,     0,   320,   331,   330,     0,   333,     0,   322,
       0,     0,   157,   160,     0,     0,   163,   166,     0,   215,
       0,   156,     0,     0,   178,   181,     0,   147,   184,     0,
       0,    43,    53,     0,     0,   170,   215,   273,   274,   205,
     152,     0,   482,     0,     0,     0,   499,     0,     0,   254,
     253,   287,    25,   252,   251,     0,   206,     0,   145,   146,
       0,   308,     0,   133,   139,   140,     0,     0,    82,     0,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   370,     0,
     311,     0,   372,     0,   339,   340,   338,     0,   357,     0,
       0,     0,     0,   406,     0,     0,    85,     0,    85,     0,
      85,   153,   325,     0,   323,     0,   418,   416,   310,   334,
     337,   336,    54,     0,     0,     0,   167,   215,     0,     0,
     164,     0,     0,    55,     0,     0,     0,     0,    41,    42,
       0,   171,   215,   168,   206,     0,     0,   153,     0,     0,
     490,   480,   482,     0,   493,   494,   495,     0,     0,     0,
     482,     0,     0,     0,     0,    22,     0,    20,     0,   277,
     237,   236,   148,    46,     0,   283,     0,   199,   174,   177,
      85,   151,     0,   297,   298,   222,   223,   225,   224,   226,
     219,   220,   221,   255,   256,   268,   227,   228,   269,   266,
     267,     0,     0,   217,   292,   281,     0,   306,   307,   218,
     257,   258,   229,   230,     0,   216,     0,     0,   271,   272,
     270,   264,   265,   260,   259,   261,   262,   263,   280,     0,
     285,     0,   311,   312,     0,   315,   338,   343,     0,   347,
     215,   433,     0,   439,   429,   410,     0,   443,     0,   450,
       0,   457,     0,     0,   463,     0,   466,     0,   330,     0,
       0,   158,     0,     0,     0,     0,   165,   172,   288,     0,
       0,   289,     0,     0,   169,     0,   483,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   492,   500,   501,     0,    23,    26,     0,    82,     0,
       0,    44,    45,     0,    51,     0,     0,     0,   284,     0,
     294,     0,   232,   279,   231,   299,     0,   278,   286,   316,
       0,   313,   345,   215,   346,   434,     0,   440,   430,   411,
     444,   446,   451,   453,   458,   460,   324,   464,   326,   467,
     335,     0,   159,   190,     0,   187,   179,   182,   185,     0,
       0,   477,   476,   482,     0,     0,   481,     0,   485,   491,
     497,   496,     0,   482,     0,   498,    21,    24,     0,    85,
     128,    47,     0,     0,   200,     0,   201,   208,     0,   234,
       0,   293,     0,   318,   344,   436,   447,   454,   461,    52,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
       0,     0,     0,   484,     0,     0,   488,   238,     0,    46,
     233,     0,   202,     0,   235,     0,   437,     0,     0,     0,
       0,   180,   183,   186,   291,   502,     0,   486,   479,   478,
     502,     0,     0,   241,    48,   203,   295,   317,   191,   193,
     188,     0,     0,     0,   487,     0,   489,   239,     0,   296,
       0,     0,     0,     0,     0,     0,     0,   242,   192,     0,
     196,   189,   503,   504,   505,   194,     0,     0,     0,   195,
     197,     0,   198
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -568,  -568,  -568,   190,   584,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   548,  -568,  -568,   589,  -568,  -568,
    -568,  -190,  -568,  -568,  -568,  -568,  -568,  -568,   413,  -568,
     594,   -55,  -568,   581,  -568,  -280,  -375,  -519,  -568,  -568,
    -156,   -93,   -79,  -568,    56,  -278,   -13,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,   660,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -504,  -568,  -568,  -290,  -568,  -227,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,   422,  -568,  -568,   296,     0,
    -568,  -568,  -568,   392,  -568,   -22,  -568,  -263,   280,  -271,
    -264,   148,  -568,   206,  -568,  -568,  -568,  -568,   501,   607,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -151,  -105,
    -568,  -568,  -568,   300,  -568,  -568,  -568,   -73,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,   393,  -564,  -433,  -567,
    -568,  -568,  -295,   -39,   235,  -568,  -568,  -568,  -182,  -568
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   448,   328,   726,   329,   598,
      16,    17,    35,    36,    63,    18,    32,    33,   330,   331,
     733,   734,   332,   333,   334,   335,   336,   337,   170,   171,
      29,    30,    45,    46,    47,    19,   154,   229,    80,    20,
      81,    82,   338,   339,   230,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   467,   349,   350,   351,   564,   823,
     565,   824,   567,   825,   352,   820,   880,   881,   897,   896,
     901,   606,   807,   353,   354,   355,   418,   357,   358,   359,
     360,   727,   862,   728,   878,   607,   808,   879,   225,   664,
     521,   368,   812,   226,   227,   542,   543,   220,   221,   708,
     271,   527,   372,   235,   160,    54,    22,    86,   108,    67,
      68,    23,    24,   158,    84,    52,    25,   159,   272,   362,
     107,   213,   214,   218,   215,   381,   759,   544,   379,   758,
     376,   755,   377,   846,   378,   757,   382,   760,   383,   816,
     384,   762,   385,   817,   386,   764,   387,   818,   388,   767,
     389,   769,    26,    41,    27,    40,   363,   581,   582,   583,
     364,   584,   585,   586,   587,   365,   447,   366,   873,   367
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   408,   156,   356,   212,   401,   403,   417,   536,   236,
     538,   590,   540,   661,   446,   712,   440,   678,   115,   680,
     270,   682,     2,   719,   391,   435,   -80,    59,     3,    87,
      88,    76,   576,   553,   576,   612,   216,   805,   469,   705,
     736,   223,    66,   471,   472,    42,   717,   737,   404,   690,
     692,     4,    43,     5,   461,     6,   707,     7,   494,   495,
      60,    48,     8,   683,   491,   174,   233,   494,   495,     9,
      28,    56,   174,   468,   397,    10,   219,   243,   771,   405,
     471,   472,   110,    31,   224,    43,   155,   462,   519,    44,
      71,   739,    11,   610,   470,    34,   211,    64,   222,   600,
     737,    77,    78,   737,    37,   175,   234,   244,    71,   233,
     516,   256,   374,    12,   398,    39,   400,   515,   735,   516,
     164,   165,    44,   212,   663,   749,    65,   483,   484,   411,
     550,   828,    53,    79,   834,   491,   257,   556,   494,   495,
     496,   392,   784,   497,   231,    89,   223,   786,    43,    90,
     675,    91,    92,   613,   715,   275,   571,   369,   711,   393,
     716,    61,   212,   369,   483,   484,   238,   239,   269,   394,
     369,    62,   491,   676,   493,   494,   495,   496,   212,   611,
     497,   404,    38,    74,    93,    44,    13,    55,   515,    13,
     516,   524,   524,   258,   399,   518,    14,    56,   404,   525,
     525,   780,   212,   212,    57,   256,   707,   256,    75,   420,
     412,   369,   405,   792,   406,   211,   829,   407,   240,    56,
     737,   432,   526,   666,   260,   515,   835,   516,   261,   405,
     257,   262,   257,   404,   442,   444,   166,   810,   710,   737,
     167,   831,   168,    92,   402,   241,   263,   738,   737,   669,
      58,   471,   472,   667,   211,   737,   265,    66,   324,   439,
      69,   684,   155,   803,   405,   212,   748,   593,   686,   594,
     211,   212,   212,   212,   212,   772,   783,   212,   689,   212,
     838,   212,   844,   212,   685,   793,   706,   258,    70,   258,
     575,   687,   588,    72,   211,   211,   256,   523,   722,    71,
     821,   212,    73,   529,   530,   531,   532,   452,   273,   535,
      83,   537,   274,   539,    85,    96,   236,   259,   260,   259,
     260,   257,   261,   822,   261,   262,   730,   262,   212,    49,
      50,   552,    51,   549,    98,   483,   484,   731,   732,   716,
     263,   720,   263,   491,   563,   493,   494,   495,   496,   264,
     265,   497,   265,   799,   471,   472,  -445,   211,    99,   100,
     570,  -445,   109,   211,   211,   211,   211,   112,   113,   211,
     114,   211,  -452,   211,   151,   211,   152,  -452,   258,  -445,
     153,   603,   155,   510,   511,   512,   513,   514,   157,  -459,
     752,   161,  -240,   211,  -459,  -452,   515,  -240,   516,   369,
     751,   555,   370,   162,    60,   371,   101,   173,   259,   260,
     102,   217,  -459,   261,   219,  -240,   262,   162,   595,   596,
     211,   237,   228,   232,   401,   266,   249,   103,   104,   105,
     106,   263,   245,   554,   246,   247,   481,   482,   483,   484,
     248,   265,   250,   601,   251,   252,   491,   253,   493,   494,
     495,   496,   254,   255,   497,   267,   499,   500,   380,   212,
     212,   268,   212,   390,   233,   223,   424,   414,   422,   281,
     282,   283,   284,   425,   426,   427,   428,   429,   456,   433,
     457,   286,   697,   466,   458,   459,   510,   511,   512,   513,
     514,   699,   700,   290,   702,   463,   517,   292,   464,   515,
     293,   516,   356,   294,   465,   522,    13,   297,   561,   469,
     568,   569,   546,   533,   832,   608,   180,   181,   182,   548,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   574,   200,   201,   202,   614,
     602,   205,   206,   207,   208,   631,   635,   302,   471,   472,
     658,   211,   211,   662,   211,   663,   665,   713,   710,   304,
     305,   714,   723,   238,   725,   735,   740,   212,   415,   741,
     745,   397,   761,   773,   306,   763,   774,   779,   212,   765,
     212,   785,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    43,   316,   794,   797,   787,   850,   863,   788,   756,
     790,   795,   869,   884,   830,   833,   836,   317,   318,   319,
     766,   320,   840,   867,   321,   322,   856,   857,   871,   861,
     888,   874,   212,   886,   323,   876,   324,   325,    44,   416,
     155,   885,   483,   484,   890,   899,   893,   894,   597,    94,
     491,   902,   493,   494,   495,   496,   172,    97,   497,   864,
      95,   801,   361,   373,   798,   116,   845,   813,   729,   211,
     806,    21,   750,   768,   520,   375,   409,   471,   472,   545,
     211,   668,   211,   609,   242,   111,   413,   791,   875,   591,
       0,   534,   512,   513,   514,   396,     0,     0,     0,     0,
       0,     0,   819,   515,     0,   516,     0,     0,     0,   410,
       0,     0,     0,   212,     0,   256,     0,   419,     0,     0,
       0,   421,     0,   423,   211,   212,     0,     0,     0,     0,
       0,   431,     0,   839,     0,   436,   437,   438,     0,     0,
     257,   445,     0,     0,     0,   843,     0,     0,     0,   449,
     450,   451,     0,   453,   454,   455,     0,   847,     0,   481,
     482,   483,   484,   485,   460,     0,   488,   489,   490,   491,
       0,   493,   494,   495,   496,     0,     0,   497,     0,   499,
     500,     0,     0,     0,     0,     0,   212,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,     0,     0,
       0,   212,     0,     0,     0,   211,   507,   508,   509,   510,
     511,   512,   513,   514,     0,     0,     0,   211,   889,     0,
       0,     0,   515,     0,   516,     0,     0,   259,   260,     0,
       0,     0,   261,   898,   671,   262,     0,     0,     0,     0,
     551,     0,     0,     0,     0,     0,     0,   557,     0,     0,
     263,     0,     0,     0,   562,     0,     0,     0,   566,   877,
     265,     0,     0,     0,     0,     0,   572,     0,     0,     0,
       0,     0,   471,   472,   580,   887,   580,     0,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   615,   616,   617,   618,   619,
     620,   621,   622,   623,   624,     0,     0,   625,   626,   627,
     628,   629,   630,     0,   633,   634,     0,   636,   637,   638,
     639,   640,   641,   642,   643,   645,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,   659,     0,
       0,     0,     0,     0,   481,   482,   483,   484,   485,   670,
       0,   488,   489,   490,   491,     0,   493,   494,   495,   496,
       0,     0,   497,     0,   499,   500,     0,     0,     0,     0,
     441,     0,     0,   471,   472,     0,     0,   695,     0,     0,
     633,   645,   176,     0,     0,     0,     0,   504,   177,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,     0,
     449,   455,     0,     0,   178,     0,     0,   515,     0,   516,
       0,   455,     0,     0,   721,   445,   528,   179,   724,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
       0,     0,     0,     0,   742,   481,   482,   483,   484,   485,
       0,     0,   488,   489,   490,   491,   744,   493,   494,   495,
     496,     0,     0,   497,     0,   499,   500,     0,     0,     0,
       0,   471,   472,     0,     0,     0,     0,   256,     0,     0,
     753,     0,     0,     0,    43,     0,     0,     0,   504,     0,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
       0,   770,   257,     0,     0,     0,     0,     0,   515,     0,
     516,     0,     0,     0,     0,     0,     0,     0,   155,   781,
     782,    44,     0,     0,     0,     0,   789,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,   802,     0,     0,     0,   804,
       0,     0,     0,   481,   482,   483,   484,   485,   811,   258,
     488,   489,   490,   491,     0,   493,   494,   495,   496,     0,
       0,   497,     0,   499,   500,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     260,   827,     0,     0,   261,     0,   504,   262,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,     0,     0,
       0,     0,   263,     0,     0,     0,   515,   842,   516,   672,
       0,     0,   265,     0,     0,   872,     0,     0,     0,     0,
       0,     0,     0,   849,     0,   851,   852,   853,     0,     0,
     855,     0,     0,   858,   859,     0,   860,     0,     0,     0,
       0,   276,     0,   865,     0,     0,     3,     0,   277,   278,
     279,     0,   280,     0,   281,   282,   283,   284,     0,     0,
       0,     0,     0,     0,     0,   285,   286,   287,   288,     0,
       0,     0,     0,   883,     0,     0,     0,   289,   290,     0,
       0,   291,   292,     0,     0,   293,   892,     9,   294,   295,
       0,   296,   297,     0,     0,   298,   299,     0,     0,     0,
     300,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     301,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   302,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   305,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
       0,     0,     0,     0,     0,     0,     0,   307,   308,   309,
     310,   311,   312,   313,   314,   315,    43,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   317,   318,   319,     0,   320,     0,     0,   321,
     322,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,   324,   325,    44,   326,   155,   327,   276,     0,     0,
       0,     0,     3,     0,   277,   278,   279,     0,   280,     0,
     281,   282,   283,   284,     0,     0,     0,     0,     0,     0,
       0,   285,   286,   287,   288,     0,     0,     0,     0,     0,
       0,     0,     0,   289,   290,     0,     0,   291,   292,     0,
       0,   293,     0,     9,   294,   295,     0,   296,   297,     0,
       0,   298,   299,     0,     0,     0,   300,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   301,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   302,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    43,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,   318,
     319,     0,   320,     0,     0,   321,   322,     0,     0,     0,
       0,   281,   282,   283,   284,   323,     0,   324,   325,    44,
     326,   155,   800,   286,     0,     0,     0,     0,     0,   256,
       0,     0,     0,     0,     0,   290,     0,     0,     0,   292,
       0,     0,   293,     0,     0,   294,     0,   576,     0,   297,
       0,     0,     0,     0,   257,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   301,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,     0,     0,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   577,   316,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,   261,     0,   257,   578,
     318,   319,     0,   320,     0,     0,   321,   322,     0,   281,
     282,   283,   284,     0,   263,     0,   579,     0,   324,   325,
      44,   286,   155,     0,   265,     0,     0,   256,     0,     0,
       0,     0,     0,   290,     0,     0,     0,   292,     0,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,     0,   257,     0,     0,   258,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,   259,   260,   302,     0,     0,
     261,     0,   673,   262,     0,     0,     0,     0,     0,   304,
     305,     0,     0,     0,     0,     0,     0,     0,   263,   258,
       0,     0,     0,     0,     0,     0,     0,     0,   265,     0,
       0,   256,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   577,   316,     0,     0,     0,     0,     0,     0,   259,
     260,     0,     0,     0,   261,     0,   257,   578,   318,   319,
       0,   320,     0,     0,   321,   322,     0,   281,   282,   283,
     284,     0,   263,     0,   589,     0,   324,   325,    44,   286,
     155,     0,   265,     0,     0,   391,     0,     0,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,   258,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   301,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,   259,   260,   302,     0,     0,   261,     0,
     674,   262,     0,     0,     0,     0,     0,   304,   305,     0,
       0,     0,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
     307,   308,   309,   310,   311,   312,   313,   314,   315,    43,
     316,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   392,     0,     0,   317,   318,   319,     0,   320,
       0,     0,   321,   322,     0,   281,   282,   283,   284,     0,
     393,     0,   323,     0,   324,   325,    44,   286,   155,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,   292,     0,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   301,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,   305,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,     0,     0,
     306,     0,     0,     0,     0,     0,     0,     0,   307,   308,
     309,   310,   311,   312,   313,   314,   315,    43,   316,     0,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,     0,     0,   317,   318,   319,     0,   320,     0,   286,
     321,   322,     0,     0,     0,     0,     0,     0,     0,     0,
     323,   290,   324,   325,    44,   292,   155,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   301,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
     307,   308,   309,   310,   311,   312,   313,   314,   315,    43,
     316,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   257,   317,   318,   319,     0,   320,
       0,     0,   321,   322,     0,   281,   282,   283,   284,   693,
     604,     0,   323,   605,   324,   325,    44,   286,   155,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,   292,     0,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,   258,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   301,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,   259,   260,   302,     0,     0,   261,     0,   677,   262,
       0,     0,     0,     0,     0,   304,   305,     0,     0,     0,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     0,     0,     0,   307,   308,
     309,   310,   311,   312,   313,   314,   315,    43,   316,     0,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   694,     0,   317,   318,   319,     0,   320,     0,   286,
     321,   322,     0,     0,     0,     0,     0,     0,     0,     0,
     323,   290,   324,   325,    44,   292,   155,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   301,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,   309,   310,   311,   312,   313,   314,   315,    43,
     316,     0,     0,     0,     0,     0,     0,     0,     0,   281,
     282,   283,   284,     0,     0,   317,   318,   319,     0,   320,
       0,   286,   321,   322,     0,     0,     0,     0,     0,     0,
       0,   395,   323,   290,   324,   325,    44,   292,   155,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
     430,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    43,   316,     0,     0,     0,     0,     0,     0,     0,
       0,   281,   282,   283,   284,     0,     0,   317,   318,   319,
       0,   320,     0,   286,   321,   322,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   290,   324,   325,    44,   292,
     155,     0,   293,     0,     0,   294,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   301,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   306,     0,     0,     0,
       0,     0,     0,     0,   307,   308,   309,   310,   311,   312,
     313,   314,   315,    43,   316,     0,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   317,
     318,   319,     0,   320,     0,   286,   321,   322,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   290,   324,   325,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   301,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   304,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   632,     0,
       0,     0,     0,     0,     0,     0,   307,   308,   309,   310,
     311,   312,   313,   314,   315,    43,   316,     0,     0,     0,
       0,     0,     0,     0,     0,   281,   282,   283,   284,     0,
       0,   317,   318,   319,     0,   320,     0,   286,   321,   322,
     256,     0,     0,     0,     0,     0,     0,     0,   323,   290,
     324,   325,    44,   292,   155,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,   257,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   301,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,   305,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
     644,     0,     0,     0,     0,     0,     0,     0,   307,   308,
     309,   310,   311,   312,   313,   314,   315,    43,   316,     0,
       0,   256,   259,   260,     0,     0,     0,   261,     0,   679,
     262,     0,     0,   317,   318,   319,     0,   320,     0,     0,
     321,   322,     0,     0,     0,   263,   257,     0,     0,     0,
     323,     0,   324,   325,    44,   265,   155,   281,   282,   283,
     284,     0,     0,     0,   646,     0,     0,     0,     0,   286,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,   258,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   301,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,   259,   260,   302,     0,     0,   261,     0,
     681,   262,     0,     0,     0,     0,     0,   304,   305,     0,
       0,     0,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
     307,   308,   309,   310,   311,   312,   313,   314,   315,    43,
     316,     0,     0,     0,     0,     0,     0,     0,     0,   281,
     282,   283,   284,     0,     0,   317,   318,   319,     0,   320,
       0,   286,   321,   322,     0,     0,   256,     0,     0,     0,
       0,     0,   323,   290,   324,   325,    44,   292,   155,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,   257,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     305,     0,     0,     0,     0,     0,     0,   404,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    43,   316,     0,     0,     0,     0,     0,   688,   260,
       0,     0,     0,   261,     0,     0,   262,   317,   318,   319,
       0,   320,     0,     0,   321,   322,     0,   281,   282,   283,
     284,   263,     0,     0,   323,   660,   324,   325,    44,   286,
     155,   265,     0,     0,   256,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,   257,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   301,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   305,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,   309,   310,   311,   312,   313,   314,   315,    43,
     316,     0,     0,     0,     0,     0,   259,   260,     0,     0,
       0,   261,     0,   703,   262,   317,   318,   319,     0,   320,
       0,     0,   321,   322,     0,   281,   282,   283,   284,   263,
       0,     0,   323,   691,   324,   325,    44,   286,   155,   265,
       0,     0,   256,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,   292,     0,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,     0,   257,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   301,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,   305,     0,     0,     0,
       0,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,   308,
     309,   310,   311,   312,   313,   314,   315,    43,   316,     0,
       0,     0,     0,     0,   259,   260,     0,     0,     0,   261,
       0,   776,   262,   317,   318,   319,     0,   320,     0,     0,
     321,   322,     0,   281,   282,   283,   284,   263,     0,     0,
     323,   709,   324,   325,    44,   286,   155,   265,     0,     0,
     256,     0,     0,     0,     0,     0,     0,   290,     0,     0,
       0,   292,     0,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,   257,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   301,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   304,   305,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   308,   309,   310,
     311,   312,   313,   314,   315,    43,   316,     0,     0,     0,
       0,     0,   259,   260,     0,     0,     0,   261,     0,   777,
     262,   317,   318,   319,     0,   320,     0,     0,   321,   322,
       0,   281,   282,   283,   284,   263,     0,     0,   323,   718,
     324,   325,    44,   286,   155,   265,     0,     0,   256,     0,
       0,     0,     0,     0,     0,   290,     0,     0,     0,   292,
       0,     0,   293,     0,     0,   294,     0,     0,     0,   297,
       0,     0,     0,   257,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   301,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,     0,     0,     0,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,   308,   309,   310,   311,   312,
     313,   314,   315,    43,   316,     0,     0,     0,     0,     0,
     259,   260,     0,     0,     0,   261,     0,   778,   262,   317,
     318,   319,     0,   320,     0,     0,   321,   322,     0,   281,
     282,   283,   284,   263,     0,     0,   323,   809,   324,   325,
      44,   286,   155,   265,     0,     0,   256,     0,     0,     0,
       0,     0,     0,   290,     0,     0,     0,   292,     0,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,   257,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     305,     0,     0,     0,     0,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    43,   316,     0,     0,     0,     0,     0,   259,   260,
       0,     0,     0,   261,     0,   815,   262,   317,   318,   319,
       0,   320,     0,     0,   321,   322,     0,   281,   282,   283,
     284,   263,     0,     0,   323,   826,   324,   325,    44,   286,
     155,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   301,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   305,     0,
       0,     0,     0,     0,     0,     0,   841,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,   309,   310,   311,   312,   313,   314,   315,    43,
     316,     0,     0,     0,     0,     0,     0,     0,     0,   281,
     282,   283,   284,   848,     0,   317,   318,   319,     0,   320,
       0,   286,   321,   322,     0,     0,     0,     0,     0,     0,
       0,     0,   323,   290,   324,   325,    44,   292,   155,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   301,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    43,   316,     0,     0,     0,     0,     0,     0,     0,
       0,   281,   282,   283,   284,   882,     0,   317,   318,   319,
       0,   320,     0,   286,   321,   322,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   290,   324,   325,    44,   292,
     155,     0,   293,     0,     0,   294,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   301,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,   308,   309,   310,   311,   312,
     313,   314,   315,    43,   316,     0,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,     0,     0,   317,
     318,   319,     0,   320,     0,   286,   321,   322,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   290,   324,   325,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   301,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   304,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   308,   309,   310,
     311,   312,   313,   314,   315,    43,   316,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,   471,   472,
       0,   317,   318,   319,   176,   320,     0,     0,   321,   322,
     177,     0,     0,     0,     0,     0,     0,     0,   323,     0,
     324,   325,    44,     0,   155,     0,   178,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   179,
       0,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   558,   493,   494,   495,   496,   471,   472,   497,   498,
     499,   500,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,   559,   504,     0,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,     0,     0,     0,     0,     0,
       0,     0,     0,   515,     0,   516,     0,     0,     0,     0,
       0,     0,   560,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   558,
     493,   494,   495,   496,   471,   472,   497,   498,   499,   500,
     501,   502,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,   504,     0,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
       0,   515,     0,   516,     0,     0,     0,     0,     0,     0,
     573,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   558,   493,   494,
     495,   496,   471,   472,   497,   498,   499,   500,   501,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,   504,
       0,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,     0,     0,     0,     0,     0,     0,     0,     0,   515,
       0,   516,     0,     0,     0,     0,     0,     0,   696,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   558,   493,   494,   495,   496,
     471,   472,   497,   498,   499,   500,   501,   502,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,   504,     0,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,     0,
       0,     0,     0,     0,     0,     0,     0,   515,     0,   516,
       0,     0,     0,     0,     0,     0,   704,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   558,   493,   494,   495,   496,   471,   472,
     497,   498,   499,   500,   501,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,   504,     0,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,     0,     0,     0,
       0,     0,     0,     0,     0,   515,     0,   516,     0,     0,
       0,     0,     0,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   471,   472,     0,     0,     0,     0,
       0,     0,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   558,   493,   494,   495,   496,     0,     0,   497,   498,
     499,   500,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,   504,     0,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   471,   472,     0,     0,     0,
       0,     0,     0,   515,     0,   516,   481,   482,   483,   484,
     485,     0,   814,   488,   489,   490,   491,     0,   493,   494,
     495,   496,     0,     0,   497,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   504,
       0,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,     0,     0,     0,   471,   472,     0,     0,     0,   515,
       0,   516,     0,     0,   599,     0,     0,   481,   482,   483,
     484,   485,     0,     0,   488,   489,   490,   491,     0,   493,
     494,   495,   496,     0,     0,   497,     0,   499,   500,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,     0,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,     0,   471,   472,     0,     0,     0,     0,     0,
     515,     0,   516,     0,     0,   698,   481,   482,   483,   484,
     485,     0,     0,   488,   489,   490,   491,     0,   493,   494,
     495,   496,     0,     0,   497,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   504,
       0,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,     0,   471,   472,     0,     0,     0,     0,     0,   515,
       0,   516,     0,     0,   701,   481,   482,   483,   484,   485,
       0,     0,   488,   489,   490,   491,     0,   493,   494,   495,
     496,     0,     0,   497,     0,   499,   500,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,     0,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
       0,   471,   472,     0,     0,     0,     0,     0,   515,     0,
     516,     0,     0,   775,   481,   482,   483,   484,   485,     0,
       0,   488,   489,   490,   491,     0,   493,   494,   495,   496,
       0,     0,   497,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,     0,
     471,   472,     0,     0,     0,     0,     0,   515,     0,   516,
       0,     0,   854,   481,   482,   483,   484,   485,     0,     0,
     488,   489,   490,   491,     0,   493,   494,   495,   496,     0,
       0,   497,     0,   499,   500,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,     0,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,     0,     0,
       0,     0,     0,     0,     0,     0,   515,     0,   516,     0,
       0,   870,   481,   482,   483,   484,   485,     0,     0,   488,
     489,   490,   491,     0,   493,   494,   495,   496,     0,     0,
     497,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,   177,     0,     0,   504,     0,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,     0,   178,     0,
       0,     0,     0,     0,     0,   515,     0,   516,     0,     0,
     891,   179,     0,   180,   181,   182,   183,   184,   185,   186,
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
     471,   472,     0,     0,     0,    44,     0,     0,   176,     0,
       0,     0,     0,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,    43,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   471,   472,     0,     0,
       0,    44,   481,   482,   483,   484,   485,     0,     0,   488,
     489,   490,   491,     0,   493,   494,   495,   496,     0,     0,
     497,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     541,     0,     0,     0,     0,   504,     0,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,     0,     0,     0,
       0,     0,     0,   471,   472,   515,     0,   516,   547,     0,
       0,     0,     0,     0,     0,     0,     0,    44,   481,   482,
     483,   484,   485,     0,     0,   488,   489,   490,   491,     0,
     493,   494,   495,   496,     0,     0,   497,     0,   499,   500,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   504,     0,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,     0,     0,     0,     0,     0,   471,   472,
       0,   515,     0,   516,   743,   481,   482,   483,   484,   485,
       0,     0,   488,   489,   490,   491,     0,   493,   494,   495,
     496,     0,     0,   497,     0,   499,   500,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,     0,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
       0,     0,     0,     0,     0,     0,     0,     0,   515,     0,
     516,   747,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   558,   493,   494,   495,   496,  -215,     0,   497,   498,
     499,   500,   501,   502,   471,   472,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,   504,     0,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,     0,     0,     0,     0,     0,
       0,     0,     0,   515,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     471,   472,     0,     0,     0,     0,     0,     0,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,     0,     0,   497,   498,   499,   500,   501,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   503,   504,
       0,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   471,   472,     0,     0,     0,     0,     0,     0,   515,
       0,   516,   481,   482,   483,   484,   485,     0,     0,   488,
     489,   490,   491,     0,   493,   494,   495,   496,     0,     0,
     497,     0,   499,   500,     0,     0,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   504,     0,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   471,   472,     0,
       0,     0,     0,     0,     0,   515,     0,   516,     0,     0,
       0,     0,     0,   481,   482,   483,   484,   485,     0,     0,
     488,   489,   490,   491,     0,   493,   494,   495,   496,     0,
       0,   497,     0,   499,   500,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   746,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   471,   472,
       0,     0,     0,     0,     0,     0,   515,     0,   516,   481,
     482,   483,   484,   485,     0,     0,   488,   489,   490,   491,
       0,   493,   494,   495,   496,     0,     0,   497,     0,   499,
     500,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   796,     0,
       0,     0,   504,     0,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   471,   472,     0,     0,     0,     0,
       0,     0,   515,     0,   516,     0,     0,     0,     0,     0,
     481,   482,   483,   484,   485,     0,     0,   488,   489,   490,
     491,     0,   493,   494,   495,   496,     0,     0,   497,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,     0,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   471,   472,     0,     0,     0,
       0,     0,     0,   515,     0,   516,   481,   482,   483,   484,
     485,     0,     0,   488,   489,   490,   491,     0,   493,   494,
     495,   496,     0,     0,   497,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   471,   472,     0,     0,     0,     0,     0,     0,   515,
       0,   516,     0,     0,     0,     0,     0,   481,   482,   483,
     484,   485,     0,     0,   488,   489,   490,   491,     0,   493,
     494,   495,   496,     0,     0,   497,     0,   499,   500,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,   472,     0,     0,   508,   509,   510,   511,   512,
     513,   514,     0,     0,     0,     0,     0,     0,     0,     0,
     515,     0,   516,   481,   482,   483,   484,   485,     0,     0,
     488,     0,   256,   491,     0,   493,   494,   495,   496,     0,
       0,   497,     0,   499,   500,   256,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,   508,   509,   510,   511,   512,   513,   514,     0,     0,
       0,   257,     0,   481,   482,     0,   515,   485,   516,     0,
     488,   489,   490,   491,     0,   493,   494,   495,   496,   256,
       0,   497,     0,   499,   500,     0,     0,     0,     0,     0,
     256,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   257,     0,   504,   258,   505,   506,
     507,   508,   509,     0,     0,   257,   513,     0,   258,     0,
       0,     0,     0,     0,   259,   260,   515,     0,   516,   261,
       0,   837,   262,     0,     0,     0,     0,   259,   260,     0,
       0,     0,   261,     0,   866,   262,     0,   263,   259,   260,
       0,     0,     0,   261,     0,   868,   262,   265,     0,     0,
     263,   258,     0,     0,     0,     0,     0,     0,     0,     0,
     265,   263,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,   261,     0,   895,   262,
       0,     0,   259,   260,     0,     0,     0,   261,     0,   900,
     262,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   265,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
       0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      13,   272,    81,   230,   109,   268,   270,   285,   383,   160,
     385,   444,   387,   517,   309,   582,   306,   536,    73,   538,
      47,   540,     0,   590,    32,   303,     7,    21,     6,    14,
      15,    34,    50,    19,    50,    18,   109,   126,   118,    19,
     604,   127,   136,    20,    21,    33,    19,   143,   113,   553,
     554,    29,   136,    31,   136,    33,   121,    35,   115,   116,
      54,    74,    40,   146,   112,   136,   136,   115,   116,    47,
     136,   143,   136,   353,   136,    53,   136,   144,   174,   144,
      20,    21,   176,   130,   170,   136,   175,   169,   368,   173,
     173,   610,    70,   468,   174,   136,   109,   146,   170,   150,
     143,   104,   105,   143,   136,   176,   176,   174,   173,   136,
     167,    32,   176,    91,   176,   136,   176,   165,   136,   167,
      14,    15,   173,   228,    43,    44,   175,   104,   105,   280,
     408,   174,    58,   136,   174,   112,    57,   415,   115,   116,
     117,   149,   709,   120,   157,   130,   127,   711,   136,   134,
     151,   136,   137,   136,   168,   228,   434,   143,   174,   167,
     174,   155,   267,   143,   104,   105,   138,   139,   223,   177,
     143,   165,   112,   174,   114,   115,   116,   117,   283,   469,
     120,   113,   175,   143,   169,   173,   167,   144,   165,   167,
     167,   113,   113,   114,   267,   176,   174,   143,   113,   121,
     121,   705,   307,   308,   146,    32,   121,    32,   168,   288,
     283,   143,   144,   717,   146,   228,   783,   149,   143,   143,
     143,   300,   144,   144,   145,   165,   793,   167,   149,   144,
      57,   152,    57,   113,   307,   308,   130,   741,   143,   143,
     134,   121,   136,   137,   168,   170,   167,   170,   143,   527,
     143,    20,    21,   174,   267,   143,   177,   136,   171,   172,
     136,   151,   175,   168,   144,   370,   170,   174,   151,   176,
     283,   376,   377,   378,   379,   170,   709,   382,   549,   384,
     799,   386,   170,   388,   174,   718,   576,   114,   136,   114,
     441,   174,   443,   136,   307,   308,    32,   370,   593,   173,
     151,   406,   169,   376,   377,   378,   379,   320,   170,   382,
     136,   384,   174,   386,   175,   130,   467,   144,   145,   144,
     145,    57,   149,   174,   149,   152,    11,   152,   433,     4,
       5,   410,     7,   406,   136,   104,   105,    22,    23,   174,
     167,   176,   167,   112,   423,   114,   115,   116,   117,   174,
     177,   120,   177,   728,    20,    21,   146,   370,   136,   136,
     433,   151,   144,   376,   377,   378,   379,   175,   175,   382,
     136,   384,   146,   386,    34,   388,    34,   151,   114,   169,
     169,   460,   175,   152,   153,   154,   155,   156,   146,   146,
     668,     1,   146,   406,   151,   169,   165,   151,   167,   143,
     664,   414,   146,   138,    54,   149,    65,   175,   144,   145,
      69,   136,   169,   149,   136,   169,   152,   138,   139,   140,
     433,   174,   146,   175,   687,   174,   169,    86,    87,    88,
      89,   167,   150,   169,   150,   150,   102,   103,   104,   105,
     150,   177,   150,   456,   150,   150,   112,   150,   114,   115,
     116,   117,   150,   150,   120,   146,   122,   123,   136,   564,
     565,   174,   567,    32,   136,   127,   150,   169,   169,    14,
      15,    16,    17,   150,   169,   174,   150,   130,   172,   150,
      36,    26,   561,    19,   174,   174,   152,   153,   154,   155,
     156,   564,   565,    38,   567,   174,   169,    42,   174,   165,
      45,   167,   729,    48,   174,   136,   167,    52,    13,   118,
     146,   130,   168,   170,   785,    39,    61,    62,    63,   168,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,   172,    81,    82,    83,   136,
     175,    86,    87,    88,    89,   136,   136,    92,    20,    21,
     136,   564,   565,     7,   567,    43,   174,   168,   143,   104,
     105,   143,   176,   138,   138,   136,   150,   672,   113,   169,
     136,   136,   151,   150,   119,   151,   136,   169,   683,   151,
     685,   136,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   176,   138,   168,   136,   151,   168,   672,
     168,   168,   150,    50,   168,   168,   168,   152,   153,   154,
     683,   156,   170,   174,   159,   160,   168,   168,   151,   176,
     170,   168,   727,   176,   169,   168,   171,   172,   173,   174,
     175,   168,   104,   105,   150,   170,   168,   168,   448,    55,
     112,   170,   114,   115,   116,   117,    98,    58,   120,   839,
      56,   730,   230,   240,   727,    74,   812,   750,   602,   672,
     739,     1,   662,   685,   368,   243,   274,    20,    21,   389,
     683,   523,   685,   467,   173,    68,   283,   716,   860,   444,
      -1,   381,   154,   155,   156,   263,    -1,    -1,    -1,    -1,
      -1,    -1,   771,   165,    -1,   167,    -1,    -1,    -1,   277,
      -1,    -1,    -1,   808,    -1,    32,    -1,   285,    -1,    -1,
      -1,   289,    -1,   291,   727,   820,    -1,    -1,    -1,    -1,
      -1,   299,    -1,   802,    -1,   303,   304,   305,    -1,    -1,
      57,   309,    -1,    -1,    -1,   808,    -1,    -1,    -1,   317,
     318,   319,    -1,   321,   322,   323,    -1,   820,    -1,   102,
     103,   104,   105,   106,   332,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
     123,    -1,    -1,    -1,    -1,    -1,   881,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   896,    -1,    -1,    -1,   808,   149,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,    -1,   820,   881,    -1,
      -1,    -1,   165,    -1,   167,    -1,    -1,   144,   145,    -1,
      -1,    -1,   149,   896,   151,   152,    -1,    -1,    -1,    -1,
     408,    -1,    -1,    -1,    -1,    -1,    -1,   415,    -1,    -1,
     167,    -1,    -1,    -1,   422,    -1,    -1,    -1,   426,   862,
     177,    -1,    -1,    -1,    -1,    -1,   434,    -1,    -1,    -1,
      -1,    -1,    20,    21,   442,   878,   444,    -1,   881,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   896,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,   486,   487,
     488,   489,   490,    -1,   492,   493,    -1,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,   527,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    20,    21,    -1,    -1,   555,    -1,    -1,
     558,   559,    24,    -1,    -1,    -1,    -1,   145,    30,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    -1,
     578,   579,    -1,    -1,    46,    -1,    -1,   165,    -1,   167,
      -1,   589,    -1,    -1,   592,   593,   174,    59,   596,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,   632,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,   644,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    32,    -1,    -1,
     668,    -1,    -1,    -1,   136,    -1,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      -1,   689,    57,    -1,    -1,    -1,    -1,    -1,   165,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   707,
     708,   173,    -1,    -1,    -1,    -1,   714,    -1,   716,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   729,    -1,    -1,    -1,   733,    -1,    -1,    -1,   737,
      -1,    -1,    -1,   102,   103,   104,   105,   106,   746,   114,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   779,    -1,    -1,   149,    -1,   145,   152,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,   165,   805,   167,   174,
      -1,    -1,   177,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   821,    -1,   823,   824,   825,    -1,    -1,
     828,    -1,    -1,   831,   832,    -1,   834,    -1,    -1,    -1,
      -1,     1,    -1,   841,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,   871,    -1,    -1,    -1,    37,    38,    -1,
      -1,    41,    42,    -1,    -1,    45,   884,    47,    48,    49,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,   153,   154,    -1,   156,    -1,    -1,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,   174,   175,   176,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,    -1,
      -1,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,   169,    -1,   171,   172,   173,
     174,   175,   176,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    50,    -1,    52,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,    -1,    -1,    -1,   149,    -1,    57,   152,
     153,   154,    -1,   156,    -1,    -1,   159,   160,    -1,    14,
      15,    16,    17,    -1,   167,    -1,   169,    -1,   171,   172,
     173,    26,   175,    -1,   177,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    -1,    -1,   114,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,   144,   145,    92,    -1,    -1,
     149,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    32,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,    -1,    -1,   149,    -1,    57,   152,   153,   154,
      -1,   156,    -1,    -1,   159,   160,    -1,    14,    15,    16,
      17,    -1,   167,    -1,   169,    -1,   171,   172,   173,    26,
     175,    -1,   177,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,   144,   145,    92,    -1,    -1,   149,    -1,
     151,   152,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,   154,    -1,   156,
      -1,    -1,   159,   160,    -1,    14,    15,    16,    17,    -1,
     167,    -1,   169,    -1,   171,   172,   173,    26,   175,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,   152,   153,   154,    -1,   156,    -1,    26,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    38,   171,   172,   173,    42,   175,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,   152,   153,   154,    -1,   156,
      -1,    -1,   159,   160,    -1,    14,    15,    16,    17,    18,
     167,    -1,   169,   170,   171,   172,   173,    26,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,   144,   145,    92,    -1,    -1,   149,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,   150,    -1,   152,   153,   154,    -1,   156,    -1,    26,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    38,   171,   172,   173,    42,   175,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,   152,   153,   154,    -1,   156,
      -1,    26,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,   169,    38,   171,   172,   173,    42,   175,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    -1,    -1,   152,   153,   154,
      -1,   156,    -1,    26,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    38,   171,   172,   173,    42,
     175,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   152,
     153,   154,    -1,   156,    -1,    26,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    38,   171,   172,
     173,    42,   175,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   152,   153,   154,    -1,   156,    -1,    26,   159,   160,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    38,
     171,   172,   173,    42,   175,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    32,   144,   145,    -1,    -1,    -1,   149,    -1,   151,
     152,    -1,    -1,   152,   153,   154,    -1,   156,    -1,    -1,
     159,   160,    -1,    -1,    -1,   167,    57,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,   177,   175,    14,    15,    16,
      17,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,   144,   145,    92,    -1,    -1,   149,    -1,
     151,   152,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,   152,   153,   154,    -1,   156,
      -1,    26,   159,   160,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   169,    38,   171,   172,   173,    42,   175,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,   144,   145,
      -1,    -1,    -1,   149,    -1,    -1,   152,   152,   153,   154,
      -1,   156,    -1,    -1,   159,   160,    -1,    14,    15,    16,
      17,   167,    -1,    -1,   169,   170,   171,   172,   173,    26,
     175,   177,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,
      -1,   149,    -1,   151,   152,   152,   153,   154,    -1,   156,
      -1,    -1,   159,   160,    -1,    14,    15,    16,    17,   167,
      -1,    -1,   169,   170,   171,   172,   173,    26,   175,   177,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,   149,
      -1,   151,   152,   152,   153,   154,    -1,   156,    -1,    -1,
     159,   160,    -1,    14,    15,    16,    17,   167,    -1,    -1,
     169,   170,   171,   172,   173,    26,   175,   177,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,    -1,    -1,   149,    -1,   151,
     152,   152,   153,   154,    -1,   156,    -1,    -1,   159,   160,
      -1,    14,    15,    16,    17,   167,    -1,    -1,   169,   170,
     171,   172,   173,    26,   175,   177,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
     144,   145,    -1,    -1,    -1,   149,    -1,   151,   152,   152,
     153,   154,    -1,   156,    -1,    -1,   159,   160,    -1,    14,
      15,    16,    17,   167,    -1,    -1,   169,   170,   171,   172,
     173,    26,   175,   177,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,   144,   145,
      -1,    -1,    -1,   149,    -1,   151,   152,   152,   153,   154,
      -1,   156,    -1,    -1,   159,   160,    -1,    14,    15,    16,
      17,   167,    -1,    -1,   169,   170,   171,   172,   173,    26,
     175,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   152,   153,   154,    -1,   156,
      -1,    26,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    38,   171,   172,   173,    42,   175,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   152,   153,   154,
      -1,   156,    -1,    26,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    38,   171,   172,   173,    42,
     175,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   152,
     153,   154,    -1,   156,    -1,    26,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    38,   171,   172,
     173,    42,   175,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   152,   153,   154,    24,   156,    -1,    -1,   159,   160,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
     171,   172,   173,    -1,   175,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   167,   102,   103,   104,   105,
     106,    -1,   174,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   165,
      -1,   167,    -1,    -1,   170,    -1,    -1,   102,   103,   104,
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
      20,    21,    -1,    -1,    -1,   173,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,   136,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    20,    21,    -1,    -1,
      -1,   173,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   165,    -1,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   102,   103,
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
      -1,    -1,    -1,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   167,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,   167,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,   167,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    32,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
      -1,    57,    -1,   102,   103,    -1,   165,   106,   167,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    32,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,   145,   114,   147,   148,
     149,   150,   151,    -1,    -1,    57,   155,    -1,   114,    -1,
      -1,    -1,    -1,    -1,   144,   145,   165,    -1,   167,   149,
      -1,   151,   152,    -1,    -1,    -1,    -1,   144,   145,    -1,
      -1,    -1,   149,    -1,   151,   152,    -1,   167,   144,   145,
      -1,    -1,    -1,   149,    -1,   151,   152,   177,    -1,    -1,
     167,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   167,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,    -1,    -1,    -1,   149,    -1,   151,   152,
      -1,    -1,   144,   145,    -1,    -1,    -1,   149,    -1,   151,
     152,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   179,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    70,    91,   167,   174,   180,   188,   189,   193,   213,
     217,   233,   284,   289,   290,   294,   330,   332,   136,   208,
     209,   130,   194,   195,   136,   190,   191,   136,   175,   136,
     333,   331,    33,   136,   173,   210,   211,   212,   224,     4,
       5,     7,   293,    58,   283,   144,   143,   146,   143,    21,
      54,   155,   165,   192,   146,   175,   136,   287,   288,   136,
     136,   173,   136,   169,   143,   168,    34,   104,   105,   136,
     216,   218,   219,   136,   292,   175,   285,    14,    15,   130,
     134,   136,   137,   169,   182,   208,   130,   195,   136,   136,
     136,    65,    69,    86,    87,    88,    89,   298,   286,   144,
     176,   287,   175,   175,   136,   209,   211,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   122,   123,   124,   125,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   159,
     160,    34,    34,   169,   214,   175,   220,   146,   291,   295,
     282,     1,   138,   181,    14,    15,   130,   134,   136,   182,
     206,   207,   192,   175,   136,   176,    24,    30,    46,    59,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   224,   297,   299,   300,   302,   305,   136,   301,   136,
     275,   276,   170,   127,   170,   266,   271,   272,   146,   215,
     222,   224,   175,   136,   176,   281,   296,   174,   138,   139,
     143,   170,   286,   144,   174,   150,   150,   150,   150,   169,
     150,   150,   150,   150,   150,   150,    32,    57,   114,   144,
     145,   149,   152,   167,   174,   177,   174,   146,   174,   209,
      47,   278,   296,   170,   174,   305,     1,     8,     9,    10,
      12,    14,    15,    16,    17,    25,    26,    27,    28,    37,
      38,    41,    42,    45,    48,    49,    51,    52,    55,    56,
      60,    70,    92,    93,   104,   105,   119,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   137,   152,   153,   154,
     156,   159,   160,   169,   171,   172,   174,   176,   184,   186,
     196,   197,   200,   201,   202,   203,   204,   205,   220,   221,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   233,
     234,   235,   242,   251,   252,   253,   254,   255,   256,   257,
     258,   263,   297,   334,   338,   343,   345,   347,   269,   143,
     146,   149,   280,   206,   176,   263,   308,   310,   312,   306,
     136,   303,   314,   316,   318,   320,   322,   324,   326,   328,
      32,    32,   149,   167,   177,   168,   263,   136,   176,   305,
     176,   275,   168,   278,   113,   144,   146,   149,   277,   271,
     263,   296,   305,   334,   169,   113,   174,   223,   254,   263,
     220,   263,   169,   263,   150,   150,   169,   174,   150,   130,
      55,   263,   220,   150,   113,   223,   263,   263,   263,   172,
     252,    12,   305,    12,   305,   263,   340,   344,   183,   263,
     263,   263,   224,   263,   263,   263,   172,    36,   174,   174,
     263,   136,   169,   174,   174,   174,    19,   232,   213,   118,
     174,    20,    21,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   120,   121,   122,
     123,   124,   125,   144,   145,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   165,   167,   169,   176,   213,
     266,   268,   136,   305,   113,   121,   144,   279,   174,   305,
     305,   305,   305,   170,   301,   305,   214,   305,   214,   305,
     214,   136,   273,   274,   305,   276,   168,   168,   168,   305,
     223,   263,   220,    19,   169,   224,   223,   263,   113,   144,
     174,    13,   263,   220,   236,   238,   263,   240,   146,   130,
     305,   223,   263,   174,   172,   296,    50,   136,   152,   169,
     263,   335,   336,   337,   339,   340,   341,   342,   296,   169,
     336,   342,   126,   174,   176,   139,   140,   181,   187,   170,
     150,   224,   175,   220,   167,   170,   249,   263,    39,   281,
     214,   252,    18,   136,   136,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   136,   119,   263,   263,   136,   263,   263,   263,   263,
     263,   263,   263,   263,   119,   263,    21,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   136,   263,
     170,   249,     7,    43,   267,   174,   144,   174,   279,   223,
     263,   151,   174,   151,   151,   151,   174,   151,   215,   151,
     215,   151,   215,   146,   151,   174,   151,   174,   144,   277,
     249,   170,   249,    18,   150,   263,   174,   220,   170,   305,
     305,   170,   305,   151,   174,    19,   252,   121,   277,   170,
     143,   174,   337,   168,   143,   168,   174,    19,   170,   337,
     176,   263,   340,   176,   263,   138,   185,   259,   261,   222,
      11,    22,    23,   198,   199,   136,   335,   143,   170,   215,
     150,   169,   263,   168,   263,   136,   146,   168,   170,    44,
     267,   278,   223,   263,   174,   309,   305,   313,   307,   304,
     315,   151,   319,   151,   323,   151,   305,   327,   273,   329,
     263,   174,   170,   150,   136,   170,   151,   151,   151,   169,
     249,   263,   263,   336,   337,   136,   335,   168,   168,   263,
     168,   341,   249,   336,   176,   168,   141,   138,   305,   214,
     176,   220,   263,   168,   263,   126,   220,   250,   264,   170,
     249,   263,   270,   219,   174,   151,   317,   321,   325,   220,
     243,   151,   174,   237,   239,   241,   170,   263,   174,   337,
     168,   121,   277,   168,   174,   337,   168,   151,   215,   220,
     170,   113,   263,   305,   170,   218,   311,   305,    18,   263,
     136,   263,   263,   263,   170,   263,   168,   168,   263,   263,
     263,   176,   260,   151,   199,   263,   151,   174,   151,   150,
     170,   151,   174,   346,   168,   346,   168,   224,   262,   265,
     244,   245,    18,   263,    50,   168,   176,   224,   170,   305,
     150,   170,   263,   168,   168,   151,   247,   246,   305,   170,
     151,   248,   170
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
     261,   262,   258,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   264,   265,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   266,
     266,   267,   267,   268,   269,   269,   270,   269,   269,   271,
     271,   272,   272,   273,   273,   274,   274,   275,   276,   276,
     277,   277,   278,   278,   278,   278,   278,   278,   279,   279,
     279,   280,   280,   281,   281,   281,   281,   281,   282,   282,
     283,   283,   284,   285,   284,   286,   286,   286,   287,   288,
     288,   289,   290,   290,   291,   291,   292,   293,   293,   295,
     294,   296,   296,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   298,   298,
     298,   298,   298,   298,   299,   300,   300,   301,   301,   303,
     304,   302,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   306,   307,
     305,   305,   308,   309,   305,   310,   311,   305,   312,   313,
     305,   305,   314,   315,   305,   316,   317,   305,   305,   318,
     319,   305,   320,   321,   305,   305,   322,   323,   305,   324,
     325,   305,   326,   327,   305,   328,   329,   305,   331,   330,
     333,   332,   334,   334,   334,   334,   335,   335,   335,   335,
     336,   336,   337,   337,   338,   338,   338,   338,   338,   338,
     339,   339,   340,   341,   341,   342,   342,   343,   343,   344,
     344,   345,   346,   346,   347,   347
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
       0,     0,     9,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     4,
       3,     3,     1,     3,     4,     3,     4,     2,     4,     4,
       6,     7,     3,     5,     0,     0,     8,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       4,     0,     1,     3,     0,     3,     0,     7,     5,     2,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     4,     6,     5,     5,     4,     0,     2,
       0,     1,     5,     0,     5,     0,     3,     5,     4,     1,
       2,     4,     5,     7,     0,     2,     2,     1,     1,     0,
       7,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     0,
       0,     6,     1,     1,     1,     1,     4,     3,     4,     2,
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
    case 136: /* "name"  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3291 "ds_parser.cpp"
        break;

    case 181: /* character_sequence  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3297 "ds_parser.cpp"
        break;

    case 182: /* string_constant  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3303 "ds_parser.cpp"
        break;

    case 183: /* string_builder_body  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3309 "ds_parser.cpp"
        break;

    case 184: /* string_builder  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3315 "ds_parser.cpp"
        break;

    case 186: /* expr_reader  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3321 "ds_parser.cpp"
        break;

    case 190: /* require_module_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3327 "ds_parser.cpp"
        break;

    case 196: /* expression_label  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3333 "ds_parser.cpp"
        break;

    case 197: /* expression_goto  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3339 "ds_parser.cpp"
        break;

    case 199: /* expression_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3345 "ds_parser.cpp"
        break;

    case 201: /* expression_if_then_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3351 "ds_parser.cpp"
        break;

    case 202: /* expression_for_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3357 "ds_parser.cpp"
        break;

    case 203: /* expression_unsafe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3363 "ds_parser.cpp"
        break;

    case 204: /* expression_while_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3369 "ds_parser.cpp"
        break;

    case 205: /* expression_with  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3375 "ds_parser.cpp"
        break;

    case 206: /* annotation_argument_value  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3381 "ds_parser.cpp"
        break;

    case 207: /* annotation_argument_value_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3387 "ds_parser.cpp"
        break;

    case 208: /* annotation_argument  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3393 "ds_parser.cpp"
        break;

    case 209: /* annotation_argument_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3399 "ds_parser.cpp"
        break;

    case 210: /* annotation_declaration_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3405 "ds_parser.cpp"
        break;

    case 211: /* annotation_declaration  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3411 "ds_parser.cpp"
        break;

    case 212: /* annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3417 "ds_parser.cpp"
        break;

    case 213: /* optional_annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3423 "ds_parser.cpp"
        break;

    case 214: /* optional_function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3429 "ds_parser.cpp"
        break;

    case 215: /* optional_function_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3435 "ds_parser.cpp"
        break;

    case 216: /* function_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3441 "ds_parser.cpp"
        break;

    case 220: /* expression_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3447 "ds_parser.cpp"
        break;

    case 221: /* expression_any  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3453 "ds_parser.cpp"
        break;

    case 222: /* expressions  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3459 "ds_parser.cpp"
        break;

    case 223: /* expr_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3465 "ds_parser.cpp"
        break;

    case 224: /* name_in_namespace  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3471 "ds_parser.cpp"
        break;

    case 225: /* expression_delete  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3477 "ds_parser.cpp"
        break;

    case 226: /* expr_new  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3483 "ds_parser.cpp"
        break;

    case 227: /* expression_break  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3489 "ds_parser.cpp"
        break;

    case 228: /* expression_continue  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3495 "ds_parser.cpp"
        break;

    case 229: /* expression_return  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 230: /* expression_yield  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3507 "ds_parser.cpp"
        break;

    case 231: /* expression_try_catch  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3513 "ds_parser.cpp"
        break;

    case 234: /* expression_let  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3519 "ds_parser.cpp"
        break;

    case 235: /* expr_cast  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3525 "ds_parser.cpp"
        break;

    case 242: /* expr_type_info  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3531 "ds_parser.cpp"
        break;

    case 249: /* expr_list  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3537 "ds_parser.cpp"
        break;

    case 250: /* block_or_simple_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3543 "ds_parser.cpp"
        break;

    case 252: /* expr_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3549 "ds_parser.cpp"
        break;

    case 253: /* expr_numeric_const  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3555 "ds_parser.cpp"
        break;

    case 254: /* expr_assign  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3561 "ds_parser.cpp"
        break;

    case 255: /* expr_assign_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3567 "ds_parser.cpp"
        break;

    case 256: /* expr_named_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3573 "ds_parser.cpp"
        break;

    case 257: /* expr_method_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3579 "ds_parser.cpp"
        break;

    case 258: /* func_addr_expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3585 "ds_parser.cpp"
        break;

    case 263: /* expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3591 "ds_parser.cpp"
        break;

    case 266: /* optional_field_annotation  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3597 "ds_parser.cpp"
        break;

    case 268: /* structure_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3603 "ds_parser.cpp"
        break;

    case 269: /* struct_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3609 "ds_parser.cpp"
        break;

    case 271: /* function_argument_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3615 "ds_parser.cpp"
        break;

    case 272: /* function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3621 "ds_parser.cpp"
        break;

    case 273: /* tuple_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3627 "ds_parser.cpp"
        break;

    case 274: /* tuple_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3633 "ds_parser.cpp"
        break;

    case 275: /* variant_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3639 "ds_parser.cpp"
        break;

    case 276: /* variant_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3645 "ds_parser.cpp"
        break;

    case 278: /* variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3651 "ds_parser.cpp"
        break;

    case 281: /* let_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3657 "ds_parser.cpp"
        break;

    case 282: /* global_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3663 "ds_parser.cpp"
        break;

    case 286: /* enum_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3669 "ds_parser.cpp"
        break;

    case 291: /* optional_structure_parent  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3675 "ds_parser.cpp"
        break;

    case 296: /* variable_name_with_pos_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3681 "ds_parser.cpp"
        break;

    case 299: /* structure_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3687 "ds_parser.cpp"
        break;

    case 300: /* auto_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3693 "ds_parser.cpp"
        break;

    case 301: /* bitfield_bits  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3699 "ds_parser.cpp"
        break;

    case 302: /* bitfield_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3705 "ds_parser.cpp"
        break;

    case 305: /* type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3711 "ds_parser.cpp"
        break;

    case 334: /* make_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3717 "ds_parser.cpp"
        break;

    case 335: /* make_struct_fields  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3723 "ds_parser.cpp"
        break;

    case 336: /* make_struct_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3729 "ds_parser.cpp"
        break;

    case 337: /* optional_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3735 "ds_parser.cpp"
        break;

    case 338: /* make_struct_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3741 "ds_parser.cpp"
        break;

    case 339: /* make_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3747 "ds_parser.cpp"
        break;

    case 340: /* make_map_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3753 "ds_parser.cpp"
        break;

    case 341: /* make_any_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3759 "ds_parser.cpp"
        break;

    case 342: /* make_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3765 "ds_parser.cpp"
        break;

    case 343: /* make_dim_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3771 "ds_parser.cpp"
        break;

    case 344: /* make_table  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3777 "ds_parser.cpp"
        break;

    case 345: /* make_table_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3783 "ds_parser.cpp"
        break;

    case 346: /* array_comprehension_where  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3789 "ds_parser.cpp"
        break;

    case 347: /* array_comprehension  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3795 "ds_parser.cpp"
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
#line 438 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4095 "ds_parser.cpp"
    break;

  case 16:
#line 450 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4101 "ds_parser.cpp"
    break;

  case 17:
#line 451 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4107 "ds_parser.cpp"
    break;

  case 18:
#line 455 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4113 "ds_parser.cpp"
    break;

  case 19:
#line 459 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4123 "ds_parser.cpp"
    break;

  case 20:
#line 464 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4137 "ds_parser.cpp"
    break;

  case 21:
#line 473 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4147 "ds_parser.cpp"
    break;

  case 22:
#line 481 "ds_parser.ypp"
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
#line 4165 "ds_parser.cpp"
    break;

  case 23:
#line 497 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4175 "ds_parser.cpp"
    break;

  case 24:
#line 502 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4185 "ds_parser.cpp"
    break;

  case 25:
#line 510 "ds_parser.ypp"
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
#line 4212 "ds_parser.cpp"
    break;

  case 26:
#line 531 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4224 "ds_parser.cpp"
    break;

  case 27:
#line 540 "ds_parser.ypp"
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
#line 4248 "ds_parser.cpp"
    break;

  case 29:
#line 566 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4256 "ds_parser.cpp"
    break;

  case 30:
#line 569 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4267 "ds_parser.cpp"
    break;

  case 31:
#line 575 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4278 "ds_parser.cpp"
    break;

  case 32:
#line 584 "ds_parser.ypp"
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
#line 4304 "ds_parser.cpp"
    break;

  case 33:
#line 605 "ds_parser.ypp"
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
#line 4329 "ds_parser.cpp"
    break;

  case 34:
#line 628 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4335 "ds_parser.cpp"
    break;

  case 35:
#line 629 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4341 "ds_parser.cpp"
    break;

  case 39:
#line 642 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4349 "ds_parser.cpp"
    break;

  case 40:
#line 645 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4357 "ds_parser.cpp"
    break;

  case 41:
#line 651 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4365 "ds_parser.cpp"
    break;

  case 42:
#line 657 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4373 "ds_parser.cpp"
    break;

  case 43:
#line 660 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4381 "ds_parser.cpp"
    break;

  case 44:
#line 666 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4387 "ds_parser.cpp"
    break;

  case 45:
#line 667 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4393 "ds_parser.cpp"
    break;

  case 46:
#line 671 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4399 "ds_parser.cpp"
    break;

  case 47:
#line 672 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4405 "ds_parser.cpp"
    break;

  case 48:
#line 673 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4416 "ds_parser.cpp"
    break;

  case 49:
#line 682 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4422 "ds_parser.cpp"
    break;

  case 50:
#line 683 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4428 "ds_parser.cpp"
    break;

  case 51:
#line 687 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4439 "ds_parser.cpp"
    break;

  case 52:
#line 696 "ds_parser.ypp"
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
#line 4457 "ds_parser.cpp"
    break;

  case 53:
#line 712 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4467 "ds_parser.cpp"
    break;

  case 54:
#line 720 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4479 "ds_parser.cpp"
    break;

  case 55:
#line 730 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4490 "ds_parser.cpp"
    break;

  case 56:
#line 739 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4496 "ds_parser.cpp"
    break;

  case 57:
#line 740 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4502 "ds_parser.cpp"
    break;

  case 58:
#line 741 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4508 "ds_parser.cpp"
    break;

  case 59:
#line 742 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4514 "ds_parser.cpp"
    break;

  case 60:
#line 743 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4520 "ds_parser.cpp"
    break;

  case 61:
#line 744 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4526 "ds_parser.cpp"
    break;

  case 62:
#line 748 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4536 "ds_parser.cpp"
    break;

  case 63:
#line 753 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4546 "ds_parser.cpp"
    break;

  case 64:
#line 761 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4552 "ds_parser.cpp"
    break;

  case 65:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4558 "ds_parser.cpp"
    break;

  case 66:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4564 "ds_parser.cpp"
    break;

  case 67:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4570 "ds_parser.cpp"
    break;

  case 68:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4576 "ds_parser.cpp"
    break;

  case 69:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4582 "ds_parser.cpp"
    break;

  case 70:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4588 "ds_parser.cpp"
    break;

  case 71:
#line 768 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4596 "ds_parser.cpp"
    break;

  case 72:
#line 774 "ds_parser.ypp"
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
#line 4614 "ds_parser.cpp"
    break;

  case 73:
#line 787 "ds_parser.ypp"
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
#line 4632 "ds_parser.cpp"
    break;

  case 74:
#line 803 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4638 "ds_parser.cpp"
    break;

  case 75:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4644 "ds_parser.cpp"
    break;

  case 76:
#line 808 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4656 "ds_parser.cpp"
    break;

  case 77:
#line 815 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4670 "ds_parser.cpp"
    break;

  case 78:
#line 827 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4679 "ds_parser.cpp"
    break;

  case 79:
#line 831 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4688 "ds_parser.cpp"
    break;

  case 80:
#line 838 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4694 "ds_parser.cpp"
    break;

  case 81:
#line 839 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4700 "ds_parser.cpp"
    break;

  case 82:
#line 843 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4706 "ds_parser.cpp"
    break;

  case 83:
#line 844 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4712 "ds_parser.cpp"
    break;

  case 84:
#line 845 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4718 "ds_parser.cpp"
    break;

  case 85:
#line 849 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4728 "ds_parser.cpp"
    break;

  case 86:
#line 854 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4737 "ds_parser.cpp"
    break;

  case 87:
#line 861 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4746 "ds_parser.cpp"
    break;

  case 88:
#line 865 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4752 "ds_parser.cpp"
    break;

  case 89:
#line 866 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4758 "ds_parser.cpp"
    break;

  case 90:
#line 867 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4764 "ds_parser.cpp"
    break;

  case 91:
#line 868 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4770 "ds_parser.cpp"
    break;

  case 92:
#line 869 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4776 "ds_parser.cpp"
    break;

  case 93:
#line 870 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4782 "ds_parser.cpp"
    break;

  case 94:
#line 871 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4788 "ds_parser.cpp"
    break;

  case 95:
#line 872 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4794 "ds_parser.cpp"
    break;

  case 96:
#line 873 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4800 "ds_parser.cpp"
    break;

  case 97:
#line 874 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4806 "ds_parser.cpp"
    break;

  case 98:
#line 875 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4812 "ds_parser.cpp"
    break;

  case 99:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4818 "ds_parser.cpp"
    break;

  case 100:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4824 "ds_parser.cpp"
    break;

  case 101:
#line 878 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4830 "ds_parser.cpp"
    break;

  case 102:
#line 879 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4836 "ds_parser.cpp"
    break;

  case 103:
#line 880 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4842 "ds_parser.cpp"
    break;

  case 104:
#line 881 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4848 "ds_parser.cpp"
    break;

  case 105:
#line 882 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4854 "ds_parser.cpp"
    break;

  case 106:
#line 883 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4860 "ds_parser.cpp"
    break;

  case 107:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4866 "ds_parser.cpp"
    break;

  case 108:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4872 "ds_parser.cpp"
    break;

  case 109:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4878 "ds_parser.cpp"
    break;

  case 110:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4884 "ds_parser.cpp"
    break;

  case 111:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4890 "ds_parser.cpp"
    break;

  case 112:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4896 "ds_parser.cpp"
    break;

  case 113:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4902 "ds_parser.cpp"
    break;

  case 114:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4908 "ds_parser.cpp"
    break;

  case 115:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4914 "ds_parser.cpp"
    break;

  case 116:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4920 "ds_parser.cpp"
    break;

  case 117:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4926 "ds_parser.cpp"
    break;

  case 118:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4932 "ds_parser.cpp"
    break;

  case 119:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4938 "ds_parser.cpp"
    break;

  case 120:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4944 "ds_parser.cpp"
    break;

  case 121:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4950 "ds_parser.cpp"
    break;

  case 122:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4956 "ds_parser.cpp"
    break;

  case 123:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4962 "ds_parser.cpp"
    break;

  case 124:
#line 904 "ds_parser.ypp"
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
#line 4985 "ds_parser.cpp"
    break;

  case 125:
#line 925 "ds_parser.ypp"
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
#line 5025 "ds_parser.cpp"
    break;

  case 126:
#line 963 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5034 "ds_parser.cpp"
    break;

  case 127:
#line 970 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5043 "ds_parser.cpp"
    break;

  case 128:
#line 974 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5056 "ds_parser.cpp"
    break;

  case 129:
#line 985 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5062 "ds_parser.cpp"
    break;

  case 130:
#line 986 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5068 "ds_parser.cpp"
    break;

  case 131:
#line 987 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5074 "ds_parser.cpp"
    break;

  case 132:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5080 "ds_parser.cpp"
    break;

  case 133:
#line 989 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5086 "ds_parser.cpp"
    break;

  case 134:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5092 "ds_parser.cpp"
    break;

  case 135:
#line 991 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5098 "ds_parser.cpp"
    break;

  case 136:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5104 "ds_parser.cpp"
    break;

  case 137:
#line 993 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5110 "ds_parser.cpp"
    break;

  case 138:
#line 994 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5116 "ds_parser.cpp"
    break;

  case 139:
#line 995 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5122 "ds_parser.cpp"
    break;

  case 140:
#line 996 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5128 "ds_parser.cpp"
    break;

  case 141:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5134 "ds_parser.cpp"
    break;

  case 142:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5140 "ds_parser.cpp"
    break;

  case 143:
#line 999 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5146 "ds_parser.cpp"
    break;

  case 144:
#line 1000 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5152 "ds_parser.cpp"
    break;

  case 145:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5158 "ds_parser.cpp"
    break;

  case 146:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5164 "ds_parser.cpp"
    break;

  case 147:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5170 "ds_parser.cpp"
    break;

  case 148:
#line 1007 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5180 "ds_parser.cpp"
    break;

  case 149:
#line 1012 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5191 "ds_parser.cpp"
    break;

  case 150:
#line 1018 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5199 "ds_parser.cpp"
    break;

  case 151:
#line 1024 "ds_parser.ypp"
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
#line 5225 "ds_parser.cpp"
    break;

  case 152:
#line 1045 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5233 "ds_parser.cpp"
    break;

  case 153:
#line 1051 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5239 "ds_parser.cpp"
    break;

  case 154:
#line 1052 "ds_parser.ypp"
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
#line 5255 "ds_parser.cpp"
    break;

  case 155:
#line 1063 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5261 "ds_parser.cpp"
    break;

  case 156:
#line 1067 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5269 "ds_parser.cpp"
    break;

  case 157:
#line 1073 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5277 "ds_parser.cpp"
    break;

  case 158:
#line 1076 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5285 "ds_parser.cpp"
    break;

  case 159:
#line 1079 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5294 "ds_parser.cpp"
    break;

  case 160:
#line 1083 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5302 "ds_parser.cpp"
    break;

  case 161:
#line 1089 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5308 "ds_parser.cpp"
    break;

  case 162:
#line 1093 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5314 "ds_parser.cpp"
    break;

  case 163:
#line 1097 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5322 "ds_parser.cpp"
    break;

  case 164:
#line 1100 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5330 "ds_parser.cpp"
    break;

  case 165:
#line 1103 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5340 "ds_parser.cpp"
    break;

  case 166:
#line 1108 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5348 "ds_parser.cpp"
    break;

  case 167:
#line 1111 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5358 "ds_parser.cpp"
    break;

  case 168:
#line 1119 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5366 "ds_parser.cpp"
    break;

  case 169:
#line 1122 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5376 "ds_parser.cpp"
    break;

  case 170:
#line 1127 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5384 "ds_parser.cpp"
    break;

  case 171:
#line 1130 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5394 "ds_parser.cpp"
    break;

  case 172:
#line 1138 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5402 "ds_parser.cpp"
    break;

  case 173:
#line 1144 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5408 "ds_parser.cpp"
    break;

  case 174:
#line 1145 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5414 "ds_parser.cpp"
    break;

  case 175:
#line 1149 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5420 "ds_parser.cpp"
    break;

  case 176:
#line 1150 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5426 "ds_parser.cpp"
    break;

  case 177:
#line 1154 "ds_parser.ypp"
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
#line 5463 "ds_parser.cpp"
    break;

  case 178:
#line 1189 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5469 "ds_parser.cpp"
    break;

  case 179:
#line 1189 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5475 "ds_parser.cpp"
    break;

  case 180:
#line 1189 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5483 "ds_parser.cpp"
    break;

  case 181:
#line 1192 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5489 "ds_parser.cpp"
    break;

  case 182:
#line 1192 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5495 "ds_parser.cpp"
    break;

  case 183:
#line 1192 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5505 "ds_parser.cpp"
    break;

  case 184:
#line 1197 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5511 "ds_parser.cpp"
    break;

  case 185:
#line 1197 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5517 "ds_parser.cpp"
    break;

  case 186:
#line 1197 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5527 "ds_parser.cpp"
    break;

  case 187:
#line 1205 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5536 "ds_parser.cpp"
    break;

  case 188:
#line 1209 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5546 "ds_parser.cpp"
    break;

  case 189:
#line 1214 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5557 "ds_parser.cpp"
    break;

  case 190:
#line 1220 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5563 "ds_parser.cpp"
    break;

  case 191:
#line 1220 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5569 "ds_parser.cpp"
    break;

  case 192:
#line 1220 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5578 "ds_parser.cpp"
    break;

  case 193:
#line 1224 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5584 "ds_parser.cpp"
    break;

  case 194:
#line 1224 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5590 "ds_parser.cpp"
    break;

  case 195:
#line 1224 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5600 "ds_parser.cpp"
    break;

  case 196:
#line 1229 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5606 "ds_parser.cpp"
    break;

  case 197:
#line 1229 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5612 "ds_parser.cpp"
    break;

  case 198:
#line 1229 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5623 "ds_parser.cpp"
    break;

  case 199:
#line 1238 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5631 "ds_parser.cpp"
    break;

  case 200:
#line 1241 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5639 "ds_parser.cpp"
    break;

  case 201:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5645 "ds_parser.cpp"
    break;

  case 202:
#line 1248 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5657 "ds_parser.cpp"
    break;

  case 203:
#line 1255 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5670 "ds_parser.cpp"
    break;

  case 204:
#line 1266 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5676 "ds_parser.cpp"
    break;

  case 205:
#line 1267 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5682 "ds_parser.cpp"
    break;

  case 206:
#line 1268 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5688 "ds_parser.cpp"
    break;

  case 207:
#line 1272 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5698 "ds_parser.cpp"
    break;

  case 208:
#line 1278 "ds_parser.ypp"
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
#line 5754 "ds_parser.cpp"
    break;

  case 209:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5760 "ds_parser.cpp"
    break;

  case 210:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5766 "ds_parser.cpp"
    break;

  case 211:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5772 "ds_parser.cpp"
    break;

  case 212:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5778 "ds_parser.cpp"
    break;

  case 213:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5784 "ds_parser.cpp"
    break;

  case 214:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5790 "ds_parser.cpp"
    break;

  case 215:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5796 "ds_parser.cpp"
    break;

  case 216:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5802 "ds_parser.cpp"
    break;

  case 217:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5808 "ds_parser.cpp"
    break;

  case 218:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5814 "ds_parser.cpp"
    break;

  case 219:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5820 "ds_parser.cpp"
    break;

  case 220:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5826 "ds_parser.cpp"
    break;

  case 221:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5832 "ds_parser.cpp"
    break;

  case 222:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5838 "ds_parser.cpp"
    break;

  case 223:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5844 "ds_parser.cpp"
    break;

  case 224:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5850 "ds_parser.cpp"
    break;

  case 225:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5856 "ds_parser.cpp"
    break;

  case 226:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5862 "ds_parser.cpp"
    break;

  case 227:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5868 "ds_parser.cpp"
    break;

  case 228:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5874 "ds_parser.cpp"
    break;

  case 229:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5880 "ds_parser.cpp"
    break;

  case 230:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5886 "ds_parser.cpp"
    break;

  case 231:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5892 "ds_parser.cpp"
    break;

  case 232:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5898 "ds_parser.cpp"
    break;

  case 233:
#line 1365 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5910 "ds_parser.cpp"
    break;

  case 234:
#line 1376 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5920 "ds_parser.cpp"
    break;

  case 235:
#line 1381 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5932 "ds_parser.cpp"
    break;

  case 236:
#line 1391 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5941 "ds_parser.cpp"
    break;

  case 237:
#line 1395 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5947 "ds_parser.cpp"
    break;

  case 238:
#line 1395 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5953 "ds_parser.cpp"
    break;

  case 239:
#line 1395 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 5965 "ds_parser.cpp"
    break;

  case 240:
#line 1402 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5971 "ds_parser.cpp"
    break;

  case 241:
#line 1402 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5977 "ds_parser.cpp"
    break;

  case 242:
#line 1402 "ds_parser.ypp"
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
#line 5993 "ds_parser.cpp"
    break;

  case 243:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5999 "ds_parser.cpp"
    break;

  case 244:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6005 "ds_parser.cpp"
    break;

  case 245:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6011 "ds_parser.cpp"
    break;

  case 246:
#line 1419 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6017 "ds_parser.cpp"
    break;

  case 247:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6023 "ds_parser.cpp"
    break;

  case 248:
#line 1421 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6029 "ds_parser.cpp"
    break;

  case 249:
#line 1422 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6035 "ds_parser.cpp"
    break;

  case 250:
#line 1423 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6041 "ds_parser.cpp"
    break;

  case 251:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6047 "ds_parser.cpp"
    break;

  case 252:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6053 "ds_parser.cpp"
    break;

  case 253:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6059 "ds_parser.cpp"
    break;

  case 254:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6065 "ds_parser.cpp"
    break;

  case 255:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6071 "ds_parser.cpp"
    break;

  case 256:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6077 "ds_parser.cpp"
    break;

  case 257:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6083 "ds_parser.cpp"
    break;

  case 258:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6089 "ds_parser.cpp"
    break;

  case 259:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6095 "ds_parser.cpp"
    break;

  case 260:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6101 "ds_parser.cpp"
    break;

  case 261:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6107 "ds_parser.cpp"
    break;

  case 262:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6113 "ds_parser.cpp"
    break;

  case 263:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6119 "ds_parser.cpp"
    break;

  case 264:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6125 "ds_parser.cpp"
    break;

  case 265:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6131 "ds_parser.cpp"
    break;

  case 266:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6137 "ds_parser.cpp"
    break;

  case 267:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6143 "ds_parser.cpp"
    break;

  case 268:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6149 "ds_parser.cpp"
    break;

  case 269:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6155 "ds_parser.cpp"
    break;

  case 270:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6161 "ds_parser.cpp"
    break;

  case 271:
#line 1444 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6167 "ds_parser.cpp"
    break;

  case 272:
#line 1445 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6173 "ds_parser.cpp"
    break;

  case 273:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6179 "ds_parser.cpp"
    break;

  case 274:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6185 "ds_parser.cpp"
    break;

  case 275:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6191 "ds_parser.cpp"
    break;

  case 276:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6197 "ds_parser.cpp"
    break;

  case 277:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6203 "ds_parser.cpp"
    break;

  case 278:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6209 "ds_parser.cpp"
    break;

  case 279:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6215 "ds_parser.cpp"
    break;

  case 280:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6221 "ds_parser.cpp"
    break;

  case 281:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6227 "ds_parser.cpp"
    break;

  case 282:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6233 "ds_parser.cpp"
    break;

  case 283:
#line 1456 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6242 "ds_parser.cpp"
    break;

  case 284:
#line 1460 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6251 "ds_parser.cpp"
    break;

  case 285:
#line 1464 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6259 "ds_parser.cpp"
    break;

  case 286:
#line 1467 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6267 "ds_parser.cpp"
    break;

  case 287:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6273 "ds_parser.cpp"
    break;

  case 288:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6279 "ds_parser.cpp"
    break;

  case 289:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6285 "ds_parser.cpp"
    break;

  case 290:
#line 1473 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6295 "ds_parser.cpp"
    break;

  case 291:
#line 1478 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6305 "ds_parser.cpp"
    break;

  case 292:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6311 "ds_parser.cpp"
    break;

  case 293:
#line 1484 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6319 "ds_parser.cpp"
    break;

  case 294:
#line 1487 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6325 "ds_parser.cpp"
    break;

  case 295:
#line 1487 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6331 "ds_parser.cpp"
    break;

  case 296:
#line 1487 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6339 "ds_parser.cpp"
    break;

  case 297:
#line 1490 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6348 "ds_parser.cpp"
    break;

  case 298:
#line 1494 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6357 "ds_parser.cpp"
    break;

  case 299:
#line 1498 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6366 "ds_parser.cpp"
    break;

  case 300:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6372 "ds_parser.cpp"
    break;

  case 301:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6378 "ds_parser.cpp"
    break;

  case 302:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6384 "ds_parser.cpp"
    break;

  case 303:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6390 "ds_parser.cpp"
    break;

  case 304:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6396 "ds_parser.cpp"
    break;

  case 305:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6402 "ds_parser.cpp"
    break;

  case 306:
#line 1508 "ds_parser.ypp"
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
#line 6417 "ds_parser.cpp"
    break;

  case 307:
#line 1518 "ds_parser.ypp"
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
#line 6432 "ds_parser.cpp"
    break;

  case 308:
#line 1528 "ds_parser.ypp"
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
#line 6489 "ds_parser.cpp"
    break;

  case 309:
#line 1583 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6495 "ds_parser.cpp"
    break;

  case 310:
#line 1584 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6501 "ds_parser.cpp"
    break;

  case 311:
#line 1588 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6507 "ds_parser.cpp"
    break;

  case 312:
#line 1589 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6513 "ds_parser.cpp"
    break;

  case 313:
#line 1593 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6523 "ds_parser.cpp"
    break;

  case 314:
#line 1601 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6531 "ds_parser.cpp"
    break;

  case 315:
#line 1604 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6540 "ds_parser.cpp"
    break;

  case 316:
#line 1609 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6546 "ds_parser.cpp"
    break;

  case 317:
#line 1609 "ds_parser.ypp"
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
#line 6595 "ds_parser.cpp"
    break;

  case 318:
#line 1653 "ds_parser.ypp"
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
#line 6653 "ds_parser.cpp"
    break;

  case 319:
#line 1709 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6663 "ds_parser.cpp"
    break;

  case 320:
#line 1714 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6673 "ds_parser.cpp"
    break;

  case 321:
#line 1722 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6679 "ds_parser.cpp"
    break;

  case 322:
#line 1723 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6685 "ds_parser.cpp"
    break;

  case 323:
#line 1727 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6693 "ds_parser.cpp"
    break;

  case 324:
#line 1730 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6704 "ds_parser.cpp"
    break;

  case 325:
#line 1739 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6710 "ds_parser.cpp"
    break;

  case 326:
#line 1740 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6716 "ds_parser.cpp"
    break;

  case 327:
#line 1744 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6727 "ds_parser.cpp"
    break;

  case 328:
#line 1753 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6733 "ds_parser.cpp"
    break;

  case 329:
#line 1754 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6739 "ds_parser.cpp"
    break;

  case 330:
#line 1759 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6745 "ds_parser.cpp"
    break;

  case 331:
#line 1760 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6751 "ds_parser.cpp"
    break;

  case 332:
#line 1764 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6762 "ds_parser.cpp"
    break;

  case 333:
#line 1770 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6773 "ds_parser.cpp"
    break;

  case 334:
#line 1776 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6781 "ds_parser.cpp"
    break;

  case 335:
#line 1779 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6790 "ds_parser.cpp"
    break;

  case 336:
#line 1783 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6801 "ds_parser.cpp"
    break;

  case 337:
#line 1789 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6812 "ds_parser.cpp"
    break;

  case 338:
#line 1798 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6818 "ds_parser.cpp"
    break;

  case 339:
#line 1799 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6824 "ds_parser.cpp"
    break;

  case 340:
#line 1800 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6830 "ds_parser.cpp"
    break;

  case 341:
#line 1804 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6836 "ds_parser.cpp"
    break;

  case 342:
#line 1805 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6842 "ds_parser.cpp"
    break;

  case 343:
#line 1809 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6850 "ds_parser.cpp"
    break;

  case 344:
#line 1812 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6860 "ds_parser.cpp"
    break;

  case 345:
#line 1817 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6870 "ds_parser.cpp"
    break;

  case 346:
#line 1822 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6883 "ds_parser.cpp"
    break;

  case 347:
#line 1830 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6896 "ds_parser.cpp"
    break;

  case 348:
#line 1841 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6904 "ds_parser.cpp"
    break;

  case 349:
#line 1844 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6913 "ds_parser.cpp"
    break;

  case 350:
#line 1851 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6919 "ds_parser.cpp"
    break;

  case 351:
#line 1852 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6925 "ds_parser.cpp"
    break;

  case 352:
#line 1856 "ds_parser.ypp"
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
#line 6957 "ds_parser.cpp"
    break;

  case 353:
#line 1883 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6966 "ds_parser.cpp"
    break;

  case 354:
#line 1886 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6977 "ds_parser.cpp"
    break;

  case 355:
#line 1895 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6985 "ds_parser.cpp"
    break;

  case 356:
#line 1898 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6999 "ds_parser.cpp"
    break;

  case 357:
#line 1907 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7013 "ds_parser.cpp"
    break;

  case 358:
#line 1920 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7027 "ds_parser.cpp"
    break;

  case 362:
#line 1941 "ds_parser.ypp"
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
#line 7043 "ds_parser.cpp"
    break;

  case 363:
#line 1952 "ds_parser.ypp"
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
#line 7060 "ds_parser.cpp"
    break;

  case 364:
#line 1967 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7066 "ds_parser.cpp"
    break;

  case 365:
#line 1968 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7072 "ds_parser.cpp"
    break;

  case 366:
#line 1972 "ds_parser.ypp"
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
#line 7111 "ds_parser.cpp"
    break;

  case 367:
#line 2009 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7117 "ds_parser.cpp"
    break;

  case 368:
#line 2010 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7123 "ds_parser.cpp"
    break;

  case 369:
#line 2014 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7129 "ds_parser.cpp"
    break;

  case 370:
#line 2014 "ds_parser.ypp"
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
#line 7215 "ds_parser.cpp"
    break;

  case 371:
#line 2098 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7227 "ds_parser.cpp"
    break;

  case 372:
#line 2105 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7238 "ds_parser.cpp"
    break;

  case 373:
#line 2114 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7244 "ds_parser.cpp"
    break;

  case 374:
#line 2115 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7250 "ds_parser.cpp"
    break;

  case 375:
#line 2116 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7256 "ds_parser.cpp"
    break;

  case 376:
#line 2117 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7262 "ds_parser.cpp"
    break;

  case 377:
#line 2118 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7268 "ds_parser.cpp"
    break;

  case 378:
#line 2119 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7274 "ds_parser.cpp"
    break;

  case 379:
#line 2120 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7280 "ds_parser.cpp"
    break;

  case 380:
#line 2121 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7286 "ds_parser.cpp"
    break;

  case 381:
#line 2122 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7292 "ds_parser.cpp"
    break;

  case 382:
#line 2123 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7298 "ds_parser.cpp"
    break;

  case 383:
#line 2124 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7304 "ds_parser.cpp"
    break;

  case 384:
#line 2125 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7310 "ds_parser.cpp"
    break;

  case 385:
#line 2126 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7316 "ds_parser.cpp"
    break;

  case 386:
#line 2127 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7322 "ds_parser.cpp"
    break;

  case 387:
#line 2128 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7328 "ds_parser.cpp"
    break;

  case 388:
#line 2129 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7334 "ds_parser.cpp"
    break;

  case 389:
#line 2130 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7340 "ds_parser.cpp"
    break;

  case 390:
#line 2131 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7346 "ds_parser.cpp"
    break;

  case 391:
#line 2132 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7352 "ds_parser.cpp"
    break;

  case 392:
#line 2133 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7358 "ds_parser.cpp"
    break;

  case 393:
#line 2134 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7364 "ds_parser.cpp"
    break;

  case 394:
#line 2135 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7370 "ds_parser.cpp"
    break;

  case 395:
#line 2136 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7376 "ds_parser.cpp"
    break;

  case 396:
#line 2137 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7382 "ds_parser.cpp"
    break;

  case 397:
#line 2138 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7388 "ds_parser.cpp"
    break;

  case 398:
#line 2142 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7394 "ds_parser.cpp"
    break;

  case 399:
#line 2143 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7400 "ds_parser.cpp"
    break;

  case 400:
#line 2144 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7406 "ds_parser.cpp"
    break;

  case 401:
#line 2145 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7412 "ds_parser.cpp"
    break;

  case 402:
#line 2146 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7418 "ds_parser.cpp"
    break;

  case 403:
#line 2147 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7424 "ds_parser.cpp"
    break;

  case 404:
#line 2151 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7437 "ds_parser.cpp"
    break;

  case 405:
#line 2162 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7446 "ds_parser.cpp"
    break;

  case 406:
#line 2166 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7458 "ds_parser.cpp"
    break;

  case 407:
#line 2176 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7470 "ds_parser.cpp"
    break;

  case 408:
#line 2183 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7481 "ds_parser.cpp"
    break;

  case 409:
#line 2192 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7487 "ds_parser.cpp"
    break;

  case 410:
#line 2192 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7493 "ds_parser.cpp"
    break;

  case 411:
#line 2192 "ds_parser.ypp"
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
#line 7508 "ds_parser.cpp"
    break;

  case 412:
#line 2205 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7514 "ds_parser.cpp"
    break;

  case 413:
#line 2206 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7520 "ds_parser.cpp"
    break;

  case 414:
#line 2207 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7526 "ds_parser.cpp"
    break;

  case 415:
#line 2208 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7532 "ds_parser.cpp"
    break;

  case 416:
#line 2209 "ds_parser.ypp"
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
#line 7551 "ds_parser.cpp"
    break;

  case 417:
#line 2223 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7562 "ds_parser.cpp"
    break;

  case 418:
#line 2229 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7571 "ds_parser.cpp"
    break;

  case 419:
#line 2233 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7581 "ds_parser.cpp"
    break;

  case 420:
#line 2238 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7591 "ds_parser.cpp"
    break;

  case 421:
#line 2243 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7601 "ds_parser.cpp"
    break;

  case 422:
#line 2248 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7611 "ds_parser.cpp"
    break;

  case 423:
#line 2253 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7620 "ds_parser.cpp"
    break;

  case 424:
#line 2257 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7629 "ds_parser.cpp"
    break;

  case 425:
#line 2261 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7639 "ds_parser.cpp"
    break;

  case 426:
#line 2266 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7648 "ds_parser.cpp"
    break;

  case 427:
#line 2270 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7658 "ds_parser.cpp"
    break;

  case 428:
#line 2275 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7664 "ds_parser.cpp"
    break;

  case 429:
#line 2275 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7670 "ds_parser.cpp"
    break;

  case 430:
#line 2275 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7681 "ds_parser.cpp"
    break;

  case 431:
#line 2281 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7693 "ds_parser.cpp"
    break;

  case 432:
#line 2288 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7699 "ds_parser.cpp"
    break;

  case 433:
#line 2288 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7705 "ds_parser.cpp"
    break;

  case 434:
#line 2288 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7715 "ds_parser.cpp"
    break;

  case 435:
#line 2293 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7721 "ds_parser.cpp"
    break;

  case 436:
#line 2293 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7727 "ds_parser.cpp"
    break;

  case 437:
#line 2293 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7738 "ds_parser.cpp"
    break;

  case 438:
#line 2299 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7744 "ds_parser.cpp"
    break;

  case 439:
#line 2299 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7750 "ds_parser.cpp"
    break;

  case 440:
#line 2299 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7760 "ds_parser.cpp"
    break;

  case 441:
#line 2304 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7769 "ds_parser.cpp"
    break;

  case 442:
#line 2308 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7775 "ds_parser.cpp"
    break;

  case 443:
#line 2308 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7781 "ds_parser.cpp"
    break;

  case 444:
#line 2308 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7791 "ds_parser.cpp"
    break;

  case 445:
#line 2313 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7797 "ds_parser.cpp"
    break;

  case 446:
#line 2313 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7803 "ds_parser.cpp"
    break;

  case 447:
#line 2313 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7817 "ds_parser.cpp"
    break;

  case 448:
#line 2322 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7826 "ds_parser.cpp"
    break;

  case 449:
#line 2326 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7832 "ds_parser.cpp"
    break;

  case 450:
#line 2326 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7838 "ds_parser.cpp"
    break;

  case 451:
#line 2326 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7848 "ds_parser.cpp"
    break;

  case 452:
#line 2331 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7854 "ds_parser.cpp"
    break;

  case 453:
#line 2331 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7860 "ds_parser.cpp"
    break;

  case 454:
#line 2331 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7874 "ds_parser.cpp"
    break;

  case 455:
#line 2340 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7883 "ds_parser.cpp"
    break;

  case 456:
#line 2344 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7889 "ds_parser.cpp"
    break;

  case 457:
#line 2344 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7895 "ds_parser.cpp"
    break;

  case 458:
#line 2344 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7905 "ds_parser.cpp"
    break;

  case 459:
#line 2349 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7911 "ds_parser.cpp"
    break;

  case 460:
#line 2349 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7917 "ds_parser.cpp"
    break;

  case 461:
#line 2349 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7931 "ds_parser.cpp"
    break;

  case 462:
#line 2358 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7937 "ds_parser.cpp"
    break;

  case 463:
#line 2358 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7943 "ds_parser.cpp"
    break;

  case 464:
#line 2358 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7954 "ds_parser.cpp"
    break;

  case 465:
#line 2364 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7960 "ds_parser.cpp"
    break;

  case 466:
#line 2364 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7966 "ds_parser.cpp"
    break;

  case 467:
#line 2364 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7977 "ds_parser.cpp"
    break;

  case 468:
#line 2373 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7983 "ds_parser.cpp"
    break;

  case 469:
#line 2373 "ds_parser.ypp"
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
#line 8000 "ds_parser.cpp"
    break;

  case 470:
#line 2388 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8006 "ds_parser.cpp"
    break;

  case 471:
#line 2388 "ds_parser.ypp"
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
#line 8027 "ds_parser.cpp"
    break;

  case 472:
#line 2408 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8033 "ds_parser.cpp"
    break;

  case 473:
#line 2409 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8039 "ds_parser.cpp"
    break;

  case 474:
#line 2410 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8045 "ds_parser.cpp"
    break;

  case 475:
#line 2411 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8051 "ds_parser.cpp"
    break;

  case 476:
#line 2415 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8063 "ds_parser.cpp"
    break;

  case 477:
#line 2422 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8075 "ds_parser.cpp"
    break;

  case 478:
#line 2429 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8086 "ds_parser.cpp"
    break;

  case 479:
#line 2435 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8097 "ds_parser.cpp"
    break;

  case 480:
#line 2444 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8107 "ds_parser.cpp"
    break;

  case 481:
#line 2449 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8116 "ds_parser.cpp"
    break;

  case 482:
#line 2456 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8122 "ds_parser.cpp"
    break;

  case 483:
#line 2457 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8128 "ds_parser.cpp"
    break;

  case 484:
#line 2461 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8139 "ds_parser.cpp"
    break;

  case 485:
#line 2467 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8151 "ds_parser.cpp"
    break;

  case 486:
#line 2474 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8164 "ds_parser.cpp"
    break;

  case 487:
#line 2482 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8176 "ds_parser.cpp"
    break;

  case 488:
#line 2489 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8189 "ds_parser.cpp"
    break;

  case 489:
#line 2497 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8203 "ds_parser.cpp"
    break;

  case 490:
#line 2509 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8211 "ds_parser.cpp"
    break;

  case 491:
#line 2512 "ds_parser.ypp"
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
#line 8227 "ds_parser.cpp"
    break;

  case 492:
#line 2526 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8238 "ds_parser.cpp"
    break;

  case 493:
#line 2535 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8244 "ds_parser.cpp"
    break;

  case 494:
#line 2536 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8250 "ds_parser.cpp"
    break;

  case 495:
#line 2540 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8260 "ds_parser.cpp"
    break;

  case 496:
#line 2545 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8269 "ds_parser.cpp"
    break;

  case 497:
#line 2552 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8279 "ds_parser.cpp"
    break;

  case 498:
#line 2557 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8292 "ds_parser.cpp"
    break;

  case 499:
#line 2568 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8302 "ds_parser.cpp"
    break;

  case 500:
#line 2573 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8311 "ds_parser.cpp"
    break;

  case 501:
#line 2580 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8325 "ds_parser.cpp"
    break;

  case 502:
#line 2592 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8331 "ds_parser.cpp"
    break;

  case 503:
#line 2593 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8337 "ds_parser.cpp"
    break;

  case 504:
#line 2597 "ds_parser.ypp"
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
#line 8360 "ds_parser.cpp"
    break;

  case 505:
#line 2615 "ds_parser.ypp"
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
#line 8383 "ds_parser.cpp"
    break;


#line 8387 "ds_parser.cpp"

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
#line 2635 "ds_parser.ypp"


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
