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
#define YYLAST   7381

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  179
/* YYNRULES -- Number of rules.  */
#define YYNRULES  551
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  963

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
     801,   806,   814,   815,   816,   820,   821,   822,   823,   824,
     825,   826,   827,   833,   847,   864,   865,   866,   870,   878,
     891,   895,   902,   903,   907,   908,   909,   913,   916,   923,
     927,   928,   929,   930,   931,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,   948,   949,   950,   951,   952,   953,   954,   955,   956,
     957,   958,   959,   960,   961,   962,   963,   964,   965,   966,
     967,   968,   972,   994,   995,   996,  1000,  1039,  1046,  1050,
    1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1083,
    1088,  1094,  1100,  1134,  1137,  1143,  1144,  1155,  1159,  1165,
    1168,  1171,  1175,  1181,  1185,  1189,  1192,  1195,  1200,  1203,
    1211,  1214,  1219,  1222,  1230,  1236,  1237,  1241,  1275,  1275,
    1275,  1278,  1278,  1278,  1283,  1283,  1283,  1291,  1291,  1291,
    1297,  1307,  1318,  1333,  1336,  1342,  1343,  1350,  1361,  1362,
    1363,  1367,  1368,  1369,  1370,  1374,  1379,  1387,  1388,  1392,
    1397,  1461,  1462,  1463,  1464,  1465,  1466,  1470,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,
    1483,  1484,  1485,  1486,  1487,  1488,  1492,  1493,  1494,  1495,
    1499,  1510,  1515,  1525,  1529,  1529,  1529,  1536,  1536,  1536,
    1550,  1554,  1554,  1554,  1561,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,
    1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,
    1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,  1596,
    1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1608,  1612,
    1615,  1618,  1619,  1620,  1621,  1630,  1639,  1640,  1643,  1643,
    1643,  1646,  1650,  1654,  1658,  1659,  1660,  1661,  1662,  1663,
    1664,  1665,  1681,  1697,  1755,  1756,  1760,  1761,  1765,  1773,
    1776,  1781,  1780,  1825,  1881,  1890,  1895,  1903,  1904,  1908,
    1911,  1920,  1921,  1925,  1934,  1935,  1940,  1941,  1945,  1951,
    1957,  1960,  1964,  1970,  1979,  1980,  1981,  1985,  1986,  1990,
    1993,  1998,  2003,  2011,  2022,  2025,  2033,  2034,  2038,  2039,
    2040,  2044,  2072,  2072,  2106,  2109,  2118,  2131,  2143,  2144,
    2148,  2152,  2153,  2154,  2158,  2186,  2218,  2219,  2223,  2260,
    2261,  2265,  2266,  2267,  2272,  2271,  2356,  2363,  2372,  2373,
    2374,  2375,  2376,  2377,  2378,  2379,  2380,  2381,  2382,  2383,
    2384,  2385,  2386,  2387,  2388,  2389,  2390,  2391,  2392,  2393,
    2394,  2395,  2396,  2400,  2401,  2402,  2403,  2404,  2405,  2409,
    2420,  2424,  2434,  2441,  2450,  2450,  2450,  2463,  2464,  2465,
    2466,  2467,  2481,  2487,  2491,  2495,  2500,  2505,  2510,  2515,
    2519,  2523,  2528,  2532,  2537,  2537,  2537,  2543,  2550,  2550,
    2550,  2555,  2555,  2555,  2561,  2561,  2561,  2566,  2570,  2570,
    2570,  2575,  2575,  2575,  2584,  2588,  2588,  2588,  2593,  2593,
    2593,  2602,  2606,  2606,  2606,  2611,  2611,  2611,  2620,  2620,
    2620,  2626,  2626,  2626,  2635,  2635,  2650,  2650,  2670,  2671,
    2672,  2673,  2677,  2684,  2691,  2697,  2706,  2711,  2718,  2719,
    2723,  2729,  2736,  2744,  2751,  2759,  2771,  2774,  2788,  2797,
    2798,  2802,  2807,  2814,  2819,  2829,  2834,  2841,  2853,  2854,
    2858,  2876
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

#define YYPACT_NINF -592

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-592)))

