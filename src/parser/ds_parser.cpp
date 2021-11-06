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
#line 63 "ds_parser.ypp"

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

#line 210 "ds_parser.cpp"

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
#line 95 "ds_parser.ypp"

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

#line 410 "ds_parser.cpp"

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
#define YYLAST   7516

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  181
/* YYNRULES -- Number of rules.  */
#define YYNRULES  567
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  990

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
       0,   456,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   472,   473,   474,   478,   479,
     483,   500,   501,   502,   503,   507,   511,   516,   525,   533,
     549,   554,   562,   562,   592,   614,   618,   621,   625,   631,
     640,   661,   684,   685,   689,   693,   694,   698,   701,   707,
     713,   716,   722,   723,   727,   728,   729,   738,   739,   743,
     752,   768,   776,   786,   795,   796,   797,   798,   799,   800,
     804,   809,   817,   818,   819,   823,   824,   825,   826,   827,
     828,   829,   830,   836,   850,   867,   868,   869,   873,   881,
     894,   898,   905,   906,   910,   911,   912,   916,   919,   926,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   956,   957,   958,   959,
     960,   961,   962,   963,   964,   965,   966,   967,   968,   969,
     970,   971,   972,   973,   974,   975,   976,   977,   981,  1003,
    1004,  1005,  1009,  1047,  1055,  1059,  1070,  1071,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,
    1084,  1085,  1086,  1087,  1088,  1092,  1097,  1103,  1109,  1143,
    1146,  1152,  1153,  1164,  1168,  1174,  1177,  1180,  1184,  1190,
    1194,  1198,  1201,  1204,  1209,  1212,  1220,  1223,  1228,  1231,
    1239,  1245,  1246,  1250,  1284,  1284,  1284,  1287,  1287,  1287,
    1292,  1292,  1292,  1300,  1300,  1300,  1306,  1316,  1327,  1342,
    1345,  1351,  1352,  1359,  1370,  1371,  1372,  1376,  1377,  1378,
    1379,  1383,  1388,  1396,  1397,  1401,  1406,  1470,  1471,  1472,
    1473,  1474,  1475,  1479,  1480,  1481,  1482,  1483,  1484,  1485,
    1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,
    1496,  1497,  1501,  1502,  1503,  1504,  1508,  1519,  1524,  1534,
    1538,  1538,  1538,  1545,  1545,  1545,  1559,  1563,  1567,  1567,
    1567,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,
    1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,  1591,  1592,
    1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,
    1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,  1612,
    1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,  1624,  1628,
    1631,  1634,  1635,  1636,  1637,  1646,  1655,  1656,  1659,  1659,
    1659,  1662,  1666,  1670,  1674,  1675,  1676,  1677,  1678,  1679,
    1680,  1681,  1697,  1713,  1771,  1772,  1776,  1777,  1781,  1782,
    1786,  1787,  1788,  1792,  1801,  1804,  1809,  1808,  1854,  1919,
    1928,  1933,  1941,  1942,  1946,  1949,  1958,  1959,  1963,  1972,
    1973,  1978,  1979,  1983,  1989,  1995,  1998,  2002,  2008,  2017,
    2018,  2019,  2023,  2024,  2028,  2031,  2036,  2041,  2049,  2060,
    2063,  2071,  2072,  2076,  2077,  2078,  2082,  2110,  2110,  2144,
    2147,  2156,  2169,  2181,  2182,  2186,  2187,  2191,  2192,  2193,
    2197,  2225,  2257,  2258,  2262,  2300,  2301,  2305,  2306,  2307,
    2312,  2311,  2402,  2409,  2418,  2419,  2420,  2421,  2422,  2423,
    2424,  2425,  2426,  2427,  2428,  2429,  2430,  2431,  2432,  2433,
    2434,  2435,  2436,  2437,  2438,  2439,  2440,  2441,  2442,  2446,
    2447,  2448,  2449,  2450,  2451,  2455,  2466,  2470,  2480,  2487,
    2496,  2496,  2496,  2509,  2510,  2511,  2512,  2513,  2527,  2533,
    2537,  2541,  2546,  2551,  2556,  2561,  2565,  2569,  2574,  2578,
    2583,  2583,  2583,  2589,  2596,  2596,  2596,  2601,  2601,  2601,
    2607,  2607,  2607,  2612,  2616,  2616,  2616,  2621,  2621,  2621,
    2630,  2634,  2634,  2634,  2639,  2639,  2639,  2648,  2652,  2652,
    2652,  2657,  2657,  2657,  2666,  2666,  2666,  2672,  2672,  2672,
    2681,  2681,  2696,  2696,  2716,  2717,  2718,  2719,  2723,  2730,
    2737,  2743,  2752,  2757,  2764,  2765,  2769,  2775,  2782,  2790,
    2797,  2805,  2817,  2820,  2834,  2843,  2844,  2848,  2853,  2860,
    2865,  2875,  2880,  2887,  2899,  2900,  2904,  2922
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
     405,   406,   407,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   408,   409,   126,
      33,   410,   411,   412,   413,   414,    46,    91,    93,    40,
      41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -554

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-554)))

