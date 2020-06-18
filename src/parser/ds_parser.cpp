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
    COLCOL = 383,
    BRABRAB = 384,
    BRACBRB = 385,
    CBRCBRB = 386,
    INTEGER = 387,
    LONG_INTEGER = 388,
    UNSIGNED_INTEGER = 389,
    UNSIGNED_LONG_INTEGER = 390,
    FLOAT = 391,
    DOUBLE = 392,
    NAME = 393,
    BEGIN_STRING = 394,
    STRING_CHARACTER = 395,
    END_STRING = 396,
    BEGIN_STRING_EXPR = 397,
    END_STRING_EXPR = 398,
    END_OF_READ = 399,
    UNARY_MINUS = 400,
    UNARY_PLUS = 401,
    PRE_INC = 402,
    PRE_DEC = 403,
    POST_INC = 404,
    POST_DEC = 405,
    DEREF = 406
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
#define YYLAST   7219

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
       2,     2,     2,   162,     2,   178,   173,   158,   151,     2,
     171,   172,   156,   155,   145,   154,   167,   157,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   148,   175,
     152,   146,   153,   147,   174,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   169,     2,   170,   150,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   176,   149,   177,   161,     2,     2,     2,
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
     159,   160,   163,   164,   165,   166,   168
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   425,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,   441,   453,   454,   458,   462,
     467,   476,   484,   500,   505,   513,   513,   543,   565,   569,
     572,   578,   587,   608,   631,   632,   636,   640,   641,   645,
     648,   654,   660,   663,   669,   670,   674,   675,   676,   685,
     686,   690,   699,   715,   723,   733,   742,   743,   744,   745,
     746,   747,   751,   756,   764,   765,   766,   767,   768,   769,
     770,   771,   777,   790,   806,   807,   811,   818,   830,   834,
     841,   842,   846,   847,   848,   852,   857,   864,   868,   869,
     870,   871,   872,   873,   874,   875,   876,   877,   878,   879,
     880,   881,   882,   883,   884,   885,   886,   887,   888,   889,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   901,   902,   903,   907,   928,   966,   973,   977,   988,
     989,   990,   991,   992,   993,   994,   995,   996,   997,   998,
     999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,  1010,  1015,
    1021,  1027,  1048,  1054,  1055,  1066,  1070,  1076,  1079,  1082,
    1086,  1092,  1096,  1100,  1103,  1106,  1111,  1114,  1122,  1125,
    1130,  1133,  1141,  1147,  1148,  1152,  1153,  1157,  1192,  1192,
    1192,  1195,  1195,  1195,  1200,  1200,  1200,  1208,  1212,  1217,
    1223,  1223,  1223,  1227,  1227,  1227,  1232,  1232,  1232,  1241,
    1244,  1250,  1251,  1258,  1269,  1270,  1271,  1275,  1280,  1335,
    1336,  1337,  1338,  1339,  1340,  1344,  1345,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,
    1359,  1363,  1364,  1368,  1379,  1384,  1394,  1398,  1398,  1398,
    1405,  1405,  1405,  1419,  1423,  1423,  1423,  1431,  1432,  1433,
    1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1475,  1479,
    1482,  1485,  1486,  1487,  1488,  1493,  1498,  1499,  1502,  1502,
    1502,  1505,  1509,  1513,  1517,  1518,  1519,  1520,  1521,  1522,
    1523,  1533,  1543,  1598,  1599,  1603,  1604,  1608,  1616,  1619,
    1624,  1623,  1668,  1726,  1731,  1739,  1740,  1744,  1747,  1756,
    1757,  1761,  1770,  1771,  1776,  1777,  1781,  1787,  1793,  1796,
    1800,  1806,  1815,  1816,  1817,  1821,  1822,  1826,  1829,  1834,
    1839,  1847,  1858,  1861,  1868,  1869,  1873,  1900,  1900,  1912,
    1915,  1924,  1937,  1949,  1950,  1954,  1958,  1969,  1984,  1985,
    1989,  2026,  2027,  2031,  2031,  2115,  2122,  2131,  2132,  2133,
    2134,  2135,  2136,  2137,  2138,  2139,  2140,  2141,  2142,  2143,
    2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,
    2154,  2155,  2159,  2160,  2161,  2162,  2163,  2164,  2168,  2179,
    2183,  2193,  2200,  2209,  2209,  2209,  2222,  2223,  2224,  2225,
    2226,  2240,  2246,  2250,  2254,  2259,  2264,  2269,  2274,  2278,
    2282,  2287,  2291,  2296,  2296,  2296,  2302,  2309,  2309,  2309,
    2314,  2314,  2314,  2320,  2320,  2320,  2325,  2329,  2329,  2329,
    2334,  2334,  2334,  2343,  2347,  2347,  2347,  2352,  2352,  2352,
    2361,  2365,  2365,  2365,  2370,  2370,  2370,  2379,  2379,  2379,
    2385,  2385,  2385,  2394,  2394,  2409,  2409,  2429,  2430,  2431,
    2432,  2436,  2443,  2450,  2456,  2465,  2470,  2477,  2478,  2482,
    2488,  2495,  2503,  2510,  2518,  2530,  2533,  2547,  2556,  2557,
    2561,  2566,  2573,  2578,  2589,  2594,  2601,  2613,  2614,  2618,
    2636
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
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"", "','", "'='", "'?'",
  "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'",
  "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
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
     395,   396,   397,   398,   399,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   400,
     401,   126,    33,   402,   403,   404,   405,    46,   406,    91,
      93,    40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -581

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-581)))