#define YYTABLE_NINF -506

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -592,    30,  -592,  -592,    16,   -69,   109,   -99,  -592,   -33,
    -592,  -592,    12,  -592,  -592,  -592,  -592,  -592,   213,  -592,
     152,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
     -18,  -592,    -2,    58,    66,  -592,  -592,   109,    11,  -592,
     147,  -592,  -592,   152,   151,   176,  -592,  -592,   192,    97,
      55,  -592,    78,  -592,  -592,  -592,    84,   162,   269,  -592,
     276,    23,    16,   194,   -69,   164,   218,  -592,   223,   236,
    -592,   235,  -592,   -92,   279,   207,   212,  -592,   256,    16,
      12,  -592,  -592,  -592,     5,   219,  -592,  -592,  -592,   259,
    -592,  -592,   260,  -592,  -592,   222,  -592,  -592,  -592,  -592,
    -592,   239,    51,  -592,  -592,  -592,  -592,   349,  -592,  -592,
    7124,  -592,  -592,  -592,  -592,  -592,   262,   263,  -592,   -24,
    -592,  7211,   382,   383,  -592,   240,  -592,  -592,    52,   265,
    -592,  -592,   286,  -592,  -592,   228,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,    80,  -592,   268,   272,   277,   280,  -592,
    -592,  -592,   244,  -592,  -592,  -592,  -592,  -592,   281,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,   282,  -592,
    -592,  -592,   284,   289,  -592,  -592,  -592,  -592,   290,   291,
    -592,  -592,  -592,  -592,  -592,   548,  -592,   246,   297,  -592,
     273,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,   -74,   304,  1085,   257,  -592,   -62,
    -592,   283,   -85,    16,   312,  -592,  -592,  -592,    51,  -592,
    -592,  -592,  -592,  -592,   316,  -592,   155,   163,   169,  -592,
    -592,  -592,  -592,  -592,  -592,   431,  -592,  -592,    18,  2773,
    -592,  -592,   -58,  7124,   -54,  -592,    -6,  -592,   -28,  7124,
    -592,  -592,  4410,  -592,  -592,   312,  -592,  -592,  7038,   287,
     305,  1901,  -592,  -592,   219,  4410,   292,  4410,   308,   309,
     294,  -592,   293,   310,   335,  2912,   219,  -592,   315,  2060,
    4410,  4410,   188,   188,  6866,  6952,  4410,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  4410,  4410,  4410,   -62,  4410,  4410,
    4410,  -592,   295,  -592,   442,  -592,  -592,   296,   302,  4410,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,   -71,   303,
    -592,   306,   307,  -592,  -592,  -592,   312,  -592,  -592,  -592,
    -592,   311,  -592,  -592,   -76,  -592,  -592,  -592,  -592,  -592,
    6136,   318,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,   314,   -51,  -592,  -592,  -592,   312,    79,
    -592,  -592,   -75,  -592,  7124,  7124,  7124,  7124,   300,   262,
    7124,   240,  7124,   240,  7124,   240,  7210,   263,  -592,  -592,
    -592,   321,  -592,  -592,  5796,  -592,  -592,  3894,  -592,  -592,
     312,  -592,   -57,  -592,   286,  3894,   824,    14,   562,  -592,
     -62,  -592,  2199,  -592,  -592,   368,  4500,   489,  6430,  4410,
     824,  -592,  -592,  4410,  -592,  -592,   352,   366,  6430,  -592,
    7124,  2199,  -592,  4599,   858,   858,   327,  -592,  -592,   312,
    1430,   312,  1587,  6204,  -592,    81,   134,   858,   858,   -44,
    -592,   858,   858,  5244,    93,   326,  -592,  -592,   824,  -592,
    2338,  -592,  -592,  -592,  -592,   375,   188,  -592,    36,   367,
    4410,  4410,  4410,  4410,  4410,  4410,  4410,  4410,  4410,  4410,
    -592,  -592,  4410,  4410,  4410,  4410,  4410,  4410,   369,  2495,
    4410,   372,  4410,  4410,  4410,  4410,  4410,  4410,  4410,  4410,
    4410,  4410,  4410,  4410,  4410,  4410,  2634,  3051,  4410,  4410,
    4410,  4410,  4410,  4410,  4410,  4410,  4410,  4410,   373,  4410,
    3190,  -592,    17,  -592,    24,  -592,   341,   376,  7124,  -592,
     -35,  1370,  1681,  1842,  2000,  -592,   111,  2136,   304,  2275,
     304,  2432,   304,   106,  -592,   116,  3894,   175,  -592,  -592,
    -592,  -592,  -592,  7124,  -592,  2199,  -592,  -592,  4410,  3347,
    3504,  7124,  -592,  4698,  4410,  4410,  -592,   219,  5322,  -592,
    7124,  7124,  5402,  7124,  -592,  -592,  2571,  -592,  4797,  -592,
    -592,    22,   188,   -37,  1744,  3643,  6204,   360,    -5,   342,
     371,  -592,  -592,    85,    27,  3800,    -5,   195,  4410,  4410,
     344,  -592,  4410,   250,   386,  -592,   170,  -592,  -592,   131,
     385,  -592,    91,  6430,   -19,   240,  -592,   374,  -592,  -592,
    6430,  6430,  6430,  6430,  6430,  6430,  6430,  6430,   410,   410,
     330,  6430,  6430,   330,   249,   249,   356,  4410,  4410,  6430,
     124,  -592,  5872,   -61,   -61,  6430,   410,   410,  6430,  6430,
    6529,   532,  6498,  6430,  6430,  6430,  4410,  4410,  6430,   394,
    6283,   749,  6597,  6744,   330,   330,     8,     8,   124,   124,
     124,  -592,   539,  5948,  -592,    94,   -36,  4410,  -592,    12,
    -592,   534,   500,   361,  -592,  -592,   837,  -592,  -592,  -592,
    2199,  -592,  7124,  -592,  -592,  -592,   398,  -592,   384,  -592,
     387,  -592,   388,  7124,  -592,  7210,  -592,   263,  3288,  -592,
    6036,   -78,  -592,    98,   401,  5480,  2710,  -592,  -592,  -592,
    2849,  2988,  -592,  3127,   375,  -592,  4410,  -592,  4410,  4410,
      -7,   402,   385,   377,   378,  4410,   379,  4410,  4410,   385,
     365,   380,  6430,  -592,  -592,  6351,  -592,   404,  7124,   240,
    1270,   219,  -592,  -592,  4410,  -592,   -34,   102,  4410,  -592,
     409,   414,   415,   417,  -592,   112,   304,  -592,  3957,  6676,
    6430,  -592,  6676,  6430,  -592,  4410,  -592,  -592,  -592,  -592,
    5094,   120,   357,  -592,   312,  -592,   431,  -592,  2199,  -592,
    4896,  -592,  3441,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  3894,  -592,  -592,  -592,   431,  4410,   219,  -592,
     178,  -592,  -592,  -592,  -592,  -592,   389,   -25,  6430,  6430,
      -5,   393,    89,   360,   399,  -592,  6430,  -592,  -592,    10,
      -5,   406,  -592,  -592,  -592,  3580,   304,  -592,  -592,   824,
     405,  6430,  -592,  -592,  -592,  -592,   -19,   411,   -81,  7124,
    -592,    99,  6430,  -592,  -592,   381,  -592,    84,  -592,  -592,
    4995,  -592,  -592,  -592,  -592,  -592,  6430,  -592,  4410,   418,
    -592,  4410,  4410,  4410,  4114,  4410,   412,   413,  4410,  4410,
    -592,  4410,   397,  -592,  -592,   427,   131,  -592,  -592,  -592,
    4271,  -592,  -592,  3737,  -592,  -592,    84,  -592,  -592,  -592,
    5560,   432,   330,   330,   330,  -592,  5638,  5169,   419,  -592,
    6430,  6430,  5169,   421,   -62,  -592,  -592,  4410,  6430,  -592,
     424,  -592,  4410,  -592,   516,   422,  -592,   416,  -592,  -592,
     -62,  6430,  -592,  -592,  5718,  4410,   430,   435,  -592,  -592,
    6430,  -592,  -592
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   195,     0,     0,     0,     0,   196,     0,
     516,   514,     0,    14,     3,    10,     9,     8,     0,     7,
     386,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,    19,    18,   386,     0,     0,    86,    87,     0,   175,
      88,    90,     0,    85,   410,   409,   143,   401,   411,   387,
     388,     0,     0,     0,     0,    37,     0,    43,     0,     0,
      40,     0,   398,     0,    15,     0,     0,   177,     0,     0,
       0,    93,   145,   144,     0,     0,   142,   403,   402,     0,
     413,   412,     0,   390,   389,   392,    79,    80,    77,    78,
      76,     0,     0,    75,    84,    48,    46,    42,    39,    38,
       0,   400,   399,    16,    17,    20,     0,     0,   176,     0,
      91,     0,     0,     0,    99,    94,   169,   147,     0,   406,
     414,   384,   344,    21,    22,     0,    68,    69,    66,    67,
      65,    64,    70,     0,    41,     0,     0,     0,     0,   418,
     438,   419,   450,   420,   424,   425,   426,   427,   442,   431,
     432,   433,   434,   435,   436,   437,   439,   440,   487,   423,
     430,   441,   494,   501,   421,   428,   422,   429,     0,     0,
     449,   457,   460,   458,   459,     0,   452,     0,     0,   364,
       0,    89,   102,   103,   105,   104,   106,   107,   108,   109,
     134,   135,   132,   133,   125,   136,   137,   126,   123,   124,
     138,   139,   140,   141,   113,   114,   115,   110,   111,   112,
     128,   129,   127,   121,   122,   117,   116,   118,   119,   120,
     101,   100,   130,   131,   344,    97,     0,     0,   394,     0,
     408,     0,   344,     0,     0,    23,    24,    25,     0,    82,
     478,   481,   484,   474,     0,   454,   488,   495,   502,   508,
     511,   465,   470,   464,   477,     0,   473,   467,     0,     0,
     397,   469,     0,     0,     0,    95,     0,   357,     0,     0,
     146,   171,     0,    57,    58,     0,   280,   281,     0,     0,
       0,     0,   274,   183,     0,     0,     0,     0,     0,     0,
       0,   184,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   231,   233,   232,
     234,   235,   236,    26,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   150,   148,   278,   277,     0,     0,     0,
     164,   159,   157,   156,   158,   229,   170,   151,   275,     0,
     337,     0,     0,   162,   163,   165,     0,   155,   336,   335,
     334,    92,   340,   276,     0,   152,   339,   338,   316,   282,
     237,     0,   279,   518,   519,   520,   521,   443,   446,   444,
     447,   445,   448,     0,     0,   407,   349,   391,     0,     0,
     416,   393,   377,    71,     0,     0,     0,     0,     0,     0,
       0,    94,     0,    94,     0,    94,     0,     0,   472,   466,
     468,     0,   471,   462,     0,   453,   517,   363,   515,   365,
       0,   355,   368,    96,   344,    98,     0,     0,   179,   182,
       0,   207,     0,   185,   188,     0,   237,     0,   178,     0,
       0,   198,   201,     0,   168,   204,     0,     0,    51,    61,
       0,     0,   192,   237,   308,   309,   219,   173,   174,     0,
     528,     0,     0,     0,   545,     0,     0,   286,   285,   321,
      32,   284,   283,     0,   220,     0,   166,   167,     0,   343,
       0,   154,   160,   161,   197,   227,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     310,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       0,   394,     0,   404,   344,   385,     0,     0,     0,   378,
       0,     0,     0,     0,     0,   451,     0,     0,    97,     0,
      97,     0,    97,   175,   361,     0,   359,     0,   463,   461,
     356,   367,   366,     0,   369,     0,   358,    62,     0,     0,
       0,     0,   189,   237,     0,     0,   186,     0,     0,    63,
       0,     0,     0,     0,    49,    50,     0,   193,   237,   190,
     220,     0,     0,   175,     0,     0,   536,   526,   528,     0,
     539,   540,   541,     0,     0,     0,   528,     0,     0,     0,
       0,    29,     0,    27,     0,   312,   264,   263,   169,    54,
       0,   317,     0,   213,     0,    94,   172,     0,   331,   332,
     247,   248,   250,   249,   251,   241,   242,   243,   287,   288,
     300,   252,   253,   301,   298,   299,     0,     0,     0,   239,
     326,   315,     0,   341,   342,   240,   289,   290,   254,   255,
     305,   306,   307,   244,   245,   246,     0,     0,   238,     0,
       0,   303,   304,   302,   296,   297,   292,   291,   293,   294,
     295,   270,     0,     0,   319,     0,     0,     0,   395,     0,
     415,     0,   346,     0,   345,   417,     0,   375,   376,   374,
       0,   479,     0,   485,   475,   455,     0,   489,     0,   496,
       0,   503,     0,     0,   509,     0,   512,     0,   370,   373,
     372,     0,   180,     0,     0,     0,     0,   187,   194,   322,
       0,     0,   323,     0,   227,   191,     0,   529,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   546,   547,     0,    30,    33,     0,    94,
       0,     0,    52,    53,     0,    59,     0,     0,     0,   318,
       0,     0,     0,     0,   225,     0,    97,   328,     0,   258,
     259,   314,   256,   257,   333,     0,   272,   313,   320,   405,
       0,     0,   346,   347,     0,   350,   374,   379,     0,   383,
     237,   480,     0,   486,   476,   456,   490,   492,   497,   499,
     504,   506,   360,   510,   362,   513,   366,     0,     0,   181,
       0,   210,   208,   199,   202,   205,     0,     0,   523,   522,
     528,     0,     0,   527,     0,   531,   537,   543,   542,     0,
     528,     0,   544,    28,    31,     0,    97,   149,    55,     0,
       0,   214,   223,   224,   222,   221,     0,     0,     0,     0,
     261,     0,   327,   273,   396,    93,   351,   143,   348,   381,
     237,   382,   482,   493,   500,   507,   371,    60,     0,     0,
     209,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,     0,     0,   534,   265,     0,    54,   260,   226,   228,
       0,   215,   230,     0,   262,   354,   143,   353,   380,   483,
       0,     0,   200,   203,   206,   324,     0,   548,     0,   532,
     525,   524,   548,     0,     0,   268,    56,     0,   216,   329,
       0,   211,     0,   325,     0,     0,   533,     0,   535,   266,
       0,   217,   330,   352,     0,     0,     0,     0,   269,   212,
     549,   550,   551
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -592,  -592,  -592,  -592,  -592,   100,   506,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,   558,  -592,   495,  -592,  -592,   545,
    -592,  -592,  -592,  -291,  -592,  -592,  -592,  -592,  -592,  -592,
     403,  -592,  -592,   559,   -52,  -592,   540,   -67,  -345,  -390,
    -536,  -592,  -592,  -592,  -293,  -253,   -83,  -592,     0,  -286,
     -12,  -592,  -592,  -592,  -592,  -592,  -592,  -592,   633,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -530,  -592,  -592,  -225,  -592,   -98,  -294,  -592,  -232,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,   301,  -592,  -592,  -124,  -150,  -592,  -592,  -592,   229,
    -592,   -70,  -592,  -268,   251,  -415,  -411,   -46,  -592,  -298,
    -592,   618,  -592,  -592,  -592,   125,   592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -273,  -109,  -592,
    -592,  -592,   270,  -592,  -592,  -592,   -90,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,  -592,
    -592,  -592,  -592,  -592,  -592,   391,  -570,  -441,  -591,  -592,
    -592,  -302,   -89,   205,  -592,  -592,  -592,  -262,  -592
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   115,    43,    14,   135,   141,   466,   335,   767,
     336,   624,    15,    16,    38,    39,    70,    17,    34,    35,
     337,   338,   774,   775,   339,   340,   341,   342,   343,   344,
     142,   143,    30,    31,    32,    50,    51,    52,    18,   235,
     280,   125,    19,    84,    85,    86,   345,   346,   236,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   590,   891,   591,   892,   593,   893,   359,   581,   890,
     360,   632,   912,   361,   784,   785,   635,   362,   363,   435,
     365,   366,   367,   368,   768,   934,   769,   950,   369,   692,
     873,   633,   869,   952,   276,   804,   703,   544,   916,   277,
     278,   564,   565,   189,   190,   749,   421,   710,   550,   391,
     242,    60,    95,    21,   132,   384,    72,    73,    22,    89,
      23,   240,   130,    58,    92,    24,   241,   392,   371,   383,
     182,   183,   187,   184,   399,   815,   566,   397,   814,   394,
     811,   395,   919,   396,   813,   400,   816,   401,   883,   402,
     818,   403,   884,   404,   820,   405,   885,   406,   823,   407,
     825,    25,    45,    26,    44,   372,   607,   608,   609,   373,
     610,   611,   612,   613,   374,   465,   375,   945,   376
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,   181,   127,   422,   364,   434,   419,   575,   244,   570,
     695,   558,   427,   560,   464,   562,   485,   753,   457,   458,
     185,   616,   718,   452,   720,   760,   722,   119,   488,   489,
       2,   -92,    66,   578,    27,    28,     3,    96,    97,   121,
     420,   746,   602,   486,   602,    46,   758,   910,   731,   733,
     409,   243,   508,    71,   637,   511,   512,   571,   484,     4,
     777,     5,   243,     6,    67,   136,   137,    48,    53,     7,
      33,    47,   511,   512,   479,   778,     8,   571,   547,   707,
     571,   548,     9,    49,   549,    40,   748,   415,   708,   748,
     545,   188,    78,   111,   542,   780,   547,   572,   180,   573,
     387,    10,   574,   126,   781,   828,   275,   487,   480,   542,
     122,   123,    41,   500,   501,   538,   539,   572,   388,   709,
     572,   508,    11,   510,   511,   512,   513,   416,   778,    62,
     514,   418,   538,   539,   543,   782,    61,    82,   776,   390,
     783,    48,   771,    83,   488,   489,   582,   422,    42,   799,
     124,    62,   423,   772,   773,   424,   191,    49,   895,   841,
     243,    29,    98,   778,   181,   597,    99,   547,   100,   101,
     181,   697,   535,   536,   537,   547,    68,   410,   752,   181,
     547,   638,   843,   417,   538,   539,   601,    69,   614,   425,
     138,   389,   636,   901,   139,   411,   140,   101,   428,   701,
     698,   699,   102,   571,   412,   181,   181,    12,   237,   700,
      59,   437,   898,    13,    63,    87,   837,    54,    55,    64,
      56,    88,    48,   449,   460,   462,    78,   385,   849,   500,
     501,    80,    62,   248,    79,    78,   238,   508,    49,   510,
     511,   512,   513,   572,   778,   786,   514,   778,    57,   896,
     868,   778,   778,   626,    36,   751,    81,   546,   871,   902,
     249,   180,   723,   756,   619,   866,   620,   180,   757,   488,
     489,   779,   715,    80,   798,    37,   180,   724,   829,   914,
     860,   133,   134,   621,   622,   181,   181,   181,   181,   729,
     867,   181,    71,   181,   716,   181,    75,   181,   875,   725,
     538,   539,   180,   180,   551,   552,   553,   554,   747,   840,
     557,  -491,   559,   827,   561,   470,  -491,   763,   850,  -498,
     905,    76,    90,   377,  -498,  -505,  -267,   378,    91,    93,
    -505,  -267,   113,   105,  -491,    94,   726,    77,   114,   888,
      69,   181,  -498,   577,   379,   380,   381,   382,  -505,  -267,
     488,   489,   498,   499,   500,   501,   502,   589,   727,   505,
     596,   889,   508,   107,   510,   511,   512,   513,   108,   331,
     456,   514,   126,   516,   517,   245,   246,   247,   757,   856,
     761,   109,   180,   180,   180,   180,   133,   134,   180,   110,
     180,   116,   180,   878,   180,   629,   117,   245,   246,   803,
     876,   118,    67,   126,   128,   129,   131,   186,   188,   531,
     532,   533,   534,   535,   536,   537,   232,   233,   580,   234,
     702,   239,   243,   254,   809,   538,   539,   899,   250,   272,
     488,   489,   251,   498,   499,   500,   501,   252,   180,   181,
     253,   255,   256,   508,   257,   510,   511,   512,   513,   258,
     259,   260,   514,   273,   516,   517,   274,   390,   706,   419,
     279,   398,   627,   408,   181,   431,   430,   386,   441,   442,
     445,   439,   181,   443,   446,   450,   444,   474,   475,   476,
     555,   181,   181,   728,   181,   477,   481,   486,    12,   482,
     483,   736,   533,   534,   535,   536,   537,   540,   541,   568,
     740,   741,   587,   743,   738,   595,   538,   539,   594,   600,
     628,   634,   639,   751,   656,   500,   501,   661,   691,   704,
     754,   705,   879,   508,   755,   510,   511,   512,   513,   764,
     776,   422,   514,   766,   787,   788,   180,   370,   364,   794,
     796,   802,   803,   415,   805,   817,   830,   842,   819,   821,
     851,   854,   488,   489,   862,   844,   845,   847,   852,   863,
     864,   180,   865,   921,   915,   955,   623,   103,   894,   180,
     414,   897,   533,   534,   535,   536,   537,   900,   180,   180,
     261,   180,   933,   426,   903,   907,   538,   539,   935,   909,
     928,   929,   436,   942,   261,    65,   438,   946,   440,   948,
     956,   957,   144,   181,   262,   263,   448,   953,   961,   106,
     453,   454,   455,   962,   181,   936,   181,   463,   262,   263,
     120,   104,   812,   940,   917,   467,   468,   469,   770,   471,
     472,   473,   801,   822,    20,   498,   499,   500,   501,   502,
     478,   908,   505,   506,   507,   508,   836,   510,   511,   512,
     513,   393,   877,   576,   514,   824,   516,   517,   567,   181,
     808,    74,   520,   264,   522,   112,   696,   617,   848,   556,
     947,     0,     0,     0,     0,     0,     0,   264,   855,   429,
       0,     0,     0,     0,     0,     0,     0,    53,   858,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
     180,     0,   265,   266,     0,     0,     0,   267,   538,   539,
     268,   180,     0,   180,     0,     0,   265,   266,     0,     0,
       0,   267,     0,     0,   268,   269,     0,     0,     0,     0,
       0,   270,     0,   583,   271,     0,     0,     0,     0,   269,
     588,   579,     0,     0,   592,   887,     0,     0,   271,     0,
       0,     0,   598,     0,     0,     0,   180,     0,     0,     0,
     181,   606,     0,   606,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,   906,     0,     0,   913,
       0,     0,     0,     0,     0,   911,     0,     0,     0,     0,
       0,   640,   641,   642,   643,   644,   645,   646,   647,   648,
     649,     0,     0,   650,   651,   652,   653,   654,   655,     0,
     659,   660,     0,   662,   663,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   678,   680,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,     0,
     693,     0,     0,     0,   488,   489,     0,     0,     0,     0,
       0,     0,   498,   499,   500,   501,   502,   180,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,   261,
       0,   514,     0,   516,   517,     0,   730,     0,   488,   489,
       0,   735,     0,     0,     0,   659,   678,     0,     0,     0,
       0,     0,     0,   262,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,   473,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   473,     0,     0,   762,
     463,     0,   949,   765,     0,   538,   539,   498,   499,   500,
     501,   502,     0,     0,   505,   506,   507,   508,   958,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
       0,   707,   264,     0,   520,   521,   522,     0,   789,   790,
     708,     0,     0,   500,   501,     0,     0,     0,     0,     0,
       0,   508,     0,     0,   511,   512,   513,   792,   793,   527,
     514,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   806,   266,     0,     0,     0,   267,     0,   800,   268,
     538,   539,     0,     0,     0,     0,     0,     0,   126,     0,
       0,   810,     0,     0,   269,     0,     0,     0,     0,     0,
     807,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   838,
     839,     0,     0,     0,     0,     0,   846,     0,   606,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,   859,     0,     0,     0,   861,
       0,     0,     0,     0,     0,     0,   281,     0,     0,     0,
       0,     3,     0,   282,   283,   284,   872,   285,     0,   286,
     287,   288,   289,   290,     0,     0,     0,     0,     0,   880,
     291,   292,   293,   294,     0,     0,     0,     0,     0,     0,
       0,     0,   295,   296,     0,   297,   298,     0,   886,   299,
       0,     8,   300,   301,     0,   302,   303,     0,     0,   304,
     305,     0,     0,     0,     0,     0,   306,   149,   150,   151,
       0,   153,   154,   155,   156,   157,   307,   159,   160,   161,
     162,   163,   164,   165,   166,   167,     0,   169,   170,   171,
       0,     0,   174,   175,   176,   177,     0,     0,   308,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   920,
     310,   311,   922,   923,   924,   926,   927,     0,     0,   930,
     931,     0,   932,     0,     0,   312,   313,     0,     0,     0,
       0,   938,     0,     0,    48,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      49,   323,     0,     0,     0,     0,     0,     0,   951,     0,
       0,     0,     0,   954,     0,     0,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,   960,     0,     0,     0,
       0,     0,     0,     0,   330,     0,   331,   332,   333,   126,
     334,   281,     0,     0,     0,     0,     3,     0,   282,   283,
     284,     0,   285,     0,   286,   287,   288,   289,   290,     0,
       0,     0,     0,     0,     0,   291,   292,   293,   294,     0,
       0,     0,     0,     0,     0,     0,     0,   295,   296,     0,
     297,   298,     0,     0,   299,     0,     8,   300,   301,     0,
     302,   303,     0,     0,   304,   305,     0,     0,     0,     0,
       0,   306,   149,   150,   151,     0,   153,   154,   155,   156,
     157,   307,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,   169,   170,   171,     0,     0,   174,   175,   176,
     177,     0,     0,   308,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,   261,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    49,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,   263,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,     0,     0,     0,   286,   287,   288,   289,   290,   330,
       0,   331,   332,   333,   126,   857,   292,     0,     0,     0,
       0,     0,   261,     0,     0,     0,     0,     0,   296,     0,
       0,   298,     0,     0,   299,     0,     0,   300,     0,   602,
       0,   303,     0,     0,     0,   264,   262,   263,     0,     0,
       0,     0,   149,   150,   151,     0,   153,   154,   155,   156,
     157,   307,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,   169,   170,   171,     0,     0,   174,   175,   176,
     177,     0,     0,   308,   265,   266,     0,     0,     0,   267,
       0,   711,   268,     0,     0,   310,   311,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,   271,     0,     0,    48,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   603,   323,     0,     0,     0,
       0,     0,     0,     0,   265,   266,     0,     0,     0,   267,
       0,     0,   604,   325,   326,     0,   327,     0,     0,   328,
     329,   286,   287,   288,   289,   290,     0,   269,     0,   605,
       0,   331,   332,   292,   126,     0,   271,     0,     0,   261,
       0,     0,     0,     0,     0,   296,     0,     0,   298,     0,
       0,   299,     0,     0,   300,     0,     0,     0,   303,     0,
       0,     0,     0,   262,   263,     0,     0,     0,     0,   149,
     150,   151,     0,   153,   154,   155,   156,   157,   307,   159,
     160,   161,   162,   163,   164,   165,   166,   167,     0,   169,
     170,   171,     0,     0,   174,   175,   176,   177,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,    48,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   603,   323,     0,     0,     0,   262,   263,     0,
       0,   265,   266,     0,     0,     0,   267,     0,     0,   604,
     325,   326,     0,   327,     0,     0,   328,   329,   286,   287,
     288,   289,   290,     0,   269,     0,   615,     0,   331,   332,
     292,   126,     0,   271,     0,     0,   409,     0,     0,     0,
       0,     0,   296,     0,     0,   298,     0,     0,   299,     0,
       0,   300,     0,     0,     0,   303,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,   150,   151,     0,
     153,   154,   155,   156,   157,   307,   159,   160,   161,   162,
     163,   164,   165,   166,   167,     0,   169,   170,   171,     0,
       0,   174,   175,   176,   177,   265,   266,   308,     0,     0,
     267,     0,     0,   268,     0,     0,     0,     0,     0,   310,
     311,     0,     0,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,   712,     0,     0,   271,     0,     0,
       0,     0,     0,    48,   261,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    49,
     323,     0,     0,     0,     0,     0,     0,     0,   262,   263,
       0,     0,     0,   410,     0,     0,   324,   325,   326,     0,
     327,     0,     0,   328,   329,   286,   287,   288,   289,   290,
       0,   411,     0,   330,     0,   331,   332,   292,   126,     0,
     412,     0,     0,     0,     0,     0,     0,     0,     0,   296,
       0,     0,   298,     0,     0,   299,     0,     0,   300,     0,
       0,     0,   303,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,   149,   150,   151,     0,   153,   154,   155,
     156,   157,   307,   159,   160,   161,   162,   163,   164,   165,
     166,   167,     0,   169,   170,   171,     0,     0,   174,   175,
     176,   177,     0,     0,   308,     0,   265,   266,     0,     0,
       0,   267,     0,   713,   268,     0,   310,   311,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,   269,
       0,   312,   313,     0,     0,     0,     0,     0,   271,     0,
      48,     0,   261,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    49,   323,     0,     0,
       0,     0,     0,     0,     0,     0,   262,   263,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,     0,     0,   286,   287,   288,   289,   290,     0,
     330,     0,   331,   332,   433,   126,   292,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,     0,
       0,   298,     0,     0,   299,     0,     0,   300,     0,     0,
       0,   303,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,   149,   150,   151,     0,   153,   154,   155,   156,
     157,   307,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,   169,   170,   171,     0,     0,   174,   175,   176,
     177,     0,     0,   308,   265,   266,     0,     0,     0,   267,
       0,   714,   268,     0,     0,   310,   311,     0,   261,     0,
       0,     0,     0,     0,   451,     0,     0,   269,     0,     0,
     312,   313,     0,     0,     0,     0,   271,     0,     0,    48,
       0,     0,   262,   263,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    49,   323,     0,     0,     0,
       0,     0,     0,   286,   287,   288,   289,   290,     0,     0,
       0,     0,   324,   325,   326,   292,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,   296,     0,   330,
     298,   331,   332,   299,   126,     0,   300,     0,     0,     0,
     303,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   149,   150,   151,     0,   153,   154,   155,   156,   157,
     307,   159,   160,   161,   162,   163,   164,   165,   166,   167,
       0,   169,   170,   171,     0,     0,   174,   175,   176,   177,
     265,   266,   308,     0,     0,   267,     0,   717,   268,     0,
       0,     0,     0,     0,   310,   311,     0,   261,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,   312,
     313,     0,   271,     0,     0,     0,     0,     0,    48,     0,
       0,   262,   263,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    49,   323,     0,     0,     0,     0,
       0,     0,   286,   287,   288,   289,   290,     0,     0,     0,
       0,   324,   325,   326,   292,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   296,     0,   330,   298,
     331,   332,   299,   126,     0,   300,     0,     0,     0,   303,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     149,   150,   151,     0,   153,   154,   155,   156,   157,   307,
     159,   160,   161,   162,   163,   164,   165,   166,   167,     0,
     169,   170,   171,     0,     0,   174,   175,   176,   177,   265,
     266,   308,     0,     0,   267,     0,   719,   268,     0,     0,
       0,     0,     0,   310,   311,     0,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,   261,     0,     0,    48,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    49,   323,     0,     0,     0,   262,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   325,   326,     0,   327,     0,     0,   328,   329,   286,
     287,   288,   289,   290,     0,   630,     0,   330,   631,   331,
     332,   292,   126,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,   298,     0,     0,   299,
       0,     0,   300,     0,     0,     0,   303,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   149,   150,   151,
       0,   153,   154,   155,   156,   157,   307,   159,   160,   161,
     162,   163,   164,   165,   166,   167,     0,   169,   170,   171,
       0,     0,   174,   175,   176,   177,   265,   266,   308,     0,
       0,   267,     0,   721,   268,     0,     0,     0,     0,     0,
     310,   311,     0,   261,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,   657,   658,     0,   271,     0,
       0,     0,     0,     0,    48,     0,     0,   262,   263,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      49,   323,     0,     0,     0,     0,     0,     0,   286,   287,
     288,   289,   290,     0,     0,     0,     0,   324,   325,   326,
     292,   327,     0,     0,   328,   329,     0,     0,     0,     0,
       0,     0,   296,     0,   330,   298,   331,   332,   299,   126,
       0,   300,     0,     0,     0,   303,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,   150,   151,     0,
     153,   154,   155,   156,   157,   307,   159,   160,   161,   162,
     163,   164,   165,   166,   167,     0,   169,   170,   171,     0,
       0,   174,   175,   176,   177,   265,   266,   308,     0,     0,
     267,     0,   744,   268,     0,     0,     0,     0,     0,   310,
     311,     0,   261,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,   676,   677,     0,   271,     0,     0,
       0,     0,     0,    48,     0,     0,   262,   263,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    49,
     323,     0,     0,     0,     0,     0,     0,   286,   287,   288,
     289,   290,     0,     0,     0,     0,   324,   325,   326,   292,
     327,     0,     0,   328,   329,     0,     0,     0,     0,     0,
       0,   296,     0,   330,   298,   331,   332,   299,   126,     0,
     300,     0,     0,     0,   303,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   149,   150,   151,     0,   153,
     154,   155,   156,   157,   307,   159,   160,   161,   162,   163,
     164,   165,   166,   167,     0,   169,   170,   171,     0,     0,
     174,   175,   176,   177,   265,   266,   308,     0,     0,   267,
       0,   832,   268,     0,     0,     0,     0,     0,   310,   311,
       0,   261,     0,     0,     0,     0,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,   271,     0,     0,     0,
       0,     0,    48,     0,     0,   262,   263,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    49,   323,
       0,     0,     0,     0,     0,     0,   286,   287,   288,   289,
     290,     0,     0,     0,     0,   324,   325,   326,   292,   327,
       0,     0,   328,   329,     0,     0,     0,     0,     0,     0,
     296,   413,   330,   298,   331,   332,   299,   126,     0,   300,
       0,     0,     0,   303,   264,     0,   447,     0,     0,     0,
       0,     0,     0,     0,   149,   150,   151,     0,   153,   154,
     155,   156,   157,   307,   159,   160,   161,   162,   163,   164,
     165,   166,   167,     0,   169,   170,   171,     0,     0,   174,
     175,   176,   177,   265,   266,   308,     0,     0,   267,     0,
     833,   268,     0,     0,     0,     0,     0,   310,   311,     0,
     261,     0,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,    48,     0,     0,   262,   263,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    49,   323,     0,
       0,     0,     0,     0,     0,   286,   287,   288,   289,   290,
       0,     0,   679,     0,   324,   325,   326,   292,   327,     0,
       0,   328,   329,     0,     0,     0,     0,     0,     0,   296,
       0,   330,   298,   331,   332,   299,   126,     0,   300,     0,
       0,     0,   303,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   149,   150,   151,     0,   153,   154,   155,
     156,   157,   307,   159,   160,   161,   162,   163,   164,   165,
     166,   167,     0,   169,   170,   171,     0,     0,   174,   175,
     176,   177,   265,   266,   308,     0,     0,   267,     0,   834,
     268,     0,     0,     0,     0,     0,   310,   311,     0,   261,
       0,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,   271,     0,     0,     0,     0,     0,
      48,     0,     0,   262,   263,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    49,   323,     0,     0,
       0,     0,     0,     0,   286,   287,   288,   289,   290,     0,
       0,     0,     0,   324,   325,   326,   292,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   296,     0,
     330,   298,   331,   332,   299,   126,     0,   300,     0,     0,
       0,   303,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,   150,   151,     0,   153,   154,   155,   156,
     157,   307,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,   169,   170,   171,     0,     0,   174,   175,   176,
     177,   265,   266,   308,     0,     0,   267,     0,   835,   268,
       0,     0,     0,     0,     0,   310,   311,     0,     0,     0,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,    48,
     261,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    49,   323,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,   286,   287,   288,   289,   290,     0,     0,     0,   330,
     694,   331,   332,   292,   126,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,   298,     0,
       0,   299,     0,     0,   300,     0,     0,     0,   303,     0,
       0,     0,   571,   264,     0,     0,     0,     0,     0,   149,
     150,   151,     0,   153,   154,   155,   156,   157,   307,   159,
     160,   161,   162,   163,   164,   165,   166,   167,     0,   169,
     170,   171,     0,     0,   174,   175,   176,   177,     0,     0,
     308,     0,   826,   266,     0,     0,     0,   267,     0,     0,
     268,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,   261,   271,     0,    48,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    49,   323,     0,     0,     0,   262,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,   286,   287,
     288,   289,   290,     0,     0,     0,   330,   732,   331,   332,
     292,   126,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,     0,   298,     0,     0,   299,     0,
       0,   300,     0,     0,     0,   303,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,   150,   151,     0,
     153,   154,   155,   156,   157,   307,   159,   160,   161,   162,
     163,   164,   165,   166,   167,     0,   169,   170,   171,     0,
       0,   174,   175,   176,   177,   265,   266,   308,     0,     0,
     267,     0,   882,   268,     0,     0,     0,     0,     0,   310,
     311,     0,   261,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,    48,     0,     0,   262,   263,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    49,
     323,     0,     0,     0,     0,     0,     0,   286,   287,   288,
     289,   290,     0,     0,   734,     0,   324,   325,   326,   292,
     327,     0,     0,   328,   329,     0,     0,     0,     0,     0,
       0,   296,     0,   330,   298,   331,   332,   299,   126,     0,
     300,     0,     0,     0,   303,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   149,   150,   151,     0,   153,
     154,   155,   156,   157,   307,   159,   160,   161,   162,   163,
     164,   165,   166,   167,     0,   169,   170,   171,     0,     0,
     174,   175,   176,   177,   265,   266,   308,     0,     0,   267,
       0,   904,   268,     0,     0,     0,     0,     0,   310,   311,
       0,     0,     0,     0,     0,     0,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,   271,     0,     0,   261,
       0,     0,    48,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    49,   323,
       0,     0,     0,   262,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   286,   287,   288,   289,   290,     0,
       0,     0,   330,   750,   331,   332,   292,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,     0,
       0,   298,     0,     0,   299,     0,     0,   300,     0,     0,
       0,   303,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,   150,   151,     0,   153,   154,   155,   156,
     157,   307,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,   169,   170,   171,     0,     0,   174,   175,   176,
     177,   265,   266,   308,     0,     0,   267,     0,   939,   268,
       0,     0,     0,     0,     0,   310,   311,     0,     0,     0,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,   261,     0,     0,    48,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    49,   323,     0,     0,     0,
     262,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,   286,   287,   288,   289,   290,     0,     0,     0,   330,
     759,   331,   332,   292,   126,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,   298,     0,
       0,   299,     0,     0,   300,     0,     0,     0,   303,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
     150,   151,     0,   153,   154,   155,   156,   157,   307,   159,
     160,   161,   162,   163,   164,   165,   166,   167,     0,   169,
     170,   171,     0,     0,   174,   175,   176,   177,   265,   266,
     308,     0,     0,   267,     0,     0,   268,     0,     0,     0,
       0,     0,   310,   311,     0,     0,     0,     0,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,    48,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    49,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,   286,   287,
     288,   289,   290,     0,     0,     0,   330,   870,   331,   332,
     292,   126,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,     0,   298,     0,     0,   299,     0,
       0,   300,     0,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,   150,   151,     0,
     153,   154,   155,   156,   157,   307,   159,   160,   161,   162,
     163,   164,   165,   166,   167,     0,   169,   170,   171,     0,
       0,   174,   175,   176,   177,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    49,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   325,   326,     0,
     327,     0,     0,   328,   329,   286,   287,   288,   289,   290,
       0,     0,     0,   330,   925,   331,   332,   292,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
       0,     0,   298,     0,     0,   299,     0,     0,   300,     0,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   149,   150,   151,     0,   153,   154,   155,
     156,   157,   307,   159,   160,   161,   162,   163,   164,   165,
     166,   167,     0,   169,   170,   171,     0,     0,   174,   175,
     176,   177,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   311,     0,     0,
       0,     0,     0,     0,     0,   937,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    49,   323,     0,     0,
       0,     0,     0,     0,   286,   287,   288,   289,   290,     0,
       0,     0,     0,   324,   325,   326,   292,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   296,     0,
     330,   298,   331,   332,   299,   126,     0,   300,     0,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,   150,   151,     0,   153,   154,   155,   156,
     157,   307,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     0,   169,   170,   171,     0,     0,   174,   175,   176,
     177,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   311,     0,     0,     0,
     488,   489,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    49,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,     0,     0,   330,
       0,   331,   332,     0,   126,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   584,   510,   511,   512,   513,   488,
     489,     0,   514,   515,   516,   517,   518,   519,     0,     0,
     520,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   585,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
       0,     0,     0,   586,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   584,   510,   511,   512,   513,   488,   489,
       0,   514,   515,   516,   517,   518,   519,     0,     0,   520,
     521,   522,   523,   524,   525,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   585,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,     0,
       0,     0,   599,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   584,   510,   511,   512,   513,   488,   489,     0,
     514,   515,   516,   517,   518,   519,     0,     0,   520,   521,
     522,   523,   524,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   585,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,   737,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   584,   510,   511,   512,   513,   488,   489,     0,   514,
     515,   516,   517,   518,   519,     0,     0,   520,   521,   522,
     523,   524,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   585,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
     745,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     584,   510,   511,   512,   513,   488,   489,     0,   514,   515,
     516,   517,   518,   519,     0,     0,   520,   521,   522,   523,
     524,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     585,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,     0,     0,     0,   881,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   584,
     510,   511,   512,   513,   488,   489,     0,   514,   515,   516,
     517,   518,   519,     0,     0,   520,   521,   522,   523,   524,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   585,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,   918,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,     0,   498,   499,   500,
     501,   502,     0,     0,   505,   506,   507,   508,     0,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
       0,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,   488,   489,     0,     0,     0,     0,
     538,   539,   498,   499,   500,   501,   502,   874,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,     0,
       0,   514,     0,   516,   517,     0,     0,     0,     0,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,   488,   489,     0,   538,   539,   498,   499,   500,
     501,   502,   944,     0,   505,   506,   507,   508,     0,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
       0,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,   488,   489,   625,   498,   499,   500,   501,   502,
       0,     0,   505,   506,   507,   508,     0,   510,   511,   512,
     513,     0,     0,     0,   514,     0,   516,   517,     0,     0,
       0,     0,   520,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
     488,   489,   739,     0,     0,   498,   499,   500,   501,   502,
       0,     0,   505,   506,   507,   508,     0,   510,   511,   512,
     513,     0,     0,     0,   514,     0,   516,   517,     0,     0,
       0,     0,   520,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
     488,   489,   742,   498,   499,   500,   501,   502,     0,     0,
     505,   506,   507,   508,     0,   510,   511,   512,   513,     0,
       0,     0,   514,     0,   516,   517,     0,     0,     0,     0,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,   488,   489,
     831,     0,     0,   498,   499,   500,   501,   502,     0,     0,
     505,   506,   507,   508,     0,   510,   511,   512,   513,     0,
       0,     0,   514,     0,   516,   517,     0,     0,     0,     0,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,   488,   489,
     941,   498,   499,   500,   501,   502,     0,     0,   505,   506,
     507,   508,     0,   510,   511,   512,   513,     0,     0,     0,
     514,     0,   516,   517,     0,     0,     0,     0,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,   488,   489,   943,     0,
       0,   498,   499,   500,   501,   502,     0,     0,   505,   506,
     507,   508,     0,   510,   511,   512,   513,     0,     0,     0,
     514,     0,   516,   517,     0,     0,     0,     0,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,   488,   489,   538,   539,     0,     0,   959,   498,
     499,   500,   501,   502,     0,     0,   505,   506,   507,   508,
       0,   510,   511,   512,   513,     0,     0,     0,   514,     0,
     516,   517,     0,     0,     0,     0,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,   488,   489,
       0,     0,   538,   539,   569,   498,   499,   500,   501,   502,
       0,     0,   505,   506,   507,   508,     0,   510,   511,   512,
     513,     0,     0,     0,   514,     0,   516,   517,     0,     0,
       0,     0,   520,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
     791,   498,   499,   500,   501,   502,   488,   489,   505,   506,
     507,   508,     0,   510,   511,   512,   513,     0,     0,     0,
     514,     0,   516,   517,     0,     0,     0,     0,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,   797,     0,     0,     0,
       0,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     584,   510,   511,   512,   513,  -237,   488,   489,   514,   515,
     516,   517,   518,   519,     0,     0,   520,   521,   522,   523,
     524,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     585,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   488,   489,     0,     0,     0,     0,
       0,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,     0,     0,     0,   514,   515,
     516,   517,   518,   519,     0,     0,   520,   521,   522,   523,
     524,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   488,   489,     0,     0,   498,   499,   500,
     501,   502,   538,   539,   505,   506,   507,   508,     0,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
       0,     0,   618,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   488,   489,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,   498,   499,   500,   501,
     502,     0,     0,   505,   506,   507,   508,     0,   510,   511,
     512,   513,     0,     0,     0,   514,     0,   516,   517,     0,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   795,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     488,   489,     0,     0,   498,   499,   500,   501,   502,   538,
     539,   505,   506,   507,   508,     0,   510,   511,   512,   513,
       0,     0,     0,   514,     0,   516,   517,     0,     0,     0,
       0,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   853,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   488,   489,
       0,     0,     0,     0,     0,     0,     0,   538,   539,     0,
       0,     0,     0,   498,   499,   500,   501,   502,     0,     0,
     505,   506,   507,   508,     0,   510,   511,   512,   513,   488,
     489,     0,   514,     0,   516,   517,     0,     0,     0,     0,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,   498,   499,   500,   501,   502,   538,   539,   505,   506,
     507,   508,     0,   510,   511,   512,   513,   488,   489,     0,
     514,     0,   516,   517,     0,     0,     0,     0,   520,     0,
       0,     0,   498,   499,   500,   501,   502,     0,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,     0,
       0,   514,     0,   516,   517,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   488,   489,     0,     0,
     498,   499,   500,   501,   502,   538,   539,   505,   506,   507,
     508,     0,   510,   511,   512,   513,     0,     0,     0,   514,
       0,   516,   517,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,   535,   536,   537,   488,   489,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,   498,
     499,     0,     0,   502,     0,     0,   505,   506,   507,   508,
       0,   510,   511,   512,   513,     0,     0,     0,   514,     0,
     516,   517,     0,     0,     0,     0,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,   528,   529,   530,   531,   532,     0,     0,
       0,   536,     0,     0,     0,     0,     0,   498,   499,   500,
     501,   502,   538,   539,   505,   506,   507,   508,     0,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
       0,     0,     0,     0,     0,     0,     0,     0,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,   146,     0,     0,     0,
       0,     0,     0,     0,   531,   532,   533,   534,   535,   536,
     537,   147,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,   148,     0,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
       0,     0,     0,     0,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,     0,     0,
       0,     0,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,   147,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,   148,     0,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,   146,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,   147,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,   148,     0,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,     0,
       0,     0,     0,     0,   146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,     0,   147,
       0,     0,     0,     0,   314,   315,   316,     0,     0,     0,
       0,     0,     0,    49,   148,     0,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,     0,   147,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
     148,     0,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,     0,     0,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   210,   211,   212,   213,    48,
       0,   214,   215,   216,   217,   218,   219,     0,     0,     0,
       0,     0,     0,     0,     0,   563,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,     0,     0,
     230,   231
};

