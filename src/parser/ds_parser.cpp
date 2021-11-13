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
#line 81 "ds_parser.ypp"

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
#line 13 "ds_parser.ypp"

    #include "daScript/misc/platform.h"
    #include "daScript/ast/ast.h"
    #include "daScript/ast/ast_generate.h"
    #include "daScript/ast/ast_expressions.h"

    enum {
        CorM_COPY   = 0,
        CorM_MOVE   = (1<<0),
        CorM_CLONE  = (1<<1)
    };

    enum {
        OVERRIDE_NONE,
        OVERRIDE_OVERRIDE,
        OVERRIDE_SEALED,
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
        bool                    sealed = false;
        bool                    isPrivate = false;
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

#line 217 "ds_parser.cpp"

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
    DAS_SEALED = 350,
    ADDEQU = 351,
    SUBEQU = 352,
    DIVEQU = 353,
    MULEQU = 354,
    MODEQU = 355,
    ANDEQU = 356,
    OREQU = 357,
    XOREQU = 358,
    SHL = 359,
    SHR = 360,
    ADDADD = 361,
    SUBSUB = 362,
    LEEQU = 363,
    SHLEQU = 364,
    SHREQU = 365,
    GREQU = 366,
    EQUEQU = 367,
    NOTEQU = 368,
    RARROW = 369,
    LARROW = 370,
    QQ = 371,
    QDOT = 372,
    QBRA = 373,
    LPIPE = 374,
    LBPIPE = 375,
    LAPIPE = 376,
    LFPIPE = 377,
    RPIPE = 378,
    CLONEEQU = 379,
    ROTL = 380,
    ROTR = 381,
    ROTLEQU = 382,
    ROTREQU = 383,
    MAPTO = 384,
    COLCOL = 385,
    ANDAND = 386,
    OROR = 387,
    XORXOR = 388,
    ANDANDEQU = 389,
    OROREQU = 390,
    XORXOREQU = 391,
    BRABRAB = 392,
    BRACBRB = 393,
    CBRCBRB = 394,
    INTEGER = 395,
    LONG_INTEGER = 396,
    UNSIGNED_INTEGER = 397,
    UNSIGNED_LONG_INTEGER = 398,
    FLOAT = 399,
    DOUBLE = 400,
    NAME = 401,
    BEGIN_STRING = 402,
    STRING_CHARACTER = 403,
    STRING_CHARACTER_ESC = 404,
    END_STRING = 405,
    BEGIN_STRING_EXPR = 406,
    END_STRING_EXPR = 407,
    END_OF_READ = 408,
    UNARY_MINUS = 409,
    UNARY_PLUS = 410,
    PRE_INC = 411,
    PRE_DEC = 412,
    POST_INC = 413,
    POST_DEC = 414,
    DEREF = 415
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 113 "ds_parser.ypp"

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

#line 418 "ds_parser.cpp"

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
#define YYLAST   7683

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  182
/* YYNRULES -- Number of rules.  */
#define YYNRULES  571
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  997

#define YYUNDEFTOK  2
#define YYMAXUTOK   415

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
       2,     2,     2,   171,     2,   187,   182,   167,   160,     2,
     180,   181,   165,   164,   154,   163,   177,   166,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   157,   184,
     161,   155,   162,   156,   183,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   178,     2,   179,   159,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   185,   158,   186,   170,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   151,   152,   153,   168,
     169,   172,   173,   174,   175,   176
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   476,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   492,   493,   494,   498,   499,
     503,   520,   521,   522,   523,   527,   531,   536,   545,   553,
     569,   574,   582,   582,   612,   634,   638,   641,   645,   651,
     660,   683,   708,   709,   713,   717,   718,   722,   725,   731,
     737,   740,   746,   747,   751,   752,   753,   762,   763,   767,
     776,   792,   800,   810,   819,   820,   821,   822,   823,   824,
     828,   833,   841,   842,   843,   847,   848,   849,   850,   851,
     852,   853,   854,   860,   874,   891,   892,   893,   897,   905,
     918,   922,   929,   930,   934,   935,   936,   940,   943,   950,
     954,   955,   956,   957,   958,   959,   960,   961,   962,   963,
     964,   965,   966,   967,   968,   969,   970,   971,   972,   973,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,   988,   989,   990,   991,   992,   993,
     994,   995,   996,   997,   998,   999,  1000,  1001,  1005,  1027,
    1028,  1029,  1033,  1071,  1079,  1083,  1094,  1095,  1096,  1097,
    1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,
    1108,  1109,  1110,  1111,  1112,  1116,  1121,  1127,  1133,  1167,
    1170,  1176,  1177,  1188,  1192,  1198,  1201,  1204,  1208,  1214,
    1218,  1222,  1225,  1228,  1233,  1236,  1244,  1247,  1252,  1255,
    1263,  1269,  1270,  1274,  1308,  1308,  1308,  1311,  1311,  1311,
    1316,  1316,  1316,  1324,  1324,  1324,  1330,  1340,  1351,  1366,
    1369,  1375,  1376,  1383,  1394,  1395,  1396,  1400,  1401,  1402,
    1403,  1407,  1412,  1420,  1421,  1425,  1430,  1494,  1495,  1496,
    1497,  1498,  1499,  1503,  1504,  1505,  1506,  1507,  1508,  1509,
    1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,
    1520,  1521,  1525,  1526,  1527,  1528,  1532,  1543,  1548,  1558,
    1562,  1562,  1562,  1569,  1569,  1569,  1583,  1587,  1591,  1591,
    1591,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,  1606,
    1607,  1608,  1609,  1610,  1611,  1612,  1613,  1614,  1615,  1616,
    1617,  1618,  1619,  1620,  1621,  1622,  1623,  1624,  1625,  1626,
    1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,
    1637,  1638,  1639,  1640,  1641,  1642,  1643,  1644,  1648,  1652,
    1655,  1658,  1659,  1660,  1661,  1670,  1679,  1680,  1683,  1683,
    1683,  1686,  1690,  1694,  1698,  1699,  1700,  1701,  1702,  1703,
    1704,  1705,  1721,  1737,  1792,  1800,  1801,  1805,  1806,  1807,
    1811,  1812,  1816,  1817,  1818,  1822,  1832,  1835,  1840,  1839,
    1885,  1951,  1960,  1965,  1973,  1974,  1978,  1981,  1990,  1991,
    1995,  2004,  2005,  2010,  2011,  2015,  2021,  2027,  2030,  2034,
    2040,  2049,  2050,  2051,  2055,  2056,  2060,  2063,  2068,  2073,
    2081,  2092,  2095,  2103,  2104,  2108,  2109,  2110,  2114,  2142,
    2142,  2176,  2179,  2188,  2201,  2213,  2214,  2218,  2219,  2223,
    2224,  2225,  2229,  2257,  2289,  2290,  2294,  2295,  2299,  2343,
    2344,  2348,  2349,  2350,  2355,  2354,  2451,  2458,  2467,  2468,
    2469,  2470,  2471,  2472,  2473,  2474,  2475,  2476,  2477,  2478,
    2479,  2480,  2481,  2482,  2483,  2484,  2485,  2486,  2487,  2488,
    2489,  2490,  2491,  2495,  2496,  2497,  2498,  2499,  2500,  2504,
    2515,  2519,  2529,  2536,  2545,  2545,  2545,  2558,  2559,  2560,
    2561,  2562,  2576,  2582,  2586,  2590,  2595,  2600,  2605,  2610,
    2614,  2618,  2623,  2627,  2632,  2632,  2632,  2638,  2645,  2645,
    2645,  2650,  2650,  2650,  2656,  2656,  2656,  2661,  2665,  2665,
    2665,  2670,  2670,  2670,  2679,  2683,  2683,  2683,  2688,  2688,
    2688,  2697,  2701,  2701,  2701,  2706,  2706,  2706,  2715,  2715,
    2715,  2721,  2721,  2721,  2730,  2730,  2745,  2745,  2765,  2766,
    2767,  2768,  2772,  2779,  2786,  2792,  2801,  2806,  2813,  2814,
    2818,  2824,  2831,  2839,  2846,  2854,  2866,  2869,  2883,  2892,
    2893,  2897,  2902,  2909,  2914,  2924,  2929,  2936,  2948,  2949,
    2953,  2971
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
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"@@ <|\"",
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"",
  "\"^^=\"", "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
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
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@18", "function_argument_declaration", "function_argument_list",
  "tuple_type", "tuple_type_list", "variant_type", "variant_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@19", "enum_list",
  "single_alias", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@20", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@21", "$@22", "type_declaration", "$@23",
  "$@24", "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32",
  "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41",
  "$@42", "$@43", "$@44", "$@45", "$@46", "variant_alias_declaration",
  "$@47", "bitfield_alias_declaration", "$@48", "make_decl",
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,   409,   410,
     126,    33,   411,   412,   413,   414,   415,    46,    91,    93,
      40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -554

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-554)))

