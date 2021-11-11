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
#define YYLAST   7693

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  182
/* YYNRULES -- Number of rules.  */
#define YYNRULES  570
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  993

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
    1704,  1705,  1721,  1737,  1795,  1796,  1800,  1801,  1802,  1806,
    1807,  1811,  1812,  1813,  1817,  1827,  1830,  1835,  1834,  1880,
    1946,  1955,  1960,  1968,  1969,  1973,  1976,  1985,  1986,  1990,
    1999,  2000,  2005,  2006,  2010,  2016,  2022,  2025,  2029,  2035,
    2044,  2045,  2046,  2050,  2051,  2055,  2058,  2063,  2068,  2076,
    2087,  2090,  2098,  2099,  2103,  2104,  2105,  2109,  2137,  2137,
    2171,  2174,  2183,  2196,  2208,  2209,  2213,  2214,  2218,  2219,
    2220,  2224,  2252,  2284,  2285,  2289,  2290,  2294,  2338,  2339,
    2343,  2344,  2345,  2350,  2349,  2446,  2453,  2462,  2463,  2464,
    2465,  2466,  2467,  2468,  2469,  2470,  2471,  2472,  2473,  2474,
    2475,  2476,  2477,  2478,  2479,  2480,  2481,  2482,  2483,  2484,
    2485,  2486,  2490,  2491,  2492,  2493,  2494,  2495,  2499,  2510,
    2514,  2524,  2531,  2540,  2540,  2540,  2553,  2554,  2555,  2556,
    2557,  2571,  2577,  2581,  2585,  2590,  2595,  2600,  2605,  2609,
    2613,  2618,  2622,  2627,  2627,  2627,  2633,  2640,  2640,  2640,
    2645,  2645,  2645,  2651,  2651,  2651,  2656,  2660,  2660,  2660,
    2665,  2665,  2665,  2674,  2678,  2678,  2678,  2683,  2683,  2683,
    2692,  2696,  2696,  2696,  2701,  2701,  2701,  2710,  2710,  2710,
    2716,  2716,  2716,  2725,  2725,  2740,  2740,  2760,  2761,  2762,
    2763,  2767,  2774,  2781,  2787,  2796,  2801,  2808,  2809,  2813,
    2819,  2826,  2834,  2841,  2849,  2861,  2864,  2878,  2887,  2888,
    2892,  2897,  2904,  2909,  2919,  2924,  2931,  2943,  2944,  2948,
    2966
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

#define YYPACT_NINF -557

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-557)))

