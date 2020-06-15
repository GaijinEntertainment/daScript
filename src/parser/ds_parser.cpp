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
    DAS_EXPLICIT = 313,
    DAS_SHARED = 314,
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
    RPIPE = 376,
    CLONEEQU = 377,
    ROTL = 378,
    ROTR = 379,
    ROTLEQU = 380,
    ROTREQU = 381,
    MAPTO = 382,
    BRABRAB = 383,
    BRACBRB = 384,
    CBRCBRB = 385,
    INTEGER = 386,
    LONG_INTEGER = 387,
    UNSIGNED_INTEGER = 388,
    UNSIGNED_LONG_INTEGER = 389,
    FLOAT = 390,
    DOUBLE = 391,
    NAME = 392,
    BEGIN_STRING = 393,
    STRING_CHARACTER = 394,
    END_STRING = 395,
    BEGIN_STRING_EXPR = 396,
    END_STRING_EXPR = 397,
    END_OF_READ = 398,
    UNARY_MINUS = 399,
    UNARY_PLUS = 400,
    PRE_INC = 401,
    PRE_DEC = 402,
    POST_INC = 403,
    POST_DEC = 404,
    DEREF = 405,
    COLCOL = 406
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

#line 399 "ds_parser.cpp"

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
#define YYLAST   7250

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  173
/* YYNRULES -- Number of rules.  */
#define YYNRULES  510
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  908

#define YYUNDEFTOK  2
#define YYMAXUTOK   406

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
       2,     2,     2,   161,     2,   178,   172,   157,   150,     2,
     170,   171,   155,   154,   144,   153,   166,   156,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   147,   175,
     151,   145,   152,   146,   173,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   168,     2,   169,   149,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   176,   148,   177,   160,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   158,
     159,   162,   163,   164,   165,   167,   174
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   424,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   440,   452,   453,   457,   461,
     466,   475,   483,   499,   504,   512,   512,   542,   564,   568,
     571,   577,   586,   607,   630,   631,   635,   639,   640,   644,
     647,   653,   659,   662,   668,   669,   673,   674,   675,   684,
     685,   689,   698,   714,   722,   732,   741,   742,   743,   744,
     745,   746,   750,   755,   763,   764,   765,   766,   767,   768,
     769,   770,   776,   789,   805,   806,   810,   817,   829,   833,
     840,   841,   845,   846,   847,   851,   856,   863,   867,   868,
     869,   870,   871,   872,   873,   874,   875,   876,   877,   878,
     879,   880,   881,   882,   883,   884,   885,   886,   887,   888,
     889,   890,   891,   892,   893,   894,   895,   896,   897,   898,
     899,   900,   901,   902,   906,   927,   965,   972,   976,   987,
     988,   989,   990,   991,   992,   993,   994,   995,   996,   997,
     998,   999,  1000,  1001,  1002,  1003,  1004,  1005,  1009,  1014,
    1020,  1026,  1047,  1053,  1054,  1065,  1069,  1075,  1078,  1081,
    1085,  1091,  1095,  1099,  1102,  1105,  1110,  1113,  1121,  1124,
    1129,  1132,  1140,  1146,  1147,  1151,  1152,  1156,  1191,  1191,
    1191,  1194,  1194,  1194,  1199,  1199,  1199,  1207,  1211,  1216,
    1222,  1222,  1222,  1226,  1226,  1226,  1231,  1231,  1231,  1240,
    1243,  1249,  1250,  1257,  1268,  1269,  1270,  1274,  1279,  1334,
    1335,  1336,  1337,  1338,  1339,  1343,  1344,  1345,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,
    1358,  1362,  1363,  1367,  1378,  1383,  1393,  1397,  1397,  1397,
    1404,  1404,  1404,  1418,  1422,  1422,  1422,  1430,  1431,  1432,
    1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,
    1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,
    1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1474,  1478,
    1481,  1484,  1485,  1486,  1487,  1492,  1497,  1498,  1501,  1501,
    1501,  1504,  1508,  1512,  1516,  1517,  1518,  1519,  1520,  1521,
    1522,  1532,  1542,  1597,  1598,  1602,  1603,  1607,  1615,  1618,
    1623,  1622,  1667,  1725,  1730,  1738,  1739,  1743,  1746,  1755,
    1756,  1760,  1769,  1770,  1775,  1776,  1780,  1786,  1792,  1795,
    1799,  1805,  1814,  1815,  1816,  1820,  1821,  1825,  1828,  1833,
    1838,  1846,  1857,  1860,  1867,  1868,  1872,  1899,  1899,  1911,
    1914,  1923,  1936,  1948,  1949,  1953,  1957,  1968,  1983,  1984,
    1988,  2025,  2026,  2030,  2030,  2114,  2121,  2130,  2131,  2132,
    2133,  2134,  2135,  2136,  2137,  2138,  2139,  2140,  2141,  2142,
    2143,  2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,
    2153,  2154,  2158,  2159,  2160,  2161,  2162,  2163,  2167,  2178,
    2182,  2192,  2199,  2208,  2208,  2208,  2221,  2222,  2223,  2224,
    2225,  2239,  2245,  2249,  2253,  2258,  2263,  2268,  2273,  2277,
    2281,  2286,  2290,  2295,  2295,  2295,  2301,  2308,  2308,  2308,
    2313,  2313,  2313,  2319,  2319,  2319,  2324,  2328,  2328,  2328,
    2333,  2333,  2333,  2342,  2346,  2346,  2346,  2351,  2351,  2351,
    2360,  2364,  2364,  2364,  2369,  2369,  2369,  2378,  2378,  2378,
    2384,  2384,  2384,  2393,  2393,  2408,  2408,  2428,  2429,  2430,
    2431,  2435,  2442,  2449,  2455,  2464,  2469,  2476,  2477,  2481,
    2487,  2494,  2502,  2509,  2517,  2529,  2532,  2546,  2555,  2556,
    2560,  2565,  2572,  2577,  2588,  2593,  2600,  2612,  2613,  2617,
    2635
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
  "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
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
  "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
     395,   396,   397,   398,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,   399,   400,
     126,    33,   401,   402,   403,   404,    46,   405,    91,    93,
      40,    41,    36,    64,   406,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -570

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-570)))

