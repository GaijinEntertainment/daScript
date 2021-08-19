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
    LFPIPE = 376,
    RPIPE = 377,
    CLONEEQU = 378,
    ROTL = 379,
    ROTR = 380,
    ROTLEQU = 381,
    ROTREQU = 382,
    MAPTO = 383,
    COLCOL = 384,
    ANDAND = 385,
    OROR = 386,
    XORXOR = 387,
    ANDANDEQU = 388,
    OROREQU = 389,
    XORXOREQU = 390,
    BRABRAB = 391,
    BRACBRB = 392,
    CBRCBRB = 393,
    INTEGER = 394,
    LONG_INTEGER = 395,
    UNSIGNED_INTEGER = 396,
    UNSIGNED_LONG_INTEGER = 397,
    FLOAT = 398,
    DOUBLE = 399,
    NAME = 400,
    BEGIN_STRING = 401,
    STRING_CHARACTER = 402,
    STRING_CHARACTER_ESC = 403,
    END_STRING = 404,
    BEGIN_STRING_EXPR = 405,
    END_STRING_EXPR = 406,
    END_OF_READ = 407,
    UNARY_MINUS = 408,
    UNARY_PLUS = 409,
    PRE_INC = 410,
    PRE_DEC = 411,
    POST_INC = 412,
    POST_DEC = 413,
    DEREF = 414
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
    CaptureEntry *                  pCapt;
    vector<CaptureEntry> *          pCaptList;

#line 409 "ds_parser.cpp"

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
#define YYLAST   7343

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  178
/* YYNRULES -- Number of rules.  */
#define YYNRULES  547
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  956

#define YYUNDEFTOK  2
#define YYMAXUTOK   414

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
       2,     2,     2,   170,     2,   186,   181,   166,   159,     2,
     179,   180,   164,   163,   153,   162,   176,   165,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   156,   183,
     160,   154,   161,   155,   182,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   177,     2,   178,   158,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   184,   157,   185,   169,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   151,   152,   167,   168,
     171,   172,   173,   174,   175
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   452,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   468,   469,   470,   474,   475,
     479,   496,   497,   498,   499,   503,   507,   512,   521,   529,
     545,   550,   558,   558,   588,   610,   614,   617,   623,   632,
     653,   676,   677,   681,   685,   686,   690,   693,   699,   705,
     708,   714,   715,   719,   720,   721,   730,   731,   735,   744,
     760,   768,   778,   787,   788,   789,   790,   791,   792,   796,
     801,   809,   810,   811,   812,   813,   814,   815,   816,   822,
     836,   853,   854,   855,   859,   867,   880,   884,   891,   892,
     896,   897,   898,   902,   905,   912,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   956,   957,   961,   983,
     984,   985,   989,  1028,  1035,  1039,  1050,  1051,  1052,  1053,
    1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,  1067,  1068,  1072,  1077,  1083,  1089,  1123,
    1126,  1132,  1133,  1144,  1148,  1154,  1157,  1160,  1164,  1170,
    1174,  1178,  1181,  1184,  1189,  1192,  1200,  1203,  1208,  1211,
    1219,  1225,  1226,  1230,  1264,  1264,  1264,  1267,  1267,  1267,
    1272,  1272,  1272,  1280,  1280,  1280,  1286,  1296,  1307,  1322,
    1325,  1331,  1332,  1339,  1350,  1351,  1352,  1356,  1357,  1358,
    1359,  1363,  1368,  1376,  1377,  1381,  1386,  1450,  1451,  1452,
    1453,  1454,  1455,  1459,  1460,  1461,  1462,  1463,  1464,  1465,
    1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,
    1476,  1477,  1481,  1482,  1483,  1484,  1488,  1499,  1504,  1514,
    1518,  1518,  1518,  1525,  1525,  1525,  1539,  1543,  1543,  1543,
    1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,
    1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,
    1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,
    1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1597,  1601,  1604,  1607,  1608,  1609,
    1610,  1619,  1628,  1629,  1632,  1632,  1632,  1635,  1639,  1643,
    1647,  1648,  1649,  1650,  1651,  1652,  1653,  1654,  1670,  1686,
    1744,  1745,  1749,  1750,  1754,  1762,  1765,  1770,  1769,  1814,
    1870,  1879,  1884,  1892,  1893,  1897,  1900,  1909,  1910,  1914,
    1923,  1924,  1929,  1930,  1934,  1940,  1946,  1949,  1953,  1959,
    1968,  1969,  1970,  1974,  1975,  1979,  1982,  1987,  1992,  2000,
    2011,  2014,  2021,  2022,  2026,  2027,  2028,  2032,  2060,  2060,
    2090,  2093,  2102,  2115,  2127,  2128,  2132,  2136,  2137,  2138,
    2142,  2170,  2202,  2203,  2207,  2244,  2245,  2249,  2250,  2251,
    2256,  2255,  2340,  2347,  2356,  2357,  2358,  2359,  2360,  2361,
    2362,  2363,  2364,  2365,  2366,  2367,  2368,  2369,  2370,  2371,
    2372,  2373,  2374,  2375,  2376,  2377,  2378,  2379,  2380,  2384,
    2385,  2386,  2387,  2388,  2389,  2393,  2404,  2408,  2418,  2425,
    2434,  2434,  2434,  2447,  2448,  2449,  2450,  2451,  2465,  2471,
    2475,  2479,  2484,  2489,  2494,  2499,  2503,  2507,  2512,  2516,
    2521,  2521,  2521,  2527,  2534,  2534,  2534,  2539,  2539,  2539,
    2545,  2545,  2545,  2550,  2554,  2554,  2554,  2559,  2559,  2559,
    2568,  2572,  2572,  2572,  2577,  2577,  2577,  2586,  2590,  2590,
    2590,  2595,  2595,  2595,  2604,  2604,  2604,  2610,  2610,  2610,
    2619,  2619,  2634,  2634,  2654,  2655,  2656,  2657,  2661,  2668,
    2675,  2681,  2690,  2695,  2702,  2703,  2707,  2713,  2720,  2728,
    2735,  2743,  2755,  2758,  2772,  2781,  2782,  2786,  2791,  2798,
    2803,  2813,  2818,  2825,  2837,  2838,  2842,  2860
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
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"+=\"",
  "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"",
  "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"",
  "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
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
  "annotation_argument_value_list", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
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
  "optional_field_annotation", "optional_override",
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
     405,   406,   407,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   408,   409,   126,
      33,   410,   411,   412,   413,   414,    46,    91,    93,    40,
      41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -599

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-599)))

