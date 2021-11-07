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
#line 70 "ds_parser.ypp"

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
#line 102 "ds_parser.ypp"

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
#define YYLAST   7747

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  181
/* YYNRULES -- Number of rules.  */
#define YYNRULES  568
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  991

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
       0,   464,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   480,   481,   482,   486,   487,
     491,   508,   509,   510,   511,   515,   519,   524,   533,   541,
     557,   562,   570,   570,   600,   622,   626,   629,   633,   639,
     648,   669,   692,   693,   697,   701,   702,   706,   709,   715,
     721,   724,   730,   731,   735,   736,   737,   746,   747,   751,
     760,   776,   784,   794,   803,   804,   805,   806,   807,   808,
     812,   817,   825,   826,   827,   831,   832,   833,   834,   835,
     836,   837,   838,   844,   858,   875,   876,   877,   881,   889,
     902,   906,   913,   914,   918,   919,   920,   924,   927,   934,
     938,   939,   940,   941,   942,   943,   944,   945,   946,   947,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,   961,   962,   963,   964,   965,   966,   967,
     968,   969,   970,   971,   972,   973,   974,   975,   976,   977,
     978,   979,   980,   981,   982,   983,   984,   985,   989,  1011,
    1012,  1013,  1017,  1055,  1063,  1067,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1100,  1105,  1111,  1117,  1151,
    1154,  1160,  1161,  1172,  1176,  1182,  1185,  1188,  1192,  1198,
    1202,  1206,  1209,  1212,  1217,  1220,  1228,  1231,  1236,  1239,
    1247,  1253,  1254,  1258,  1292,  1292,  1292,  1295,  1295,  1295,
    1300,  1300,  1300,  1308,  1308,  1308,  1314,  1324,  1335,  1350,
    1353,  1359,  1360,  1367,  1378,  1379,  1380,  1384,  1385,  1386,
    1387,  1391,  1396,  1404,  1405,  1409,  1414,  1478,  1479,  1480,
    1481,  1482,  1483,  1487,  1488,  1489,  1490,  1491,  1492,  1493,
    1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,
    1504,  1505,  1509,  1510,  1511,  1512,  1516,  1527,  1532,  1542,
    1546,  1546,  1546,  1553,  1553,  1553,  1567,  1571,  1575,  1575,
    1575,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,
    1621,  1622,  1623,  1624,  1625,  1626,  1627,  1628,  1632,  1636,
    1639,  1642,  1643,  1644,  1645,  1654,  1663,  1664,  1667,  1667,
    1667,  1670,  1674,  1678,  1682,  1683,  1684,  1685,  1686,  1687,
    1688,  1689,  1705,  1721,  1779,  1780,  1784,  1785,  1786,  1790,
    1791,  1795,  1796,  1797,  1801,  1811,  1814,  1819,  1818,  1864,
    1930,  1939,  1944,  1952,  1953,  1957,  1960,  1969,  1970,  1974,
    1983,  1984,  1989,  1990,  1994,  2000,  2006,  2009,  2013,  2019,
    2028,  2029,  2030,  2034,  2035,  2039,  2042,  2047,  2052,  2060,
    2071,  2074,  2082,  2083,  2087,  2088,  2089,  2093,  2121,  2121,
    2155,  2158,  2167,  2180,  2192,  2193,  2197,  2198,  2202,  2203,
    2204,  2208,  2236,  2268,  2269,  2273,  2311,  2312,  2316,  2317,
    2318,  2323,  2322,  2419,  2426,  2435,  2436,  2437,  2438,  2439,
    2440,  2441,  2442,  2443,  2444,  2445,  2446,  2447,  2448,  2449,
    2450,  2451,  2452,  2453,  2454,  2455,  2456,  2457,  2458,  2459,
    2463,  2464,  2465,  2466,  2467,  2468,  2472,  2483,  2487,  2497,
    2504,  2513,  2513,  2513,  2526,  2527,  2528,  2529,  2530,  2544,
    2550,  2554,  2558,  2563,  2568,  2573,  2578,  2582,  2586,  2591,
    2595,  2600,  2600,  2600,  2606,  2613,  2613,  2613,  2618,  2618,
    2618,  2624,  2624,  2624,  2629,  2633,  2633,  2633,  2638,  2638,
    2638,  2647,  2651,  2651,  2651,  2656,  2656,  2656,  2665,  2669,
    2669,  2669,  2674,  2674,  2674,  2683,  2683,  2683,  2689,  2689,
    2689,  2698,  2698,  2713,  2713,  2733,  2734,  2735,  2736,  2740,
    2747,  2754,  2760,  2769,  2774,  2781,  2782,  2786,  2792,  2799,
    2807,  2814,  2822,  2834,  2837,  2851,  2860,  2861,  2865,  2870,
    2877,  2882,  2892,  2897,  2904,  2916,  2917,  2921,  2939
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
     405,   406,   407,   408,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,   409,   410,
     126,    33,   411,   412,   413,   414,   415,    46,    91,    93,
      40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -559

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-559)))

