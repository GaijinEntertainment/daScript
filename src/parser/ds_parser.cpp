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
    END_OF_READ = 396,
    UNARY_MINUS = 397,
    UNARY_PLUS = 398,
    PRE_INC = 399,
    PRE_DEC = 400,
    POST_INC = 401,
    POST_DEC = 402,
    DEREF = 403,
    COLCOL = 404
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

#line 397 "ds_parser.cpp"

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
#define YYLAST   7139

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  177
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  169
/* YYNRULES -- Number of rules.  */
#define YYNRULES  503
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  900

#define YYUNDEFTOK  2
#define YYMAXUTOK   404

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
       2,     2,     2,   159,     2,   176,   170,   155,   148,     2,
     168,   169,   153,   152,   142,   151,   164,   154,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   145,   173,
     149,   143,   150,   144,   171,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   166,     2,   167,   147,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   174,   146,   175,   158,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   156,   157,   160,
     161,   162,   163,   165,   172
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   420,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   436,   448,   449,   453,   457,
     462,   471,   479,   495,   500,   508,   508,   538,   560,   564,
     567,   573,   582,   603,   626,   627,   631,   635,   636,   640,
     643,   649,   655,   658,   664,   665,   669,   670,   671,   680,
     681,   685,   694,   710,   720,   729,   730,   731,   732,   733,
     734,   738,   743,   751,   752,   753,   754,   755,   756,   757,
     758,   764,   777,   793,   794,   798,   805,   817,   821,   828,
     829,   833,   834,   835,   839,   844,   851,   855,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,   866,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     878,   879,   880,   881,   882,   883,   884,   885,   886,   887,
     888,   889,   890,   894,   915,   953,   960,   964,   975,   976,
     977,   978,   979,   980,   981,   982,   983,   984,   985,   986,
     987,   988,   989,   990,   991,   992,   996,  1001,  1007,  1013,
    1034,  1040,  1041,  1052,  1056,  1062,  1065,  1068,  1072,  1078,
    1082,  1086,  1089,  1092,  1097,  1100,  1108,  1111,  1116,  1119,
    1127,  1133,  1134,  1138,  1139,  1143,  1178,  1178,  1178,  1181,
    1181,  1181,  1186,  1186,  1186,  1194,  1198,  1203,  1209,  1209,
    1209,  1213,  1213,  1213,  1218,  1218,  1218,  1227,  1230,  1236,
    1237,  1244,  1255,  1256,  1257,  1261,  1266,  1321,  1322,  1323,
    1324,  1325,  1326,  1330,  1331,  1332,  1333,  1334,  1335,  1336,
    1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1349,
    1350,  1354,  1365,  1370,  1380,  1384,  1384,  1384,  1391,  1391,
    1391,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1449,  1453,  1456,  1459,  1460,  1461,  1462,  1467,
    1472,  1473,  1476,  1476,  1476,  1479,  1483,  1487,  1491,  1492,
    1493,  1494,  1495,  1496,  1497,  1507,  1517,  1572,  1573,  1577,
    1578,  1582,  1590,  1593,  1598,  1597,  1642,  1698,  1703,  1711,
    1712,  1716,  1719,  1728,  1729,  1733,  1742,  1743,  1748,  1749,
    1753,  1759,  1765,  1768,  1772,  1778,  1787,  1788,  1789,  1793,
    1794,  1798,  1801,  1806,  1811,  1819,  1830,  1833,  1840,  1841,
    1845,  1872,  1872,  1884,  1887,  1896,  1909,  1921,  1922,  1926,
    1930,  1941,  1956,  1957,  1961,  1998,  1999,  2003,  2003,  2087,
    2094,  2103,  2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,
    2112,  2113,  2114,  2115,  2116,  2117,  2118,  2119,  2120,  2121,
    2122,  2123,  2124,  2125,  2126,  2127,  2131,  2132,  2133,  2134,
    2135,  2136,  2140,  2151,  2155,  2165,  2172,  2181,  2181,  2181,
    2194,  2195,  2196,  2197,  2198,  2212,  2218,  2222,  2227,  2232,
    2237,  2242,  2246,  2250,  2255,  2259,  2264,  2264,  2264,  2270,
    2277,  2277,  2277,  2282,  2282,  2282,  2288,  2288,  2288,  2293,
    2297,  2297,  2297,  2302,  2302,  2302,  2311,  2315,  2315,  2315,
    2320,  2320,  2320,  2329,  2333,  2333,  2333,  2338,  2338,  2338,
    2347,  2347,  2347,  2353,  2353,  2353,  2362,  2362,  2377,  2377,
    2397,  2398,  2399,  2400,  2404,  2411,  2418,  2424,  2433,  2438,
    2445,  2446,  2450,  2456,  2463,  2471,  2478,  2486,  2498,  2501,
    2515,  2524,  2525,  2529,  2534,  2541,  2546,  2557,  2562,  2569,
    2581,  2582,  2586,  2604
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
  "STRING_CHARACTER", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "'.'", "DEREF", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "COLCOL", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "module_declaration", "character_sequence", "string_constant",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@1", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_if_then_else", "expression_for_loop",
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
     395,   396,    44,    61,    63,    58,   124,    94,    38,    60,
      62,    45,    43,    42,    47,    37,   397,   398,   126,    33,
     399,   400,   401,   402,    46,   403,    91,    93,    40,    41,
      36,    64,   404,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -566

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-566)))