#define YYTABLE_NINF -465

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -581,    31,  -581,  -581,   -80,   -67,   -38,   -21,   -47,  -581,
      35,  -581,  -581,    -1,  -581,  -581,  -581,  -581,  -581,   190,
    -581,   180,  -581,  -581,  -581,  -581,  -581,  -581,     2,  -581,
      62,   108,   117,  -581,  -581,    14,  -581,   -85,   155,  -581,
     162,   172,  -581,   176,   215,   174,  -581,   -65,  -581,  -581,
    -581,     5,   211,  -581,   177,    27,   -80,   218,   -67,   214,
    -581,   217,   220,  -581,   171,  -581,   216,  -581,   -90,   193,
     194,  -581,   234,   -80,    -1,  -581,  7057,   340,   342,  -581,
     207,   204,  -581,   233,  -581,  -581,   381,  -581,  -581,  -581,
    -581,  -581,   243,    89,  -581,  -581,  -581,  -581,   330,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,   213,   -89,  6982,
    -581,  -581,   248,   249,  -581,   -20,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,   -93,   245,  -581,  -581,    85,  -581,   221,
     -83,   219,  -581,   149,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,   -12,  -581,  -581,   -61,  -581,   238,   239,   244,   253,
    -581,  -581,  -581,   235,  -581,  -581,  -581,  -581,  -581,   255,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,   257,
    -581,  -581,  -581,   262,   263,  -581,  -581,  -581,  -581,   265,
     268,  -581,  -581,  -581,  -581,  -581,  2245,  -581,   246,   275,
    -581,   250,  -581,   -80,  -581,    -3,  -581,    82,  6982,  -581,
    1246,  -581,  -581,  -581,  -581,  -581,   -76,  -581,  -581,  -581,
      89,  -581,   -82,  4464,  -581,  -581,  -581,  -581,  -581,   286,
    -581,    99,   128,   159,  -581,  -581,  -581,  -581,  -581,  -581,
     395,  -581,  -581,    41,  2427,  -581,  -581,   -79,  6982,   -64,
     -19,   290,  -581,   100,  -581,   300,  4659,  -581,  4464,  -581,
    -581,   290,  -581,  -581,  6905,   259,  1873,  -581,  -581,   204,
    4464,   260,  4464,   280,   281,   266,  -581,   264,   282,   306,
    2554,   204,  -581,   292,  2024,  4464,  4464,   158,  6747,  6828,
    4464,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  4464,  4464,
    4464,    85,  4464,  4464,  4464,  -581,   267,  -581,   410,  -581,
    -581,   272,   273,  4464,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,   -88,   279,  -581,   284,   291,  -581,  -581,  -581,
     436,  -581,  -581,  -581,   287,  -581,  -581,   -74,  -581,  -581,
    -581,  -581,  -581,  5927,   294,  -581,  -581,  -581,  -581,  -581,
       7,   319,  6982,  -581,    -7,  -581,  -581,   903,  6982,  6982,
    6982,  6982,   296,   248,  6982,   207,  6982,   207,  6982,   207,
    7059,   249,  -581,  -581,  -581,   299,  -581,  -581,  5600,  -581,
    -581,  4659,  -581,  -581,   301,  -581,  -581,  -581,  6982,  -581,
    2681,  -581,   823,     9,   173,  -581,    85,  2681,  -581,  -581,
     351,  4546,   463,  6205,  4464,   823,  -581,  -581,  4464,  -581,
    -581,   329,   346,  6205,  -581,  6982,  2681,  -581,  4645,    69,
      69,   305,  -581,   290,   458,   290,  1575,  5994,  -581,   151,
     196,    69,    69,   -63,  -581,    69,    69,  5108,   -71,   304,
    -581,  -581,   823,  -581,  2151,  -581,  -581,  -581,   442,   290,
     207,   158,  -581,    20,   344,  4464,  4464,  4464,  4464,  4464,
    4464,  4464,  4464,  4464,  4464,  -581,  -581,  4464,  4464,  4464,
    4464,  4464,  4464,   345,  2808,  4464,   347,  4464,  4464,  4464,
    4464,  4464,  4464,  4464,  4464,  2935,  3062,  4464,  4464,  4464,
    4464,  4464,  4464,  4464,  4464,  4464,  4464,   349,  4464,  3189,
    -581,   481,   446,   318,  -581,   954,  -581,  -581,  -581,  2681,
    -581,   210,  3283,   289,  1723,  -581,    36,  3432,   245,  3581,
     245,  3730,   245,   101,  -581,   110,  4659,   116,  -581,  -581,
    -581,  3879,  -581,  5820,  -581,  4464,  3338,  2300,  -581,  4744,
    4464,  4464,  -581,   204,  5180,  -581,  6982,  6982,  5250,  6982,
    -581,  -581,  4054,  -581,  4843,  -581,  -581,    10,   158,    55,
    1724,  3487,  5994,   350,   -29,   327,   353,  -581,  -581,   -25,
      11,  3636,   -29,   164,  4464,  4464,   322,  -581,  4464,   361,
     362,  -581,   169,  -581,  -581,   242,   367,  -581,    -4,  6205,
    -581,  -581,   245,  -581,   355,  -581,  -581,  6205,  6205,  6205,
    6205,  6205,  6205,  6205,  6205,   627,   627,   295,  6205,  6205,
     295,  6371,  6371,   338,  4464,  6205,     3,  -581,  5669,   -70,
     -70,  6205,   627,   627,  6205,  6205,  4464,  6205,   373,  6066,
     650,  6272,  6344,   295,   295,   930,   930,     3,     3,     3,
    -581,   511,  5739,  -581,    54,    33,  -581,   290,  -581,   395,
    -581,  2681,  -581,  4942,  -581,  6982,  -581,  -581,  -581,   375,
    -581,   365,  -581,   366,  -581,   370,  6982,  -581,  7059,  -581,
     249,   395,  4464,   -33,  -581,    63,   387,   376,  5320,  -581,
    -581,  -581,  6451,  6461,  -581,  6501,   372,  -581,  4464,  -581,
    4464,  4464,   -10,   406,   367,   379,   380,  4464,   382,  4464,
    4464,   367,   378,   388,  6205,  -581,  -581,  6133,  -581,   417,
    6982,   207,  1423,   204,  -581,  -581,  4464,  -581,    18,    73,
    4464,  -581,   -94,  -581,  3785,  6432,  -581,  6432,  -581,  4464,
    -581,  -581,  -581,  -581,     5,  -581,  -581,  5041,  -581,  -581,
    6520,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    4659,  -581,  -581,  -581,  6205,   204,  -581,  -581,   148,  -581,
    -581,  -581,  -581,  3934,     4,  6205,  6205,   -29,   389,    74,
     350,   390,  -581,  6205,  -581,  -581,    16,   -29,   391,  -581,
    -581,  -581,  6530,   245,  -581,  -581,   823,   393,  6205,  4083,
    -581,  -581,  6982,  -581,    72,  6205,  -581,     5,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  6982,  4210,   424,  4464,  4464,
    4464,  -581,  5390,  4464,   396,   397,  4464,  4464,  -581,  4464,
     394,  -581,  -581,   415,   242,  -581,  4464,  6205,  6560,  -581,
     399,  -581,  6579,   418,  5460,   419,   295,   295,   295,  -581,
    1044,   405,  -581,  6205,  6205,  1044,   407,    85,  -581,  -581,
    6205,  -581,  -581,  -581,  -581,  -581,  4337,   526,   408,  -581,
     402,  -581,  -581,    85,  -581,   409,  6982,   431,  5530,  4464,
     414,   428,  -581,  -581,  6600,  -581,  -581,  6205,  -581,  -581,
    -581,  6982,   413,  6628,  -581,  -581,   427,  -581
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
       0,     0,    75,     0,   153,    76,    78,     0,    74,   372,
     371,     0,     0,   355,   357,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   359,     0,   363,     0,     0,
       0,   155,     0,     0,     0,    81,     0,     0,     0,    87,
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
    -581,  -581,  -581,   152,   546,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,   505,  -581,  -581,   548,  -581,  -581,
    -581,  -237,  -581,  -581,  -581,  -581,  -581,  -581,   368,  -581,
     554,   -56,  -581,   541,  -581,  -284,  -377,  -520,  -581,  -581,
    -200,  -136,   -77,  -581,    17,  -279,   -13,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,   621,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -504,  -581,  -581,  -291,  -581,  -228,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,   403,  -581,
    -581,   256,   -42,  -581,  -581,  -581,   360,  -581,   -60,  -581,
    -264,   247,  -272,  -265,   105,  -581,   156,  -581,  -581,  -581,
    -581,   464,   571,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -151,  -106,  -581,  -581,  -581,   258,  -581,  -581,  -581,
     -75,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,
    -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,  -581,   356,
    -580,  -435,  -565,  -581,  -581,  -297,   -69,   197,  -581,  -581,
    -581,  -223,  -581
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
     683,   578,   685,   473,   474,   437,   739,   722,   555,   708,
     720,     2,    42,   809,   216,    59,   223,     3,   614,    76,
     578,    87,    88,   493,   271,   471,   496,   497,    66,   174,
     463,   693,   695,   496,   497,   233,   174,    43,    28,   399,
       4,    48,     5,    64,     6,    31,     7,    44,    60,   371,
     470,     8,   372,   393,   219,   373,   666,   753,     9,   224,
      74,   602,   155,   464,    10,   243,   521,   110,   175,   473,
     474,    65,   742,   612,   234,   376,   211,   517,   400,   518,
      34,   472,    11,   164,   165,    75,   518,   526,   485,   486,
      77,    78,   740,   402,   244,   527,   493,    37,   495,   496,
     497,   498,   212,    12,   499,    56,    56,    43,   738,    38,
     413,   552,   406,   240,   790,   233,   223,    44,   558,   528,
     710,   740,   775,    79,   231,   718,   714,   788,    55,   740,
     719,   404,   222,   276,   371,   371,   371,   573,   615,    89,
     241,   740,   212,    90,   407,    91,    92,   270,   741,   406,
     517,    61,   518,    39,   485,   486,    13,   710,   212,   833,
     613,    62,   493,    72,   520,   496,   497,   498,   406,   678,
     499,   839,   394,   401,    49,    50,   836,    51,    93,   740,
      13,   407,   212,   212,   784,   256,    14,    56,   740,   414,
     395,   679,   422,    43,   406,   211,   796,   740,   713,   396,
     407,   166,   834,    44,   434,   167,   752,   168,    92,    72,
     257,   258,   840,   444,   446,   776,   517,   101,   518,    53,
     814,   102,   256,   807,   849,   371,   407,  -450,   408,   686,
     672,   409,  -450,   733,   274,   211,    57,   275,   103,   104,
     105,   106,    58,   687,   734,   735,   212,   257,   258,   689,
    -450,   211,   212,   212,   212,   212,  -457,   787,   212,   692,
     212,  -457,   212,   843,   212,   688,   797,   709,   259,   238,
     239,   690,   577,    66,   590,   211,   211,   525,   725,  -457,
      69,   826,   212,   531,   532,   533,   534,  -464,   454,   537,
      70,   539,  -464,   541,    71,   473,   474,  -240,   236,   260,
     261,   256,  -240,   827,   262,   259,   595,   263,   596,   212,
    -464,   325,   441,   551,   155,   554,   162,   597,   598,   719,
    -240,   723,   264,    72,   556,    73,   257,   258,   565,    83,
      96,   266,    98,    85,   803,    99,   260,   261,   100,   211,
     572,   262,   109,   674,   263,   211,   211,   211,   211,   112,
     113,   211,   114,   211,   151,   211,   152,   211,   153,   264,
     155,   157,   161,   162,    60,   605,   217,   219,   266,   173,
     245,   246,   756,   228,   237,   211,   247,   232,   483,   484,
     485,   486,   755,   557,   259,   248,   249,   250,   493,   251,
     495,   496,   497,   498,   252,   253,   499,   254,   501,   502,
     255,   267,   211,   268,   382,   269,   403,   392,   233,   223,
     416,   424,   426,   427,   430,   260,   261,   428,   431,   429,
     262,   458,   676,   263,   435,   603,   459,   460,   461,   512,
     513,   514,   515,   516,   465,   468,    13,   524,   264,   466,
     212,   212,   517,   212,   518,   519,   467,   266,   535,   548,
     471,   550,   282,   283,   284,   285,   563,   570,   571,   576,
     604,   610,   616,   633,   287,   637,   700,   660,   665,   666,
     256,   702,   703,   668,   705,   713,   291,   716,   717,   726,
     293,   238,   728,   294,   357,   738,   295,   743,   578,   744,
     298,   748,   750,   399,   778,   257,   258,   837,   765,   767,
     180,   181,   182,   769,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   777,
     200,   201,   202,   783,   789,   205,   206,   207,   208,   791,
     792,   303,   794,   211,   211,   798,   211,   801,   799,   835,
     838,   841,   855,   305,   306,   845,   861,   862,   868,   212,
     874,   866,   876,   259,   872,   879,   889,   881,   890,   891,
     212,   893,   212,   895,   898,   904,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   579,   317,   899,   907,
     760,    94,   599,   172,   260,   261,    97,   869,   375,   262,
      95,   770,   580,   319,   320,   116,   321,   850,   818,   322,
     323,   732,    21,   754,   212,   611,   522,   264,   772,   581,
     671,   325,   326,   363,   155,   411,   266,   242,   547,   111,
     415,   536,   880,   593,     0,     0,   377,   473,   474,     0,
     795,     0,     0,     0,     0,   802,   805,     0,     0,     0,
       0,     0,   211,     0,     0,   810,     0,   398,     0,     0,
     473,   474,     0,   211,     0,   211,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,   421,
       0,     0,     0,   423,     0,   425,     0,     0,   824,     0,
       0,     0,     0,   433,     0,     0,   212,   438,   439,   440,
       0,     0,     0,   447,     0,     0,     0,   211,     0,   212,
       0,   451,   452,   453,     0,   455,   456,   457,     0,   844,
       0,     0,   485,   486,     0,     0,   462,   848,     0,     0,
     493,     0,   495,   496,   497,   498,     0,     0,   499,     0,
     852,     0,     0,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
     212,   512,   513,   514,   515,   516,     0,     0,     0,     0,
       0,     0,     0,     0,   517,   212,   518,     0,     0,   211,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
       0,   894,   211,   553,     0,     0,     0,   517,     0,   518,
     559,     0,     0,     0,     0,     0,   903,   564,     0,     0,
       0,   568,     0,     0,     0,     0,     0,     0,     0,   574,
       0,     0,     0,   473,   474,     0,     0,   582,     0,   582,
       0,     0,     0,     0,   882,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     892,     0,     0,   211,     0,     0,     0,     0,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   211,     0,
     627,   628,   629,   630,   631,   632,     0,   635,   636,     0,
     638,   639,   640,   641,   642,   643,   644,   645,   647,   649,
     650,   651,   652,   653,   654,   655,   656,   657,   658,   659,
       0,   662,     0,   473,   474,     0,   483,   484,   485,   486,
     487,     0,   673,   490,   491,   492,   493,     0,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,     0,     0,
     473,   474,     0,     0,     0,     0,     0,     0,     0,     0,
     698,     0,     0,   635,   647,     0,     0,     0,     0,     0,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,   451,   457,     0,   256,     0,     0,     0,
     517,     0,   518,     0,   457,     0,     0,   724,   447,   155,
       0,   727,     0,     0,     0,     0,   483,   484,   485,   486,
     487,   257,   258,   490,   491,   492,   493,     0,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,     0,     0,
       0,     0,     0,     0,     0,   485,   486,   745,     0,     0,
       0,     0,     0,   493,     0,   495,   496,   497,   498,   747,
     506,   499,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,     0,   473,   474,     0,     0,   526,   259,
     517,     0,   518,     0,   757,     0,   527,     0,   530,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
       0,     0,     0,     0,     0,   774,     0,   517,     0,   518,
     669,   261,     0,     0,     0,   262,     0,     0,   263,     0,
       0,     0,     0,   785,   786,     0,     0,     0,     0,     0,
     793,     0,   582,   264,     0,     0,     0,     0,     0,   670,
       0,     0,   266,     0,     0,   363,     0,     0,     0,   806,
       0,     0,     0,   808,     0,     0,     0,   483,   484,   485,
     486,   487,   815,     0,   490,   491,   492,   493,     0,   495,
     496,   497,   498,     0,     0,   499,     0,   501,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   832,     0,     0,     0,
       0,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,     0,     0,     0,     0,     0,     0,     0,
       0,   517,   847,   518,     0,     0,     0,     0,     0,   877,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   854,
       0,   856,   857,   858,     0,     0,   860,     0,     0,   863,
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
       0,     0,     0,     0,    43,   308,   309,   310,   311,   312,
     313,   314,   315,   316,    44,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,   319,   320,     0,   321,     0,     0,   322,   323,     0,
       0,     0,     0,     0,     0,     0,     0,   324,     0,   325,
     326,   327,   155,   328,   277,     0,     0,     0,     0,     3,
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
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    44,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,   319,   320,
       0,   321,     0,     0,   322,   323,     0,     0,     0,   282,
     283,   284,   285,     0,   324,     0,   325,   326,   327,   155,
     804,   287,     0,     0,     0,     0,     0,   256,     0,     0,
       0,     0,     0,   291,     0,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,   257,   258,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   579,   317,     0,     0,     0,     0,     0,
       0,   260,   261,     0,     0,     0,   262,     0,     0,   580,
     319,   320,     0,   321,     0,     0,   322,   323,   282,   283,
     284,   285,     0,     0,   264,     0,   591,     0,   325,   326,
     287,   155,     0,   266,     0,   256,   393,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
     257,   258,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,     0,     0,     0,     0,   260,
     261,     0,     0,     0,   262,   394,   677,   263,   318,   319,
     320,     0,   321,     0,     0,   322,   323,   282,   283,   284,
     285,     0,   264,   395,     0,   324,     0,   325,   326,   287,
     155,   266,   396,     0,     0,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   417,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    44,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,   319,   320,
       0,   321,     0,     0,   322,   323,     0,     0,   282,   283,
     284,   285,     0,     0,   324,     0,   325,   326,   418,   155,
     287,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,   282,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   287,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,     0,   291,
       0,     0,     0,   293,     0,   324,   294,   325,   326,   295,
     155,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,   258,     0,   318,   319,   320,     0,   321,
       0,     0,   322,   323,   282,   283,   284,   285,   696,     0,
     606,     0,   324,   607,   325,   326,   287,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   291,     0,
       0,     0,   293,     0,     0,   294,     0,     0,   295,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
     259,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   302,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,   260,   261,   303,     0,     0,   262,     0,     0,   263,
       0,     0,     0,     0,     0,   305,   306,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
     265,     0,     0,   266,     0,     0,     0,     0,    43,   308,
     309,   310,   311,   312,   313,   314,   315,   316,    44,   317,
       0,   282,   283,   284,   285,     0,     0,     0,     0,     0,
       0,     0,   697,   287,   318,   319,   320,     0,   321,     0,
       0,   322,   323,     0,     0,   291,     0,     0,     0,   293,
       0,   324,   294,   325,   326,   295,   155,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,   282,   283,
     284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
     287,   318,   319,   320,     0,   321,     0,     0,   322,   323,
       0,     0,   291,     0,     0,     0,   293,   397,   324,   294,
     325,   326,   295,   155,     0,     0,   298,     0,     0,   432,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,   282,   283,   284,   285,     0,
       0,     0,     0,     0,     0,     0,     0,   287,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,     0,   291,
       0,     0,     0,   293,     0,   324,   294,   325,   326,   295,
     155,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   287,   318,   319,   320,     0,   321,
       0,     0,   322,   323,     0,     0,   291,     0,     0,     0,
     293,     0,   324,   294,   325,   326,   295,   155,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   634,     0,
       0,     0,     0,     0,     0,     0,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    44,   317,     0,   282,
     283,   284,   285,     0,     0,     0,     0,     0,     0,     0,
       0,   287,   318,   319,   320,     0,   321,     0,     0,   322,
     323,     0,     0,   291,     0,     0,     0,   293,     0,   324,
     294,   325,   326,   295,   155,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   646,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,   282,   283,   284,   285,
       0,     0,     0,   648,     0,     0,     0,     0,   287,   318,
     319,   320,     0,   321,     0,     0,   322,   323,     0,     0,
     291,     0,     0,     0,   293,     0,   324,   294,   325,   326,
     295,   155,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,     0,   184,   185,
     186,   187,   188,   302,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,   200,   201,   202,     0,     0,   205,
     206,   207,   208,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   308,   309,   310,   311,   312,   313,   314,   315,   316,
      44,   317,     0,   282,   283,   284,   285,     0,     0,     0,
       0,     0,     0,     0,     0,   287,   318,   319,   320,     0,
     321,     0,     0,   322,   323,     0,     0,   291,     0,     0,
       0,   293,     0,   324,   294,   325,   326,   295,   155,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,    43,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    44,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,   258,     0,   318,   319,   320,     0,   321,     0,     0,
     322,   323,   282,   283,   284,   285,     0,     0,     0,     0,
     324,   663,   325,   326,   287,   155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   291,     0,     0,     0,
     293,     0,     0,   294,     0,     0,   295,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,   259,     0,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,   260,
     261,   303,     0,     0,   262,     0,     0,   263,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,   675,     0,
       0,   266,     0,     0,   256,     0,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    44,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     258,     0,   318,   319,   320,     0,   321,     0,     0,   322,
     323,   282,   283,   284,   285,     0,     0,     0,     0,   324,
     694,   325,   326,   287,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,   293,
       0,     0,   294,     0,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   260,   261,
     303,     0,     0,   262,     0,   680,   263,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,   256,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   257,   258,
       0,   318,   319,   320,     0,   321,     0,     0,   322,   323,
     282,   283,   284,   285,     0,     0,     0,     0,   324,   712,
     325,   326,   287,   155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,   293,     0,
       0,   294,     0,     0,   295,     0,     0,     0,   298,     0,
       0,     0,     0,     0,     0,     0,   259,     0,   180,   181,
     182,     0,   184,   185,   186,   187,   188,   302,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,   200,   201,
     202,     0,     0,   205,   206,   207,   208,   260,   261,   303,
       0,     0,   262,     0,   682,   263,     0,     0,     0,     0,
       0,   305,   306,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,     0,   256,     0,    43,   308,   309,   310,   311,   312,
     313,   314,   315,   316,    44,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,   258,     0,
     318,   319,   320,     0,   321,     0,     0,   322,   323,   282,
     283,   284,   285,     0,     0,     0,     0,   324,   721,   325,
     326,   287,   155,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,   260,   261,   303,     0,
       0,   262,     0,   684,   263,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,   256,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   257,   258,     0,   318,
     319,   320,     0,   321,     0,     0,   322,   323,   282,   283,
     284,   285,     0,     0,     0,     0,   324,   813,   325,   326,
     287,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,   293,     0,     0,   294,
       0,     0,   295,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,   406,   259,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,   691,   261,   303,     0,     0,
     262,     0,     0,   263,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   318,   319,
     320,     0,   321,     0,     0,   322,   323,   282,   283,   284,
     285,     0,     0,     0,     0,   324,   831,   325,   326,   287,
     155,   257,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,   259,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   846,     0,     0,
     260,   261,     0,     0,     0,   262,     0,   706,   263,     0,
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    44,   317,   264,   282,   283,   284,   285,   853,     0,
       0,     0,   266,     0,     0,     0,   287,   318,   319,   320,
       0,   321,     0,     0,   322,   323,     0,     0,   291,     0,
       0,     0,   293,     0,   324,   294,   325,   326,   295,   155,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,     0,   184,   185,   186,   187,
     188,   302,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,   200,   201,   202,     0,     0,   205,   206,   207,
     208,     0,     0,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   306,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,   308,
     309,   310,   311,   312,   313,   314,   315,   316,    44,   317,
       0,   282,   283,   284,   285,   887,     0,     0,     0,     0,
       0,     0,     0,   287,   318,   319,   320,     0,   321,     0,
       0,   322,   323,     0,     0,   291,     0,     0,     0,   293,
       0,   324,   294,   325,   326,   295,   155,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,   282,   283,
     284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
     287,   318,   319,   320,     0,   321,     0,     0,   322,   323,
       0,     0,   291,     0,     0,     0,   293,     0,   324,   294,
     325,   326,   295,   155,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,   473,   474,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,   325,   326,     0,
     155,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     560,   495,   496,   497,   498,   473,   474,   499,   500,   501,
     502,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,   561,   506,     0,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,     0,     0,     0,     0,     0,
       0,     0,     0,   517,     0,   518,   257,   258,     0,     0,
       0,   562,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   560,
     495,   496,   497,   498,   473,   474,   499,   500,   501,   502,
     503,   504,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   506,     0,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,     0,   260,   261,     0,     0,     0,
     262,     0,   517,   263,   518,     0,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   560,   495,
     496,   497,   498,   473,   474,   499,   500,   501,   502,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,     0,     0,     0,     0,     0,     0,     0,
       0,   517,     0,   518,     0,     0,     0,     0,     0,   699,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   560,   495,   496,
     497,   498,   473,   474,   499,   500,   501,   502,   503,   504,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,     0,     0,     0,     0,     0,   707,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   560,   495,   496,   497,
     498,   473,   474,   499,   500,   501,   502,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   506,
       0,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,     0,     0,     0,     0,     0,     0,     0,     0,   517,
       0,   518,     0,     0,     0,     0,     0,   758,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,   474,
       0,     0,     0,     0,     0,     0,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   560,   495,   496,   497,   498,
       0,     0,   499,   500,   501,   502,   503,   504,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   506,     0,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     473,   474,     0,     0,     0,     0,     0,     0,   517,     0,
     518,   483,   484,   485,   486,   487,   819,     0,   490,   491,
     492,   493,     0,   495,   496,   497,   498,     0,     0,   499,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,     0,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,     0,     0,     0,
     473,   474,     0,     0,     0,   517,     0,   518,     0,     0,
     601,     0,     0,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   701,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   704,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   779,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   859,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
     473,   474,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   875,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   473,
     474,     0,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,   896,   483,   484,   485,   486,   487,     0,     0,
     490,   491,   492,   493,     0,   495,   496,   497,   498,     0,
       0,   499,     0,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   473,
     474,     0,     0,     0,     0,     0,     0,   517,     0,   518,
     549,     0,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,     0,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   518,   746,
     473,   474,   483,   484,   485,   486,   487,     0,     0,   490,
     491,   492,   493,     0,   495,   496,   497,   498,     0,     0,
     499,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,     0,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   518,   751,
       0,     0,     0,     0,     0,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   560,   495,   496,   497,   498,  -215,
       0,   499,   500,   501,   502,   503,   504,   473,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   506,     0,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,     0,
       0,     0,     0,     0,     0,     0,     0,   517,     0,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,   474,     0,     0,     0,     0,
       0,     0,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,     0,     0,   499,   500,
     501,   502,   503,   504,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   505,   506,     0,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   473,   474,     0,     0,
       0,     0,     0,     0,   517,     0,   518,   483,   484,   485,
     486,   487,     0,     0,   490,   491,   492,   493,     0,   495,
     496,   497,   498,     0,     0,   499,     0,   501,   502,     0,
       0,   594,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,     0,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   473,   474,     0,     0,     0,     0,     0,
       0,   517,     0,   518,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   487,     0,     0,   490,   491,   492,   493,
       0,   495,   496,   497,   498,     0,     0,   499,     0,   501,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   749,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   473,   474,     0,     0,     0,
       0,     0,     0,   517,     0,   518,   483,   484,   485,   486,
     487,     0,     0,   490,   491,   492,   493,     0,   495,   496,
     497,   498,     0,     0,   499,     0,   501,   502,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   800,     0,     0,     0,
     506,     0,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   473,   474,     0,     0,     0,     0,     0,     0,
     517,     0,   518,     0,     0,     0,     0,     0,   483,   484,
     485,   486,   487,     0,     0,   490,   491,   492,   493,     0,
     495,   496,   497,   498,     0,     0,   499,     0,   501,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,     0,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   473,   474,     0,     0,     0,     0,
       0,     0,   517,     0,   518,   483,   484,   485,   486,   487,
       0,     0,   490,   491,   492,   493,     0,   495,   496,   497,
     498,   473,   474,   499,     0,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,   514,   515,
     516,     0,     0,     0,     0,     0,     0,     0,     0,   517,
       0,   518,     0,     0,     0,     0,     0,   483,   484,   485,
     486,   487,   473,   474,   490,   491,   492,   493,     0,   495,
     496,   497,   498,     0,     0,   499,     0,   501,   502,     0,
       0,     0,     0,     0,   483,   484,   485,   486,   487,     0,
       0,   490,     0,   256,   493,     0,   495,   496,   497,   498,
       0,     0,   499,   256,   501,   502,   510,   511,   512,   513,
     514,   515,   516,     0,     0,     0,     0,     0,   257,   258,
       0,   517,     0,   518,     0,     0,     0,     0,   257,   258,
       0,     0,     0,   510,   511,   512,   513,   514,   515,   516,
       0,     0,     0,   256,     0,   483,   484,     0,   517,   487,
     518,     0,   490,   491,   492,   493,     0,   495,   496,   497,
     498,     0,   256,   499,     0,   501,   502,     0,   257,   258,
       0,     0,   256,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   257,   258,   506,
       0,   507,   508,   509,   510,   511,     0,   257,   258,   515,
       0,     0,   256,     0,     0,     0,     0,   260,   261,   517,
       0,   518,   262,     0,   780,   263,     0,   260,   261,     0,
       0,   256,   262,     0,   781,   263,   259,   257,   258,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     264,     0,   256,     0,     0,   259,   257,   258,     0,   266,
       0,     0,     0,     0,     0,   259,     0,   260,   261,     0,
       0,     0,   262,     0,   782,   263,     0,   257,   258,     0,
     256,     0,     0,     0,     0,     0,   260,   261,     0,     0,
     264,   262,     0,   820,   263,   259,   260,   261,     0,   266,
       0,   262,     0,   842,   263,   257,   258,     0,     0,   264,
       0,     0,     0,     0,   259,     0,     0,     0,   266,   264,
       0,     0,     0,     0,     0,     0,   260,   261,   266,     0,
       0,   262,     0,   871,   263,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,   261,     0,     0,   264,
     262,     0,   873,   263,     0,     0,     0,     0,   266,     0,
       0,     0,     0,   259,     0,     0,   260,   261,   264,     0,
       0,   262,     0,   900,   263,     0,     0,   266,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,   176,     0,     0,   260,   261,     0,   177,   266,   262,
       0,   905,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,   266,   179,     0,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,     0,   179,     0,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,   179,    44,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,     0,
       0,     0,     0,    43,   308,   309,   310,     0,     0,     0,
       0,     0,   179,    44,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,   179,
      44,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     135,   136,   137,   138,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   543,     0,     0,
       0,     0,     0,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      13,   273,   230,   109,    81,   269,   271,   286,   385,   160,
     387,   446,   389,   310,     7,   519,   307,    73,   538,   584,
     540,    50,   542,    20,    21,   304,   606,   592,    19,    19,
      19,     0,    33,   127,   109,    21,   129,     6,    18,    34,
      50,    14,    15,   113,    47,   119,   116,   117,   138,   138,
     138,   555,   556,   116,   117,   138,   138,   128,   138,   138,
      29,    74,    31,   148,    33,   132,    35,   138,    54,   145,
     354,    40,   148,    32,   138,   151,    43,    44,    47,   172,
     145,   152,   176,   171,    53,   146,   370,   177,   177,    20,
      21,   176,   612,   470,   177,   177,   109,   167,   177,   169,
     138,   175,    71,    14,    15,   170,   169,   114,   105,   106,
     105,   106,   145,   177,   175,   122,   113,   138,   115,   116,
     117,   118,   228,    92,   121,   145,   145,   128,   138,   176,
     281,   410,   114,   145,   714,   138,   129,   138,   417,   146,
     122,   145,   175,   138,   157,   170,   175,   712,   146,   145,
     175,   170,   172,   228,   145,   145,   145,   436,   138,   132,
     172,   145,   268,   136,   146,   138,   139,   223,   172,   114,
     167,   157,   169,   138,   105,   106,   169,   122,   284,   175,
     471,   167,   113,   128,   177,   116,   117,   118,   114,   153,
     121,   175,   151,   268,     4,     5,   122,     7,   171,   145,
     169,   146,   308,   309,   708,    32,   175,   145,   145,   284,
     169,   175,   289,   128,   114,   228,   720,   145,   145,   178,
     146,   132,   787,   138,   301,   136,   172,   138,   139,   128,
      57,    58,   797,   308,   309,   172,   167,    66,   169,    59,
     744,    70,    32,   170,   172,   145,   146,   148,   148,   148,
     529,   151,   153,    11,   172,   268,   148,   175,    87,    88,
      89,    90,   145,   153,    22,    23,   372,    57,    58,   153,
     171,   284,   378,   379,   380,   381,   148,   712,   384,   551,
     386,   153,   388,   803,   390,   175,   721,   578,   115,   140,
     141,   175,   443,   138,   445,   308,   309,   372,   595,   171,
     138,   153,   408,   378,   379,   380,   381,   148,   321,   384,
     138,   386,   153,   388,   138,    20,    21,   148,   469,   146,
     147,    32,   153,   175,   151,   115,   175,   154,   177,   435,
     171,   173,   174,   408,   176,   412,   140,   141,   142,   175,
     171,   177,   169,   128,   171,   171,    57,    58,   425,   138,
     132,   178,   138,   176,   731,   138,   146,   147,   138,   372,
     435,   151,   146,   153,   154,   378,   379,   380,   381,   176,
     176,   384,   138,   386,    34,   388,    34,   390,   171,   169,
     176,   148,     1,   140,    54,   462,   138,   138,   178,   176,
     152,   152,   671,   148,   175,   408,   152,   176,   103,   104,
     105,   106,   667,   416,   115,   152,   171,   152,   113,   152,
     115,   116,   117,   118,   152,   152,   121,   152,   123,   124,
     152,   175,   435,   148,   138,   175,   690,    32,   138,   129,
     171,   171,   152,   152,   152,   146,   147,   171,   132,   175,
     151,   174,   153,   154,   152,   458,    36,   175,   175,   154,
     155,   156,   157,   158,   175,    19,   169,   138,   169,   175,
     566,   567,   167,   569,   169,   171,   175,   178,   172,   170,
     119,   170,    14,    15,    16,    17,    13,   148,   132,   174,
     176,    39,   138,   138,    26,   138,   563,   138,     7,    43,
      32,   566,   567,   175,   569,   145,    38,   170,   145,   177,
      42,   140,   140,    45,   732,   138,    48,   152,    50,   171,
      52,   138,     1,   138,   138,    57,    58,   789,   153,   153,
      62,    63,    64,   153,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,   152,
      82,    83,    84,   171,   138,    87,    88,    89,    90,   170,
     170,    93,   170,   566,   567,   177,   569,   140,   170,   170,
     170,   170,   138,   105,   106,   172,   170,   170,   153,   675,
     152,   177,   153,   115,   175,   170,    50,   170,   170,   177,
     686,   172,   688,   152,   170,   172,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   170,   172,
     675,    55,   450,    98,   146,   147,    58,   844,   240,   151,
      56,   686,   154,   155,   156,    74,   158,   817,   754,   161,
     162,   604,     1,   665,   730,   469,   370,   169,   688,   171,
     525,   173,   174,   230,   176,   275,   178,   173,   391,    68,
     284,   383,   865,   446,    -1,    -1,   243,    20,    21,    -1,
     719,    -1,    -1,    -1,    -1,   730,   733,    -1,    -1,    -1,
      -1,    -1,   675,    -1,    -1,   742,    -1,   264,    -1,    -1,
      20,    21,    -1,   686,    -1,   688,    -1,    -1,    -1,    -1,
      -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   286,
      -1,    -1,    -1,   290,    -1,   292,    -1,    -1,   775,    -1,
      -1,    -1,    -1,   300,    -1,    -1,   812,   304,   305,   306,
      -1,    -1,    -1,   310,    -1,    -1,    -1,   730,    -1,   825,
      -1,   318,   319,   320,    -1,   322,   323,   324,    -1,   806,
      -1,    -1,   105,   106,    -1,    -1,   333,   812,    -1,    -1,
     113,    -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     825,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
     886,   154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,   901,   169,    -1,    -1,   812,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,   886,   825,   410,    -1,    -1,    -1,   167,    -1,   169,
     417,    -1,    -1,    -1,    -1,    -1,   901,   424,    -1,    -1,
      -1,   428,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   436,
      -1,    -1,    -1,    20,    21,    -1,    -1,   444,    -1,   446,
      -1,    -1,    -1,    -1,   867,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     883,    -1,    -1,   886,    -1,    -1,    -1,    -1,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   901,    -1,
     487,   488,   489,   490,   491,   492,    -1,   494,   495,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
      -1,   518,    -1,    20,    21,    -1,   103,   104,   105,   106,
     107,    -1,   529,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     557,    -1,    -1,   560,   561,    -1,    -1,    -1,    -1,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,   580,   581,    -1,    32,    -1,    -1,    -1,
     167,    -1,   169,    -1,   591,    -1,    -1,   594,   595,   176,
      -1,   598,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    57,    58,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   634,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,   116,   117,   118,   646,
     147,   121,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    20,    21,    -1,    -1,   114,   115,
     167,    -1,   169,    -1,   671,    -1,   122,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,    -1,    -1,    -1,   692,    -1,   167,    -1,   169,
     146,   147,    -1,    -1,    -1,   151,    -1,    -1,   154,    -1,
      -1,    -1,    -1,   710,   711,    -1,    -1,    -1,    -1,    -1,
     717,    -1,   719,   169,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,   178,    -1,    -1,   732,    -1,    -1,    -1,   736,
      -1,    -1,    -1,   740,    -1,    -1,    -1,   103,   104,   105,
     106,   107,   749,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   783,    -1,    -1,    -1,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,   809,   169,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   826,
      -1,   828,   829,   830,    -1,    -1,   833,    -1,    -1,   836,
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
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,   156,    -1,   158,    -1,    -1,   161,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,   173,
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
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,
      -1,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,   171,    -1,   173,   174,   175,   176,
     177,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,    -1,    -1,    -1,   151,    -1,    -1,   154,
     155,   156,    -1,   158,    -1,    -1,   161,   162,    14,    15,
      16,    17,    -1,    -1,   169,    -1,   171,    -1,   173,   174,
      26,   176,    -1,   178,    -1,    32,    32,    -1,    -1,    -1,
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
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,   146,
     147,    -1,    -1,    -1,   151,   151,   153,   154,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    14,    15,    16,
      17,    -1,   169,   169,    -1,   171,    -1,   173,   174,    26,
     176,   178,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,
      -1,   158,    -1,    -1,   161,   162,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   171,    -1,   173,   174,   175,   176,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,   171,    45,   173,   174,    48,
     176,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    -1,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    14,    15,    16,    17,    18,    -1,
     169,    -1,   171,   172,   173,   174,    26,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   146,   147,    93,    -1,    -1,   151,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,   178,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    26,   154,   155,   156,    -1,   158,    -1,
      -1,   161,   162,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,   171,    45,   173,   174,    48,   176,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,   154,   155,   156,    -1,   158,    -1,    -1,   161,   162,
      -1,    -1,    38,    -1,    -1,    -1,    42,   170,   171,    45,
     173,   174,    48,   176,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,   171,    45,   173,   174,    48,
     176,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,   154,   155,   156,    -1,   158,
      -1,    -1,   161,   162,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,   171,    45,   173,   174,    48,   176,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,   154,   155,   156,    -1,   158,    -1,    -1,   161,
     162,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,   171,
      45,   173,   174,    48,   176,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    14,    15,    16,    17,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,   154,
     155,   156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,   171,    45,   173,   174,
      48,   176,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,   154,   155,   156,    -1,
     158,    -1,    -1,   161,   162,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,   171,    45,   173,   174,    48,   176,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    -1,   154,   155,   156,    -1,   158,    -1,    -1,
     161,   162,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
     171,   172,   173,   174,    26,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   146,
     147,    93,    -1,    -1,   151,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,   178,    -1,    -1,    32,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    -1,   154,   155,   156,    -1,   158,    -1,    -1,   161,
     162,    14,    15,    16,    17,    -1,    -1,    -1,    -1,   171,
     172,   173,   174,    26,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   146,   147,
      93,    -1,    -1,   151,    -1,   153,   154,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    32,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      -1,   154,   155,   156,    -1,   158,    -1,    -1,   161,   162,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,   171,   172,
     173,   174,    26,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   146,   147,    93,
      -1,    -1,   151,    -1,   153,   154,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    32,    -1,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,
     154,   155,   156,    -1,   158,    -1,    -1,   161,   162,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,   171,   172,   173,
     174,    26,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   146,   147,    93,    -1,
      -1,   151,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    32,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,   154,
     155,   156,    -1,   158,    -1,    -1,   161,   162,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,   171,   172,   173,   174,
      26,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,   146,   147,    93,    -1,    -1,
     151,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,   171,   172,   173,   174,    26,
     176,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,   115,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
     146,   147,    -1,    -1,    -1,   151,    -1,   153,   154,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   169,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    26,   154,   155,   156,
      -1,   158,    -1,    -1,   161,   162,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,   171,    45,   173,   174,    48,   176,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,   154,   155,   156,    -1,   158,    -1,
      -1,   161,   162,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,   171,    45,   173,   174,    48,   176,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,   154,   155,   156,    -1,   158,    -1,    -1,   161,   162,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,   171,    45,
     173,   174,    48,   176,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
     156,    -1,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,   173,   174,    -1,
     176,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   146,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,    57,    58,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
     125,   126,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,   146,   147,    -1,    -1,    -1,
     151,    -1,   167,   154,   169,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    20,    21,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,   103,   104,   105,   106,   107,   175,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   167,    -1,   169,    -1,    -1,
     172,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,   172,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      20,    21,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,   125,   126,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    20,    21,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    20,    21,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,    -1,    32,   113,    -1,   115,   116,   117,   118,
      -1,    -1,   121,    32,   123,   124,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    57,    58,
      -1,   167,    -1,   169,    -1,    -1,    -1,    -1,    57,    58,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,    32,    -1,   103,   104,    -1,   167,   107,
     169,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    32,   121,    -1,   123,   124,    -1,    57,    58,
      -1,    -1,    32,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    57,    58,   147,
      -1,   149,   150,   151,   152,   153,    -1,    57,    58,   157,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   146,   147,   167,
      -1,   169,   151,    -1,   153,   154,    -1,   146,   147,    -1,
      -1,    32,   151,    -1,   153,   154,   115,    57,    58,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
     169,    -1,    32,    -1,    -1,   115,    57,    58,    -1,   178,
      -1,    -1,    -1,    -1,    -1,   115,    -1,   146,   147,    -1,
      -1,    -1,   151,    -1,   153,   154,    -1,    57,    58,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,
     169,   151,    -1,   153,   154,   115,   146,   147,    -1,   178,
      -1,   151,    -1,   153,   154,    57,    58,    -1,    -1,   169,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,   178,   169,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   178,    -1,
      -1,   151,    -1,   153,   154,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,   169,
     151,    -1,   153,   154,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,   115,    -1,    -1,   146,   147,   169,    -1,
      -1,   151,    -1,   153,   154,    -1,    -1,   178,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    24,    -1,    -1,   146,   147,    -1,    30,   178,   151,
      -1,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,   138,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    60,   138,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
     138,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,   161,   162
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   180,     0,     6,    29,    31,    33,    35,    40,    47,
      53,    71,    92,   169,   175,   181,   189,   190,   194,   214,
     218,   234,   288,   293,   294,   298,   334,   336,   138,   209,
     210,   132,   195,   196,   138,   191,   192,   138,   176,   138,
     337,   335,    33,   128,   138,   211,   212,   213,   225,     4,
       5,     7,   297,    59,   287,   146,   145,   148,   145,    21,
      54,   157,   167,   193,   148,   176,   138,   291,   292,   138,
     138,   138,   128,   171,   145,   170,    34,   105,   106,   138,
     217,   219,   220,   138,   296,   176,   289,    14,    15,   132,
     136,   138,   139,   171,   183,   209,   132,   196,   138,   138,
     138,    66,    70,    87,    88,    89,    90,   302,   290,   146,
     177,   291,   176,   176,   138,   210,   212,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   123,   124,   125,   126,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   161,
     162,    34,    34,   171,   215,   176,   221,   148,   295,   299,
     286,     1,   140,   182,    14,    15,   132,   136,   138,   183,
     207,   208,   193,   176,   138,   177,    24,    30,    46,    60,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,   225,   301,   303,   304,   306,   309,   138,   305,   138,
     279,   280,   172,   129,   172,   270,   275,   276,   148,   216,
     223,   225,   176,   138,   177,   285,   300,   175,   140,   141,
     145,   172,   290,   146,   175,   152,   152,   152,   152,   171,
     152,   152,   152,   152,   152,   152,    32,    57,    58,   115,
     146,   147,   151,   154,   169,   175,   178,   175,   148,   175,
     210,    47,   282,   300,   172,   175,   309,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    25,    26,    27,    28,
      37,    38,    41,    42,    45,    48,    49,    51,    52,    55,
      56,    61,    71,    93,    94,   105,   106,   120,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   139,   154,   155,
     156,   158,   161,   162,   171,   173,   174,   175,   177,   185,
     187,   197,   198,   201,   202,   203,   204,   205,   206,   221,
     222,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     234,   235,   236,   243,   252,   253,   254,   255,   256,   257,
     258,   259,   264,   267,   301,   338,   342,   347,   349,   351,
     273,   145,   148,   151,   284,   207,   177,   267,   312,   314,
     316,   310,   138,   307,   318,   320,   322,   324,   326,   328,
     330,   332,    32,    32,   151,   169,   178,   170,   267,   138,
     177,   309,   177,   279,   170,   282,   114,   146,   148,   151,
     281,   275,   267,   300,   309,   338,   171,   114,   175,   224,
     255,   267,   221,   267,   171,   267,   152,   152,   171,   175,
     152,   132,    55,   267,   221,   152,   114,   224,   267,   267,
     267,   174,   253,    12,   309,    12,   309,   267,   344,   348,
     184,   267,   267,   267,   225,   267,   267,   267,   174,    36,
     175,   175,   267,   138,   171,   175,   175,   175,    19,   233,
     214,   119,   175,    20,    21,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     122,   123,   124,   125,   126,   146,   147,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   167,   169,   171,
     177,   214,   270,   272,   138,   309,   114,   122,   146,   283,
     175,   309,   309,   309,   309,   172,   305,   309,   215,   309,
     215,   309,   215,   138,   277,   278,   309,   280,   170,   170,
     170,   309,   224,   267,   221,    19,   171,   225,   224,   267,
     114,   146,   175,    13,   267,   221,   237,   239,   267,   241,
     148,   132,   309,   224,   267,   175,   174,   300,    50,   138,
     154,   171,   267,   339,   340,   341,   343,   344,   345,   346,
     300,   171,   340,   346,   127,   175,   177,   141,   142,   182,
     188,   172,   152,   225,   176,   221,   169,   172,   250,   267,
      39,   285,   215,   253,    18,   138,   138,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   138,   120,   267,   267,   138,   267,   267,
     267,   267,   267,   267,   267,   267,   120,   267,    21,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     138,   265,   267,   172,   250,     7,    43,   271,   175,   146,
     175,   283,   224,   267,   153,   175,   153,   153,   153,   175,
     153,   216,   153,   216,   153,   216,   148,   153,   175,   153,
     175,   146,   281,   250,   172,   250,    18,   152,   267,   175,
     221,   172,   309,   309,   172,   309,   153,   175,    19,   253,
     122,   281,   172,   145,   175,   341,   170,   145,   170,   175,
      19,   172,   341,   177,   267,   344,   177,   267,   140,   186,
     260,   262,   223,    11,    22,    23,   199,   200,   138,   339,
     145,   172,   216,   152,   171,   267,   170,   267,   138,   148,
       1,   170,   172,    44,   271,   282,   224,   267,   175,   313,
     309,   317,   311,   308,   319,   153,   323,   153,   327,   153,
     309,   331,   277,   333,   267,   175,   172,   152,   138,   172,
     153,   153,   153,   171,   250,   267,   267,   340,   341,   138,
     339,   170,   170,   267,   170,   345,   250,   340,   177,   170,
     143,   140,   309,   215,   177,   221,   267,   170,   267,   127,
     221,   251,   268,   172,   250,   267,   266,   274,   220,   175,
     153,   321,   325,   329,   221,   244,   153,   175,   238,   240,
     242,   172,   267,   175,   341,   170,   122,   281,   170,   175,
     341,   170,   153,   216,   221,   172,   114,   267,   309,   172,
     219,   315,   309,    18,   267,   138,   267,   267,   267,   172,
     267,   170,   170,   267,   267,   267,   177,   261,   153,   200,
     267,   153,   175,   153,   152,   172,   153,   175,   350,   170,
     350,   170,   225,   263,   269,   245,   246,    18,   267,    50,
     170,   177,   225,   172,   309,   152,   172,   267,   170,   170,
     153,   248,   247,   309,   172,   153,   249,   172
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
    case 138: /* "name"  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3318 "ds_parser.cpp"
        break;

    case 182: /* character_sequence  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3324 "ds_parser.cpp"
        break;

    case 183: /* string_constant  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3330 "ds_parser.cpp"
        break;

    case 184: /* string_builder_body  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp"
        break;

    case 185: /* string_builder  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp"
        break;

    case 187: /* expr_reader  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp"
        break;

    case 191: /* require_module_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3354 "ds_parser.cpp"
        break;

    case 197: /* expression_label  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3360 "ds_parser.cpp"
        break;

    case 198: /* expression_goto  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3366 "ds_parser.cpp"
        break;

    case 200: /* expression_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3372 "ds_parser.cpp"
        break;

    case 202: /* expression_if_then_else  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3378 "ds_parser.cpp"
        break;

    case 203: /* expression_for_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3384 "ds_parser.cpp"
        break;

    case 204: /* expression_unsafe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3390 "ds_parser.cpp"
        break;

    case 205: /* expression_while_loop  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3396 "ds_parser.cpp"
        break;

    case 206: /* expression_with  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3402 "ds_parser.cpp"
        break;

    case 207: /* annotation_argument_value  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3408 "ds_parser.cpp"
        break;

    case 208: /* annotation_argument_value_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3414 "ds_parser.cpp"
        break;

    case 209: /* annotation_argument  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3420 "ds_parser.cpp"
        break;

    case 210: /* annotation_argument_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3426 "ds_parser.cpp"
        break;

    case 211: /* annotation_declaration_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3432 "ds_parser.cpp"
        break;

    case 212: /* annotation_declaration  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3438 "ds_parser.cpp"
        break;

    case 213: /* annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3444 "ds_parser.cpp"
        break;

    case 214: /* optional_annotation_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3450 "ds_parser.cpp"
        break;

    case 215: /* optional_function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3456 "ds_parser.cpp"
        break;

    case 216: /* optional_function_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3462 "ds_parser.cpp"
        break;

    case 217: /* function_name  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3468 "ds_parser.cpp"
        break;

    case 221: /* expression_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3474 "ds_parser.cpp"
        break;

    case 222: /* expression_any  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3480 "ds_parser.cpp"
        break;

    case 223: /* expressions  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3486 "ds_parser.cpp"
        break;

    case 224: /* expr_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3492 "ds_parser.cpp"
        break;

    case 225: /* name_in_namespace  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3498 "ds_parser.cpp"
        break;

    case 226: /* expression_delete  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3504 "ds_parser.cpp"
        break;

    case 227: /* expr_new  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp"
        break;

    case 228: /* expression_break  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp"
        break;

    case 229: /* expression_continue  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp"
        break;

    case 230: /* expression_return  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 231: /* expression_yield  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp"
        break;

    case 232: /* expression_try_catch  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp"
        break;

    case 235: /* expression_let  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3546 "ds_parser.cpp"
        break;

    case 236: /* expr_cast  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3552 "ds_parser.cpp"
        break;

    case 243: /* expr_type_info  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3558 "ds_parser.cpp"
        break;

    case 250: /* expr_list  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3564 "ds_parser.cpp"
        break;

    case 251: /* block_or_simple_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3570 "ds_parser.cpp"
        break;

    case 253: /* expr_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3576 "ds_parser.cpp"
        break;

    case 254: /* expr_numeric_const  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3582 "ds_parser.cpp"
        break;

    case 255: /* expr_assign  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3588 "ds_parser.cpp"
        break;

    case 256: /* expr_assign_pipe  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3594 "ds_parser.cpp"
        break;

    case 257: /* expr_named_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3600 "ds_parser.cpp"
        break;

    case 258: /* expr_method_call  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3606 "ds_parser.cpp"
        break;

    case 259: /* func_addr_expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3612 "ds_parser.cpp"
        break;

    case 264: /* expr_field  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3618 "ds_parser.cpp"
        break;

    case 267: /* expr  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3624 "ds_parser.cpp"
        break;

    case 270: /* optional_field_annotation  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3630 "ds_parser.cpp"
        break;

    case 272: /* structure_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3636 "ds_parser.cpp"
        break;

    case 273: /* struct_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3642 "ds_parser.cpp"
        break;

    case 275: /* function_argument_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3648 "ds_parser.cpp"
        break;

    case 276: /* function_argument_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3654 "ds_parser.cpp"
        break;

    case 277: /* tuple_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3660 "ds_parser.cpp"
        break;

    case 278: /* tuple_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3666 "ds_parser.cpp"
        break;

    case 279: /* variant_type  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3672 "ds_parser.cpp"
        break;

    case 280: /* variant_type_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3678 "ds_parser.cpp"
        break;

    case 282: /* variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3684 "ds_parser.cpp"
        break;

    case 285: /* let_variable_declaration  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3690 "ds_parser.cpp"
        break;

    case 286: /* global_variable_declaration_list  */