#define YYTABLE_NINF -522

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -554,    36,  -554,  -554,    28,   -65,    95,   -21,  -554,   -48,
    -554,  -554,    57,  -554,  -554,  -554,  -554,  -554,   228,  -554,
      84,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
     -16,  -554,    35,    49,    88,  -554,  -554,    95,    -1,  -554,
     120,    26,  -554,  -554,  -554,    84,   136,   149,  -554,  -554,
     177,   197,   162,  -554,   118,  -554,  -554,  -554,   219,   313,
     318,  -554,   332,    15,    28,   236,   -65,   230,   191,  -554,
     277,   283,  -554,  7285,   247,  -101,   348,   248,   249,  -554,
     289,    28,    57,  -554,  -554,  -554,    14,  -554,  -554,  -554,
     290,  -554,  -554,   292,  -554,  -554,   252,  -554,  -554,  -554,
    -554,  -554,   241,    78,  -554,  -554,  -554,  -554,   385,  -554,
    -554,   279,   280,   286,   287,  -554,  -554,  -554,   273,  -554,
    -554,  -554,  -554,  -554,   298,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,   299,  -554,  -554,  -554,   304,   305,
    -554,  -554,  -554,  -554,   306,   307,  -554,  -554,  -554,  -554,
    -554,  3792,  -554,  -554,   285,  -554,  -554,  -554,   324,   325,
    -554,    86,  -554,  6987,   442,   443,  -554,   308,   294,   102,
     329,  -554,  -554,   350,  -554,  -554,   271,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,   104,  -554,  -554,  -554,  -554,  -554,
     343,  -554,   167,   204,   207,  -554,  -554,  -554,  -554,  -554,
    -554,   457,  -554,  -554,    13,  2985,  -554,  -554,  -554,   309,
     334,  -554,   322,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,   328,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,   330,
    -554,  -554,   -95,   335,  -554,  -554,   291,  -554,   -66,  -554,
     323,   -81,    28,   364,  -554,  -554,  -554,    78,  -554,  7285,
    7285,  7285,  7285,   331,   324,  7285,   308,  7285,   308,  7285,
     308,  7371,   325,  -554,  -554,  -554,   339,  -554,  -554,  -554,
    7199,   333,   353,  -554,   340,   358,   365,   345,   366,  -554,
     372,  4622,  4622,  6971,  7113,  4622,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  4622,  4622,  4622,   -66,  4622,  4622,  -554,
    4622,  -554,   346,  -554,  -554,  -554,   -69,  -554,  -554,  -554,
    -554,   357,  -554,  -554,  -554,  -554,  -554,  -554,  5759,   356,
    -554,  -554,  -554,  -554,  -554,   -89,  7285,   -87,  -554,  -554,
    -554,   -20,  -554,   210,  7285,  -554,  1297,  -554,  -554,  -554,
    -554,  -554,  -554,   326,   -79,  -554,  -554,  -554,   364,   137,
    -554,  -554,   214,  -554,   368,   984,   692,  1582,  -554,   -88,
    1893,   335,  2054,   335,  2212,   335,   126,  -554,    42,  3792,
     160,  -554,  2348,  -554,   -66,  -554,  4622,  -554,  -554,  4622,
    -554,  7285,   554,   554,   364,  1642,   364,  1799,  6319,  -554,
    -121,   264,   554,   554,    50,  -554,   554,   554,  5207,    99,
    -554,  2411,   400,    17,   392,  4622,  4622,  -554,  -554,  4622,
    4622,  4622,  4622,   394,  4622,   399,  4622,  4622,  4622,  4622,
    4622,  4622,  4622,  4622,  3124,  4622,  4622,  4622,  4622,  4622,
    4622,  4622,  4622,  4622,  4622,   -30,  4622,  -554,  3263,  -554,
    -554,  3792,  -554,  -554,   364,  -554,   -54,  -554,   350,  3792,
    -554,  4622,  -554,  -554,   364,  2113,  -554,   294,  4622,  4622,
    -554,   369,   408,  3420,   294,  2272,   221,   221,  -554,   512,
     370,   373,  4622,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
     374,   375,   376,  -554,  -554,  -554,   364,  -554,   -82,  -554,
    6251,  -554,    97,  -554,    12,  -554,   371,   406,  7285,  -554,
     -43,  -554,  7285,  -554,  -554,  -554,   410,  -554,   401,  -554,
     403,  -554,   404,  7285,  -554,  7371,  -554,   325,  3559,  3716,
    7285,  5285,  7285,  7285,  5365,  7285,  2505,     3,   221,   -46,
    1956,  3855,  6319,   407,   -15,   388,   414,  -554,  -554,   -38,
      30,  4012,   -15,   168,  4622,  4622,   387,  -554,  4622,   268,
     422,  -554,   208,  -554,   436,  -554,   107,  6545,    77,   308,
     427,  -554,  -554,   232,   232,   987,   987,  6904,  6904,   413,
     132,  -554,  5835,   -63,   -63,   232,   232,  6720,  6613,  6692,
     448,  6398,   338,  6798,  6826,   987,   987,   593,   593,   132,
     132,   132,   449,  4622,  -554,   451,  4622,   599,  5911,  -554,
     109,  -554,  -554,  -554,  7285,  -554,  2568,  -554,   473,    32,
    2568,  -554,  -554,   482,  1113,   589,  6545,   473,  -554,   450,
     468,  6545,  -554,  2568,  -554,  4712,   426,  -554,  -554,   425,
    -554,  -554,   473,  -554,  -554,  -554,  -554,   221,  -554,  4622,
    4622,  4622,  4622,  4622,  4622,  4622,  4622,  4622,  4622,  2707,
    4622,  4622,  4622,  4622,  4622,  4622,  2846,   -67,  4622,  -554,
      57,  -554,   603,   569,   429,  -554,  -554,   567,  -554,  -554,
    -554,  2568,  -554,  2644,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  3792,  -554,  -554,  -554,  -554,   111,   470,
    5443,  2783,  -554,  2922,  3061,  -554,  3200,   400,  4622,  -554,
    4622,  4622,    -6,   471,   436,   439,   441,  4622,   447,  4622,
    4622,   436,   435,   462,  6545,  -554,  -554,  6466,  -554,   474,
    7285,   308,   -42,   151,  4622,  -554,   481,   484,   496,   497,
    -554,   157,   335,  -554,  4169,  -554,  -554,  4622,  -554,  5987,
    -554,  6063,  -554,  -554,  -554,  3361,  -554,  6151,  -554,  4622,
    -554,  4811,  4622,  4622,  -554,   294,  -554,  -554,  -554,  -554,
    4910,  -554,  -554,  -554,   302,  -554,  6545,  6545,  6545,  6545,
    6545,  6545,  6545,  6545,  6545,  6545,  4622,  4622,  6545,  6545,
    6545,  6545,  6545,  6545,  6545,  4622,  4622,  6545,  -554,   770,
     164,   351,  -554,   351,  -554,   457,  -554,  2568,  -554,  5009,
    -554,  -554,  -554,  -554,  -554,   172,  -554,  -554,  -554,  -554,
    -554,   464,     4,  6545,  6545,   -15,   466,    92,   407,   467,
    -554,  6545,  -554,  -554,    39,   -15,   469,  -554,  -554,  -554,
    3496,   335,   475,  6545,  -554,  -554,  -554,  -554,    77,   476,
     -90,  7285,  -554,   112,  6545,  -554,  -554,  -554,   457,  4622,
      51,  -554,  -554,  -554,  1482,   294,  -554,  -554,  4622,  -554,
      19,  6545,    19,  6545,  -554,   444,  -554,  -554,   384,   364,
    -554,  5108,  -554,  -554,  4622,   501,  -554,  4622,  4622,  4622,
    4326,  4622,   478,   480,  4622,  4622,  -554,  4622,   463,  -554,
    -554,   490,  -554,  -554,  -554,  4483,  -554,  -554,  3653,  -554,
    6545,   294,  -554,  -554,   473,  -554,  -554,   621,  -554,  -554,
    5523,   500,   987,   987,   987,  -554,  5601,   964,   485,  -554,
    6545,  6545,   964,   486,   -66,  -554,  4622,  6545,  -554,  -554,
     302,    14,  -554,    14,  -554,  4622,  -554,   613,   487,  -554,
     483,  -554,  -554,   -66,  6545,  -554,  -554,   491,   294,  5681,
    4622,   488,   495,  -554,  -554,  -554,  -554,  6545,  -554,  -554
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   201,     0,     0,     0,     0,   202,     0,
     532,   530,     0,    14,     3,    10,     9,     8,     0,     7,
     401,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   416,    19,    18,   401,     0,     0,    86,    87,
       0,   181,    88,    90,     0,    85,   426,   425,   149,   417,
     427,   402,   403,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   183,
       0,     0,     0,    93,   151,   150,     0,   148,   419,   418,
       0,   429,   428,     0,   405,   404,   407,    79,    80,    77,
      78,    76,     0,     0,    75,    84,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   434,   454,   435,   466,   436,
     440,   441,   442,   443,   458,   447,   448,   449,   450,   451,
     452,   453,   455,   456,   503,   439,   446,   457,   510,   517,
     437,   444,   438,   445,     0,     0,   465,   473,   476,   474,
     475,   412,   413,   415,     0,    16,    17,    20,     0,     0,
     182,     0,    91,     0,     0,     0,    99,    94,     0,     0,
     422,   430,   399,   354,    21,    22,     0,    68,    69,    66,
      67,    65,    64,    70,     0,    41,   494,   497,   500,   490,
       0,   470,   504,   511,   518,   524,   527,   481,   486,   480,
     493,     0,   489,   483,     0,     0,   485,   414,   468,     0,
       0,   379,     0,    89,   147,   102,   103,   105,   104,   106,
     107,   108,   109,   134,   135,   132,   133,   125,   136,   137,
     126,   123,   124,   145,     0,   146,   138,   139,   140,   141,
     113,   114,   115,   110,   111,   112,   128,   129,   127,   121,
     122,   117,   116,   118,   119,   120,   101,   100,   144,     0,
     130,   131,   354,    97,   175,   153,     0,   409,     0,   424,
       0,   354,     0,     0,    23,    24,    25,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    94,     0,    94,     0,
      94,     0,     0,   488,   482,   484,     0,   487,   287,   288,
       0,     0,     0,   281,     0,     0,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,   237,   239,   238,   240,
     241,   242,    26,     0,     0,     0,     0,     0,     0,   478,
       0,   224,   225,   285,   284,   235,   282,   347,   346,   345,
     344,    92,   350,   283,   349,   348,   326,   289,     0,     0,
     286,   534,   535,   536,   537,     0,     0,     0,   143,   142,
      95,     0,   372,     0,     0,   152,     0,   459,   462,   460,
     463,   461,   464,     0,     0,   423,   364,   406,     0,     0,
     432,   408,   392,    71,     0,     0,     0,     0,   467,     0,
       0,    97,     0,    97,     0,    97,   181,   376,     0,   374,
       0,   479,   185,   188,     0,   213,     0,   204,   207,     0,
     210,     0,   315,   316,     0,   544,     0,     0,     0,   561,
       0,     0,   293,   292,   331,    32,   291,   290,     0,   226,
     353,     0,   233,     0,     0,     0,     0,   317,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   278,     0,   477,     0,   469,
     533,   378,   531,   380,     0,   370,   383,    96,   354,    98,
     177,     0,    57,    58,     0,     0,   189,     0,     0,     0,
     190,     0,     0,     0,     0,     0,     0,     0,   156,   154,
       0,     0,     0,   170,   165,   163,   162,   164,   176,   157,
       0,     0,     0,   168,   169,   171,     0,   161,     0,   158,
     243,   409,     0,   420,   354,   400,     0,     0,     0,   393,
       0,   495,     0,   501,   491,   471,     0,   505,     0,   512,
       0,   519,     0,     0,   525,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,     0,   552,   542,   544,     0,   555,   556,   557,     0,
       0,     0,   544,     0,     0,     0,     0,    29,     0,    27,
       0,   319,   270,   269,     0,   327,     0,   219,     0,    94,
       0,   341,   342,   294,   295,   307,   308,   305,   306,     0,
     336,   324,     0,   351,   352,   296,   297,   312,   313,   314,
       0,     0,   310,   311,   309,   303,   304,   299,   298,   300,
     301,   302,     0,     0,   276,     0,     0,     0,     0,   329,
       0,   371,   382,   381,     0,   384,     0,   373,     0,     0,
       0,   191,   194,     0,   243,     0,   184,     0,   174,     0,
       0,    51,    61,     0,   198,   243,   225,   179,   180,     0,
     172,   173,     0,   160,   166,   167,   203,     0,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   410,
       0,   431,     0,   356,     0,   355,   433,     0,   390,   391,
     389,     0,   496,     0,   502,   492,   472,   506,   508,   513,
     515,   520,   522,   375,   526,   377,   529,   186,     0,     0,
       0,     0,   332,     0,     0,   333,     0,   233,     0,   545,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   554,   562,   563,     0,    30,    33,
       0,    94,     0,     0,     0,   328,     0,     0,     0,     0,
     231,     0,    97,   338,     0,   322,   343,     0,   325,     0,
     277,     0,   279,   320,   330,   385,   388,   387,    62,     0,
     195,   243,     0,     0,   192,     0,    63,    49,    50,   199,
     243,   196,   226,   175,    54,   178,   253,   254,   256,   255,
     257,   247,   248,   249,   258,   259,     0,     0,   245,   246,
     260,   261,   250,   251,   252,     0,     0,   244,   421,     0,
       0,   360,   357,   360,   365,   389,   394,     0,   398,   243,
     498,   509,   516,   523,   187,     0,   216,   214,   205,   208,
     211,     0,     0,   539,   538,   544,     0,     0,   543,     0,
     547,   553,   559,   558,     0,   544,     0,   560,    28,    31,
       0,    97,     0,   220,   229,   230,   228,   227,     0,     0,
       0,     0,   267,     0,   337,   323,   321,   280,   381,     0,
       0,   193,   200,   197,     0,     0,    52,    53,     0,    59,
     264,   265,   262,   263,   411,    93,   361,   362,   356,     0,
     396,   243,   397,   499,     0,     0,   215,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,     0,   550,
     271,     0,   266,   232,   234,     0,   221,   236,     0,   268,
     386,     0,   155,    55,     0,   369,   366,   358,   363,   395,
       0,     0,   206,   209,   212,   334,     0,   564,     0,   548,
     541,   540,   564,     0,     0,   274,     0,   222,   339,    60,
      54,     0,   359,     0,   217,     0,   335,     0,     0,   549,
       0,   551,   272,     0,   223,   340,    56,     0,     0,     0,
       0,     0,     0,   275,   367,   368,   218,   565,   566,   567
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -554,  -554,  -554,  -554,  -554,   254,   606,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,   640,  -554,   572,  -554,  -554,   617,
    -554,  -554,  -554,  -276,  -554,  -554,  -554,  -554,  -554,  -554,
     409,  -554,  -554,   623,   -60,  -554,   609,    -5,  -323,  -278,
    -386,  -554,  -554,  -554,  -540,  -554,  -167,  -554,  -100,  -471,
     -12,  -554,  -554,  -554,  -554,  -554,  -554,  -554,   693,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -445,  -554,  -554,  -173,  -554,   -31,  -469,  -554,  -363,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,   377,  -554,  -554,  -162,  -201,  -554,  -111,  -554,  -554,
    -554,   235,  -554,   169,  -554,  -340,   424,  -463,  -468,    20,
    -554,  -346,  -554,   674,  -554,  -554,  -554,   199,    80,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -357,
     -71,  -554,  -554,  -554,   452,  -554,  -554,  -554,    18,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,   423,  -553,  -402,
    -539,  -554,  -554,  -299,   -14,   310,  -554,  -554,  -554,  -224,
    -554
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   157,    45,    14,   176,   182,   421,   333,   749,
     334,   580,    15,    16,    38,    39,    72,    17,    34,    35,
     500,   501,   888,   889,   502,   503,   504,   505,   506,   507,
     183,   184,    30,    31,    32,    52,    53,    54,    18,   263,
     365,   167,    19,    86,   168,    87,   335,   508,   366,   509,
     336,   510,   337,   511,   512,   513,   514,   515,   516,   517,
     338,   552,   907,   553,   908,   555,   909,   339,   550,   906,
     340,   586,   927,   341,   760,   761,   589,   342,   343,   643,
     519,   344,   345,   346,   750,   954,   751,   973,   347,   627,
     877,   587,   871,   975,   361,   823,   963,   898,   694,   524,
     961,   362,   363,   397,   398,   211,   212,   731,   475,   701,
     530,   381,   271,    62,    96,    21,   173,   374,    42,    75,
      22,    90,    23,   269,   171,    60,    93,    24,   270,   382,
     349,   373,   148,   149,   209,   150,   284,   706,   399,   282,
     705,   279,   702,   280,   903,   281,   704,   285,   707,   286,
     831,   287,   709,   288,   832,   289,   711,   290,   833,   291,
     714,   292,   716,    25,    47,    26,    46,   350,   563,   564,
     565,   351,   566,   567,   568,   569,   352,   420,   353,   968,
     354
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   265,   147,   518,   476,   538,   631,   540,   391,   542,
     393,   273,   395,   636,   642,   572,   419,   473,   432,   -92,
      68,   161,   728,   630,   654,   735,   474,   657,   658,    97,
      98,   753,   525,   742,   558,   590,     2,   667,   925,   433,
     434,   272,     3,   558,    40,   294,    27,    28,   163,   740,
     443,   779,    69,   445,   446,   272,   469,   557,   210,   570,
     632,   146,   575,    50,   576,     4,   522,     5,   632,     6,
      55,   698,   632,   535,    33,     7,   430,   730,   522,    51,
     699,   730,     8,    80,   153,   360,   622,   623,     9,   729,
      48,   151,   177,   178,   264,   536,   470,    43,   472,   527,
     633,   668,   634,   718,   377,   635,   523,    10,   633,   378,
     431,   700,   633,   465,   466,   624,    49,   476,   818,   164,
     165,    74,   435,   436,    40,   380,   439,   639,    11,   440,
     441,   442,   443,    44,   444,   445,   446,   447,    63,   752,
     738,   448,    61,   449,   450,   739,   625,   626,   272,   451,
     452,   453,   433,   434,    99,   154,   527,   754,   100,   166,
     101,   102,   591,    41,    70,   776,   445,   446,   734,   780,
     666,    40,   295,    29,   454,    71,   455,   456,   457,   458,
     459,   848,   789,   527,   463,   527,    50,   911,    64,   690,
     296,   756,   754,   846,   103,   465,   466,   691,   795,   297,
     757,   692,    51,   544,   754,    65,   632,   473,   147,   147,
     147,   147,   379,    12,   147,   914,   147,   179,   147,    13,
     147,   180,   917,   181,   102,   545,   465,   466,    50,   147,
     828,   758,    56,    57,   931,    58,   759,   437,   438,    64,
      36,    66,   147,   147,    51,   443,   633,   444,   445,   446,
     447,   688,   433,   434,   448,    80,   375,   277,   266,   582,
     754,    37,   754,    59,   754,   754,   213,   146,   146,   146,
     146,    82,    84,   146,    73,   146,   745,   146,    85,   146,
     689,    77,   543,   842,   278,   147,   267,   755,   146,   774,
      64,   834,   929,   147,    78,   854,    83,   384,   385,   386,
     387,   146,   146,   390,   733,   392,   912,   394,   465,   466,
     868,   762,   879,   885,   425,   526,   918,    82,   402,   873,
     645,   546,    79,  -507,   886,   887,    80,   652,  -507,   862,
     845,   415,   417,   904,   880,   869,   108,   437,   438,   855,
     147,    81,   895,   547,   146,   443,  -507,   444,   445,   446,
     447,   739,   146,   743,   448,   905,   900,   367,   433,   434,
    -514,   368,   693,  -521,  -273,  -514,    88,   527,  -521,  -273,
     528,    91,    89,   529,   471,   106,   870,    92,   369,   370,
     371,   372,   479,  -514,   915,    94,  -521,  -273,   174,   175,
     477,    95,   549,   478,   460,   461,   462,   463,   464,   146,
     197,   155,   331,   656,   896,   264,    71,   156,   465,   466,
     897,   174,   175,   577,   578,   274,   275,   583,   274,   275,
     276,   977,   109,   978,   198,   199,   822,   936,   110,   556,
     152,   938,   158,   159,   160,   169,   172,   170,    69,   186,
     187,   435,   436,   437,   438,   439,   188,   189,   440,   441,
     442,   443,   190,   444,   445,   446,   447,   147,   191,   192,
     448,   147,   449,   450,   193,   194,   195,   196,   207,   208,
     210,   778,   147,   861,   147,   921,   260,   261,   264,   147,
     786,   147,   147,   200,   147,   268,   272,   262,   283,   293,
     356,   364,   355,   433,   434,   794,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   357,   358,   376,   359,   380,
     521,   388,   404,   405,   465,   466,   146,   401,   407,   406,
     146,   518,   201,   202,   409,   408,   410,   203,   429,   531,
     204,   146,   411,   146,    12,   468,   588,   592,   146,   599,
     146,   146,   476,   146,   601,   205,   697,   649,   659,   695,
     703,   696,   648,   660,   206,   469,   661,   663,   664,   665,
     733,   713,   708,   147,   710,   712,   736,   737,   721,   748,
     723,   724,   746,   726,   433,   434,   435,   436,   437,   438,
     439,   752,   348,   440,   441,   442,   443,   763,   444,   445,
     446,   447,   764,   766,   768,   448,   770,   449,   450,   197,
     772,   667,   785,   451,   452,   453,   787,   788,   792,   793,
     821,   822,   824,   433,   434,   835,   847,   849,   882,   850,
     856,   859,   146,   198,   199,   852,   864,   935,   454,   865,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     857,   866,   867,   910,   913,   916,   941,   919,   953,   465,
     466,   955,   775,   962,   924,   922,   948,   264,   949,   437,
     438,   965,   980,   969,   971,   981,   988,   443,   982,   104,
     445,   446,   447,   989,   984,   579,   448,    67,    55,   147,
     185,   698,   200,   107,   976,   820,   383,   105,   412,   413,
     699,   162,   418,   884,    20,   923,   841,   937,   437,   438,
     422,   423,   424,   926,   426,   427,   443,   428,   444,   445,
     446,   447,   899,   637,   715,   448,   400,   827,   933,    76,
     687,   825,   202,   403,   197,   853,   203,   573,   970,   204,
     465,   466,     0,     0,     0,     0,   389,     0,   146,     0,
       0,     0,     0,   520,   205,     0,     0,     0,   198,   199,
     826,     0,     0,   206,     0,     0,     0,   462,   463,   464,
       0,     0,     0,     0,   959,     0,     0,   960,   860,   465,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,     0,     0,   554,     0,     0,     0,
     433,   434,   562,     0,   562,     0,     0,     0,     0,     0,
     147,     0,     0,     0,     0,     0,     0,   200,     0,     0,
       0,   985,   593,   594,     0,     0,   595,   596,   597,   598,
       0,   600,     0,   602,   603,   604,   605,   606,   607,   608,
     609,   611,   612,   613,   614,   615,   616,   617,   618,   619,
     620,   621,     0,   628,     0,     0,   201,   202,     0,     0,
       0,   203,     0,   533,   204,     0,     0,     0,   638,   146,
       0,     0,   644,     0,     0,   646,   647,     0,     0,   205,
     651,     0,   655,   435,   436,   437,   438,   439,   206,   662,
     440,   441,   442,   443,     0,   444,   445,   446,   447,   928,
       0,     0,   448,     0,   449,   450,     0,     0,     0,     0,
     451,   452,   453,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   454,   720,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   422,   428,     0,
       0,     0,   972,     0,     0,     0,   465,   466,   428,     0,
       0,   744,   418,   894,     0,   747,     0,     0,     0,     0,
       0,   983,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     769,     0,     0,   771,     0,     0,     0,   433,   434,     0,
       0,     0,     0,   777,     0,     0,   197,   781,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     790,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     198,   199,     0,     0,     0,     0,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   808,   809,   810,   811,
     812,   813,   814,   817,     0,   819,     0,   435,   436,   437,
     438,   439,     0,     0,   440,   441,   442,   443,   829,   444,
     445,   446,   447,     0,     0,     0,   448,     0,   449,   450,
     435,   436,   437,   438,   451,   452,   453,     0,     0,   200,
     443,     0,   444,   445,   446,   447,     0,   843,   844,   448,
       0,   449,   450,     0,   851,     0,   562,     0,     0,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   863,     0,   433,   434,     0,     0,     0,   201,   202,
     465,   466,     0,   203,   874,     0,   204,   967,     0,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,   808,
     817,   205,     0,   465,   466,     0,     0,   532,     0,     0,
     206,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   890,   891,     0,     0,     0,     0,     0,
       0,     0,   892,   893,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   901,     0,     0,     0,   669,   670,
     671,   672,   673,   674,   675,   676,   435,   436,   437,   438,
     439,   677,   678,   440,   441,   442,   443,   782,   444,   445,
     446,   447,     0,     0,     0,   448,   680,   449,   450,   681,
     682,     0,     0,   451,   452,   453,   683,   684,   685,     0,
       0,     0,     0,     0,     0,     0,   930,     0,     0,     0,
       0,   520,     0,     0,     0,   934,     0,   783,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,   940,     0,     0,   942,   943,   944,   946,   947,   465,
     466,   950,   951,     0,   952,     0,   784,     0,   480,     0,
       0,     0,   957,     3,     0,   481,   482,   483,     0,   484,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
       0,     0,   485,   303,   486,   487,     0,     0,     0,     0,
       0,     0,     0,   974,   488,   304,     0,   489,   305,     0,
       0,   306,   979,     8,   307,   490,     0,   491,   308,     0,
       0,   492,   493,     0,     0,     0,     0,   987,   494,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   309,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     310,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,   330,     0,   331,   332,
     498,   264,   499,   480,     0,     0,     0,     0,     3,     0,
     481,   482,   483,     0,   484,     0,   298,   299,   300,   301,
     302,     0,     0,     0,     0,     0,     0,   485,   303,   486,
     487,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     304,     0,   489,   305,     0,     0,   306,     0,     8,   307,
     490,     0,   491,   308,     0,     0,   492,   493,     0,     0,
       0,     0,     0,   494,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   310,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,   197,     0,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    51,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   198,   199,
       0,     0,     0,     0,   323,   324,   325,     0,   326,     0,
       0,   327,   328,     0,     0,     0,   298,   299,   300,   301,
     302,   330,     0,   331,   332,   498,   264,   932,   303,     0,
       0,     0,     0,     0,   197,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,   558,     0,   308,     0,     0,     0,   200,   198,   199,
       0,     0,     0,     0,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   310,   201,   202,     0,     0,
       0,   203,     0,   534,   204,     0,     0,   311,   312,     0,
       0,     0,     0,     0,     0,     0,     0,   200,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,   206,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   559,   322,     0,
       0,     0,     0,     0,     0,     0,   201,   202,     0,     0,
       0,   203,     0,     0,   560,   324,   325,     0,   326,     0,
       0,   327,   328,   298,   299,   300,   301,   302,     0,   205,
       0,   561,     0,   331,   332,   303,   264,     0,   206,     0,
       0,   197,     0,     0,     0,     0,     0,   304,     0,     0,
     305,     0,     0,   306,     0,     0,   307,     0,     0,     0,
     308,     0,     0,     0,     0,   198,   199,     0,     0,     0,
       0,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     309,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,   200,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   559,   322,     0,     0,     0,   198,
     199,     0,     0,   201,   202,     0,     0,     0,   203,     0,
       0,   560,   324,   325,     0,   326,     0,     0,   327,   328,
     298,   299,   300,   301,   302,     0,   205,     0,   571,     0,
     331,   332,   303,   264,     0,   206,     0,     0,   294,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,   200,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   201,   202,   310,
       0,     0,   203,     0,   537,   204,     0,     0,     0,     0,
       0,   311,   312,     0,     0,     0,     0,     0,     0,     0,
     205,     0,     0,     0,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,    50,   197,     0,     0,     0,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    51,   322,     0,     0,     0,     0,     0,     0,     0,
     198,   199,     0,     0,     0,   295,     0,     0,   323,   324,
     325,     0,   326,     0,     0,   327,   328,   298,   299,   300,
     301,   302,     0,   296,     0,   330,     0,   331,   332,   303,
     264,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,     0,     0,   308,     0,     0,     0,     0,   200,
       0,     0,     0,     0,     0,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   309,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   310,     0,   201,   202,
       0,     0,     0,   203,     0,   539,   204,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,   640,     0,     0,
       0,   205,     0,   496,   497,     0,     0,     0,     0,     0,
     206,     0,    50,     0,   197,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    51,   322,
       0,     0,     0,     0,     0,     0,     0,     0,   198,   199,
       0,     0,     0,     0,     0,   323,   324,   325,     0,   326,
       0,     0,   327,   328,     0,     0,   298,   299,   300,   301,
     302,     0,   330,     0,   331,   332,   641,   264,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,   200,     0,     0,
       0,     0,     0,     0,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   310,   201,   202,     0,     0,
       0,   203,     0,   541,   204,     0,     0,   311,   312,     0,
     197,     0,     0,     0,     0,     0,   653,     0,     0,   205,
       0,     0,   496,   497,     0,     0,     0,     0,   206,     0,
       0,    50,     0,     0,   198,   199,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    51,   322,     0,
       0,     0,     0,     0,     0,   298,   299,   300,   301,   302,
       0,     0,     0,     0,   323,   324,   325,   303,   326,     0,
       0,   327,   328,     0,     0,     0,     0,     0,     0,   304,
       0,   330,   305,   331,   332,   306,   264,     0,   307,     0,
       0,     0,   308,   200,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   309,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,   201,   202,   310,     0,     0,   203,     0,     0,
     204,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,   205,     0,   548,     0,     0,
       0,     0,     0,     0,   206,     0,     0,   197,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    51,   322,     0,     0,
       0,   198,   199,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   298,   299,   300,   301,   302,     0,   584,     0,
     330,   585,   331,   332,   303,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   309,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   201,
     202,   310,     0,     0,   203,     0,   727,   204,     0,     0,
       0,     0,     0,   311,   312,     0,   197,     0,     0,     0,
       0,     0,   205,     0,     0,     0,     0,     0,   496,   497,
       0,   206,     0,     0,     0,     0,     0,    50,     0,     0,
     198,   199,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    51,   322,     0,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
     323,   324,   325,   303,   326,     0,     0,   327,   328,     0,
       0,     0,     0,     0,     0,   304,     0,   330,   305,   331,
     332,   306,   264,     0,   307,     0,     0,     0,   308,   200,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   309,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,   201,   202,
     310,     0,     0,   203,     0,   830,   204,     0,     0,     0,
       0,     0,   311,   312,     0,   197,     0,     0,     0,     0,
       0,   205,     0,     0,     0,     0,     0,   806,   807,     0,
     206,     0,     0,     0,     0,     0,    50,     0,     0,   198,
     199,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    51,   322,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,   323,
     324,   325,   303,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,   304,     0,   330,   305,   331,   332,
     306,   264,     0,   307,     0,     0,     0,   308,   200,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   201,   202,   310,
       0,     0,   203,     0,   837,   204,     0,     0,     0,     0,
       0,   311,   312,     0,   197,     0,     0,     0,     0,     0,
     205,     0,     0,     0,     0,     0,   815,   816,     0,   206,
       0,     0,     0,     0,     0,    50,     0,     0,   198,   199,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    51,   322,     0,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,     0,     0,     0,     0,   323,   324,
     325,   303,   326,     0,     0,   327,   328,     0,     0,     0,
       0,     0,     0,   304,     0,   330,   305,   331,   332,   306,
     264,     0,   307,     0,     0,     0,   308,   200,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   309,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   201,   202,   310,     0,
       0,   203,     0,   838,   204,     0,     0,     0,     0,     0,
     311,   312,     0,   197,     0,     0,     0,     0,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,   206,     0,
       0,     0,     0,     0,    50,     0,     0,   198,   199,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      51,   322,     0,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,     0,     0,   610,     0,   323,   324,   325,
     303,   326,     0,     0,   327,   328,     0,     0,     0,     0,
       0,     0,   304,   329,   330,   305,   331,   332,   306,   264,
       0,   307,     0,     0,     0,   308,   200,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   309,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,   201,   202,   310,     0,     0,
     203,     0,   839,   204,     0,     0,     0,     0,     0,   311,
     312,     0,   197,     0,     0,     0,     0,     0,   205,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,    50,     0,     0,   198,   199,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    51,
     322,     0,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,   323,   324,   325,   303,
     326,     0,     0,   327,   328,     0,     0,     0,     0,     0,
       0,   304,     0,   330,   305,   331,   332,   306,   264,     0,
     307,     0,     0,     0,   308,   200,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   309,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,   201,   202,   310,     0,     0,   203,
       0,   840,   204,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,   205,     0,     0,
       0,     0,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,    50,   197,     0,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    51,   322,
       0,     0,     0,     0,     0,     0,     0,   198,   199,     0,
       0,     0,     0,     0,     0,   323,   324,   325,     0,   326,
       0,     0,   327,   328,   298,   299,   300,   301,   302,     0,
       0,     0,   330,   629,   331,   332,   303,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,     0,     0,   650,   632,   200,     0,     0,     0,
       0,     0,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   309,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   310,     0,   878,   202,     0,     0,     0,
     203,     0,     0,   204,     0,   311,   312,     0,   197,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   205,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,    50,
       0,     0,   198,   199,     0,     0,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    51,   322,     0,     0,     0,
       0,     0,     0,   298,   299,   300,   301,   302,     0,     0,
       0,     0,   323,   324,   325,   303,   326,     0,     0,   327,
     328,     0,     0,     0,     0,     0,     0,   304,     0,   330,
     305,   331,   332,   306,   264,     0,   307,     0,     0,     0,
     308,   200,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     309,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
     201,   202,   310,     0,     0,   203,     0,   920,   204,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,   205,     0,     0,     0,     0,     0,     0,
       0,     0,   206,     0,     0,   197,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,   198,
     199,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   324,   325,     0,   326,     0,     0,   327,   328,
     298,   299,   300,   301,   302,     0,     0,     0,   330,   717,
     331,   332,   303,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,   200,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,   201,   202,   310,
       0,     0,   203,     0,   958,   204,     0,     0,     0,     0,
       0,   311,   312,     0,   197,     0,     0,     0,     0,     0,
     205,     0,     0,     0,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,    50,     0,     0,   198,   199,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    51,   322,     0,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,     0,     0,   719,     0,   323,   324,
     325,   303,   326,     0,     0,   327,   328,     0,     0,     0,
       0,     0,     0,   304,     0,   330,   305,   331,   332,   306,
     264,     0,   307,     0,     0,     0,   308,   200,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   309,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   201,   202,   310,     0,
       0,   203,     0,     0,   204,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,   206,     0,
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
       0,     0,     0,     0,     0,     0,     0,   311,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    51,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   324,   325,     0,   326,     0,
       0,   327,   328,   298,   299,   300,   301,   302,     0,     0,
       0,   330,   741,   331,   332,   303,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,     0,     0,
     305,     0,     0,   306,     0,     0,   307,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     309,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    51,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   324,   325,     0,   326,     0,     0,   327,   328,
     298,   299,   300,   301,   302,     0,     0,     0,   330,   872,
     331,   332,   303,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,   305,     0,     0,
     306,     0,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   309,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    51,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   324,
     325,     0,   326,     0,     0,   327,   328,   298,   299,   300,
     301,   302,     0,     0,     0,   330,   945,   331,   332,   303,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   309,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,   956,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    51,   322,
       0,     0,     0,     0,     0,     0,   298,   299,   300,   301,
     302,     0,     0,     0,     0,   323,   324,   325,   303,   326,
       0,     0,   327,   328,     0,     0,     0,     0,     0,     0,
     304,     0,   330,   305,   331,   332,   306,   264,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   309,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,   312,     0,
       0,     0,   433,   434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    51,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   324,   325,     0,   326,     0,
       0,   327,   328,     0,     0,     0,     0,     0,     0,     0,
       0,   330,     0,   331,   332,     0,   264,   669,   670,   671,
     672,   673,   674,   675,   676,   435,   436,   437,   438,   439,
     677,   678,   440,   441,   442,   443,   782,   444,   445,   446,
     447,   433,   434,     0,   448,   680,   449,   450,   681,   682,
       0,     0,   451,   452,   453,   683,   684,   685,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   783,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   465,   466,
       0,     0,     0,     0,     0,   791,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   670,   671,   672,
     673,   674,   675,   676,   435,   436,   437,   438,   439,   677,
     678,   440,   441,   442,   443,   782,   444,   445,   446,   447,
     433,   434,     0,   448,   680,   449,   450,   681,   682,     0,
       0,   451,   452,   453,   683,   684,   685,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   783,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,     0,
       0,     0,     0,     0,   881,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   670,   671,   672,   673,
     674,   675,   676,   435,   436,   437,   438,   439,   677,   678,
     440,   441,   442,   443,   782,   444,   445,   446,   447,   433,
     434,     0,   448,   680,   449,   450,   681,   682,     0,     0,
     451,   452,   453,   683,   684,   685,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   783,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,     0,     0,
       0,     0,     0,   883,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   670,   671,   672,   673,   674,
     675,   676,   435,   436,   437,   438,   439,   677,   678,   440,
     441,   442,   443,   782,   444,   445,   446,   447,   433,   434,
       0,   448,   680,   449,   450,   681,   682,     0,     0,   451,
     452,   453,   683,   684,   685,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   783,   454,     0,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   465,   466,     0,     0,     0,
       0,     0,   902,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   670,   671,   672,   673,   674,   675,
     676,   435,   436,   437,   438,   439,   677,   678,   440,   441,
     442,   443,   782,   444,   445,   446,   447,   433,   434,     0,
     448,   680,   449,   450,   681,   682,     0,     0,   451,   452,
     453,   683,   684,   685,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   783,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,     0,     0,     0,     0,
       0,   939,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,     0,     0,
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
       0,   465,   466,   433,   434,   722,     0,     0,   435,   436,
     437,   438,   439,     0,     0,   440,   441,   442,   443,     0,
     444,   445,   446,   447,     0,     0,     0,   448,     0,   449,
     450,     0,     0,     0,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   433,   434,   725,   435,   436,   437,   438,
     439,     0,     0,   440,   441,   442,   443,     0,   444,   445,
     446,   447,     0,     0,     0,   448,     0,   449,   450,     0,
       0,     0,     0,   451,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   433,   434,   836,     0,     0,   435,   436,   437,   438,
     439,     0,     0,   440,   441,   442,   443,     0,   444,   445,
     446,   447,     0,     0,     0,   448,     0,   449,   450,     0,
       0,     0,     0,   451,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   433,   434,   964,   435,   436,   437,   438,   439,     0,
       0,   440,   441,   442,   443,     0,   444,   445,   446,   447,
       0,     0,     0,   448,     0,   449,   450,     0,     0,     0,
       0,   451,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,   433,
     434,   966,     0,     0,   435,   436,   437,   438,   439,     0,
       0,   440,   441,   442,   443,     0,   444,   445,   446,   447,
       0,     0,     0,   448,     0,   449,   450,     0,     0,     0,
       0,   451,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,   433,   434,   465,   466,     0,
       0,   986,   435,   436,   437,   438,   439,     0,     0,   440,
     441,   442,   443,     0,   444,   445,   446,   447,     0,     0,
       0,   448,     0,   449,   450,     0,     0,     0,     0,   451,
     452,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,     0,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,   433,   434,     0,     0,   465,   466,   467,   435,   436,
     437,   438,   439,     0,     0,   440,   441,   442,   443,     0,
     444,   445,   446,   447,     0,     0,     0,   448,     0,   449,
     450,     0,     0,     0,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,   433,   434,     0,
       0,   465,   466,   765,   435,   436,   437,   438,   439,     0,
       0,   440,   441,   442,   443,     0,   444,   445,   446,   447,
       0,     0,     0,   448,     0,   449,   450,     0,     0,     0,
       0,   451,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,   433,   434,     0,     0,   465,   466,   773,
     435,   436,   437,   438,   439,     0,     0,   440,   441,   442,
     443,     0,   444,   445,   446,   447,     0,     0,     0,   448,
       0,   449,   450,     0,     0,     0,     0,   451,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,   875,   435,   436,   437,   438,
     439,   433,   434,   440,   441,   442,   443,     0,   444,   445,
     446,   447,     0,     0,     0,   448,     0,   449,   450,     0,
       0,     0,     0,   451,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   876,     0,     0,     0,     0,   669,   670,   671,   672,
     673,   674,   675,   676,   435,   436,   437,   438,   439,   677,
     678,   440,   441,   442,   443,   782,   444,   445,   446,   447,
    -243,   433,   434,   448,   680,   449,   450,   681,   682,     0,
       0,   451,   452,   453,   683,   684,   685,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   783,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,     0,     0,     0,     0,   669,   670,   671,   672,
     673,   674,   675,   676,   435,   436,   437,   438,   439,   677,
     678,   440,   441,   442,   443,   679,   444,   445,   446,   447,
       0,     0,     0,   448,   680,   449,   450,   681,   682,     0,
       0,   451,   452,   453,   683,   684,   685,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   686,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   433,   434,
       0,     0,   435,   436,   437,   438,   439,   465,   466,   440,
     441,   442,   443,     0,   444,   445,   446,   447,     0,     0,
       0,   448,     0,   449,   450,     0,     0,   574,     0,   451,
     452,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,     0,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   433,   434,     0,     0,
       0,     0,     0,     0,     0,   465,   466,     0,     0,     0,
       0,   435,   436,   437,   438,   439,     0,     0,   440,   441,
     442,   443,     0,   444,   445,   446,   447,     0,     0,     0,
     448,     0,   449,   450,     0,     0,     0,     0,   451,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   454,   767,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   433,   434,     0,     0,   435,
     436,   437,   438,   439,   465,   466,   440,   441,   442,   443,
       0,   444,   445,   446,   447,     0,     0,     0,   448,     0,
     449,   450,     0,     0,     0,     0,   451,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   858,     0,     0,
       0,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   433,   434,     0,     0,     0,     0,     0,
       0,     0,   465,   466,     0,     0,     0,     0,   435,   436,
     437,   438,   439,     0,     0,   440,   441,   442,   443,     0,
     444,   445,   446,   447,     0,     0,     0,   448,     0,   449,
     450,     0,     0,     0,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   433,   434,     0,     0,   435,   436,   437,   438,
     439,   465,   466,   440,   441,   442,   443,     0,   444,   445,
     446,   447,     0,     0,     0,   448,     0,   449,   450,     0,
     433,   434,     0,   451,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,     0,     0,     0,     0,   435,   436,   437,   438,   439,
       0,     0,   440,   441,   442,   443,     0,   444,   445,   446,
     447,     0,     0,     0,   448,     0,   449,   450,   433,   434,
       0,     0,   451,   435,   436,   437,   438,   439,     0,     0,
     440,   441,   442,   443,     0,   444,   445,   446,   447,     0,
       0,     0,   448,     0,   449,   450,   433,   434,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   465,   466,
       0,     0,     0,     0,     0,     0,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,     0,     0,
       0,   435,   436,   437,   438,   439,     0,     0,   440,   441,
     442,   443,     0,   444,   445,   446,   447,     0,     0,     0,
     448,     0,   449,   450,   433,   434,     0,     0,     0,   435,
     436,   437,   438,   439,     0,     0,   440,   441,   442,   443,
       0,   444,   445,   446,   447,     0,     0,     0,   448,     0,
     449,   450,     0,     0,     0,     0,     0,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,   458,   459,   460,   461,
     462,   463,   464,     0,     0,   111,     0,     0,     0,     0,
       0,   112,   465,   466,     0,     0,     0,   435,   436,   437,
     438,   439,     0,     0,   440,     0,   113,   443,     0,   444,
     445,   446,   447,     0,   214,     0,   448,     0,   449,   450,
       0,   114,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   466,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
      50,     0,     0,   233,   234,     0,     0,     0,     0,     0,
     235,   236,   237,   238,   239,     0,    51,   240,   241,   242,
     243,   244,   245,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,   112,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,     0,   256,   257,   113,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     113,     0,     0,     0,     0,   313,   314,   315,     0,     0,
       0,     0,     0,     0,    51,   114,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,     0,     0,
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
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396
};

