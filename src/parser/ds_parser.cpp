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
#define YYLAST   7355

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  178
/* YYNRULES -- Number of rules.  */
#define YYNRULES  548
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  958

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
     545,   550,   558,   558,   588,   610,   614,   617,   621,   627,
     636,   657,   680,   681,   685,   689,   690,   694,   697,   703,
     709,   712,   718,   719,   723,   724,   725,   734,   735,   739,
     748,   764,   772,   782,   791,   792,   793,   794,   795,   796,
     800,   805,   813,   814,   815,   816,   817,   818,   819,   820,
     826,   840,   857,   858,   859,   863,   871,   884,   888,   895,
     896,   900,   901,   902,   906,   909,   916,   920,   921,   922,
     923,   924,   925,   926,   927,   928,   929,   930,   931,   932,
     933,   934,   935,   936,   937,   938,   939,   940,   941,   942,
     943,   944,   945,   946,   947,   948,   949,   950,   951,   952,
     953,   954,   955,   956,   957,   958,   959,   960,   961,   965,
     987,   988,   989,   993,  1032,  1039,  1043,  1054,  1055,  1056,
    1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,
    1067,  1068,  1069,  1070,  1071,  1072,  1076,  1081,  1087,  1093,
    1127,  1130,  1136,  1137,  1148,  1152,  1158,  1161,  1164,  1168,
    1174,  1178,  1182,  1185,  1188,  1193,  1196,  1204,  1207,  1212,
    1215,  1223,  1229,  1230,  1234,  1268,  1268,  1268,  1271,  1271,
    1271,  1276,  1276,  1276,  1284,  1284,  1284,  1290,  1300,  1311,
    1326,  1329,  1335,  1336,  1343,  1354,  1355,  1356,  1360,  1361,
    1362,  1363,  1367,  1372,  1380,  1381,  1385,  1390,  1454,  1455,
    1456,  1457,  1458,  1459,  1463,  1464,  1465,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,
    1479,  1480,  1481,  1485,  1486,  1487,  1488,  1492,  1503,  1508,
    1518,  1522,  1522,  1522,  1529,  1529,  1529,  1543,  1547,  1547,
    1547,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,  1562,
    1563,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,
    1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,  1591,  1592,
    1593,  1594,  1595,  1596,  1597,  1601,  1605,  1608,  1611,  1612,
    1613,  1614,  1623,  1632,  1633,  1636,  1636,  1636,  1639,  1643,
    1647,  1651,  1652,  1653,  1654,  1655,  1656,  1657,  1658,  1674,
    1690,  1748,  1749,  1753,  1754,  1758,  1766,  1769,  1774,  1773,
    1818,  1874,  1883,  1888,  1896,  1897,  1901,  1904,  1913,  1914,
    1918,  1927,  1928,  1933,  1934,  1938,  1944,  1950,  1953,  1957,
    1963,  1972,  1973,  1974,  1978,  1979,  1983,  1986,  1991,  1996,
    2004,  2015,  2018,  2025,  2026,  2030,  2031,  2032,  2036,  2064,
    2064,  2094,  2097,  2106,  2119,  2131,  2132,  2136,  2140,  2141,
    2142,  2146,  2174,  2206,  2207,  2211,  2248,  2249,  2253,  2254,
    2255,  2260,  2259,  2344,  2351,  2360,  2361,  2362,  2363,  2364,
    2365,  2366,  2367,  2368,  2369,  2370,  2371,  2372,  2373,  2374,
    2375,  2376,  2377,  2378,  2379,  2380,  2381,  2382,  2383,  2384,
    2388,  2389,  2390,  2391,  2392,  2393,  2397,  2408,  2412,  2422,
    2429,  2438,  2438,  2438,  2451,  2452,  2453,  2454,  2455,  2469,
    2475,  2479,  2483,  2488,  2493,  2498,  2503,  2507,  2511,  2516,
    2520,  2525,  2525,  2525,  2531,  2538,  2538,  2538,  2543,  2543,
    2543,  2549,  2549,  2549,  2554,  2558,  2558,  2558,  2563,  2563,
    2563,  2572,  2576,  2576,  2576,  2581,  2581,  2581,  2590,  2594,
    2594,  2594,  2599,  2599,  2599,  2608,  2608,  2608,  2614,  2614,
    2614,  2623,  2623,  2638,  2638,  2658,  2659,  2660,  2661,  2665,
    2672,  2679,  2685,  2694,  2699,  2706,  2707,  2711,  2717,  2724,
    2732,  2739,  2747,  2759,  2762,  2776,  2785,  2786,  2790,  2795,
    2802,  2807,  2817,  2822,  2829,  2841,  2842,  2846,  2864
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

#define YYPACT_NINF -607

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-607)))

