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
#define YYLAST   7446

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  179
/* YYNRULES -- Number of rules.  */
#define YYNRULES  550
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  962

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
       0,   453,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   469,   470,   471,   475,   476,
     480,   497,   498,   499,   500,   504,   508,   513,   522,   530,
     546,   551,   559,   559,   589,   611,   615,   618,   622,   628,
     637,   658,   681,   682,   686,   690,   691,   695,   698,   704,
     710,   713,   719,   720,   724,   725,   726,   735,   736,   740,
     749,   765,   773,   783,   792,   793,   794,   795,   796,   797,
     801,   806,   814,   815,   819,   820,   821,   822,   823,   824,
     825,   826,   832,   846,   863,   864,   865,   869,   877,   890,
     894,   901,   902,   906,   907,   908,   912,   915,   922,   926,
     927,   928,   929,   930,   931,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,   948,   949,   950,   951,   952,   953,   954,   955,   956,
     957,   958,   959,   960,   961,   962,   963,   964,   965,   966,
     967,   971,   993,   994,   995,   999,  1038,  1045,  1049,  1060,
    1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1082,  1087,
    1093,  1099,  1133,  1136,  1142,  1143,  1154,  1158,  1164,  1167,
    1170,  1174,  1180,  1184,  1188,  1191,  1194,  1199,  1202,  1210,
    1213,  1218,  1221,  1229,  1235,  1236,  1240,  1274,  1274,  1274,
    1277,  1277,  1277,  1282,  1282,  1282,  1290,  1290,  1290,  1296,
    1306,  1317,  1332,  1335,  1341,  1342,  1349,  1360,  1361,  1362,
    1366,  1367,  1368,  1369,  1373,  1378,  1386,  1387,  1391,  1396,
    1460,  1461,  1462,  1463,  1464,  1465,  1469,  1470,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,
    1483,  1484,  1485,  1486,  1487,  1491,  1492,  1493,  1494,  1498,
    1509,  1514,  1524,  1528,  1528,  1528,  1535,  1535,  1535,  1549,
    1553,  1553,  1553,  1560,  1561,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,
    1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,
    1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,  1596,
    1597,  1598,  1599,  1600,  1601,  1602,  1603,  1607,  1611,  1614,
    1617,  1618,  1619,  1620,  1629,  1638,  1639,  1642,  1642,  1642,
    1645,  1649,  1653,  1657,  1658,  1659,  1660,  1661,  1662,  1663,
    1664,  1680,  1696,  1754,  1755,  1759,  1760,  1764,  1772,  1775,
    1780,  1779,  1824,  1880,  1889,  1894,  1902,  1903,  1907,  1910,
    1919,  1920,  1924,  1933,  1934,  1939,  1940,  1944,  1950,  1956,
    1959,  1963,  1969,  1978,  1979,  1980,  1984,  1985,  1989,  1992,
    1997,  2002,  2010,  2021,  2024,  2032,  2033,  2037,  2038,  2039,
    2043,  2071,  2071,  2105,  2108,  2117,  2130,  2142,  2143,  2147,
    2151,  2152,  2153,  2157,  2185,  2217,  2218,  2222,  2259,  2260,
    2264,  2265,  2266,  2271,  2270,  2355,  2362,  2371,  2372,  2373,
    2374,  2375,  2376,  2377,  2378,  2379,  2380,  2381,  2382,  2383,
    2384,  2385,  2386,  2387,  2388,  2389,  2390,  2391,  2392,  2393,
    2394,  2395,  2399,  2400,  2401,  2402,  2403,  2404,  2408,  2419,
    2423,  2433,  2440,  2449,  2449,  2449,  2462,  2463,  2464,  2465,
    2466,  2480,  2486,  2490,  2494,  2499,  2504,  2509,  2514,  2518,
    2522,  2527,  2531,  2536,  2536,  2536,  2542,  2549,  2549,  2549,
    2554,  2554,  2554,  2560,  2560,  2560,  2565,  2569,  2569,  2569,
    2574,  2574,  2574,  2583,  2587,  2587,  2587,  2592,  2592,  2592,
    2601,  2605,  2605,  2605,  2610,  2610,  2610,  2619,  2619,  2619,
    2625,  2625,  2625,  2634,  2634,  2649,  2649,  2669,  2670,  2671,
    2672,  2676,  2683,  2690,  2696,  2705,  2710,  2717,  2718,  2722,
    2728,  2735,  2743,  2750,  2758,  2770,  2773,  2787,  2796,  2797,
    2801,  2806,  2813,  2818,  2828,  2833,  2840,  2852,  2853,  2857,
    2875
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

#define YYPACT_NINF -596

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-596)))