static const yytype_int16 yycheck[] =
{
      12,   168,    73,   366,   361,   391,   474,   393,   286,   395,
     288,   173,   290,   476,   485,   417,   315,   357,   341,     7,
      21,    81,    19,   468,   495,   564,    46,   496,   497,    14,
      15,   584,   378,   572,    49,    18,     0,   119,   128,    20,
      21,   136,     6,    49,   145,    32,    18,    19,    34,    19,
     113,    19,    53,   116,   117,   136,   145,   414,   145,   416,
     114,    73,   183,   129,   185,    29,   145,    31,   114,    33,
      82,   114,   114,   161,   139,    39,   145,   123,   145,   145,
     123,   123,    46,   129,   185,   180,   116,   117,    52,   558,
      33,    73,    14,    15,   184,   183,   185,   145,   185,   153,
     154,   183,   156,   548,   185,   159,   185,    71,   154,   271,
     179,   154,   154,   176,   177,   145,    59,   474,   185,   105,
     106,    41,   103,   104,   145,   145,   107,   484,    92,   110,
     111,   112,   113,   181,   115,   116,   117,   118,   154,   145,
     178,   122,    58,   124,   125,   183,   176,   177,   136,   130,
     131,   132,    20,    21,   139,    75,   153,   153,   143,   145,
     145,   146,   145,   184,   165,   636,   116,   117,   183,   640,
     516,   145,   159,   145,   155,   176,   157,   158,   159,   160,
     161,   734,   653,   153,   165,   153,   129,   183,   153,   177,
     177,   114,   153,   732,   179,   176,   177,   185,   667,   186,
     123,   524,   145,   161,   153,   156,   114,   547,   279,   280,
     281,   282,   272,   177,   285,   123,   287,   139,   289,   183,
     291,   143,   183,   145,   146,   183,   176,   177,   129,   300,
     701,   154,     4,     5,   183,     7,   159,   105,   106,   153,
     145,   153,   313,   314,   145,   113,   154,   115,   116,   117,
     118,   154,    20,    21,   122,   129,   268,   153,   156,   160,
     153,   166,   153,    35,   153,   153,   180,   279,   280,   281,
     282,   153,    53,   285,   154,   287,   575,   289,    59,   291,
     183,   145,   156,   728,   180,   356,   184,   180,   300,   180,
     153,   180,   180,   364,   145,   740,   178,   279,   280,   281,
     282,   313,   314,   285,   153,   287,   845,   289,   176,   177,
     153,   589,   775,    11,   326,   178,   855,   153,   300,   764,
     487,   161,   145,   156,    22,    23,   129,   494,   161,   178,
     732,   313,   314,   161,   779,   178,   145,   105,   106,   741,
     411,   179,   178,   183,   356,   113,   179,   115,   116,   117,
     118,   183,   364,   185,   122,   183,   827,    66,    20,    21,
     156,    70,   524,   156,   156,   161,    53,   153,   161,   161,
     156,    53,    59,   159,   356,   139,   762,    59,    87,    88,
      89,    90,   364,   179,   847,    53,   179,   179,   147,   148,
     180,    59,   404,   183,   162,   163,   164,   165,   166,   411,
      32,    53,   181,   182,    53,   184,   176,    59,   176,   177,
      59,   147,   148,   149,   150,   147,   148,   429,   147,   148,
     149,   961,   145,   963,    56,    57,    42,    43,   145,   411,
     183,   899,   184,   184,   145,   145,   184,   145,    53,   160,
     160,   103,   104,   105,   106,   107,   160,   160,   110,   111,
     112,   113,   179,   115,   116,   117,   118,   528,   160,   160,
     122,   532,   124,   125,   160,   160,   160,   160,   183,   145,
     145,   638,   543,   751,   545,   861,    34,    34,   184,   550,
     647,   552,   553,   115,   555,   156,   136,   179,   145,    32,
     156,   156,   183,    20,    21,   662,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   183,   178,   184,   178,   145,
     184,   180,   179,   160,   176,   177,   528,   178,   160,   179,
     532,   884,   154,   155,   179,   160,   160,   159,   182,   161,
     162,   543,   160,   545,   177,   179,   136,   145,   550,   145,
     552,   553,   899,   555,   145,   177,   528,   139,    36,   178,
     532,   145,   183,   183,   186,   145,   183,   183,   183,   183,
     153,   543,   161,   634,   161,   161,   178,   153,   550,   147,
     552,   553,   185,   555,    20,    21,   103,   104,   105,   106,
     107,   145,   205,   110,   111,   112,   113,   160,   115,   116,
     117,   118,   179,   145,   145,   122,   145,   124,   125,    32,
       1,   119,    13,   130,   131,   132,   156,   139,   182,   184,
       7,    42,   183,    20,    21,   145,   145,   178,   785,   178,
     185,   147,   634,    56,    57,   178,   145,   183,   155,   145,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     178,   145,   145,   179,   178,   178,   145,   178,   185,   176,
     177,   161,   634,    32,   178,   180,   178,   184,   178,   105,
     106,   161,    49,   178,   178,   178,   178,   113,   185,    63,
     116,   117,   118,   178,   183,   421,   122,    37,   690,   750,
     108,   114,   115,    66,   960,   690,   277,    64,   311,   312,
     123,    82,   315,   793,     1,   868,   727,   898,   105,   106,
     323,   324,   325,   870,   327,   328,   113,   330,   115,   116,
     117,   118,   823,   478,   545,   122,   292,   697,   885,    45,
     521,   154,   155,   300,    32,   739,   159,   417,   952,   162,
     176,   177,    -1,    -1,    -1,    -1,   284,    -1,   750,    -1,
      -1,    -1,    -1,   366,   177,    -1,    -1,    -1,    56,    57,
     183,    -1,    -1,   186,    -1,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,   931,    -1,    -1,   934,   750,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   406,    -1,    -1,   409,    -1,    -1,    -1,
      20,    21,   415,    -1,   417,    -1,    -1,    -1,    -1,    -1,
     871,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   978,   435,   436,    -1,    -1,   439,   440,   441,   442,
      -1,   444,    -1,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,    -1,   466,    -1,    -1,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,    -1,   481,   871,
      -1,    -1,   485,    -1,    -1,   488,   489,    -1,    -1,   177,
     493,    -1,   495,   103,   104,   105,   106,   107,   186,   502,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,   871,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   549,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   560,   561,    -1,
      -1,    -1,   954,    -1,    -1,    -1,   176,   177,   571,    -1,
      -1,   574,   575,   183,    -1,   578,    -1,    -1,    -1,    -1,
      -1,   973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     623,    -1,    -1,   626,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   636,    -1,    -1,    32,   640,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     653,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,   679,   680,   681,   682,
     683,   684,   685,   686,    -1,   688,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,   701,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
     103,   104,   105,   106,   130,   131,   132,    -1,    -1,   115,
     113,    -1,   115,   116,   117,   118,    -1,   730,   731,   122,
      -1,   124,   125,    -1,   737,    -1,   739,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   754,    -1,    20,    21,    -1,    -1,    -1,   154,   155,
     176,   177,    -1,   159,   767,    -1,   162,   183,    -1,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,   782,
     783,   177,    -1,   176,   177,    -1,    -1,   183,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   806,   807,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   815,   816,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   827,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   879,    -1,    -1,    -1,
      -1,   884,    -1,    -1,    -1,   888,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   904,    -1,    -1,   907,   908,   909,   910,   911,   176,
     177,   914,   915,    -1,   917,    -1,   183,    -1,     1,    -1,
      -1,    -1,   925,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   956,    37,    38,    -1,    40,    41,    -1,
      -1,    44,   965,    46,    47,    48,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,   980,    61,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   181,   182,
     183,   184,   185,     1,    -1,    -1,    -1,    -1,     6,    -1,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    -1,    -1,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    32,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    14,    15,    16,    17,
      18,   179,    -1,   181,   182,   183,   184,   185,    26,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    49,    -1,    51,    -1,    -1,    -1,   115,    56,    57,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    14,    15,    16,    17,    18,    -1,   177,
      -1,   179,    -1,   181,   182,    26,   184,    -1,   186,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    56,
      57,    -1,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,   170,
      14,    15,    16,    17,    18,    -1,   177,    -1,   179,    -1,
     181,   182,    26,   184,    -1,   186,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   154,   155,    93,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,   129,    32,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,   177,    -1,   179,    -1,   181,   182,    26,
     184,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,   154,   155,
      -1,    -1,    -1,   159,    -1,   161,   162,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   177,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
     186,    -1,   129,    -1,    32,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   179,    -1,   181,   182,   183,   184,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,   154,   155,    -1,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,   105,   106,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,   177,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,   186,    -1,
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
      -1,    -1,    -1,    -1,    -1,   177,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    32,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    14,    15,    16,    17,    18,    -1,   177,    -1,
     179,   180,   181,   182,    26,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
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
      -1,   177,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
     186,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,
      57,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   162,
     163,   164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,
      44,   184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    21,    -1,   162,   163,   164,
      26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    38,   178,   179,    41,   181,   182,    44,   184,
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
      -1,    38,    -1,   179,    41,   181,   182,    44,   184,    -1,
      47,    -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,   154,   155,    93,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,   129,    32,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   179,   180,   181,   182,    26,   184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    54,   114,   115,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    93,    -1,   154,   155,    -1,    -1,    -1,
     159,    -1,    -1,   162,    -1,   105,   106,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   129,
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
      14,    15,    16,    17,    18,    -1,    -1,    -1,   179,   180,
     181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,   154,   155,    93,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    56,    57,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,   160,    -1,   162,   163,
     164,    26,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   179,    41,   181,   182,    44,
     184,    -1,    47,    -1,    -1,    -1,    51,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    88,    89,    90,   154,   155,    93,    -1,
      -1,   159,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   179,   180,   181,   182,    26,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,   170,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   179,   180,
     181,   182,    26,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   179,   180,   181,   182,    26,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,   162,   163,   164,    26,   166,
      -1,    -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   179,    41,   181,   182,    44,   184,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,
      -1,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,   181,   182,    -1,   184,    95,    96,    97,
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
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,    20,    21,   180,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    20,    21,   180,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    20,    21,   180,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    20,    21,   180,    -1,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
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
      -1,    -1,    -1,    -1,    -1,    20,    21,   176,   177,    -1,
      -1,   180,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,   176,   177,   178,   103,   104,
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
      -1,    -1,    -1,    20,    21,    -1,    -1,   176,   177,   178,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,   178,   103,   104,   105,   106,
     107,    20,    21,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,   178,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    20,    21,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    20,    21,
      -1,    -1,   103,   104,   105,   106,   107,   176,   177,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,   128,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    20,    21,    -1,    -1,   103,
     104,   105,   106,   107,   176,   177,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,    -1,    -1,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    20,    21,    -1,    -1,   103,   104,   105,   106,
     107,   176,   177,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      20,    21,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,
      -1,    -1,   130,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    20,    21,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    20,    21,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,   176,   177,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,    -1,    45,   113,    -1,   115,
     116,   117,   118,    -1,    37,    -1,   122,    -1,   124,   125,
      -1,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     129,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,    -1,   145,   130,   131,   132,
     133,   134,   135,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,   169,   170,    45,    -1,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      45,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   188,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   177,   183,   191,   199,   200,   204,   225,   229,
     245,   302,   307,   309,   314,   350,   352,    18,    19,   145,
     219,   220,   221,   139,   205,   206,   145,   166,   201,   202,
     145,   184,   305,   145,   181,   190,   353,   351,    33,    59,
     129,   145,   222,   223,   224,   237,     4,     5,     7,    35,
     312,    58,   300,   154,   153,   156,   153,   201,    21,    53,
     165,   176,   203,   154,   305,   306,   300,   145,   145,   145,
     129,   179,   153,   178,    53,    59,   230,   232,    53,    59,
     308,    53,    59,   313,    53,    59,   301,    14,    15,   139,
     143,   145,   146,   179,   193,   220,   139,   206,   145,   145,
     145,    24,    30,    45,    60,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   237,   317,   319,   320,
     322,   325,   183,   185,   305,    53,    59,   189,   184,   184,
     145,   221,   223,    34,   105,   106,   145,   228,   231,   145,
     145,   311,   184,   303,   147,   148,   192,    14,    15,   139,
     143,   145,   193,   217,   218,   203,   160,   160,   160,   160,
     179,   160,   160,   160,   160,   160,   160,    32,    56,    57,
     115,   154,   155,   159,   162,   177,   186,   183,   145,   321,
     145,   292,   293,   180,    37,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   116,   117,   123,   124,   125,   126,   127,
     130,   131,   132,   133,   134,   135,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   169,   170,   176,   177,
      34,    34,   179,   226,   184,   233,   156,   184,   156,   310,
     315,   299,   136,   281,   147,   148,   149,   153,   180,   328,
     330,   332,   326,   145,   323,   334,   336,   338,   340,   342,
     344,   346,   348,    32,    32,   159,   177,   186,    14,    15,
      16,    17,    18,    26,    38,    41,    44,    47,    51,    71,
      93,   105,   106,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   146,   162,   163,   164,   166,   169,   170,   178,
     179,   181,   182,   195,   197,   233,   237,   239,   247,   254,
     257,   260,   264,   265,   268,   269,   270,   275,   278,   317,
     354,   358,   363,   365,   367,   183,   156,   183,   178,   178,
     180,   281,   288,   289,   156,   227,   235,    66,    70,    87,
      88,    89,    90,   318,   304,   237,   184,   185,   281,   221,
     145,   298,   316,   217,   325,   325,   325,   325,   180,   321,
     325,   226,   325,   226,   325,   226,   145,   290,   291,   325,
     293,   178,   325,   354,   179,   160,   179,   160,   160,   179,
     160,   160,   278,   278,    12,   325,    12,   325,   278,   360,
     364,   194,   278,   278,   278,   237,   278,   278,   278,   182,
     145,   179,   225,    20,    21,   103,   104,   105,   106,   107,
     110,   111,   112,   113,   115,   116,   117,   118,   122,   124,
     125,   130,   131,   132,   155,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   176,   177,   178,   179,   145,
     185,   325,   185,   292,    46,   295,   316,   180,   183,   325,
       1,     8,     9,    10,    12,    25,    27,    28,    37,    40,
      48,    50,    54,    55,    61,    94,   120,   121,   183,   185,
     207,   208,   211,   212,   213,   214,   215,   216,   234,   236,
     238,   240,   241,   242,   243,   244,   245,   246,   266,   267,
     278,   184,   145,   185,   286,   298,   178,   153,   156,   159,
     297,   161,   183,   161,   161,   161,   183,   161,   227,   161,
     227,   161,   227,   156,   161,   183,   161,   183,   179,   237,
     255,   278,   248,   250,   278,   252,   325,   316,    49,   145,
     162,   179,   278,   355,   356,   357,   359,   360,   361,   362,
     316,   179,   356,   362,   128,   183,   185,   149,   150,   192,
     198,   180,   160,   237,   177,   180,   258,   278,   136,   263,
      18,   145,   145,   278,   278,   278,   278,   278,   278,   145,
     278,   145,   278,   278,   278,   278,   278,   278,   278,   278,
      21,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   116,   117,   145,   176,   177,   276,   278,   180,
     258,   295,   114,   154,   156,   159,   294,   288,   278,   316,
     114,   183,   236,   266,   278,   233,   278,   278,   183,   139,
      54,   278,   233,   114,   236,   278,   182,   264,   264,    36,
     183,   183,   278,   183,   183,   183,   298,   119,   183,    95,
      96,    97,    98,    99,   100,   101,   102,   108,   109,   114,
     123,   126,   127,   133,   134,   135,   154,   304,   154,   183,
     177,   185,   225,   281,   285,   178,   145,   325,   114,   123,
     154,   296,   329,   325,   333,   327,   324,   335,   161,   339,
     161,   343,   161,   325,   347,   290,   349,   180,   258,   160,
     278,   325,   180,   325,   325,   180,   325,   161,    19,   264,
     123,   294,   180,   153,   183,   357,   178,   153,   178,   183,
      19,   180,   357,   185,   278,   360,   185,   278,   147,   196,
     271,   273,   145,   355,   153,   180,   114,   123,   154,   159,
     261,   262,   226,   160,   179,   178,   145,   156,   145,   278,
     145,   278,     1,   178,   180,   325,   236,   278,   233,    19,
     236,   278,   114,   154,   183,    13,   233,   156,   139,   236,
     278,   183,   182,   184,   233,   264,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   120,   121,   278,   278,
     278,   278,   278,   278,   278,   120,   121,   278,   185,   278,
     224,     7,    42,   282,   183,   154,   183,   296,   236,   278,
     161,   337,   341,   345,   180,   145,   180,   161,   161,   161,
     161,   263,   258,   278,   278,   356,   357,   145,   355,   178,
     178,   278,   178,   361,   258,   356,   185,   178,   151,   147,
     325,   226,   178,   278,   145,   145,   145,   145,   153,   178,
     227,   279,   180,   258,   278,   178,   178,   277,   154,   294,
     258,   183,   233,   183,   235,    11,    22,    23,   209,   210,
     278,   278,   278,   278,   183,   178,    53,    59,   284,   284,
     236,   278,   183,   331,   161,   183,   256,   249,   251,   253,
     179,   183,   357,   178,   123,   294,   178,   183,   357,   178,
     161,   227,   180,   261,   178,   128,   233,   259,   325,   180,
     278,   183,   185,   233,   278,   183,    43,   282,   295,   183,
     278,   145,   278,   278,   278,   180,   278,   278,   178,   178,
     278,   278,   278,   185,   272,   161,   114,   278,   161,   233,
     233,   287,    32,   283,   180,   161,   180,   183,   366,   178,
     366,   178,   237,   274,   278,   280,   210,   231,   231,   278,
      49,   178,   185,   237,   183,   233,   180,   278,   178,   178
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
     228,   228,   228,   228,   228,   228,   228,   228,   229,   230,
     230,   230,   231,   232,   233,   233,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   235,   235,   235,   236,   236,
     236,   237,   237,   237,   238,   239,   239,   239,   239,   240,
     241,   242,   242,   242,   242,   242,   243,   243,   243,   243,
     244,   245,   245,   246,   248,   249,   247,   250,   251,   247,
     252,   253,   247,   255,   256,   254,   257,   257,   257,   258,
     258,   259,   259,   259,   260,   260,   260,   261,   261,   261,
     261,   262,   262,   263,   263,   264,   264,   265,   265,   265,
     265,   265,   265,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   267,   267,   267,   267,   268,   269,   269,   270,
     271,   272,   270,   273,   274,   270,   275,   275,   276,   277,
     275,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   279,   280,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   281,   281,   282,   282,   283,   283,
     284,   284,   284,   285,   286,   286,   287,   286,   286,   286,
     288,   288,   289,   289,   290,   290,   291,   291,   292,   293,
     293,   294,   294,   295,   295,   295,   295,   295,   295,   296,
     296,   296,   297,   297,   298,   298,   298,   298,   298,   299,
     299,   300,   300,   301,   301,   301,   302,   303,   302,   304,
     304,   304,   305,   306,   306,   307,   307,   308,   308,   308,
     309,   309,   310,   310,   311,   312,   312,   313,   313,   313,
     315,   314,   316,   316,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   318,
     318,   318,   318,   318,   318,   319,   320,   320,   321,   321,
     323,   324,   322,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     326,   327,   325,   325,   328,   329,   325,   330,   331,   325,
     332,   333,   325,   325,   334,   335,   325,   336,   337,   325,
     325,   338,   339,   325,   340,   341,   325,   325,   342,   343,
     325,   344,   345,   325,   346,   347,   325,   348,   349,   325,
     351,   350,   353,   352,   354,   354,   354,   354,   355,   355,
     355,   355,   356,   356,   357,   357,   358,   358,   358,   358,
     358,   358,   359,   359,   360,   361,   361,   362,   362,   363,
     363,   364,   364,   365,   366,   366,   367,   367
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
       1,     3,     3,     2,     0,     4,     0,     1,     0,     1,
       0,     1,     1,     4,     0,     3,     0,     8,     8,     5,
       2,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     1,     1,     2,     3,     5,     3,     3,     1,
       1,     1,     0,     1,     4,     6,     5,     5,     4,     0,
       3,     0,     1,     0,     1,     1,     6,     0,     6,     0,
       3,     5,     3,     2,     3,     4,     2,     0,     1,     1,
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
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3439 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3445 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3451 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3457 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3463 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3469 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3475 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3481 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3487 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3493 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3499 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3505 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3511 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3517 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3523 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3529 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3535 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3541 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_name  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3547 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3553 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument_list  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3559 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration_name  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3565 "ds_parser.cpp"
        break;

    case 223: /* annotation_declaration  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3571 "ds_parser.cpp"
        break;

    case 224: /* annotation_list  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3577 "ds_parser.cpp"
        break;

    case 225: /* optional_annotation_list  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3583 "ds_parser.cpp"
        break;

    case 226: /* optional_function_argument_list  */
#line 128 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3589 "ds_parser.cpp"
        break;

    case 227: /* optional_function_type  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3595 "ds_parser.cpp"
        break;

    case 228: /* function_name  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3601 "ds_parser.cpp"
        break;

    case 233: /* expression_block  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3607 "ds_parser.cpp"
        break;

    case 234: /* expression_any  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3613 "ds_parser.cpp"
        break;

    case 235: /* expressions  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3619 "ds_parser.cpp"
        break;

    case 236: /* expr_pipe  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3625 "ds_parser.cpp"
        break;

    case 237: /* name_in_namespace  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3631 "ds_parser.cpp"
        break;

    case 238: /* expression_delete  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3637 "ds_parser.cpp"
        break;

    case 239: /* expr_new  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3643 "ds_parser.cpp"
        break;

    case 240: /* expression_break  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3649 "ds_parser.cpp"
        break;

    case 241: /* expression_continue  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3655 "ds_parser.cpp"
        break;

    case 242: /* expression_return  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3661 "ds_parser.cpp"
        break;

    case 243: /* expression_yield  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3667 "ds_parser.cpp"
        break;

    case 244: /* expression_try_catch  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3673 "ds_parser.cpp"
        break;

    case 246: /* expression_let  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3679 "ds_parser.cpp"
        break;

    case 247: /* expr_cast  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3685 "ds_parser.cpp"
        break;

    case 254: /* expr_type_decl  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3691 "ds_parser.cpp"
        break;

    case 257: /* expr_type_info  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3697 "ds_parser.cpp"
        break;

    case 258: /* expr_list  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3703 "ds_parser.cpp"
        break;

    case 259: /* block_or_simple_block  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3709 "ds_parser.cpp"
        break;

    case 261: /* capture_entry  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3715 "ds_parser.cpp"
        break;

    case 262: /* capture_list  */
#line 138 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3721 "ds_parser.cpp"
        break;

    case 263: /* optional_capture_list  */
#line 138 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3727 "ds_parser.cpp"
        break;

    case 264: /* expr_block  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3733 "ds_parser.cpp"
        break;

    case 265: /* expr_numeric_const  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3739 "ds_parser.cpp"
        break;

    case 266: /* expr_assign  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3745 "ds_parser.cpp"
        break;

    case 267: /* expr_assign_pipe  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3751 "ds_parser.cpp"
        break;

    case 268: /* expr_named_call  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3757 "ds_parser.cpp"
        break;

    case 269: /* expr_method_call  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3763 "ds_parser.cpp"
        break;

    case 270: /* func_addr_expr  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3769 "ds_parser.cpp"
        break;

    case 275: /* expr_field  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3775 "ds_parser.cpp"
        break;

    case 278: /* expr  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3781 "ds_parser.cpp"
        break;

    case 281: /* optional_field_annotation  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3787 "ds_parser.cpp"
        break;

    case 285: /* structure_variable_declaration  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3793 "ds_parser.cpp"
        break;

    case 286: /* struct_variable_declaration_list  */
#line 128 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3799 "ds_parser.cpp"
        break;

    case 288: /* function_argument_declaration  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3805 "ds_parser.cpp"
        break;

    case 289: /* function_argument_list  */
#line 128 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3811 "ds_parser.cpp"
        break;

    case 290: /* tuple_type  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3817 "ds_parser.cpp"
        break;

    case 291: /* tuple_type_list  */
#line 128 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3823 "ds_parser.cpp"
        break;

    case 292: /* variant_type  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3829 "ds_parser.cpp"
        break;

    case 293: /* variant_type_list  */
#line 128 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3835 "ds_parser.cpp"
        break;

    case 295: /* variable_declaration  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3841 "ds_parser.cpp"
        break;

    case 298: /* let_variable_declaration  */
#line 127 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3847 "ds_parser.cpp"
        break;

    case 299: /* global_variable_declaration_list  */
#line 128 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3853 "ds_parser.cpp"
        break;

    case 304: /* enum_list  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3859 "ds_parser.cpp"
        break;

    case 310: /* optional_structure_parent  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3865 "ds_parser.cpp"
        break;

    case 316: /* variable_name_with_pos_list  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3871 "ds_parser.cpp"
        break;

    case 319: /* structure_type_declaration  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3877 "ds_parser.cpp"
        break;

    case 320: /* auto_type_declaration  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3883 "ds_parser.cpp"
        break;

    case 321: /* bitfield_bits  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3889 "ds_parser.cpp"
        break;

    case 322: /* bitfield_type_declaration  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3895 "ds_parser.cpp"
        break;

    case 325: /* type_declaration  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3901 "ds_parser.cpp"
        break;

    case 354: /* make_decl  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3907 "ds_parser.cpp"
        break;

    case 355: /* make_struct_fields  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3913 "ds_parser.cpp"
        break;

    case 356: /* make_struct_dim  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3919 "ds_parser.cpp"
        break;

    case 357: /* optional_block  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3925 "ds_parser.cpp"
        break;

    case 358: /* make_struct_decl  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3931 "ds_parser.cpp"
        break;

    case 359: /* make_tuple  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3937 "ds_parser.cpp"
        break;

    case 360: /* make_map_tuple  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3943 "ds_parser.cpp"
        break;

    case 361: /* make_any_tuple  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3949 "ds_parser.cpp"
        break;

    case 362: /* make_dim  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3955 "ds_parser.cpp"
        break;

    case 363: /* make_dim_decl  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3961 "ds_parser.cpp"
        break;

    case 364: /* make_table  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3967 "ds_parser.cpp"
        break;

    case 365: /* make_table_decl  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3973 "ds_parser.cpp"
        break;

    case 366: /* array_comprehension_where  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3979 "ds_parser.cpp"
        break;

    case 367: /* array_comprehension  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3985 "ds_parser.cpp"
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
#line 472 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4277 "ds_parser.cpp"
    break;

  case 16:
#line 473 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4283 "ds_parser.cpp"
    break;

  case 17:
#line 474 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4289 "ds_parser.cpp"
    break;

  case 18:
#line 478 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4295 "ds_parser.cpp"
    break;

  case 19:
#line 479 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4301 "ds_parser.cpp"
    break;

  case 20:
#line 483 "ds_parser.ypp"
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
#line 4320 "ds_parser.cpp"
    break;

  case 21:
#line 500 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4326 "ds_parser.cpp"
    break;

  case 22:
#line 501 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4332 "ds_parser.cpp"
    break;

  case 23:
#line 502 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4338 "ds_parser.cpp"
    break;

  case 24:
#line 503 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4344 "ds_parser.cpp"
    break;

  case 25:
#line 507 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4350 "ds_parser.cpp"
    break;

  case 26:
#line 511 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4360 "ds_parser.cpp"
    break;

  case 27:
#line 516 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4374 "ds_parser.cpp"
    break;

  case 28:
#line 525 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4384 "ds_parser.cpp"
    break;

  case 29:
#line 533 "ds_parser.ypp"
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
#line 4402 "ds_parser.cpp"
    break;

  case 30:
#line 549 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4412 "ds_parser.cpp"
    break;

  case 31:
#line 554 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4422 "ds_parser.cpp"
    break;

  case 32:
#line 562 "ds_parser.ypp"
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
#line 4449 "ds_parser.cpp"
    break;

  case 33:
#line 583 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4461 "ds_parser.cpp"
    break;

  case 34:
#line 592 "ds_parser.ypp"
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
#line 4485 "ds_parser.cpp"
    break;

  case 36:
#line 618 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4493 "ds_parser.cpp"
    break;

  case 37:
#line 621 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4502 "ds_parser.cpp"
    break;

  case 38:
#line 625 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4513 "ds_parser.cpp"
    break;

  case 39:
#line 631 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4524 "ds_parser.cpp"
    break;

  case 40:
#line 640 "ds_parser.ypp"
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
#line 4550 "ds_parser.cpp"
    break;

  case 41:
#line 661 "ds_parser.ypp"
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
#line 4575 "ds_parser.cpp"
    break;

  case 42:
#line 684 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4581 "ds_parser.cpp"
    break;

  case 43:
#line 685 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4587 "ds_parser.cpp"
    break;

  case 47:
#line 698 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4595 "ds_parser.cpp"
    break;

  case 48:
#line 701 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4603 "ds_parser.cpp"
    break;

  case 49:
#line 707 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4611 "ds_parser.cpp"
    break;

  case 50:
#line 713 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4619 "ds_parser.cpp"
    break;

  case 51:
#line 716 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4627 "ds_parser.cpp"
    break;

  case 52:
#line 722 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4633 "ds_parser.cpp"
    break;

  case 53:
#line 723 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4639 "ds_parser.cpp"
    break;

  case 54:
#line 727 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4645 "ds_parser.cpp"
    break;

  case 55:
#line 728 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4651 "ds_parser.cpp"
    break;

  case 56:
#line 729 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4662 "ds_parser.cpp"
    break;

  case 57:
#line 738 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4668 "ds_parser.cpp"
    break;

  case 58:
#line 739 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4674 "ds_parser.cpp"
    break;

  case 59:
#line 743 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4685 "ds_parser.cpp"
    break;

  case 60:
#line 752 "ds_parser.ypp"
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
#line 4703 "ds_parser.cpp"
    break;

  case 61:
#line 768 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4713 "ds_parser.cpp"
    break;

  case 62:
#line 776 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4725 "ds_parser.cpp"
    break;

  case 63:
#line 786 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4736 "ds_parser.cpp"
    break;

  case 64:
#line 795 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4742 "ds_parser.cpp"
    break;

  case 65:
#line 796 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4748 "ds_parser.cpp"
    break;

  case 66:
#line 797 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4754 "ds_parser.cpp"
    break;

  case 67:
#line 798 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4760 "ds_parser.cpp"
    break;

  case 68:
#line 799 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4766 "ds_parser.cpp"
    break;

  case 69:
#line 800 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4772 "ds_parser.cpp"
    break;

  case 70:
#line 804 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4782 "ds_parser.cpp"
    break;

  case 71:
#line 809 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4792 "ds_parser.cpp"
    break;

  case 72:
#line 817 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4798 "ds_parser.cpp"
    break;

  case 73:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4804 "ds_parser.cpp"
    break;

  case 74:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4810 "ds_parser.cpp"
    break;

  case 75:
#line 823 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4816 "ds_parser.cpp"
    break;

  case 76:
#line 824 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4822 "ds_parser.cpp"
    break;

  case 77:
#line 825 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4828 "ds_parser.cpp"
    break;

  case 78:
#line 826 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4834 "ds_parser.cpp"
    break;

  case 79:
#line 827 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4840 "ds_parser.cpp"
    break;

  case 80:
#line 828 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4846 "ds_parser.cpp"
    break;

  case 81:
#line 829 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4852 "ds_parser.cpp"
    break;

  case 82:
#line 830 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4860 "ds_parser.cpp"
    break;

  case 83:
#line 836 "ds_parser.ypp"
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
#line 4879 "ds_parser.cpp"
    break;

  case 84:
#line 850 "ds_parser.ypp"
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
#line 4898 "ds_parser.cpp"
    break;

  case 85:
#line 867 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4904 "ds_parser.cpp"
    break;

  case 86:
#line 868 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4910 "ds_parser.cpp"
    break;

  case 87:
#line 869 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4916 "ds_parser.cpp"
    break;

  case 88:
#line 873 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4929 "ds_parser.cpp"
    break;

  case 89:
#line 881 "ds_parser.ypp"
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
#line 4944 "ds_parser.cpp"
    break;

  case 90:
#line 894 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4953 "ds_parser.cpp"
    break;

  case 91:
#line 898 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4962 "ds_parser.cpp"
    break;

  case 92:
#line 905 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4968 "ds_parser.cpp"
    break;

  case 93:
#line 906 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4974 "ds_parser.cpp"
    break;

  case 94:
#line 910 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4980 "ds_parser.cpp"
    break;

  case 95:
#line 911 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4986 "ds_parser.cpp"
    break;

  case 96:
#line 912 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4992 "ds_parser.cpp"
    break;

  case 97:
#line 916 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5000 "ds_parser.cpp"
    break;

  case 98:
#line 919 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5009 "ds_parser.cpp"
    break;

  case 99:
#line 926 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5018 "ds_parser.cpp"
    break;

  case 100:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5024 "ds_parser.cpp"
    break;

  case 101:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5030 "ds_parser.cpp"
    break;

  case 102:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5036 "ds_parser.cpp"
    break;

  case 103:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5042 "ds_parser.cpp"
    break;

  case 104:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5048 "ds_parser.cpp"
    break;

  case 105:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5054 "ds_parser.cpp"
    break;

  case 106:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5060 "ds_parser.cpp"
    break;

  case 107:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5066 "ds_parser.cpp"
    break;

  case 108:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5072 "ds_parser.cpp"
    break;

  case 109:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5078 "ds_parser.cpp"
    break;

  case 110:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5084 "ds_parser.cpp"
    break;

  case 111:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5090 "ds_parser.cpp"
    break;

  case 112:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5096 "ds_parser.cpp"
    break;

  case 113:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5102 "ds_parser.cpp"
    break;

  case 114:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5108 "ds_parser.cpp"
    break;

  case 115:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5114 "ds_parser.cpp"
    break;

  case 116:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5120 "ds_parser.cpp"
    break;

  case 117:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5126 "ds_parser.cpp"
    break;

  case 118:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5132 "ds_parser.cpp"
    break;

  case 119:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5138 "ds_parser.cpp"
    break;

  case 120:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5144 "ds_parser.cpp"
    break;

  case 121:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5150 "ds_parser.cpp"
    break;

  case 122:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5156 "ds_parser.cpp"
    break;

  case 123:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5162 "ds_parser.cpp"
    break;

  case 124:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5168 "ds_parser.cpp"
    break;

  case 125:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5174 "ds_parser.cpp"
    break;

  case 126:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5180 "ds_parser.cpp"
    break;

  case 127:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5186 "ds_parser.cpp"
    break;

  case 128:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5192 "ds_parser.cpp"
    break;

  case 129:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5198 "ds_parser.cpp"
    break;

  case 130:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5204 "ds_parser.cpp"
    break;

  case 131:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5210 "ds_parser.cpp"
    break;

  case 132:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5216 "ds_parser.cpp"
    break;

  case 133:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5222 "ds_parser.cpp"
    break;

  case 134:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5228 "ds_parser.cpp"
    break;

  case 135:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5234 "ds_parser.cpp"
    break;

  case 136:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5240 "ds_parser.cpp"
    break;

  case 137:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5246 "ds_parser.cpp"
    break;

  case 138:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5252 "ds_parser.cpp"
    break;

  case 139:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5258 "ds_parser.cpp"
    break;

  case 140:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5264 "ds_parser.cpp"
    break;

  case 141:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5270 "ds_parser.cpp"
    break;

  case 142:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5276 "ds_parser.cpp"
    break;

  case 143:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5282 "ds_parser.cpp"
    break;

  case 144:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5288 "ds_parser.cpp"
    break;

  case 145:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5294 "ds_parser.cpp"
    break;

  case 146:
#line 976 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5300 "ds_parser.cpp"
    break;

  case 147:
#line 977 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5306 "ds_parser.cpp"
    break;

  case 148:
#line 981 "ds_parser.ypp"
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
#line 5330 "ds_parser.cpp"
    break;

  case 149:
#line 1003 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5336 "ds_parser.cpp"
    break;

  case 150:
#line 1004 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5342 "ds_parser.cpp"
    break;

  case 151:
#line 1005 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5348 "ds_parser.cpp"
    break;

  case 152:
#line 1009 "ds_parser.ypp"
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
#line 5388 "ds_parser.cpp"
    break;

  case 153:
#line 1047 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5398 "ds_parser.cpp"
    break;

  case 154:
#line 1055 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5407 "ds_parser.cpp"
    break;

  case 155:
#line 1059 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5420 "ds_parser.cpp"
    break;

  case 156:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5426 "ds_parser.cpp"
    break;

  case 157:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5432 "ds_parser.cpp"
    break;

  case 158:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5438 "ds_parser.cpp"
    break;

  case 159:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5444 "ds_parser.cpp"
    break;

  case 160:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5450 "ds_parser.cpp"
    break;

  case 161:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5456 "ds_parser.cpp"
    break;

  case 162:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5462 "ds_parser.cpp"
    break;

  case 163:
#line 1077 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5468 "ds_parser.cpp"
    break;

  case 164:
#line 1078 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5474 "ds_parser.cpp"
    break;

  case 165:
#line 1079 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5480 "ds_parser.cpp"
    break;

  case 166:
#line 1080 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5486 "ds_parser.cpp"
    break;

  case 167:
#line 1081 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5492 "ds_parser.cpp"
    break;

  case 168:
#line 1082 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5498 "ds_parser.cpp"
    break;

  case 169:
#line 1083 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5504 "ds_parser.cpp"
    break;

  case 170:
#line 1084 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5510 "ds_parser.cpp"
    break;

  case 171:
#line 1085 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5516 "ds_parser.cpp"
    break;

  case 172:
#line 1086 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5522 "ds_parser.cpp"
    break;

  case 173:
#line 1087 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5528 "ds_parser.cpp"
    break;

  case 174:
#line 1088 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5534 "ds_parser.cpp"
    break;

  case 175:
#line 1092 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5544 "ds_parser.cpp"
    break;

  case 176:
#line 1097 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5555 "ds_parser.cpp"
    break;

  case 177:
#line 1103 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5563 "ds_parser.cpp"
    break;

  case 178:
#line 1109 "ds_parser.ypp"
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
#line 5602 "ds_parser.cpp"
    break;

  case 179:
#line 1143 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5610 "ds_parser.cpp"
    break;

  case 180:
#line 1146 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5618 "ds_parser.cpp"
    break;

  case 181:
#line 1152 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5624 "ds_parser.cpp"
    break;

  case 182:
#line 1153 "ds_parser.ypp"
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
#line 5640 "ds_parser.cpp"
    break;

  case 183:
#line 1164 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5646 "ds_parser.cpp"
    break;

  case 184:
#line 1168 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5654 "ds_parser.cpp"
    break;

  case 185:
#line 1174 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5662 "ds_parser.cpp"
    break;

  case 186:
#line 1177 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5670 "ds_parser.cpp"
    break;

  case 187:
#line 1180 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5679 "ds_parser.cpp"
    break;

  case 188:
#line 1184 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5687 "ds_parser.cpp"
    break;

  case 189:
#line 1190 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5693 "ds_parser.cpp"
    break;

  case 190:
#line 1194 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5699 "ds_parser.cpp"
    break;

  case 191:
#line 1198 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5707 "ds_parser.cpp"
    break;

  case 192:
#line 1201 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5715 "ds_parser.cpp"
    break;

  case 193:
#line 1204 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5725 "ds_parser.cpp"
    break;

  case 194:
#line 1209 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5733 "ds_parser.cpp"
    break;

  case 195:
#line 1212 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5743 "ds_parser.cpp"
    break;

  case 196:
#line 1220 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5751 "ds_parser.cpp"
    break;

  case 197:
#line 1223 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5761 "ds_parser.cpp"
    break;

  case 198:
#line 1228 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5769 "ds_parser.cpp"
    break;

  case 199:
#line 1231 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5779 "ds_parser.cpp"
    break;

  case 200:
#line 1239 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5787 "ds_parser.cpp"
    break;

  case 201:
#line 1245 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5793 "ds_parser.cpp"
    break;

  case 202:
#line 1246 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5799 "ds_parser.cpp"
    break;

  case 203:
#line 1250 "ds_parser.ypp"
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
#line 5835 "ds_parser.cpp"
    break;

  case 204:
#line 1284 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5841 "ds_parser.cpp"
    break;

  case 205:
#line 1284 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5847 "ds_parser.cpp"
    break;

  case 206:
#line 1284 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5855 "ds_parser.cpp"
    break;

  case 207:
#line 1287 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5861 "ds_parser.cpp"
    break;

  case 208:
#line 1287 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5867 "ds_parser.cpp"
    break;

  case 209:
#line 1287 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5877 "ds_parser.cpp"
    break;

  case 210:
#line 1292 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5883 "ds_parser.cpp"
    break;

  case 211:
#line 1292 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5889 "ds_parser.cpp"
    break;

  case 212:
#line 1292 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5899 "ds_parser.cpp"
    break;

  case 213:
#line 1300 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5905 "ds_parser.cpp"
    break;

  case 214:
#line 1300 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5911 "ds_parser.cpp"
    break;

  case 215:
#line 1300 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5919 "ds_parser.cpp"
    break;

  case 216:
#line 1306 "ds_parser.ypp"
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
#line 5934 "ds_parser.cpp"
    break;

  case 217:
#line 1316 "ds_parser.ypp"
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
#line 5950 "ds_parser.cpp"
    break;

  case 218:
#line 1327 "ds_parser.ypp"
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
#line 5967 "ds_parser.cpp"
    break;

  case 219:
#line 1342 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5975 "ds_parser.cpp"
    break;

  case 220:
#line 1345 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5983 "ds_parser.cpp"
    break;

  case 221:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5989 "ds_parser.cpp"
    break;

  case 222:
#line 1352 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6001 "ds_parser.cpp"
    break;

  case 223:
#line 1359 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6014 "ds_parser.cpp"
    break;

  case 224:
#line 1370 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6020 "ds_parser.cpp"
    break;

  case 225:
#line 1371 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6026 "ds_parser.cpp"
    break;

  case 226:
#line 1372 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6032 "ds_parser.cpp"
    break;

  case 227:
#line 1376 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6038 "ds_parser.cpp"
    break;

  case 228:
#line 1377 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6044 "ds_parser.cpp"
    break;

  case 229:
#line 1378 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6050 "ds_parser.cpp"
    break;

  case 230:
#line 1379 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6056 "ds_parser.cpp"
    break;

  case 231:
#line 1383 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6066 "ds_parser.cpp"
    break;

  case 232:
#line 1388 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6076 "ds_parser.cpp"
    break;

  case 233:
#line 1396 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6082 "ds_parser.cpp"
    break;

  case 234:
#line 1397 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6088 "ds_parser.cpp"
    break;

  case 235:
#line 1401 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6098 "ds_parser.cpp"
    break;

  case 236:
#line 1407 "ds_parser.ypp"
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
#line 6163 "ds_parser.cpp"
    break;

  case 237:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6169 "ds_parser.cpp"
    break;

  case 238:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6175 "ds_parser.cpp"
    break;

  case 239:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6181 "ds_parser.cpp"
    break;

  case 240:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6187 "ds_parser.cpp"
    break;

  case 241:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6193 "ds_parser.cpp"
    break;

  case 242:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6199 "ds_parser.cpp"
    break;

  case 243:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6205 "ds_parser.cpp"
    break;

  case 244:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6211 "ds_parser.cpp"
    break;

  case 245:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6217 "ds_parser.cpp"
    break;

  case 246:
#line 1482 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6223 "ds_parser.cpp"
    break;

  case 247:
#line 1483 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6229 "ds_parser.cpp"
    break;

  case 248:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6235 "ds_parser.cpp"
    break;

  case 249:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6241 "ds_parser.cpp"
    break;

  case 250:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6247 "ds_parser.cpp"
    break;

  case 251:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6253 "ds_parser.cpp"
    break;

  case 252:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6259 "ds_parser.cpp"
    break;

  case 253:
#line 1489 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6265 "ds_parser.cpp"
    break;

  case 254:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6271 "ds_parser.cpp"
    break;

  case 255:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6277 "ds_parser.cpp"
    break;

  case 256:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6283 "ds_parser.cpp"
    break;

  case 257:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6289 "ds_parser.cpp"
    break;

  case 258:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6295 "ds_parser.cpp"
    break;

  case 259:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6301 "ds_parser.cpp"
    break;

  case 260:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6307 "ds_parser.cpp"
    break;

  case 261:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp"
    break;

  case 262:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6319 "ds_parser.cpp"
    break;

  case 263:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6325 "ds_parser.cpp"
    break;

  case 264:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6331 "ds_parser.cpp"
    break;

  case 265:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp"
    break;

  case 266:
#line 1508 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6349 "ds_parser.cpp"
    break;

  case 267:
#line 1519 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6359 "ds_parser.cpp"
    break;

  case 268:
#line 1524 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6371 "ds_parser.cpp"
    break;

  case 269:
#line 1534 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6380 "ds_parser.cpp"
    break;

  case 270:
#line 1538 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6386 "ds_parser.cpp"
    break;

  case 271:
#line 1538 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6392 "ds_parser.cpp"
    break;

  case 272:
#line 1538 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6404 "ds_parser.cpp"
    break;

  case 273:
#line 1545 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6410 "ds_parser.cpp"
    break;

  case 274:
#line 1545 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6416 "ds_parser.cpp"
    break;

  case 275:
#line 1545 "ds_parser.ypp"
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
#line 6432 "ds_parser.cpp"
    break;

  case 276:
#line 1559 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6441 "ds_parser.cpp"
    break;

  case 277:
#line 1563 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6450 "ds_parser.cpp"
    break;

  case 278:
#line 1567 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6456 "ds_parser.cpp"
    break;

  case 279:
#line 1567 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6462 "ds_parser.cpp"
    break;

  case 280:
#line 1567 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6471 "ds_parser.cpp"
    break;

  case 281:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6477 "ds_parser.cpp"
    break;

  case 282:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6483 "ds_parser.cpp"
    break;

  case 283:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6489 "ds_parser.cpp"
    break;

  case 284:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6495 "ds_parser.cpp"
    break;

  case 285:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6501 "ds_parser.cpp"
    break;

  case 286:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6507 "ds_parser.cpp"
    break;

  case 287:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6513 "ds_parser.cpp"
    break;

  case 288:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6519 "ds_parser.cpp"
    break;

  case 289:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6525 "ds_parser.cpp"
    break;

  case 290:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6531 "ds_parser.cpp"
    break;

  case 291:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6537 "ds_parser.cpp"
    break;

  case 292:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6543 "ds_parser.cpp"
    break;

  case 293:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6549 "ds_parser.cpp"
    break;

  case 294:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6555 "ds_parser.cpp"
    break;

  case 295:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6561 "ds_parser.cpp"
    break;

  case 296:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6567 "ds_parser.cpp"
    break;

  case 297:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6573 "ds_parser.cpp"
    break;

  case 298:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6579 "ds_parser.cpp"
    break;

  case 299:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 300:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6591 "ds_parser.cpp"
    break;

  case 301:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 302:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 303:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 304:
#line 1597 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 305:
#line 1598 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6621 "ds_parser.cpp"
    break;

  case 306:
#line 1599 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6627 "ds_parser.cpp"
    break;

  case 307:
#line 1600 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6633 "ds_parser.cpp"
    break;

  case 308:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6639 "ds_parser.cpp"
    break;

  case 309:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6645 "ds_parser.cpp"
    break;

  case 310:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6651 "ds_parser.cpp"
    break;

  case 311:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6657 "ds_parser.cpp"
    break;

  case 312:
#line 1605 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6663 "ds_parser.cpp"
    break;

  case 313:
#line 1606 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6669 "ds_parser.cpp"
    break;

  case 314:
#line 1607 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6675 "ds_parser.cpp"
    break;

  case 315:
#line 1608 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6681 "ds_parser.cpp"
    break;

  case 316:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6687 "ds_parser.cpp"
    break;

  case 317:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6693 "ds_parser.cpp"
    break;

  case 318:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6699 "ds_parser.cpp"
    break;

  case 319:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6705 "ds_parser.cpp"
    break;

  case 320:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6711 "ds_parser.cpp"
    break;

  case 321:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6717 "ds_parser.cpp"
    break;

  case 322:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6723 "ds_parser.cpp"
    break;

  case 323:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6729 "ds_parser.cpp"
    break;

  case 324:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6735 "ds_parser.cpp"
    break;

  case 325:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6741 "ds_parser.cpp"
    break;

  case 326:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6747 "ds_parser.cpp"
    break;

  case 327:
#line 1620 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6756 "ds_parser.cpp"
    break;

  case 328:
#line 1624 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6765 "ds_parser.cpp"
    break;

  case 329:
#line 1628 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6773 "ds_parser.cpp"
    break;

  case 330:
#line 1631 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6781 "ds_parser.cpp"
    break;

  case 331:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6787 "ds_parser.cpp"
    break;

  case 332:
#line 1635 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6793 "ds_parser.cpp"
    break;

  case 333:
#line 1636 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6799 "ds_parser.cpp"
    break;

  case 334:
#line 1637 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6813 "ds_parser.cpp"
    break;

  case 335:
#line 1646 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6827 "ds_parser.cpp"
    break;

  case 336:
#line 1655 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6833 "ds_parser.cpp"
    break;

  case 337:
#line 1656 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6841 "ds_parser.cpp"
    break;

  case 338:
#line 1659 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6847 "ds_parser.cpp"
    break;

  case 339:
#line 1659 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6853 "ds_parser.cpp"
    break;

  case 340:
#line 1659 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6861 "ds_parser.cpp"
    break;

  case 341:
#line 1662 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6870 "ds_parser.cpp"
    break;

  case 342:
#line 1666 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6879 "ds_parser.cpp"
    break;

  case 343:
#line 1670 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6888 "ds_parser.cpp"
    break;

  case 344:
#line 1674 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6894 "ds_parser.cpp"
    break;

  case 345:
#line 1675 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6900 "ds_parser.cpp"
    break;

  case 346:
#line 1676 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6906 "ds_parser.cpp"
    break;

  case 347:
#line 1677 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6912 "ds_parser.cpp"
    break;

  case 348:
#line 1678 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6918 "ds_parser.cpp"
    break;

  case 349:
#line 1679 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6924 "ds_parser.cpp"
    break;

  case 350:
#line 1680 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6930 "ds_parser.cpp"
    break;

  case 351:
#line 1681 "ds_parser.ypp"
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
#line 6951 "ds_parser.cpp"
    break;

  case 352:
#line 1697 "ds_parser.ypp"
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
#line 6972 "ds_parser.cpp"
    break;

  case 353:
#line 1713 "ds_parser.ypp"
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
#line 7032 "ds_parser.cpp"
    break;

  case 354:
#line 1771 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7038 "ds_parser.cpp"
    break;

  case 355:
#line 1772 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7044 "ds_parser.cpp"
    break;

  case 356:
#line 1776 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7050 "ds_parser.cpp"
    break;

  case 357:
#line 1777 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7056 "ds_parser.cpp"
    break;

  case 358:
#line 1781 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7062 "ds_parser.cpp"
    break;

  case 359:
#line 1782 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7068 "ds_parser.cpp"
    break;

  case 360:
#line 1786 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7074 "ds_parser.cpp"
    break;

  case 361:
#line 1787 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7080 "ds_parser.cpp"
    break;

  case 362:
#line 1788 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7086 "ds_parser.cpp"
    break;

  case 363:
#line 1792 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7097 "ds_parser.cpp"
    break;

  case 364:
#line 1801 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7105 "ds_parser.cpp"
    break;

  case 365:
#line 1804 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7114 "ds_parser.cpp"
    break;

  case 366:
#line 1809 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7120 "ds_parser.cpp"
    break;

  case 367:
#line 1809 "ds_parser.ypp"
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
#line 7170 "ds_parser.cpp"
    break;

  case 368:
#line 1856 "ds_parser.ypp"
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
                if ( (yyvsp[-3].b) ) {
                    finit = new ExprCast((yyvsp[-1].pFuncDecl)->at, finit, make_smart<TypeDecl>(Type::autoinfer));
                }
                VariableDeclaration * decl = new VariableDeclaration(
                    vars,
                    new TypeDecl(Type::autoinfer),
                    finit
                );
                decl->override = (yyvsp[-3].b);
                decl->isPrivate = (yyvsp[-4].b);
                (yyvsp[-7].pVarDeclList)->push_back(decl);
                modifyToClassMember((yyvsp[-1].pFuncDecl), g_thisStructure, false, (yyvsp[-2].b));
            } else {
                if ( (yyvsp[-3].b) ) {
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
#line 7238 "ds_parser.cpp"
    break;

  case 369:
#line 1919 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7249 "ds_parser.cpp"
    break;

  case 370:
#line 1928 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7259 "ds_parser.cpp"
    break;

  case 371:
#line 1933 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7269 "ds_parser.cpp"
    break;

  case 372:
#line 1941 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7275 "ds_parser.cpp"
    break;

  case 373:
#line 1942 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7281 "ds_parser.cpp"
    break;

  case 374:
#line 1946 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7289 "ds_parser.cpp"
    break;

  case 375:
#line 1949 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7300 "ds_parser.cpp"
    break;

  case 376:
#line 1958 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7306 "ds_parser.cpp"
    break;

  case 377:
#line 1959 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7312 "ds_parser.cpp"
    break;

  case 378:
#line 1963 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7323 "ds_parser.cpp"
    break;

  case 379:
#line 1972 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7329 "ds_parser.cpp"
    break;

  case 380:
#line 1973 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7335 "ds_parser.cpp"
    break;

  case 381:
#line 1978 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7341 "ds_parser.cpp"
    break;

  case 382:
#line 1979 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7347 "ds_parser.cpp"
    break;

  case 383:
#line 1983 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7358 "ds_parser.cpp"
    break;

  case 384:
#line 1989 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7369 "ds_parser.cpp"
    break;

  case 385:
#line 1995 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7377 "ds_parser.cpp"
    break;

  case 386:
#line 1998 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7386 "ds_parser.cpp"
    break;

  case 387:
#line 2002 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7397 "ds_parser.cpp"
    break;

  case 388:
#line 2008 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7408 "ds_parser.cpp"
    break;

  case 389:
#line 2017 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7414 "ds_parser.cpp"
    break;

  case 390:
#line 2018 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7420 "ds_parser.cpp"
    break;

  case 391:
#line 2019 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7426 "ds_parser.cpp"
    break;

  case 392:
#line 2023 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7432 "ds_parser.cpp"
    break;

  case 393:
#line 2024 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7438 "ds_parser.cpp"
    break;

  case 394:
#line 2028 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7446 "ds_parser.cpp"
    break;

  case 395:
#line 2031 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7456 "ds_parser.cpp"
    break;

  case 396:
#line 2036 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7466 "ds_parser.cpp"
    break;

  case 397:
#line 2041 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7479 "ds_parser.cpp"
    break;

  case 398:
#line 2049 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7492 "ds_parser.cpp"
    break;

  case 399:
#line 2060 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7500 "ds_parser.cpp"
    break;

  case 400:
#line 2063 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7510 "ds_parser.cpp"
    break;

  case 401:
#line 2071 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7516 "ds_parser.cpp"
    break;

  case 402:
#line 2072 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7522 "ds_parser.cpp"
    break;

  case 403:
#line 2076 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7528 "ds_parser.cpp"
    break;

  case 404:
#line 2077 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7534 "ds_parser.cpp"
    break;

  case 405:
#line 2078 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7540 "ds_parser.cpp"
    break;

  case 406:
#line 2082 "ds_parser.ypp"
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
#line 7573 "ds_parser.cpp"
    break;

  case 407:
#line 2110 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7579 "ds_parser.cpp"
    break;

  case 408:
#line 2110 "ds_parser.ypp"
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
#line 7615 "ds_parser.cpp"
    break;

  case 409:
#line 2144 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7623 "ds_parser.cpp"
    break;

  case 410:
#line 2147 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7637 "ds_parser.cpp"
    break;

  case 411:
#line 2156 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7651 "ds_parser.cpp"
    break;

  case 412:
#line 2169 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7665 "ds_parser.cpp"
    break;

  case 417:
#line 2191 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7671 "ds_parser.cpp"
    break;

  case 418:
#line 2192 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7677 "ds_parser.cpp"
    break;

  case 419:
#line 2193 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7683 "ds_parser.cpp"
    break;

  case 420:
#line 2197 "ds_parser.ypp"
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
#line 7716 "ds_parser.cpp"
    break;

  case 421:
#line 2225 "ds_parser.ypp"
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
#line 7750 "ds_parser.cpp"
    break;

  case 422:
#line 2257 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7756 "ds_parser.cpp"
    break;

  case 423:
#line 2258 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7762 "ds_parser.cpp"
    break;

  case 424:
#line 2262 "ds_parser.ypp"
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
#line 7802 "ds_parser.cpp"
    break;

  case 425:
#line 2300 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7808 "ds_parser.cpp"
    break;

  case 426:
#line 2301 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7814 "ds_parser.cpp"
    break;

  case 427:
#line 2305 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7820 "ds_parser.cpp"
    break;

  case 428:
#line 2306 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7826 "ds_parser.cpp"
    break;

  case 429:
#line 2307 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7832 "ds_parser.cpp"
    break;

  case 430:
#line 2312 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7838 "ds_parser.cpp"
    break;

  case 431:
#line 2312 "ds_parser.ypp"
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
                        }
                    } else {
                        if ( pDecl->override ) {
                                auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                                oldFd->init = init;
                                oldFd->parentType = oldFd->type->isAuto();
                                oldFd->privateField = pDecl->isPrivate;
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
#line 7930 "ds_parser.cpp"
    break;

  case 432:
#line 2402 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7942 "ds_parser.cpp"
    break;

  case 433:
#line 2409 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7953 "ds_parser.cpp"
    break;

  case 434:
#line 2418 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7959 "ds_parser.cpp"
    break;

  case 435:
#line 2419 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7965 "ds_parser.cpp"
    break;

  case 436:
#line 2420 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7971 "ds_parser.cpp"
    break;

  case 437:
#line 2421 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7977 "ds_parser.cpp"
    break;

  case 438:
#line 2422 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7983 "ds_parser.cpp"
    break;

  case 439:
#line 2423 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7989 "ds_parser.cpp"
    break;

  case 440:
#line 2424 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7995 "ds_parser.cpp"
    break;

  case 441:
#line 2425 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 8001 "ds_parser.cpp"
    break;

  case 442:
#line 2426 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8007 "ds_parser.cpp"
    break;

  case 443:
#line 2427 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8013 "ds_parser.cpp"
    break;

  case 444:
#line 2428 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8019 "ds_parser.cpp"
    break;

  case 445:
#line 2429 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8025 "ds_parser.cpp"
    break;

  case 446:
#line 2430 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8031 "ds_parser.cpp"
    break;

  case 447:
#line 2431 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8037 "ds_parser.cpp"
    break;

  case 448:
#line 2432 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8043 "ds_parser.cpp"
    break;

  case 449:
#line 2433 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8049 "ds_parser.cpp"
    break;

  case 450:
#line 2434 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8055 "ds_parser.cpp"
    break;

  case 451:
#line 2435 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8061 "ds_parser.cpp"
    break;

  case 452:
#line 2436 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8067 "ds_parser.cpp"
    break;

  case 453:
#line 2437 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8073 "ds_parser.cpp"
    break;

  case 454:
#line 2438 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8079 "ds_parser.cpp"
    break;

  case 455:
#line 2439 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8085 "ds_parser.cpp"
    break;

  case 456:
#line 2440 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8091 "ds_parser.cpp"
    break;

  case 457:
#line 2441 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8097 "ds_parser.cpp"
    break;

  case 458:
#line 2442 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8103 "ds_parser.cpp"
    break;

  case 459:
#line 2446 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8109 "ds_parser.cpp"
    break;

  case 460:
#line 2447 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8115 "ds_parser.cpp"
    break;

  case 461:
#line 2448 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8121 "ds_parser.cpp"
    break;

  case 462:
#line 2449 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8127 "ds_parser.cpp"
    break;

  case 463:
#line 2450 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8133 "ds_parser.cpp"
    break;

  case 464:
#line 2451 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8139 "ds_parser.cpp"
    break;

  case 465:
#line 2455 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8152 "ds_parser.cpp"
    break;

  case 466:
#line 2466 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8161 "ds_parser.cpp"
    break;

  case 467:
#line 2470 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8173 "ds_parser.cpp"
    break;

  case 468:
#line 2480 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8185 "ds_parser.cpp"
    break;

  case 469:
#line 2487 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8196 "ds_parser.cpp"
    break;

  case 470:
#line 2496 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8202 "ds_parser.cpp"
    break;

  case 471:
#line 2496 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8208 "ds_parser.cpp"
    break;

  case 472:
#line 2496 "ds_parser.ypp"
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
#line 8223 "ds_parser.cpp"
    break;

  case 473:
#line 2509 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8229 "ds_parser.cpp"
    break;

  case 474:
#line 2510 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8235 "ds_parser.cpp"
    break;

  case 475:
#line 2511 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8241 "ds_parser.cpp"
    break;

  case 476:
#line 2512 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8247 "ds_parser.cpp"
    break;

  case 477:
#line 2513 "ds_parser.ypp"
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
#line 8266 "ds_parser.cpp"
    break;

  case 478:
#line 2527 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8277 "ds_parser.cpp"
    break;

  case 479:
#line 2533 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8286 "ds_parser.cpp"
    break;

  case 480:
#line 2537 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8295 "ds_parser.cpp"
    break;

  case 481:
#line 2541 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8305 "ds_parser.cpp"
    break;

  case 482:
#line 2546 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8315 "ds_parser.cpp"
    break;

  case 483:
#line 2551 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8325 "ds_parser.cpp"
    break;

  case 484:
#line 2556 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8335 "ds_parser.cpp"
    break;

  case 485:
#line 2561 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8344 "ds_parser.cpp"
    break;

  case 486:
#line 2565 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8353 "ds_parser.cpp"
    break;

  case 487:
#line 2569 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8363 "ds_parser.cpp"
    break;

  case 488:
#line 2574 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8372 "ds_parser.cpp"
    break;

  case 489:
#line 2578 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8382 "ds_parser.cpp"
    break;

  case 490:
#line 2583 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8388 "ds_parser.cpp"
    break;

  case 491:
#line 2583 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8394 "ds_parser.cpp"
    break;

  case 492:
#line 2583 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8405 "ds_parser.cpp"
    break;

  case 493:
#line 2589 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8417 "ds_parser.cpp"
    break;

  case 494:
#line 2596 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8423 "ds_parser.cpp"
    break;

  case 495:
#line 2596 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8429 "ds_parser.cpp"
    break;

  case 496:
#line 2596 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8439 "ds_parser.cpp"
    break;

  case 497:
#line 2601 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8445 "ds_parser.cpp"
    break;

  case 498:
#line 2601 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8451 "ds_parser.cpp"
    break;

  case 499:
#line 2601 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8462 "ds_parser.cpp"
    break;

  case 500:
#line 2607 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8468 "ds_parser.cpp"
    break;

  case 501:
#line 2607 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8474 "ds_parser.cpp"
    break;

  case 502:
#line 2607 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8484 "ds_parser.cpp"
    break;

  case 503:
#line 2612 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8493 "ds_parser.cpp"
    break;

  case 504:
#line 2616 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8499 "ds_parser.cpp"
    break;

  case 505:
#line 2616 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8505 "ds_parser.cpp"
    break;

  case 506:
#line 2616 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8515 "ds_parser.cpp"
    break;

  case 507:
#line 2621 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8521 "ds_parser.cpp"
    break;

  case 508:
#line 2621 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8527 "ds_parser.cpp"
    break;

  case 509:
#line 2621 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8541 "ds_parser.cpp"
    break;

  case 510:
#line 2630 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8550 "ds_parser.cpp"
    break;

  case 511:
#line 2634 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8556 "ds_parser.cpp"
    break;

  case 512:
#line 2634 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8562 "ds_parser.cpp"
    break;

  case 513:
#line 2634 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8572 "ds_parser.cpp"
    break;

  case 514:
#line 2639 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8578 "ds_parser.cpp"
    break;

  case 515:
#line 2639 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8584 "ds_parser.cpp"
    break;

  case 516:
#line 2639 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8598 "ds_parser.cpp"
    break;

  case 517:
#line 2648 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8607 "ds_parser.cpp"
    break;

  case 518:
#line 2652 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8613 "ds_parser.cpp"
    break;

  case 519:
#line 2652 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8619 "ds_parser.cpp"
    break;

  case 520:
#line 2652 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8629 "ds_parser.cpp"
    break;

  case 521:
#line 2657 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8635 "ds_parser.cpp"
    break;

  case 522:
#line 2657 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8641 "ds_parser.cpp"
    break;

  case 523:
#line 2657 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8655 "ds_parser.cpp"
    break;

  case 524:
#line 2666 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8661 "ds_parser.cpp"
    break;

  case 525:
#line 2666 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8667 "ds_parser.cpp"
    break;

  case 526:
#line 2666 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8678 "ds_parser.cpp"
    break;

  case 527:
#line 2672 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8684 "ds_parser.cpp"
    break;

  case 528:
#line 2672 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8690 "ds_parser.cpp"
    break;

  case 529:
#line 2672 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8701 "ds_parser.cpp"
    break;

  case 530:
#line 2681 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8707 "ds_parser.cpp"
    break;

  case 531:
#line 2681 "ds_parser.ypp"
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
#line 8724 "ds_parser.cpp"
    break;

  case 532:
#line 2696 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8730 "ds_parser.cpp"
    break;

  case 533:
#line 2696 "ds_parser.ypp"
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
#line 8751 "ds_parser.cpp"
    break;

  case 534:
#line 2716 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8757 "ds_parser.cpp"
    break;

  case 535:
#line 2717 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8763 "ds_parser.cpp"
    break;

  case 536:
#line 2718 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8769 "ds_parser.cpp"
    break;

  case 537:
#line 2719 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8775 "ds_parser.cpp"
    break;

  case 538:
#line 2723 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8787 "ds_parser.cpp"
    break;

  case 539:
#line 2730 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8799 "ds_parser.cpp"
    break;

  case 540:
#line 2737 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8810 "ds_parser.cpp"
    break;

  case 541:
#line 2743 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8821 "ds_parser.cpp"
    break;

  case 542:
#line 2752 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8831 "ds_parser.cpp"
    break;

  case 543:
#line 2757 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8840 "ds_parser.cpp"
    break;

  case 544:
#line 2764 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8846 "ds_parser.cpp"
    break;

  case 545:
#line 2765 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8852 "ds_parser.cpp"
    break;

  case 546:
#line 2769 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8863 "ds_parser.cpp"
    break;

  case 547:
#line 2775 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8875 "ds_parser.cpp"
    break;

  case 548:
#line 2782 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8888 "ds_parser.cpp"
    break;

  case 549:
#line 2790 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8900 "ds_parser.cpp"
    break;

  case 550:
#line 2797 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8913 "ds_parser.cpp"
    break;

  case 551:
#line 2805 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8927 "ds_parser.cpp"
    break;

  case 552:
#line 2817 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8935 "ds_parser.cpp"
    break;

  case 553:
#line 2820 "ds_parser.ypp"
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
#line 8951 "ds_parser.cpp"
    break;

  case 554:
#line 2834 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8962 "ds_parser.cpp"
    break;

  case 555:
#line 2843 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8968 "ds_parser.cpp"
    break;

  case 556:
#line 2844 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8974 "ds_parser.cpp"
    break;

  case 557:
#line 2848 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8984 "ds_parser.cpp"
    break;

  case 558:
#line 2853 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8993 "ds_parser.cpp"
    break;

  case 559:
#line 2860 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9003 "ds_parser.cpp"
    break;

  case 560:
#line 2865 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9015 "ds_parser.cpp"
    break;

  case 561:
#line 2875 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9025 "ds_parser.cpp"
    break;

  case 562:
#line 2880 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9034 "ds_parser.cpp"
    break;

  case 563:
#line 2887 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9048 "ds_parser.cpp"
    break;

  case 564:
#line 2899 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9054 "ds_parser.cpp"
    break;

  case 565:
#line 2900 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9060 "ds_parser.cpp"
    break;

  case 566:
#line 2904 "ds_parser.ypp"
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
#line 9083 "ds_parser.cpp"
    break;

  case 567:
#line 2922 "ds_parser.ypp"
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
#line 9106 "ds_parser.cpp"
    break;


#line 9110 "ds_parser.cpp"

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
#line 2942 "ds_parser.ypp"


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