static const yytype_int16 yycheck[] =
{
      12,   110,    85,   276,   236,   291,   274,   422,   132,   420,
     540,   401,   285,   403,   316,   405,   361,   608,   312,   313,
     110,   462,   558,   309,   560,   616,   562,    79,    20,    21,
       0,     7,    21,    19,    18,    19,     6,    14,    15,    34,
      46,    19,    49,   119,    49,    33,    19,   128,   578,   579,
      32,   136,   113,   145,    18,   116,   117,   114,   356,    29,
     630,    31,   136,    33,    53,    14,    15,   129,    80,    39,
     139,    59,   116,   117,   145,   153,    46,   114,   153,   114,
     114,   156,    52,   145,   159,   184,   123,   145,   123,   123,
     388,   145,   129,   185,   145,   114,   153,   154,   110,   156,
     185,    71,   159,   184,   123,   183,   180,   183,   179,   145,
     105,   106,   145,   105,   106,   176,   177,   154,   242,   154,
     154,   113,    92,   115,   116,   117,   118,   185,   153,   153,
     122,   185,   176,   177,   185,   154,   154,    53,   145,   145,
     159,   129,    11,    59,    20,    21,   432,   420,   181,   185,
     145,   153,   180,    22,    23,   183,   180,   145,   183,   750,
     136,   145,   139,   153,   273,   451,   143,   153,   145,   146,
     279,   154,   164,   165,   166,   153,   165,   159,   183,   288,
     153,   145,   752,   273,   176,   177,   459,   176,   461,   279,
     139,   243,   486,   183,   143,   177,   145,   146,   288,   544,
     183,   177,   179,   114,   186,   314,   315,   177,   156,   185,
      58,   294,   123,   183,   156,    53,   746,     4,     5,   153,
       7,    59,   129,   306,   314,   315,   129,   239,   758,   105,
     106,   153,   153,   153,   179,   129,   184,   113,   145,   115,
     116,   117,   118,   154,   153,   635,   122,   153,    35,   840,
     786,   153,   153,   160,   145,   153,   178,   178,   788,   850,
     180,   273,   156,   178,   183,   153,   185,   279,   183,    20,
      21,   180,   161,   153,   180,   166,   288,   161,   180,   180,
     178,   147,   148,   149,   150,   394,   395,   396,   397,   575,
     178,   400,   145,   402,   183,   404,   145,   406,   178,   183,
     176,   177,   314,   315,   394,   395,   396,   397,   602,   750,
     400,   156,   402,   728,   404,   327,   161,   619,   759,   156,
     856,   145,    53,    66,   161,   156,   156,    70,    59,    53,
     161,   161,    53,   139,   179,    59,   161,   145,    59,   161,
     176,   450,   179,   426,    87,    88,    89,    90,   179,   179,
      20,    21,   103,   104,   105,   106,   107,   440,   183,   110,
     450,   183,   113,   145,   115,   116,   117,   118,   145,   181,
     182,   122,   184,   124,   125,   147,   148,   149,   183,   769,
     185,   145,   394,   395,   396,   397,   147,   148,   400,   154,
     402,   184,   404,   804,   406,   478,   184,   147,   148,    42,
      43,   145,    53,   184,   145,   145,   184,   145,   145,   160,
     161,   162,   163,   164,   165,   166,    34,    34,   430,   179,
     544,   156,   136,   179,   710,   176,   177,   842,   160,   183,
      20,    21,   160,   103,   104,   105,   106,   160,   450,   548,
     160,   160,   160,   113,   160,   115,   116,   117,   118,   160,
     160,   160,   122,   156,   124,   125,   183,   145,   548,   727,
     156,   145,   474,    32,   573,   160,   179,   184,   160,   160,
     160,   179,   581,   179,   139,   160,   183,   182,    36,   183,
     180,   590,   591,   573,   593,   183,   183,   119,   177,   183,
     183,   581,   162,   163,   164,   165,   166,   179,   184,   178,
     590,   591,    13,   593,   587,   139,   176,   177,   156,   182,
     184,   136,   145,   153,   145,   105,   106,   145,   145,   178,
     178,   145,   808,   113,   153,   115,   116,   117,   118,   185,
     145,   804,   122,   147,   160,   179,   548,   236,   770,   145,
       1,     7,    42,   145,   183,   161,   145,   145,   161,   161,
     185,   147,    20,    21,   145,   178,   178,   178,   178,   145,
     145,   573,   145,   145,   183,    49,   466,    61,   179,   581,
     269,   178,   162,   163,   164,   165,   166,   178,   590,   591,
      32,   593,   185,   282,   178,   180,   176,   177,   161,   178,
     178,   178,   291,   161,    32,    37,   295,   178,   297,   178,
     178,   185,   107,   712,    56,    57,   305,   183,   178,    64,
     309,   310,   311,   178,   723,   906,   725,   316,    56,    57,
      80,    62,   712,   916,   877,   324,   325,   326,   628,   328,
     329,   330,   699,   723,     1,   103,   104,   105,   106,   107,
     339,   866,   110,   111,   112,   113,   744,   115,   116,   117,
     118,   248,   802,   424,   122,   725,   124,   125,   407,   768,
     706,    43,   130,   115,   132,    73,   541,   462,   757,   399,
     932,    -1,    -1,    -1,    -1,    -1,    -1,   115,   768,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   699,   771,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     712,    -1,   154,   155,    -1,    -1,    -1,   159,   176,   177,
     162,   723,    -1,   725,    -1,    -1,   154,   155,    -1,    -1,
      -1,   159,    -1,    -1,   162,   177,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   432,   186,    -1,    -1,    -1,    -1,   177,
     439,   179,    -1,    -1,   443,   828,    -1,    -1,   186,    -1,
      -1,    -1,   451,    -1,    -1,    -1,   768,    -1,    -1,    -1,
     869,   460,    -1,   462,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,   859,    -1,    -1,   869,
      -1,    -1,    -1,    -1,    -1,   868,    -1,    -1,    -1,    -1,
      -1,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,    -1,    -1,   502,   503,   504,   505,   506,   507,    -1,
     509,   510,    -1,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,    -1,
     539,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,   107,   869,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    32,
      -1,   122,    -1,   124,   125,    -1,   575,    -1,    20,    21,
      -1,   580,    -1,    -1,    -1,   584,   585,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   604,   605,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   615,    -1,    -1,   618,
     619,    -1,   934,   622,    -1,   176,   177,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,   950,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,   114,   115,    -1,   130,   131,   132,    -1,   657,   658,
     123,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,   116,   117,   118,   676,   677,   155,
     122,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   154,   155,    -1,    -1,    -1,   159,    -1,   697,   162,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
      -1,   710,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   748,
     749,    -1,    -1,    -1,    -1,    -1,   755,    -1,   757,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   770,    -1,    -1,    -1,   774,    -1,    -1,    -1,   778,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,    10,   795,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,   808,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    41,    -1,   827,    44,
      -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   888,
     105,   106,   891,   892,   893,   894,   895,    -1,    -1,   898,
     899,    -1,   901,    -1,    -1,   120,   121,    -1,    -1,    -1,
      -1,   910,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,   937,    -1,
      -1,    -1,    -1,   942,    -1,    -1,    -1,   162,   163,   164,
      -1,   166,    -1,    -1,   169,   170,   955,    -1,    -1,    -1,
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
     159,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,   186,    -1,    -1,
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
      -1,   159,    -1,   161,   162,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,   177,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,   186,    -1,
     129,    -1,    32,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     179,    -1,   181,   182,   183,   184,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,   154,   155,    -1,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,   105,   106,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,   177,    -1,    -1,
     120,   121,    -1,    -1,    -1,    -1,   186,    -1,    -1,   129,
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
      -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    32,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,    -1,   166,    -1,    -1,   169,   170,    14,
      15,    16,    17,    18,    -1,   177,    -1,   179,   180,   181,
     182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
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
      -1,    -1,    -1,    -1,   120,   121,    -1,   186,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    56,    57,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,
      -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      38,   178,   179,    41,   181,   182,    44,   184,    -1,    47,
      -1,    -1,    -1,    51,   115,    -1,    54,    -1,    -1,    -1,
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
      -1,    -1,    21,    -1,   162,   163,   164,    26,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,   179,    41,   181,   182,    44,   184,    -1,    47,    -1,
      -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,   154,   155,    93,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,
      32,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    14,    15,    16,    17,    18,    -1,    -1,    -1,   179,
     180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,
     162,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,   186,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   179,   180,   181,   182,
      26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
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
      17,    18,    -1,    -1,   160,    -1,   162,   163,   164,    26,
     166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,   179,    41,   181,   182,    44,   184,    -1,
      47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   154,   155,    93,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    32,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   179,   180,   181,   182,    26,   184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   154,   155,    93,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    32,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    14,    15,    16,    17,    18,    -1,    -1,    -1,   179,
     180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,   154,   155,
      93,    -1,    -1,   159,    -1,    -1,   162,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     179,    41,   181,   182,    44,   184,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,   181,   182,    -1,   184,    95,    96,    97,    98,    99,
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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    20,    21,    -1,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     176,   177,   103,   104,   105,   106,   107,   183,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   176,   177,   103,   104,   105,
     106,   107,   183,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    20,    21,   180,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      20,    21,   180,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      20,    21,   180,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    20,    21,
     180,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
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
      -1,    -1,    20,    21,   176,   177,    -1,    -1,   180,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,   176,   177,   178,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
     178,   103,   104,   105,   106,   107,    20,    21,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,   178,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    20,    21,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    20,    21,    -1,    -1,   103,   104,   105,
     106,   107,   176,   177,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,   128,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      20,    21,    -1,    -1,   103,   104,   105,   106,   107,   176,
     177,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    20,
      21,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,   176,   177,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    20,    21,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    20,    21,    -1,    -1,
     103,   104,   105,   106,   107,   176,   177,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,   103,
     104,    -1,    -1,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,   176,   177,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,   161,   162,   163,   164,   165,
     166,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    45,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   129,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
     169,   170
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   188,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   177,   183,   191,   199,   200,   204,   225,   229,
     245,   300,   305,   307,   312,   348,   350,    18,    19,   145,
     219,   220,   221,   139,   205,   206,   145,   166,   201,   202,
     184,   145,   181,   190,   351,   349,    33,    59,   129,   145,
     222,   223,   224,   237,     4,     5,     7,    35,   310,    58,
     298,   154,   153,   156,   153,   201,    21,    53,   165,   176,
     203,   145,   303,   304,   298,   145,   145,   145,   129,   179,
     153,   178,    53,    59,   230,   231,   232,    53,    59,   306,
      53,    59,   311,    53,    59,   299,    14,    15,   139,   143,
     145,   146,   179,   193,   220,   139,   206,   145,   145,   145,
     154,   185,   303,    53,    59,   189,   184,   184,   145,   221,
     223,    34,   105,   106,   145,   228,   184,   233,   145,   145,
     309,   184,   301,   147,   148,   192,    14,    15,   139,   143,
     145,   193,   217,   218,   203,    24,    30,    45,    60,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
     237,   315,   317,   318,   320,   323,   145,   319,   145,   290,
     291,   180,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     124,   125,   126,   127,   130,   131,   132,   133,   134,   135,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     169,   170,    34,    34,   179,   226,   235,   156,   184,   156,
     308,   313,   297,   136,   281,   147,   148,   149,   153,   180,
     160,   160,   160,   160,   179,   160,   160,   160,   160,   160,
     160,    32,    56,    57,   115,   154,   155,   159,   162,   177,
     183,   186,   183,   156,   183,   180,   281,   286,   287,   156,
     227,     1,     8,     9,    10,    12,    14,    15,    16,    17,
      18,    25,    26,    27,    28,    37,    38,    40,    41,    44,
      47,    48,    50,    51,    54,    55,    61,    71,    93,    94,
     105,   106,   120,   121,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   146,   162,   163,   164,   166,   169,   170,
     179,   181,   182,   183,   185,   195,   197,   207,   208,   211,
     212,   213,   214,   215,   216,   233,   234,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   254,
     257,   260,   264,   265,   266,   267,   268,   269,   270,   275,
     278,   315,   352,   356,   361,   363,   365,    66,    70,    87,
      88,    89,    90,   316,   302,   237,   184,   185,   281,   221,
     145,   296,   314,   217,   326,   328,   330,   324,   145,   321,
     332,   334,   336,   338,   340,   342,   344,   346,    32,    32,
     159,   177,   186,   178,   278,   145,   185,   323,   185,   290,
      46,   293,   314,   180,   183,   323,   278,   314,   323,   352,
     179,   160,   114,   183,   236,   266,   278,   233,   278,   179,
     278,   160,   160,   179,   183,   160,   139,    54,   278,   233,
     160,   114,   236,   278,   278,   278,   182,   264,   264,    12,
     323,    12,   323,   278,   358,   362,   194,   278,   278,   278,
     237,   278,   278,   278,   182,    36,   183,   183,   278,   145,
     179,   183,   183,   183,   296,   225,   119,   183,    20,    21,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   122,   123,   124,   125,   126,   127,
     130,   131,   132,   133,   134,   135,   154,   155,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   176,   177,
     179,   184,   145,   185,   284,   296,   178,   153,   156,   159,
     295,   323,   323,   323,   323,   180,   319,   323,   226,   323,
     226,   323,   226,   145,   288,   289,   323,   291,   178,   178,
     293,   114,   154,   156,   159,   292,   286,   233,    19,   179,
     237,   255,   236,   278,   114,   154,   183,    13,   278,   233,
     248,   250,   278,   252,   156,   139,   323,   236,   278,   183,
     182,   314,    49,   145,   162,   179,   278,   353,   354,   355,
     357,   358,   359,   360,   314,   179,   354,   360,   128,   183,
     185,   149,   150,   192,   198,   180,   160,   237,   184,   233,
     177,   180,   258,   278,   136,   263,   264,    18,   145,   145,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   145,   120,   121,   278,
     278,   145,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   120,   121,   278,    21,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   145,   276,   278,   180,   258,   302,   154,   183,   177,
     185,   225,   281,   283,   178,   145,   323,   114,   123,   154,
     294,   161,   183,   161,   161,   161,   183,   161,   227,   161,
     227,   161,   227,   156,   161,   183,   161,   183,   323,   236,
     278,   258,   180,   258,   160,   278,   323,   183,   233,   180,
     323,   323,   180,   323,   161,   183,    19,   264,   123,   292,
     180,   153,   183,   355,   178,   153,   178,   183,    19,   180,
     355,   185,   278,   358,   185,   278,   147,   196,   271,   273,
     235,    11,    22,    23,   209,   210,   145,   353,   153,   180,
     114,   123,   154,   159,   261,   262,   226,   160,   179,   278,
     278,   178,   278,   278,   145,   156,     1,   178,   180,   185,
     278,   224,     7,    42,   282,   183,   154,   183,   294,   236,
     278,   327,   323,   331,   325,   322,   333,   161,   337,   161,
     341,   161,   323,   345,   288,   347,   154,   292,   183,   180,
     145,   180,   161,   161,   161,   161,   263,   258,   278,   278,
     354,   355,   145,   353,   178,   178,   278,   178,   359,   258,
     354,   185,   178,   151,   147,   323,   226,   185,   233,   278,
     178,   278,   145,   145,   145,   145,   153,   178,   227,   279,
     180,   258,   278,   277,   183,   178,    43,   282,   293,   236,
     278,   183,   161,   335,   339,   343,   278,   233,   161,   183,
     256,   249,   251,   253,   179,   183,   355,   178,   123,   292,
     178,   183,   355,   178,   161,   227,   233,   180,   261,   178,
     128,   233,   259,   323,   180,   183,   285,   232,   183,   329,
     278,   145,   278,   278,   278,   180,   278,   278,   178,   178,
     278,   278,   278,   185,   272,   161,   210,   114,   278,   161,
     231,   180,   161,   180,   183,   364,   178,   364,   178,   237,
     274,   278,   280,   183,   278,    49,   178,   185,   237,   180,
     278,   178,   178
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
     218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
     220,   220,   220,   221,   221,   222,   222,   222,   223,   223,
     224,   224,   225,   225,   226,   226,   226,   227,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   229,   230,   230,   230,   231,   232,   233,   233,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   235,
     235,   235,   236,   236,   236,   237,   237,   237,   238,   239,
     239,   239,   239,   240,   241,   242,   242,   242,   242,   242,
     243,   243,   243,   243,   244,   245,   245,   246,   248,   249,
     247,   250,   251,   247,   252,   253,   247,   255,   256,   254,
     257,   257,   257,   258,   258,   259,   259,   259,   260,   260,
     260,   261,   261,   261,   261,   262,   262,   263,   263,   264,
     264,   265,   265,   265,   265,   265,   265,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   267,   267,   267,   267,
     268,   269,   269,   270,   271,   272,   270,   273,   274,   270,
     275,   276,   277,   275,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   279,   280,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   281,   281,   282,   282,   283,   284,
     284,   285,   284,   284,   284,   286,   286,   287,   287,   288,
     288,   289,   289,   290,   291,   291,   292,   292,   293,   293,
     293,   293,   293,   293,   294,   294,   294,   295,   295,   296,
     296,   296,   296,   296,   297,   297,   298,   298,   299,   299,
     299,   300,   301,   300,   302,   302,   302,   303,   304,   304,
     305,   306,   306,   306,   307,   307,   308,   308,   309,   310,
     310,   311,   311,   311,   313,   312,   314,   314,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   316,   316,   316,   316,   316,   316,   317,
     318,   318,   319,   319,   321,   322,   320,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   324,   325,   323,   323,   326,   327,
     323,   328,   329,   323,   330,   331,   323,   323,   332,   333,
     323,   334,   335,   323,   323,   336,   337,   323,   338,   339,
     323,   323,   340,   341,   323,   342,   343,   323,   344,   345,
     323,   346,   347,   323,   349,   348,   351,   350,   352,   352,
     352,   352,   353,   353,   353,   353,   354,   354,   355,   355,
     356,   356,   356,   356,   356,   356,   357,   357,   358,   359,
     359,   360,   360,   361,   361,   362,   362,   363,   364,   364,
     365,   365
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
       2,     2,     3,     0,     1,     1,     4,     2,     3,     7,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     2,     2,     1,     3,     2,     2,     2,
       4,     5,     2,     1,     1,     2,     3,     4,     2,     3,
       3,     4,     2,     3,     4,     1,     1,     2,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       6,     5,     6,     3,     0,     0,     8,     0,     0,     9,
       3,     0,     0,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     4,     3,     1,     3,     4,     3,
       4,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     0,     4,     0,     1,     3,     0,
       3,     0,     7,     5,     5,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     3,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     3,     5,     4,     1,     2,
       4,     0,     1,     1,     7,     9,     0,     2,     2,     1,
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3400 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3406 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3412 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3418 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3424 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3430 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3436 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3442 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3448 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3454 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3460 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3466 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3472 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3478 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3484 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3490 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3496 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3502 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3508 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3514 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3520 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3526 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3532 "ds_parser.cpp"
        break;

    case 224: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3538 "ds_parser.cpp"
        break;

    case 225: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3544 "ds_parser.cpp"
        break;

    case 226: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3550 "ds_parser.cpp"
        break;

    case 227: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3556 "ds_parser.cpp"
        break;

    case 228: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3562 "ds_parser.cpp"
        break;

    case 233: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3568 "ds_parser.cpp"
        break;

    case 234: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3574 "ds_parser.cpp"
        break;

    case 235: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3580 "ds_parser.cpp"
        break;

    case 236: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3586 "ds_parser.cpp"
        break;

    case 237: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3592 "ds_parser.cpp"
        break;

    case 238: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3598 "ds_parser.cpp"
        break;

    case 239: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3604 "ds_parser.cpp"
        break;

    case 240: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3610 "ds_parser.cpp"
        break;

    case 241: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3616 "ds_parser.cpp"
        break;

    case 242: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3622 "ds_parser.cpp"
        break;

    case 243: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3628 "ds_parser.cpp"
        break;

    case 244: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3634 "ds_parser.cpp"
        break;

    case 246: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3640 "ds_parser.cpp"
        break;

    case 247: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3646 "ds_parser.cpp"
        break;

    case 254: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3652 "ds_parser.cpp"
        break;

    case 257: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp"
        break;

    case 258: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 259: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 261: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3676 "ds_parser.cpp"
        break;

    case 262: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3682 "ds_parser.cpp"
        break;

    case 263: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3688 "ds_parser.cpp"
        break;

    case 264: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3694 "ds_parser.cpp"
        break;

    case 265: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3700 "ds_parser.cpp"
        break;

    case 266: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3706 "ds_parser.cpp"
        break;

    case 267: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3712 "ds_parser.cpp"
        break;

    case 268: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3718 "ds_parser.cpp"
        break;

    case 269: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3724 "ds_parser.cpp"
        break;

    case 270: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3730 "ds_parser.cpp"
        break;

    case 275: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3736 "ds_parser.cpp"
        break;

    case 278: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3742 "ds_parser.cpp"
        break;

    case 281: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3748 "ds_parser.cpp"
        break;

    case 283: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3754 "ds_parser.cpp"
        break;

    case 284: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3760 "ds_parser.cpp"
        break;

    case 286: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3766 "ds_parser.cpp"
        break;

    case 287: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3772 "ds_parser.cpp"
        break;

    case 288: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3778 "ds_parser.cpp"
        break;

    case 289: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3784 "ds_parser.cpp"
        break;

    case 290: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3790 "ds_parser.cpp"
        break;

    case 291: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3796 "ds_parser.cpp"
        break;

    case 293: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3802 "ds_parser.cpp"
        break;

    case 296: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3808 "ds_parser.cpp"
        break;

    case 297: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3814 "ds_parser.cpp"
        break;

    case 302: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3820 "ds_parser.cpp"
        break;

    case 308: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3826 "ds_parser.cpp"
        break;

    case 314: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3832 "ds_parser.cpp"
        break;

    case 317: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3838 "ds_parser.cpp"
        break;

    case 318: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3844 "ds_parser.cpp"
        break;

    case 319: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3850 "ds_parser.cpp"
        break;

    case 320: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3856 "ds_parser.cpp"
        break;

    case 323: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3862 "ds_parser.cpp"
        break;

    case 352: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3868 "ds_parser.cpp"
        break;

    case 353: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3874 "ds_parser.cpp"
        break;

    case 354: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3880 "ds_parser.cpp"
        break;

    case 355: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3886 "ds_parser.cpp"
        break;

    case 356: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3892 "ds_parser.cpp"
        break;

    case 357: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3898 "ds_parser.cpp"
        break;

    case 358: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3904 "ds_parser.cpp"
        break;

    case 359: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3910 "ds_parser.cpp"
        break;

    case 360: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3916 "ds_parser.cpp"
        break;

    case 361: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3922 "ds_parser.cpp"
        break;

    case 362: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3928 "ds_parser.cpp"
        break;

    case 363: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3934 "ds_parser.cpp"
        break;

    case 364: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3940 "ds_parser.cpp"
        break;

    case 365: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3946 "ds_parser.cpp"
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
#line 4238 "ds_parser.cpp"
    break;

  case 16:
#line 470 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4244 "ds_parser.cpp"
    break;

  case 17:
#line 471 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4250 "ds_parser.cpp"
    break;

  case 18:
#line 475 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4256 "ds_parser.cpp"
    break;

  case 19:
#line 476 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4262 "ds_parser.cpp"
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
#line 4281 "ds_parser.cpp"
    break;

  case 21:
#line 497 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4287 "ds_parser.cpp"
    break;

  case 22:
#line 498 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4293 "ds_parser.cpp"
    break;

  case 23:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4299 "ds_parser.cpp"
    break;

  case 24:
#line 500 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4305 "ds_parser.cpp"
    break;

  case 25:
#line 504 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4311 "ds_parser.cpp"
    break;

  case 26:
#line 508 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4321 "ds_parser.cpp"
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
#line 4335 "ds_parser.cpp"
    break;

  case 28:
#line 522 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4345 "ds_parser.cpp"
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
#line 4363 "ds_parser.cpp"
    break;

  case 30:
#line 546 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4373 "ds_parser.cpp"
    break;

  case 31:
#line 551 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4383 "ds_parser.cpp"
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
#line 4410 "ds_parser.cpp"
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
#line 4422 "ds_parser.cpp"
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
#line 4446 "ds_parser.cpp"
    break;

  case 36:
#line 615 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4454 "ds_parser.cpp"
    break;

  case 37:
#line 618 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4463 "ds_parser.cpp"
    break;

  case 38:
#line 622 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4474 "ds_parser.cpp"
    break;

  case 39:
#line 628 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4485 "ds_parser.cpp"
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
#line 4511 "ds_parser.cpp"
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
#line 4536 "ds_parser.cpp"
    break;

  case 42:
#line 681 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4542 "ds_parser.cpp"
    break;

  case 43:
#line 682 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4548 "ds_parser.cpp"
    break;

  case 47:
#line 695 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4556 "ds_parser.cpp"
    break;

  case 48:
#line 698 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4564 "ds_parser.cpp"
    break;

  case 49:
#line 704 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4572 "ds_parser.cpp"
    break;

  case 50:
#line 710 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4580 "ds_parser.cpp"
    break;

  case 51:
#line 713 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4588 "ds_parser.cpp"
    break;

  case 52:
#line 719 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4594 "ds_parser.cpp"
    break;

  case 53:
#line 720 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4600 "ds_parser.cpp"
    break;

  case 54:
#line 724 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4606 "ds_parser.cpp"
    break;

  case 55:
#line 725 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4612 "ds_parser.cpp"
    break;

  case 56:
#line 726 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4623 "ds_parser.cpp"
    break;

  case 57:
#line 735 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4629 "ds_parser.cpp"
    break;

  case 58:
#line 736 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4635 "ds_parser.cpp"
    break;

  case 59:
#line 740 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4646 "ds_parser.cpp"
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
#line 4664 "ds_parser.cpp"
    break;

  case 61:
#line 765 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4674 "ds_parser.cpp"
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
#line 4686 "ds_parser.cpp"
    break;

  case 63:
#line 783 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4697 "ds_parser.cpp"
    break;

  case 64:
#line 792 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4703 "ds_parser.cpp"
    break;

  case 65:
#line 793 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4709 "ds_parser.cpp"
    break;

  case 66:
#line 794 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4715 "ds_parser.cpp"
    break;

  case 67:
#line 795 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4721 "ds_parser.cpp"
    break;

  case 68:
#line 796 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4727 "ds_parser.cpp"
    break;

  case 69:
#line 797 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4733 "ds_parser.cpp"
    break;

  case 70:
#line 801 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4743 "ds_parser.cpp"
    break;

  case 71:
#line 806 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4753 "ds_parser.cpp"
    break;

  case 72:
#line 814 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4759 "ds_parser.cpp"
    break;

  case 73:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4765 "ds_parser.cpp"
    break;

  case 74:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4771 "ds_parser.cpp"
    break;

  case 75:
#line 820 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4777 "ds_parser.cpp"
    break;

  case 76:
#line 821 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4783 "ds_parser.cpp"
    break;

  case 77:
#line 822 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4789 "ds_parser.cpp"
    break;

  case 78:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4795 "ds_parser.cpp"
    break;

  case 79:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4801 "ds_parser.cpp"
    break;

  case 80:
#line 825 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4807 "ds_parser.cpp"
    break;

  case 81:
#line 826 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4813 "ds_parser.cpp"
    break;

  case 82:
#line 827 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4821 "ds_parser.cpp"
    break;

  case 83:
#line 833 "ds_parser.ypp"
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
#line 4840 "ds_parser.cpp"
    break;

  case 84:
#line 847 "ds_parser.ypp"
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
#line 4859 "ds_parser.cpp"
    break;

  case 85:
#line 864 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4865 "ds_parser.cpp"
    break;

  case 86:
#line 865 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4871 "ds_parser.cpp"
    break;

  case 87:
#line 866 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4877 "ds_parser.cpp"
    break;

  case 88:
#line 870 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4890 "ds_parser.cpp"
    break;

  case 89:
#line 878 "ds_parser.ypp"
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
#line 4905 "ds_parser.cpp"
    break;

  case 90:
#line 891 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4914 "ds_parser.cpp"
    break;

  case 91:
#line 895 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4923 "ds_parser.cpp"
    break;

  case 92:
#line 902 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4929 "ds_parser.cpp"
    break;

  case 93:
#line 903 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4935 "ds_parser.cpp"
    break;

  case 94:
#line 907 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4941 "ds_parser.cpp"
    break;

  case 95:
#line 908 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4947 "ds_parser.cpp"
    break;

  case 96:
#line 909 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4953 "ds_parser.cpp"
    break;

  case 97:
#line 913 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4961 "ds_parser.cpp"
    break;

  case 98:
#line 916 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4970 "ds_parser.cpp"
    break;

  case 99:
#line 923 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4979 "ds_parser.cpp"
    break;

  case 100:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4985 "ds_parser.cpp"
    break;

  case 101:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4991 "ds_parser.cpp"
    break;

  case 102:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4997 "ds_parser.cpp"
    break;

  case 103:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5003 "ds_parser.cpp"
    break;

  case 104:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5009 "ds_parser.cpp"
    break;

  case 105:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5015 "ds_parser.cpp"
    break;

  case 106:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5021 "ds_parser.cpp"
    break;

  case 107:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5027 "ds_parser.cpp"
    break;

  case 108:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5033 "ds_parser.cpp"
    break;

  case 109:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5039 "ds_parser.cpp"
    break;

  case 110:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5045 "ds_parser.cpp"
    break;

  case 111:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5051 "ds_parser.cpp"
    break;

  case 112:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5057 "ds_parser.cpp"
    break;

  case 113:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5063 "ds_parser.cpp"
    break;

  case 114:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5069 "ds_parser.cpp"
    break;

  case 115:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5075 "ds_parser.cpp"
    break;

  case 116:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5081 "ds_parser.cpp"
    break;

  case 117:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5087 "ds_parser.cpp"
    break;

  case 118:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5093 "ds_parser.cpp"
    break;

  case 119:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5099 "ds_parser.cpp"
    break;

  case 120:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5105 "ds_parser.cpp"
    break;

  case 121:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5111 "ds_parser.cpp"
    break;

  case 122:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5117 "ds_parser.cpp"
    break;

  case 123:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5123 "ds_parser.cpp"
    break;

  case 124:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5129 "ds_parser.cpp"
    break;

  case 125:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5135 "ds_parser.cpp"
    break;

  case 126:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5141 "ds_parser.cpp"
    break;

  case 127:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5147 "ds_parser.cpp"
    break;

  case 128:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5153 "ds_parser.cpp"
    break;

  case 129:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5159 "ds_parser.cpp"
    break;

  case 130:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5165 "ds_parser.cpp"
    break;

  case 131:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5171 "ds_parser.cpp"
    break;

  case 132:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5177 "ds_parser.cpp"
    break;

  case 133:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5183 "ds_parser.cpp"
    break;

  case 134:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5189 "ds_parser.cpp"
    break;

  case 135:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5195 "ds_parser.cpp"
    break;

  case 136:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5201 "ds_parser.cpp"
    break;

  case 137:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5207 "ds_parser.cpp"
    break;

  case 138:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5213 "ds_parser.cpp"
    break;

  case 139:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5219 "ds_parser.cpp"
    break;

  case 140:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5225 "ds_parser.cpp"
    break;

  case 141:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5231 "ds_parser.cpp"
    break;

  case 142:
#line 972 "ds_parser.ypp"
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
#line 5255 "ds_parser.cpp"
    break;

  case 143:
#line 994 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5261 "ds_parser.cpp"
    break;

  case 144:
#line 995 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5267 "ds_parser.cpp"
    break;

  case 145:
#line 996 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5273 "ds_parser.cpp"
    break;

  case 146:
#line 1000 "ds_parser.ypp"
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
#line 5314 "ds_parser.cpp"
    break;

  case 147:
#line 1039 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5323 "ds_parser.cpp"
    break;

  case 148:
#line 1046 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5332 "ds_parser.cpp"
    break;

  case 149:
#line 1050 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5345 "ds_parser.cpp"
    break;

  case 150:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5351 "ds_parser.cpp"
    break;

  case 151:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5357 "ds_parser.cpp"
    break;

  case 152:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5363 "ds_parser.cpp"
    break;

  case 153:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5369 "ds_parser.cpp"
    break;

  case 154:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5375 "ds_parser.cpp"
    break;

  case 155:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5381 "ds_parser.cpp"
    break;

  case 156:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5387 "ds_parser.cpp"
    break;

  case 157:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5393 "ds_parser.cpp"
    break;

  case 158:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5399 "ds_parser.cpp"
    break;

  case 159:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5405 "ds_parser.cpp"
    break;

  case 160:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5411 "ds_parser.cpp"
    break;

  case 161:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5417 "ds_parser.cpp"
    break;

  case 162:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5423 "ds_parser.cpp"
    break;

  case 163:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5429 "ds_parser.cpp"
    break;

  case 164:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5435 "ds_parser.cpp"
    break;

  case 165:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5441 "ds_parser.cpp"
    break;

  case 166:
#line 1077 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5447 "ds_parser.cpp"
    break;

  case 167:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5453 "ds_parser.cpp"
    break;

  case 168:
#line 1079 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5459 "ds_parser.cpp"
    break;

  case 169:
#line 1083 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5469 "ds_parser.cpp"
    break;

  case 170:
#line 1088 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5480 "ds_parser.cpp"
    break;

  case 171:
#line 1094 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5488 "ds_parser.cpp"
    break;

  case 172:
#line 1100 "ds_parser.ypp"
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
#line 5527 "ds_parser.cpp"
    break;

  case 173:
#line 1134 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5535 "ds_parser.cpp"
    break;

  case 174:
#line 1137 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5543 "ds_parser.cpp"
    break;

  case 175:
#line 1143 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5549 "ds_parser.cpp"
    break;

  case 176:
#line 1144 "ds_parser.ypp"
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
#line 5565 "ds_parser.cpp"
    break;

  case 177:
#line 1155 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5571 "ds_parser.cpp"
    break;

  case 178:
#line 1159 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5579 "ds_parser.cpp"
    break;

  case 179:
#line 1165 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5587 "ds_parser.cpp"
    break;

  case 180:
#line 1168 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5595 "ds_parser.cpp"
    break;

  case 181:
#line 1171 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5604 "ds_parser.cpp"
    break;

  case 182:
#line 1175 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5612 "ds_parser.cpp"
    break;

  case 183:
#line 1181 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5618 "ds_parser.cpp"
    break;

  case 184:
#line 1185 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5624 "ds_parser.cpp"
    break;

  case 185:
#line 1189 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5632 "ds_parser.cpp"
    break;

  case 186:
#line 1192 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5640 "ds_parser.cpp"
    break;

  case 187:
#line 1195 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5650 "ds_parser.cpp"
    break;

  case 188:
#line 1200 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5658 "ds_parser.cpp"
    break;

  case 189:
#line 1203 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5668 "ds_parser.cpp"
    break;

  case 190:
#line 1211 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5676 "ds_parser.cpp"
    break;

  case 191:
#line 1214 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5686 "ds_parser.cpp"
    break;

  case 192:
#line 1219 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5694 "ds_parser.cpp"
    break;

  case 193:
#line 1222 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5704 "ds_parser.cpp"
    break;

  case 194:
#line 1230 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5712 "ds_parser.cpp"
    break;

  case 195:
#line 1236 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5718 "ds_parser.cpp"
    break;

  case 196:
#line 1237 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5724 "ds_parser.cpp"
    break;

  case 197:
#line 1241 "ds_parser.ypp"
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
#line 5760 "ds_parser.cpp"
    break;

  case 198:
#line 1275 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5766 "ds_parser.cpp"
    break;

  case 199:
#line 1275 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5772 "ds_parser.cpp"
    break;

  case 200:
#line 1275 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5780 "ds_parser.cpp"
    break;

  case 201:
#line 1278 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5786 "ds_parser.cpp"
    break;

  case 202:
#line 1278 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5792 "ds_parser.cpp"
    break;

  case 203:
#line 1278 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5802 "ds_parser.cpp"
    break;

  case 204:
#line 1283 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5808 "ds_parser.cpp"
    break;

  case 205:
#line 1283 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5814 "ds_parser.cpp"
    break;

  case 206:
#line 1283 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5824 "ds_parser.cpp"
    break;

  case 207:
#line 1291 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5830 "ds_parser.cpp"
    break;

  case 208:
#line 1291 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5836 "ds_parser.cpp"
    break;

  case 209:
#line 1291 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5844 "ds_parser.cpp"
    break;

  case 210:
#line 1297 "ds_parser.ypp"
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
#line 5859 "ds_parser.cpp"
    break;

  case 211:
#line 1307 "ds_parser.ypp"
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
#line 5875 "ds_parser.cpp"
    break;

  case 212:
#line 1318 "ds_parser.ypp"
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
#line 5892 "ds_parser.cpp"
    break;

  case 213:
#line 1333 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5900 "ds_parser.cpp"
    break;

  case 214:
#line 1336 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5908 "ds_parser.cpp"
    break;

  case 215:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5914 "ds_parser.cpp"
    break;

  case 216:
#line 1343 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5926 "ds_parser.cpp"
    break;

  case 217:
#line 1350 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5939 "ds_parser.cpp"
    break;

  case 218:
#line 1361 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5945 "ds_parser.cpp"
    break;

  case 219:
#line 1362 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5951 "ds_parser.cpp"
    break;

  case 220:
#line 1363 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5957 "ds_parser.cpp"
    break;

  case 221:
#line 1367 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5963 "ds_parser.cpp"
    break;

  case 222:
#line 1368 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5969 "ds_parser.cpp"
    break;

  case 223:
#line 1369 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5975 "ds_parser.cpp"
    break;

  case 224:
#line 1370 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5981 "ds_parser.cpp"
    break;

  case 225:
#line 1374 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5991 "ds_parser.cpp"
    break;

  case 226:
#line 1379 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6001 "ds_parser.cpp"
    break;

  case 227:
#line 1387 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6007 "ds_parser.cpp"
    break;

  case 228:
#line 1388 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6013 "ds_parser.cpp"
    break;

  case 229:
#line 1392 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6023 "ds_parser.cpp"
    break;

  case 230:
#line 1398 "ds_parser.ypp"
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
#line 6088 "ds_parser.cpp"
    break;

  case 231:
#line 1461 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6094 "ds_parser.cpp"
    break;

  case 232:
#line 1462 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6100 "ds_parser.cpp"
    break;

  case 233:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6106 "ds_parser.cpp"
    break;

  case 234:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6112 "ds_parser.cpp"
    break;

  case 235:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6118 "ds_parser.cpp"
    break;

  case 236:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6124 "ds_parser.cpp"
    break;

  case 237:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6130 "ds_parser.cpp"
    break;

  case 238:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6136 "ds_parser.cpp"
    break;

  case 239:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6142 "ds_parser.cpp"
    break;

  case 240:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6148 "ds_parser.cpp"
    break;

  case 241:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6154 "ds_parser.cpp"
    break;

  case 242:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6160 "ds_parser.cpp"
    break;

  case 243:
#line 1476 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6166 "ds_parser.cpp"
    break;

  case 244:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6172 "ds_parser.cpp"
    break;

  case 245:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6178 "ds_parser.cpp"
    break;

  case 246:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6184 "ds_parser.cpp"
    break;

  case 247:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6190 "ds_parser.cpp"
    break;

  case 248:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6196 "ds_parser.cpp"
    break;

  case 249:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6202 "ds_parser.cpp"
    break;

  case 250:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6208 "ds_parser.cpp"
    break;

  case 251:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6214 "ds_parser.cpp"
    break;

  case 252:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6220 "ds_parser.cpp"
    break;

  case 253:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6226 "ds_parser.cpp"
    break;

  case 254:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6232 "ds_parser.cpp"
    break;

  case 255:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6238 "ds_parser.cpp"
    break;

  case 256:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6244 "ds_parser.cpp"
    break;

  case 257:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6250 "ds_parser.cpp"
    break;

  case 258:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6256 "ds_parser.cpp"
    break;

  case 259:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6262 "ds_parser.cpp"
    break;

  case 260:
#line 1499 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6274 "ds_parser.cpp"
    break;

  case 261:
#line 1510 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6284 "ds_parser.cpp"
    break;

  case 262:
#line 1515 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6296 "ds_parser.cpp"
    break;

  case 263:
#line 1525 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6305 "ds_parser.cpp"
    break;

  case 264:
#line 1529 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6311 "ds_parser.cpp"
    break;

  case 265:
#line 1529 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6317 "ds_parser.cpp"
    break;

  case 266:
#line 1529 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6329 "ds_parser.cpp"
    break;

  case 267:
#line 1536 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6335 "ds_parser.cpp"
    break;

  case 268:
#line 1536 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6341 "ds_parser.cpp"
    break;

  case 269:
#line 1536 "ds_parser.ypp"
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
#line 6357 "ds_parser.cpp"
    break;

  case 270:
#line 1550 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6366 "ds_parser.cpp"
    break;

  case 271:
#line 1554 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6372 "ds_parser.cpp"
    break;

  case 272:
#line 1554 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6378 "ds_parser.cpp"
    break;

  case 273:
#line 1554 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6387 "ds_parser.cpp"
    break;

  case 274:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6393 "ds_parser.cpp"
    break;

  case 275:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6399 "ds_parser.cpp"
    break;

  case 276:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6405 "ds_parser.cpp"
    break;

  case 277:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6411 "ds_parser.cpp"
    break;

  case 278:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6417 "ds_parser.cpp"
    break;

  case 279:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6423 "ds_parser.cpp"
    break;

  case 280:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6429 "ds_parser.cpp"
    break;

  case 281:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6435 "ds_parser.cpp"
    break;

  case 282:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6441 "ds_parser.cpp"
    break;

  case 283:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6447 "ds_parser.cpp"
    break;

  case 284:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6453 "ds_parser.cpp"
    break;

  case 285:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6459 "ds_parser.cpp"
    break;

  case 286:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6465 "ds_parser.cpp"
    break;

  case 287:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp"
    break;

  case 288:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6477 "ds_parser.cpp"
    break;

  case 289:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6483 "ds_parser.cpp"
    break;

  case 290:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6489 "ds_parser.cpp"
    break;

  case 291:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6495 "ds_parser.cpp"
    break;

  case 292:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6501 "ds_parser.cpp"
    break;

  case 293:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6507 "ds_parser.cpp"
    break;

  case 294:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6513 "ds_parser.cpp"
    break;

  case 295:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6519 "ds_parser.cpp"
    break;

  case 296:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6525 "ds_parser.cpp"
    break;

  case 297:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 298:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 299:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 300:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 301:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 302:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 303:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 304:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6573 "ds_parser.cpp"
    break;

  case 305:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 306:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 307:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6591 "ds_parser.cpp"
    break;

  case 308:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 309:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 310:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 311:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 312:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6621 "ds_parser.cpp"
    break;

  case 313:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6627 "ds_parser.cpp"
    break;

  case 314:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6633 "ds_parser.cpp"
    break;

  case 315:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6639 "ds_parser.cpp"
    break;

  case 316:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6645 "ds_parser.cpp"
    break;

  case 317:
#line 1604 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6654 "ds_parser.cpp"
    break;

  case 318:
#line 1608 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6663 "ds_parser.cpp"
    break;

  case 319:
#line 1612 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6671 "ds_parser.cpp"
    break;

  case 320:
#line 1615 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6679 "ds_parser.cpp"
    break;

  case 321:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6685 "ds_parser.cpp"
    break;

  case 322:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6691 "ds_parser.cpp"
    break;

  case 323:
#line 1620 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6697 "ds_parser.cpp"
    break;

  case 324:
#line 1621 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6711 "ds_parser.cpp"
    break;

  case 325:
#line 1630 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6725 "ds_parser.cpp"
    break;

  case 326:
#line 1639 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6731 "ds_parser.cpp"
    break;

  case 327:
#line 1640 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6739 "ds_parser.cpp"
    break;

  case 328:
#line 1643 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6745 "ds_parser.cpp"
    break;

  case 329:
#line 1643 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6751 "ds_parser.cpp"
    break;

  case 330:
#line 1643 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6759 "ds_parser.cpp"
    break;

  case 331:
#line 1646 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6768 "ds_parser.cpp"
    break;

  case 332:
#line 1650 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6777 "ds_parser.cpp"
    break;

  case 333:
#line 1654 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6786 "ds_parser.cpp"
    break;

  case 334:
#line 1658 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6792 "ds_parser.cpp"
    break;

  case 335:
#line 1659 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6798 "ds_parser.cpp"
    break;

  case 336:
#line 1660 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6804 "ds_parser.cpp"
    break;

  case 337:
#line 1661 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6810 "ds_parser.cpp"
    break;

  case 338:
#line 1662 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6816 "ds_parser.cpp"
    break;

  case 339:
#line 1663 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6822 "ds_parser.cpp"
    break;

  case 340:
#line 1664 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6828 "ds_parser.cpp"
    break;

  case 341:
#line 1665 "ds_parser.ypp"
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
#line 6849 "ds_parser.cpp"
    break;

  case 342:
#line 1681 "ds_parser.ypp"
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
#line 6870 "ds_parser.cpp"
    break;

  case 343:
#line 1697 "ds_parser.ypp"
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
#line 6930 "ds_parser.cpp"
    break;

  case 344:
#line 1755 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6936 "ds_parser.cpp"
    break;

  case 345:
#line 1756 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6942 "ds_parser.cpp"
    break;

  case 346:
#line 1760 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6948 "ds_parser.cpp"
    break;

  case 347:
#line 1761 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6954 "ds_parser.cpp"
    break;

  case 348:
#line 1765 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6964 "ds_parser.cpp"
    break;

  case 349:
#line 1773 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6972 "ds_parser.cpp"
    break;

  case 350:
#line 1776 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6981 "ds_parser.cpp"
    break;

  case 351:
#line 1781 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6987 "ds_parser.cpp"
    break;

  case 352:
#line 1781 "ds_parser.ypp"
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
#line 7036 "ds_parser.cpp"
    break;

  case 353:
#line 1825 "ds_parser.ypp"
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
#line 7097 "ds_parser.cpp"
    break;

  case 354:
#line 1881 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7108 "ds_parser.cpp"
    break;

  case 355:
#line 1890 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7118 "ds_parser.cpp"
    break;

  case 356:
#line 1895 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7128 "ds_parser.cpp"
    break;

  case 357:
#line 1903 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7134 "ds_parser.cpp"
    break;

  case 358:
#line 1904 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7140 "ds_parser.cpp"
    break;

  case 359:
#line 1908 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7148 "ds_parser.cpp"
    break;

  case 360:
#line 1911 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7159 "ds_parser.cpp"
    break;

  case 361:
#line 1920 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7165 "ds_parser.cpp"
    break;

  case 362:
#line 1921 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7171 "ds_parser.cpp"
    break;

  case 363:
#line 1925 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7182 "ds_parser.cpp"
    break;

  case 364:
#line 1934 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7188 "ds_parser.cpp"
    break;

  case 365:
#line 1935 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7194 "ds_parser.cpp"
    break;

  case 366:
#line 1940 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7200 "ds_parser.cpp"
    break;

  case 367:
#line 1941 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7206 "ds_parser.cpp"
    break;

  case 368:
#line 1945 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7217 "ds_parser.cpp"
    break;

  case 369:
#line 1951 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7228 "ds_parser.cpp"
    break;

  case 370:
#line 1957 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7236 "ds_parser.cpp"
    break;

  case 371:
#line 1960 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7245 "ds_parser.cpp"
    break;

  case 372:
#line 1964 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7256 "ds_parser.cpp"
    break;

  case 373:
#line 1970 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7267 "ds_parser.cpp"
    break;

  case 374:
#line 1979 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7273 "ds_parser.cpp"
    break;

  case 375:
#line 1980 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7279 "ds_parser.cpp"
    break;

  case 376:
#line 1981 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7285 "ds_parser.cpp"
    break;

  case 377:
#line 1985 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7291 "ds_parser.cpp"
    break;

  case 378:
#line 1986 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7297 "ds_parser.cpp"
    break;

  case 379:
#line 1990 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7305 "ds_parser.cpp"
    break;

  case 380:
#line 1993 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7315 "ds_parser.cpp"
    break;

  case 381:
#line 1998 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7325 "ds_parser.cpp"
    break;

  case 382:
#line 2003 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7338 "ds_parser.cpp"
    break;

  case 383:
#line 2011 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7351 "ds_parser.cpp"
    break;

  case 384:
#line 2022 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7359 "ds_parser.cpp"
    break;

  case 385:
#line 2025 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7369 "ds_parser.cpp"
    break;

  case 386:
#line 2033 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7375 "ds_parser.cpp"
    break;

  case 387:
#line 2034 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7381 "ds_parser.cpp"
    break;

  case 388:
#line 2038 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7387 "ds_parser.cpp"
    break;

  case 389:
#line 2039 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7393 "ds_parser.cpp"
    break;

  case 390:
#line 2040 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7399 "ds_parser.cpp"
    break;

  case 391:
#line 2044 "ds_parser.ypp"
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
#line 7432 "ds_parser.cpp"
    break;

  case 392:
#line 2072 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7438 "ds_parser.cpp"
    break;

  case 393:
#line 2072 "ds_parser.ypp"
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
#line 7474 "ds_parser.cpp"
    break;

  case 394:
#line 2106 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7482 "ds_parser.cpp"
    break;

  case 395:
#line 2109 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7496 "ds_parser.cpp"
    break;

  case 396:
#line 2118 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7510 "ds_parser.cpp"
    break;

  case 397:
#line 2131 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7524 "ds_parser.cpp"
    break;

  case 401:
#line 2152 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7530 "ds_parser.cpp"
    break;

  case 402:
#line 2153 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7536 "ds_parser.cpp"
    break;

  case 403:
#line 2154 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7542 "ds_parser.cpp"
    break;

  case 404:
#line 2158 "ds_parser.ypp"
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
#line 7575 "ds_parser.cpp"
    break;

  case 405:
#line 2186 "ds_parser.ypp"
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
#line 7609 "ds_parser.cpp"
    break;

  case 406:
#line 2218 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7615 "ds_parser.cpp"
    break;

  case 407:
#line 2219 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7621 "ds_parser.cpp"
    break;

  case 408:
#line 2223 "ds_parser.ypp"
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
#line 7660 "ds_parser.cpp"
    break;

  case 409:
#line 2260 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7666 "ds_parser.cpp"
    break;

  case 410:
#line 2261 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7672 "ds_parser.cpp"
    break;

  case 411:
#line 2265 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7678 "ds_parser.cpp"
    break;

  case 412:
#line 2266 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7684 "ds_parser.cpp"
    break;

  case 413:
#line 2267 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7690 "ds_parser.cpp"
    break;

  case 414:
#line 2272 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7696 "ds_parser.cpp"
    break;

  case 415:
#line 2272 "ds_parser.ypp"
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
#line 7782 "ds_parser.cpp"
    break;

  case 416:
#line 2356 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7794 "ds_parser.cpp"
    break;

  case 417:
#line 2363 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7805 "ds_parser.cpp"
    break;

  case 418:
#line 2372 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7811 "ds_parser.cpp"
    break;

  case 419:
#line 2373 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7817 "ds_parser.cpp"
    break;

  case 420:
#line 2374 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7823 "ds_parser.cpp"
    break;

  case 421:
#line 2375 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7829 "ds_parser.cpp"
    break;

  case 422:
#line 2376 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7835 "ds_parser.cpp"
    break;

  case 423:
#line 2377 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7841 "ds_parser.cpp"
    break;

  case 424:
#line 2378 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7847 "ds_parser.cpp"
    break;

  case 425:
#line 2379 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7853 "ds_parser.cpp"
    break;

  case 426:
#line 2380 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7859 "ds_parser.cpp"
    break;

  case 427:
#line 2381 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7865 "ds_parser.cpp"
    break;

  case 428:
#line 2382 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7871 "ds_parser.cpp"
    break;

  case 429:
#line 2383 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7877 "ds_parser.cpp"
    break;

  case 430:
#line 2384 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7883 "ds_parser.cpp"
    break;

  case 431:
#line 2385 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7889 "ds_parser.cpp"
    break;

  case 432:
#line 2386 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7895 "ds_parser.cpp"
    break;

  case 433:
#line 2387 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7901 "ds_parser.cpp"
    break;

  case 434:
#line 2388 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7907 "ds_parser.cpp"
    break;

  case 435:
#line 2389 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7913 "ds_parser.cpp"
    break;

  case 436:
#line 2390 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7919 "ds_parser.cpp"
    break;

  case 437:
#line 2391 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7925 "ds_parser.cpp"
    break;

  case 438:
#line 2392 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7931 "ds_parser.cpp"
    break;

  case 439:
#line 2393 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7937 "ds_parser.cpp"
    break;

  case 440:
#line 2394 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7943 "ds_parser.cpp"
    break;

  case 441:
#line 2395 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7949 "ds_parser.cpp"
    break;

  case 442:
#line 2396 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7955 "ds_parser.cpp"
    break;

  case 443:
#line 2400 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7961 "ds_parser.cpp"
    break;

  case 444:
#line 2401 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7967 "ds_parser.cpp"
    break;

  case 445:
#line 2402 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7973 "ds_parser.cpp"
    break;

  case 446:
#line 2403 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7979 "ds_parser.cpp"
    break;

  case 447:
#line 2404 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7985 "ds_parser.cpp"
    break;

  case 448:
#line 2405 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7991 "ds_parser.cpp"
    break;

  case 449:
#line 2409 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8004 "ds_parser.cpp"
    break;

  case 450:
#line 2420 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8013 "ds_parser.cpp"
    break;

  case 451:
#line 2424 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8025 "ds_parser.cpp"
    break;

  case 452:
#line 2434 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8037 "ds_parser.cpp"
    break;

  case 453:
#line 2441 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8048 "ds_parser.cpp"
    break;

  case 454:
#line 2450 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8054 "ds_parser.cpp"
    break;

  case 455:
#line 2450 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8060 "ds_parser.cpp"
    break;

  case 456:
#line 2450 "ds_parser.ypp"
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
#line 8075 "ds_parser.cpp"
    break;

  case 457:
#line 2463 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8081 "ds_parser.cpp"
    break;

  case 458:
#line 2464 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8087 "ds_parser.cpp"
    break;

  case 459:
#line 2465 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8093 "ds_parser.cpp"
    break;

  case 460:
#line 2466 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8099 "ds_parser.cpp"
    break;

  case 461:
#line 2467 "ds_parser.ypp"
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
#line 8118 "ds_parser.cpp"
    break;

  case 462:
#line 2481 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8129 "ds_parser.cpp"
    break;

  case 463:
#line 2487 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8138 "ds_parser.cpp"
    break;

  case 464:
#line 2491 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8147 "ds_parser.cpp"
    break;

  case 465:
#line 2495 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8157 "ds_parser.cpp"
    break;

  case 466:
#line 2500 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8167 "ds_parser.cpp"
    break;

  case 467:
#line 2505 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8177 "ds_parser.cpp"
    break;

  case 468:
#line 2510 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8187 "ds_parser.cpp"
    break;

  case 469:
#line 2515 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8196 "ds_parser.cpp"
    break;

  case 470:
#line 2519 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8205 "ds_parser.cpp"
    break;

  case 471:
#line 2523 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8215 "ds_parser.cpp"
    break;

  case 472:
#line 2528 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8224 "ds_parser.cpp"
    break;

  case 473:
#line 2532 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8234 "ds_parser.cpp"
    break;

  case 474:
#line 2537 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8240 "ds_parser.cpp"
    break;

  case 475:
#line 2537 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8246 "ds_parser.cpp"
    break;

  case 476:
#line 2537 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8257 "ds_parser.cpp"
    break;

  case 477:
#line 2543 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8269 "ds_parser.cpp"
    break;

  case 478:
#line 2550 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8275 "ds_parser.cpp"
    break;

  case 479:
#line 2550 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8281 "ds_parser.cpp"
    break;

  case 480:
#line 2550 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8291 "ds_parser.cpp"
    break;

  case 481:
#line 2555 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8297 "ds_parser.cpp"
    break;

  case 482:
#line 2555 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8303 "ds_parser.cpp"
    break;

  case 483:
#line 2555 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8314 "ds_parser.cpp"
    break;

  case 484:
#line 2561 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8320 "ds_parser.cpp"
    break;

  case 485:
#line 2561 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8326 "ds_parser.cpp"
    break;

  case 486:
#line 2561 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8336 "ds_parser.cpp"
    break;

  case 487:
#line 2566 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8345 "ds_parser.cpp"
    break;

  case 488:
#line 2570 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8351 "ds_parser.cpp"
    break;

  case 489:
#line 2570 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8357 "ds_parser.cpp"
    break;

  case 490:
#line 2570 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8367 "ds_parser.cpp"
    break;

  case 491:
#line 2575 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8373 "ds_parser.cpp"
    break;

  case 492:
#line 2575 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8379 "ds_parser.cpp"
    break;

  case 493:
#line 2575 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8393 "ds_parser.cpp"
    break;

  case 494:
#line 2584 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8402 "ds_parser.cpp"
    break;

  case 495:
#line 2588 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8408 "ds_parser.cpp"
    break;

  case 496:
#line 2588 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8414 "ds_parser.cpp"
    break;

  case 497:
#line 2588 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8424 "ds_parser.cpp"
    break;

  case 498:
#line 2593 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8430 "ds_parser.cpp"
    break;

  case 499:
#line 2593 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8436 "ds_parser.cpp"
    break;

  case 500:
#line 2593 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8450 "ds_parser.cpp"
    break;

  case 501:
#line 2602 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8459 "ds_parser.cpp"
    break;

  case 502:
#line 2606 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8465 "ds_parser.cpp"
    break;

  case 503:
#line 2606 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8471 "ds_parser.cpp"
    break;

  case 504:
#line 2606 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8481 "ds_parser.cpp"
    break;

  case 505:
#line 2611 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8487 "ds_parser.cpp"
    break;

  case 506:
#line 2611 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8493 "ds_parser.cpp"
    break;

  case 507:
#line 2611 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8507 "ds_parser.cpp"
    break;

  case 508:
#line 2620 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8513 "ds_parser.cpp"
    break;

  case 509:
#line 2620 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8519 "ds_parser.cpp"
    break;

  case 510:
#line 2620 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8530 "ds_parser.cpp"
    break;

  case 511:
#line 2626 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8536 "ds_parser.cpp"
    break;

  case 512:
#line 2626 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8542 "ds_parser.cpp"
    break;

  case 513:
#line 2626 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8553 "ds_parser.cpp"
    break;

  case 514:
#line 2635 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8559 "ds_parser.cpp"
    break;

  case 515:
#line 2635 "ds_parser.ypp"
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
#line 8576 "ds_parser.cpp"
    break;

  case 516:
#line 2650 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8582 "ds_parser.cpp"
    break;

  case 517:
#line 2650 "ds_parser.ypp"
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
#line 8603 "ds_parser.cpp"
    break;

  case 518:
#line 2670 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8609 "ds_parser.cpp"
    break;

  case 519:
#line 2671 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8615 "ds_parser.cpp"
    break;

  case 520:
#line 2672 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8621 "ds_parser.cpp"
    break;

  case 521:
#line 2673 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8627 "ds_parser.cpp"
    break;

  case 522:
#line 2677 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8639 "ds_parser.cpp"
    break;

  case 523:
#line 2684 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8651 "ds_parser.cpp"
    break;

  case 524:
#line 2691 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8662 "ds_parser.cpp"
    break;

  case 525:
#line 2697 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8673 "ds_parser.cpp"
    break;

  case 526:
#line 2706 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8683 "ds_parser.cpp"
    break;

  case 527:
#line 2711 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8692 "ds_parser.cpp"
    break;

  case 528:
#line 2718 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8698 "ds_parser.cpp"
    break;

  case 529:
#line 2719 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8704 "ds_parser.cpp"
    break;

  case 530:
#line 2723 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8715 "ds_parser.cpp"
    break;

  case 531:
#line 2729 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8727 "ds_parser.cpp"
    break;

  case 532:
#line 2736 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8740 "ds_parser.cpp"
    break;

  case 533:
#line 2744 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8752 "ds_parser.cpp"
    break;

  case 534:
#line 2751 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8765 "ds_parser.cpp"
    break;

  case 535:
#line 2759 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8779 "ds_parser.cpp"
    break;

  case 536:
#line 2771 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8787 "ds_parser.cpp"
    break;

  case 537:
#line 2774 "ds_parser.ypp"
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
#line 8803 "ds_parser.cpp"
    break;

  case 538:
#line 2788 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8814 "ds_parser.cpp"
    break;

  case 539:
#line 2797 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8820 "ds_parser.cpp"
    break;

  case 540:
#line 2798 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8826 "ds_parser.cpp"
    break;

  case 541:
#line 2802 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8836 "ds_parser.cpp"
    break;

  case 542:
#line 2807 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8845 "ds_parser.cpp"
    break;

  case 543:
#line 2814 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8855 "ds_parser.cpp"
    break;

  case 544:
#line 2819 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8867 "ds_parser.cpp"
    break;

  case 545:
#line 2829 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8877 "ds_parser.cpp"
    break;

  case 546:
#line 2834 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8886 "ds_parser.cpp"
    break;

  case 547:
#line 2841 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8900 "ds_parser.cpp"
    break;

  case 548:
#line 2853 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8906 "ds_parser.cpp"
    break;

  case 549:
#line 2854 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8912 "ds_parser.cpp"
    break;

  case 550:
#line 2858 "ds_parser.ypp"
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
#line 8935 "ds_parser.cpp"
    break;

  case 551:
#line 2876 "ds_parser.ypp"
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
#line 8958 "ds_parser.cpp"
    break;


#line 8962 "ds_parser.cpp"

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
#line 2896 "ds_parser.ypp"


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