#define YYTABLE_NINF -505

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -596,    23,  -596,  -596,    15,   -62,    95,   -92,  -596,   -29,
    -596,  -596,    28,  -596,  -596,  -596,  -596,  -596,   224,  -596,
      70,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,   -13,
    -596,    59,    -5,    82,  -596,  -596,    95,    25,  -596,     9,
    -596,  -596,    70,   126,   139,  -596,  -596,   153,   114,   128,
    -596,    77,  -596,  -596,  -596,   279,   296,   317,  -596,   321,
      24,    15,   227,   -62,   149,   228,  -596,   232,   240,  -596,
     236,  -596,   -77,   333,   204,   222,  -596,   269,    15,    28,
    -596,  -596,  -596,     1,   244,  -596,  -596,  -596,   294,  -596,
    -596,   295,  -596,  -596,   258,  -596,  -596,  -596,  -596,  -596,
     257,    74,  -596,  -596,  -596,  -596,   392,  -596,  -596,  7189,
    -596,  -596,  -596,  -596,  -596,   301,   302,  -596,   -50,  -596,
    7276,   414,   422,  -596,   280,  -596,  -596,   -19,   314,  -596,
    -596,   340,  -596,  -596,   186,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,    83,  -596,   318,   325,   327,   328,  -596,  -596,
    -596,   298,  -596,  -596,  -596,  -596,  -596,   329,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,   334,  -596,  -596,
    -596,   335,   336,  -596,  -596,  -596,  -596,   337,   341,  -596,
    -596,  -596,  -596,  -596,    79,  -596,   322,   323,  -596,   324,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,   -76,   350,  1139,   309,  -596,   202,  -596,
     297,   -60,    15,   363,  -596,  -596,  -596,    74,  -596,  -596,
    -596,  -596,  -596,   369,  -596,   264,   288,   312,  -596,  -596,
    -596,  -596,  -596,  -596,   483,  -596,  -596,    39,  2827,  -596,
    -596,   -65,  7189,   -43,  -596,    -2,  -596,   178,  7189,  -596,
    -596,  4464,  -596,  -596,   363,  -596,  -596,  7103,   338,   358,
    1955,  -596,  -596,   244,  4464,   342,  4464,   359,   360,   343,
    -596,   344,   366,   377,  2966,   244,  -596,   370,  2114,  4464,
    4464,   234,   234,  6931,  7017,  4464,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,  4464,  4464,  4464,   202,  4464,  4464,  4464,
    -596,   349,  -596,   497,  -596,  -596,   351,   365,  4464,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,   -18,   371,  -596,
     372,   374,  -596,  -596,  -596,   363,  -596,  -596,  -596,  -596,
     376,  -596,  -596,   -69,  -596,  -596,  -596,  -596,  -596,  6115,
     364,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,   378,    10,  -596,  -596,  -596,   363,   130,  -596,
    -596,    27,  -596,  7189,  7189,  7189,  7189,   362,   301,  7189,
     280,  7189,   280,  7189,   280,  7275,   302,  -596,  -596,  -596,
     383,  -596,  -596,  5775,  -596,  -596,  3495,  -596,  -596,   363,
    -596,   -63,  -596,   340,  3495,   792,     6,   -15,  -596,   202,
    -596,  2253,  -596,  -596,   444,  4554,   551,  6409,  4464,   792,
    -596,  -596,  4464,  -596,  -596,   409,   427,  6409,  -596,  7189,
    2253,  -596,  4653,   316,   316,   385,  -596,  -596,   363,  1484,
     363,  1641,  6183,  -596,  -100,   253,   316,   316,   -53,  -596,
     316,   316,  5223,    92,   386,  -596,  -596,   792,  -596,  2392,
    -596,  -596,  -596,  -596,   433,   234,  -596,    31,   426,  4464,
    4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  -596,
    -596,  4464,  4464,  4464,  4464,  4464,  4464,   428,  2549,  4464,
     429,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,  4464,
    4464,  4464,  4464,  4464,  4464,  2688,  3105,  4464,  4464,  4464,
    4464,  4464,  4464,  4464,  4464,  4464,  4464,   430,  4464,  3244,
    -596,   -84,  -596,    14,  -596,   394,   431,  7189,  -596,   -42,
     472,   930,  1424,  1735,  -596,    89,  1896,   350,  2054,   350,
    2190,   350,   118,  -596,   156,  3495,   161,  -596,  -596,  -596,
    -596,  -596,  7189,  -596,  2253,  -596,  -596,  4464,  3401,  3558,
    7189,  -596,  4752,  4464,  4464,  -596,   244,  5301,  -596,  7189,
    7189,  5381,  7189,  -596,  -596,  2329,  -596,  4851,  -596,  -596,
      21,   234,   -28,  1798,  3697,  6183,   432,    -6,   401,   435,
    -596,  -596,   -99,    26,  3854,    -6,   163,  4464,  4464,   396,
    -596,  4464,   283,   436,  -596,   319,  -596,  -596,   231,   437,
    -596,    96,  6409,   -49,   280,  -596,   424,  -596,  -596,  6409,
    6409,  6409,  6409,  6409,  6409,  6409,  6409,   164,   164,   347,
    6409,  6409,   347,   247,   247,   407,  4464,  4464,  6409,    16,
    -596,  5851,   -58,   -58,  6409,   164,   164,  6409,  6409,  6584,
    6477,  6556,  6409,  6409,  6409,  4464,  4464,  6409,   445,  6262,
     434,  6662,  6809,   347,   347,   587,   587,    16,    16,    16,
    -596,   588,  5927,  -596,    98,    33,  4464,  -596,    28,  -596,
     594,   560,   420,  -596,  -596,   583,  -596,  -596,  -596,  2253,
    -596,  7189,  -596,  -596,  -596,   459,  -596,   448,  -596,   451,
    -596,   452,  7189,  -596,  7275,  -596,   302,   946,  -596,  6015,
      -8,  -596,   111,   461,  5459,  2486,  -596,  -596,  -596,  2625,
    2764,  -596,  2903,   433,  -596,  4464,  -596,  4464,  4464,     8,
     469,   437,   439,   441,  4464,   442,  4464,  4464,   437,   438,
     447,  6409,  -596,  -596,  6330,  -596,   475,  7189,   280,  1324,
     244,  -596,  -596,  4464,  -596,   -41,   132,  4464,  -596,   484,
     485,   490,   491,  -596,   141,   350,  -596,  4011,  6733,  6409,
    -596,  6733,  6409,  -596,  4464,  -596,  -596,  -596,  -596,   911,
     143,   393,  -596,   363,  -596,   483,  -596,  2253,  -596,  4950,
    -596,  3042,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  3495,  -596,  -596,  -596,   483,  4464,   244,  -596,   162,
    -596,  -596,  -596,  -596,  -596,   449,    49,  6409,  6409,    -6,
     460,    91,   432,   463,  -596,  6409,  -596,  -596,    56,    -6,
     464,  -596,  -596,  -596,  3181,   350,  -596,  -596,   792,   465,
    6409,  -596,  -596,  -596,  -596,   -49,   466,   -75,  7189,  -596,
     120,  6409,  -596,  -596,   468,  -596,   279,  -596,  -596,  5049,
    -596,  -596,  -596,  -596,  -596,  6409,  -596,  4464,   498,  -596,
    4464,  4464,  4464,  4168,  4464,   470,   476,  4464,  4464,  -596,
    4464,   462,  -596,  -596,   492,   231,  -596,  -596,  -596,  4325,
    -596,  -596,  3338,  -596,  -596,   279,  -596,  -596,  -596,  5539,
     494,   347,   347,   347,  -596,  5617,  5148,   478,  -596,  6409,
    6409,  5148,   479,   202,  -596,  -596,  4464,  6409,  -596,   480,
    -596,  4464,  -596,   610,   489,  -596,   477,  -596,  -596,   202,
    6409,  -596,  -596,  5697,  4464,   495,   496,  -596,  -596,  6409,
    -596,  -596
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    91,     1,   194,     0,     0,     0,     0,   195,     0,
     515,   513,     0,    14,     3,    10,     9,     8,     0,     7,
     385,     6,    11,     5,     4,    12,    13,    73,    72,    80,
      82,    34,    47,    44,    45,    36,     0,    42,    35,     0,
      19,    18,   385,     0,     0,    85,    86,     0,   174,    87,
      89,     0,    84,   409,   408,   142,   400,   410,   386,   387,
       0,     0,     0,     0,    37,     0,    43,     0,     0,    40,
       0,   397,     0,    15,     0,     0,   176,     0,     0,     0,
      92,   144,   143,     0,     0,   141,   402,   401,     0,   412,
     411,     0,   389,   388,   391,    78,    79,    76,    77,    75,
       0,     0,    74,    83,    48,    46,    42,    39,    38,     0,
     399,   398,    16,    17,    20,     0,     0,   175,     0,    90,
       0,     0,     0,    98,    93,   168,   146,     0,   405,   413,
     383,   343,    21,    22,     0,    68,    69,    66,    67,    65,
      64,    70,     0,    41,     0,     0,     0,     0,   417,   437,
     418,   449,   419,   423,   424,   425,   426,   441,   430,   431,
     432,   433,   434,   435,   436,   438,   439,   486,   422,   429,
     440,   493,   500,   420,   427,   421,   428,     0,     0,   448,
     456,   459,   457,   458,     0,   451,     0,     0,   363,     0,
      88,   101,   102,   104,   103,   105,   106,   107,   108,   133,
     134,   131,   132,   124,   135,   136,   125,   122,   123,   137,
     138,   139,   140,   112,   113,   114,   109,   110,   111,   127,
     128,   126,   120,   121,   116,   115,   117,   118,   119,   100,
      99,   129,   130,   343,    96,     0,     0,   393,     0,   407,
       0,   343,     0,     0,    23,    24,    25,     0,    81,   477,
     480,   483,   473,     0,   453,   487,   494,   501,   507,   510,
     464,   469,   463,   476,     0,   472,   466,     0,     0,   396,
     468,     0,     0,     0,    94,     0,   356,     0,     0,   145,
     170,     0,    57,    58,     0,   279,   280,     0,     0,     0,
       0,   273,   182,     0,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,   232,   231,   233,
     234,   235,    26,     0,     0,     0,     0,     0,     0,     0,
     217,   218,   149,   147,   277,   276,     0,     0,     0,   163,
     158,   156,   155,   157,   228,   169,   150,   274,     0,   336,
       0,     0,   161,   162,   164,     0,   154,   335,   334,   333,
      91,   339,   275,     0,   151,   338,   337,   315,   281,   236,
       0,   278,   517,   518,   519,   520,   442,   445,   443,   446,
     444,   447,     0,     0,   406,   348,   390,     0,     0,   415,
     392,   376,    71,     0,     0,     0,     0,     0,     0,     0,
      93,     0,    93,     0,    93,     0,     0,   471,   465,   467,
       0,   470,   461,     0,   452,   516,   362,   514,   364,     0,
     354,   367,    95,   343,    97,     0,     0,   178,   181,     0,
     206,     0,   184,   187,     0,   236,     0,   177,     0,     0,
     197,   200,     0,   167,   203,     0,     0,    51,    61,     0,
       0,   191,   236,   307,   308,   218,   172,   173,     0,   527,
       0,     0,     0,   544,     0,     0,   285,   284,   320,    32,
     283,   282,     0,   219,     0,   165,   166,     0,   342,     0,
     153,   159,   160,   196,   226,     0,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     393,     0,   403,   343,   384,     0,     0,     0,   377,     0,
       0,     0,     0,     0,   450,     0,     0,    96,     0,    96,
       0,    96,   174,   360,     0,   358,     0,   462,   460,   355,
     366,   365,     0,   368,     0,   357,    62,     0,     0,     0,
       0,   188,   236,     0,     0,   185,     0,     0,    63,     0,
       0,     0,     0,    49,    50,     0,   192,   236,   189,   219,
       0,     0,   174,     0,     0,   535,   525,   527,     0,   538,
     539,   540,     0,     0,     0,   527,     0,     0,     0,     0,
      29,     0,    27,     0,   311,   263,   262,   168,    54,     0,
     316,     0,   212,     0,    93,   171,     0,   330,   331,   246,
     247,   249,   248,   250,   240,   241,   242,   286,   287,   299,
     251,   252,   300,   297,   298,     0,     0,     0,   238,   325,
     314,     0,   340,   341,   239,   288,   289,   253,   254,   304,
     305,   306,   243,   244,   245,     0,     0,   237,     0,     0,
     302,   303,   301,   295,   296,   291,   290,   292,   293,   294,
     269,     0,     0,   318,     0,     0,     0,   394,     0,   414,
       0,   345,     0,   344,   416,     0,   374,   375,   373,     0,
     478,     0,   484,   474,   454,     0,   488,     0,   495,     0,
     502,     0,     0,   508,     0,   511,     0,   369,   372,   371,
       0,   179,     0,     0,     0,     0,   186,   193,   321,     0,
       0,   322,     0,   226,   190,     0,   528,     0,     0,   527,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,   545,   546,     0,    30,    33,     0,    93,     0,
       0,    52,    53,     0,    59,     0,     0,     0,   317,     0,
       0,     0,     0,   224,     0,    96,   327,     0,   257,   258,
     313,   255,   256,   332,     0,   271,   312,   319,   404,     0,
       0,   345,   346,     0,   349,   373,   378,     0,   382,   236,
     479,     0,   485,   475,   455,   489,   491,   496,   498,   503,
     505,   359,   509,   361,   512,   365,     0,     0,   180,     0,
     209,   207,   198,   201,   204,     0,     0,   522,   521,   527,
       0,     0,   526,     0,   530,   536,   542,   541,     0,   527,
       0,   543,    28,    31,     0,    96,   148,    55,     0,     0,
     213,   222,   223,   221,   220,     0,     0,     0,     0,   260,
       0,   326,   272,   395,    92,   350,   142,   347,   380,   236,
     381,   481,   492,   499,   506,   370,    60,     0,     0,   208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,   533,   264,     0,    54,   259,   225,   227,     0,
     214,   229,     0,   261,   353,   142,   352,   379,   482,     0,
       0,   199,   202,   205,   323,     0,   547,     0,   531,   524,
     523,   547,     0,     0,   267,    56,     0,   215,   328,     0,
     210,     0,   324,     0,     0,   532,     0,   534,   265,     0,
     216,   329,   351,     0,     0,     0,     0,   268,   211,   548,
     549,   550
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -596,  -596,  -596,  -596,  -596,   203,   609,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,   634,  -596,   566,  -596,  -596,   612,
    -596,  -596,  -596,  -229,  -596,  -596,  -596,  -596,  -596,  -596,
     443,  -596,  -596,   617,   -46,  -596,   603,   -10,  -332,  -390,
    -539,  -596,  -596,  -596,  -226,  -185,   -83,  -596,    68,  -282,
     -12,  -596,  -596,  -596,  -596,  -596,  -596,  -596,   695,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -530,  -596,  -596,  -164,  -596,   -36,  -281,  -596,  -233,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,   356,  -596,  -596,  -124,   -93,  -596,  -596,  -596,   290,
    -596,    -9,  -596,  -268,   305,  -415,  -408,    11,  -596,  -289,
    -596,   672,  -596,  -596,  -596,   177,   646,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -271,  -106,  -596,
    -596,  -596,   326,  -596,  -596,  -596,   -90,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,  -596,
    -596,  -596,  -596,  -596,  -596,   440,  -595,  -434,  -591,  -596,
    -596,  -300,   -37,   259,  -596,  -596,  -596,  -210,  -596
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   114,    42,    14,   134,   140,   465,   334,   766,
     335,   623,    15,    16,    37,    38,    69,    17,    33,    34,
     336,   337,   773,   774,   338,   339,   340,   341,   342,   343,
     141,   142,    29,    30,    31,    49,    50,    51,    18,   234,
     279,   124,    19,    83,    84,    85,   344,   345,   235,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   589,   890,   590,   891,   592,   892,   358,   580,   889,
     359,   631,   911,   360,   783,   784,   634,   361,   362,   434,
     364,   365,   366,   367,   767,   933,   768,   949,   368,   691,
     872,   632,   868,   951,   275,   803,   702,   543,   915,   276,
     277,   563,   564,   188,   189,   748,   420,   709,   549,   390,
     241,    59,    94,    21,   131,   383,    71,    72,    22,    88,
      23,   239,   129,    57,    91,    24,   240,   391,   370,   382,
     181,   182,   186,   183,   398,   814,   565,   396,   813,   393,
     810,   394,   918,   395,   812,   399,   815,   400,   882,   401,
     817,   402,   883,   403,   819,   404,   884,   405,   822,   406,
     824,    25,    44,    26,    43,   371,   606,   607,   608,   372,
     609,   610,   611,   612,   373,   464,   374,   944,   375
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,   126,   363,   180,   421,   418,   574,   243,   433,   694,
     557,   569,   559,   426,   561,   463,   752,   260,   717,   184,
     719,   -91,   721,     2,   759,   577,   451,   615,   484,     3,
     456,   457,   118,    27,   776,   120,   487,   488,    95,    96,
     745,   261,   262,   601,   419,   757,    65,   730,   732,   636,
     485,   570,     4,   909,     5,   507,     6,   601,   510,   511,
     242,    45,     7,   510,   511,   779,   483,    52,    70,     8,
     696,   408,   706,   570,   780,     9,   242,    32,    66,   755,
     414,   707,   747,   618,   756,   619,   570,    46,   135,   136,
     546,   571,    39,   572,    10,   747,   573,   179,   544,   697,
     263,    77,   187,    61,   274,   781,   121,   122,   110,   125,
     782,   260,   708,   571,   486,    11,    40,   387,   537,   538,
     415,   499,   500,   537,   538,   386,   571,   478,    58,   507,
     190,   509,   510,   511,   512,   261,   262,   236,   513,   264,
     265,    60,   417,   389,   266,   777,   123,   267,   421,   581,
     242,    62,    41,   775,    70,   541,   842,    47,   840,   546,
      28,   479,   268,    97,   578,   237,   180,    98,   596,    99,
     100,   270,   180,    48,   546,   827,   637,   751,   541,   546,
     546,   180,   416,   547,   487,   488,   548,   600,   424,   613,
      67,   698,   537,   538,   263,   542,   388,   427,   409,   699,
      12,    68,   777,   101,   635,   570,    13,   180,   180,   777,
     436,   700,    61,   137,   897,   836,   410,   138,   798,   139,
     100,    47,   448,   459,   461,   411,   384,   848,    53,    54,
      79,    55,   894,   264,   265,    63,   247,    48,   266,   900,
      35,   267,   770,    77,   785,   571,   867,    77,   895,   777,
     714,   777,   625,   771,   772,    80,   268,   870,   901,    56,
     179,    36,   269,   248,   777,   270,   179,   487,   488,   499,
     500,    74,   715,   777,   722,   179,   778,   507,   797,   509,
     510,   511,   512,    61,    75,   750,   513,   180,   180,   180,
     180,   828,   728,   180,   865,   180,    79,   180,    76,   180,
     913,   179,   179,   550,   551,   552,   553,    78,   545,   556,
     859,   558,   826,   560,   469,   839,   904,   723,   762,   866,
     746,   874,   725,   887,   849,    68,   532,   533,   534,   535,
     536,    47,    81,   244,   245,   246,   487,   488,    82,   724,
     537,   538,   576,   180,   726,   888,   756,    48,   760,    86,
     497,   498,   499,   500,   501,    87,   588,   504,   422,   595,
     507,   423,   509,   510,   511,   512,   104,   487,   488,   513,
      89,   515,   516,   106,    92,   376,    90,   107,   855,   377,
      93,   179,   179,   179,   179,   108,   112,   179,   115,   179,
     109,   179,   113,   179,   628,   877,   378,   379,   380,   381,
     132,   133,   620,   621,   132,   133,   116,   530,   531,   532,
     533,   534,   535,   536,   117,   330,   455,   579,   125,   701,
    -490,   499,   500,   537,   538,  -490,   898,   808,   125,   507,
     244,   245,   510,   511,   512,   802,   875,   179,   513,   127,
     128,   180,   130,  -490,  -497,    66,   185,   187,   231,  -497,
     497,   498,   499,   500,   487,   488,   232,   705,   418,   233,
     507,   626,   509,   510,   511,   512,   180,  -497,  -504,   513,
     238,   515,   516,  -504,   180,  -266,   242,   253,   249,   272,
    -266,   385,   727,   180,   180,   250,   180,   251,   252,   254,
     735,  -504,   537,   538,   255,   256,   257,   258,  -266,   739,
     740,   259,   742,   737,   260,   271,   278,   273,   389,   532,
     533,   534,   535,   536,   397,   407,   445,   429,   430,   440,
     441,   438,   442,   537,   538,   878,   444,   443,   261,   262,
     449,   473,   421,   474,   475,   179,   363,   497,   498,   499,
     500,   501,   554,   539,   504,   505,   506,   507,   476,   509,
     510,   511,   512,    12,   480,   481,   513,   482,   515,   516,
     179,   567,   540,   485,   586,   593,   594,   599,   179,   633,
     627,   638,   703,   655,   660,   690,   704,   179,   179,   753,
     179,   763,   775,   765,   786,   750,   787,   263,   754,   795,
     793,   369,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   801,   802,   804,   414,   180,   829,   487,   488,   816,
     537,   538,   818,   820,   841,   260,   180,   843,   180,   844,
     846,   811,   853,   850,   413,   851,   264,   265,   893,   861,
     862,   266,   821,   710,   267,   863,   864,   425,   896,   261,
     262,   899,   902,   920,   908,   906,   435,   932,   927,   268,
     437,   914,   439,   934,   928,   941,   945,   947,   270,   954,
     447,   180,   956,   952,   452,   453,   454,   955,   622,   102,
      64,   462,   143,   960,   961,   105,   935,   854,   103,   466,
     467,   468,   119,   470,   471,   472,    52,   857,   800,   939,
     392,   916,   499,   500,   477,   769,    20,   706,   263,   179,
     507,   907,   509,   510,   511,   512,   707,   835,   876,   513,
     179,   566,   179,   575,    73,   823,   807,   695,   111,   847,
     616,   946,     0,     0,   555,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   805,   265,     0,
       0,     0,   266,     0,   886,   267,     0,     0,     0,     0,
       0,   534,   535,   536,     0,   179,     0,     0,     0,     0,
     268,     0,   180,   537,   538,     0,   806,     0,     0,   270,
       0,     0,     0,     0,     0,   905,     0,     0,   912,     0,
       0,     0,     0,     0,   910,     0,     0,   582,     0,     0,
       0,     0,     0,     0,   587,     0,     0,     0,   591,     0,
       0,     0,     0,     0,     0,     0,   597,     0,     0,     0,
       0,     0,   487,   488,     0,   605,     0,   605,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   639,   640,   641,   642,   643,
     644,   645,   646,   647,   648,     0,   179,   649,   650,   651,
     652,   653,   654,     0,   658,   659,     0,   661,   662,   663,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   677,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,   689,     0,   692,   497,   498,   499,   500,   501,
       0,     0,   504,   505,   506,   507,     0,   509,   510,   511,
     512,     0,     0,     0,   513,     0,   515,   516,     0,     0,
       0,   948,   519,   520,   521,     0,     0,     0,     0,     0,
     729,   487,   488,     0,     0,   734,     0,   957,     0,   658,
     677,     0,     0,     0,     0,     0,     0,   526,     0,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   466,
     472,     0,   260,     0,     0,     0,     0,     0,   537,   538,
     472,     0,     0,   761,   462,     0,   125,   764,   260,     0,
       0,     0,     0,     0,     0,     0,   261,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   262,     0,     0,     0,     0,     0,     0,
       0,     0,   788,   789,   497,   498,   499,   500,   501,     0,
       0,   504,   505,   506,   507,     0,   509,   510,   511,   512,
       0,   791,   792,   513,     0,   515,   516,     0,     0,     0,
       0,   519,   520,   521,     0,   263,     0,     0,     0,     0,
       0,     0,   799,     0,     0,     0,     0,     0,     0,     0,
     570,   263,     0,     0,     0,   809,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,     0,     0,
       0,     0,     0,     0,   264,   265,     0,   537,   538,   266,
       0,     0,   267,     0,   873,     0,     0,     0,     0,     0,
     825,   265,     0,   837,   838,   266,     0,   268,   267,     0,
     845,     0,   605,   711,     0,     0,   270,     0,     0,     0,
       0,     0,     0,   268,     0,   369,     0,     0,     0,   858,
       0,     0,   270,   860,     0,     0,     0,     0,     0,     0,
     280,     0,     0,     0,     0,     3,     0,   281,   282,   283,
     871,   284,     0,   285,   286,   287,   288,   289,     0,     0,
       0,     0,     0,   879,   290,   291,   292,   293,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   295,     0,   296,
     297,     0,   885,   298,     0,     8,   299,   300,     0,   301,
     302,     0,     0,   303,   304,     0,     0,     0,     0,     0,
     305,   148,   149,   150,     0,   152,   153,   154,   155,   156,
     306,   158,   159,   160,   161,   162,   163,   164,   165,   166,
       0,   168,   169,   170,     0,     0,   173,   174,   175,   176,
       0,     0,   307,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   919,   309,   310,   921,   922,   923,   925,
     926,     0,     0,   929,   930,     0,   931,     0,     0,   311,
     312,     0,     0,     0,     0,   937,     0,     0,    47,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    48,   322,     0,     0,     0,     0,
       0,     0,   950,     0,     0,     0,     0,   953,     0,     0,
       0,   323,   324,   325,     0,   326,     0,     0,   327,   328,
     959,     0,     0,     0,     0,     0,     0,     0,   329,     0,
     330,   331,   332,   125,   333,   280,     0,     0,     0,     0,
       3,     0,   281,   282,   283,     0,   284,     0,   285,   286,
     287,   288,   289,     0,     0,     0,     0,     0,     0,   290,
     291,   292,   293,     0,     0,     0,     0,     0,     0,     0,
       0,   294,   295,     0,   296,   297,     0,     0,   298,     0,
       8,   299,   300,     0,   301,   302,     0,     0,   303,   304,
       0,     0,     0,     0,     0,   305,   148,   149,   150,     0,
     152,   153,   154,   155,   156,   306,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,   168,   169,   170,     0,
       0,   173,   174,   175,   176,     0,     0,   307,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,   260,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    48,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   262,     0,     0,     0,     0,   323,   324,   325,     0,
     326,     0,     0,   327,   328,     0,     0,     0,   285,   286,
     287,   288,   289,   329,     0,   330,   331,   332,   125,   856,
     291,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,   295,     0,     0,   297,     0,     0,   298,     0,
       0,   299,     0,   601,     0,   302,     0,     0,     0,   263,
     261,   262,     0,     0,     0,     0,   148,   149,   150,     0,
     152,   153,   154,   155,   156,   306,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,   168,   169,   170,     0,
       0,   173,   174,   175,   176,     0,     0,   307,   264,   265,
       0,     0,     0,   266,     0,   712,   267,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   268,     0,     0,     0,     0,     0,     0,     0,     0,
     270,     0,     0,    47,     0,     0,     0,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   602,
     322,     0,     0,     0,     0,     0,     0,     0,   264,   265,
       0,     0,     0,   266,     0,     0,   603,   324,   325,     0,
     326,     0,     0,   327,   328,   285,   286,   287,   288,   289,
       0,   268,     0,   604,     0,   330,   331,   291,   125,     0,
     270,     0,     0,   260,     0,     0,     0,     0,     0,   295,
       0,     0,   297,     0,     0,   298,     0,     0,   299,     0,
       0,     0,   302,     0,     0,     0,     0,   261,   262,     0,
       0,     0,     0,   148,   149,   150,     0,   152,   153,   154,
     155,   156,   306,   158,   159,   160,   161,   162,   163,   164,
     165,   166,     0,   168,   169,   170,     0,     0,   173,   174,
     175,   176,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   310,     0,     0,
       0,     0,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
      47,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   602,   322,     0,     0,
       0,   261,   262,     0,     0,   264,   265,     0,     0,     0,
     266,     0,     0,   603,   324,   325,     0,   326,     0,     0,
     327,   328,   285,   286,   287,   288,   289,     0,   268,     0,
     614,     0,   330,   331,   291,   125,     0,   270,     0,     0,
     408,     0,     0,     0,     0,     0,   295,     0,     0,   297,
       0,     0,   298,     0,     0,   299,     0,     0,     0,   302,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,     0,   152,   153,   154,   155,   156,   306,
     158,   159,   160,   161,   162,   163,   164,   165,   166,     0,
     168,   169,   170,     0,     0,   173,   174,   175,   176,   264,
     265,   307,     0,     0,   266,     0,   713,   267,     0,     0,
       0,     0,     0,   309,   310,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,    47,   260,     0,
       0,     0,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    48,   322,     0,     0,     0,     0,     0,
       0,     0,   261,   262,     0,     0,     0,   409,     0,     0,
     323,   324,   325,     0,   326,     0,     0,   327,   328,   285,
     286,   287,   288,   289,     0,   410,     0,   329,     0,   330,
     331,   291,   125,     0,   411,     0,     0,     0,     0,     0,
       0,     0,     0,   295,     0,     0,   297,     0,     0,   298,
       0,     0,   299,     0,     0,     0,   302,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   148,   149,   150,
       0,   152,   153,   154,   155,   156,   306,   158,   159,   160,
     161,   162,   163,   164,   165,   166,     0,   168,   169,   170,
       0,     0,   173,   174,   175,   176,     0,     0,   307,     0,
     264,   265,     0,     0,     0,   266,     0,   716,   267,     0,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   268,     0,   311,   312,     0,     0,     0,
       0,     0,   270,     0,    47,     0,   260,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      48,   322,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   262,     0,     0,     0,     0,     0,   323,   324,   325,
       0,   326,     0,     0,   327,   328,     0,     0,   285,   286,
     287,   288,   289,     0,   329,     0,   330,   331,   432,   125,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,     0,     0,   297,     0,     0,   298,     0,
       0,   299,     0,     0,     0,   302,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,   148,   149,   150,     0,
     152,   153,   154,   155,   156,   306,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,   168,   169,   170,     0,
       0,   173,   174,   175,   176,     0,     0,   307,   264,   265,
       0,     0,     0,   266,     0,   718,   267,     0,     0,   309,
     310,     0,   260,     0,     0,     0,     0,     0,   450,     0,
       0,   268,     0,     0,   311,   312,     0,     0,     0,     0,
     270,     0,     0,    47,     0,     0,   261,   262,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    48,
     322,     0,     0,     0,     0,     0,     0,   285,   286,   287,
     288,   289,     0,     0,     0,     0,   323,   324,   325,   291,
     326,     0,     0,   327,   328,     0,     0,     0,     0,     0,
       0,   295,     0,   329,   297,   330,   331,   298,   125,     0,
     299,     0,     0,     0,   302,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   148,   149,   150,     0,   152,
     153,   154,   155,   156,   306,   158,   159,   160,   161,   162,
     163,   164,   165,   166,     0,   168,   169,   170,     0,     0,
     173,   174,   175,   176,   264,   265,   307,     0,     0,   266,
       0,   720,   267,     0,     0,     0,     0,     0,   309,   310,
       0,   260,     0,     0,     0,     0,     0,   268,     0,     0,
       0,     0,     0,   311,   312,     0,   270,     0,     0,     0,
       0,     0,    47,     0,     0,   261,   262,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    48,   322,
       0,     0,     0,     0,     0,     0,   285,   286,   287,   288,
     289,     0,     0,     0,     0,   323,   324,   325,   291,   326,
       0,     0,   327,   328,     0,     0,     0,     0,     0,     0,
     295,     0,   329,   297,   330,   331,   298,   125,     0,   299,
       0,     0,     0,   302,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   148,   149,   150,     0,   152,   153,
     154,   155,   156,   306,   158,   159,   160,   161,   162,   163,
     164,   165,   166,     0,   168,   169,   170,     0,     0,   173,
     174,   175,   176,   264,   265,   307,     0,     0,   266,     0,
     743,   267,     0,     0,     0,     0,     0,   309,   310,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,   260,     0,
       0,    47,     0,     0,     0,     0,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    48,   322,     0,
       0,     0,   261,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   324,   325,     0,   326,     0,
       0,   327,   328,   285,   286,   287,   288,   289,     0,   629,
       0,   329,   630,   330,   331,   291,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   295,     0,     0,
     297,     0,     0,   298,     0,     0,   299,     0,     0,     0,
     302,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   148,   149,   150,     0,   152,   153,   154,   155,   156,
     306,   158,   159,   160,   161,   162,   163,   164,   165,   166,
       0,   168,   169,   170,     0,     0,   173,   174,   175,   176,
     264,   265,   307,     0,     0,   266,     0,   831,   267,     0,
       0,     0,     0,     0,   309,   310,     0,   260,     0,     0,
       0,     0,     0,   268,     0,     0,     0,     0,     0,   656,
     657,     0,   270,     0,     0,     0,     0,     0,    47,     0,
       0,   261,   262,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    48,   322,     0,     0,     0,     0,
       0,     0,   285,   286,   287,   288,   289,     0,     0,     0,
       0,   323,   324,   325,   291,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   295,     0,   329,   297,
     330,   331,   298,   125,     0,   299,     0,     0,     0,   302,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,     0,   152,   153,   154,   155,   156,   306,
     158,   159,   160,   161,   162,   163,   164,   165,   166,     0,
     168,   169,   170,     0,     0,   173,   174,   175,   176,   264,
     265,   307,     0,     0,   266,     0,   832,   267,     0,     0,
       0,     0,     0,   309,   310,     0,   260,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,     0,   675,   676,
       0,   270,     0,     0,     0,     0,     0,    47,     0,     0,
     261,   262,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    48,   322,     0,     0,     0,     0,     0,
       0,   285,   286,   287,   288,   289,     0,     0,     0,     0,
     323,   324,   325,   291,   326,     0,     0,   327,   328,     0,
       0,     0,     0,     0,     0,   295,     0,   329,   297,   330,
     331,   298,   125,     0,   299,     0,     0,     0,   302,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   148,
     149,   150,     0,   152,   153,   154,   155,   156,   306,   158,
     159,   160,   161,   162,   163,   164,   165,   166,     0,   168,
     169,   170,     0,     0,   173,   174,   175,   176,   264,   265,
     307,     0,     0,   266,     0,   833,   267,     0,     0,     0,
       0,     0,   309,   310,     0,   260,     0,     0,     0,     0,
       0,   268,     0,     0,     0,     0,     0,     0,     0,     0,
     270,     0,     0,     0,     0,     0,    47,     0,     0,   261,
     262,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    48,   322,     0,     0,     0,     0,     0,     0,
     285,   286,   287,   288,   289,     0,     0,     0,     0,   323,
     324,   325,   291,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,   295,   412,   329,   297,   330,   331,
     298,   125,     0,   299,     0,     0,     0,   302,   263,     0,
     446,     0,     0,     0,     0,     0,     0,     0,   148,   149,
     150,     0,   152,   153,   154,   155,   156,   306,   158,   159,
     160,   161,   162,   163,   164,   165,   166,     0,   168,   169,
     170,     0,     0,   173,   174,   175,   176,   264,   265,   307,
       0,     0,   266,     0,   834,   267,     0,     0,     0,     0,
       0,   309,   310,     0,   260,     0,     0,     0,     0,     0,
     268,     0,     0,     0,     0,     0,     0,     0,     0,   270,
       0,     0,     0,     0,     0,    47,     0,     0,   261,   262,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    48,   322,     0,     0,     0,     0,     0,     0,   285,
     286,   287,   288,   289,     0,     0,   678,     0,   323,   324,
     325,   291,   326,     0,     0,   327,   328,     0,     0,     0,
       0,     0,     0,   295,     0,   329,   297,   330,   331,   298,
     125,     0,   299,     0,     0,     0,   302,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,   149,   150,
       0,   152,   153,   154,   155,   156,   306,   158,   159,   160,
     161,   162,   163,   164,   165,   166,     0,   168,   169,   170,
       0,     0,   173,   174,   175,   176,   264,   265,   307,     0,
       0,   266,     0,   881,   267,     0,     0,     0,     0,     0,
     309,   310,     0,   260,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,     0,     0,   270,     0,
       0,     0,     0,     0,    47,     0,     0,   261,   262,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      48,   322,     0,     0,     0,     0,     0,     0,   285,   286,
     287,   288,   289,     0,     0,     0,     0,   323,   324,   325,
     291,   326,     0,     0,   327,   328,     0,     0,     0,     0,
       0,     0,   295,     0,   329,   297,   330,   331,   298,   125,
       0,   299,     0,     0,     0,   302,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,   149,   150,     0,
     152,   153,   154,   155,   156,   306,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,   168,   169,   170,     0,
       0,   173,   174,   175,   176,   264,   265,   307,     0,     0,
     266,     0,   903,   267,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     260,     0,     0,    47,     0,     0,     0,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    48,
     322,     0,     0,     0,   261,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   324,   325,     0,
     326,     0,     0,   327,   328,   285,   286,   287,   288,   289,
       0,     0,     0,   329,   693,   330,   331,   291,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
       0,     0,   297,     0,     0,   298,     0,     0,   299,     0,
       0,     0,   302,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   148,   149,   150,     0,   152,   153,   154,
     155,   156,   306,   158,   159,   160,   161,   162,   163,   164,
     165,   166,     0,   168,   169,   170,     0,     0,   173,   174,
     175,   176,   264,   265,   307,     0,     0,   266,     0,   938,
     267,     0,     0,     0,     0,     0,   309,   310,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,     0,     0,
       0,     0,     0,     0,   270,     0,     0,   260,     0,     0,
      47,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    48,   322,     0,     0,
       0,   261,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   285,   286,   287,   288,   289,     0,     0,     0,
     329,   731,   330,   331,   291,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,     0,     0,   297,
       0,     0,   298,     0,     0,   299,     0,     0,     0,   302,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,     0,   152,   153,   154,   155,   156,   306,
     158,   159,   160,   161,   162,   163,   164,   165,   166,     0,
     168,   169,   170,     0,     0,   173,   174,   175,   176,   264,
     265,   307,     0,     0,   266,     0,     0,   267,     0,     0,
       0,     0,     0,   309,   310,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,    47,     0,     0,
       0,     0,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    48,   322,     0,     0,     0,     0,     0,
       0,   285,   286,   287,   288,   289,     0,     0,   733,     0,
     323,   324,   325,   291,   326,     0,     0,   327,   328,     0,
       0,     0,     0,     0,     0,   295,     0,   329,   297,   330,
     331,   298,   125,     0,   299,     0,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   148,
     149,   150,     0,   152,   153,   154,   155,   156,   306,   158,
     159,   160,   161,   162,   163,   164,   165,   166,     0,   168,
     169,   170,     0,     0,   173,   174,   175,   176,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    48,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,   285,   286,
     287,   288,   289,     0,     0,     0,   329,   749,   330,   331,
     291,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,     0,     0,   297,     0,     0,   298,     0,
       0,   299,     0,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,   149,   150,     0,
     152,   153,   154,   155,   156,   306,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,   168,   169,   170,     0,
       0,   173,   174,   175,   176,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    48,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   324,   325,     0,
     326,     0,     0,   327,   328,   285,   286,   287,   288,   289,
       0,     0,     0,   329,   758,   330,   331,   291,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
       0,     0,   297,     0,     0,   298,     0,     0,   299,     0,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   148,   149,   150,     0,   152,   153,   154,
     155,   156,   306,   158,   159,   160,   161,   162,   163,   164,
     165,   166,     0,   168,   169,   170,     0,     0,   173,   174,
     175,   176,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    48,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   285,   286,   287,   288,   289,     0,     0,     0,
     329,   869,   330,   331,   291,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,     0,     0,   297,
       0,     0,   298,     0,     0,   299,     0,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,     0,   152,   153,   154,   155,   156,   306,
     158,   159,   160,   161,   162,   163,   164,   165,   166,     0,
     168,   169,   170,     0,     0,   173,   174,   175,   176,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,     0,     0,
       0,     0,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    48,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,   324,   325,     0,   326,     0,     0,   327,   328,   285,
     286,   287,   288,   289,     0,     0,     0,   329,   924,   330,
     331,   291,   125,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   295,     0,     0,   297,     0,     0,   298,
       0,     0,   299,     0,     0,     0,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,   149,   150,
       0,   152,   153,   154,   155,   156,   306,   158,   159,   160,
     161,   162,   163,   164,   165,   166,     0,   168,   169,   170,
       0,     0,   173,   174,   175,   176,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   936,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      48,   322,     0,     0,     0,     0,     0,     0,   285,   286,
     287,   288,   289,     0,     0,     0,     0,   323,   324,   325,
     291,   326,     0,     0,   327,   328,     0,     0,     0,     0,
       0,     0,   295,     0,   329,   297,   330,   331,   298,   125,
       0,   299,     0,     0,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,   149,   150,     0,
     152,   153,   154,   155,   156,   306,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,   168,   169,   170,     0,
       0,   173,   174,   175,   176,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,   487,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    48,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   324,   325,     0,
     326,     0,     0,   327,   328,     0,     0,     0,     0,     0,
       0,     0,     0,   329,     0,   330,   331,     0,   125,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   583,   509,
     510,   511,   512,   487,   488,     0,   513,   514,   515,   516,
     517,   518,     0,     0,   519,   520,   521,   522,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   584,   526,
       0,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     537,   538,     0,     0,     0,     0,     0,   585,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   583,   509,   510,
     511,   512,   487,   488,     0,   513,   514,   515,   516,   517,
     518,     0,     0,   519,   520,   521,   522,   523,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,   526,     0,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     538,     0,     0,     0,     0,     0,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   583,   509,   510,   511,
     512,   487,   488,     0,   513,   514,   515,   516,   517,   518,
       0,     0,   519,   520,   521,   522,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   584,   526,     0,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   537,   538,
       0,     0,     0,     0,     0,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   583,   509,   510,   511,   512,
     487,   488,     0,   513,   514,   515,   516,   517,   518,     0,
       0,   519,   520,   521,   522,   523,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   538,     0,
       0,     0,     0,     0,   744,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   583,   509,   510,   511,   512,   487,
     488,     0,   513,   514,   515,   516,   517,   518,     0,     0,
     519,   520,   521,   522,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   584,   526,     0,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   537,   538,     0,     0,
       0,     0,     0,   880,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   583,   509,   510,   511,   512,   487,   488,
       0,   513,   514,   515,   516,   517,   518,     0,     0,   519,
     520,   521,   522,   523,   524,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   537,   538,     0,     0,     0,
       0,     0,   917,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   487,   488,     0,     0,     0,     0,     0,
       0,   497,   498,   499,   500,   501,     0,     0,   504,   505,
     506,   507,     0,   509,   510,   511,   512,     0,     0,     0,
     513,     0,   515,   516,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,     0,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,     0,     0,     0,     0,     0,
       0,   487,   488,     0,   537,   538,   497,   498,   499,   500,
     501,   943,     0,   504,   505,   506,   507,     0,   509,   510,
     511,   512,     0,     0,     0,   513,     0,   515,   516,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     538,   487,   488,   624,   497,   498,   499,   500,   501,     0,
       0,   504,   505,   506,   507,     0,   509,   510,   511,   512,
       0,     0,     0,   513,     0,   515,   516,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   538,   487,
     488,   738,     0,     0,   497,   498,   499,   500,   501,     0,
       0,   504,   505,   506,   507,     0,   509,   510,   511,   512,
       0,     0,     0,   513,     0,   515,   516,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   538,   487,
     488,   741,   497,   498,   499,   500,   501,     0,     0,   504,
     505,   506,   507,     0,   509,   510,   511,   512,     0,     0,
       0,   513,     0,   515,   516,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   537,   538,   487,   488,   830,
       0,     0,   497,   498,   499,   500,   501,     0,     0,   504,
     505,   506,   507,     0,   509,   510,   511,   512,     0,     0,
       0,   513,     0,   515,   516,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   537,   538,   487,   488,   940,
     497,   498,   499,   500,   501,     0,     0,   504,   505,   506,
     507,     0,   509,   510,   511,   512,     0,     0,     0,   513,
       0,   515,   516,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,     0,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   537,   538,   487,   488,   942,     0,     0,
     497,   498,   499,   500,   501,     0,     0,   504,   505,   506,
     507,     0,   509,   510,   511,   512,     0,     0,     0,   513,
       0,   515,   516,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,     0,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,     0,     0,     0,     0,     0,     0,
       0,   487,   488,   537,   538,     0,     0,   958,   497,   498,
     499,   500,   501,     0,     0,   504,   505,   506,   507,     0,
     509,   510,   511,   512,     0,     0,     0,   513,     0,   515,
     516,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,     0,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,     0,     0,     0,     0,     0,   487,   488,     0,
       0,   537,   538,   568,   497,   498,   499,   500,   501,     0,
       0,   504,   505,   506,   507,     0,   509,   510,   511,   512,
       0,     0,     0,   513,     0,   515,   516,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   538,   790,
     497,   498,   499,   500,   501,   487,   488,   504,   505,   506,
     507,     0,   509,   510,   511,   512,     0,     0,     0,   513,
       0,   515,   516,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,     0,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   537,   538,   796,     0,     0,     0,     0,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   583,
     509,   510,   511,   512,  -236,   487,   488,   513,   514,   515,
     516,   517,   518,     0,     0,   519,   520,   521,   522,   523,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
     526,     0,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,   538,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   487,   488,     0,     0,     0,     0,     0,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,     0,     0,     0,   513,   514,   515,
     516,   517,   518,     0,     0,   519,   520,   521,   522,   523,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     526,     0,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   487,   488,     0,     0,   497,   498,   499,   500,
     501,   537,   538,   504,   505,   506,   507,     0,   509,   510,
     511,   512,     0,     0,     0,   513,     0,   515,   516,     0,
       0,   617,     0,   519,   520,   521,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     487,   488,     0,     0,     0,     0,     0,     0,     0,   537,
     538,     0,     0,     0,     0,   497,   498,   499,   500,   501,
       0,     0,   504,   505,   506,   507,     0,   509,   510,   511,
     512,     0,     0,     0,   513,     0,   515,   516,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   526,   794,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   487,
     488,     0,     0,   497,   498,   499,   500,   501,   537,   538,
     504,   505,   506,   507,     0,   509,   510,   511,   512,     0,
       0,     0,   513,     0,   515,   516,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   852,     0,     0,     0,   526,     0,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   487,   488,     0,
       0,     0,     0,     0,     0,     0,   537,   538,     0,     0,
       0,     0,   497,   498,   499,   500,   501,     0,     0,   504,
     505,   506,   507,     0,   509,   510,   511,   512,     0,     0,
       0,   513,     0,   515,   516,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   487,   488,     0,     0,
     497,   498,   499,   500,   501,   537,   538,   504,   505,   506,
     507,     0,   509,   510,   511,   512,     0,     0,     0,   513,
       0,   515,   516,     0,   487,   488,     0,   519,     0,   521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   537,   538,     0,     0,     0,     0,   497,
     498,   499,   500,   501,     0,     0,   504,   505,   506,   507,
       0,   509,   510,   511,   512,     0,     0,     0,   513,     0,
     515,   516,   487,   488,     0,     0,   519,   497,   498,   499,
     500,   501,     0,     0,   504,   505,   506,   507,     0,   509,
     510,   511,   512,     0,     0,     0,   513,     0,   515,   516,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   537,   538,     0,     0,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,     0,     0,   487,   488,     0,     0,     0,     0,     0,
     537,   538,     0,     0,     0,   497,   498,   499,   500,   501,
       0,     0,   504,   505,   506,   507,     0,   509,   510,   511,
     512,     0,     0,     0,   513,     0,   515,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,   532,   533,   534,   535,   536,   487,
     488,     0,     0,     0,     0,     0,   497,   498,   537,   538,
     501,     0,     0,   504,   505,   506,   507,     0,   509,   510,
     511,   512,     0,     0,     0,   513,     0,   515,   516,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,     0,
     527,   528,   529,   530,   531,     0,     0,     0,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     538,     0,   497,   498,   499,   500,   501,     0,     0,   504,
     505,   506,   507,     0,   509,   510,   511,   512,     0,     0,
       0,   513,     0,   515,   516,     0,     0,     0,     0,     0,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   144,     0,     0,     0,     0,
       0,   145,     0,     0,     0,     0,     0,     0,     0,   530,
     531,   532,   533,   534,   535,   536,   146,     0,     0,     0,
       0,     0,     0,     0,     0,   537,   538,     0,     0,     0,
       0,   147,     0,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,     0,     0,     0,     0,     0,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,   147,     0,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,     0,   146,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,   147,     0,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,     0,   146,     0,     0,     0,     0,   313,
     314,   315,     0,     0,     0,     0,     0,     0,    48,   147,
       0,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,     0,     0,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,   147,     0,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,     0,     0,
       0,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     209,   210,   211,   212,    47,     0,   213,   214,   215,   216,
     217,   218,     0,     0,     0,     0,     0,     0,     0,     0,
     562,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,     0,     0,   229,   230
};