#define YYTABLE_NINF -458

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -566,    29,  -566,  -566,   -33,   -63,   -30,   -17,   -86,  -566,
     -14,  -566,  -566,    -8,  -566,  -566,  -566,  -566,  -566,   321,
    -566,    54,  -566,  -566,  -566,  -566,  -566,  -566,    42,  -566,
      55,   -11,    85,  -566,  -566,    27,  -566,   -89,   100,  -566,
     129,   138,  -566,    99,   147,   107,  -566,   -25,  -566,  -566,
    -566,    -4,   149,  -566,   128,    19,   -33,   175,   -63,   184,
    -566,   186,   195,  -566,   250,  -566,   204,  -566,   -68,   176,
     182,   223,  -566,   -33,    -8,  -566,  6980,   325,   326,  -566,
     197,   192,  -566,   225,  -566,  -566,   367,  -566,  -566,  -566,
    -566,  -566,   235,    72,  -566,  -566,  -566,  -566,   320,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,   201,   -67,  5885,
    -566,  -566,   241,   242,  -566,   -47,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,   -76,   243,  -566,  -566,   -97,  -566,   205,
     -65,   207,  -566,   111,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,    51,  -566,  -566,   -49,  -566,   237,   240,   244,   246,
    -566,  -566,  -566,   222,  -566,  -566,  -566,  -566,  -566,   249,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,   251,
    -566,  -566,  -566,   252,   253,  -566,  -566,  -566,  -566,   254,
     256,  -566,  -566,  -566,  -566,  -566,   710,  -566,   219,   261,
    -566,   236,  -566,   -33,  -566,    -3,  -566,  -110,  5885,  -566,
    1263,  -566,  -566,  -566,  -566,  -566,   209,  -566,  -566,  -566,
      72,  -566,   -64,  4531,  -566,  -566,  -566,  -566,  -566,   272,
    -566,    93,    97,   113,  -566,  -566,  -566,  -566,  -566,   376,
    -566,  -566,    -9,  2440,  -566,  -566,   -62,  5885,   -61,    52,
     275,  -566,    91,  -566,   285,  6347,  -566,  4531,  -566,  -566,
     275,  -566,  -566,  5808,   255,  1882,  -566,  -566,   192,  4531,
     260,  4531,   269,   271,   263,  -566,   264,   280,   307,  2571,
    -566,   289,  2031,  4531,  4531,   146,   963,  4661,  4531,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  4531,  4531,  4531,   -97,
    4531,  4531,  4531,  -566,   270,  -566,   407,  -566,  -566,   273,
     274,  4531,  -566,  -566,  -566,  -566,  -566,  -566,  -566,   -78,
     276,  -566,   277,   278,  -566,  -566,  -566,   425,  -566,  -566,
    -566,   279,  -566,  -566,   -70,  -566,  -566,  -566,  -566,  6287,
     284,  -566,  -566,  -566,  -566,  -566,     4,   313,  5885,  -566,
      46,  -566,  -566,  1065,  5885,  5885,  5885,  5885,   286,   241,
    5885,   197,  5885,   197,  5885,   197,  5962,   242,  -566,  -566,
    -566,   305,  -566,  -566,  5957,  -566,  -566,  6347,  -566,  -566,
     306,  -566,  -566,  -566,  5885,  -566,  2702,  -566,   957,     3,
     291,  -566,   -97,  2702,  -566,  -566,   341,  4659,   461,  6559,
    4531,   957,  -566,  -566,  4531,  -566,  -566,   331,   349,  6559,
    5885,  2702,  -566,  4756,    22,    22,   310,  -566,   275,   454,
     275,  1588,  6353,  -566,     5,   173,    22,    22,   -69,  -566,
      22,    22,  5283,   -26,   308,  -566,  -566,   957,  -566,  2162,
    -566,  -566,  -566,   444,   275,   197,   146,  -566,    21,   353,
    4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,
    -566,  -566,  4531,  4531,  4531,  4531,  4531,  4531,   354,  2833,
    4531,   355,  4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,
    2964,  3125,  4531,  4531,  4531,  4531,  4531,  4531,  4531,  4531,
    4531,  4531,   356,  4531,  3256,  -566,   486,   451,   322,  -566,
     522,  -566,  -566,  -566,  2702,  -566,   694,  1059,  1681,  1853,
    -566,   -10,  2255,   243,  2961,   243,  3071,   243,    81,  -566,
     136,  6347,   142,  -566,  -566,  -566,  3255,  -566,  6182,  -566,
    4531,  3403,  2309,  -566,  4853,  4531,  4531,  -566,   192,  5352,
    -566,  5885,  5885,  5421,  5885,  -566,  -566,  3402,  -566,  4950,
    -566,  -566,     7,   146,   -71,  1735,  3550,  6353,   359,   -23,
     330,   361,  -566,  -566,   -90,    18,  3697,   -23,     9,  4531,
    4531,   323,  -566,  4531,   368,   370,  -566,   140,  -566,  -566,
     234,   373,  -566,    75,  6559,  -566,  -566,   243,  -566,   360,
    -566,  -566,  6559,  6559,  6559,  6559,  6559,  6559,  6559,  6559,
     230,   230,   660,  6559,  6559,   660,   311,   311,   342,  4531,
    6559,   683,  -566,  6034,   -60,   -60,  6559,   230,   230,  6559,
    6559,  4531,  6559,   377,  6423,  6625,  6695,  6761,   660,   660,
      58,    58,   683,   683,   683,  -566,  6105,  -566,    83,   296,
    -566,   275,  -566,   376,  -566,  2702,  -566,  5047,  -566,  5885,
    -566,  -566,  -566,   382,  -566,   383,  -566,   387,  -566,   388,
    5885,  -566,  5962,  -566,   242,   376,  4531,   -58,  -566,    86,
     394,   409,  5490,  -566,  -566,  -566,  3549,  3696,  -566,  3843,
     378,  -566,  4531,  -566,  4531,  4531,   -19,   412,   373,   385,
     386,  4531,   389,  4531,  4531,   373,   375,   392,  6559,  -566,
    -566,  6489,  -566,   418,  5885,   197,  1438,   192,  -566,  -566,
    4531,  -566,    80,    95,  4531,  -566,   -85,  -566,  3844,  6821,
    -566,  6821,  -566,  4531,  -566,  -566,  -566,    -4,  -566,  -566,
    5144,  -566,  -566,  3990,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  6347,  -566,  -566,  -566,  6559,   192,  -566,
    -566,   151,  -566,  -566,  -566,  -566,  3991,    23,  6559,  6559,
     -23,   393,    98,   359,   395,  -566,  6559,  -566,  -566,    41,
     -23,   396,  -566,  -566,  -566,  6619,   243,  -566,  -566,   957,
     397,  6559,  4138,  -566,  -566,  5885,  -566,    90,  6559,    -4,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  5885,  4269,   429,
    4531,  4531,  4531,  -566,  5559,  4531,   398,   401,  4531,  4531,
    -566,  4531,   400,  -566,  -566,   419,   234,  -566,  4531,  6559,
    6846,  -566,   403,  -566,  6860,   421,  5628,   423,   660,   660,
     660,  -566,  5210,   404,  -566,  6559,  6559,  5210,   410,   -97,
    -566,  -566,  6559,  -566,  -566,  -566,  -566,  -566,  4400,   528,
     424,  -566,   420,  -566,  -566,   -97,  -566,   427,  5885,   445,
    5697,  4531,   426,   432,  -566,  -566,  6887,  -566,  -566,  6559,
    -566,  -566,  -566,  5885,   431,  6896,  -566,  -566,   435,  -566
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    79,     1,   173,     0,     0,     0,     0,     0,   174,
       0,   468,   466,     0,    14,     3,    10,     9,     8,     0,
       7,   348,     6,    11,     5,     4,    12,    13,    69,    71,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    74,   151,     0,    75,    77,     0,    73,   366,
     365,     0,     0,   349,   351,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   353,     0,   357,     0,     0,
       0,     0,   153,     0,     0,    80,     0,     0,     0,    86,
      81,     0,   123,   362,   367,   346,     0,    67,    68,    65,
      66,    64,     0,     0,    63,    72,    40,    38,    34,    31,
      30,   396,   399,   397,   400,   398,   401,     0,     0,     0,
     359,   358,     0,     0,   152,     0,    78,    89,    90,    92,
      91,    93,    94,    95,    96,   115,   116,   113,   114,   106,
     117,   118,   107,   104,   105,   119,   120,   121,   122,   109,
     110,   108,   102,   103,    98,    97,    99,   100,   101,    88,
      87,   111,   112,   307,    84,   146,   125,     0,   364,     0,
       0,     0,    16,     0,    59,    60,    57,    58,    56,    55,
      61,     0,    33,   353,     0,   360,     0,     0,     0,     0,
     371,   391,   372,   403,   373,   377,   378,   379,   380,   395,
     384,   385,   386,   387,   388,   389,   390,   392,   393,   439,
     376,   383,   394,   446,   453,   374,   381,   375,   382,     0,
       0,   402,   410,   413,   411,   412,     0,   405,     0,     0,
     326,     0,    76,     0,    82,     0,   319,     0,     0,   124,
       0,   363,   312,   369,   350,   347,   339,   352,    17,    18,
       0,    70,     0,     0,   354,   430,   433,   436,   426,     0,
     407,   440,   447,   454,   460,   463,   417,   422,   429,     0,
     425,   419,     0,     0,   356,   421,     0,     0,     0,     0,
       0,   317,   330,    83,   307,    85,   148,     0,    49,    50,
       0,   247,   248,     0,     0,     0,   241,   159,     0,     0,
       0,     0,     0,     0,     0,   160,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,   207,
     209,   208,   210,   211,   212,    19,     0,     0,     0,     0,
       0,     0,     0,   202,   203,   128,   126,   245,   244,     0,
       0,     0,   141,   136,   134,   135,   205,   147,   129,   242,
       0,   300,     0,     0,   139,   140,   142,   171,   133,   299,
     298,    79,   303,   243,     0,   130,   302,   301,   280,   213,
       0,   246,   470,   471,   472,   473,   307,     0,     0,   340,
       0,    62,   361,     0,     0,     0,     0,     0,     0,     0,
       0,    81,     0,    81,     0,    81,     0,     0,   424,   418,
     420,     0,   423,   415,     0,   406,   469,   325,   467,   327,
       0,   318,   329,   328,     0,   331,     0,   320,     0,     0,
     155,   158,     0,     0,   161,   164,     0,   213,     0,   154,
       0,     0,   176,   179,     0,   145,   182,     0,     0,    43,
       0,     0,   168,   213,   271,   272,   203,   150,     0,   480,
       0,     0,     0,   497,     0,     0,   252,   251,   285,    25,
     250,   249,     0,   204,     0,   143,   144,     0,   306,     0,
     132,   137,   138,     0,     0,    81,     0,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     273,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,   309,     0,   370,
       0,   337,   338,   336,     0,   355,     0,     0,     0,     0,
     404,     0,     0,    84,     0,    84,     0,    84,   151,   323,
       0,   321,     0,   416,   414,   308,   332,   335,   334,    53,
       0,     0,     0,   165,   213,     0,     0,   162,     0,     0,
      54,     0,     0,     0,     0,    41,    42,     0,   169,   213,
     166,   204,     0,     0,   151,     0,     0,   488,   478,   480,
       0,   491,   492,   493,     0,     0,     0,   480,     0,     0,
       0,     0,    22,     0,    20,     0,   275,   235,   234,   146,
      46,     0,   281,     0,   197,   172,   175,    84,   149,     0,
     295,   296,   220,   221,   223,   222,   224,   217,   218,   219,
     253,   254,   266,   225,   226,   267,   264,   265,     0,     0,
     215,   290,   279,     0,   304,   305,   216,   255,   256,   227,
     228,     0,   214,     0,     0,   269,   270,   268,   262,   263,
     258,   257,   259,   260,   261,   278,     0,   283,     0,   309,
     310,     0,   313,   336,   341,     0,   345,   213,   431,     0,
     437,   427,   408,     0,   441,     0,   448,     0,   455,     0,
       0,   461,     0,   464,     0,   328,     0,     0,   156,     0,
       0,     0,     0,   163,   170,   286,     0,     0,   287,     0,
       0,   167,     0,   481,     0,     0,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   498,
     499,     0,    23,    26,     0,    81,     0,     0,    44,    45,
       0,    51,     0,     0,     0,   282,     0,   292,     0,   230,
     277,   229,   297,     0,   276,   284,   314,     0,   311,   343,
     213,   344,   432,     0,   438,   428,   409,   442,   444,   449,
     451,   456,   458,   322,   462,   324,   465,   333,     0,   157,
     188,     0,   185,   177,   180,   183,     0,     0,   475,   474,
     480,     0,     0,   479,     0,   483,   489,   495,   494,     0,
     480,     0,   496,    21,    24,     0,    84,   127,    47,     0,
       0,   198,     0,   199,   206,     0,   232,     0,   291,     0,
     316,   342,   434,   445,   452,   459,    52,     0,     0,     0,
       0,     0,     0,   288,     0,     0,     0,     0,     0,     0,
     482,     0,     0,   486,   236,     0,    46,   231,     0,   200,
       0,   233,     0,   435,     0,     0,     0,     0,   178,   181,
     184,   289,   500,     0,   484,   477,   476,   500,     0,     0,
     239,    48,   201,   293,   315,   189,   191,   186,     0,     0,
       0,   485,     0,   487,   237,     0,   294,     0,     0,     0,
       0,     0,     0,     0,   240,   190,     0,   194,   187,   501,
     502,   503,   192,     0,     0,     0,   193,   195,     0,   196
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -566,  -566,  -566,   156,   553,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,   512,  -566,  -566,   556,  -566,  -566,
    -566,  -225,  -566,  -566,  -566,  -566,  -566,   379,  -566,   559,
     -55,  -566,   543,  -566,  -286,  -373,  -516,  -566,  -566,  -191,
    -126,   -79,  -566,    24,  -278,   -13,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,   626,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,
    -498,  -566,  -566,  -290,  -566,  -226,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,   428,  -566,  -566,   265,   -27,  -566,
    -566,  -566,   362,  -566,   -53,  -566,  -263,   257,  -269,  -264,
     117,  -566,   169,  -566,  -566,  -566,  -566,   465,   571,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -151,  -108,  -566,
    -566,  -566,   262,  -566,  -566,  -566,   -77,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,  -566,
    -566,  -566,  -566,  -566,  -566,   357,  -565,  -427,  -559,  -566,
    -566,  -295,   -59,   202,  -566,  -566,  -566,  -212,  -566
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   445,   327,   723,   328,   595,
      16,    17,    35,    36,    63,    18,    32,    33,   329,   330,
     730,   731,   331,   332,   333,   334,   335,   170,   171,    29,
      30,    45,    46,    47,    19,   154,   229,    80,    20,    81,
      82,   336,   337,   230,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   464,   347,   348,   349,   561,   820,   562,
     821,   564,   822,   350,   817,   877,   878,   894,   893,   898,
     603,   804,   351,   352,   353,   416,   355,   356,   357,   358,
     724,   859,   725,   875,   604,   805,   876,   225,   661,   518,
     366,   809,   226,   227,   539,   540,   220,   221,   705,   271,
     524,   370,   235,   160,    54,    22,    86,   108,    67,    68,
      23,    24,   158,    84,    52,    25,   159,   272,   360,   107,
     213,   214,   218,   215,   379,   756,   541,   377,   755,   374,
     752,   375,   843,   376,   754,   380,   757,   381,   813,   382,
     759,   383,   814,   384,   761,   385,   815,   386,   764,   387,
     766,    26,    41,    27,    40,   361,   578,   579,   580,   362,
     581,   582,   583,   584,   363,   444,   364,   870,   365
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   212,   156,   406,   354,   399,   401,   415,   533,   236,
     535,   -79,   537,   443,   587,   437,   658,   675,   115,   677,
     709,   679,   550,   389,   432,    42,   702,   573,   716,     2,
      76,   573,   216,    87,    88,     3,   733,   714,    43,   609,
     802,   402,   468,   469,   270,   491,   492,   466,    59,   704,
     223,   488,   687,   689,   491,   492,    64,   458,     4,   273,
       5,    48,     6,   274,     7,   465,    31,    66,   174,     8,
     233,   174,   403,   395,   219,    44,     9,   712,   468,   469,
     516,    60,    10,   713,   734,    65,   164,   165,    38,   155,
     459,   736,   607,   224,   243,    56,   211,   513,    11,    77,
      78,    71,    28,   467,   512,    34,   513,   110,   175,    43,
     234,   372,    53,   396,   398,   768,   732,    74,    37,    12,
     212,    39,   222,   597,   244,   480,   481,    43,   547,   409,
     223,    79,   233,   488,    57,   553,   491,   492,   493,   390,
     672,   494,    75,   783,   231,   367,    44,   781,    89,   367,
     708,   275,    90,   568,    91,    92,   610,   391,   521,   212,
     367,   480,   481,   673,    44,   734,   522,   392,   269,   488,
      13,   490,   491,   492,   493,   212,   608,   494,   590,   515,
     591,    61,   713,   734,   717,    55,   512,    93,   513,   523,
     397,    62,   402,   240,    56,    13,   825,    56,   212,   212,
     704,   166,    14,   402,   777,   167,   410,   168,    92,   418,
     402,   509,   510,   511,   831,   211,   789,   734,   828,   400,
     241,   826,   512,   403,   513,   734,   680,    58,   734,   439,
     441,   832,   734,   367,   403,    66,   404,   707,  -443,   405,
     807,   403,  -450,  -443,   735,   727,   666,  -450,   238,   239,
     468,   469,   745,    71,   211,   769,   728,   729,  -457,   841,
     212,  -443,   800,  -457,    69,  -450,   212,   212,   212,   212,
     211,    71,   212,    70,   212,    73,   212,   686,   212,   780,
     835,  -457,    72,   703,    83,  -238,   681,   572,   790,   585,
    -238,   520,   683,   211,   211,   719,   212,   526,   527,   528,
     529,   818,    85,   532,    96,   534,   449,   536,  -238,   682,
     162,   592,   593,   236,   101,   684,   323,   436,   102,    98,
     155,    99,   212,   256,   819,    49,    50,   546,    51,   549,
     100,   468,   469,   480,   481,   103,   104,   105,   106,   660,
     746,   488,   560,   490,   491,   492,   493,   109,   257,   494,
     112,   367,   796,   567,   368,   211,   113,   369,   114,   151,
     152,   211,   211,   211,   211,   153,   155,   211,   161,   211,
     157,   211,   162,   211,    60,   173,   217,   219,   600,   232,
     237,   507,   508,   509,   510,   511,   245,   749,   228,   246,
     249,   211,   266,   247,   512,   248,   513,   748,   250,   552,
     251,   252,   253,   254,   258,   255,   267,   378,   388,   268,
     233,   223,   478,   479,   480,   481,   482,   211,   422,   485,
     423,   399,   488,   412,   490,   491,   492,   493,   420,   426,
     494,   424,   496,   497,   259,   260,   427,   425,   430,   261,
     598,   453,   262,   454,   463,    13,   455,   456,   519,   460,
     461,   462,   514,   212,   212,   530,   212,   263,   466,   551,
     505,   506,   507,   508,   509,   510,   511,   265,   281,   282,
     283,   284,   543,   545,   558,   512,   565,   513,   566,   694,
     286,   571,   599,   605,   696,   697,   256,   699,   611,   628,
     632,   655,   290,   659,   660,   662,   292,   710,   720,   293,
     354,   707,   294,   711,   573,   238,   297,   722,   732,   737,
     738,   257,   742,   829,   180,   181,   182,   395,   184,   185,
     186,   187,   188,   300,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   758,   200,   201,   202,   760,   762,   205,
     206,   207,   208,   770,   771,   301,   776,   782,   211,   211,
     791,   211,   784,   785,   256,   794,   787,   303,   304,   792,
     827,   212,   830,   833,   847,   853,   837,   258,   854,   860,
     866,   871,   212,   868,   212,   858,   864,   873,   881,   257,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   574,
     315,   882,   753,   890,   887,   883,   885,   259,   260,   891,
     896,   594,   261,   763,   899,   575,   317,   318,    94,   319,
     172,   861,   320,   321,    97,    95,   212,   116,   842,   371,
     263,   810,   576,   726,   323,   324,    44,    21,   155,   765,
     265,   517,   747,   606,   521,   258,   407,   665,   242,   111,
     411,   531,   522,   588,   542,   872,     0,   795,   798,     0,
       0,     0,     0,     0,   788,     0,   211,   803,   359,     0,
       0,     0,     0,     0,     0,   663,   260,   211,     0,   211,
     261,   373,     0,   262,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,   263,   816,
       0,   394,     0,     0,     0,   664,     0,   212,   265,     0,
       0,     0,     0,   468,   469,   408,     0,     0,     0,   212,
       0,   211,     0,   417,     0,     0,     0,   419,     0,   421,
     836,     0,     0,     0,     0,     0,   256,   429,   840,     0,
     433,   434,   435,     0,     0,     0,   442,     0,     0,     0,
     844,     0,   256,     0,   446,   447,   448,     0,   450,   451,
     452,   257,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   478,   479,   480,   481,     0,     0,   257,     0,     0,
     212,   488,     0,   490,   491,   492,   493,     0,     0,   494,
       0,   496,   497,     0,     0,   212,   480,   481,     0,     0,
       0,     0,   211,     0,   488,     0,   490,   491,   492,   493,
       0,   886,   494,     0,   211,     0,     0,   258,     0,     0,
       0,   507,   508,   509,   510,   511,   895,     0,     0,     0,
       0,     0,     0,   258,   512,     0,   513,     0,     0,     0,
       0,     0,     0,     0,   548,     0,     0,   259,   260,     0,
       0,   554,   261,     0,   668,   262,   874,   512,   559,   513,
       0,     0,   563,   259,   260,     0,     0,     0,   261,   569,
     263,   262,   884,     0,     0,   211,     0,   577,     0,   577,
     265,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     211,     0,     0,   264,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   612,   613,
     614,   615,   616,   617,   618,   619,   620,   621,     0,     0,
     622,   623,   624,   625,   626,   627,     0,   630,   631,     0,
     633,   634,   635,   636,   637,   638,   639,   640,   642,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
       0,   656,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   667,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,     0,   468,   469,     0,
     692,     0,     0,   630,   642,     0,     0,   176,     0,     0,
       0,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   452,     0,     0,     0,     0,   178,
       0,     0,     0,     0,   452,     0,     0,   718,   442,     0,
       0,   721,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,     0,     0,   739,   478,   479,
     480,   481,   482,     0,     0,   485,   486,   487,   488,   741,
     490,   491,   492,   493,     0,     0,   494,     0,   496,   497,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,   256,     0,   750,     0,     0,     0,     0,    43,     0,
       0,   501,     0,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,     0,   767,     0,   257,     0,     0,     0,
       0,   512,     0,   513,     0,     0,     0,     0,     0,     0,
       0,   155,   778,   779,     0,    44,     0,     0,     0,   786,
       0,   577,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,     0,     0,     0,   799,     0,
       0,     0,   801,     0,     0,     0,   478,   479,   480,   481,
     482,   808,   258,   485,   486,   487,   488,     0,   490,   491,
     492,   493,     0,     0,   494,     0,   496,   497,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   824,     0,     0,   261,     0,   501,
     262,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,     0,     0,     0,     0,   263,     0,     0,     0,   512,
     839,   513,   669,     0,     0,   265,     0,     0,   525,     0,
       0,     0,     0,     0,     0,     0,   846,     0,   848,   849,
     850,     0,     0,   852,     0,     0,   855,   856,     0,   857,
       0,     0,     0,     0,   276,     0,   862,     0,     0,     3,
       0,   277,   278,   279,     0,   280,     0,   281,   282,   283,
     284,     0,     0,     0,     0,     0,     0,     0,   285,   286,
     287,   288,     0,     0,     0,     0,   880,     0,     0,     0,
     289,   290,     0,     0,   291,   292,     0,     0,   293,   889,
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
       0,     0,     0,     0,   316,   317,   318,     0,   319,     0,
       0,   320,   321,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,   323,   324,    44,   325,   155,   326,   276,
       0,     0,     0,     0,     3,     0,   277,   278,   279,     0,
     280,     0,   281,   282,   283,   284,     0,     0,     0,     0,
       0,     0,     0,   285,   286,   287,   288,     0,     0,     0,
       0,     0,     0,     0,     0,   289,   290,     0,     0,   291,
     292,     0,     0,   293,     0,     9,   294,   295,     0,   296,
     297,     0,     0,   298,   299,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   300,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   301,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,     0,     0,     0,
       0,     0,     0,     0,   306,   307,   308,   309,   310,   311,
     312,   313,   314,    43,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
     317,   318,     0,   319,     0,     0,   320,   321,     0,     0,
       0,     0,   281,   282,   283,   284,   322,     0,   323,   324,
      44,   325,   155,   797,   286,     0,     0,     0,     0,     0,
     256,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,   257,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   300,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   574,   315,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,   261,     0,   257,   575,
     317,   318,     0,   319,     0,     0,   320,   321,     0,   281,
     282,   283,   284,     0,   263,     0,   586,     0,   323,   324,
      44,   286,   155,     0,   265,     0,     0,   389,     0,     0,
       0,     0,     0,   290,     0,     0,     0,   292,     0,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,   258,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   300,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,   259,   260,   301,     0,     0,   261,
       0,   670,   262,     0,     0,     0,     0,     0,   303,   304,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,   306,   307,   308,   309,   310,   311,   312,   313,   314,
      43,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   390,     0,   256,   316,   317,   318,     0,
     319,     0,     0,   320,   321,     0,   281,   282,   283,   284,
       0,   391,     0,   322,     0,   323,   324,    44,   286,   155,
     257,   392,     0,     0,     0,     0,     0,     0,     0,     0,
     290,     0,     0,     0,   292,     0,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,   258,   205,   206,   207,
     208,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,   413,     0,   259,   260,     0,     0,
     305,   261,     0,   671,   262,     0,     0,     0,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,   316,   317,   318,     0,   319,     0,     0,
     320,   321,     0,     0,     0,   281,   282,   283,   284,     0,
     322,     0,   323,   324,    44,   414,   155,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,   292,     0,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     300,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,   305,
       0,     0,     0,     0,     0,     0,     0,   306,   307,   308,
     309,   310,   311,   312,   313,   314,    43,   315,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
       0,     0,   316,   317,   318,     0,   319,     0,   286,   320,
     321,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     290,   323,   324,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,   316,   317,   318,     0,   319,     0,     0,
     320,   321,     0,   281,   282,   283,   284,   690,   601,     0,
     322,   602,   323,   324,    44,   286,   155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   290,     0,     0,
       0,   292,     0,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,   258,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   259,   260,
     301,     0,     0,   261,     0,   674,   262,     0,     0,     0,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,     0,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   284,   691,     0,
     316,   317,   318,     0,   319,     0,   286,   320,   321,     0,
       0,     0,     0,     0,     0,     0,     0,   322,   290,   323,
     324,    44,   292,   155,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   300,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,   304,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   306,   307,   308,   309,
     310,   311,   312,   313,   314,    43,   315,     0,     0,     0,
       0,     0,     0,     0,     0,   281,   282,   283,   284,     0,
       0,   316,   317,   318,     0,   319,     0,   286,   320,   321,
       0,     0,     0,     0,     0,     0,     0,   393,   322,   290,
     323,   324,    44,   292,   155,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,   428,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     300,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   306,   307,   308,
     309,   310,   311,   312,   313,   314,    43,   315,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
       0,     0,   316,   317,   318,     0,   319,     0,   286,   320,
     321,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     290,   323,   324,    44,   292,   155,     0,   293,     0,     0,
     294,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   300,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,   304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,     0,     0,     0,     0,     0,   306,   307,
     308,   309,   310,   311,   312,   313,   314,    43,   315,     0,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,     0,     0,   316,   317,   318,     0,   319,     0,   286,
     320,   321,     0,     0,     0,     0,     0,     0,     0,     0,
     322,   290,   323,   324,    44,   292,   155,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   300,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,    43,   315,
       0,     0,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   284,     0,     0,   316,   317,   318,     0,   319,     0,
     286,   320,   321,   256,     0,     0,     0,     0,     0,     0,
       0,   322,   290,   323,   324,    44,   292,   155,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,   257,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   300,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,   304,     0,
       0,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,   641,     0,     0,     0,     0,     0,     0,     0,
     306,   307,   308,   309,   310,   311,   312,   313,   314,    43,
     315,     0,     0,   256,   259,   260,     0,     0,     0,   261,
       0,   676,   262,     0,     0,   316,   317,   318,     0,   319,
       0,     0,   320,   321,     0,     0,     0,   263,   257,     0,
       0,     0,   322,     0,   323,   324,    44,   265,   155,   281,
     282,   283,   284,     0,     0,     0,   643,     0,     0,     0,
       0,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   290,     0,     0,     0,   292,     0,     0,
     293,     0,     0,   294,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,   258,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   300,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,   259,   260,   301,     0,     0,   261,
       0,   678,   262,     0,     0,     0,     0,     0,   303,   304,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,   306,   307,   308,   309,   310,   311,   312,   313,   314,
      43,   315,     0,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,     0,     0,   316,   317,   318,     0,
     319,     0,   286,   320,   321,     0,     0,   256,     0,     0,
       0,     0,     0,   322,   290,   323,   324,    44,   292,   155,
       0,   293,     0,     0,   294,     0,     0,     0,   297,     0,
       0,     0,   257,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   300,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   303,
     304,     0,     0,     0,     0,     0,     0,   402,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   306,   307,   308,   309,   310,   311,   312,   313,
     314,    43,   315,     0,     0,     0,     0,     0,   685,   260,
       0,     0,     0,   261,     0,     0,   262,   316,   317,   318,
       0,   319,     0,     0,   320,   321,     0,   281,   282,   283,
     284,   263,     0,     0,   322,   657,   323,   324,    44,   286,
     155,   265,     0,     0,   256,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,   292,     0,     0,   293,     0,
       0,   294,     0,     0,     0,   297,     0,     0,     0,   257,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   300,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   304,     0,     0,
       0,     0,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,    43,   315,
       0,     0,     0,     0,     0,   259,   260,     0,     0,     0,
     261,     0,   700,   262,   316,   317,   318,     0,   319,     0,
       0,   320,   321,     0,   281,   282,   283,   284,   263,     0,
       0,   322,   688,   323,   324,    44,   286,   155,   265,     0,
       0,   256,     0,     0,     0,     0,     0,     0,   290,     0,
       0,     0,   292,     0,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,   257,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   300,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,   304,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   306,   307,   308,   309,
     310,   311,   312,   313,   314,    43,   315,     0,     0,     0,
       0,     0,   259,   260,     0,     0,     0,   261,     0,   773,
     262,   316,   317,   318,     0,   319,     0,     0,   320,   321,
       0,   281,   282,   283,   284,   263,     0,     0,   322,   706,
     323,   324,    44,   286,   155,   265,     0,     0,   256,     0,
       0,     0,     0,     0,     0,   290,     0,     0,     0,   292,
       0,     0,   293,     0,     0,   294,     0,     0,     0,   297,
       0,     0,     0,   257,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   300,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   304,     0,     0,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   306,   307,   308,   309,   310,   311,   312,
     313,   314,    43,   315,     0,     0,     0,     0,     0,   259,
     260,     0,     0,     0,   261,     0,   774,   262,   316,   317,
     318,     0,   319,     0,     0,   320,   321,     0,   281,   282,
     283,   284,   263,     0,     0,   322,   715,   323,   324,    44,
     286,   155,   265,     0,     0,   256,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,   292,     0,     0,   293,
       0,     0,   294,     0,     0,     0,   297,     0,     0,     0,
     257,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   300,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,   304,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     306,   307,   308,   309,   310,   311,   312,   313,   314,    43,
     315,     0,     0,     0,     0,     0,   259,   260,     0,     0,
       0,   261,     0,   775,   262,   316,   317,   318,     0,   319,
       0,     0,   320,   321,     0,   281,   282,   283,   284,   263,
       0,     0,   322,   806,   323,   324,    44,   286,   155,   265,
       0,     0,   256,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,   292,     0,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,     0,   257,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     300,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,     0,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   306,   307,   308,
     309,   310,   311,   312,   313,   314,    43,   315,     0,     0,
       0,     0,     0,   259,   260,     0,     0,     0,   261,     0,
     812,   262,   316,   317,   318,     0,   319,     0,     0,   320,
     321,     0,   281,   282,   283,   284,   263,     0,     0,   322,
     823,   323,   324,    44,   286,   155,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     292,     0,     0,   293,     0,     0,   294,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   300,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
     838,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,   307,   308,   309,   310,   311,
     312,   313,   314,    43,   315,     0,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,   845,     0,   316,
     317,   318,     0,   319,     0,   286,   320,   321,     0,     0,
       0,     0,     0,     0,     0,     0,   322,   290,   323,   324,
      44,   292,   155,     0,   293,     0,     0,   294,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   300,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,   304,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    43,   315,     0,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   284,   879,     0,
     316,   317,   318,     0,   319,     0,   286,   320,   321,     0,
       0,     0,     0,     0,     0,     0,     0,   322,   290,   323,
     324,    44,   292,   155,     0,   293,     0,     0,   294,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   300,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,   304,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   306,   307,   308,   309,
     310,   311,   312,   313,   314,    43,   315,     0,     0,     0,
       0,     0,     0,     0,     0,   281,   282,   283,   284,     0,
       0,   316,   317,   318,     0,   319,     0,   286,   320,   321,
       0,     0,     0,     0,     0,     0,     0,     0,   322,   290,
     323,   324,    44,   292,   155,     0,   293,     0,     0,   294,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     300,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   306,   307,   308,
     309,   310,   311,   312,   313,   314,    43,   315,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,   468,
     469,     0,   316,   317,   318,   176,   319,     0,     0,   320,
     321,   177,     0,     0,     0,     0,     0,     0,     0,   322,
       0,   323,   324,    44,     0,   155,     0,   178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   555,   490,   491,   492,   493,   468,   469,   494,   495,
     496,   497,   498,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,   556,   501,     0,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,   512,     0,   513,     0,     0,     0,     0,
       0,     0,   557,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   555,   490,
     491,   492,   493,   468,   469,   494,   495,   496,   497,   498,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   556,
     501,     0,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
     512,     0,   513,     0,     0,     0,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   555,   490,   491,   492,   493,
     468,   469,   494,   495,   496,   497,   498,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,   501,     0,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,   512,     0,   513,
       0,     0,     0,     0,     0,     0,   693,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   555,   490,   491,   492,   493,   468,   469,   494,
     495,   496,   497,   498,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,   501,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,     0,     0,     0,     0,
       0,     0,     0,     0,   512,     0,   513,     0,     0,     0,
       0,     0,     0,   701,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   555,
     490,   491,   492,   493,   468,   469,   494,   495,   496,   497,
     498,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,   501,     0,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,   512,     0,   513,     0,     0,     0,     0,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   555,   490,   491,   492,
     493,     0,     0,   494,   495,   496,   497,   498,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,   501,     0,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
       0,     0,     0,   468,   469,     0,     0,     0,   512,     0,
     513,   478,   479,   480,   481,   482,     0,   811,   485,   486,
     487,   488,     0,   490,   491,   492,   493,     0,     0,   494,
       0,   496,   497,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,     0,     0,     0,     0,
       0,     0,   468,   469,   512,     0,   513,     0,     0,     0,
       0,     0,     0,   869,   478,   479,   480,   481,   482,     0,
       0,   485,   486,   487,   488,     0,   490,   491,   492,   493,
       0,     0,   494,     0,   496,   497,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   501,     0,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,     0,
       0,   468,   469,     0,     0,     0,     0,   512,     0,   513,
       0,     0,   596,   478,   479,   480,   481,   482,     0,     0,
     485,   486,   487,   488,     0,   490,   491,   492,   493,     0,
       0,   494,     0,   496,   497,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   501,     0,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,     0,     0,
     468,   469,     0,     0,     0,     0,   512,     0,   513,     0,
       0,   695,   478,   479,   480,   481,   482,     0,     0,   485,
     486,   487,   488,     0,   490,   491,   492,   493,     0,     0,
     494,     0,   496,   497,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   501,     0,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,     0,     0,   468,
     469,     0,     0,     0,     0,   512,     0,   513,     0,     0,
     698,   478,   479,   480,   481,   482,     0,     0,   485,   486,
     487,   488,     0,   490,   491,   492,   493,     0,     0,   494,
       0,   496,   497,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,     0,     0,   468,   469,
       0,     0,     0,     0,   512,     0,   513,     0,     0,   772,
     478,   479,   480,   481,   482,     0,     0,   485,   486,   487,
     488,     0,   490,   491,   492,   493,     0,     0,   494,     0,
     496,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,     0,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,     0,     0,   468,   469,     0,
       0,     0,     0,   512,     0,   513,     0,     0,   851,   478,
     479,   480,   481,   482,     0,     0,   485,   486,   487,   488,
       0,   490,   491,   492,   493,     0,     0,   494,     0,   496,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,     0,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,     0,     0,     0,     0,     0,     0,
       0,     0,   512,     0,   513,     0,     0,   867,   478,   479,
     480,   481,   482,     0,     0,   485,   486,   487,   488,     0,
     490,   491,   492,   493,     0,     0,   494,     0,   496,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,   177,     0,
       0,   501,     0,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,     0,   178,     0,     0,     0,     0,     0,
       0,   512,     0,   513,     0,     0,   888,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,   306,   307,   308,     0,     0,     0,
       0,     0,     0,    43,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,     0,   468,   469,     0,
      44,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,     0,   468,   469,     0,    44,   478,   479,
     480,   481,   482,     0,     0,   485,   486,   487,   488,     0,
     490,   491,   492,   493,     0,     0,   494,     0,   496,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   538,     0,     0,
       0,   501,     0,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,   512,     0,   513,   544,   468,   469,     0,     0,     0,
       0,     0,     0,     0,    44,   478,   479,   480,   481,   482,
       0,     0,   485,   486,   487,   488,     0,   490,   491,   492,
     493,     0,     0,   494,     0,   496,   497,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,     0,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
       0,     0,     0,     0,     0,     0,     0,     0,   512,     0,
     513,   740,   468,   469,     0,     0,   478,   479,   480,   481,
     482,     0,     0,   485,   486,   487,   488,     0,   490,   491,
     492,   493,     0,     0,   494,     0,   496,   497,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   501,
       0,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,   512,
       0,   513,   744,     0,     0,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   555,   490,   491,   492,   493,  -213,
       0,   494,   495,   496,   497,   498,   499,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   556,   501,     0,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,     0,     0,
       0,     0,     0,     0,     0,     0,   512,     0,   513,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,   256,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   257,     0,   494,   495,   496,   497,
     498,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     500,   501,     0,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   468,   469,     0,     0,     0,     0,     0,
       0,   512,     0,   513,   478,   479,   480,   481,   482,     0,
     258,   485,   486,   487,   488,     0,   490,   491,   492,   493,
       0,     0,   494,     0,   496,   497,     0,     0,   589,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   260,     0,     0,     0,   261,     0,   501,   262,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   468,
     469,     0,     0,   263,     0,     0,     0,   512,     0,   513,
       0,     0,     0,   265,   478,   479,   480,   481,   482,     0,
       0,   485,   486,   487,   488,     0,   490,   491,   492,   493,
       0,     0,   494,     0,   496,   497,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   501,   743,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   468,
     469,     0,     0,     0,     0,     0,     0,   512,     0,   513,
     478,   479,   480,   481,   482,     0,     0,   485,   486,   487,
     488,     0,   490,   491,   492,   493,     0,     0,   494,     0,
     496,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   793,
       0,     0,     0,   501,     0,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   468,   469,     0,     0,     0,
       0,   256,     0,   512,     0,   513,     0,     0,     0,     0,
     478,   479,   480,   481,   482,     0,     0,   485,   486,   487,
     488,     0,   490,   491,   492,   493,   257,     0,   494,     0,
     496,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,     0,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   468,   469,     0,     0,     0,
       0,     0,     0,   512,     0,   513,   478,   479,   480,   481,
     482,     0,   258,   485,   486,   487,   488,     0,   490,   491,
     492,   493,     0,     0,   494,     0,   496,   497,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,     0,     0,     0,   261,     0,   834,
     262,     0,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   468,   469,     0,     0,   263,     0,     0,     0,   512,
       0,   513,     0,     0,     0,   265,   478,   479,   480,   481,
     482,     0,     0,   485,   486,   487,   488,     0,   490,   491,
     492,   493,     0,     0,   494,     0,   496,   497,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   504,   505,   506,   507,   508,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,   512,
       0,   513,   478,   479,   480,   481,   482,     0,     0,   485,
     486,   487,   488,     0,   490,   491,   492,   493,   256,     0,
     494,     0,   496,   497,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,     0,     0,     0,
     505,   506,   507,   508,   509,   510,   511,   257,     0,   256,
       0,     0,   478,   479,     0,   512,   482,   513,   256,   485,
     486,   487,   488,     0,   490,   491,   492,   493,     0,     0,
     494,     0,   496,   497,   257,     0,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,   501,     0,   502,   503,   504,
     505,   506,     0,   258,     0,   510,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   512,     0,   513,     0,   259,
     260,     0,     0,     0,   261,     0,   863,   262,     0,     0,
     258,     0,     0,   259,   260,     0,     0,     0,   261,   258,
     865,   262,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,   263,     0,     0,     0,
     259,   260,     0,     0,     0,   261,   265,   892,   262,   259,
     260,     0,     0,     0,   261,     0,   897,   262,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,   263,   265,     0,     0,     0,     0,     0,     0,
       0,     0,   265,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   135,   136,   137,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      13,   109,    81,   272,   230,   268,   270,   285,   381,   160,
     383,     7,   385,   308,   441,   305,   514,   533,    73,   535,
     579,   537,    19,    32,   302,    33,    19,    50,   587,     0,
      34,    50,   109,    14,    15,     6,   601,    19,   135,    18,
     125,   112,    20,    21,    47,   114,   115,   117,    21,   120,
     126,   111,   550,   551,   114,   115,   145,   135,    29,   169,
      31,    74,    33,   173,    35,   351,   129,   135,   135,    40,
     135,   135,   143,   135,   135,   172,    47,   167,    20,    21,
     366,    54,    53,   173,   142,   174,    14,    15,   174,   174,
     168,   607,   465,   169,   143,   142,   109,   166,    69,   103,
     104,   172,   135,   173,   164,   135,   166,   175,   175,   135,
     175,   175,    58,   175,   175,   173,   135,   142,   135,    90,
     228,   135,   169,   149,   173,   103,   104,   135,   406,   280,
     126,   135,   135,   111,   145,   413,   114,   115,   116,   148,
     150,   119,   167,   708,   157,   142,   172,   706,   129,   142,
     173,   228,   133,   431,   135,   136,   135,   166,   112,   267,
     142,   103,   104,   173,   172,   142,   120,   176,   223,   111,
     166,   113,   114,   115,   116,   283,   466,   119,   173,   175,
     175,   154,   173,   142,   175,   143,   164,   168,   166,   143,
     267,   164,   112,   142,   142,   166,   173,   142,   306,   307,
     120,   129,   173,   112,   702,   133,   283,   135,   136,   288,
     112,   153,   154,   155,   173,   228,   714,   142,   120,   167,
     169,   780,   164,   143,   166,   142,   145,   142,   142,   306,
     307,   790,   142,   142,   143,   135,   145,   142,   145,   148,
     738,   143,   145,   150,   169,    11,   524,   150,   137,   138,
      20,    21,   169,   172,   267,   169,    22,    23,   145,   169,
     368,   168,   167,   150,   135,   168,   374,   375,   376,   377,
     283,   172,   380,   135,   382,   168,   384,   546,   386,   706,
     796,   168,   135,   573,   135,   145,   150,   438,   715,   440,
     150,   368,   150,   306,   307,   590,   404,   374,   375,   376,
     377,   150,   174,   380,   129,   382,   319,   384,   168,   173,
     137,   138,   139,   464,    64,   173,   170,   171,    68,   135,
     174,   135,   430,    32,   173,     4,     5,   404,     7,   408,
     135,    20,    21,   103,   104,    85,    86,    87,    88,    43,
      44,   111,   421,   113,   114,   115,   116,   143,    57,   119,
     174,   142,   725,   430,   145,   368,   174,   148,   135,    34,
      34,   374,   375,   376,   377,   168,   174,   380,     1,   382,
     145,   384,   137,   386,    54,   174,   135,   135,   457,   174,
     173,   151,   152,   153,   154,   155,   149,   665,   145,   149,
     168,   404,   173,   149,   164,   149,   166,   661,   149,   412,
     149,   149,   149,   149,   113,   149,   145,   135,    32,   173,
     135,   126,   101,   102,   103,   104,   105,   430,   149,   108,
     149,   684,   111,   168,   113,   114,   115,   116,   168,   149,
     119,   168,   121,   122,   143,   144,   129,   173,   149,   148,
     453,   171,   151,    36,    19,   166,   173,   173,   135,   173,
     173,   173,   168,   561,   562,   169,   564,   166,   117,   168,
     149,   150,   151,   152,   153,   154,   155,   176,    14,    15,
      16,    17,   167,   167,    13,   164,   145,   166,   129,   558,
      26,   171,   174,    39,   561,   562,    32,   564,   135,   135,
     135,   135,    38,     7,    43,   173,    42,   167,   175,    45,
     726,   142,    48,   142,    50,   137,    52,   137,   135,   149,
     168,    57,   135,   782,    60,    61,    62,   135,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,   150,    80,    81,    82,   150,   150,    85,
      86,    87,    88,   149,   135,    91,   168,   135,   561,   562,
     175,   564,   167,   167,    32,   137,   167,   103,   104,   167,
     167,   669,   167,   167,   135,   167,   169,   113,   167,   150,
     149,   167,   680,   150,   682,   175,   173,   167,    50,    57,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   167,   669,   167,   149,   175,   169,   143,   144,   167,
     169,   445,   148,   680,   169,   151,   152,   153,    55,   155,
      98,   836,   158,   159,    58,    56,   724,    74,   809,   240,
     166,   747,   168,   599,   170,   171,   172,     1,   174,   682,
     176,   366,   659,   464,   112,   113,   274,   520,   173,    68,
     283,   379,   120,   441,   387,   857,    -1,   724,   727,    -1,
      -1,    -1,    -1,    -1,   713,    -1,   669,   736,   230,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   680,    -1,   682,
     148,   243,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   166,   768,
      -1,   263,    -1,    -1,    -1,   173,    -1,   805,   176,    -1,
      -1,    -1,    -1,    20,    21,   277,    -1,    -1,    -1,   817,
      -1,   724,    -1,   285,    -1,    -1,    -1,   289,    -1,   291,
     799,    -1,    -1,    -1,    -1,    -1,    32,   299,   805,    -1,
     302,   303,   304,    -1,    -1,    -1,   308,    -1,    -1,    -1,
     817,    -1,    32,    -1,   316,   317,   318,    -1,   320,   321,
     322,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   331,
      -1,   101,   102,   103,   104,    -1,    -1,    57,    -1,    -1,
     878,   111,    -1,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,   122,    -1,    -1,   893,   103,   104,    -1,    -1,
      -1,    -1,   805,    -1,   111,    -1,   113,   114,   115,   116,
      -1,   878,   119,    -1,   817,    -1,    -1,   113,    -1,    -1,
      -1,   151,   152,   153,   154,   155,   893,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   164,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   406,    -1,    -1,   143,   144,    -1,
      -1,   413,   148,    -1,   150,   151,   859,   164,   420,   166,
      -1,    -1,   424,   143,   144,    -1,    -1,    -1,   148,   431,
     166,   151,   875,    -1,    -1,   878,    -1,   439,    -1,   441,
     176,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
     893,    -1,    -1,   173,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,    -1,    -1,
     482,   483,   484,   485,   486,   487,    -1,   489,   490,    -1,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
      -1,   513,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   524,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    20,    21,    -1,
     552,    -1,    -1,   555,   556,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   575,   576,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,   586,    -1,    -1,   589,   590,    -1,
      -1,   593,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    -1,   629,   101,   102,
     103,   104,   105,    -1,    -1,   108,   109,   110,   111,   641,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,   122,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    32,    -1,   665,    -1,    -1,    -1,    -1,   135,    -1,
      -1,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,    -1,   686,    -1,    57,    -1,    -1,    -1,
      -1,   164,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   704,   705,    -1,   172,    -1,    -1,    -1,   711,
      -1,   713,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   726,    -1,    -1,    -1,   730,    -1,
      -1,    -1,   734,    -1,    -1,    -1,   101,   102,   103,   104,
     105,   743,   113,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   776,    -1,    -1,   148,    -1,   144,
     151,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,   164,
     802,   166,   173,    -1,    -1,   176,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   818,    -1,   820,   821,
     822,    -1,    -1,   825,    -1,    -1,   828,   829,    -1,   831,
      -1,    -1,    -1,    -1,     1,    -1,   838,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,   868,    -1,    -1,    -1,
      37,    38,    -1,    -1,    41,    42,    -1,    -1,    45,   881,
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
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,   155,    -1,
      -1,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,   170,   171,   172,   173,   174,   175,     1,
      -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,
      42,    -1,    -1,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    -1,    -1,    60,    61,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,    -1,   155,    -1,    -1,   158,   159,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,   168,    -1,   170,   171,
     172,   173,   174,   175,    26,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,    -1,    -1,    -1,   148,    -1,    57,   151,
     152,   153,    -1,   155,    -1,    -1,   158,   159,    -1,    14,
      15,    16,    17,    -1,   166,    -1,   168,    -1,   170,   171,
     172,    26,   174,    -1,   176,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    60,    61,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    86,    87,    88,   143,   144,    91,    -1,    -1,   148,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    32,   151,   152,   153,    -1,
     155,    -1,    -1,   158,   159,    -1,    14,    15,    16,    17,
      -1,   166,    -1,   168,    -1,   170,   171,   172,    26,   174,
      57,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,   113,    85,    86,    87,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   143,   144,    -1,    -1,
     118,   148,    -1,   150,   151,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,   151,   152,   153,    -1,   155,    -1,    -1,
     158,   159,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
     168,    -1,   170,   171,   172,   173,   174,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   151,   152,   153,    -1,   155,    -1,    26,   158,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      38,   170,   171,   172,    42,   174,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,   151,   152,   153,    -1,   155,    -1,    -1,
     158,   159,    -1,    14,    15,    16,    17,    18,   166,    -1,
     168,   169,   170,   171,   172,    26,   174,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,   113,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,   143,   144,
      91,    -1,    -1,   148,    -1,   150,   151,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,   149,    -1,
     151,   152,   153,    -1,   155,    -1,    26,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    38,   170,
     171,   172,    42,   174,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    61,    62,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    -1,    85,    86,    87,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   151,   152,   153,    -1,   155,    -1,    26,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,    38,
     170,   171,   172,    42,   174,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    60,    61,    62,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   151,   152,   153,    -1,   155,    -1,    26,   158,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      38,   170,   171,   172,    42,   174,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    86,    87,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,   151,   152,   153,    -1,   155,    -1,    26,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    38,   170,   171,   172,    42,   174,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   151,   152,   153,    -1,   155,    -1,
      26,   158,   159,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    38,   170,   171,   172,    42,   174,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      86,    87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    -1,    -1,    32,   143,   144,    -1,    -1,    -1,   148,
      -1,   150,   151,    -1,    -1,   151,   152,   153,    -1,   155,
      -1,    -1,   158,   159,    -1,    -1,    -1,   166,    57,    -1,
      -1,    -1,   168,    -1,   170,   171,   172,   176,   174,    14,
      15,    16,    17,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    60,    61,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    86,    87,    88,   143,   144,    91,    -1,    -1,   148,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,   151,   152,   153,    -1,
     155,    -1,    26,   158,   159,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   168,    38,   170,   171,   172,    42,   174,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    60,    61,    62,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,   143,   144,
      -1,    -1,    -1,   148,    -1,    -1,   151,   151,   152,   153,
      -1,   155,    -1,    -1,   158,   159,    -1,    14,    15,    16,
      17,   166,    -1,    -1,   168,   169,   170,   171,   172,    26,
     174,   176,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,   143,   144,    -1,    -1,    -1,
     148,    -1,   150,   151,   151,   152,   153,    -1,   155,    -1,
      -1,   158,   159,    -1,    14,    15,    16,    17,   166,    -1,
      -1,   168,   169,   170,   171,   172,    26,   174,   176,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      60,    61,    62,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    -1,    85,    86,    87,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,   143,   144,    -1,    -1,    -1,   148,    -1,   150,
     151,   151,   152,   153,    -1,   155,    -1,    -1,   158,   159,
      -1,    14,    15,    16,    17,   166,    -1,    -1,   168,   169,
     170,   171,   172,    26,   174,   176,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    60,    61,    62,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,    -1,    -1,   148,    -1,   150,   151,   151,   152,
     153,    -1,   155,    -1,    -1,   158,   159,    -1,    14,    15,
      16,    17,   166,    -1,    -1,   168,   169,   170,   171,   172,
      26,   174,   176,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    60,    61,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      86,    87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,    -1,
      -1,   148,    -1,   150,   151,   151,   152,   153,    -1,   155,
      -1,    -1,   158,   159,    -1,    14,    15,    16,    17,   166,
      -1,    -1,   168,   169,   170,   171,   172,    26,   174,   176,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    60,    61,    62,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,   143,   144,    -1,    -1,    -1,   148,    -1,
     150,   151,   151,   152,   153,    -1,   155,    -1,    -1,   158,
     159,    -1,    14,    15,    16,    17,   166,    -1,    -1,   168,
     169,   170,   171,   172,    26,   174,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,   151,
     152,   153,    -1,   155,    -1,    26,   158,   159,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    38,   170,   171,
     172,    42,   174,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    86,    87,    88,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     151,   152,   153,    -1,   155,    -1,    26,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    38,   170,
     171,   172,    42,   174,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    61,    62,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    -1,    85,    86,    87,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   151,   152,   153,    -1,   155,    -1,    26,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    38,
     170,   171,   172,    42,   174,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    86,    87,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   151,   152,   153,    24,   155,    -1,    -1,   158,
     159,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    20,    21,   119,   120,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    20,    21,   119,   120,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      20,    21,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    20,    21,   119,   120,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,    -1,   119,   120,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   164,    -1,
     166,   101,   102,   103,   104,   105,    -1,   173,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   164,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,   173,   101,   102,   103,   104,   105,    -1,
      -1,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   164,    -1,   166,
      -1,    -1,   169,   101,   102,   103,   104,   105,    -1,    -1,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   164,    -1,   166,    -1,
      -1,   169,   101,   102,   103,   104,   105,    -1,    -1,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,   164,    -1,   166,    -1,    -1,
     169,   101,   102,   103,   104,   105,    -1,    -1,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   164,    -1,   166,    -1,    -1,   169,
     101,   102,   103,   104,   105,    -1,    -1,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   164,    -1,   166,    -1,    -1,   169,   101,
     102,   103,   104,   105,    -1,    -1,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   164,    -1,   166,    -1,    -1,   169,   101,   102,
     103,   104,   105,    -1,    -1,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,   166,    -1,    -1,   169,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    20,    21,    -1,
     172,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    20,    21,    -1,   172,   101,   102,
     103,   104,   105,    -1,    -1,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,   166,   167,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,   101,   102,   103,   104,   105,
      -1,    -1,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,
     166,   167,    20,    21,    -1,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,   166,   167,    -1,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,   122,   123,   124,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    32,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    57,    -1,   119,   120,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,   166,   101,   102,   103,   104,   105,    -1,
     113,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,    -1,    -1,    -1,   148,    -1,   144,   151,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    20,
      21,    -1,    -1,   166,    -1,    -1,    -1,   164,    -1,   166,
      -1,    -1,    -1,   176,   101,   102,   103,   104,   105,    -1,
      -1,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   166,
     101,   102,   103,   104,   105,    -1,    -1,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    20,    21,    -1,    -1,    -1,
      -1,    32,    -1,   164,    -1,   166,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,    57,    -1,   119,    -1,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,   166,   101,   102,   103,   104,
     105,    -1,   113,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,    -1,    -1,    -1,   148,    -1,   150,
     151,    -1,   147,   148,   149,   150,   151,   152,   153,   154,
     155,    20,    21,    -1,    -1,   166,    -1,    -1,    -1,   164,
      -1,   166,    -1,    -1,    -1,   176,   101,   102,   103,   104,
     105,    -1,    -1,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   148,   149,   150,   151,   152,   153,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,   166,   101,   102,   103,   104,   105,    -1,    -1,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,    32,    -1,
     119,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,   154,   155,    57,    -1,    32,
      -1,    -1,   101,   102,    -1,   164,   105,   166,    32,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,   122,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,   144,    -1,   146,   147,   148,
     149,   150,    -1,   113,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,   166,    -1,   143,
     144,    -1,    -1,    -1,   148,    -1,   150,   151,    -1,    -1,
     113,    -1,    -1,   143,   144,    -1,    -1,    -1,   148,   113,
     150,   151,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,   166,    -1,    -1,    -1,
     143,   144,    -1,    -1,    -1,   148,   176,   150,   151,   143,
     144,    -1,    -1,    -1,   148,    -1,   150,   151,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,   158,   159
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   178,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    69,    90,   166,   173,   179,   187,   188,   192,   211,
     215,   231,   282,   287,   288,   292,   328,   330,   135,   206,
     207,   129,   193,   194,   135,   189,   190,   135,   174,   135,
     331,   329,    33,   135,   172,   208,   209,   210,   222,     4,
       5,     7,   291,    58,   281,   143,   142,   145,   142,    21,
      54,   154,   164,   191,   145,   174,   135,   285,   286,   135,
     135,   172,   135,   168,   142,   167,    34,   103,   104,   135,
     214,   216,   217,   135,   290,   174,   283,    14,    15,   129,
     133,   135,   136,   168,   181,   206,   129,   194,   135,   135,
     135,    64,    68,    85,    86,    87,    88,   296,   284,   143,
     175,   285,   174,   174,   135,   207,   209,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   121,   122,   123,   124,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   158,
     159,    34,    34,   168,   212,   174,   218,   145,   289,   293,
     280,     1,   137,   180,    14,    15,   129,   133,   135,   181,
     204,   205,   191,   174,   135,   175,    24,    30,    46,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   222,   295,   297,   298,   300,   303,   135,   299,   135,
     273,   274,   169,   126,   169,   264,   269,   270,   145,   213,
     220,   222,   174,   135,   175,   279,   294,   173,   137,   138,
     142,   169,   284,   143,   173,   149,   149,   149,   149,   168,
     149,   149,   149,   149,   149,   149,    32,    57,   113,   143,
     144,   148,   151,   166,   173,   176,   173,   145,   173,   207,
      47,   276,   294,   169,   173,   303,     1,     8,     9,    10,
      12,    14,    15,    16,    17,    25,    26,    27,    28,    37,
      38,    41,    42,    45,    48,    49,    51,    52,    55,    56,
      69,    91,    92,   103,   104,   118,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   136,   151,   152,   153,   155,
     158,   159,   168,   170,   171,   173,   175,   183,   185,   195,
     196,   199,   200,   201,   202,   203,   218,   219,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   231,   232,   233,
     240,   249,   250,   251,   252,   253,   254,   255,   256,   261,
     295,   332,   336,   341,   343,   345,   267,   142,   145,   148,
     278,   204,   175,   261,   306,   308,   310,   304,   135,   301,
     312,   314,   316,   318,   320,   322,   324,   326,    32,    32,
     148,   166,   176,   167,   261,   135,   175,   303,   175,   273,
     167,   276,   112,   143,   145,   148,   275,   269,   261,   294,
     303,   332,   168,   112,   173,   221,   252,   261,   218,   261,
     168,   261,   149,   149,   168,   173,   149,   129,    55,   261,
     149,   112,   221,   261,   261,   261,   171,   250,    12,   303,
      12,   303,   261,   338,   342,   182,   261,   261,   261,   222,
     261,   261,   261,   171,    36,   173,   173,   261,   135,   168,
     173,   173,   173,    19,   230,   211,   117,   173,    20,    21,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   119,   120,   121,   122,   123,   124,
     143,   144,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   164,   166,   168,   175,   211,   264,   266,   135,
     303,   112,   120,   143,   277,   173,   303,   303,   303,   303,
     169,   299,   303,   212,   303,   212,   303,   212,   135,   271,
     272,   303,   274,   167,   167,   167,   303,   221,   261,   218,
      19,   168,   222,   221,   261,   112,   143,   173,    13,   261,
     218,   234,   236,   261,   238,   145,   129,   303,   221,   261,
     173,   171,   294,    50,   135,   151,   168,   261,   333,   334,
     335,   337,   338,   339,   340,   294,   168,   334,   340,   125,
     173,   175,   138,   139,   180,   186,   169,   149,   222,   174,
     218,   166,   169,   247,   261,    39,   279,   212,   250,    18,
     135,   135,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   135,   118,
     261,   261,   135,   261,   261,   261,   261,   261,   261,   261,
     261,   118,   261,    21,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   135,   261,   169,   247,     7,
      43,   265,   173,   143,   173,   277,   221,   261,   150,   173,
     150,   150,   150,   173,   150,   213,   150,   213,   150,   213,
     145,   150,   173,   150,   173,   143,   275,   247,   169,   247,
      18,   149,   261,   173,   218,   169,   303,   303,   169,   303,
     150,   173,    19,   250,   120,   275,   169,   142,   173,   335,
     167,   142,   167,   173,    19,   169,   335,   175,   261,   338,
     175,   261,   137,   184,   257,   259,   220,    11,    22,    23,
     197,   198,   135,   333,   142,   169,   213,   149,   168,   261,
     167,   261,   135,   145,   167,   169,    44,   265,   276,   221,
     261,   173,   307,   303,   311,   305,   302,   313,   150,   317,
     150,   321,   150,   303,   325,   271,   327,   261,   173,   169,
     149,   135,   169,   150,   150,   150,   168,   247,   261,   261,
     334,   335,   135,   333,   167,   167,   261,   167,   339,   247,
     334,   175,   167,   140,   137,   303,   212,   175,   218,   261,
     167,   261,   125,   218,   248,   262,   169,   247,   261,   268,
     217,   173,   150,   315,   319,   323,   218,   241,   150,   173,
     235,   237,   239,   169,   261,   173,   335,   167,   120,   275,
     167,   173,   335,   167,   150,   213,   218,   169,   112,   261,
     303,   169,   216,   309,   303,    18,   261,   135,   261,   261,
     261,   169,   261,   167,   167,   261,   261,   261,   175,   258,
     150,   198,   261,   150,   173,   150,   149,   169,   150,   173,
     344,   167,   344,   167,   222,   260,   263,   242,   243,    18,
     261,    50,   167,   175,   222,   169,   303,   149,   169,   261,
     167,   167,   150,   245,   244,   303,   169,   150,   246,   169
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   177,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   179,   180,   180,   181,   182,
     182,   182,   183,   184,   184,   186,   185,   187,   188,   189,
     189,   189,   190,   190,   191,   191,   192,   193,   193,   194,
     194,   195,   196,   196,   197,   197,   198,   198,   198,   199,
     199,   200,   201,   202,   203,   204,   204,   204,   204,   204,
     204,   205,   205,   206,   206,   206,   206,   206,   206,   206,
     206,   207,   207,   208,   208,   209,   209,   210,   210,   211,
     211,   212,   212,   212,   213,   213,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   216,   217,   218,   218,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   220,   220,   220,   221,
     221,   222,   222,   222,   223,   224,   224,   224,   224,   225,
     226,   227,   227,   227,   227,   227,   228,   228,   228,   228,
     229,   230,   230,   231,   231,   232,   234,   235,   233,   236,
     237,   233,   238,   239,   233,   240,   240,   240,   241,   242,
     240,   243,   244,   240,   245,   246,   240,   247,   247,   248,
     248,   248,   249,   249,   249,   250,   250,   251,   251,   251,
     251,   251,   251,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   253,
     253,   254,   255,   255,   256,   257,   258,   256,   259,   260,
     256,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   262,   263,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   264,   264,   265,
     265,   266,   267,   267,   268,   267,   267,   269,   269,   270,
     270,   271,   271,   272,   272,   273,   274,   274,   275,   275,
     276,   276,   276,   276,   276,   276,   277,   277,   277,   278,
     278,   279,   279,   279,   279,   279,   280,   280,   281,   281,
     282,   283,   282,   284,   284,   284,   285,   286,   286,   287,
     288,   288,   289,   289,   290,   291,   291,   293,   292,   294,
     294,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   296,   296,   296,   296,
     296,   296,   297,   298,   298,   299,   299,   301,   302,   300,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   304,   305,   303,   303,
     306,   307,   303,   308,   309,   303,   310,   311,   303,   303,
     312,   313,   303,   314,   315,   303,   303,   316,   317,   303,
     318,   319,   303,   303,   320,   321,   303,   322,   323,   303,
     324,   325,   303,   326,   327,   303,   329,   328,   331,   330,
     332,   332,   332,   332,   333,   333,   333,   333,   334,   334,
     335,   335,   336,   336,   336,   336,   336,   336,   337,   337,
     338,   339,   339,   340,   340,   341,   341,   342,   342,   343,
     344,   344,   345,   345
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     0,
       2,     4,     3,     1,     2,     0,     4,     2,     2,     1,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     4,     6,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       5,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     2,     3,     7,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     3,
       2,     1,     3,     2,     2,     2,     4,     5,     2,     1,
       1,     2,     3,     4,     2,     3,     3,     4,     2,     3,
       4,     0,     2,     1,     1,     3,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     5,     8,    10,     0,     0,
      10,     0,     0,    13,     0,     0,    15,     1,     3,     1,
       2,     3,     1,     1,     2,     1,     5,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     6,     5,     6,     3,     0,     0,     8,     0,     0,
       9,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     4,     3,     3,
       1,     3,     4,     3,     4,     2,     4,     4,     6,     7,
       3,     5,     0,     0,     8,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     0,     4,     0,
       1,     3,     0,     3,     0,     7,     5,     2,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     2,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     4,     6,     5,     5,     4,     0,     2,     0,     1,
       5,     0,     5,     0,     3,     5,     4,     1,     2,     4,
       5,     7,     0,     2,     2,     1,     1,     0,     7,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3,     0,     0,     6,
       1,     1,     1,     1,     4,     3,     4,     2,     3,     2,
       3,     2,     2,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     8,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     0,     7,     0,     7,
       1,     1,     1,     1,     3,     3,     5,     5,     1,     3,
       0,     2,     6,     5,     7,     8,     6,     8,     1,     3,
       3,     1,     1,     1,     3,     5,     5,     1,     3,     4,
       0,     3,    10,    10
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
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3290 "ds_parser.cpp"
        break;

    case 180: /* character_sequence  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3296 "ds_parser.cpp"
        break;

    case 181: /* string_constant  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3302 "ds_parser.cpp"
        break;

    case 182: /* string_builder_body  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3308 "ds_parser.cpp"
        break;

    case 183: /* string_builder  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3314 "ds_parser.cpp"
        break;

    case 185: /* expr_reader  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3320 "ds_parser.cpp"
        break;

    case 189: /* require_module_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3326 "ds_parser.cpp"
        break;

    case 195: /* expression_label  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3332 "ds_parser.cpp"
        break;

    case 196: /* expression_goto  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3338 "ds_parser.cpp"
        break;

    case 198: /* expression_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3344 "ds_parser.cpp"
        break;

    case 200: /* expression_if_then_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3350 "ds_parser.cpp"
        break;

    case 201: /* expression_for_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3356 "ds_parser.cpp"
        break;

    case 202: /* expression_while_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3362 "ds_parser.cpp"
        break;

    case 203: /* expression_with  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3368 "ds_parser.cpp"
        break;

    case 204: /* annotation_argument_value  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3374 "ds_parser.cpp"
        break;

    case 205: /* annotation_argument_value_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3380 "ds_parser.cpp"
        break;

    case 206: /* annotation_argument  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3386 "ds_parser.cpp"
        break;

    case 207: /* annotation_argument_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3392 "ds_parser.cpp"
        break;

    case 208: /* annotation_declaration_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3398 "ds_parser.cpp"
        break;

    case 209: /* annotation_declaration  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3404 "ds_parser.cpp"
        break;

    case 210: /* annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3410 "ds_parser.cpp"
        break;

    case 211: /* optional_annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3416 "ds_parser.cpp"
        break;

    case 212: /* optional_function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3422 "ds_parser.cpp"
        break;

    case 213: /* optional_function_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3428 "ds_parser.cpp"
        break;

    case 214: /* function_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3434 "ds_parser.cpp"
        break;

    case 218: /* expression_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3440 "ds_parser.cpp"
        break;

    case 219: /* expression_any  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3446 "ds_parser.cpp"
        break;

    case 220: /* expressions  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3452 "ds_parser.cpp"
        break;

    case 221: /* expr_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3458 "ds_parser.cpp"
        break;

    case 222: /* name_in_namespace  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3464 "ds_parser.cpp"
        break;

    case 223: /* expression_delete  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3470 "ds_parser.cpp"
        break;

    case 224: /* expr_new  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3476 "ds_parser.cpp"
        break;

    case 225: /* expression_break  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3482 "ds_parser.cpp"
        break;

    case 226: /* expression_continue  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3488 "ds_parser.cpp"
        break;

    case 227: /* expression_return  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3494 "ds_parser.cpp"
        break;

    case 228: /* expression_yield  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3500 "ds_parser.cpp"
        break;

    case 229: /* expression_try_catch  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3506 "ds_parser.cpp"
        break;

    case 232: /* expression_let  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3512 "ds_parser.cpp"
        break;

    case 233: /* expr_cast  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3518 "ds_parser.cpp"
        break;

    case 240: /* expr_type_info  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3524 "ds_parser.cpp"
        break;

    case 247: /* expr_list  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3530 "ds_parser.cpp"
        break;

    case 248: /* block_or_simple_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3536 "ds_parser.cpp"
        break;

    case 250: /* expr_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3542 "ds_parser.cpp"
        break;

    case 251: /* expr_numeric_const  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3548 "ds_parser.cpp"
        break;

    case 252: /* expr_assign  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3554 "ds_parser.cpp"
        break;

    case 253: /* expr_assign_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3560 "ds_parser.cpp"
        break;

    case 254: /* expr_named_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3566 "ds_parser.cpp"
        break;

    case 255: /* expr_method_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3572 "ds_parser.cpp"
        break;

    case 256: /* func_addr_expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3578 "ds_parser.cpp"
        break;

    case 261: /* expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3584 "ds_parser.cpp"
        break;

    case 264: /* optional_field_annotation  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3590 "ds_parser.cpp"
        break;

    case 266: /* structure_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3596 "ds_parser.cpp"
        break;

    case 267: /* struct_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3602 "ds_parser.cpp"
        break;

    case 269: /* function_argument_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3608 "ds_parser.cpp"
        break;

    case 270: /* function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3614 "ds_parser.cpp"
        break;

    case 271: /* tuple_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3620 "ds_parser.cpp"
        break;

    case 272: /* tuple_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3626 "ds_parser.cpp"
        break;

    case 273: /* variant_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3632 "ds_parser.cpp"
        break;

    case 274: /* variant_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3638 "ds_parser.cpp"
        break;

    case 276: /* variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3644 "ds_parser.cpp"
        break;

    case 279: /* let_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3650 "ds_parser.cpp"
        break;

    case 280: /* global_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3656 "ds_parser.cpp"
        break;

    case 284: /* enum_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3662 "ds_parser.cpp"
        break;

    case 289: /* optional_structure_parent  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3668 "ds_parser.cpp"
        break;

    case 294: /* variable_name_with_pos_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3674 "ds_parser.cpp"
        break;

    case 297: /* structure_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3680 "ds_parser.cpp"
        break;

    case 298: /* auto_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3686 "ds_parser.cpp"
        break;

    case 299: /* bitfield_bits  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3692 "ds_parser.cpp"
        break;

    case 300: /* bitfield_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3698 "ds_parser.cpp"
        break;

    case 303: /* type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3704 "ds_parser.cpp"
        break;

    case 332: /* make_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3710 "ds_parser.cpp"
        break;

    case 333: /* make_struct_fields  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3716 "ds_parser.cpp"
        break;

    case 334: /* make_struct_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3722 "ds_parser.cpp"
        break;

    case 335: /* optional_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3728 "ds_parser.cpp"
        break;

    case 336: /* make_struct_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3734 "ds_parser.cpp"
        break;

    case 337: /* make_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3740 "ds_parser.cpp"
        break;

    case 338: /* make_map_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3746 "ds_parser.cpp"
        break;

    case 339: /* make_any_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3752 "ds_parser.cpp"
        break;

    case 340: /* make_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3758 "ds_parser.cpp"
        break;

    case 341: /* make_dim_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3764 "ds_parser.cpp"
        break;

    case 342: /* make_table  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3770 "ds_parser.cpp"
        break;

    case 343: /* make_table_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3776 "ds_parser.cpp"
        break;

    case 344: /* array_comprehension_where  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3782 "ds_parser.cpp"
        break;

    case 345: /* array_comprehension  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3788 "ds_parser.cpp"
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
#line 436 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4088 "ds_parser.cpp"
    break;

  case 16:
#line 448 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4094 "ds_parser.cpp"
    break;

  case 17:
#line 449 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4100 "ds_parser.cpp"
    break;

  case 18:
#line 453 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4106 "ds_parser.cpp"
    break;

  case 19:
#line 457 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4116 "ds_parser.cpp"
    break;

  case 20:
#line 462 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4130 "ds_parser.cpp"
    break;

  case 21:
#line 471 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4140 "ds_parser.cpp"
    break;

  case 22:
#line 479 "ds_parser.ypp"
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
#line 4158 "ds_parser.cpp"
    break;

  case 23:
#line 495 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4168 "ds_parser.cpp"
    break;

  case 24:
#line 500 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4178 "ds_parser.cpp"
    break;

  case 25:
#line 508 "ds_parser.ypp"
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
#line 4205 "ds_parser.cpp"
    break;

  case 26:
#line 529 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4217 "ds_parser.cpp"
    break;

  case 27:
#line 538 "ds_parser.ypp"
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
#line 4241 "ds_parser.cpp"
    break;

  case 29:
#line 564 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4249 "ds_parser.cpp"
    break;

  case 30:
#line 567 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4260 "ds_parser.cpp"
    break;

  case 31:
#line 573 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4271 "ds_parser.cpp"
    break;

  case 32:
#line 582 "ds_parser.ypp"
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
#line 4297 "ds_parser.cpp"
    break;

  case 33:
#line 603 "ds_parser.ypp"
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
#line 4322 "ds_parser.cpp"
    break;

  case 34:
#line 626 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4328 "ds_parser.cpp"
    break;

  case 35:
#line 627 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4334 "ds_parser.cpp"
    break;

  case 39:
#line 640 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4342 "ds_parser.cpp"
    break;

  case 40:
#line 643 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4350 "ds_parser.cpp"
    break;

  case 41:
#line 649 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4358 "ds_parser.cpp"
    break;

  case 42:
#line 655 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4366 "ds_parser.cpp"
    break;

  case 43:
#line 658 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4374 "ds_parser.cpp"
    break;

  case 44:
#line 664 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4380 "ds_parser.cpp"
    break;

  case 45:
#line 665 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4386 "ds_parser.cpp"
    break;

  case 46:
#line 669 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4392 "ds_parser.cpp"
    break;

  case 47:
#line 670 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4398 "ds_parser.cpp"
    break;

  case 48:
#line 671 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4409 "ds_parser.cpp"
    break;

  case 49:
#line 680 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4415 "ds_parser.cpp"
    break;

  case 50:
#line 681 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4421 "ds_parser.cpp"
    break;

  case 51:
#line 685 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4432 "ds_parser.cpp"
    break;

  case 52:
#line 694 "ds_parser.ypp"
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
#line 4450 "ds_parser.cpp"
    break;

  case 53:
#line 710 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4462 "ds_parser.cpp"
    break;

  case 54:
#line 720 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4473 "ds_parser.cpp"
    break;

  case 55:
#line 729 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4479 "ds_parser.cpp"
    break;

  case 56:
#line 730 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4485 "ds_parser.cpp"
    break;

  case 57:
#line 731 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4491 "ds_parser.cpp"
    break;

  case 58:
#line 732 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4497 "ds_parser.cpp"
    break;

  case 59:
#line 733 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4503 "ds_parser.cpp"
    break;

  case 60:
#line 734 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4509 "ds_parser.cpp"
    break;

  case 61:
#line 738 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4519 "ds_parser.cpp"
    break;

  case 62:
#line 743 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4529 "ds_parser.cpp"
    break;

  case 63:
#line 751 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4535 "ds_parser.cpp"
    break;

  case 64:
#line 752 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4541 "ds_parser.cpp"
    break;

  case 65:
#line 753 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4547 "ds_parser.cpp"
    break;

  case 66:
#line 754 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4553 "ds_parser.cpp"
    break;

  case 67:
#line 755 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4559 "ds_parser.cpp"
    break;

  case 68:
#line 756 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4565 "ds_parser.cpp"
    break;

  case 69:
#line 757 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4571 "ds_parser.cpp"
    break;

  case 70:
#line 758 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4579 "ds_parser.cpp"
    break;

  case 71:
#line 764 "ds_parser.ypp"
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
#line 4597 "ds_parser.cpp"
    break;

  case 72:
#line 777 "ds_parser.ypp"
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
#line 4615 "ds_parser.cpp"
    break;

  case 73:
#line 793 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4621 "ds_parser.cpp"
    break;

  case 74:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4627 "ds_parser.cpp"
    break;

  case 75:
#line 798 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4639 "ds_parser.cpp"
    break;

  case 76:
#line 805 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4653 "ds_parser.cpp"
    break;

  case 77:
#line 817 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4662 "ds_parser.cpp"
    break;

  case 78:
#line 821 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4671 "ds_parser.cpp"
    break;

  case 79:
#line 828 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4677 "ds_parser.cpp"
    break;

  case 80:
#line 829 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4683 "ds_parser.cpp"
    break;

  case 81:
#line 833 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4689 "ds_parser.cpp"
    break;

  case 82:
#line 834 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4695 "ds_parser.cpp"
    break;

  case 83:
#line 835 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4701 "ds_parser.cpp"
    break;

  case 84:
#line 839 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4711 "ds_parser.cpp"
    break;

  case 85:
#line 844 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4720 "ds_parser.cpp"
    break;

  case 86:
#line 851 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4729 "ds_parser.cpp"
    break;

  case 87:
#line 855 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4735 "ds_parser.cpp"
    break;

  case 88:
#line 856 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4741 "ds_parser.cpp"
    break;

  case 89:
#line 857 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4747 "ds_parser.cpp"
    break;

  case 90:
#line 858 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4753 "ds_parser.cpp"
    break;

  case 91:
#line 859 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4759 "ds_parser.cpp"
    break;

  case 92:
#line 860 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4765 "ds_parser.cpp"
    break;

  case 93:
#line 861 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4771 "ds_parser.cpp"
    break;

  case 94:
#line 862 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4777 "ds_parser.cpp"
    break;

  case 95:
#line 863 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4783 "ds_parser.cpp"
    break;

  case 96:
#line 864 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4789 "ds_parser.cpp"
    break;

  case 97:
#line 865 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4795 "ds_parser.cpp"
    break;

  case 98:
#line 866 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4801 "ds_parser.cpp"
    break;

  case 99:
#line 867 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4807 "ds_parser.cpp"
    break;

  case 100:
#line 868 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4813 "ds_parser.cpp"
    break;

  case 101:
#line 869 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4819 "ds_parser.cpp"
    break;

  case 102:
#line 870 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4825 "ds_parser.cpp"
    break;

  case 103:
#line 871 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4831 "ds_parser.cpp"
    break;

  case 104:
#line 872 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4837 "ds_parser.cpp"
    break;

  case 105:
#line 873 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4843 "ds_parser.cpp"
    break;

  case 106:
#line 874 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4849 "ds_parser.cpp"
    break;

  case 107:
#line 875 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4855 "ds_parser.cpp"
    break;

  case 108:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4861 "ds_parser.cpp"
    break;

  case 109:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4867 "ds_parser.cpp"
    break;

  case 110:
#line 878 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4873 "ds_parser.cpp"
    break;

  case 111:
#line 879 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4879 "ds_parser.cpp"
    break;

  case 112:
#line 880 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4885 "ds_parser.cpp"
    break;

  case 113:
#line 881 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4891 "ds_parser.cpp"
    break;

  case 114:
#line 882 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4897 "ds_parser.cpp"
    break;

  case 115:
#line 883 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4903 "ds_parser.cpp"
    break;

  case 116:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4909 "ds_parser.cpp"
    break;

  case 117:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4915 "ds_parser.cpp"
    break;

  case 118:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4921 "ds_parser.cpp"
    break;

  case 119:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4927 "ds_parser.cpp"
    break;

  case 120:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4933 "ds_parser.cpp"
    break;

  case 121:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4939 "ds_parser.cpp"
    break;

  case 122:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4945 "ds_parser.cpp"
    break;

  case 123:
#line 894 "ds_parser.ypp"
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
#line 4968 "ds_parser.cpp"
    break;

  case 124:
#line 915 "ds_parser.ypp"
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
#line 5008 "ds_parser.cpp"
    break;

  case 125:
#line 953 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5017 "ds_parser.cpp"
    break;

  case 126:
#line 960 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5026 "ds_parser.cpp"
    break;

  case 127:
#line 964 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5039 "ds_parser.cpp"
    break;

  case 128:
#line 975 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5045 "ds_parser.cpp"
    break;

  case 129:
#line 976 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5051 "ds_parser.cpp"
    break;

  case 130:
#line 977 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5057 "ds_parser.cpp"
    break;

  case 131:
#line 978 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5063 "ds_parser.cpp"
    break;

  case 132:
#line 979 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5069 "ds_parser.cpp"
    break;

  case 133:
#line 980 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5075 "ds_parser.cpp"
    break;

  case 134:
#line 981 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5081 "ds_parser.cpp"
    break;

  case 135:
#line 982 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5087 "ds_parser.cpp"
    break;

  case 136:
#line 983 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5093 "ds_parser.cpp"
    break;

  case 137:
#line 984 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5099 "ds_parser.cpp"
    break;

  case 138:
#line 985 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5105 "ds_parser.cpp"
    break;

  case 139:
#line 986 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5111 "ds_parser.cpp"
    break;

  case 140:
#line 987 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5117 "ds_parser.cpp"
    break;

  case 141:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5123 "ds_parser.cpp"
    break;

  case 142:
#line 989 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5129 "ds_parser.cpp"
    break;

  case 143:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5135 "ds_parser.cpp"
    break;

  case 144:
#line 991 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5141 "ds_parser.cpp"
    break;

  case 145:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5147 "ds_parser.cpp"
    break;

  case 146:
#line 996 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5157 "ds_parser.cpp"
    break;

  case 147:
#line 1001 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5168 "ds_parser.cpp"
    break;

  case 148:
#line 1007 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5176 "ds_parser.cpp"
    break;

  case 149:
#line 1013 "ds_parser.ypp"
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
#line 5202 "ds_parser.cpp"
    break;

  case 150:
#line 1034 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5210 "ds_parser.cpp"
    break;

  case 151:
#line 1040 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5216 "ds_parser.cpp"
    break;

  case 152:
#line 1041 "ds_parser.ypp"
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
#line 5232 "ds_parser.cpp"
    break;

  case 153:
#line 1052 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5238 "ds_parser.cpp"
    break;

  case 154:
#line 1056 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5246 "ds_parser.cpp"
    break;

  case 155:
#line 1062 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5254 "ds_parser.cpp"
    break;

  case 156:
#line 1065 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5262 "ds_parser.cpp"
    break;

  case 157:
#line 1068 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5271 "ds_parser.cpp"
    break;

  case 158:
#line 1072 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5279 "ds_parser.cpp"
    break;

  case 159:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5285 "ds_parser.cpp"
    break;

  case 160:
#line 1082 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5291 "ds_parser.cpp"
    break;

  case 161:
#line 1086 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5299 "ds_parser.cpp"
    break;

  case 162:
#line 1089 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5307 "ds_parser.cpp"
    break;

  case 163:
#line 1092 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5317 "ds_parser.cpp"
    break;

  case 164:
#line 1097 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5325 "ds_parser.cpp"
    break;

  case 165:
#line 1100 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5335 "ds_parser.cpp"
    break;

  case 166:
#line 1108 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5343 "ds_parser.cpp"
    break;

  case 167:
#line 1111 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5353 "ds_parser.cpp"
    break;

  case 168:
#line 1116 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5361 "ds_parser.cpp"
    break;

  case 169:
#line 1119 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5371 "ds_parser.cpp"
    break;

  case 170:
#line 1127 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5379 "ds_parser.cpp"
    break;

  case 171:
#line 1133 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5385 "ds_parser.cpp"
    break;

  case 172:
#line 1134 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5391 "ds_parser.cpp"
    break;

  case 173:
#line 1138 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5397 "ds_parser.cpp"
    break;

  case 174:
#line 1139 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5403 "ds_parser.cpp"
    break;

  case 175:
#line 1143 "ds_parser.ypp"
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
#line 5440 "ds_parser.cpp"
    break;

  case 176:
#line 1178 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5446 "ds_parser.cpp"
    break;

  case 177:
#line 1178 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5452 "ds_parser.cpp"
    break;

  case 178:
#line 1178 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5460 "ds_parser.cpp"
    break;

  case 179:
#line 1181 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5466 "ds_parser.cpp"
    break;

  case 180:
#line 1181 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5472 "ds_parser.cpp"
    break;

  case 181:
#line 1181 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5482 "ds_parser.cpp"
    break;

  case 182:
#line 1186 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5488 "ds_parser.cpp"
    break;

  case 183:
#line 1186 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5494 "ds_parser.cpp"
    break;

  case 184:
#line 1186 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5504 "ds_parser.cpp"
    break;

  case 185:
#line 1194 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5513 "ds_parser.cpp"
    break;

  case 186:
#line 1198 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5523 "ds_parser.cpp"
    break;

  case 187:
#line 1203 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5534 "ds_parser.cpp"
    break;

  case 188:
#line 1209 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5540 "ds_parser.cpp"
    break;

  case 189:
#line 1209 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5546 "ds_parser.cpp"
    break;

  case 190:
#line 1209 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5555 "ds_parser.cpp"
    break;

  case 191:
#line 1213 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5561 "ds_parser.cpp"
    break;

  case 192:
#line 1213 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5567 "ds_parser.cpp"
    break;

  case 193:
#line 1213 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5577 "ds_parser.cpp"
    break;

  case 194:
#line 1218 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5583 "ds_parser.cpp"
    break;

  case 195:
#line 1218 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5589 "ds_parser.cpp"
    break;

  case 196:
#line 1218 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5600 "ds_parser.cpp"
    break;

  case 197:
#line 1227 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5608 "ds_parser.cpp"
    break;

  case 198:
#line 1230 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5616 "ds_parser.cpp"
    break;

  case 199:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5622 "ds_parser.cpp"
    break;

  case 200:
#line 1237 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5634 "ds_parser.cpp"
    break;

  case 201:
#line 1244 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5647 "ds_parser.cpp"
    break;

  case 202:
#line 1255 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5653 "ds_parser.cpp"
    break;

  case 203:
#line 1256 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5659 "ds_parser.cpp"
    break;

  case 204:
#line 1257 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5665 "ds_parser.cpp"
    break;

  case 205:
#line 1261 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5675 "ds_parser.cpp"
    break;

  case 206:
#line 1267 "ds_parser.ypp"
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
#line 5731 "ds_parser.cpp"
    break;

  case 207:
#line 1321 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5737 "ds_parser.cpp"
    break;

  case 208:
#line 1322 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5743 "ds_parser.cpp"
    break;

  case 209:
#line 1323 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5749 "ds_parser.cpp"
    break;

  case 210:
#line 1324 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5755 "ds_parser.cpp"
    break;

  case 211:
#line 1325 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5761 "ds_parser.cpp"
    break;

  case 212:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5767 "ds_parser.cpp"
    break;

  case 213:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5773 "ds_parser.cpp"
    break;

  case 214:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5779 "ds_parser.cpp"
    break;

  case 215:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5785 "ds_parser.cpp"
    break;

  case 216:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5791 "ds_parser.cpp"
    break;

  case 217:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5797 "ds_parser.cpp"
    break;

  case 218:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5803 "ds_parser.cpp"
    break;

  case 219:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5809 "ds_parser.cpp"
    break;

  case 220:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5815 "ds_parser.cpp"
    break;

  case 221:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5821 "ds_parser.cpp"
    break;

  case 222:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5827 "ds_parser.cpp"
    break;

  case 223:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5833 "ds_parser.cpp"
    break;

  case 224:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5839 "ds_parser.cpp"
    break;

  case 225:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5845 "ds_parser.cpp"
    break;

  case 226:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5851 "ds_parser.cpp"
    break;

  case 227:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5857 "ds_parser.cpp"
    break;

  case 228:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5863 "ds_parser.cpp"
    break;

  case 229:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5869 "ds_parser.cpp"
    break;

  case 230:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5875 "ds_parser.cpp"
    break;

  case 231:
#line 1354 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5887 "ds_parser.cpp"
    break;

  case 232:
#line 1365 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5897 "ds_parser.cpp"
    break;

  case 233:
#line 1370 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5909 "ds_parser.cpp"
    break;

  case 234:
#line 1380 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5918 "ds_parser.cpp"
    break;

  case 235:
#line 1384 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5924 "ds_parser.cpp"
    break;

  case 236:
#line 1384 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5930 "ds_parser.cpp"
    break;

  case 237:
#line 1384 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 5942 "ds_parser.cpp"
    break;

  case 238:
#line 1391 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5948 "ds_parser.cpp"
    break;

  case 239:
#line 1391 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5954 "ds_parser.cpp"
    break;

  case 240:
#line 1391 "ds_parser.ypp"
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
#line 5970 "ds_parser.cpp"
    break;

  case 241:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5976 "ds_parser.cpp"
    break;

  case 242:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5982 "ds_parser.cpp"
    break;

  case 243:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5988 "ds_parser.cpp"
    break;

  case 244:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5994 "ds_parser.cpp"
    break;

  case 245:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6000 "ds_parser.cpp"
    break;

  case 246:
#line 1410 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6006 "ds_parser.cpp"
    break;

  case 247:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6012 "ds_parser.cpp"
    break;

  case 248:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6018 "ds_parser.cpp"
    break;

  case 249:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6024 "ds_parser.cpp"
    break;

  case 250:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6030 "ds_parser.cpp"
    break;

  case 251:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6036 "ds_parser.cpp"
    break;

  case 252:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6042 "ds_parser.cpp"
    break;

  case 253:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6048 "ds_parser.cpp"
    break;

  case 254:
#line 1418 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6054 "ds_parser.cpp"
    break;

  case 255:
#line 1419 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6060 "ds_parser.cpp"
    break;

  case 256:
#line 1420 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6066 "ds_parser.cpp"
    break;

  case 257:
#line 1421 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6072 "ds_parser.cpp"
    break;

  case 258:
#line 1422 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6078 "ds_parser.cpp"
    break;

  case 259:
#line 1423 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6084 "ds_parser.cpp"
    break;

  case 260:
#line 1424 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6090 "ds_parser.cpp"
    break;

  case 261:
#line 1425 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6096 "ds_parser.cpp"
    break;

  case 262:
#line 1426 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6102 "ds_parser.cpp"
    break;

  case 263:
#line 1427 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6108 "ds_parser.cpp"
    break;

  case 264:
#line 1428 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6114 "ds_parser.cpp"
    break;

  case 265:
#line 1429 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6120 "ds_parser.cpp"
    break;

  case 266:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6126 "ds_parser.cpp"
    break;

  case 267:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6132 "ds_parser.cpp"
    break;

  case 268:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6138 "ds_parser.cpp"
    break;

  case 269:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6144 "ds_parser.cpp"
    break;

  case 270:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6150 "ds_parser.cpp"
    break;

  case 271:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6156 "ds_parser.cpp"
    break;

  case 272:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6162 "ds_parser.cpp"
    break;

  case 273:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6168 "ds_parser.cpp"
    break;

  case 274:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6174 "ds_parser.cpp"
    break;

  case 275:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6180 "ds_parser.cpp"
    break;

  case 276:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6186 "ds_parser.cpp"
    break;

  case 277:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6192 "ds_parser.cpp"
    break;

  case 278:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6198 "ds_parser.cpp"
    break;

  case 279:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6204 "ds_parser.cpp"
    break;

  case 280:
#line 1444 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6210 "ds_parser.cpp"
    break;

  case 281:
#line 1445 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6219 "ds_parser.cpp"
    break;

  case 282:
#line 1449 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6228 "ds_parser.cpp"
    break;

  case 283:
#line 1453 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6236 "ds_parser.cpp"
    break;

  case 284:
#line 1456 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6244 "ds_parser.cpp"
    break;

  case 285:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6250 "ds_parser.cpp"
    break;

  case 286:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6256 "ds_parser.cpp"
    break;

  case 287:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6262 "ds_parser.cpp"
    break;

  case 288:
#line 1462 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6272 "ds_parser.cpp"
    break;

  case 289:
#line 1467 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6282 "ds_parser.cpp"
    break;

  case 290:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6288 "ds_parser.cpp"
    break;

  case 291:
#line 1473 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6296 "ds_parser.cpp"
    break;

  case 292:
#line 1476 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6302 "ds_parser.cpp"
    break;

  case 293:
#line 1476 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6308 "ds_parser.cpp"
    break;

  case 294:
#line 1476 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6316 "ds_parser.cpp"
    break;

  case 295:
#line 1479 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6325 "ds_parser.cpp"
    break;

  case 296:
#line 1483 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6334 "ds_parser.cpp"
    break;

  case 297:
#line 1487 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6343 "ds_parser.cpp"
    break;

  case 298:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6349 "ds_parser.cpp"
    break;

  case 299:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6355 "ds_parser.cpp"
    break;

  case 300:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6361 "ds_parser.cpp"
    break;

  case 301:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6367 "ds_parser.cpp"
    break;

  case 302:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6373 "ds_parser.cpp"
    break;

  case 303:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6379 "ds_parser.cpp"
    break;

  case 304:
#line 1497 "ds_parser.ypp"
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
#line 6394 "ds_parser.cpp"
    break;

  case 305:
#line 1507 "ds_parser.ypp"
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
#line 6409 "ds_parser.cpp"
    break;

  case 306:
#line 1517 "ds_parser.ypp"
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
#line 6466 "ds_parser.cpp"
    break;

  case 307:
#line 1572 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6472 "ds_parser.cpp"
    break;

  case 308:
#line 1573 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6478 "ds_parser.cpp"
    break;

  case 309:
#line 1577 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6484 "ds_parser.cpp"
    break;

  case 310:
#line 1578 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6490 "ds_parser.cpp"
    break;

  case 311:
#line 1582 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6500 "ds_parser.cpp"
    break;

  case 312:
#line 1590 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6508 "ds_parser.cpp"
    break;

  case 313:
#line 1593 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6517 "ds_parser.cpp"
    break;

  case 314:
#line 1598 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6523 "ds_parser.cpp"
    break;

  case 315:
#line 1598 "ds_parser.ypp"
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
#line 6572 "ds_parser.cpp"
    break;

  case 316:
#line 1642 "ds_parser.ypp"
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
#line 6630 "ds_parser.cpp"
    break;

  case 317:
#line 1698 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6640 "ds_parser.cpp"
    break;

  case 318:
#line 1703 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6650 "ds_parser.cpp"
    break;

  case 319:
#line 1711 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6656 "ds_parser.cpp"
    break;

  case 320:
#line 1712 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6662 "ds_parser.cpp"
    break;

  case 321:
#line 1716 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6670 "ds_parser.cpp"
    break;

  case 322:
#line 1719 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6681 "ds_parser.cpp"
    break;

  case 323:
#line 1728 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6687 "ds_parser.cpp"
    break;

  case 324:
#line 1729 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6693 "ds_parser.cpp"
    break;

  case 325:
#line 1733 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6704 "ds_parser.cpp"
    break;

  case 326:
#line 1742 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6710 "ds_parser.cpp"
    break;

  case 327:
#line 1743 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6716 "ds_parser.cpp"
    break;

  case 328:
#line 1748 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6722 "ds_parser.cpp"
    break;

  case 329:
#line 1749 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6728 "ds_parser.cpp"
    break;

  case 330:
#line 1753 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6739 "ds_parser.cpp"
    break;

  case 331:
#line 1759 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6750 "ds_parser.cpp"
    break;

  case 332:
#line 1765 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6758 "ds_parser.cpp"
    break;

  case 333:
#line 1768 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6767 "ds_parser.cpp"
    break;

  case 334:
#line 1772 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6778 "ds_parser.cpp"
    break;

  case 335:
#line 1778 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6789 "ds_parser.cpp"
    break;

  case 336:
#line 1787 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6795 "ds_parser.cpp"
    break;

  case 337:
#line 1788 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6801 "ds_parser.cpp"
    break;

  case 338:
#line 1789 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6807 "ds_parser.cpp"
    break;

  case 339:
#line 1793 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6813 "ds_parser.cpp"
    break;

  case 340:
#line 1794 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6819 "ds_parser.cpp"
    break;

  case 341:
#line 1798 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6827 "ds_parser.cpp"
    break;

  case 342:
#line 1801 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6837 "ds_parser.cpp"
    break;

  case 343:
#line 1806 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6847 "ds_parser.cpp"
    break;

  case 344:
#line 1811 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6860 "ds_parser.cpp"
    break;

  case 345:
#line 1819 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6873 "ds_parser.cpp"
    break;

  case 346:
#line 1830 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6881 "ds_parser.cpp"
    break;

  case 347:
#line 1833 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6890 "ds_parser.cpp"
    break;

  case 348:
#line 1840 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6896 "ds_parser.cpp"
    break;

  case 349:
#line 1841 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6902 "ds_parser.cpp"
    break;

  case 350:
#line 1845 "ds_parser.ypp"
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
#line 6934 "ds_parser.cpp"
    break;

  case 351:
#line 1872 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6943 "ds_parser.cpp"
    break;

  case 352:
#line 1875 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6954 "ds_parser.cpp"
    break;

  case 353:
#line 1884 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6962 "ds_parser.cpp"
    break;

  case 354:
#line 1887 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6976 "ds_parser.cpp"
    break;

  case 355:
#line 1896 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6990 "ds_parser.cpp"
    break;

  case 356:
#line 1909 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7004 "ds_parser.cpp"
    break;

  case 360:
#line 1930 "ds_parser.ypp"
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
#line 7020 "ds_parser.cpp"
    break;

  case 361:
#line 1941 "ds_parser.ypp"
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
#line 7037 "ds_parser.cpp"
    break;

  case 362:
#line 1956 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7043 "ds_parser.cpp"
    break;

  case 363:
#line 1957 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7049 "ds_parser.cpp"
    break;

  case 364:
#line 1961 "ds_parser.ypp"
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
#line 7088 "ds_parser.cpp"
    break;

  case 365:
#line 1998 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7094 "ds_parser.cpp"
    break;

  case 366:
#line 1999 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7100 "ds_parser.cpp"
    break;

  case 367:
#line 2003 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7106 "ds_parser.cpp"
    break;

  case 368:
#line 2003 "ds_parser.ypp"
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
#line 7192 "ds_parser.cpp"
    break;

  case 369:
#line 2087 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7204 "ds_parser.cpp"
    break;

  case 370:
#line 2094 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7215 "ds_parser.cpp"
    break;

  case 371:
#line 2103 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7221 "ds_parser.cpp"
    break;

  case 372:
#line 2104 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7227 "ds_parser.cpp"
    break;

  case 373:
#line 2105 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7233 "ds_parser.cpp"
    break;

  case 374:
#line 2106 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7239 "ds_parser.cpp"
    break;

  case 375:
#line 2107 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7245 "ds_parser.cpp"
    break;

  case 376:
#line 2108 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7251 "ds_parser.cpp"
    break;

  case 377:
#line 2109 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7257 "ds_parser.cpp"
    break;

  case 378:
#line 2110 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7263 "ds_parser.cpp"
    break;

  case 379:
#line 2111 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7269 "ds_parser.cpp"
    break;

  case 380:
#line 2112 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7275 "ds_parser.cpp"
    break;

  case 381:
#line 2113 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7281 "ds_parser.cpp"
    break;

  case 382:
#line 2114 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7287 "ds_parser.cpp"
    break;

  case 383:
#line 2115 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7293 "ds_parser.cpp"
    break;

  case 384:
#line 2116 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7299 "ds_parser.cpp"
    break;

  case 385:
#line 2117 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7305 "ds_parser.cpp"
    break;

  case 386:
#line 2118 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7311 "ds_parser.cpp"
    break;

  case 387:
#line 2119 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7317 "ds_parser.cpp"
    break;

  case 388:
#line 2120 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7323 "ds_parser.cpp"
    break;

  case 389:
#line 2121 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7329 "ds_parser.cpp"
    break;

  case 390:
#line 2122 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7335 "ds_parser.cpp"
    break;

  case 391:
#line 2123 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7341 "ds_parser.cpp"
    break;

  case 392:
#line 2124 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7347 "ds_parser.cpp"
    break;

  case 393:
#line 2125 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7353 "ds_parser.cpp"
    break;

  case 394:
#line 2126 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7359 "ds_parser.cpp"
    break;

  case 395:
#line 2127 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7365 "ds_parser.cpp"
    break;

  case 396:
#line 2131 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7371 "ds_parser.cpp"
    break;

  case 397:
#line 2132 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7377 "ds_parser.cpp"
    break;

  case 398:
#line 2133 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7383 "ds_parser.cpp"
    break;

  case 399:
#line 2134 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7389 "ds_parser.cpp"
    break;

  case 400:
#line 2135 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7395 "ds_parser.cpp"
    break;

  case 401:
#line 2136 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7401 "ds_parser.cpp"
    break;

  case 402:
#line 2140 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7414 "ds_parser.cpp"
    break;

  case 403:
#line 2151 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7423 "ds_parser.cpp"
    break;

  case 404:
#line 2155 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7435 "ds_parser.cpp"
    break;

  case 405:
#line 2165 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7447 "ds_parser.cpp"
    break;

  case 406:
#line 2172 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7458 "ds_parser.cpp"
    break;

  case 407:
#line 2181 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7464 "ds_parser.cpp"
    break;

  case 408:
#line 2181 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7470 "ds_parser.cpp"
    break;

  case 409:
#line 2181 "ds_parser.ypp"
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
#line 7485 "ds_parser.cpp"
    break;

  case 410:
#line 2194 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7491 "ds_parser.cpp"
    break;

  case 411:
#line 2195 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7497 "ds_parser.cpp"
    break;

  case 412:
#line 2196 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7503 "ds_parser.cpp"
    break;

  case 413:
#line 2197 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7509 "ds_parser.cpp"
    break;

  case 414:
#line 2198 "ds_parser.ypp"
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
#line 7528 "ds_parser.cpp"
    break;

  case 415:
#line 2212 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7539 "ds_parser.cpp"
    break;

  case 416:
#line 2218 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7548 "ds_parser.cpp"
    break;

  case 417:
#line 2222 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7558 "ds_parser.cpp"
    break;

  case 418:
#line 2227 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7568 "ds_parser.cpp"
    break;

  case 419:
#line 2232 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7578 "ds_parser.cpp"
    break;

  case 420:
#line 2237 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7588 "ds_parser.cpp"
    break;

  case 421:
#line 2242 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7597 "ds_parser.cpp"
    break;

  case 422:
#line 2246 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7606 "ds_parser.cpp"
    break;

  case 423:
#line 2250 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7616 "ds_parser.cpp"
    break;

  case 424:
#line 2255 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7625 "ds_parser.cpp"
    break;

  case 425:
#line 2259 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7635 "ds_parser.cpp"
    break;

  case 426:
#line 2264 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7641 "ds_parser.cpp"
    break;

  case 427:
#line 2264 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7647 "ds_parser.cpp"
    break;

  case 428:
#line 2264 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7658 "ds_parser.cpp"
    break;

  case 429:
#line 2270 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7670 "ds_parser.cpp"
    break;

  case 430:
#line 2277 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7676 "ds_parser.cpp"
    break;

  case 431:
#line 2277 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7682 "ds_parser.cpp"
    break;

  case 432:
#line 2277 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7692 "ds_parser.cpp"
    break;

  case 433:
#line 2282 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7698 "ds_parser.cpp"
    break;

  case 434:
#line 2282 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7704 "ds_parser.cpp"
    break;

  case 435:
#line 2282 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7715 "ds_parser.cpp"
    break;

  case 436:
#line 2288 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7721 "ds_parser.cpp"
    break;

  case 437:
#line 2288 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7727 "ds_parser.cpp"
    break;

  case 438:
#line 2288 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7737 "ds_parser.cpp"
    break;

  case 439:
#line 2293 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7746 "ds_parser.cpp"
    break;

  case 440:
#line 2297 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7752 "ds_parser.cpp"
    break;

  case 441:
#line 2297 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7758 "ds_parser.cpp"
    break;

  case 442:
#line 2297 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7768 "ds_parser.cpp"
    break;

  case 443:
#line 2302 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7774 "ds_parser.cpp"
    break;

  case 444:
#line 2302 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7780 "ds_parser.cpp"
    break;

  case 445:
#line 2302 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7794 "ds_parser.cpp"
    break;

  case 446:
#line 2311 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7803 "ds_parser.cpp"
    break;

  case 447:
#line 2315 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7809 "ds_parser.cpp"
    break;

  case 448:
#line 2315 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7815 "ds_parser.cpp"
    break;

  case 449:
#line 2315 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7825 "ds_parser.cpp"
    break;

  case 450:
#line 2320 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7831 "ds_parser.cpp"
    break;

  case 451:
#line 2320 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7837 "ds_parser.cpp"
    break;

  case 452:
#line 2320 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7851 "ds_parser.cpp"
    break;

  case 453:
#line 2329 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7860 "ds_parser.cpp"
    break;

  case 454:
#line 2333 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7866 "ds_parser.cpp"
    break;

  case 455:
#line 2333 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7872 "ds_parser.cpp"
    break;

  case 456:
#line 2333 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7882 "ds_parser.cpp"
    break;

  case 457:
#line 2338 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7888 "ds_parser.cpp"
    break;

  case 458:
#line 2338 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7894 "ds_parser.cpp"
    break;

  case 459:
#line 2338 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7908 "ds_parser.cpp"
    break;

  case 460:
#line 2347 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7914 "ds_parser.cpp"
    break;

  case 461:
#line 2347 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7920 "ds_parser.cpp"
    break;

  case 462:
#line 2347 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7931 "ds_parser.cpp"
    break;

  case 463:
#line 2353 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7937 "ds_parser.cpp"
    break;

  case 464:
#line 2353 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7943 "ds_parser.cpp"
    break;

  case 465:
#line 2353 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7954 "ds_parser.cpp"
    break;

  case 466:
#line 2362 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7960 "ds_parser.cpp"
    break;

  case 467:
#line 2362 "ds_parser.ypp"
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
#line 7977 "ds_parser.cpp"
    break;

  case 468:
#line 2377 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7983 "ds_parser.cpp"
    break;

  case 469:
#line 2377 "ds_parser.ypp"
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
#line 8004 "ds_parser.cpp"
    break;

  case 470:
#line 2397 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8010 "ds_parser.cpp"
    break;

  case 471:
#line 2398 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8016 "ds_parser.cpp"
    break;

  case 472:
#line 2399 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8022 "ds_parser.cpp"
    break;

  case 473:
#line 2400 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8028 "ds_parser.cpp"
    break;

  case 474:
#line 2404 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8040 "ds_parser.cpp"
    break;

  case 475:
#line 2411 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8052 "ds_parser.cpp"
    break;

  case 476:
#line 2418 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8063 "ds_parser.cpp"
    break;

  case 477:
#line 2424 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8074 "ds_parser.cpp"
    break;

  case 478:
#line 2433 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8084 "ds_parser.cpp"
    break;

  case 479:
#line 2438 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8093 "ds_parser.cpp"
    break;

  case 480:
#line 2445 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8099 "ds_parser.cpp"
    break;

  case 481:
#line 2446 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8105 "ds_parser.cpp"
    break;

  case 482:
#line 2450 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8116 "ds_parser.cpp"
    break;

  case 483:
#line 2456 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8128 "ds_parser.cpp"
    break;

  case 484:
#line 2463 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8141 "ds_parser.cpp"
    break;

  case 485:
#line 2471 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8153 "ds_parser.cpp"
    break;

  case 486:
#line 2478 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8166 "ds_parser.cpp"
    break;

  case 487:
#line 2486 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8180 "ds_parser.cpp"
    break;

  case 488:
#line 2498 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8188 "ds_parser.cpp"
    break;

  case 489:
#line 2501 "ds_parser.ypp"
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
#line 8204 "ds_parser.cpp"
    break;

  case 490:
#line 2515 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8215 "ds_parser.cpp"
    break;

  case 491:
#line 2524 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8221 "ds_parser.cpp"
    break;

  case 492:
#line 2525 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8227 "ds_parser.cpp"
    break;

  case 493:
#line 2529 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8237 "ds_parser.cpp"
    break;

  case 494:
#line 2534 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8246 "ds_parser.cpp"
    break;

  case 495:
#line 2541 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8256 "ds_parser.cpp"
    break;

  case 496:
#line 2546 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8269 "ds_parser.cpp"
    break;

  case 497:
#line 2557 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8279 "ds_parser.cpp"
    break;

  case 498:
#line 2562 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8288 "ds_parser.cpp"
    break;

  case 499:
#line 2569 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8302 "ds_parser.cpp"
    break;

  case 500:
#line 2581 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8308 "ds_parser.cpp"
    break;

  case 501:
#line 2582 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8314 "ds_parser.cpp"
    break;

  case 502:
#line 2586 "ds_parser.ypp"
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
#line 8337 "ds_parser.cpp"
    break;

  case 503:
#line 2604 "ds_parser.ypp"
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
#line 8360 "ds_parser.cpp"
    break;


#line 8364 "ds_parser.cpp"

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
#line 2624 "ds_parser.ypp"


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