#define YYTABLE_NINF -465

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -570,    24,  -570,  -570,   -57,   -37,    58,    86,  -110,  -570,
     106,  -570,  -570,     3,  -570,  -570,  -570,  -570,  -570,   254,
    -570,   102,  -570,  -570,  -570,  -570,  -570,  -570,    81,  -570,
     112,   118,   144,  -570,  -570,     0,  -570,   -61,   154,  -570,
     170,   177,  -570,   141,   180,   155,  -570,    67,  -570,  -570,
    -570,    13,   189,  -570,   151,    17,   -57,   197,   -37,   195,
    -570,   199,   200,  -570,   320,  -570,   202,  -570,   -94,   158,
     168,   208,  -570,   -57,     3,  -570,  7089,   315,   316,  -570,
     185,   181,  -570,   206,  -570,  -570,   355,  -570,  -570,  -570,
    -570,  -570,   219,    48,  -570,  -570,  -570,  -570,   307,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,   186,   -87,  5993,
    -570,  -570,   226,   227,  -570,   -43,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,   -89,   222,  -570,  -570,   -63,  -570,   194,
     -70,   201,  -570,   -40,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,     2,  -570,  -570,   -97,  -570,   221,   223,   232,   233,
    -570,  -570,  -570,   217,  -570,  -570,  -570,  -570,  -570,   237,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,   238,
    -570,  -570,  -570,   243,   245,  -570,  -570,  -570,  -570,   247,
     248,  -570,  -570,  -570,  -570,  -570,    85,  -570,   231,   250,
    -570,   236,  -570,   -57,  -570,    -5,  -570,   -38,  5993,  -570,
    1246,  -570,  -570,  -570,  -570,  -570,   104,  -570,  -570,  -570,
      48,  -570,   -69,  4558,  -570,  -570,  -570,  -570,  -570,   263,
    -570,    97,   100,   129,  -570,  -570,  -570,  -570,  -570,  -570,
     369,  -570,  -570,    -9,  2437,  -570,  -570,   -67,  5993,   -65,
      70,   276,  -570,   -41,  -570,   290,  7005,  -570,  4558,  -570,
    -570,   276,  -570,  -570,  5917,   249,   951,  -570,  -570,   181,
    4558,   251,  4558,   272,   273,   255,  -570,   252,   277,   298,
    2570,   181,  -570,   279,  2022,  4558,  4558,   117,   460,  4690,
    4558,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  4558,  4558,
    4558,   -63,  4558,  4558,  4558,  -570,   258,  -570,   396,  -570,
    -570,   261,   262,  4558,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,   -79,   264,  -570,   265,   275,  -570,  -570,  -570,
     414,  -570,  -570,  -570,   283,  -570,  -570,   -78,  -570,  -570,
    -570,  -570,  -570,  6391,   286,  -570,  -570,  -570,  -570,  -570,
       7,   321,  5993,  -570,    68,  -570,  -570,  5249,  5993,  5993,
    5993,  5993,   282,   226,  5993,   185,  5993,   185,  5993,   185,
    6069,   227,  -570,  -570,  -570,   288,  -570,  -570,  6066,  -570,
    -570,  7005,  -570,  -570,   293,  -570,  -570,  -570,  5993,  -570,
    2703,  -570,  1043,    14,   453,  -570,   -63,  2703,  -570,  -570,
     340,  4688,   451,  6667,  4558,  1043,  -570,  -570,  4558,  -570,
    -570,   319,   337,  6667,  -570,  5993,  2703,  -570,  4787,   454,
     454,   296,  -570,   276,  1575,   276,  1724,  6457,  -570,    30,
     183,   454,   454,   -29,  -570,   454,   454,  5388,   -72,   294,
    -570,  -570,  1043,  -570,  2155,  -570,  -570,  -570,   432,   276,
     185,   117,  -570,    31,   336,  4558,  4558,  4558,  4558,  4558,
    4558,  4558,  4558,  4558,  4558,  -570,  -570,  4558,  4558,  4558,
    4558,  4558,  4558,   339,  2836,  4558,   341,  4558,  4558,  4558,
    4558,  4558,  4558,  4558,  4558,  2969,  3132,  4558,  4558,  4558,
    4558,  4558,  4558,  4558,  4558,  4558,  4558,   351,  4558,  3265,
    -570,   482,   450,   323,  -570,   721,  -570,  -570,  -570,  2703,
    -570,   532,  1060,  1872,  2966,  -570,    -3,  3078,   222,  3264,
     222,  3413,   222,    34,  -570,    54,  7005,    89,  -570,  -570,
    -570,  3562,  -570,  6284,  -570,  4558,  3414,  2304,  -570,  4886,
    4558,  4558,  -570,   181,  5460,  -570,  5993,  5993,  5529,  5993,
    -570,  -570,  3711,  -570,  4985,  -570,  -570,    16,   117,   -76,
    1873,  3563,  6457,   352,   -24,   326,   356,  -570,  -570,    52,
      26,  3712,   -24,   135,  4558,  4558,   324,  -570,  4558,   363,
     364,  -570,   169,  -570,  -570,   246,   368,  -570,    27,  6667,
    -570,  -570,   222,  -570,   357,  -570,  -570,  6667,  6667,  6667,
    6667,  6667,  6667,  6667,  6667,   225,   225,   628,  6667,  6667,
     628,  6821,  6821,   342,  4558,  6667,   299,  -570,  6142,   -32,
     -32,  6667,   225,   225,  6667,  6667,  4558,  6667,   370,  6529,
     651,  6733,  6755,   628,   628,     8,     8,   299,   299,   299,
    -570,   508,  6209,  -570,    47,   257,  -570,   276,  -570,   369,
    -570,  2703,  -570,  5084,  -570,  5993,  -570,  -570,  -570,   376,
    -570,   362,  -570,   366,  -570,   367,  5993,  -570,  6069,  -570,
     227,   369,  4558,  -100,  -570,    57,   365,   378,  5598,  -570,
    -570,  -570,  3860,  4009,  -570,  6845,   385,  -570,  4558,  -570,
    4558,  4558,   -10,   384,   368,   388,   389,  4558,   392,  4558,
    4558,   368,   386,   393,  6667,  -570,  -570,  6595,  -570,   426,
    5993,   185,  1423,   181,  -570,  -570,  4558,  -570,    74,    73,
    4558,  -570,   -71,  -570,  3861,   331,  -570,   331,  -570,  4558,
    -570,  -570,  -570,  -570,    13,  -570,  -570,  5183,  -570,  -570,
    6890,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    7005,  -570,  -570,  -570,  6667,   181,  -570,  -570,   110,  -570,
    -570,  -570,  -570,  4010,   -55,  6667,  6667,   -24,   397,    75,
     352,   404,  -570,  6667,  -570,  -570,    50,   -24,   405,  -570,
    -570,  -570,  6901,   222,  -570,  -570,  1043,   406,  6667,  4159,
    -570,  -570,  5993,  -570,    66,  6667,  -570,    13,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  5993,  4292,   439,  4558,  4558,
    4558,  -570,  5667,  4558,   409,   410,  4558,  4558,  -570,  4558,
     407,  -570,  -570,   429,   246,  -570,  4558,  6667,  6935,  -570,
     408,  -570,  6949,   434,  5736,   435,   628,   628,   628,  -570,
    5322,   417,  -570,  6667,  6667,  5322,   419,   -63,  -570,  -570,
    6667,  -570,  -570,  -570,  -570,  -570,  4425,   541,   423,  -570,
     416,  -570,  -570,   -63,  -570,   424,  5993,   443,  5805,  4558,
     427,   433,  -570,  -570,  6962,  -570,  -570,  6667,  -570,  -570,
    -570,  5993,   430,  6976,  -570,  -570,   436,  -570
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   175,     0,     0,     0,     0,     0,   176,
       0,   475,   473,     0,    14,     3,    10,     9,     8,     0,
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
     390,   391,   392,   393,   394,   395,   396,   398,   399,   446,
     382,   389,   400,   453,   460,   380,   387,   381,   388,     0,
       0,   408,   416,   419,   417,   418,     0,   411,     0,     0,
     332,     0,    77,     0,    83,     0,   325,     0,     0,   125,
       0,   369,   318,   375,   356,   353,   345,   358,    17,    18,
       0,    71,     0,     0,   360,   437,   440,   443,   433,     0,
     413,   447,   454,   461,   467,   470,   424,   429,   423,   436,
       0,   432,   426,     0,     0,   362,   428,     0,     0,     0,
       0,     0,   323,   336,    84,   313,    86,   150,     0,    49,
      50,     0,   253,   254,     0,     0,     0,   247,   161,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,   209,   211,   210,   212,   213,   214,    19,     0,     0,
       0,     0,     0,     0,     0,   204,   205,   129,   127,   251,
     250,     0,     0,     0,   143,   138,   136,   135,   137,   207,
     149,   130,   248,     0,   306,     0,     0,   141,   142,   144,
     173,   134,   305,   304,    80,   309,   249,     0,   131,   308,
     307,   286,   255,   215,     0,   252,   477,   478,   479,   480,
     313,     0,     0,   346,     0,    63,   367,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,    82,     0,    82,
       0,     0,   431,   425,   427,     0,   430,   421,     0,   412,
     476,   331,   474,   333,     0,   324,   335,   334,     0,   337,
       0,   326,     0,     0,   157,   160,     0,     0,   163,   166,
       0,   215,     0,   156,     0,     0,   178,   181,     0,   147,
     184,     0,     0,    43,    53,     0,     0,   170,   215,   278,
     279,   205,   152,     0,   487,     0,     0,     0,   504,     0,
       0,   259,   258,   291,    25,   257,   256,     0,   206,     0,
     145,   146,     0,   312,     0,   133,   139,   140,     0,     0,
      82,     0,   132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,   281,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   244,     0,     0,
     374,     0,   315,     0,   376,     0,   343,   344,   342,     0,
     361,     0,     0,     0,     0,   410,     0,     0,    85,     0,
      85,     0,    85,   153,   329,     0,   327,     0,   422,   420,
     314,   338,   341,   340,    54,     0,     0,     0,   167,   215,
       0,     0,   164,     0,     0,    55,     0,     0,     0,     0,
      41,    42,     0,   171,   215,   168,   206,     0,     0,   153,
       0,     0,   495,   485,   487,     0,   498,   499,   500,     0,
       0,     0,   487,     0,     0,     0,     0,    22,     0,    20,
       0,   282,   237,   236,   148,    46,     0,   287,     0,   199,
     174,   177,    85,   151,     0,   301,   302,   222,   223,   225,
     224,   226,   219,   220,   221,   260,   261,   273,   227,   228,
     274,   271,   272,     0,     0,   217,   296,   285,     0,   310,
     311,   218,   262,   263,   229,   230,     0,   216,     0,     0,
     276,   277,   275,   269,   270,   265,   264,   266,   267,   268,
     243,     0,     0,   289,     0,   315,   316,     0,   319,   342,
     347,     0,   351,   215,   438,     0,   444,   434,   414,     0,
     448,     0,   455,     0,   462,     0,     0,   468,     0,   471,
       0,   334,     0,     0,   158,     0,     0,     0,     0,   165,
     172,   292,     0,     0,   293,     0,     0,   169,     0,   488,
       0,     0,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   505,   506,     0,    23,    26,
       0,    82,     0,     0,    44,    45,     0,    51,     0,     0,
       0,   288,     0,   298,     0,   232,   284,   231,   303,     0,
     245,   283,   290,   320,     0,   317,   349,   215,   350,   439,
       0,   445,   435,   415,   449,   451,   456,   458,   463,   465,
     328,   469,   330,   472,   339,     0,   159,   190,     0,   187,
     179,   182,   185,     0,     0,   482,   481,   487,     0,     0,
     486,     0,   490,   496,   502,   501,     0,   487,     0,   503,
      21,    24,     0,    85,   128,    47,     0,     0,   200,     0,
     201,   208,     0,   234,     0,   297,   246,     0,   322,   348,
     441,   452,   459,   466,    52,     0,     0,     0,     0,     0,
       0,   294,     0,     0,     0,     0,     0,     0,   489,     0,
       0,   493,   238,     0,    46,   233,     0,   202,     0,   235,
       0,   442,     0,     0,     0,     0,   180,   183,   186,   295,
     507,     0,   491,   484,   483,   507,     0,     0,   241,    48,
     203,   299,   321,   191,   193,   188,     0,     0,     0,   492,
       0,   494,   239,     0,   300,     0,     0,     0,     0,     0,
       0,     0,   242,   192,     0,   196,   189,   508,   509,   510,
     194,     0,     0,     0,   195,   197,     0,   198
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -570,  -570,  -570,   159,   549,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,   507,  -570,  -570,   550,  -570,  -570,
    -570,  -234,  -570,  -570,  -570,  -570,  -570,  -570,   372,  -570,
     557,   -56,  -570,   540,  -570,  -294,  -377,  -520,  -570,  -570,
    -202,  -138,   -77,  -570,    15,  -279,   -13,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,   616,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -504,  -570,  -570,  -291,  -570,  -228,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,   403,  -570,
    -570,   256,   -47,  -570,  -570,  -570,   350,  -570,   -60,  -570,
    -264,   239,  -272,  -265,   107,  -570,   160,  -570,  -570,  -570,
    -570,   462,   559,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -151,  -106,  -570,  -570,  -570,   253,  -570,  -570,  -570,
     -75,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,
    -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,  -570,   353,
    -569,  -435,  -565,  -570,  -570,  -297,   -81,   193,  -570,  -570,
    -570,  -225,  -570
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   450,   329,   729,   330,   600,
      16,    17,    35,    36,    63,    18,    32,    33,   331,   332,
     736,   737,   333,   334,   335,   336,   337,   338,   170,   171,
      29,    30,    45,    46,    47,    19,   154,   229,    80,    20,
      81,    82,   339,   340,   230,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   469,   350,   351,   352,   566,   828,
     567,   829,   569,   830,   353,   825,   885,   886,   902,   901,
     906,   608,   811,   354,   355,   356,   420,   358,   359,   360,
     361,   730,   867,   731,   883,   362,   661,   816,   609,   812,
     884,   225,   667,   523,   370,   817,   226,   227,   544,   545,
     220,   221,   711,   272,   529,   374,   235,   160,    54,    22,
      86,   108,    67,    68,    23,    24,   158,    84,    52,    25,
     159,   273,   364,   107,   213,   214,   218,   215,   383,   763,
     546,   381,   762,   378,   759,   379,   851,   380,   761,   384,
     764,   385,   821,   386,   766,   387,   822,   388,   768,   389,
     823,   390,   771,   391,   773,    26,    41,    27,    40,   365,
     583,   584,   585,   366,   586,   587,   588,   589,   367,   449,
     368,   878,   369
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   410,   357,   212,   156,   403,   405,   419,   538,   236,
     540,   592,   542,   448,   -80,   664,   442,   115,   681,   715,
     683,    59,   685,   393,     2,   437,   578,   722,   473,   474,
       3,    87,    88,   555,   216,   708,    42,   739,   406,   223,
     578,   471,   271,    66,   740,   720,   710,    76,   243,   614,
     174,   693,   695,     4,    60,     5,   809,     6,   463,     7,
     470,    48,   164,   165,     8,    43,    38,   233,   174,   407,
     399,     9,   219,   406,    43,   775,   521,    10,   244,   602,
      28,   493,   224,   110,   496,   497,    64,   496,   497,   740,
     175,   464,   742,   612,    31,    11,   211,   472,    71,   238,
     239,    56,    44,   371,   407,   155,   408,   234,   376,   409,
     400,    44,   402,   485,   486,    65,    12,   256,    77,    78,
     833,   493,   212,   495,   496,   497,   498,   738,   222,   499,
     413,   552,   233,   274,   517,   223,   518,   275,   558,   518,
      43,   394,   257,   258,   231,   790,   240,   788,    89,   678,
      79,   714,    90,   276,    91,    92,    61,   573,   371,   395,
     371,    53,   212,   514,   515,   516,    62,   270,   615,   396,
     371,   740,   679,   241,   517,    13,   518,    44,   212,   166,
     613,   686,   526,   167,   520,   168,    92,    93,   406,   406,
     527,   740,    13,   401,   740,    34,   710,   836,   741,    14,
     259,   740,   212,   212,   784,   595,   687,   596,    71,   414,
     740,    74,   422,   528,    56,   211,   796,   713,   752,   407,
     407,   718,   834,    37,   434,   839,    55,   719,   776,   688,
     260,   261,   840,   444,   446,   262,    75,   849,   263,   404,
     814,   689,   807,    39,  -450,   473,   474,  -457,   371,  -450,
     672,   372,  -457,   264,   373,   211,    56,   733,    49,    50,
     265,    51,   826,   266,   690,    57,   212,  -450,   734,   735,
    -457,   211,   212,   212,   212,   212,  -464,   787,   212,   692,
     212,  -464,   212,   843,   212,   827,   797,   709,    58,   325,
     441,    66,   577,   155,   590,   211,   211,   525,   725,  -464,
     666,   753,   212,   531,   532,   533,   534,    69,   454,   537,
     719,   539,   723,   541,    70,    71,  -240,    72,   236,   473,
     474,  -240,   162,   597,   598,    73,    83,    85,    96,   212,
     485,   486,    98,   551,   112,   554,    99,   100,   493,  -240,
     495,   496,   497,   498,   113,   114,   499,   109,   565,   151,
     152,   473,   474,   157,   803,   153,   161,   155,   162,   211,
     572,    60,   173,   217,   219,   211,   211,   211,   211,   228,
     232,   211,   245,   211,   246,   211,   237,   211,   512,   513,
     514,   515,   516,   247,   248,   605,   101,   249,   250,   251,
     102,   517,   756,   518,   252,   211,   253,   268,   254,   255,
     382,   392,   755,   557,   485,   486,   267,   103,   104,   105,
     106,   269,   493,   233,   495,   496,   497,   498,   223,   416,
     499,   424,   211,   426,   427,   428,   403,   429,   430,   431,
     435,   458,   459,   468,   483,   484,   460,   461,   487,   465,
     466,   490,   491,   492,   493,   603,   495,   496,   497,   498,
     467,    13,   499,   535,   501,   502,   519,   548,   524,   471,
     212,   212,   550,   212,   563,   517,   570,   518,   571,   576,
     604,   610,   443,   616,   473,   474,   633,   506,   637,   507,
     508,   509,   510,   511,   176,   256,   700,   515,   660,   665,
     177,   702,   703,   666,   705,   716,   713,   517,   668,   518,
     717,   726,   238,   728,   357,   738,   178,   748,   743,   750,
     257,   258,   744,   399,   765,   778,   777,   837,   767,   769,
     179,   789,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   211,   783,   211,   791,   792,   485,
     486,   794,   799,   798,   256,   801,   835,   493,   259,   212,
     496,   497,   498,   838,   841,   499,   855,   845,   861,   862,
     212,   868,   212,   872,   866,   874,   879,   876,   881,   257,
     258,   889,   890,   891,   895,   893,   898,    43,   260,   261,
     760,   904,   899,   262,    94,   172,   263,   907,    97,   599,
     869,   770,   375,    95,   116,   850,   818,    21,   754,   732,
     517,   264,   518,   556,   212,   411,   522,   111,   772,   611,
     547,   266,   671,   363,    44,   242,   536,   415,   795,   593,
     880,     0,     0,     0,     0,     0,   377,   259,   473,   474,
       0,     0,     0,     0,     0,   802,   805,     0,     0,     0,
       0,     0,   211,     0,     0,   810,     0,   398,     0,     0,
       0,   473,   474,   211,     0,   211,     0,   260,   261,     0,
       0,   412,   262,     0,   674,   263,     0,     0,     0,   421,
       0,     0,     0,   423,     0,   425,     0,     0,   824,     0,
     264,     0,     0,   433,     0,     0,   212,   438,   439,   440,
     266,     0,     0,   447,     0,     0,     0,   211,     0,   212,
       0,   451,   452,   453,     0,   455,   456,   457,     0,   844,
       0,   483,   484,   485,   486,     0,   462,   848,     0,     0,
       0,   493,     0,   495,   496,   497,   498,     0,     0,   499,
     852,   501,   502,   256,   483,   484,   485,   486,   487,     0,
       0,   490,   491,   492,   493,     0,   495,   496,   497,   498,
       0,     0,   499,     0,   501,   502,     0,     0,   257,   258,
     212,   512,   513,   514,   515,   516,     0,     0,     0,     0,
       0,     0,     0,     0,   517,   212,   518,     0,     0,   211,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
       0,   894,   211,   553,     0,     0,     0,   517,     0,   518,
     559,     0,     0,     0,     0,     0,   903,   564,     0,     0,
       0,   568,     0,     0,     0,   526,   259,     0,     0,   574,
       0,     0,     0,   527,     0,     0,     0,   582,     0,   582,
       0,     0,     0,     0,   882,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   261,     0,     0,
     892,   262,     0,   211,   263,     0,     0,     0,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   211,   264,
     627,   628,   629,   630,   631,   632,   670,   635,   636,   266,
     638,   639,   640,   641,   642,   643,   644,   645,   647,   649,
     650,   651,   652,   653,   654,   655,   656,   657,   658,   659,
       0,   662,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   673,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     698,     0,     0,   635,   647,   282,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   287,     0,     0,
       0,     0,     0,   451,   457,     0,     0,     0,     0,   291,
       0,     0,     0,   293,   457,     0,   294,   724,   447,   295,
       0,   727,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,   745,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,   747,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,   473,   474,   417,     0,     0,     0,     0,
       0,   307,     0,     0,   757,     0,     0,     0,     0,   308,
     309,   310,   311,   312,   313,   314,   315,   316,    43,   317,
       0,     0,   256,     0,     0,   774,     0,     0,     0,     0,
       0,     0,     0,     0,   318,   319,   320,     0,   321,     0,
       0,   322,   323,   785,   786,     0,     0,   257,   258,     0,
     793,   324,   582,   325,   326,    44,   418,   155,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,   806,
       0,     0,     0,   808,     0,     0,   483,   484,   485,   486,
     487,     0,   815,   490,   491,   492,   493,     0,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   832,     0,     0,   506,
       0,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,     0,     0,     0,     0,   260,   261,     0,     0,   517,
     262,   518,   847,   263,     0,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,     0,     0,     0,   264,   854,
       0,   856,   857,   858,     0,   675,   860,     0,   266,   863,
     864,     0,   865,     0,     0,     0,     0,   277,     0,   870,
       0,     0,     3,     0,   278,   279,   280,     0,   281,     0,
     282,   283,   284,   285,     0,     0,     0,     0,     0,     0,
       0,   286,   287,   288,   289,     0,     0,     0,     0,   888,
       0,     0,     0,   290,   291,     0,     0,   292,   293,     0,
       0,   294,   897,     9,   295,   296,     0,   297,   298,     0,
       0,   299,   300,     0,     0,     0,     0,   301,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   302,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   303,
     304,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    43,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
     319,   320,     0,   321,     0,     0,   322,   323,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,   325,   326,
      44,   327,   155,   328,   277,     0,     0,     0,     0,     3,
       0,   278,   279,   280,     0,   281,     0,   282,   283,   284,
     285,     0,     0,     0,     0,     0,     0,     0,   286,   287,
     288,   289,     0,     0,     0,     0,     0,     0,     0,     0,
     290,   291,     0,     0,   292,   293,     0,     0,   294,     0,
       9,   295,   296,     0,   297,   298,     0,     0,   299,   300,
       0,     0,     0,     0,   301,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      43,   317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   318,   319,   320,     0,
     321,     0,     0,   322,   323,     0,     0,     0,     0,   282,
     283,   284,   285,   324,     0,   325,   326,    44,   327,   155,
     804,   287,     0,     0,     0,     0,     0,   256,     0,     0,
       0,     0,     0,   291,     0,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,   578,     0,   298,     0,     0,
       0,     0,   257,   258,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   579,   317,     0,     0,     0,     0,     0,     0,
     260,   261,     0,     0,     0,   262,     0,     0,   580,   319,
     320,     0,   321,     0,     0,   322,   323,     0,   282,   283,
     284,   285,     0,   264,     0,   581,     0,   325,   326,    44,
     287,   155,     0,   266,     0,     0,   256,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
       0,   257,   258,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   579,   317,     0,     0,     0,     0,     0,     0,   260,
     261,     0,     0,     0,   262,     0,     0,   580,   319,   320,
       0,   321,     0,     0,   322,   323,     0,   282,   283,   284,
     285,     0,   264,     0,   591,     0,   325,   326,    44,   287,
     155,     0,   266,     0,   256,   393,     0,     0,     0,     0,
       0,   291,     0,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,   257,
     258,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      43,   317,     0,     0,     0,     0,     0,   260,   261,     0,
       0,     0,   262,   394,   676,   263,   318,   319,   320,     0,
     321,     0,     0,   322,   323,     0,   282,   283,   284,   285,
     264,   395,     0,   324,     0,   325,   326,    44,   287,   155,
     266,   396,     0,     0,     0,     0,     0,     0,     0,     0,
     291,     0,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    43,
     317,     0,     0,     0,     0,     0,     0,     0,     0,   282,
     283,   284,   285,     0,     0,   318,   319,   320,     0,   321,
       0,   287,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   291,   325,   326,    44,   293,   155,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    43,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,   282,   283,
     284,   285,   696,   606,     0,   324,   607,   325,   326,    44,
     287,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    43,   317,     0,     0,     0,     0,     0,     0,     0,
       0,   282,   283,   284,   285,   697,     0,   318,   319,   320,
       0,   321,     0,   287,   322,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   291,   325,   326,    44,   293,
     155,     0,   294,     0,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,   309,   310,   311,   312,
     313,   314,   315,   316,    43,   317,     0,     0,     0,     0,
       0,     0,     0,     0,   282,   283,   284,   285,     0,     0,
     318,   319,   320,     0,   321,     0,   287,   322,   323,     0,
       0,     0,     0,     0,     0,     0,   397,   324,   291,   325,
     326,    44,   293,   155,     0,   294,     0,     0,   295,     0,
       0,     0,   298,     0,     0,   432,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   302,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   306,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    43,   317,     0,
       0,     0,     0,     0,     0,     0,     0,   282,   283,   284,
     285,     0,     0,   318,   319,   320,     0,   321,     0,   287,
     322,   323,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   291,   325,   326,    44,   293,   155,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      43,   317,     0,     0,     0,     0,     0,     0,     0,     0,
     282,   283,   284,   285,     0,     0,   318,   319,   320,     0,
     321,     0,   287,   322,   323,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   291,   325,   326,    44,   293,   155,
       0,   294,     0,     0,   295,     0,     0,     0,   298,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   302,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   634,     0,     0,     0,
       0,     0,     0,     0,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    43,   317,     0,     0,     0,     0,     0,
       0,     0,     0,   282,   283,   284,   285,     0,     0,   318,
     319,   320,     0,   321,     0,   287,   322,   323,   256,     0,
       0,     0,     0,     0,     0,     0,   324,   291,   325,   326,
      44,   293,   155,     0,   294,     0,     0,   295,     0,     0,
       0,   298,     0,   257,   258,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,   646,
       0,     0,     0,     0,     0,     0,     0,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    43,   317,     0,     0,
     256,   260,   261,     0,     0,     0,   262,     0,   677,   263,
       0,     0,   318,   319,   320,     0,   321,     0,     0,   322,
     323,     0,     0,     0,   264,   257,   258,     0,     0,   324,
       0,   325,   326,    44,   266,   155,   282,   283,   284,   285,
       0,     0,     0,   648,     0,     0,     0,     0,   287,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     291,     0,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,   260,   261,   303,     0,     0,   262,     0,
     680,   263,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    43,
     317,     0,     0,     0,     0,     0,     0,     0,     0,   282,
     283,   284,   285,     0,     0,   318,   319,   320,     0,   321,
       0,   287,   322,   323,     0,     0,   256,     0,     0,     0,
       0,     0,   324,   291,   325,   326,    44,   293,   155,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,   257,   258,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    43,   317,     0,     0,     0,     0,     0,   260,
     261,     0,     0,     0,   262,     0,   682,   263,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,   282,   283,
     284,   285,   264,     0,     0,   324,   663,   325,   326,    44,
     287,   155,   266,     0,     0,   256,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
     257,   258,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    43,   317,     0,     0,     0,     0,     0,   260,   261,
       0,     0,     0,   262,     0,   684,   263,   318,   319,   320,
       0,   321,     0,     0,   322,   323,     0,   282,   283,   284,
     285,   264,     0,     0,   324,   694,   325,   326,    44,   287,
     155,   266,     0,     0,   256,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,   257,
     258,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,   406,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      43,   317,     0,     0,     0,     0,     0,   691,   261,     0,
       0,     0,   262,     0,     0,   263,   318,   319,   320,     0,
     321,     0,     0,   322,   323,     0,   282,   283,   284,   285,
     264,     0,     0,   324,   712,   325,   326,    44,   287,   155,
     266,     0,     0,   256,     0,     0,     0,     0,     0,     0,
     291,     0,     0,     0,   293,     0,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,   257,   258,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    43,
     317,     0,     0,     0,     0,     0,   260,   261,     0,     0,
       0,   262,     0,   706,   263,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,   282,   283,   284,   285,   264,
       0,     0,   324,   721,   325,   326,    44,   287,   155,   266,
       0,     0,   256,     0,     0,     0,     0,     0,     0,   291,
       0,     0,     0,   293,     0,     0,   294,     0,     0,   295,
       0,     0,     0,   298,     0,     0,     0,   257,   258,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   308,
     309,   310,   311,   312,   313,   314,   315,   316,    43,   317,
       0,     0,     0,     0,     0,   260,   261,     0,     0,     0,
     262,     0,   780,   263,   318,   319,   320,     0,   321,     0,
       0,   322,   323,     0,   282,   283,   284,   285,   264,     0,
       0,   324,   813,   325,   326,    44,   287,   155,   266,     0,
       0,   256,     0,     0,     0,     0,     0,     0,   291,     0,
       0,     0,   293,     0,     0,   294,     0,     0,   295,     0,
       0,     0,   298,     0,     0,     0,   257,   258,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   302,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   306,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    43,   317,     0,
       0,     0,     0,     0,   260,   261,     0,     0,     0,   262,
       0,   781,   263,   318,   319,   320,     0,   321,     0,     0,
     322,   323,     0,   282,   283,   284,   285,   264,     0,     0,
     324,   831,   325,   326,    44,   287,   155,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,     0,     0,
       0,   293,     0,     0,   294,     0,     0,   295,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   846,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    43,   317,     0,     0,
       0,     0,     0,     0,     0,     0,   282,   283,   284,   285,
     853,     0,   318,   319,   320,     0,   321,     0,   287,   322,
     323,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     291,   325,   326,    44,   293,   155,     0,   294,     0,     0,
     295,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    43,
     317,     0,     0,     0,     0,     0,     0,     0,     0,   282,
     283,   284,   285,   887,     0,   318,   319,   320,     0,   321,
       0,   287,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   291,   325,   326,    44,   293,   155,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    43,   317,     0,     0,     0,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,   318,   319,
     320,     0,   321,     0,   287,   322,   323,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   291,   325,   326,    44,
     293,   155,     0,   294,     0,     0,   295,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    43,   317,     0,     0,     0,
       0,     0,   445,     0,     0,     0,     0,     0,   473,   474,
       0,   318,   319,   320,   176,   321,     0,     0,   322,   323,
     177,     0,     0,     0,     0,     0,     0,     0,   324,     0,
     325,   326,    44,     0,   155,     0,   178,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     179,     0,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   560,   495,   496,   497,   498,   473,   474,   499,
     500,   501,   502,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,   561,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,     0,     0,     0,     0,
       0,     0,     0,     0,   517,     0,   518,     0,     0,     0,
       0,     0,     0,   562,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   560,   495,   496,   497,   498,   473,   474,   499,   500,
     501,   502,   503,   504,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   506,     0,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,     0,     0,     0,     0,     0,
       0,     0,     0,   517,     0,   518,     0,     0,     0,     0,
       0,     0,   575,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     560,   495,   496,   497,   498,   473,   474,   499,   500,   501,
     502,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   506,     0,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,     0,     0,     0,     0,     0,     0,
       0,     0,   517,     0,   518,     0,     0,     0,     0,     0,
       0,   699,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   560,
     495,   496,   497,   498,   473,   474,   499,   500,   501,   502,
     503,   504,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,     0,     0,     0,     0,     0,     0,     0,
       0,   517,     0,   518,     0,     0,     0,     0,     0,     0,
     707,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   560,   495,
     496,   497,   498,   473,   474,   499,   500,   501,   502,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,     0,     0,     0,     0,     0,     0,   758,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   473,
     474,     0,     0,     0,     0,     0,     0,     0,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   560,   495,   496,
     497,   498,     0,     0,   499,   500,   501,   502,   503,   504,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   506,
       0,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,     0,   473,   474,     0,     0,     0,     0,     0,   517,
       0,   518,   483,   484,   485,   486,   487,     0,   819,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,     0,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,     0,   473,   474,
       0,     0,     0,     0,     0,   517,     0,   518,     0,     0,
       0,     0,     0,     0,   530,   483,   484,   485,   486,   487,
       0,     0,   490,   491,   492,   493,     0,   495,   496,   497,
     498,     0,     0,   499,     0,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,     0,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     473,   474,     0,     0,     0,     0,     0,     0,   517,     0,
     518,   483,   484,   485,   486,   487,     0,   877,   490,   491,
     492,   493,     0,   495,   496,   497,   498,     0,     0,   499,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,     0,     0,     0,   473,
     474,     0,     0,     0,   517,     0,   518,     0,     0,   601,
       0,     0,     0,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,     0,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   473,   474,
       0,     0,     0,     0,     0,     0,   517,     0,   518,     0,
       0,   701,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,     0,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   473,   474,     0,
       0,     0,     0,     0,     0,   517,     0,   518,     0,     0,
     704,   483,   484,   485,   486,   487,     0,     0,   490,   491,
     492,   493,     0,   495,   496,   497,   498,     0,     0,   499,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   473,   474,     0,     0,
       0,     0,     0,     0,   517,     0,   518,     0,     0,   779,
     483,   484,   485,   486,   487,     0,     0,   490,   491,   492,
     493,     0,   495,   496,   497,   498,     0,     0,   499,     0,
     501,   502,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,     0,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   473,   474,     0,     0,     0,
       0,     0,     0,   517,     0,   518,     0,     0,   859,   483,
     484,   485,   486,   487,     0,     0,   490,   491,   492,   493,
       0,   495,   496,   497,   498,     0,     0,   499,     0,   501,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,     0,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,     0,     0,     0,     0,     0,     0,
       0,     0,   517,     0,   518,     0,     0,   875,   483,   484,
     485,   486,   487,     0,     0,   490,   491,   492,   493,     0,
     495,   496,   497,   498,     0,     0,   499,     0,   501,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,   177,     0,     0,
       0,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   178,     0,     0,     0,     0,     0,     0,
       0,   517,     0,   518,     0,     0,   896,   179,     0,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,     0,     0,   308,   309,   310,     0,     0,
       0,     0,     0,   179,    43,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   473,   474,     0,     0,
       0,    44,     0,   176,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   179,
      43,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   473,   474,     0,     0,     0,    44,     0,   483,
     484,   485,   486,   487,     0,     0,   490,   491,   492,   493,
       0,   495,   496,   497,   498,     0,     0,   499,     0,   501,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   543,     0,     0,     0,
       0,     0,   506,     0,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,     0,     0,     0,     0,     0,   473,
     474,     0,   517,     0,   518,   549,     0,     0,     0,     0,
       0,     0,     0,    44,     0,   483,   484,   485,   486,   487,
       0,     0,   490,   491,   492,   493,     0,   495,   496,   497,
     498,     0,     0,   499,     0,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,     0,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
       0,     0,     0,     0,   473,   474,     0,     0,   517,     0,
     518,   746,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,     0,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,     0,     0,     0,
       0,     0,     0,     0,     0,   517,     0,   518,   751,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   560,   495,
     496,   497,   498,  -215,     0,   499,   500,   501,   502,   503,
     504,   473,   474,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,   474,     0,
       0,     0,     0,     0,     0,     0,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
       0,     0,   499,   500,   501,   502,   503,   504,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   505,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   473,
     474,     0,     0,     0,     0,     0,     0,   517,     0,   518,
     483,   484,   485,   486,   487,     0,     0,   490,   491,   492,
     493,     0,   495,   496,   497,   498,     0,     0,   499,     0,
     501,   502,     0,     0,   594,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,     0,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   473,   474,     0,     0,     0,
       0,     0,     0,   517,     0,   518,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,   749,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   473,   474,     0,
       0,     0,     0,     0,     0,   517,     0,   518,   483,   484,
     485,   486,   487,     0,     0,   490,   491,   492,   493,     0,
     495,   496,   497,   498,     0,     0,   499,     0,   501,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   800,     0,     0,
       0,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   473,   474,     0,     0,     0,     0,     0,
       0,   517,     0,   518,     0,     0,     0,     0,     0,     0,
     483,   484,   485,   486,   487,   473,   474,   490,   491,   492,
     493,     0,   495,   496,   497,   498,     0,     0,   499,     0,
     501,   502,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,     0,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,     0,     0,     0,     0,     0,
       0,     0,     0,   517,     0,   518,   483,   484,   485,   486,
     487,   473,   474,   490,   491,   492,   493,     0,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,   483,   484,
     485,   486,   487,     0,     0,   490,   491,   492,   493,     0,
     495,   496,   497,   498,     0,     0,   499,   256,   501,   502,
       0,     0,     0,   509,   510,   511,   512,   513,   514,   515,
     516,     0,     0,     0,     0,     0,     0,     0,     0,   517,
       0,   518,   257,   258,     0,     0,   510,   511,   512,   513,
     514,   515,   516,     0,     0,     0,     0,     0,     0,     0,
       0,   517,   256,   518,   483,   484,   485,   486,   487,     0,
       0,   490,     0,   256,   493,     0,   495,   496,   497,   498,
       0,     0,   499,     0,   501,   502,     0,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   257,   258,
     259,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,     0,
       0,   256,     0,     0,     0,     0,     0,   517,     0,   518,
     260,   261,   257,   258,   256,   262,     0,   782,   263,     0,
       0,     0,     0,     0,     0,   259,   257,   258,   256,     0,
       0,     0,     0,   264,     0,     0,   259,     0,     0,   257,
     258,     0,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,   258,   260,   261,   256,     0,     0,
     262,     0,   820,   263,     0,     0,   260,   261,     0,     0,
     259,   262,     0,   842,   263,     0,     0,     0,   264,     0,
       0,     0,   257,   258,   259,     0,     0,     0,   266,   264,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   266,
     260,   261,     0,     0,     0,   262,     0,   871,   263,     0,
       0,   259,     0,     0,   260,   261,     0,     0,     0,   262,
       0,   873,   263,   264,     0,     0,     0,   260,   261,     0,
       0,     0,   262,   266,   900,   263,     0,   264,     0,     0,
     259,   260,   261,     0,     0,     0,   262,   266,   905,   263,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,   264,     0,     0,     0,     0,     0,
     260,   261,     0,     0,   266,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   135,   136,   137,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,   149,
     150
};