#define YYTABLE_NINF -502

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -599,    37,  -599,  -599,   -68,   -13,    36,   -20,  -599,   -61,
    -599,  -599,    53,  -599,  -599,  -599,  -599,  -599,   235,  -599,
     152,  -599,  -599,  -599,  -599,  -599,  -599,   111,  -599,   122,
     137,   126,  -599,  -599,    21,  -599,   159,  -599,  -599,   152,
     168,   176,  -599,  -599,   187,   228,   186,  -599,     6,  -599,
    -599,  -599,    14,   219,   221,  -599,   233,    15,   -68,   239,
     -13,   251,  -599,   254,   264,  -599,   268,  -599,   -93,   270,
     240,   242,  -599,   274,   -68,    53,  -599,  -599,  -599,    10,
     243,  -599,  -599,  -599,   284,  -599,  -599,   289,  -599,  -599,
     257,  -599,  -599,  -599,  -599,  -599,   200,    87,  -599,  -599,
    -599,  -599,   382,  -599,  -599,  1249,  -599,  -599,  -599,  -599,
    -599,   297,   298,  -599,   -89,  -599,  7173,   410,   411,  -599,
     267,  -599,  -599,    35,   292,  -599,  -599,   302,  -599,  -599,
     291,  -599,  -599,  -599,  -599,  -599,  -599,  -599,   -32,  -599,
     290,   293,   294,   295,  -599,  -599,  -599,   277,  -599,  -599,
    -599,  -599,  -599,   301,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,   303,  -599,  -599,  -599,   304,   305,  -599,
    -599,  -599,  -599,   307,   312,  -599,  -599,  -599,  -599,  -599,
     716,  -599,   279,   317,  -599,   299,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,   -85,
     318,  1375,   280,  -599,    78,  -599,   306,   -66,  -599,  -599,
     -97,  -599,  -599,  -599,    87,  -599,  -599,  -599,  -599,  -599,
     338,  -599,   129,   205,   216,  -599,  -599,  -599,  -599,  -599,
    -599,   417,  -599,  -599,    16,  2904,  -599,  -599,   -47,  1249,
     -42,   -68,  -599,    -5,  -599,   227,  1249,  -599,  -599,  4541,
    -599,  -599,   302,  -599,  -599,  7086,   308,   325,  1122,  -599,
    -599,   243,  4541,   309,  4541,   326,   329,   319,  -599,   314,
     339,   361,  3043,   243,  -599,   341,  2191,  4541,  4541,   224,
     224,  6911,  7000,  4541,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  4541,  4541,  4541,    78,  4541,  4541,  4541,  -599,   320,
    -599,   468,  -599,  -599,   322,   323,  4541,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,   -73,   327,  -599,   328,   331,
    -599,  -599,  -599,   302,  -599,  -599,  -599,  -599,   332,  -599,
    -599,   -74,  -599,  -599,  -599,  -599,  -599,  6192,   337,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
     324,   -22,  -599,  -599,  -599,  -599,   376,  1249,  -599,   -43,
    -599,  1249,  1249,  1249,  1249,   340,   297,  1249,   267,  1249,
     267,  1249,   267,  7172,   298,  -599,  -599,  -599,   344,  -599,
    -599,  5852,  -599,  -599,  3258,  -599,  -599,    65,   302,  -599,
     -49,  -599,   387,  3258,   431,     0,    90,  -599,    78,  -599,
    2330,  -599,  -599,   405,  4631,   512,  6486,  4541,   431,  -599,
    -599,  4541,  -599,  -599,   370,   388,  6486,  -599,  1249,  2330,
    -599,  4730,    61,    61,   346,  -599,  -599,   302,  1720,   302,
    1877,  6260,  -599,   145,   190,    61,    61,   -59,  -599,    61,
      61,  5300,   102,   347,  -599,  -599,   431,  -599,  2469,  -599,
    -599,  -599,  -599,   396,   224,  -599,    -3,   395,  4541,  4541,
    4541,  4541,  4541,  4541,  4541,  4541,  4541,  4541,  -599,  -599,
    4541,  4541,  4541,  4541,  4541,  4541,   400,  2626,  4541,   406,
    4541,  4541,  4541,  4541,  4541,  4541,  4541,  4541,  4541,  4541,
    4541,  4541,  4541,  4541,  2765,  3182,  4541,  4541,  4541,  4541,
    4541,  4541,  4541,  4541,  4541,  4541,   407,  4541,  3321,  -599,
    -105,  -599,    26,  -599,   573,  -599,  -599,  -599,  2330,   172,
     735,   585,  1193,  -599,    99,  1317,   318,  1660,   318,  1971,
     318,    71,  -599,   123,  3258,   203,  -599,  -599,   372,  -599,
    -599,  -599,  1249,  -599,  2330,  -599,  -599,  4541,  3478,  3635,
    1249,  -599,  4829,  4541,  4541,  -599,   243,  5378,  -599,  1249,
    1249,  5458,  1249,  -599,  -599,  2132,  -599,  4928,  -599,  -599,
       3,   224,   -54,  2034,  3774,  6260,   401,   -15,   379,   413,
    -599,  -599,   -39,    17,  3931,   -15,   209,  4541,  4541,   373,
    -599,  4541,   265,   412,  -599,   232,  -599,  -599,   266,   422,
    -599,    -8,  6486,   -26,   267,  -599,   409,  -599,  -599,  6486,
    6486,  6486,  6486,  6486,  6486,  6486,  6486,   315,   315,   238,
    6486,  6486,   238,  6845,  6845,   391,  4541,  4541,  6486,    19,
    -599,  5928,   -63,   -63,  6486,   315,   315,  6486,  6486,  6661,
    6554,  6633,  6486,  6486,  6486,  4541,  4541,  6486,   426,  6339,
     923,  6739,  6767,   238,   238,   353,   353,    19,    19,    19,
    -599,   571,  6004,  -599,    68,   -14,  4541,  -599,    53,  -599,
     566,   532,   392,   417,  -599,  2330,  -599,  5027,  -599,  1249,
    -599,  -599,  -599,   434,  -599,   415,  -599,   420,  -599,   421,
    1249,  -599,  7172,  -599,   298,  -599,   773,  -599,  6092,   -98,
    -599,   101,   438,  5536,  2267,  -599,  -599,  -599,  2406,  2563,
    -599,  2702,   396,  -599,  4541,  -599,  4541,  4541,    12,   439,
     422,   423,   424,  4541,   425,  4541,  4541,   422,   402,   428,
    6486,  -599,  -599,  6407,  -599,   451,  1249,   267,  1560,   243,
    -599,  -599,  4541,  -599,   -27,    83,  4541,  -599,   440,   454,
     455,   459,  -599,    88,   318,  -599,  4088,   544,  6486,  -599,
     544,  6486,  -599,  4541,  -599,  -599,  -599,  -599,   703,    93,
     375,  -599,   302,  -599,  -599,  5126,  -599,  -599,  2841,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  3258,  -599,
    -599,  -599,   417,  4541,   243,  -599,   215,  -599,  -599,  -599,
    -599,  -599,   430,    23,  6486,  6486,   -15,   432,   -24,   401,
     433,  -599,  6486,  -599,  -599,    34,   -15,   436,  -599,  -599,
    -599,  2980,   318,  -599,  -599,   431,   441,  6486,  -599,  -599,
    -599,  -599,   -26,   442,   -90,  1249,  -599,   103,  6486,  -599,
    -599,   429,  -599,    14,  -599,  -599,  -599,  -599,  -599,  -599,
    6486,  -599,  4541,   471,  -599,  4541,  4541,  4541,  4245,  4541,
     444,   445,  4541,  4541,  -599,  4541,   443,  -599,  -599,   457,
     266,  -599,  -599,  -599,  4402,  -599,  -599,  3119,  -599,  -599,
      14,  -599,  -599,  5616,   458,   238,   238,   238,  -599,  5694,
    5225,   447,  -599,  6486,  6486,  5225,   449,    78,  -599,  -599,
    4541,  6486,  -599,   448,  -599,  4541,  -599,   584,   456,  -599,
     450,  -599,  -599,    78,  6486,  -599,  -599,  5774,  4541,   460,
     461,  -599,  -599,  6486,  -599,  -599
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    88,     1,   191,     0,     0,     0,     0,   192,     0,
     512,   510,     0,    14,     3,    10,     9,     8,     0,     7,
     382,     6,    11,     5,     4,    12,    13,    77,    79,    34,
      46,    43,    44,    36,    41,    35,     0,    19,    18,   382,
       0,     0,    82,    83,     0,   171,    84,    86,     0,    81,
     406,   405,   139,   397,   407,   383,   384,     0,     0,     0,
       0,     0,    42,     0,     0,    39,     0,   394,     0,    15,
       0,     0,   173,     0,     0,     0,    89,   141,   140,     0,
       0,   138,   399,   398,     0,   409,   408,     0,   386,   385,
     388,    75,    76,    73,    74,    72,     0,     0,    71,    80,
      47,    45,    41,    38,    37,     0,   396,   395,    16,    17,
      20,     0,     0,   172,     0,    87,     0,     0,     0,    95,
      90,   165,   143,     0,   402,   410,   380,     0,    21,    22,
       0,    67,    68,    65,    66,    64,    63,    69,     0,    40,
       0,     0,     0,     0,   414,   434,   415,   446,   416,   420,
     421,   422,   423,   438,   427,   428,   429,   430,   431,   432,
     433,   435,   436,   483,   419,   426,   437,   490,   497,   417,
     424,   418,   425,     0,     0,   445,   453,   456,   454,   455,
       0,   448,     0,     0,   360,     0,    85,    98,    99,   101,
     100,   102,   103,   104,   105,   130,   131,   128,   129,   121,
     132,   133,   122,   119,   120,   134,   135,   136,   137,   109,
     110,   111,   106,   107,   108,   124,   125,   123,   117,   118,
     113,   112,   114,   115,   116,    97,    96,   126,   127,   340,
      93,     0,     0,   390,     0,   404,     0,     0,   412,   389,
     373,    23,    24,    25,     0,    78,   474,   477,   480,   470,
       0,   450,   484,   491,   498,   504,   507,   461,   466,   460,
     473,     0,   469,   463,     0,     0,   393,   465,     0,     0,
       0,     0,    91,     0,   353,     0,     0,   142,   167,     0,
      56,    57,     0,   276,   277,     0,     0,     0,     0,   270,
     179,     0,     0,     0,     0,     0,     0,     0,   180,     0,
       0,     0,     0,     0,   438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   227,   229,   228,   230,   231,   232,
      26,     0,     0,     0,     0,     0,     0,     0,   214,   215,
     146,   144,   274,   273,     0,     0,     0,   160,   155,   153,
     152,   154,   225,   166,   147,   271,     0,   333,     0,     0,
     158,   159,   161,     0,   151,   332,   331,   330,    88,   336,
     272,     0,   148,   335,   334,   312,   278,   233,     0,   275,
     514,   515,   516,   517,   439,   442,   440,   443,   441,   444,
       0,     0,   403,   345,   387,   381,     0,     0,   374,     0,
      70,     0,     0,     0,     0,     0,     0,     0,    90,     0,
      90,     0,    90,     0,     0,   468,   462,   464,     0,   467,
     458,     0,   449,   513,   359,   511,   361,     0,     0,   351,
     364,    92,   340,    94,     0,     0,   175,   178,     0,   203,
       0,   181,   184,     0,   233,     0,   174,     0,     0,   194,
     197,     0,   164,   200,     0,     0,    50,    60,     0,     0,
     188,   233,   304,   305,   215,   169,   170,     0,   524,     0,
       0,     0,   541,     0,     0,   282,   281,   317,    32,   280,
     279,     0,   216,     0,   162,   163,     0,   339,     0,   150,
     156,   157,   193,   223,     0,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   306,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   267,     0,     0,   390,
       0,   400,   340,   413,     0,   371,   372,   370,     0,     0,
       0,     0,     0,   447,     0,     0,    93,     0,    93,     0,
      93,   171,   357,     0,   355,     0,   459,   457,     0,   352,
     363,   362,     0,   365,     0,   354,    61,     0,     0,     0,
       0,   185,   233,     0,     0,   182,     0,     0,    62,     0,
       0,     0,     0,    48,    49,     0,   189,   233,   186,   216,
       0,     0,   171,     0,     0,   532,   522,   524,     0,   535,
     536,   537,     0,     0,     0,   524,     0,     0,     0,     0,
      29,     0,    27,     0,   308,   260,   259,   165,    53,     0,
     313,     0,   209,     0,    90,   168,     0,   327,   328,   243,
     244,   246,   245,   247,   237,   238,   239,   283,   284,   296,
     248,   249,   297,   294,   295,     0,     0,     0,   235,   322,
     311,     0,   337,   338,   236,   285,   286,   250,   251,   301,
     302,   303,   240,   241,   242,     0,     0,   234,     0,     0,
     299,   300,   298,   292,   293,   288,   287,   289,   290,   291,
     266,     0,     0,   315,     0,     0,     0,   391,     0,   411,
       0,   342,     0,   370,   375,     0,   379,   233,   475,     0,
     481,   471,   451,     0,   485,     0,   492,     0,   499,     0,
       0,   505,     0,   508,     0,   341,   366,   369,   368,     0,
     176,     0,     0,     0,     0,   183,   190,   318,     0,     0,
     319,     0,   223,   187,     0,   525,     0,     0,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   542,   543,     0,    30,    33,     0,    90,     0,     0,
      51,    52,     0,    58,     0,     0,     0,   314,     0,     0,
       0,     0,   221,     0,    93,   324,     0,   254,   255,   310,
     252,   253,   329,     0,   268,   309,   316,   401,     0,     0,
     342,   343,     0,   346,   377,   233,   378,   476,     0,   482,
     472,   452,   486,   488,   493,   495,   500,   502,   356,   506,
     358,   509,   362,     0,     0,   177,     0,   206,   204,   195,
     198,   201,     0,     0,   519,   518,   524,     0,     0,   523,
       0,   527,   533,   539,   538,     0,   524,     0,   540,    28,
      31,     0,    93,   145,    54,     0,     0,   210,   219,   220,
     218,   217,     0,     0,     0,     0,   257,     0,   323,   269,
     392,    89,   347,   139,   344,   376,   478,   489,   496,   503,
     367,    59,     0,     0,   205,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,     0,   530,   261,     0,
      53,   256,   222,   224,     0,   211,   226,     0,   258,   350,
     139,   349,   479,     0,     0,   196,   199,   202,   320,     0,
     544,     0,   528,   521,   520,   544,     0,     0,   264,    55,
       0,   212,   325,     0,   207,     0,   321,     0,     0,   529,
       0,   531,   262,     0,   213,   326,   348,     0,     0,     0,
       0,   265,   208,   545,   546,   547
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -599,  -599,  -599,  -599,  -599,   173,   579,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,   538,  -599,  -599,   586,
    -599,  -599,  -599,  -256,  -599,  -599,  -599,  -599,  -599,  -599,
     408,  -599,   587,   -62,  -599,   574,   -48,  -341,  -384,  -533,
    -599,  -599,  -599,  -257,  -215,   -79,  -599,    38,  -280,   -12,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,   662,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
    -531,  -599,  -599,  -198,  -599,   -75,  -299,  -599,  -229,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
     476,  -599,  -599,   130,  -129,  -599,  -599,  -599,   255,  -599,
     -44,  -599,  -267,   269,  -415,  -412,   135,  -599,  -202,  -599,
     641,  -599,  -599,  -599,   142,   614,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -269,   -96,  -599,  -599,
    -599,   287,  -599,  -599,  -599,   -77,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,  -599,
    -599,  -599,  -599,  -599,   399,  -598,  -439,  -583,  -599,  -599,
    -293,   -70,   231,  -599,  -599,  -599,  -233,  -599
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   110,    39,    14,   130,   136,   464,   332,   765,
     333,   623,    15,    16,    34,    35,    65,    17,    31,    32,
     334,   335,   772,   773,   336,   337,   338,   339,   340,   341,
     137,   138,    28,    29,    46,    47,    48,    18,   230,   277,
     120,    19,    79,    80,    81,   342,   343,   231,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     589,   885,   590,   886,   592,   887,   356,   580,   884,   357,
     631,   906,   358,   782,   783,   634,   359,   360,   433,   362,
     363,   364,   365,   766,   927,   767,   943,   366,   691,   869,
     632,   865,   945,   273,   802,   702,   542,   910,   274,   275,
     562,   563,   184,   185,   747,   419,   548,   389,   239,   237,
      56,    90,    21,   127,   381,    67,    68,    22,    84,    23,
     235,   125,    54,    87,    24,   236,   240,   368,   380,   177,
     178,   182,   179,   396,   811,   564,   394,   810,   391,   807,
     392,   912,   393,   809,   397,   812,   398,   877,   399,   814,
     400,   878,   401,   816,   402,   879,   403,   819,   404,   821,
      25,    41,    26,    40,   369,   606,   607,   608,   370,   609,
     610,   611,   612,   371,   463,   372,   938,   373
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,   122,   361,   416,   420,   574,   569,   694,   432,   176,
     455,   456,   114,   425,   556,   636,   558,   483,   560,   577,
     462,   615,   744,   715,   751,   717,   450,   719,   180,    91,
      92,   775,   758,   -88,   601,   385,   756,     2,   904,   486,
     487,   418,    61,     3,   116,   484,   729,   731,   406,   696,
     506,   271,    66,   509,   510,   776,   386,   509,   510,   387,
     570,   601,   388,    49,    58,   570,     4,    77,     5,   746,
       6,   545,   477,    78,    62,    73,     7,    27,   697,   238,
     546,   486,   487,     8,    37,   824,    42,   570,   778,     9,
     570,   186,   106,   175,   121,   272,   746,   779,   412,   892,
     571,   131,   132,   183,   386,   571,   478,   572,    10,   485,
     573,   547,    43,   536,   537,   117,   118,   536,   537,   384,
      38,   244,   257,   540,   498,   499,    30,   571,   780,    11,
     571,   540,   506,   781,   508,   509,   510,   511,   413,   754,
     238,   512,   637,   415,   755,   776,   258,   259,   245,   420,
     581,   482,   839,   386,    93,   119,   386,   774,    94,    75,
      95,    96,   271,   541,    36,   837,   498,   499,   750,   596,
     386,   797,   777,   176,   506,   407,   776,   509,   510,   511,
     176,    33,    44,   512,    76,   635,    63,   776,   600,   176,
     613,   232,   414,   408,    97,   536,   537,    64,    45,   423,
      73,   700,   409,   698,   257,   260,   889,    44,   426,   417,
      55,   699,   435,   833,    12,   176,   176,   895,    58,   233,
      13,   776,   382,    45,   447,   845,   133,   720,   258,   259,
     134,    44,   135,    96,   458,   460,   749,   536,   537,    50,
      51,   862,    52,   568,   261,   262,    75,    45,   796,   263,
     784,   864,   264,   890,   776,   867,   776,   175,   486,   487,
     712,   856,   625,   896,   175,    57,   863,   265,   706,   578,
      53,   871,    82,   175,    85,    58,   267,   769,    83,    60,
      86,   825,   713,   908,   721,  -487,    88,   260,   770,   771,
    -487,   176,    89,    59,   727,   176,   176,   176,   176,   175,
     175,   176,   745,   176,    66,   176,   722,   176,  -487,   836,
     544,   823,   468,    70,   549,   550,   551,   552,   846,   899,
     555,    71,   557,   108,   559,   761,   261,   262,   618,   109,
     619,   263,    72,   708,   264,   486,   487,   128,   129,   620,
     621,   496,   497,   498,   499,   576,   374,   128,   129,   265,
     375,   506,   176,   508,   509,   510,   511,    73,   267,   588,
     512,  -494,   514,   515,   723,    74,  -494,   376,   377,   378,
     379,   595,  -501,   486,   487,   175,   882,  -501,   100,   175,
     175,   175,   175,   852,  -494,   175,   724,   175,  -263,   175,
     874,   175,   755,  -263,   759,  -501,   102,   628,   883,   103,
     531,   532,   533,   534,   535,   328,   454,   421,   121,   104,
     422,  -263,   241,   242,   536,   537,   579,   801,   872,   113,
     498,   499,   105,   893,   111,   804,   112,   121,   506,   123,
     508,   509,   510,   511,   124,    62,   175,   512,   241,   242,
     243,   126,   181,   183,   227,   228,   229,   238,   234,   405,
     246,   486,   487,   247,   248,   249,   250,   416,   498,   499,
     626,   251,   268,   252,   253,   254,   506,   255,   508,   509,
     510,   511,   256,   269,   276,   512,   176,   531,   532,   533,
     534,   535,   270,   395,   176,   429,   439,   428,   437,   440,
     383,   536,   537,   176,   176,   726,   176,   442,   441,   443,
     444,   448,   472,   734,   473,   474,   475,   736,   539,    12,
     479,   480,   738,   739,   481,   741,   538,   533,   534,   535,
     553,   543,   566,   271,   484,   586,   593,   594,   599,   536,
     537,   627,   633,   420,   496,   497,   498,   499,   500,   361,
     638,   503,   504,   505,   506,   655,   508,   509,   510,   511,
     725,   660,   690,   512,   749,   514,   515,   752,   762,   764,
     175,   518,   519,   520,   486,   487,   753,   774,   175,   785,
     786,   792,   794,   800,   801,   803,   813,   175,   175,   412,
     175,   815,   817,   826,   838,   858,   525,   847,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   850,   859,
     860,   840,   841,   843,   861,   257,   848,   536,   537,   888,
     891,   894,   909,   176,   897,   121,   914,   257,   928,   935,
     903,   901,   921,   922,   176,   939,   176,   941,   926,   258,
     259,   946,   808,   948,   949,   950,    98,   622,   954,   955,
     139,   258,   259,   818,   929,    99,   101,   496,   497,   115,
     799,   500,   390,   933,   503,   504,   505,   506,   911,   508,
     509,   510,   511,    20,   902,   768,   512,   832,   514,   515,
     176,   873,   701,   565,   518,   519,   520,   575,   820,   705,
      69,   695,   107,   554,   427,   844,    49,   545,   260,   851,
     854,   616,   940,     0,     0,     0,   546,   175,     0,   525,
     260,   526,   527,   528,   529,   530,     0,   367,   175,   534,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   537,     0,   486,   487,     0,     0,   703,   262,     0,
       0,     0,   263,     0,     0,   264,     0,     0,     0,   261,
     262,   411,     0,     0,   263,   881,   710,   264,   257,     0,
     265,     0,     0,     0,   175,   424,   704,     0,     0,   267,
       0,     0,   265,     0,   434,     0,     0,   257,   436,   176,
     438,   267,   258,   259,     0,     0,   900,     0,   446,     0,
       0,     0,   451,   452,   453,   905,     0,     0,   907,   461,
       0,   258,   259,     0,     0,     0,     0,   465,   466,   467,
       0,   469,   470,   471,     0,   257,   496,   497,   498,   499,
     500,     0,   476,   503,   504,   505,   506,     0,   508,   509,
     510,   511,     0,     0,     0,   512,     0,   514,   515,   258,
     259,   260,     0,   518,   519,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,     0,   175,     0,     0,     0,     0,   525,     0,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     261,   262,     0,     0,     0,   263,     0,     0,   264,   536,
     537,     0,     0,     0,     0,     0,   870,   570,   260,   261,
     262,     0,     0,   265,   263,     0,     0,   264,     0,   266,
       0,     0,   267,     0,     0,     0,   582,     0,     0,     0,
       0,     0,   265,   587,     0,   942,     0,   591,   709,     0,
       0,   267,     0,     0,     0,   597,     0,   822,   262,     0,
       0,   951,   263,     0,   605,   264,   605,     0,     0,     0,
       0,     0,     0,   486,   487,     0,     0,     0,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,     0,     0,   649,   650,   651,   652,
     653,   654,     0,   658,   659,     0,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     677,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,     0,   692,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   707,     0,   496,   497,   498,   499,
     500,     0,     0,   503,   504,   505,   506,     0,   508,   509,
     510,   511,     0,     0,     0,   512,     0,   514,   515,     0,
     728,     0,     0,     0,     0,   733,     0,     0,     0,   658,
     677,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     471,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     471,     0,     0,   760,   461,     0,     0,   763,     0,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   787,   788,     0,     0,   283,   284,   285,   286,
     287,     0,     0,     0,     0,     0,     0,     0,   289,     0,
       0,   790,   791,     0,     0,     0,     0,     0,     0,     0,
     293,     0,     0,   295,     0,     0,   296,     0,     0,   297,
       0,     0,   798,   300,     0,     0,     0,     0,     0,     0,
       0,   805,     0,     0,   144,   145,   146,     0,   148,   149,
     150,   151,   152,   304,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,   164,   165,   166,     0,     0,   169,
     170,   171,   172,     0,     0,   305,     0,     0,     0,     0,
       0,     0,   834,   835,     0,   257,     0,   307,   308,   842,
       0,   605,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,   309,   310,   367,     0,     0,     0,   855,   258,
     259,    44,   857,     0,     0,     0,     0,     0,   311,   312,
     313,   314,   315,   316,   317,   318,   319,    45,   320,   868,
       0,     0,     0,   140,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,   321,   322,   323,     0,   324,     0,
       0,   325,   326,     0,   142,     0,     0,     0,     0,   880,
       0,   327,     0,   328,   329,   431,   121,     0,   260,   143,
       0,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,     0,     0,     0,     0,     0,   261,   262,   257,
       0,     0,   263,     0,   711,   264,     0,     0,   913,     0,
       0,   915,   916,   917,   919,   920,     0,     0,   923,   924,
     265,   925,     0,   258,   259,     0,   278,     0,    44,   267,
     931,     3,     0,   279,   280,   281,     0,   282,     0,   283,
     284,   285,   286,   287,    45,     0,     0,     0,     0,     0,
     288,   289,   290,   291,     0,     0,   944,     0,     0,     0,
       0,   947,   292,   293,     0,   294,   295,     0,     0,   296,
       0,     8,   297,   298,   953,   299,   300,     0,     0,   301,
     302,     0,   260,     0,     0,     0,   303,   144,   145,   146,
       0,   148,   149,   150,   151,   152,   304,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     0,   164,   165,   166,
       0,     0,   169,   170,   171,   172,     0,     0,   305,   306,
       0,   261,   262,     0,     0,     0,   263,     0,   714,   264,
     307,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   265,   309,   310,     0,     0,     0,
       0,     0,     0,   267,    44,     0,     0,     0,     0,     0,
       0,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      45,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,   322,   323,
       0,   324,     0,     0,   325,   326,     0,     0,     0,     0,
       0,     0,     0,     0,   327,     0,   328,   329,   330,   121,
     331,   278,     0,     0,     0,     0,     3,     0,   279,   280,
     281,     0,   282,     0,   283,   284,   285,   286,   287,     0,
       0,     0,     0,     0,     0,   288,   289,   290,   291,     0,
       0,     0,     0,     0,     0,     0,     0,   292,   293,     0,
     294,   295,     0,     0,   296,     0,     8,   297,   298,     0,
     299,   300,     0,     0,   301,   302,     0,     0,     0,     0,
       0,   303,   144,   145,   146,     0,   148,   149,   150,   151,
     152,   304,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,   164,   165,   166,     0,     0,   169,   170,   171,
     172,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,   308,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   310,     0,     0,     0,     0,     0,     0,     0,    44,
       0,     0,   257,     0,     0,     0,   311,   312,   313,   314,
     315,   316,   317,   318,   319,    45,   320,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,   321,   322,   323,     0,   324,     0,     0,   325,
     326,     0,     0,     0,   283,   284,   285,   286,   287,   327,
       0,   328,   329,   330,   121,   853,   289,     0,     0,     0,
       0,     0,   257,     0,     0,     0,     0,     0,   293,     0,
       0,   295,     0,     0,   296,     0,     0,   297,     0,   601,
       0,   300,     0,     0,     0,   260,   258,   259,     0,     0,
       0,     0,   144,   145,   146,     0,   148,   149,   150,   151,
     152,   304,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,   164,   165,   166,     0,     0,   169,   170,   171,
     172,     0,     0,   305,   261,   262,     0,     0,     0,   263,
       0,   716,   264,     0,     0,   307,   308,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   267,     0,     0,    44,
       0,     0,     0,     0,     0,     0,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   602,   320,     0,     0,     0,
       0,     0,     0,     0,   261,   262,     0,     0,     0,   263,
       0,     0,   603,   322,   323,     0,   324,     0,     0,   325,
     326,   283,   284,   285,   286,   287,     0,   265,     0,   604,
       0,   328,   329,   289,   121,     0,   267,     0,     0,   257,
       0,     0,     0,     0,     0,   293,     0,     0,   295,     0,
       0,   296,     0,     0,   297,     0,     0,     0,   300,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,   144,
     145,   146,     0,   148,   149,   150,   151,   152,   304,   154,
     155,   156,   157,   158,   159,   160,   161,   162,     0,   164,
     165,   166,     0,     0,   169,   170,   171,   172,     0,     0,
     305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,   308,     0,     0,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,    44,     0,     0,     0,
       0,     0,     0,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   602,   320,     0,     0,     0,   258,   259,     0,
       0,   261,   262,     0,     0,     0,   263,     0,     0,   603,
     322,   323,     0,   324,     0,     0,   325,   326,   283,   284,
     285,   286,   287,     0,   265,     0,   614,     0,   328,   329,
     289,   121,     0,   267,     0,     0,   406,     0,     0,     0,
       0,     0,   293,     0,     0,   295,     0,     0,   296,     0,
       0,   297,     0,     0,     0,   300,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,   145,   146,     0,
     148,   149,   150,   151,   152,   304,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,   164,   165,   166,     0,
       0,   169,   170,   171,   172,   261,   262,   305,     0,     0,
     263,     0,   718,   264,     0,     0,     0,     0,     0,   307,
     308,     0,     0,     0,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,    44,   257,     0,     0,     0,     0,     0,
     311,   312,   313,   314,   315,   316,   317,   318,   319,    45,
     320,     0,     0,     0,     0,     0,     0,     0,   258,   259,
       0,     0,     0,   407,     0,     0,   321,   322,   323,     0,
     324,     0,     0,   325,   326,   283,   284,   285,   286,   287,
       0,   408,     0,   327,     0,   328,   329,   289,   121,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,   293,
       0,     0,   295,     0,     0,   296,     0,     0,   297,     0,
       0,     0,   300,     0,     0,     0,     0,   260,     0,     0,
       0,     0,     0,   144,   145,   146,     0,   148,   149,   150,
     151,   152,   304,   154,   155,   156,   157,   158,   159,   160,
     161,   162,     0,   164,   165,   166,     0,     0,   169,   170,
     171,   172,     0,     0,   305,     0,   261,   262,     0,     0,
       0,   263,     0,   742,   264,     0,   307,   308,     0,   257,
       0,     0,     0,     0,     0,   449,     0,     0,     0,   265,
       0,   309,   310,     0,     0,     0,     0,     0,   267,     0,
      44,     0,     0,   258,   259,     0,     0,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    45,   320,     0,     0,
       0,     0,     0,     0,   283,   284,   285,   286,   287,     0,
       0,     0,     0,   321,   322,   323,   289,   324,     0,     0,
     325,   326,     0,     0,     0,     0,     0,     0,   293,     0,
     327,   295,   328,   329,   296,   121,     0,   297,     0,     0,
       0,   300,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   145,   146,     0,   148,   149,   150,   151,
     152,   304,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,   164,   165,   166,     0,     0,   169,   170,   171,
     172,   261,   262,   305,     0,     0,   263,     0,   828,   264,
       0,     0,     0,     0,     0,   307,   308,     0,   257,     0,
       0,     0,     0,     0,   265,     0,     0,     0,     0,     0,
     309,   310,     0,   267,     0,     0,     0,     0,     0,    44,
       0,     0,   258,   259,     0,     0,   311,   312,   313,   314,
     315,   316,   317,   318,   319,    45,   320,     0,     0,     0,
       0,     0,     0,   283,   284,   285,   286,   287,     0,     0,
       0,     0,   321,   322,   323,   289,   324,     0,     0,   325,
     326,     0,     0,     0,     0,     0,     0,   293,     0,   327,
     295,   328,   329,   296,   121,     0,   297,     0,     0,     0,
     300,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,   145,   146,     0,   148,   149,   150,   151,   152,
     304,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,   164,   165,   166,     0,     0,   169,   170,   171,   172,
     261,   262,   305,     0,     0,   263,     0,   829,   264,     0,
       0,     0,     0,     0,   307,   308,     0,     0,     0,     0,
       0,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,   267,     0,     0,   257,     0,     0,    44,     0,
       0,     0,     0,     0,     0,   311,   312,   313,   314,   315,
     316,   317,   318,   319,    45,   320,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   321,   322,   323,     0,   324,     0,     0,   325,   326,
     283,   284,   285,   286,   287,     0,   629,     0,   327,   630,
     328,   329,   289,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,     0,     0,   295,     0,     0,
     296,     0,     0,   297,     0,     0,     0,   300,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   144,   145,
     146,     0,   148,   149,   150,   151,   152,   304,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     0,   164,   165,
     166,     0,     0,   169,   170,   171,   172,   261,   262,   305,
       0,     0,   263,     0,   830,   264,     0,     0,     0,     0,
       0,   307,   308,     0,   257,     0,     0,     0,     0,     0,
     265,     0,     0,     0,     0,     0,   656,   657,     0,   267,
       0,     0,     0,     0,     0,    44,     0,     0,   258,   259,
       0,     0,   311,   312,   313,   314,   315,   316,   317,   318,
     319,    45,   320,     0,     0,     0,     0,     0,     0,   283,
     284,   285,   286,   287,     0,     0,     0,     0,   321,   322,
     323,   289,   324,     0,     0,   325,   326,     0,     0,     0,
       0,     0,     0,   293,     0,   327,   295,   328,   329,   296,
     121,     0,   297,     0,     0,     0,   300,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,   146,
       0,   148,   149,   150,   151,   152,   304,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     0,   164,   165,   166,
       0,     0,   169,   170,   171,   172,   261,   262,   305,     0,
       0,   263,     0,   831,   264,     0,     0,     0,     0,     0,
     307,   308,     0,   257,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,   675,   676,     0,   267,     0,
       0,     0,     0,     0,    44,     0,     0,   258,   259,     0,
       0,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      45,   320,     0,     0,     0,     0,     0,     0,   283,   284,
     285,   286,   287,     0,     0,     0,     0,   321,   322,   323,
     289,   324,     0,     0,   325,   326,     0,     0,     0,     0,
       0,     0,   293,     0,   327,   295,   328,   329,   296,   121,
       0,   297,     0,     0,     0,   300,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,   145,   146,     0,
     148,   149,   150,   151,   152,   304,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,   164,   165,   166,     0,
       0,   169,   170,   171,   172,   261,   262,   305,     0,     0,
     263,     0,   876,   264,     0,     0,     0,     0,     0,   307,
     308,     0,   257,     0,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,    44,     0,     0,   258,   259,     0,     0,
     311,   312,   313,   314,   315,   316,   317,   318,   319,    45,
     320,     0,     0,     0,     0,     0,     0,   283,   284,   285,
     286,   287,     0,     0,     0,     0,   321,   322,   323,   289,
     324,     0,     0,   325,   326,     0,     0,     0,     0,     0,
       0,   293,   410,   327,   295,   328,   329,   296,   121,     0,
     297,     0,     0,     0,   300,   260,     0,   445,     0,     0,
       0,     0,     0,     0,     0,   144,   145,   146,     0,   148,
     149,   150,   151,   152,   304,   154,   155,   156,   157,   158,
     159,   160,   161,   162,     0,   164,   165,   166,     0,     0,
     169,   170,   171,   172,   261,   262,   305,     0,     0,   263,
       0,   898,   264,     0,     0,     0,     0,     0,   307,   308,
       0,   257,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   267,     0,     0,     0,
       0,     0,    44,     0,     0,   258,   259,     0,     0,   311,
     312,   313,   314,   315,   316,   317,   318,   319,    45,   320,
       0,     0,     0,     0,     0,     0,   283,   284,   285,   286,
     287,     0,     0,   678,     0,   321,   322,   323,   289,   324,
       0,     0,   325,   326,     0,     0,     0,     0,     0,     0,
     293,     0,   327,   295,   328,   329,   296,   121,     0,   297,
       0,     0,     0,   300,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   145,   146,     0,   148,   149,
     150,   151,   152,   304,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,   164,   165,   166,     0,     0,   169,
     170,   171,   172,   261,   262,   305,     0,     0,   263,     0,
     932,   264,     0,     0,     0,     0,     0,   307,   308,     0,
     257,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,    44,     0,     0,   258,   259,     0,     0,   311,   312,
     313,   314,   315,   316,   317,   318,   319,    45,   320,     0,
       0,     0,     0,     0,     0,   283,   284,   285,   286,   287,
       0,     0,     0,     0,   321,   322,   323,   289,   324,     0,
       0,   325,   326,     0,     0,     0,     0,     0,     0,   293,
       0,   327,   295,   328,   329,   296,   121,     0,   297,     0,
       0,     0,   300,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,   145,   146,     0,   148,   149,   150,
     151,   152,   304,   154,   155,   156,   157,   158,   159,   160,
     161,   162,     0,   164,   165,   166,     0,     0,   169,   170,
     171,   172,   261,   262,   305,     0,     0,   263,     0,     0,
     264,     0,     0,     0,     0,     0,   307,   308,     0,     0,
       0,     0,     0,     0,     0,   265,     0,     0,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    45,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   322,   323,     0,   324,     0,     0,
     325,   326,   283,   284,   285,   286,   287,     0,     0,     0,
     327,   693,   328,   329,   289,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,     0,     0,   295,
       0,     0,   296,     0,     0,   297,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   145,   146,     0,   148,   149,   150,   151,   152,   304,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     0,
     164,   165,   166,     0,     0,   169,   170,   171,   172,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,     0,     0,
       0,     0,     0,     0,   311,   312,   313,   314,   315,   316,
     317,   318,   319,    45,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     321,   322,   323,     0,   324,     0,     0,   325,   326,   283,
     284,   285,   286,   287,     0,     0,     0,   327,   730,   328,
     329,   289,   121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,     0,     0,   295,     0,     0,   296,
       0,     0,   297,     0,     0,     0,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,   146,
       0,   148,   149,   150,   151,   152,   304,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     0,   164,   165,   166,
       0,     0,   169,   170,   171,   172,     0,     0,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,     0,     0,     0,
       0,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      45,   320,     0,     0,     0,     0,     0,     0,   283,   284,
     285,   286,   287,     0,     0,   732,     0,   321,   322,   323,
     289,   324,     0,     0,   325,   326,     0,     0,     0,     0,
       0,     0,   293,     0,   327,   295,   328,   329,   296,   121,
       0,   297,     0,     0,     0,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,   145,   146,     0,
     148,   149,   150,   151,   152,   304,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,   164,   165,   166,     0,
       0,   169,   170,   171,   172,     0,     0,   305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
     311,   312,   313,   314,   315,   316,   317,   318,   319,    45,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   321,   322,   323,     0,
     324,     0,     0,   325,   326,   283,   284,   285,   286,   287,
       0,     0,     0,   327,   748,   328,   329,   289,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   293,
       0,     0,   295,     0,     0,   296,     0,     0,   297,     0,
       0,     0,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,   145,   146,     0,   148,   149,   150,
     151,   152,   304,   154,   155,   156,   157,   158,   159,   160,
     161,   162,     0,   164,   165,   166,     0,     0,   169,   170,
     171,   172,     0,     0,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    45,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   322,   323,     0,   324,     0,     0,
     325,   326,   283,   284,   285,   286,   287,     0,     0,     0,
     327,   757,   328,   329,   289,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,     0,     0,   295,
       0,     0,   296,     0,     0,   297,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   145,   146,     0,   148,   149,   150,   151,   152,   304,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     0,
     164,   165,   166,     0,     0,   169,   170,   171,   172,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,     0,     0,
       0,     0,     0,     0,   311,   312,   313,   314,   315,   316,
     317,   318,   319,    45,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     321,   322,   323,     0,   324,     0,     0,   325,   326,   283,
     284,   285,   286,   287,     0,     0,     0,   327,   866,   328,
     329,   289,   121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,     0,     0,   295,     0,     0,   296,
       0,     0,   297,     0,     0,     0,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,   146,
       0,   148,   149,   150,   151,   152,   304,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     0,   164,   165,   166,
       0,     0,   169,   170,   171,   172,     0,     0,   305,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,     0,     0,     0,
       0,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      45,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,   322,   323,
       0,   324,     0,     0,   325,   326,   283,   284,   285,   286,
     287,     0,     0,     0,   327,   918,   328,   329,   289,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     293,     0,     0,   295,     0,     0,   296,     0,     0,   297,
       0,     0,     0,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   145,   146,     0,   148,   149,
     150,   151,   152,   304,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,   164,   165,   166,     0,     0,   169,
     170,   171,   172,     0,     0,   305,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,   308,     0,
       0,     0,     0,     0,     0,     0,   930,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,     0,     0,     0,     0,     0,     0,   311,   312,
     313,   314,   315,   316,   317,   318,   319,    45,   320,     0,
       0,     0,     0,     0,     0,   283,   284,   285,   286,   287,
       0,     0,     0,     0,   321,   322,   323,   289,   324,     0,
       0,   325,   326,     0,     0,     0,     0,     0,     0,   293,
       0,   327,   295,   328,   329,   296,   121,     0,   297,     0,
       0,     0,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,   145,   146,     0,   148,   149,   150,
     151,   152,   304,   154,   155,   156,   157,   158,   159,   160,
     161,   162,     0,   164,   165,   166,     0,     0,   169,   170,
     171,   172,     0,     0,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   308,     0,     0,
       0,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    45,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   322,   323,     0,   324,     0,     0,
     325,   326,     0,     0,     0,     0,     0,     0,     0,     0,
     327,     0,   328,   329,     0,   121,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   583,   508,   509,   510,   511,
     486,   487,     0,   512,   513,   514,   515,   516,   517,     0,
       0,   518,   519,   520,   521,   522,   523,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,   525,     0,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   536,   537,     0,
       0,     0,     0,     0,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   583,   508,   509,   510,   511,   486,
     487,     0,   512,   513,   514,   515,   516,   517,     0,     0,
     518,   519,   520,   521,   522,   523,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   584,   525,     0,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   536,   537,     0,     0,
       0,     0,     0,   598,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   583,   508,   509,   510,   511,   486,   487,
       0,   512,   513,   514,   515,   516,   517,     0,     0,   518,
     519,   520,   521,   522,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,   525,     0,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   536,   537,     0,     0,     0,
       0,     0,   735,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   583,   508,   509,   510,   511,   486,   487,     0,
     512,   513,   514,   515,   516,   517,     0,     0,   518,   519,
     520,   521,   522,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   584,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   536,   537,     0,     0,     0,     0,
       0,   743,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   583,   508,   509,   510,   511,   486,   487,     0,   512,
     513,   514,   515,   516,   517,     0,     0,   518,   519,   520,
     521,   522,   523,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   536,   537,     0,     0,     0,     0,     0,
     806,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     583,   508,   509,   510,   511,   486,   487,     0,   512,   513,
     514,   515,   516,   517,     0,     0,   518,   519,   520,   521,
     522,   523,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     584,   525,     0,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   536,   537,     0,     0,     0,     0,     0,   875,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,   496,   497,
     498,   499,   500,     0,     0,   503,   504,   505,   506,     0,
     508,   509,   510,   511,     0,     0,     0,   512,     0,   514,
     515,     0,     0,     0,     0,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,     0,     0,     0,     0,     0,     0,   486,   487,
       0,   536,   537,   496,   497,   498,   499,   500,   937,     0,
     503,   504,   505,   506,     0,   508,   509,   510,   511,     0,
       0,     0,   512,     0,   514,   515,     0,     0,     0,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   536,   537,   486,   487,
     624,   496,   497,   498,   499,   500,     0,     0,   503,   504,
     505,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   536,   537,   486,   487,   737,     0,
       0,   496,   497,   498,   499,   500,     0,     0,   503,   504,
     505,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   536,   537,   486,   487,   740,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,     0,     0,     0,   512,     0,
     514,   515,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   536,   537,   486,   487,   827,     0,     0,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,     0,     0,     0,   512,     0,
     514,   515,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   536,   537,   486,   487,   934,   496,   497,   498,
     499,   500,     0,     0,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   537,   486,   487,   936,     0,     0,   496,   497,   498,
     499,   500,     0,     0,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     536,   537,     0,     0,   952,   496,   497,   498,   499,   500,
       0,     0,   503,   504,   505,   506,     0,   508,   509,   510,
     511,     0,     0,     0,   512,     0,   514,   515,     0,     0,
       0,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,     0,
       0,     0,     0,     0,   486,   487,     0,     0,   536,   537,
     567,   496,   497,   498,   499,   500,     0,     0,   503,   504,
     505,   506,     0,   508,   509,   510,   511,     0,     0,     0,
     512,     0,   514,   515,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   536,   537,   789,   496,   497,   498,
     499,   500,   486,   487,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   537,   795,     0,     0,     0,     0,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   583,   508,   509,   510,
     511,  -233,   486,   487,   512,   513,   514,   515,   516,   517,
       0,     0,   518,   519,   520,   521,   522,   523,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   584,   525,     0,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,     0,     0,     0,   512,   513,   514,   515,   516,   517,
       0,     0,   518,   519,   520,   521,   522,   523,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   524,   525,     0,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   486,
     487,     0,     0,   496,   497,   498,   499,   500,   536,   537,
     503,   504,   505,   506,     0,   508,   509,   510,   511,     0,
       0,     0,   512,     0,   514,   515,     0,     0,   617,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   486,   487,     0,
       0,     0,     0,     0,     0,     0,   536,   537,     0,     0,
       0,     0,   496,   497,   498,   499,   500,     0,     0,   503,
     504,   505,   506,     0,   508,   509,   510,   511,     0,     0,
       0,   512,     0,   514,   515,     0,     0,     0,     0,   518,
     519,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   793,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   486,   487,     0,     0,
     496,   497,   498,   499,   500,   536,   537,   503,   504,   505,
     506,     0,   508,   509,   510,   511,     0,     0,     0,   512,
       0,   514,   515,     0,     0,     0,     0,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   849,     0,
       0,     0,   525,     0,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   486,   487,     0,     0,     0,     0,
       0,     0,     0,   536,   537,     0,     0,     0,     0,   496,
     497,   498,   499,   500,     0,     0,   503,   504,   505,   506,
       0,   508,   509,   510,   511,     0,     0,     0,   512,     0,
     514,   515,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   486,   487,     0,     0,   496,   497,   498,
     499,   500,   536,   537,   503,   504,   505,   506,     0,   508,
     509,   510,   511,     0,     0,     0,   512,     0,   514,   515,
       0,   486,   487,     0,   518,     0,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   537,     0,     0,     0,     0,   496,   497,   498,   499,
     500,     0,     0,   503,   504,   505,   506,     0,   508,   509,
     510,   511,     0,     0,     0,   512,     0,   514,   515,   486,
     487,     0,     0,   518,   496,   497,   498,   499,   500,     0,
       0,   503,   504,   505,   506,     0,   508,   509,   510,   511,
       0,     0,     0,   512,     0,   514,   515,   486,   487,     0,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   536,
     537,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   536,   537,     0,
       0,     0,   496,   497,   498,   499,   500,     0,     0,   503,
     504,   505,   506,     0,   508,   509,   510,   511,     0,     0,
       0,   512,     0,   514,   515,   486,   487,     0,     0,     0,
     496,   497,   498,   499,   500,     0,     0,   503,   504,   505,
     506,     0,   508,   509,   510,   511,     0,     0,     0,   512,
       0,   514,   515,     0,     0,     0,     0,     0,   528,   529,
     530,   531,   532,   533,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   536,   537,     0,     0,     0,
       0,     0,     0,   457,     0,     0,     0,   529,   530,   531,
     532,   533,   534,   535,     0,   140,     0,     0,     0,     0,
       0,   141,     0,   536,   537,     0,     0,     0,   496,   497,
     498,   499,   500,     0,     0,   503,   142,     0,   506,     0,
     508,   509,   510,   511,     0,     0,     0,   512,     0,   514,
     515,   143,     0,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,     0,   529,   530,   531,   532,   533,
     534,   535,   459,     0,     0,     0,     0,     0,     0,     0,
       0,   536,   537,     0,   140,     0,     0,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
     143,     0,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    45,   143,     0,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   140,     0,     0,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,   142,     0,     0,
       0,     0,   311,   312,   313,     0,     0,     0,     0,     0,
       0,    45,   143,     0,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,     0,     0,     0,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,    44,     0,   209,   210,   211,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,   561,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
       0,     0,   225,   226
};