static const yytype_int16 yycheck[] =
{
      12,    84,   235,   109,   275,   273,   421,   131,   290,   539,
     400,   419,   402,   284,   404,   315,   607,    32,   557,   109,
     559,     7,   561,     0,   615,    19,   308,   461,   360,     6,
     311,   312,    78,    18,   629,    34,    20,    21,    14,    15,
      19,    56,    57,    49,    46,    19,    21,   577,   578,    18,
     119,   114,    29,   128,    31,   113,    33,    49,   116,   117,
     136,    33,    39,   116,   117,   114,   355,    79,   145,    46,
     154,    32,   114,   114,   123,    52,   136,   139,    53,   178,
     145,   123,   123,   183,   183,   185,   114,    59,    14,    15,
     153,   154,   184,   156,    71,   123,   159,   109,   387,   183,
     115,   129,   145,   153,   180,   154,   105,   106,   185,   184,
     159,    32,   154,   154,   183,    92,   145,   241,   176,   177,
     185,   105,   106,   176,   177,   185,   154,   145,    58,   113,
     180,   115,   116,   117,   118,    56,    57,   156,   122,   154,
     155,   154,   185,   145,   159,   153,   145,   162,   419,   431,
     136,   156,   181,   145,   145,   145,   751,   129,   749,   153,
     145,   179,   177,   139,   179,   184,   272,   143,   450,   145,
     146,   186,   278,   145,   153,   183,   145,   183,   145,   153,
     153,   287,   272,   156,    20,    21,   159,   458,   278,   460,
     165,   177,   176,   177,   115,   185,   242,   287,   159,   185,
     177,   176,   153,   179,   485,   114,   183,   313,   314,   153,
     293,   543,   153,   139,   123,   745,   177,   143,   185,   145,
     146,   129,   305,   313,   314,   186,   238,   757,     4,     5,
     153,     7,   183,   154,   155,   153,   153,   145,   159,   183,
     145,   162,    11,   129,   634,   154,   785,   129,   839,   153,
     161,   153,   160,    22,    23,   178,   177,   787,   849,    35,
     272,   166,   183,   180,   153,   186,   278,    20,    21,   105,
     106,   145,   183,   153,   156,   287,   180,   113,   180,   115,
     116,   117,   118,   153,   145,   153,   122,   393,   394,   395,
     396,   180,   574,   399,   153,   401,   153,   403,   145,   405,
     180,   313,   314,   393,   394,   395,   396,   179,   178,   399,
     178,   401,   727,   403,   326,   749,   855,   161,   618,   178,
     601,   178,   161,   161,   758,   176,   162,   163,   164,   165,
     166,   129,    53,   147,   148,   149,    20,    21,    59,   183,
     176,   177,   425,   449,   183,   183,   183,   145,   185,    53,
     103,   104,   105,   106,   107,    59,   439,   110,   180,   449,
     113,   183,   115,   116,   117,   118,   139,    20,    21,   122,
      53,   124,   125,   145,    53,    66,    59,   145,   768,    70,
      59,   393,   394,   395,   396,   145,    53,   399,   184,   401,
     154,   403,    59,   405,   477,   803,    87,    88,    89,    90,
     147,   148,   149,   150,   147,   148,   184,   160,   161,   162,
     163,   164,   165,   166,   145,   181,   182,   429,   184,   543,
     156,   105,   106,   176,   177,   161,   841,   709,   184,   113,
     147,   148,   116,   117,   118,    42,    43,   449,   122,   145,
     145,   547,   184,   179,   156,    53,   145,   145,    34,   161,
     103,   104,   105,   106,    20,    21,    34,   547,   726,   179,
     113,   473,   115,   116,   117,   118,   572,   179,   156,   122,
     156,   124,   125,   161,   580,   156,   136,   179,   160,   156,
     161,   184,   572,   589,   590,   160,   592,   160,   160,   160,
     580,   179,   176,   177,   160,   160,   160,   160,   179,   589,
     590,   160,   592,   586,    32,   183,   156,   183,   145,   162,
     163,   164,   165,   166,   145,    32,   139,   179,   160,   160,
     160,   179,   179,   176,   177,   807,   160,   183,    56,    57,
     160,   182,   803,    36,   183,   547,   769,   103,   104,   105,
     106,   107,   180,   179,   110,   111,   112,   113,   183,   115,
     116,   117,   118,   177,   183,   183,   122,   183,   124,   125,
     572,   178,   184,   119,    13,   156,   139,   182,   580,   136,
     184,   145,   178,   145,   145,   145,   145,   589,   590,   178,
     592,   185,   145,   147,   160,   153,   179,   115,   153,     1,
     145,   235,   158,   159,   160,   161,   162,   163,   164,   165,
     166,     7,    42,   183,   145,   711,   145,    20,    21,   161,
     176,   177,   161,   161,   145,    32,   722,   178,   724,   178,
     178,   711,   147,   185,   268,   178,   154,   155,   179,   145,
     145,   159,   722,   161,   162,   145,   145,   281,   178,    56,
      57,   178,   178,   145,   178,   180,   290,   185,   178,   177,
     294,   183,   296,   161,   178,   161,   178,   178,   186,    49,
     304,   767,   185,   183,   308,   309,   310,   178,   465,    60,
      36,   315,   106,   178,   178,    63,   905,   767,    61,   323,
     324,   325,    79,   327,   328,   329,   698,   770,   698,   915,
     247,   876,   105,   106,   338,   627,     1,   114,   115,   711,
     113,   865,   115,   116,   117,   118,   123,   743,   801,   122,
     722,   406,   724,   423,    42,   724,   705,   540,    72,   756,
     461,   931,    -1,    -1,   398,    -1,    -1,   287,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
      -1,    -1,   159,    -1,   827,   162,    -1,    -1,    -1,    -1,
      -1,   164,   165,   166,    -1,   767,    -1,    -1,    -1,    -1,
     177,    -1,   868,   176,   177,    -1,   183,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,   858,    -1,    -1,   868,    -1,
      -1,    -1,    -1,    -1,   867,    -1,    -1,   431,    -1,    -1,
      -1,    -1,    -1,    -1,   438,    -1,    -1,    -1,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   450,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   459,    -1,   461,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,    -1,   868,   501,   502,   503,
     504,   505,   506,    -1,   508,   509,    -1,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,    -1,   538,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,   933,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
     574,    20,    21,    -1,    -1,   579,    -1,   949,    -1,   583,
     584,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   603,
     604,    -1,    32,    -1,    -1,    -1,    -1,    -1,   176,   177,
     614,    -1,    -1,   617,   618,    -1,   184,   621,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   656,   657,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,   675,   676,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   696,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,    -1,    -1,   709,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,    -1,   176,   177,   159,
      -1,    -1,   162,    -1,   183,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,   747,   748,   159,    -1,   177,   162,    -1,
     754,    -1,   756,   183,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   769,    -1,    -1,    -1,   773,
      -1,    -1,   186,   777,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,
     794,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,   807,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      41,    -1,   826,    44,    -1,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   887,   105,   106,   890,   891,   892,   893,
     894,    -1,    -1,   897,   898,    -1,   900,    -1,    -1,   120,
     121,    -1,    -1,    -1,    -1,   909,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,   936,    -1,    -1,    -1,    -1,   941,    -1,    -1,
      -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,   170,
     954,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
     181,   182,   183,   184,   185,     1,    -1,    -1,    -1,    -1,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    -1,    -1,    44,    -1,
      46,    47,    48,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    32,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    -1,    -1,    -1,    14,    15,
      16,    17,    18,   179,    -1,   181,   182,   183,   184,   185,
      26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    49,    -1,    51,    -1,    -1,    -1,   115,
      56,    57,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,   177,    -1,   179,    -1,   181,   182,    26,   184,    -1,
     186,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    56,    57,    -1,    -1,   154,   155,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,   177,    -1,
     179,    -1,   181,   182,    26,   184,    -1,   186,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   154,
     155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    32,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,   166,    -1,    -1,   169,   170,    14,
      15,    16,    17,    18,    -1,   177,    -1,   179,    -1,   181,
     182,    26,   184,    -1,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,   177,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,   186,    -1,   129,    -1,    32,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
      -1,   166,    -1,    -1,   169,   170,    -1,    -1,    14,    15,
      16,    17,    18,    -1,   179,    -1,   181,   182,   183,   184,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,   105,
     106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,   177,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,
     186,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,   162,   163,   164,    26,
     166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   179,    41,   181,   182,    44,   184,    -1,
      47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   154,   155,    93,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,   186,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,
      -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   179,    41,   181,   182,    44,   184,    -1,    47,
      -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   154,   155,    93,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    32,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    14,    15,    16,    17,    18,    -1,   177,
      -1,   179,   180,   181,   182,    26,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     154,   155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,   120,
     121,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    56,    57,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,   162,   163,   164,    26,   166,    -1,    -1,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,
     181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   154,
     155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      56,    57,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     162,   163,   164,    26,   166,    -1,    -1,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,
     182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   154,   155,
      93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,
      57,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,
     163,   164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    38,   178,   179,    41,   181,   182,
      44,   184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   154,   155,    93,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    21,    -1,   162,   163,
     164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,    44,
     184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   154,   155,    93,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
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
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      32,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   179,   180,   181,   182,    26,   184,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   154,   155,    93,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    32,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     179,   180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   154,
     155,    93,    -1,    -1,   159,    -1,    -1,   162,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,   160,    -1,
     162,   163,   164,    26,   166,    -1,    -1,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,
     182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   179,   180,   181,   182,
      26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
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
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,   162,   163,   164,
      26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   179,    41,   181,   182,    44,   184,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,
     166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,   181,   182,    -1,   184,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    20,    21,    -1,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    20,    21,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    20,    21,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
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
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,   176,   177,   103,   104,   105,   106,
     107,   183,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    20,    21,   180,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    20,
      21,   180,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    20,
      21,   180,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    20,    21,   180,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    20,    21,   180,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    20,    21,   180,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   176,   177,    -1,    -1,   180,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,   176,   177,   178,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,   178,
     103,   104,   105,   106,   107,    20,    21,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,   178,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    20,    21,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    20,    21,    -1,    -1,   103,   104,   105,   106,
     107,   176,   177,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,   128,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    20,
      21,    -1,    -1,   103,   104,   105,   106,   107,   176,   177,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    20,    21,    -1,    -1,
     103,   104,   105,   106,   107,   176,   177,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    20,    21,    -1,   130,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    20,    21,    -1,    -1,   130,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,    20,
      21,    -1,    -1,    -1,    -1,    -1,   103,   104,   176,   177,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
     161,   162,   163,   164,   165,   166,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,
      -1,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    45,    -1,    -1,    -1,    -1,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,   145,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   129,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,   169,   170
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   188,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   177,   183,   191,   199,   200,   204,   225,   229,
     245,   300,   305,   307,   312,   348,   350,    18,   145,   219,
     220,   221,   139,   205,   206,   145,   166,   201,   202,   184,
     145,   181,   190,   351,   349,    33,    59,   129,   145,   222,
     223,   224,   237,     4,     5,     7,    35,   310,    58,   298,
     154,   153,   156,   153,   201,    21,    53,   165,   176,   203,
     145,   303,   304,   298,   145,   145,   145,   129,   179,   153,
     178,    53,    59,   230,   231,   232,    53,    59,   306,    53,
      59,   311,    53,    59,   299,    14,    15,   139,   143,   145,
     146,   179,   193,   220,   139,   206,   145,   145,   145,   154,
     185,   303,    53,    59,   189,   184,   184,   145,   221,   223,
      34,   105,   106,   145,   228,   184,   233,   145,   145,   309,
     184,   301,   147,   148,   192,    14,    15,   139,   143,   145,
     193,   217,   218,   203,    24,    30,    45,    60,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,   237,
     315,   317,   318,   320,   323,   145,   319,   145,   290,   291,
     180,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   124,
     125,   126,   127,   130,   131,   132,   133,   134,   135,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   169,
     170,    34,    34,   179,   226,   235,   156,   184,   156,   308,
     313,   297,   136,   281,   147,   148,   149,   153,   180,   160,
     160,   160,   160,   179,   160,   160,   160,   160,   160,   160,
      32,    56,    57,   115,   154,   155,   159,   162,   177,   183,
     186,   183,   156,   183,   180,   281,   286,   287,   156,   227,
       1,     8,     9,    10,    12,    14,    15,    16,    17,    18,
      25,    26,    27,    28,    37,    38,    40,    41,    44,    47,
      48,    50,    51,    54,    55,    61,    71,    93,    94,   105,
     106,   120,   121,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   146,   162,   163,   164,   166,   169,   170,   179,
     181,   182,   183,   185,   195,   197,   207,   208,   211,   212,
     213,   214,   215,   216,   233,   234,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   254,   257,
     260,   264,   265,   266,   267,   268,   269,   270,   275,   278,
     315,   352,   356,   361,   363,   365,    66,    70,    87,    88,
      89,    90,   316,   302,   237,   184,   185,   281,   221,   145,
     296,   314,   217,   326,   328,   330,   324,   145,   321,   332,
     334,   336,   338,   340,   342,   344,   346,    32,    32,   159,
     177,   186,   178,   278,   145,   185,   323,   185,   290,    46,
     293,   314,   180,   183,   323,   278,   314,   323,   352,   179,
     160,   114,   183,   236,   266,   278,   233,   278,   179,   278,
     160,   160,   179,   183,   160,   139,    54,   278,   233,   160,
     114,   236,   278,   278,   278,   182,   264,   264,    12,   323,
      12,   323,   278,   358,   362,   194,   278,   278,   278,   237,
     278,   278,   278,   182,    36,   183,   183,   278,   145,   179,
     183,   183,   183,   296,   225,   119,   183,    20,    21,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   122,   123,   124,   125,   126,   127,   130,
     131,   132,   133,   134,   135,   154,   155,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   176,   177,   179,
     184,   145,   185,   284,   296,   178,   153,   156,   159,   295,
     323,   323,   323,   323,   180,   319,   323,   226,   323,   226,
     323,   226,   145,   288,   289,   323,   291,   178,   178,   293,
     114,   154,   156,   159,   292,   286,   233,    19,   179,   237,
     255,   236,   278,   114,   154,   183,    13,   278,   233,   248,
     250,   278,   252,   156,   139,   323,   236,   278,   183,   182,
     314,    49,   145,   162,   179,   278,   353,   354,   355,   357,
     358,   359,   360,   314,   179,   354,   360,   128,   183,   185,
     149,   150,   192,   198,   180,   160,   237,   184,   233,   177,
     180,   258,   278,   136,   263,   264,    18,   145,   145,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   145,   120,   121,   278,   278,
     145,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   120,   121,   278,    21,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     145,   276,   278,   180,   258,   302,   154,   183,   177,   185,
     225,   281,   283,   178,   145,   323,   114,   123,   154,   294,
     161,   183,   161,   161,   161,   183,   161,   227,   161,   227,
     161,   227,   156,   161,   183,   161,   183,   323,   236,   278,
     258,   180,   258,   160,   278,   323,   183,   233,   180,   323,
     323,   180,   323,   161,   183,    19,   264,   123,   292,   180,
     153,   183,   355,   178,   153,   178,   183,    19,   180,   355,
     185,   278,   358,   185,   278,   147,   196,   271,   273,   235,
      11,    22,    23,   209,   210,   145,   353,   153,   180,   114,
     123,   154,   159,   261,   262,   226,   160,   179,   278,   278,
     178,   278,   278,   145,   156,     1,   178,   180,   185,   278,
     224,     7,    42,   282,   183,   154,   183,   294,   236,   278,
     327,   323,   331,   325,   322,   333,   161,   337,   161,   341,
     161,   323,   345,   288,   347,   154,   292,   183,   180,   145,
     180,   161,   161,   161,   161,   263,   258,   278,   278,   354,
     355,   145,   353,   178,   178,   278,   178,   359,   258,   354,
     185,   178,   151,   147,   323,   226,   185,   233,   278,   178,
     278,   145,   145,   145,   145,   153,   178,   227,   279,   180,
     258,   278,   277,   183,   178,    43,   282,   293,   236,   278,
     183,   161,   335,   339,   343,   278,   233,   161,   183,   256,
     249,   251,   253,   179,   183,   355,   178,   123,   292,   178,
     183,   355,   178,   161,   227,   233,   180,   261,   178,   128,
     233,   259,   323,   180,   183,   285,   232,   183,   329,   278,
     145,   278,   278,   278,   180,   278,   278,   178,   178,   278,
     278,   278,   185,   272,   161,   210,   114,   278,   161,   231,
     180,   161,   180,   183,   364,   178,   364,   178,   237,   274,
     278,   280,   183,   278,    49,   178,   185,   237,   180,   278,
     178,   178
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   187,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   189,   190,   190,
     191,   192,   192,   192,   192,   193,   194,   194,   194,   195,
     196,   196,   198,   197,   199,   200,   201,   201,   201,   201,
     202,   202,   203,   203,   204,   205,   205,   206,   206,   207,
     208,   208,   209,   209,   210,   210,   210,   211,   211,   212,
     213,   214,   215,   216,   217,   217,   217,   217,   217,   217,
     218,   218,   219,   219,   220,   220,   220,   220,   220,   220,
     220,   220,   221,   221,   222,   222,   222,   223,   223,   224,
     224,   225,   225,   226,   226,   226,   227,   227,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   229,   230,   230,   230,   231,   232,   233,   233,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   235,   235,
     235,   236,   236,   236,   237,   237,   237,   238,   239,   239,
     239,   239,   240,   241,   242,   242,   242,   242,   242,   243,
     243,   243,   243,   244,   245,   245,   246,   248,   249,   247,
     250,   251,   247,   252,   253,   247,   255,   256,   254,   257,
     257,   257,   258,   258,   259,   259,   259,   260,   260,   260,
     261,   261,   261,   261,   262,   262,   263,   263,   264,   264,
     265,   265,   265,   265,   265,   265,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   267,   267,   267,   267,   268,
     269,   269,   270,   271,   272,   270,   273,   274,   270,   275,
     276,   277,   275,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   279,   280,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   281,   281,   282,   282,   283,   284,   284,
     285,   284,   284,   284,   286,   286,   287,   287,   288,   288,
     289,   289,   290,   291,   291,   292,   292,   293,   293,   293,
     293,   293,   293,   294,   294,   294,   295,   295,   296,   296,
     296,   296,   296,   297,   297,   298,   298,   299,   299,   299,
     300,   301,   300,   302,   302,   302,   303,   304,   304,   305,
     306,   306,   306,   307,   307,   308,   308,   309,   310,   310,
     311,   311,   311,   313,   312,   314,   314,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   316,   316,   316,   316,   316,   316,   317,   318,
     318,   319,   319,   321,   322,   320,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   324,   325,   323,   323,   326,   327,   323,
     328,   329,   323,   330,   331,   323,   323,   332,   333,   323,
     334,   335,   323,   323,   336,   337,   323,   338,   339,   323,
     323,   340,   341,   323,   342,   343,   323,   344,   345,   323,
     346,   347,   323,   349,   348,   351,   350,   352,   352,   352,
     352,   353,   353,   353,   353,   354,   354,   355,   355,   356,
     356,   356,   356,   356,   356,   357,   357,   358,   359,   359,
     360,   360,   361,   361,   362,   362,   363,   364,   364,   365,
     365
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
       1,     3,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     1,     1,     1,     1,     4,     1,
       3,     0,     3,     0,     2,     3,     0,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     0,     1,     1,     4,     2,     3,     7,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     2,     2,     1,     3,     2,     2,     2,     4,
       5,     2,     1,     1,     2,     3,     4,     2,     3,     3,
       4,     2,     3,     4,     1,     1,     2,     0,     0,     7,
       0,     0,     7,     0,     0,     7,     0,     0,     6,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     1,     3,     0,     4,     1,     6,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     6,
       5,     6,     3,     0,     0,     8,     0,     0,     9,     3,
       0,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     4,     3,     1,     3,     4,     3,     4,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     0,     4,     0,     1,     3,     0,     3,
       0,     7,     5,     5,     2,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     1,     1,     2,     3,
       5,     3,     3,     1,     1,     1,     0,     1,     4,     6,
       5,     5,     4,     0,     3,     0,     1,     0,     1,     1,
       6,     0,     6,     0,     3,     5,     4,     1,     2,     4,
       0,     1,     1,     7,     9,     0,     2,     2,     1,     1,
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3412 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3418 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3424 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3430 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3436 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3442 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3448 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3454 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3460 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3466 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3472 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3478 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3484 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3490 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3496 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3502 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3508 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3514 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3520 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3526 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3532 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3538 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3544 "ds_parser.cpp"
        break;

    case 224: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3550 "ds_parser.cpp"
        break;

    case 225: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3556 "ds_parser.cpp"
        break;

    case 226: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3562 "ds_parser.cpp"
        break;

    case 227: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3568 "ds_parser.cpp"
        break;

    case 228: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3574 "ds_parser.cpp"
        break;

    case 233: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3580 "ds_parser.cpp"
        break;

    case 234: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3586 "ds_parser.cpp"
        break;

    case 235: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3592 "ds_parser.cpp"
        break;

    case 236: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3598 "ds_parser.cpp"
        break;

    case 237: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3604 "ds_parser.cpp"
        break;

    case 238: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3610 "ds_parser.cpp"
        break;

    case 239: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3616 "ds_parser.cpp"
        break;

    case 240: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3622 "ds_parser.cpp"
        break;

    case 241: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3628 "ds_parser.cpp"
        break;

    case 242: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3634 "ds_parser.cpp"
        break;

    case 243: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3640 "ds_parser.cpp"
        break;

    case 244: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3646 "ds_parser.cpp"
        break;

    case 246: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3652 "ds_parser.cpp"
        break;

    case 247: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp"
        break;

    case 254: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 257: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 258: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3676 "ds_parser.cpp"
        break;

    case 259: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3682 "ds_parser.cpp"
        break;

    case 261: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3688 "ds_parser.cpp"
        break;

    case 262: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3694 "ds_parser.cpp"
        break;

    case 263: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3700 "ds_parser.cpp"
        break;

    case 264: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3706 "ds_parser.cpp"
        break;

    case 265: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3712 "ds_parser.cpp"
        break;

    case 266: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3718 "ds_parser.cpp"
        break;

    case 267: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3724 "ds_parser.cpp"
        break;

    case 268: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3730 "ds_parser.cpp"
        break;

    case 269: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3736 "ds_parser.cpp"
        break;

    case 270: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3742 "ds_parser.cpp"
        break;

    case 275: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3748 "ds_parser.cpp"
        break;

    case 278: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3754 "ds_parser.cpp"
        break;

    case 281: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3760 "ds_parser.cpp"
        break;

    case 283: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3766 "ds_parser.cpp"
        break;

    case 284: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3772 "ds_parser.cpp"
        break;

    case 286: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3778 "ds_parser.cpp"
        break;

    case 287: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3784 "ds_parser.cpp"
        break;

    case 288: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3790 "ds_parser.cpp"
        break;

    case 289: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3796 "ds_parser.cpp"
        break;

    case 290: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3802 "ds_parser.cpp"
        break;

    case 291: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3808 "ds_parser.cpp"
        break;

    case 293: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3814 "ds_parser.cpp"
        break;

    case 296: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3820 "ds_parser.cpp"
        break;

    case 297: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3826 "ds_parser.cpp"
        break;

    case 302: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3832 "ds_parser.cpp"
        break;

    case 308: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3838 "ds_parser.cpp"
        break;

    case 314: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3844 "ds_parser.cpp"
        break;

    case 317: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3850 "ds_parser.cpp"
        break;

    case 318: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3856 "ds_parser.cpp"
        break;

    case 319: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3862 "ds_parser.cpp"
        break;

    case 320: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3868 "ds_parser.cpp"
        break;

    case 323: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3874 "ds_parser.cpp"
        break;

    case 352: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3880 "ds_parser.cpp"
        break;

    case 353: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3886 "ds_parser.cpp"
        break;

    case 354: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3892 "ds_parser.cpp"
        break;

    case 355: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3898 "ds_parser.cpp"
        break;

    case 356: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3904 "ds_parser.cpp"
        break;

    case 357: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3910 "ds_parser.cpp"
        break;

    case 358: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3916 "ds_parser.cpp"
        break;

    case 359: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3922 "ds_parser.cpp"
        break;

    case 360: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3928 "ds_parser.cpp"
        break;

    case 361: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3934 "ds_parser.cpp"
        break;

    case 362: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3940 "ds_parser.cpp"
        break;

    case 363: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3946 "ds_parser.cpp"
        break;

    case 364: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3952 "ds_parser.cpp"
        break;

    case 365: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3958 "ds_parser.cpp"
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
#line 469 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4250 "ds_parser.cpp"
    break;

  case 16:
#line 470 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4256 "ds_parser.cpp"
    break;

  case 17:
#line 471 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4262 "ds_parser.cpp"
    break;

  case 18:
#line 475 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4268 "ds_parser.cpp"
    break;

  case 19:
#line 476 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4274 "ds_parser.cpp"
    break;

  case 20:
#line 480 "ds_parser.ypp"
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
#line 4293 "ds_parser.cpp"
    break;

  case 21:
#line 497 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4299 "ds_parser.cpp"
    break;

  case 22:
#line 498 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4305 "ds_parser.cpp"
    break;

  case 23:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4311 "ds_parser.cpp"
    break;

  case 24:
#line 500 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4317 "ds_parser.cpp"
    break;

  case 25:
#line 504 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4323 "ds_parser.cpp"
    break;

  case 26:
#line 508 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4333 "ds_parser.cpp"
    break;

  case 27:
#line 513 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4347 "ds_parser.cpp"
    break;

  case 28:
#line 522 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4357 "ds_parser.cpp"
    break;

  case 29:
#line 530 "ds_parser.ypp"
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
#line 4375 "ds_parser.cpp"
    break;

  case 30:
#line 546 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4385 "ds_parser.cpp"
    break;

  case 31:
#line 551 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4395 "ds_parser.cpp"
    break;

  case 32:
#line 559 "ds_parser.ypp"
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
#line 4422 "ds_parser.cpp"
    break;

  case 33:
#line 580 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4434 "ds_parser.cpp"
    break;

  case 34:
#line 589 "ds_parser.ypp"
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
#line 4458 "ds_parser.cpp"
    break;

  case 36:
#line 615 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4466 "ds_parser.cpp"
    break;

  case 37:
#line 618 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4475 "ds_parser.cpp"
    break;

  case 38:
#line 622 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4486 "ds_parser.cpp"
    break;

  case 39:
#line 628 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4497 "ds_parser.cpp"
    break;

  case 40:
#line 637 "ds_parser.ypp"
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
#line 4523 "ds_parser.cpp"
    break;

  case 41:
#line 658 "ds_parser.ypp"
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
#line 4548 "ds_parser.cpp"
    break;

  case 42:
#line 681 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4554 "ds_parser.cpp"
    break;

  case 43:
#line 682 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4560 "ds_parser.cpp"
    break;

  case 47:
#line 695 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4568 "ds_parser.cpp"
    break;

  case 48:
#line 698 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4576 "ds_parser.cpp"
    break;

  case 49:
#line 704 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4584 "ds_parser.cpp"
    break;

  case 50:
#line 710 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4592 "ds_parser.cpp"
    break;

  case 51:
#line 713 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4600 "ds_parser.cpp"
    break;

  case 52:
#line 719 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4606 "ds_parser.cpp"
    break;

  case 53:
#line 720 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4612 "ds_parser.cpp"
    break;

  case 54:
#line 724 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4618 "ds_parser.cpp"
    break;

  case 55:
#line 725 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4624 "ds_parser.cpp"
    break;

  case 56:
#line 726 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4635 "ds_parser.cpp"
    break;

  case 57:
#line 735 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4641 "ds_parser.cpp"
    break;

  case 58:
#line 736 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4647 "ds_parser.cpp"
    break;

  case 59:
#line 740 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4658 "ds_parser.cpp"
    break;

  case 60:
#line 749 "ds_parser.ypp"
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
#line 4676 "ds_parser.cpp"
    break;

  case 61:
#line 765 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4686 "ds_parser.cpp"
    break;

  case 62:
#line 773 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4698 "ds_parser.cpp"
    break;

  case 63:
#line 783 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4709 "ds_parser.cpp"
    break;

  case 64:
#line 792 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4715 "ds_parser.cpp"
    break;

  case 65:
#line 793 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4721 "ds_parser.cpp"
    break;

  case 66:
#line 794 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4727 "ds_parser.cpp"
    break;

  case 67:
#line 795 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4733 "ds_parser.cpp"
    break;

  case 68:
#line 796 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4739 "ds_parser.cpp"
    break;

  case 69:
#line 797 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4745 "ds_parser.cpp"
    break;

  case 70:
#line 801 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4755 "ds_parser.cpp"
    break;

  case 71:
#line 806 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4765 "ds_parser.cpp"
    break;

  case 72:
#line 814 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4771 "ds_parser.cpp"
    break;

  case 73:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4777 "ds_parser.cpp"
    break;

  case 74:
#line 819 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4783 "ds_parser.cpp"
    break;

  case 75:
#line 820 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4789 "ds_parser.cpp"
    break;

  case 76:
#line 821 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4795 "ds_parser.cpp"
    break;

  case 77:
#line 822 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4801 "ds_parser.cpp"
    break;

  case 78:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4807 "ds_parser.cpp"
    break;

  case 79:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4813 "ds_parser.cpp"
    break;

  case 80:
#line 825 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4819 "ds_parser.cpp"
    break;

  case 81:
#line 826 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4827 "ds_parser.cpp"
    break;

  case 82:
#line 832 "ds_parser.ypp"
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
#line 4846 "ds_parser.cpp"
    break;

  case 83:
#line 846 "ds_parser.ypp"
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
#line 4865 "ds_parser.cpp"
    break;

  case 84:
#line 863 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4871 "ds_parser.cpp"
    break;

  case 85:
#line 864 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4877 "ds_parser.cpp"
    break;

  case 86:
#line 865 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4883 "ds_parser.cpp"
    break;

  case 87:
#line 869 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4896 "ds_parser.cpp"
    break;

  case 88:
#line 877 "ds_parser.ypp"
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
#line 4911 "ds_parser.cpp"
    break;

  case 89:
#line 890 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4920 "ds_parser.cpp"
    break;

  case 90:
#line 894 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4929 "ds_parser.cpp"
    break;

  case 91:
#line 901 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4935 "ds_parser.cpp"
    break;

  case 92:
#line 902 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4941 "ds_parser.cpp"
    break;

  case 93:
#line 906 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4947 "ds_parser.cpp"
    break;

  case 94:
#line 907 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4953 "ds_parser.cpp"
    break;

  case 95:
#line 908 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4959 "ds_parser.cpp"
    break;

  case 96:
#line 912 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4967 "ds_parser.cpp"
    break;

  case 97:
#line 915 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4976 "ds_parser.cpp"
    break;

  case 98:
#line 922 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4985 "ds_parser.cpp"
    break;

  case 99:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4991 "ds_parser.cpp"
    break;

  case 100:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4997 "ds_parser.cpp"
    break;

  case 101:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5003 "ds_parser.cpp"
    break;

  case 102:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5009 "ds_parser.cpp"
    break;

  case 103:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5015 "ds_parser.cpp"
    break;

  case 104:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5021 "ds_parser.cpp"
    break;

  case 105:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5027 "ds_parser.cpp"
    break;

  case 106:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5033 "ds_parser.cpp"
    break;

  case 107:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5039 "ds_parser.cpp"
    break;

  case 108:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5045 "ds_parser.cpp"
    break;

  case 109:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5051 "ds_parser.cpp"
    break;

  case 110:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5057 "ds_parser.cpp"
    break;

  case 111:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5063 "ds_parser.cpp"
    break;

  case 112:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5069 "ds_parser.cpp"
    break;

  case 113:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5075 "ds_parser.cpp"
    break;

  case 114:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5081 "ds_parser.cpp"
    break;

  case 115:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5087 "ds_parser.cpp"
    break;

  case 116:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5093 "ds_parser.cpp"
    break;

  case 117:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5099 "ds_parser.cpp"
    break;

  case 118:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5105 "ds_parser.cpp"
    break;

  case 119:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5111 "ds_parser.cpp"
    break;

  case 120:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5117 "ds_parser.cpp"
    break;

  case 121:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5123 "ds_parser.cpp"
    break;

  case 122:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5129 "ds_parser.cpp"
    break;

  case 123:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5135 "ds_parser.cpp"
    break;

  case 124:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5141 "ds_parser.cpp"
    break;

  case 125:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5147 "ds_parser.cpp"
    break;

  case 126:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5153 "ds_parser.cpp"
    break;

  case 127:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5159 "ds_parser.cpp"
    break;

  case 128:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5165 "ds_parser.cpp"
    break;

  case 129:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5171 "ds_parser.cpp"
    break;

  case 130:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5177 "ds_parser.cpp"
    break;

  case 131:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5183 "ds_parser.cpp"
    break;

  case 132:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5189 "ds_parser.cpp"
    break;

  case 133:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5195 "ds_parser.cpp"
    break;

  case 134:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5201 "ds_parser.cpp"
    break;

  case 135:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5207 "ds_parser.cpp"
    break;

  case 136:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5213 "ds_parser.cpp"
    break;

  case 137:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5219 "ds_parser.cpp"
    break;

  case 138:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5225 "ds_parser.cpp"
    break;

  case 139:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5231 "ds_parser.cpp"
    break;

  case 140:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5237 "ds_parser.cpp"
    break;

  case 141:
#line 971 "ds_parser.ypp"
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
#line 5261 "ds_parser.cpp"
    break;

  case 142:
#line 993 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5267 "ds_parser.cpp"
    break;

  case 143:
#line 994 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5273 "ds_parser.cpp"
    break;

  case 144:
#line 995 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5279 "ds_parser.cpp"
    break;

  case 145:
#line 999 "ds_parser.ypp"
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
#line 5320 "ds_parser.cpp"
    break;

  case 146:
#line 1038 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5329 "ds_parser.cpp"
    break;

  case 147:
#line 1045 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5338 "ds_parser.cpp"
    break;

  case 148:
#line 1049 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5351 "ds_parser.cpp"
    break;

  case 149:
#line 1060 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5357 "ds_parser.cpp"
    break;

  case 150:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5363 "ds_parser.cpp"
    break;

  case 151:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5369 "ds_parser.cpp"
    break;

  case 152:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5375 "ds_parser.cpp"
    break;

  case 153:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5381 "ds_parser.cpp"
    break;

  case 154:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5387 "ds_parser.cpp"
    break;

  case 155:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5393 "ds_parser.cpp"
    break;

  case 156:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5399 "ds_parser.cpp"
    break;

  case 157:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5405 "ds_parser.cpp"
    break;

  case 158:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5411 "ds_parser.cpp"
    break;

  case 159:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5417 "ds_parser.cpp"
    break;

  case 160:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5423 "ds_parser.cpp"
    break;

  case 161:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5429 "ds_parser.cpp"
    break;

  case 162:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5435 "ds_parser.cpp"
    break;

  case 163:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5441 "ds_parser.cpp"
    break;

  case 164:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5447 "ds_parser.cpp"
    break;

  case 165:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5453 "ds_parser.cpp"
    break;

  case 166:
#line 1077 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5459 "ds_parser.cpp"
    break;

  case 167:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5465 "ds_parser.cpp"
    break;

  case 168:
#line 1082 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5475 "ds_parser.cpp"
    break;

  case 169:
#line 1087 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5486 "ds_parser.cpp"
    break;

  case 170:
#line 1093 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5494 "ds_parser.cpp"
    break;

  case 171:
#line 1099 "ds_parser.ypp"
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
#line 5533 "ds_parser.cpp"
    break;

  case 172:
#line 1133 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5541 "ds_parser.cpp"
    break;

  case 173:
#line 1136 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5549 "ds_parser.cpp"
    break;

  case 174:
#line 1142 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5555 "ds_parser.cpp"
    break;

  case 175:
#line 1143 "ds_parser.ypp"
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
#line 5571 "ds_parser.cpp"
    break;

  case 176:
#line 1154 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5577 "ds_parser.cpp"
    break;

  case 177:
#line 1158 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5585 "ds_parser.cpp"
    break;

  case 178:
#line 1164 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5593 "ds_parser.cpp"
    break;

  case 179:
#line 1167 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5601 "ds_parser.cpp"
    break;

  case 180:
#line 1170 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5610 "ds_parser.cpp"
    break;

  case 181:
#line 1174 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5618 "ds_parser.cpp"
    break;

  case 182:
#line 1180 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5624 "ds_parser.cpp"
    break;

  case 183:
#line 1184 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5630 "ds_parser.cpp"
    break;

  case 184:
#line 1188 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5638 "ds_parser.cpp"
    break;

  case 185:
#line 1191 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5646 "ds_parser.cpp"
    break;

  case 186:
#line 1194 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5656 "ds_parser.cpp"
    break;

  case 187:
#line 1199 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5664 "ds_parser.cpp"
    break;

  case 188:
#line 1202 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5674 "ds_parser.cpp"
    break;

  case 189:
#line 1210 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5682 "ds_parser.cpp"
    break;

  case 190:
#line 1213 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5692 "ds_parser.cpp"
    break;

  case 191:
#line 1218 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5700 "ds_parser.cpp"
    break;

  case 192:
#line 1221 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5710 "ds_parser.cpp"
    break;

  case 193:
#line 1229 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5718 "ds_parser.cpp"
    break;

  case 194:
#line 1235 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5724 "ds_parser.cpp"
    break;

  case 195:
#line 1236 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5730 "ds_parser.cpp"
    break;

  case 196:
#line 1240 "ds_parser.ypp"
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
#line 5766 "ds_parser.cpp"
    break;

  case 197:
#line 1274 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5772 "ds_parser.cpp"
    break;

  case 198:
#line 1274 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5778 "ds_parser.cpp"
    break;

  case 199:
#line 1274 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5786 "ds_parser.cpp"
    break;

  case 200:
#line 1277 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5792 "ds_parser.cpp"
    break;

  case 201:
#line 1277 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5798 "ds_parser.cpp"
    break;

  case 202:
#line 1277 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5808 "ds_parser.cpp"
    break;

  case 203:
#line 1282 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5814 "ds_parser.cpp"
    break;

  case 204:
#line 1282 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5820 "ds_parser.cpp"
    break;

  case 205:
#line 1282 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5830 "ds_parser.cpp"
    break;

  case 206:
#line 1290 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5836 "ds_parser.cpp"
    break;

  case 207:
#line 1290 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5842 "ds_parser.cpp"
    break;

  case 208:
#line 1290 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5850 "ds_parser.cpp"
    break;

  case 209:
#line 1296 "ds_parser.ypp"
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
#line 5865 "ds_parser.cpp"
    break;

  case 210:
#line 1306 "ds_parser.ypp"
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
#line 5881 "ds_parser.cpp"
    break;

  case 211:
#line 1317 "ds_parser.ypp"
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
#line 5898 "ds_parser.cpp"
    break;

  case 212:
#line 1332 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5906 "ds_parser.cpp"
    break;

  case 213:
#line 1335 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5914 "ds_parser.cpp"
    break;

  case 214:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5920 "ds_parser.cpp"
    break;

  case 215:
#line 1342 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5932 "ds_parser.cpp"
    break;

  case 216:
#line 1349 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5945 "ds_parser.cpp"
    break;

  case 217:
#line 1360 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5951 "ds_parser.cpp"
    break;

  case 218:
#line 1361 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5957 "ds_parser.cpp"
    break;

  case 219:
#line 1362 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5963 "ds_parser.cpp"
    break;

  case 220:
#line 1366 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5969 "ds_parser.cpp"
    break;

  case 221:
#line 1367 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5975 "ds_parser.cpp"
    break;

  case 222:
#line 1368 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5981 "ds_parser.cpp"
    break;

  case 223:
#line 1369 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5987 "ds_parser.cpp"
    break;

  case 224:
#line 1373 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5997 "ds_parser.cpp"
    break;

  case 225:
#line 1378 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6007 "ds_parser.cpp"
    break;

  case 226:
#line 1386 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6013 "ds_parser.cpp"
    break;

  case 227:
#line 1387 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6019 "ds_parser.cpp"
    break;

  case 228:
#line 1391 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6029 "ds_parser.cpp"
    break;

  case 229:
#line 1397 "ds_parser.ypp"
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
#line 6094 "ds_parser.cpp"
    break;

  case 230:
#line 1460 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6100 "ds_parser.cpp"
    break;

  case 231:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6106 "ds_parser.cpp"
    break;

  case 232:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6112 "ds_parser.cpp"
    break;

  case 233:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6118 "ds_parser.cpp"
    break;

  case 234:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6124 "ds_parser.cpp"
    break;

  case 235:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6130 "ds_parser.cpp"
    break;

  case 236:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6136 "ds_parser.cpp"
    break;

  case 237:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6142 "ds_parser.cpp"
    break;

  case 238:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6148 "ds_parser.cpp"
    break;

  case 239:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6154 "ds_parser.cpp"
    break;

  case 240:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6160 "ds_parser.cpp"
    break;

  case 241:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6166 "ds_parser.cpp"
    break;

  case 242:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6172 "ds_parser.cpp"
    break;

  case 243:
#line 1476 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6178 "ds_parser.cpp"
    break;

  case 244:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6184 "ds_parser.cpp"
    break;

  case 245:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6190 "ds_parser.cpp"
    break;

  case 246:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6196 "ds_parser.cpp"
    break;

  case 247:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6202 "ds_parser.cpp"
    break;

  case 248:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6208 "ds_parser.cpp"
    break;

  case 249:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6214 "ds_parser.cpp"
    break;

  case 250:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6220 "ds_parser.cpp"
    break;

  case 251:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6226 "ds_parser.cpp"
    break;

  case 252:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6232 "ds_parser.cpp"
    break;

  case 253:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6238 "ds_parser.cpp"
    break;

  case 254:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6244 "ds_parser.cpp"
    break;

  case 255:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6250 "ds_parser.cpp"
    break;

  case 256:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6256 "ds_parser.cpp"
    break;

  case 257:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6262 "ds_parser.cpp"
    break;

  case 258:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6268 "ds_parser.cpp"
    break;

  case 259:
#line 1498 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6280 "ds_parser.cpp"
    break;

  case 260:
#line 1509 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6290 "ds_parser.cpp"
    break;

  case 261:
#line 1514 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6302 "ds_parser.cpp"
    break;

  case 262:
#line 1524 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6311 "ds_parser.cpp"
    break;

  case 263:
#line 1528 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6317 "ds_parser.cpp"
    break;

  case 264:
#line 1528 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6323 "ds_parser.cpp"
    break;

  case 265:
#line 1528 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6335 "ds_parser.cpp"
    break;

  case 266:
#line 1535 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6341 "ds_parser.cpp"
    break;

  case 267:
#line 1535 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6347 "ds_parser.cpp"
    break;

  case 268:
#line 1535 "ds_parser.ypp"
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
#line 6363 "ds_parser.cpp"
    break;

  case 269:
#line 1549 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6372 "ds_parser.cpp"
    break;

  case 270:
#line 1553 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6378 "ds_parser.cpp"
    break;

  case 271:
#line 1553 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6384 "ds_parser.cpp"
    break;

  case 272:
#line 1553 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6393 "ds_parser.cpp"
    break;

  case 273:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6399 "ds_parser.cpp"
    break;

  case 274:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6405 "ds_parser.cpp"
    break;

  case 275:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6411 "ds_parser.cpp"
    break;

  case 276:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6417 "ds_parser.cpp"
    break;

  case 277:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6423 "ds_parser.cpp"
    break;

  case 278:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6429 "ds_parser.cpp"
    break;

  case 279:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6435 "ds_parser.cpp"
    break;

  case 280:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6441 "ds_parser.cpp"
    break;

  case 281:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6447 "ds_parser.cpp"
    break;

  case 282:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 283:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 284:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 285:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 286:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 287:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 288:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 289:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 290:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 291:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 292:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 293:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 294:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 295:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 296:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 297:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 298:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 299:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 300:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 301:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 302:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6573 "ds_parser.cpp"
    break;

  case 303:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 304:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 305:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6591 "ds_parser.cpp"
    break;

  case 306:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 307:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 308:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 309:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 310:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6621 "ds_parser.cpp"
    break;

  case 311:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6627 "ds_parser.cpp"
    break;

  case 312:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6633 "ds_parser.cpp"
    break;

  case 313:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6639 "ds_parser.cpp"
    break;

  case 314:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6645 "ds_parser.cpp"
    break;

  case 315:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6651 "ds_parser.cpp"
    break;

  case 316:
#line 1603 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6660 "ds_parser.cpp"
    break;

  case 317:
#line 1607 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6669 "ds_parser.cpp"
    break;

  case 318:
#line 1611 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6677 "ds_parser.cpp"
    break;

  case 319:
#line 1614 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6685 "ds_parser.cpp"
    break;

  case 320:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6691 "ds_parser.cpp"
    break;

  case 321:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6697 "ds_parser.cpp"
    break;

  case 322:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6703 "ds_parser.cpp"
    break;

  case 323:
#line 1620 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6717 "ds_parser.cpp"
    break;

  case 324:
#line 1629 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6731 "ds_parser.cpp"
    break;

  case 325:
#line 1638 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6737 "ds_parser.cpp"
    break;

  case 326:
#line 1639 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6745 "ds_parser.cpp"
    break;

  case 327:
#line 1642 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6751 "ds_parser.cpp"
    break;

  case 328:
#line 1642 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6757 "ds_parser.cpp"
    break;

  case 329:
#line 1642 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6765 "ds_parser.cpp"
    break;

  case 330:
#line 1645 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6774 "ds_parser.cpp"
    break;

  case 331:
#line 1649 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6783 "ds_parser.cpp"
    break;

  case 332:
#line 1653 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6792 "ds_parser.cpp"
    break;

  case 333:
#line 1657 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6798 "ds_parser.cpp"
    break;

  case 334:
#line 1658 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6804 "ds_parser.cpp"
    break;

  case 335:
#line 1659 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6810 "ds_parser.cpp"
    break;

  case 336:
#line 1660 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6816 "ds_parser.cpp"
    break;

  case 337:
#line 1661 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6822 "ds_parser.cpp"
    break;

  case 338:
#line 1662 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6828 "ds_parser.cpp"
    break;

  case 339:
#line 1663 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6834 "ds_parser.cpp"
    break;

  case 340:
#line 1664 "ds_parser.ypp"
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
#line 6855 "ds_parser.cpp"
    break;

  case 341:
#line 1680 "ds_parser.ypp"
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
#line 6876 "ds_parser.cpp"
    break;

  case 342:
#line 1696 "ds_parser.ypp"
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
#line 6936 "ds_parser.cpp"
    break;

  case 343:
#line 1754 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6942 "ds_parser.cpp"
    break;

  case 344:
#line 1755 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6948 "ds_parser.cpp"
    break;

  case 345:
#line 1759 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6954 "ds_parser.cpp"
    break;

  case 346:
#line 1760 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6960 "ds_parser.cpp"
    break;

  case 347:
#line 1764 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6970 "ds_parser.cpp"
    break;

  case 348:
#line 1772 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6978 "ds_parser.cpp"
    break;

  case 349:
#line 1775 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6987 "ds_parser.cpp"
    break;

  case 350:
#line 1780 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6993 "ds_parser.cpp"
    break;

  case 351:
#line 1780 "ds_parser.ypp"
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
#line 7042 "ds_parser.cpp"
    break;

  case 352:
#line 1824 "ds_parser.ypp"
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
#line 7103 "ds_parser.cpp"
    break;

  case 353:
#line 1880 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7114 "ds_parser.cpp"
    break;

  case 354:
#line 1889 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7124 "ds_parser.cpp"
    break;

  case 355:
#line 1894 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7134 "ds_parser.cpp"
    break;

  case 356:
#line 1902 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7140 "ds_parser.cpp"
    break;

  case 357:
#line 1903 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7146 "ds_parser.cpp"
    break;

  case 358:
#line 1907 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7154 "ds_parser.cpp"
    break;

  case 359:
#line 1910 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7165 "ds_parser.cpp"
    break;

  case 360:
#line 1919 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7171 "ds_parser.cpp"
    break;

  case 361:
#line 1920 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7177 "ds_parser.cpp"
    break;

  case 362:
#line 1924 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7188 "ds_parser.cpp"
    break;

  case 363:
#line 1933 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7194 "ds_parser.cpp"
    break;

  case 364:
#line 1934 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7200 "ds_parser.cpp"
    break;

  case 365:
#line 1939 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7206 "ds_parser.cpp"
    break;

  case 366:
#line 1940 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7212 "ds_parser.cpp"
    break;

  case 367:
#line 1944 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7223 "ds_parser.cpp"
    break;

  case 368:
#line 1950 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7234 "ds_parser.cpp"
    break;

  case 369:
#line 1956 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7242 "ds_parser.cpp"
    break;

  case 370:
#line 1959 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7251 "ds_parser.cpp"
    break;

  case 371:
#line 1963 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7262 "ds_parser.cpp"
    break;

  case 372:
#line 1969 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7273 "ds_parser.cpp"
    break;

  case 373:
#line 1978 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7279 "ds_parser.cpp"
    break;

  case 374:
#line 1979 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7285 "ds_parser.cpp"
    break;

  case 375:
#line 1980 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7291 "ds_parser.cpp"
    break;

  case 376:
#line 1984 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7297 "ds_parser.cpp"
    break;

  case 377:
#line 1985 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7303 "ds_parser.cpp"
    break;

  case 378:
#line 1989 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7311 "ds_parser.cpp"
    break;

  case 379:
#line 1992 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7321 "ds_parser.cpp"
    break;

  case 380:
#line 1997 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7331 "ds_parser.cpp"
    break;

  case 381:
#line 2002 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7344 "ds_parser.cpp"
    break;

  case 382:
#line 2010 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7357 "ds_parser.cpp"
    break;

  case 383:
#line 2021 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7365 "ds_parser.cpp"
    break;

  case 384:
#line 2024 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7375 "ds_parser.cpp"
    break;

  case 385:
#line 2032 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7381 "ds_parser.cpp"
    break;

  case 386:
#line 2033 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7387 "ds_parser.cpp"
    break;

  case 387:
#line 2037 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7393 "ds_parser.cpp"
    break;

  case 388:
#line 2038 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7399 "ds_parser.cpp"
    break;

  case 389:
#line 2039 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7405 "ds_parser.cpp"
    break;

  case 390:
#line 2043 "ds_parser.ypp"
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
#line 7438 "ds_parser.cpp"
    break;

  case 391:
#line 2071 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7444 "ds_parser.cpp"
    break;

  case 392:
#line 2071 "ds_parser.ypp"
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
#line 7480 "ds_parser.cpp"
    break;

  case 393:
#line 2105 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7488 "ds_parser.cpp"
    break;

  case 394:
#line 2108 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7502 "ds_parser.cpp"
    break;

  case 395:
#line 2117 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7516 "ds_parser.cpp"
    break;

  case 396:
#line 2130 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7530 "ds_parser.cpp"
    break;

  case 400:
#line 2151 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7536 "ds_parser.cpp"
    break;

  case 401:
#line 2152 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7542 "ds_parser.cpp"
    break;

  case 402:
#line 2153 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7548 "ds_parser.cpp"
    break;

  case 403:
#line 2157 "ds_parser.ypp"
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
#line 7581 "ds_parser.cpp"
    break;

  case 404:
#line 2185 "ds_parser.ypp"
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
#line 7615 "ds_parser.cpp"
    break;

  case 405:
#line 2217 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7621 "ds_parser.cpp"
    break;

  case 406:
#line 2218 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7627 "ds_parser.cpp"
    break;

  case 407:
#line 2222 "ds_parser.ypp"
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
#line 7666 "ds_parser.cpp"
    break;

  case 408:
#line 2259 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7672 "ds_parser.cpp"
    break;

  case 409:
#line 2260 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7678 "ds_parser.cpp"
    break;

  case 410:
#line 2264 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7684 "ds_parser.cpp"
    break;

  case 411:
#line 2265 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7690 "ds_parser.cpp"
    break;

  case 412:
#line 2266 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7696 "ds_parser.cpp"
    break;

  case 413:
#line 2271 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7702 "ds_parser.cpp"
    break;

  case 414:
#line 2271 "ds_parser.ypp"
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
#line 7788 "ds_parser.cpp"
    break;

  case 415:
#line 2355 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7800 "ds_parser.cpp"
    break;

  case 416:
#line 2362 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7811 "ds_parser.cpp"
    break;

  case 417:
#line 2371 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7817 "ds_parser.cpp"
    break;

  case 418:
#line 2372 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7823 "ds_parser.cpp"
    break;

  case 419:
#line 2373 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7829 "ds_parser.cpp"
    break;

  case 420:
#line 2374 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7835 "ds_parser.cpp"
    break;

  case 421:
#line 2375 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7841 "ds_parser.cpp"
    break;

  case 422:
#line 2376 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7847 "ds_parser.cpp"
    break;

  case 423:
#line 2377 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7853 "ds_parser.cpp"
    break;

  case 424:
#line 2378 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7859 "ds_parser.cpp"
    break;

  case 425:
#line 2379 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7865 "ds_parser.cpp"
    break;

  case 426:
#line 2380 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7871 "ds_parser.cpp"
    break;

  case 427:
#line 2381 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7877 "ds_parser.cpp"
    break;

  case 428:
#line 2382 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7883 "ds_parser.cpp"
    break;

  case 429:
#line 2383 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7889 "ds_parser.cpp"
    break;

  case 430:
#line 2384 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7895 "ds_parser.cpp"
    break;

  case 431:
#line 2385 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7901 "ds_parser.cpp"
    break;

  case 432:
#line 2386 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7907 "ds_parser.cpp"
    break;

  case 433:
#line 2387 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7913 "ds_parser.cpp"
    break;

  case 434:
#line 2388 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7919 "ds_parser.cpp"
    break;

  case 435:
#line 2389 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7925 "ds_parser.cpp"
    break;

  case 436:
#line 2390 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7931 "ds_parser.cpp"
    break;

  case 437:
#line 2391 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7937 "ds_parser.cpp"
    break;

  case 438:
#line 2392 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7943 "ds_parser.cpp"
    break;

  case 439:
#line 2393 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7949 "ds_parser.cpp"
    break;

  case 440:
#line 2394 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7955 "ds_parser.cpp"
    break;

  case 441:
#line 2395 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7961 "ds_parser.cpp"
    break;

  case 442:
#line 2399 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7967 "ds_parser.cpp"
    break;

  case 443:
#line 2400 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7973 "ds_parser.cpp"
    break;

  case 444:
#line 2401 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7979 "ds_parser.cpp"
    break;

  case 445:
#line 2402 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7985 "ds_parser.cpp"
    break;

  case 446:
#line 2403 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7991 "ds_parser.cpp"
    break;

  case 447:
#line 2404 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7997 "ds_parser.cpp"
    break;

  case 448:
#line 2408 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8010 "ds_parser.cpp"
    break;

  case 449:
#line 2419 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8019 "ds_parser.cpp"
    break;

  case 450:
#line 2423 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8031 "ds_parser.cpp"
    break;

  case 451:
#line 2433 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8043 "ds_parser.cpp"
    break;

  case 452:
#line 2440 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8054 "ds_parser.cpp"
    break;

  case 453:
#line 2449 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8060 "ds_parser.cpp"
    break;

  case 454:
#line 2449 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8066 "ds_parser.cpp"
    break;

  case 455:
#line 2449 "ds_parser.ypp"
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
#line 8081 "ds_parser.cpp"
    break;

  case 456:
#line 2462 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8087 "ds_parser.cpp"
    break;

  case 457:
#line 2463 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8093 "ds_parser.cpp"
    break;

  case 458:
#line 2464 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8099 "ds_parser.cpp"
    break;

  case 459:
#line 2465 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8105 "ds_parser.cpp"
    break;

  case 460:
#line 2466 "ds_parser.ypp"
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
#line 8124 "ds_parser.cpp"
    break;

  case 461:
#line 2480 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8135 "ds_parser.cpp"
    break;

  case 462:
#line 2486 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8144 "ds_parser.cpp"
    break;

  case 463:
#line 2490 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8153 "ds_parser.cpp"
    break;

  case 464:
#line 2494 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8163 "ds_parser.cpp"
    break;

  case 465:
#line 2499 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8173 "ds_parser.cpp"
    break;

  case 466:
#line 2504 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8183 "ds_parser.cpp"
    break;

  case 467:
#line 2509 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8193 "ds_parser.cpp"
    break;

  case 468:
#line 2514 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8202 "ds_parser.cpp"
    break;

  case 469:
#line 2518 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8211 "ds_parser.cpp"
    break;

  case 470:
#line 2522 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8221 "ds_parser.cpp"
    break;

  case 471:
#line 2527 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8230 "ds_parser.cpp"
    break;

  case 472:
#line 2531 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8240 "ds_parser.cpp"
    break;

  case 473:
#line 2536 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8246 "ds_parser.cpp"
    break;

  case 474:
#line 2536 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8252 "ds_parser.cpp"
    break;

  case 475:
#line 2536 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8263 "ds_parser.cpp"
    break;

  case 476:
#line 2542 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8275 "ds_parser.cpp"
    break;

  case 477:
#line 2549 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8281 "ds_parser.cpp"
    break;

  case 478:
#line 2549 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8287 "ds_parser.cpp"
    break;

  case 479:
#line 2549 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8297 "ds_parser.cpp"
    break;

  case 480:
#line 2554 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8303 "ds_parser.cpp"
    break;

  case 481:
#line 2554 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8309 "ds_parser.cpp"
    break;

  case 482:
#line 2554 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8320 "ds_parser.cpp"
    break;

  case 483:
#line 2560 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8326 "ds_parser.cpp"
    break;

  case 484:
#line 2560 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8332 "ds_parser.cpp"
    break;

  case 485:
#line 2560 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8342 "ds_parser.cpp"
    break;

  case 486:
#line 2565 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8351 "ds_parser.cpp"
    break;

  case 487:
#line 2569 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8357 "ds_parser.cpp"
    break;

  case 488:
#line 2569 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8363 "ds_parser.cpp"
    break;

  case 489:
#line 2569 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8373 "ds_parser.cpp"
    break;

  case 490:
#line 2574 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8379 "ds_parser.cpp"
    break;

  case 491:
#line 2574 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8385 "ds_parser.cpp"
    break;

  case 492:
#line 2574 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8399 "ds_parser.cpp"
    break;

  case 493:
#line 2583 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8408 "ds_parser.cpp"
    break;

  case 494:
#line 2587 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8414 "ds_parser.cpp"
    break;

  case 495:
#line 2587 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8420 "ds_parser.cpp"
    break;

  case 496:
#line 2587 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8430 "ds_parser.cpp"
    break;

  case 497:
#line 2592 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8436 "ds_parser.cpp"
    break;

  case 498:
#line 2592 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8442 "ds_parser.cpp"
    break;

  case 499:
#line 2592 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8456 "ds_parser.cpp"
    break;

  case 500:
#line 2601 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8465 "ds_parser.cpp"
    break;

  case 501:
#line 2605 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8471 "ds_parser.cpp"
    break;

  case 502:
#line 2605 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8477 "ds_parser.cpp"
    break;

  case 503:
#line 2605 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8487 "ds_parser.cpp"
    break;

  case 504:
#line 2610 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8493 "ds_parser.cpp"
    break;

  case 505:
#line 2610 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8499 "ds_parser.cpp"
    break;

  case 506:
#line 2610 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8513 "ds_parser.cpp"
    break;

  case 507:
#line 2619 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8519 "ds_parser.cpp"
    break;

  case 508:
#line 2619 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8525 "ds_parser.cpp"
    break;

  case 509:
#line 2619 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8536 "ds_parser.cpp"
    break;

  case 510:
#line 2625 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8542 "ds_parser.cpp"
    break;

  case 511:
#line 2625 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8548 "ds_parser.cpp"
    break;

  case 512:
#line 2625 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8559 "ds_parser.cpp"
    break;

  case 513:
#line 2634 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8565 "ds_parser.cpp"
    break;

  case 514:
#line 2634 "ds_parser.ypp"
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
#line 8582 "ds_parser.cpp"
    break;

  case 515:
#line 2649 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8588 "ds_parser.cpp"
    break;

  case 516:
#line 2649 "ds_parser.ypp"
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
#line 8609 "ds_parser.cpp"
    break;

  case 517:
#line 2669 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8615 "ds_parser.cpp"
    break;

  case 518:
#line 2670 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8621 "ds_parser.cpp"
    break;

  case 519:
#line 2671 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8627 "ds_parser.cpp"
    break;

  case 520:
#line 2672 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8633 "ds_parser.cpp"
    break;

  case 521:
#line 2676 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8645 "ds_parser.cpp"
    break;

  case 522:
#line 2683 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8657 "ds_parser.cpp"
    break;

  case 523:
#line 2690 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8668 "ds_parser.cpp"
    break;

  case 524:
#line 2696 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8679 "ds_parser.cpp"
    break;

  case 525:
#line 2705 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8689 "ds_parser.cpp"
    break;

  case 526:
#line 2710 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8698 "ds_parser.cpp"
    break;

  case 527:
#line 2717 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8704 "ds_parser.cpp"
    break;

  case 528:
#line 2718 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8710 "ds_parser.cpp"
    break;

  case 529:
#line 2722 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8721 "ds_parser.cpp"
    break;

  case 530:
#line 2728 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8733 "ds_parser.cpp"
    break;

  case 531:
#line 2735 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8746 "ds_parser.cpp"
    break;

  case 532:
#line 2743 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8758 "ds_parser.cpp"
    break;

  case 533:
#line 2750 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8771 "ds_parser.cpp"
    break;

  case 534:
#line 2758 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8785 "ds_parser.cpp"
    break;

  case 535:
#line 2770 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8793 "ds_parser.cpp"
    break;

  case 536:
#line 2773 "ds_parser.ypp"
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
#line 8809 "ds_parser.cpp"
    break;

  case 537:
#line 2787 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8820 "ds_parser.cpp"
    break;

  case 538:
#line 2796 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8826 "ds_parser.cpp"
    break;

  case 539:
#line 2797 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8832 "ds_parser.cpp"
    break;

  case 540:
#line 2801 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8842 "ds_parser.cpp"
    break;

  case 541:
#line 2806 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8851 "ds_parser.cpp"
    break;

  case 542:
#line 2813 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8861 "ds_parser.cpp"
    break;

  case 543:
#line 2818 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8873 "ds_parser.cpp"
    break;

  case 544:
#line 2828 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8883 "ds_parser.cpp"
    break;

  case 545:
#line 2833 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8892 "ds_parser.cpp"
    break;

  case 546:
#line 2840 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8906 "ds_parser.cpp"
    break;

  case 547:
#line 2852 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8912 "ds_parser.cpp"
    break;

  case 548:
#line 2853 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8918 "ds_parser.cpp"
    break;

  case 549:
#line 2857 "ds_parser.ypp"
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
#line 8941 "ds_parser.cpp"
    break;

  case 550:
#line 2875 "ds_parser.ypp"
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
#line 8964 "ds_parser.cpp"
    break;


#line 8968 "ds_parser.cpp"

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
#line 2895 "ds_parser.ypp"


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