#define YYTABLE_NINF -526

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -554,    21,  -554,  -554,    23,   -55,   117,   -75,  -554,   -78,
    -554,  -554,    15,  -554,  -554,  -554,  -554,  -554,   224,  -554,
      35,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
     -58,  -554,   -16,    69,    50,  -554,  -554,   117,    19,  -554,
     106,    55,  -554,  -554,  -554,    35,   128,   161,  -554,  -554,
     175,   209,   193,  -554,   -20,  -554,  -554,  -554,   355,   381,
     386,  -554,   391,    18,    23,   216,   -55,   206,   257,  -554,
     285,   295,  -554,  7450,   258,   -81,   414,   242,   287,  -554,
     336,    23,    15,  -554,  -554,  -554,     9,  -554,  -554,  -554,
     337,  -554,  -554,   395,  -554,  -554,   308,  -554,  -554,  -554,
    -554,  -554,   243,    76,  -554,  -554,  -554,  -554,   441,  -554,
    -554,   338,   340,   341,   343,  -554,  -554,  -554,   318,  -554,
    -554,  -554,  -554,  -554,   344,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,   345,  -554,  -554,  -554,   347,   356,
    -554,  -554,  -554,  -554,   358,   360,  -554,  -554,  -554,  -554,
    -554,  3372,  -554,  -554,   339,  -554,  -554,  -554,   376,   380,
    -554,    46,  -554,  7062,   497,   498,  -554,   353,   349,   125,
    -554,   390,  -554,  -554,   401,  -554,  -554,   315,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,   143,  -554,  -554,  -554,  -554,
    -554,   393,  -554,   -10,   233,   266,  -554,  -554,  -554,  -554,
    -554,  -554,   508,  -554,  -554,    -4,  2998,  -554,  -554,  -554,
     357,   392,  -554,   359,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,   369,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
     371,  -554,  -554,   -92,   394,  -554,  -554,   422,  -554,   399,
     373,   -79,    23,   411,  -554,  -554,  -554,    76,  -554,  7450,
    7450,  7450,  7450,   378,   376,  7450,   353,  7450,   353,  7450,
     353,  7537,   380,  -554,  -554,  -554,   382,  -554,  -554,  -554,
    7363,   383,   407,  -554,   404,   409,   424,   410,   434,   416,
    -554,   436,  4646,  4646,  7189,  7276,  4646,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  4646,  4646,  4646,   -50,  4646,  4646,
    -554,  4646,  -554,   377,  -554,  -554,  -554,   -77,  -554,  -554,
    -554,  -554,   397,  -554,  -554,  -554,  -554,  -554,  -554,   768,
     419,  -554,  -554,  -554,  -554,  -554,   -60,  7450,   -32,  -554,
    -554,  -554,     0,  -554,   -33,  7450,  -554,  1299,  -554,  -554,
    -554,  -554,  -554,  -554,   387,     3,   -50,  -554,  -554,  -554,
     411,    83,  -554,  -554,   215,  -554,    79,   988,   192,  1421,
    -554,   -85,  1582,   394,  1740,   394,  1898,   394,   170,  -554,
     -83,  3372,    56,  -554,  1961,  -554,   -50,  -554,  4646,  -554,
    -554,  4646,  -554,  4646,  7450,   244,   244,   411,  1646,   411,
    1804,  6360,  -554,   173,   365,   244,   244,   -54,  -554,   244,
     244,  5236,   176,  -554,  2420,   465,    44,   457,  4646,  4646,
    -554,  -554,  4646,  4646,  4646,  4646,   458,  4646,   459,  4646,
    4646,  4646,  4646,  4646,  4646,  4646,  4646,  3138,  4646,  4646,
    4646,  4646,  4646,  4646,  4646,  4646,  4646,  4646,   -34,  4646,
    -554,  3278,  -554,  -554,  3372,  -554,  -554,   411,  -554,   139,
    -554,   401,  3372,  -554,  4646,  -554,  -554,   411,  2120,  -554,
     349,  4646,  4646,  -554,   425,   476,  3436,   140,  2280,   247,
     247,  -554,   581,   435,   437,  4646,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,   439,   444,   445,  -554,  -554,  -554,   411,
    -554,   -84,  -554,  6292,  -554,    59,  -554,  -554,    16,  -554,
     443,   472,  7450,  -554,   -49,  -554,  7450,  -554,  -554,  -554,
     474,  -554,   468,  -554,   470,  -554,   481,  7450,  -554,  7537,
    -554,   380,  3576,  3734,  7450,  5314,  7450,  7450,  5395,  7450,
    5473,  2056,     6,   247,    42,  1962,  3874,  6360,   490,   -11,
     466,   492,  -554,  -554,   156,    28,  4032,   -11,   194,  4646,
    4646,   461,  -554,  4646,   326,   500,  -554,   281,  -554,   503,
    -554,   151,  6588,   174,   353,   489,  -554,  -554,   635,   635,
     448,   448,    14,    14,   471,   736,  -554,  5872,   -36,   -36,
     635,   635,  6736,   556,  6656,   511,  6440,  6763,  6843,  6979,
     448,   448,   270,   270,   736,   736,   736,   512,  4646,  -554,
     519,  4646,   653,  5950,  -554,   165,  -554,  -554,  -554,  7450,
    -554,  2578,  -554,   293,    32,  2578,  -554,  -554,   546,  1114,
     658,  6588,   293,  -554,   520,   536,  6588,  -554,  2578,  -554,
    4736,   495,  -554,  -554,   505,  -554,  -554,   293,  -554,  -554,
    -554,  -554,   247,  -554,  4646,  4646,  4646,  4646,  4646,  4646,
    4646,  4646,  4646,  4646,  2718,  4646,  4646,  4646,  4646,  4646,
    4646,  2858,    47,  4646,  -554,    15,  -554,   673,     7,   507,
    -554,  -554,   851,  -554,  -554,  -554,  2578,  -554,  2216,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  3372,  -554,
    -554,  -554,  -554,   184,   547,  5554,  2356,  -554,  2514,  2654,
    -554,  2794,  -554,   465,  4646,  -554,  4646,  4646,     4,   548,
     503,   513,   516,  4646,   523,  4646,  4646,   503,   518,   526,
    6588,  -554,  -554,  6508,  -554,   551,  7450,   353,   123,   164,
    4646,  -554,   560,   561,   562,   563,  -554,   195,   394,  -554,
    4190,  -554,  -554,  4646,  -554,  6026,  -554,  6102,  -554,  -554,
    -554,  2937,  -554,  6191,  -554,  4646,  -554,  4836,  4646,  4646,
    -554,   349,  -554,  -554,  -554,  -554,  4936,  -554,  -554,  -554,
      65,  -554,  6588,  6588,  6588,  6588,  6588,  6588,  6588,  6588,
    6588,  6588,  4646,  4646,  6588,  6588,  6588,  6588,  6588,  6588,
    6588,  4646,  4646,  6588,  -554,   475,   217,   438,  -554,  -554,
     438,  -554,   508,  -554,  2578,  -554,  5036,  -554,  -554,  -554,
    -554,  -554,   142,  -554,  -554,  -554,  -554,  -554,   531,    52,
    6588,  6588,   -11,   533,   157,   490,   534,  -554,  6588,  -554,
    -554,    71,   -11,   545,  -554,  -554,  -554,  3074,   394,   549,
    6588,  -554,  -554,  -554,  -554,   174,   550,   -90,  7450,  -554,
     187,  6588,  -554,  -554,  -554,   508,  4646,    92,  -554,  -554,
    -554,  1485,   349,  -554,  -554,  4646,  -554,  6911,  6588,  6911,
    6588,  -554,   542,  -554,  -554,    13,   411,  -554,  5136,  -554,
    -554,  4646,   582,  -554,  4646,  4646,  4646,  4348,  4646,   552,
     553,  4646,  4646,  -554,  4646,   554,  -554,  -554,   573,  -554,
    -554,  -554,  4506,  -554,  -554,  3214,  -554,  6588,   349,  -554,
    -554,   293,  -554,  -554,   704,  -554,  -554,  5632,   575,   448,
     448,   448,  -554,  5713,   963,   559,  -554,  6588,  6588,   963,
     564,   -50,  -554,  4646,  6588,  -554,  -554,    65,     9,  -554,
       9,  -554,  4646,  -554,   696,   567,  -554,   569,  -554,  -554,
     -50,  6588,  -554,  -554,   566,   349,  5791,  4646,   568,   580,
    -554,  -554,  -554,  -554,  6588,  -554,  -554
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   201,     0,     0,     0,     0,   202,     0,
     536,   534,     0,    14,     3,    10,     9,     8,     0,     7,
     403,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   418,    19,    18,   403,     0,     0,    86,    87,
       0,   181,    88,    90,     0,    85,   430,   429,   149,   419,
     431,   404,   405,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   183,
       0,     0,     0,    93,   151,   150,     0,   148,   421,   420,
       0,   433,   432,   426,   407,   406,   409,    79,    80,    77,
      78,    76,     0,     0,    75,    84,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   438,   458,   439,   470,   440,
     444,   445,   446,   447,   462,   451,   452,   453,   454,   455,
     456,   457,   459,   460,   507,   443,   450,   461,   514,   521,
     441,   448,   442,   449,     0,     0,   469,   477,   480,   478,
     479,   414,   415,   417,     0,    16,    17,    20,     0,     0,
     182,     0,    91,     0,     0,     0,    99,    94,     0,     0,
     427,     0,   434,   401,   355,    21,    22,     0,    68,    69,
      66,    67,    65,    64,    70,     0,    41,   498,   501,   504,
     494,     0,   474,   508,   515,   522,   528,   531,   485,   490,
     484,   497,     0,   493,   487,     0,     0,   489,   416,   472,
       0,     0,   381,     0,    89,   147,   102,   103,   105,   104,
     106,   107,   108,   109,   134,   135,   132,   133,   125,   136,
     137,   126,   123,   124,   145,     0,   146,   138,   139,   140,
     141,   113,   114,   115,   110,   111,   112,   128,   129,   127,
     121,   122,   117,   116,   118,   119,   120,   101,   100,   144,
       0,   130,   131,   355,    97,   175,   153,     0,   411,   424,
       0,   355,     0,     0,    23,    24,    25,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    94,     0,    94,     0,
      94,     0,     0,   492,   486,   488,     0,   491,   287,   288,
       0,     0,     0,   281,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,   237,   239,   238,
     240,   241,   242,    26,     0,     0,     0,     0,     0,     0,
     482,     0,   224,   225,   285,   284,   235,   282,   347,   346,
     345,   344,    92,   350,   283,   349,   348,   326,   289,     0,
       0,   286,   538,   539,   540,   541,     0,     0,     0,   143,
     142,    95,     0,   374,     0,     0,   152,     0,   463,   466,
     464,   467,   465,   468,     0,     0,     0,   428,   366,   408,
       0,     0,   436,   410,   394,    71,     0,     0,     0,     0,
     471,     0,     0,    97,     0,    97,     0,    97,   181,   378,
       0,   376,     0,   483,   185,   188,     0,   213,     0,   204,
     207,     0,   210,     0,     0,   315,   316,     0,   548,     0,
       0,     0,   565,     0,     0,   293,   292,   331,    32,   291,
     290,     0,   226,   353,     0,   233,     0,     0,     0,     0,
     317,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   278,     0,
     481,     0,   473,   537,   380,   535,   382,     0,   372,   385,
      96,   355,    98,   177,     0,    57,    58,     0,     0,   189,
       0,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,   156,   154,     0,     0,     0,   170,   165,   163,   162,
     164,   176,   157,     0,     0,     0,   168,   169,   171,     0,
     161,     0,   158,   243,   411,     0,   422,   425,   355,   402,
       0,     0,     0,   395,     0,   499,     0,   505,   495,   475,
       0,   509,     0,   516,     0,   523,     0,     0,   529,     0,
     532,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,   556,   546,   548,
       0,   559,   560,   561,     0,     0,     0,   548,     0,     0,
       0,     0,    29,     0,    27,     0,   319,   270,   269,     0,
     327,     0,   219,     0,    94,     0,   341,   342,   294,   295,
     307,   308,   305,   306,     0,   336,   324,     0,   351,   352,
     296,   297,   312,   313,   314,     0,     0,   310,   311,   309,
     303,   304,   299,   298,   300,   301,   302,     0,     0,   276,
       0,     0,     0,     0,   329,     0,   373,   384,   383,     0,
     386,     0,   375,     0,     0,     0,   191,   194,     0,   243,
       0,   184,     0,   174,     0,     0,    51,    61,     0,   198,
     243,   225,   179,   180,     0,   172,   173,     0,   160,   166,
     167,   203,     0,   159,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   412,     0,   435,     0,   357,     0,
     356,   437,     0,   392,   393,   391,     0,   500,     0,   506,
     496,   476,   510,   512,   517,   519,   524,   526,   377,   530,
     379,   533,   186,     0,     0,     0,     0,   332,     0,     0,
     333,     0,   354,   233,     0,   549,     0,     0,   548,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     558,   566,   567,     0,    30,    33,     0,    94,     0,     0,
       0,   328,     0,     0,     0,     0,   231,     0,    97,   338,
       0,   322,   343,     0,   325,     0,   277,     0,   279,   320,
     330,   387,   390,   389,    62,     0,   195,   243,     0,     0,
     192,     0,    63,    49,    50,   199,   243,   196,   226,   175,
      54,   178,   253,   254,   256,   255,   257,   247,   248,   249,
     258,   259,     0,     0,   245,   246,   260,   261,   250,   251,
     252,     0,     0,   244,   423,     0,     0,   362,   358,   359,
     362,   367,   391,   396,     0,   400,   243,   502,   513,   520,
     527,   187,     0,   216,   214,   205,   208,   211,     0,     0,
     543,   542,   548,     0,     0,   547,     0,   551,   557,   563,
     562,     0,   548,     0,   564,    28,    31,     0,    97,     0,
     220,   229,   230,   228,   227,     0,     0,     0,     0,   267,
       0,   337,   323,   321,   280,   383,     0,     0,   193,   200,
     197,     0,     0,    52,    53,     0,    59,   264,   265,   262,
     263,   413,    93,   363,   364,   357,     0,   398,   243,   399,
     503,     0,     0,   215,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,     0,   554,   271,     0,   266,
     232,   234,     0,   221,   236,     0,   268,   388,     0,   155,
      55,     0,   371,   368,   360,   365,   397,     0,     0,   206,
     209,   212,   334,     0,   568,     0,   552,   545,   544,   568,
       0,     0,   274,     0,   222,   339,    60,    54,     0,   361,
       0,   217,     0,   335,     0,     0,   553,     0,   555,   272,
       0,   223,   340,    56,     0,     0,     0,     0,     0,     0,
     275,   369,   370,   218,   569,   570,   571
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -554,  -554,  -554,  -554,  -554,   324,   697,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,   724,  -554,   654,  -554,  -554,   698,
    -554,  -554,  -554,  -204,  -554,  -554,  -554,  -554,  -554,  -554,
     488,  -554,  -554,   702,   -65,  -554,   685,    73,  -320,  -277,
    -387,  -554,  -554,  -554,  -553,  -554,  -167,  -554,   -30,  -474,
     -12,  -554,  -554,  -554,  -554,  -554,  -554,  -554,   769,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -454,  -554,  -554,  -103,  -554,    40,  -469,  -554,  -362,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,   372,  -554,  -554,  -162,  -130,  -554,   -53,  -554,  -554,
    -554,   297,  -554,   227,  -554,  -338,   487,  -472,  -473,    80,
    -554,  -351,  -554,   739,  -554,  -554,  -554,   262,    88,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -360,   -70,  -554,  -554,  -554,   509,  -554,  -554,  -554,   -29,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,   491,  -552,
    -405,  -551,  -554,  -554,  -297,    36,   367,  -554,  -554,  -554,
    -165,  -554
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   157,    45,    14,   177,   183,   424,   334,   755,
     335,   585,    15,    16,    38,    39,    72,    17,    34,    35,
     503,   504,   895,   896,   505,   506,   507,   508,   509,   510,
     184,   185,    30,    31,    32,    52,    53,    54,    18,   264,
     366,   167,    19,    86,   168,    87,   336,   511,   367,   512,
     337,   513,   338,   514,   515,   516,   517,   518,   519,   520,
     339,   556,   914,   557,   915,   559,   916,   340,   554,   913,
     341,   591,   934,   342,   766,   767,   594,   343,   344,   648,
     522,   345,   346,   347,   756,   961,   757,   980,   348,   632,
     884,   592,   878,   982,   362,   830,   970,   905,   699,   528,
     968,   363,   364,   399,   400,   212,   213,   737,   478,   706,
     534,   383,   271,    62,    96,    21,   174,   375,    42,    75,
      22,    90,    23,   377,   171,   172,    60,    93,    24,   270,
     384,   350,   374,   148,   149,   210,   150,   284,   711,   401,
     282,   710,   279,   707,   280,   910,   281,   709,   285,   712,
     286,   838,   287,   714,   288,   839,   289,   716,   290,   840,
     291,   719,   292,   721,    25,    47,    26,    46,   351,   568,
     569,   570,   352,   571,   572,   573,   574,   353,   423,   354,
     975,   355
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   266,   479,   147,   636,   521,   542,   641,   544,   393,
     546,   395,   273,   397,   647,   577,   161,   635,   741,   422,
     476,     2,   435,   -92,   659,   734,   748,     3,   294,   529,
     662,   663,    97,    98,   436,   437,   672,   759,   563,   932,
      68,    27,    28,   163,   151,   272,   477,   746,    48,   828,
       4,   785,     5,   563,     6,   828,   943,   562,   272,   575,
       7,   146,   595,   448,   449,    40,   703,     8,    43,   433,
      55,    40,    69,     9,    49,   704,   892,   539,   446,   548,
      50,   448,   449,   627,   628,    33,   472,   893,   894,   361,
     178,   179,    10,    61,   735,   265,    51,    63,   723,   540,
     673,   549,   829,   434,    44,   153,   705,   379,   829,   380,
      41,   198,   629,    11,   211,   164,   165,   479,   438,   439,
     440,   441,   442,   468,   469,   443,   473,   644,   446,    74,
     447,   448,   449,   450,    82,   199,   200,   451,    64,   452,
     453,   468,   469,   630,   631,    50,   382,  -511,   480,   525,
     758,   481,  -511,   272,   475,   166,   295,   637,    99,    83,
     531,    51,   100,   154,   101,   102,   736,   782,   671,    29,
    -511,   786,    80,   740,   296,   461,   462,   463,   464,   465,
     466,   467,   531,   297,   795,    70,   531,   853,   855,   526,
     596,   468,   469,   525,   695,   201,    71,   638,   103,    12,
      64,    40,   696,   801,    66,    13,   760,   381,   697,   147,
     147,   147,   147,   476,   693,   147,   180,   147,   550,   147,
     181,   147,   182,   102,   198,   760,    65,   214,    56,    57,
     147,    58,   835,   824,   202,   203,   918,    64,   637,   204,
     551,   535,   205,   694,   147,   147,   760,   736,   199,   200,
     386,   387,   388,   389,   637,   924,   392,   206,   394,    59,
     396,    73,   530,    36,   436,   437,   207,   146,   146,   146,
     146,   404,   637,   146,    77,   146,   938,   146,   638,   146,
     849,   921,   267,   751,    37,   418,   420,   147,   146,   762,
     436,   437,   861,   531,   638,   147,   639,   277,   763,   640,
      80,   919,   146,   146,   911,   760,    50,    78,   201,   886,
     268,   925,   638,   436,   437,   428,   880,   768,   739,   760,
     413,    79,    51,   650,   278,   265,   912,   547,   474,   764,
     657,   887,   761,   852,   765,   744,   482,   587,   760,    80,
     745,   760,   862,   869,   147,   146,   780,   202,   203,   875,
     440,   441,   204,   146,   537,   205,   106,   580,   446,   581,
     907,   448,   449,   450,   527,   841,   698,   451,   936,   531,
     206,    82,   532,    81,   876,   533,   440,   441,   745,   207,
     749,   877,   922,    71,   446,   561,   447,   448,   449,   450,
    -518,   175,   176,   451,   553,  -518,   902,   438,   439,   440,
     441,   442,   146,   108,   443,   444,   445,   446,    84,   447,
     448,   449,   450,  -518,    85,   984,   451,   985,   452,   453,
     588,   468,   469,  -525,   454,   455,   456,   158,  -525,   332,
     661,   109,   265,   945,    88,   465,   466,   467,  -273,    91,
      89,   110,   152,  -273,    94,    92,  -525,   468,   469,   457,
      95,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,  -273,   147,   274,   275,   276,   147,   155,   436,   437,
     468,   469,   159,   156,   274,   275,   784,   147,   265,   147,
     868,   928,   160,   169,   147,   792,   147,   147,   368,   147,
     170,   903,   369,   173,    69,   436,   437,   904,   191,   187,
     800,   188,   189,   702,   190,   192,   193,   708,   194,   370,
     371,   372,   373,   175,   176,   582,   583,   195,   718,   196,
     146,   197,   209,   208,   146,   726,   211,   728,   729,   521,
     731,   261,   262,   263,   265,   146,   269,   146,   272,   283,
     293,   356,   146,   358,   146,   146,   479,   146,   359,   357,
     360,   365,   438,   439,   440,   441,   376,   382,   378,   390,
     432,   403,   446,   406,   447,   448,   449,   450,   407,   147,
     409,   451,   524,   452,   453,    12,   436,   437,   349,   438,
     439,   440,   441,   442,   408,   410,   443,   444,   445,   446,
     411,   447,   448,   449,   450,   412,   413,   414,   451,   471,
     452,   453,   593,   597,   604,   606,   454,   455,   456,   653,
     781,   463,   464,   465,   466,   467,   654,   664,   701,   665,
     472,   666,   700,   668,   889,   468,   469,   146,   669,   670,
     713,   457,   715,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   717,   739,   742,   743,   752,   754,   758,
     769,   770,   468,   469,   778,   436,   437,   772,   774,   901,
     438,   439,   440,   441,   442,   776,   672,   443,   444,   445,
     446,   791,   447,   448,   449,   450,   794,   793,   798,   451,
     827,   452,   453,    55,   415,   416,   147,   454,   421,   456,
     799,   831,   856,   842,   854,   857,   425,   426,   427,   866,
     429,   430,   859,   431,   863,   864,   871,   872,   873,   874,
     933,   917,   920,   923,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   926,   940,   942,   867,   948,   931,
     929,   955,   956,   468,   469,   962,   969,   972,   976,   523,
     960,   440,   441,   978,   146,   987,   988,   995,   584,   446,
     991,   447,   448,   449,   450,   989,   436,   437,   451,   996,
     104,    67,   186,   983,   107,   385,   105,   162,   826,   891,
      20,   966,   930,   848,   967,   944,   720,   906,   642,   402,
     555,   860,   834,   558,    76,   560,   692,   578,   436,   437,
     567,   405,   567,   391,   977,     0,     0,     0,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,   147,     0,
     598,   599,   468,   469,   600,   601,   602,   603,   992,   605,
       0,   607,   608,   609,   610,   611,   612,   613,   614,   616,
     617,   618,   619,   620,   621,   622,   623,   624,   625,   626,
       0,   633,   440,   441,     0,     0,     0,     0,     0,   935,
     446,     0,   447,   448,   449,   450,   643,     0,     0,   451,
     649,     0,     0,   651,   652,     0,   146,     0,   656,     0,
     660,     0,   438,   439,   440,   441,   442,   667,     0,   443,
     444,   445,   446,   198,   447,   448,   449,   450,     0,     0,
       0,   451,     0,   452,   453,     0,     0,     0,     0,   454,
     455,   456,     0,     0,     0,     0,     0,   199,   200,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   457,   725,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,   425,   431,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   431,   979,
       0,   750,   421,     0,     0,   753,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   703,   201,   990,     0,
       0,     0,     0,     0,     0,   704,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     775,     0,     0,   777,     0,     0,   832,   203,     0,     0,
       0,   204,     0,   783,   205,     0,     0,   787,     0,     0,
     198,     0,     0,     0,     0,     0,     0,     0,     0,   206,
     796,     0,     0,     0,     0,   833,     0,     0,   207,     0,
       0,     0,     0,     0,   199,   200,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   811,   814,   815,   816,   817,
     818,   819,   820,   823,     0,   825,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,   836,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,   201,     0,     0,     0,   850,   851,
       0,     0,     0,     0,     0,   858,     0,   567,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,   870,     0,   436,   437,     0,     0,     0,     0,
     468,   469,     0,   202,   203,   881,     0,   974,   204,     0,
       0,   205,     0,     0,     0,     0,     0,     0,     0,     0,
     814,   823,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,   536,     0,     0,   207,     0,     0,     0,     0,
       0,     0,     0,     0,   897,   898,     0,     0,     0,     0,
       0,     0,     0,   899,   900,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   908,     0,     0,     0,
     674,   675,   676,   677,   678,   679,   680,   681,   438,   439,
     440,   441,   442,   682,   683,   443,   444,   445,   446,   788,
     447,   448,   449,   450,     0,     0,     0,   451,   685,   452,
     453,   686,   687,     0,     0,   454,   455,   456,   688,   689,
     690,     0,     0,     0,     0,     0,     0,     0,   937,     0,
       0,     0,     0,   523,     0,     0,     0,   941,     0,   789,
     457,     0,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,   947,     0,     0,   949,   950,   951,   953,
     954,   468,   469,   957,   958,     0,   959,     0,   790,     0,
     483,     0,     0,     0,   964,     3,     0,   484,   485,   486,
       0,   487,     0,   298,   299,   300,   301,   302,     0,     0,
       0,     0,     0,     0,   488,   303,   489,   490,     0,     0,
       0,     0,     0,     0,     0,   981,   491,   304,     0,   492,
     305,     0,     0,   306,   986,     8,   307,   493,     0,   494,
     308,     0,     0,   495,   496,     0,     0,     0,     0,   994,
     497,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     310,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   311,   498,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,   198,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,   199,   200,   331,
       0,   332,   333,   501,   265,   502,   483,     0,     0,     0,
       0,     3,     0,   484,   485,   486,     0,   487,     0,   298,
     299,   300,   301,   302,     0,     0,     0,     0,     0,     0,
     488,   303,   489,   490,     0,     0,     0,     0,     0,     0,
       0,     0,   491,   304,     0,   492,   305,     0,     0,   306,
       0,     8,   307,   493,     0,   494,   308,   201,     0,   495,
     496,     0,     0,     0,     0,     0,   497,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   202,   203,   311,   498,
       0,   204,     0,   538,   205,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,   499,   500,   207,     0,
       0,     0,     0,     0,   198,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,   199,   200,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,     0,
     298,   299,   300,   301,   302,   331,     0,   332,   333,   501,
     265,   939,   303,     0,     0,     0,     0,     0,   198,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,   563,     0,   308,   201,     0,
       0,     0,   199,   200,     0,     0,     0,   309,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   310,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   202,   203,   311,
       0,     0,   204,     0,   541,   205,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
     206,     0,   201,     0,     0,     0,     0,     0,     0,   207,
       0,     0,   198,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   564,   323,     0,     0,   199,   200,     0,     0,
       0,   202,   203,     0,     0,     0,   204,     0,     0,   565,
     325,   326,     0,   327,     0,     0,   328,   329,   298,   299,
     300,   301,   302,     0,   206,     0,   566,     0,   332,   333,
     303,   265,     0,   207,     0,     0,   198,     0,     0,     0,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,     0,   308,   201,     0,     0,     0,
     199,   200,     0,     0,     0,   309,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,   202,   203,   311,     0,     0,
     204,     0,   543,   205,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,   206,     0,
     201,     0,     0,     0,     0,     0,     0,   207,     0,     0,
     198,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     564,   323,     0,     0,   199,   200,     0,     0,     0,   202,
     203,     0,     0,     0,   204,     0,     0,   565,   325,   326,
       0,   327,     0,     0,   328,   329,   298,   299,   300,   301,
     302,     0,   206,     0,   576,     0,   332,   333,   303,   265,
       0,   207,     0,   198,   294,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,   201,     0,     0,   199,   200,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,   202,   203,   311,     0,     0,   204,     0,
     545,   205,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,   206,   201,     0,     0,
       0,     0,     0,     0,     0,   207,     0,     0,   198,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,   199,   200,     0,     0,   202,   203,     0,     0,
       0,   204,   295,     0,   205,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,   206,
     296,   552,   331,     0,   332,   333,   303,   265,   207,   297,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   311,     0,     0,   204,     0,   733,   205,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,   206,   645,     0,     0,     0,     0,
       0,   499,   500,   207,     0,     0,     0,     0,   198,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,   199,   200,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,     0,     0,   298,   299,   300,   301,   302,     0,
     331,     0,   332,   333,   646,   265,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   311,     0,     0,   204,     0,   837,   205,
       0,     0,     0,     0,     0,     0,   312,   313,   198,     0,
       0,     0,     0,     0,   206,   658,     0,     0,     0,     0,
       0,   499,   500,   207,     0,     0,     0,     0,     0,     0,
      50,     0,   199,   200,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,   324,   325,   326,   303,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   304,     0,
     331,   305,   332,   333,   306,   265,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   311,     0,     0,   204,     0,   844,   205,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,   206,     0,     0,     0,     0,     0,
       0,     0,     0,   207,     0,     0,   198,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
     199,   200,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,   298,   299,   300,   301,   302,     0,   589,     0,
     331,   590,   332,   333,   303,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   845,   205,     0,     0,
       0,     0,     0,     0,   312,   313,   198,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,   499,
     500,   207,     0,     0,     0,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   324,   325,   326,   303,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   304,     0,   331,   305,
     332,   333,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   846,   205,     0,     0,
       0,     0,     0,     0,   312,   313,   198,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,   812,
     813,   207,     0,     0,     0,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   324,   325,   326,   303,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   304,     0,   331,   305,
     332,   333,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   847,   205,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,   198,
       0,     0,   206,     0,     0,     0,     0,     0,     0,   821,
     822,   207,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,   199,   200,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   324,   325,   326,   303,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   304,     0,   331,   305,
     332,   333,   306,   265,     0,   307,     0,     0,     0,   308,
       0,     0,   637,   201,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,   885,   203,     0,     0,     0,   204,     0,     0,
     205,     0,     0,     0,   312,   313,   198,     0,     0,     0,
       0,     0,     0,     0,     0,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   207,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,   615,
       0,   324,   325,   326,   303,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   304,   330,   331,   305,
     332,   333,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   927,   205,     0,     0,
       0,     0,     0,     0,   312,   313,   198,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   324,   325,   326,   303,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   304,     0,   331,   305,
     332,   333,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   965,   205,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,   198,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,   199,   200,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,     0,     0,   331,   634,
     332,   333,   303,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,   201,     0,
     655,     0,     0,     0,     0,     0,     0,   309,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   310,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   202,   203,   311,
       0,     0,   204,     0,     0,   205,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
     206,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,   324,
     325,   326,   303,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,   304,     0,   331,   305,   332,   333,
     306,   265,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   310,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,   298,   299,
     300,   301,   302,     0,     0,     0,   331,   722,   332,   333,
     303,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,     0,     0,   724,     0,   324,   325,   326,
     303,   327,     0,     0,   328,   329,     0,     0,     0,     0,
       0,     0,   304,     0,   331,   305,   332,   333,   306,   265,
       0,   307,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,   298,   299,   300,   301,
     302,     0,     0,     0,   331,   738,   332,   333,   303,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   747,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,   298,   299,   300,   301,   302,     0,     0,     0,
     331,   879,   332,   333,   303,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,     0,     0,   331,   952,
     332,   333,   303,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   310,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,     0,     0,     0,     0,
       0,   963,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,   324,
     325,   326,   303,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,   304,     0,   331,   305,   332,   333,
     306,   265,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   310,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,     0,     0,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    51,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,     0,     0,   331,     0,   332,   333,
       0,   265,   674,   675,   676,   677,   678,   679,   680,   681,
     438,   439,   440,   441,   442,   682,   683,   443,   444,   445,
     446,   788,   447,   448,   449,   450,   436,   437,     0,   451,
     685,   452,   453,   686,   687,     0,     0,   454,   455,   456,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   789,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
     797,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   674,   675,   676,   677,   678,   679,   680,   681,
     438,   439,   440,   441,   442,   682,   683,   443,   444,   445,
     446,   788,   447,   448,   449,   450,   436,   437,     0,   451,
     685,   452,   453,   686,   687,     0,     0,   454,   455,   456,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   789,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
     888,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   674,   675,   676,   677,   678,   679,   680,   681,
     438,   439,   440,   441,   442,   682,   683,   443,   444,   445,
     446,   788,   447,   448,   449,   450,   436,   437,     0,   451,
     685,   452,   453,   686,   687,     0,     0,   454,   455,   456,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   789,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
     890,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   674,   675,   676,   677,   678,   679,   680,   681,
     438,   439,   440,   441,   442,   682,   683,   443,   444,   445,
     446,   788,   447,   448,   449,   450,   436,   437,     0,   451,
     685,   452,   453,   686,   687,     0,     0,   454,   455,   456,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   789,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
     909,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   674,   675,   676,   677,   678,   679,   680,   681,
     438,   439,   440,   441,   442,   682,   683,   443,   444,   445,
     446,   788,   447,   448,   449,   450,   436,   437,     0,   451,
     685,   452,   453,   686,   687,     0,     0,   454,   455,   456,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   789,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
     946,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,   437,     0,     0,     0,     0,
     438,   439,   440,   441,   442,     0,     0,   443,   444,   445,
     446,     0,   447,   448,   449,   450,     0,     0,     0,   451,
       0,   452,   453,     0,     0,     0,     0,   454,   455,   456,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   436,   437,   586,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,     0,
     447,   448,   449,   450,     0,     0,     0,   451,     0,   452,
     453,     0,     0,     0,     0,   454,   455,   456,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     457,     0,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   436,   437,   727,     0,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   436,   437,   730,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   436,   437,   732,     0,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,     0,
     447,   448,   449,   450,     0,     0,     0,   451,     0,   452,
     453,     0,     0,     0,     0,   454,   455,   456,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     457,     0,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   436,   437,   843,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,     0,   447,   448,
     449,   450,     0,     0,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   457,     0,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   436,   437,   971,     0,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   436,   437,   973,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     436,   437,   993,     0,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,     0,   447,   448,
     449,   450,     0,     0,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   457,     0,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
       0,     0,     0,     0,     0,     0,   436,   437,     0,   468,
     469,   771,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,     0,     0,   451,     0,   452,   453,     0,     0,     0,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   457,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,     0,
       0,     0,   436,   437,     0,     0,     0,   468,   469,   779,
     438,   439,   440,   441,   442,     0,     0,   443,   444,   445,
     446,     0,   447,   448,   449,   450,     0,     0,     0,   451,
       0,   452,   453,     0,     0,     0,     0,   454,   455,   456,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   882,   438,   439,   440,   441,
     442,   436,   437,   443,   444,   445,   446,     0,   447,   448,
     449,   450,     0,     0,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   457,     0,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   883,     0,     0,     0,     0,     0,   674,   675,   676,
     677,   678,   679,   680,   681,   438,   439,   440,   441,   442,
     682,   683,   443,   444,   445,   446,   788,   447,   448,   449,
     450,  -243,   436,   437,   451,   685,   452,   453,   686,   687,
       0,     0,   454,   455,   456,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   789,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   437,     0,     0,     0,     0,     0,     0,   674,   675,
     676,   677,   678,   679,   680,   681,   438,   439,   440,   441,
     442,   682,   683,   443,   444,   445,   446,   684,   447,   448,
     449,   450,     0,     0,     0,   451,   685,   452,   453,   686,
     687,     0,     0,   454,   455,   456,   688,   689,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   457,     0,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     436,   437,     0,     0,   438,   439,   440,   441,   442,   468,
     469,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,     0,     0,   451,     0,   452,   453,     0,     0,   579,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   457,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   436,   437,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,     0,     0,   451,     0,   452,   453,     0,     0,     0,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   457,   773,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   436,   437,
       0,     0,   438,   439,   440,   441,   442,   468,   469,   443,
     444,   445,   446,     0,   447,   448,   449,   450,     0,     0,
       0,   451,     0,   452,   453,     0,     0,     0,     0,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     865,     0,     0,     0,   457,     0,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   436,   437,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,   438,   439,   440,   441,   442,     0,     0,   443,
     444,   445,   446,     0,   447,   448,   449,   450,     0,     0,
       0,   451,     0,   452,   453,     0,     0,     0,     0,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   457,     0,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   436,   437,     0,     0,
     438,   439,   440,   441,   442,   468,   469,   443,   444,   445,
     446,     0,   447,   448,   449,   450,     0,     0,     0,   451,
       0,   452,   453,   436,   437,     0,     0,   454,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
     438,   439,   440,   441,   442,     0,     0,   443,   444,   445,
     446,     0,   447,   448,   449,   450,     0,     0,     0,   451,
       0,   452,   453,   436,   437,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   436,   437,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,     0,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,   438,   439,     0,     0,   442,
     468,   469,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,     0,     0,     0,   466,     0,     0,
       0,     0,     0,   438,   439,   440,   441,   442,   468,   469,
     443,   444,   445,   446,     0,   447,   448,   449,   450,   215,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,     0,     0,   241,   242,   243,   244,   245,   246,     0,
       0,   417,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,   112,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
       0,     0,   257,   258,   113,     0,     0,     0,     0,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,     0,     0,   419,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,     0,     0,     0,     0,     0,   112,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,   114,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
     112,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,   113,     0,     0,     0,     0,
     314,   315,   316,     0,     0,     0,     0,     0,     0,    51,
     114,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,   114,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398
};