#define YYTABLE_NINF -523

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -559,    21,  -559,  -559,    44,   -83,    84,   -99,  -559,   -34,
    -559,  -559,    50,  -559,  -559,  -559,  -559,  -559,   254,  -559,
      -5,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
     -54,  -559,   -18,   171,     3,  -559,  -559,    84,    15,  -559,
      46,    -2,  -559,  -559,  -559,    -5,    93,   128,  -559,  -559,
     137,   206,   177,  -559,    83,  -559,  -559,  -559,   160,   195,
     360,  -559,   361,    26,    44,   225,   -83,   200,   248,  -559,
     266,   269,  -559,  7514,   198,   -64,   372,   219,   241,  -559,
     287,    44,    50,  -559,  -559,  -559,     1,  -559,  -559,  -559,
     300,  -559,  -559,   317,  -559,  -559,   285,  -559,  -559,  -559,
    -559,  -559,   301,    81,  -559,  -559,  -559,  -559,   333,  -559,
    -559,   324,   325,   329,   330,  -559,  -559,  -559,   298,  -559,
    -559,  -559,  -559,  -559,   331,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,   332,  -559,  -559,  -559,   335,   336,
    -559,  -559,  -559,  -559,   337,   340,  -559,  -559,  -559,  -559,
    -559,  2356,  -559,  -559,   299,  -559,  -559,  -559,   348,   362,
    -559,  -105,  -559,  7126,   472,   476,  -559,   334,   328,   -36,
     358,  -559,  -559,   380,  -559,  -559,   310,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,    13,  -559,  -559,  -559,  -559,  -559,
     373,  -559,   209,   211,   217,  -559,  -559,  -559,  -559,  -559,
    -559,   486,  -559,  -559,    37,  2998,  -559,  -559,  -559,   338,
     367,  -559,   341,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,   342,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,   347,
    -559,  -559,   -82,   371,  -559,  -559,   364,  -559,   201,  -559,
     344,   -63,    44,   386,  -559,  -559,  -559,    81,  -559,  7514,
    7514,  7514,  7514,   353,   348,  7514,   334,  7514,   334,  7514,
     334,  7601,   362,  -559,  -559,  -559,   351,  -559,  -559,  -559,
    7427,   355,   376,  -559,   359,   383,   387,   365,   388,  -559,
     389,  4646,  4646,  7253,  7340,  4646,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  4646,  4646,  4646,   201,  4646,  4646,  -559,
    4646,  -559,   370,  -559,  -559,  -559,  -103,  -559,  -559,  -559,
    -559,   377,  -559,  -559,  -559,  -559,  -559,  -559,  5791,   381,
    -559,  -559,  -559,  -559,  -559,   -43,  7514,   -26,  -559,  -559,
    -559,    -9,  -559,   151,  7514,  -559,  1299,  -559,  -559,  -559,
    -559,  -559,  -559,   374,    52,  -559,  -559,  -559,   386,   146,
    -559,  -559,   218,  -559,   349,   391,   570,   580,  -559,   -51,
     599,   371,   690,   371,   732,   371,   133,  -559,   124,  2356,
     150,  -559,   746,  -559,   201,  -559,  4646,  -559,  -559,  4646,
    -559,  7514,    45,    45,   386,  1646,   386,  1804,  6353,  -559,
     258,   318,    45,    45,   -37,  -559,    45,    45,  5236,   130,
    -559,  2420,   420,    10,   412,  4646,  4646,  -559,  -559,  4646,
    4646,  4646,  4646,   414,  4646,   416,  4646,  4646,  4646,  4646,
    4646,  4646,  4646,  4646,  3138,  4646,  4646,  4646,  4646,  4646,
    4646,  4646,  4646,  4646,  4646,   -46,  4646,  -559,  3278,  -559,
    -559,  2356,  -559,  -559,   386,  -559,   -70,  -559,   380,  2356,
    -559,  4646,  -559,  -559,   386,  2120,  -559,   328,  4646,  4646,
    -559,   382,   424,  3436,   328,  2280,   202,   202,  -559,   529,
     396,   397,  4646,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
     398,   400,   401,  -559,  -559,  -559,   386,  -559,   -74,  -559,
    6285,  -559,   -13,  -559,    17,  -559,   392,   422,  7514,  -559,
       0,  -559,  7514,  -559,  -559,  -559,   426,  -559,   408,  -559,
     411,  -559,   429,  7514,  -559,  7601,  -559,   362,  3576,  3734,
    7514,  5314,  7514,  7514,  5395,  7514,   925,     4,   202,   125,
    1962,  3874,  6353,   438,   -15,   418,   449,  -559,  -559,  -120,
      25,  4032,   -15,   278,  4646,  4646,   423,  -559,  4646,   307,
     456,  -559,   236,  -559,   460,  -559,   117,  6581,   -10,   334,
     450,  -559,  -559,    68,    68,   244,   244,  7043,  7043,   430,
      11,  -559,  5867,   -39,   -39,    68,    68,  6757,  6649,  6729,
     470,  6433,  6836,  6864,  7018,   244,   244,   322,   322,    11,
      11,    11,   471,  4646,  -559,   473,  4646,   619,  5943,  -559,
     138,  -559,  -559,  -559,  7514,  -559,  2578,  -559,   482,    29,
    2578,  -559,  -559,   501,  1114,   610,  6581,   482,  -559,   467,
     485,  6581,  -559,  2578,  -559,  4736,   445,  -559,  -559,   444,
    -559,  -559,   482,  -559,  -559,  -559,  -559,   202,  -559,  4646,
    4646,  4646,  4646,  4646,  4646,  4646,  4646,  4646,  4646,  2718,
    4646,  4646,  4646,  4646,  4646,  4646,  2858,    66,  4646,  -559,
      50,  -559,   623,     9,   448,  -559,  -559,   240,  -559,  -559,
    -559,  2578,  -559,  1014,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  2356,  -559,  -559,  -559,  -559,   141,   487,
    5473,  1421,  -559,  1582,  1740,  -559,  1898,   420,  4646,  -559,
    4646,  4646,   -11,   488,   460,   474,   475,  4646,   478,  4646,
    4646,   460,   453,   483,  6581,  -559,  -559,  6501,  -559,   502,
    7514,   334,   126,   162,  4646,  -559,   489,   505,   506,   515,
    -559,   167,   371,  -559,  4190,  -559,  -559,  4646,  -559,  6019,
    -559,  6095,  -559,  -559,  -559,  2059,  -559,  6184,  -559,  4646,
    -559,  4836,  4646,  4646,  -559,   328,  -559,  -559,  -559,  -559,
    4936,  -559,  -559,  -559,   379,  -559,  6581,  6581,  6581,  6581,
    6581,  6581,  6581,  6581,  6581,  6581,  4646,  4646,  6581,  6581,
    6581,  6581,  6581,  6581,  6581,  4646,  4646,  6581,  -559,   861,
     175,   384,  -559,  -559,   384,  -559,   486,  -559,  2578,  -559,
    5036,  -559,  -559,  -559,  -559,  -559,   161,  -559,  -559,  -559,
    -559,  -559,   484,    63,  6581,  6581,   -15,   490,   129,   438,
     491,  -559,  6581,  -559,  -559,    82,   -15,   492,  -559,  -559,
    -559,  2061,   371,   493,  6581,  -559,  -559,  -559,  -559,   -10,
     494,   -87,  7514,  -559,   145,  6581,  -559,  -559,  -559,   486,
    4646,   103,  -559,  -559,  -559,  1485,   328,  -559,  -559,  4646,
    -559,  6943,  6581,  6943,  6581,  -559,   479,  -559,  -559,    51,
     386,  -559,  5136,  -559,  -559,  4646,   519,  -559,  4646,  4646,
    4646,  4348,  4646,   496,   497,  4646,  4646,  -559,  4646,   480,
    -559,  -559,   510,  -559,  -559,  -559,  4506,  -559,  -559,  2219,
    -559,  6581,   328,  -559,  -559,   482,  -559,  -559,   636,  -559,
    -559,  5554,   518,   244,   244,   244,  -559,  5632,   988,   498,
    -559,  6581,  6581,   988,   503,   201,  -559,  4646,  6581,  -559,
    -559,   379,     1,  -559,     1,  -559,  4646,  -559,   632,   504,
    -559,   499,  -559,  -559,   201,  6581,  -559,  -559,   500,   328,
    5713,  4646,   508,   509,  -559,  -559,  -559,  -559,  6581,  -559,
    -559
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   201,     0,     0,     0,     0,   202,     0,
     533,   531,     0,    14,     3,    10,     9,     8,     0,     7,
     402,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   417,    19,    18,   402,     0,     0,    86,    87,
       0,   181,    88,    90,     0,    85,   427,   426,   149,   418,
     428,   403,   404,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   183,
       0,     0,     0,    93,   151,   150,     0,   148,   420,   419,
       0,   430,   429,     0,   406,   405,   408,    79,    80,    77,
      78,    76,     0,     0,    75,    84,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   435,   455,   436,   467,   437,
     441,   442,   443,   444,   459,   448,   449,   450,   451,   452,
     453,   454,   456,   457,   504,   440,   447,   458,   511,   518,
     438,   445,   439,   446,     0,     0,   466,   474,   477,   475,
     476,   413,   414,   416,     0,    16,    17,    20,     0,     0,
     182,     0,    91,     0,     0,     0,    99,    94,     0,     0,
     423,   431,   400,   354,    21,    22,     0,    68,    69,    66,
      67,    65,    64,    70,     0,    41,   495,   498,   501,   491,
       0,   471,   505,   512,   519,   525,   528,   482,   487,   481,
     494,     0,   490,   484,     0,     0,   486,   415,   469,     0,
       0,   380,     0,    89,   147,   102,   103,   105,   104,   106,
     107,   108,   109,   134,   135,   132,   133,   125,   136,   137,
     126,   123,   124,   145,     0,   146,   138,   139,   140,   141,
     113,   114,   115,   110,   111,   112,   128,   129,   127,   121,
     122,   117,   116,   118,   119,   120,   101,   100,   144,     0,
     130,   131,   354,    97,   175,   153,     0,   410,     0,   425,
       0,   354,     0,     0,    23,    24,    25,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    94,     0,    94,     0,
      94,     0,     0,   489,   483,   485,     0,   488,   287,   288,
       0,     0,     0,   281,     0,     0,     0,     0,     0,   459,
       0,     0,     0,     0,     0,     0,   237,   239,   238,   240,
     241,   242,    26,     0,     0,     0,     0,     0,     0,   479,
       0,   224,   225,   285,   284,   235,   282,   347,   346,   345,
     344,    92,   350,   283,   349,   348,   326,   289,     0,     0,
     286,   535,   536,   537,   538,     0,     0,     0,   143,   142,
      95,     0,   373,     0,     0,   152,     0,   460,   463,   461,
     464,   462,   465,     0,     0,   424,   365,   407,     0,     0,
     433,   409,   393,    71,     0,     0,     0,     0,   468,     0,
       0,    97,     0,    97,     0,    97,   181,   377,     0,   375,
       0,   480,   185,   188,     0,   213,     0,   204,   207,     0,
     210,     0,   315,   316,     0,   545,     0,     0,     0,   562,
       0,     0,   293,   292,   331,    32,   291,   290,     0,   226,
     353,     0,   233,     0,     0,     0,     0,   317,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   278,     0,   478,     0,   470,
     534,   379,   532,   381,     0,   371,   384,    96,   354,    98,
     177,     0,    57,    58,     0,     0,   189,     0,     0,     0,
     190,     0,     0,     0,     0,     0,     0,     0,   156,   154,
       0,     0,     0,   170,   165,   163,   162,   164,   176,   157,
       0,     0,     0,   168,   169,   171,     0,   161,     0,   158,
     243,   410,     0,   421,   354,   401,     0,     0,     0,   394,
       0,   496,     0,   502,   492,   472,     0,   506,     0,   513,
       0,   520,     0,     0,   526,     0,   529,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,     0,   553,   543,   545,     0,   556,   557,   558,     0,
       0,     0,   545,     0,     0,     0,     0,    29,     0,    27,
       0,   319,   270,   269,     0,   327,     0,   219,     0,    94,
       0,   341,   342,   294,   295,   307,   308,   305,   306,     0,
     336,   324,     0,   351,   352,   296,   297,   312,   313,   314,
       0,     0,   310,   311,   309,   303,   304,   299,   298,   300,
     301,   302,     0,     0,   276,     0,     0,     0,     0,   329,
       0,   372,   383,   382,     0,   385,     0,   374,     0,     0,
       0,   191,   194,     0,   243,     0,   184,     0,   174,     0,
       0,    51,    61,     0,   198,   243,   225,   179,   180,     0,
     172,   173,     0,   160,   166,   167,   203,     0,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   411,
       0,   432,     0,   356,     0,   355,   434,     0,   391,   392,
     390,     0,   497,     0,   503,   493,   473,   507,   509,   514,
     516,   521,   523,   376,   527,   378,   530,   186,     0,     0,
       0,     0,   332,     0,     0,   333,     0,   233,     0,   546,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,   563,   564,     0,    30,    33,
       0,    94,     0,     0,     0,   328,     0,     0,     0,     0,
     231,     0,    97,   338,     0,   322,   343,     0,   325,     0,
     277,     0,   279,   320,   330,   386,   389,   388,    62,     0,
     195,   243,     0,     0,   192,     0,    63,    49,    50,   199,
     243,   196,   226,   175,    54,   178,   253,   254,   256,   255,
     257,   247,   248,   249,   258,   259,     0,     0,   245,   246,
     260,   261,   250,   251,   252,     0,     0,   244,   422,     0,
       0,   361,   357,   358,   361,   366,   390,   395,     0,   399,
     243,   499,   510,   517,   524,   187,     0,   216,   214,   205,
     208,   211,     0,     0,   540,   539,   545,     0,     0,   544,
       0,   548,   554,   560,   559,     0,   545,     0,   561,    28,
      31,     0,    97,     0,   220,   229,   230,   228,   227,     0,
       0,     0,     0,   267,     0,   337,   323,   321,   280,   382,
       0,     0,   193,   200,   197,     0,     0,    52,    53,     0,
      59,   264,   265,   262,   263,   412,    93,   362,   363,   356,
       0,   397,   243,   398,   500,     0,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   547,     0,     0,
     551,   271,     0,   266,   232,   234,     0,   221,   236,     0,
     268,   387,     0,   155,    55,     0,   370,   367,   359,   364,
     396,     0,     0,   206,   209,   212,   334,     0,   565,     0,
     549,   542,   541,   565,     0,     0,   274,     0,   222,   339,
      60,    54,     0,   360,     0,   217,     0,   335,     0,     0,
     550,     0,   552,   272,     0,   223,   340,    56,     0,     0,
       0,     0,     0,     0,   275,   368,   369,   218,   566,   567,
     568
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -559,  -559,  -559,  -559,  -559,   270,   629,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,   657,  -559,   587,  -559,  -559,   631,
    -559,  -559,  -559,  -263,  -559,  -559,  -559,  -559,  -559,  -559,
     432,  -559,  -559,   635,   -68,  -559,   618,    20,  -322,  -274,
    -386,  -559,  -559,  -559,  -489,  -559,  -167,  -559,   -86,  -475,
     -12,  -559,  -559,  -559,  -559,  -559,  -559,  -559,   710,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -446,  -559,  -559,  -157,  -559,   -14,  -467,  -559,  -362,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,   378,  -559,  -559,  -165,  -185,  -559,  -108,  -559,  -559,
    -559,   239,  -559,   173,  -559,  -340,   428,  -465,  -468,    27,
    -559,  -339,  -559,   676,  -559,  -559,  -559,   207,    78,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -358,
     -71,  -559,  -559,  -559,   439,  -559,  -559,  -559,    24,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,   427,  -558,  -402,
    -539,  -559,  -559,  -297,    -8,   312,  -559,  -559,  -559,  -219,
    -559
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   157,    45,    14,   176,   182,   421,   333,   749,
     334,   580,    15,    16,    38,    39,    72,    17,    34,    35,
     500,   501,   889,   890,   502,   503,   504,   505,   506,   507,
     183,   184,    30,    31,    32,    52,    53,    54,    18,   263,
     365,   167,    19,    86,   168,    87,   335,   508,   366,   509,
     336,   510,   337,   511,   512,   513,   514,   515,   516,   517,
     338,   552,   908,   553,   909,   555,   910,   339,   550,   907,
     340,   586,   928,   341,   760,   761,   589,   342,   343,   643,
     519,   344,   345,   346,   750,   955,   751,   974,   347,   627,
     878,   587,   872,   976,   361,   824,   964,   899,   694,   524,
     962,   362,   363,   397,   398,   211,   212,   731,   475,   701,
     530,   381,   271,    62,    96,    21,   173,   374,    42,    75,
      22,    90,    23,   269,   171,    60,    93,    24,   270,   382,
     349,   373,   148,   149,   209,   150,   284,   706,   399,   282,
     705,   279,   702,   280,   904,   281,   704,   285,   707,   286,
     832,   287,   709,   288,   833,   289,   711,   290,   834,   291,
     714,   292,   716,    25,    47,    26,    46,   350,   563,   564,
     565,   351,   566,   567,   568,   569,   352,   420,   353,   969,
     354
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   265,   147,   476,   518,   538,   631,   540,   273,   542,
     642,   636,   391,   161,   393,   572,   395,   473,   419,   432,
     654,     2,   630,   728,   -92,   735,   753,     3,   590,   657,
     658,   433,   434,   742,   558,   163,    68,   474,   558,   525,
      97,    98,   926,   430,   740,   632,   667,    40,   779,    64,
       4,   822,     5,    61,     6,   272,   557,    33,   570,   738,
       7,   146,    27,    28,   739,   433,   434,     8,    69,   294,
      55,   622,   623,     9,   272,   443,   213,   431,   445,   446,
     445,   446,    40,    48,   527,   633,    41,   634,   433,   434,
     635,   729,    10,   822,   937,   177,   178,   151,   264,   360,
     624,    63,   718,   469,   823,   756,   378,   164,   165,    49,
     668,   535,    43,    11,   757,   698,   476,   437,   438,    74,
     210,   266,   153,   377,   699,   443,   639,   444,   445,   446,
     447,   625,   626,   536,   448,   752,    64,   380,   465,   466,
     465,   466,   688,   470,    40,   758,   823,   166,    44,   267,
     759,   437,   438,   154,   272,   700,   591,    66,   527,   443,
     472,   776,   445,   446,   447,   780,    99,   277,   448,   734,
     100,   689,   101,   102,   437,   438,   849,   666,   789,   527,
      50,    70,   443,   527,   444,   445,   446,   447,   465,   466,
      29,   448,    71,   847,   278,   690,    51,   295,   522,    12,
     795,    73,   692,   691,   379,    13,   103,   473,   147,   147,
     147,   147,   522,    84,   147,   296,   147,   754,   147,    85,
     147,   179,   465,   466,   297,   180,   829,   181,   102,   147,
      36,   460,   461,   462,   463,   464,   754,    82,   523,    77,
     632,   632,   147,   147,   632,   465,   466,   912,    88,   730,
     730,    37,   818,   915,    89,    80,   375,   754,    56,    57,
      50,    58,    83,    80,   433,   434,   918,   146,   146,   146,
     146,   754,   197,   146,    78,   146,    51,   146,   745,   146,
     633,   633,   843,    79,   633,   147,   544,   932,   146,    59,
     543,   582,   754,   147,   855,   754,   198,   199,   755,   754,
      64,   146,   146,   384,   385,   386,   387,   913,   545,   390,
     880,   392,   546,   394,   425,   762,   733,   919,   874,   774,
     645,   869,   835,   905,   402,   526,   930,   652,    65,    82,
     846,    50,   477,   881,   547,   478,    80,   415,   417,   856,
     147,   863,   433,   434,   146,   906,   870,    51,   435,   436,
     437,   438,   146,   901,   896,   698,   200,    81,   443,   693,
     444,   445,   446,   447,   699,   106,  -508,   448,  -515,   449,
     450,  -508,   527,  -515,  -522,   528,   871,    71,   529,  -522,
     471,   197,   152,   916,   331,   656,    69,   264,   479,  -508,
     886,  -515,   549,  -273,   108,   826,   202,  -522,  -273,   146,
     203,   887,   888,   204,   158,   198,   199,   460,   461,   462,
     463,   464,   109,    91,    94,   110,  -273,   583,   205,    92,
      95,   465,   466,   197,   827,   155,   159,   206,   437,   438,
     367,   156,   939,   160,   368,   556,   443,   897,   444,   445,
     446,   447,   575,   898,   576,   448,   169,   198,   199,   174,
     175,   369,   370,   371,   372,   274,   275,   147,   274,   275,
     276,   147,   739,   170,   743,   200,   174,   175,   577,   578,
     172,   778,   147,   978,   147,   979,   922,   862,   190,   147,
     786,   147,   147,   207,   147,   186,   187,   462,   463,   464,
     188,   189,   191,   192,   208,   794,   193,   194,   195,   465,
     466,   196,   433,   434,   201,   202,   260,   200,   210,   203,
     261,   531,   204,   264,   262,   268,   146,   272,   293,   283,
     146,   358,   355,   518,   356,   357,   359,   205,   364,   376,
     401,   146,   380,   146,   388,   404,   206,   405,   146,   406,
     146,   146,   476,   146,   407,   409,   201,   202,   408,   410,
     411,   203,   697,   429,   204,    12,   703,   588,   592,   521,
     599,   468,   601,   147,   649,   659,   648,   713,   696,   205,
     708,   695,   469,   710,   721,   532,   723,   724,   206,   726,
     660,   661,   663,   348,   664,   665,   435,   436,   437,   438,
     439,   712,   733,   440,   441,   442,   443,   736,   444,   445,
     446,   447,   197,   737,   748,   448,   752,   449,   450,   746,
     764,   763,   197,   451,   452,   453,   766,   768,   883,   770,
     772,   667,   146,   785,   787,   788,   198,   199,   792,   793,
     821,   197,   825,   836,   848,   865,   198,   199,   454,   857,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     860,   866,   867,   850,   851,   198,   199,   853,   775,   465,
     466,   868,   858,   936,   911,   942,   954,   264,   963,   914,
     917,   920,   956,   925,   923,   949,   950,   970,    55,   147,
     966,   981,   972,   982,   985,   983,   200,   989,   990,   412,
     413,   579,   104,   418,    67,   185,   200,   107,   977,   105,
     162,   422,   423,   424,   927,   426,   427,   885,   428,   383,
     820,    20,   924,   842,   938,   200,   900,   637,   715,   934,
     400,    76,   197,   389,   828,   201,   202,   403,   687,   573,
     203,   854,   533,   204,   971,   201,   202,     0,   146,     0,
     203,     0,   534,   204,   520,     0,   198,   199,   205,     0,
       0,     0,     0,     0,   201,   202,     0,   206,   205,   203,
       0,   537,   204,     0,   197,   960,     0,   206,   961,     0,
       0,     0,     0,     0,   861,     0,     0,   205,   197,     0,
       0,     0,     0,     0,   551,     0,   206,   554,   198,   199,
       0,     0,     0,   562,     0,   562,     0,     0,     0,     0,
       0,   147,   198,   199,     0,     0,   200,     0,     0,     0,
       0,     0,   986,   593,   594,     0,     0,   595,   596,   597,
     598,     0,   600,     0,   602,   603,   604,   605,   606,   607,
     608,   609,   611,   612,   613,   614,   615,   616,   617,   618,
     619,   620,   621,     0,   628,   201,   202,     0,   200,     0,
     203,     0,   539,   204,     0,     0,     0,     0,     0,   638,
     146,     0,   200,   644,     0,     0,   646,   647,   205,     0,
       0,   651,     0,   655,     0,     0,     0,   206,     0,     0,
     662,   433,   434,     0,     0,     0,     0,   201,   202,     0,
       0,     0,   203,     0,   541,   204,   929,     0,     0,     0,
       0,   201,   202,     0,     0,     0,   203,     0,     0,   204,
     205,     0,     0,     0,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,   205,     0,   548,   720,     0,     0,
       0,     0,     0,   206,     0,     0,     0,     0,   422,   428,
       0,     0,     0,   973,     0,     0,     0,     0,     0,   428,
       0,     0,   744,   418,     0,     0,   747,   197,     0,     0,
       0,     0,   984,     0,     0,   435,   436,   437,   438,   439,
       0,     0,   440,   441,   442,   443,     0,   444,   445,   446,
     447,   198,   199,     0,   448,     0,   449,   450,     0,     0,
       0,     0,   451,   452,   453,     0,     0,     0,     0,     0,
       0,   769,     0,     0,   771,     0,     0,     0,   433,   434,
       0,     0,     0,     0,   777,     0,     0,   454,   781,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,   790,     0,     0,     0,     0,     0,     0,   465,   466,
       0,   200,     0,     0,     0,   895,   197,   796,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   808,   809,   810,
     811,   812,   813,   814,   817,     0,   819,     0,     0,     0,
     198,   199,     0,     0,     0,     0,     0,     0,     0,   830,
     201,   202,     0,     0,     0,   203,     0,   727,   204,     0,
       0,     0,   435,   436,   437,   438,   439,     0,     0,   440,
     441,   442,   443,   205,   444,   445,   446,   447,   844,   845,
       0,   448,   206,   449,   450,   852,     0,   562,     0,   451,
     452,   453,     0,     0,     0,     0,     0,     0,     0,     0,
     200,     0,   864,     0,   433,   434,     0,     0,     0,     0,
       0,     0,     0,     0,   454,   875,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
     808,   817,     0,     0,     0,   465,   466,     0,     0,   201,
     202,     0,   968,     0,   203,     0,   831,   204,     0,     0,
       0,     0,     0,     0,   891,   892,     0,     0,     0,     0,
       0,     0,   205,   893,   894,     0,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,   902,     0,     0,     0,
     669,   670,   671,   672,   673,   674,   675,   676,   435,   436,
     437,   438,   439,   677,   678,   440,   441,   442,   443,   782,
     444,   445,   446,   447,     0,     0,     0,   448,   680,   449,
     450,   681,   682,     0,     0,   451,   452,   453,   683,   684,
     685,     0,     0,     0,     0,     0,     0,     0,   931,     0,
       0,     0,     0,   520,     0,     0,     0,   935,     0,   783,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,   941,     0,     0,   943,   944,   945,   947,
     948,   465,   466,   951,   952,     0,   953,     0,   784,     0,
     480,     0,     0,     0,   958,     3,     0,   481,   482,   483,
       0,   484,     0,   298,   299,   300,   301,   302,     0,     0,
       0,     0,     0,     0,   485,   303,   486,   487,     0,     0,
       0,     0,     0,     0,     0,   975,   488,   304,     0,   489,
     305,     0,     0,   306,   980,     8,   307,   490,     0,   491,
     308,     0,     0,   492,   493,     0,     0,     0,     0,   988,
     494,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     309,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   310,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    51,   322,     0,     0,     0,
       0,     0,     0,   197,     0,     0,     0,     0,     0,     0,
       0,     0,   323,   324,   325,     0,   326,     0,     0,   327,
     328,     0,     0,     0,     0,     0,     0,   198,   199,   330,
       0,   331,   332,   498,   264,   499,   480,     0,     0,     0,
       0,     3,     0,   481,   482,   483,     0,   484,     0,   298,
     299,   300,   301,   302,     0,     0,     0,     0,     0,     0,
     485,   303,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,   488,   304,     0,   489,   305,     0,     0,   306,
       0,     8,   307,   490,     0,   491,   308,   200,     0,   492,
     493,     0,     0,     0,     0,     0,   494,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   309,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   201,   202,   310,   495,
       0,   203,     0,   838,   204,     0,     0,     0,     0,     0,
       0,   311,   312,     0,     0,     0,     0,     0,     0,   205,
       0,     0,     0,     0,     0,     0,   496,   497,   206,     0,
       0,     0,     0,     0,   197,    50,     0,     0,     0,     0,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    51,   322,     0,     0,     0,     0,     0,   198,   199,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   324,
     325,     0,   326,     0,     0,   327,   328,     0,     0,     0,
     298,   299,   300,   301,   302,   330,     0,   331,   332,   498,
     264,   933,   303,     0,     0,     0,     0,     0,   197,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,   558,     0,   308,   200,     0,
       0,     0,   198,   199,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   201,   202,   310,
       0,     0,   203,     0,   839,   204,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
     205,     0,   200,     0,     0,     0,     0,     0,     0,   206,
       0,     0,   197,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   559,   322,     0,     0,   198,   199,     0,     0,
       0,   201,   202,     0,     0,     0,   203,     0,     0,   560,
     324,   325,     0,   326,     0,     0,   327,   328,   298,   299,
     300,   301,   302,     0,   205,     0,   561,     0,   331,   332,
     303,   264,     0,   206,     0,     0,   197,     0,     0,     0,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,     0,   308,   200,     0,     0,     0,
     198,   199,     0,     0,     0,     0,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   309,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,   201,   202,   310,     0,     0,
     203,     0,   840,   204,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,   205,     0,
     200,     0,     0,     0,     0,     0,     0,   206,     0,     0,
     197,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     559,   322,     0,     0,   198,   199,     0,     0,     0,   201,
     202,     0,     0,     0,   203,     0,     0,   560,   324,   325,
       0,   326,     0,     0,   327,   328,   298,   299,   300,   301,
     302,     0,   205,     0,   571,     0,   331,   332,   303,   264,
       0,   206,     0,     0,   294,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,   200,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,   201,   202,   310,     0,     0,   203,     0,
     841,   204,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,   205,     0,     0,     0,
       0,     0,     0,     0,     0,   206,     0,     0,     0,     0,
       0,   197,    50,   197,     0,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    51,   322,
       0,     0,     0,     0,     0,   198,   199,   198,   199,     0,
       0,     0,   295,     0,     0,   323,   324,   325,     0,   326,
       0,     0,   327,   328,   298,   299,   300,   301,   302,     0,
     296,     0,   330,     0,   331,   332,   303,   264,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,     0,     0,   632,   200,     0,   200,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   310,   879,   202,   201,   202,     0,   203,
       0,   203,   204,   921,   204,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,   640,     0,   205,     0,   205,
       0,   496,   497,     0,     0,     0,   206,     0,   206,     0,
      50,   197,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,     0,     0,     0,     0,   198,   199,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,     0,     0,   298,   299,   300,   301,   302,     0,
     330,     0,   331,   332,   641,   264,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,     0,     0,     0,   200,     0,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   310,   201,   202,     0,     0,     0,   203,
       0,   959,   204,     0,     0,     0,   311,   312,   197,     0,
       0,     0,     0,     0,     0,   653,     0,   205,     0,     0,
       0,   496,   497,     0,     0,     0,   206,     0,     0,     0,
      50,     0,   198,   199,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,   323,   324,   325,   303,   326,     0,     0,
     327,   328,     0,     0,     0,     0,     0,     0,   304,     0,
     330,   305,   331,   332,   306,   264,     0,   307,     0,     0,
       0,   308,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   201,   202,   310,     0,     0,   203,     0,     0,   204,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,   205,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   298,   299,   300,   301,   302,     0,   584,     0,
     330,   585,   331,   332,   303,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   264,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   264,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   815,
     816,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   264,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,   610,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,   329,   330,   305,
     331,   332,   306,   264,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   264,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   324,   325,     0,   326,     0,     0,   327,   328,
     298,   299,   300,   301,   302,     0,     0,     0,   330,   629,
     331,   332,   303,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,     0,     0,
     650,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,   323,
     324,   325,   303,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,   304,     0,   330,   305,   331,   332,
     306,   264,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,   298,   299,
     300,   301,   302,     0,     0,     0,   330,   717,   331,   332,
     303,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   309,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      51,   322,     0,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,     0,     0,   719,     0,   323,   324,   325,
     303,   326,     0,     0,   327,   328,     0,     0,     0,     0,
       0,     0,   304,     0,   330,   305,   331,   332,   306,   264,
       0,   307,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   309,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      51,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,   324,   325,
       0,   326,     0,     0,   327,   328,   298,   299,   300,   301,
     302,     0,     0,     0,   330,   732,   331,   332,   303,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    51,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,   324,   325,     0,   326,
       0,     0,   327,   328,   298,   299,   300,   301,   302,     0,
       0,     0,   330,   741,   331,   332,   303,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   298,   299,   300,   301,   302,     0,     0,     0,
     330,   873,   331,   332,   303,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   324,   325,     0,   326,     0,     0,   327,   328,
     298,   299,   300,   301,   302,     0,     0,     0,   330,   946,
     331,   332,   303,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,   957,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,   323,
     324,   325,   303,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,   304,     0,   330,   305,   331,   332,
     306,   264,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,   433,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,   330,     0,   331,   332,
       0,   264,   669,   670,   671,   672,   673,   674,   675,   676,
     435,   436,   437,   438,   439,   677,   678,   440,   441,   442,
     443,   782,   444,   445,   446,   447,   433,   434,     0,   448,
     680,   449,   450,   681,   682,     0,     0,   451,   452,   453,
     683,   684,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
     791,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   674,   675,   676,
     435,   436,   437,   438,   439,   677,   678,   440,   441,   442,
     443,   782,   444,   445,   446,   447,   433,   434,     0,   448,
     680,   449,   450,   681,   682,     0,     0,   451,   452,   453,
     683,   684,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
     882,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   674,   675,   676,
     435,   436,   437,   438,   439,   677,   678,   440,   441,   442,
     443,   782,   444,   445,   446,   447,   433,   434,     0,   448,
     680,   449,   450,   681,   682,     0,     0,   451,   452,   453,
     683,   684,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
     884,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   674,   675,   676,
     435,   436,   437,   438,   439,   677,   678,   440,   441,   442,
     443,   782,   444,   445,   446,   447,   433,   434,     0,   448,
     680,   449,   450,   681,   682,     0,     0,   451,   452,   453,
     683,   684,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
     903,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   674,   675,   676,
     435,   436,   437,   438,   439,   677,   678,   440,   441,   442,
     443,   782,   444,   445,   446,   447,   433,   434,     0,   448,
     680,   449,   450,   681,   682,     0,     0,   451,   452,   453,
     683,   684,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   783,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
     940,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,     0,     0,     0,
     435,   436,   437,   438,   439,     0,     0,   440,   441,   442,
     443,     0,   444,   445,   446,   447,     0,     0,     0,   448,
       0,   449,   450,     0,     0,     0,     0,   451,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,   433,   434,   581,   435,   436,
     437,   438,   439,     0,     0,   440,   441,   442,   443,     0,
     444,   445,   446,   447,     0,     0,     0,   448,     0,   449,
     450,     0,     0,     0,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   433,   434,   722,     0,     0,     0,   435,
     436,   437,   438,   439,     0,     0,   440,   441,   442,   443,
       0,   444,   445,   446,   447,     0,     0,     0,   448,     0,
     449,   450,     0,     0,     0,     0,   451,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,   466,   433,   434,   725,   435,   436,   437,
     438,   439,     0,     0,   440,   441,   442,   443,     0,   444,
     445,   446,   447,     0,     0,     0,   448,     0,   449,   450,
       0,     0,     0,     0,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   466,   433,   434,   837,     0,     0,     0,   435,   436,
     437,   438,   439,     0,     0,   440,   441,   442,   443,     0,
     444,   445,   446,   447,     0,     0,     0,   448,     0,   449,
     450,     0,     0,     0,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   433,   434,   965,   435,   436,   437,   438,
     439,     0,     0,   440,   441,   442,   443,     0,   444,   445,
     446,   447,     0,     0,     0,   448,     0,   449,   450,     0,
       0,     0,     0,   451,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   433,   434,   967,     0,     0,     0,   435,   436,   437,
     438,   439,     0,     0,   440,   441,   442,   443,     0,   444,
     445,   446,   447,     0,     0,     0,   448,     0,   449,   450,
       0,     0,     0,     0,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,   433,   434,     0,
     465,   466,     0,     0,   987,   435,   436,   437,   438,   439,
       0,     0,   440,   441,   442,   443,     0,   444,   445,   446,
     447,     0,     0,     0,   448,     0,   449,   450,     0,     0,
       0,     0,   451,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,     0,     0,   433,   434,     0,     0,     0,   465,   466,
     467,   435,   436,   437,   438,   439,     0,     0,   440,   441,
     442,   443,     0,   444,   445,   446,   447,     0,     0,     0,
     448,     0,   449,   450,     0,     0,     0,     0,   451,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,   433,
     434,     0,     0,     0,   465,   466,   765,   435,   436,   437,
     438,   439,     0,     0,   440,   441,   442,   443,     0,   444,
     445,   446,   447,     0,     0,     0,   448,     0,   449,   450,
       0,     0,     0,     0,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,   433,   434,     0,     0,     0,
     465,   466,   773,   435,   436,   437,   438,   439,     0,     0,
     440,   441,   442,   443,     0,   444,   445,   446,   447,     0,
       0,     0,   448,     0,   449,   450,     0,     0,     0,     0,
     451,   452,   453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,   876,   435,
     436,   437,   438,   439,   433,   434,   440,   441,   442,   443,
       0,   444,   445,   446,   447,     0,     0,     0,   448,     0,
     449,   450,     0,     0,     0,     0,   451,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,   466,   877,     0,     0,     0,     0,     0,
     669,   670,   671,   672,   673,   674,   675,   676,   435,   436,
     437,   438,   439,   677,   678,   440,   441,   442,   443,   782,
     444,   445,   446,   447,  -243,   433,   434,   448,   680,   449,
     450,   681,   682,     0,     0,   451,   452,   453,   683,   684,
     685,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   783,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   434,     0,     0,     0,     0,     0,
       0,   669,   670,   671,   672,   673,   674,   675,   676,   435,
     436,   437,   438,   439,   677,   678,   440,   441,   442,   443,
     679,   444,   445,   446,   447,     0,     0,     0,   448,   680,
     449,   450,   681,   682,     0,     0,   451,   452,   453,   683,
     684,   685,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     686,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   433,   434,     0,     0,   435,   436,   437,
     438,   439,   465,   466,   440,   441,   442,   443,     0,   444,
     445,   446,   447,     0,     0,     0,   448,     0,   449,   450,
       0,     0,   574,     0,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   433,   434,     0,     0,     0,     0,     0,     0,     0,
     465,   466,     0,     0,     0,     0,     0,   435,   436,   437,
     438,   439,     0,     0,   440,   441,   442,   443,     0,   444,
     445,   446,   447,     0,     0,     0,   448,     0,   449,   450,
       0,     0,     0,     0,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
     767,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   433,   434,     0,     0,   435,   436,   437,   438,   439,
     465,   466,   440,   441,   442,   443,     0,   444,   445,   446,
     447,     0,     0,     0,   448,     0,   449,   450,     0,     0,
       0,     0,   451,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   859,     0,     0,     0,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   433,
     434,     0,     0,     0,     0,     0,     0,     0,   465,   466,
       0,     0,     0,     0,     0,   435,   436,   437,   438,   439,
       0,     0,   440,   441,   442,   443,     0,   444,   445,   446,
     447,     0,     0,     0,   448,     0,   449,   450,     0,     0,
       0,     0,   451,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   433,
     434,     0,     0,   435,   436,   437,   438,   439,   465,   466,
     440,   441,   442,   443,     0,   444,   445,   446,   447,     0,
       0,     0,   448,     0,   449,   450,     0,   433,   434,     0,
     451,     0,   453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,     0,     0,
       0,     0,     0,   435,   436,   437,   438,   439,     0,     0,
     440,   441,   442,   443,     0,   444,   445,   446,   447,     0,
       0,     0,   448,     0,   449,   450,   433,   434,     0,     0,
     451,   435,   436,   437,   438,   439,     0,     0,   440,   441,
     442,   443,     0,   444,   445,   446,   447,     0,     0,     0,
     448,     0,   449,   450,   433,   434,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,     0,     0,     0,     0,
     435,   436,   437,   438,   439,     0,     0,   440,   441,   442,
     443,     0,   444,   445,   446,   447,     0,     0,     0,   448,
       0,   449,   450,   433,   434,     0,     0,     0,   435,   436,
     437,   438,   439,     0,     0,   440,   441,   442,   443,     0,
     444,   445,   446,   447,     0,     0,     0,   448,     0,   449,
     450,     0,     0,     0,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   465,   466,     0,     0,     0,     0,   435,   436,     0,
       0,   439,     0,     0,   440,   441,   442,   443,     0,   444,
     445,   446,   447,   433,   434,     0,   448,     0,   449,   450,
       0,     0,     0,     0,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,   455,   456,   457,   458,   459,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   466,   435,   436,   437,   438,   439,     0,     0,   440,
     441,   442,   443,     0,   444,   445,   446,   447,     0,     0,
       0,   448,     0,   449,   450,     0,     0,   435,   436,   437,
     438,   439,     0,     0,   440,     0,     0,   443,     0,   444,
     445,   446,   447,   214,     0,     0,   448,     0,   449,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   465,   466,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   466,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
     235,   236,   237,   238,   239,     0,     0,   240,   241,   242,
     243,   244,   245,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,   112,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,     0,   256,   257,   113,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,     0,     0,
       0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
     112,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
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
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,   113,
       0,     0,     0,     0,   313,   314,   315,     0,     0,     0,
       0,     0,     0,    51,   114,     0,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,   113,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,   114,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   396
};