#define YYTABLE_NINF -525

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -557,    21,  -557,  -557,    16,   -19,   120,  -100,  -557,   -38,
    -557,  -557,     4,  -557,  -557,  -557,  -557,  -557,   152,  -557,
      49,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
     -25,  -557,    35,    -4,    47,  -557,  -557,   120,     2,  -557,
      23,    56,  -557,  -557,  -557,    49,    60,   102,  -557,  -557,
     106,   154,   156,  -557,    38,  -557,  -557,  -557,    22,   223,
     296,  -557,   316,    14,    16,   211,   -19,   193,   240,  -557,
     260,   276,  -557,  7460,   243,   -88,   345,   249,   252,  -557,
     299,    16,     4,  -557,  -557,  -557,    17,  -557,  -557,  -557,
     300,  -557,  -557,   354,  -557,  -557,   262,  -557,  -557,  -557,
    -557,  -557,    89,    86,  -557,  -557,  -557,  -557,   401,  -557,
    -557,   297,   301,   303,   308,  -557,  -557,  -557,   277,  -557,
    -557,  -557,  -557,  -557,   309,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,   310,  -557,  -557,  -557,   311,   314,
    -557,  -557,  -557,  -557,   324,   326,  -557,  -557,  -557,  -557,
    -557,  3224,  -557,  -557,   312,  -557,  -557,  -557,   315,   347,
    -557,   109,  -557,  7072,   426,   460,  -557,   318,   319,  -101,
    -557,   349,  -557,  -557,   362,  -557,  -557,   265,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,   146,  -557,  -557,  -557,  -557,
    -557,   355,  -557,   168,   202,   219,  -557,  -557,  -557,  -557,
    -557,  -557,   468,  -557,  -557,    67,  3008,  -557,  -557,  -557,
     321,   350,  -557,   322,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,   329,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
     330,  -557,  -557,   -75,   356,  -557,  -557,   284,  -557,   357,
     331,   -93,    16,   369,  -557,  -557,  -557,    86,  -557,  7460,
    7460,  7460,  7460,   336,   315,  7460,   318,  7460,   318,  7460,
     318,  7547,   347,  -557,  -557,  -557,   340,  -557,  -557,  -557,
    7373,   341,   359,  -557,   343,   363,   365,   348,   366,  -557,
     368,  4656,  4656,  7199,  7286,  4656,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  4656,  4656,  4656,   232,  4656,  4656,  -557,
    4656,  -557,   351,  -557,  -557,  -557,   -70,  -557,  -557,  -557,
    -557,   353,  -557,  -557,  -557,  -557,  -557,  -557,  5776,   358,
    -557,  -557,  -557,  -557,  -557,   -34,  7460,     5,  -557,  -557,
    -557,    -3,  -557,   226,  7460,  -557,  1309,  -557,  -557,  -557,
    -557,  -557,  -557,   361,     9,   232,  -557,  -557,  -557,   369,
      99,  -557,  -557,   132,  -557,    84,   396,   516,  1024,  -557,
     195,  1431,   356,  1592,   356,  1750,   356,   178,  -557,   203,
    3224,   205,  -557,  1908,  -557,   232,  -557,  4656,  -557,  -557,
    4656,  -557,  7460,   302,   302,   369,  1656,   369,  1814,  6338,
    -557,  -120,   317,   302,   302,   -46,  -557,   302,   302,  5221,
     119,  -557,  2430,   393,    27,   390,  4656,  4656,  -557,  -557,
    4656,  4656,  4656,  4656,   391,  4656,   395,  4656,  4656,  4656,
    4656,  4656,  4656,  4656,  4656,  3148,  4656,  4656,  4656,  4656,
    4656,  4656,  4656,  4656,  4656,  4656,   -31,  4656,  -557,  3288,
    -557,  -557,  3224,  -557,  -557,   369,  -557,   104,  -557,   362,
    3224,  -557,  4656,  -557,  -557,   369,  2130,  -557,   319,  4656,
    4656,  -557,   371,   392,  3446,   319,  2290,   241,   241,  -557,
     503,   373,   376,  4656,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,   377,   378,   379,  -557,  -557,  -557,   369,  -557,   -94,
    -557,  6270,  -557,    73,  -557,  -557,    12,  -557,   370,   404,
    7460,  -557,   -35,  -557,  7460,  -557,  -557,  -557,   407,  -557,
     385,  -557,   402,  -557,   405,  7460,  -557,  7547,  -557,   347,
    3586,  3744,  7460,  5299,  7460,  7460,  5380,  7460,  1971,    13,
     241,   126,  1972,  3884,  6338,   412,   -13,   389,   416,  -557,
    -557,   182,    28,  4042,   -13,   110,  4656,  4656,   398,  -557,
    4656,   183,   423,  -557,   255,  -557,   429,  -557,   158,  6566,
     -41,   318,   421,  -557,  -557,    58,    58,   325,   325,  6989,
    6989,   397,   278,  -557,  5852,   -49,   -49,    58,    58,  6714,
     620,  6634,   439,  6418,  6741,  6821,  6957,   325,   325,    19,
      19,   278,   278,   278,   440,  4656,  -557,   441,  4656,   587,
    5928,  -557,   166,  -557,  -557,  -557,  7460,  -557,  2588,  -557,
     490,    34,  2588,  -557,  -557,   469,   871,   577,  6566,   490,
    -557,   434,   453,  6566,  -557,  2588,  -557,  1124,   417,  -557,
    -557,   414,  -557,  -557,   490,  -557,  -557,  -557,  -557,   241,
    -557,  4656,  4656,  4656,  4656,  4656,  4656,  4656,  4656,  4656,
    4656,  2728,  4656,  4656,  4656,  4656,  4656,  4656,  2868,    69,
    4656,  -557,     4,  -557,   598,     7,   427,  -557,  -557,   763,
    -557,  -557,  -557,  2588,  -557,  2066,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  3224,  -557,  -557,  -557,  -557,
     172,   464,  5458,  2226,  -557,  2366,  2524,  -557,  2664,   393,
    4656,  -557,  4656,  4656,    -1,   466,   429,   435,   438,  4656,
     446,  4656,  4656,   429,   432,   447,  6566,  -557,  -557,  6486,
    -557,   471,  7460,   318,   -33,   118,  4656,  -557,   481,   482,
     483,   484,  -557,   161,   356,  -557,  4200,  -557,  -557,  4656,
    -557,  6004,  -557,  6080,  -557,  -557,  -557,  2807,  -557,  6169,
    -557,  4656,  -557,  4746,  4656,  4656,  -557,   319,  -557,  -557,
    -557,  -557,  4846,  -557,  -557,  -557,   380,  -557,  6566,  6566,
    6566,  6566,  6566,  6566,  6566,  6566,  6566,  6566,  4656,  4656,
    6566,  6566,  6566,  6566,  6566,  6566,  6566,  4656,  4656,  6566,
    -557,   998,   164,   352,  -557,  -557,   352,  -557,   468,  -557,
    2588,  -557,  4946,  -557,  -557,  -557,  -557,  -557,   206,  -557,
    -557,  -557,  -557,  -557,   451,   -66,  6566,  6566,   -13,   454,
     136,   412,   455,  -557,  6566,  -557,  -557,   -45,   -13,   456,
    -557,  -557,  -557,  2944,   356,   457,  6566,  -557,  -557,  -557,
    -557,   -41,   458,   -91,  7460,  -557,   175,  6566,  -557,  -557,
    -557,   468,  4656,    50,  -557,  -557,  -557,  1495,   319,  -557,
    -557,  4656,  -557,  6889,  6566,  6889,  6566,  -557,   452,  -557,
    -557,    53,   369,  -557,  5046,  -557,  -557,  4656,   493,  -557,
    4656,  4656,  4656,  4358,  4656,   463,   465,  4656,  4656,  -557,
    4656,   459,  -557,  -557,   485,  -557,  -557,  -557,  4516,  -557,
    -557,  3084,  -557,  6566,   319,  -557,  -557,   490,  -557,  -557,
     611,  -557,  -557,  5539,   496,   325,   325,   325,  -557,  5617,
    5146,   480,  -557,  6566,  6566,  5146,   486,   232,  -557,  4656,
    6566,  -557,  -557,   380,    17,  -557,    17,  -557,  4656,  -557,
     612,   487,  -557,   476,  -557,  -557,   232,  6566,  -557,  -557,
     479,   319,  5698,  4656,   491,   494,  -557,  -557,  -557,  -557,
    6566,  -557,  -557
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   201,     0,     0,     0,     0,   202,     0,
     535,   533,     0,    14,     3,    10,     9,     8,     0,     7,
     402,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   417,    19,    18,   402,     0,     0,    86,    87,
       0,   181,    88,    90,     0,    85,   429,   428,   149,   418,
     430,   403,   404,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   183,
       0,     0,     0,    93,   151,   150,     0,   148,   420,   419,
       0,   432,   431,   425,   406,   405,   408,    79,    80,    77,
      78,    76,     0,     0,    75,    84,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   437,   457,   438,   469,   439,
     443,   444,   445,   446,   461,   450,   451,   452,   453,   454,
     455,   456,   458,   459,   506,   442,   449,   460,   513,   520,
     440,   447,   441,   448,     0,     0,   468,   476,   479,   477,
     478,   413,   414,   416,     0,    16,    17,    20,     0,     0,
     182,     0,    91,     0,     0,     0,    99,    94,     0,     0,
     426,     0,   433,   400,   354,    21,    22,     0,    68,    69,
      66,    67,    65,    64,    70,     0,    41,   497,   500,   503,
     493,     0,   473,   507,   514,   521,   527,   530,   484,   489,
     483,   496,     0,   492,   486,     0,     0,   488,   415,   471,
       0,     0,   380,     0,    89,   147,   102,   103,   105,   104,
     106,   107,   108,   109,   134,   135,   132,   133,   125,   136,
     137,   126,   123,   124,   145,     0,   146,   138,   139,   140,
     141,   113,   114,   115,   110,   111,   112,   128,   129,   127,
     121,   122,   117,   116,   118,   119,   120,   101,   100,   144,
       0,   130,   131,   354,    97,   175,   153,     0,   410,   423,
       0,   354,     0,     0,    23,    24,    25,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    94,     0,    94,     0,
      94,     0,     0,   491,   485,   487,     0,   490,   287,   288,
       0,     0,     0,   281,     0,     0,     0,     0,     0,   461,
       0,     0,     0,     0,     0,     0,   237,   239,   238,   240,
     241,   242,    26,     0,     0,     0,     0,     0,     0,   481,
       0,   224,   225,   285,   284,   235,   282,   347,   346,   345,
     344,    92,   350,   283,   349,   348,   326,   289,     0,     0,
     286,   537,   538,   539,   540,     0,     0,     0,   143,   142,
      95,     0,   373,     0,     0,   152,     0,   462,   465,   463,
     466,   464,   467,     0,     0,     0,   427,   365,   407,     0,
       0,   435,   409,   393,    71,     0,     0,     0,     0,   470,
       0,     0,    97,     0,    97,     0,    97,   181,   377,     0,
     375,     0,   482,   185,   188,     0,   213,     0,   204,   207,
       0,   210,     0,   315,   316,     0,   547,     0,     0,     0,
     564,     0,     0,   293,   292,   331,    32,   291,   290,     0,
     226,   353,     0,   233,     0,     0,     0,     0,   317,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   278,     0,   480,     0,
     472,   536,   379,   534,   381,     0,   371,   384,    96,   354,
      98,   177,     0,    57,    58,     0,     0,   189,     0,     0,
       0,   190,     0,     0,     0,     0,     0,     0,     0,   156,
     154,     0,     0,     0,   170,   165,   163,   162,   164,   176,
     157,     0,     0,     0,   168,   169,   171,     0,   161,     0,
     158,   243,   410,     0,   421,   424,   354,   401,     0,     0,
       0,   394,     0,   498,     0,   504,   494,   474,     0,   508,
       0,   515,     0,   522,     0,     0,   528,     0,   531,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,   555,   545,   547,     0,   558,   559,
     560,     0,     0,     0,   547,     0,     0,     0,     0,    29,
       0,    27,     0,   319,   270,   269,     0,   327,     0,   219,
       0,    94,     0,   341,   342,   294,   295,   307,   308,   305,
     306,     0,   336,   324,     0,   351,   352,   296,   297,   312,
     313,   314,     0,     0,   310,   311,   309,   303,   304,   299,
     298,   300,   301,   302,     0,     0,   276,     0,     0,     0,
       0,   329,     0,   372,   383,   382,     0,   385,     0,   374,
       0,     0,     0,   191,   194,     0,   243,     0,   184,     0,
     174,     0,     0,    51,    61,     0,   198,   243,   225,   179,
     180,     0,   172,   173,     0,   160,   166,   167,   203,     0,
     159,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   411,     0,   434,     0,   356,     0,   355,   436,     0,
     391,   392,   390,     0,   499,     0,   505,   495,   475,   509,
     511,   516,   518,   523,   525,   376,   529,   378,   532,   186,
       0,     0,     0,     0,   332,     0,     0,   333,     0,   233,
       0,   548,     0,     0,   547,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   557,   565,   566,     0,
      30,    33,     0,    94,     0,     0,     0,   328,     0,     0,
       0,     0,   231,     0,    97,   338,     0,   322,   343,     0,
     325,     0,   277,     0,   279,   320,   330,   386,   389,   388,
      62,     0,   195,   243,     0,     0,   192,     0,    63,    49,
      50,   199,   243,   196,   226,   175,    54,   178,   253,   254,
     256,   255,   257,   247,   248,   249,   258,   259,     0,     0,
     245,   246,   260,   261,   250,   251,   252,     0,     0,   244,
     422,     0,     0,   361,   357,   358,   361,   366,   390,   395,
       0,   399,   243,   501,   512,   519,   526,   187,     0,   216,
     214,   205,   208,   211,     0,     0,   542,   541,   547,     0,
       0,   546,     0,   550,   556,   562,   561,     0,   547,     0,
     563,    28,    31,     0,    97,     0,   220,   229,   230,   228,
     227,     0,     0,     0,     0,   267,     0,   337,   323,   321,
     280,   382,     0,     0,   193,   200,   197,     0,     0,    52,
      53,     0,    59,   264,   265,   262,   263,   412,    93,   362,
     363,   356,     0,   397,   243,   398,   502,     0,     0,   215,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   549,
       0,     0,   553,   271,     0,   266,   232,   234,     0,   221,
     236,     0,   268,   387,     0,   155,    55,     0,   370,   367,
     359,   364,   396,     0,     0,   206,   209,   212,   334,     0,
     567,     0,   551,   544,   543,   567,     0,     0,   274,     0,
     222,   339,    60,    54,     0,   360,     0,   217,     0,   335,
       0,     0,   552,     0,   554,   272,     0,   223,   340,    56,
       0,     0,     0,     0,     0,     0,   275,   368,   369,   218,
     568,   569,   570
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -557,  -557,  -557,  -557,  -557,   242,   606,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,   637,  -557,   569,  -557,  -557,   616,
    -557,  -557,  -557,  -280,  -557,  -557,  -557,  -557,  -557,  -557,
     408,  -557,  -557,   622,   -65,  -557,   602,    -5,  -323,  -275,
    -387,  -557,  -557,  -557,  -526,  -557,  -167,  -557,  -107,  -472,
     -12,  -557,  -557,  -557,  -557,  -557,  -557,  -557,   688,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -447,  -557,  -557,  -181,  -557,   -37,  -456,  -557,  -362,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,   386,  -557,  -557,  -166,  -210,  -557,  -133,  -557,  -557,
    -557,   216,  -557,   149,  -557,  -337,   410,  -467,  -469,     0,
    -557,  -348,  -557,   655,  -557,  -557,  -557,   185,    36,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -358,   -71,  -557,  -557,  -557,   420,  -557,  -557,  -557,    24,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,   415,  -556,
    -401,  -541,  -557,  -557,  -303,   -36,   290,  -557,  -557,  -557,
    -243,  -557
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   157,    45,    14,   177,   183,   422,   333,   751,
     334,   582,    15,    16,    38,    39,    72,    17,    34,    35,
     501,   502,   891,   892,   503,   504,   505,   506,   507,   508,
     184,   185,    30,    31,    32,    52,    53,    54,    18,   264,
     365,   167,    19,    86,   168,    87,   335,   509,   366,   510,
     336,   511,   337,   512,   513,   514,   515,   516,   517,   518,
     338,   554,   910,   555,   911,   557,   912,   339,   552,   909,
     340,   588,   930,   341,   762,   763,   591,   342,   343,   645,
     520,   344,   345,   346,   752,   957,   753,   976,   347,   629,
     880,   589,   874,   978,   361,   826,   966,   901,   696,   526,
     964,   362,   363,   398,   399,   212,   213,   733,   476,   703,
     532,   382,   271,    62,    96,    21,   174,   374,    42,    75,
      22,    90,    23,   376,   171,   172,    60,    93,    24,   270,
     383,   349,   373,   148,   149,   210,   150,   284,   708,   400,
     282,   707,   279,   704,   280,   906,   281,   706,   285,   709,
     286,   834,   287,   711,   288,   835,   289,   713,   290,   836,
     291,   716,   292,   718,    25,    47,    26,    46,   350,   565,
     566,   567,   351,   568,   569,   570,   571,   352,   421,   353,
     971,   354
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   266,   147,   477,   519,   540,   633,   542,   273,   544,
     638,   392,   420,   394,   644,   396,   161,   574,   433,   -92,
     474,     2,   632,    68,   656,   737,   669,     3,    97,    98,
     755,   527,   730,   744,    27,    28,   560,    48,   928,   434,
     435,   659,   660,   475,   272,   592,    40,   742,   560,   824,
       4,   163,     5,   781,     6,    69,   267,   559,    40,   572,
       7,   146,   272,    49,   577,   444,   578,     8,   446,   447,
      55,   446,   447,     9,   758,    84,   431,    74,   434,   435,
     700,    85,   634,   759,   268,    41,   624,   625,   756,   701,
     670,   732,    10,   378,   265,   824,   939,   151,   153,   294,
     178,   179,   825,   720,   731,   379,   360,    61,    43,   756,
     432,   154,   470,    11,   760,   626,   198,   477,   914,   761,
     702,    33,   635,   164,   165,   438,   439,   641,   466,   467,
      63,   466,   467,   444,    50,   445,   446,   447,   448,   920,
     199,   200,   449,   381,    44,   754,   627,   628,   825,   272,
      51,   211,   471,    65,    99,   523,    56,    57,   100,    58,
     101,   102,    29,   166,   438,   439,   778,   529,    70,   668,
     782,   736,   444,   593,   445,   446,   447,   448,    73,    71,
     851,   449,   529,   791,   463,   464,   465,    59,   529,    64,
     692,   473,    82,   849,   103,   524,   466,   467,   693,    12,
     201,    66,    40,   694,   756,    13,    77,   380,   147,   147,
     147,   147,   474,   797,   147,   523,   147,    83,   147,   634,
     147,   461,   462,   463,   464,   465,   180,   295,   690,   147,
     181,   831,   182,   102,   934,   466,   467,   175,   176,   202,
     203,   634,   147,   147,   204,   296,   533,   205,    78,    50,
     732,   634,    79,    64,   297,   820,    80,   691,   529,   635,
     917,   636,   206,    64,   637,    51,    36,   146,   146,   146,
     146,   207,   735,   146,   747,   146,    88,   146,   528,   146,
     584,   635,    89,   845,    80,   147,   529,    37,   146,   530,
     214,   635,   531,   147,   741,   857,   745,   865,   434,   435,
     277,   146,   146,   385,   386,   387,   388,   915,    80,   391,
     882,   393,   756,   395,   426,   871,   764,   921,    82,   876,
     756,   647,   434,   435,   403,  -510,   756,   278,   654,   756,
    -510,   274,   275,   848,   883,   545,    81,   416,   418,   757,
     872,   147,   858,   898,   146,   434,   435,   776,  -510,    91,
     367,   106,   146,   837,   368,    92,   932,   537,   903,  -517,
     695,   740,    50,   525,  -517,   546,   741,   548,   907,    94,
      71,   369,   370,   371,   372,    95,  -524,   873,    51,   538,
     472,  -524,  -517,   918,   438,   439,   108,   547,   480,   549,
     908,   888,   444,   551,   445,   446,   447,   448,   155,  -524,
     146,   449,   889,   890,   156,   899,   109,   478,   438,   439,
     479,   900,  -273,   274,   275,   276,   444,  -273,   585,   446,
     447,   448,   110,   331,   658,   449,   265,   152,   198,   436,
     437,   438,   439,   941,   158,  -273,   558,   159,   980,   444,
     981,   445,   446,   447,   448,   160,   169,   173,   449,   170,
     450,   451,   199,   200,    69,   466,   467,   191,   187,   147,
     261,   209,   188,   147,   189,   175,   176,   579,   580,   190,
     192,   193,   194,   780,   147,   195,   147,   924,   864,   466,
     467,   147,   788,   147,   147,   196,   147,   197,   461,   462,
     463,   464,   465,   211,   262,   269,   208,   796,   263,   272,
     293,   283,   466,   467,   265,   355,   357,   356,   358,   359,
     434,   435,   201,   364,   375,   381,   377,   389,   146,   402,
     406,   405,   146,   407,   408,   519,   409,   411,   410,   412,
     590,    12,   651,   146,   430,   146,   594,   601,   469,   661,
     146,   603,   146,   146,   477,   146,   522,   710,   198,   697,
     698,   202,   203,   470,   699,   650,   204,   662,   705,   205,
     663,   665,   666,   667,   712,   147,   735,   714,   738,   715,
     739,   750,   199,   200,   206,   754,   723,   766,   725,   726,
     534,   728,   765,   207,   748,   768,   770,   772,   774,   669,
     787,   789,   348,   790,   436,   437,   438,   439,   440,   795,
     794,   441,   442,   443,   444,   823,   445,   446,   447,   448,
     838,   827,   850,   449,   852,   450,   451,   853,   859,   862,
     885,   452,   453,   454,   146,   855,   860,   867,   868,   869,
     870,   913,   201,   916,   919,   922,   938,   927,   925,   944,
     434,   435,   951,   965,   952,   956,   455,   958,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   968,   972,
     777,   983,   985,   987,   581,   974,   984,   466,   467,   104,
     991,   202,   203,   992,    67,   265,   204,   186,   535,   205,
      55,   147,   107,   979,   162,   384,   105,   822,   887,    20,
     926,   940,   844,   902,   206,   639,   717,   413,   414,   830,
      76,   419,   401,   207,   390,   856,   929,   689,   575,   423,
     424,   425,   973,   427,   428,   404,   429,     0,     0,     0,
       0,   936,     0,     0,   436,   437,   438,   439,   440,     0,
       0,   441,   442,   443,   444,     0,   445,   446,   447,   448,
     146,     0,     0,   449,     0,   450,   451,     0,     0,     0,
       0,   452,   521,   454,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   962,     0,     0,
     963,     0,     0,     0,     0,     0,   863,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,     0,     0,
       0,     0,     0,   553,     0,   198,   556,   466,   467,     0,
       0,     0,   564,   147,   564,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   988,     0,     0,     0,     0,   199,
     200,     0,   595,   596,     0,     0,   597,   598,   599,   600,
       0,   602,     0,   604,   605,   606,   607,   608,   609,   610,
     611,   613,   614,   615,   616,   617,   618,   619,   620,   621,
     622,   623,     0,   630,     0,     0,     0,     0,     0,     0,
       0,     0,   146,     0,     0,     0,     0,     0,   640,     0,
       0,     0,   646,     0,     0,   648,   649,     0,   700,   201,
     653,     0,   657,     0,     0,     0,     0,   701,     0,   664,
       0,   434,   435,     0,     0,     0,     0,     0,   931,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   828,   203,
       0,     0,     0,   204,     0,     0,   205,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   722,     0,     0,
       0,   206,     0,     0,     0,   975,     0,   829,   423,   429,
     207,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,   746,   419,   986,     0,   749,   671,   672,   673,
     674,   675,   676,   677,   678,   436,   437,   438,   439,   440,
     679,   680,   441,   442,   443,   444,   784,   445,   446,   447,
     448,     0,     0,     0,   449,   682,   450,   451,   683,   684,
       0,     0,   452,   453,   454,   685,   686,   687,     0,     0,
       0,   771,     0,     0,   773,     0,     0,     0,   434,   435,
       0,     0,     0,     0,   779,     0,   785,   455,   783,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,     0,
       0,   792,     0,     0,     0,     0,     0,     0,   466,   467,
       0,     0,     0,     0,     0,   786,   198,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   810,   811,   812,
     813,   814,   815,   816,   819,     0,   821,     0,     0,     0,
     199,   200,     0,     0,     0,     0,     0,     0,     0,   832,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   437,   438,   439,   440,     0,     0,   441,
     442,   443,   444,     0,   445,   446,   447,   448,   846,   847,
       0,   449,     0,   450,   451,   854,     0,   564,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
     201,     0,   866,     0,   434,   435,     0,     0,     0,     0,
       0,     0,     0,     0,   455,   877,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,     0,     0,
     810,   819,     0,     0,     0,   466,   467,     0,     0,   202,
     203,     0,   897,     0,   204,     0,   536,   205,     0,     0,
       0,     0,     0,     0,   893,   894,     0,     0,     0,     0,
       0,     0,   206,   895,   896,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,   904,     0,     0,     0,
     671,   672,   673,   674,   675,   676,   677,   678,   436,   437,
     438,   439,   440,   679,   680,   441,   442,   443,   444,   784,
     445,   446,   447,   448,     0,     0,     0,   449,   682,   450,
     451,   683,   684,     0,     0,   452,   453,   454,   685,   686,
     687,     0,     0,     0,     0,     0,     0,     0,   933,     0,
       0,     0,     0,   521,     0,     0,     0,   937,     0,   785,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,   943,     0,     0,   945,   946,   947,   949,
     950,   466,   467,   953,   954,     0,   955,     0,   793,     0,
     481,     0,     0,     0,   960,     3,     0,   482,   483,   484,
       0,   485,     0,   298,   299,   300,   301,   302,     0,     0,
       0,     0,     0,     0,   486,   303,   487,   488,     0,     0,
       0,     0,     0,     0,     0,   977,   489,   304,     0,   490,
     305,     0,     0,   306,   982,     8,   307,   491,     0,   492,
     308,     0,     0,   493,   494,     0,     0,     0,     0,   990,
     495,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     309,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   310,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    51,   322,     0,     0,     0,
       0,     0,     0,   198,     0,     0,     0,     0,     0,     0,
       0,     0,   323,   324,   325,     0,   326,     0,     0,   327,
     328,     0,     0,     0,     0,     0,     0,   199,   200,   330,
       0,   331,   332,   499,   265,   500,   481,     0,     0,     0,
       0,     3,     0,   482,   483,   484,     0,   485,     0,   298,
     299,   300,   301,   302,     0,     0,     0,     0,     0,     0,
     486,   303,   487,   488,     0,     0,     0,     0,     0,     0,
       0,     0,   489,   304,     0,   490,   305,     0,     0,   306,
       0,     8,   307,   491,     0,   492,   308,   201,     0,   493,
     494,     0,     0,     0,     0,     0,   495,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   309,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   202,   203,   310,   496,
       0,   204,     0,   539,   205,     0,     0,     0,     0,     0,
       0,   311,   312,     0,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,   497,   498,   207,     0,
       0,     0,     0,     0,   198,    50,     0,     0,     0,     0,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    51,   322,     0,     0,     0,     0,     0,   199,   200,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   324,
     325,     0,   326,     0,     0,   327,   328,     0,     0,     0,
     298,   299,   300,   301,   302,   330,     0,   331,   332,   499,
     265,   935,   303,     0,     0,     0,     0,     0,   198,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,   560,     0,   308,   201,     0,
       0,     0,   199,   200,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   202,   203,   310,
       0,     0,   204,     0,   541,   205,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
     206,     0,   201,     0,     0,     0,     0,     0,     0,   207,
       0,     0,   198,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   561,   322,     0,     0,   199,   200,     0,     0,
       0,   202,   203,     0,     0,     0,   204,     0,     0,   562,
     324,   325,     0,   326,     0,     0,   327,   328,   298,   299,
     300,   301,   302,     0,   206,     0,   563,     0,   331,   332,
     303,   265,     0,   207,     0,     0,   198,     0,     0,     0,
       0,     0,   304,     0,     0,   305,     0,     0,   306,     0,
       0,   307,     0,     0,     0,   308,   201,     0,     0,     0,
     199,   200,     0,     0,     0,     0,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   309,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,   202,   203,   310,     0,     0,
     204,     0,   543,   205,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,   206,     0,
     201,     0,     0,     0,     0,     0,     0,   207,     0,     0,
     198,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     561,   322,     0,     0,   199,   200,     0,     0,     0,   202,
     203,     0,     0,     0,   204,     0,     0,   562,   324,   325,
       0,   326,     0,     0,   327,   328,   298,   299,   300,   301,
     302,     0,   206,     0,   573,     0,   331,   332,   303,   265,
       0,   207,     0,   198,   294,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,   201,     0,     0,   199,   200,     0,
       0,     0,     0,     0,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,   202,   203,   310,     0,     0,   204,     0,
       0,   205,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,   206,   201,   550,     0,
       0,     0,     0,     0,     0,   207,     0,     0,   198,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    51,   322,
       0,     0,   199,   200,     0,     0,   202,   203,     0,     0,
       0,   204,   295,   729,   205,   323,   324,   325,     0,   326,
       0,     0,   327,   328,   298,   299,   300,   301,   302,   206,
     296,     0,   330,     0,   331,   332,   303,   265,   207,   297,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   310,     0,     0,   204,     0,   833,   205,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,   206,   642,     0,     0,     0,     0,
       0,   497,   498,   207,     0,     0,     0,     0,   198,     0,
      50,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,     0,   199,   200,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,     0,     0,   298,   299,   300,   301,   302,     0,
     330,     0,   331,   332,   643,   265,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   310,     0,     0,   204,     0,   840,   205,
       0,     0,     0,     0,     0,     0,   311,   312,   198,     0,
       0,     0,     0,     0,   206,   655,     0,     0,     0,     0,
       0,   497,   498,   207,     0,     0,     0,     0,     0,     0,
      50,     0,   199,   200,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,   323,   324,   325,   303,   326,     0,     0,
     327,   328,     0,     0,     0,     0,     0,     0,   304,     0,
     330,   305,   331,   332,   306,   265,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   310,     0,     0,   204,     0,   841,   205,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,   206,     0,     0,     0,     0,     0,
       0,     0,     0,   207,     0,     0,   198,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
     199,   200,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   298,   299,   300,   301,   302,     0,   586,     0,
     330,   587,   331,   332,   303,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   310,     0,     0,   204,     0,   842,   205,     0,     0,
       0,     0,     0,     0,   311,   312,   198,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,   497,
     498,   207,     0,     0,     0,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   310,     0,     0,   204,     0,   843,   205,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,   198,
       0,     0,   206,     0,     0,     0,     0,     0,     0,   808,
     809,   207,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,   199,   200,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   265,     0,   307,     0,     0,     0,   308,
       0,     0,   634,   201,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   310,   881,   203,     0,     0,     0,   204,     0,     0,
     205,     0,     0,     0,   311,   312,   198,     0,     0,     0,
       0,     0,     0,     0,     0,   206,     0,     0,     0,   817,
     818,     0,     0,     0,   207,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   310,     0,     0,   204,     0,   923,   205,     0,     0,
       0,     0,     0,     0,   311,   312,   198,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,   612,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,   329,   330,   305,
     331,   332,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   310,     0,     0,   204,     0,   961,   205,     0,     0,
       0,     0,     0,     0,   311,   312,   198,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,    50,     0,
     199,   200,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,   323,   324,   325,   303,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   304,     0,   330,   305,
     331,   332,   306,   265,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   310,     0,     0,   204,     0,     0,   205,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   324,   325,     0,   326,     0,     0,   327,   328,
     298,   299,   300,   301,   302,     0,     0,     0,   330,   631,
     331,   332,   303,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,     0,     0,
     652,     0,     0,     0,     0,     0,     0,     0,   115,   116,
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
     306,   265,     0,   307,     0,     0,     0,   308,     0,     0,
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
     300,   301,   302,     0,     0,     0,   330,   719,   331,   332,
     303,   265,     0,     0,     0,     0,     0,     0,     0,     0,
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
     300,   301,   302,     0,     0,   721,     0,   323,   324,   325,
     303,   326,     0,     0,   327,   328,     0,     0,     0,     0,
       0,     0,   304,     0,   330,   305,   331,   332,   306,   265,
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
     302,     0,     0,     0,   330,   734,   331,   332,   303,   265,
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
       0,     0,   330,   743,   331,   332,   303,   265,     0,     0,
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
     330,   875,   331,   332,   303,   265,     0,     0,     0,     0,
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
     298,   299,   300,   301,   302,     0,     0,     0,   330,   948,
     331,   332,   303,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,   959,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,   323,
     324,   325,   303,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,   304,     0,   330,   305,   331,   332,
     306,   265,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,   434,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,   330,     0,   331,   332,
       0,   265,   671,   672,   673,   674,   675,   676,   677,   678,
     436,   437,   438,   439,   440,   679,   680,   441,   442,   443,
     444,   784,   445,   446,   447,   448,   434,   435,     0,   449,
     682,   450,   451,   683,   684,     0,     0,   452,   453,   454,
     685,   686,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   466,   467,     0,     0,     0,     0,     0,
     884,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   671,   672,   673,   674,   675,   676,   677,   678,
     436,   437,   438,   439,   440,   679,   680,   441,   442,   443,
     444,   784,   445,   446,   447,   448,   434,   435,     0,   449,
     682,   450,   451,   683,   684,     0,     0,   452,   453,   454,
     685,   686,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   466,   467,     0,     0,     0,     0,     0,
     886,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   671,   672,   673,   674,   675,   676,   677,   678,
     436,   437,   438,   439,   440,   679,   680,   441,   442,   443,
     444,   784,   445,   446,   447,   448,   434,   435,     0,   449,
     682,   450,   451,   683,   684,     0,     0,   452,   453,   454,
     685,   686,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   466,   467,     0,     0,     0,     0,     0,
     905,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   671,   672,   673,   674,   675,   676,   677,   678,
     436,   437,   438,   439,   440,   679,   680,   441,   442,   443,
     444,   784,   445,   446,   447,   448,   434,   435,     0,   449,
     682,   450,   451,   683,   684,     0,     0,   452,   453,   454,
     685,   686,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   466,   467,     0,     0,     0,     0,     0,
     942,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   435,     0,     0,     0,     0,     0,     0,     0,
     436,   437,   438,   439,   440,     0,     0,   441,   442,   443,
     444,     0,   445,   446,   447,   448,     0,     0,     0,   449,
       0,   450,   451,     0,     0,     0,     0,   452,   453,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,     0,     0,     0,     0,   434,
     435,     0,     0,   466,   467,   436,   437,   438,   439,   440,
     970,     0,   441,   442,   443,   444,     0,   445,   446,   447,
     448,     0,     0,     0,   449,     0,   450,   451,     0,     0,
       0,     0,   452,   453,   454,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   466,   467,
     434,   435,   583,   436,   437,   438,   439,   440,     0,     0,
     441,   442,   443,   444,     0,   445,   446,   447,   448,     0,
       0,     0,   449,     0,   450,   451,     0,     0,     0,     0,
     452,   453,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   434,   435,
     724,     0,     0,     0,   436,   437,   438,   439,   440,     0,
       0,   441,   442,   443,   444,     0,   445,   446,   447,   448,
       0,     0,     0,   449,     0,   450,   451,     0,     0,     0,
       0,   452,   453,   454,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   434,
     435,   727,   436,   437,   438,   439,   440,     0,     0,   441,
     442,   443,   444,     0,   445,   446,   447,   448,     0,     0,
       0,   449,     0,   450,   451,     0,     0,     0,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   434,   435,   839,
       0,     0,     0,   436,   437,   438,   439,   440,     0,     0,
     441,   442,   443,   444,     0,   445,   446,   447,   448,     0,
       0,     0,   449,     0,   450,   451,     0,     0,     0,     0,
     452,   453,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   434,   435,
     967,   436,   437,   438,   439,   440,     0,     0,   441,   442,
     443,   444,     0,   445,   446,   447,   448,     0,     0,     0,
     449,     0,   450,   451,     0,     0,     0,     0,   452,   453,
     454,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   466,   467,   434,   435,   969,     0,
       0,     0,   436,   437,   438,   439,   440,     0,     0,   441,
     442,   443,   444,     0,   445,   446,   447,   448,     0,     0,
       0,   449,     0,   450,   451,     0,     0,     0,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   466,   467,     0,     0,   989,
     436,   437,   438,   439,   440,     0,     0,   441,   442,   443,
     444,     0,   445,   446,   447,   448,     0,     0,     0,   449,
       0,   450,   451,     0,     0,     0,     0,   452,   453,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,     0,     0,     0,   434,   435,
       0,     0,     0,   466,   467,   468,   436,   437,   438,   439,
     440,     0,     0,   441,   442,   443,   444,     0,   445,   446,
     447,   448,     0,     0,     0,   449,     0,   450,   451,     0,
       0,     0,     0,   452,   453,   454,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
       0,     0,     0,     0,   434,   435,     0,     0,     0,   466,
     467,   767,   436,   437,   438,   439,   440,     0,     0,   441,
     442,   443,   444,     0,   445,   446,   447,   448,     0,     0,
       0,   449,     0,   450,   451,     0,     0,     0,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,     0,     0,
     434,   435,     0,     0,     0,   466,   467,   775,   436,   437,
     438,   439,   440,     0,     0,   441,   442,   443,   444,     0,
     445,   446,   447,   448,     0,     0,     0,   449,     0,   450,
     451,     0,     0,     0,     0,   452,   453,   454,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   878,   436,   437,   438,   439,   440,   434,
     435,   441,   442,   443,   444,     0,   445,   446,   447,   448,
       0,     0,     0,   449,     0,   450,   451,     0,     0,     0,
       0,   452,   453,   454,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   879,
       0,     0,     0,     0,     0,   671,   672,   673,   674,   675,
     676,   677,   678,   436,   437,   438,   439,   440,   679,   680,
     441,   442,   443,   444,   784,   445,   446,   447,   448,  -243,
     434,   435,   449,   682,   450,   451,   683,   684,     0,     0,
     452,   453,   454,   685,   686,   687,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   785,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   466,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,     0,     0,     0,     0,     0,   671,   672,   673,   674,
     675,   676,   677,   678,   436,   437,   438,   439,   440,   679,
     680,   441,   442,   443,   444,   681,   445,   446,   447,   448,
       0,     0,     0,   449,   682,   450,   451,   683,   684,     0,
       0,   452,   453,   454,   685,   686,   687,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   688,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   434,   435,
       0,     0,   436,   437,   438,   439,   440,   466,   467,   441,
     442,   443,   444,     0,   445,   446,   447,   448,     0,     0,
       0,   449,     0,   450,   451,     0,     0,   576,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   434,   435,     0,     0,
       0,     0,     0,     0,     0,   466,   467,     0,     0,     0,
       0,     0,   436,   437,   438,   439,   440,     0,     0,   441,
     442,   443,   444,     0,   445,   446,   447,   448,     0,     0,
       0,   449,     0,   450,   451,     0,     0,     0,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,   769,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   434,   435,     0,     0,
     436,   437,   438,   439,   440,   466,   467,   441,   442,   443,
     444,     0,   445,   446,   447,   448,     0,     0,     0,   449,
       0,   450,   451,     0,     0,     0,     0,   452,   453,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   861,     0,
       0,     0,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   434,   435,     0,     0,     0,     0,
       0,     0,     0,   466,   467,     0,     0,     0,     0,     0,
     436,   437,   438,   439,   440,     0,     0,   441,   442,   443,
     444,     0,   445,   446,   447,   448,     0,     0,     0,   449,
       0,   450,   451,     0,     0,     0,     0,   452,   453,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   434,   435,     0,     0,   436,   437,
     438,   439,   440,   466,   467,   441,   442,   443,   444,     0,
     445,   446,   447,   448,     0,     0,     0,   449,     0,   450,
     451,   434,   435,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   466,   467,     0,     0,     0,     0,     0,   436,   437,
     438,   439,   440,     0,     0,   441,   442,   443,   444,     0,
     445,   446,   447,   448,     0,     0,     0,   449,     0,   450,
     451,   434,   435,     0,     0,   436,   437,   438,   439,   440,
       0,     0,   441,   442,   443,   444,     0,   445,   446,   447,
     448,     0,     0,     0,   449,     0,   450,   451,     0,     0,
       0,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   466,   467,     0,     0,     0,     0,     0,     0,     0,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   434,
     435,     0,     0,     0,     0,     0,     0,     0,   466,   467,
       0,     0,     0,     0,     0,   436,   437,   438,   439,   440,
       0,     0,   441,   442,   443,   444,     0,   445,   446,   447,
     448,     0,     0,     0,   449,     0,   450,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,   435,     0,
       0,   458,   459,   460,   461,   462,   463,   464,   465,     0,
       0,     0,     0,   436,   437,     0,     0,   440,   466,   467,
     441,   442,   443,   444,     0,   445,   446,   447,   448,   434,
     435,     0,   449,     0,   450,   451,     0,     0,     0,     0,
     452,   453,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   455,     0,   456,   457,   458,
     459,   460,     0,     0,     0,   464,     0,     0,     0,     0,
       0,   436,   437,   438,   439,   440,   466,   467,   441,   442,
     443,   444,     0,   445,   446,   447,   448,     0,     0,     0,
     449,     0,   450,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,     0,     0,
     441,     0,     0,   444,     0,   445,   446,   447,   448,   215,
       0,     0,   449,     0,   450,   451,     0,     0,   459,   460,
     461,   462,   463,   464,   465,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   466,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     459,   460,   461,   462,   463,   464,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   466,   467,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,     0,     0,   241,   242,   243,   244,   245,   246,     0,
       0,   415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,   112,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
       0,     0,   257,   258,   113,     0,     0,     0,     0,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,     0,     0,   417,     0,
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
     313,   314,   315,     0,     0,     0,     0,     0,     0,    51,
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
       0,     0,     0,   397
};