static const yytype_int16 yycheck[] =
{
      12,   168,   362,    73,   477,   367,   393,   479,   395,   286,
     397,   288,   174,   290,   488,   420,    81,   471,   569,   316,
     358,     0,   342,     7,   498,    19,   577,     6,    32,   380,
     499,   500,    14,    15,    20,    21,   120,   589,    49,   129,
      21,    18,    19,    34,    73,   137,    46,    19,    33,    42,
      29,    19,    31,    49,    33,    42,    43,   417,   137,   419,
      39,    73,    18,   117,   118,   146,   115,    46,   146,   146,
      82,   146,    53,    52,    59,   124,    11,   162,   114,   162,
     130,   117,   118,   117,   118,   140,   146,    22,    23,   181,
      14,    15,    71,    58,   563,   185,   146,   155,   552,   184,
     184,   184,    95,   180,   182,   186,   155,   186,    95,   271,
     185,    32,   146,    92,   146,   106,   107,   477,   104,   105,
     106,   107,   108,   177,   178,   111,   186,   487,   114,    41,
     116,   117,   118,   119,   154,    56,    57,   123,   154,   125,
     126,   177,   178,   177,   178,   130,   146,   157,   181,   146,
     146,   184,   162,   137,   186,   146,   160,   115,   140,   179,
     154,   146,   144,    75,   146,   147,   124,   641,   519,   146,
     180,   645,   130,   184,   178,   161,   162,   163,   164,   165,
     166,   167,   154,   187,   658,   166,   154,   738,   740,   186,
     146,   177,   178,   146,   178,   116,   177,   155,   180,   178,
     154,   146,   186,   672,   154,   184,   154,   272,   528,   279,
     280,   281,   282,   551,   155,   285,   140,   287,   162,   289,
     144,   291,   146,   147,    32,   154,   157,   181,     4,     5,
     300,     7,   706,   186,   155,   156,   184,   154,   115,   160,
     184,   162,   163,   184,   314,   315,   154,   124,    56,    57,
     279,   280,   281,   282,   115,   184,   285,   178,   287,    35,
     289,   155,   179,   146,    20,    21,   187,   279,   280,   281,
     282,   300,   115,   285,   146,   287,   184,   289,   155,   291,
     734,   124,   157,   580,   167,   314,   315,   357,   300,   115,
      20,    21,   746,   154,   155,   365,   157,   154,   124,   160,
     130,   852,   314,   315,   162,   154,   130,   146,   116,   781,
     185,   862,   155,    20,    21,   327,   770,   594,   154,   154,
     180,   146,   146,   490,   181,   185,   184,   157,   357,   155,
     497,   785,   181,   738,   160,   179,   365,   161,   154,   130,
     184,   154,   747,   179,   414,   357,   181,   155,   156,   154,
     106,   107,   160,   365,   162,   163,   140,   184,   114,   186,
     834,   117,   118,   119,   376,   181,   528,   123,   181,   154,
     178,   154,   157,   180,   179,   160,   106,   107,   184,   187,
     186,   768,   854,   177,   114,   414,   116,   117,   118,   119,
     157,   148,   149,   123,   406,   162,   179,   104,   105,   106,
     107,   108,   414,   146,   111,   112,   113,   114,    53,   116,
     117,   118,   119,   180,    59,   968,   123,   970,   125,   126,
     432,   177,   178,   157,   131,   132,   133,   185,   162,   182,
     183,   146,   185,   906,    53,   165,   166,   167,   157,    53,
      59,   146,   184,   162,    53,    59,   180,   177,   178,   156,
      59,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   180,   532,   148,   149,   150,   536,    53,    20,    21,
     177,   178,   185,    59,   148,   149,   643,   547,   185,   549,
     757,   868,   146,   146,   554,   652,   556,   557,    66,   559,
      95,    53,    70,   185,    53,    20,    21,    59,   180,   161,
     667,   161,   161,   532,   161,   161,   161,   536,   161,    87,
      88,    89,    90,   148,   149,   150,   151,   161,   547,   161,
     532,   161,   146,   184,   536,   554,   146,   556,   557,   891,
     559,    34,    34,   180,   185,   547,   146,   549,   137,   146,
      32,   184,   554,   184,   556,   557,   906,   559,   179,   157,
     179,   157,   104,   105,   106,   107,   157,   146,   185,   181,
     183,   179,   114,   180,   116,   117,   118,   119,   161,   639,
     161,   123,   185,   125,   126,   178,    20,    21,   206,   104,
     105,   106,   107,   108,   180,   161,   111,   112,   113,   114,
     180,   116,   117,   118,   119,   161,   180,   161,   123,   180,
     125,   126,   137,   146,   146,   146,   131,   132,   133,   184,
     639,   163,   164,   165,   166,   167,   140,    36,   146,   184,
     146,   184,   179,   184,   791,   177,   178,   639,   184,   184,
     162,   156,   162,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   162,   154,   179,   154,   186,   148,   146,
     161,   180,   177,   178,     1,    20,    21,   146,   146,   184,
     104,   105,   106,   107,   108,   146,   120,   111,   112,   113,
     114,    13,   116,   117,   118,   119,   140,   157,   183,   123,
       7,   125,   126,   695,   312,   313,   756,   131,   316,   133,
     185,   184,   179,   146,   146,   179,   324,   325,   326,   148,
     328,   329,   179,   331,   186,   179,   146,   146,   146,   146,
     877,   180,   179,   179,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   179,   892,   184,   756,   146,   179,
     181,   179,   179,   177,   178,   162,    32,   162,   179,   367,
     186,   106,   107,   179,   756,    49,   179,   179,   424,   114,
     184,   116,   117,   118,   119,   186,    20,    21,   123,   179,
      63,    37,   108,   967,    66,   277,    64,    82,   695,   799,
       1,   938,   875,   733,   941,   905,   549,   830,   481,   292,
     408,   745,   702,   411,    45,   413,   524,   420,    20,    21,
     418,   300,   420,   284,   959,    -1,    -1,    -1,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,   878,    -1,
     438,   439,   177,   178,   442,   443,   444,   445,   985,   447,
      -1,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
      -1,   469,   106,   107,    -1,    -1,    -1,    -1,    -1,   878,
     114,    -1,   116,   117,   118,   119,   484,    -1,    -1,   123,
     488,    -1,    -1,   491,   492,    -1,   878,    -1,   496,    -1,
     498,    -1,   104,   105,   106,   107,   108,   505,    -1,   111,
     112,   113,   114,    32,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   553,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,   565,   566,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   576,   961,
      -1,   579,   580,    -1,    -1,   583,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   980,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     628,    -1,    -1,   631,    -1,    -1,   155,   156,    -1,    -1,
      -1,   160,    -1,   641,   163,    -1,    -1,   645,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
     658,    -1,    -1,    -1,    -1,   184,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    56,    57,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,    -1,   693,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,   706,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,   736,   737,
      -1,    -1,    -1,    -1,    -1,   743,    -1,   745,    -1,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   760,    -1,    20,    21,    -1,    -1,    -1,    -1,
     177,   178,    -1,   155,   156,   773,    -1,   184,   160,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     788,   789,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   812,   813,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   821,   822,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   834,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   886,    -1,
      -1,    -1,    -1,   891,    -1,    -1,    -1,   895,    -1,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,   911,    -1,    -1,   914,   915,   916,   917,
     918,   177,   178,   921,   922,    -1,   924,    -1,   184,    -1,
       1,    -1,    -1,    -1,   932,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   963,    37,    38,    -1,    40,
      41,    -1,    -1,    44,   972,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,   987,
      61,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,   165,    -1,   167,    -1,    -1,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,   180,
      -1,   182,   183,   184,   185,   186,     1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,    44,
      -1,    46,    47,    48,    -1,    50,    51,   116,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   155,   156,    93,    94,
      -1,   160,    -1,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   187,    -1,
      -1,    -1,    -1,    -1,    32,   130,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,    -1,   167,    -1,    -1,   170,   171,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   180,    -1,   182,   183,   184,
     185,   186,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    49,    -1,    51,   116,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   155,   156,    93,
      -1,    -1,   160,    -1,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    32,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    56,    57,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,   167,    -1,    -1,   170,   171,    14,    15,
      16,    17,    18,    -1,   178,    -1,   180,    -1,   182,   183,
      26,   185,    -1,   187,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,   116,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,   155,   156,    93,    -1,    -1,
     160,    -1,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      32,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,    -1,    56,    57,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,   167,    -1,    -1,   170,   171,    14,    15,    16,    17,
      18,    -1,   178,    -1,   180,    -1,   182,   183,    26,   185,
      -1,   187,    -1,    32,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   116,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   155,   156,    93,    -1,    -1,   160,    -1,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    32,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,    -1,    56,    57,    -1,    -1,   155,   156,    -1,    -1,
      -1,   160,   160,    -1,   163,   163,   164,   165,    -1,   167,
      -1,    -1,   170,   171,    14,    15,    16,    17,    18,   178,
     178,   180,   180,    -1,   182,   183,    26,   185,   187,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   155,   156,    93,    -1,    -1,   160,    -1,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,   178,   115,    -1,    -1,    -1,    -1,
      -1,   121,   122,   187,    -1,    -1,    -1,    -1,    32,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,   165,    -1,   167,    -1,    -1,
     170,   171,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     180,    -1,   182,   183,   184,   185,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   155,   156,    93,    -1,    -1,   160,    -1,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    32,    -1,
      -1,    -1,    -1,    -1,   178,   115,    -1,    -1,    -1,    -1,
      -1,   121,   122,   187,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,    56,    57,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   163,   164,   165,    26,   167,    -1,    -1,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     180,    41,   182,   183,    44,   185,    -1,    47,    -1,    -1,
      -1,    51,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   155,   156,    93,    -1,    -1,   160,    -1,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    32,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,   165,    -1,   167,    -1,    -1,
     170,   171,    14,    15,    16,    17,    18,    -1,   178,    -1,
     180,   181,   182,   183,    26,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    32,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   187,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      56,    57,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    32,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   187,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      56,    57,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    32,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   187,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    56,    57,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,   155,   156,    -1,    -1,    -1,   160,    -1,    -1,
     163,    -1,    -1,    -1,   106,   107,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   130,    -1,
      56,    57,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,   179,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    32,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      56,    57,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    32,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,   164,   165,    -1,   167,    -1,    -1,   170,   171,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   180,   181,
     182,   183,    26,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,   116,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   155,   156,    93,
      -1,    -1,   160,    -1,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   163,
     164,   165,    26,   167,    -1,    -1,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   180,    41,   182,   183,
      44,   185,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
     164,   165,    -1,   167,    -1,    -1,   170,   171,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   180,   181,   182,   183,
      26,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,   161,    -1,   163,   164,   165,
      26,   167,    -1,    -1,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   180,    41,   182,   183,    44,   185,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,   165,
      -1,   167,    -1,    -1,   170,   171,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   180,   181,   182,   183,    26,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,   164,   165,    -1,   167,
      -1,    -1,   170,   171,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   180,   181,   182,   183,    26,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,   165,    -1,   167,    -1,    -1,
     170,   171,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     180,   181,   182,   183,    26,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,   164,   165,    -1,   167,    -1,    -1,   170,   171,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   180,   181,
     182,   183,    26,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   163,
     164,   165,    26,   167,    -1,    -1,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   180,    41,   182,   183,
      44,   185,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
     164,   165,    -1,   167,    -1,    -1,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
      -1,   185,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    20,    21,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    20,    21,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    20,    21,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    20,    21,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    20,    21,    -1,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    20,    21,   181,   104,   105,
     106,   107,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
     126,    -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,    20,    21,   181,    -1,    -1,    -1,   104,
     105,   106,   107,   108,    -1,    -1,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,
     125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,    20,    21,   181,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,    20,    21,   181,    -1,    -1,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
     126,    -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,    20,    21,   181,   104,   105,   106,   107,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,
      -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,    20,    21,   181,    -1,    -1,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,    20,    21,   181,   104,   105,   106,   107,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      20,    21,   181,    -1,    -1,    -1,   104,   105,   106,   107,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,
      -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,   177,
     178,   179,    -1,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,   177,   178,   179,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,   104,   105,   106,   107,
     108,    20,    21,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,
      -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    20,    21,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      20,    21,    -1,    -1,   104,   105,   106,   107,   108,   177,
     178,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,   129,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    20,    21,
      -1,    -1,   104,   105,   106,   107,   108,   177,   178,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    20,    21,    -1,    -1,
     104,   105,   106,   107,   108,   177,   178,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    20,    21,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    20,    21,    -1,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,   108,
     177,   178,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,
     159,   160,   161,   162,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,   108,   177,   178,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    37,
      -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,   135,   136,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,   170,   171,    45,    -1,    -1,    -1,    -1,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,    45,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,   146,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   189,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   178,   184,   192,   200,   201,   205,   226,   230,
     246,   303,   308,   310,   316,   352,   354,    18,    19,   146,
     220,   221,   222,   140,   206,   207,   146,   167,   202,   203,
     146,   185,   306,   146,   182,   191,   355,   353,    33,    59,
     130,   146,   223,   224,   225,   238,     4,     5,     7,    35,
     314,    58,   301,   155,   154,   157,   154,   202,    21,    53,
     166,   177,   204,   155,   306,   307,   301,   146,   146,   146,
     130,   180,   154,   179,    53,    59,   231,   233,    53,    59,
     309,    53,    59,   315,    53,    59,   302,    14,    15,   140,
     144,   146,   147,   180,   194,   221,   140,   207,   146,   146,
     146,    24,    30,    45,    60,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   238,   319,   321,   322,
     324,   327,   184,   186,   306,    53,    59,   190,   185,   185,
     146,   222,   224,    34,   106,   107,   146,   229,   232,   146,
      95,   312,   313,   185,   304,   148,   149,   193,    14,    15,
     140,   144,   146,   194,   218,   219,   204,   161,   161,   161,
     161,   180,   161,   161,   161,   161,   161,   161,    32,    56,
      57,   116,   155,   156,   160,   163,   178,   187,   184,   146,
     323,   146,   293,   294,   181,    37,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   117,   118,   124,   125,   126,   127,
     128,   131,   132,   133,   134,   135,   136,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   170,   171,   177,
     178,    34,    34,   180,   227,   185,   234,   157,   185,   146,
     317,   300,   137,   282,   148,   149,   150,   154,   181,   330,
     332,   334,   328,   146,   325,   336,   338,   340,   342,   344,
     346,   348,   350,    32,    32,   160,   178,   187,    14,    15,
      16,    17,    18,    26,    38,    41,    44,    47,    51,    61,
      71,    93,   106,   107,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   147,   163,   164,   165,   167,   170,   171,
     179,   180,   182,   183,   196,   198,   234,   238,   240,   248,
     255,   258,   261,   265,   266,   269,   270,   271,   276,   279,
     319,   356,   360,   365,   367,   369,   184,   157,   184,   179,
     179,   181,   282,   289,   290,   157,   228,   236,    66,    70,
      87,    88,    89,    90,   320,   305,   157,   311,   185,   186,
     282,   222,   146,   299,   318,   218,   327,   327,   327,   327,
     181,   323,   327,   227,   327,   227,   327,   227,   146,   291,
     292,   327,   294,   179,   327,   356,   180,   161,   180,   161,
     161,   180,   161,   180,   161,   279,   279,    12,   327,    12,
     327,   279,   362,   366,   195,   279,   279,   279,   238,   279,
     279,   279,   183,   146,   180,   226,    20,    21,   104,   105,
     106,   107,   108,   111,   112,   113,   114,   116,   117,   118,
     119,   123,   125,   126,   131,   132,   133,   156,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   177,   178,
     179,   180,   146,   186,   327,   186,   293,    46,   296,   318,
     181,   184,   327,     1,     8,     9,    10,    12,    25,    27,
      28,    37,    40,    48,    50,    54,    55,    61,    94,   121,
     122,   184,   186,   208,   209,   212,   213,   214,   215,   216,
     217,   235,   237,   239,   241,   242,   243,   244,   245,   246,
     247,   267,   268,   279,   185,   146,   186,   238,   287,   299,
     179,   154,   157,   160,   298,   162,   184,   162,   162,   162,
     184,   162,   228,   162,   228,   162,   228,   157,   162,   184,
     162,   184,   180,   238,   256,   279,   249,   251,   279,   253,
     279,   327,   318,    49,   146,   163,   180,   279,   357,   358,
     359,   361,   362,   363,   364,   318,   180,   358,   364,   129,
     184,   186,   150,   151,   193,   199,   181,   161,   238,   178,
     181,   259,   279,   137,   264,    18,   146,   146,   279,   279,
     279,   279,   279,   279,   146,   279,   146,   279,   279,   279,
     279,   279,   279,   279,   279,    21,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   117,   118,   146,
     177,   178,   277,   279,   181,   259,   296,   115,   155,   157,
     160,   295,   289,   279,   318,   115,   184,   237,   267,   279,
     234,   279,   279,   184,   140,    54,   279,   234,   115,   237,
     279,   183,   265,   265,    36,   184,   184,   279,   184,   184,
     184,   299,   120,   184,    96,    97,    98,    99,   100,   101,
     102,   103,   109,   110,   115,   124,   127,   128,   134,   135,
     136,   155,   305,   155,   184,   178,   186,   226,   282,   286,
     179,   146,   327,   115,   124,   155,   297,   331,   327,   335,
     329,   326,   337,   162,   341,   162,   345,   162,   327,   349,
     291,   351,   181,   259,   161,   279,   327,   181,   327,   327,
     181,   327,   181,   162,    19,   265,   124,   295,   181,   154,
     184,   359,   179,   154,   179,   184,    19,   181,   359,   186,
     279,   362,   186,   279,   148,   197,   272,   274,   146,   357,
     154,   181,   115,   124,   155,   160,   262,   263,   227,   161,
     180,   179,   146,   157,   146,   279,   146,   279,     1,   179,
     181,   327,   237,   279,   234,    19,   237,   279,   115,   155,
     184,    13,   234,   157,   140,   237,   279,   184,   183,   185,
     234,   265,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   121,   122,   279,   279,   279,   279,   279,   279,
     279,   121,   122,   279,   186,   279,   225,     7,    42,    95,
     283,   184,   155,   184,   297,   237,   279,   162,   339,   343,
     347,   181,   146,   181,   162,   162,   162,   162,   264,   259,
     279,   279,   358,   359,   146,   357,   179,   179,   279,   179,
     363,   259,   358,   186,   179,   152,   148,   327,   227,   179,
     279,   146,   146,   146,   146,   154,   179,   228,   280,   181,
     259,   279,   179,   179,   278,   155,   295,   259,   184,   234,
     184,   236,    11,    22,    23,   210,   211,   279,   279,   279,
     279,   184,   179,    53,    59,   285,   285,   237,   279,   184,
     333,   162,   184,   257,   250,   252,   254,   180,   184,   359,
     179,   124,   295,   179,   184,   359,   179,   162,   228,   181,
     262,   179,   129,   234,   260,   327,   181,   279,   184,   186,
     234,   279,   184,    43,   283,   296,   184,   279,   146,   279,
     279,   279,   181,   279,   279,   179,   179,   279,   279,   279,
     186,   273,   162,   115,   279,   162,   234,   234,   288,    32,
     284,   181,   162,   181,   184,   368,   179,   368,   179,   238,
     275,   279,   281,   211,   232,   232,   279,    49,   179,   186,
     238,   184,   234,   181,   279,   179,   179
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   188,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   190,   190,   190,   191,   191,
     192,   193,   193,   193,   193,   194,   195,   195,   195,   196,
     197,   197,   199,   198,   200,   201,   202,   202,   202,   202,
     203,   203,   204,   204,   205,   206,   206,   207,   207,   208,
     209,   209,   210,   210,   211,   211,   211,   212,   212,   213,
     214,   215,   216,   217,   218,   218,   218,   218,   218,   218,
     219,   219,   220,   220,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   222,   222,   223,   223,   223,   224,   224,
     225,   225,   226,   226,   227,   227,   227,   228,   228,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   230,   231,
     231,   231,   232,   233,   234,   234,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   236,   236,   236,   237,   237,
     237,   238,   238,   238,   239,   240,   240,   240,   240,   241,
     242,   243,   243,   243,   243,   243,   244,   244,   244,   244,
     245,   246,   246,   247,   249,   250,   248,   251,   252,   248,
     253,   254,   248,   256,   257,   255,   258,   258,   258,   259,
     259,   260,   260,   260,   261,   261,   261,   262,   262,   262,
     262,   263,   263,   264,   264,   265,   265,   266,   266,   266,
     266,   266,   266,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   268,   268,   268,   268,   269,   270,   270,   271,
     272,   273,   271,   274,   275,   271,   276,   276,   277,   278,
     276,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   280,   281,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   282,   282,   283,   283,   283,
     284,   284,   285,   285,   285,   286,   287,   287,   288,   287,
     287,   287,   289,   289,   290,   290,   291,   291,   292,   292,
     293,   294,   294,   295,   295,   296,   296,   296,   296,   296,
     296,   297,   297,   297,   298,   298,   299,   299,   299,   299,
     299,   300,   300,   301,   301,   302,   302,   302,   303,   304,
     303,   305,   305,   305,   306,   307,   307,   308,   308,   309,
     309,   309,   310,   310,   311,   311,   312,   312,   313,   314,
     314,   315,   315,   315,   317,   316,   318,   318,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   320,   320,   320,   320,   320,   320,   321,
     322,   322,   323,   323,   325,   326,   324,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   328,   329,   327,   327,   330,   331,
     327,   332,   333,   327,   334,   335,   327,   327,   336,   337,
     327,   338,   339,   327,   327,   340,   341,   327,   342,   343,
     327,   327,   344,   345,   327,   346,   347,   327,   348,   349,
     327,   350,   351,   327,   353,   352,   355,   354,   356,   356,
     356,   356,   357,   357,   357,   357,   358,   358,   359,   359,
     360,   360,   360,   360,   360,   360,   361,   361,   362,   363,
     363,   364,   364,   365,   365,   366,   366,   367,   368,   368,
     369,   369
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
       1,     1,     3,     3,     3,     7,     1,     1,     1,     2,
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
       1,     3,     3,     2,     4,     0,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     4,     0,     3,     0,     8,
       8,     5,     2,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     4,     6,     5,     5,
       4,     0,     3,     0,     1,     0,     1,     1,     6,     0,
       6,     0,     3,     5,     3,     2,     3,     4,     2,     0,
       1,     1,     7,     9,     0,     2,     0,     1,     3,     1,
       1,     0,     1,     1,     0,     8,     1,     3,     1,     1,
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
    case 146: /* "name"  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3487 "ds_parser.cpp"
        break;

    case 191: /* module_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3493 "ds_parser.cpp"
        break;

    case 193: /* character_sequence  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3499 "ds_parser.cpp"
        break;

    case 194: /* string_constant  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3505 "ds_parser.cpp"
        break;

    case 195: /* string_builder_body  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3511 "ds_parser.cpp"
        break;

    case 196: /* string_builder  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3517 "ds_parser.cpp"
        break;

    case 198: /* expr_reader  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3523 "ds_parser.cpp"
        break;

    case 202: /* require_module_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3529 "ds_parser.cpp"
        break;

    case 208: /* expression_label  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3535 "ds_parser.cpp"
        break;

    case 209: /* expression_goto  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3541 "ds_parser.cpp"
        break;

    case 211: /* expression_else  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3547 "ds_parser.cpp"
        break;

    case 213: /* expression_if_then_else  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3553 "ds_parser.cpp"
        break;

    case 214: /* expression_for_loop  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3559 "ds_parser.cpp"
        break;

    case 215: /* expression_unsafe  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3565 "ds_parser.cpp"
        break;

    case 216: /* expression_while_loop  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3571 "ds_parser.cpp"
        break;

    case 217: /* expression_with  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3577 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value  */
#line 149 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3583 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_value_list  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3589 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3595 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument  */
#line 149 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3601 "ds_parser.cpp"
        break;

    case 222: /* annotation_argument_list  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3607 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3613 "ds_parser.cpp"
        break;

    case 224: /* annotation_declaration  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3619 "ds_parser.cpp"
        break;

    case 225: /* annotation_list  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3625 "ds_parser.cpp"
        break;

    case 226: /* optional_annotation_list  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3631 "ds_parser.cpp"
        break;

    case 227: /* optional_function_argument_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3637 "ds_parser.cpp"
        break;

    case 228: /* optional_function_type  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3643 "ds_parser.cpp"
        break;

    case 229: /* function_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3649 "ds_parser.cpp"
        break;

    case 234: /* expression_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3655 "ds_parser.cpp"
        break;

    case 235: /* expression_any  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3661 "ds_parser.cpp"
        break;

    case 236: /* expressions  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3667 "ds_parser.cpp"
        break;

    case 237: /* expr_pipe  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3673 "ds_parser.cpp"
        break;

    case 238: /* name_in_namespace  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3679 "ds_parser.cpp"
        break;

    case 239: /* expression_delete  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3685 "ds_parser.cpp"
        break;

    case 240: /* expr_new  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3691 "ds_parser.cpp"
        break;

    case 241: /* expression_break  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3697 "ds_parser.cpp"
        break;

    case 242: /* expression_continue  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3703 "ds_parser.cpp"
        break;

    case 243: /* expression_return  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3709 "ds_parser.cpp"
        break;

    case 244: /* expression_yield  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3715 "ds_parser.cpp"
        break;

    case 245: /* expression_try_catch  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3721 "ds_parser.cpp"
        break;

    case 247: /* expression_let  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3727 "ds_parser.cpp"
        break;

    case 248: /* expr_cast  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3733 "ds_parser.cpp"
        break;

    case 255: /* expr_type_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3739 "ds_parser.cpp"
        break;

    case 258: /* expr_type_info  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3745 "ds_parser.cpp"
        break;

    case 259: /* expr_list  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3751 "ds_parser.cpp"
        break;

    case 260: /* block_or_simple_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3757 "ds_parser.cpp"
        break;

    case 262: /* capture_entry  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3763 "ds_parser.cpp"
        break;

    case 263: /* capture_list  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3769 "ds_parser.cpp"
        break;

    case 264: /* optional_capture_list  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3775 "ds_parser.cpp"
        break;

    case 265: /* expr_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3781 "ds_parser.cpp"
        break;

    case 266: /* expr_numeric_const  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3787 "ds_parser.cpp"
        break;

    case 267: /* expr_assign  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3793 "ds_parser.cpp"
        break;

    case 268: /* expr_assign_pipe  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3799 "ds_parser.cpp"
        break;

    case 269: /* expr_named_call  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3805 "ds_parser.cpp"
        break;

    case 270: /* expr_method_call  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3811 "ds_parser.cpp"
        break;

    case 271: /* func_addr_expr  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3817 "ds_parser.cpp"
        break;

    case 276: /* expr_field  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3823 "ds_parser.cpp"
        break;

    case 279: /* expr  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3829 "ds_parser.cpp"
        break;

    case 282: /* optional_field_annotation  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3835 "ds_parser.cpp"
        break;

    case 286: /* structure_variable_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3841 "ds_parser.cpp"
        break;

    case 287: /* struct_variable_declaration_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3847 "ds_parser.cpp"
        break;

    case 289: /* function_argument_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3853 "ds_parser.cpp"
        break;

    case 290: /* function_argument_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3859 "ds_parser.cpp"
        break;

    case 291: /* tuple_type  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3865 "ds_parser.cpp"
        break;

    case 292: /* tuple_type_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3871 "ds_parser.cpp"
        break;

    case 293: /* variant_type  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3877 "ds_parser.cpp"
        break;

    case 294: /* variant_type_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3883 "ds_parser.cpp"
        break;

    case 296: /* variable_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3889 "ds_parser.cpp"
        break;

    case 299: /* let_variable_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3895 "ds_parser.cpp"
        break;

    case 300: /* global_variable_declaration_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3901 "ds_parser.cpp"
        break;

    case 305: /* enum_list  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3907 "ds_parser.cpp"
        break;

    case 311: /* optional_structure_parent  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3913 "ds_parser.cpp"
        break;

    case 318: /* variable_name_with_pos_list  */
#line 144 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3919 "ds_parser.cpp"
        break;

    case 321: /* structure_type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3925 "ds_parser.cpp"
        break;

    case 322: /* auto_type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3931 "ds_parser.cpp"
        break;

    case 323: /* bitfield_bits  */
#line 143 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3937 "ds_parser.cpp"
        break;

    case 324: /* bitfield_type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3943 "ds_parser.cpp"
        break;

    case 327: /* type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3949 "ds_parser.cpp"
        break;

    case 356: /* make_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3955 "ds_parser.cpp"
        break;

    case 357: /* make_struct_fields  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3961 "ds_parser.cpp"
        break;

    case 358: /* make_struct_dim  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3967 "ds_parser.cpp"
        break;

    case 359: /* optional_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3973 "ds_parser.cpp"
        break;

    case 360: /* make_struct_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3979 "ds_parser.cpp"
        break;

    case 361: /* make_tuple  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3985 "ds_parser.cpp"
        break;

    case 362: /* make_map_tuple  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3991 "ds_parser.cpp"
        break;

    case 363: /* make_any_tuple  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3997 "ds_parser.cpp"
        break;

    case 364: /* make_dim  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4003 "ds_parser.cpp"
        break;

    case 365: /* make_dim_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4009 "ds_parser.cpp"
        break;

    case 366: /* make_table  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4015 "ds_parser.cpp"
        break;

    case 367: /* make_table_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4021 "ds_parser.cpp"
        break;

    case 368: /* array_comprehension_where  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4027 "ds_parser.cpp"
        break;

    case 369: /* array_comprehension  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4033 "ds_parser.cpp"
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
#line 492 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4325 "ds_parser.cpp"
    break;

  case 16:
#line 493 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4331 "ds_parser.cpp"
    break;

  case 17:
#line 494 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4337 "ds_parser.cpp"
    break;

  case 18:
#line 498 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4343 "ds_parser.cpp"
    break;

  case 19:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4349 "ds_parser.cpp"
    break;

  case 20:
#line 503 "ds_parser.ypp"
    {
        g_Program->thisModule->isPublic = (yyvsp[0].b);
        g_Program->thisModule->isModule = true;
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das_yyerror("this module already has a name " + g_Program->thisModule->name,tokAt((yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !g_Program->policies.ignore_shared_modules ) {
            g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }
#line 4368 "ds_parser.cpp"
    break;

  case 21:
#line 520 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4374 "ds_parser.cpp"
    break;

  case 22:
#line 521 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4380 "ds_parser.cpp"
    break;

  case 23:
#line 522 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4386 "ds_parser.cpp"
    break;

  case 24:
#line 523 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4392 "ds_parser.cpp"
    break;

  case 25:
#line 527 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4398 "ds_parser.cpp"
    break;

  case 26:
#line 531 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4408 "ds_parser.cpp"
    break;

  case 27:
#line 536 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4422 "ds_parser.cpp"
    break;

  case 28:
#line 545 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4432 "ds_parser.cpp"
    break;

  case 29:
#line 553 "ds_parser.ypp"
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
#line 4450 "ds_parser.cpp"
    break;

  case 30:
#line 569 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4460 "ds_parser.cpp"
    break;

  case 31:
#line 574 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4470 "ds_parser.cpp"
    break;

  case 32:
#line 582 "ds_parser.ypp"
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
            das_yyerror("too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt((yylsp[0])),
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
#line 4497 "ds_parser.cpp"
    break;

  case 33:
#line 603 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4509 "ds_parser.cpp"
    break;

  case 34:
#line 612 "ds_parser.ypp"
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
#line 4533 "ds_parser.cpp"
    break;

  case 36:
#line 638 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4541 "ds_parser.cpp"
    break;

  case 37:
#line 641 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4550 "ds_parser.cpp"
    break;

  case 38:
#line 645 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4561 "ds_parser.cpp"
    break;

  case 39:
#line 651 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4572 "ds_parser.cpp"
    break;

  case 40:
#line 660 "ds_parser.ypp"
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-1].s), g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.moduleName) ) {
            g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-1].s),"",(yyvsp[0].b),tokAt((yylsp[-1]))));
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
            g_Program->allRequireDecl.push_back(make_tuple((Module *)nullptr,*(yyvsp[-1].s),"",(yyvsp[0].b),tokAt((yylsp[-1]))));
            das_yyerror("required module not found " + *(yyvsp[-1].s),tokAt((yylsp[-1])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-1].s);
    }
#line 4600 "ds_parser.cpp"
    break;

  case 41:
#line 683 "ds_parser.ypp"
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-3].s), g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.moduleName) ) {
            g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-3].s),*(yyvsp[-1].s),(yyvsp[0].b),tokAt((yylsp[-3]))));
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
            g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-3].s),*(yyvsp[-1].s),(yyvsp[0].b),tokAt((yylsp[-3]))));
            das_yyerror("required module not found " + *(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-3].s);
        delete (yyvsp[-1].s);
    }
#line 4627 "ds_parser.cpp"
    break;

  case 42:
#line 708 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4633 "ds_parser.cpp"
    break;

  case 43:
#line 709 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4639 "ds_parser.cpp"
    break;

  case 47:
#line 722 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4647 "ds_parser.cpp"
    break;

  case 48:
#line 725 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4655 "ds_parser.cpp"
    break;

  case 49:
#line 731 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4663 "ds_parser.cpp"
    break;

  case 50:
#line 737 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4671 "ds_parser.cpp"
    break;

  case 51:
#line 740 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4679 "ds_parser.cpp"
    break;

  case 52:
#line 746 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4685 "ds_parser.cpp"
    break;

  case 53:
#line 747 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4691 "ds_parser.cpp"
    break;

  case 54:
#line 751 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4697 "ds_parser.cpp"
    break;

  case 55:
#line 752 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4703 "ds_parser.cpp"
    break;

  case 56:
#line 753 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4714 "ds_parser.cpp"
    break;

  case 57:
#line 762 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4720 "ds_parser.cpp"
    break;

  case 58:
#line 763 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4726 "ds_parser.cpp"
    break;

  case 59:
#line 767 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4737 "ds_parser.cpp"
    break;

  case 60:
#line 776 "ds_parser.ypp"
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
#line 4755 "ds_parser.cpp"
    break;

  case 61:
#line 792 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4765 "ds_parser.cpp"
    break;

  case 62:
#line 800 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4777 "ds_parser.cpp"
    break;

  case 63:
#line 810 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4788 "ds_parser.cpp"
    break;

  case 64:
#line 819 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4794 "ds_parser.cpp"
    break;

  case 65:
#line 820 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4800 "ds_parser.cpp"
    break;

  case 66:
#line 821 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4806 "ds_parser.cpp"
    break;

  case 67:
#line 822 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4812 "ds_parser.cpp"
    break;

  case 68:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4818 "ds_parser.cpp"
    break;

  case 69:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4824 "ds_parser.cpp"
    break;

  case 70:
#line 828 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4834 "ds_parser.cpp"
    break;

  case 71:
#line 833 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4844 "ds_parser.cpp"
    break;

  case 72:
#line 841 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4850 "ds_parser.cpp"
    break;

  case 73:
#line 842 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4856 "ds_parser.cpp"
    break;

  case 74:
#line 843 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4862 "ds_parser.cpp"
    break;

  case 75:
#line 847 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4868 "ds_parser.cpp"
    break;

  case 76:
#line 848 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4874 "ds_parser.cpp"
    break;

  case 77:
#line 849 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4880 "ds_parser.cpp"
    break;

  case 78:
#line 850 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4886 "ds_parser.cpp"
    break;

  case 79:
#line 851 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4892 "ds_parser.cpp"
    break;

  case 80:
#line 852 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4898 "ds_parser.cpp"
    break;

  case 81:
#line 853 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4904 "ds_parser.cpp"
    break;

  case 82:
#line 854 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4912 "ds_parser.cpp"
    break;

  case 83:
#line 860 "ds_parser.ypp"
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
#line 4931 "ds_parser.cpp"
    break;

  case 84:
#line 874 "ds_parser.ypp"
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
#line 4950 "ds_parser.cpp"
    break;

  case 85:
#line 891 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4956 "ds_parser.cpp"
    break;

  case 86:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4962 "ds_parser.cpp"
    break;

  case 87:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4968 "ds_parser.cpp"
    break;

  case 88:
#line 897 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4981 "ds_parser.cpp"
    break;

  case 89:
#line 905 "ds_parser.ypp"
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
#line 4996 "ds_parser.cpp"
    break;

  case 90:
#line 918 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5005 "ds_parser.cpp"
    break;

  case 91:
#line 922 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5014 "ds_parser.cpp"
    break;

  case 92:
#line 929 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 5020 "ds_parser.cpp"
    break;

  case 93:
#line 930 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 5026 "ds_parser.cpp"
    break;

  case 94:
#line 934 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5032 "ds_parser.cpp"
    break;

  case 95:
#line 935 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5038 "ds_parser.cpp"
    break;

  case 96:
#line 936 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5044 "ds_parser.cpp"
    break;

  case 97:
#line 940 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5052 "ds_parser.cpp"
    break;

  case 98:
#line 943 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5061 "ds_parser.cpp"
    break;

  case 99:
#line 950 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5070 "ds_parser.cpp"
    break;

  case 100:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5076 "ds_parser.cpp"
    break;

  case 101:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5082 "ds_parser.cpp"
    break;

  case 102:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5088 "ds_parser.cpp"
    break;

  case 103:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5094 "ds_parser.cpp"
    break;

  case 104:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5100 "ds_parser.cpp"
    break;

  case 105:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5106 "ds_parser.cpp"
    break;

  case 106:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5112 "ds_parser.cpp"
    break;

  case 107:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5118 "ds_parser.cpp"
    break;

  case 108:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5124 "ds_parser.cpp"
    break;

  case 109:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5130 "ds_parser.cpp"
    break;

  case 110:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5136 "ds_parser.cpp"
    break;

  case 111:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5142 "ds_parser.cpp"
    break;

  case 112:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5148 "ds_parser.cpp"
    break;

  case 113:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5154 "ds_parser.cpp"
    break;

  case 114:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5160 "ds_parser.cpp"
    break;

  case 115:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5166 "ds_parser.cpp"
    break;

  case 116:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5172 "ds_parser.cpp"
    break;

  case 117:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5178 "ds_parser.cpp"
    break;

  case 118:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5184 "ds_parser.cpp"
    break;

  case 119:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5190 "ds_parser.cpp"
    break;

  case 120:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5196 "ds_parser.cpp"
    break;

  case 121:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5202 "ds_parser.cpp"
    break;

  case 122:
#line 976 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5208 "ds_parser.cpp"
    break;

  case 123:
#line 977 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5214 "ds_parser.cpp"
    break;

  case 124:
#line 978 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5220 "ds_parser.cpp"
    break;

  case 125:
#line 979 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5226 "ds_parser.cpp"
    break;

  case 126:
#line 980 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5232 "ds_parser.cpp"
    break;

  case 127:
#line 981 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5238 "ds_parser.cpp"
    break;

  case 128:
#line 982 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5244 "ds_parser.cpp"
    break;

  case 129:
#line 983 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5250 "ds_parser.cpp"
    break;

  case 130:
#line 984 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5256 "ds_parser.cpp"
    break;

  case 131:
#line 985 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5262 "ds_parser.cpp"
    break;

  case 132:
#line 986 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5268 "ds_parser.cpp"
    break;

  case 133:
#line 987 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5274 "ds_parser.cpp"
    break;

  case 134:
#line 988 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5280 "ds_parser.cpp"
    break;

  case 135:
#line 989 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5286 "ds_parser.cpp"
    break;

  case 136:
#line 990 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5292 "ds_parser.cpp"
    break;

  case 137:
#line 991 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5298 "ds_parser.cpp"
    break;

  case 138:
#line 992 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5304 "ds_parser.cpp"
    break;

  case 139:
#line 993 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5310 "ds_parser.cpp"
    break;

  case 140:
#line 994 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5316 "ds_parser.cpp"
    break;

  case 141:
#line 995 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5322 "ds_parser.cpp"
    break;

  case 142:
#line 996 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5328 "ds_parser.cpp"
    break;

  case 143:
#line 997 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5334 "ds_parser.cpp"
    break;

  case 144:
#line 998 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5340 "ds_parser.cpp"
    break;

  case 145:
#line 999 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5346 "ds_parser.cpp"
    break;

  case 146:
#line 1000 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5352 "ds_parser.cpp"
    break;

  case 147:
#line 1001 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5358 "ds_parser.cpp"
    break;

  case 148:
#line 1005 "ds_parser.ypp"
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
#line 5382 "ds_parser.cpp"
    break;

  case 149:
#line 1027 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5388 "ds_parser.cpp"
    break;

  case 150:
#line 1028 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5394 "ds_parser.cpp"
    break;

  case 151:
#line 1029 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5400 "ds_parser.cpp"
    break;

  case 152:
#line 1033 "ds_parser.ypp"
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
#line 5440 "ds_parser.cpp"
    break;

  case 153:
#line 1071 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5450 "ds_parser.cpp"
    break;

  case 154:
#line 1079 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5459 "ds_parser.cpp"
    break;

  case 155:
#line 1083 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5472 "ds_parser.cpp"
    break;

  case 156:
#line 1094 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5478 "ds_parser.cpp"
    break;

  case 157:
#line 1095 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5484 "ds_parser.cpp"
    break;

  case 158:
#line 1096 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5490 "ds_parser.cpp"
    break;

  case 159:
#line 1097 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5496 "ds_parser.cpp"
    break;

  case 160:
#line 1098 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5502 "ds_parser.cpp"
    break;

  case 161:
#line 1099 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5508 "ds_parser.cpp"
    break;

  case 162:
#line 1100 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5514 "ds_parser.cpp"
    break;

  case 163:
#line 1101 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5520 "ds_parser.cpp"
    break;

  case 164:
#line 1102 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5526 "ds_parser.cpp"
    break;

  case 165:
#line 1103 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5532 "ds_parser.cpp"
    break;

  case 166:
#line 1104 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5538 "ds_parser.cpp"
    break;

  case 167:
#line 1105 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5544 "ds_parser.cpp"
    break;

  case 168:
#line 1106 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5550 "ds_parser.cpp"
    break;

  case 169:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5556 "ds_parser.cpp"
    break;

  case 170:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5562 "ds_parser.cpp"
    break;

  case 171:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5568 "ds_parser.cpp"
    break;

  case 172:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5574 "ds_parser.cpp"
    break;

  case 173:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5580 "ds_parser.cpp"
    break;

  case 174:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5586 "ds_parser.cpp"
    break;

  case 175:
#line 1116 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5596 "ds_parser.cpp"
    break;

  case 176:
#line 1121 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5607 "ds_parser.cpp"
    break;

  case 177:
#line 1127 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5615 "ds_parser.cpp"
    break;

  case 178:
#line 1133 "ds_parser.ypp"
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
#line 5654 "ds_parser.cpp"
    break;

  case 179:
#line 1167 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5662 "ds_parser.cpp"
    break;

  case 180:
#line 1170 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5670 "ds_parser.cpp"
    break;

  case 181:
#line 1176 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5676 "ds_parser.cpp"
    break;

  case 182:
#line 1177 "ds_parser.ypp"
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
#line 5692 "ds_parser.cpp"
    break;

  case 183:
#line 1188 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5698 "ds_parser.cpp"
    break;

  case 184:
#line 1192 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5706 "ds_parser.cpp"
    break;

  case 185:
#line 1198 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5714 "ds_parser.cpp"
    break;

  case 186:
#line 1201 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5722 "ds_parser.cpp"
    break;

  case 187:
#line 1204 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5731 "ds_parser.cpp"
    break;

  case 188:
#line 1208 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5739 "ds_parser.cpp"
    break;

  case 189:
#line 1214 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5745 "ds_parser.cpp"
    break;

  case 190:
#line 1218 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5751 "ds_parser.cpp"
    break;

  case 191:
#line 1222 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5759 "ds_parser.cpp"
    break;

  case 192:
#line 1225 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5767 "ds_parser.cpp"
    break;

  case 193:
#line 1228 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5777 "ds_parser.cpp"
    break;

  case 194:
#line 1233 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5785 "ds_parser.cpp"
    break;

  case 195:
#line 1236 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5795 "ds_parser.cpp"
    break;

  case 196:
#line 1244 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5803 "ds_parser.cpp"
    break;

  case 197:
#line 1247 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5813 "ds_parser.cpp"
    break;

  case 198:
#line 1252 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5821 "ds_parser.cpp"
    break;

  case 199:
#line 1255 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5831 "ds_parser.cpp"
    break;

  case 200:
#line 1263 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5839 "ds_parser.cpp"
    break;

  case 201:
#line 1269 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5845 "ds_parser.cpp"
    break;

  case 202:
#line 1270 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5851 "ds_parser.cpp"
    break;

  case 203:
#line 1274 "ds_parser.ypp"
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
#line 5887 "ds_parser.cpp"
    break;

  case 204:
#line 1308 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5893 "ds_parser.cpp"
    break;

  case 205:
#line 1308 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5899 "ds_parser.cpp"
    break;

  case 206:
#line 1308 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5907 "ds_parser.cpp"
    break;

  case 207:
#line 1311 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5913 "ds_parser.cpp"
    break;

  case 208:
#line 1311 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5919 "ds_parser.cpp"
    break;

  case 209:
#line 1311 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5929 "ds_parser.cpp"
    break;

  case 210:
#line 1316 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5935 "ds_parser.cpp"
    break;

  case 211:
#line 1316 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5941 "ds_parser.cpp"
    break;

  case 212:
#line 1316 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5951 "ds_parser.cpp"
    break;

  case 213:
#line 1324 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5957 "ds_parser.cpp"
    break;

  case 214:
#line 1324 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5963 "ds_parser.cpp"
    break;

  case 215:
#line 1324 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5971 "ds_parser.cpp"
    break;

  case 216:
#line 1330 "ds_parser.ypp"
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
#line 5986 "ds_parser.cpp"
    break;

  case 217:
#line 1340 "ds_parser.ypp"
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
#line 6002 "ds_parser.cpp"
    break;

  case 218:
#line 1351 "ds_parser.ypp"
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
#line 6019 "ds_parser.cpp"
    break;

  case 219:
#line 1366 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6027 "ds_parser.cpp"
    break;

  case 220:
#line 1369 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6035 "ds_parser.cpp"
    break;

  case 221:
#line 1375 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6041 "ds_parser.cpp"
    break;

  case 222:
#line 1376 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6053 "ds_parser.cpp"
    break;

  case 223:
#line 1383 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6066 "ds_parser.cpp"
    break;

  case 224:
#line 1394 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6072 "ds_parser.cpp"
    break;

  case 225:
#line 1395 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6078 "ds_parser.cpp"
    break;

  case 226:
#line 1396 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6084 "ds_parser.cpp"
    break;

  case 227:
#line 1400 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6090 "ds_parser.cpp"
    break;

  case 228:
#line 1401 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6096 "ds_parser.cpp"
    break;

  case 229:
#line 1402 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6102 "ds_parser.cpp"
    break;

  case 230:
#line 1403 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6108 "ds_parser.cpp"
    break;

  case 231:
#line 1407 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6118 "ds_parser.cpp"
    break;

  case 232:
#line 1412 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6128 "ds_parser.cpp"
    break;

  case 233:
#line 1420 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6134 "ds_parser.cpp"
    break;

  case 234:
#line 1421 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6140 "ds_parser.cpp"
    break;

  case 235:
#line 1425 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6150 "ds_parser.cpp"
    break;

  case 236:
#line 1431 "ds_parser.ypp"
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
                das_yyerror("can only have capture section for the lambda",
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
                            das_yyerror("macro [" +pA->annotation->name + "] failed to apply to the block\n" + err, tokAt((yylsp[-4])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("blocks are only allowed function macros", tokAt((yylsp[-4])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-4].faList) );
            delete (yyvsp[-4].faList);
        }
    }
#line 6215 "ds_parser.cpp"
    break;

  case 237:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6221 "ds_parser.cpp"
    break;

  case 238:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6227 "ds_parser.cpp"
    break;

  case 239:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6233 "ds_parser.cpp"
    break;

  case 240:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6239 "ds_parser.cpp"
    break;

  case 241:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6245 "ds_parser.cpp"
    break;

  case 242:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6251 "ds_parser.cpp"
    break;

  case 243:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6257 "ds_parser.cpp"
    break;

  case 244:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6263 "ds_parser.cpp"
    break;

  case 245:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6269 "ds_parser.cpp"
    break;

  case 246:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6275 "ds_parser.cpp"
    break;

  case 247:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6281 "ds_parser.cpp"
    break;

  case 248:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6287 "ds_parser.cpp"
    break;

  case 249:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6293 "ds_parser.cpp"
    break;

  case 250:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6299 "ds_parser.cpp"
    break;

  case 251:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6305 "ds_parser.cpp"
    break;

  case 252:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6311 "ds_parser.cpp"
    break;

  case 253:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6317 "ds_parser.cpp"
    break;

  case 254:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6323 "ds_parser.cpp"
    break;

  case 255:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6329 "ds_parser.cpp"
    break;

  case 256:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6335 "ds_parser.cpp"
    break;

  case 257:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6341 "ds_parser.cpp"
    break;

  case 258:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6347 "ds_parser.cpp"
    break;

  case 259:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6353 "ds_parser.cpp"
    break;

  case 260:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6359 "ds_parser.cpp"
    break;

  case 261:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6365 "ds_parser.cpp"
    break;

  case 262:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6371 "ds_parser.cpp"
    break;

  case 263:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6377 "ds_parser.cpp"
    break;

  case 264:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6383 "ds_parser.cpp"
    break;

  case 265:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6389 "ds_parser.cpp"
    break;

  case 266:
#line 1532 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6401 "ds_parser.cpp"
    break;

  case 267:
#line 1543 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6411 "ds_parser.cpp"
    break;

  case 268:
#line 1548 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6423 "ds_parser.cpp"
    break;

  case 269:
#line 1558 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6432 "ds_parser.cpp"
    break;

  case 270:
#line 1562 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6438 "ds_parser.cpp"
    break;

  case 271:
#line 1562 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6444 "ds_parser.cpp"
    break;

  case 272:
#line 1562 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6456 "ds_parser.cpp"
    break;

  case 273:
#line 1569 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6462 "ds_parser.cpp"
    break;

  case 274:
#line 1569 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6468 "ds_parser.cpp"
    break;

  case 275:
#line 1569 "ds_parser.ypp"
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
#line 6484 "ds_parser.cpp"
    break;

  case 276:
#line 1583 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6493 "ds_parser.cpp"
    break;

  case 277:
#line 1587 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6502 "ds_parser.cpp"
    break;

  case 278:
#line 1591 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6508 "ds_parser.cpp"
    break;

  case 279:
#line 1591 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6514 "ds_parser.cpp"
    break;

  case 280:
#line 1591 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6523 "ds_parser.cpp"
    break;

  case 281:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6529 "ds_parser.cpp"
    break;

  case 282:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6535 "ds_parser.cpp"
    break;

  case 283:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6541 "ds_parser.cpp"
    break;

  case 284:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6547 "ds_parser.cpp"
    break;

  case 285:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6553 "ds_parser.cpp"
    break;

  case 286:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6559 "ds_parser.cpp"
    break;

  case 287:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6565 "ds_parser.cpp"
    break;

  case 288:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6571 "ds_parser.cpp"
    break;

  case 289:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6577 "ds_parser.cpp"
    break;

  case 290:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6583 "ds_parser.cpp"
    break;

  case 291:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6589 "ds_parser.cpp"
    break;

  case 292:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6595 "ds_parser.cpp"
    break;

  case 293:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6601 "ds_parser.cpp"
    break;

  case 294:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6607 "ds_parser.cpp"
    break;

  case 295:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6613 "ds_parser.cpp"
    break;

  case 296:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6619 "ds_parser.cpp"
    break;

  case 297:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6625 "ds_parser.cpp"
    break;

  case 298:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6631 "ds_parser.cpp"
    break;

  case 299:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6637 "ds_parser.cpp"
    break;

  case 300:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6643 "ds_parser.cpp"
    break;

  case 301:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6649 "ds_parser.cpp"
    break;

  case 302:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6655 "ds_parser.cpp"
    break;

  case 303:
#line 1620 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6661 "ds_parser.cpp"
    break;

  case 304:
#line 1621 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6667 "ds_parser.cpp"
    break;

  case 305:
#line 1622 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6673 "ds_parser.cpp"
    break;

  case 306:
#line 1623 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6679 "ds_parser.cpp"
    break;

  case 307:
#line 1624 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6685 "ds_parser.cpp"
    break;

  case 308:
#line 1625 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6691 "ds_parser.cpp"
    break;

  case 309:
#line 1626 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6697 "ds_parser.cpp"
    break;

  case 310:
#line 1627 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6703 "ds_parser.cpp"
    break;

  case 311:
#line 1628 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6709 "ds_parser.cpp"
    break;

  case 312:
#line 1629 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6715 "ds_parser.cpp"
    break;

  case 313:
#line 1630 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6721 "ds_parser.cpp"
    break;

  case 314:
#line 1631 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6727 "ds_parser.cpp"
    break;

  case 315:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6733 "ds_parser.cpp"
    break;

  case 316:
#line 1633 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6739 "ds_parser.cpp"
    break;

  case 317:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6745 "ds_parser.cpp"
    break;

  case 318:
#line 1635 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6751 "ds_parser.cpp"
    break;

  case 319:
#line 1636 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6757 "ds_parser.cpp"
    break;

  case 320:
#line 1637 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6763 "ds_parser.cpp"
    break;

  case 321:
#line 1638 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6769 "ds_parser.cpp"
    break;

  case 322:
#line 1639 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6775 "ds_parser.cpp"
    break;

  case 323:
#line 1640 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6781 "ds_parser.cpp"
    break;

  case 324:
#line 1641 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6787 "ds_parser.cpp"
    break;

  case 325:
#line 1642 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6793 "ds_parser.cpp"
    break;

  case 326:
#line 1643 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6799 "ds_parser.cpp"
    break;

  case 327:
#line 1644 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6808 "ds_parser.cpp"
    break;

  case 328:
#line 1648 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6817 "ds_parser.cpp"
    break;

  case 329:
#line 1652 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6825 "ds_parser.cpp"
    break;

  case 330:
#line 1655 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6833 "ds_parser.cpp"
    break;

  case 331:
#line 1658 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6839 "ds_parser.cpp"
    break;

  case 332:
#line 1659 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6845 "ds_parser.cpp"
    break;

  case 333:
#line 1660 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6851 "ds_parser.cpp"
    break;

  case 334:
#line 1661 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6865 "ds_parser.cpp"
    break;

  case 335:
#line 1670 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6879 "ds_parser.cpp"
    break;

  case 336:
#line 1679 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6885 "ds_parser.cpp"
    break;

  case 337:
#line 1680 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6893 "ds_parser.cpp"
    break;

  case 338:
#line 1683 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6899 "ds_parser.cpp"
    break;

  case 339:
#line 1683 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6905 "ds_parser.cpp"
    break;

  case 340:
#line 1683 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6913 "ds_parser.cpp"
    break;

  case 341:
#line 1686 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6922 "ds_parser.cpp"
    break;

  case 342:
#line 1690 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6931 "ds_parser.cpp"
    break;

  case 343:
#line 1694 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6940 "ds_parser.cpp"
    break;

  case 344:
#line 1698 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6946 "ds_parser.cpp"
    break;

  case 345:
#line 1699 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6952 "ds_parser.cpp"
    break;

  case 346:
#line 1700 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6958 "ds_parser.cpp"
    break;

  case 347:
#line 1701 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6964 "ds_parser.cpp"
    break;

  case 348:
#line 1702 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6970 "ds_parser.cpp"
    break;

  case 349:
#line 1703 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6976 "ds_parser.cpp"
    break;

  case 350:
#line 1704 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6982 "ds_parser.cpp"
    break;

  case 351:
#line 1705 "ds_parser.ypp"
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
#line 7003 "ds_parser.cpp"
    break;

  case 352:
#line 1721 "ds_parser.ypp"
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
#line 7024 "ds_parser.cpp"
    break;

  case 353:
#line 1737 "ds_parser.ypp"
    {
        Enumeration * pEnum = nullptr;
        Expression * resConst = nullptr;
        auto enums = g_Program->findEnum(*(yyvsp[-1].s));
        auto aliases = g_Program->findAlias(*(yyvsp[-1].s));
        if ( enums.size()+aliases.size() > 1 ) {
            string candidates;
            if ( enums.size() ) candidates += g_Program->describeCandidates(enums);
            if ( aliases.size() ) candidates += g_Program->describeCandidates(aliases);
            das_yyerror("too many options for the " + *(yyvsp[-1].s) + "\n" + candidates, tokAt((yylsp[-1])),
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
#line 7084 "ds_parser.cpp"
    break;

  case 354:
#line 1792 "ds_parser.ypp"
    {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 7094 "ds_parser.cpp"
    break;

  case 355:
#line 1800 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7100 "ds_parser.cpp"
    break;

  case 356:
#line 1801 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7106 "ds_parser.cpp"
    break;

  case 357:
#line 1805 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_NONE; }
#line 7112 "ds_parser.cpp"
    break;

  case 358:
#line 1806 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_OVERRIDE; }
#line 7118 "ds_parser.cpp"
    break;

  case 359:
#line 1807 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_SEALED; }
#line 7124 "ds_parser.cpp"
    break;

  case 360:
#line 1811 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7130 "ds_parser.cpp"
    break;

  case 361:
#line 1812 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7136 "ds_parser.cpp"
    break;

  case 362:
#line 1816 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7142 "ds_parser.cpp"
    break;

  case 363:
#line 1817 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7148 "ds_parser.cpp"
    break;

  case 364:
#line 1818 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7154 "ds_parser.cpp"
    break;

  case 365:
#line 1822 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7166 "ds_parser.cpp"
    break;

  case 366:
#line 1832 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7174 "ds_parser.cpp"
    break;

  case 367:
#line 1835 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7183 "ds_parser.cpp"
    break;

  case 368:
#line 1840 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7189 "ds_parser.cpp"
    break;

  case 369:
#line 1840 "ds_parser.ypp"
    {
        if ( !g_thisStructure->isClass ) {
            das_yyerror("structure can't have a member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->name==g_thisStructure->name || (yyvsp[-1].pFuncDecl)->name=="finalize" ) {
            das_yyerror("initializers and finalizers can't be abstract " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-6].faList)!=nullptr ) {
            das_yyerror("abstract functions can't have annotations " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
            delete (yyvsp[-6].faList);
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
            decl->isPrivate = (yyvsp[-4].b);
            (yyvsp[-7].pVarDeclList)->push_back(decl);
        }
        (yyvsp[-1].pFuncDecl)->delRef();
        (yyval.pVarDeclList) = (yyvsp[-7].pVarDeclList);
    }
#line 7239 "ds_parser.cpp"
    break;

  case 370:
#line 1887 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->atDecl = tokRangeAt((yylsp[-5]),(yylsp[0]));
        (yyvsp[-1].pFuncDecl)->body = (yyvsp[0].pExpression);
        if ( !g_thisStructure ) {
            das_yyerror("internal error or invalid macro. member function is declared outside of a class",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( !g_thisStructure->isClass ) {
            das_yyerror("structure can't have a member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            (yyvsp[-1].pFuncDecl)->privateFunction = g_thisStructure->privateStructure;
            if ( (yyvsp[-1].pFuncDecl)->name != g_thisStructure->name && (yyvsp[-1].pFuncDecl)->name != "finalize") {
                auto varName = (yyvsp[-1].pFuncDecl)->name;
                (yyvsp[-1].pFuncDecl)->name = g_thisStructure->name + "`" + (yyvsp[-1].pFuncDecl)->name;
                auto vars = new vector<pair<string,LineInfo>>();
                vars->emplace_back(make_pair(varName,(yyvsp[-1].pFuncDecl)->at));
                Expression * finit = new ExprAddr((yyvsp[-1].pFuncDecl)->at, inThisModule((yyvsp[-1].pFuncDecl)->name));
                if ( (yyvsp[-3].i) == OVERRIDE_OVERRIDE ) {
                    finit = new ExprCast((yyvsp[-1].pFuncDecl)->at, finit, make_smart<TypeDecl>(Type::autoinfer));
                }
                VariableDeclaration * decl = new VariableDeclaration(
                    vars,
                    new TypeDecl(Type::autoinfer),
                    finit
                );
                decl->override = (yyvsp[-3].i) == OVERRIDE_OVERRIDE;
                decl->sealed = (yyvsp[-3].i) == OVERRIDE_SEALED;
                decl->isPrivate = (yyvsp[-4].b);
                (yyvsp[-7].pVarDeclList)->push_back(decl);
                modifyToClassMember((yyvsp[-1].pFuncDecl), g_thisStructure, false, (yyvsp[-2].b));
            } else {
                if ( (yyvsp[-3].i) ) {
                    das_yyerror("can't override an initializer or a finalizer " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                        (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[-2].b) ) {
                    das_yyerror("can't have a constant initializer or a finalizer " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                        (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[-1].pFuncDecl)->name!="finalize" ) {
                    auto ctr = makeClassConstructor(g_thisStructure, (yyvsp[-1].pFuncDecl));
                    if ( !g_Program->addFunction(ctr) ) {
                        das_yyerror("intializer is already defined " + ctr->getMangledName(),
                            ctr->at, CompilationError::function_already_declared);
                    }
                    (yyvsp[-1].pFuncDecl)->name = g_thisStructure->name + "`" + g_thisStructure->name;
                    modifyToClassMember((yyvsp[-1].pFuncDecl), g_thisStructure, false, false);
                } else {
                    modifyToClassMember((yyvsp[-1].pFuncDecl), g_thisStructure, true, false);
                }
            }
            assignDefaultArguments((yyvsp[-1].pFuncDecl));
            runFunctionAnnotations((yyvsp[-1].pFuncDecl), (yyvsp[-6].faList), tokAt((yylsp[-6])));
            if ( !g_Program->addFunction((yyvsp[-1].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                    (yyvsp[-1].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[-1].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-7].pVarDeclList);
    }
#line 7308 "ds_parser.cpp"
    break;

  case 371:
#line 1951 "ds_parser.ypp"
    {
        das_yyerror("structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7319 "ds_parser.cpp"
    break;

  case 372:
#line 1960 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7329 "ds_parser.cpp"
    break;

  case 373:
#line 1965 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7339 "ds_parser.cpp"
    break;

  case 374:
#line 1973 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7345 "ds_parser.cpp"
    break;

  case 375:
#line 1974 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7351 "ds_parser.cpp"
    break;

  case 376:
#line 1978 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7359 "ds_parser.cpp"
    break;

  case 377:
#line 1981 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7370 "ds_parser.cpp"
    break;

  case 378:
#line 1990 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7376 "ds_parser.cpp"
    break;

  case 379:
#line 1991 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7382 "ds_parser.cpp"
    break;

  case 380:
#line 1995 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7393 "ds_parser.cpp"
    break;

  case 381:
#line 2004 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7399 "ds_parser.cpp"
    break;

  case 382:
#line 2005 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7405 "ds_parser.cpp"
    break;

  case 383:
#line 2010 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7411 "ds_parser.cpp"
    break;

  case 384:
#line 2011 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7417 "ds_parser.cpp"
    break;

  case 385:
#line 2015 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7428 "ds_parser.cpp"
    break;

  case 386:
#line 2021 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7439 "ds_parser.cpp"
    break;

  case 387:
#line 2027 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7447 "ds_parser.cpp"
    break;

  case 388:
#line 2030 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7456 "ds_parser.cpp"
    break;

  case 389:
#line 2034 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7467 "ds_parser.cpp"
    break;

  case 390:
#line 2040 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7478 "ds_parser.cpp"
    break;

  case 391:
#line 2049 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7484 "ds_parser.cpp"
    break;

  case 392:
#line 2050 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7490 "ds_parser.cpp"
    break;

  case 393:
#line 2051 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7496 "ds_parser.cpp"
    break;

  case 394:
#line 2055 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7502 "ds_parser.cpp"
    break;

  case 395:
#line 2056 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7508 "ds_parser.cpp"
    break;

  case 396:
#line 2060 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7516 "ds_parser.cpp"
    break;

  case 397:
#line 2063 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7526 "ds_parser.cpp"
    break;

  case 398:
#line 2068 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7536 "ds_parser.cpp"
    break;

  case 399:
#line 2073 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7549 "ds_parser.cpp"
    break;

  case 400:
#line 2081 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7562 "ds_parser.cpp"
    break;

  case 401:
#line 2092 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7570 "ds_parser.cpp"
    break;

  case 402:
#line 2095 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7580 "ds_parser.cpp"
    break;

  case 403:
#line 2103 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7586 "ds_parser.cpp"
    break;

  case 404:
#line 2104 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7592 "ds_parser.cpp"
    break;

  case 405:
#line 2108 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7598 "ds_parser.cpp"
    break;

  case 406:
#line 2109 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7604 "ds_parser.cpp"
    break;

  case 407:
#line 2110 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7610 "ds_parser.cpp"
    break;

  case 408:
#line 2114 "ds_parser.ypp"
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
#line 7643 "ds_parser.cpp"
    break;

  case 409:
#line 2142 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7649 "ds_parser.cpp"
    break;

  case 410:
#line 2142 "ds_parser.ypp"
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
#line 7685 "ds_parser.cpp"
    break;

  case 411:
#line 2176 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7693 "ds_parser.cpp"
    break;

  case 412:
#line 2179 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7707 "ds_parser.cpp"
    break;

  case 413:
#line 2188 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7721 "ds_parser.cpp"
    break;

  case 414:
#line 2201 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7735 "ds_parser.cpp"
    break;

  case 419:
#line 2223 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7741 "ds_parser.cpp"
    break;

  case 420:
#line 2224 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7747 "ds_parser.cpp"
    break;

  case 421:
#line 2225 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7753 "ds_parser.cpp"
    break;

  case 422:
#line 2229 "ds_parser.ypp"
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
                            das_yyerror("macro [" +pA->annotation->name + "] failed to finalize the enumeration " + pEnum->name + "\n" + err, tokAt((yylsp[-6])),
                                CompilationError::invalid_annotation);
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
#line 7786 "ds_parser.cpp"
    break;

  case 423:
#line 2257 "ds_parser.ypp"
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
                            das_yyerror("macro [" +pA->annotation->name + "] failed to finalize the enumeration " +pEnum->name + "\n" + err, tokAt((yylsp[-8])),
                                CompilationError::invalid_annotation);
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
#line 7820 "ds_parser.cpp"
    break;

  case 424:
#line 2289 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7826 "ds_parser.cpp"
    break;

  case 425:
#line 2290 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7832 "ds_parser.cpp"
    break;

  case 426:
#line 2294 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7838 "ds_parser.cpp"
    break;

  case 427:
#line 2295 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7844 "ds_parser.cpp"
    break;

  case 428:
#line 2299 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
                pStruct->parent = structs.back().get();
                if ( pStruct->parent->sealed ) {
                    das_yyerror("can't derive from a sealed class or structure "+*(yyvsp[0].s),tokAt((yylsp[0])),
                        CompilationError::invalid_override);
                }
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
        pStruct->sealed = (yyvsp[-2].b);
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
#line 7890 "ds_parser.cpp"
    break;

  case 429:
#line 2343 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7896 "ds_parser.cpp"
    break;

  case 430:
#line 2344 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7902 "ds_parser.cpp"
    break;

  case 431:
#line 2348 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7908 "ds_parser.cpp"
    break;

  case 432:
#line 2349 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7914 "ds_parser.cpp"
    break;

  case 433:
#line 2350 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7920 "ds_parser.cpp"
    break;

  case 434:
#line 2355 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7926 "ds_parser.cpp"
    break;

  case 435:
#line 2355 "ds_parser.ypp"
    {
        if ( (yyvsp[-4].pStructure) ) {
            auto pStruct = (yyvsp[-4].pStructure);
            pStruct->at = tokAt((yylsp[-4]));
            if ( pStruct->parent && pStruct->parent->isClass != pStruct->isClass ) {
                if ( pStruct->isClass ) {
                    das_yyerror("class can only derive from a class", pStruct->at,
                        CompilationError::invalid_override);
                } else {
                    das_yyerror("structure can only derive from a structure", pStruct->at,
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
                    if ( !pStruct->isClass && pDecl->isPrivate ) {
                        das_yyerror("only class member can be private "+name_at.first,name_at.second,
                            CompilationError::invalid_private);
                    }
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name_at.first);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything "+name_at.first,name_at.second,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name_at.first, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, name_at.second);
                            pStruct->fields.back().privateField = pDecl->isPrivate;
                            pStruct->fields.back().sealed = pDecl->sealed;
                        }
                    } else {
                        if ( pDecl->sealed || pDecl->override ) {
                            if ( oldFd->sealed ) {
                                das_yyerror("structure field "+name_at.first+" is sealed",
                                    name_at.second, CompilationError::invalid_override);
                            }
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            oldFd->init = init;
                            oldFd->parentType = oldFd->type->isAuto();
                            oldFd->privateField = pDecl->isPrivate;
                            oldFd->sealed = pDecl->sealed;
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
                                das_yyerror("macro [" +pA->annotation->name + "] failed to apply to the structure " + pStruct->name + "\n" + err,
                                    tokAt((yylsp[-6])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-7].faList)->size()!=1 ) {
                                das_yyerror("structures are only allowed one structure macro", tokAt((yylsp[-6])),
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
#line 8024 "ds_parser.cpp"
    break;

  case 436:
#line 2451 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8036 "ds_parser.cpp"
    break;

  case 437:
#line 2458 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 8047 "ds_parser.cpp"
    break;

  case 438:
#line 2467 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 8053 "ds_parser.cpp"
    break;

  case 439:
#line 2468 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 8059 "ds_parser.cpp"
    break;

  case 440:
#line 2469 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8065 "ds_parser.cpp"
    break;

  case 441:
#line 2470 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8071 "ds_parser.cpp"
    break;

  case 442:
#line 2471 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8077 "ds_parser.cpp"
    break;

  case 443:
#line 2472 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 8083 "ds_parser.cpp"
    break;

  case 444:
#line 2473 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 8089 "ds_parser.cpp"
    break;

  case 445:
#line 2474 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 8095 "ds_parser.cpp"
    break;

  case 446:
#line 2475 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8101 "ds_parser.cpp"
    break;

  case 447:
#line 2476 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8107 "ds_parser.cpp"
    break;

  case 448:
#line 2477 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8113 "ds_parser.cpp"
    break;

  case 449:
#line 2478 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8119 "ds_parser.cpp"
    break;

  case 450:
#line 2479 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8125 "ds_parser.cpp"
    break;

  case 451:
#line 2480 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8131 "ds_parser.cpp"
    break;

  case 452:
#line 2481 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8137 "ds_parser.cpp"
    break;

  case 453:
#line 2482 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8143 "ds_parser.cpp"
    break;

  case 454:
#line 2483 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8149 "ds_parser.cpp"
    break;

  case 455:
#line 2484 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8155 "ds_parser.cpp"
    break;

  case 456:
#line 2485 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8161 "ds_parser.cpp"
    break;

  case 457:
#line 2486 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8167 "ds_parser.cpp"
    break;

  case 458:
#line 2487 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8173 "ds_parser.cpp"
    break;

  case 459:
#line 2488 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8179 "ds_parser.cpp"
    break;

  case 460:
#line 2489 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8185 "ds_parser.cpp"
    break;

  case 461:
#line 2490 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8191 "ds_parser.cpp"
    break;

  case 462:
#line 2491 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8197 "ds_parser.cpp"
    break;

  case 463:
#line 2495 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8203 "ds_parser.cpp"
    break;

  case 464:
#line 2496 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8209 "ds_parser.cpp"
    break;

  case 465:
#line 2497 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8215 "ds_parser.cpp"
    break;

  case 466:
#line 2498 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8221 "ds_parser.cpp"
    break;

  case 467:
#line 2499 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8227 "ds_parser.cpp"
    break;

  case 468:
#line 2500 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8233 "ds_parser.cpp"
    break;

  case 469:
#line 2504 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8246 "ds_parser.cpp"
    break;

  case 470:
#line 2515 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8255 "ds_parser.cpp"
    break;

  case 471:
#line 2519 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8267 "ds_parser.cpp"
    break;

  case 472:
#line 2529 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8279 "ds_parser.cpp"
    break;

  case 473:
#line 2536 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8290 "ds_parser.cpp"
    break;

  case 474:
#line 2545 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8296 "ds_parser.cpp"
    break;

  case 475:
#line 2545 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8302 "ds_parser.cpp"
    break;

  case 476:
#line 2545 "ds_parser.ypp"
    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror("only 32 different bits are allowed in a bitfield",tokAt((yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
#line 8317 "ds_parser.cpp"
    break;

  case 477:
#line 2558 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8323 "ds_parser.cpp"
    break;

  case 478:
#line 2559 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8329 "ds_parser.cpp"
    break;

  case 479:
#line 2560 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8335 "ds_parser.cpp"
    break;

  case 480:
#line 2561 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8341 "ds_parser.cpp"
    break;

  case 481:
#line 2562 "ds_parser.ypp"
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
#line 8360 "ds_parser.cpp"
    break;

  case 482:
#line 2576 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8371 "ds_parser.cpp"
    break;

  case 483:
#line 2582 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8380 "ds_parser.cpp"
    break;

  case 484:
#line 2586 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8389 "ds_parser.cpp"
    break;

  case 485:
#line 2590 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8399 "ds_parser.cpp"
    break;

  case 486:
#line 2595 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8409 "ds_parser.cpp"
    break;

  case 487:
#line 2600 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8419 "ds_parser.cpp"
    break;

  case 488:
#line 2605 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8429 "ds_parser.cpp"
    break;

  case 489:
#line 2610 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8438 "ds_parser.cpp"
    break;

  case 490:
#line 2614 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8447 "ds_parser.cpp"
    break;

  case 491:
#line 2618 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8457 "ds_parser.cpp"
    break;

  case 492:
#line 2623 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8466 "ds_parser.cpp"
    break;

  case 493:
#line 2627 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8476 "ds_parser.cpp"
    break;

  case 494:
#line 2632 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8482 "ds_parser.cpp"
    break;

  case 495:
#line 2632 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8488 "ds_parser.cpp"
    break;

  case 496:
#line 2632 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8499 "ds_parser.cpp"
    break;

  case 497:
#line 2638 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8511 "ds_parser.cpp"
    break;

  case 498:
#line 2645 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8517 "ds_parser.cpp"
    break;

  case 499:
#line 2645 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8523 "ds_parser.cpp"
    break;

  case 500:
#line 2645 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8533 "ds_parser.cpp"
    break;

  case 501:
#line 2650 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8539 "ds_parser.cpp"
    break;

  case 502:
#line 2650 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8545 "ds_parser.cpp"
    break;

  case 503:
#line 2650 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8556 "ds_parser.cpp"
    break;

  case 504:
#line 2656 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8562 "ds_parser.cpp"
    break;

  case 505:
#line 2656 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8568 "ds_parser.cpp"
    break;

  case 506:
#line 2656 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8578 "ds_parser.cpp"
    break;

  case 507:
#line 2661 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8587 "ds_parser.cpp"
    break;

  case 508:
#line 2665 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8593 "ds_parser.cpp"
    break;

  case 509:
#line 2665 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8599 "ds_parser.cpp"
    break;

  case 510:
#line 2665 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8609 "ds_parser.cpp"
    break;

  case 511:
#line 2670 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8615 "ds_parser.cpp"
    break;

  case 512:
#line 2670 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8621 "ds_parser.cpp"
    break;

  case 513:
#line 2670 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8635 "ds_parser.cpp"
    break;

  case 514:
#line 2679 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8644 "ds_parser.cpp"
    break;

  case 515:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8650 "ds_parser.cpp"
    break;

  case 516:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8656 "ds_parser.cpp"
    break;

  case 517:
#line 2683 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8666 "ds_parser.cpp"
    break;

  case 518:
#line 2688 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8672 "ds_parser.cpp"
    break;

  case 519:
#line 2688 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8678 "ds_parser.cpp"
    break;

  case 520:
#line 2688 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8692 "ds_parser.cpp"
    break;

  case 521:
#line 2697 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8701 "ds_parser.cpp"
    break;

  case 522:
#line 2701 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8707 "ds_parser.cpp"
    break;

  case 523:
#line 2701 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8713 "ds_parser.cpp"
    break;

  case 524:
#line 2701 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8723 "ds_parser.cpp"
    break;

  case 525:
#line 2706 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8729 "ds_parser.cpp"
    break;

  case 526:
#line 2706 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8735 "ds_parser.cpp"
    break;

  case 527:
#line 2706 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8749 "ds_parser.cpp"
    break;

  case 528:
#line 2715 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8755 "ds_parser.cpp"
    break;

  case 529:
#line 2715 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8761 "ds_parser.cpp"
    break;

  case 530:
#line 2715 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8772 "ds_parser.cpp"
    break;

  case 531:
#line 2721 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8778 "ds_parser.cpp"
    break;

  case 532:
#line 2721 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8784 "ds_parser.cpp"
    break;

  case 533:
#line 2721 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8795 "ds_parser.cpp"
    break;

  case 534:
#line 2730 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8801 "ds_parser.cpp"
    break;

  case 535:
#line 2730 "ds_parser.ypp"
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
#line 8818 "ds_parser.cpp"
    break;

  case 536:
#line 2745 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8824 "ds_parser.cpp"
    break;

  case 537:
#line 2745 "ds_parser.ypp"
    {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt((yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror("only 32 different bits are allowed in a bitfield",tokAt((yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !g_Program->addAlias(btype) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-4].s),tokAt((yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
#line 8845 "ds_parser.cpp"
    break;

  case 538:
#line 2765 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8851 "ds_parser.cpp"
    break;

  case 539:
#line 2766 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8857 "ds_parser.cpp"
    break;

  case 540:
#line 2767 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8863 "ds_parser.cpp"
    break;

  case 541:
#line 2768 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8869 "ds_parser.cpp"
    break;

  case 542:
#line 2772 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8881 "ds_parser.cpp"
    break;

  case 543:
#line 2779 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8893 "ds_parser.cpp"
    break;

  case 544:
#line 2786 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8904 "ds_parser.cpp"
    break;

  case 545:
#line 2792 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8915 "ds_parser.cpp"
    break;

  case 546:
#line 2801 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8925 "ds_parser.cpp"
    break;

  case 547:
#line 2806 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8934 "ds_parser.cpp"
    break;

  case 548:
#line 2813 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8940 "ds_parser.cpp"
    break;

  case 549:
#line 2814 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8946 "ds_parser.cpp"
    break;

  case 550:
#line 2818 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8957 "ds_parser.cpp"
    break;

  case 551:
#line 2824 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8969 "ds_parser.cpp"
    break;

  case 552:
#line 2831 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8982 "ds_parser.cpp"
    break;

  case 553:
#line 2839 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8994 "ds_parser.cpp"
    break;

  case 554:
#line 2846 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9007 "ds_parser.cpp"
    break;

  case 555:
#line 2854 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9021 "ds_parser.cpp"
    break;

  case 556:
#line 2866 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 9029 "ds_parser.cpp"
    break;

  case 557:
#line 2869 "ds_parser.ypp"
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
#line 9045 "ds_parser.cpp"
    break;

  case 558:
#line 2883 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9056 "ds_parser.cpp"
    break;

  case 559:
#line 2892 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9062 "ds_parser.cpp"
    break;

  case 560:
#line 2893 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9068 "ds_parser.cpp"
    break;

  case 561:
#line 2897 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9078 "ds_parser.cpp"
    break;

  case 562:
#line 2902 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9087 "ds_parser.cpp"
    break;

  case 563:
#line 2909 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9097 "ds_parser.cpp"
    break;

  case 564:
#line 2914 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9109 "ds_parser.cpp"
    break;

  case 565:
#line 2924 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9119 "ds_parser.cpp"
    break;

  case 566:
#line 2929 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9128 "ds_parser.cpp"
    break;

  case 567:
#line 2936 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9142 "ds_parser.cpp"
    break;

  case 568:
#line 2948 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9148 "ds_parser.cpp"
    break;

  case 569:
#line 2949 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9154 "ds_parser.cpp"
    break;

  case 570:
#line 2953 "ds_parser.ypp"
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
#line 9177 "ds_parser.cpp"
    break;

  case 571:
#line 2971 "ds_parser.ypp"
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
#line 9200 "ds_parser.cpp"
    break;


#line 9204 "ds_parser.cpp"

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
#line 2991 "ds_parser.ypp"


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
                    das_yyerror("can't have default values in a type declaration",
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
                        das_yyerror("macro [" +pA->annotation->name + "] failed to apply to a function " + func->name + "\n" + err, at,
                            CompilationError::invalid_annotation);
                    }
                } else {
                    das_yyerror("functions are only allowed function macros",
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