static const yytype_int16 yycheck[] =
{
      12,   168,    73,   361,   366,   391,   474,   393,   173,   395,
     485,   476,   286,    81,   288,   417,   290,   357,   315,   341,
     495,     0,   468,    19,     7,   564,   584,     6,    18,   496,
     497,    20,    21,   572,    49,    34,    21,    46,    49,   378,
      14,    15,   129,   146,    19,   115,   120,   146,    19,   154,
      29,    42,    31,    58,    33,   137,   414,   140,   416,   179,
      39,    73,    18,    19,   184,    20,    21,    46,    53,    32,
      82,   117,   118,    52,   137,   114,   181,   180,   117,   118,
     117,   118,   146,    33,   154,   155,   185,   157,    20,    21,
     160,   558,    71,    42,    43,    14,    15,    73,   185,   181,
     146,   155,   548,   146,    95,   115,   271,   106,   107,    59,
     184,   162,   146,    92,   124,   115,   474,   106,   107,    41,
     146,   157,   186,   186,   124,   114,   484,   116,   117,   118,
     119,   177,   178,   184,   123,   146,   154,   146,   177,   178,
     177,   178,   155,   186,   146,   155,    95,   146,   182,   185,
     160,   106,   107,    75,   137,   155,   146,   154,   154,   114,
     186,   636,   117,   118,   119,   640,   140,   154,   123,   184,
     144,   184,   146,   147,   106,   107,   734,   516,   653,   154,
     130,   166,   114,   154,   116,   117,   118,   119,   177,   178,
     146,   123,   177,   732,   181,   178,   146,   160,   146,   178,
     667,   155,   524,   186,   272,   184,   180,   547,   279,   280,
     281,   282,   146,    53,   285,   178,   287,   154,   289,    59,
     291,   140,   177,   178,   187,   144,   701,   146,   147,   300,
     146,   163,   164,   165,   166,   167,   154,   154,   186,   146,
     115,   115,   313,   314,   115,   177,   178,   184,    53,   124,
     124,   167,   186,   124,    59,   130,   268,   154,     4,     5,
     130,     7,   179,   130,    20,    21,   184,   279,   280,   281,
     282,   154,    32,   285,   146,   287,   146,   289,   575,   291,
     155,   155,   728,   146,   155,   356,   162,   184,   300,    35,
     157,   161,   154,   364,   740,   154,    56,    57,   181,   154,
     154,   313,   314,   279,   280,   281,   282,   846,   184,   285,
     775,   287,   162,   289,   326,   589,   154,   856,   764,   181,
     487,   154,   181,   162,   300,   179,   181,   494,   157,   154,
     732,   130,   181,   779,   184,   184,   130,   313,   314,   741,
     411,   179,    20,    21,   356,   184,   179,   146,   104,   105,
     106,   107,   364,   828,   179,   115,   116,   180,   114,   524,
     116,   117,   118,   119,   124,   140,   157,   123,   157,   125,
     126,   162,   154,   162,   157,   157,   762,   177,   160,   162,
     356,    32,   184,   848,   182,   183,    53,   185,   364,   180,
      11,   180,   404,   157,   146,   155,   156,   180,   162,   411,
     160,    22,    23,   163,   185,    56,    57,   163,   164,   165,
     166,   167,   146,    53,    53,   146,   180,   429,   178,    59,
      59,   177,   178,    32,   184,    53,   185,   187,   106,   107,
      66,    59,   900,   146,    70,   411,   114,    53,   116,   117,
     118,   119,   184,    59,   186,   123,   146,    56,    57,   148,
     149,    87,    88,    89,    90,   148,   149,   528,   148,   149,
     150,   532,   184,   146,   186,   116,   148,   149,   150,   151,
     185,   638,   543,   962,   545,   964,   862,   751,   180,   550,
     647,   552,   553,   184,   555,   161,   161,   165,   166,   167,
     161,   161,   161,   161,   146,   662,   161,   161,   161,   177,
     178,   161,    20,    21,   155,   156,    34,   116,   146,   160,
      34,   162,   163,   185,   180,   157,   528,   137,    32,   146,
     532,   179,   184,   885,   157,   184,   179,   178,   157,   185,
     179,   543,   146,   545,   181,   180,   187,   161,   550,   180,
     552,   553,   900,   555,   161,   180,   155,   156,   161,   161,
     161,   160,   528,   183,   163,   178,   532,   137,   146,   185,
     146,   180,   146,   634,   140,    36,   184,   543,   146,   178,
     162,   179,   146,   162,   550,   184,   552,   553,   187,   555,
     184,   184,   184,   205,   184,   184,   104,   105,   106,   107,
     108,   162,   154,   111,   112,   113,   114,   179,   116,   117,
     118,   119,    32,   154,   148,   123,   146,   125,   126,   186,
     180,   161,    32,   131,   132,   133,   146,   146,   785,   146,
       1,   120,   634,    13,   157,   140,    56,    57,   183,   185,
       7,    32,   184,   146,   146,   146,    56,    57,   156,   186,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     148,   146,   146,   179,   179,    56,    57,   179,   634,   177,
     178,   146,   179,   184,   180,   146,   186,   185,    32,   179,
     179,   179,   162,   179,   181,   179,   179,   179,   690,   750,
     162,    49,   179,   179,   184,   186,   116,   179,   179,   311,
     312,   421,    63,   315,    37,   108,   116,    66,   961,    64,
      82,   323,   324,   325,   871,   327,   328,   793,   330,   277,
     690,     1,   869,   727,   899,   116,   824,   478,   545,   886,
     292,    45,    32,   284,   697,   155,   156,   300,   521,   417,
     160,   739,   162,   163,   953,   155,   156,    -1,   750,    -1,
     160,    -1,   162,   163,   366,    -1,    56,    57,   178,    -1,
      -1,    -1,    -1,    -1,   155,   156,    -1,   187,   178,   160,
      -1,   162,   163,    -1,    32,   932,    -1,   187,   935,    -1,
      -1,    -1,    -1,    -1,   750,    -1,    -1,   178,    32,    -1,
      -1,    -1,    -1,    -1,   406,    -1,   187,   409,    56,    57,
      -1,    -1,    -1,   415,    -1,   417,    -1,    -1,    -1,    -1,
      -1,   872,    56,    57,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,   979,   435,   436,    -1,    -1,   439,   440,   441,
     442,    -1,   444,    -1,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,    -1,   466,   155,   156,    -1,   116,    -1,
     160,    -1,   162,   163,    -1,    -1,    -1,    -1,    -1,   481,
     872,    -1,   116,   485,    -1,    -1,   488,   489,   178,    -1,
      -1,   493,    -1,   495,    -1,    -1,    -1,   187,    -1,    -1,
     502,    20,    21,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,    -1,   160,    -1,   162,   163,   872,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,   160,    -1,    -1,   163,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,   178,    -1,   180,   549,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,   560,   561,
      -1,    -1,    -1,   955,    -1,    -1,    -1,    -1,    -1,   571,
      -1,    -1,   574,   575,    -1,    -1,   578,    32,    -1,    -1,
      -1,    -1,   974,    -1,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    56,    57,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,   623,    -1,    -1,   626,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   636,    -1,    -1,   156,   640,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,   653,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      -1,   116,    -1,    -1,    -1,   184,    32,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   679,   680,   681,
     682,   683,   684,   685,   686,    -1,   688,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   701,
     155,   156,    -1,    -1,    -1,   160,    -1,   162,   163,    -1,
      -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,   178,   116,   117,   118,   119,   730,   731,
      -1,   123,   187,   125,   126,   737,    -1,   739,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   754,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   767,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
     782,   783,    -1,    -1,    -1,   177,   178,    -1,    -1,   155,
     156,    -1,   184,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   806,   807,    -1,    -1,    -1,    -1,
      -1,    -1,   178,   815,   816,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,   828,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   880,    -1,
      -1,    -1,    -1,   885,    -1,    -1,    -1,   889,    -1,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,   905,    -1,    -1,   908,   909,   910,   911,
     912,   177,   178,   915,   916,    -1,   918,    -1,   184,    -1,
       1,    -1,    -1,    -1,   926,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   957,    37,    38,    -1,    40,
      41,    -1,    -1,    44,   966,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,   981,
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
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    62,    63,
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
      56,    57,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
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
      -1,   187,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   155,   156,    93,    -1,    -1,   160,    -1,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    32,   130,    32,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,    -1,    56,    57,    56,    57,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,   167,
      -1,    -1,   170,   171,    14,    15,    16,    17,    18,    -1,
     178,    -1,   180,    -1,   182,   183,    26,   185,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,   115,   116,    -1,   116,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,   155,   156,   155,   156,    -1,   160,
      -1,   160,   163,   162,   163,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,   178,    -1,   178,
      -1,   121,   122,    -1,    -1,    -1,   187,    -1,   187,    -1,
     130,    32,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,   165,    -1,   167,    -1,    -1,
     170,   171,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     180,    -1,   182,   183,   184,   185,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,   155,   156,    -1,    -1,    -1,   160,
      -1,   162,   163,    -1,    -1,    -1,   106,   107,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,   178,    -1,    -1,
      -1,   121,   122,    -1,    -1,    -1,   187,    -1,    -1,    -1,
     130,    -1,    56,    57,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   163,   164,   165,    26,   167,    -1,    -1,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     180,    41,   182,   183,    44,   185,    -1,    47,    -1,    -1,
      -1,    51,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   155,   156,    93,    -1,    -1,   160,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,   165,    -1,   167,    -1,    -1,
     170,   171,    14,    15,    16,    17,    18,    -1,   178,    -1,
     180,   181,   182,   183,    26,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,   179,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   163,
     164,   165,    26,   167,    -1,    -1,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   180,    41,   182,   183,
      44,   185,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
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
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
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
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
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
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
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
     167,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     177,   178,    -1,    -1,   181,   104,   105,   106,   107,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   177,   178,
     179,   104,   105,   106,   107,   108,    -1,    -1,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,
     123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   177,   178,   179,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     177,   178,   179,   104,   105,   106,   107,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   104,
     105,   106,   107,   108,    20,    21,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,
     125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    20,    21,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,    -1,    -1,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    20,    21,    -1,    -1,   104,   105,   106,
     107,   108,   177,   178,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    20,    21,    -1,    -1,   104,   105,   106,   107,   108,
     177,   178,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    20,
      21,    -1,    -1,   104,   105,   106,   107,   108,   177,   178,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,   126,    -1,    20,    21,    -1,
     131,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,   126,    20,    21,    -1,    -1,
     131,   104,   105,   106,   107,   108,    -1,    -1,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,
     123,    -1,   125,   126,    20,    21,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    20,    21,    -1,    -1,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
     126,    -1,    -1,    -1,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   177,   178,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    20,    21,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,   158,   159,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,   104,   105,   106,
     107,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,   116,
     117,   118,   119,    37,    -1,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,   170,   171,    45,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    45,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,   146,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   189,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   178,   184,   192,   200,   201,   205,   226,   230,
     246,   303,   308,   310,   315,   351,   353,    18,    19,   146,
     220,   221,   222,   140,   206,   207,   146,   167,   202,   203,
     146,   185,   306,   146,   182,   191,   354,   352,    33,    59,
     130,   146,   223,   224,   225,   238,     4,     5,     7,    35,
     313,    58,   301,   155,   154,   157,   154,   202,    21,    53,
     166,   177,   204,   155,   306,   307,   301,   146,   146,   146,
     130,   180,   154,   179,    53,    59,   231,   233,    53,    59,
     309,    53,    59,   314,    53,    59,   302,    14,    15,   140,
     144,   146,   147,   180,   194,   221,   140,   207,   146,   146,
     146,    24,    30,    45,    60,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   238,   318,   320,   321,
     323,   326,   184,   186,   306,    53,    59,   190,   185,   185,
     146,   222,   224,    34,   106,   107,   146,   229,   232,   146,
     146,   312,   185,   304,   148,   149,   193,    14,    15,   140,
     144,   146,   194,   218,   219,   204,   161,   161,   161,   161,
     180,   161,   161,   161,   161,   161,   161,    32,    56,    57,
     116,   155,   156,   160,   163,   178,   187,   184,   146,   322,
     146,   293,   294,   181,    37,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   117,   118,   124,   125,   126,   127,   128,
     131,   132,   133,   134,   135,   136,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   170,   171,   177,   178,
      34,    34,   180,   227,   185,   234,   157,   185,   157,   311,
     316,   300,   137,   282,   148,   149,   150,   154,   181,   329,
     331,   333,   327,   146,   324,   335,   337,   339,   341,   343,
     345,   347,   349,    32,    32,   160,   178,   187,    14,    15,
      16,    17,    18,    26,    38,    41,    44,    47,    51,    71,
      93,   106,   107,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   147,   163,   164,   165,   167,   170,   171,   179,
     180,   182,   183,   196,   198,   234,   238,   240,   248,   255,
     258,   261,   265,   266,   269,   270,   271,   276,   279,   318,
     355,   359,   364,   366,   368,   184,   157,   184,   179,   179,
     181,   282,   289,   290,   157,   228,   236,    66,    70,    87,
      88,    89,    90,   319,   305,   238,   185,   186,   282,   222,
     146,   299,   317,   218,   326,   326,   326,   326,   181,   322,
     326,   227,   326,   227,   326,   227,   146,   291,   292,   326,
     294,   179,   326,   355,   180,   161,   180,   161,   161,   180,
     161,   161,   279,   279,    12,   326,    12,   326,   279,   361,
     365,   195,   279,   279,   279,   238,   279,   279,   279,   183,
     146,   180,   226,    20,    21,   104,   105,   106,   107,   108,
     111,   112,   113,   114,   116,   117,   118,   119,   123,   125,
     126,   131,   132,   133,   156,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   177,   178,   179,   180,   146,
     186,   326,   186,   293,    46,   296,   317,   181,   184,   326,
       1,     8,     9,    10,    12,    25,    27,    28,    37,    40,
      48,    50,    54,    55,    61,    94,   121,   122,   184,   186,
     208,   209,   212,   213,   214,   215,   216,   217,   235,   237,
     239,   241,   242,   243,   244,   245,   246,   247,   267,   268,
     279,   185,   146,   186,   287,   299,   179,   154,   157,   160,
     298,   162,   184,   162,   162,   162,   184,   162,   228,   162,
     228,   162,   228,   157,   162,   184,   162,   184,   180,   238,
     256,   279,   249,   251,   279,   253,   326,   317,    49,   146,
     163,   180,   279,   356,   357,   358,   360,   361,   362,   363,
     317,   180,   357,   363,   129,   184,   186,   150,   151,   193,
     199,   181,   161,   238,   178,   181,   259,   279,   137,   264,
      18,   146,   146,   279,   279,   279,   279,   279,   279,   146,
     279,   146,   279,   279,   279,   279,   279,   279,   279,   279,
      21,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   117,   118,   146,   177,   178,   277,   279,   181,
     259,   296,   115,   155,   157,   160,   295,   289,   279,   317,
     115,   184,   237,   267,   279,   234,   279,   279,   184,   140,
      54,   279,   234,   115,   237,   279,   183,   265,   265,    36,
     184,   184,   279,   184,   184,   184,   299,   120,   184,    96,
      97,    98,    99,   100,   101,   102,   103,   109,   110,   115,
     124,   127,   128,   134,   135,   136,   155,   305,   155,   184,
     178,   186,   226,   282,   286,   179,   146,   326,   115,   124,
     155,   297,   330,   326,   334,   328,   325,   336,   162,   340,
     162,   344,   162,   326,   348,   291,   350,   181,   259,   161,
     279,   326,   181,   326,   326,   181,   326,   162,    19,   265,
     124,   295,   181,   154,   184,   358,   179,   154,   179,   184,
      19,   181,   358,   186,   279,   361,   186,   279,   148,   197,
     272,   274,   146,   356,   154,   181,   115,   124,   155,   160,
     262,   263,   227,   161,   180,   179,   146,   157,   146,   279,
     146,   279,     1,   179,   181,   326,   237,   279,   234,    19,
     237,   279,   115,   155,   184,    13,   234,   157,   140,   237,
     279,   184,   183,   185,   234,   265,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   121,   122,   279,   279,
     279,   279,   279,   279,   279,   121,   122,   279,   186,   279,
     225,     7,    42,    95,   283,   184,   155,   184,   297,   237,
     279,   162,   338,   342,   346,   181,   146,   181,   162,   162,
     162,   162,   264,   259,   279,   279,   357,   358,   146,   356,
     179,   179,   279,   179,   362,   259,   357,   186,   179,   152,
     148,   326,   227,   179,   279,   146,   146,   146,   146,   154,
     179,   228,   280,   181,   259,   279,   179,   179,   278,   155,
     295,   259,   184,   234,   184,   236,    11,    22,    23,   210,
     211,   279,   279,   279,   279,   184,   179,    53,    59,   285,
     285,   237,   279,   184,   332,   162,   184,   257,   250,   252,
     254,   180,   184,   358,   179,   124,   295,   179,   184,   358,
     179,   162,   228,   181,   262,   179,   129,   234,   260,   326,
     181,   279,   184,   186,   234,   279,   184,    43,   283,   296,
     184,   279,   146,   279,   279,   279,   181,   279,   279,   179,
     179,   279,   279,   279,   186,   273,   162,   115,   279,   162,
     234,   234,   288,    32,   284,   181,   162,   181,   184,   367,
     179,   367,   179,   238,   275,   279,   281,   211,   232,   232,
     279,    49,   179,   186,   238,   184,   234,   181,   279,   179,
     179
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
     279,   279,   279,   279,   282,   282,   283,   283,   283,   284,
     284,   285,   285,   285,   286,   287,   287,   288,   287,   287,
     287,   289,   289,   290,   290,   291,   291,   292,   292,   293,
     294,   294,   295,   295,   296,   296,   296,   296,   296,   296,
     297,   297,   297,   298,   298,   299,   299,   299,   299,   299,
     300,   300,   301,   301,   302,   302,   302,   303,   304,   303,
     305,   305,   305,   306,   307,   307,   308,   308,   309,   309,
     309,   310,   310,   311,   311,   312,   313,   313,   314,   314,
     314,   316,   315,   317,   317,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     319,   319,   319,   319,   319,   319,   320,   321,   321,   322,
     322,   324,   325,   323,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   327,   328,   326,   326,   329,   330,   326,   331,   332,
     326,   333,   334,   326,   326,   335,   336,   326,   337,   338,
     326,   326,   339,   340,   326,   341,   342,   326,   326,   343,
     344,   326,   345,   346,   326,   347,   348,   326,   349,   350,
     326,   352,   351,   354,   353,   355,   355,   355,   355,   356,
     356,   356,   356,   357,   357,   358,   358,   359,   359,   359,
     359,   359,   359,   360,   360,   361,   362,   362,   363,   363,
     364,   364,   365,   365,   366,   367,   367,   368,   368
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
       1,     3,     3,     2,     0,     4,     0,     1,     1,     0,
       1,     0,     1,     1,     4,     0,     3,     0,     8,     8,
       5,     2,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     4,     6,     5,     5,     4,
       0,     3,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     3,     5,     3,     2,     3,     4,     2,     0,     1,
       1,     7,     9,     0,     2,     2,     1,     1,     0,     1,
       1,     0,     8,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       3,     0,     0,     6,     1,     1,     1,     1,     4,     3,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       2,     0,     0,     6,     2,     0,     0,     6,     0,     0,
       8,     0,     0,     6,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     0,     0,     6,     0,     0,
       6,     0,     7,     0,     7,     1,     1,     1,     1,     3,
       3,     5,     5,     1,     3,     0,     2,     6,     5,     7,
       8,     6,     8,     1,     3,     3,     1,     1,     1,     3,
       5,     5,     1,     3,     4,     0,     3,    10,    10
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
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3496 "ds_parser.cpp"
        break;

    case 191: /* module_name  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3502 "ds_parser.cpp"
        break;

    case 193: /* character_sequence  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3508 "ds_parser.cpp"
        break;

    case 194: /* string_constant  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3514 "ds_parser.cpp"
        break;

    case 195: /* string_builder_body  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3520 "ds_parser.cpp"
        break;

    case 196: /* string_builder  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3526 "ds_parser.cpp"
        break;

    case 198: /* expr_reader  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3532 "ds_parser.cpp"
        break;

    case 202: /* require_module_name  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3538 "ds_parser.cpp"
        break;

    case 208: /* expression_label  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3544 "ds_parser.cpp"
        break;

    case 209: /* expression_goto  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3550 "ds_parser.cpp"
        break;

    case 211: /* expression_else  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3556 "ds_parser.cpp"
        break;

    case 213: /* expression_if_then_else  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3562 "ds_parser.cpp"
        break;

    case 214: /* expression_for_loop  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3568 "ds_parser.cpp"
        break;

    case 215: /* expression_unsafe  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3574 "ds_parser.cpp"
        break;

    case 216: /* expression_while_loop  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3580 "ds_parser.cpp"
        break;

    case 217: /* expression_with  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3586 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value  */
#line 138 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3592 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_value_list  */
#line 139 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3598 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument_name  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3604 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument  */
#line 138 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3610 "ds_parser.cpp"
        break;

    case 222: /* annotation_argument_list  */
#line 139 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3616 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration_name  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3622 "ds_parser.cpp"
        break;

    case 224: /* annotation_declaration  */
#line 140 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3628 "ds_parser.cpp"
        break;

    case 225: /* annotation_list  */
#line 141 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3634 "ds_parser.cpp"
        break;

    case 226: /* optional_annotation_list  */
#line 141 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3640 "ds_parser.cpp"
        break;

    case 227: /* optional_function_argument_list  */
#line 135 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3646 "ds_parser.cpp"
        break;

    case 228: /* optional_function_type  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3652 "ds_parser.cpp"
        break;

    case 229: /* function_name  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3658 "ds_parser.cpp"
        break;

    case 234: /* expression_block  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 235: /* expression_any  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 236: /* expressions  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3676 "ds_parser.cpp"
        break;

    case 237: /* expr_pipe  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3682 "ds_parser.cpp"
        break;

    case 238: /* name_in_namespace  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3688 "ds_parser.cpp"
        break;

    case 239: /* expression_delete  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3694 "ds_parser.cpp"
        break;

    case 240: /* expr_new  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3700 "ds_parser.cpp"
        break;

    case 241: /* expression_break  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3706 "ds_parser.cpp"
        break;

    case 242: /* expression_continue  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3712 "ds_parser.cpp"
        break;

    case 243: /* expression_return  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3718 "ds_parser.cpp"
        break;

    case 244: /* expression_yield  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3724 "ds_parser.cpp"
        break;

    case 245: /* expression_try_catch  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3730 "ds_parser.cpp"
        break;

    case 247: /* expression_let  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3736 "ds_parser.cpp"
        break;

    case 248: /* expr_cast  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3742 "ds_parser.cpp"
        break;

    case 255: /* expr_type_decl  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3748 "ds_parser.cpp"
        break;

    case 258: /* expr_type_info  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3754 "ds_parser.cpp"
        break;

    case 259: /* expr_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3760 "ds_parser.cpp"
        break;

    case 260: /* block_or_simple_block  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3766 "ds_parser.cpp"
        break;

    case 262: /* capture_entry  */
#line 144 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3772 "ds_parser.cpp"
        break;

    case 263: /* capture_list  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3778 "ds_parser.cpp"
        break;

    case 264: /* optional_capture_list  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3784 "ds_parser.cpp"
        break;

    case 265: /* expr_block  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3790 "ds_parser.cpp"
        break;

    case 266: /* expr_numeric_const  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3796 "ds_parser.cpp"
        break;

    case 267: /* expr_assign  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3802 "ds_parser.cpp"
        break;

    case 268: /* expr_assign_pipe  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3808 "ds_parser.cpp"
        break;

    case 269: /* expr_named_call  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3814 "ds_parser.cpp"
        break;

    case 270: /* expr_method_call  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3820 "ds_parser.cpp"
        break;

    case 271: /* func_addr_expr  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3826 "ds_parser.cpp"
        break;

    case 276: /* expr_field  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3832 "ds_parser.cpp"
        break;

    case 279: /* expr  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3838 "ds_parser.cpp"
        break;

    case 282: /* optional_field_annotation  */
#line 139 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3844 "ds_parser.cpp"
        break;

    case 286: /* structure_variable_declaration  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3850 "ds_parser.cpp"
        break;

    case 287: /* struct_variable_declaration_list  */
#line 135 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3856 "ds_parser.cpp"
        break;

    case 289: /* function_argument_declaration  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3862 "ds_parser.cpp"
        break;

    case 290: /* function_argument_list  */
#line 135 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3868 "ds_parser.cpp"
        break;

    case 291: /* tuple_type  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3874 "ds_parser.cpp"
        break;

    case 292: /* tuple_type_list  */
#line 135 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3880 "ds_parser.cpp"
        break;

    case 293: /* variant_type  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3886 "ds_parser.cpp"
        break;

    case 294: /* variant_type_list  */
#line 135 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3892 "ds_parser.cpp"
        break;

    case 296: /* variable_declaration  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3898 "ds_parser.cpp"
        break;

    case 299: /* let_variable_declaration  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3904 "ds_parser.cpp"
        break;

    case 300: /* global_variable_declaration_list  */
#line 135 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3910 "ds_parser.cpp"
        break;

    case 305: /* enum_list  */
#line 143 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3916 "ds_parser.cpp"
        break;

    case 311: /* optional_structure_parent  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3922 "ds_parser.cpp"
        break;

    case 317: /* variable_name_with_pos_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3928 "ds_parser.cpp"
        break;

    case 320: /* structure_type_declaration  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3934 "ds_parser.cpp"
        break;

    case 321: /* auto_type_declaration  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3940 "ds_parser.cpp"
        break;

    case 322: /* bitfield_bits  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3946 "ds_parser.cpp"
        break;

    case 323: /* bitfield_type_declaration  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3952 "ds_parser.cpp"
        break;

    case 326: /* type_declaration  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3958 "ds_parser.cpp"
        break;

    case 355: /* make_decl  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3964 "ds_parser.cpp"
        break;

    case 356: /* make_struct_fields  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3970 "ds_parser.cpp"
        break;

    case 357: /* make_struct_dim  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3976 "ds_parser.cpp"
        break;

    case 358: /* optional_block  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3982 "ds_parser.cpp"
        break;

    case 359: /* make_struct_decl  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3988 "ds_parser.cpp"
        break;

    case 360: /* make_tuple  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3994 "ds_parser.cpp"
        break;

    case 361: /* make_map_tuple  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4000 "ds_parser.cpp"
        break;

    case 362: /* make_any_tuple  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4006 "ds_parser.cpp"
        break;

    case 363: /* make_dim  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4012 "ds_parser.cpp"
        break;

    case 364: /* make_dim_decl  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4018 "ds_parser.cpp"
        break;

    case 365: /* make_table  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4024 "ds_parser.cpp"
        break;

    case 366: /* make_table_decl  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4030 "ds_parser.cpp"
        break;

    case 367: /* array_comprehension_where  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4036 "ds_parser.cpp"
        break;

    case 368: /* array_comprehension  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4042 "ds_parser.cpp"
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
#line 480 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4334 "ds_parser.cpp"
    break;

  case 16:
#line 481 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4340 "ds_parser.cpp"
    break;

  case 17:
#line 482 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4346 "ds_parser.cpp"
    break;

  case 18:
#line 486 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4352 "ds_parser.cpp"
    break;

  case 19:
#line 487 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4358 "ds_parser.cpp"
    break;

  case 20:
#line 491 "ds_parser.ypp"
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
#line 4377 "ds_parser.cpp"
    break;

  case 21:
#line 508 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4383 "ds_parser.cpp"
    break;

  case 22:
#line 509 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4389 "ds_parser.cpp"
    break;

  case 23:
#line 510 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4395 "ds_parser.cpp"
    break;

  case 24:
#line 511 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4401 "ds_parser.cpp"
    break;

  case 25:
#line 515 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4407 "ds_parser.cpp"
    break;

  case 26:
#line 519 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4417 "ds_parser.cpp"
    break;

  case 27:
#line 524 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4431 "ds_parser.cpp"
    break;

  case 28:
#line 533 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4441 "ds_parser.cpp"
    break;

  case 29:
#line 541 "ds_parser.ypp"
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
#line 4459 "ds_parser.cpp"
    break;

  case 30:
#line 557 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4469 "ds_parser.cpp"
    break;

  case 31:
#line 562 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4479 "ds_parser.cpp"
    break;

  case 32:
#line 570 "ds_parser.ypp"
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
#line 4506 "ds_parser.cpp"
    break;

  case 33:
#line 591 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4518 "ds_parser.cpp"
    break;

  case 34:
#line 600 "ds_parser.ypp"
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
#line 4542 "ds_parser.cpp"
    break;

  case 36:
#line 626 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4550 "ds_parser.cpp"
    break;

  case 37:
#line 629 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4559 "ds_parser.cpp"
    break;

  case 38:
#line 633 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4570 "ds_parser.cpp"
    break;

  case 39:
#line 639 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4581 "ds_parser.cpp"
    break;

  case 40:
#line 648 "ds_parser.ypp"
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
#line 4607 "ds_parser.cpp"
    break;

  case 41:
#line 669 "ds_parser.ypp"
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
#line 4632 "ds_parser.cpp"
    break;

  case 42:
#line 692 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4638 "ds_parser.cpp"
    break;

  case 43:
#line 693 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4644 "ds_parser.cpp"
    break;

  case 47:
#line 706 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4652 "ds_parser.cpp"
    break;

  case 48:
#line 709 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4660 "ds_parser.cpp"
    break;

  case 49:
#line 715 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4668 "ds_parser.cpp"
    break;

  case 50:
#line 721 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4676 "ds_parser.cpp"
    break;

  case 51:
#line 724 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4684 "ds_parser.cpp"
    break;

  case 52:
#line 730 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4690 "ds_parser.cpp"
    break;

  case 53:
#line 731 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4696 "ds_parser.cpp"
    break;

  case 54:
#line 735 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4702 "ds_parser.cpp"
    break;

  case 55:
#line 736 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4708 "ds_parser.cpp"
    break;

  case 56:
#line 737 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4719 "ds_parser.cpp"
    break;

  case 57:
#line 746 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4725 "ds_parser.cpp"
    break;

  case 58:
#line 747 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4731 "ds_parser.cpp"
    break;

  case 59:
#line 751 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4742 "ds_parser.cpp"
    break;

  case 60:
#line 760 "ds_parser.ypp"
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
#line 4760 "ds_parser.cpp"
    break;

  case 61:
#line 776 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4770 "ds_parser.cpp"
    break;

  case 62:
#line 784 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4782 "ds_parser.cpp"
    break;

  case 63:
#line 794 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4793 "ds_parser.cpp"
    break;

  case 64:
#line 803 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4799 "ds_parser.cpp"
    break;

  case 65:
#line 804 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4805 "ds_parser.cpp"
    break;

  case 66:
#line 805 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4811 "ds_parser.cpp"
    break;

  case 67:
#line 806 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4817 "ds_parser.cpp"
    break;

  case 68:
#line 807 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4823 "ds_parser.cpp"
    break;

  case 69:
#line 808 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4829 "ds_parser.cpp"
    break;

  case 70:
#line 812 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4839 "ds_parser.cpp"
    break;

  case 71:
#line 817 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4849 "ds_parser.cpp"
    break;

  case 72:
#line 825 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4855 "ds_parser.cpp"
    break;

  case 73:
#line 826 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4861 "ds_parser.cpp"
    break;

  case 74:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4867 "ds_parser.cpp"
    break;

  case 75:
#line 831 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4873 "ds_parser.cpp"
    break;

  case 76:
#line 832 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4879 "ds_parser.cpp"
    break;

  case 77:
#line 833 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4885 "ds_parser.cpp"
    break;

  case 78:
#line 834 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4891 "ds_parser.cpp"
    break;

  case 79:
#line 835 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4897 "ds_parser.cpp"
    break;

  case 80:
#line 836 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4903 "ds_parser.cpp"
    break;

  case 81:
#line 837 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4909 "ds_parser.cpp"
    break;

  case 82:
#line 838 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4917 "ds_parser.cpp"
    break;

  case 83:
#line 844 "ds_parser.ypp"
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
#line 4936 "ds_parser.cpp"
    break;

  case 84:
#line 858 "ds_parser.ypp"
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
#line 4955 "ds_parser.cpp"
    break;

  case 85:
#line 875 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4961 "ds_parser.cpp"
    break;

  case 86:
#line 876 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4967 "ds_parser.cpp"
    break;

  case 87:
#line 877 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4973 "ds_parser.cpp"
    break;

  case 88:
#line 881 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4986 "ds_parser.cpp"
    break;

  case 89:
#line 889 "ds_parser.ypp"
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
#line 5001 "ds_parser.cpp"
    break;

  case 90:
#line 902 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5010 "ds_parser.cpp"
    break;

  case 91:
#line 906 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5019 "ds_parser.cpp"
    break;

  case 92:
#line 913 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 5025 "ds_parser.cpp"
    break;

  case 93:
#line 914 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 5031 "ds_parser.cpp"
    break;

  case 94:
#line 918 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5037 "ds_parser.cpp"
    break;

  case 95:
#line 919 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5043 "ds_parser.cpp"
    break;

  case 96:
#line 920 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5049 "ds_parser.cpp"
    break;

  case 97:
#line 924 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5057 "ds_parser.cpp"
    break;

  case 98:
#line 927 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5066 "ds_parser.cpp"
    break;

  case 99:
#line 934 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5075 "ds_parser.cpp"
    break;

  case 100:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5081 "ds_parser.cpp"
    break;

  case 101:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5087 "ds_parser.cpp"
    break;

  case 102:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5093 "ds_parser.cpp"
    break;

  case 103:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5099 "ds_parser.cpp"
    break;

  case 104:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5105 "ds_parser.cpp"
    break;

  case 105:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5111 "ds_parser.cpp"
    break;

  case 106:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5117 "ds_parser.cpp"
    break;

  case 107:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5123 "ds_parser.cpp"
    break;

  case 108:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5129 "ds_parser.cpp"
    break;

  case 109:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5135 "ds_parser.cpp"
    break;

  case 110:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5141 "ds_parser.cpp"
    break;

  case 111:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5147 "ds_parser.cpp"
    break;

  case 112:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5153 "ds_parser.cpp"
    break;

  case 113:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5159 "ds_parser.cpp"
    break;

  case 114:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5165 "ds_parser.cpp"
    break;

  case 115:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5171 "ds_parser.cpp"
    break;

  case 116:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5177 "ds_parser.cpp"
    break;

  case 117:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5183 "ds_parser.cpp"
    break;

  case 118:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5189 "ds_parser.cpp"
    break;

  case 119:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5195 "ds_parser.cpp"
    break;

  case 120:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5201 "ds_parser.cpp"
    break;

  case 121:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5207 "ds_parser.cpp"
    break;

  case 122:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5213 "ds_parser.cpp"
    break;

  case 123:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5219 "ds_parser.cpp"
    break;

  case 124:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5225 "ds_parser.cpp"
    break;

  case 125:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5231 "ds_parser.cpp"
    break;

  case 126:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5237 "ds_parser.cpp"
    break;

  case 127:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5243 "ds_parser.cpp"
    break;

  case 128:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5249 "ds_parser.cpp"
    break;

  case 129:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5255 "ds_parser.cpp"
    break;

  case 130:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5261 "ds_parser.cpp"
    break;

  case 131:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5267 "ds_parser.cpp"
    break;

  case 132:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5273 "ds_parser.cpp"
    break;

  case 133:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5279 "ds_parser.cpp"
    break;

  case 134:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5285 "ds_parser.cpp"
    break;

  case 135:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5291 "ds_parser.cpp"
    break;

  case 136:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5297 "ds_parser.cpp"
    break;

  case 137:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5303 "ds_parser.cpp"
    break;

  case 138:
#line 976 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5309 "ds_parser.cpp"
    break;

  case 139:
#line 977 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5315 "ds_parser.cpp"
    break;

  case 140:
#line 978 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5321 "ds_parser.cpp"
    break;

  case 141:
#line 979 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5327 "ds_parser.cpp"
    break;

  case 142:
#line 980 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5333 "ds_parser.cpp"
    break;

  case 143:
#line 981 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5339 "ds_parser.cpp"
    break;

  case 144:
#line 982 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5345 "ds_parser.cpp"
    break;

  case 145:
#line 983 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5351 "ds_parser.cpp"
    break;

  case 146:
#line 984 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5357 "ds_parser.cpp"
    break;

  case 147:
#line 985 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5363 "ds_parser.cpp"
    break;

  case 148:
#line 989 "ds_parser.ypp"
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
#line 5387 "ds_parser.cpp"
    break;

  case 149:
#line 1011 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5393 "ds_parser.cpp"
    break;

  case 150:
#line 1012 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5399 "ds_parser.cpp"
    break;

  case 151:
#line 1013 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5405 "ds_parser.cpp"
    break;

  case 152:
#line 1017 "ds_parser.ypp"
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
#line 5445 "ds_parser.cpp"
    break;

  case 153:
#line 1055 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5455 "ds_parser.cpp"
    break;

  case 154:
#line 1063 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5464 "ds_parser.cpp"
    break;

  case 155:
#line 1067 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5477 "ds_parser.cpp"
    break;

  case 156:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5483 "ds_parser.cpp"
    break;

  case 157:
#line 1079 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5489 "ds_parser.cpp"
    break;

  case 158:
#line 1080 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5495 "ds_parser.cpp"
    break;

  case 159:
#line 1081 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5501 "ds_parser.cpp"
    break;

  case 160:
#line 1082 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5507 "ds_parser.cpp"
    break;

  case 161:
#line 1083 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5513 "ds_parser.cpp"
    break;

  case 162:
#line 1084 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5519 "ds_parser.cpp"
    break;

  case 163:
#line 1085 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5525 "ds_parser.cpp"
    break;

  case 164:
#line 1086 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5531 "ds_parser.cpp"
    break;

  case 165:
#line 1087 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5537 "ds_parser.cpp"
    break;

  case 166:
#line 1088 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5543 "ds_parser.cpp"
    break;

  case 167:
#line 1089 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5549 "ds_parser.cpp"
    break;

  case 168:
#line 1090 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5555 "ds_parser.cpp"
    break;

  case 169:
#line 1091 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5561 "ds_parser.cpp"
    break;

  case 170:
#line 1092 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5567 "ds_parser.cpp"
    break;

  case 171:
#line 1093 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5573 "ds_parser.cpp"
    break;

  case 172:
#line 1094 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5579 "ds_parser.cpp"
    break;

  case 173:
#line 1095 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5585 "ds_parser.cpp"
    break;

  case 174:
#line 1096 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5591 "ds_parser.cpp"
    break;

  case 175:
#line 1100 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5601 "ds_parser.cpp"
    break;

  case 176:
#line 1105 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5612 "ds_parser.cpp"
    break;

  case 177:
#line 1111 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5620 "ds_parser.cpp"
    break;

  case 178:
#line 1117 "ds_parser.ypp"
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
#line 5659 "ds_parser.cpp"
    break;

  case 179:
#line 1151 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5667 "ds_parser.cpp"
    break;

  case 180:
#line 1154 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5675 "ds_parser.cpp"
    break;

  case 181:
#line 1160 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5681 "ds_parser.cpp"
    break;

  case 182:
#line 1161 "ds_parser.ypp"
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
#line 5697 "ds_parser.cpp"
    break;

  case 183:
#line 1172 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5703 "ds_parser.cpp"
    break;

  case 184:
#line 1176 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5711 "ds_parser.cpp"
    break;

  case 185:
#line 1182 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5719 "ds_parser.cpp"
    break;

  case 186:
#line 1185 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5727 "ds_parser.cpp"
    break;

  case 187:
#line 1188 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5736 "ds_parser.cpp"
    break;

  case 188:
#line 1192 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5744 "ds_parser.cpp"
    break;

  case 189:
#line 1198 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5750 "ds_parser.cpp"
    break;

  case 190:
#line 1202 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5756 "ds_parser.cpp"
    break;

  case 191:
#line 1206 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5764 "ds_parser.cpp"
    break;

  case 192:
#line 1209 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5772 "ds_parser.cpp"
    break;

  case 193:
#line 1212 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5782 "ds_parser.cpp"
    break;

  case 194:
#line 1217 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5790 "ds_parser.cpp"
    break;

  case 195:
#line 1220 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5800 "ds_parser.cpp"
    break;

  case 196:
#line 1228 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5808 "ds_parser.cpp"
    break;

  case 197:
#line 1231 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5818 "ds_parser.cpp"
    break;

  case 198:
#line 1236 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5826 "ds_parser.cpp"
    break;

  case 199:
#line 1239 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5836 "ds_parser.cpp"
    break;

  case 200:
#line 1247 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5844 "ds_parser.cpp"
    break;

  case 201:
#line 1253 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5850 "ds_parser.cpp"
    break;

  case 202:
#line 1254 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5856 "ds_parser.cpp"
    break;

  case 203:
#line 1258 "ds_parser.ypp"
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
#line 5892 "ds_parser.cpp"
    break;

  case 204:
#line 1292 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5898 "ds_parser.cpp"
    break;

  case 205:
#line 1292 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5904 "ds_parser.cpp"
    break;

  case 206:
#line 1292 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5912 "ds_parser.cpp"
    break;

  case 207:
#line 1295 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5918 "ds_parser.cpp"
    break;

  case 208:
#line 1295 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5924 "ds_parser.cpp"
    break;

  case 209:
#line 1295 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5934 "ds_parser.cpp"
    break;

  case 210:
#line 1300 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5940 "ds_parser.cpp"
    break;

  case 211:
#line 1300 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5946 "ds_parser.cpp"
    break;

  case 212:
#line 1300 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5956 "ds_parser.cpp"
    break;

  case 213:
#line 1308 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5962 "ds_parser.cpp"
    break;

  case 214:
#line 1308 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5968 "ds_parser.cpp"
    break;

  case 215:
#line 1308 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5976 "ds_parser.cpp"
    break;

  case 216:
#line 1314 "ds_parser.ypp"
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
#line 5991 "ds_parser.cpp"
    break;

  case 217:
#line 1324 "ds_parser.ypp"
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
#line 6007 "ds_parser.cpp"
    break;

  case 218:
#line 1335 "ds_parser.ypp"
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
#line 6024 "ds_parser.cpp"
    break;

  case 219:
#line 1350 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6032 "ds_parser.cpp"
    break;

  case 220:
#line 1353 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6040 "ds_parser.cpp"
    break;

  case 221:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6046 "ds_parser.cpp"
    break;

  case 222:
#line 1360 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6058 "ds_parser.cpp"
    break;

  case 223:
#line 1367 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6071 "ds_parser.cpp"
    break;

  case 224:
#line 1378 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6077 "ds_parser.cpp"
    break;

  case 225:
#line 1379 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6083 "ds_parser.cpp"
    break;

  case 226:
#line 1380 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6089 "ds_parser.cpp"
    break;

  case 227:
#line 1384 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6095 "ds_parser.cpp"
    break;

  case 228:
#line 1385 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6101 "ds_parser.cpp"
    break;

  case 229:
#line 1386 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6107 "ds_parser.cpp"
    break;

  case 230:
#line 1387 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6113 "ds_parser.cpp"
    break;

  case 231:
#line 1391 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6123 "ds_parser.cpp"
    break;

  case 232:
#line 1396 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6133 "ds_parser.cpp"
    break;

  case 233:
#line 1404 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6139 "ds_parser.cpp"
    break;

  case 234:
#line 1405 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6145 "ds_parser.cpp"
    break;

  case 235:
#line 1409 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6155 "ds_parser.cpp"
    break;

  case 236:
#line 1415 "ds_parser.ypp"
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
#line 6220 "ds_parser.cpp"
    break;

  case 237:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6226 "ds_parser.cpp"
    break;

  case 238:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6232 "ds_parser.cpp"
    break;

  case 239:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6238 "ds_parser.cpp"
    break;

  case 240:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6244 "ds_parser.cpp"
    break;

  case 241:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6250 "ds_parser.cpp"
    break;

  case 242:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6256 "ds_parser.cpp"
    break;

  case 243:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6262 "ds_parser.cpp"
    break;

  case 244:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6268 "ds_parser.cpp"
    break;

  case 245:
#line 1489 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6274 "ds_parser.cpp"
    break;

  case 246:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6280 "ds_parser.cpp"
    break;

  case 247:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6286 "ds_parser.cpp"
    break;

  case 248:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6292 "ds_parser.cpp"
    break;

  case 249:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6298 "ds_parser.cpp"
    break;

  case 250:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6304 "ds_parser.cpp"
    break;

  case 251:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6310 "ds_parser.cpp"
    break;

  case 252:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6316 "ds_parser.cpp"
    break;

  case 253:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6322 "ds_parser.cpp"
    break;

  case 254:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6328 "ds_parser.cpp"
    break;

  case 255:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6334 "ds_parser.cpp"
    break;

  case 256:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6340 "ds_parser.cpp"
    break;

  case 257:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6346 "ds_parser.cpp"
    break;

  case 258:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6352 "ds_parser.cpp"
    break;

  case 259:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6358 "ds_parser.cpp"
    break;

  case 260:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6364 "ds_parser.cpp"
    break;

  case 261:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6370 "ds_parser.cpp"
    break;

  case 262:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6376 "ds_parser.cpp"
    break;

  case 263:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6382 "ds_parser.cpp"
    break;

  case 264:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6388 "ds_parser.cpp"
    break;

  case 265:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6394 "ds_parser.cpp"
    break;

  case 266:
#line 1516 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6406 "ds_parser.cpp"
    break;

  case 267:
#line 1527 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6416 "ds_parser.cpp"
    break;

  case 268:
#line 1532 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6428 "ds_parser.cpp"
    break;

  case 269:
#line 1542 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6437 "ds_parser.cpp"
    break;

  case 270:
#line 1546 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6443 "ds_parser.cpp"
    break;

  case 271:
#line 1546 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6449 "ds_parser.cpp"
    break;

  case 272:
#line 1546 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6461 "ds_parser.cpp"
    break;

  case 273:
#line 1553 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6467 "ds_parser.cpp"
    break;

  case 274:
#line 1553 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6473 "ds_parser.cpp"
    break;

  case 275:
#line 1553 "ds_parser.ypp"
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
#line 6489 "ds_parser.cpp"
    break;

  case 276:
#line 1567 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6498 "ds_parser.cpp"
    break;

  case 277:
#line 1571 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6507 "ds_parser.cpp"
    break;

  case 278:
#line 1575 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6513 "ds_parser.cpp"
    break;

  case 279:
#line 1575 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6519 "ds_parser.cpp"
    break;

  case 280:
#line 1575 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6528 "ds_parser.cpp"
    break;

  case 281:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6534 "ds_parser.cpp"
    break;

  case 282:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6540 "ds_parser.cpp"
    break;

  case 283:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6546 "ds_parser.cpp"
    break;

  case 284:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6552 "ds_parser.cpp"
    break;

  case 285:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6558 "ds_parser.cpp"
    break;

  case 286:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6564 "ds_parser.cpp"
    break;

  case 287:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6570 "ds_parser.cpp"
    break;

  case 288:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6576 "ds_parser.cpp"
    break;

  case 289:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6582 "ds_parser.cpp"
    break;

  case 290:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6588 "ds_parser.cpp"
    break;

  case 291:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6594 "ds_parser.cpp"
    break;

  case 292:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6600 "ds_parser.cpp"
    break;

  case 293:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6606 "ds_parser.cpp"
    break;

  case 294:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6612 "ds_parser.cpp"
    break;

  case 295:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6618 "ds_parser.cpp"
    break;

  case 296:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6624 "ds_parser.cpp"
    break;

  case 297:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6630 "ds_parser.cpp"
    break;

  case 298:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6636 "ds_parser.cpp"
    break;

  case 299:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6642 "ds_parser.cpp"
    break;

  case 300:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6648 "ds_parser.cpp"
    break;

  case 301:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6654 "ds_parser.cpp"
    break;

  case 302:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6660 "ds_parser.cpp"
    break;

  case 303:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6666 "ds_parser.cpp"
    break;

  case 304:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6672 "ds_parser.cpp"
    break;

  case 305:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6678 "ds_parser.cpp"
    break;

  case 306:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6684 "ds_parser.cpp"
    break;

  case 307:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6690 "ds_parser.cpp"
    break;

  case 308:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6696 "ds_parser.cpp"
    break;

  case 309:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6702 "ds_parser.cpp"
    break;

  case 310:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6708 "ds_parser.cpp"
    break;

  case 311:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6714 "ds_parser.cpp"
    break;

  case 312:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6720 "ds_parser.cpp"
    break;

  case 313:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6726 "ds_parser.cpp"
    break;

  case 314:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6732 "ds_parser.cpp"
    break;

  case 315:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6738 "ds_parser.cpp"
    break;

  case 316:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6744 "ds_parser.cpp"
    break;

  case 317:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6750 "ds_parser.cpp"
    break;

  case 318:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6756 "ds_parser.cpp"
    break;

  case 319:
#line 1620 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6762 "ds_parser.cpp"
    break;

  case 320:
#line 1621 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6768 "ds_parser.cpp"
    break;

  case 321:
#line 1622 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6774 "ds_parser.cpp"
    break;

  case 322:
#line 1623 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6780 "ds_parser.cpp"
    break;

  case 323:
#line 1624 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6786 "ds_parser.cpp"
    break;

  case 324:
#line 1625 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6792 "ds_parser.cpp"
    break;

  case 325:
#line 1626 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6798 "ds_parser.cpp"
    break;

  case 326:
#line 1627 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6804 "ds_parser.cpp"
    break;

  case 327:
#line 1628 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6813 "ds_parser.cpp"
    break;

  case 328:
#line 1632 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6822 "ds_parser.cpp"
    break;

  case 329:
#line 1636 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6830 "ds_parser.cpp"
    break;

  case 330:
#line 1639 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6838 "ds_parser.cpp"
    break;

  case 331:
#line 1642 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6844 "ds_parser.cpp"
    break;

  case 332:
#line 1643 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6850 "ds_parser.cpp"
    break;

  case 333:
#line 1644 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6856 "ds_parser.cpp"
    break;

  case 334:
#line 1645 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6870 "ds_parser.cpp"
    break;

  case 335:
#line 1654 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6884 "ds_parser.cpp"
    break;

  case 336:
#line 1663 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6890 "ds_parser.cpp"
    break;

  case 337:
#line 1664 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6898 "ds_parser.cpp"
    break;

  case 338:
#line 1667 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6904 "ds_parser.cpp"
    break;

  case 339:
#line 1667 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6910 "ds_parser.cpp"
    break;

  case 340:
#line 1667 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6918 "ds_parser.cpp"
    break;

  case 341:
#line 1670 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6927 "ds_parser.cpp"
    break;

  case 342:
#line 1674 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6936 "ds_parser.cpp"
    break;

  case 343:
#line 1678 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6945 "ds_parser.cpp"
    break;

  case 344:
#line 1682 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6951 "ds_parser.cpp"
    break;

  case 345:
#line 1683 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6957 "ds_parser.cpp"
    break;

  case 346:
#line 1684 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6963 "ds_parser.cpp"
    break;

  case 347:
#line 1685 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6969 "ds_parser.cpp"
    break;

  case 348:
#line 1686 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6975 "ds_parser.cpp"
    break;

  case 349:
#line 1687 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6981 "ds_parser.cpp"
    break;

  case 350:
#line 1688 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6987 "ds_parser.cpp"
    break;

  case 351:
#line 1689 "ds_parser.ypp"
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
#line 7008 "ds_parser.cpp"
    break;

  case 352:
#line 1705 "ds_parser.ypp"
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
#line 7029 "ds_parser.cpp"
    break;

  case 353:
#line 1721 "ds_parser.ypp"
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
#line 7089 "ds_parser.cpp"
    break;

  case 354:
#line 1779 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7095 "ds_parser.cpp"
    break;

  case 355:
#line 1780 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7101 "ds_parser.cpp"
    break;

  case 356:
#line 1784 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_NONE; }
#line 7107 "ds_parser.cpp"
    break;

  case 357:
#line 1785 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_OVERRIDE; }
#line 7113 "ds_parser.cpp"
    break;

  case 358:
#line 1786 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_SEALED; }
#line 7119 "ds_parser.cpp"
    break;

  case 359:
#line 1790 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7125 "ds_parser.cpp"
    break;

  case 360:
#line 1791 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7131 "ds_parser.cpp"
    break;

  case 361:
#line 1795 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7137 "ds_parser.cpp"
    break;

  case 362:
#line 1796 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7143 "ds_parser.cpp"
    break;

  case 363:
#line 1797 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7149 "ds_parser.cpp"
    break;

  case 364:
#line 1801 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7161 "ds_parser.cpp"
    break;

  case 365:
#line 1811 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7169 "ds_parser.cpp"
    break;

  case 366:
#line 1814 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7178 "ds_parser.cpp"
    break;

  case 367:
#line 1819 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7184 "ds_parser.cpp"
    break;

  case 368:
#line 1819 "ds_parser.ypp"
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
#line 7234 "ds_parser.cpp"
    break;

  case 369:
#line 1866 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->atDecl = tokRangeAt((yylsp[-5]),(yylsp[0]));
        (yyvsp[-1].pFuncDecl)->body = (yyvsp[0].pExpression);
        if ( !g_thisStructure ) {
            das_yyerror("internal error. member function outside of class.",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( !g_thisStructure->isClass ) {
            das_yyerror("structure can't have member function",
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
                    das_yyerror("can't override initializer or finalizer " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                        (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[-2].b) ) {
                    das_yyerror("can't have constant initializer or finalizer " + (yyvsp[-1].pFuncDecl)->getMangledName(),
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
#line 7303 "ds_parser.cpp"
    break;

  case 370:
#line 1930 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7314 "ds_parser.cpp"
    break;

  case 371:
#line 1939 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7324 "ds_parser.cpp"
    break;

  case 372:
#line 1944 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7334 "ds_parser.cpp"
    break;

  case 373:
#line 1952 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7340 "ds_parser.cpp"
    break;

  case 374:
#line 1953 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7346 "ds_parser.cpp"
    break;

  case 375:
#line 1957 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7354 "ds_parser.cpp"
    break;

  case 376:
#line 1960 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7365 "ds_parser.cpp"
    break;

  case 377:
#line 1969 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7371 "ds_parser.cpp"
    break;

  case 378:
#line 1970 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7377 "ds_parser.cpp"
    break;

  case 379:
#line 1974 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7388 "ds_parser.cpp"
    break;

  case 380:
#line 1983 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7394 "ds_parser.cpp"
    break;

  case 381:
#line 1984 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7400 "ds_parser.cpp"
    break;

  case 382:
#line 1989 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7406 "ds_parser.cpp"
    break;

  case 383:
#line 1990 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7412 "ds_parser.cpp"
    break;

  case 384:
#line 1994 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7423 "ds_parser.cpp"
    break;

  case 385:
#line 2000 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7434 "ds_parser.cpp"
    break;

  case 386:
#line 2006 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7442 "ds_parser.cpp"
    break;

  case 387:
#line 2009 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7451 "ds_parser.cpp"
    break;

  case 388:
#line 2013 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7462 "ds_parser.cpp"
    break;

  case 389:
#line 2019 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7473 "ds_parser.cpp"
    break;

  case 390:
#line 2028 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7479 "ds_parser.cpp"
    break;

  case 391:
#line 2029 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7485 "ds_parser.cpp"
    break;

  case 392:
#line 2030 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7491 "ds_parser.cpp"
    break;

  case 393:
#line 2034 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7497 "ds_parser.cpp"
    break;

  case 394:
#line 2035 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7503 "ds_parser.cpp"
    break;

  case 395:
#line 2039 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7511 "ds_parser.cpp"
    break;

  case 396:
#line 2042 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7521 "ds_parser.cpp"
    break;

  case 397:
#line 2047 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7531 "ds_parser.cpp"
    break;

  case 398:
#line 2052 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7544 "ds_parser.cpp"
    break;

  case 399:
#line 2060 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7557 "ds_parser.cpp"
    break;

  case 400:
#line 2071 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7565 "ds_parser.cpp"
    break;

  case 401:
#line 2074 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7575 "ds_parser.cpp"
    break;

  case 402:
#line 2082 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7581 "ds_parser.cpp"
    break;

  case 403:
#line 2083 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7587 "ds_parser.cpp"
    break;

  case 404:
#line 2087 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7593 "ds_parser.cpp"
    break;

  case 405:
#line 2088 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7599 "ds_parser.cpp"
    break;

  case 406:
#line 2089 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7605 "ds_parser.cpp"
    break;

  case 407:
#line 2093 "ds_parser.ypp"
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
#line 7638 "ds_parser.cpp"
    break;

  case 408:
#line 2121 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7644 "ds_parser.cpp"
    break;

  case 409:
#line 2121 "ds_parser.ypp"
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
#line 7680 "ds_parser.cpp"
    break;

  case 410:
#line 2155 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7688 "ds_parser.cpp"
    break;

  case 411:
#line 2158 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7702 "ds_parser.cpp"
    break;

  case 412:
#line 2167 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7716 "ds_parser.cpp"
    break;

  case 413:
#line 2180 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7730 "ds_parser.cpp"
    break;

  case 418:
#line 2202 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7736 "ds_parser.cpp"
    break;

  case 419:
#line 2203 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7742 "ds_parser.cpp"
    break;

  case 420:
#line 2204 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7748 "ds_parser.cpp"
    break;

  case 421:
#line 2208 "ds_parser.ypp"
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
#line 7781 "ds_parser.cpp"
    break;

  case 422:
#line 2236 "ds_parser.ypp"
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
#line 7815 "ds_parser.cpp"
    break;

  case 423:
#line 2268 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7821 "ds_parser.cpp"
    break;

  case 424:
#line 2269 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7827 "ds_parser.cpp"
    break;

  case 425:
#line 2273 "ds_parser.ypp"
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
#line 7867 "ds_parser.cpp"
    break;

  case 426:
#line 2311 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7873 "ds_parser.cpp"
    break;

  case 427:
#line 2312 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7879 "ds_parser.cpp"
    break;

  case 428:
#line 2316 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7885 "ds_parser.cpp"
    break;

  case 429:
#line 2317 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7891 "ds_parser.cpp"
    break;

  case 430:
#line 2318 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7897 "ds_parser.cpp"
    break;

  case 431:
#line 2323 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7903 "ds_parser.cpp"
    break;

  case 432:
#line 2323 "ds_parser.ypp"
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
                    if ( !pStruct->isClass && pDecl->isPrivate ) {
                        das_yyerror("only class member can be private, "+name_at.first,name_at.second,
                            CompilationError::invalid_private);
                    }
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
#line 8001 "ds_parser.cpp"
    break;

  case 433:
#line 2419 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8013 "ds_parser.cpp"
    break;

  case 434:
#line 2426 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 8024 "ds_parser.cpp"
    break;

  case 435:
#line 2435 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 8030 "ds_parser.cpp"
    break;

  case 436:
#line 2436 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 8036 "ds_parser.cpp"
    break;

  case 437:
#line 2437 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8042 "ds_parser.cpp"
    break;

  case 438:
#line 2438 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8048 "ds_parser.cpp"
    break;

  case 439:
#line 2439 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8054 "ds_parser.cpp"
    break;

  case 440:
#line 2440 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 8060 "ds_parser.cpp"
    break;

  case 441:
#line 2441 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 8066 "ds_parser.cpp"
    break;

  case 442:
#line 2442 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 8072 "ds_parser.cpp"
    break;

  case 443:
#line 2443 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8078 "ds_parser.cpp"
    break;

  case 444:
#line 2444 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8084 "ds_parser.cpp"
    break;

  case 445:
#line 2445 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8090 "ds_parser.cpp"
    break;

  case 446:
#line 2446 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8096 "ds_parser.cpp"
    break;

  case 447:
#line 2447 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8102 "ds_parser.cpp"
    break;

  case 448:
#line 2448 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8108 "ds_parser.cpp"
    break;

  case 449:
#line 2449 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8114 "ds_parser.cpp"
    break;

  case 450:
#line 2450 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8120 "ds_parser.cpp"
    break;

  case 451:
#line 2451 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8126 "ds_parser.cpp"
    break;

  case 452:
#line 2452 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8132 "ds_parser.cpp"
    break;

  case 453:
#line 2453 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8138 "ds_parser.cpp"
    break;

  case 454:
#line 2454 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8144 "ds_parser.cpp"
    break;

  case 455:
#line 2455 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8150 "ds_parser.cpp"
    break;

  case 456:
#line 2456 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8156 "ds_parser.cpp"
    break;

  case 457:
#line 2457 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8162 "ds_parser.cpp"
    break;

  case 458:
#line 2458 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8168 "ds_parser.cpp"
    break;

  case 459:
#line 2459 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8174 "ds_parser.cpp"
    break;

  case 460:
#line 2463 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8180 "ds_parser.cpp"
    break;

  case 461:
#line 2464 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8186 "ds_parser.cpp"
    break;

  case 462:
#line 2465 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8192 "ds_parser.cpp"
    break;

  case 463:
#line 2466 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8198 "ds_parser.cpp"
    break;

  case 464:
#line 2467 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8204 "ds_parser.cpp"
    break;

  case 465:
#line 2468 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8210 "ds_parser.cpp"
    break;

  case 466:
#line 2472 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8223 "ds_parser.cpp"
    break;

  case 467:
#line 2483 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8232 "ds_parser.cpp"
    break;

  case 468:
#line 2487 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8244 "ds_parser.cpp"
    break;

  case 469:
#line 2497 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8256 "ds_parser.cpp"
    break;

  case 470:
#line 2504 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8267 "ds_parser.cpp"
    break;

  case 471:
#line 2513 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8273 "ds_parser.cpp"
    break;

  case 472:
#line 2513 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8279 "ds_parser.cpp"
    break;

  case 473:
#line 2513 "ds_parser.ypp"
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
#line 8294 "ds_parser.cpp"
    break;

  case 474:
#line 2526 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8300 "ds_parser.cpp"
    break;

  case 475:
#line 2527 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8306 "ds_parser.cpp"
    break;

  case 476:
#line 2528 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8312 "ds_parser.cpp"
    break;

  case 477:
#line 2529 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8318 "ds_parser.cpp"
    break;

  case 478:
#line 2530 "ds_parser.ypp"
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
#line 8337 "ds_parser.cpp"
    break;

  case 479:
#line 2544 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8348 "ds_parser.cpp"
    break;

  case 480:
#line 2550 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8357 "ds_parser.cpp"
    break;

  case 481:
#line 2554 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8366 "ds_parser.cpp"
    break;

  case 482:
#line 2558 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8376 "ds_parser.cpp"
    break;

  case 483:
#line 2563 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8386 "ds_parser.cpp"
    break;

  case 484:
#line 2568 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8396 "ds_parser.cpp"
    break;

  case 485:
#line 2573 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8406 "ds_parser.cpp"
    break;

  case 486:
#line 2578 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8415 "ds_parser.cpp"
    break;

  case 487:
#line 2582 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8424 "ds_parser.cpp"
    break;

  case 488:
#line 2586 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8434 "ds_parser.cpp"
    break;

  case 489:
#line 2591 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8443 "ds_parser.cpp"
    break;

  case 490:
#line 2595 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8453 "ds_parser.cpp"
    break;

  case 491:
#line 2600 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8459 "ds_parser.cpp"
    break;

  case 492:
#line 2600 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8465 "ds_parser.cpp"
    break;

  case 493:
#line 2600 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8476 "ds_parser.cpp"
    break;

  case 494:
#line 2606 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8488 "ds_parser.cpp"
    break;

  case 495:
#line 2613 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8494 "ds_parser.cpp"
    break;

  case 496:
#line 2613 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8500 "ds_parser.cpp"
    break;

  case 497:
#line 2613 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8510 "ds_parser.cpp"
    break;

  case 498:
#line 2618 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8516 "ds_parser.cpp"
    break;

  case 499:
#line 2618 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8522 "ds_parser.cpp"
    break;

  case 500:
#line 2618 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8533 "ds_parser.cpp"
    break;

  case 501:
#line 2624 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8539 "ds_parser.cpp"
    break;

  case 502:
#line 2624 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8545 "ds_parser.cpp"
    break;

  case 503:
#line 2624 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8555 "ds_parser.cpp"
    break;

  case 504:
#line 2629 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8564 "ds_parser.cpp"
    break;

  case 505:
#line 2633 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8570 "ds_parser.cpp"
    break;

  case 506:
#line 2633 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8576 "ds_parser.cpp"
    break;

  case 507:
#line 2633 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8586 "ds_parser.cpp"
    break;

  case 508:
#line 2638 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8592 "ds_parser.cpp"
    break;

  case 509:
#line 2638 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8598 "ds_parser.cpp"
    break;

  case 510:
#line 2638 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8612 "ds_parser.cpp"
    break;

  case 511:
#line 2647 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8621 "ds_parser.cpp"
    break;

  case 512:
#line 2651 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8627 "ds_parser.cpp"
    break;

  case 513:
#line 2651 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8633 "ds_parser.cpp"
    break;

  case 514:
#line 2651 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8643 "ds_parser.cpp"
    break;

  case 515:
#line 2656 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8649 "ds_parser.cpp"
    break;

  case 516:
#line 2656 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8655 "ds_parser.cpp"
    break;

  case 517:
#line 2656 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8669 "ds_parser.cpp"
    break;

  case 518:
#line 2665 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8678 "ds_parser.cpp"
    break;

  case 519:
#line 2669 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8684 "ds_parser.cpp"
    break;

  case 520:
#line 2669 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8690 "ds_parser.cpp"
    break;

  case 521:
#line 2669 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8700 "ds_parser.cpp"
    break;

  case 522:
#line 2674 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8706 "ds_parser.cpp"
    break;

  case 523:
#line 2674 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8712 "ds_parser.cpp"
    break;

  case 524:
#line 2674 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8726 "ds_parser.cpp"
    break;

  case 525:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8732 "ds_parser.cpp"
    break;

  case 526:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8738 "ds_parser.cpp"
    break;

  case 527:
#line 2683 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8749 "ds_parser.cpp"
    break;

  case 528:
#line 2689 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8755 "ds_parser.cpp"
    break;

  case 529:
#line 2689 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8761 "ds_parser.cpp"
    break;

  case 530:
#line 2689 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8772 "ds_parser.cpp"
    break;

  case 531:
#line 2698 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8778 "ds_parser.cpp"
    break;

  case 532:
#line 2698 "ds_parser.ypp"
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
#line 8795 "ds_parser.cpp"
    break;

  case 533:
#line 2713 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8801 "ds_parser.cpp"
    break;

  case 534:
#line 2713 "ds_parser.ypp"
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
#line 8822 "ds_parser.cpp"
    break;

  case 535:
#line 2733 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8828 "ds_parser.cpp"
    break;

  case 536:
#line 2734 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8834 "ds_parser.cpp"
    break;

  case 537:
#line 2735 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8840 "ds_parser.cpp"
    break;

  case 538:
#line 2736 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8846 "ds_parser.cpp"
    break;

  case 539:
#line 2740 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8858 "ds_parser.cpp"
    break;

  case 540:
#line 2747 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8870 "ds_parser.cpp"
    break;

  case 541:
#line 2754 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8881 "ds_parser.cpp"
    break;

  case 542:
#line 2760 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8892 "ds_parser.cpp"
    break;

  case 543:
#line 2769 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8902 "ds_parser.cpp"
    break;

  case 544:
#line 2774 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8911 "ds_parser.cpp"
    break;

  case 545:
#line 2781 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8917 "ds_parser.cpp"
    break;

  case 546:
#line 2782 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8923 "ds_parser.cpp"
    break;

  case 547:
#line 2786 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8934 "ds_parser.cpp"
    break;

  case 548:
#line 2792 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8946 "ds_parser.cpp"
    break;

  case 549:
#line 2799 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8959 "ds_parser.cpp"
    break;

  case 550:
#line 2807 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8971 "ds_parser.cpp"
    break;

  case 551:
#line 2814 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8984 "ds_parser.cpp"
    break;

  case 552:
#line 2822 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8998 "ds_parser.cpp"
    break;

  case 553:
#line 2834 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 9006 "ds_parser.cpp"
    break;

  case 554:
#line 2837 "ds_parser.ypp"
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
#line 9022 "ds_parser.cpp"
    break;

  case 555:
#line 2851 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9033 "ds_parser.cpp"
    break;

  case 556:
#line 2860 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9039 "ds_parser.cpp"
    break;

  case 557:
#line 2861 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9045 "ds_parser.cpp"
    break;

  case 558:
#line 2865 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9055 "ds_parser.cpp"
    break;

  case 559:
#line 2870 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9064 "ds_parser.cpp"
    break;

  case 560:
#line 2877 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9074 "ds_parser.cpp"
    break;

  case 561:
#line 2882 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9086 "ds_parser.cpp"
    break;

  case 562:
#line 2892 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9096 "ds_parser.cpp"
    break;

  case 563:
#line 2897 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9105 "ds_parser.cpp"
    break;

  case 564:
#line 2904 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9119 "ds_parser.cpp"
    break;

  case 565:
#line 2916 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9125 "ds_parser.cpp"
    break;

  case 566:
#line 2917 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9131 "ds_parser.cpp"
    break;

  case 567:
#line 2921 "ds_parser.ypp"
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
#line 9154 "ds_parser.cpp"
    break;

  case 568:
#line 2939 "ds_parser.ypp"
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
#line 9177 "ds_parser.cpp"
    break;


#line 9181 "ds_parser.cpp"

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
#line 2959 "ds_parser.ypp"


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