static const yytype_int16 yycheck[] =
{
      12,    80,   231,   270,   273,   420,   418,   538,   288,   105,
     309,   310,    74,   282,   398,    18,   400,   358,   402,    19,
     313,   460,    19,   556,   607,   558,   306,   560,   105,    14,
      15,   629,   615,     7,    49,   237,    19,     0,   128,    20,
      21,    46,    21,     6,    34,   119,   577,   578,    32,   154,
     113,   136,   145,   116,   117,   153,   153,   116,   117,   156,
     114,    49,   159,    75,   153,   114,    29,    53,    31,   123,
      33,   114,   145,    59,    53,   129,    39,   145,   183,   145,
     123,    20,    21,    46,   145,   183,    33,   114,   114,    52,
     114,   180,   185,   105,   184,   180,   123,   123,   145,   123,
     154,    14,    15,   145,   153,   154,   179,   156,    71,   183,
     159,   154,    59,   176,   177,   105,   106,   176,   177,   185,
     181,   153,    32,   145,   105,   106,   139,   154,   154,    92,
     154,   145,   113,   159,   115,   116,   117,   118,   185,   178,
     145,   122,   145,   185,   183,   153,    56,    57,   180,   418,
     430,   353,   750,   153,   139,   145,   153,   145,   143,   153,
     145,   146,   136,   185,   184,   748,   105,   106,   183,   449,
     153,   185,   180,   269,   113,   159,   153,   116,   117,   118,
     276,   145,   129,   122,   178,   484,   165,   153,   457,   285,
     459,   156,   269,   177,   179,   176,   177,   176,   145,   276,
     129,   542,   186,   177,    32,   115,   183,   129,   285,   271,
      58,   185,   291,   744,   177,   311,   312,   183,   153,   184,
     183,   153,   234,   145,   303,   756,   139,   156,    56,    57,
     143,   129,   145,   146,   311,   312,   153,   176,   177,     4,
       5,   153,     7,   178,   154,   155,   153,   145,   180,   159,
     634,   784,   162,   836,   153,   786,   153,   269,    20,    21,
     161,   178,   160,   846,   276,   154,   178,   177,   548,   179,
      35,   178,    53,   285,    53,   153,   186,    11,    59,   153,
      59,   180,   183,   180,   161,   156,    53,   115,    22,    23,
     161,   387,    59,   156,   574,   391,   392,   393,   394,   311,
     312,   397,   601,   399,   145,   401,   183,   403,   179,   748,
     387,   726,   324,   145,   391,   392,   393,   394,   757,   852,
     397,   145,   399,    53,   401,   618,   154,   155,   183,    59,
     185,   159,   145,   161,   162,    20,    21,   147,   148,   149,
     150,   103,   104,   105,   106,   424,    66,   147,   148,   177,
      70,   113,   448,   115,   116,   117,   118,   129,   186,   438,
     122,   156,   124,   125,   161,   179,   161,    87,    88,    89,
      90,   448,   156,    20,    21,   387,   161,   161,   139,   391,
     392,   393,   394,   767,   179,   397,   183,   399,   156,   401,
     802,   403,   183,   161,   185,   179,   145,   476,   183,   145,
     162,   163,   164,   165,   166,   181,   182,   180,   184,   145,
     183,   179,   147,   148,   176,   177,   428,    42,    43,   145,
     105,   106,   154,   838,   184,   705,   184,   184,   113,   145,
     115,   116,   117,   118,   145,    53,   448,   122,   147,   148,
     149,   184,   145,   145,    34,    34,   179,   145,   156,    32,
     160,    20,    21,   160,   160,   160,   179,   724,   105,   106,
     472,   160,   183,   160,   160,   160,   113,   160,   115,   116,
     117,   118,   160,   156,   156,   122,   572,   162,   163,   164,
     165,   166,   183,   145,   580,   160,   160,   179,   179,   160,
     184,   176,   177,   589,   590,   572,   592,   183,   179,   160,
     139,   160,   182,   580,    36,   183,   183,   586,   184,   177,
     183,   183,   589,   590,   183,   592,   179,   164,   165,   166,
     180,   145,   178,   136,   119,    13,   156,   139,   182,   176,
     177,   184,   136,   802,   103,   104,   105,   106,   107,   768,
     145,   110,   111,   112,   113,   145,   115,   116,   117,   118,
     178,   145,   145,   122,   153,   124,   125,   178,   185,   147,
     572,   130,   131,   132,    20,    21,   153,   145,   580,   160,
     179,   145,     1,     7,    42,   183,   161,   589,   590,   145,
     592,   161,   161,   145,   145,   145,   155,   185,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   147,   145,
     145,   178,   178,   178,   145,    32,   178,   176,   177,   179,
     178,   178,   183,   709,   178,   184,   145,    32,   161,   161,
     178,   180,   178,   178,   720,   178,   722,   178,   185,    56,
      57,   183,   709,    49,   178,   185,    57,   464,   178,   178,
     102,    56,    57,   720,   900,    58,    60,   103,   104,    75,
     698,   107,   244,   910,   110,   111,   112,   113,   873,   115,
     116,   117,   118,     1,   862,   627,   122,   742,   124,   125,
     766,   800,   542,   404,   130,   131,   132,   422,   722,   544,
      39,   539,    68,   396,   285,   755,   698,   114,   115,   766,
     769,   460,   925,    -1,    -1,    -1,   123,   709,    -1,   155,
     115,   157,   158,   159,   160,   161,    -1,   231,   720,   165,
     722,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    20,    21,    -1,    -1,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,   162,    -1,    -1,    -1,   154,
     155,   265,    -1,    -1,   159,   824,   161,   162,    32,    -1,
     177,    -1,    -1,    -1,   766,   279,   183,    -1,    -1,   186,
      -1,    -1,   177,    -1,   288,    -1,    -1,    32,   292,   865,
     294,   186,    56,    57,    -1,    -1,   855,    -1,   302,    -1,
      -1,    -1,   306,   307,   308,   864,    -1,    -1,   865,   313,
      -1,    56,    57,    -1,    -1,    -1,    -1,   321,   322,   323,
      -1,   325,   326,   327,    -1,    32,   103,   104,   105,   106,
     107,    -1,   336,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    56,
      57,   115,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,   865,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     154,   155,    -1,    -1,    -1,   159,    -1,    -1,   162,   176,
     177,    -1,    -1,    -1,    -1,    -1,   183,   114,   115,   154,
     155,    -1,    -1,   177,   159,    -1,    -1,   162,    -1,   183,
      -1,    -1,   186,    -1,    -1,    -1,   430,    -1,    -1,    -1,
      -1,    -1,   177,   437,    -1,   927,    -1,   441,   183,    -1,
      -1,   186,    -1,    -1,    -1,   449,    -1,   154,   155,    -1,
      -1,   943,   159,    -1,   458,   162,   460,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,    -1,    -1,   500,   501,   502,   503,
     504,   505,    -1,   507,   508,    -1,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,    -1,   537,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   548,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
     574,    -1,    -1,    -1,    -1,   579,    -1,    -1,    -1,   583,
     584,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   603,
     604,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     614,    -1,    -1,   617,   618,    -1,    -1,   621,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   656,   657,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,   675,   676,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,   696,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   705,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   746,   747,    -1,    32,    -1,   105,   106,   753,
      -1,   755,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,   120,   121,   768,    -1,    -1,    -1,   772,    56,
      57,   129,   776,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   793,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    -1,    45,    -1,    -1,    -1,    -1,   823,
      -1,   179,    -1,   181,   182,   183,   184,    -1,   115,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,   154,   155,    32,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,   882,    -1,
      -1,   885,   886,   887,   888,   889,    -1,    -1,   892,   893,
     177,   895,    -1,    56,    57,    -1,     1,    -1,   129,   186,
     904,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,   145,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,   930,    -1,    -1,    -1,
      -1,   935,    37,    38,    -1,    40,    41,    -1,    -1,    44,
      -1,    46,    47,    48,   948,    50,    51,    -1,    -1,    54,
      55,    -1,   115,    -1,    -1,    -1,    61,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    94,
      -1,   154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
      -1,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,   181,   182,   183,   184,
     185,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    41,    -1,    -1,    44,    -1,    46,    47,    48,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    32,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    14,    15,    16,    17,    18,   179,
      -1,   181,   182,   183,   184,   185,    26,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    49,
      -1,    51,    -1,    -1,    -1,   115,    56,    57,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    14,    15,    16,    17,    18,    -1,   177,    -1,   179,
      -1,   181,   182,    26,   184,    -1,   186,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,
      -1,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,   177,    -1,   179,    -1,   181,   182,
      26,   184,    -1,   186,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,   154,   155,    93,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,   129,    32,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,   177,    -1,   179,    -1,   181,   182,    26,   184,    -1,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,   162,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,   177,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,   186,    -1,
     129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     179,    41,   181,   182,    44,   184,    -1,    47,    -1,    -1,
      -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   154,   155,    93,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   179,
      41,   181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     154,   155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,    -1,    32,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,   170,
      14,    15,    16,    17,    18,    -1,   177,    -1,   179,   180,
     181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   154,   155,    93,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   186,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,   163,
     164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,    44,
     184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   154,   155,    93,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   186,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,   162,   163,   164,
      26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   179,    41,   181,   182,    44,   184,
      -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,   154,   155,    93,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,   162,   163,   164,    26,
     166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    38,   178,   179,    41,   181,   182,    44,   184,    -1,
      47,    -1,    -1,    -1,    51,   115,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   154,   155,    93,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    21,    -1,   162,   163,   164,    26,   166,
      -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   179,    41,   181,   182,    44,   184,    -1,    47,
      -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   154,   155,    93,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   179,    41,   181,   182,    44,   184,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   154,   155,    93,    -1,    -1,   159,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     179,   180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,    -1,   166,    -1,    -1,   169,   170,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   179,   180,   181,
     182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,   160,    -1,   162,   163,   164,
      26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   179,    41,   181,   182,    44,   184,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   179,   180,   181,   182,    26,   184,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     179,   180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,    -1,   166,    -1,    -1,   169,   170,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   179,   180,   181,
     182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
      -1,   166,    -1,    -1,   169,   170,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   179,   180,   181,   182,    26,   184,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   179,    41,   181,   182,    44,   184,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,    -1,   184,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      20,    21,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    20,
      21,    -1,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    20,    21,
      -1,   122,   123,   124,   125,   126,   127,    -1,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    20,    21,    -1,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    20,    21,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    20,    21,    -1,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   176,   177,   103,   104,   105,   106,   107,   183,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    20,    21,
     180,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    20,    21,   180,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    20,    21,   180,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    20,    21,   180,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    20,    21,   180,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    20,    21,   180,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     176,   177,    -1,    -1,   180,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   176,   177,
     178,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,   178,   103,   104,   105,
     106,   107,    20,    21,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,   178,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    20,    21,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    20,
      21,    -1,    -1,   103,   104,   105,   106,   107,   176,   177,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,   128,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    20,    21,    -1,    -1,
     103,   104,   105,   106,   107,   176,   177,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    20,    21,    -1,    -1,   103,   104,   105,
     106,   107,   176,   177,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    20,    21,    -1,   130,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,
      21,    -1,    -1,   130,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    20,    21,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   160,   161,   162,
     163,   164,   165,   166,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,   176,   177,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,    45,    -1,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,   160,   161,   162,   163,   164,
     165,   166,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   129,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,   169,   170
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   188,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   177,   183,   191,   199,   200,   204,   224,   228,
     244,   299,   304,   306,   311,   347,   349,   145,   219,   220,
     139,   205,   206,   145,   201,   202,   184,   145,   181,   190,
     350,   348,    33,    59,   129,   145,   221,   222,   223,   236,
       4,     5,     7,    35,   309,    58,   297,   154,   153,   156,
     153,    21,    53,   165,   176,   203,   145,   302,   303,   297,
     145,   145,   145,   129,   179,   153,   178,    53,    59,   229,
     230,   231,    53,    59,   305,    53,    59,   310,    53,    59,
     298,    14,    15,   139,   143,   145,   146,   179,   193,   219,
     139,   206,   145,   145,   145,   154,   185,   302,    53,    59,
     189,   184,   184,   145,   220,   222,    34,   105,   106,   145,
     227,   184,   232,   145,   145,   308,   184,   300,   147,   148,
     192,    14,    15,   139,   143,   145,   193,   217,   218,   203,
      24,    30,    45,    60,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,   236,   314,   316,   317,   319,
     322,   145,   318,   145,   289,   290,   180,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   124,   125,   126,   127,   130,
     131,   132,   133,   134,   135,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   169,   170,    34,    34,   179,
     225,   234,   156,   184,   156,   307,   312,   296,   145,   295,
     313,   147,   148,   149,   153,   180,   160,   160,   160,   160,
     179,   160,   160,   160,   160,   160,   160,    32,    56,    57,
     115,   154,   155,   159,   162,   177,   183,   186,   183,   156,
     183,   136,   180,   280,   285,   286,   156,   226,     1,     8,
       9,    10,    12,    14,    15,    16,    17,    18,    25,    26,
      27,    28,    37,    38,    40,    41,    44,    47,    48,    50,
      51,    54,    55,    61,    71,    93,    94,   105,   106,   120,
     121,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     146,   162,   163,   164,   166,   169,   170,   179,   181,   182,
     183,   185,   195,   197,   207,   208,   211,   212,   213,   214,
     215,   216,   232,   233,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   253,   256,   259,   263,
     264,   265,   266,   267,   268,   269,   274,   277,   314,   351,
     355,   360,   362,   364,    66,    70,    87,    88,    89,    90,
     315,   301,   236,   184,   185,   295,   153,   156,   159,   294,
     217,   325,   327,   329,   323,   145,   320,   331,   333,   335,
     337,   339,   341,   343,   345,    32,    32,   159,   177,   186,
     178,   277,   145,   185,   322,   185,   289,   220,    46,   292,
     313,   180,   183,   322,   277,   313,   322,   351,   179,   160,
     114,   183,   235,   265,   277,   232,   277,   179,   277,   160,
     160,   179,   183,   160,   139,    54,   277,   232,   160,   114,
     235,   277,   277,   277,   182,   263,   263,    12,   322,    12,
     322,   277,   357,   361,   194,   277,   277,   277,   236,   277,
     277,   277,   182,    36,   183,   183,   277,   145,   179,   183,
     183,   183,   295,   224,   119,   183,    20,    21,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   122,   123,   124,   125,   126,   127,   130,   131,
     132,   133,   134,   135,   154,   155,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   176,   177,   179,   184,
     145,   185,   283,   145,   322,   114,   123,   154,   293,   322,
     322,   322,   322,   180,   318,   322,   225,   322,   225,   322,
     225,   145,   287,   288,   322,   290,   178,   178,   178,   292,
     114,   154,   156,   159,   291,   285,   232,    19,   179,   236,
     254,   235,   277,   114,   154,   183,    13,   277,   232,   247,
     249,   277,   251,   156,   139,   322,   235,   277,   183,   182,
     313,    49,   145,   162,   179,   277,   352,   353,   354,   356,
     357,   358,   359,   313,   179,   353,   359,   128,   183,   185,
     149,   150,   192,   198,   180,   160,   236,   184,   232,   177,
     180,   257,   277,   136,   262,   263,    18,   145,   145,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   145,   120,   121,   277,   277,
     145,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   120,   121,   277,    21,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     145,   275,   277,   180,   257,   301,   154,   183,   177,   185,
     224,   280,   282,   154,   183,   293,   235,   277,   161,   183,
     161,   161,   161,   183,   161,   226,   161,   226,   161,   226,
     156,   161,   183,   161,   183,   178,   322,   235,   277,   257,
     180,   257,   160,   277,   322,   183,   232,   180,   322,   322,
     180,   322,   161,   183,    19,   263,   123,   291,   180,   153,
     183,   354,   178,   153,   178,   183,    19,   180,   354,   185,
     277,   357,   185,   277,   147,   196,   270,   272,   234,    11,
      22,    23,   209,   210,   145,   352,   153,   180,   114,   123,
     154,   159,   260,   261,   225,   160,   179,   277,   277,   178,
     277,   277,   145,   156,     1,   178,   180,   185,   277,   223,
       7,    42,   281,   183,   235,   277,   183,   326,   322,   330,
     324,   321,   332,   161,   336,   161,   340,   161,   322,   344,
     287,   346,   154,   291,   183,   180,   145,   180,   161,   161,
     161,   161,   262,   257,   277,   277,   353,   354,   145,   352,
     178,   178,   277,   178,   358,   257,   353,   185,   178,   151,
     147,   322,   225,   185,   232,   277,   178,   277,   145,   145,
     145,   145,   153,   178,   226,   278,   180,   257,   277,   276,
     183,   178,    43,   281,   292,   183,   161,   334,   338,   342,
     277,   232,   161,   183,   255,   248,   250,   252,   179,   183,
     354,   178,   123,   291,   178,   183,   354,   178,   161,   226,
     232,   180,   260,   178,   128,   232,   258,   322,   180,   183,
     284,   231,   328,   277,   145,   277,   277,   277,   180,   277,
     277,   178,   178,   277,   277,   277,   185,   271,   161,   210,
     114,   277,   161,   230,   180,   161,   180,   183,   363,   178,
     363,   178,   236,   273,   277,   279,   183,   277,    49,   178,
     185,   236,   180,   277,   178,   178
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   187,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   189,   190,   190,
     191,   192,   192,   192,   192,   193,   194,   194,   194,   195,
     196,   196,   198,   197,   199,   200,   201,   201,   201,   202,
     202,   203,   203,   204,   205,   205,   206,   206,   207,   208,
     208,   209,   209,   210,   210,   210,   211,   211,   212,   213,
     214,   215,   216,   217,   217,   217,   217,   217,   217,   218,
     218,   219,   219,   219,   219,   219,   219,   219,   219,   220,
     220,   221,   221,   221,   222,   222,   223,   223,   224,   224,
     225,   225,   225,   226,   226,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   228,   229,
     229,   229,   230,   231,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   234,   235,   235,
     235,   236,   236,   236,   237,   238,   238,   238,   238,   239,
     240,   241,   241,   241,   241,   241,   242,   242,   242,   242,
     243,   244,   244,   245,   247,   248,   246,   249,   250,   246,
     251,   252,   246,   254,   255,   253,   256,   256,   256,   257,
     257,   258,   258,   258,   259,   259,   259,   260,   260,   260,
     260,   261,   261,   262,   262,   263,   263,   264,   264,   264,
     264,   264,   264,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   266,   266,   266,   266,   267,   268,   268,   269,
     270,   271,   269,   272,   273,   269,   274,   275,   276,   274,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   278,   279,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     280,   280,   281,   281,   282,   283,   283,   284,   283,   283,
     283,   285,   285,   286,   286,   287,   287,   288,   288,   289,
     290,   290,   291,   291,   292,   292,   292,   292,   292,   292,
     293,   293,   293,   294,   294,   295,   295,   295,   295,   295,
     296,   296,   297,   297,   298,   298,   298,   299,   300,   299,
     301,   301,   301,   302,   303,   303,   304,   305,   305,   305,
     306,   306,   307,   307,   308,   309,   309,   310,   310,   310,
     312,   311,   313,   313,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   315,
     315,   315,   315,   315,   315,   316,   317,   317,   318,   318,
     320,   321,   319,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     323,   324,   322,   322,   325,   326,   322,   327,   328,   322,
     329,   330,   322,   322,   331,   332,   322,   333,   334,   322,
     322,   335,   336,   322,   337,   338,   322,   322,   339,   340,
     322,   341,   342,   322,   343,   344,   322,   345,   346,   322,
     348,   347,   350,   349,   351,   351,   351,   351,   352,   352,
     352,   352,   353,   353,   354,   354,   355,   355,   355,   355,
     355,   355,   356,   356,   357,   358,   358,   359,   359,   360,
     360,   361,   361,   362,   363,   363,   364,   364
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     3,     3,     2,
       4,     0,     1,     2,     1,     3,     1,     3,     3,     3,
       2,     1,     1,     0,     2,     4,     1,     1,     4,     6,
       2,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     1,     5,     1,
       3,     1,     1,     1,     1,     4,     1,     3,     0,     3,
       0,     2,     3,     0,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     0,
       1,     1,     4,     2,     3,     7,     1,     1,     1,     2,
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
       0,     0,     8,     0,     0,     9,     3,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       4,     3,     1,     3,     4,     3,     4,     2,     4,     4,
       7,     8,     3,     5,     0,     0,     8,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     4,     0,     1,     3,     0,     3,     0,     7,     5,
       5,     2,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     4,     6,     5,     5,     4,
       0,     2,     0,     1,     0,     1,     1,     6,     0,     5,
       0,     3,     5,     4,     1,     2,     4,     0,     1,     1,
       7,     9,     0,     2,     2,     1,     1,     0,     1,     1,
       0,     8,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       0,     0,     6,     1,     1,     1,     1,     4,     3,     4,
       2,     2,     3,     2,     3,     2,     2,     3,     3,     2,
       0,     0,     6,     2,     0,     0,     6,     0,     0,     8,
       0,     0,     6,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     0,     0,     6,     0,     0,     6,
       0,     7,     0,     7,     1,     1,     1,     1,     3,     3,
       5,     5,     1,     3,     0,     2,     6,     5,     7,     8,
       6,     8,     1,     3,     3,     1,     1,     1,     3,     5,
       5,     1,     3,     4,     0,     3,    10,    10
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3385 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3391 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3397 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3403 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3409 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3415 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3421 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3427 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3433 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3439 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3445 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3451 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3457 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3463 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3469 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3475 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3481 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3487 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3493 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3499 "ds_parser.cpp"
        break;

    case 221: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3505 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3511 "ds_parser.cpp"
        break;

    case 223: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3517 "ds_parser.cpp"
        break;

    case 224: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3523 "ds_parser.cpp"
        break;

    case 225: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3529 "ds_parser.cpp"
        break;

    case 226: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3535 "ds_parser.cpp"
        break;

    case 227: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3541 "ds_parser.cpp"
        break;

    case 232: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3547 "ds_parser.cpp"
        break;

    case 233: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3553 "ds_parser.cpp"
        break;

    case 234: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3559 "ds_parser.cpp"
        break;

    case 235: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3565 "ds_parser.cpp"
        break;

    case 236: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3571 "ds_parser.cpp"
        break;

    case 237: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3577 "ds_parser.cpp"
        break;

    case 238: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3583 "ds_parser.cpp"
        break;

    case 239: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3589 "ds_parser.cpp"
        break;

    case 240: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3595 "ds_parser.cpp"
        break;

    case 241: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3601 "ds_parser.cpp"
        break;

    case 242: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3607 "ds_parser.cpp"
        break;

    case 243: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3613 "ds_parser.cpp"
        break;

    case 245: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3619 "ds_parser.cpp"
        break;

    case 246: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3625 "ds_parser.cpp"
        break;

    case 253: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3631 "ds_parser.cpp"
        break;

    case 256: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3637 "ds_parser.cpp"
        break;

    case 257: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3643 "ds_parser.cpp"
        break;

    case 258: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3649 "ds_parser.cpp"
        break;

    case 260: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3655 "ds_parser.cpp"
        break;

    case 261: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3661 "ds_parser.cpp"
        break;

    case 262: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3667 "ds_parser.cpp"
        break;

    case 263: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3673 "ds_parser.cpp"
        break;

    case 264: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3679 "ds_parser.cpp"
        break;

    case 265: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3685 "ds_parser.cpp"
        break;

    case 266: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3691 "ds_parser.cpp"
        break;

    case 267: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3697 "ds_parser.cpp"
        break;

    case 268: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3703 "ds_parser.cpp"
        break;

    case 269: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3709 "ds_parser.cpp"
        break;

    case 274: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3715 "ds_parser.cpp"
        break;

    case 277: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3721 "ds_parser.cpp"
        break;

    case 280: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3727 "ds_parser.cpp"
        break;

    case 282: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3733 "ds_parser.cpp"
        break;

    case 283: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3739 "ds_parser.cpp"
        break;

    case 285: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3745 "ds_parser.cpp"
        break;

    case 286: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3751 "ds_parser.cpp"
        break;

    case 287: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3757 "ds_parser.cpp"
        break;

    case 288: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3763 "ds_parser.cpp"
        break;

    case 289: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3769 "ds_parser.cpp"
        break;

    case 290: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3775 "ds_parser.cpp"
        break;

    case 292: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3781 "ds_parser.cpp"
        break;

    case 295: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3787 "ds_parser.cpp"
        break;

    case 296: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3793 "ds_parser.cpp"
        break;

    case 301: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3799 "ds_parser.cpp"
        break;

    case 307: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3805 "ds_parser.cpp"
        break;

    case 313: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3811 "ds_parser.cpp"
        break;

    case 316: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3817 "ds_parser.cpp"
        break;

    case 317: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3823 "ds_parser.cpp"
        break;

    case 318: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3829 "ds_parser.cpp"
        break;

    case 319: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3835 "ds_parser.cpp"
        break;

    case 322: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3841 "ds_parser.cpp"
        break;

    case 351: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3847 "ds_parser.cpp"
        break;

    case 352: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3853 "ds_parser.cpp"
        break;

    case 353: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3859 "ds_parser.cpp"
        break;

    case 354: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3865 "ds_parser.cpp"
        break;

    case 355: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3871 "ds_parser.cpp"
        break;

    case 356: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3877 "ds_parser.cpp"
        break;

    case 357: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3883 "ds_parser.cpp"
        break;

    case 358: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3889 "ds_parser.cpp"
        break;

    case 359: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3895 "ds_parser.cpp"
        break;

    case 360: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3901 "ds_parser.cpp"
        break;

    case 361: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3907 "ds_parser.cpp"
        break;

    case 362: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3913 "ds_parser.cpp"
        break;

    case 363: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3919 "ds_parser.cpp"
        break;

    case 364: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3925 "ds_parser.cpp"
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
#line 468 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4217 "ds_parser.cpp"
    break;

  case 16:
#line 469 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4223 "ds_parser.cpp"
    break;

  case 17:
#line 470 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4229 "ds_parser.cpp"
    break;

  case 18:
#line 474 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4235 "ds_parser.cpp"
    break;

  case 19:
#line 475 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4241 "ds_parser.cpp"
    break;

  case 20:
#line 479 "ds_parser.ypp"
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
#line 4260 "ds_parser.cpp"
    break;

  case 21:
#line 496 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4266 "ds_parser.cpp"
    break;

  case 22:
#line 497 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4272 "ds_parser.cpp"
    break;

  case 23:
#line 498 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4278 "ds_parser.cpp"
    break;

  case 24:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4284 "ds_parser.cpp"
    break;

  case 25:
#line 503 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4290 "ds_parser.cpp"
    break;

  case 26:
#line 507 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4300 "ds_parser.cpp"
    break;

  case 27:
#line 512 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4314 "ds_parser.cpp"
    break;

  case 28:
#line 521 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4324 "ds_parser.cpp"
    break;

  case 29:
#line 529 "ds_parser.ypp"
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
#line 4342 "ds_parser.cpp"
    break;

  case 30:
#line 545 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4352 "ds_parser.cpp"
    break;

  case 31:
#line 550 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4362 "ds_parser.cpp"
    break;

  case 32:
#line 558 "ds_parser.ypp"
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
#line 4389 "ds_parser.cpp"
    break;

  case 33:
#line 579 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4401 "ds_parser.cpp"
    break;

  case 34:
#line 588 "ds_parser.ypp"
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
#line 4425 "ds_parser.cpp"
    break;

  case 36:
#line 614 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4433 "ds_parser.cpp"
    break;

  case 37:
#line 617 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4444 "ds_parser.cpp"
    break;

  case 38:
#line 623 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4455 "ds_parser.cpp"
    break;

  case 39:
#line 632 "ds_parser.ypp"
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
#line 4481 "ds_parser.cpp"
    break;

  case 40:
#line 653 "ds_parser.ypp"
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
#line 4506 "ds_parser.cpp"
    break;

  case 41:
#line 676 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4512 "ds_parser.cpp"
    break;

  case 42:
#line 677 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4518 "ds_parser.cpp"
    break;

  case 46:
#line 690 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4526 "ds_parser.cpp"
    break;

  case 47:
#line 693 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4534 "ds_parser.cpp"
    break;

  case 48:
#line 699 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4542 "ds_parser.cpp"
    break;

  case 49:
#line 705 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4550 "ds_parser.cpp"
    break;

  case 50:
#line 708 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4558 "ds_parser.cpp"
    break;

  case 51:
#line 714 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4564 "ds_parser.cpp"
    break;

  case 52:
#line 715 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4570 "ds_parser.cpp"
    break;

  case 53:
#line 719 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4576 "ds_parser.cpp"
    break;

  case 54:
#line 720 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4582 "ds_parser.cpp"
    break;

  case 55:
#line 721 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4593 "ds_parser.cpp"
    break;

  case 56:
#line 730 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4599 "ds_parser.cpp"
    break;

  case 57:
#line 731 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4605 "ds_parser.cpp"
    break;

  case 58:
#line 735 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4616 "ds_parser.cpp"
    break;

  case 59:
#line 744 "ds_parser.ypp"
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
#line 4634 "ds_parser.cpp"
    break;

  case 60:
#line 760 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4644 "ds_parser.cpp"
    break;

  case 61:
#line 768 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4656 "ds_parser.cpp"
    break;

  case 62:
#line 778 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4667 "ds_parser.cpp"
    break;

  case 63:
#line 787 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4673 "ds_parser.cpp"
    break;

  case 64:
#line 788 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4679 "ds_parser.cpp"
    break;

  case 65:
#line 789 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4685 "ds_parser.cpp"
    break;

  case 66:
#line 790 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4691 "ds_parser.cpp"
    break;

  case 67:
#line 791 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4697 "ds_parser.cpp"
    break;

  case 68:
#line 792 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4703 "ds_parser.cpp"
    break;

  case 69:
#line 796 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4713 "ds_parser.cpp"
    break;

  case 70:
#line 801 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4723 "ds_parser.cpp"
    break;

  case 71:
#line 809 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4729 "ds_parser.cpp"
    break;

  case 72:
#line 810 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4735 "ds_parser.cpp"
    break;

  case 73:
#line 811 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4741 "ds_parser.cpp"
    break;

  case 74:
#line 812 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4747 "ds_parser.cpp"
    break;

  case 75:
#line 813 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4753 "ds_parser.cpp"
    break;

  case 76:
#line 814 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4759 "ds_parser.cpp"
    break;

  case 77:
#line 815 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4765 "ds_parser.cpp"
    break;

  case 78:
#line 816 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4773 "ds_parser.cpp"
    break;

  case 79:
#line 822 "ds_parser.ypp"
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
#line 4792 "ds_parser.cpp"
    break;

  case 80:
#line 836 "ds_parser.ypp"
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
#line 4811 "ds_parser.cpp"
    break;

  case 81:
#line 853 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4817 "ds_parser.cpp"
    break;

  case 82:
#line 854 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4823 "ds_parser.cpp"
    break;

  case 83:
#line 855 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4829 "ds_parser.cpp"
    break;

  case 84:
#line 859 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4842 "ds_parser.cpp"
    break;

  case 85:
#line 867 "ds_parser.ypp"
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
#line 4857 "ds_parser.cpp"
    break;

  case 86:
#line 880 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4866 "ds_parser.cpp"
    break;

  case 87:
#line 884 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4875 "ds_parser.cpp"
    break;

  case 88:
#line 891 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4881 "ds_parser.cpp"
    break;

  case 89:
#line 892 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4887 "ds_parser.cpp"
    break;

  case 90:
#line 896 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4893 "ds_parser.cpp"
    break;

  case 91:
#line 897 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4899 "ds_parser.cpp"
    break;

  case 92:
#line 898 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4905 "ds_parser.cpp"
    break;

  case 93:
#line 902 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4913 "ds_parser.cpp"
    break;

  case 94:
#line 905 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4922 "ds_parser.cpp"
    break;

  case 95:
#line 912 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4931 "ds_parser.cpp"
    break;

  case 96:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4937 "ds_parser.cpp"
    break;

  case 97:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4943 "ds_parser.cpp"
    break;

  case 98:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4949 "ds_parser.cpp"
    break;

  case 99:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4955 "ds_parser.cpp"
    break;

  case 100:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4961 "ds_parser.cpp"
    break;

  case 101:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4967 "ds_parser.cpp"
    break;

  case 102:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4973 "ds_parser.cpp"
    break;

  case 103:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4979 "ds_parser.cpp"
    break;

  case 104:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4985 "ds_parser.cpp"
    break;

  case 105:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4991 "ds_parser.cpp"
    break;

  case 106:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4997 "ds_parser.cpp"
    break;

  case 107:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5003 "ds_parser.cpp"
    break;

  case 108:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5009 "ds_parser.cpp"
    break;

  case 109:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5015 "ds_parser.cpp"
    break;

  case 110:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5021 "ds_parser.cpp"
    break;

  case 111:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5027 "ds_parser.cpp"
    break;

  case 112:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5033 "ds_parser.cpp"
    break;

  case 113:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5039 "ds_parser.cpp"
    break;

  case 114:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5045 "ds_parser.cpp"
    break;

  case 115:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5051 "ds_parser.cpp"
    break;

  case 116:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5057 "ds_parser.cpp"
    break;

  case 117:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5063 "ds_parser.cpp"
    break;

  case 118:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5069 "ds_parser.cpp"
    break;

  case 119:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5075 "ds_parser.cpp"
    break;

  case 120:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5081 "ds_parser.cpp"
    break;

  case 121:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5087 "ds_parser.cpp"
    break;

  case 122:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5093 "ds_parser.cpp"
    break;

  case 123:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5099 "ds_parser.cpp"
    break;

  case 124:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5105 "ds_parser.cpp"
    break;

  case 125:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5111 "ds_parser.cpp"
    break;

  case 126:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5117 "ds_parser.cpp"
    break;

  case 127:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5123 "ds_parser.cpp"
    break;

  case 128:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5129 "ds_parser.cpp"
    break;

  case 129:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5135 "ds_parser.cpp"
    break;

  case 130:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5141 "ds_parser.cpp"
    break;

  case 131:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5147 "ds_parser.cpp"
    break;

  case 132:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5153 "ds_parser.cpp"
    break;

  case 133:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5159 "ds_parser.cpp"
    break;

  case 134:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5165 "ds_parser.cpp"
    break;

  case 135:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5171 "ds_parser.cpp"
    break;

  case 136:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5177 "ds_parser.cpp"
    break;

  case 137:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5183 "ds_parser.cpp"
    break;

  case 138:
#line 961 "ds_parser.ypp"
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
#line 5207 "ds_parser.cpp"
    break;

  case 139:
#line 983 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5213 "ds_parser.cpp"
    break;

  case 140:
#line 984 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5219 "ds_parser.cpp"
    break;

  case 141:
#line 985 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5225 "ds_parser.cpp"
    break;

  case 142:
#line 989 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-2]));
        pFunction->name = *(yyvsp[-2].s);
        pFunction->result = TypeDeclPtr((yyvsp[0].pTypeDecl));
        pFunction->privateFunction = !(yyvsp[-3].b);
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
#line 5266 "ds_parser.cpp"
    break;

  case 143:
#line 1028 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5275 "ds_parser.cpp"
    break;

  case 144:
#line 1035 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5284 "ds_parser.cpp"
    break;

  case 145:
#line 1039 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5297 "ds_parser.cpp"
    break;

  case 146:
#line 1050 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5303 "ds_parser.cpp"
    break;

  case 147:
#line 1051 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5309 "ds_parser.cpp"
    break;

  case 148:
#line 1052 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5315 "ds_parser.cpp"
    break;

  case 149:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5321 "ds_parser.cpp"
    break;

  case 150:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5327 "ds_parser.cpp"
    break;

  case 151:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5333 "ds_parser.cpp"
    break;

  case 152:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5339 "ds_parser.cpp"
    break;

  case 153:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5345 "ds_parser.cpp"
    break;

  case 154:
#line 1058 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5351 "ds_parser.cpp"
    break;

  case 155:
#line 1059 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5357 "ds_parser.cpp"
    break;

  case 156:
#line 1060 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5363 "ds_parser.cpp"
    break;

  case 157:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5369 "ds_parser.cpp"
    break;

  case 158:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5375 "ds_parser.cpp"
    break;

  case 159:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5381 "ds_parser.cpp"
    break;

  case 160:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5387 "ds_parser.cpp"
    break;

  case 161:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5393 "ds_parser.cpp"
    break;

  case 162:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5399 "ds_parser.cpp"
    break;

  case 163:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5405 "ds_parser.cpp"
    break;

  case 164:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5411 "ds_parser.cpp"
    break;

  case 165:
#line 1072 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5421 "ds_parser.cpp"
    break;

  case 166:
#line 1077 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5432 "ds_parser.cpp"
    break;

  case 167:
#line 1083 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5440 "ds_parser.cpp"
    break;

  case 168:
#line 1089 "ds_parser.ypp"
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
#line 5479 "ds_parser.cpp"
    break;

  case 169:
#line 1123 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5487 "ds_parser.cpp"
    break;

  case 170:
#line 1126 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5495 "ds_parser.cpp"
    break;

  case 171:
#line 1132 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5501 "ds_parser.cpp"
    break;

  case 172:
#line 1133 "ds_parser.ypp"
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
#line 5517 "ds_parser.cpp"
    break;

  case 173:
#line 1144 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5523 "ds_parser.cpp"
    break;

  case 174:
#line 1148 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5531 "ds_parser.cpp"
    break;

  case 175:
#line 1154 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5539 "ds_parser.cpp"
    break;

  case 176:
#line 1157 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5547 "ds_parser.cpp"
    break;

  case 177:
#line 1160 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5556 "ds_parser.cpp"
    break;

  case 178:
#line 1164 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5564 "ds_parser.cpp"
    break;

  case 179:
#line 1170 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5570 "ds_parser.cpp"
    break;

  case 180:
#line 1174 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5576 "ds_parser.cpp"
    break;

  case 181:
#line 1178 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5584 "ds_parser.cpp"
    break;

  case 182:
#line 1181 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5592 "ds_parser.cpp"
    break;

  case 183:
#line 1184 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5602 "ds_parser.cpp"
    break;

  case 184:
#line 1189 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5610 "ds_parser.cpp"
    break;

  case 185:
#line 1192 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5620 "ds_parser.cpp"
    break;

  case 186:
#line 1200 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5628 "ds_parser.cpp"
    break;

  case 187:
#line 1203 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5638 "ds_parser.cpp"
    break;

  case 188:
#line 1208 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5646 "ds_parser.cpp"
    break;

  case 189:
#line 1211 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5656 "ds_parser.cpp"
    break;

  case 190:
#line 1219 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5664 "ds_parser.cpp"
    break;

  case 191:
#line 1225 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5670 "ds_parser.cpp"
    break;

  case 192:
#line 1226 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5676 "ds_parser.cpp"
    break;

  case 193:
#line 1230 "ds_parser.ypp"
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
#line 5712 "ds_parser.cpp"
    break;

  case 194:
#line 1264 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5718 "ds_parser.cpp"
    break;

  case 195:
#line 1264 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5724 "ds_parser.cpp"
    break;

  case 196:
#line 1264 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5732 "ds_parser.cpp"
    break;

  case 197:
#line 1267 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5738 "ds_parser.cpp"
    break;

  case 198:
#line 1267 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5744 "ds_parser.cpp"
    break;

  case 199:
#line 1267 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5754 "ds_parser.cpp"
    break;

  case 200:
#line 1272 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5760 "ds_parser.cpp"
    break;

  case 201:
#line 1272 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5766 "ds_parser.cpp"
    break;

  case 202:
#line 1272 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5776 "ds_parser.cpp"
    break;

  case 203:
#line 1280 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5782 "ds_parser.cpp"
    break;

  case 204:
#line 1280 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5788 "ds_parser.cpp"
    break;

  case 205:
#line 1280 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5796 "ds_parser.cpp"
    break;

  case 206:
#line 1286 "ds_parser.ypp"
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
#line 5811 "ds_parser.cpp"
    break;

  case 207:
#line 1296 "ds_parser.ypp"
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
#line 5827 "ds_parser.cpp"
    break;

  case 208:
#line 1307 "ds_parser.ypp"
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
#line 5844 "ds_parser.cpp"
    break;

  case 209:
#line 1322 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5852 "ds_parser.cpp"
    break;

  case 210:
#line 1325 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5860 "ds_parser.cpp"
    break;

  case 211:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5866 "ds_parser.cpp"
    break;

  case 212:
#line 1332 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5878 "ds_parser.cpp"
    break;

  case 213:
#line 1339 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5891 "ds_parser.cpp"
    break;

  case 214:
#line 1350 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5897 "ds_parser.cpp"
    break;

  case 215:
#line 1351 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5903 "ds_parser.cpp"
    break;

  case 216:
#line 1352 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5909 "ds_parser.cpp"
    break;

  case 217:
#line 1356 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5915 "ds_parser.cpp"
    break;

  case 218:
#line 1357 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5921 "ds_parser.cpp"
    break;

  case 219:
#line 1358 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5927 "ds_parser.cpp"
    break;

  case 220:
#line 1359 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5933 "ds_parser.cpp"
    break;

  case 221:
#line 1363 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5943 "ds_parser.cpp"
    break;

  case 222:
#line 1368 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5953 "ds_parser.cpp"
    break;

  case 223:
#line 1376 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5959 "ds_parser.cpp"
    break;

  case 224:
#line 1377 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5965 "ds_parser.cpp"
    break;

  case 225:
#line 1381 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5975 "ds_parser.cpp"
    break;

  case 226:
#line 1387 "ds_parser.ypp"
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
#line 6040 "ds_parser.cpp"
    break;

  case 227:
#line 1450 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6046 "ds_parser.cpp"
    break;

  case 228:
#line 1451 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6052 "ds_parser.cpp"
    break;

  case 229:
#line 1452 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6058 "ds_parser.cpp"
    break;

  case 230:
#line 1453 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6064 "ds_parser.cpp"
    break;

  case 231:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6070 "ds_parser.cpp"
    break;

  case 232:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6076 "ds_parser.cpp"
    break;

  case 233:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6082 "ds_parser.cpp"
    break;

  case 234:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6088 "ds_parser.cpp"
    break;

  case 235:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6094 "ds_parser.cpp"
    break;

  case 236:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6100 "ds_parser.cpp"
    break;

  case 237:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6106 "ds_parser.cpp"
    break;

  case 238:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6112 "ds_parser.cpp"
    break;

  case 239:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6118 "ds_parser.cpp"
    break;

  case 240:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6124 "ds_parser.cpp"
    break;

  case 241:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6130 "ds_parser.cpp"
    break;

  case 242:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6136 "ds_parser.cpp"
    break;

  case 243:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6142 "ds_parser.cpp"
    break;

  case 244:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6148 "ds_parser.cpp"
    break;

  case 245:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6154 "ds_parser.cpp"
    break;

  case 246:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6160 "ds_parser.cpp"
    break;

  case 247:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6166 "ds_parser.cpp"
    break;

  case 248:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6172 "ds_parser.cpp"
    break;

  case 249:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6178 "ds_parser.cpp"
    break;

  case 250:
#line 1476 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6184 "ds_parser.cpp"
    break;

  case 251:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6190 "ds_parser.cpp"
    break;

  case 252:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6196 "ds_parser.cpp"
    break;

  case 253:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6202 "ds_parser.cpp"
    break;

  case 254:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6208 "ds_parser.cpp"
    break;

  case 255:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6214 "ds_parser.cpp"
    break;

  case 256:
#line 1488 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6226 "ds_parser.cpp"
    break;

  case 257:
#line 1499 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6236 "ds_parser.cpp"
    break;

  case 258:
#line 1504 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6248 "ds_parser.cpp"
    break;

  case 259:
#line 1514 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6257 "ds_parser.cpp"
    break;

  case 260:
#line 1518 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6263 "ds_parser.cpp"
    break;

  case 261:
#line 1518 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6269 "ds_parser.cpp"
    break;

  case 262:
#line 1518 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6281 "ds_parser.cpp"
    break;

  case 263:
#line 1525 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6287 "ds_parser.cpp"
    break;

  case 264:
#line 1525 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6293 "ds_parser.cpp"
    break;

  case 265:
#line 1525 "ds_parser.ypp"
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
#line 6309 "ds_parser.cpp"
    break;

  case 266:
#line 1539 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6318 "ds_parser.cpp"
    break;

  case 267:
#line 1543 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6324 "ds_parser.cpp"
    break;

  case 268:
#line 1543 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6330 "ds_parser.cpp"
    break;

  case 269:
#line 1543 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6339 "ds_parser.cpp"
    break;

  case 270:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6345 "ds_parser.cpp"
    break;

  case 271:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6351 "ds_parser.cpp"
    break;

  case 272:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6357 "ds_parser.cpp"
    break;

  case 273:
#line 1553 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6363 "ds_parser.cpp"
    break;

  case 274:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6369 "ds_parser.cpp"
    break;

  case 275:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6375 "ds_parser.cpp"
    break;

  case 276:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6381 "ds_parser.cpp"
    break;

  case 277:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6387 "ds_parser.cpp"
    break;

  case 278:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6393 "ds_parser.cpp"
    break;

  case 279:
#line 1559 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6399 "ds_parser.cpp"
    break;

  case 280:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6405 "ds_parser.cpp"
    break;

  case 281:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6411 "ds_parser.cpp"
    break;

  case 282:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6417 "ds_parser.cpp"
    break;

  case 283:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6423 "ds_parser.cpp"
    break;

  case 284:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6429 "ds_parser.cpp"
    break;

  case 285:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6435 "ds_parser.cpp"
    break;

  case 286:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6441 "ds_parser.cpp"
    break;

  case 287:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6447 "ds_parser.cpp"
    break;

  case 288:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 289:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 290:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 291:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 292:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 293:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 294:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 295:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 296:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 297:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 298:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 299:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 300:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 301:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 302:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 303:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 304:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 305:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 306:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 307:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 308:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6573 "ds_parser.cpp"
    break;

  case 309:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 310:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 311:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6591 "ds_parser.cpp"
    break;

  case 312:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6597 "ds_parser.cpp"
    break;

  case 313:
#line 1593 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6606 "ds_parser.cpp"
    break;

  case 314:
#line 1597 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6615 "ds_parser.cpp"
    break;

  case 315:
#line 1601 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6623 "ds_parser.cpp"
    break;

  case 316:
#line 1604 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6631 "ds_parser.cpp"
    break;

  case 317:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6637 "ds_parser.cpp"
    break;

  case 318:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6643 "ds_parser.cpp"
    break;

  case 319:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6649 "ds_parser.cpp"
    break;

  case 320:
#line 1610 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6663 "ds_parser.cpp"
    break;

  case 321:
#line 1619 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6677 "ds_parser.cpp"
    break;

  case 322:
#line 1628 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6683 "ds_parser.cpp"
    break;

  case 323:
#line 1629 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6691 "ds_parser.cpp"
    break;

  case 324:
#line 1632 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6697 "ds_parser.cpp"
    break;

  case 325:
#line 1632 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6703 "ds_parser.cpp"
    break;

  case 326:
#line 1632 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6711 "ds_parser.cpp"
    break;

  case 327:
#line 1635 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6720 "ds_parser.cpp"
    break;

  case 328:
#line 1639 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6729 "ds_parser.cpp"
    break;

  case 329:
#line 1643 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6738 "ds_parser.cpp"
    break;

  case 330:
#line 1647 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6744 "ds_parser.cpp"
    break;

  case 331:
#line 1648 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6750 "ds_parser.cpp"
    break;

  case 332:
#line 1649 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6756 "ds_parser.cpp"
    break;

  case 333:
#line 1650 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6762 "ds_parser.cpp"
    break;

  case 334:
#line 1651 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6768 "ds_parser.cpp"
    break;

  case 335:
#line 1652 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6774 "ds_parser.cpp"
    break;

  case 336:
#line 1653 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6780 "ds_parser.cpp"
    break;

  case 337:
#line 1654 "ds_parser.ypp"
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
#line 6801 "ds_parser.cpp"
    break;

  case 338:
#line 1670 "ds_parser.ypp"
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
#line 6822 "ds_parser.cpp"
    break;

  case 339:
#line 1686 "ds_parser.ypp"
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
#line 6882 "ds_parser.cpp"
    break;

  case 340:
#line 1744 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6888 "ds_parser.cpp"
    break;

  case 341:
#line 1745 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6894 "ds_parser.cpp"
    break;

  case 342:
#line 1749 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6900 "ds_parser.cpp"
    break;

  case 343:
#line 1750 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6906 "ds_parser.cpp"
    break;

  case 344:
#line 1754 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6916 "ds_parser.cpp"
    break;

  case 345:
#line 1762 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6924 "ds_parser.cpp"
    break;

  case 346:
#line 1765 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6933 "ds_parser.cpp"
    break;

  case 347:
#line 1770 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6939 "ds_parser.cpp"
    break;

  case 348:
#line 1770 "ds_parser.ypp"
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
#line 6988 "ds_parser.cpp"
    break;

  case 349:
#line 1814 "ds_parser.ypp"
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
            assignDefaultArguments((yyvsp[0].pFuncDecl));
            runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt((yylsp[-3])));
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[0].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7049 "ds_parser.cpp"
    break;

  case 350:
#line 1870 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7060 "ds_parser.cpp"
    break;

  case 351:
#line 1879 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7070 "ds_parser.cpp"
    break;

  case 352:
#line 1884 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7080 "ds_parser.cpp"
    break;

  case 353:
#line 1892 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7086 "ds_parser.cpp"
    break;

  case 354:
#line 1893 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7092 "ds_parser.cpp"
    break;

  case 355:
#line 1897 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7100 "ds_parser.cpp"
    break;

  case 356:
#line 1900 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7111 "ds_parser.cpp"
    break;

  case 357:
#line 1909 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7117 "ds_parser.cpp"
    break;

  case 358:
#line 1910 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7123 "ds_parser.cpp"
    break;

  case 359:
#line 1914 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7134 "ds_parser.cpp"
    break;

  case 360:
#line 1923 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7140 "ds_parser.cpp"
    break;

  case 361:
#line 1924 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7146 "ds_parser.cpp"
    break;

  case 362:
#line 1929 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7152 "ds_parser.cpp"
    break;

  case 363:
#line 1930 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7158 "ds_parser.cpp"
    break;

  case 364:
#line 1934 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7169 "ds_parser.cpp"
    break;

  case 365:
#line 1940 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7180 "ds_parser.cpp"
    break;

  case 366:
#line 1946 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7188 "ds_parser.cpp"
    break;

  case 367:
#line 1949 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7197 "ds_parser.cpp"
    break;

  case 368:
#line 1953 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7208 "ds_parser.cpp"
    break;

  case 369:
#line 1959 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7219 "ds_parser.cpp"
    break;

  case 370:
#line 1968 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7225 "ds_parser.cpp"
    break;

  case 371:
#line 1969 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7231 "ds_parser.cpp"
    break;

  case 372:
#line 1970 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7237 "ds_parser.cpp"
    break;

  case 373:
#line 1974 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7243 "ds_parser.cpp"
    break;

  case 374:
#line 1975 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7249 "ds_parser.cpp"
    break;

  case 375:
#line 1979 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7257 "ds_parser.cpp"
    break;

  case 376:
#line 1982 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7267 "ds_parser.cpp"
    break;

  case 377:
#line 1987 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7277 "ds_parser.cpp"
    break;

  case 378:
#line 1992 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7290 "ds_parser.cpp"
    break;

  case 379:
#line 2000 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7303 "ds_parser.cpp"
    break;

  case 380:
#line 2011 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7311 "ds_parser.cpp"
    break;

  case 381:
#line 2014 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7320 "ds_parser.cpp"
    break;

  case 382:
#line 2021 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7326 "ds_parser.cpp"
    break;

  case 383:
#line 2022 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7332 "ds_parser.cpp"
    break;

  case 384:
#line 2026 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7338 "ds_parser.cpp"
    break;

  case 385:
#line 2027 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7344 "ds_parser.cpp"
    break;

  case 386:
#line 2028 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7350 "ds_parser.cpp"
    break;

  case 387:
#line 2032 "ds_parser.ypp"
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
#line 7383 "ds_parser.cpp"
    break;

  case 388:
#line 2060 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7389 "ds_parser.cpp"
    break;

  case 389:
#line 2060 "ds_parser.ypp"
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
                if ( (yyvsp[-4].b) ) {
                    pVar->type->constant = true;
                } else {
                    pVar->type->removeConstant = true;
                }
                pVar->global_shared = (yyvsp[-3].b);
                pVar->private_variable = !(yyvsp[-2].b);
                if ( !g_Program->addVariable(pVar) )
                    das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }
#line 7421 "ds_parser.cpp"
    break;

  case 390:
#line 2090 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7429 "ds_parser.cpp"
    break;

  case 391:
#line 2093 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7443 "ds_parser.cpp"
    break;

  case 392:
#line 2102 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7457 "ds_parser.cpp"
    break;

  case 393:
#line 2115 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7471 "ds_parser.cpp"
    break;

  case 397:
#line 2136 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7477 "ds_parser.cpp"
    break;

  case 398:
#line 2137 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7483 "ds_parser.cpp"
    break;

  case 399:
#line 2138 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7489 "ds_parser.cpp"
    break;

  case 400:
#line 2142 "ds_parser.ypp"
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
#line 7522 "ds_parser.cpp"
    break;

  case 401:
#line 2170 "ds_parser.ypp"
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
#line 7556 "ds_parser.cpp"
    break;

  case 402:
#line 2202 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7562 "ds_parser.cpp"
    break;

  case 403:
#line 2203 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7568 "ds_parser.cpp"
    break;

  case 404:
#line 2207 "ds_parser.ypp"
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
#line 7607 "ds_parser.cpp"
    break;

  case 405:
#line 2244 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7613 "ds_parser.cpp"
    break;

  case 406:
#line 2245 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7619 "ds_parser.cpp"
    break;

  case 407:
#line 2249 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7625 "ds_parser.cpp"
    break;

  case 408:
#line 2250 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7631 "ds_parser.cpp"
    break;

  case 409:
#line 2251 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7637 "ds_parser.cpp"
    break;

  case 410:
#line 2256 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7643 "ds_parser.cpp"
    break;

  case 411:
#line 2256 "ds_parser.ypp"
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
#line 7729 "ds_parser.cpp"
    break;

  case 412:
#line 2340 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7741 "ds_parser.cpp"
    break;

  case 413:
#line 2347 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7752 "ds_parser.cpp"
    break;

  case 414:
#line 2356 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7758 "ds_parser.cpp"
    break;

  case 415:
#line 2357 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7764 "ds_parser.cpp"
    break;

  case 416:
#line 2358 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7770 "ds_parser.cpp"
    break;

  case 417:
#line 2359 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7776 "ds_parser.cpp"
    break;

  case 418:
#line 2360 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7782 "ds_parser.cpp"
    break;

  case 419:
#line 2361 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7788 "ds_parser.cpp"
    break;

  case 420:
#line 2362 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7794 "ds_parser.cpp"
    break;

  case 421:
#line 2363 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7800 "ds_parser.cpp"
    break;

  case 422:
#line 2364 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7806 "ds_parser.cpp"
    break;

  case 423:
#line 2365 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7812 "ds_parser.cpp"
    break;

  case 424:
#line 2366 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7818 "ds_parser.cpp"
    break;

  case 425:
#line 2367 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7824 "ds_parser.cpp"
    break;

  case 426:
#line 2368 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7830 "ds_parser.cpp"
    break;

  case 427:
#line 2369 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7836 "ds_parser.cpp"
    break;

  case 428:
#line 2370 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7842 "ds_parser.cpp"
    break;

  case 429:
#line 2371 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7848 "ds_parser.cpp"
    break;

  case 430:
#line 2372 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7854 "ds_parser.cpp"
    break;

  case 431:
#line 2373 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7860 "ds_parser.cpp"
    break;

  case 432:
#line 2374 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7866 "ds_parser.cpp"
    break;

  case 433:
#line 2375 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7872 "ds_parser.cpp"
    break;

  case 434:
#line 2376 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7878 "ds_parser.cpp"
    break;

  case 435:
#line 2377 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7884 "ds_parser.cpp"
    break;

  case 436:
#line 2378 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7890 "ds_parser.cpp"
    break;

  case 437:
#line 2379 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7896 "ds_parser.cpp"
    break;

  case 438:
#line 2380 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7902 "ds_parser.cpp"
    break;

  case 439:
#line 2384 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7908 "ds_parser.cpp"
    break;

  case 440:
#line 2385 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7914 "ds_parser.cpp"
    break;

  case 441:
#line 2386 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7920 "ds_parser.cpp"
    break;

  case 442:
#line 2387 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7926 "ds_parser.cpp"
    break;

  case 443:
#line 2388 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7932 "ds_parser.cpp"
    break;

  case 444:
#line 2389 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7938 "ds_parser.cpp"
    break;

  case 445:
#line 2393 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7951 "ds_parser.cpp"
    break;

  case 446:
#line 2404 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7960 "ds_parser.cpp"
    break;

  case 447:
#line 2408 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7972 "ds_parser.cpp"
    break;

  case 448:
#line 2418 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7984 "ds_parser.cpp"
    break;

  case 449:
#line 2425 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7995 "ds_parser.cpp"
    break;

  case 450:
#line 2434 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8001 "ds_parser.cpp"
    break;

  case 451:
#line 2434 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8007 "ds_parser.cpp"
    break;

  case 452:
#line 2434 "ds_parser.ypp"
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
#line 8022 "ds_parser.cpp"
    break;

  case 453:
#line 2447 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8028 "ds_parser.cpp"
    break;

  case 454:
#line 2448 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8034 "ds_parser.cpp"
    break;

  case 455:
#line 2449 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8040 "ds_parser.cpp"
    break;

  case 456:
#line 2450 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8046 "ds_parser.cpp"
    break;

  case 457:
#line 2451 "ds_parser.ypp"
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
#line 8065 "ds_parser.cpp"
    break;

  case 458:
#line 2465 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8076 "ds_parser.cpp"
    break;

  case 459:
#line 2471 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8085 "ds_parser.cpp"
    break;

  case 460:
#line 2475 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8094 "ds_parser.cpp"
    break;

  case 461:
#line 2479 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8104 "ds_parser.cpp"
    break;

  case 462:
#line 2484 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8114 "ds_parser.cpp"
    break;

  case 463:
#line 2489 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8124 "ds_parser.cpp"
    break;

  case 464:
#line 2494 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8134 "ds_parser.cpp"
    break;

  case 465:
#line 2499 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8143 "ds_parser.cpp"
    break;

  case 466:
#line 2503 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8152 "ds_parser.cpp"
    break;

  case 467:
#line 2507 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8162 "ds_parser.cpp"
    break;

  case 468:
#line 2512 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8171 "ds_parser.cpp"
    break;

  case 469:
#line 2516 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8181 "ds_parser.cpp"
    break;

  case 470:
#line 2521 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8187 "ds_parser.cpp"
    break;

  case 471:
#line 2521 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8193 "ds_parser.cpp"
    break;

  case 472:
#line 2521 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8204 "ds_parser.cpp"
    break;

  case 473:
#line 2527 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8216 "ds_parser.cpp"
    break;

  case 474:
#line 2534 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8222 "ds_parser.cpp"
    break;

  case 475:
#line 2534 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8228 "ds_parser.cpp"
    break;

  case 476:
#line 2534 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8238 "ds_parser.cpp"
    break;

  case 477:
#line 2539 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8244 "ds_parser.cpp"
    break;

  case 478:
#line 2539 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8250 "ds_parser.cpp"
    break;

  case 479:
#line 2539 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8261 "ds_parser.cpp"
    break;

  case 480:
#line 2545 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8267 "ds_parser.cpp"
    break;

  case 481:
#line 2545 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8273 "ds_parser.cpp"
    break;

  case 482:
#line 2545 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8283 "ds_parser.cpp"
    break;

  case 483:
#line 2550 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8292 "ds_parser.cpp"
    break;

  case 484:
#line 2554 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8298 "ds_parser.cpp"
    break;

  case 485:
#line 2554 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8304 "ds_parser.cpp"
    break;

  case 486:
#line 2554 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8314 "ds_parser.cpp"
    break;

  case 487:
#line 2559 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8320 "ds_parser.cpp"
    break;

  case 488:
#line 2559 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8326 "ds_parser.cpp"
    break;

  case 489:
#line 2559 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8340 "ds_parser.cpp"
    break;

  case 490:
#line 2568 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8349 "ds_parser.cpp"
    break;

  case 491:
#line 2572 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8355 "ds_parser.cpp"
    break;

  case 492:
#line 2572 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8361 "ds_parser.cpp"
    break;

  case 493:
#line 2572 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8371 "ds_parser.cpp"
    break;

  case 494:
#line 2577 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8377 "ds_parser.cpp"
    break;

  case 495:
#line 2577 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8383 "ds_parser.cpp"
    break;

  case 496:
#line 2577 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8397 "ds_parser.cpp"
    break;

  case 497:
#line 2586 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8406 "ds_parser.cpp"
    break;

  case 498:
#line 2590 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8412 "ds_parser.cpp"
    break;

  case 499:
#line 2590 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8418 "ds_parser.cpp"
    break;

  case 500:
#line 2590 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8428 "ds_parser.cpp"
    break;

  case 501:
#line 2595 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8434 "ds_parser.cpp"
    break;

  case 502:
#line 2595 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8440 "ds_parser.cpp"
    break;

  case 503:
#line 2595 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8454 "ds_parser.cpp"
    break;

  case 504:
#line 2604 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8460 "ds_parser.cpp"
    break;

  case 505:
#line 2604 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8466 "ds_parser.cpp"
    break;

  case 506:
#line 2604 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8477 "ds_parser.cpp"
    break;

  case 507:
#line 2610 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8483 "ds_parser.cpp"
    break;

  case 508:
#line 2610 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8489 "ds_parser.cpp"
    break;

  case 509:
#line 2610 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8500 "ds_parser.cpp"
    break;

  case 510:
#line 2619 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8506 "ds_parser.cpp"
    break;

  case 511:
#line 2619 "ds_parser.ypp"
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
#line 8523 "ds_parser.cpp"
    break;

  case 512:
#line 2634 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8529 "ds_parser.cpp"
    break;

  case 513:
#line 2634 "ds_parser.ypp"
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
#line 8550 "ds_parser.cpp"
    break;

  case 514:
#line 2654 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8556 "ds_parser.cpp"
    break;

  case 515:
#line 2655 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8562 "ds_parser.cpp"
    break;

  case 516:
#line 2656 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8568 "ds_parser.cpp"
    break;

  case 517:
#line 2657 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8574 "ds_parser.cpp"
    break;

  case 518:
#line 2661 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8586 "ds_parser.cpp"
    break;

  case 519:
#line 2668 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8598 "ds_parser.cpp"
    break;

  case 520:
#line 2675 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8609 "ds_parser.cpp"
    break;

  case 521:
#line 2681 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8620 "ds_parser.cpp"
    break;

  case 522:
#line 2690 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8630 "ds_parser.cpp"
    break;

  case 523:
#line 2695 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8639 "ds_parser.cpp"
    break;

  case 524:
#line 2702 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8645 "ds_parser.cpp"
    break;

  case 525:
#line 2703 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8651 "ds_parser.cpp"
    break;

  case 526:
#line 2707 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8662 "ds_parser.cpp"
    break;

  case 527:
#line 2713 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8674 "ds_parser.cpp"
    break;

  case 528:
#line 2720 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8687 "ds_parser.cpp"
    break;

  case 529:
#line 2728 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8699 "ds_parser.cpp"
    break;

  case 530:
#line 2735 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8712 "ds_parser.cpp"
    break;

  case 531:
#line 2743 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8726 "ds_parser.cpp"
    break;

  case 532:
#line 2755 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8734 "ds_parser.cpp"
    break;

  case 533:
#line 2758 "ds_parser.ypp"
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
#line 8750 "ds_parser.cpp"
    break;

  case 534:
#line 2772 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8761 "ds_parser.cpp"
    break;

  case 535:
#line 2781 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8767 "ds_parser.cpp"
    break;

  case 536:
#line 2782 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8773 "ds_parser.cpp"
    break;

  case 537:
#line 2786 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8783 "ds_parser.cpp"
    break;

  case 538:
#line 2791 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8792 "ds_parser.cpp"
    break;

  case 539:
#line 2798 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8802 "ds_parser.cpp"
    break;

  case 540:
#line 2803 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8814 "ds_parser.cpp"
    break;

  case 541:
#line 2813 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8824 "ds_parser.cpp"
    break;

  case 542:
#line 2818 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8833 "ds_parser.cpp"
    break;

  case 543:
#line 2825 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8847 "ds_parser.cpp"
    break;

  case 544:
#line 2837 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8853 "ds_parser.cpp"
    break;

  case 545:
#line 2838 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8859 "ds_parser.cpp"
    break;

  case 546:
#line 2842 "ds_parser.ypp"
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
#line 8882 "ds_parser.cpp"
    break;

  case 547:
#line 2860 "ds_parser.ypp"
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
#line 8905 "ds_parser.cpp"
    break;


#line 8909 "ds_parser.cpp"

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
#line 2880 "ds_parser.ypp"


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