static const yytype_int16 yycheck[] =
{
      13,   273,   230,   109,    81,   269,   271,   286,   385,   160,
     387,   446,   389,   310,     7,   519,   307,    73,   538,   584,
     540,    21,   542,    32,     0,   304,    50,   592,    20,    21,
       6,    14,    15,    19,   109,    19,    33,   606,   114,   128,
      50,   119,    47,   137,   144,    19,   122,    34,   145,    18,
     137,   555,   556,    29,    54,    31,   127,    33,   137,    35,
     354,    74,    14,    15,    40,   137,   176,   137,   137,   145,
     137,    47,   137,   114,   137,   175,   370,    53,   175,   151,
     137,   113,   171,   177,   116,   117,   147,   116,   117,   144,
     177,   170,   612,   470,   131,    71,   109,   175,   174,   139,
     140,   144,   174,   144,   145,   176,   147,   177,   177,   150,
     177,   174,   177,   105,   106,   176,    92,    32,   105,   106,
     175,   113,   228,   115,   116,   117,   118,   137,   171,   121,
     281,   410,   137,   171,   166,   128,   168,   175,   417,   168,
     137,   150,    57,    58,   157,   714,   144,   712,   131,   152,
     137,   175,   135,   228,   137,   138,   156,   436,   144,   168,
     144,    59,   268,   155,   156,   157,   166,   223,   137,   178,
     144,   144,   175,   171,   166,   168,   168,   174,   284,   131,
     471,   147,   114,   135,   177,   137,   138,   170,   114,   114,
     122,   144,   168,   268,   144,   137,   122,   122,   171,   175,
     115,   144,   308,   309,   708,   175,   152,   177,   174,   284,
     144,   144,   289,   145,   144,   228,   720,   144,   171,   145,
     145,   169,   787,   137,   301,   175,   145,   175,   171,   175,
     145,   146,   797,   308,   309,   150,   169,   171,   153,   169,
     744,   152,   169,   137,   147,    20,    21,   147,   144,   152,
     529,   147,   152,   168,   150,   268,   144,    11,     4,     5,
     175,     7,   152,   178,   175,   147,   372,   170,    22,    23,
     170,   284,   378,   379,   380,   381,   147,   712,   384,   551,
     386,   152,   388,   803,   390,   175,   721,   578,   144,   172,
     173,   137,   443,   176,   445,   308,   309,   372,   595,   170,
      43,    44,   408,   378,   379,   380,   381,   137,   321,   384,
     175,   386,   177,   388,   137,   174,   147,   137,   469,    20,
      21,   152,   139,   140,   141,   170,   137,   176,   131,   435,
     105,   106,   137,   408,   176,   412,   137,   137,   113,   170,
     115,   116,   117,   118,   176,   137,   121,   145,   425,    34,
      34,    20,    21,   147,   731,   170,     1,   176,   139,   372,
     435,    54,   176,   137,   137,   378,   379,   380,   381,   147,
     176,   384,   151,   386,   151,   388,   175,   390,   153,   154,
     155,   156,   157,   151,   151,   462,    66,   170,   151,   151,
      70,   166,   671,   168,   151,   408,   151,   147,   151,   151,
     137,    32,   667,   416,   105,   106,   175,    87,    88,    89,
      90,   175,   113,   137,   115,   116,   117,   118,   128,   170,
     121,   170,   435,   151,   151,   170,   690,   175,   151,   131,
     151,   173,    36,    19,   103,   104,   175,   175,   107,   175,
     175,   110,   111,   112,   113,   458,   115,   116,   117,   118,
     175,   168,   121,   171,   123,   124,   170,   169,   137,   119,
     566,   567,   169,   569,    13,   166,   147,   168,   131,   173,
     176,    39,    12,   137,    20,    21,   137,   146,   137,   148,
     149,   150,   151,   152,    24,    32,   563,   156,   137,     7,
      30,   566,   567,    43,   569,   169,   144,   166,   175,   168,
     144,   177,   139,   139,   732,   137,    46,   137,   151,     1,
      57,    58,   170,   137,   152,   137,   151,   789,   152,   152,
      60,   137,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   566,   567,   170,   569,   169,   169,   105,
     106,   169,   169,   177,    32,   139,   169,   113,   115,   675,
     116,   117,   118,   169,   169,   121,   137,   171,   169,   169,
     686,   152,   688,   175,   177,   151,   169,   152,   169,    57,
      58,    50,   169,   177,   151,   171,   169,   137,   145,   146,
     675,   171,   169,   150,    55,    98,   153,   171,    58,   450,
     844,   686,   240,    56,    74,   817,   754,     1,   665,   604,
     166,   168,   168,   170,   730,   275,   370,    68,   688,   469,
     391,   178,   525,   230,   174,   173,   383,   284,   719,   446,
     865,    -1,    -1,    -1,    -1,    -1,   243,   115,    20,    21,
      -1,    -1,    -1,    -1,    -1,   730,   733,    -1,    -1,    -1,
      -1,    -1,   675,    -1,    -1,   742,    -1,   264,    -1,    -1,
      -1,    20,    21,   686,    -1,   688,    -1,   145,   146,    -1,
      -1,   278,   150,    -1,   152,   153,    -1,    -1,    -1,   286,
      -1,    -1,    -1,   290,    -1,   292,    -1,    -1,   775,    -1,
     168,    -1,    -1,   300,    -1,    -1,   812,   304,   305,   306,
     178,    -1,    -1,   310,    -1,    -1,    -1,   730,    -1,   825,
      -1,   318,   319,   320,    -1,   322,   323,   324,    -1,   806,
      -1,   103,   104,   105,   106,    -1,   333,   812,    -1,    -1,
      -1,   113,    -1,   115,   116,   117,   118,    -1,    -1,   121,
     825,   123,   124,    32,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,   124,    -1,    -1,    57,    58,
     886,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,   901,   168,    -1,    -1,   812,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   886,   825,   410,    -1,    -1,    -1,   166,    -1,   168,
     417,    -1,    -1,    -1,    -1,    -1,   901,   424,    -1,    -1,
      -1,   428,    -1,    -1,    -1,   114,   115,    -1,    -1,   436,
      -1,    -1,    -1,   122,    -1,    -1,    -1,   444,    -1,   446,
      -1,    -1,    -1,    -1,   867,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,
     883,   150,    -1,   886,   153,    -1,    -1,    -1,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   901,   168,
     487,   488,   489,   490,   491,   492,   175,   494,   495,   178,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
      -1,   518,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   529,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     557,    -1,    -1,   560,   561,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,   580,   581,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,   591,    -1,    45,   594,   595,    48,
      -1,   598,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,   634,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,   646,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    20,    21,   114,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,   671,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,    32,    -1,    -1,   692,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,   155,    -1,   157,    -1,
      -1,   160,   161,   710,   711,    -1,    -1,    57,    58,    -1,
     717,   170,   719,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   732,    -1,    -1,    -1,   736,
      -1,    -1,    -1,   740,    -1,    -1,   103,   104,   105,   106,
     107,    -1,   749,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   783,    -1,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,   166,
     150,   168,   809,   153,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,   826,
      -1,   828,   829,   830,    -1,   175,   833,    -1,   178,   836,
     837,    -1,   839,    -1,    -1,    -1,    -1,     1,    -1,   846,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,   876,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,    -1,
      -1,    45,   889,    47,    48,    49,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
     154,   155,    -1,   157,    -1,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,
     174,   175,   176,   177,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    -1,    41,    42,    -1,    -1,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,   155,    -1,
     157,    -1,    -1,   160,   161,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,   170,    -1,   172,   173,   174,   175,   176,
     177,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    50,    -1,    52,    -1,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,
     155,    -1,   157,    -1,    -1,   160,   161,    -1,    14,    15,
      16,    17,    -1,   168,    -1,   170,    -1,   172,   173,   174,
      26,   176,    -1,   178,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    57,    58,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,   157,    -1,    -1,   160,   161,    -1,    14,    15,    16,
      17,    -1,   168,    -1,   170,    -1,   172,   173,   174,    26,
     176,    -1,   178,    -1,    32,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    57,
      58,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,
      -1,    -1,   150,   150,   152,   153,   153,   154,   155,    -1,
     157,    -1,    -1,   160,   161,    -1,    14,    15,    16,    17,
     168,   168,    -1,   170,    -1,   172,   173,   174,    26,   176,
     178,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,   153,   154,   155,    -1,   157,
      -1,    26,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    38,   172,   173,   174,    42,   176,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,
     155,    -1,   157,    -1,    -1,   160,   161,    -1,    14,    15,
      16,    17,    18,   168,    -1,   170,   171,   172,   173,   174,
      26,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,   151,    -1,   153,   154,   155,
      -1,   157,    -1,    26,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    38,   172,   173,   174,    42,
     176,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
     153,   154,   155,    -1,   157,    -1,    26,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,   170,    38,   172,
     173,   174,    42,   176,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,   153,   154,   155,    -1,   157,    -1,    26,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    38,   172,   173,   174,    42,   176,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,   153,   154,   155,    -1,
     157,    -1,    26,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    38,   172,   173,   174,    42,   176,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   153,
     154,   155,    -1,   157,    -1,    26,   160,   161,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    38,   172,   173,
     174,    42,   176,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      32,   145,   146,    -1,    -1,    -1,   150,    -1,   152,   153,
      -1,    -1,   153,   154,   155,    -1,   157,    -1,    -1,   160,
     161,    -1,    -1,    -1,   168,    57,    58,    -1,    -1,   170,
      -1,   172,   173,   174,   178,   176,    14,    15,    16,    17,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   145,   146,    93,    -1,    -1,   150,    -1,
     152,   153,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,   153,   154,   155,    -1,   157,
      -1,    26,   160,   161,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   170,    38,   172,   173,   174,    42,   176,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    57,    58,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,    -1,    -1,   150,    -1,   152,   153,   153,   154,
     155,    -1,   157,    -1,    -1,   160,   161,    -1,    14,    15,
      16,    17,   168,    -1,    -1,   170,   171,   172,   173,   174,
      26,   176,   178,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      57,    58,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,   145,   146,
      -1,    -1,    -1,   150,    -1,   152,   153,   153,   154,   155,
      -1,   157,    -1,    -1,   160,   161,    -1,    14,    15,    16,
      17,   168,    -1,    -1,   170,   171,   172,   173,   174,    26,
     176,   178,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    57,
      58,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,   153,   153,   154,   155,    -1,
     157,    -1,    -1,   160,   161,    -1,    14,    15,    16,    17,
     168,    -1,    -1,   170,   171,   172,   173,   174,    26,   176,
     178,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    57,    58,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,
      -1,   150,    -1,   152,   153,   153,   154,   155,    -1,   157,
      -1,    -1,   160,   161,    -1,    14,    15,    16,    17,   168,
      -1,    -1,   170,   171,   172,   173,   174,    26,   176,   178,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    57,    58,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,    -1,
     150,    -1,   152,   153,   153,   154,   155,    -1,   157,    -1,
      -1,   160,   161,    -1,    14,    15,    16,    17,   168,    -1,
      -1,   170,   171,   172,   173,   174,    26,   176,   178,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    57,    58,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,   145,   146,    -1,    -1,    -1,   150,
      -1,   152,   153,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,    -1,    14,    15,    16,    17,   168,    -1,    -1,
     170,   171,   172,   173,   174,    26,   176,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   153,   154,   155,    -1,   157,    -1,    26,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      38,   172,   173,   174,    42,   176,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   153,   154,   155,    -1,   157,
      -1,    26,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    38,   172,   173,   174,    42,   176,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,   153,   154,
     155,    -1,   157,    -1,    26,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    38,   172,   173,   174,
      42,   176,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   153,   154,   155,    24,   157,    -1,    -1,   160,   161,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
     172,   173,   174,    -1,   176,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,   174,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    20,    21,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   168,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   166,
      -1,   168,   103,   104,   105,   106,   107,    -1,   175,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,   103,   104,   105,   106,   107,    -1,   175,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,   171,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,    -1,
      -1,   171,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,
     171,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,   171,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,   168,    -1,    -1,   171,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   168,    -1,    -1,   171,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,    -1,    -1,   171,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    60,   137,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    20,    21,    -1,    -1,
      -1,   174,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
     137,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    20,    21,    -1,    -1,    -1,   174,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   166,    -1,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   166,    -1,
     168,   169,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,   125,
     126,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    20,    21,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,   168,   103,   104,   105,   106,
     107,    20,    21,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,   121,    32,   123,   124,
      -1,    -1,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,   168,    57,    58,    -1,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    32,   168,   103,   104,   105,   106,   107,    -1,
      -1,   110,    -1,    32,   113,    -1,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,   124,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,
     145,   146,    57,    58,    32,   150,    -1,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    57,    58,    32,    -1,
      -1,    -1,    -1,   168,    -1,    -1,   115,    -1,    -1,    57,
      58,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,   145,   146,    32,    -1,    -1,
     150,    -1,   152,   153,    -1,    -1,   145,   146,    -1,    -1,
     115,   150,    -1,   152,   153,    -1,    -1,    -1,   168,    -1,
      -1,    -1,    57,    58,   115,    -1,    -1,    -1,   178,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,   178,
     145,   146,    -1,    -1,    -1,   150,    -1,   152,   153,    -1,
      -1,   115,    -1,    -1,   145,   146,    -1,    -1,    -1,   150,
      -1,   152,   153,   168,    -1,    -1,    -1,   145,   146,    -1,
      -1,    -1,   150,   178,   152,   153,    -1,   168,    -1,    -1,
     115,   145,   146,    -1,    -1,    -1,   150,   178,   152,   153,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,
     145,   146,    -1,    -1,   178,   150,    -1,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   180,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    71,    92,   168,   175,   181,   189,   190,   194,   214,
     218,   234,   288,   293,   294,   298,   334,   336,   137,   209,
     210,   131,   195,   196,   137,   191,   192,   137,   176,   137,
     337,   335,    33,   137,   174,   211,   212,   213,   225,     4,
       5,     7,   297,    59,   287,   145,   144,   147,   144,    21,
      54,   156,   166,   193,   147,   176,   137,   291,   292,   137,
     137,   174,   137,   170,   144,   169,    34,   105,   106,   137,
     217,   219,   220,   137,   296,   176,   289,    14,    15,   131,
     135,   137,   138,   170,   183,   209,   131,   196,   137,   137,
     137,    66,    70,    87,    88,    89,    90,   302,   290,   145,
     177,   291,   176,   176,   137,   210,   212,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   123,   124,   125,   126,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   160,
     161,    34,    34,   170,   215,   176,   221,   147,   295,   299,
     286,     1,   139,   182,    14,    15,   131,   135,   137,   183,
     207,   208,   193,   176,   137,   177,    24,    30,    46,    60,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,   225,   301,   303,   304,   306,   309,   137,   305,   137,
     279,   280,   171,   128,   171,   270,   275,   276,   147,   216,
     223,   225,   176,   137,   177,   285,   300,   175,   139,   140,
     144,   171,   290,   145,   175,   151,   151,   151,   151,   170,
     151,   151,   151,   151,   151,   151,    32,    57,    58,   115,
     145,   146,   150,   153,   168,   175,   178,   175,   147,   175,
     210,    47,   282,   300,   171,   175,   309,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    25,    26,    27,    28,
      37,    38,    41,    42,    45,    48,    49,    51,    52,    55,
      56,    61,    71,    93,    94,   105,   106,   120,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   138,   153,   154,
     155,   157,   160,   161,   170,   172,   173,   175,   177,   185,
     187,   197,   198,   201,   202,   203,   204,   205,   206,   221,
     222,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     234,   235,   236,   243,   252,   253,   254,   255,   256,   257,
     258,   259,   264,   267,   301,   338,   342,   347,   349,   351,
     273,   144,   147,   150,   284,   207,   177,   267,   312,   314,
     316,   310,   137,   307,   318,   320,   322,   324,   326,   328,
     330,   332,    32,    32,   150,   168,   178,   169,   267,   137,
     177,   309,   177,   279,   169,   282,   114,   145,   147,   150,
     281,   275,   267,   300,   309,   338,   170,   114,   175,   224,
     255,   267,   221,   267,   170,   267,   151,   151,   170,   175,
     151,   131,    55,   267,   221,   151,   114,   224,   267,   267,
     267,   173,   253,    12,   309,    12,   309,   267,   344,   348,
     184,   267,   267,   267,   225,   267,   267,   267,   173,    36,
     175,   175,   267,   137,   170,   175,   175,   175,    19,   233,
     214,   119,   175,    20,    21,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     122,   123,   124,   125,   126,   145,   146,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   166,   168,   170,
     177,   214,   270,   272,   137,   309,   114,   122,   145,   283,
     175,   309,   309,   309,   309,   171,   305,   309,   215,   309,
     215,   309,   215,   137,   277,   278,   309,   280,   169,   169,
     169,   309,   224,   267,   221,    19,   170,   225,   224,   267,
     114,   145,   175,    13,   267,   221,   237,   239,   267,   241,
     147,   131,   309,   224,   267,   175,   173,   300,    50,   137,
     153,   170,   267,   339,   340,   341,   343,   344,   345,   346,
     300,   170,   340,   346,   127,   175,   177,   140,   141,   182,
     188,   171,   151,   225,   176,   221,   168,   171,   250,   267,
      39,   285,   215,   253,    18,   137,   137,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   137,   120,   267,   267,   137,   267,   267,
     267,   267,   267,   267,   267,   267,   120,   267,    21,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     137,   265,   267,   171,   250,     7,    43,   271,   175,   145,
     175,   283,   224,   267,   152,   175,   152,   152,   152,   175,
     152,   216,   152,   216,   152,   216,   147,   152,   175,   152,
     175,   145,   281,   250,   171,   250,    18,   151,   267,   175,
     221,   171,   309,   309,   171,   309,   152,   175,    19,   253,
     122,   281,   171,   144,   175,   341,   169,   144,   169,   175,
      19,   171,   341,   177,   267,   344,   177,   267,   139,   186,
     260,   262,   223,    11,    22,    23,   199,   200,   137,   339,
     144,   171,   216,   151,   170,   267,   169,   267,   137,   147,
       1,   169,   171,    44,   271,   282,   224,   267,   175,   313,
     309,   317,   311,   308,   319,   152,   323,   152,   327,   152,
     309,   331,   277,   333,   267,   175,   171,   151,   137,   171,
     152,   152,   152,   170,   250,   267,   267,   340,   341,   137,
     339,   169,   169,   267,   169,   345,   250,   340,   177,   169,
     142,   139,   309,   215,   177,   221,   267,   169,   267,   127,
     221,   251,   268,   171,   250,   267,   266,   274,   220,   175,
     152,   321,   325,   329,   221,   244,   152,   175,   238,   240,
     242,   171,   267,   175,   341,   169,   122,   281,   169,   175,
     341,   169,   152,   216,   221,   171,   114,   267,   309,   171,
     219,   315,   309,    18,   267,   137,   267,   267,   267,   171,
     267,   169,   169,   267,   267,   267,   177,   261,   152,   200,
     267,   152,   175,   152,   151,   171,   152,   175,   350,   169,
     350,   169,   225,   263,   269,   245,   246,    18,   267,    50,
     169,   177,   225,   171,   309,   151,   171,   267,   169,   169,
     152,   248,   247,   309,   171,   152,   249,   171
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   179,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   183,   184,
     184,   184,   185,   186,   186,   188,   187,   189,   190,   191,
     191,   191,   192,   192,   193,   193,   194,   195,   195,   196,
     196,   197,   198,   198,   199,   199,   200,   200,   200,   201,
     201,   202,   203,   204,   205,   206,   207,   207,   207,   207,
     207,   207,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   211,   211,   212,   212,   213,   213,
     214,   214,   215,   215,   215,   216,   216,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   218,   219,   220,   221,   221,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   223,   223,
     223,   224,   224,   225,   225,   225,   226,   227,   227,   227,
     227,   228,   229,   230,   230,   230,   230,   230,   231,   231,
     231,   231,   232,   233,   233,   234,   234,   235,   237,   238,
     236,   239,   240,   236,   241,   242,   236,   243,   243,   243,
     244,   245,   243,   246,   247,   243,   248,   249,   243,   250,
     250,   251,   251,   251,   252,   252,   252,   253,   253,   254,
     254,   254,   254,   254,   254,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   256,   256,   257,   258,   258,   259,   260,   261,   259,
     262,   263,   259,   264,   265,   266,   264,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   268,   269,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   270,   270,   271,   271,   272,   273,   273,
     274,   273,   273,   275,   275,   276,   276,   277,   277,   278,
     278,   279,   280,   280,   281,   281,   282,   282,   282,   282,
     282,   282,   283,   283,   283,   284,   284,   285,   285,   285,
     285,   285,   286,   286,   287,   287,   288,   289,   288,   290,
     290,   290,   291,   292,   292,   293,   294,   294,   295,   295,
     296,   297,   297,   299,   298,   300,   300,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   302,   302,   302,   302,   302,   302,   303,   304,
     304,   305,   305,   307,   308,   306,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   310,   311,   309,   309,   312,   313,   309,
     314,   315,   309,   316,   317,   309,   309,   318,   319,   309,
     320,   321,   309,   309,   322,   323,   309,   324,   325,   309,
     309,   326,   327,   309,   328,   329,   309,   330,   331,   309,
     332,   333,   309,   335,   334,   337,   336,   338,   338,   338,
     338,   339,   339,   339,   339,   340,   340,   341,   341,   342,
     342,   342,   342,   342,   342,   343,   343,   344,   345,   345,
     346,   346,   347,   347,   348,   348,   349,   350,   350,   351,
     351
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
       4,     3,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     8,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     0,     7,     0,     7,     1,     1,     1,
       1,     3,     3,     5,     5,     1,     3,     0,     2,     6,
       5,     7,     8,     6,     8,     1,     3,     3,     1,     1,
       1,     3,     5,     5,     1,     3,     4,     0,     3,    10,
      10
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
    case 137: /* "name"  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3326 "ds_parser.cpp"
        break;

    case 182: /* character_sequence  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3332 "ds_parser.cpp"
        break;

    case 183: /* string_constant  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3338 "ds_parser.cpp"
        break;

    case 184: /* string_builder_body  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3344 "ds_parser.cpp"
        break;

    case 185: /* string_builder  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3350 "ds_parser.cpp"
        break;

    case 187: /* expr_reader  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3356 "ds_parser.cpp"
        break;

    case 191: /* require_module_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3362 "ds_parser.cpp"
        break;

    case 197: /* expression_label  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3368 "ds_parser.cpp"
        break;

    case 198: /* expression_goto  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3374 "ds_parser.cpp"
        break;

    case 200: /* expression_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3380 "ds_parser.cpp"
        break;

    case 202: /* expression_if_then_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3386 "ds_parser.cpp"
        break;

    case 203: /* expression_for_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3392 "ds_parser.cpp"
        break;

    case 204: /* expression_unsafe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3398 "ds_parser.cpp"
        break;

    case 205: /* expression_while_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3404 "ds_parser.cpp"
        break;

    case 206: /* expression_with  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3410 "ds_parser.cpp"
        break;

    case 207: /* annotation_argument_value  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3416 "ds_parser.cpp"
        break;

    case 208: /* annotation_argument_value_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3422 "ds_parser.cpp"
        break;

    case 209: /* annotation_argument  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3428 "ds_parser.cpp"
        break;

    case 210: /* annotation_argument_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3434 "ds_parser.cpp"
        break;

    case 211: /* annotation_declaration_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3440 "ds_parser.cpp"
        break;

    case 212: /* annotation_declaration  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3446 "ds_parser.cpp"
        break;

    case 213: /* annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3452 "ds_parser.cpp"
        break;

    case 214: /* optional_annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3458 "ds_parser.cpp"
        break;

    case 215: /* optional_function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3464 "ds_parser.cpp"
        break;

    case 216: /* optional_function_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3470 "ds_parser.cpp"
        break;

    case 217: /* function_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3476 "ds_parser.cpp"
        break;

    case 221: /* expression_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3482 "ds_parser.cpp"
        break;

    case 222: /* expression_any  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3488 "ds_parser.cpp"
        break;

    case 223: /* expressions  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3494 "ds_parser.cpp"
        break;

    case 224: /* expr_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3500 "ds_parser.cpp"
        break;

    case 225: /* name_in_namespace  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3506 "ds_parser.cpp"
        break;

    case 226: /* expression_delete  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3512 "ds_parser.cpp"
        break;

    case 227: /* expr_new  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3518 "ds_parser.cpp"
        break;

    case 228: /* expression_break  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3524 "ds_parser.cpp"
        break;

    case 229: /* expression_continue  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3530 "ds_parser.cpp"
        break;

    case 230: /* expression_return  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3536 "ds_parser.cpp"
        break;

    case 231: /* expression_yield  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3542 "ds_parser.cpp"
        break;

    case 232: /* expression_try_catch  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3548 "ds_parser.cpp"
        break;

    case 235: /* expression_let  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3554 "ds_parser.cpp"
        break;

    case 236: /* expr_cast  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3560 "ds_parser.cpp"
        break;

    case 243: /* expr_type_info  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3566 "ds_parser.cpp"
        break;

    case 250: /* expr_list  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3572 "ds_parser.cpp"
        break;

    case 251: /* block_or_simple_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3578 "ds_parser.cpp"
        break;

    case 253: /* expr_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3584 "ds_parser.cpp"
        break;

    case 254: /* expr_numeric_const  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3590 "ds_parser.cpp"
        break;

    case 255: /* expr_assign  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3596 "ds_parser.cpp"
        break;

    case 256: /* expr_assign_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3602 "ds_parser.cpp"
        break;

    case 257: /* expr_named_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3608 "ds_parser.cpp"
        break;

    case 258: /* expr_method_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3614 "ds_parser.cpp"
        break;

    case 259: /* func_addr_expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3620 "ds_parser.cpp"
        break;

    case 264: /* expr_field  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3626 "ds_parser.cpp"
        break;

    case 267: /* expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3632 "ds_parser.cpp"
        break;

    case 270: /* optional_field_annotation  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3638 "ds_parser.cpp"
        break;

    case 272: /* structure_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3644 "ds_parser.cpp"
        break;

    case 273: /* struct_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3650 "ds_parser.cpp"
        break;

    case 275: /* function_argument_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3656 "ds_parser.cpp"
        break;

    case 276: /* function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3662 "ds_parser.cpp"
        break;

    case 277: /* tuple_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3668 "ds_parser.cpp"
        break;

    case 278: /* tuple_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3674 "ds_parser.cpp"
        break;

    case 279: /* variant_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3680 "ds_parser.cpp"
        break;

    case 280: /* variant_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3686 "ds_parser.cpp"
        break;

    case 282: /* variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3692 "ds_parser.cpp"
        break;

    case 285: /* let_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3698 "ds_parser.cpp"
        break;

    case 286: /* global_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3704 "ds_parser.cpp"
        break;

    case 290: /* enum_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3710 "ds_parser.cpp"
        break;

    case 295: /* optional_structure_parent  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3716 "ds_parser.cpp"
        break;

    case 300: /* variable_name_with_pos_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3722 "ds_parser.cpp"
        break;

    case 303: /* structure_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3728 "ds_parser.cpp"
        break;

    case 304: /* auto_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3734 "ds_parser.cpp"
        break;

    case 305: /* bitfield_bits  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3740 "ds_parser.cpp"
        break;

    case 306: /* bitfield_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3746 "ds_parser.cpp"
        break;

    case 309: /* type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3752 "ds_parser.cpp"
        break;

    case 338: /* make_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3758 "ds_parser.cpp"
        break;

    case 339: /* make_struct_fields  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3764 "ds_parser.cpp"
        break;

    case 340: /* make_struct_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3770 "ds_parser.cpp"
        break;

    case 341: /* optional_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3776 "ds_parser.cpp"
        break;

    case 342: /* make_struct_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3782 "ds_parser.cpp"
        break;

    case 343: /* make_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3788 "ds_parser.cpp"
        break;

    case 344: /* make_map_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3794 "ds_parser.cpp"
        break;

    case 345: /* make_any_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3800 "ds_parser.cpp"
        break;

    case 346: /* make_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3806 "ds_parser.cpp"
        break;

    case 347: /* make_dim_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3812 "ds_parser.cpp"
        break;

    case 348: /* make_table  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3818 "ds_parser.cpp"
        break;

    case 349: /* make_table_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3824 "ds_parser.cpp"
        break;

    case 350: /* array_comprehension_where  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3830 "ds_parser.cpp"
        break;

    case 351: /* array_comprehension  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3836 "ds_parser.cpp"
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
#line 440 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4136 "ds_parser.cpp"
    break;

  case 16:
#line 452 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4142 "ds_parser.cpp"
    break;

  case 17:
#line 453 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4148 "ds_parser.cpp"
    break;

  case 18:
#line 457 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4154 "ds_parser.cpp"
    break;

  case 19:
#line 461 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4164 "ds_parser.cpp"
    break;

  case 20:
#line 466 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4178 "ds_parser.cpp"
    break;

  case 21:
#line 475 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4188 "ds_parser.cpp"
    break;

  case 22:
#line 483 "ds_parser.ypp"
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
#line 4206 "ds_parser.cpp"
    break;

  case 23:
#line 499 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4216 "ds_parser.cpp"
    break;

  case 24:
#line 504 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4226 "ds_parser.cpp"
    break;

  case 25:
#line 512 "ds_parser.ypp"
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
#line 4253 "ds_parser.cpp"
    break;

  case 26:
#line 533 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4265 "ds_parser.cpp"
    break;

  case 27:
#line 542 "ds_parser.ypp"
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
#line 4289 "ds_parser.cpp"
    break;

  case 29:
#line 568 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4297 "ds_parser.cpp"
    break;

  case 30:
#line 571 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4308 "ds_parser.cpp"
    break;

  case 31:
#line 577 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4319 "ds_parser.cpp"
    break;

  case 32:
#line 586 "ds_parser.ypp"
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
#line 4345 "ds_parser.cpp"
    break;

  case 33:
#line 607 "ds_parser.ypp"
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
#line 4370 "ds_parser.cpp"
    break;

  case 34:
#line 630 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4376 "ds_parser.cpp"
    break;

  case 35:
#line 631 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4382 "ds_parser.cpp"
    break;

  case 39:
#line 644 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4390 "ds_parser.cpp"
    break;

  case 40:
#line 647 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4398 "ds_parser.cpp"
    break;

  case 41:
#line 653 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4406 "ds_parser.cpp"
    break;

  case 42:
#line 659 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4414 "ds_parser.cpp"
    break;

  case 43:
#line 662 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4422 "ds_parser.cpp"
    break;

  case 44:
#line 668 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4428 "ds_parser.cpp"
    break;

  case 45:
#line 669 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4434 "ds_parser.cpp"
    break;

  case 46:
#line 673 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4440 "ds_parser.cpp"
    break;

  case 47:
#line 674 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4446 "ds_parser.cpp"
    break;

  case 48:
#line 675 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4457 "ds_parser.cpp"
    break;

  case 49:
#line 684 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4463 "ds_parser.cpp"
    break;

  case 50:
#line 685 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4469 "ds_parser.cpp"
    break;

  case 51:
#line 689 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4480 "ds_parser.cpp"
    break;

  case 52:
#line 698 "ds_parser.ypp"
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
#line 4498 "ds_parser.cpp"
    break;

  case 53:
#line 714 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4508 "ds_parser.cpp"
    break;

  case 54:
#line 722 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4520 "ds_parser.cpp"
    break;

  case 55:
#line 732 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4531 "ds_parser.cpp"
    break;

  case 56:
#line 741 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4537 "ds_parser.cpp"
    break;

  case 57:
#line 742 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4543 "ds_parser.cpp"
    break;

  case 58:
#line 743 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4549 "ds_parser.cpp"
    break;

  case 59:
#line 744 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4555 "ds_parser.cpp"
    break;

  case 60:
#line 745 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4561 "ds_parser.cpp"
    break;

  case 61:
#line 746 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4567 "ds_parser.cpp"
    break;

  case 62:
#line 750 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4577 "ds_parser.cpp"
    break;

  case 63:
#line 755 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4587 "ds_parser.cpp"
    break;

  case 64:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4593 "ds_parser.cpp"
    break;

  case 65:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4599 "ds_parser.cpp"
    break;

  case 66:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4605 "ds_parser.cpp"
    break;

  case 67:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4611 "ds_parser.cpp"
    break;

  case 68:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4617 "ds_parser.cpp"
    break;

  case 69:
#line 768 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4623 "ds_parser.cpp"
    break;

  case 70:
#line 769 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4629 "ds_parser.cpp"
    break;

  case 71:
#line 770 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4637 "ds_parser.cpp"
    break;

  case 72:
#line 776 "ds_parser.ypp"
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
#line 4655 "ds_parser.cpp"
    break;

  case 73:
#line 789 "ds_parser.ypp"
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
#line 4673 "ds_parser.cpp"
    break;

  case 74:
#line 805 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4679 "ds_parser.cpp"
    break;

  case 75:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4685 "ds_parser.cpp"
    break;

  case 76:
#line 810 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4697 "ds_parser.cpp"
    break;

  case 77:
#line 817 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4711 "ds_parser.cpp"
    break;

  case 78:
#line 829 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4720 "ds_parser.cpp"
    break;

  case 79:
#line 833 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4729 "ds_parser.cpp"
    break;

  case 80:
#line 840 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4735 "ds_parser.cpp"
    break;

  case 81:
#line 841 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4741 "ds_parser.cpp"
    break;

  case 82:
#line 845 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4747 "ds_parser.cpp"
    break;

  case 83:
#line 846 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4753 "ds_parser.cpp"
    break;

  case 84:
#line 847 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4759 "ds_parser.cpp"
    break;

  case 85:
#line 851 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4769 "ds_parser.cpp"
    break;

  case 86:
#line 856 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4778 "ds_parser.cpp"
    break;

  case 87:
#line 863 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4787 "ds_parser.cpp"
    break;

  case 88:
#line 867 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4793 "ds_parser.cpp"
    break;

  case 89:
#line 868 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4799 "ds_parser.cpp"
    break;

  case 90:
#line 869 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4805 "ds_parser.cpp"
    break;

  case 91:
#line 870 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4811 "ds_parser.cpp"
    break;

  case 92:
#line 871 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4817 "ds_parser.cpp"
    break;

  case 93:
#line 872 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4823 "ds_parser.cpp"
    break;

  case 94:
#line 873 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4829 "ds_parser.cpp"
    break;

  case 95:
#line 874 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4835 "ds_parser.cpp"
    break;

  case 96:
#line 875 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4841 "ds_parser.cpp"
    break;

  case 97:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4847 "ds_parser.cpp"
    break;

  case 98:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4853 "ds_parser.cpp"
    break;

  case 99:
#line 878 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4859 "ds_parser.cpp"
    break;

  case 100:
#line 879 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4865 "ds_parser.cpp"
    break;

  case 101:
#line 880 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4871 "ds_parser.cpp"
    break;

  case 102:
#line 881 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4877 "ds_parser.cpp"
    break;

  case 103:
#line 882 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4883 "ds_parser.cpp"
    break;

  case 104:
#line 883 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4889 "ds_parser.cpp"
    break;

  case 105:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4895 "ds_parser.cpp"
    break;

  case 106:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4901 "ds_parser.cpp"
    break;

  case 107:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4907 "ds_parser.cpp"
    break;

  case 108:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4913 "ds_parser.cpp"
    break;

  case 109:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4919 "ds_parser.cpp"
    break;

  case 110:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4925 "ds_parser.cpp"
    break;

  case 111:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4931 "ds_parser.cpp"
    break;

  case 112:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4937 "ds_parser.cpp"
    break;

  case 113:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4943 "ds_parser.cpp"
    break;

  case 114:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4949 "ds_parser.cpp"
    break;

  case 115:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4955 "ds_parser.cpp"
    break;

  case 116:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4961 "ds_parser.cpp"
    break;

  case 117:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4967 "ds_parser.cpp"
    break;

  case 118:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4973 "ds_parser.cpp"
    break;

  case 119:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4979 "ds_parser.cpp"
    break;

  case 120:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4985 "ds_parser.cpp"
    break;

  case 121:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4991 "ds_parser.cpp"
    break;

  case 122:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4997 "ds_parser.cpp"
    break;

  case 123:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5003 "ds_parser.cpp"
    break;

  case 124:
#line 906 "ds_parser.ypp"
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
#line 5026 "ds_parser.cpp"
    break;

  case 125:
#line 927 "ds_parser.ypp"
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
#line 5066 "ds_parser.cpp"
    break;

  case 126:
#line 965 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5075 "ds_parser.cpp"
    break;

  case 127:
#line 972 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5084 "ds_parser.cpp"
    break;

  case 128:
#line 976 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5097 "ds_parser.cpp"
    break;

  case 129:
#line 987 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5103 "ds_parser.cpp"
    break;

  case 130:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5109 "ds_parser.cpp"
    break;

  case 131:
#line 989 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5115 "ds_parser.cpp"
    break;

  case 132:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5121 "ds_parser.cpp"
    break;

  case 133:
#line 991 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5127 "ds_parser.cpp"
    break;

  case 134:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5133 "ds_parser.cpp"
    break;

  case 135:
#line 993 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5139 "ds_parser.cpp"
    break;

  case 136:
#line 994 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5145 "ds_parser.cpp"
    break;

  case 137:
#line 995 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5151 "ds_parser.cpp"
    break;

  case 138:
#line 996 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5157 "ds_parser.cpp"
    break;

  case 139:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5163 "ds_parser.cpp"
    break;

  case 140:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5169 "ds_parser.cpp"
    break;

  case 141:
#line 999 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5175 "ds_parser.cpp"
    break;

  case 142:
#line 1000 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5181 "ds_parser.cpp"
    break;

  case 143:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5187 "ds_parser.cpp"
    break;

  case 144:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5193 "ds_parser.cpp"
    break;

  case 145:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5199 "ds_parser.cpp"
    break;

  case 146:
#line 1004 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5205 "ds_parser.cpp"
    break;

  case 147:
#line 1005 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5211 "ds_parser.cpp"
    break;

  case 148:
#line 1009 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5221 "ds_parser.cpp"
    break;

  case 149:
#line 1014 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5232 "ds_parser.cpp"
    break;

  case 150:
#line 1020 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5240 "ds_parser.cpp"
    break;

  case 151:
#line 1026 "ds_parser.ypp"
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
#line 5266 "ds_parser.cpp"
    break;

  case 152:
#line 1047 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5274 "ds_parser.cpp"
    break;

  case 153:
#line 1053 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5280 "ds_parser.cpp"
    break;

  case 154:
#line 1054 "ds_parser.ypp"
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
#line 5296 "ds_parser.cpp"
    break;

  case 155:
#line 1065 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5302 "ds_parser.cpp"
    break;

  case 156:
#line 1069 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5310 "ds_parser.cpp"
    break;

  case 157:
#line 1075 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5318 "ds_parser.cpp"
    break;

  case 158:
#line 1078 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5326 "ds_parser.cpp"
    break;

  case 159:
#line 1081 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5335 "ds_parser.cpp"
    break;

  case 160:
#line 1085 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5343 "ds_parser.cpp"
    break;

  case 161:
#line 1091 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5349 "ds_parser.cpp"
    break;

  case 162:
#line 1095 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5355 "ds_parser.cpp"
    break;

  case 163:
#line 1099 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5363 "ds_parser.cpp"
    break;

  case 164:
#line 1102 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5371 "ds_parser.cpp"
    break;

  case 165:
#line 1105 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5381 "ds_parser.cpp"
    break;

  case 166:
#line 1110 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5389 "ds_parser.cpp"
    break;

  case 167:
#line 1113 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5399 "ds_parser.cpp"
    break;

  case 168:
#line 1121 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5407 "ds_parser.cpp"
    break;

  case 169:
#line 1124 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5417 "ds_parser.cpp"
    break;

  case 170:
#line 1129 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5425 "ds_parser.cpp"
    break;

  case 171:
#line 1132 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5435 "ds_parser.cpp"
    break;

  case 172:
#line 1140 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5443 "ds_parser.cpp"
    break;

  case 173:
#line 1146 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5449 "ds_parser.cpp"
    break;

  case 174:
#line 1147 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5455 "ds_parser.cpp"
    break;

  case 175:
#line 1151 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5461 "ds_parser.cpp"
    break;

  case 176:
#line 1152 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5467 "ds_parser.cpp"
    break;

  case 177:
#line 1156 "ds_parser.ypp"
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
#line 5504 "ds_parser.cpp"
    break;

  case 178:
#line 1191 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5510 "ds_parser.cpp"
    break;

  case 179:
#line 1191 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5516 "ds_parser.cpp"
    break;

  case 180:
#line 1191 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5524 "ds_parser.cpp"
    break;

  case 181:
#line 1194 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5530 "ds_parser.cpp"
    break;

  case 182:
#line 1194 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5536 "ds_parser.cpp"
    break;

  case 183:
#line 1194 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5546 "ds_parser.cpp"
    break;

  case 184:
#line 1199 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5552 "ds_parser.cpp"
    break;

  case 185:
#line 1199 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5558 "ds_parser.cpp"
    break;

  case 186:
#line 1199 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5568 "ds_parser.cpp"
    break;

  case 187:
#line 1207 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5577 "ds_parser.cpp"
    break;

  case 188:
#line 1211 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5587 "ds_parser.cpp"
    break;

  case 189:
#line 1216 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5598 "ds_parser.cpp"
    break;

  case 190:
#line 1222 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5604 "ds_parser.cpp"
    break;

  case 191:
#line 1222 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5610 "ds_parser.cpp"
    break;

  case 192:
#line 1222 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5619 "ds_parser.cpp"
    break;

  case 193:
#line 1226 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5625 "ds_parser.cpp"
    break;

  case 194:
#line 1226 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5631 "ds_parser.cpp"
    break;

  case 195:
#line 1226 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5641 "ds_parser.cpp"
    break;

  case 196:
#line 1231 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5647 "ds_parser.cpp"
    break;

  case 197:
#line 1231 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5653 "ds_parser.cpp"
    break;

  case 198:
#line 1231 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5664 "ds_parser.cpp"
    break;

  case 199:
#line 1240 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5672 "ds_parser.cpp"
    break;

  case 200:
#line 1243 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5680 "ds_parser.cpp"
    break;

  case 201:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5686 "ds_parser.cpp"
    break;

  case 202:
#line 1250 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5698 "ds_parser.cpp"
    break;

  case 203:
#line 1257 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5711 "ds_parser.cpp"
    break;

  case 204:
#line 1268 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5717 "ds_parser.cpp"
    break;

  case 205:
#line 1269 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5723 "ds_parser.cpp"
    break;

  case 206:
#line 1270 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5729 "ds_parser.cpp"
    break;

  case 207:
#line 1274 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5739 "ds_parser.cpp"
    break;

  case 208:
#line 1280 "ds_parser.ypp"
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
#line 5795 "ds_parser.cpp"
    break;

  case 209:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5801 "ds_parser.cpp"
    break;

  case 210:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5807 "ds_parser.cpp"
    break;

  case 211:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5813 "ds_parser.cpp"
    break;

  case 212:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5819 "ds_parser.cpp"
    break;

  case 213:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5825 "ds_parser.cpp"
    break;

  case 214:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5831 "ds_parser.cpp"
    break;

  case 215:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5837 "ds_parser.cpp"
    break;

  case 216:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5843 "ds_parser.cpp"
    break;

  case 217:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5849 "ds_parser.cpp"
    break;

  case 218:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5855 "ds_parser.cpp"
    break;

  case 219:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5861 "ds_parser.cpp"
    break;

  case 220:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5867 "ds_parser.cpp"
    break;

  case 221:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5873 "ds_parser.cpp"
    break;

  case 222:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5879 "ds_parser.cpp"
    break;

  case 223:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5885 "ds_parser.cpp"
    break;

  case 224:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5891 "ds_parser.cpp"
    break;

  case 225:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5897 "ds_parser.cpp"
    break;

  case 226:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5903 "ds_parser.cpp"
    break;

  case 227:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5909 "ds_parser.cpp"
    break;

  case 228:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5915 "ds_parser.cpp"
    break;

  case 229:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5921 "ds_parser.cpp"
    break;

  case 230:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5927 "ds_parser.cpp"
    break;

  case 231:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5933 "ds_parser.cpp"
    break;

  case 232:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5939 "ds_parser.cpp"
    break;

  case 233:
#line 1367 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5951 "ds_parser.cpp"
    break;

  case 234:
#line 1378 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5961 "ds_parser.cpp"
    break;

  case 235:
#line 1383 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5973 "ds_parser.cpp"
    break;

  case 236:
#line 1393 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5982 "ds_parser.cpp"
    break;

  case 237:
#line 1397 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5988 "ds_parser.cpp"
    break;

  case 238:
#line 1397 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5994 "ds_parser.cpp"
    break;

  case 239:
#line 1397 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6006 "ds_parser.cpp"
    break;

  case 240:
#line 1404 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6012 "ds_parser.cpp"
    break;

  case 241:
#line 1404 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6018 "ds_parser.cpp"
    break;

  case 242:
#line 1404 "ds_parser.ypp"
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
#line 6034 "ds_parser.cpp"
    break;

  case 243:
#line 1418 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6043 "ds_parser.cpp"
    break;

  case 244:
#line 1422 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6049 "ds_parser.cpp"
    break;

  case 245:
#line 1422 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6055 "ds_parser.cpp"
    break;

  case 246:
#line 1422 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6065 "ds_parser.cpp"
    break;

  case 247:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6071 "ds_parser.cpp"
    break;

  case 248:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6077 "ds_parser.cpp"
    break;

  case 249:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6083 "ds_parser.cpp"
    break;

  case 250:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6089 "ds_parser.cpp"
    break;

  case 251:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6095 "ds_parser.cpp"
    break;

  case 252:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6101 "ds_parser.cpp"
    break;

  case 253:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6107 "ds_parser.cpp"
    break;

  case 254:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6113 "ds_parser.cpp"
    break;

  case 255:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6119 "ds_parser.cpp"
    break;

  case 256:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6125 "ds_parser.cpp"
    break;

  case 257:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6131 "ds_parser.cpp"
    break;

  case 258:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6137 "ds_parser.cpp"
    break;

  case 259:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6143 "ds_parser.cpp"
    break;

  case 260:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6149 "ds_parser.cpp"
    break;

  case 261:
#line 1444 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6155 "ds_parser.cpp"
    break;

  case 262:
#line 1445 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6161 "ds_parser.cpp"
    break;

  case 263:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6167 "ds_parser.cpp"
    break;

  case 264:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6173 "ds_parser.cpp"
    break;

  case 265:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6179 "ds_parser.cpp"
    break;

  case 266:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6185 "ds_parser.cpp"
    break;

  case 267:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6191 "ds_parser.cpp"
    break;

  case 268:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6197 "ds_parser.cpp"
    break;

  case 269:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6203 "ds_parser.cpp"
    break;

  case 270:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6209 "ds_parser.cpp"
    break;

  case 271:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6215 "ds_parser.cpp"
    break;

  case 272:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6221 "ds_parser.cpp"
    break;

  case 273:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6227 "ds_parser.cpp"
    break;

  case 274:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6233 "ds_parser.cpp"
    break;

  case 275:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6239 "ds_parser.cpp"
    break;

  case 276:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6245 "ds_parser.cpp"
    break;

  case 277:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6251 "ds_parser.cpp"
    break;

  case 278:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6257 "ds_parser.cpp"
    break;

  case 279:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6263 "ds_parser.cpp"
    break;

  case 280:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6269 "ds_parser.cpp"
    break;

  case 281:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6275 "ds_parser.cpp"
    break;

  case 282:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6281 "ds_parser.cpp"
    break;

  case 283:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6287 "ds_parser.cpp"
    break;

  case 284:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6293 "ds_parser.cpp"
    break;

  case 285:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6299 "ds_parser.cpp"
    break;

  case 286:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6305 "ds_parser.cpp"
    break;

  case 287:
#line 1470 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6314 "ds_parser.cpp"
    break;

  case 288:
#line 1474 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6323 "ds_parser.cpp"
    break;

  case 289:
#line 1478 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6331 "ds_parser.cpp"
    break;

  case 290:
#line 1481 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6339 "ds_parser.cpp"
    break;

  case 291:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6345 "ds_parser.cpp"
    break;

  case 292:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6351 "ds_parser.cpp"
    break;

  case 293:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6357 "ds_parser.cpp"
    break;

  case 294:
#line 1487 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6367 "ds_parser.cpp"
    break;

  case 295:
#line 1492 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6377 "ds_parser.cpp"
    break;

  case 296:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6383 "ds_parser.cpp"
    break;

  case 297:
#line 1498 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6391 "ds_parser.cpp"
    break;

  case 298:
#line 1501 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6397 "ds_parser.cpp"
    break;

  case 299:
#line 1501 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6403 "ds_parser.cpp"
    break;

  case 300:
#line 1501 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6411 "ds_parser.cpp"
    break;

  case 301:
#line 1504 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6420 "ds_parser.cpp"
    break;

  case 302:
#line 1508 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6429 "ds_parser.cpp"
    break;

  case 303:
#line 1512 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6438 "ds_parser.cpp"
    break;

  case 304:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6444 "ds_parser.cpp"
    break;

  case 305:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6450 "ds_parser.cpp"
    break;

  case 306:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6456 "ds_parser.cpp"
    break;

  case 307:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6462 "ds_parser.cpp"
    break;

  case 308:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6468 "ds_parser.cpp"
    break;

  case 309:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6474 "ds_parser.cpp"
    break;

  case 310:
#line 1522 "ds_parser.ypp"
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
#line 6489 "ds_parser.cpp"
    break;

  case 311:
#line 1532 "ds_parser.ypp"
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
#line 6504 "ds_parser.cpp"
    break;

  case 312:
#line 1542 "ds_parser.ypp"
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
#line 6561 "ds_parser.cpp"
    break;

  case 313:
#line 1597 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6567 "ds_parser.cpp"
    break;

  case 314:
#line 1598 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6573 "ds_parser.cpp"
    break;

  case 315:
#line 1602 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6579 "ds_parser.cpp"
    break;

  case 316:
#line 1603 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6585 "ds_parser.cpp"
    break;

  case 317:
#line 1607 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6595 "ds_parser.cpp"
    break;

  case 318:
#line 1615 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6603 "ds_parser.cpp"
    break;

  case 319:
#line 1618 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6612 "ds_parser.cpp"
    break;

  case 320:
#line 1623 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6618 "ds_parser.cpp"
    break;

  case 321:
#line 1623 "ds_parser.ypp"
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
#line 6667 "ds_parser.cpp"
    break;

  case 322:
#line 1667 "ds_parser.ypp"
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
            runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt((yylsp[-3])));
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[0].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 6727 "ds_parser.cpp"
    break;

  case 323:
#line 1725 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6737 "ds_parser.cpp"
    break;

  case 324:
#line 1730 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6747 "ds_parser.cpp"
    break;

  case 325:
#line 1738 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6753 "ds_parser.cpp"
    break;

  case 326:
#line 1739 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6759 "ds_parser.cpp"
    break;

  case 327:
#line 1743 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6767 "ds_parser.cpp"
    break;

  case 328:
#line 1746 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6778 "ds_parser.cpp"
    break;

  case 329:
#line 1755 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6784 "ds_parser.cpp"
    break;

  case 330:
#line 1756 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6790 "ds_parser.cpp"
    break;

  case 331:
#line 1760 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6801 "ds_parser.cpp"
    break;

  case 332:
#line 1769 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6807 "ds_parser.cpp"
    break;

  case 333:
#line 1770 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6813 "ds_parser.cpp"
    break;

  case 334:
#line 1775 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6819 "ds_parser.cpp"
    break;

  case 335:
#line 1776 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6825 "ds_parser.cpp"
    break;

  case 336:
#line 1780 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6836 "ds_parser.cpp"
    break;

  case 337:
#line 1786 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6847 "ds_parser.cpp"
    break;

  case 338:
#line 1792 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6855 "ds_parser.cpp"
    break;

  case 339:
#line 1795 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6864 "ds_parser.cpp"
    break;

  case 340:
#line 1799 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6875 "ds_parser.cpp"
    break;

  case 341:
#line 1805 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6886 "ds_parser.cpp"
    break;

  case 342:
#line 1814 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6892 "ds_parser.cpp"
    break;

  case 343:
#line 1815 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6898 "ds_parser.cpp"
    break;

  case 344:
#line 1816 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6904 "ds_parser.cpp"
    break;

  case 345:
#line 1820 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6910 "ds_parser.cpp"
    break;

  case 346:
#line 1821 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6916 "ds_parser.cpp"
    break;

  case 347:
#line 1825 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6924 "ds_parser.cpp"
    break;

  case 348:
#line 1828 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6934 "ds_parser.cpp"
    break;

  case 349:
#line 1833 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6944 "ds_parser.cpp"
    break;

  case 350:
#line 1838 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6957 "ds_parser.cpp"
    break;

  case 351:
#line 1846 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6970 "ds_parser.cpp"
    break;

  case 352:
#line 1857 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6978 "ds_parser.cpp"
    break;

  case 353:
#line 1860 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6987 "ds_parser.cpp"
    break;

  case 354:
#line 1867 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6993 "ds_parser.cpp"
    break;

  case 355:
#line 1868 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6999 "ds_parser.cpp"
    break;

  case 356:
#line 1872 "ds_parser.ypp"
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
#line 7031 "ds_parser.cpp"
    break;

  case 357:
#line 1899 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7040 "ds_parser.cpp"
    break;

  case 358:
#line 1902 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7051 "ds_parser.cpp"
    break;

  case 359:
#line 1911 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7059 "ds_parser.cpp"
    break;

  case 360:
#line 1914 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7073 "ds_parser.cpp"
    break;

  case 361:
#line 1923 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7087 "ds_parser.cpp"
    break;

  case 362:
#line 1936 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7101 "ds_parser.cpp"
    break;

  case 366:
#line 1957 "ds_parser.ypp"
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
#line 7117 "ds_parser.cpp"
    break;

  case 367:
#line 1968 "ds_parser.ypp"
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
#line 7134 "ds_parser.cpp"
    break;

  case 368:
#line 1983 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7140 "ds_parser.cpp"
    break;

  case 369:
#line 1984 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7146 "ds_parser.cpp"
    break;

  case 370:
#line 1988 "ds_parser.ypp"
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
#line 7185 "ds_parser.cpp"
    break;

  case 371:
#line 2025 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7191 "ds_parser.cpp"
    break;

  case 372:
#line 2026 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7197 "ds_parser.cpp"
    break;

  case 373:
#line 2030 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7203 "ds_parser.cpp"
    break;

  case 374:
#line 2030 "ds_parser.ypp"
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
#line 7289 "ds_parser.cpp"
    break;

  case 375:
#line 2114 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7301 "ds_parser.cpp"
    break;

  case 376:
#line 2121 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7312 "ds_parser.cpp"
    break;

  case 377:
#line 2130 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7318 "ds_parser.cpp"
    break;

  case 378:
#line 2131 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7324 "ds_parser.cpp"
    break;

  case 379:
#line 2132 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7330 "ds_parser.cpp"
    break;

  case 380:
#line 2133 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7336 "ds_parser.cpp"
    break;

  case 381:
#line 2134 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7342 "ds_parser.cpp"
    break;

  case 382:
#line 2135 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7348 "ds_parser.cpp"
    break;

  case 383:
#line 2136 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7354 "ds_parser.cpp"
    break;

  case 384:
#line 2137 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7360 "ds_parser.cpp"
    break;

  case 385:
#line 2138 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7366 "ds_parser.cpp"
    break;

  case 386:
#line 2139 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7372 "ds_parser.cpp"
    break;

  case 387:
#line 2140 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7378 "ds_parser.cpp"
    break;

  case 388:
#line 2141 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7384 "ds_parser.cpp"
    break;

  case 389:
#line 2142 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7390 "ds_parser.cpp"
    break;

  case 390:
#line 2143 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7396 "ds_parser.cpp"
    break;

  case 391:
#line 2144 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7402 "ds_parser.cpp"
    break;

  case 392:
#line 2145 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7408 "ds_parser.cpp"
    break;

  case 393:
#line 2146 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7414 "ds_parser.cpp"
    break;

  case 394:
#line 2147 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7420 "ds_parser.cpp"
    break;

  case 395:
#line 2148 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7426 "ds_parser.cpp"
    break;

  case 396:
#line 2149 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7432 "ds_parser.cpp"
    break;

  case 397:
#line 2150 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7438 "ds_parser.cpp"
    break;

  case 398:
#line 2151 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7444 "ds_parser.cpp"
    break;

  case 399:
#line 2152 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7450 "ds_parser.cpp"
    break;

  case 400:
#line 2153 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7456 "ds_parser.cpp"
    break;

  case 401:
#line 2154 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7462 "ds_parser.cpp"
    break;

  case 402:
#line 2158 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7468 "ds_parser.cpp"
    break;

  case 403:
#line 2159 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7474 "ds_parser.cpp"
    break;

  case 404:
#line 2160 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7480 "ds_parser.cpp"
    break;

  case 405:
#line 2161 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7486 "ds_parser.cpp"
    break;

  case 406:
#line 2162 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7492 "ds_parser.cpp"
    break;

  case 407:
#line 2163 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7498 "ds_parser.cpp"
    break;

  case 408:
#line 2167 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7511 "ds_parser.cpp"
    break;

  case 409:
#line 2178 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7520 "ds_parser.cpp"
    break;

  case 410:
#line 2182 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7532 "ds_parser.cpp"
    break;

  case 411:
#line 2192 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7544 "ds_parser.cpp"
    break;

  case 412:
#line 2199 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7555 "ds_parser.cpp"
    break;

  case 413:
#line 2208 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7561 "ds_parser.cpp"
    break;

  case 414:
#line 2208 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7567 "ds_parser.cpp"
    break;

  case 415:
#line 2208 "ds_parser.ypp"
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
#line 7582 "ds_parser.cpp"
    break;

  case 416:
#line 2221 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7588 "ds_parser.cpp"
    break;

  case 417:
#line 2222 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7594 "ds_parser.cpp"
    break;

  case 418:
#line 2223 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7600 "ds_parser.cpp"
    break;

  case 419:
#line 2224 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7606 "ds_parser.cpp"
    break;

  case 420:
#line 2225 "ds_parser.ypp"
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
#line 7625 "ds_parser.cpp"
    break;

  case 421:
#line 2239 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7636 "ds_parser.cpp"
    break;

  case 422:
#line 2245 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7645 "ds_parser.cpp"
    break;

  case 423:
#line 2249 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7654 "ds_parser.cpp"
    break;

  case 424:
#line 2253 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7664 "ds_parser.cpp"
    break;

  case 425:
#line 2258 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7674 "ds_parser.cpp"
    break;

  case 426:
#line 2263 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7684 "ds_parser.cpp"
    break;

  case 427:
#line 2268 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7694 "ds_parser.cpp"
    break;

  case 428:
#line 2273 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7703 "ds_parser.cpp"
    break;

  case 429:
#line 2277 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7712 "ds_parser.cpp"
    break;

  case 430:
#line 2281 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7722 "ds_parser.cpp"
    break;

  case 431:
#line 2286 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7731 "ds_parser.cpp"
    break;

  case 432:
#line 2290 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7741 "ds_parser.cpp"
    break;

  case 433:
#line 2295 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7747 "ds_parser.cpp"
    break;

  case 434:
#line 2295 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7753 "ds_parser.cpp"
    break;

  case 435:
#line 2295 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7764 "ds_parser.cpp"
    break;

  case 436:
#line 2301 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7776 "ds_parser.cpp"
    break;

  case 437:
#line 2308 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7782 "ds_parser.cpp"
    break;

  case 438:
#line 2308 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7788 "ds_parser.cpp"
    break;

  case 439:
#line 2308 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7798 "ds_parser.cpp"
    break;

  case 440:
#line 2313 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7804 "ds_parser.cpp"
    break;

  case 441:
#line 2313 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7810 "ds_parser.cpp"
    break;

  case 442:
#line 2313 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7821 "ds_parser.cpp"
    break;

  case 443:
#line 2319 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7827 "ds_parser.cpp"
    break;

  case 444:
#line 2319 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7833 "ds_parser.cpp"
    break;

  case 445:
#line 2319 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7843 "ds_parser.cpp"
    break;

  case 446:
#line 2324 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7852 "ds_parser.cpp"
    break;

  case 447:
#line 2328 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7858 "ds_parser.cpp"
    break;

  case 448:
#line 2328 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7864 "ds_parser.cpp"
    break;

  case 449:
#line 2328 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7874 "ds_parser.cpp"
    break;

  case 450:
#line 2333 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7880 "ds_parser.cpp"
    break;

  case 451:
#line 2333 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7886 "ds_parser.cpp"
    break;

  case 452:
#line 2333 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7900 "ds_parser.cpp"
    break;

  case 453:
#line 2342 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7909 "ds_parser.cpp"
    break;

  case 454:
#line 2346 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7915 "ds_parser.cpp"
    break;

  case 455:
#line 2346 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7921 "ds_parser.cpp"
    break;

  case 456:
#line 2346 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7931 "ds_parser.cpp"
    break;

  case 457:
#line 2351 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7937 "ds_parser.cpp"
    break;

  case 458:
#line 2351 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7943 "ds_parser.cpp"
    break;

  case 459:
#line 2351 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7957 "ds_parser.cpp"
    break;

  case 460:
#line 2360 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7966 "ds_parser.cpp"
    break;

  case 461:
#line 2364 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7972 "ds_parser.cpp"
    break;

  case 462:
#line 2364 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7978 "ds_parser.cpp"
    break;

  case 463:
#line 2364 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7988 "ds_parser.cpp"
    break;

  case 464:
#line 2369 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7994 "ds_parser.cpp"
    break;

  case 465:
#line 2369 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8000 "ds_parser.cpp"
    break;

  case 466:
#line 2369 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8014 "ds_parser.cpp"
    break;

  case 467:
#line 2378 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8020 "ds_parser.cpp"
    break;

  case 468:
#line 2378 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8026 "ds_parser.cpp"
    break;

  case 469:
#line 2378 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8037 "ds_parser.cpp"
    break;

  case 470:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8043 "ds_parser.cpp"
    break;

  case 471:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8049 "ds_parser.cpp"
    break;

  case 472:
#line 2384 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8060 "ds_parser.cpp"
    break;

  case 473:
#line 2393 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8066 "ds_parser.cpp"
    break;

  case 474:
#line 2393 "ds_parser.ypp"
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
#line 8083 "ds_parser.cpp"
    break;

  case 475:
#line 2408 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8089 "ds_parser.cpp"
    break;

  case 476:
#line 2408 "ds_parser.ypp"
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
#line 8110 "ds_parser.cpp"
    break;

  case 477:
#line 2428 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8116 "ds_parser.cpp"
    break;

  case 478:
#line 2429 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8122 "ds_parser.cpp"
    break;

  case 479:
#line 2430 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8128 "ds_parser.cpp"
    break;

  case 480:
#line 2431 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8134 "ds_parser.cpp"
    break;

  case 481:
#line 2435 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8146 "ds_parser.cpp"
    break;

  case 482:
#line 2442 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8158 "ds_parser.cpp"
    break;

  case 483:
#line 2449 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8169 "ds_parser.cpp"
    break;

  case 484:
#line 2455 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8180 "ds_parser.cpp"
    break;

  case 485:
#line 2464 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8190 "ds_parser.cpp"
    break;

  case 486:
#line 2469 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8199 "ds_parser.cpp"
    break;

  case 487:
#line 2476 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8205 "ds_parser.cpp"
    break;

  case 488:
#line 2477 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8211 "ds_parser.cpp"
    break;

  case 489:
#line 2481 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8222 "ds_parser.cpp"
    break;

  case 490:
#line 2487 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8234 "ds_parser.cpp"
    break;

  case 491:
#line 2494 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8247 "ds_parser.cpp"
    break;

  case 492:
#line 2502 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8259 "ds_parser.cpp"
    break;

  case 493:
#line 2509 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8272 "ds_parser.cpp"
    break;

  case 494:
#line 2517 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8286 "ds_parser.cpp"
    break;

  case 495:
#line 2529 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8294 "ds_parser.cpp"
    break;

  case 496:
#line 2532 "ds_parser.ypp"
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
#line 8310 "ds_parser.cpp"
    break;

  case 497:
#line 2546 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8321 "ds_parser.cpp"
    break;

  case 498:
#line 2555 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8327 "ds_parser.cpp"
    break;

  case 499:
#line 2556 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8333 "ds_parser.cpp"
    break;

  case 500:
#line 2560 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8343 "ds_parser.cpp"
    break;

  case 501:
#line 2565 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8352 "ds_parser.cpp"
    break;

  case 502:
#line 2572 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8362 "ds_parser.cpp"
    break;

  case 503:
#line 2577 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8375 "ds_parser.cpp"
    break;

  case 504:
#line 2588 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8385 "ds_parser.cpp"
    break;

  case 505:
#line 2593 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8394 "ds_parser.cpp"
    break;

  case 506:
#line 2600 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8408 "ds_parser.cpp"
    break;

  case 507:
#line 2612 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8414 "ds_parser.cpp"
    break;

  case 508:
#line 2613 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8420 "ds_parser.cpp"
    break;

  case 509:
#line 2617 "ds_parser.ypp"
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
#line 8443 "ds_parser.cpp"
    break;

  case 510:
#line 2635 "ds_parser.ypp"
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
#line 8466 "ds_parser.cpp"
    break;


#line 8470 "ds_parser.cpp"

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
#line 2655 "ds_parser.ypp"


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