#define YYTABLE_NINF -503

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -607,    27,  -607,  -607,   -61,   -64,   -74,   -30,  -607,   -63,
    -607,  -607,    15,  -607,  -607,  -607,  -607,  -607,   194,  -607,
      56,  -607,  -607,  -607,  -607,  -607,  -607,    33,  -607,   -13,
       8,    39,  -607,  -607,   -74,     2,  -607,    88,  -607,  -607,
      56,   119,   163,  -607,  -607,   174,   198,   162,  -607,   -15,
    -607,  -607,  -607,   181,   212,   239,  -607,   271,    23,   -61,
     206,   -64,   178,   210,  -607,   211,   214,  -607,   207,  -607,
     -94,   273,   182,   187,  -607,   217,   -61,    15,  -607,  -607,
    -607,    10,   189,  -607,  -607,  -607,   230,  -607,  -607,   233,
    -607,  -607,   196,  -607,  -607,  -607,  -607,  -607,   191,    75,
    -607,  -607,  -607,  -607,   326,  -607,  -607,  1075,  -607,  -607,
    -607,  -607,  -607,   243,   245,  -607,   -24,  -607,  7185,   360,
     361,  -607,   218,  -607,  -607,   -99,   240,  -607,  -607,   254,
    -607,  -607,   204,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
     -23,  -607,   241,   242,   244,   246,  -607,  -607,  -607,   221,
    -607,  -607,  -607,  -607,  -607,   247,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,   248,  -607,  -607,  -607,   249,
     250,  -607,  -607,  -607,  -607,   251,   253,  -607,  -607,  -607,
    -607,  -607,   480,  -607,   220,   256,  -607,   222,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,   -69,   258,  1201,   190,  -607,   109,  -607,   231,   -81,
    -607,  -607,   151,  -607,  -607,  -607,    75,  -607,  -607,  -607,
    -607,  -607,   272,  -607,    76,    97,   106,  -607,  -607,  -607,
    -607,  -607,  -607,   384,  -607,  -607,    11,  2730,  -607,  -607,
     -65,  1075,   -59,   -61,  -607,     4,  -607,   157,  1075,  -607,
    -607,  4367,  -607,  -607,   254,  -607,  -607,  7098,   264,   259,
     948,  -607,  -607,   189,  4367,   276,  4367,   260,   261,   278,
    -607,   267,   262,   284,  2869,   189,  -607,   265,  2017,  4367,
    4367,   147,   147,  6926,  7012,  4367,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  4367,  4367,  4367,   109,  4367,  4367,  4367,
    -607,   255,  -607,   390,  -607,  -607,   269,   280,  4367,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,   -62,   281,  -607,
     286,   289,  -607,  -607,  -607,   254,  -607,  -607,  -607,  -607,
     290,  -607,  -607,   -83,  -607,  -607,  -607,  -607,  -607,  6093,
     287,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,   291,   -58,  -607,  -607,  -607,  -607,   285,  1075,
    -607,   -45,  -607,  1075,  1075,  1075,  1075,   293,   243,  1075,
     218,  1075,   218,  1075,   218,  7184,   245,  -607,  -607,  -607,
     296,  -607,  -607,  5753,  -607,  -607,  4008,  -607,  -607,    37,
     254,  -607,   -60,  -607,   295,  4008,   543,     5,  1019,  -607,
     109,  -607,  2156,  -607,  -607,   358,  4457,   466,  6387,  4367,
     543,  -607,  -607,  4367,  -607,  -607,   328,   359,  6387,  -607,
    1075,  2156,  -607,  4556,   343,   343,   315,  -607,  -607,   254,
    1546,   254,  1703,  6161,  -607,    61,   186,   343,   343,   -70,
    -607,   343,   343,  5201,    82,   316,  -607,  -607,   543,  -607,
    2295,  -607,  -607,  -607,  -607,   363,   147,  -607,    16,   356,
    4367,  4367,  4367,  4367,  4367,  4367,  4367,  4367,  4367,  4367,
    -607,  -607,  4367,  4367,  4367,  4367,  4367,  4367,   357,  2452,
    4367,   362,  4367,  4367,  4367,  4367,  4367,  4367,  4367,  4367,
    4367,  4367,  4367,  4367,  4367,  4367,  2591,  3008,  4367,  4367,
    4367,  4367,  4367,  4367,  4367,  4367,  4367,  4367,   364,  4367,
    3147,  -607,   -44,  -607,     9,  -607,  1143,  -607,  -607,  -607,
    2156,  1486,  1797,  1958,  2093,  -607,    67,  2232,   258,  2389,
     258,  2528,   258,    18,  -607,    91,  4008,   108,  -607,  -607,
     325,  -607,  -607,  -607,  1075,  -607,  2156,  -607,  -607,  4367,
    3304,  3461,  1075,  -607,  4655,  4367,  4367,  -607,   189,  5279,
    -607,  1075,  1075,  5359,  1075,  -607,  -607,  2667,  -607,  4754,
    -607,  -607,    22,   147,   -53,  1860,  3600,  6161,   368,    -7,
     332,   369,  -607,  -607,    -1,    26,  3757,    -7,    99,  4367,
    4367,   330,  -607,  4367,   195,   370,  -607,   107,  -607,  -607,
     283,   366,  -607,     0,  6387,   -46,   218,  -607,   372,  -607,
    -607,  6387,  6387,  6387,  6387,  6387,  6387,  6387,  6387,   365,
     365,   748,  6387,  6387,   748,  6804,  6804,   339,  4367,  4367,
    6387,   252,  -607,  5829,   -54,   -54,  6387,   365,   365,  6387,
    6387,  6562,  6455,  6534,  6387,  6387,  6387,  4367,  4367,  6387,
     378,  6240,  6640,  6668,   329,   748,   748,    19,    19,   252,
     252,   252,  -607,   523,  5905,  -607,    50,   -43,  4367,  -607,
      15,  -607,   518,   484,   352,   384,  -607,  2156,  -607,  4853,
    -607,  1075,  -607,  -607,  -607,   393,  -607,   382,  -607,   383,
    -607,   385,  1075,  -607,  7184,  -607,   245,  -607,  3245,  -607,
    5993,   -50,  -607,    59,   394,  5437,  2806,  -607,  -607,  -607,
    2945,  3084,  -607,  3398,   363,  -607,  4367,  -607,  4367,  4367,
     -14,   400,   366,   371,   373,  4367,   374,  4367,  4367,   366,
     375,   376,  6387,  -607,  -607,  6308,  -607,   401,  1075,   218,
    1386,   189,  -607,  -607,  4367,  -607,   -42,    63,  4367,  -607,
     402,   405,   408,   410,  -607,    70,   258,  -607,  3914,  6739,
    6387,  -607,  6739,  6387,  -607,  4367,  -607,  -607,  -607,  -607,
    5051,    73,   305,  -607,   254,  -607,  -607,  4952,  -607,  -607,
    3537,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    4008,  -607,  -607,  -607,   384,  4367,   189,  -607,   120,  -607,
    -607,  -607,  -607,  -607,   377,   -32,  6387,  6387,    -7,   379,
     -26,   368,   380,  -607,  6387,  -607,  -607,   -10,    -7,   381,
    -607,  -607,  -607,  3694,   258,  -607,  -607,   543,   386,  6387,
    -607,  -607,  -607,  -607,   -46,   387,   -79,  1075,  -607,    69,
    6387,  -607,  -607,   388,  -607,   181,  -607,  -607,  -607,  -607,
    -607,  -607,  6387,  -607,  4367,   416,  -607,  4367,  4367,  4367,
    4071,  4367,   391,   395,  4367,  4367,  -607,  4367,   389,  -607,
    -607,   407,   283,  -607,  -607,  -607,  4228,  -607,  -607,  3851,
    -607,  -607,   181,  -607,  -607,  5517,   411,   748,   748,   748,
    -607,  5595,  5126,   397,  -607,  6387,  6387,  5126,   398,   109,
    -607,  -607,  4367,  6387,  -607,   403,  -607,  4367,  -607,   528,
     406,  -607,   404,  -607,  -607,   109,  6387,  -607,  -607,  5675,
    4367,   409,   413,  -607,  -607,  6387,  -607,  -607
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    89,     1,   192,     0,     0,     0,     0,   193,     0,
     513,   511,     0,    14,     3,    10,     9,     8,     0,     7,
     383,     6,    11,     5,     4,    12,    13,    78,    80,    34,
      47,    44,    45,    36,     0,    42,    35,     0,    19,    18,
     383,     0,     0,    83,    84,     0,   172,    85,    87,     0,
      82,   407,   406,   140,   398,   408,   384,   385,     0,     0,
       0,     0,    37,     0,    43,     0,     0,    40,     0,   395,
       0,    15,     0,     0,   174,     0,     0,     0,    90,   142,
     141,     0,     0,   139,   400,   399,     0,   410,   409,     0,
     387,   386,   389,    76,    77,    74,    75,    73,     0,     0,
      72,    81,    48,    46,    42,    39,    38,     0,   397,   396,
      16,    17,    20,     0,     0,   173,     0,    88,     0,     0,
       0,    96,    91,   166,   144,     0,   403,   411,   381,     0,
      21,    22,     0,    68,    69,    66,    67,    65,    64,    70,
       0,    41,     0,     0,     0,     0,   415,   435,   416,   447,
     417,   421,   422,   423,   424,   439,   428,   429,   430,   431,
     432,   433,   434,   436,   437,   484,   420,   427,   438,   491,
     498,   418,   425,   419,   426,     0,     0,   446,   454,   457,
     455,   456,     0,   449,     0,     0,   361,     0,    86,    99,
     100,   102,   101,   103,   104,   105,   106,   131,   132,   129,
     130,   122,   133,   134,   123,   120,   121,   135,   136,   137,
     138,   110,   111,   112,   107,   108,   109,   125,   126,   124,
     118,   119,   114,   113,   115,   116,   117,    98,    97,   127,
     128,   341,    94,     0,     0,   391,     0,   405,     0,     0,
     413,   390,   374,    23,    24,    25,     0,    79,   475,   478,
     481,   471,     0,   451,   485,   492,   499,   505,   508,   462,
     467,   461,   474,     0,   470,   464,     0,     0,   394,   466,
       0,     0,     0,     0,    92,     0,   354,     0,     0,   143,
     168,     0,    57,    58,     0,   277,   278,     0,     0,     0,
       0,   271,   180,     0,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,   230,   229,   231,
     232,   233,    26,     0,     0,     0,     0,     0,     0,     0,
     215,   216,   147,   145,   275,   274,     0,     0,     0,   161,
     156,   154,   153,   155,   226,   167,   148,   272,     0,   334,
       0,     0,   159,   160,   162,     0,   152,   333,   332,   331,
      89,   337,   273,     0,   149,   336,   335,   313,   279,   234,
       0,   276,   515,   516,   517,   518,   440,   443,   441,   444,
     442,   445,     0,     0,   404,   346,   388,   382,     0,     0,
     375,     0,    71,     0,     0,     0,     0,     0,     0,     0,
      91,     0,    91,     0,    91,     0,     0,   469,   463,   465,
       0,   468,   459,     0,   450,   514,   360,   512,   362,     0,
       0,   352,   365,    93,   341,    95,     0,     0,   176,   179,
       0,   204,     0,   182,   185,     0,   234,     0,   175,     0,
       0,   195,   198,     0,   165,   201,     0,     0,    51,    61,
       0,     0,   189,   234,   305,   306,   216,   170,   171,     0,
     525,     0,     0,     0,   542,     0,     0,   283,   282,   318,
      32,   281,   280,     0,   217,     0,   163,   164,     0,   340,
       0,   151,   157,   158,   194,   224,     0,   150,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,   391,     0,   401,   341,   414,     0,   372,   373,   371,
       0,     0,     0,     0,     0,   448,     0,     0,    94,     0,
      94,     0,    94,   172,   358,     0,   356,     0,   460,   458,
       0,   353,   364,   363,     0,   366,     0,   355,    62,     0,
       0,     0,     0,   186,   234,     0,     0,   183,     0,     0,
      63,     0,     0,     0,     0,    49,    50,     0,   190,   234,
     187,   217,     0,     0,   172,     0,     0,   533,   523,   525,
       0,   536,   537,   538,     0,     0,     0,   525,     0,     0,
       0,     0,    29,     0,    27,     0,   309,   261,   260,   166,
      54,     0,   314,     0,   210,     0,    91,   169,     0,   328,
     329,   244,   245,   247,   246,   248,   238,   239,   240,   284,
     285,   297,   249,   250,   298,   295,   296,     0,     0,     0,
     236,   323,   312,     0,   338,   339,   237,   286,   287,   251,
     252,   302,   303,   304,   241,   242,   243,     0,     0,   235,
       0,     0,   300,   301,   299,   293,   294,   289,   288,   290,
     291,   292,   267,     0,     0,   316,     0,     0,     0,   392,
       0,   412,     0,   343,     0,   371,   376,     0,   380,   234,
     476,     0,   482,   472,   452,     0,   486,     0,   493,     0,
     500,     0,     0,   506,     0,   509,     0,   342,   367,   370,
     369,     0,   177,     0,     0,     0,     0,   184,   191,   319,
       0,     0,   320,     0,   224,   188,     0,   526,     0,     0,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,   543,   544,     0,    30,    33,     0,    91,
       0,     0,    52,    53,     0,    59,     0,     0,     0,   315,
       0,     0,     0,     0,   222,     0,    94,   325,     0,   255,
     256,   311,   253,   254,   330,     0,   269,   310,   317,   402,
       0,     0,   343,   344,     0,   347,   378,   234,   379,   477,
       0,   483,   473,   453,   487,   489,   494,   496,   501,   503,
     357,   507,   359,   510,   363,     0,     0,   178,     0,   207,
     205,   196,   199,   202,     0,     0,   520,   519,   525,     0,
       0,   524,     0,   528,   534,   540,   539,     0,   525,     0,
     541,    28,    31,     0,    94,   146,    55,     0,     0,   211,
     220,   221,   219,   218,     0,     0,     0,     0,   258,     0,
     324,   270,   393,    90,   348,   140,   345,   377,   479,   490,
     497,   504,   368,    60,     0,     0,   206,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,     0,   531,
     262,     0,    54,   257,   223,   225,     0,   212,   227,     0,
     259,   351,   140,   350,   480,     0,     0,   197,   200,   203,
     321,     0,   545,     0,   529,   522,   521,   545,     0,     0,
     265,    56,     0,   213,   326,     0,   208,     0,   322,     0,
       0,   530,     0,   532,   263,     0,   214,   327,   349,     0,
       0,     0,     0,   266,   209,   546,   547,   548
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -607,  -607,  -607,  -607,  -607,   112,   525,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,   551,  -607,   488,  -607,  -607,   527,
    -607,  -607,  -607,  -309,  -607,  -607,  -607,  -607,  -607,  -607,
     351,  -607,   539,   -56,  -607,   522,  -100,  -338,  -393,  -541,
    -607,  -607,  -607,  -311,  -273,   -80,  -607,   -22,  -280,   -12,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,   602,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -527,  -607,  -607,  -258,  -607,  -133,  -281,  -607,  -230,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
     300,  -607,  -607,    68,  -189,  -607,  -607,  -607,   193,  -607,
    -110,  -607,  -268,   213,  -416,  -412,    74,  -607,  -207,  -607,
     581,  -607,  -607,  -607,    81,   556,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -270,  -106,  -607,  -607,
    -607,   232,  -607,  -607,  -607,   -78,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,   344,  -606,  -450,  -591,  -607,  -607,
    -300,  -125,   175,  -607,  -607,  -607,  -291,  -607
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   112,    40,    14,   132,   138,   466,   334,   767,
     335,   625,    15,    16,    35,    36,    67,    17,    31,    32,
     336,   337,   774,   775,   338,   339,   340,   341,   342,   343,
     139,   140,    28,    29,    47,    48,    49,    18,   232,   279,
     122,    19,    81,    82,    83,   344,   345,   233,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     591,   887,   592,   888,   594,   889,   358,   582,   886,   359,
     633,   908,   360,   784,   785,   636,   361,   362,   435,   364,
     365,   366,   367,   768,   929,   769,   945,   368,   693,   871,
     634,   867,   947,   275,   804,   704,   544,   912,   276,   277,
     564,   565,   186,   187,   749,   421,   550,   391,   241,   239,
      57,    92,    21,   129,   383,    69,    70,    22,    86,    23,
     237,   127,    55,    89,    24,   238,   242,   370,   382,   179,
     180,   184,   181,   398,   813,   566,   396,   812,   393,   809,
     394,   914,   395,   811,   399,   814,   400,   879,   401,   816,
     402,   880,   403,   818,   404,   881,   405,   821,   406,   823,
      25,    42,    26,    41,   371,   608,   609,   610,   372,   611,
     612,   613,   614,   373,   465,   374,   940,   375
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      50,   178,   124,   363,   418,   422,   576,   558,   571,   560,
     434,   562,   617,   696,   427,   464,   -89,   717,   753,   719,
     116,   721,   485,    63,   579,   777,   760,     2,   452,   182,
     457,   458,   387,     3,   638,   603,   486,    93,    94,   488,
     489,   746,   603,   408,   118,   758,   511,   512,    43,   906,
     420,    68,   731,   733,   572,    64,     4,   234,     5,   508,
       6,   572,   511,   512,   240,    50,     7,   273,   780,   547,
     748,    33,   572,     8,    44,    30,    75,   781,   548,     9,
     414,   748,    38,   479,    27,   235,   185,   542,   572,   133,
     134,   108,    34,   388,   573,   177,   574,   894,    10,   575,
     487,   573,   542,   778,   386,   123,   538,   539,   782,   549,
     698,   274,   573,   783,    56,   119,   120,   480,    39,    11,
     415,   778,   538,   539,   500,   501,   417,   543,   573,    59,
     246,   776,   508,   826,   510,   511,   512,   513,    77,   699,
      59,   514,   799,   778,    45,   273,   841,    75,   484,   240,
     422,   891,   583,   778,    37,   121,   188,   247,   388,   839,
      46,   639,    95,    78,    60,   178,    96,    65,    97,    98,
     409,   598,   178,   897,   722,   388,   752,   756,    66,   388,
     779,   178,   757,   535,   536,   537,   700,    58,   410,   602,
      59,   615,    61,   416,   701,   538,   539,   411,    51,    52,
     425,    53,    99,   778,    12,   637,   702,   178,   178,   428,
      13,    45,   778,   437,   135,   570,   751,   419,   136,   835,
     137,    98,   778,   864,   384,   449,    77,    46,   714,    54,
     798,   847,  -488,    68,    79,   460,   462,  -488,    45,   827,
      80,   858,   627,   786,   620,   866,   621,   892,   865,   910,
     715,   873,   723,  -495,    46,  -488,   376,   898,  -495,   177,
     377,   869,  -502,  -264,    72,    84,   177,  -502,  -264,   725,
     708,    85,   488,   489,   724,   177,  -495,   378,   379,   380,
     381,   884,   757,   178,   761,  -502,  -264,   178,   178,   178,
     178,   726,    87,   178,   771,   178,   729,   178,    88,   178,
     838,   177,   177,   885,   388,   772,   773,   389,    73,   848,
     390,   546,   825,   901,   470,   551,   552,   553,   554,    74,
     763,   557,   747,   559,    90,   561,   110,    75,   330,   456,
      91,   123,   111,   130,   131,   622,   623,   423,   130,   131,
     424,    76,   243,   244,   178,   102,   578,   803,   874,   488,
     489,   243,   244,   245,    66,   104,   105,   500,   501,   106,
     590,   107,   115,   488,   489,   508,   113,   510,   511,   512,
     513,   114,   597,   123,   514,   125,   854,   177,   126,    64,
     128,   177,   177,   177,   177,   488,   489,   177,   183,   177,
     185,   177,   876,   177,   229,   230,   236,   231,   630,   240,
     252,   248,   249,   270,   250,   272,   251,   253,   254,   255,
     256,   257,   271,   258,   278,   385,   407,   397,   581,   431,
     441,   442,   445,   446,   895,   450,   475,   806,   538,   539,
     545,   273,   498,   499,   500,   501,   502,   474,   177,   505,
     506,   507,   508,   430,   510,   511,   512,   513,   500,   501,
     444,   514,   476,   516,   517,   439,   508,   443,   418,   511,
     512,   513,   628,   477,   481,   514,   540,    12,   178,   482,
     500,   501,   483,   555,   568,   541,   178,   486,   508,   588,
     510,   511,   512,   513,   595,   178,   178,   514,   178,   531,
     532,   533,   534,   535,   536,   537,   728,   601,   596,   635,
     629,   640,   657,   727,   736,   538,   539,   662,   738,   692,
     754,   776,   259,   740,   741,   764,   743,   766,   788,   538,
     539,   751,   755,   794,   796,   802,   803,   533,   534,   535,
     536,   537,   787,   369,   422,   805,   260,   261,   414,   828,
     363,   538,   539,   815,   817,   840,   819,   860,   852,   842,
     861,   843,   845,   862,   850,   863,   890,   893,   896,   899,
     849,   916,   177,   488,   489,   905,   903,   413,   930,   923,
     177,   911,   937,   924,   928,   941,   943,   950,   624,   177,
     177,   426,   177,   100,   951,    62,   948,   956,   103,   952,
     436,   957,   141,   931,   438,   262,   440,   392,   101,   117,
     801,   935,   913,    20,   448,   178,   904,   770,   453,   454,
     455,   834,   703,   875,   822,   463,   178,   577,   178,   567,
     707,    71,   697,   467,   468,   469,   109,   471,   472,   473,
     556,   429,   846,   810,   263,   264,   942,   618,   478,   265,
       0,     0,   266,     0,   820,     0,   498,   499,   500,   501,
     502,     0,     0,   505,   506,   507,   508,   267,   510,   511,
     512,   513,   178,   268,     0,   514,   269,   516,   517,     0,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     853,   856,     0,     0,     0,     0,     0,     0,   527,   177,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     177,     0,   177,     0,     0,     0,     0,     0,     0,   538,
     539,     0,     0,     0,     0,     0,     0,   123,     0,     0,
       0,     0,   584,     0,     0,     0,     0,     0,     0,   589,
       0,     0,     0,   593,     0,     0,   883,     0,     0,     0,
       0,   599,     0,     0,     0,     0,   177,     0,     0,     0,
     607,   178,   607,     0,     0,     0,     0,     0,   488,   489,
       0,     0,     0,     0,     0,     0,     0,   902,     0,     0,
       0,     0,     0,     0,     0,     0,   907,     0,     0,   909,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
       0,     0,   651,   652,   653,   654,   655,   656,     0,   660,
     661,     0,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   679,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,   691,     0,   694,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     709,   498,   499,   500,   501,   177,     0,     0,     0,     0,
       0,   508,     0,   510,   511,   512,   513,     0,     0,     0,
     514,     0,   516,   517,     0,     0,   730,     0,     0,     0,
       0,   735,     0,     0,     0,   660,   679,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,   473,     0,     0,     0,
     533,   534,   535,   536,   537,     0,   473,   944,     0,   762,
     463,     0,     0,   765,   538,   539,     0,     0,     0,     0,
       0,     0,     0,   953,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
       0,     0,   285,   286,   287,   288,   289,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,   792,   793,     0,
       0,     0,     0,     0,     0,     0,   295,     0,     0,   297,
       0,     0,   298,     0,     0,   299,     0,     0,   800,   302,
       0,     0,     0,     0,     0,     0,     0,   807,     0,     0,
     146,   147,   148,     0,   150,   151,   152,   153,   154,   306,
     156,   157,   158,   159,   160,   161,   162,   163,   164,     0,
     166,   167,   168,     0,     0,   171,   172,   173,   174,     0,
       0,   307,     0,     0,     0,     0,     0,     0,   836,   837,
       0,   259,     0,   309,   310,   844,     0,   607,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   311,   312,
     369,     0,     0,     0,   857,   260,   261,    45,   859,     0,
       0,     0,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    46,   322,   870,     0,     0,     0,   142,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
     323,   324,   325,     0,   326,     0,     0,   327,   328,     0,
     144,     0,     0,     0,     0,   882,     0,   329,     0,   330,
     331,   433,   123,     0,   262,   145,     0,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,     0,     0,
       0,     0,     0,   263,   264,   259,     0,     0,   265,     0,
       0,   266,     0,     0,   915,     0,     0,   917,   918,   919,
     921,   922,     0,     0,   925,   926,   267,   927,   580,   260,
     261,     0,   280,     0,    45,   269,   933,     3,     0,   281,
     282,   283,     0,   284,     0,   285,   286,   287,   288,   289,
      46,     0,     0,     0,     0,     0,   290,   291,   292,   293,
       0,     0,   946,     0,     0,     0,     0,   949,   294,   295,
       0,   296,   297,     0,     0,   298,     0,     8,   299,   300,
     955,   301,   302,     0,     0,   303,   304,   547,   262,     0,
       0,     0,   305,   146,   147,   148,   548,   150,   151,   152,
     153,   154,   306,   156,   157,   158,   159,   160,   161,   162,
     163,   164,     0,   166,   167,   168,     0,     0,   171,   172,
     173,   174,     0,     0,   307,   308,     0,   705,   264,     0,
       0,     0,   265,     0,     0,   266,   309,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     267,   311,   312,     0,     0,     0,   706,     0,     0,   269,
      45,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    46,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,     0,     0,     0,     0,     0,     0,     0,     0,
     329,     0,   330,   331,   332,   123,   333,   280,     0,     0,
       0,     0,     3,     0,   281,   282,   283,     0,   284,     0,
     285,   286,   287,   288,   289,     0,     0,     0,     0,     0,
       0,   290,   291,   292,   293,     0,     0,     0,     0,     0,
       0,     0,     0,   294,   295,     0,   296,   297,     0,     0,
     298,     0,     8,   299,   300,     0,   301,   302,     0,     0,
     303,   304,     0,     0,     0,     0,     0,   305,   146,   147,
     148,     0,   150,   151,   152,   153,   154,   306,   156,   157,
     158,   159,   160,   161,   162,   163,   164,     0,   166,   167,
     168,     0,     0,   171,   172,   173,   174,     0,     0,   307,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,    45,     0,     0,   259,     0,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    46,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,   261,     0,     0,     0,     0,   323,   324,
     325,     0,   326,     0,     0,   327,   328,     0,     0,     0,
     285,   286,   287,   288,   289,   329,     0,   330,   331,   332,
     123,   855,   291,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   295,     0,     0,   297,     0,     0,
     298,     0,     0,   299,     0,   603,     0,   302,     0,     0,
       0,   262,   260,   261,     0,     0,     0,     0,   146,   147,
     148,     0,   150,   151,   152,   153,   154,   306,   156,   157,
     158,   159,   160,   161,   162,   163,   164,     0,   166,   167,
     168,     0,     0,   171,   172,   173,   174,     0,     0,   307,
     263,   264,     0,     0,     0,   265,     0,   710,   266,     0,
       0,   309,   310,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,     0,    45,     0,     0,     0,     0,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   604,   322,     0,     0,     0,     0,     0,     0,     0,
     263,   264,     0,     0,     0,   265,     0,     0,   605,   324,
     325,     0,   326,     0,     0,   327,   328,   285,   286,   287,
     288,   289,     0,   267,     0,   606,     0,   330,   331,   291,
     123,     0,   269,     0,     0,   259,     0,     0,     0,     0,
       0,   295,     0,     0,   297,     0,     0,   298,     0,     0,
     299,     0,     0,     0,   302,     0,     0,     0,     0,   260,
     261,     0,     0,     0,     0,   146,   147,   148,     0,   150,
     151,   152,   153,   154,   306,   156,   157,   158,   159,   160,
     161,   162,   163,   164,     0,   166,   167,   168,     0,     0,
     171,   172,   173,   174,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,   310,
       0,     0,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,    45,     0,     0,     0,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   604,   322,
       0,     0,     0,   260,   261,     0,     0,   263,   264,     0,
       0,     0,   265,     0,     0,   605,   324,   325,     0,   326,
       0,     0,   327,   328,   285,   286,   287,   288,   289,     0,
     267,     0,   616,     0,   330,   331,   291,   123,     0,   269,
       0,     0,   408,     0,     0,     0,     0,     0,   295,     0,
       0,   297,     0,     0,   298,     0,     0,   299,     0,     0,
       0,   302,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,   147,   148,     0,   150,   151,   152,   153,
     154,   306,   156,   157,   158,   159,   160,   161,   162,   163,
     164,     0,   166,   167,   168,     0,     0,   171,   172,   173,
     174,   263,   264,   307,     0,     0,   265,     0,     0,   266,
       0,     0,     0,     0,     0,   309,   310,     0,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
     711,     0,     0,   269,     0,     0,     0,     0,     0,    45,
     259,     0,     0,     0,     0,     0,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    46,   322,     0,     0,     0,
       0,     0,     0,     0,   260,   261,     0,     0,     0,   409,
       0,     0,   323,   324,   325,     0,   326,     0,     0,   327,
     328,   285,   286,   287,   288,   289,     0,   410,     0,   329,
       0,   330,   331,   291,   123,     0,   411,     0,     0,     0,
       0,     0,     0,     0,     0,   295,     0,     0,   297,     0,
       0,   298,     0,     0,   299,     0,     0,     0,   302,     0,
       0,     0,     0,   262,     0,     0,     0,     0,     0,   146,
     147,   148,     0,   150,   151,   152,   153,   154,   306,   156,
     157,   158,   159,   160,   161,   162,   163,   164,     0,   166,
     167,   168,     0,     0,   171,   172,   173,   174,     0,     0,
     307,     0,   263,   264,     0,     0,     0,   265,     0,   712,
     266,     0,   309,   310,     0,   259,     0,     0,     0,     0,
       0,   451,     0,     0,     0,   267,     0,   311,   312,     0,
       0,     0,     0,     0,   269,     0,    45,     0,     0,   260,
     261,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    46,   322,     0,     0,     0,     0,     0,     0,
     285,   286,   287,   288,   289,     0,     0,     0,     0,   323,
     324,   325,   291,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,   295,     0,   329,   297,   330,   331,
     298,   123,     0,   299,     0,     0,     0,   302,   262,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   146,   147,
     148,     0,   150,   151,   152,   153,   154,   306,   156,   157,
     158,   159,   160,   161,   162,   163,   164,     0,   166,   167,
     168,     0,     0,   171,   172,   173,   174,   263,   264,   307,
       0,     0,   265,     0,   713,   266,     0,     0,     0,     0,
       0,   309,   310,     0,   259,     0,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,   311,   312,     0,   269,
       0,     0,     0,     0,     0,    45,     0,     0,   260,   261,
       0,     0,   313,   314,   315,   316,   317,   318,   319,   320,
     321,    46,   322,     0,     0,     0,     0,     0,     0,   285,
     286,   287,   288,   289,     0,     0,     0,     0,   323,   324,
     325,   291,   326,     0,     0,   327,   328,     0,     0,     0,
       0,     0,     0,   295,     0,   329,   297,   330,   331,   298,
     123,     0,   299,     0,     0,     0,   302,   262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,   147,   148,
       0,   150,   151,   152,   153,   154,   306,   156,   157,   158,
     159,   160,   161,   162,   163,   164,     0,   166,   167,   168,
       0,     0,   171,   172,   173,   174,   263,   264,   307,     0,
       0,   265,     0,   716,   266,     0,     0,     0,     0,     0,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,   269,     0,
       0,   259,     0,     0,    45,     0,     0,     0,     0,     0,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
      46,   322,     0,     0,     0,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,   324,   325,
       0,   326,     0,     0,   327,   328,   285,   286,   287,   288,
     289,     0,   631,     0,   329,   632,   330,   331,   291,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     295,     0,     0,   297,     0,     0,   298,     0,     0,   299,
       0,     0,     0,   302,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   146,   147,   148,     0,   150,   151,
     152,   153,   154,   306,   156,   157,   158,   159,   160,   161,
     162,   163,   164,     0,   166,   167,   168,     0,     0,   171,
     172,   173,   174,   263,   264,   307,     0,     0,   265,     0,
     718,   266,     0,     0,     0,     0,     0,   309,   310,     0,
     259,     0,     0,     0,     0,     0,   267,     0,     0,     0,
       0,     0,   658,   659,     0,   269,     0,     0,     0,     0,
       0,    45,     0,     0,   260,   261,     0,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    46,   322,     0,
       0,     0,     0,     0,     0,   285,   286,   287,   288,   289,
       0,     0,     0,     0,   323,   324,   325,   291,   326,     0,
       0,   327,   328,     0,     0,     0,     0,     0,     0,   295,
       0,   329,   297,   330,   331,   298,   123,     0,   299,     0,
       0,     0,   302,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   147,   148,     0,   150,   151,   152,
     153,   154,   306,   156,   157,   158,   159,   160,   161,   162,
     163,   164,     0,   166,   167,   168,     0,     0,   171,   172,
     173,   174,   263,   264,   307,     0,     0,   265,     0,   720,
     266,     0,     0,     0,     0,     0,   309,   310,     0,   259,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,   677,   678,     0,   269,     0,     0,     0,     0,     0,
      45,     0,     0,   260,   261,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    46,   322,     0,     0,
       0,     0,     0,     0,   285,   286,   287,   288,   289,     0,
       0,     0,     0,   323,   324,   325,   291,   326,     0,     0,
     327,   328,     0,     0,     0,     0,     0,     0,   295,     0,
     329,   297,   330,   331,   298,   123,     0,   299,     0,     0,
       0,   302,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,   147,   148,     0,   150,   151,   152,   153,
     154,   306,   156,   157,   158,   159,   160,   161,   162,   163,
     164,     0,   166,   167,   168,     0,     0,   171,   172,   173,
     174,   263,   264,   307,     0,     0,   265,     0,   744,   266,
       0,     0,     0,     0,     0,   309,   310,     0,   259,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,    45,
       0,     0,   260,   261,     0,     0,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    46,   322,     0,     0,     0,
       0,     0,     0,   285,   286,   287,   288,   289,     0,     0,
       0,     0,   323,   324,   325,   291,   326,     0,     0,   327,
     328,     0,     0,     0,     0,     0,     0,   295,   412,   329,
     297,   330,   331,   298,   123,     0,   299,     0,     0,     0,
     302,   262,     0,   447,     0,     0,     0,     0,     0,     0,
       0,   146,   147,   148,     0,   150,   151,   152,   153,   154,
     306,   156,   157,   158,   159,   160,   161,   162,   163,   164,
       0,   166,   167,   168,     0,     0,   171,   172,   173,   174,
     263,   264,   307,     0,     0,   265,     0,   830,   266,     0,
       0,     0,     0,     0,   309,   310,     0,   259,     0,     0,
       0,     0,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,    45,     0,
       0,   260,   261,     0,     0,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    46,   322,     0,     0,     0,     0,
       0,     0,   285,   286,   287,   288,   289,     0,     0,   680,
       0,   323,   324,   325,   291,   326,     0,     0,   327,   328,
       0,     0,     0,     0,     0,     0,   295,     0,   329,   297,
     330,   331,   298,   123,     0,   299,     0,     0,     0,   302,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   148,     0,   150,   151,   152,   153,   154,   306,
     156,   157,   158,   159,   160,   161,   162,   163,   164,     0,
     166,   167,   168,     0,     0,   171,   172,   173,   174,   263,
     264,   307,     0,     0,   265,     0,   831,   266,     0,     0,
       0,     0,     0,   309,   310,     0,   259,     0,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,   269,     0,     0,     0,     0,     0,    45,     0,     0,
     260,   261,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    46,   322,     0,     0,     0,     0,     0,
       0,   285,   286,   287,   288,   289,     0,     0,     0,     0,
     323,   324,   325,   291,   326,     0,     0,   327,   328,     0,
       0,     0,     0,     0,     0,   295,     0,   329,   297,   330,
     331,   298,   123,     0,   299,     0,     0,     0,   302,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,     0,   150,   151,   152,   153,   154,   306,   156,
     157,   158,   159,   160,   161,   162,   163,   164,     0,   166,
     167,   168,     0,     0,   171,   172,   173,   174,   263,   264,
     307,     0,     0,   265,     0,   832,   266,     0,     0,     0,
       0,     0,   309,   310,     0,     0,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
     269,     0,     0,     0,     0,     0,    45,   259,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    46,   322,     0,     0,     0,     0,     0,     0,
       0,   260,   261,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,   285,   286,
     287,   288,   289,     0,     0,     0,   329,   695,   330,   331,
     291,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,     0,     0,   297,     0,     0,   298,     0,
       0,   299,     0,     0,     0,   302,     0,     0,     0,   572,
     262,     0,     0,     0,     0,     0,   146,   147,   148,     0,
     150,   151,   152,   153,   154,   306,   156,   157,   158,   159,
     160,   161,   162,   163,   164,     0,   166,   167,   168,     0,
       0,   171,   172,   173,   174,     0,     0,   307,     0,   824,
     264,     0,     0,     0,   265,     0,     0,   266,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
     259,   269,     0,    45,     0,     0,     0,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    46,
     322,     0,     0,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   324,   325,     0,
     326,     0,     0,   327,   328,   285,   286,   287,   288,   289,
       0,     0,     0,   329,   732,   330,   331,   291,   123,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
       0,     0,   297,     0,     0,   298,     0,     0,   299,     0,
       0,     0,   302,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   147,   148,     0,   150,   151,   152,
     153,   154,   306,   156,   157,   158,   159,   160,   161,   162,
     163,   164,     0,   166,   167,   168,     0,     0,   171,   172,
     173,   174,   263,   264,   307,     0,     0,   265,     0,   833,
     266,     0,     0,     0,     0,     0,   309,   310,     0,   259,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
      45,     0,     0,   260,   261,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    46,   322,     0,     0,
       0,     0,     0,     0,   285,   286,   287,   288,   289,     0,
       0,   734,     0,   323,   324,   325,   291,   326,     0,     0,
     327,   328,     0,     0,     0,     0,     0,     0,   295,     0,
     329,   297,   330,   331,   298,   123,     0,   299,     0,     0,
       0,   302,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,   147,   148,     0,   150,   151,   152,   153,
     154,   306,   156,   157,   158,   159,   160,   161,   162,   163,
     164,     0,   166,   167,   168,     0,     0,   171,   172,   173,
     174,   263,   264,   307,     0,     0,   265,     0,   878,   266,
       0,     0,     0,     0,     0,   309,   310,     0,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,   269,     0,     0,   259,     0,     0,    45,
       0,     0,     0,     0,     0,     0,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    46,   322,     0,     0,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,   324,   325,     0,   326,     0,     0,   327,
     328,   285,   286,   287,   288,   289,     0,     0,     0,   329,
     750,   330,   331,   291,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   295,     0,     0,   297,     0,
       0,   298,     0,     0,   299,     0,     0,     0,   302,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,     0,   150,   151,   152,   153,   154,   306,   156,
     157,   158,   159,   160,   161,   162,   163,   164,     0,   166,
     167,   168,     0,     0,   171,   172,   173,   174,   263,   264,
     307,     0,     0,   265,     0,   900,   266,     0,     0,     0,
       0,     0,   309,   310,     0,     0,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
     269,     0,     0,   259,     0,     0,    45,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    46,   322,     0,     0,     0,   260,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,   285,   286,
     287,   288,   289,     0,     0,     0,   329,   759,   330,   331,
     291,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   295,     0,     0,   297,     0,     0,   298,     0,
       0,   299,     0,     0,     0,   302,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,   147,   148,     0,
     150,   151,   152,   153,   154,   306,   156,   157,   158,   159,
     160,   161,   162,   163,   164,     0,   166,   167,   168,     0,
       0,   171,   172,   173,   174,   263,   264,   307,     0,     0,
     265,     0,   934,   266,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,   269,     0,     0,
     259,     0,     0,    45,     0,     0,     0,     0,     0,     0,
     313,   314,   315,   316,   317,   318,   319,   320,   321,    46,
     322,     0,     0,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   324,   325,     0,
     326,     0,     0,   327,   328,   285,   286,   287,   288,   289,
       0,     0,     0,   329,   868,   330,   331,   291,   123,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
       0,     0,   297,     0,     0,   298,     0,     0,   299,     0,
       0,     0,   302,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   147,   148,     0,   150,   151,   152,
     153,   154,   306,   156,   157,   158,   159,   160,   161,   162,
     163,   164,     0,   166,   167,   168,     0,     0,   171,   172,
     173,   174,   263,   264,   307,     0,     0,   265,     0,     0,
     266,     0,     0,     0,     0,     0,   309,   310,     0,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
      45,     0,     0,     0,     0,     0,     0,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    46,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   324,   325,     0,   326,     0,     0,
     327,   328,   285,   286,   287,   288,   289,     0,     0,     0,
     329,   920,   330,   331,   291,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,     0,     0,   297,
       0,     0,   298,     0,     0,   299,     0,     0,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   148,     0,   150,   151,   152,   153,   154,   306,
     156,   157,   158,   159,   160,   161,   162,   163,   164,     0,
     166,   167,   168,     0,     0,   171,   172,   173,   174,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   310,     0,     0,     0,     0,     0,
       0,     0,   932,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,     0,     0,
       0,     0,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    46,   322,     0,     0,     0,     0,     0,
       0,   285,   286,   287,   288,   289,     0,     0,     0,     0,
     323,   324,   325,   291,   326,     0,     0,   327,   328,     0,
       0,     0,     0,     0,     0,   295,     0,   329,   297,   330,
     331,   298,   123,     0,   299,     0,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,     0,   150,   151,   152,   153,   154,   306,   156,
     157,   158,   159,   160,   161,   162,   163,   164,     0,   166,
     167,   168,     0,     0,   171,   172,   173,   174,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   310,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   321,    46,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,   325,     0,   326,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,   329,     0,   330,   331,
       0,   123,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   585,   510,   511,   512,   513,   488,   489,     0,   514,
     515,   516,   517,   518,   519,     0,     0,   520,   521,   522,
     523,   524,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   586,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
     587,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     585,   510,   511,   512,   513,   488,   489,     0,   514,   515,
     516,   517,   518,   519,     0,     0,   520,   521,   522,   523,
     524,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,     0,     0,     0,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   585,
     510,   511,   512,   513,   488,   489,     0,   514,   515,   516,
     517,   518,   519,     0,     0,   520,   521,   522,   523,   524,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   586,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,   737,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   585,   510,
     511,   512,   513,   488,   489,     0,   514,   515,   516,   517,
     518,   519,     0,     0,   520,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,     0,   745,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   585,   510,   511,
     512,   513,   488,   489,     0,   514,   515,   516,   517,   518,
     519,     0,     0,   520,   521,   522,   523,   524,   525,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   586,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   538,
     539,     0,     0,     0,     0,     0,   808,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   585,   510,   511,   512,
     513,   488,   489,     0,   514,   515,   516,   517,   518,   519,
       0,     0,   520,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
       0,     0,     0,     0,     0,   877,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,   489,     0,     0,
       0,     0,     0,     0,   498,   499,   500,   501,   502,     0,
       0,   505,   506,   507,   508,     0,   510,   511,   512,   513,
       0,     0,     0,   514,     0,   516,   517,     0,     0,     0,
       0,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,     0,     0,
       0,   488,   489,     0,     0,     0,     0,   538,   539,   498,
     499,   500,   501,   502,   872,     0,   505,   506,   507,   508,
       0,   510,   511,   512,   513,     0,     0,     0,   514,     0,
     516,   517,     0,     0,     0,     0,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,   488,
     489,     0,   538,   539,   498,   499,   500,   501,   502,   939,
       0,   505,   506,   507,   508,     0,   510,   511,   512,   513,
       0,     0,     0,   514,     0,   516,   517,     0,     0,     0,
       0,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   538,   539,   488,
     489,   626,   498,   499,   500,   501,   502,     0,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,     0,
       0,   514,     0,   516,   517,     0,     0,     0,     0,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,   488,   489,   739,
       0,     0,   498,   499,   500,   501,   502,     0,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,     0,
       0,   514,     0,   516,   517,     0,     0,     0,     0,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,   488,   489,   742,
     498,   499,   500,   501,   502,     0,     0,   505,   506,   507,
     508,     0,   510,   511,   512,   513,     0,     0,     0,   514,
       0,   516,   517,     0,     0,     0,     0,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,   488,   489,   829,     0,     0,
     498,   499,   500,   501,   502,     0,     0,   505,   506,   507,
     508,     0,   510,   511,   512,   513,     0,     0,     0,   514,
       0,   516,   517,     0,     0,     0,     0,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,   488,   489,   936,   498,   499,
     500,   501,   502,     0,     0,   505,   506,   507,   508,     0,
     510,   511,   512,   513,     0,     0,     0,   514,     0,   516,
     517,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,   488,   489,   938,     0,     0,   498,   499,
     500,   501,   502,     0,     0,   505,   506,   507,   508,     0,
     510,   511,   512,   513,     0,     0,     0,   514,     0,   516,
     517,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,   488,
     489,   538,   539,     0,     0,   954,   498,   499,   500,   501,
     502,     0,     0,   505,   506,   507,   508,     0,   510,   511,
     512,   513,     0,     0,     0,   514,     0,   516,   517,     0,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   488,   489,     0,     0,   538,
     539,   569,   498,   499,   500,   501,   502,     0,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,     0,
       0,   514,     0,   516,   517,     0,     0,     0,     0,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,   791,   498,   499,
     500,   501,   502,   488,   489,   505,   506,   507,   508,     0,
     510,   511,   512,   513,     0,     0,     0,   514,     0,   516,
     517,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,   797,     0,     0,     0,     0,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   585,   510,   511,
     512,   513,  -234,   488,   489,   514,   515,   516,   517,   518,
     519,     0,     0,   520,   521,   522,   523,   524,   525,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   586,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   538,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,     0,     0,     0,     0,     0,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,     0,     0,     0,   514,   515,   516,   517,   518,
     519,     0,     0,   520,   521,   522,   523,   524,   525,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   526,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     488,   489,     0,     0,   498,   499,   500,   501,   502,   538,
     539,   505,   506,   507,   508,     0,   510,   511,   512,   513,
       0,     0,     0,   514,     0,   516,   517,     0,     0,   619,
       0,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   488,   489,
       0,     0,     0,     0,     0,     0,     0,   538,   539,     0,
       0,     0,     0,   498,   499,   500,   501,   502,     0,     0,
     505,   506,   507,   508,     0,   510,   511,   512,   513,     0,
       0,     0,   514,     0,   516,   517,     0,     0,     0,     0,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,   795,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   488,   489,     0,
       0,   498,   499,   500,   501,   502,   538,   539,   505,   506,
     507,   508,     0,   510,   511,   512,   513,     0,     0,     0,
     514,     0,   516,   517,     0,     0,     0,     0,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   488,   489,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
     498,   499,   500,   501,   502,     0,     0,   505,   506,   507,
     508,     0,   510,   511,   512,   513,     0,     0,     0,   514,
       0,   516,   517,     0,     0,     0,     0,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   488,   489,     0,     0,   498,   499,
     500,   501,   502,   538,   539,   505,   506,   507,   508,     0,
     510,   511,   512,   513,     0,     0,     0,   514,     0,   516,
     517,     0,   488,   489,     0,   520,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,   498,   499,   500,
     501,   502,     0,     0,   505,   506,   507,   508,     0,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
     488,   489,     0,     0,   520,   498,   499,   500,   501,   502,
       0,     0,   505,   506,   507,   508,     0,   510,   511,   512,
     513,     0,     0,     0,   514,     0,   516,   517,   488,   489,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,     0,     0,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
       0,     0,     0,   498,   499,   500,   501,   502,     0,     0,
     505,   506,   507,   508,     0,   510,   511,   512,   513,   488,
     489,     0,   514,     0,   516,   517,     0,     0,     0,     0,
       0,   498,   499,   500,   501,   502,     0,     0,   505,   506,
     507,   508,     0,   510,   511,   512,   513,     0,     0,     0,
     514,     0,   516,   517,     0,     0,     0,     0,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
       0,     0,     0,     0,   488,   489,     0,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,   498,   499,   538,   539,   502,     0,     0,   505,
     506,   507,   508,     0,   510,   511,   512,   513,     0,     0,
       0,   514,     0,   516,   517,     0,     0,     0,     0,   520,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,     0,     0,     0,   536,     0,     0,   498,   499,   500,
     501,   502,     0,     0,   505,   538,   539,   508,     0,   510,
     511,   512,   513,     0,     0,     0,   514,     0,   516,   517,
       0,     0,     0,     0,     0,     0,     0,     0,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     142,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,   531,   532,   533,   534,   535,   536,
     537,   144,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,   145,     0,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,     0,
       0,     0,     0,     0,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    45,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,   145,     0,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    45,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,   145,     0,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,     0,   144,
       0,     0,     0,     0,   313,   314,   315,     0,     0,     0,
       0,     0,     0,    46,   145,     0,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,     0,     0,     0,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   207,
     208,   209,   210,    45,     0,   211,   212,   213,   214,   215,
     216,     0,     0,     0,     0,     0,     0,     0,     0,   563,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,     0,     0,   227,   228
};