#line 125 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3696 "ds_parser.cpp"
        break;

    case 290: /* enum_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3702 "ds_parser.cpp"
        break;

    case 295: /* optional_structure_parent  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3708 "ds_parser.cpp"
        break;

    case 300: /* variable_name_with_pos_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3714 "ds_parser.cpp"
        break;

    case 303: /* structure_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3720 "ds_parser.cpp"
        break;

    case 304: /* auto_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3726 "ds_parser.cpp"
        break;

    case 305: /* bitfield_bits  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3732 "ds_parser.cpp"
        break;

    case 306: /* bitfield_type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3738 "ds_parser.cpp"
        break;

    case 309: /* type_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3744 "ds_parser.cpp"
        break;

    case 338: /* make_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3750 "ds_parser.cpp"
        break;

    case 339: /* make_struct_fields  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3756 "ds_parser.cpp"
        break;

    case 340: /* make_struct_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3762 "ds_parser.cpp"
        break;

    case 341: /* optional_block  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3768 "ds_parser.cpp"
        break;

    case 342: /* make_struct_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3774 "ds_parser.cpp"
        break;

    case 343: /* make_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3780 "ds_parser.cpp"
        break;

    case 344: /* make_map_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3786 "ds_parser.cpp"
        break;

    case 345: /* make_any_tuple  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3792 "ds_parser.cpp"
        break;

    case 346: /* make_dim  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3798 "ds_parser.cpp"
        break;

    case 347: /* make_dim_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3804 "ds_parser.cpp"
        break;

    case 348: /* make_table  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3810 "ds_parser.cpp"
        break;

    case 349: /* make_table_decl  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3816 "ds_parser.cpp"
        break;

    case 350: /* array_comprehension_where  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3822 "ds_parser.cpp"
        break;

    case 351: /* array_comprehension  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3828 "ds_parser.cpp"
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
#line 441 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4128 "ds_parser.cpp"
    break;

  case 16:
#line 453 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4134 "ds_parser.cpp"
    break;

  case 17:
#line 454 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4140 "ds_parser.cpp"
    break;

  case 18:
#line 458 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4146 "ds_parser.cpp"
    break;

  case 19:
#line 462 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4156 "ds_parser.cpp"
    break;

  case 20:
#line 467 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4170 "ds_parser.cpp"
    break;

  case 21:
#line 476 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4180 "ds_parser.cpp"
    break;

  case 22:
#line 484 "ds_parser.ypp"
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
#line 4198 "ds_parser.cpp"
    break;

  case 23:
#line 500 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4208 "ds_parser.cpp"
    break;

  case 24:
#line 505 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4218 "ds_parser.cpp"
    break;

  case 25:
#line 513 "ds_parser.ypp"
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
#line 4245 "ds_parser.cpp"
    break;

  case 26:
#line 534 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4257 "ds_parser.cpp"
    break;

  case 27:
#line 543 "ds_parser.ypp"
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
#line 4281 "ds_parser.cpp"
    break;

  case 29:
#line 569 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4289 "ds_parser.cpp"
    break;

  case 30:
#line 572 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4300 "ds_parser.cpp"
    break;

  case 31:
#line 578 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4311 "ds_parser.cpp"
    break;

  case 32:
#line 587 "ds_parser.ypp"
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
#line 4337 "ds_parser.cpp"
    break;

  case 33:
#line 608 "ds_parser.ypp"
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
#line 4362 "ds_parser.cpp"
    break;

  case 34:
#line 631 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4368 "ds_parser.cpp"
    break;

  case 35:
#line 632 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4374 "ds_parser.cpp"
    break;

  case 39:
#line 645 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4382 "ds_parser.cpp"
    break;

  case 40:
#line 648 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4390 "ds_parser.cpp"
    break;

  case 41:
#line 654 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4398 "ds_parser.cpp"
    break;

  case 42:
#line 660 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4406 "ds_parser.cpp"
    break;

  case 43:
#line 663 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4414 "ds_parser.cpp"
    break;

  case 44:
#line 669 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4420 "ds_parser.cpp"
    break;

  case 45:
#line 670 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4426 "ds_parser.cpp"
    break;

  case 46:
#line 674 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4432 "ds_parser.cpp"
    break;

  case 47:
#line 675 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4438 "ds_parser.cpp"
    break;

  case 48:
#line 676 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4449 "ds_parser.cpp"
    break;

  case 49:
#line 685 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4455 "ds_parser.cpp"
    break;

  case 50:
#line 686 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4461 "ds_parser.cpp"
    break;

  case 51:
#line 690 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4472 "ds_parser.cpp"
    break;

  case 52:
#line 699 "ds_parser.ypp"
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
#line 4490 "ds_parser.cpp"
    break;

  case 53:
#line 715 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4500 "ds_parser.cpp"
    break;

  case 54:
#line 723 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4512 "ds_parser.cpp"
    break;

  case 55:
#line 733 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4523 "ds_parser.cpp"
    break;

  case 56:
#line 742 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4529 "ds_parser.cpp"
    break;

  case 57:
#line 743 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4535 "ds_parser.cpp"
    break;

  case 58:
#line 744 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4541 "ds_parser.cpp"
    break;

  case 59:
#line 745 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4547 "ds_parser.cpp"
    break;

  case 60:
#line 746 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4553 "ds_parser.cpp"
    break;

  case 61:
#line 747 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4559 "ds_parser.cpp"
    break;

  case 62:
#line 751 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4569 "ds_parser.cpp"
    break;

  case 63:
#line 756 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4579 "ds_parser.cpp"
    break;

  case 64:
#line 764 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4585 "ds_parser.cpp"
    break;

  case 65:
#line 765 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4591 "ds_parser.cpp"
    break;

  case 66:
#line 766 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4597 "ds_parser.cpp"
    break;

  case 67:
#line 767 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4603 "ds_parser.cpp"
    break;

  case 68:
#line 768 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4609 "ds_parser.cpp"
    break;

  case 69:
#line 769 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4615 "ds_parser.cpp"
    break;

  case 70:
#line 770 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4621 "ds_parser.cpp"
    break;

  case 71:
#line 771 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4629 "ds_parser.cpp"
    break;

  case 72:
#line 777 "ds_parser.ypp"
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
#line 4647 "ds_parser.cpp"
    break;

  case 73:
#line 790 "ds_parser.ypp"
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
#line 4665 "ds_parser.cpp"
    break;

  case 74:
#line 806 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4671 "ds_parser.cpp"
    break;

  case 75:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4677 "ds_parser.cpp"
    break;

  case 76:
#line 811 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4689 "ds_parser.cpp"
    break;

  case 77:
#line 818 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4703 "ds_parser.cpp"
    break;

  case 78:
#line 830 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4712 "ds_parser.cpp"
    break;

  case 79:
#line 834 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4721 "ds_parser.cpp"
    break;

  case 80:
#line 841 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4727 "ds_parser.cpp"
    break;

  case 81:
#line 842 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4733 "ds_parser.cpp"
    break;

  case 82:
#line 846 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4739 "ds_parser.cpp"
    break;

  case 83:
#line 847 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4745 "ds_parser.cpp"
    break;

  case 84:
#line 848 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4751 "ds_parser.cpp"
    break;

  case 85:
#line 852 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4761 "ds_parser.cpp"
    break;

  case 86:
#line 857 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4770 "ds_parser.cpp"
    break;

  case 87:
#line 864 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4779 "ds_parser.cpp"
    break;

  case 88:
#line 868 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4785 "ds_parser.cpp"
    break;

  case 89:
#line 869 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4791 "ds_parser.cpp"
    break;

  case 90:
#line 870 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4797 "ds_parser.cpp"
    break;

  case 91:
#line 871 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4803 "ds_parser.cpp"
    break;

  case 92:
#line 872 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4809 "ds_parser.cpp"
    break;

  case 93:
#line 873 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4815 "ds_parser.cpp"
    break;

  case 94:
#line 874 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4821 "ds_parser.cpp"
    break;

  case 95:
#line 875 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4827 "ds_parser.cpp"
    break;

  case 96:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4833 "ds_parser.cpp"
    break;

  case 97:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4839 "ds_parser.cpp"
    break;

  case 98:
#line 878 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4845 "ds_parser.cpp"
    break;

  case 99:
#line 879 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4851 "ds_parser.cpp"
    break;

  case 100:
#line 880 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4857 "ds_parser.cpp"
    break;

  case 101:
#line 881 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4863 "ds_parser.cpp"
    break;

  case 102:
#line 882 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4869 "ds_parser.cpp"
    break;

  case 103:
#line 883 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4875 "ds_parser.cpp"
    break;

  case 104:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4881 "ds_parser.cpp"
    break;

  case 105:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4887 "ds_parser.cpp"
    break;

  case 106:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4893 "ds_parser.cpp"
    break;

  case 107:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4899 "ds_parser.cpp"
    break;

  case 108:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4905 "ds_parser.cpp"
    break;

  case 109:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4911 "ds_parser.cpp"
    break;

  case 110:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4917 "ds_parser.cpp"
    break;

  case 111:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4923 "ds_parser.cpp"
    break;

  case 112:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4929 "ds_parser.cpp"
    break;

  case 113:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4935 "ds_parser.cpp"
    break;

  case 114:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4941 "ds_parser.cpp"
    break;

  case 115:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4947 "ds_parser.cpp"
    break;

  case 116:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4953 "ds_parser.cpp"
    break;

  case 117:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4959 "ds_parser.cpp"
    break;

  case 118:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4965 "ds_parser.cpp"
    break;

  case 119:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4971 "ds_parser.cpp"
    break;

  case 120:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4977 "ds_parser.cpp"
    break;

  case 121:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4983 "ds_parser.cpp"
    break;

  case 122:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4989 "ds_parser.cpp"
    break;

  case 123:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4995 "ds_parser.cpp"
    break;

  case 124:
#line 907 "ds_parser.ypp"
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
#line 5018 "ds_parser.cpp"
    break;

  case 125:
#line 928 "ds_parser.ypp"
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
#line 5058 "ds_parser.cpp"
    break;

  case 126:
#line 966 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5067 "ds_parser.cpp"
    break;

  case 127:
#line 973 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5076 "ds_parser.cpp"
    break;

  case 128:
#line 977 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5089 "ds_parser.cpp"
    break;

  case 129:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5095 "ds_parser.cpp"
    break;

  case 130:
#line 989 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5101 "ds_parser.cpp"
    break;

  case 131:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5107 "ds_parser.cpp"
    break;

  case 132:
#line 991 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5113 "ds_parser.cpp"
    break;

  case 133:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5119 "ds_parser.cpp"
    break;

  case 134:
#line 993 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5125 "ds_parser.cpp"
    break;

  case 135:
#line 994 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5131 "ds_parser.cpp"
    break;

  case 136:
#line 995 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5137 "ds_parser.cpp"
    break;

  case 137:
#line 996 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5143 "ds_parser.cpp"
    break;

  case 138:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5149 "ds_parser.cpp"
    break;

  case 139:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5155 "ds_parser.cpp"
    break;

  case 140:
#line 999 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5161 "ds_parser.cpp"
    break;

  case 141:
#line 1000 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5167 "ds_parser.cpp"
    break;

  case 142:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5173 "ds_parser.cpp"
    break;

  case 143:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5179 "ds_parser.cpp"
    break;

  case 144:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5185 "ds_parser.cpp"
    break;

  case 145:
#line 1004 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5191 "ds_parser.cpp"
    break;

  case 146:
#line 1005 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5197 "ds_parser.cpp"
    break;

  case 147:
#line 1006 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5203 "ds_parser.cpp"
    break;

  case 148:
#line 1010 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5213 "ds_parser.cpp"
    break;

  case 149:
#line 1015 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5224 "ds_parser.cpp"
    break;

  case 150:
#line 1021 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5232 "ds_parser.cpp"
    break;

  case 151:
#line 1027 "ds_parser.ypp"
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
#line 5258 "ds_parser.cpp"
    break;

  case 152:
#line 1048 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5266 "ds_parser.cpp"
    break;

  case 153:
#line 1054 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5272 "ds_parser.cpp"
    break;

  case 154:
#line 1055 "ds_parser.ypp"
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
#line 5288 "ds_parser.cpp"
    break;

  case 155:
#line 1066 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5294 "ds_parser.cpp"
    break;

  case 156:
#line 1070 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5302 "ds_parser.cpp"
    break;

  case 157:
#line 1076 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5310 "ds_parser.cpp"
    break;

  case 158:
#line 1079 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5318 "ds_parser.cpp"
    break;

  case 159:
#line 1082 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5327 "ds_parser.cpp"
    break;

  case 160:
#line 1086 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5335 "ds_parser.cpp"
    break;

  case 161:
#line 1092 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5341 "ds_parser.cpp"
    break;

  case 162:
#line 1096 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5347 "ds_parser.cpp"
    break;

  case 163:
#line 1100 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5355 "ds_parser.cpp"
    break;

  case 164:
#line 1103 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5363 "ds_parser.cpp"
    break;

  case 165:
#line 1106 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5373 "ds_parser.cpp"
    break;

  case 166:
#line 1111 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5381 "ds_parser.cpp"
    break;

  case 167:
#line 1114 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5391 "ds_parser.cpp"
    break;

  case 168:
#line 1122 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5399 "ds_parser.cpp"
    break;

  case 169:
#line 1125 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5409 "ds_parser.cpp"
    break;

  case 170:
#line 1130 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5417 "ds_parser.cpp"
    break;

  case 171:
#line 1133 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5427 "ds_parser.cpp"
    break;

  case 172:
#line 1141 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5435 "ds_parser.cpp"
    break;

  case 173:
#line 1147 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5441 "ds_parser.cpp"
    break;

  case 174:
#line 1148 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5447 "ds_parser.cpp"
    break;

  case 175:
#line 1152 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5453 "ds_parser.cpp"
    break;

  case 176:
#line 1153 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5459 "ds_parser.cpp"
    break;

  case 177:
#line 1157 "ds_parser.ypp"
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
#line 5496 "ds_parser.cpp"
    break;

  case 178:
#line 1192 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5502 "ds_parser.cpp"
    break;

  case 179:
#line 1192 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5508 "ds_parser.cpp"
    break;

  case 180:
#line 1192 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5516 "ds_parser.cpp"
    break;

  case 181:
#line 1195 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5522 "ds_parser.cpp"
    break;

  case 182:
#line 1195 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5528 "ds_parser.cpp"
    break;

  case 183:
#line 1195 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5538 "ds_parser.cpp"
    break;

  case 184:
#line 1200 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5544 "ds_parser.cpp"
    break;

  case 185:
#line 1200 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5550 "ds_parser.cpp"
    break;

  case 186:
#line 1200 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5560 "ds_parser.cpp"
    break;

  case 187:
#line 1208 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5569 "ds_parser.cpp"
    break;

  case 188:
#line 1212 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5579 "ds_parser.cpp"
    break;

  case 189:
#line 1217 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5590 "ds_parser.cpp"
    break;

  case 190:
#line 1223 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5596 "ds_parser.cpp"
    break;

  case 191:
#line 1223 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5602 "ds_parser.cpp"
    break;

  case 192:
#line 1223 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5611 "ds_parser.cpp"
    break;

  case 193:
#line 1227 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5617 "ds_parser.cpp"
    break;

  case 194:
#line 1227 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5623 "ds_parser.cpp"
    break;

  case 195:
#line 1227 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5633 "ds_parser.cpp"
    break;

  case 196:
#line 1232 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5639 "ds_parser.cpp"
    break;

  case 197:
#line 1232 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5645 "ds_parser.cpp"
    break;

  case 198:
#line 1232 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5656 "ds_parser.cpp"
    break;

  case 199:
#line 1241 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5664 "ds_parser.cpp"
    break;

  case 200:
#line 1244 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5672 "ds_parser.cpp"
    break;

  case 201:
#line 1250 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5678 "ds_parser.cpp"
    break;

  case 202:
#line 1251 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5690 "ds_parser.cpp"
    break;

  case 203:
#line 1258 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5703 "ds_parser.cpp"
    break;

  case 204:
#line 1269 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5709 "ds_parser.cpp"
    break;

  case 205:
#line 1270 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5715 "ds_parser.cpp"
    break;

  case 206:
#line 1271 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5721 "ds_parser.cpp"
    break;

  case 207:
#line 1275 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5731 "ds_parser.cpp"
    break;

  case 208:
#line 1281 "ds_parser.ypp"
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
#line 5787 "ds_parser.cpp"
    break;

  case 209:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5793 "ds_parser.cpp"
    break;

  case 210:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5799 "ds_parser.cpp"
    break;

  case 211:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5805 "ds_parser.cpp"
    break;

  case 212:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5811 "ds_parser.cpp"
    break;

  case 213:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5817 "ds_parser.cpp"
    break;

  case 214:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5823 "ds_parser.cpp"
    break;

  case 215:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5829 "ds_parser.cpp"
    break;

  case 216:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5835 "ds_parser.cpp"
    break;

  case 217:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5841 "ds_parser.cpp"
    break;

  case 218:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5847 "ds_parser.cpp"
    break;

  case 219:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5853 "ds_parser.cpp"
    break;

  case 220:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5859 "ds_parser.cpp"
    break;

  case 221:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5865 "ds_parser.cpp"
    break;

  case 222:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5871 "ds_parser.cpp"
    break;

  case 223:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5877 "ds_parser.cpp"
    break;

  case 224:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5883 "ds_parser.cpp"
    break;

  case 225:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5889 "ds_parser.cpp"
    break;

  case 226:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5895 "ds_parser.cpp"
    break;

  case 227:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5901 "ds_parser.cpp"
    break;

  case 228:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5907 "ds_parser.cpp"
    break;

  case 229:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5913 "ds_parser.cpp"
    break;

  case 230:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5919 "ds_parser.cpp"
    break;

  case 231:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5925 "ds_parser.cpp"
    break;

  case 232:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5931 "ds_parser.cpp"
    break;

  case 233:
#line 1368 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5943 "ds_parser.cpp"
    break;

  case 234:
#line 1379 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5953 "ds_parser.cpp"
    break;

  case 235:
#line 1384 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5965 "ds_parser.cpp"
    break;

  case 236:
#line 1394 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5974 "ds_parser.cpp"
    break;

  case 237:
#line 1398 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5980 "ds_parser.cpp"
    break;

  case 238:
#line 1398 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5986 "ds_parser.cpp"
    break;

  case 239:
#line 1398 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 5998 "ds_parser.cpp"
    break;

  case 240:
#line 1405 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6004 "ds_parser.cpp"
    break;

  case 241:
#line 1405 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6010 "ds_parser.cpp"
    break;

  case 242:
#line 1405 "ds_parser.ypp"
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
#line 6026 "ds_parser.cpp"
    break;

  case 243:
#line 1419 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6035 "ds_parser.cpp"
    break;

  case 244:
#line 1423 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6041 "ds_parser.cpp"
    break;

  case 245:
#line 1423 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6047 "ds_parser.cpp"
    break;

  case 246:
#line 1423 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6057 "ds_parser.cpp"
    break;

  case 247:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6063 "ds_parser.cpp"
    break;

  case 248:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6069 "ds_parser.cpp"
    break;

  case 249:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6075 "ds_parser.cpp"
    break;

  case 250:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6081 "ds_parser.cpp"
    break;

  case 251:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6087 "ds_parser.cpp"
    break;

  case 252:
#line 1436 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6093 "ds_parser.cpp"
    break;

  case 253:
#line 1437 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6099 "ds_parser.cpp"
    break;

  case 254:
#line 1438 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6105 "ds_parser.cpp"
    break;

  case 255:
#line 1439 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6111 "ds_parser.cpp"
    break;

  case 256:
#line 1440 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6117 "ds_parser.cpp"
    break;

  case 257:
#line 1441 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6123 "ds_parser.cpp"
    break;

  case 258:
#line 1442 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6129 "ds_parser.cpp"
    break;

  case 259:
#line 1443 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6135 "ds_parser.cpp"
    break;

  case 260:
#line 1444 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6141 "ds_parser.cpp"
    break;

  case 261:
#line 1445 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6147 "ds_parser.cpp"
    break;

  case 262:
#line 1446 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6153 "ds_parser.cpp"
    break;

  case 263:
#line 1447 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6159 "ds_parser.cpp"
    break;

  case 264:
#line 1448 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6165 "ds_parser.cpp"
    break;

  case 265:
#line 1449 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6171 "ds_parser.cpp"
    break;

  case 266:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6177 "ds_parser.cpp"
    break;

  case 267:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6183 "ds_parser.cpp"
    break;

  case 268:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6189 "ds_parser.cpp"
    break;

  case 269:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6195 "ds_parser.cpp"
    break;

  case 270:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6201 "ds_parser.cpp"
    break;

  case 271:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6207 "ds_parser.cpp"
    break;

  case 272:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6213 "ds_parser.cpp"
    break;

  case 273:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6219 "ds_parser.cpp"
    break;

  case 274:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6225 "ds_parser.cpp"
    break;

  case 275:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6231 "ds_parser.cpp"
    break;

  case 276:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6237 "ds_parser.cpp"
    break;

  case 277:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6243 "ds_parser.cpp"
    break;

  case 278:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6249 "ds_parser.cpp"
    break;

  case 279:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6255 "ds_parser.cpp"
    break;

  case 280:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6261 "ds_parser.cpp"
    break;

  case 281:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6267 "ds_parser.cpp"
    break;

  case 282:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6273 "ds_parser.cpp"
    break;

  case 283:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6279 "ds_parser.cpp"
    break;

  case 284:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6285 "ds_parser.cpp"
    break;

  case 285:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6291 "ds_parser.cpp"
    break;

  case 286:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6297 "ds_parser.cpp"
    break;

  case 287:
#line 1471 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6306 "ds_parser.cpp"
    break;

  case 288:
#line 1475 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6315 "ds_parser.cpp"
    break;

  case 289:
#line 1479 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6323 "ds_parser.cpp"
    break;

  case 290:
#line 1482 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6331 "ds_parser.cpp"
    break;

  case 291:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp"
    break;

  case 292:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6343 "ds_parser.cpp"
    break;

  case 293:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6349 "ds_parser.cpp"
    break;

  case 294:
#line 1488 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6359 "ds_parser.cpp"
    break;

  case 295:
#line 1493 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6369 "ds_parser.cpp"
    break;

  case 296:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6375 "ds_parser.cpp"
    break;

  case 297:
#line 1499 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6383 "ds_parser.cpp"
    break;

  case 298:
#line 1502 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6389 "ds_parser.cpp"
    break;

  case 299:
#line 1502 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6395 "ds_parser.cpp"
    break;

  case 300:
#line 1502 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6403 "ds_parser.cpp"
    break;

  case 301:
#line 1505 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6412 "ds_parser.cpp"
    break;

  case 302:
#line 1509 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6421 "ds_parser.cpp"
    break;

  case 303:
#line 1513 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6430 "ds_parser.cpp"
    break;

  case 304:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6436 "ds_parser.cpp"
    break;

  case 305:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6442 "ds_parser.cpp"
    break;

  case 306:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6448 "ds_parser.cpp"
    break;

  case 307:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6454 "ds_parser.cpp"
    break;

  case 308:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6460 "ds_parser.cpp"
    break;

  case 309:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6466 "ds_parser.cpp"
    break;

  case 310:
#line 1523 "ds_parser.ypp"
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
#line 6481 "ds_parser.cpp"
    break;

  case 311:
#line 1533 "ds_parser.ypp"
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
#line 6496 "ds_parser.cpp"
    break;

  case 312:
#line 1543 "ds_parser.ypp"
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
#line 6553 "ds_parser.cpp"
    break;

  case 313:
#line 1598 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6559 "ds_parser.cpp"
    break;

  case 314:
#line 1599 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6565 "ds_parser.cpp"
    break;

  case 315:
#line 1603 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6571 "ds_parser.cpp"
    break;

  case 316:
#line 1604 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6577 "ds_parser.cpp"
    break;

  case 317:
#line 1608 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6587 "ds_parser.cpp"
    break;

  case 318:
#line 1616 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6595 "ds_parser.cpp"
    break;

  case 319:
#line 1619 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6604 "ds_parser.cpp"
    break;

  case 320:
#line 1624 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6610 "ds_parser.cpp"
    break;

  case 321:
#line 1624 "ds_parser.ypp"
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
#line 6659 "ds_parser.cpp"
    break;

  case 322:
#line 1668 "ds_parser.ypp"
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
#line 6719 "ds_parser.cpp"
    break;

  case 323:
#line 1726 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6729 "ds_parser.cpp"
    break;

  case 324:
#line 1731 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6739 "ds_parser.cpp"
    break;

  case 325:
#line 1739 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6745 "ds_parser.cpp"
    break;

  case 326:
#line 1740 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6751 "ds_parser.cpp"
    break;

  case 327:
#line 1744 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6759 "ds_parser.cpp"
    break;

  case 328:
#line 1747 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6770 "ds_parser.cpp"
    break;

  case 329:
#line 1756 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6776 "ds_parser.cpp"
    break;

  case 330:
#line 1757 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6782 "ds_parser.cpp"
    break;

  case 331:
#line 1761 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6793 "ds_parser.cpp"
    break;

  case 332:
#line 1770 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6799 "ds_parser.cpp"
    break;

  case 333:
#line 1771 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6805 "ds_parser.cpp"
    break;

  case 334:
#line 1776 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6811 "ds_parser.cpp"
    break;

  case 335:
#line 1777 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6817 "ds_parser.cpp"
    break;

  case 336:
#line 1781 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6828 "ds_parser.cpp"
    break;

  case 337:
#line 1787 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6839 "ds_parser.cpp"
    break;

  case 338:
#line 1793 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6847 "ds_parser.cpp"
    break;

  case 339:
#line 1796 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6856 "ds_parser.cpp"
    break;

  case 340:
#line 1800 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6867 "ds_parser.cpp"
    break;

  case 341:
#line 1806 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6878 "ds_parser.cpp"
    break;

  case 342:
#line 1815 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6884 "ds_parser.cpp"
    break;

  case 343:
#line 1816 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6890 "ds_parser.cpp"
    break;

  case 344:
#line 1817 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6896 "ds_parser.cpp"
    break;

  case 345:
#line 1821 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6902 "ds_parser.cpp"
    break;

  case 346:
#line 1822 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6908 "ds_parser.cpp"
    break;

  case 347:
#line 1826 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6916 "ds_parser.cpp"
    break;

  case 348:
#line 1829 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6926 "ds_parser.cpp"
    break;

  case 349:
#line 1834 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6936 "ds_parser.cpp"
    break;

  case 350:
#line 1839 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6949 "ds_parser.cpp"
    break;

  case 351:
#line 1847 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6962 "ds_parser.cpp"
    break;

  case 352:
#line 1858 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6970 "ds_parser.cpp"
    break;

  case 353:
#line 1861 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6979 "ds_parser.cpp"
    break;

  case 354:
#line 1868 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6985 "ds_parser.cpp"
    break;

  case 355:
#line 1869 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6991 "ds_parser.cpp"
    break;

  case 356:
#line 1873 "ds_parser.ypp"
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
#line 7023 "ds_parser.cpp"
    break;

  case 357:
#line 1900 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7032 "ds_parser.cpp"
    break;

  case 358:
#line 1903 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7043 "ds_parser.cpp"
    break;

  case 359:
#line 1912 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7051 "ds_parser.cpp"
    break;

  case 360:
#line 1915 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7065 "ds_parser.cpp"
    break;

  case 361:
#line 1924 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7079 "ds_parser.cpp"
    break;

  case 362:
#line 1937 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7093 "ds_parser.cpp"
    break;

  case 366:
#line 1958 "ds_parser.ypp"
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
#line 7109 "ds_parser.cpp"
    break;

  case 367:
#line 1969 "ds_parser.ypp"
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
#line 7126 "ds_parser.cpp"
    break;

  case 368:
#line 1984 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7132 "ds_parser.cpp"
    break;

  case 369:
#line 1985 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7138 "ds_parser.cpp"
    break;

  case 370:
#line 1989 "ds_parser.ypp"
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
#line 7177 "ds_parser.cpp"
    break;

  case 371:
#line 2026 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7183 "ds_parser.cpp"
    break;

  case 372:
#line 2027 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7189 "ds_parser.cpp"
    break;

  case 373:
#line 2031 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7195 "ds_parser.cpp"
    break;

  case 374:
#line 2031 "ds_parser.ypp"
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
#line 7281 "ds_parser.cpp"
    break;

  case 375:
#line 2115 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7293 "ds_parser.cpp"
    break;

  case 376:
#line 2122 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7304 "ds_parser.cpp"
    break;

  case 377:
#line 2131 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7310 "ds_parser.cpp"
    break;

  case 378:
#line 2132 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7316 "ds_parser.cpp"
    break;

  case 379:
#line 2133 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7322 "ds_parser.cpp"
    break;

  case 380:
#line 2134 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7328 "ds_parser.cpp"
    break;

  case 381:
#line 2135 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7334 "ds_parser.cpp"
    break;

  case 382:
#line 2136 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7340 "ds_parser.cpp"
    break;

  case 383:
#line 2137 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7346 "ds_parser.cpp"
    break;

  case 384:
#line 2138 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7352 "ds_parser.cpp"
    break;

  case 385:
#line 2139 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7358 "ds_parser.cpp"
    break;

  case 386:
#line 2140 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7364 "ds_parser.cpp"
    break;

  case 387:
#line 2141 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7370 "ds_parser.cpp"
    break;

  case 388:
#line 2142 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7376 "ds_parser.cpp"
    break;

  case 389:
#line 2143 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7382 "ds_parser.cpp"
    break;

  case 390:
#line 2144 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7388 "ds_parser.cpp"
    break;

  case 391:
#line 2145 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7394 "ds_parser.cpp"
    break;

  case 392:
#line 2146 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7400 "ds_parser.cpp"
    break;

  case 393:
#line 2147 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7406 "ds_parser.cpp"
    break;

  case 394:
#line 2148 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7412 "ds_parser.cpp"
    break;

  case 395:
#line 2149 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7418 "ds_parser.cpp"
    break;

  case 396:
#line 2150 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7424 "ds_parser.cpp"
    break;

  case 397:
#line 2151 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7430 "ds_parser.cpp"
    break;

  case 398:
#line 2152 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7436 "ds_parser.cpp"
    break;

  case 399:
#line 2153 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7442 "ds_parser.cpp"
    break;

  case 400:
#line 2154 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7448 "ds_parser.cpp"
    break;

  case 401:
#line 2155 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7454 "ds_parser.cpp"
    break;

  case 402:
#line 2159 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7460 "ds_parser.cpp"
    break;

  case 403:
#line 2160 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7466 "ds_parser.cpp"
    break;

  case 404:
#line 2161 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7472 "ds_parser.cpp"
    break;

  case 405:
#line 2162 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7478 "ds_parser.cpp"
    break;

  case 406:
#line 2163 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7484 "ds_parser.cpp"
    break;

  case 407:
#line 2164 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7490 "ds_parser.cpp"
    break;

  case 408:
#line 2168 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7503 "ds_parser.cpp"
    break;

  case 409:
#line 2179 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7512 "ds_parser.cpp"
    break;

  case 410:
#line 2183 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7524 "ds_parser.cpp"
    break;

  case 411:
#line 2193 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7536 "ds_parser.cpp"
    break;

  case 412:
#line 2200 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7547 "ds_parser.cpp"
    break;

  case 413:
#line 2209 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7553 "ds_parser.cpp"
    break;

  case 414:
#line 2209 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7559 "ds_parser.cpp"
    break;

  case 415:
#line 2209 "ds_parser.ypp"
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
#line 7574 "ds_parser.cpp"
    break;

  case 416:
#line 2222 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7580 "ds_parser.cpp"
    break;

  case 417:
#line 2223 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7586 "ds_parser.cpp"
    break;

  case 418:
#line 2224 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7592 "ds_parser.cpp"
    break;

  case 419:
#line 2225 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7598 "ds_parser.cpp"
    break;

  case 420:
#line 2226 "ds_parser.ypp"
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
#line 7617 "ds_parser.cpp"
    break;

  case 421:
#line 2240 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7628 "ds_parser.cpp"
    break;

  case 422:
#line 2246 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7637 "ds_parser.cpp"
    break;

  case 423:
#line 2250 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7646 "ds_parser.cpp"
    break;

  case 424:
#line 2254 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7656 "ds_parser.cpp"
    break;

  case 425:
#line 2259 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7666 "ds_parser.cpp"
    break;

  case 426:
#line 2264 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7676 "ds_parser.cpp"
    break;

  case 427:
#line 2269 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7686 "ds_parser.cpp"
    break;

  case 428:
#line 2274 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7695 "ds_parser.cpp"
    break;

  case 429:
#line 2278 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7704 "ds_parser.cpp"
    break;

  case 430:
#line 2282 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7714 "ds_parser.cpp"
    break;

  case 431:
#line 2287 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7723 "ds_parser.cpp"
    break;

  case 432:
#line 2291 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7733 "ds_parser.cpp"
    break;

  case 433:
#line 2296 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7739 "ds_parser.cpp"
    break;

  case 434:
#line 2296 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7745 "ds_parser.cpp"
    break;

  case 435:
#line 2296 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7756 "ds_parser.cpp"
    break;

  case 436:
#line 2302 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7768 "ds_parser.cpp"
    break;

  case 437:
#line 2309 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7774 "ds_parser.cpp"
    break;

  case 438:
#line 2309 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7780 "ds_parser.cpp"
    break;

  case 439:
#line 2309 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7790 "ds_parser.cpp"
    break;

  case 440:
#line 2314 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7796 "ds_parser.cpp"
    break;

  case 441:
#line 2314 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7802 "ds_parser.cpp"
    break;

  case 442:
#line 2314 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7813 "ds_parser.cpp"
    break;

  case 443:
#line 2320 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7819 "ds_parser.cpp"
    break;

  case 444:
#line 2320 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7825 "ds_parser.cpp"
    break;

  case 445:
#line 2320 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7835 "ds_parser.cpp"
    break;

  case 446:
#line 2325 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7844 "ds_parser.cpp"
    break;

  case 447:
#line 2329 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7850 "ds_parser.cpp"
    break;

  case 448:
#line 2329 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7856 "ds_parser.cpp"
    break;

  case 449:
#line 2329 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7866 "ds_parser.cpp"
    break;

  case 450:
#line 2334 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7872 "ds_parser.cpp"
    break;

  case 451:
#line 2334 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7878 "ds_parser.cpp"
    break;

  case 452:
#line 2334 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7892 "ds_parser.cpp"
    break;

  case 453:
#line 2343 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7901 "ds_parser.cpp"
    break;

  case 454:
#line 2347 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7907 "ds_parser.cpp"
    break;

  case 455:
#line 2347 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7913 "ds_parser.cpp"
    break;

  case 456:
#line 2347 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7923 "ds_parser.cpp"
    break;

  case 457:
#line 2352 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7929 "ds_parser.cpp"
    break;

  case 458:
#line 2352 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7935 "ds_parser.cpp"
    break;

  case 459:
#line 2352 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7949 "ds_parser.cpp"
    break;

  case 460:
#line 2361 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7958 "ds_parser.cpp"
    break;

  case 461:
#line 2365 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7964 "ds_parser.cpp"
    break;

  case 462:
#line 2365 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7970 "ds_parser.cpp"
    break;

  case 463:
#line 2365 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7980 "ds_parser.cpp"
    break;

  case 464:
#line 2370 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7986 "ds_parser.cpp"
    break;

  case 465:
#line 2370 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7992 "ds_parser.cpp"
    break;

  case 466:
#line 2370 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8006 "ds_parser.cpp"
    break;

  case 467:
#line 2379 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8012 "ds_parser.cpp"
    break;

  case 468:
#line 2379 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8018 "ds_parser.cpp"
    break;

  case 469:
#line 2379 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8029 "ds_parser.cpp"
    break;

  case 470:
#line 2385 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8035 "ds_parser.cpp"
    break;

  case 471:
#line 2385 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8041 "ds_parser.cpp"
    break;

  case 472:
#line 2385 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8052 "ds_parser.cpp"
    break;

  case 473:
#line 2394 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8058 "ds_parser.cpp"
    break;

  case 474:
#line 2394 "ds_parser.ypp"
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
#line 8075 "ds_parser.cpp"
    break;

  case 475:
#line 2409 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8081 "ds_parser.cpp"
    break;

  case 476:
#line 2409 "ds_parser.ypp"
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
#line 8102 "ds_parser.cpp"
    break;

  case 477:
#line 2429 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8108 "ds_parser.cpp"
    break;

  case 478:
#line 2430 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8114 "ds_parser.cpp"
    break;

  case 479:
#line 2431 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8120 "ds_parser.cpp"
    break;

  case 480:
#line 2432 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8126 "ds_parser.cpp"
    break;

  case 481:
#line 2436 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8138 "ds_parser.cpp"
    break;

  case 482:
#line 2443 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8150 "ds_parser.cpp"
    break;

  case 483:
#line 2450 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8161 "ds_parser.cpp"
    break;

  case 484:
#line 2456 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8172 "ds_parser.cpp"
    break;

  case 485:
#line 2465 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8182 "ds_parser.cpp"
    break;

  case 486:
#line 2470 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8191 "ds_parser.cpp"
    break;

  case 487:
#line 2477 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8197 "ds_parser.cpp"
    break;

  case 488:
#line 2478 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8203 "ds_parser.cpp"
    break;

  case 489:
#line 2482 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8214 "ds_parser.cpp"
    break;

  case 490:
#line 2488 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8226 "ds_parser.cpp"
    break;

  case 491:
#line 2495 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8239 "ds_parser.cpp"
    break;

  case 492:
#line 2503 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8251 "ds_parser.cpp"
    break;

  case 493:
#line 2510 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8264 "ds_parser.cpp"
    break;

  case 494:
#line 2518 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8278 "ds_parser.cpp"
    break;

  case 495:
#line 2530 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8286 "ds_parser.cpp"
    break;

  case 496:
#line 2533 "ds_parser.ypp"
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
#line 8302 "ds_parser.cpp"
    break;

  case 497:
#line 2547 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8313 "ds_parser.cpp"
    break;

  case 498:
#line 2556 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8319 "ds_parser.cpp"
    break;

  case 499:
#line 2557 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8325 "ds_parser.cpp"
    break;

  case 500:
#line 2561 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8335 "ds_parser.cpp"
    break;

  case 501:
#line 2566 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8344 "ds_parser.cpp"
    break;

  case 502:
#line 2573 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8354 "ds_parser.cpp"
    break;

  case 503:
#line 2578 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8367 "ds_parser.cpp"
    break;

  case 504:
#line 2589 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8377 "ds_parser.cpp"
    break;

  case 505:
#line 2594 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8386 "ds_parser.cpp"
    break;

  case 506:
#line 2601 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8400 "ds_parser.cpp"
    break;

  case 507:
#line 2613 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8406 "ds_parser.cpp"
    break;

  case 508:
#line 2614 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8412 "ds_parser.cpp"
    break;

  case 509:
#line 2618 "ds_parser.ypp"
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
#line 8435 "ds_parser.cpp"
    break;

  case 510:
#line 2636 "ds_parser.ypp"
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
#line 8458 "ds_parser.cpp"
    break;


#line 8462 "ds_parser.cpp"

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
#line 2656 "ds_parser.ypp"


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