static const yytype_int16 yycheck[] =
{
      12,   168,    73,   361,   366,   392,   475,   394,   174,   396,
     477,   286,   315,   288,   486,   290,    81,   418,   341,     7,
     357,     0,   469,    21,   496,   566,   120,     6,    14,    15,
     586,   379,    19,   574,    18,    19,    49,    33,   129,    20,
      21,   497,   498,    46,   137,    18,   146,    19,    49,    42,
      29,    34,    31,    19,    33,    53,   157,   415,   146,   417,
      39,    73,   137,    59,   184,   114,   186,    46,   117,   118,
      82,   117,   118,    52,   115,    53,   146,    41,    20,    21,
     115,    59,   115,   124,   185,   185,   117,   118,   154,   124,
     184,   124,    71,   186,   185,    42,    43,    73,   186,    32,
      14,    15,    95,   550,   560,   271,   181,    58,   146,   154,
     180,    75,   146,    92,   155,   146,    32,   475,   184,   160,
     155,   140,   155,   106,   107,   106,   107,   485,   177,   178,
     155,   177,   178,   114,   130,   116,   117,   118,   119,   184,
      56,    57,   123,   146,   182,   146,   177,   178,    95,   137,
     146,   146,   186,   157,   140,   146,     4,     5,   144,     7,
     146,   147,   146,   146,   106,   107,   638,   154,   166,   517,
     642,   184,   114,   146,   116,   117,   118,   119,   155,   177,
     736,   123,   154,   655,   165,   166,   167,    35,   154,   154,
     178,   186,   154,   734,   180,   186,   177,   178,   186,   178,
     116,   154,   146,   526,   154,   184,   146,   272,   279,   280,
     281,   282,   549,   669,   285,   146,   287,   179,   289,   115,
     291,   163,   164,   165,   166,   167,   140,   160,   155,   300,
     144,   703,   146,   147,   184,   177,   178,   148,   149,   155,
     156,   115,   313,   314,   160,   178,   162,   163,   146,   130,
     124,   115,   146,   154,   187,   186,   130,   184,   154,   155,
     124,   157,   178,   154,   160,   146,   146,   279,   280,   281,
     282,   187,   154,   285,   577,   287,    53,   289,   179,   291,
     161,   155,    59,   730,   130,   356,   154,   167,   300,   157,
     181,   155,   160,   364,   184,   742,   186,   179,    20,    21,
     154,   313,   314,   279,   280,   281,   282,   848,   130,   285,
     777,   287,   154,   289,   326,   154,   591,   858,   154,   766,
     154,   488,    20,    21,   300,   157,   154,   181,   495,   154,
     162,   148,   149,   734,   781,   157,   180,   313,   314,   181,
     179,   412,   743,   179,   356,    20,    21,   181,   180,    53,
      66,   140,   364,   181,    70,    59,   181,   162,   830,   157,
     526,   179,   130,   375,   162,   162,   184,   162,   162,    53,
     177,    87,    88,    89,    90,    59,   157,   764,   146,   184,
     356,   162,   180,   850,   106,   107,   146,   184,   364,   184,
     184,    11,   114,   405,   116,   117,   118,   119,    53,   180,
     412,   123,    22,    23,    59,    53,   146,   181,   106,   107,
     184,    59,   157,   148,   149,   150,   114,   162,   430,   117,
     118,   119,   146,   182,   183,   123,   185,   184,    32,   104,
     105,   106,   107,   902,   185,   180,   412,   185,   964,   114,
     966,   116,   117,   118,   119,   146,   146,   185,   123,    95,
     125,   126,    56,    57,    53,   177,   178,   180,   161,   530,
      34,   146,   161,   534,   161,   148,   149,   150,   151,   161,
     161,   161,   161,   640,   545,   161,   547,   864,   753,   177,
     178,   552,   649,   554,   555,   161,   557,   161,   163,   164,
     165,   166,   167,   146,    34,   146,   184,   664,   180,   137,
      32,   146,   177,   178,   185,   184,   184,   157,   179,   179,
      20,    21,   116,   157,   157,   146,   185,   181,   530,   179,
     161,   180,   534,   180,   161,   887,   161,   161,   180,   161,
     137,   178,   140,   545,   183,   547,   146,   146,   180,    36,
     552,   146,   554,   555,   902,   557,   185,   162,    32,   179,
     146,   155,   156,   146,   530,   184,   160,   184,   534,   163,
     184,   184,   184,   184,   162,   636,   154,   162,   179,   545,
     154,   148,    56,    57,   178,   146,   552,   180,   554,   555,
     184,   557,   161,   187,   186,   146,   146,   146,     1,   120,
      13,   157,   206,   140,   104,   105,   106,   107,   108,   185,
     183,   111,   112,   113,   114,     7,   116,   117,   118,   119,
     146,   184,   146,   123,   179,   125,   126,   179,   186,   148,
     787,   131,   132,   133,   636,   179,   179,   146,   146,   146,
     146,   180,   116,   179,   179,   179,   184,   179,   181,   146,
      20,    21,   179,    32,   179,   186,   156,   162,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   162,   179,
     636,    49,   186,   184,   422,   179,   179,   177,   178,    63,
     179,   155,   156,   179,    37,   185,   160,   108,   162,   163,
     692,   752,    66,   963,    82,   277,    64,   692,   795,     1,
     871,   901,   729,   826,   178,   479,   547,   311,   312,   699,
      45,   315,   292,   187,   284,   741,   873,   522,   418,   323,
     324,   325,   955,   327,   328,   300,   330,    -1,    -1,    -1,
      -1,   888,    -1,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     752,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,
      -1,   131,   366,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   934,    -1,    -1,
     937,    -1,    -1,    -1,    -1,    -1,   752,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,   407,    -1,    32,   410,   177,   178,    -1,
      -1,    -1,   416,   874,   418,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   981,    -1,    -1,    -1,    -1,    56,
      57,    -1,   436,   437,    -1,    -1,   440,   441,   442,   443,
      -1,   445,    -1,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,    -1,   467,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   874,    -1,    -1,    -1,    -1,    -1,   482,    -1,
      -1,    -1,   486,    -1,    -1,   489,   490,    -1,   115,   116,
     494,    -1,   496,    -1,    -1,    -1,    -1,   124,    -1,   503,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   874,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,    -1,   160,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   551,    -1,    -1,
      -1,   178,    -1,    -1,    -1,   957,    -1,   184,   562,   563,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   573,
      -1,    -1,   576,   577,   976,    -1,   580,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,   625,    -1,    -1,   628,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   638,    -1,   155,   156,   642,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,   655,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      -1,    -1,    -1,    -1,    -1,   184,    32,   671,   672,   673,
     674,   675,   676,   677,   678,   679,   680,   681,   682,   683,
     684,   685,   686,   687,   688,    -1,   690,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   703,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   732,   733,
      -1,   123,    -1,   125,   126,   739,    -1,   741,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   756,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   769,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
     784,   785,    -1,    -1,    -1,   177,   178,    -1,    -1,   155,
     156,    -1,   184,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   808,   809,    -1,    -1,    -1,    -1,
      -1,    -1,   178,   817,   818,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,   830,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   882,    -1,
      -1,    -1,    -1,   887,    -1,    -1,    -1,   891,    -1,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,   907,    -1,    -1,   910,   911,   912,   913,
     914,   177,   178,   917,   918,    -1,   920,    -1,   184,    -1,
       1,    -1,    -1,    -1,   928,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   959,    37,    38,    -1,    40,
      41,    -1,    -1,    44,   968,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,   983,
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
      -1,   187,    -1,    32,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   116,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   155,   156,    93,    -1,    -1,   160,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   116,   180,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    32,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,    -1,    56,    57,    -1,    -1,   155,   156,    -1,    -1,
      -1,   160,   160,   162,   163,   163,   164,   165,    -1,   167,
      -1,    -1,   170,   171,    14,    15,    16,    17,    18,   178,
     178,    -1,   180,    -1,   182,   183,    26,   185,   187,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
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
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
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
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,   155,   156,    -1,    -1,    -1,   160,    -1,    -1,
     163,    -1,    -1,    -1,   106,   107,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   121,
     122,    -1,    -1,    -1,   187,    -1,    -1,    -1,   130,    -1,
      56,    57,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    32,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      56,    57,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,   163,   164,   165,    26,   167,    -1,    -1,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    38,   179,   180,    41,
     182,   183,    44,   185,    -1,    47,    -1,    -1,    -1,    51,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   155,
     156,    93,    -1,    -1,   160,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
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
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,   177,   178,   104,   105,   106,   107,   108,
     184,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      20,    21,   181,   104,   105,   106,   107,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    20,    21,
     181,    -1,    -1,    -1,   104,   105,   106,   107,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    20,
      21,   181,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,    20,    21,   181,
      -1,    -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    20,    21,
     181,   104,   105,   106,   107,   108,    -1,    -1,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,
     123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,    20,    21,   181,    -1,
      -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   177,   178,    -1,    -1,   181,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,   177,   178,   179,   104,   105,   106,   107,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,
      -1,    -1,    -1,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   177,
     178,   179,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   177,   178,   179,   104,   105,
     106,   107,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
     126,    -1,    -1,    -1,    -1,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,   104,   105,   106,   107,   108,    20,
      21,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      20,    21,   123,   124,   125,   126,   127,   128,    -1,    -1,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,    -1,
      -1,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    20,    21,
      -1,    -1,   104,   105,   106,   107,   108,   177,   178,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,   129,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    20,    21,    -1,    -1,
     104,   105,   106,   107,   108,   177,   178,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    20,    21,    -1,    -1,   104,   105,
     106,   107,   108,   177,   178,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
     126,    20,    21,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
     126,    20,    21,    -1,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
      -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,   108,   177,   178,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    20,
      21,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,   158,   159,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   177,   178,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,    -1,    -1,    -1,
     123,    -1,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,   116,   117,   118,   119,    37,
      -1,    -1,   123,    -1,   125,   126,    -1,    -1,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,
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
      16,    17,    18,    26,    38,    41,    44,    47,    51,    71,
      93,   106,   107,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   147,   163,   164,   165,   167,   170,   171,   179,
     180,   182,   183,   196,   198,   234,   238,   240,   248,   255,
     258,   261,   265,   266,   269,   270,   271,   276,   279,   319,
     356,   360,   365,   367,   369,   184,   157,   184,   179,   179,
     181,   282,   289,   290,   157,   228,   236,    66,    70,    87,
      88,    89,    90,   320,   305,   157,   311,   185,   186,   282,
     222,   146,   299,   318,   218,   327,   327,   327,   327,   181,
     323,   327,   227,   327,   227,   327,   227,   146,   291,   292,
     327,   294,   179,   327,   356,   180,   161,   180,   161,   161,
     180,   161,   161,   279,   279,    12,   327,    12,   327,   279,
     362,   366,   195,   279,   279,   279,   238,   279,   279,   279,
     183,   146,   180,   226,    20,    21,   104,   105,   106,   107,
     108,   111,   112,   113,   114,   116,   117,   118,   119,   123,
     125,   126,   131,   132,   133,   156,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   177,   178,   179,   180,
     146,   186,   327,   186,   293,    46,   296,   318,   181,   184,
     327,     1,     8,     9,    10,    12,    25,    27,    28,    37,
      40,    48,    50,    54,    55,    61,    94,   121,   122,   184,
     186,   208,   209,   212,   213,   214,   215,   216,   217,   235,
     237,   239,   241,   242,   243,   244,   245,   246,   247,   267,
     268,   279,   185,   146,   186,   238,   287,   299,   179,   154,
     157,   160,   298,   162,   184,   162,   162,   162,   184,   162,
     228,   162,   228,   162,   228,   157,   162,   184,   162,   184,
     180,   238,   256,   279,   249,   251,   279,   253,   327,   318,
      49,   146,   163,   180,   279,   357,   358,   359,   361,   362,
     363,   364,   318,   180,   358,   364,   129,   184,   186,   150,
     151,   193,   199,   181,   161,   238,   178,   181,   259,   279,
     137,   264,    18,   146,   146,   279,   279,   279,   279,   279,
     279,   146,   279,   146,   279,   279,   279,   279,   279,   279,
     279,   279,    21,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   117,   118,   146,   177,   178,   277,
     279,   181,   259,   296,   115,   155,   157,   160,   295,   289,
     279,   318,   115,   184,   237,   267,   279,   234,   279,   279,
     184,   140,    54,   279,   234,   115,   237,   279,   183,   265,
     265,    36,   184,   184,   279,   184,   184,   184,   299,   120,
     184,    96,    97,    98,    99,   100,   101,   102,   103,   109,
     110,   115,   124,   127,   128,   134,   135,   136,   155,   305,
     155,   184,   178,   186,   226,   282,   286,   179,   146,   327,
     115,   124,   155,   297,   331,   327,   335,   329,   326,   337,
     162,   341,   162,   345,   162,   327,   349,   291,   351,   181,
     259,   161,   279,   327,   181,   327,   327,   181,   327,   162,
      19,   265,   124,   295,   181,   154,   184,   359,   179,   154,
     179,   184,    19,   181,   359,   186,   279,   362,   186,   279,
     148,   197,   272,   274,   146,   357,   154,   181,   115,   124,
     155,   160,   262,   263,   227,   161,   180,   179,   146,   157,
     146,   279,   146,   279,     1,   179,   181,   327,   237,   279,
     234,    19,   237,   279,   115,   155,   184,    13,   234,   157,
     140,   237,   279,   184,   183,   185,   234,   265,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   121,   122,
     279,   279,   279,   279,   279,   279,   279,   121,   122,   279,
     186,   279,   225,     7,    42,    95,   283,   184,   155,   184,
     297,   237,   279,   162,   339,   343,   347,   181,   146,   181,
     162,   162,   162,   162,   264,   259,   279,   279,   358,   359,
     146,   357,   179,   179,   279,   179,   363,   259,   358,   186,
     179,   152,   148,   327,   227,   179,   279,   146,   146,   146,
     146,   154,   179,   228,   280,   181,   259,   279,   179,   179,
     278,   155,   295,   259,   184,   234,   184,   236,    11,    22,
      23,   210,   211,   279,   279,   279,   279,   184,   179,    53,
      59,   285,   285,   237,   279,   184,   333,   162,   184,   257,
     250,   252,   254,   180,   184,   359,   179,   124,   295,   179,
     184,   359,   179,   162,   228,   181,   262,   179,   129,   234,
     260,   327,   181,   279,   184,   186,   234,   279,   184,    43,
     283,   296,   184,   279,   146,   279,   279,   279,   181,   279,
     279,   179,   179,   279,   279,   279,   186,   273,   162,   115,
     279,   162,   234,   234,   288,    32,   284,   181,   162,   181,
     184,   368,   179,   368,   179,   238,   275,   279,   281,   211,
     232,   232,   279,    49,   179,   186,   238,   184,   234,   181,
     279,   179,   179
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
     309,   310,   310,   311,   311,   312,   312,   313,   314,   314,
     315,   315,   315,   317,   316,   318,   318,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   320,   320,   320,   320,   320,   320,   321,   322,
     322,   323,   323,   325,   326,   324,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   328,   329,   327,   327,   330,   331,   327,
     332,   333,   327,   334,   335,   327,   327,   336,   337,   327,
     338,   339,   327,   327,   340,   341,   327,   342,   343,   327,
     327,   344,   345,   327,   346,   347,   327,   348,   349,   327,
     350,   351,   327,   353,   352,   355,   354,   356,   356,   356,
     356,   357,   357,   357,   357,   358,   358,   359,   359,   360,
     360,   360,   360,   360,   360,   361,   361,   362,   363,   363,
     364,   364,   365,   365,   366,   366,   367,   368,   368,   369,
     369
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
       1,     7,     9,     0,     2,     0,     1,     3,     1,     1,
       0,     1,     1,     0,     8,     1,     3,     1,     1,     1,
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
    case 146: /* "name"  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3489 "ds_parser.cpp"
        break;

    case 191: /* module_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3495 "ds_parser.cpp"
        break;

    case 193: /* character_sequence  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3501 "ds_parser.cpp"
        break;

    case 194: /* string_constant  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3507 "ds_parser.cpp"
        break;

    case 195: /* string_builder_body  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3513 "ds_parser.cpp"
        break;

    case 196: /* string_builder  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3519 "ds_parser.cpp"
        break;

    case 198: /* expr_reader  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3525 "ds_parser.cpp"
        break;

    case 202: /* require_module_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3531 "ds_parser.cpp"
        break;

    case 208: /* expression_label  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3537 "ds_parser.cpp"
        break;

    case 209: /* expression_goto  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3543 "ds_parser.cpp"
        break;

    case 211: /* expression_else  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3549 "ds_parser.cpp"
        break;

    case 213: /* expression_if_then_else  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3555 "ds_parser.cpp"
        break;

    case 214: /* expression_for_loop  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3561 "ds_parser.cpp"
        break;

    case 215: /* expression_unsafe  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3567 "ds_parser.cpp"
        break;

    case 216: /* expression_while_loop  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3573 "ds_parser.cpp"
        break;

    case 217: /* expression_with  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3579 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value  */
#line 149 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3585 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_value_list  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3591 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3597 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument  */
#line 149 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3603 "ds_parser.cpp"
        break;

    case 222: /* annotation_argument_list  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3609 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3615 "ds_parser.cpp"
        break;

    case 224: /* annotation_declaration  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3621 "ds_parser.cpp"
        break;

    case 225: /* annotation_list  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3627 "ds_parser.cpp"
        break;

    case 226: /* optional_annotation_list  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3633 "ds_parser.cpp"
        break;

    case 227: /* optional_function_argument_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3639 "ds_parser.cpp"
        break;

    case 228: /* optional_function_type  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3645 "ds_parser.cpp"
        break;

    case 229: /* function_name  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3651 "ds_parser.cpp"
        break;

    case 234: /* expression_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3657 "ds_parser.cpp"
        break;

    case 235: /* expression_any  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 236: /* expressions  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3669 "ds_parser.cpp"
        break;

    case 237: /* expr_pipe  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 238: /* name_in_namespace  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3681 "ds_parser.cpp"
        break;

    case 239: /* expression_delete  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 240: /* expr_new  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 241: /* expression_break  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 242: /* expression_continue  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
        break;

    case 243: /* expression_return  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3711 "ds_parser.cpp"
        break;

    case 244: /* expression_yield  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3717 "ds_parser.cpp"
        break;

    case 245: /* expression_try_catch  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3723 "ds_parser.cpp"
        break;

    case 247: /* expression_let  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3729 "ds_parser.cpp"
        break;

    case 248: /* expr_cast  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3735 "ds_parser.cpp"
        break;

    case 255: /* expr_type_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3741 "ds_parser.cpp"
        break;

    case 258: /* expr_type_info  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3747 "ds_parser.cpp"
        break;

    case 259: /* expr_list  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3753 "ds_parser.cpp"
        break;

    case 260: /* block_or_simple_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3759 "ds_parser.cpp"
        break;

    case 262: /* capture_entry  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3765 "ds_parser.cpp"
        break;

    case 263: /* capture_list  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3771 "ds_parser.cpp"
        break;

    case 264: /* optional_capture_list  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3777 "ds_parser.cpp"
        break;

    case 265: /* expr_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3783 "ds_parser.cpp"
        break;

    case 266: /* expr_numeric_const  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3789 "ds_parser.cpp"
        break;

    case 267: /* expr_assign  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3795 "ds_parser.cpp"
        break;

    case 268: /* expr_assign_pipe  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3801 "ds_parser.cpp"
        break;

    case 269: /* expr_named_call  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3807 "ds_parser.cpp"
        break;

    case 270: /* expr_method_call  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3813 "ds_parser.cpp"
        break;

    case 271: /* func_addr_expr  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3819 "ds_parser.cpp"
        break;

    case 276: /* expr_field  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3825 "ds_parser.cpp"
        break;

    case 279: /* expr  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3831 "ds_parser.cpp"
        break;

    case 282: /* optional_field_annotation  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3837 "ds_parser.cpp"
        break;

    case 286: /* structure_variable_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3843 "ds_parser.cpp"
        break;

    case 287: /* struct_variable_declaration_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3849 "ds_parser.cpp"
        break;

    case 289: /* function_argument_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3855 "ds_parser.cpp"
        break;

    case 290: /* function_argument_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3861 "ds_parser.cpp"
        break;

    case 291: /* tuple_type  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3867 "ds_parser.cpp"
        break;

    case 292: /* tuple_type_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3873 "ds_parser.cpp"
        break;

    case 293: /* variant_type  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3879 "ds_parser.cpp"
        break;

    case 294: /* variant_type_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3885 "ds_parser.cpp"
        break;

    case 296: /* variable_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3891 "ds_parser.cpp"
        break;

    case 299: /* let_variable_declaration  */
#line 145 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3897 "ds_parser.cpp"
        break;

    case 300: /* global_variable_declaration_list  */
#line 146 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3903 "ds_parser.cpp"
        break;

    case 305: /* enum_list  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3909 "ds_parser.cpp"
        break;

    case 311: /* optional_structure_parent  */
#line 142 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3915 "ds_parser.cpp"
        break;

    case 318: /* variable_name_with_pos_list  */
#line 144 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3921 "ds_parser.cpp"
        break;

    case 321: /* structure_type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3927 "ds_parser.cpp"
        break;

    case 322: /* auto_type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3933 "ds_parser.cpp"
        break;

    case 323: /* bitfield_bits  */
#line 143 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3939 "ds_parser.cpp"
        break;

    case 324: /* bitfield_type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3945 "ds_parser.cpp"
        break;

    case 327: /* type_declaration  */
#line 147 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3951 "ds_parser.cpp"
        break;

    case 356: /* make_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3957 "ds_parser.cpp"
        break;

    case 357: /* make_struct_fields  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3963 "ds_parser.cpp"
        break;

    case 358: /* make_struct_dim  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3969 "ds_parser.cpp"
        break;

    case 359: /* optional_block  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3975 "ds_parser.cpp"
        break;

    case 360: /* make_struct_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3981 "ds_parser.cpp"
        break;

    case 361: /* make_tuple  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3987 "ds_parser.cpp"
        break;

    case 362: /* make_map_tuple  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3993 "ds_parser.cpp"
        break;

    case 363: /* make_any_tuple  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3999 "ds_parser.cpp"
        break;

    case 364: /* make_dim  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4005 "ds_parser.cpp"
        break;

    case 365: /* make_dim_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4011 "ds_parser.cpp"
        break;

    case 366: /* make_table  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4017 "ds_parser.cpp"
        break;

    case 367: /* make_table_decl  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4023 "ds_parser.cpp"
        break;

    case 368: /* array_comprehension_where  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4029 "ds_parser.cpp"
        break;

    case 369: /* array_comprehension  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4035 "ds_parser.cpp"
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
#line 4327 "ds_parser.cpp"
    break;

  case 16:
#line 493 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4333 "ds_parser.cpp"
    break;

  case 17:
#line 494 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4339 "ds_parser.cpp"
    break;

  case 18:
#line 498 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4345 "ds_parser.cpp"
    break;

  case 19:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4351 "ds_parser.cpp"
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
#line 4370 "ds_parser.cpp"
    break;

  case 21:
#line 520 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4376 "ds_parser.cpp"
    break;

  case 22:
#line 521 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4382 "ds_parser.cpp"
    break;

  case 23:
#line 522 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4388 "ds_parser.cpp"
    break;

  case 24:
#line 523 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4394 "ds_parser.cpp"
    break;

  case 25:
#line 527 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4400 "ds_parser.cpp"
    break;

  case 26:
#line 531 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4410 "ds_parser.cpp"
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
#line 4424 "ds_parser.cpp"
    break;

  case 28:
#line 545 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4434 "ds_parser.cpp"
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
#line 4452 "ds_parser.cpp"
    break;

  case 30:
#line 569 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4462 "ds_parser.cpp"
    break;

  case 31:
#line 574 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4472 "ds_parser.cpp"
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
#line 4499 "ds_parser.cpp"
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
#line 4511 "ds_parser.cpp"
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
#line 4535 "ds_parser.cpp"
    break;

  case 36:
#line 638 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4543 "ds_parser.cpp"
    break;

  case 37:
#line 641 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4552 "ds_parser.cpp"
    break;

  case 38:
#line 645 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4563 "ds_parser.cpp"
    break;

  case 39:
#line 651 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4574 "ds_parser.cpp"
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
#line 4602 "ds_parser.cpp"
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
#line 4629 "ds_parser.cpp"
    break;

  case 42:
#line 708 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4635 "ds_parser.cpp"
    break;

  case 43:
#line 709 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4641 "ds_parser.cpp"
    break;

  case 47:
#line 722 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4649 "ds_parser.cpp"
    break;

  case 48:
#line 725 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4657 "ds_parser.cpp"
    break;

  case 49:
#line 731 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4665 "ds_parser.cpp"
    break;

  case 50:
#line 737 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4673 "ds_parser.cpp"
    break;

  case 51:
#line 740 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4681 "ds_parser.cpp"
    break;

  case 52:
#line 746 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4687 "ds_parser.cpp"
    break;

  case 53:
#line 747 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4693 "ds_parser.cpp"
    break;

  case 54:
#line 751 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4699 "ds_parser.cpp"
    break;

  case 55:
#line 752 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4705 "ds_parser.cpp"
    break;

  case 56:
#line 753 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4716 "ds_parser.cpp"
    break;

  case 57:
#line 762 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4722 "ds_parser.cpp"
    break;

  case 58:
#line 763 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4728 "ds_parser.cpp"
    break;

  case 59:
#line 767 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4739 "ds_parser.cpp"
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
#line 4757 "ds_parser.cpp"
    break;

  case 61:
#line 792 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4767 "ds_parser.cpp"
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
#line 4779 "ds_parser.cpp"
    break;

  case 63:
#line 810 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4790 "ds_parser.cpp"
    break;

  case 64:
#line 819 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4796 "ds_parser.cpp"
    break;

  case 65:
#line 820 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4802 "ds_parser.cpp"
    break;

  case 66:
#line 821 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4808 "ds_parser.cpp"
    break;

  case 67:
#line 822 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4814 "ds_parser.cpp"
    break;

  case 68:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4820 "ds_parser.cpp"
    break;

  case 69:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4826 "ds_parser.cpp"
    break;

  case 70:
#line 828 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4836 "ds_parser.cpp"
    break;

  case 71:
#line 833 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4846 "ds_parser.cpp"
    break;

  case 72:
#line 841 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4852 "ds_parser.cpp"
    break;

  case 73:
#line 842 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4858 "ds_parser.cpp"
    break;

  case 74:
#line 843 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4864 "ds_parser.cpp"
    break;

  case 75:
#line 847 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4870 "ds_parser.cpp"
    break;

  case 76:
#line 848 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4876 "ds_parser.cpp"
    break;

  case 77:
#line 849 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4882 "ds_parser.cpp"
    break;

  case 78:
#line 850 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4888 "ds_parser.cpp"
    break;

  case 79:
#line 851 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4894 "ds_parser.cpp"
    break;

  case 80:
#line 852 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4900 "ds_parser.cpp"
    break;

  case 81:
#line 853 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4906 "ds_parser.cpp"
    break;

  case 82:
#line 854 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4914 "ds_parser.cpp"
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
#line 4933 "ds_parser.cpp"
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
#line 4952 "ds_parser.cpp"
    break;

  case 85:
#line 891 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4958 "ds_parser.cpp"
    break;

  case 86:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4964 "ds_parser.cpp"
    break;

  case 87:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4970 "ds_parser.cpp"
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
#line 4983 "ds_parser.cpp"
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
#line 4998 "ds_parser.cpp"
    break;

  case 90:
#line 918 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5007 "ds_parser.cpp"
    break;

  case 91:
#line 922 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5016 "ds_parser.cpp"
    break;

  case 92:
#line 929 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 5022 "ds_parser.cpp"
    break;

  case 93:
#line 930 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 5028 "ds_parser.cpp"
    break;

  case 94:
#line 934 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5034 "ds_parser.cpp"
    break;

  case 95:
#line 935 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5040 "ds_parser.cpp"
    break;

  case 96:
#line 936 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5046 "ds_parser.cpp"
    break;

  case 97:
#line 940 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5054 "ds_parser.cpp"
    break;

  case 98:
#line 943 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5063 "ds_parser.cpp"
    break;

  case 99:
#line 950 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5072 "ds_parser.cpp"
    break;

  case 100:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5078 "ds_parser.cpp"
    break;

  case 101:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5084 "ds_parser.cpp"
    break;

  case 102:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5090 "ds_parser.cpp"
    break;

  case 103:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5096 "ds_parser.cpp"
    break;

  case 104:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5102 "ds_parser.cpp"
    break;

  case 105:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5108 "ds_parser.cpp"
    break;

  case 106:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5114 "ds_parser.cpp"
    break;

  case 107:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5120 "ds_parser.cpp"
    break;

  case 108:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5126 "ds_parser.cpp"
    break;

  case 109:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5132 "ds_parser.cpp"
    break;

  case 110:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5138 "ds_parser.cpp"
    break;

  case 111:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5144 "ds_parser.cpp"
    break;

  case 112:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5150 "ds_parser.cpp"
    break;

  case 113:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5156 "ds_parser.cpp"
    break;

  case 114:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5162 "ds_parser.cpp"
    break;

  case 115:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5168 "ds_parser.cpp"
    break;

  case 116:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5174 "ds_parser.cpp"
    break;

  case 117:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5180 "ds_parser.cpp"
    break;

  case 118:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5186 "ds_parser.cpp"
    break;

  case 119:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5192 "ds_parser.cpp"
    break;

  case 120:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5198 "ds_parser.cpp"
    break;

  case 121:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5204 "ds_parser.cpp"
    break;

  case 122:
#line 976 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5210 "ds_parser.cpp"
    break;

  case 123:
#line 977 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5216 "ds_parser.cpp"
    break;

  case 124:
#line 978 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5222 "ds_parser.cpp"
    break;

  case 125:
#line 979 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5228 "ds_parser.cpp"
    break;

  case 126:
#line 980 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5234 "ds_parser.cpp"
    break;

  case 127:
#line 981 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5240 "ds_parser.cpp"
    break;

  case 128:
#line 982 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5246 "ds_parser.cpp"
    break;

  case 129:
#line 983 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5252 "ds_parser.cpp"
    break;

  case 130:
#line 984 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5258 "ds_parser.cpp"
    break;

  case 131:
#line 985 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5264 "ds_parser.cpp"
    break;

  case 132:
#line 986 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5270 "ds_parser.cpp"
    break;

  case 133:
#line 987 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5276 "ds_parser.cpp"
    break;

  case 134:
#line 988 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5282 "ds_parser.cpp"
    break;

  case 135:
#line 989 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5288 "ds_parser.cpp"
    break;

  case 136:
#line 990 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5294 "ds_parser.cpp"
    break;

  case 137:
#line 991 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5300 "ds_parser.cpp"
    break;

  case 138:
#line 992 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5306 "ds_parser.cpp"
    break;

  case 139:
#line 993 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5312 "ds_parser.cpp"
    break;

  case 140:
#line 994 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5318 "ds_parser.cpp"
    break;

  case 141:
#line 995 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5324 "ds_parser.cpp"
    break;

  case 142:
#line 996 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5330 "ds_parser.cpp"
    break;

  case 143:
#line 997 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5336 "ds_parser.cpp"
    break;

  case 144:
#line 998 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5342 "ds_parser.cpp"
    break;

  case 145:
#line 999 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5348 "ds_parser.cpp"
    break;

  case 146:
#line 1000 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5354 "ds_parser.cpp"
    break;

  case 147:
#line 1001 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5360 "ds_parser.cpp"
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
#line 5384 "ds_parser.cpp"
    break;

  case 149:
#line 1027 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5390 "ds_parser.cpp"
    break;

  case 150:
#line 1028 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5396 "ds_parser.cpp"
    break;

  case 151:
#line 1029 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5402 "ds_parser.cpp"
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
#line 5442 "ds_parser.cpp"
    break;

  case 153:
#line 1071 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5452 "ds_parser.cpp"
    break;

  case 154:
#line 1079 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5461 "ds_parser.cpp"
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
#line 5474 "ds_parser.cpp"
    break;

  case 156:
#line 1094 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5480 "ds_parser.cpp"
    break;

  case 157:
#line 1095 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5486 "ds_parser.cpp"
    break;

  case 158:
#line 1096 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5492 "ds_parser.cpp"
    break;

  case 159:
#line 1097 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5498 "ds_parser.cpp"
    break;

  case 160:
#line 1098 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5504 "ds_parser.cpp"
    break;

  case 161:
#line 1099 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5510 "ds_parser.cpp"
    break;

  case 162:
#line 1100 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5516 "ds_parser.cpp"
    break;

  case 163:
#line 1101 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5522 "ds_parser.cpp"
    break;

  case 164:
#line 1102 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5528 "ds_parser.cpp"
    break;

  case 165:
#line 1103 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5534 "ds_parser.cpp"
    break;

  case 166:
#line 1104 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5540 "ds_parser.cpp"
    break;

  case 167:
#line 1105 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5546 "ds_parser.cpp"
    break;

  case 168:
#line 1106 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5552 "ds_parser.cpp"
    break;

  case 169:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5558 "ds_parser.cpp"
    break;

  case 170:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5564 "ds_parser.cpp"
    break;

  case 171:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5570 "ds_parser.cpp"
    break;

  case 172:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5576 "ds_parser.cpp"
    break;

  case 173:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5582 "ds_parser.cpp"
    break;

  case 174:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5588 "ds_parser.cpp"
    break;

  case 175:
#line 1116 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5598 "ds_parser.cpp"
    break;

  case 176:
#line 1121 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5609 "ds_parser.cpp"
    break;

  case 177:
#line 1127 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5617 "ds_parser.cpp"
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
#line 5656 "ds_parser.cpp"
    break;

  case 179:
#line 1167 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5664 "ds_parser.cpp"
    break;

  case 180:
#line 1170 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5672 "ds_parser.cpp"
    break;

  case 181:
#line 1176 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5678 "ds_parser.cpp"
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
#line 5694 "ds_parser.cpp"
    break;

  case 183:
#line 1188 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5700 "ds_parser.cpp"
    break;

  case 184:
#line 1192 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5708 "ds_parser.cpp"
    break;

  case 185:
#line 1198 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5716 "ds_parser.cpp"
    break;

  case 186:
#line 1201 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5724 "ds_parser.cpp"
    break;

  case 187:
#line 1204 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5733 "ds_parser.cpp"
    break;

  case 188:
#line 1208 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5741 "ds_parser.cpp"
    break;

  case 189:
#line 1214 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5747 "ds_parser.cpp"
    break;

  case 190:
#line 1218 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5753 "ds_parser.cpp"
    break;

  case 191:
#line 1222 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5761 "ds_parser.cpp"
    break;

  case 192:
#line 1225 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5769 "ds_parser.cpp"
    break;

  case 193:
#line 1228 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5779 "ds_parser.cpp"
    break;

  case 194:
#line 1233 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5787 "ds_parser.cpp"
    break;

  case 195:
#line 1236 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5797 "ds_parser.cpp"
    break;

  case 196:
#line 1244 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5805 "ds_parser.cpp"
    break;

  case 197:
#line 1247 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5815 "ds_parser.cpp"
    break;

  case 198:
#line 1252 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5823 "ds_parser.cpp"
    break;

  case 199:
#line 1255 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5833 "ds_parser.cpp"
    break;

  case 200:
#line 1263 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5841 "ds_parser.cpp"
    break;

  case 201:
#line 1269 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5847 "ds_parser.cpp"
    break;

  case 202:
#line 1270 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5853 "ds_parser.cpp"
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
#line 5889 "ds_parser.cpp"
    break;

  case 204:
#line 1308 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5895 "ds_parser.cpp"
    break;

  case 205:
#line 1308 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5901 "ds_parser.cpp"
    break;

  case 206:
#line 1308 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5909 "ds_parser.cpp"
    break;

  case 207:
#line 1311 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5915 "ds_parser.cpp"
    break;

  case 208:
#line 1311 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5921 "ds_parser.cpp"
    break;

  case 209:
#line 1311 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5931 "ds_parser.cpp"
    break;

  case 210:
#line 1316 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5937 "ds_parser.cpp"
    break;

  case 211:
#line 1316 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5943 "ds_parser.cpp"
    break;

  case 212:
#line 1316 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5953 "ds_parser.cpp"
    break;

  case 213:
#line 1324 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5959 "ds_parser.cpp"
    break;

  case 214:
#line 1324 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5965 "ds_parser.cpp"
    break;

  case 215:
#line 1324 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5973 "ds_parser.cpp"
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
#line 5988 "ds_parser.cpp"
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
#line 6004 "ds_parser.cpp"
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
#line 6021 "ds_parser.cpp"
    break;

  case 219:
#line 1366 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6029 "ds_parser.cpp"
    break;

  case 220:
#line 1369 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6037 "ds_parser.cpp"
    break;

  case 221:
#line 1375 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6043 "ds_parser.cpp"
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
#line 6055 "ds_parser.cpp"
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
#line 6068 "ds_parser.cpp"
    break;

  case 224:
#line 1394 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6074 "ds_parser.cpp"
    break;

  case 225:
#line 1395 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6080 "ds_parser.cpp"
    break;

  case 226:
#line 1396 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6086 "ds_parser.cpp"
    break;

  case 227:
#line 1400 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6092 "ds_parser.cpp"
    break;

  case 228:
#line 1401 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6098 "ds_parser.cpp"
    break;

  case 229:
#line 1402 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6104 "ds_parser.cpp"
    break;

  case 230:
#line 1403 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6110 "ds_parser.cpp"
    break;

  case 231:
#line 1407 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6120 "ds_parser.cpp"
    break;

  case 232:
#line 1412 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6130 "ds_parser.cpp"
    break;

  case 233:
#line 1420 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6136 "ds_parser.cpp"
    break;

  case 234:
#line 1421 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6142 "ds_parser.cpp"
    break;

  case 235:
#line 1425 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6152 "ds_parser.cpp"
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
#line 6217 "ds_parser.cpp"
    break;

  case 237:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6223 "ds_parser.cpp"
    break;

  case 238:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6229 "ds_parser.cpp"
    break;

  case 239:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6235 "ds_parser.cpp"
    break;

  case 240:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6241 "ds_parser.cpp"
    break;

  case 241:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6247 "ds_parser.cpp"
    break;

  case 242:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6253 "ds_parser.cpp"
    break;

  case 243:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6259 "ds_parser.cpp"
    break;

  case 244:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6265 "ds_parser.cpp"
    break;

  case 245:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6271 "ds_parser.cpp"
    break;

  case 246:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6277 "ds_parser.cpp"
    break;

  case 247:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6283 "ds_parser.cpp"
    break;

  case 248:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6289 "ds_parser.cpp"
    break;

  case 249:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6295 "ds_parser.cpp"
    break;

  case 250:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6301 "ds_parser.cpp"
    break;

  case 251:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6307 "ds_parser.cpp"
    break;

  case 252:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp"
    break;

  case 253:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6319 "ds_parser.cpp"
    break;

  case 254:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6325 "ds_parser.cpp"
    break;

  case 255:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6331 "ds_parser.cpp"
    break;

  case 256:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp"
    break;

  case 257:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6343 "ds_parser.cpp"
    break;

  case 258:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6349 "ds_parser.cpp"
    break;

  case 259:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6355 "ds_parser.cpp"
    break;

  case 260:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6361 "ds_parser.cpp"
    break;

  case 261:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6367 "ds_parser.cpp"
    break;

  case 262:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6373 "ds_parser.cpp"
    break;

  case 263:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6379 "ds_parser.cpp"
    break;

  case 264:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6385 "ds_parser.cpp"
    break;

  case 265:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6391 "ds_parser.cpp"
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
#line 6403 "ds_parser.cpp"
    break;

  case 267:
#line 1543 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6413 "ds_parser.cpp"
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
#line 6425 "ds_parser.cpp"
    break;

  case 269:
#line 1558 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6434 "ds_parser.cpp"
    break;

  case 270:
#line 1562 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6440 "ds_parser.cpp"
    break;

  case 271:
#line 1562 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6446 "ds_parser.cpp"
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
#line 6458 "ds_parser.cpp"
    break;

  case 273:
#line 1569 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6464 "ds_parser.cpp"
    break;

  case 274:
#line 1569 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6470 "ds_parser.cpp"
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
#line 6486 "ds_parser.cpp"
    break;

  case 276:
#line 1583 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6495 "ds_parser.cpp"
    break;

  case 277:
#line 1587 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6504 "ds_parser.cpp"
    break;

  case 278:
#line 1591 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6510 "ds_parser.cpp"
    break;

  case 279:
#line 1591 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6516 "ds_parser.cpp"
    break;

  case 280:
#line 1591 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6525 "ds_parser.cpp"
    break;

  case 281:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6531 "ds_parser.cpp"
    break;

  case 282:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6537 "ds_parser.cpp"
    break;

  case 283:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6543 "ds_parser.cpp"
    break;

  case 284:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6549 "ds_parser.cpp"
    break;

  case 285:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6555 "ds_parser.cpp"
    break;

  case 286:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6561 "ds_parser.cpp"
    break;

  case 287:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6567 "ds_parser.cpp"
    break;

  case 288:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6573 "ds_parser.cpp"
    break;

  case 289:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6579 "ds_parser.cpp"
    break;

  case 290:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 291:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6591 "ds_parser.cpp"
    break;

  case 292:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 293:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 294:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 295:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 296:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6621 "ds_parser.cpp"
    break;

  case 297:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6627 "ds_parser.cpp"
    break;

  case 298:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6633 "ds_parser.cpp"
    break;

  case 299:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6639 "ds_parser.cpp"
    break;

  case 300:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6645 "ds_parser.cpp"
    break;

  case 301:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6651 "ds_parser.cpp"
    break;

  case 302:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6657 "ds_parser.cpp"
    break;

  case 303:
#line 1620 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6663 "ds_parser.cpp"
    break;

  case 304:
#line 1621 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6669 "ds_parser.cpp"
    break;

  case 305:
#line 1622 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6675 "ds_parser.cpp"
    break;

  case 306:
#line 1623 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6681 "ds_parser.cpp"
    break;

  case 307:
#line 1624 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6687 "ds_parser.cpp"
    break;

  case 308:
#line 1625 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6693 "ds_parser.cpp"
    break;

  case 309:
#line 1626 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6699 "ds_parser.cpp"
    break;

  case 310:
#line 1627 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6705 "ds_parser.cpp"
    break;

  case 311:
#line 1628 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6711 "ds_parser.cpp"
    break;

  case 312:
#line 1629 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6717 "ds_parser.cpp"
    break;

  case 313:
#line 1630 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6723 "ds_parser.cpp"
    break;

  case 314:
#line 1631 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6729 "ds_parser.cpp"
    break;

  case 315:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6735 "ds_parser.cpp"
    break;

  case 316:
#line 1633 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6741 "ds_parser.cpp"
    break;

  case 317:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6747 "ds_parser.cpp"
    break;

  case 318:
#line 1635 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6753 "ds_parser.cpp"
    break;

  case 319:
#line 1636 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6759 "ds_parser.cpp"
    break;

  case 320:
#line 1637 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6765 "ds_parser.cpp"
    break;

  case 321:
#line 1638 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6771 "ds_parser.cpp"
    break;

  case 322:
#line 1639 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6777 "ds_parser.cpp"
    break;

  case 323:
#line 1640 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6783 "ds_parser.cpp"
    break;

  case 324:
#line 1641 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6789 "ds_parser.cpp"
    break;

  case 325:
#line 1642 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6795 "ds_parser.cpp"
    break;

  case 326:
#line 1643 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6801 "ds_parser.cpp"
    break;

  case 327:
#line 1644 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6810 "ds_parser.cpp"
    break;

  case 328:
#line 1648 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6819 "ds_parser.cpp"
    break;

  case 329:
#line 1652 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6827 "ds_parser.cpp"
    break;

  case 330:
#line 1655 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6835 "ds_parser.cpp"
    break;

  case 331:
#line 1658 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6841 "ds_parser.cpp"
    break;

  case 332:
#line 1659 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6847 "ds_parser.cpp"
    break;

  case 333:
#line 1660 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6853 "ds_parser.cpp"
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
#line 6867 "ds_parser.cpp"
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
#line 6881 "ds_parser.cpp"
    break;

  case 336:
#line 1679 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6887 "ds_parser.cpp"
    break;

  case 337:
#line 1680 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6895 "ds_parser.cpp"
    break;

  case 338:
#line 1683 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6901 "ds_parser.cpp"
    break;

  case 339:
#line 1683 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6907 "ds_parser.cpp"
    break;

  case 340:
#line 1683 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6915 "ds_parser.cpp"
    break;

  case 341:
#line 1686 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6924 "ds_parser.cpp"
    break;

  case 342:
#line 1690 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6933 "ds_parser.cpp"
    break;

  case 343:
#line 1694 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6942 "ds_parser.cpp"
    break;

  case 344:
#line 1698 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6948 "ds_parser.cpp"
    break;

  case 345:
#line 1699 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6954 "ds_parser.cpp"
    break;

  case 346:
#line 1700 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6960 "ds_parser.cpp"
    break;

  case 347:
#line 1701 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6966 "ds_parser.cpp"
    break;

  case 348:
#line 1702 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6972 "ds_parser.cpp"
    break;

  case 349:
#line 1703 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6978 "ds_parser.cpp"
    break;

  case 350:
#line 1704 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6984 "ds_parser.cpp"
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
#line 7005 "ds_parser.cpp"
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
#line 7026 "ds_parser.cpp"
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
#line 7086 "ds_parser.cpp"
    break;

  case 354:
#line 1795 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7092 "ds_parser.cpp"
    break;

  case 355:
#line 1796 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7098 "ds_parser.cpp"
    break;

  case 356:
#line 1800 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_NONE; }
#line 7104 "ds_parser.cpp"
    break;

  case 357:
#line 1801 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_OVERRIDE; }
#line 7110 "ds_parser.cpp"
    break;

  case 358:
#line 1802 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_SEALED; }
#line 7116 "ds_parser.cpp"
    break;

  case 359:
#line 1806 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7122 "ds_parser.cpp"
    break;

  case 360:
#line 1807 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7128 "ds_parser.cpp"
    break;

  case 361:
#line 1811 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7134 "ds_parser.cpp"
    break;

  case 362:
#line 1812 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7140 "ds_parser.cpp"
    break;

  case 363:
#line 1813 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7146 "ds_parser.cpp"
    break;

  case 364:
#line 1817 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7158 "ds_parser.cpp"
    break;

  case 365:
#line 1827 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7166 "ds_parser.cpp"
    break;

  case 366:
#line 1830 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7175 "ds_parser.cpp"
    break;

  case 367:
#line 1835 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7181 "ds_parser.cpp"
    break;

  case 368:
#line 1835 "ds_parser.ypp"
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
#line 7231 "ds_parser.cpp"
    break;

  case 369:
#line 1882 "ds_parser.ypp"
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
#line 7300 "ds_parser.cpp"
    break;

  case 370:
#line 1946 "ds_parser.ypp"
    {
        das_yyerror("structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7311 "ds_parser.cpp"
    break;

  case 371:
#line 1955 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7321 "ds_parser.cpp"
    break;

  case 372:
#line 1960 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7331 "ds_parser.cpp"
    break;

  case 373:
#line 1968 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7337 "ds_parser.cpp"
    break;

  case 374:
#line 1969 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7343 "ds_parser.cpp"
    break;

  case 375:
#line 1973 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7351 "ds_parser.cpp"
    break;

  case 376:
#line 1976 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7362 "ds_parser.cpp"
    break;

  case 377:
#line 1985 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7368 "ds_parser.cpp"
    break;

  case 378:
#line 1986 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7374 "ds_parser.cpp"
    break;

  case 379:
#line 1990 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7385 "ds_parser.cpp"
    break;

  case 380:
#line 1999 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7391 "ds_parser.cpp"
    break;

  case 381:
#line 2000 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7397 "ds_parser.cpp"
    break;

  case 382:
#line 2005 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7403 "ds_parser.cpp"
    break;

  case 383:
#line 2006 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7409 "ds_parser.cpp"
    break;

  case 384:
#line 2010 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7420 "ds_parser.cpp"
    break;

  case 385:
#line 2016 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7431 "ds_parser.cpp"
    break;

  case 386:
#line 2022 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7439 "ds_parser.cpp"
    break;

  case 387:
#line 2025 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7448 "ds_parser.cpp"
    break;

  case 388:
#line 2029 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7459 "ds_parser.cpp"
    break;

  case 389:
#line 2035 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7470 "ds_parser.cpp"
    break;

  case 390:
#line 2044 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7476 "ds_parser.cpp"
    break;

  case 391:
#line 2045 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7482 "ds_parser.cpp"
    break;

  case 392:
#line 2046 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7488 "ds_parser.cpp"
    break;

  case 393:
#line 2050 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7494 "ds_parser.cpp"
    break;

  case 394:
#line 2051 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7500 "ds_parser.cpp"
    break;

  case 395:
#line 2055 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7508 "ds_parser.cpp"
    break;

  case 396:
#line 2058 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7518 "ds_parser.cpp"
    break;

  case 397:
#line 2063 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7528 "ds_parser.cpp"
    break;

  case 398:
#line 2068 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7541 "ds_parser.cpp"
    break;

  case 399:
#line 2076 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7554 "ds_parser.cpp"
    break;

  case 400:
#line 2087 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7562 "ds_parser.cpp"
    break;

  case 401:
#line 2090 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7572 "ds_parser.cpp"
    break;

  case 402:
#line 2098 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7578 "ds_parser.cpp"
    break;

  case 403:
#line 2099 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7584 "ds_parser.cpp"
    break;

  case 404:
#line 2103 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7590 "ds_parser.cpp"
    break;

  case 405:
#line 2104 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7596 "ds_parser.cpp"
    break;

  case 406:
#line 2105 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7602 "ds_parser.cpp"
    break;

  case 407:
#line 2109 "ds_parser.ypp"
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
#line 7635 "ds_parser.cpp"
    break;

  case 408:
#line 2137 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7641 "ds_parser.cpp"
    break;

  case 409:
#line 2137 "ds_parser.ypp"
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
#line 7677 "ds_parser.cpp"
    break;

  case 410:
#line 2171 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7685 "ds_parser.cpp"
    break;

  case 411:
#line 2174 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7699 "ds_parser.cpp"
    break;

  case 412:
#line 2183 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7713 "ds_parser.cpp"
    break;

  case 413:
#line 2196 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7727 "ds_parser.cpp"
    break;

  case 418:
#line 2218 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7733 "ds_parser.cpp"
    break;

  case 419:
#line 2219 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7739 "ds_parser.cpp"
    break;

  case 420:
#line 2220 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7745 "ds_parser.cpp"
    break;

  case 421:
#line 2224 "ds_parser.ypp"
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
#line 7778 "ds_parser.cpp"
    break;

  case 422:
#line 2252 "ds_parser.ypp"
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
#line 7812 "ds_parser.cpp"
    break;

  case 423:
#line 2284 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7818 "ds_parser.cpp"
    break;

  case 424:
#line 2285 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7824 "ds_parser.cpp"
    break;

  case 425:
#line 2289 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7830 "ds_parser.cpp"
    break;

  case 426:
#line 2290 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7836 "ds_parser.cpp"
    break;

  case 427:
#line 2294 "ds_parser.ypp"
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
#line 7882 "ds_parser.cpp"
    break;

  case 428:
#line 2338 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7888 "ds_parser.cpp"
    break;

  case 429:
#line 2339 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7894 "ds_parser.cpp"
    break;

  case 430:
#line 2343 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7900 "ds_parser.cpp"
    break;

  case 431:
#line 2344 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7906 "ds_parser.cpp"
    break;

  case 432:
#line 2345 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7912 "ds_parser.cpp"
    break;

  case 433:
#line 2350 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7918 "ds_parser.cpp"
    break;

  case 434:
#line 2350 "ds_parser.ypp"
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
#line 8016 "ds_parser.cpp"
    break;

  case 435:
#line 2446 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8028 "ds_parser.cpp"
    break;

  case 436:
#line 2453 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 8039 "ds_parser.cpp"
    break;

  case 437:
#line 2462 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 8045 "ds_parser.cpp"
    break;

  case 438:
#line 2463 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 8051 "ds_parser.cpp"
    break;

  case 439:
#line 2464 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8057 "ds_parser.cpp"
    break;

  case 440:
#line 2465 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8063 "ds_parser.cpp"
    break;

  case 441:
#line 2466 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8069 "ds_parser.cpp"
    break;

  case 442:
#line 2467 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 8075 "ds_parser.cpp"
    break;

  case 443:
#line 2468 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 8081 "ds_parser.cpp"
    break;

  case 444:
#line 2469 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 8087 "ds_parser.cpp"
    break;

  case 445:
#line 2470 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8093 "ds_parser.cpp"
    break;

  case 446:
#line 2471 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8099 "ds_parser.cpp"
    break;

  case 447:
#line 2472 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8105 "ds_parser.cpp"
    break;

  case 448:
#line 2473 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8111 "ds_parser.cpp"
    break;

  case 449:
#line 2474 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8117 "ds_parser.cpp"
    break;

  case 450:
#line 2475 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8123 "ds_parser.cpp"
    break;

  case 451:
#line 2476 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8129 "ds_parser.cpp"
    break;

  case 452:
#line 2477 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8135 "ds_parser.cpp"
    break;

  case 453:
#line 2478 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8141 "ds_parser.cpp"
    break;

  case 454:
#line 2479 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8147 "ds_parser.cpp"
    break;

  case 455:
#line 2480 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8153 "ds_parser.cpp"
    break;

  case 456:
#line 2481 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8159 "ds_parser.cpp"
    break;

  case 457:
#line 2482 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8165 "ds_parser.cpp"
    break;

  case 458:
#line 2483 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8171 "ds_parser.cpp"
    break;

  case 459:
#line 2484 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8177 "ds_parser.cpp"
    break;

  case 460:
#line 2485 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8183 "ds_parser.cpp"
    break;

  case 461:
#line 2486 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8189 "ds_parser.cpp"
    break;

  case 462:
#line 2490 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8195 "ds_parser.cpp"
    break;

  case 463:
#line 2491 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8201 "ds_parser.cpp"
    break;

  case 464:
#line 2492 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8207 "ds_parser.cpp"
    break;

  case 465:
#line 2493 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8213 "ds_parser.cpp"
    break;

  case 466:
#line 2494 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8219 "ds_parser.cpp"
    break;

  case 467:
#line 2495 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8225 "ds_parser.cpp"
    break;

  case 468:
#line 2499 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8238 "ds_parser.cpp"
    break;

  case 469:
#line 2510 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8247 "ds_parser.cpp"
    break;

  case 470:
#line 2514 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8259 "ds_parser.cpp"
    break;

  case 471:
#line 2524 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8271 "ds_parser.cpp"
    break;

  case 472:
#line 2531 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8282 "ds_parser.cpp"
    break;

  case 473:
#line 2540 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8288 "ds_parser.cpp"
    break;

  case 474:
#line 2540 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8294 "ds_parser.cpp"
    break;

  case 475:
#line 2540 "ds_parser.ypp"
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
#line 8309 "ds_parser.cpp"
    break;

  case 476:
#line 2553 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8315 "ds_parser.cpp"
    break;

  case 477:
#line 2554 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8321 "ds_parser.cpp"
    break;

  case 478:
#line 2555 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8327 "ds_parser.cpp"
    break;

  case 479:
#line 2556 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8333 "ds_parser.cpp"
    break;

  case 480:
#line 2557 "ds_parser.ypp"
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
#line 8352 "ds_parser.cpp"
    break;

  case 481:
#line 2571 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8363 "ds_parser.cpp"
    break;

  case 482:
#line 2577 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8372 "ds_parser.cpp"
    break;

  case 483:
#line 2581 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8381 "ds_parser.cpp"
    break;

  case 484:
#line 2585 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8391 "ds_parser.cpp"
    break;

  case 485:
#line 2590 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8401 "ds_parser.cpp"
    break;

  case 486:
#line 2595 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8411 "ds_parser.cpp"
    break;

  case 487:
#line 2600 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8421 "ds_parser.cpp"
    break;

  case 488:
#line 2605 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8430 "ds_parser.cpp"
    break;

  case 489:
#line 2609 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8439 "ds_parser.cpp"
    break;

  case 490:
#line 2613 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8449 "ds_parser.cpp"
    break;

  case 491:
#line 2618 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8458 "ds_parser.cpp"
    break;

  case 492:
#line 2622 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8468 "ds_parser.cpp"
    break;

  case 493:
#line 2627 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8474 "ds_parser.cpp"
    break;

  case 494:
#line 2627 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8480 "ds_parser.cpp"
    break;

  case 495:
#line 2627 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8491 "ds_parser.cpp"
    break;

  case 496:
#line 2633 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8503 "ds_parser.cpp"
    break;

  case 497:
#line 2640 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8509 "ds_parser.cpp"
    break;

  case 498:
#line 2640 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8515 "ds_parser.cpp"
    break;

  case 499:
#line 2640 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8525 "ds_parser.cpp"
    break;

  case 500:
#line 2645 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8531 "ds_parser.cpp"
    break;

  case 501:
#line 2645 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8537 "ds_parser.cpp"
    break;

  case 502:
#line 2645 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8548 "ds_parser.cpp"
    break;

  case 503:
#line 2651 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8554 "ds_parser.cpp"
    break;

  case 504:
#line 2651 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8560 "ds_parser.cpp"
    break;

  case 505:
#line 2651 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8570 "ds_parser.cpp"
    break;

  case 506:
#line 2656 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8579 "ds_parser.cpp"
    break;

  case 507:
#line 2660 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8585 "ds_parser.cpp"
    break;

  case 508:
#line 2660 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8591 "ds_parser.cpp"
    break;

  case 509:
#line 2660 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8601 "ds_parser.cpp"
    break;

  case 510:
#line 2665 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8607 "ds_parser.cpp"
    break;

  case 511:
#line 2665 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8613 "ds_parser.cpp"
    break;

  case 512:
#line 2665 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8627 "ds_parser.cpp"
    break;

  case 513:
#line 2674 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8636 "ds_parser.cpp"
    break;

  case 514:
#line 2678 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8642 "ds_parser.cpp"
    break;

  case 515:
#line 2678 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8648 "ds_parser.cpp"
    break;

  case 516:
#line 2678 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8658 "ds_parser.cpp"
    break;

  case 517:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8664 "ds_parser.cpp"
    break;

  case 518:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8670 "ds_parser.cpp"
    break;

  case 519:
#line 2683 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8684 "ds_parser.cpp"
    break;

  case 520:
#line 2692 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8693 "ds_parser.cpp"
    break;

  case 521:
#line 2696 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8699 "ds_parser.cpp"
    break;

  case 522:
#line 2696 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8705 "ds_parser.cpp"
    break;

  case 523:
#line 2696 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8715 "ds_parser.cpp"
    break;

  case 524:
#line 2701 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8721 "ds_parser.cpp"
    break;

  case 525:
#line 2701 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8727 "ds_parser.cpp"
    break;

  case 526:
#line 2701 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8741 "ds_parser.cpp"
    break;

  case 527:
#line 2710 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8747 "ds_parser.cpp"
    break;

  case 528:
#line 2710 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8753 "ds_parser.cpp"
    break;

  case 529:
#line 2710 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8764 "ds_parser.cpp"
    break;

  case 530:
#line 2716 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8770 "ds_parser.cpp"
    break;

  case 531:
#line 2716 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8776 "ds_parser.cpp"
    break;

  case 532:
#line 2716 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8787 "ds_parser.cpp"
    break;

  case 533:
#line 2725 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8793 "ds_parser.cpp"
    break;

  case 534:
#line 2725 "ds_parser.ypp"
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
#line 8810 "ds_parser.cpp"
    break;

  case 535:
#line 2740 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8816 "ds_parser.cpp"
    break;

  case 536:
#line 2740 "ds_parser.ypp"
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
#line 8837 "ds_parser.cpp"
    break;

  case 537:
#line 2760 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8843 "ds_parser.cpp"
    break;

  case 538:
#line 2761 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8849 "ds_parser.cpp"
    break;

  case 539:
#line 2762 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8855 "ds_parser.cpp"
    break;

  case 540:
#line 2763 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8861 "ds_parser.cpp"
    break;

  case 541:
#line 2767 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8873 "ds_parser.cpp"
    break;

  case 542:
#line 2774 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8885 "ds_parser.cpp"
    break;

  case 543:
#line 2781 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8896 "ds_parser.cpp"
    break;

  case 544:
#line 2787 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8907 "ds_parser.cpp"
    break;

  case 545:
#line 2796 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8917 "ds_parser.cpp"
    break;

  case 546:
#line 2801 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8926 "ds_parser.cpp"
    break;

  case 547:
#line 2808 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8932 "ds_parser.cpp"
    break;

  case 548:
#line 2809 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8938 "ds_parser.cpp"
    break;

  case 549:
#line 2813 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8949 "ds_parser.cpp"
    break;

  case 550:
#line 2819 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8961 "ds_parser.cpp"
    break;

  case 551:
#line 2826 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8974 "ds_parser.cpp"
    break;

  case 552:
#line 2834 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8986 "ds_parser.cpp"
    break;

  case 553:
#line 2841 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8999 "ds_parser.cpp"
    break;

  case 554:
#line 2849 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9013 "ds_parser.cpp"
    break;

  case 555:
#line 2861 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 9021 "ds_parser.cpp"
    break;

  case 556:
#line 2864 "ds_parser.ypp"
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
#line 9037 "ds_parser.cpp"
    break;

  case 557:
#line 2878 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9048 "ds_parser.cpp"
    break;

  case 558:
#line 2887 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9054 "ds_parser.cpp"
    break;

  case 559:
#line 2888 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9060 "ds_parser.cpp"
    break;

  case 560:
#line 2892 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9070 "ds_parser.cpp"
    break;

  case 561:
#line 2897 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9079 "ds_parser.cpp"
    break;

  case 562:
#line 2904 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9089 "ds_parser.cpp"
    break;

  case 563:
#line 2909 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9101 "ds_parser.cpp"
    break;

  case 564:
#line 2919 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9111 "ds_parser.cpp"
    break;

  case 565:
#line 2924 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9120 "ds_parser.cpp"
    break;

  case 566:
#line 2931 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9134 "ds_parser.cpp"
    break;

  case 567:
#line 2943 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9140 "ds_parser.cpp"
    break;

  case 568:
#line 2944 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9146 "ds_parser.cpp"
    break;

  case 569:
#line 2948 "ds_parser.ypp"
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
#line 9169 "ds_parser.cpp"
    break;

  case 570:
#line 2966 "ds_parser.ypp"
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
#line 9192 "ds_parser.cpp"
    break;


#line 9196 "ds_parser.cpp"

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
#line 2986 "ds_parser.ypp"


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