static const yytype_int16 yycheck[] =
{
      12,   107,    82,   233,   272,   275,   422,   400,   420,   402,
     290,   404,   462,   540,   284,   315,     7,   558,   609,   560,
      76,   562,   360,    21,    19,   631,   617,     0,   308,   107,
     311,   312,   239,     6,    18,    49,   119,    14,    15,    20,
      21,    19,    49,    32,    34,    19,   116,   117,    33,   128,
      46,   145,   579,   580,   114,    53,    29,   156,    31,   113,
      33,   114,   116,   117,   145,    77,    39,   136,   114,   114,
     123,   145,   114,    46,    59,   139,   129,   123,   123,    52,
     145,   123,   145,   145,   145,   184,   145,   145,   114,    14,
      15,   185,   166,   153,   154,   107,   156,   123,    71,   159,
     183,   154,   145,   153,   185,   184,   176,   177,   154,   154,
     154,   180,   154,   159,    58,   105,   106,   179,   181,    92,
     185,   153,   176,   177,   105,   106,   185,   185,   154,   153,
     153,   145,   113,   183,   115,   116,   117,   118,   153,   183,
     153,   122,   185,   153,   129,   136,   752,   129,   355,   145,
     420,   183,   432,   153,   184,   145,   180,   180,   153,   750,
     145,   145,   139,   178,   156,   271,   143,   165,   145,   146,
     159,   451,   278,   183,   156,   153,   183,   178,   176,   153,
     180,   287,   183,   164,   165,   166,   177,   154,   177,   459,
     153,   461,   153,   271,   185,   176,   177,   186,     4,     5,
     278,     7,   179,   153,   177,   486,   544,   313,   314,   287,
     183,   129,   153,   293,   139,   178,   153,   273,   143,   746,
     145,   146,   153,   153,   236,   305,   153,   145,   161,    35,
     180,   758,   156,   145,    53,   313,   314,   161,   129,   180,
      59,   178,   160,   636,   183,   786,   185,   838,   178,   180,
     183,   178,   161,   156,   145,   179,    66,   848,   161,   271,
      70,   788,   156,   156,   145,    53,   278,   161,   161,   161,
     550,    59,    20,    21,   183,   287,   179,    87,    88,    89,
      90,   161,   183,   389,   185,   179,   179,   393,   394,   395,
     396,   183,    53,   399,    11,   401,   576,   403,    59,   405,
     750,   313,   314,   183,   153,    22,    23,   156,   145,   759,
     159,   389,   728,   854,   326,   393,   394,   395,   396,   145,
     620,   399,   603,   401,    53,   403,    53,   129,   181,   182,
      59,   184,    59,   147,   148,   149,   150,   180,   147,   148,
     183,   179,   147,   148,   450,   139,   426,    42,    43,    20,
      21,   147,   148,   149,   176,   145,   145,   105,   106,   145,
     440,   154,   145,    20,    21,   113,   184,   115,   116,   117,
     118,   184,   450,   184,   122,   145,   769,   389,   145,    53,
     184,   393,   394,   395,   396,    20,    21,   399,   145,   401,
     145,   403,   804,   405,    34,    34,   156,   179,   478,   145,
     179,   160,   160,   183,   160,   183,   160,   160,   160,   160,
     160,   160,   156,   160,   156,   184,    32,   145,   430,   160,
     160,   160,   160,   139,   840,   160,    36,   707,   176,   177,
     145,   136,   103,   104,   105,   106,   107,   182,   450,   110,
     111,   112,   113,   179,   115,   116,   117,   118,   105,   106,
     183,   122,   183,   124,   125,   179,   113,   179,   726,   116,
     117,   118,   474,   183,   183,   122,   179,   177,   574,   183,
     105,   106,   183,   180,   178,   184,   582,   119,   113,    13,
     115,   116,   117,   118,   156,   591,   592,   122,   594,   160,
     161,   162,   163,   164,   165,   166,   574,   182,   139,   136,
     184,   145,   145,   178,   582,   176,   177,   145,   588,   145,
     178,   145,    32,   591,   592,   185,   594,   147,   179,   176,
     177,   153,   153,   145,     1,     7,    42,   162,   163,   164,
     165,   166,   160,   233,   804,   183,    56,    57,   145,   145,
     770,   176,   177,   161,   161,   145,   161,   145,   147,   178,
     145,   178,   178,   145,   178,   145,   179,   178,   178,   178,
     185,   145,   574,    20,    21,   178,   180,   267,   161,   178,
     582,   183,   161,   178,   185,   178,   178,    49,   466,   591,
     592,   281,   594,    58,   178,    34,   183,   178,    61,   185,
     290,   178,   104,   902,   294,   115,   296,   246,    59,    77,
     700,   912,   875,     1,   304,   711,   864,   629,   308,   309,
     310,   744,   544,   802,   724,   315,   722,   424,   724,   406,
     546,    40,   541,   323,   324,   325,    70,   327,   328,   329,
     398,   287,   757,   711,   154,   155,   927,   462,   338,   159,
      -1,    -1,   162,    -1,   722,    -1,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,   177,   115,   116,
     117,   118,   768,   183,    -1,   122,   186,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   700,    -1,
     768,   771,    -1,    -1,    -1,    -1,    -1,    -1,   155,   711,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     722,    -1,   724,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
      -1,    -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,   439,
      -1,    -1,    -1,   443,    -1,    -1,   826,    -1,    -1,    -1,
      -1,   451,    -1,    -1,    -1,    -1,   768,    -1,    -1,    -1,
     460,   867,   462,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   857,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,    -1,   867,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
      -1,    -1,   502,   503,   504,   505,   506,   507,    -1,   509,
     510,    -1,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,    -1,   539,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     550,   103,   104,   105,   106,   867,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,   576,    -1,    -1,    -1,
      -1,   581,    -1,    -1,    -1,   585,   586,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   605,   606,    -1,    -1,    -1,
     162,   163,   164,   165,   166,    -1,   616,   929,    -1,   619,
     620,    -1,    -1,   623,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   945,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   658,   659,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,   677,   678,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,   698,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   707,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   748,   749,
      -1,    32,    -1,   105,   106,   755,    -1,   757,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
     770,    -1,    -1,    -1,   774,    56,    57,   129,   778,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   795,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
     162,   163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,
      45,    -1,    -1,    -1,    -1,   825,    -1,   179,    -1,   181,
     182,   183,   184,    -1,   115,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,   154,   155,    32,    -1,    -1,   159,    -1,
      -1,   162,    -1,    -1,   884,    -1,    -1,   887,   888,   889,
     890,   891,    -1,    -1,   894,   895,   177,   897,   179,    56,
      57,    -1,     1,    -1,   129,   186,   906,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
     145,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,   932,    -1,    -1,    -1,    -1,   937,    37,    38,
      -1,    40,    41,    -1,    -1,    44,    -1,    46,    47,    48,
     950,    50,    51,    -1,    -1,    54,    55,   114,   115,    -1,
      -1,    -1,    61,    62,    63,    64,   123,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    93,    94,    -1,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,   162,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   120,   121,    -1,    -1,    -1,   183,    -1,    -1,   186,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,   181,   182,   183,   184,   185,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    32,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   179,    -1,   181,   182,   183,
     184,   185,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    49,    -1,    51,    -1,    -1,
      -1,   115,    56,    57,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    83,
      84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    93,
     154,   155,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,   166,    -1,    -1,   169,   170,    14,    15,    16,
      17,    18,    -1,   177,    -1,   179,    -1,   181,   182,    26,
     184,    -1,   186,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    56,    57,    -1,    -1,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,   166,
      -1,    -1,   169,   170,    14,    15,    16,    17,    18,    -1,
     177,    -1,   179,    -1,   181,   182,    26,   184,    -1,   186,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,   154,   155,    93,    -1,    -1,   159,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,
      32,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,   166,    -1,    -1,   169,
     170,    14,    15,    16,    17,    18,    -1,   177,    -1,   179,
      -1,   181,   182,    26,   184,    -1,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,   154,   155,    -1,    -1,    -1,   159,    -1,   161,
     162,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,   177,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,   186,    -1,   129,    -1,    -1,    56,
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
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
      -1,    32,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
      -1,   166,    -1,    -1,   169,   170,    14,    15,    16,    17,
      18,    -1,   177,    -1,   179,   180,   181,   182,    26,   184,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    83,    84,    -1,    -1,    87,
      88,    89,    90,   154,   155,    93,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   186,    -1,    -1,    -1,    -1,
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
      89,    90,   154,   155,    93,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,   120,   121,    -1,   186,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,   162,   163,   164,    26,   166,    -1,    -1,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    38,   178,   179,
      41,   181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,
      51,   115,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    83,    84,    -1,    -1,    87,    88,    89,    90,
     154,   155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    56,    57,    -1,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,   162,   163,   164,    26,   166,    -1,    -1,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,
     181,   182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    87,    88,    89,    90,   154,
     155,    93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,   105,   106,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,   129,    32,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   179,   180,   181,   182,
      26,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    83,    84,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    93,    -1,   154,
     155,    -1,    -1,    -1,   159,    -1,    -1,   162,    -1,   105,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,   186,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
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
     162,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    56,    57,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,   160,    -1,   162,   163,   164,    26,   166,    -1,    -1,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     179,    41,   181,   182,    44,   184,    -1,    47,    -1,    -1,
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
      93,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,    -1,    32,    -1,    -1,   129,    -1,    -1,    -1,
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
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     162,   163,   164,    26,   166,    -1,    -1,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,   179,    41,   181,
     182,    44,   184,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      83,    84,    -1,    -1,    87,    88,    89,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,    -1,   166,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   181,   182,
      -1,   184,    95,    96,    97,    98,    99,   100,   101,   102,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
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
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   176,   177,   103,
     104,   105,   106,   107,   183,    -1,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   176,   177,   103,   104,   105,   106,   107,   183,
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
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,   176,   177,    -1,    -1,   180,   103,   104,   105,   106,
     107,    -1,    -1,   110,   111,   112,   113,    -1,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   176,
     177,   178,   103,   104,   105,   106,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,   178,   103,   104,
     105,   106,   107,    20,    21,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,   178,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    20,    21,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      20,    21,    -1,    -1,   103,   104,   105,   106,   107,   176,
     177,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,    -1,    -1,   128,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    20,    21,    -1,
      -1,   103,   104,   105,   106,   107,   176,   177,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,   110,   111,   112,
     113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    20,    21,    -1,    -1,   103,   104,
     105,   106,   107,   176,   177,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,    -1,    20,    21,    -1,   130,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,
      20,    21,    -1,    -1,   130,   103,   104,   105,   106,   107,
      -1,    -1,   110,   111,   112,   113,    -1,   115,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,   124,   125,    20,    21,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   118,    20,
      21,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,    -1,   115,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   176,   177,   107,    -1,    -1,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,   157,   158,   159,   160,
     161,    -1,    -1,    -1,   165,    -1,    -1,   103,   104,   105,
     106,   107,    -1,    -1,   110,   176,   177,   113,    -1,   115,
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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   129,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,   169,   170
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   188,     0,     6,    29,    31,    33,    39,    46,    52,
      71,    92,   177,   183,   191,   199,   200,   204,   224,   228,
     244,   299,   304,   306,   311,   347,   349,   145,   219,   220,
     139,   205,   206,   145,   166,   201,   202,   184,   145,   181,
     190,   350,   348,    33,    59,   129,   145,   221,   222,   223,
     236,     4,     5,     7,    35,   309,    58,   297,   154,   153,
     156,   153,   201,    21,    53,   165,   176,   203,   145,   302,
     303,   297,   145,   145,   145,   129,   179,   153,   178,    53,
      59,   229,   230,   231,    53,    59,   305,    53,    59,   310,
      53,    59,   298,    14,    15,   139,   143,   145,   146,   179,
     193,   219,   139,   206,   145,   145,   145,   154,   185,   302,
      53,    59,   189,   184,   184,   145,   220,   222,    34,   105,
     106,   145,   227,   184,   232,   145,   145,   308,   184,   300,
     147,   148,   192,    14,    15,   139,   143,   145,   193,   217,
     218,   203,    24,    30,    45,    60,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,   236,   314,   316,
     317,   319,   322,   145,   318,   145,   289,   290,   180,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   124,   125,   126,
     127,   130,   131,   132,   133,   134,   135,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   169,   170,    34,
      34,   179,   225,   234,   156,   184,   156,   307,   312,   296,
     145,   295,   313,   147,   148,   149,   153,   180,   160,   160,
     160,   160,   179,   160,   160,   160,   160,   160,   160,    32,
      56,    57,   115,   154,   155,   159,   162,   177,   183,   186,
     183,   156,   183,   136,   180,   280,   285,   286,   156,   226,
       1,     8,     9,    10,    12,    14,    15,    16,    17,    18,
      25,    26,    27,    28,    37,    38,    40,    41,    44,    47,
      48,    50,    51,    54,    55,    61,    71,    93,    94,   105,
     106,   120,   121,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   146,   162,   163,   164,   166,   169,   170,   179,
     181,   182,   183,   185,   195,   197,   207,   208,   211,   212,
     213,   214,   215,   216,   232,   233,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   253,   256,
     259,   263,   264,   265,   266,   267,   268,   269,   274,   277,
     314,   351,   355,   360,   362,   364,    66,    70,    87,    88,
      89,    90,   315,   301,   236,   184,   185,   295,   153,   156,
     159,   294,   217,   325,   327,   329,   323,   145,   320,   331,
     333,   335,   337,   339,   341,   343,   345,    32,    32,   159,
     177,   186,   178,   277,   145,   185,   322,   185,   289,   220,
      46,   292,   313,   180,   183,   322,   277,   313,   322,   351,
     179,   160,   114,   183,   235,   265,   277,   232,   277,   179,
     277,   160,   160,   179,   183,   160,   139,    54,   277,   232,
     160,   114,   235,   277,   277,   277,   182,   263,   263,    12,
     322,    12,   322,   277,   357,   361,   194,   277,   277,   277,
     236,   277,   277,   277,   182,    36,   183,   183,   277,   145,
     179,   183,   183,   183,   295,   224,   119,   183,    20,    21,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   122,   123,   124,   125,   126,   127,
     130,   131,   132,   133,   134,   135,   154,   155,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   176,   177,
     179,   184,   145,   185,   283,   145,   322,   114,   123,   154,
     293,   322,   322,   322,   322,   180,   318,   322,   225,   322,
     225,   322,   225,   145,   287,   288,   322,   290,   178,   178,
     178,   292,   114,   154,   156,   159,   291,   285,   232,    19,
     179,   236,   254,   235,   277,   114,   154,   183,    13,   277,
     232,   247,   249,   277,   251,   156,   139,   322,   235,   277,
     183,   182,   313,    49,   145,   162,   179,   277,   352,   353,
     354,   356,   357,   358,   359,   313,   179,   353,   359,   128,
     183,   185,   149,   150,   192,   198,   180,   160,   236,   184,
     232,   177,   180,   257,   277,   136,   262,   263,    18,   145,
     145,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   145,   120,   121,
     277,   277,   145,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   120,   121,   277,
      21,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   145,   275,   277,   180,   257,   301,   154,   183,
     177,   185,   224,   280,   282,   154,   183,   293,   235,   277,
     161,   183,   161,   161,   161,   183,   161,   226,   161,   226,
     161,   226,   156,   161,   183,   161,   183,   178,   322,   235,
     277,   257,   180,   257,   160,   277,   322,   183,   232,   180,
     322,   322,   180,   322,   161,   183,    19,   263,   123,   291,
     180,   153,   183,   354,   178,   153,   178,   183,    19,   180,
     354,   185,   277,   357,   185,   277,   147,   196,   270,   272,
     234,    11,    22,    23,   209,   210,   145,   352,   153,   180,
     114,   123,   154,   159,   260,   261,   225,   160,   179,   277,
     277,   178,   277,   277,   145,   156,     1,   178,   180,   185,
     277,   223,     7,    42,   281,   183,   235,   277,   183,   326,
     322,   330,   324,   321,   332,   161,   336,   161,   340,   161,
     322,   344,   287,   346,   154,   291,   183,   180,   145,   180,
     161,   161,   161,   161,   262,   257,   277,   277,   353,   354,
     145,   352,   178,   178,   277,   178,   358,   257,   353,   185,
     178,   151,   147,   322,   225,   185,   232,   277,   178,   277,
     145,   145,   145,   145,   153,   178,   226,   278,   180,   257,
     277,   276,   183,   178,    43,   281,   292,   183,   161,   334,
     338,   342,   277,   232,   161,   183,   255,   248,   250,   252,
     179,   183,   354,   178,   123,   291,   178,   183,   354,   178,
     161,   226,   232,   180,   260,   178,   128,   232,   258,   322,
     180,   183,   284,   231,   328,   277,   145,   277,   277,   277,
     180,   277,   277,   178,   178,   277,   277,   277,   185,   271,
     161,   210,   114,   277,   161,   230,   180,   161,   180,   183,
     363,   178,   363,   178,   236,   273,   277,   279,   183,   277,
      49,   178,   185,   236,   180,   277,   178,   178
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
     218,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     220,   220,   221,   221,   221,   222,   222,   223,   223,   224,
     224,   225,   225,   225,   226,   226,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   228,
     229,   229,   229,   230,   231,   232,   232,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   234,   234,   234,   235,
     235,   235,   236,   236,   236,   237,   238,   238,   238,   238,
     239,   240,   241,   241,   241,   241,   241,   242,   242,   242,
     242,   243,   244,   244,   245,   247,   248,   246,   249,   250,
     246,   251,   252,   246,   254,   255,   253,   256,   256,   256,
     257,   257,   258,   258,   258,   259,   259,   259,   260,   260,
     260,   260,   261,   261,   262,   262,   263,   263,   264,   264,
     264,   264,   264,   264,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   266,   266,   266,   266,   267,   268,   268,
     269,   270,   271,   269,   272,   273,   269,   274,   275,   276,
     274,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   278,   279,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   280,   280,   281,   281,   282,   283,   283,   284,   283,
     283,   283,   285,   285,   286,   286,   287,   287,   288,   288,
     289,   290,   290,   291,   291,   292,   292,   292,   292,   292,
     292,   293,   293,   293,   294,   294,   295,   295,   295,   295,
     295,   296,   296,   297,   297,   298,   298,   298,   299,   300,
     299,   301,   301,   301,   302,   303,   303,   304,   305,   305,
     305,   306,   306,   307,   307,   308,   309,   309,   310,   310,
     310,   312,   311,   313,   313,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     315,   315,   315,   315,   315,   315,   316,   317,   317,   318,
     318,   320,   321,   319,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   323,   324,   322,   322,   325,   326,   322,   327,   328,
     322,   329,   330,   322,   322,   331,   332,   322,   333,   334,
     322,   322,   335,   336,   322,   337,   338,   322,   322,   339,
     340,   322,   341,   342,   322,   343,   344,   322,   345,   346,
     322,   348,   347,   350,   349,   351,   351,   351,   351,   352,
     352,   352,   352,   353,   353,   354,   354,   355,   355,   355,
     355,   355,   355,   356,   356,   357,   358,   358,   359,   359,
     360,   360,   361,   361,   362,   363,   363,   364,   364
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
       1,     3,     3,     3,     3,     3,     3,     3,     1,     5,
       1,     3,     1,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       0,     1,     1,     4,     2,     3,     7,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     3,
       2,     2,     1,     3,     2,     2,     2,     4,     5,     2,
       1,     1,     2,     3,     4,     2,     3,     3,     4,     2,
       3,     4,     1,     1,     2,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     1,     3,     0,     4,     1,     6,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     4,     4,     6,     5,     6,
       3,     0,     0,     8,     0,     0,     9,     3,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     4,     3,     1,     3,     4,     3,     4,     2,     4,
       4,     7,     8,     3,     5,     0,     0,     8,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     4,     0,     1,     3,     0,     3,     0,     7,
       5,     5,     2,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     4,     6,     5,     5,
       4,     0,     2,     0,     1,     0,     1,     1,     6,     0,
       5,     0,     3,     5,     4,     1,     2,     4,     0,     1,
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
    case 145: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3387 "ds_parser.cpp"
        break;

    case 190: /* module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3393 "ds_parser.cpp"
        break;

    case 192: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3399 "ds_parser.cpp"
        break;

    case 193: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3405 "ds_parser.cpp"
        break;

    case 194: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3411 "ds_parser.cpp"
        break;

    case 195: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3417 "ds_parser.cpp"
        break;

    case 197: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3423 "ds_parser.cpp"
        break;

    case 201: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3429 "ds_parser.cpp"
        break;

    case 207: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3435 "ds_parser.cpp"
        break;

    case 208: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3441 "ds_parser.cpp"
        break;

    case 210: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3447 "ds_parser.cpp"
        break;

    case 212: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3453 "ds_parser.cpp"
        break;

    case 213: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3459 "ds_parser.cpp"
        break;

    case 214: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3465 "ds_parser.cpp"
        break;

    case 215: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3471 "ds_parser.cpp"
        break;

    case 216: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3477 "ds_parser.cpp"
        break;

    case 217: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3483 "ds_parser.cpp"
        break;

    case 218: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3489 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3495 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3501 "ds_parser.cpp"
        break;

    case 221: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3507 "ds_parser.cpp"
        break;

    case 222: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3513 "ds_parser.cpp"
        break;

    case 223: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3519 "ds_parser.cpp"
        break;

    case 224: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3525 "ds_parser.cpp"
        break;

    case 225: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3531 "ds_parser.cpp"
        break;

    case 226: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3537 "ds_parser.cpp"
        break;

    case 227: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3543 "ds_parser.cpp"
        break;

    case 232: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3549 "ds_parser.cpp"
        break;

    case 233: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3555 "ds_parser.cpp"
        break;

    case 234: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3561 "ds_parser.cpp"
        break;

    case 235: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3567 "ds_parser.cpp"
        break;

    case 236: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3573 "ds_parser.cpp"
        break;

    case 237: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3579 "ds_parser.cpp"
        break;

    case 238: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3585 "ds_parser.cpp"
        break;

    case 239: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3591 "ds_parser.cpp"
        break;

    case 240: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3597 "ds_parser.cpp"
        break;

    case 241: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3603 "ds_parser.cpp"
        break;

    case 242: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3609 "ds_parser.cpp"
        break;

    case 243: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3615 "ds_parser.cpp"
        break;

    case 245: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3621 "ds_parser.cpp"
        break;

    case 246: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3627 "ds_parser.cpp"
        break;

    case 253: /* expr_type_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3633 "ds_parser.cpp"
        break;

    case 256: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3639 "ds_parser.cpp"
        break;

    case 257: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3645 "ds_parser.cpp"
        break;

    case 258: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 260: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3657 "ds_parser.cpp"
        break;

    case 261: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3663 "ds_parser.cpp"
        break;

    case 262: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3669 "ds_parser.cpp"
        break;

    case 263: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 264: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 265: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 266: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 267: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 268: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
        break;

    case 269: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3711 "ds_parser.cpp"
        break;

    case 274: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3717 "ds_parser.cpp"
        break;

    case 277: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3723 "ds_parser.cpp"
        break;

    case 280: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3729 "ds_parser.cpp"
        break;

    case 282: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3735 "ds_parser.cpp"
        break;

    case 283: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3741 "ds_parser.cpp"
        break;

    case 285: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3747 "ds_parser.cpp"
        break;

    case 286: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3753 "ds_parser.cpp"
        break;

    case 287: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3759 "ds_parser.cpp"
        break;

    case 288: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3765 "ds_parser.cpp"
        break;

    case 289: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3771 "ds_parser.cpp"
        break;

    case 290: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3777 "ds_parser.cpp"
        break;

    case 292: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3783 "ds_parser.cpp"
        break;

    case 295: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3789 "ds_parser.cpp"
        break;

    case 296: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3795 "ds_parser.cpp"
        break;

    case 301: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3801 "ds_parser.cpp"
        break;

    case 307: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3807 "ds_parser.cpp"
        break;

    case 313: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3813 "ds_parser.cpp"
        break;

    case 316: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3819 "ds_parser.cpp"
        break;

    case 317: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3825 "ds_parser.cpp"
        break;

    case 318: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3831 "ds_parser.cpp"
        break;

    case 319: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3837 "ds_parser.cpp"
        break;

    case 322: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3843 "ds_parser.cpp"
        break;

    case 351: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3849 "ds_parser.cpp"
        break;

    case 352: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3855 "ds_parser.cpp"
        break;

    case 353: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3861 "ds_parser.cpp"
        break;

    case 354: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3867 "ds_parser.cpp"
        break;

    case 355: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3873 "ds_parser.cpp"
        break;

    case 356: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3879 "ds_parser.cpp"
        break;

    case 357: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3885 "ds_parser.cpp"
        break;

    case 358: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3891 "ds_parser.cpp"
        break;

    case 359: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3897 "ds_parser.cpp"
        break;

    case 360: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3903 "ds_parser.cpp"
        break;

    case 361: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3909 "ds_parser.cpp"
        break;

    case 362: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3915 "ds_parser.cpp"
        break;

    case 363: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3921 "ds_parser.cpp"
        break;

    case 364: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3927 "ds_parser.cpp"
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
#line 4219 "ds_parser.cpp"
    break;

  case 16:
#line 469 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4225 "ds_parser.cpp"
    break;

  case 17:
#line 470 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4231 "ds_parser.cpp"
    break;

  case 18:
#line 474 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4237 "ds_parser.cpp"
    break;

  case 19:
#line 475 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4243 "ds_parser.cpp"
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
#line 4262 "ds_parser.cpp"
    break;

  case 21:
#line 496 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4268 "ds_parser.cpp"
    break;

  case 22:
#line 497 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4274 "ds_parser.cpp"
    break;

  case 23:
#line 498 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4280 "ds_parser.cpp"
    break;

  case 24:
#line 499 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4286 "ds_parser.cpp"
    break;

  case 25:
#line 503 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4292 "ds_parser.cpp"
    break;

  case 26:
#line 507 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4302 "ds_parser.cpp"
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
#line 4316 "ds_parser.cpp"
    break;

  case 28:
#line 521 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4326 "ds_parser.cpp"
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
#line 4344 "ds_parser.cpp"
    break;

  case 30:
#line 545 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4354 "ds_parser.cpp"
    break;

  case 31:
#line 550 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4364 "ds_parser.cpp"
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
#line 4391 "ds_parser.cpp"
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
#line 4403 "ds_parser.cpp"
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
#line 4427 "ds_parser.cpp"
    break;

  case 36:
#line 614 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4435 "ds_parser.cpp"
    break;

  case 37:
#line 617 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4444 "ds_parser.cpp"
    break;

  case 38:
#line 621 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4455 "ds_parser.cpp"
    break;

  case 39:
#line 627 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4466 "ds_parser.cpp"
    break;

  case 40:
#line 636 "ds_parser.ypp"
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
#line 4492 "ds_parser.cpp"
    break;

  case 41:
#line 657 "ds_parser.ypp"
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
#line 4517 "ds_parser.cpp"
    break;

  case 42:
#line 680 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4523 "ds_parser.cpp"
    break;

  case 43:
#line 681 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4529 "ds_parser.cpp"
    break;

  case 47:
#line 694 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4537 "ds_parser.cpp"
    break;

  case 48:
#line 697 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4545 "ds_parser.cpp"
    break;

  case 49:
#line 703 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4553 "ds_parser.cpp"
    break;

  case 50:
#line 709 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4561 "ds_parser.cpp"
    break;

  case 51:
#line 712 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4569 "ds_parser.cpp"
    break;

  case 52:
#line 718 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4575 "ds_parser.cpp"
    break;

  case 53:
#line 719 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4581 "ds_parser.cpp"
    break;

  case 54:
#line 723 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4587 "ds_parser.cpp"
    break;

  case 55:
#line 724 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4593 "ds_parser.cpp"
    break;

  case 56:
#line 725 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4604 "ds_parser.cpp"
    break;

  case 57:
#line 734 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4610 "ds_parser.cpp"
    break;

  case 58:
#line 735 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4616 "ds_parser.cpp"
    break;

  case 59:
#line 739 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4627 "ds_parser.cpp"
    break;

  case 60:
#line 748 "ds_parser.ypp"
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
#line 4645 "ds_parser.cpp"
    break;

  case 61:
#line 764 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4655 "ds_parser.cpp"
    break;

  case 62:
#line 772 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4667 "ds_parser.cpp"
    break;

  case 63:
#line 782 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4678 "ds_parser.cpp"
    break;

  case 64:
#line 791 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4684 "ds_parser.cpp"
    break;

  case 65:
#line 792 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4690 "ds_parser.cpp"
    break;

  case 66:
#line 793 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4696 "ds_parser.cpp"
    break;

  case 67:
#line 794 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4702 "ds_parser.cpp"
    break;

  case 68:
#line 795 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4708 "ds_parser.cpp"
    break;

  case 69:
#line 796 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4714 "ds_parser.cpp"
    break;

  case 70:
#line 800 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4724 "ds_parser.cpp"
    break;

  case 71:
#line 805 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4734 "ds_parser.cpp"
    break;

  case 72:
#line 813 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4740 "ds_parser.cpp"
    break;

  case 73:
#line 814 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4746 "ds_parser.cpp"
    break;

  case 74:
#line 815 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4752 "ds_parser.cpp"
    break;

  case 75:
#line 816 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4758 "ds_parser.cpp"
    break;

  case 76:
#line 817 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4764 "ds_parser.cpp"
    break;

  case 77:
#line 818 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4770 "ds_parser.cpp"
    break;

  case 78:
#line 819 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4776 "ds_parser.cpp"
    break;

  case 79:
#line 820 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4784 "ds_parser.cpp"
    break;

  case 80:
#line 826 "ds_parser.ypp"
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
#line 4803 "ds_parser.cpp"
    break;

  case 81:
#line 840 "ds_parser.ypp"
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
#line 4822 "ds_parser.cpp"
    break;

  case 82:
#line 857 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4828 "ds_parser.cpp"
    break;

  case 83:
#line 858 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4834 "ds_parser.cpp"
    break;

  case 84:
#line 859 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4840 "ds_parser.cpp"
    break;

  case 85:
#line 863 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4853 "ds_parser.cpp"
    break;

  case 86:
#line 871 "ds_parser.ypp"
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
#line 4868 "ds_parser.cpp"
    break;

  case 87:
#line 884 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4877 "ds_parser.cpp"
    break;

  case 88:
#line 888 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4886 "ds_parser.cpp"
    break;

  case 89:
#line 895 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4892 "ds_parser.cpp"
    break;

  case 90:
#line 896 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4898 "ds_parser.cpp"
    break;

  case 91:
#line 900 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4904 "ds_parser.cpp"
    break;

  case 92:
#line 901 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4910 "ds_parser.cpp"
    break;

  case 93:
#line 902 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4916 "ds_parser.cpp"
    break;

  case 94:
#line 906 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 4924 "ds_parser.cpp"
    break;

  case 95:
#line 909 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4933 "ds_parser.cpp"
    break;

  case 96:
#line 916 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4942 "ds_parser.cpp"
    break;

  case 97:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4948 "ds_parser.cpp"
    break;

  case 98:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4954 "ds_parser.cpp"
    break;

  case 99:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4960 "ds_parser.cpp"
    break;

  case 100:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4966 "ds_parser.cpp"
    break;

  case 101:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4972 "ds_parser.cpp"
    break;

  case 102:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4978 "ds_parser.cpp"
    break;

  case 103:
#line 926 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4984 "ds_parser.cpp"
    break;

  case 104:
#line 927 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4990 "ds_parser.cpp"
    break;

  case 105:
#line 928 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4996 "ds_parser.cpp"
    break;

  case 106:
#line 929 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5002 "ds_parser.cpp"
    break;

  case 107:
#line 930 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5008 "ds_parser.cpp"
    break;

  case 108:
#line 931 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5014 "ds_parser.cpp"
    break;

  case 109:
#line 932 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5020 "ds_parser.cpp"
    break;

  case 110:
#line 933 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5026 "ds_parser.cpp"
    break;

  case 111:
#line 934 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5032 "ds_parser.cpp"
    break;

  case 112:
#line 935 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5038 "ds_parser.cpp"
    break;

  case 113:
#line 936 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5044 "ds_parser.cpp"
    break;

  case 114:
#line 937 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5050 "ds_parser.cpp"
    break;

  case 115:
#line 938 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5056 "ds_parser.cpp"
    break;

  case 116:
#line 939 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5062 "ds_parser.cpp"
    break;

  case 117:
#line 940 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5068 "ds_parser.cpp"
    break;

  case 118:
#line 941 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5074 "ds_parser.cpp"
    break;

  case 119:
#line 942 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5080 "ds_parser.cpp"
    break;

  case 120:
#line 943 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5086 "ds_parser.cpp"
    break;

  case 121:
#line 944 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5092 "ds_parser.cpp"
    break;

  case 122:
#line 945 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5098 "ds_parser.cpp"
    break;

  case 123:
#line 946 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5104 "ds_parser.cpp"
    break;

  case 124:
#line 947 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5110 "ds_parser.cpp"
    break;

  case 125:
#line 948 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5116 "ds_parser.cpp"
    break;

  case 126:
#line 949 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5122 "ds_parser.cpp"
    break;

  case 127:
#line 950 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5128 "ds_parser.cpp"
    break;

  case 128:
#line 951 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5134 "ds_parser.cpp"
    break;

  case 129:
#line 952 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5140 "ds_parser.cpp"
    break;

  case 130:
#line 953 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5146 "ds_parser.cpp"
    break;

  case 131:
#line 954 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5152 "ds_parser.cpp"
    break;

  case 132:
#line 955 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5158 "ds_parser.cpp"
    break;

  case 133:
#line 956 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5164 "ds_parser.cpp"
    break;

  case 134:
#line 957 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5170 "ds_parser.cpp"
    break;

  case 135:
#line 958 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5176 "ds_parser.cpp"
    break;

  case 136:
#line 959 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5182 "ds_parser.cpp"
    break;

  case 137:
#line 960 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5188 "ds_parser.cpp"
    break;

  case 138:
#line 961 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5194 "ds_parser.cpp"
    break;

  case 139:
#line 965 "ds_parser.ypp"
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
#line 5218 "ds_parser.cpp"
    break;

  case 140:
#line 987 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5224 "ds_parser.cpp"
    break;

  case 141:
#line 988 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5230 "ds_parser.cpp"
    break;

  case 142:
#line 989 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5236 "ds_parser.cpp"
    break;

  case 143:
#line 993 "ds_parser.ypp"
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
#line 5277 "ds_parser.cpp"
    break;

  case 144:
#line 1032 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5286 "ds_parser.cpp"
    break;

  case 145:
#line 1039 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5295 "ds_parser.cpp"
    break;

  case 146:
#line 1043 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5308 "ds_parser.cpp"
    break;

  case 147:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5314 "ds_parser.cpp"
    break;

  case 148:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5320 "ds_parser.cpp"
    break;

  case 149:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5326 "ds_parser.cpp"
    break;

  case 150:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5332 "ds_parser.cpp"
    break;

  case 151:
#line 1058 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5338 "ds_parser.cpp"
    break;

  case 152:
#line 1059 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5344 "ds_parser.cpp"
    break;

  case 153:
#line 1060 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5350 "ds_parser.cpp"
    break;

  case 154:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5356 "ds_parser.cpp"
    break;

  case 155:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5362 "ds_parser.cpp"
    break;

  case 156:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5368 "ds_parser.cpp"
    break;

  case 157:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5374 "ds_parser.cpp"
    break;

  case 158:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5380 "ds_parser.cpp"
    break;

  case 159:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5386 "ds_parser.cpp"
    break;

  case 160:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5392 "ds_parser.cpp"
    break;

  case 161:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5398 "ds_parser.cpp"
    break;

  case 162:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5404 "ds_parser.cpp"
    break;

  case 163:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5410 "ds_parser.cpp"
    break;

  case 164:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5416 "ds_parser.cpp"
    break;

  case 165:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5422 "ds_parser.cpp"
    break;

  case 166:
#line 1076 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5432 "ds_parser.cpp"
    break;

  case 167:
#line 1081 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5443 "ds_parser.cpp"
    break;

  case 168:
#line 1087 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5451 "ds_parser.cpp"
    break;

  case 169:
#line 1093 "ds_parser.ypp"
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
#line 5490 "ds_parser.cpp"
    break;

  case 170:
#line 1127 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5498 "ds_parser.cpp"
    break;

  case 171:
#line 1130 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5506 "ds_parser.cpp"
    break;

  case 172:
#line 1136 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5512 "ds_parser.cpp"
    break;

  case 173:
#line 1137 "ds_parser.ypp"
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
#line 5528 "ds_parser.cpp"
    break;

  case 174:
#line 1148 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5534 "ds_parser.cpp"
    break;

  case 175:
#line 1152 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5542 "ds_parser.cpp"
    break;

  case 176:
#line 1158 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5550 "ds_parser.cpp"
    break;

  case 177:
#line 1161 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5558 "ds_parser.cpp"
    break;

  case 178:
#line 1164 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5567 "ds_parser.cpp"
    break;

  case 179:
#line 1168 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5575 "ds_parser.cpp"
    break;

  case 180:
#line 1174 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5581 "ds_parser.cpp"
    break;

  case 181:
#line 1178 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5587 "ds_parser.cpp"
    break;

  case 182:
#line 1182 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5595 "ds_parser.cpp"
    break;

  case 183:
#line 1185 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5603 "ds_parser.cpp"
    break;

  case 184:
#line 1188 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5613 "ds_parser.cpp"
    break;

  case 185:
#line 1193 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5621 "ds_parser.cpp"
    break;

  case 186:
#line 1196 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5631 "ds_parser.cpp"
    break;

  case 187:
#line 1204 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5639 "ds_parser.cpp"
    break;

  case 188:
#line 1207 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5649 "ds_parser.cpp"
    break;

  case 189:
#line 1212 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5657 "ds_parser.cpp"
    break;

  case 190:
#line 1215 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5667 "ds_parser.cpp"
    break;

  case 191:
#line 1223 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5675 "ds_parser.cpp"
    break;

  case 192:
#line 1229 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5681 "ds_parser.cpp"
    break;

  case 193:
#line 1230 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5687 "ds_parser.cpp"
    break;

  case 194:
#line 1234 "ds_parser.ypp"
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
#line 5723 "ds_parser.cpp"
    break;

  case 195:
#line 1268 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5729 "ds_parser.cpp"
    break;

  case 196:
#line 1268 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5735 "ds_parser.cpp"
    break;

  case 197:
#line 1268 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5743 "ds_parser.cpp"
    break;

  case 198:
#line 1271 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5749 "ds_parser.cpp"
    break;

  case 199:
#line 1271 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5755 "ds_parser.cpp"
    break;

  case 200:
#line 1271 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5765 "ds_parser.cpp"
    break;

  case 201:
#line 1276 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5771 "ds_parser.cpp"
    break;

  case 202:
#line 1276 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5777 "ds_parser.cpp"
    break;

  case 203:
#line 1276 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5787 "ds_parser.cpp"
    break;

  case 204:
#line 1284 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5793 "ds_parser.cpp"
    break;

  case 205:
#line 1284 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5799 "ds_parser.cpp"
    break;

  case 206:
#line 1284 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5807 "ds_parser.cpp"
    break;

  case 207:
#line 1290 "ds_parser.ypp"
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
#line 5822 "ds_parser.cpp"
    break;

  case 208:
#line 1300 "ds_parser.ypp"
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
#line 5838 "ds_parser.cpp"
    break;

  case 209:
#line 1311 "ds_parser.ypp"
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
#line 5855 "ds_parser.cpp"
    break;

  case 210:
#line 1326 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5863 "ds_parser.cpp"
    break;

  case 211:
#line 1329 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5871 "ds_parser.cpp"
    break;

  case 212:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5877 "ds_parser.cpp"
    break;

  case 213:
#line 1336 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5889 "ds_parser.cpp"
    break;

  case 214:
#line 1343 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5902 "ds_parser.cpp"
    break;

  case 215:
#line 1354 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5908 "ds_parser.cpp"
    break;

  case 216:
#line 1355 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5914 "ds_parser.cpp"
    break;

  case 217:
#line 1356 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5920 "ds_parser.cpp"
    break;

  case 218:
#line 1360 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5926 "ds_parser.cpp"
    break;

  case 219:
#line 1361 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5932 "ds_parser.cpp"
    break;

  case 220:
#line 1362 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5938 "ds_parser.cpp"
    break;

  case 221:
#line 1363 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5944 "ds_parser.cpp"
    break;

  case 222:
#line 1367 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5954 "ds_parser.cpp"
    break;

  case 223:
#line 1372 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5964 "ds_parser.cpp"
    break;

  case 224:
#line 1380 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5970 "ds_parser.cpp"
    break;

  case 225:
#line 1381 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5976 "ds_parser.cpp"
    break;

  case 226:
#line 1385 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5986 "ds_parser.cpp"
    break;

  case 227:
#line 1391 "ds_parser.ypp"
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
#line 6051 "ds_parser.cpp"
    break;

  case 228:
#line 1454 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6057 "ds_parser.cpp"
    break;

  case 229:
#line 1455 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6063 "ds_parser.cpp"
    break;

  case 230:
#line 1456 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6069 "ds_parser.cpp"
    break;

  case 231:
#line 1457 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6075 "ds_parser.cpp"
    break;

  case 232:
#line 1458 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6081 "ds_parser.cpp"
    break;

  case 233:
#line 1459 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6087 "ds_parser.cpp"
    break;

  case 234:
#line 1463 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6093 "ds_parser.cpp"
    break;

  case 235:
#line 1464 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6099 "ds_parser.cpp"
    break;

  case 236:
#line 1465 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6105 "ds_parser.cpp"
    break;

  case 237:
#line 1466 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6111 "ds_parser.cpp"
    break;

  case 238:
#line 1467 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6117 "ds_parser.cpp"
    break;

  case 239:
#line 1468 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6123 "ds_parser.cpp"
    break;

  case 240:
#line 1469 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6129 "ds_parser.cpp"
    break;

  case 241:
#line 1470 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6135 "ds_parser.cpp"
    break;

  case 242:
#line 1471 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6141 "ds_parser.cpp"
    break;

  case 243:
#line 1472 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6147 "ds_parser.cpp"
    break;

  case 244:
#line 1473 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6153 "ds_parser.cpp"
    break;

  case 245:
#line 1474 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6159 "ds_parser.cpp"
    break;

  case 246:
#line 1475 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6165 "ds_parser.cpp"
    break;

  case 247:
#line 1476 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6171 "ds_parser.cpp"
    break;

  case 248:
#line 1477 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6177 "ds_parser.cpp"
    break;

  case 249:
#line 1478 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6183 "ds_parser.cpp"
    break;

  case 250:
#line 1479 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6189 "ds_parser.cpp"
    break;

  case 251:
#line 1480 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6195 "ds_parser.cpp"
    break;

  case 252:
#line 1481 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6201 "ds_parser.cpp"
    break;

  case 253:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6207 "ds_parser.cpp"
    break;

  case 254:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6213 "ds_parser.cpp"
    break;

  case 255:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6219 "ds_parser.cpp"
    break;

  case 256:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6225 "ds_parser.cpp"
    break;

  case 257:
#line 1492 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6237 "ds_parser.cpp"
    break;

  case 258:
#line 1503 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6247 "ds_parser.cpp"
    break;

  case 259:
#line 1508 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6259 "ds_parser.cpp"
    break;

  case 260:
#line 1518 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6268 "ds_parser.cpp"
    break;

  case 261:
#line 1522 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6274 "ds_parser.cpp"
    break;

  case 262:
#line 1522 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6280 "ds_parser.cpp"
    break;

  case 263:
#line 1522 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6292 "ds_parser.cpp"
    break;

  case 264:
#line 1529 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6298 "ds_parser.cpp"
    break;

  case 265:
#line 1529 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6304 "ds_parser.cpp"
    break;

  case 266:
#line 1529 "ds_parser.ypp"
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
#line 6320 "ds_parser.cpp"
    break;

  case 267:
#line 1543 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6329 "ds_parser.cpp"
    break;

  case 268:
#line 1547 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6335 "ds_parser.cpp"
    break;

  case 269:
#line 1547 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6341 "ds_parser.cpp"
    break;

  case 270:
#line 1547 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6350 "ds_parser.cpp"
    break;

  case 271:
#line 1554 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6356 "ds_parser.cpp"
    break;

  case 272:
#line 1555 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6362 "ds_parser.cpp"
    break;

  case 273:
#line 1556 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6368 "ds_parser.cpp"
    break;

  case 274:
#line 1557 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6374 "ds_parser.cpp"
    break;

  case 275:
#line 1558 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6380 "ds_parser.cpp"
    break;

  case 276:
#line 1559 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6386 "ds_parser.cpp"
    break;

  case 277:
#line 1560 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6392 "ds_parser.cpp"
    break;

  case 278:
#line 1561 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6398 "ds_parser.cpp"
    break;

  case 279:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6404 "ds_parser.cpp"
    break;

  case 280:
#line 1563 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6410 "ds_parser.cpp"
    break;

  case 281:
#line 1564 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6416 "ds_parser.cpp"
    break;

  case 282:
#line 1565 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6422 "ds_parser.cpp"
    break;

  case 283:
#line 1566 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6428 "ds_parser.cpp"
    break;

  case 284:
#line 1567 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6434 "ds_parser.cpp"
    break;

  case 285:
#line 1568 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6440 "ds_parser.cpp"
    break;

  case 286:
#line 1569 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6446 "ds_parser.cpp"
    break;

  case 287:
#line 1570 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6452 "ds_parser.cpp"
    break;

  case 288:
#line 1571 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6458 "ds_parser.cpp"
    break;

  case 289:
#line 1572 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6464 "ds_parser.cpp"
    break;

  case 290:
#line 1573 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6470 "ds_parser.cpp"
    break;

  case 291:
#line 1574 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6476 "ds_parser.cpp"
    break;

  case 292:
#line 1575 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6482 "ds_parser.cpp"
    break;

  case 293:
#line 1576 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6488 "ds_parser.cpp"
    break;

  case 294:
#line 1577 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6494 "ds_parser.cpp"
    break;

  case 295:
#line 1578 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6500 "ds_parser.cpp"
    break;

  case 296:
#line 1579 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6506 "ds_parser.cpp"
    break;

  case 297:
#line 1580 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6512 "ds_parser.cpp"
    break;

  case 298:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6518 "ds_parser.cpp"
    break;

  case 299:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6524 "ds_parser.cpp"
    break;

  case 300:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6530 "ds_parser.cpp"
    break;

  case 301:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6536 "ds_parser.cpp"
    break;

  case 302:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6542 "ds_parser.cpp"
    break;

  case 303:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6548 "ds_parser.cpp"
    break;

  case 304:
#line 1587 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6554 "ds_parser.cpp"
    break;

  case 305:
#line 1588 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6560 "ds_parser.cpp"
    break;

  case 306:
#line 1589 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6566 "ds_parser.cpp"
    break;

  case 307:
#line 1590 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6572 "ds_parser.cpp"
    break;

  case 308:
#line 1591 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6578 "ds_parser.cpp"
    break;

  case 309:
#line 1592 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6584 "ds_parser.cpp"
    break;

  case 310:
#line 1593 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6590 "ds_parser.cpp"
    break;

  case 311:
#line 1594 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6596 "ds_parser.cpp"
    break;

  case 312:
#line 1595 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6602 "ds_parser.cpp"
    break;

  case 313:
#line 1596 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6608 "ds_parser.cpp"
    break;

  case 314:
#line 1597 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6617 "ds_parser.cpp"
    break;

  case 315:
#line 1601 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6626 "ds_parser.cpp"
    break;

  case 316:
#line 1605 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6634 "ds_parser.cpp"
    break;

  case 317:
#line 1608 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6642 "ds_parser.cpp"
    break;

  case 318:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6648 "ds_parser.cpp"
    break;

  case 319:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6654 "ds_parser.cpp"
    break;

  case 320:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6660 "ds_parser.cpp"
    break;

  case 321:
#line 1614 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6674 "ds_parser.cpp"
    break;

  case 322:
#line 1623 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6688 "ds_parser.cpp"
    break;

  case 323:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6694 "ds_parser.cpp"
    break;

  case 324:
#line 1633 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6702 "ds_parser.cpp"
    break;

  case 325:
#line 1636 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6708 "ds_parser.cpp"
    break;

  case 326:
#line 1636 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6714 "ds_parser.cpp"
    break;

  case 327:
#line 1636 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6722 "ds_parser.cpp"
    break;

  case 328:
#line 1639 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6731 "ds_parser.cpp"
    break;

  case 329:
#line 1643 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6740 "ds_parser.cpp"
    break;

  case 330:
#line 1647 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6749 "ds_parser.cpp"
    break;

  case 331:
#line 1651 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6755 "ds_parser.cpp"
    break;

  case 332:
#line 1652 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6761 "ds_parser.cpp"
    break;

  case 333:
#line 1653 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6767 "ds_parser.cpp"
    break;

  case 334:
#line 1654 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6773 "ds_parser.cpp"
    break;

  case 335:
#line 1655 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6779 "ds_parser.cpp"
    break;

  case 336:
#line 1656 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6785 "ds_parser.cpp"
    break;

  case 337:
#line 1657 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6791 "ds_parser.cpp"
    break;

  case 338:
#line 1658 "ds_parser.ypp"
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
#line 6812 "ds_parser.cpp"
    break;

  case 339:
#line 1674 "ds_parser.ypp"
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
#line 6833 "ds_parser.cpp"
    break;

  case 340:
#line 1690 "ds_parser.ypp"
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
#line 6893 "ds_parser.cpp"
    break;

  case 341:
#line 1748 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6899 "ds_parser.cpp"
    break;

  case 342:
#line 1749 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6905 "ds_parser.cpp"
    break;

  case 343:
#line 1753 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6911 "ds_parser.cpp"
    break;

  case 344:
#line 1754 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6917 "ds_parser.cpp"
    break;

  case 345:
#line 1758 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6927 "ds_parser.cpp"
    break;

  case 346:
#line 1766 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6935 "ds_parser.cpp"
    break;

  case 347:
#line 1769 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6944 "ds_parser.cpp"
    break;

  case 348:
#line 1774 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6950 "ds_parser.cpp"
    break;

  case 349:
#line 1774 "ds_parser.ypp"
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
#line 6999 "ds_parser.cpp"
    break;

  case 350:
#line 1818 "ds_parser.ypp"
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
#line 7060 "ds_parser.cpp"
    break;

  case 351:
#line 1874 "ds_parser.ypp"
    {
        das_yyerror("Structure field or class method annotation expected to remain on the same line with field or class.",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7071 "ds_parser.cpp"
    break;

  case 352:
#line 1883 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7081 "ds_parser.cpp"
    break;

  case 353:
#line 1888 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7091 "ds_parser.cpp"
    break;

  case 354:
#line 1896 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7097 "ds_parser.cpp"
    break;

  case 355:
#line 1897 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7103 "ds_parser.cpp"
    break;

  case 356:
#line 1901 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7111 "ds_parser.cpp"
    break;

  case 357:
#line 1904 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7122 "ds_parser.cpp"
    break;

  case 358:
#line 1913 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7128 "ds_parser.cpp"
    break;

  case 359:
#line 1914 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7134 "ds_parser.cpp"
    break;

  case 360:
#line 1918 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7145 "ds_parser.cpp"
    break;

  case 361:
#line 1927 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7151 "ds_parser.cpp"
    break;

  case 362:
#line 1928 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7157 "ds_parser.cpp"
    break;

  case 363:
#line 1933 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7163 "ds_parser.cpp"
    break;

  case 364:
#line 1934 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7169 "ds_parser.cpp"
    break;

  case 365:
#line 1938 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7180 "ds_parser.cpp"
    break;

  case 366:
#line 1944 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7191 "ds_parser.cpp"
    break;

  case 367:
#line 1950 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7199 "ds_parser.cpp"
    break;

  case 368:
#line 1953 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7208 "ds_parser.cpp"
    break;

  case 369:
#line 1957 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7219 "ds_parser.cpp"
    break;

  case 370:
#line 1963 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7230 "ds_parser.cpp"
    break;

  case 371:
#line 1972 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7236 "ds_parser.cpp"
    break;

  case 372:
#line 1973 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7242 "ds_parser.cpp"
    break;

  case 373:
#line 1974 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7248 "ds_parser.cpp"
    break;

  case 374:
#line 1978 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7254 "ds_parser.cpp"
    break;

  case 375:
#line 1979 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7260 "ds_parser.cpp"
    break;

  case 376:
#line 1983 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7268 "ds_parser.cpp"
    break;

  case 377:
#line 1986 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7278 "ds_parser.cpp"
    break;

  case 378:
#line 1991 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7288 "ds_parser.cpp"
    break;

  case 379:
#line 1996 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7301 "ds_parser.cpp"
    break;

  case 380:
#line 2004 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7314 "ds_parser.cpp"
    break;

  case 381:
#line 2015 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7322 "ds_parser.cpp"
    break;

  case 382:
#line 2018 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7331 "ds_parser.cpp"
    break;

  case 383:
#line 2025 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7337 "ds_parser.cpp"
    break;

  case 384:
#line 2026 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7343 "ds_parser.cpp"
    break;

  case 385:
#line 2030 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7349 "ds_parser.cpp"
    break;

  case 386:
#line 2031 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7355 "ds_parser.cpp"
    break;

  case 387:
#line 2032 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7361 "ds_parser.cpp"
    break;

  case 388:
#line 2036 "ds_parser.ypp"
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
#line 7394 "ds_parser.cpp"
    break;

  case 389:
#line 2064 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7400 "ds_parser.cpp"
    break;

  case 390:
#line 2064 "ds_parser.ypp"
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
#line 7432 "ds_parser.cpp"
    break;

  case 391:
#line 2094 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7440 "ds_parser.cpp"
    break;

  case 392:
#line 2097 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7454 "ds_parser.cpp"
    break;

  case 393:
#line 2106 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7468 "ds_parser.cpp"
    break;

  case 394:
#line 2119 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7482 "ds_parser.cpp"
    break;

  case 398:
#line 2140 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7488 "ds_parser.cpp"
    break;

  case 399:
#line 2141 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7494 "ds_parser.cpp"
    break;

  case 400:
#line 2142 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7500 "ds_parser.cpp"
    break;

  case 401:
#line 2146 "ds_parser.ypp"
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
#line 7533 "ds_parser.cpp"
    break;

  case 402:
#line 2174 "ds_parser.ypp"
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
#line 7567 "ds_parser.cpp"
    break;

  case 403:
#line 2206 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7573 "ds_parser.cpp"
    break;

  case 404:
#line 2207 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7579 "ds_parser.cpp"
    break;

  case 405:
#line 2211 "ds_parser.ypp"
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
#line 7618 "ds_parser.cpp"
    break;

  case 406:
#line 2248 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7624 "ds_parser.cpp"
    break;

  case 407:
#line 2249 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7630 "ds_parser.cpp"
    break;

  case 408:
#line 2253 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7636 "ds_parser.cpp"
    break;

  case 409:
#line 2254 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7642 "ds_parser.cpp"
    break;

  case 410:
#line 2255 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7648 "ds_parser.cpp"
    break;

  case 411:
#line 2260 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7654 "ds_parser.cpp"
    break;

  case 412:
#line 2260 "ds_parser.ypp"
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
#line 7740 "ds_parser.cpp"
    break;

  case 413:
#line 2344 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7752 "ds_parser.cpp"
    break;

  case 414:
#line 2351 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7763 "ds_parser.cpp"
    break;

  case 415:
#line 2360 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7769 "ds_parser.cpp"
    break;

  case 416:
#line 2361 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7775 "ds_parser.cpp"
    break;

  case 417:
#line 2362 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7781 "ds_parser.cpp"
    break;

  case 418:
#line 2363 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7787 "ds_parser.cpp"
    break;

  case 419:
#line 2364 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7793 "ds_parser.cpp"
    break;

  case 420:
#line 2365 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7799 "ds_parser.cpp"
    break;

  case 421:
#line 2366 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7805 "ds_parser.cpp"
    break;

  case 422:
#line 2367 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7811 "ds_parser.cpp"
    break;

  case 423:
#line 2368 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7817 "ds_parser.cpp"
    break;

  case 424:
#line 2369 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7823 "ds_parser.cpp"
    break;

  case 425:
#line 2370 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7829 "ds_parser.cpp"
    break;

  case 426:
#line 2371 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7835 "ds_parser.cpp"
    break;

  case 427:
#line 2372 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7841 "ds_parser.cpp"
    break;

  case 428:
#line 2373 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7847 "ds_parser.cpp"
    break;

  case 429:
#line 2374 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7853 "ds_parser.cpp"
    break;

  case 430:
#line 2375 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7859 "ds_parser.cpp"
    break;

  case 431:
#line 2376 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7865 "ds_parser.cpp"
    break;

  case 432:
#line 2377 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7871 "ds_parser.cpp"
    break;

  case 433:
#line 2378 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7877 "ds_parser.cpp"
    break;

  case 434:
#line 2379 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7883 "ds_parser.cpp"
    break;

  case 435:
#line 2380 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7889 "ds_parser.cpp"
    break;

  case 436:
#line 2381 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7895 "ds_parser.cpp"
    break;

  case 437:
#line 2382 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7901 "ds_parser.cpp"
    break;

  case 438:
#line 2383 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7907 "ds_parser.cpp"
    break;

  case 439:
#line 2384 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7913 "ds_parser.cpp"
    break;

  case 440:
#line 2388 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7919 "ds_parser.cpp"
    break;

  case 441:
#line 2389 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7925 "ds_parser.cpp"
    break;

  case 442:
#line 2390 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7931 "ds_parser.cpp"
    break;

  case 443:
#line 2391 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7937 "ds_parser.cpp"
    break;

  case 444:
#line 2392 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7943 "ds_parser.cpp"
    break;

  case 445:
#line 2393 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7949 "ds_parser.cpp"
    break;

  case 446:
#line 2397 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7962 "ds_parser.cpp"
    break;

  case 447:
#line 2408 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7971 "ds_parser.cpp"
    break;

  case 448:
#line 2412 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7983 "ds_parser.cpp"
    break;

  case 449:
#line 2422 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7995 "ds_parser.cpp"
    break;

  case 450:
#line 2429 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8006 "ds_parser.cpp"
    break;

  case 451:
#line 2438 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8012 "ds_parser.cpp"
    break;

  case 452:
#line 2438 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8018 "ds_parser.cpp"
    break;

  case 453:
#line 2438 "ds_parser.ypp"
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
#line 8033 "ds_parser.cpp"
    break;

  case 454:
#line 2451 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8039 "ds_parser.cpp"
    break;

  case 455:
#line 2452 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8045 "ds_parser.cpp"
    break;

  case 456:
#line 2453 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8051 "ds_parser.cpp"
    break;

  case 457:
#line 2454 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8057 "ds_parser.cpp"
    break;

  case 458:
#line 2455 "ds_parser.ypp"
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
#line 8076 "ds_parser.cpp"
    break;

  case 459:
#line 2469 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8087 "ds_parser.cpp"
    break;

  case 460:
#line 2475 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8096 "ds_parser.cpp"
    break;

  case 461:
#line 2479 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8105 "ds_parser.cpp"
    break;

  case 462:
#line 2483 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8115 "ds_parser.cpp"
    break;

  case 463:
#line 2488 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8125 "ds_parser.cpp"
    break;

  case 464:
#line 2493 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8135 "ds_parser.cpp"
    break;

  case 465:
#line 2498 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8145 "ds_parser.cpp"
    break;

  case 466:
#line 2503 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8154 "ds_parser.cpp"
    break;

  case 467:
#line 2507 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8163 "ds_parser.cpp"
    break;

  case 468:
#line 2511 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8173 "ds_parser.cpp"
    break;

  case 469:
#line 2516 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8182 "ds_parser.cpp"
    break;

  case 470:
#line 2520 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8192 "ds_parser.cpp"
    break;

  case 471:
#line 2525 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8198 "ds_parser.cpp"
    break;

  case 472:
#line 2525 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8204 "ds_parser.cpp"
    break;

  case 473:
#line 2525 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8215 "ds_parser.cpp"
    break;

  case 474:
#line 2531 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8227 "ds_parser.cpp"
    break;

  case 475:
#line 2538 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8233 "ds_parser.cpp"
    break;

  case 476:
#line 2538 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8239 "ds_parser.cpp"
    break;

  case 477:
#line 2538 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8249 "ds_parser.cpp"
    break;

  case 478:
#line 2543 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8255 "ds_parser.cpp"
    break;

  case 479:
#line 2543 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8261 "ds_parser.cpp"
    break;

  case 480:
#line 2543 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8272 "ds_parser.cpp"
    break;

  case 481:
#line 2549 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8278 "ds_parser.cpp"
    break;

  case 482:
#line 2549 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8284 "ds_parser.cpp"
    break;

  case 483:
#line 2549 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8294 "ds_parser.cpp"
    break;

  case 484:
#line 2554 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8303 "ds_parser.cpp"
    break;

  case 485:
#line 2558 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8309 "ds_parser.cpp"
    break;

  case 486:
#line 2558 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8315 "ds_parser.cpp"
    break;

  case 487:
#line 2558 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8325 "ds_parser.cpp"
    break;

  case 488:
#line 2563 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8331 "ds_parser.cpp"
    break;

  case 489:
#line 2563 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8337 "ds_parser.cpp"
    break;

  case 490:
#line 2563 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8351 "ds_parser.cpp"
    break;

  case 491:
#line 2572 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8360 "ds_parser.cpp"
    break;

  case 492:
#line 2576 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8366 "ds_parser.cpp"
    break;

  case 493:
#line 2576 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8372 "ds_parser.cpp"
    break;

  case 494:
#line 2576 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8382 "ds_parser.cpp"
    break;

  case 495:
#line 2581 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8388 "ds_parser.cpp"
    break;

  case 496:
#line 2581 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8394 "ds_parser.cpp"
    break;

  case 497:
#line 2581 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8408 "ds_parser.cpp"
    break;

  case 498:
#line 2590 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8417 "ds_parser.cpp"
    break;

  case 499:
#line 2594 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8423 "ds_parser.cpp"
    break;

  case 500:
#line 2594 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8429 "ds_parser.cpp"
    break;

  case 501:
#line 2594 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8439 "ds_parser.cpp"
    break;

  case 502:
#line 2599 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8445 "ds_parser.cpp"
    break;

  case 503:
#line 2599 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8451 "ds_parser.cpp"
    break;

  case 504:
#line 2599 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8465 "ds_parser.cpp"
    break;

  case 505:
#line 2608 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8471 "ds_parser.cpp"
    break;

  case 506:
#line 2608 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8477 "ds_parser.cpp"
    break;

  case 507:
#line 2608 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8488 "ds_parser.cpp"
    break;

  case 508:
#line 2614 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8494 "ds_parser.cpp"
    break;

  case 509:
#line 2614 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8500 "ds_parser.cpp"
    break;

  case 510:
#line 2614 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8511 "ds_parser.cpp"
    break;

  case 511:
#line 2623 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8517 "ds_parser.cpp"
    break;

  case 512:
#line 2623 "ds_parser.ypp"
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
#line 8534 "ds_parser.cpp"
    break;

  case 513:
#line 2638 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8540 "ds_parser.cpp"
    break;

  case 514:
#line 2638 "ds_parser.ypp"
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
#line 8561 "ds_parser.cpp"
    break;

  case 515:
#line 2658 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8567 "ds_parser.cpp"
    break;

  case 516:
#line 2659 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8573 "ds_parser.cpp"
    break;

  case 517:
#line 2660 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8579 "ds_parser.cpp"
    break;

  case 518:
#line 2661 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8585 "ds_parser.cpp"
    break;

  case 519:
#line 2665 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8597 "ds_parser.cpp"
    break;

  case 520:
#line 2672 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8609 "ds_parser.cpp"
    break;

  case 521:
#line 2679 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8620 "ds_parser.cpp"
    break;

  case 522:
#line 2685 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8631 "ds_parser.cpp"
    break;

  case 523:
#line 2694 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8641 "ds_parser.cpp"
    break;

  case 524:
#line 2699 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8650 "ds_parser.cpp"
    break;

  case 525:
#line 2706 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8656 "ds_parser.cpp"
    break;

  case 526:
#line 2707 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8662 "ds_parser.cpp"
    break;

  case 527:
#line 2711 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8673 "ds_parser.cpp"
    break;

  case 528:
#line 2717 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8685 "ds_parser.cpp"
    break;

  case 529:
#line 2724 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8698 "ds_parser.cpp"
    break;

  case 530:
#line 2732 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8710 "ds_parser.cpp"
    break;

  case 531:
#line 2739 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8723 "ds_parser.cpp"
    break;

  case 532:
#line 2747 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8737 "ds_parser.cpp"
    break;

  case 533:
#line 2759 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8745 "ds_parser.cpp"
    break;

  case 534:
#line 2762 "ds_parser.ypp"
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
#line 8761 "ds_parser.cpp"
    break;

  case 535:
#line 2776 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8772 "ds_parser.cpp"
    break;

  case 536:
#line 2785 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8778 "ds_parser.cpp"
    break;

  case 537:
#line 2786 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8784 "ds_parser.cpp"
    break;

  case 538:
#line 2790 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8794 "ds_parser.cpp"
    break;

  case 539:
#line 2795 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8803 "ds_parser.cpp"
    break;

  case 540:
#line 2802 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8813 "ds_parser.cpp"
    break;

  case 541:
#line 2807 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8825 "ds_parser.cpp"
    break;

  case 542:
#line 2817 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8835 "ds_parser.cpp"
    break;

  case 543:
#line 2822 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8844 "ds_parser.cpp"
    break;

  case 544:
#line 2829 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8858 "ds_parser.cpp"
    break;

  case 545:
#line 2841 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8864 "ds_parser.cpp"
    break;

  case 546:
#line 2842 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8870 "ds_parser.cpp"
    break;

  case 547:
#line 2846 "ds_parser.ypp"
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
#line 8893 "ds_parser.cpp"
    break;

  case 548:
#line 2864 "ds_parser.ypp"
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
#line 8916 "ds_parser.cpp"
    break;


#line 8920 "ds_parser.cpp"

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
#line 2884 "ds_parser.ypp"


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
