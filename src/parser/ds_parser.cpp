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
#define YYPURE 2

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


/* First part of user prologue.  */
#line 75 "ds_parser.ypp"

    #include "daScript/misc/platform.h"
    #include "daScript/simulate/debug_info.h"
    #include "daScript/ast/compilation_errors.h"

    #include "parser_state.h"

    #ifdef _MSC_VER
    #pragma warning(disable:4262)
    #pragma warning(disable:4127)
    #pragma warning(disable:4702)
    #endif

    using namespace das;

    union DAS_YYSTYPE;
    struct DAS_YYLTYPE;

    #define YY_NO_UNISTD_H
    #include "lex.yy.h"

    void das_yyerror (const string & error, const das::LineInfo & at, das::CompilationError cerr = das::CompilationError::unspecified);
    void das_checkName(const string & name, const LineInfo &at);
    void das_yyerror (DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error);
    void das_yyfatalerror(DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr);
    int yylex(DAS_YYSTYPE *lvalp, DAS_YYLTYPE *llocp, yyscan_t scanner);
    void yybegin(const char * str);

    void das_yybegin_reader ( yyscan_t yyscanner );
    void das_yyend_reader ( yyscan_t yyscanner );

    __forceinline string inThisModule ( const string & name ) { return "_::" + name; }

    #undef yyextra
    #define yyextra (*((das::DasParserState **)(scanner)))

#line 115 "ds_parser.cpp"

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
    }
    using namespace das;

    struct VariableNameAndPosition {
        string      name;
        string      aka;
        LineInfo    at;
    };

    struct VariableDeclaration {
        VariableDeclaration ( vector<VariableNameAndPosition> * n, TypeDecl * t, Expression * i )
            : pNameList(n), pTypeDecl(t), pInit(i) {}
        virtual ~VariableDeclaration () {
            if ( pNameList ) delete pNameList;
            delete pTypeDecl;
            if ( pInit ) delete pInit;
            if ( annotation ) delete annotation;
        }
        vector<VariableNameAndPosition>   *pNameList;
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

    LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li );
    LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 219 "ds_parser.cpp"

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
    DAS_AKA = 296,
    DAS_ASSUME = 297,
    DAS_CAST = 298,
    DAS_OVERRIDE = 299,
    DAS_ABSTRACT = 300,
    DAS_UPCAST = 301,
    DAS_ITERATOR = 302,
    DAS_VAR = 303,
    DAS_ADDR = 304,
    DAS_CONTINUE = 305,
    DAS_WHERE = 306,
    DAS_PASS = 307,
    DAS_REINTERPRET = 308,
    DAS_MODULE = 309,
    DAS_PUBLIC = 310,
    DAS_LABEL = 311,
    DAS_GOTO = 312,
    DAS_IMPLICIT = 313,
    DAS_EXPLICIT = 314,
    DAS_SHARED = 315,
    DAS_PRIVATE = 316,
    DAS_SMART_PTR = 317,
    DAS_UNSAFE = 318,
    DAS_TBOOL = 319,
    DAS_TVOID = 320,
    DAS_TSTRING = 321,
    DAS_TAUTO = 322,
    DAS_TINT = 323,
    DAS_TINT2 = 324,
    DAS_TINT3 = 325,
    DAS_TINT4 = 326,
    DAS_TUINT = 327,
    DAS_TBITFIELD = 328,
    DAS_TUINT2 = 329,
    DAS_TUINT3 = 330,
    DAS_TUINT4 = 331,
    DAS_TFLOAT = 332,
    DAS_TFLOAT2 = 333,
    DAS_TFLOAT3 = 334,
    DAS_TFLOAT4 = 335,
    DAS_TRANGE = 336,
    DAS_TURANGE = 337,
    DAS_TBLOCK = 338,
    DAS_TINT64 = 339,
    DAS_TUINT64 = 340,
    DAS_TDOUBLE = 341,
    DAS_TFUNCTION = 342,
    DAS_TLAMBDA = 343,
    DAS_TINT8 = 344,
    DAS_TUINT8 = 345,
    DAS_TINT16 = 346,
    DAS_TUINT16 = 347,
    DAS_TTUPLE = 348,
    DAS_TVARIANT = 349,
    DAS_GENERATOR = 350,
    DAS_YIELD = 351,
    DAS_SEALED = 352,
    ADDEQU = 353,
    SUBEQU = 354,
    DIVEQU = 355,
    MULEQU = 356,
    MODEQU = 357,
    ANDEQU = 358,
    OREQU = 359,
    XOREQU = 360,
    SHL = 361,
    SHR = 362,
    ADDADD = 363,
    SUBSUB = 364,
    LEEQU = 365,
    SHLEQU = 366,
    SHREQU = 367,
    GREQU = 368,
    EQUEQU = 369,
    NOTEQU = 370,
    RARROW = 371,
    LARROW = 372,
    QQ = 373,
    QDOT = 374,
    QBRA = 375,
    LPIPE = 376,
    LBPIPE = 377,
    LAPIPE = 378,
    LFPIPE = 379,
    RPIPE = 380,
    CLONEEQU = 381,
    ROTL = 382,
    ROTR = 383,
    ROTLEQU = 384,
    ROTREQU = 385,
    MAPTO = 386,
    COLCOL = 387,
    ANDAND = 388,
    OROR = 389,
    XORXOR = 390,
    ANDANDEQU = 391,
    OROREQU = 392,
    XORXOREQU = 393,
    BRABRAB = 394,
    BRACBRB = 395,
    CBRCBRB = 396,
    INTEGER = 397,
    LONG_INTEGER = 398,
    UNSIGNED_INTEGER = 399,
    UNSIGNED_LONG_INTEGER = 400,
    FLOAT = 401,
    DOUBLE = 402,
    NAME = 403,
    BEGIN_STRING = 404,
    STRING_CHARACTER = 405,
    STRING_CHARACTER_ESC = 406,
    END_STRING = 407,
    BEGIN_STRING_EXPR = 408,
    END_STRING_EXPR = 409,
    END_OF_READ = 410,
    UNARY_MINUS = 411,
    UNARY_PLUS = 412,
    PRE_INC = 413,
    PRE_DEC = 414,
    POST_INC = 415,
    POST_DEC = 416,
    DEREF = 417
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 123 "ds_parser.ypp"

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
    vector<VariableNameAndPosition> * pNameWithPosList;
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

#line 422 "ds_parser.cpp"

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



int das_yyparse (yyscan_t scanner);

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
#define YYLAST   7623

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  190
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  184
/* YYNRULES -- Number of rules.  */
#define YYNRULES  576
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1007

#define YYUNDEFTOK  2
#define YYMAXUTOK   417

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
       2,     2,     2,   173,     2,   189,   184,   169,   162,     2,
     182,   183,   167,   166,   156,   165,   179,   168,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   159,   186,
     163,   157,   164,   158,   185,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   180,     2,   181,   161,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   187,   160,   188,   172,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   170,   171,   174,   175,   176,   177,   178
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   489,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   505,   506,   507,   511,   512,
     516,   533,   534,   535,   536,   540,   544,   549,   558,   566,
     582,   587,   595,   595,   625,   646,   650,   653,   657,   663,
     672,   695,   720,   721,   725,   729,   730,   734,   737,   743,
     749,   752,   758,   759,   763,   764,   765,   774,   775,   779,
     788,   805,   813,   823,   832,   832,   839,   840,   841,   842,
     843,   844,   848,   853,   861,   862,   863,   867,   868,   869,
     870,   871,   872,   873,   874,   880,   894,   911,   912,   913,
     917,   925,   938,   942,   949,   950,   954,   955,   956,   960,
     963,   970,   974,   975,   976,   977,   978,   979,   980,   981,
     982,   983,   984,   985,   986,   987,   988,   989,   990,   991,
     992,   993,   994,   995,   996,   997,   998,   999,  1000,  1001,
    1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,
    1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,
    1025,  1047,  1048,  1049,  1053,  1092,  1100,  1104,  1115,  1116,
    1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1138,  1143,
    1149,  1155,  1189,  1192,  1198,  1199,  1210,  1214,  1220,  1223,
    1226,  1230,  1236,  1240,  1244,  1247,  1250,  1255,  1258,  1266,
    1269,  1274,  1277,  1285,  1291,  1292,  1296,  1331,  1331,  1331,
    1334,  1334,  1334,  1339,  1339,  1339,  1347,  1347,  1347,  1353,
    1363,  1374,  1389,  1392,  1398,  1399,  1406,  1417,  1418,  1419,
    1423,  1424,  1425,  1426,  1430,  1435,  1443,  1444,  1448,  1453,
    1518,  1519,  1520,  1521,  1522,  1523,  1527,  1528,  1529,  1530,
    1531,  1532,  1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,
    1541,  1542,  1543,  1544,  1545,  1549,  1550,  1551,  1552,  1556,
    1567,  1572,  1582,  1586,  1586,  1586,  1593,  1593,  1593,  1607,
    1611,  1615,  1615,  1615,  1622,  1623,  1624,  1625,  1626,  1627,
    1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,  1637,
    1638,  1639,  1640,  1641,  1642,  1643,  1644,  1645,  1646,  1647,
    1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1672,  1676,  1679,  1682,  1683,  1684,  1685,  1694,  1703,
    1704,  1707,  1707,  1707,  1710,  1714,  1718,  1722,  1723,  1724,
    1725,  1726,  1727,  1728,  1729,  1745,  1761,  1816,  1824,  1825,
    1829,  1830,  1831,  1835,  1836,  1840,  1841,  1842,  1846,  1856,
    1859,  1864,  1863,  1909,  1975,  1984,  1989,  1997,  1998,  2002,
    2005,  2014,  2015,  2019,  2028,  2029,  2034,  2035,  2039,  2045,
    2051,  2054,  2058,  2064,  2073,  2074,  2075,  2079,  2080,  2084,
    2087,  2092,  2097,  2105,  2116,  2119,  2127,  2128,  2132,  2133,
    2134,  2138,  2171,  2171,  2210,  2213,  2222,  2235,  2247,  2248,
    2252,  2253,  2257,  2258,  2259,  2263,  2291,  2323,  2324,  2328,
    2329,  2333,  2377,  2378,  2382,  2383,  2384,  2389,  2388,  2485,
    2492,  2501,  2507,  2518,  2519,  2520,  2521,  2522,  2523,  2524,
    2525,  2526,  2527,  2528,  2529,  2530,  2531,  2532,  2533,  2534,
    2535,  2536,  2537,  2538,  2539,  2540,  2541,  2542,  2546,  2547,
    2548,  2549,  2550,  2551,  2555,  2566,  2570,  2580,  2587,  2596,
    2596,  2596,  2609,  2610,  2611,  2612,  2613,  2627,  2633,  2637,
    2641,  2646,  2651,  2656,  2661,  2665,  2669,  2674,  2678,  2683,
    2683,  2683,  2689,  2696,  2696,  2696,  2701,  2701,  2701,  2707,
    2707,  2707,  2712,  2716,  2716,  2716,  2721,  2721,  2721,  2730,
    2734,  2734,  2734,  2739,  2739,  2739,  2748,  2752,  2752,  2752,
    2757,  2757,  2757,  2766,  2766,  2766,  2772,  2772,  2772,  2781,
    2781,  2796,  2796,  2816,  2817,  2818,  2819,  2823,  2830,  2837,
    2843,  2852,  2857,  2864,  2865,  2869,  2875,  2882,  2890,  2897,
    2905,  2917,  2920,  2934,  2943,  2944,  2948,  2953,  2960,  2965,
    2975,  2980,  2987,  2999,  3000,  3004,  3027
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
  "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"", "\"abstract\"",
  "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"",
  "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"",
  "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"",
  "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"",
  "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"",
  "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
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
  "expression_while_loop", "expression_with", "expression_with_alias",
  "$@2", "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "expr_type_decl", "$@9", "$@10", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@11", "$@12",
  "$@13", "$@14", "expr_field", "$@15", "$@16", "expr", "$@17", "$@18",
  "optional_field_annotation", "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@19", "function_argument_declaration", "function_argument_list",
  "tuple_type", "tuple_type_list", "variant_type", "variant_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@20", "enum_list",
  "single_alias", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@21", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@22", "$@23", "type_declaration", "$@24",
  "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "variant_alias_declaration",
  "$@48", "bitfield_alias_declaration", "$@49", "make_decl",
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
     405,   406,   407,   408,   409,   410,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
     411,   412,   126,    33,   413,   414,   415,   416,   417,    46,
      91,    93,    40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -724

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-724)))

#define YYTABLE_NINF -531

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -724,    34,  -724,  -724,    27,   -56,   -88,   -53,  -724,   -86,
    -724,  -724,    22,  -724,  -724,  -724,  -724,  -724,   201,  -724,
      37,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     -14,  -724,    90,   150,   171,  -724,  -724,   -88,    11,  -724,
      36,   204,  -724,  -724,  -724,    37,   219,   221,  -724,  -724,
     233,   259,   226,  -724,   -35,  -724,  -724,  -724,   322,   323,
     325,  -724,   335,    13,    27,   253,   -56,   224,   274,  -724,
     285,   295,  -724,  7388,   251,   -84,   360,   257,   258,  -724,
     298,    27,    22,  -724,  -724,  -724,     1,  -724,  -724,  -724,
     299,  -724,  -724,   353,  -724,  -724,   279,  -724,  -724,  -724,
    -724,  -724,   156,    76,  -724,  -724,  -724,  -724,   406,  -724,
    -724,   301,   304,   305,   313,  -724,  -724,  -724,   303,  -724,
    -724,  -724,  -724,  -724,   314,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,   320,  -724,  -724,  -724,   330,   331,
    -724,  -724,  -724,  -724,   332,   333,  -724,  -724,  -724,  -724,
    -724,  2517,  -724,  -724,   300,  -724,  -724,  -724,   349,   350,
    -724,    21,  -724,  7000,   465,   466,  -724,   319,   315,    78,
    -724,   357,  -724,  -724,   364,  -724,  -724,   289,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   108,  -724,  -724,  -724,  -724,
    -724,   359,  -724,   183,   184,   190,  -724,  -724,  -724,  -724,
    -724,  -724,   476,  -724,  -724,     9,  2929,  -724,  -724,  -724,
     329,   365,  -724,   339,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   328,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     337,  -724,  -724,   -81,   367,  -724,  -724,   269,  -724,   370,
     324,   -83,    27,   368,  -724,  -724,  -724,    76,  -724,  7388,
    7388,  7388,  7388,   336,   349,  7388,   319,  7388,   319,  7388,
     319,  7475,   350,  -724,  -724,  -724,   351,  -724,  -724,  -724,
    7301,   361,   374,  -724,   369,   376,   379,   371,   383,   373,
    -724,   386,  4597,  4597,  7127,  7214,  4597,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  4597,  4597,  4597,   162,  4597,  4597,
    -724,  4597,  -724,   372,  -724,  -724,  -724,   -32,  -724,  -724,
    -724,  -724,   378,  -724,  -724,  -724,  -724,  -724,  -724,  5689,
     377,  -724,  -724,  -724,  -724,  -724,   -74,  7388,   -47,  -724,
    -724,  -724,     0,  -724,   252,  7388,  -724,  1245,  -724,  -724,
    -724,  -724,  -724,  -724,   380,    -1,   162,  -724,  -724,  -724,
     368,   137,   511,  -724,   220,  -724,   404,   416,  1698,  2012,
    -724,    75,  2170,   367,  2299,   367,  2357,   367,   142,  -724,
     112,  2517,   176,  -724,  2454,  -724,   162,  -724,  4597,  -724,
    -724,  4597,  -724,  4597,  7388,   213,   213,   368,  1596,   368,
    1756,  6255,  -724,   -28,   277,   213,   213,   -48,  -724,   213,
     213,   800,    99,  -724,  2358,   417,    33,   412,  4597,  4597,
    -724,  -724,  4597,  4597,  4597,  4597,   422,  4597,   423,  4597,
    4597,  4597,  4597,  4597,  4597,  4597,  4597,  3066,  4597,  4597,
    4597,  4597,  4597,  4597,  4597,  4597,  4597,  4597,   -40,  4597,
    -724,  3203,  -724,  -724,  2517,  -724,  -724,   368,  -724,   -63,
    -724,   364,  2517,  -724,  4597,  -724,  -724,   368,  2076,  -724,
     315,  4597,  4597,   427,  -724,   390,   435,  3363,    61,  2221,
     247,   247,  -724,   529,   393,   394,  4597,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   396,   397,   399,  -724,  -724,
    -724,   368,  -724,   -80,  -724,  6187,  -724,    97,  -724,  -724,
      14,  -724,   405,   439,   440,  7388,  -724,   -39,  -724,  7388,
    -724,  -724,  -724,   441,  -724,   426,  -724,   428,  -724,   430,
    7388,  -724,  7475,  -724,   350,  3500,  3660,  7388,  5143,  7388,
    7388,  5221,  7388,  5299,  2596,     7,   247,   106,  1916,  3820,
    6255,   442,   -21,   410,   443,  -724,  -724,   111,    20,  3980,
     -21,    15,  4597,  4597,   407,  -724,  4597,   206,   447,  -724,
     191,  -724,   452,  -724,   122,  6473,   -42,   319,   438,  -724,
    -724,  1532,  1532,    17,    17,  6915,  6915,   421,    48,  -724,
    5765,   -67,   -67,  1532,  1532,  6677,  6541,  6609,   456,  6337,
     717,  6745,  6881,    17,    17,  1852,  1852,    48,    48,    48,
     458,  4597,  -724,   459,  4597,   607,  5841,  -724,   125,  -724,
    -724,  -724,  7388,  -724,  2518,  -724,   291,    24,  2518,  -724,
    -724,   487,   543,   597,  6473,   291,   454,  -724,   453,   472,
    6473,  -724,  2518,  -724,  1057,   431,  -724,  -724,   432,  -724,
    -724,   291,  -724,  -724,  -724,  -724,   247,  -724,  4597,  4597,
    4597,  4597,  4597,  4597,  4597,  4597,  4597,  4597,  2655,  4597,
    4597,  4597,  4597,  4597,  4597,  2792,     3,  4597,  -724,    22,
    -724,   608,     6,   436,  -724,  -724,   576,   355,  -724,  -724,
    -724,  2518,  -724,  2733,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  2517,  -724,  -724,  -724,  -724,   133,   470,
    5377,  2870,  -724,  3007,  3144,  -724,  3202,  -724,   417,  4597,
    -724,  4597,  4597,   -18,   475,   452,   444,   445,  4597,   446,
    4597,  4597,   452,   450,   484,  6473,  -724,  -724,  6405,  -724,
     478,  7388,   319,   109,   143,  4597,  -724,   483,   492,   518,
     519,  -724,   144,   367,  -724,  4140,  -724,  -724,  4597,  -724,
    5917,  -724,  5993,  -724,  -724,  -724,  2654,  -724,  6084,  -724,
    4597,  -724,  4687,  4597,  4597,  -724,   315,  -724,  -724,  -724,
    -724,  -724,  4789,  -724,  -724,  -724,   352,  -724,  6473,  6473,
    6473,  6473,  6473,  6473,  6473,  6473,  6473,  6473,  4597,  4597,
    6473,  6473,  6473,  6473,  6473,  6473,  6473,  4597,  4597,  6473,
    -724,   902,   170,   362,  -724,  -724,   362,  -724,   526,   476,
    -724,  2518,  -724,  4891,  -724,  -724,  -724,  -724,  -724,   182,
    -724,  -724,  -724,  -724,  -724,   500,   -34,  6473,  6473,   -21,
     502,   123,   442,   503,  -724,  6473,  -724,  -724,    73,   -21,
     504,  -724,  -724,  -724,  3299,   367,   505,  6473,  -724,  -724,
    -724,  -724,   -42,   508,   -95,  7388,  -724,   134,  6473,  -724,
    -724,  -724,   476,  4597,    86,  -724,  -724,  4597,  -724,  1433,
     315,  -724,  -724,  4597,  -724,  6813,  6473,  6813,  6473,  -724,
     449,  -724,  -724,    32,   368,  -724,  -724,  4993,  -724,  -724,
    4597,   538,  -724,  4597,  4597,  4597,  4300,  4597,   509,   512,
    4597,  4597,  -724,  4597,   506,  -724,  -724,   528,  -724,  -724,
    -724,  4460,  -724,  -724,  3441,  -724,  6473,   315,  6473,  -724,
    -724,   291,  -724,  -724,   663,  -724,  -724,  5455,   532,    17,
      17,    17,  -724,  5533,  5068,   516,  -724,  6473,  6473,  5068,
     517,   162,  -724,  4597,  6473,  -724,  -724,   352,     1,  -724,
       1,  -724,  4597,  -724,   648,   521,  -724,   525,  -724,  -724,
     162,  6473,  -724,  -724,   534,   315,  5611,  4597,   533,   537,
    -724,  -724,  -724,  -724,  6473,  -724,  -724
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    94,     1,   204,     0,     0,     0,     0,   205,     0,
     541,   539,     0,    14,     3,    10,     9,     8,     0,     7,
     406,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   421,    19,    18,   406,     0,     0,    88,    89,
       0,   184,    90,    92,     0,    87,   433,   432,   151,   422,
     434,   407,   408,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   186,
       0,     0,     0,    95,   153,   152,     0,   150,   424,   423,
       0,   436,   435,   429,   410,   409,   412,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   443,   463,   444,   475,   445,
     449,   450,   451,   452,   467,   456,   457,   458,   459,   460,
     461,   462,   464,   465,   512,   448,   455,   466,   519,   526,
     446,   453,   447,   454,     0,     0,   474,   482,   485,   483,
     484,   417,   418,   420,     0,    16,    17,    20,     0,     0,
     185,     0,    93,     0,     0,     0,   101,    96,     0,     0,
     430,     0,   437,   404,   358,    21,    22,     0,    70,    71,
      68,    69,    67,    66,    72,     0,    41,   503,   506,   509,
     499,     0,   479,   513,   520,   527,   533,   536,   490,   495,
     489,   502,     0,   498,   492,     0,     0,   494,   419,   477,
       0,     0,   384,     0,    91,   149,   104,   105,   107,   106,
     108,   109,   110,   111,   136,   137,   134,   135,   127,   138,
     139,   128,   125,   126,   147,     0,   148,   140,   141,   142,
     143,   115,   116,   117,   112,   113,   114,   130,   131,   129,
     123,   124,   119,   118,   120,   121,   122,   103,   102,   146,
       0,   132,   133,   358,    99,   178,   155,     0,   414,   427,
       0,   358,     0,     0,    23,    24,    25,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    96,     0,    96,     0,
      96,     0,     0,   497,   491,   493,     0,   496,   290,   291,
       0,     0,     0,   284,     0,     0,     0,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,   240,   242,   241,
     243,   244,   245,    26,     0,     0,     0,     0,     0,     0,
     487,     0,   227,   228,   288,   287,   238,   285,   350,   349,
     348,   347,    94,   353,   286,   352,   351,   329,   292,     0,
       0,   289,   543,   544,   545,   546,     0,     0,     0,   145,
     144,    97,     0,   377,     0,     0,   154,     0,   468,   471,
     469,   472,   470,   473,     0,     0,     0,   431,   369,   411,
       0,     0,   439,   413,   397,    73,     0,     0,     0,     0,
     476,     0,     0,    99,     0,    99,     0,    99,   184,   381,
       0,   379,     0,   488,   188,   191,     0,   216,     0,   207,
     210,     0,   213,     0,     0,   318,   319,     0,   553,     0,
       0,     0,   570,     0,     0,   296,   295,   334,    32,   294,
     293,     0,   229,   356,     0,   236,     0,     0,     0,     0,
     320,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     486,     0,   478,   542,   383,   540,   385,     0,   375,   388,
      98,   358,   100,   180,     0,    57,    58,     0,     0,   192,
       0,     0,     0,     0,   193,     0,     0,     0,     0,     0,
       0,     0,   158,   156,     0,     0,     0,   173,   168,   165,
     164,   166,   167,   179,   159,     0,     0,     0,   171,   172,
     174,     0,   163,     0,   160,   246,   414,     0,   425,   428,
     358,   405,     0,     0,     0,     0,   398,     0,   504,     0,
     510,   500,   480,     0,   514,     0,   521,     0,   528,     0,
       0,   534,     0,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
     561,   551,   553,     0,   564,   565,   566,     0,     0,     0,
     553,     0,     0,     0,     0,    29,     0,    27,     0,   322,
     273,   272,     0,   330,     0,   222,     0,    96,     0,   344,
     345,   297,   298,   310,   311,   308,   309,     0,   339,   327,
       0,   354,   355,   299,   300,   315,   316,   317,     0,     0,
     313,   314,   312,   306,   307,   302,   301,   303,   304,   305,
       0,     0,   279,     0,     0,     0,     0,   332,     0,   376,
     387,   386,     0,   389,     0,   378,     0,     0,     0,   194,
     197,     0,   246,     0,   187,     0,     0,   177,     0,     0,
      51,    61,     0,   201,   246,   228,   182,   183,     0,   175,
     176,     0,   162,   169,   170,   206,     0,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     438,     0,   360,     0,   359,   440,   441,     0,   395,   396,
     394,     0,   505,     0,   511,   501,   481,   515,   517,   522,
     524,   529,   531,   380,   535,   382,   538,   189,     0,     0,
       0,     0,   335,     0,     0,   336,     0,   357,   236,     0,
     554,     0,     0,   553,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   563,   571,   572,     0,    30,
      33,     0,    96,     0,     0,     0,   331,     0,     0,     0,
       0,   234,     0,    99,   341,     0,   325,   346,     0,   328,
       0,   280,     0,   282,   323,   333,   390,   393,   392,    62,
       0,   198,   246,     0,     0,   195,     0,    63,    64,    49,
      50,   202,   246,   199,   229,   178,    54,   181,   256,   257,
     259,   258,   260,   250,   251,   252,   261,   262,     0,     0,
     248,   249,   263,   264,   253,   254,   255,     0,     0,   247,
     426,     0,     0,   365,   361,   362,   365,   370,     0,   394,
     399,     0,   403,   246,   507,   518,   525,   532,   190,     0,
     219,   217,   208,   211,   214,     0,     0,   548,   547,   553,
       0,     0,   552,     0,   556,   562,   568,   567,     0,   553,
       0,   569,    28,    31,     0,    99,     0,   223,   232,   233,
     231,   230,     0,     0,     0,     0,   270,     0,   340,   326,
     324,   283,   386,     0,     0,   196,   203,     0,   200,     0,
       0,    52,    53,     0,    59,   267,   268,   265,   266,   416,
      95,   366,   367,   360,     0,   442,   401,   246,   402,   508,
       0,     0,   218,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,     0,     0,   559,   274,     0,   269,   235,
     237,     0,   224,   239,     0,   271,   391,     0,    65,   157,
      55,     0,   374,   371,   363,   368,   400,     0,     0,   209,
     212,   215,   337,     0,   573,     0,   557,   550,   549,   573,
       0,     0,   277,     0,   225,   342,    60,    54,     0,   364,
       0,   220,     0,   338,     0,     0,   558,     0,   560,   275,
       0,   226,   343,    56,     0,     0,     0,     0,     0,     0,
     278,   372,   373,   221,   574,   575,   576
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -724,  -724,  -724,  -724,  -724,   306,   652,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,   687,  -724,   617,  -724,  -724,   661,
    -724,  -724,  -724,  -246,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,   457,  -724,  -724,   671,   -59,  -724,   654,    40,
    -323,  -278,  -384,  -724,  -724,  -724,  -723,  -724,  -167,  -724,
     -65,  -470,   -12,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     740,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -455,  -724,  -724,  -140,  -724,     5,  -477,
    -724,  -363,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,   308,  -724,  -724,  -160,  -169,  -724,   -91,
    -724,  -724,  -724,   267,  -724,   202,  -724,  -338,   464,  -473,
    -472,    69,  -724,  -349,  -724,   733,  -724,  -724,  -724,   255,
      44,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -360,   -70,  -724,  -724,  -724,   495,  -724,  -724,
    -724,   -29,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     482,  -545,  -413,  -555,  -724,  -724,  -301,    35,   363,  -724,
    -724,  -724,  -183,  -724
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   157,    45,    14,   177,   183,   424,   334,   760,
     335,   588,    15,    16,    38,    39,    72,    17,    34,    35,
     504,   505,   903,   904,   506,   507,   508,   509,   510,   511,
     512,   897,   184,   185,    30,    31,    32,    52,    53,    54,
      18,   264,   366,   167,    19,    86,   168,    87,   336,   513,
     367,   514,   337,   515,   338,   516,   517,   518,   519,   520,
     521,   522,   339,   559,   923,   560,   924,   562,   925,   340,
     557,   922,   341,   594,   943,   342,   771,   772,   597,   343,
     344,   651,   524,   345,   346,   347,   761,   971,   762,   990,
     348,   635,   891,   595,   885,   992,   362,   836,   980,   913,
     703,   530,   978,   363,   364,   399,   400,   212,   213,   742,
     478,   711,   537,   383,   271,    62,    96,    21,   174,   375,
      42,    75,    22,    90,    23,   377,   171,   172,    60,    93,
      24,   270,   384,   350,   374,   148,   149,   210,   150,   284,
     716,   401,   282,   715,   279,   712,   280,   919,   281,   714,
     285,   717,   286,   845,   287,   719,   288,   846,   289,   721,
     290,   847,   291,   724,   292,   726,    25,    47,    26,    46,
     351,   571,   572,   573,   352,   574,   575,   576,   577,   353,
     423,   354,   985,   355
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   266,   479,   147,   523,   639,   644,   580,   393,   545,
     395,   547,   397,   549,   273,   422,   638,   746,   650,   435,
     476,   -94,   161,   666,   667,   753,   739,    97,    98,   663,
     566,   531,    68,   566,     2,   163,   941,   436,   437,   751,
       3,   294,   676,   790,   151,    27,    28,   764,   477,   446,
     834,   598,   448,   449,   640,    48,   272,   565,   272,   578,
      36,   146,    43,     4,    40,     5,    69,     6,   436,   437,
      55,   448,   449,     7,   472,   767,   834,   953,   708,   630,
     631,    37,     8,    49,   768,    74,    33,   709,     9,   740,
     178,   179,   265,   534,   641,    40,   642,    61,    44,   643,
     728,   211,   361,   835,   153,   379,   677,    10,   632,   164,
     165,   380,   468,   469,   473,   769,   433,   479,   710,   154,
     770,    82,   765,   438,   439,   440,   441,   647,    11,   835,
     763,   468,   469,   446,    41,   447,   448,   449,   450,   633,
     634,   475,   451,    63,   452,   453,    83,   527,   382,   166,
     434,   527,   927,   272,    50,    99,   440,   441,   583,   100,
     584,   101,   102,   534,   446,   745,   447,   448,   449,   450,
      51,   295,   675,   451,   787,    29,   534,    64,   791,    70,
     534,   599,   463,   464,   465,   466,   467,   528,   860,   296,
      71,   830,   801,    73,   699,   103,   468,   469,   297,   807,
     862,   750,   700,   754,   214,    56,    57,   701,    58,   147,
     147,   147,   147,   381,    12,   147,   476,   147,   180,   147,
      13,   147,   181,   640,   182,   102,   640,   468,   469,   765,
     147,    50,   741,   436,   437,   741,    59,   267,    80,   542,
     640,   842,   765,   413,   147,   147,    64,    51,   265,   930,
     386,   387,   388,   389,   697,   994,   392,   995,   394,   933,
     396,   543,   590,   641,   277,   268,   641,   146,   146,   146,
     146,   404,   947,   146,    80,   146,   551,   146,   765,   146,
     641,   765,   756,   698,   856,   418,   420,   147,   146,   765,
     765,   278,   749,    64,    50,   147,   868,   750,   552,   744,
     882,   550,   146,   146,   928,   766,   175,   176,   785,    65,
      51,   436,   437,   893,   934,   428,   848,   945,   532,   773,
     887,   440,   441,   653,   876,   883,    82,    66,   474,   446,
     859,   661,   448,   449,   450,   894,   482,   368,   451,   869,
     553,   369,  -516,  -523,   147,   146,   920,  -516,  -523,  -530,
    -276,   910,    40,   146,  -530,  -276,   274,   275,   370,   371,
     372,   373,   554,   900,   529,  -516,  -523,    77,   921,    78,
     702,   916,  -530,  -276,   901,   902,   534,    84,    88,   535,
      91,    79,   536,    85,    89,   564,    92,   198,   931,   884,
      94,    80,   468,   469,   556,   106,    95,   438,   439,   440,
     441,   442,   146,    71,   443,   444,   445,   446,    81,   447,
     448,   449,   450,   199,   200,   155,   451,   911,   452,   453,
     591,   156,   108,   912,   454,   455,   456,   175,   176,   585,
     586,   332,   665,   109,   265,   480,   198,   152,   481,   274,
     275,   276,   955,   110,   158,   159,   160,   169,   198,   457,
     170,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,    69,   199,   200,   187,   147,   173,   188,   189,   147,
     468,   469,   708,   201,   199,   200,   190,   192,   265,   789,
     147,   709,   147,   193,   875,   191,   208,   147,   797,   147,
     147,   937,   147,   194,   195,   196,   197,   209,   211,   261,
     262,   263,   265,   272,   806,   269,   707,   283,   293,   359,
     713,   378,   839,   203,   349,   356,   382,   204,   360,   390,
     205,   723,   201,   146,   357,   358,   365,   146,   731,   376,
     733,   734,   403,   736,   201,   206,   523,   407,   146,   409,
     146,   840,   410,   406,   207,   146,   412,   146,   146,   414,
     146,   408,   533,   411,   479,   413,   596,   432,    12,   471,
     600,   202,   203,   436,   437,   668,   204,   526,   538,   205,
     607,   609,   147,   202,   203,   656,   657,   658,   204,   669,
     670,   205,   672,   673,   206,   674,   704,   705,   706,   472,
     718,   747,   720,   207,   722,   757,   206,   759,   744,   748,
     763,   774,   539,   775,   777,   207,   779,   781,   783,   676,
     796,   798,   799,   786,   800,   833,   804,   838,   849,   805,
     415,   416,   837,   861,   421,   863,   864,   866,   873,   896,
     146,   878,   425,   426,   427,   952,   429,   430,   870,   431,
     879,   678,   679,   680,   681,   682,   683,   684,   685,   438,
     439,   440,   441,   442,   686,   687,   443,   444,   445,   446,
     793,   447,   448,   449,   450,   871,   880,   881,   451,   689,
     452,   453,   690,   691,   915,   525,   454,   455,   456,   692,
     693,   694,   926,   929,   932,   935,   958,    55,   938,   940,
     965,   147,   972,   966,   970,   979,   982,   986,   988,   997,
     794,   457,   998,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   999,  1005,   104,   558,   942,  1006,   561,
    1001,   563,   468,   469,    67,   186,   570,   107,   570,   795,
     587,   993,   874,   950,   385,   105,   162,   436,   437,   832,
     899,    20,   939,   855,   954,   914,   601,   602,   645,   146,
     603,   604,   605,   606,   725,   608,   402,   610,   611,   612,
     613,   614,   615,   616,   617,   619,   620,   621,   622,   623,
     624,   625,   626,   627,   628,   629,   841,   636,    76,   391,
     976,   696,   405,   581,   977,   867,   987,     0,     0,     0,
       0,     0,   646,     0,     0,     0,   652,     0,     0,   654,
     655,     0,     0,     0,     0,   660,     0,   664,     0,     0,
       0,     0,     0,     0,   671,   147,     0,     0,     0,     0,
     436,   437,     0,   438,   439,   440,   441,   442,  1002,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   944,     0,     0,     0,
       0,     0,     0,     0,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   146,     0,     0,   425,   431,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   431,     0,     0,
     755,   421,     0,     0,   758,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,     0,   447,   448,
     449,   450,   436,   437,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,   780,
       0,     0,   782,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   788,     0,     0,     0,   792,     0,   457,   989,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     802,     0,     0,     0,     0,     0,     0,     0,  1000,   468,
     469,     0,     0,   589,     0,     0,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   817,   820,   821,   822,   823,
     824,   825,   826,   829,     0,   831,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   843,
     447,   448,   449,   450,     0,     0,     0,   451,     0,   452,
     453,     0,     0,     0,     0,   454,   455,   456,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   857,
     858,     0,     0,     0,     0,     0,   865,     0,   570,     0,
     457,     0,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,   877,     0,     0,     0,   436,   437,     0,
       0,   468,   469,     0,     0,     0,   888,     0,   909,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   820,   829,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   905,   906,     0,     0,
       0,     0,     0,     0,     0,   907,   908,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   917,
       0,     0,     0,     0,     0,   678,   679,   680,   681,   682,
     683,   684,   685,   438,   439,   440,   441,   442,   686,   687,
     443,   444,   445,   446,   793,   447,   448,   449,   450,     0,
       0,     0,   451,   689,   452,   453,   690,   691,     0,     0,
     454,   455,   456,   692,   693,   694,     0,     0,     0,     0,
       0,   946,     0,     0,     0,   948,     0,   525,     0,     0,
       0,   951,     0,     0,   794,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   957,     0,
       0,   959,   960,   961,   963,   964,   468,   469,   967,   968,
       0,   969,     0,   803,     0,     0,   483,     0,     0,   974,
       0,     3,     0,   484,   485,   486,     0,   487,     0,   298,
     299,   300,   301,   302,     0,     0,     0,     0,     0,     0,
     488,   303,   489,   490,     0,     0,     0,     0,     0,     0,
       0,   991,   491,   304,     0,   492,     0,   493,   305,     0,
     996,   306,     0,     8,   307,   494,     0,   495,   308,     0,
       0,   496,   497,     0,     0,  1004,     0,     0,   498,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     311,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
       0,     0,     0,     0,     0,     0,     0,   331,     0,   332,
     333,   502,   265,   503,   483,     0,     0,     0,     0,     3,
       0,   484,   485,   486,     0,   487,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,   488,   303,
     489,   490,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   304,     0,   492,     0,   493,   305,     0,     0,   306,
       0,     8,   307,   494,     0,   495,   308,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   311,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   437,     0,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,     0,
     298,   299,   300,   301,   302,   331,     0,   332,   333,   502,
     265,   949,   303,     0,     0,     0,     0,     0,   198,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
     440,   441,   306,     0,     0,   307,     0,   566,   446,   308,
     447,   448,   449,   450,   199,   200,     0,   451,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,     0,     0,   463,   464,   465,
     466,   467,     0,     0,   312,   313,     0,     0,     0,     0,
       0,   468,   469,     0,   201,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     198,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   567,   323,     0,     0,     0,     0,
       0,     0,     0,   202,   203,     0,   199,   200,   204,     0,
       0,   568,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   206,     0,   569,     0,
     332,   333,   303,   265,     0,   207,     0,     0,   198,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,   199,   200,   201,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,   202,   203,     0,     0,     0,
     204,     0,   540,   205,   312,   313,     0,     0,     0,     0,
       0,     0,   436,   437,   201,     0,     0,     0,   206,     0,
       0,     0,     0,     0,     0,     0,     0,   207,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   567,   323,     0,     0,     0,     0,
       0,     0,     0,   202,   203,     0,     0,     0,   204,     0,
       0,   568,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   206,     0,   579,     0,
     332,   333,   303,   265,     0,   207,     0,     0,   294,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
     440,   441,   306,     0,     0,   307,     0,     0,   446,   308,
     447,   448,   449,   450,     0,     0,     0,   451,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,     0,   312,   313,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
     199,   200,     0,     0,     0,     0,     0,     0,   295,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   296,     0,   331,     0,
     332,   333,   303,   265,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   541,   205,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,   206,   648,     0,     0,     0,     0,     0,   500,
     501,   207,   198,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,   199,   200,
       0,     0,     0,     0,     0,   298,   299,   300,   301,   302,
       0,   324,   325,   326,     0,   327,     0,   303,   328,   329,
       0,     0,     0,     0,     0,     0,     0,     0,   331,   304,
     332,   333,   649,   265,   305,     0,     0,   306,     0,     0,
     307,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   115,   116,   117,   201,   119,
     120,   121,   122,   123,   310,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   202,   203,   312,
     313,   198,   204,     0,   544,   205,     0,     0,   662,     0,
       0,     0,     0,     0,   500,   501,     0,     0,     0,     0,
     206,     0,     0,    50,     0,     0,     0,   199,   200,   207,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    51,
     323,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,     0,     0,     0,   303,     0,   324,   325,   326,   198,
     327,     0,     0,   328,   329,     0,   304,     0,     0,     0,
       0,   305,     0,   331,   306,   332,   333,   307,   265,     0,
       0,   308,     0,     0,     0,   199,   200,   201,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   311,     0,     0,   202,   203,     0,     0,
       0,   204,     0,   546,   205,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,   201,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,   198,     0,   207,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,   199,   200,   202,   203,     0,     0,     0,   204,
       0,   548,   205,   324,   325,   326,     0,   327,     0,     0,
     328,   329,   298,   299,   300,   301,   302,   206,   592,     0,
     331,   593,   332,   333,   303,   265,   207,     0,     0,   198,
       0,     0,     0,     0,     0,     0,   304,     0,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,   199,   200,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   311,     0,     0,   204,     0,     0,   205,
       0,     0,     0,     0,     0,     0,   312,   313,   198,     0,
       0,     0,     0,     0,   206,   201,   555,     0,     0,     0,
       0,   500,   501,   207,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,   199,   200,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,   298,
     299,   300,   301,   302,   202,   203,     0,     0,     0,   204,
       0,   303,   205,   324,   325,   326,   198,   327,     0,     0,
     328,   329,     0,   304,     0,     0,     0,   206,   305,     0,
     331,   306,   332,   333,   307,   265,   207,     0,   308,     0,
       0,     0,   199,   200,   201,     0,     0,     0,   309,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     311,     0,     0,   202,   203,     0,     0,     0,   204,     0,
     738,   205,     0,   312,   313,   198,     0,     0,     0,     0,
       0,   640,   201,     0,     0,     0,   206,     0,   818,   819,
       0,     0,     0,     0,     0,   207,     0,    50,     0,     0,
       0,   199,   200,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,   298,   299,   300,   301,
     302,   892,   203,     0,     0,     0,   204,     0,   303,   205,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
     304,     0,     0,     0,   206,   305,     0,   331,   306,   332,
     333,   307,   265,   207,     0,   308,     0,     0,     0,     0,
       0,   201,     0,     0,     0,   309,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   311,     0,     0,
     202,   203,     0,     0,     0,   204,     0,   844,   205,     0,
     312,   313,   198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,   827,   828,     0,     0,     0,
       0,     0,   207,     0,    50,     0,     0,     0,   199,   200,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,   298,   299,   300,   301,   302,     0,     0,
       0,     0,     0,     0,     0,   303,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,     0,   304,     0,     0,
       0,     0,   305,     0,   331,   306,   332,   333,   307,   265,
       0,     0,   308,     0,     0,     0,     0,     0,   201,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,     0,     0,   311,     0,     0,   202,   203,     0,
       0,     0,   204,     0,   851,   205,     0,   312,   313,   198,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     206,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,    50,     0,     0,     0,   199,   200,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
     298,   299,   300,   301,   302,     0,     0,   618,     0,     0,
       0,     0,   303,     0,   324,   325,   326,     0,   327,     0,
       0,   328,   329,     0,   304,     0,     0,     0,     0,   305,
     330,   331,   306,   332,   333,   307,   265,     0,     0,   308,
       0,     0,     0,     0,     0,   201,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,   202,   203,     0,     0,     0,   204,
       0,   852,   205,     0,   312,   313,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,   207,     0,    50,     0,
       0,     0,   199,   200,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,     0,   303,
       0,   324,   325,   326,   198,   327,     0,     0,   328,   329,
       0,   304,     0,     0,     0,     0,   305,     0,   331,   306,
     332,   333,   307,   265,     0,     0,   308,     0,     0,     0,
     199,   200,   201,     0,     0,     0,   309,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   311,     0,
       0,   202,   203,     0,     0,     0,   204,     0,   853,   205,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
     201,     0,     0,     0,   206,     0,     0,     0,     0,     0,
       0,   198,     0,   207,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,   199,   200,   202,
     203,     0,     0,     0,   204,     0,   854,   205,   324,   325,
     326,     0,   327,     0,     0,   328,   329,   298,   299,   300,
     301,   302,   206,     0,     0,   331,   637,   332,   333,   303,
     265,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,     0,     0,   305,     0,     0,   306,
       0,     0,   307,     0,     0,     0,   308,   201,     0,   659,
       0,     0,     0,     0,     0,     0,   309,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   202,   203,   311,     0,
       0,   204,     0,   936,   205,     0,     0,     0,     0,     0,
       0,   312,   313,   198,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,     0,     0,   207,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,   199,
     200,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,     0,     0,     0,   303,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,   304,     0,
       0,     0,     0,   305,     0,   331,   306,   332,   333,   307,
     265,     0,     0,   308,     0,     0,     0,     0,     0,   201,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,   202,   203,
       0,     0,     0,   204,     0,   975,   205,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     207,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   727,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
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
       0,     0,     0,   729,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,     0,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
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
       0,     0,   331,   743,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
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
       0,     0,   331,   752,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
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
       0,     0,   331,   886,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
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
       0,     0,   331,   962,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,   973,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
       0,     0,     0,   303,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,     0,   304,     0,     0,     0,     0,
     305,     0,   331,   306,   332,   333,   307,   265,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     310,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,   313,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,     0,     0,   331,
       0,   332,   333,     0,   265,   678,   679,   680,   681,   682,
     683,   684,   685,   438,   439,   440,   441,   442,   686,   687,
     443,   444,   445,   446,   793,   447,   448,   449,   450,   436,
     437,     0,   451,   689,   452,   453,   690,   691,     0,     0,
     454,   455,   456,   692,   693,   694,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   794,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,   895,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   678,   679,   680,
     681,   682,   683,   684,   685,   438,   439,   440,   441,   442,
     686,   687,   443,   444,   445,   446,   793,   447,   448,   449,
     450,   436,   437,     0,   451,   689,   452,   453,   690,   691,
       0,     0,   454,   455,   456,   692,   693,   694,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   794,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,   898,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   678,
     679,   680,   681,   682,   683,   684,   685,   438,   439,   440,
     441,   442,   686,   687,   443,   444,   445,   446,   793,   447,
     448,   449,   450,   436,   437,     0,   451,   689,   452,   453,
     690,   691,     0,     0,   454,   455,   456,   692,   693,   694,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   794,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,   918,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   437,
       0,   678,   679,   680,   681,   682,   683,   684,   685,   438,
     439,   440,   441,   442,   686,   687,   443,   444,   445,   446,
     793,   447,   448,   449,   450,     0,     0,     0,   451,   689,
     452,   453,   690,   691,     0,     0,   454,   455,   456,   692,
     693,   694,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     794,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   436,   437,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   438,   439,   440,   441,   442,   956,
       0,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,     0,     0,   451,     0,   452,   453,     0,     0,     0,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   457,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,     0,
       0,   436,   437,     0,     0,     0,     0,   468,   469,   438,
     439,   440,   441,   442,   984,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,   436,
     437,     0,   468,   469,     0,     0,   732,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,   436,   437,     0,
     468,   469,     0,     0,   735,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,   436,   437,     0,   468,   469,
       0,     0,   737,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,   436,   437,     0,   468,   469,     0,     0,
     850,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,   436,   437,     0,   468,   469,     0,     0,   981,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,   436,
     437,     0,   468,   469,     0,     0,   983,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,   436,   437,     0,     0,     0,
     468,   469,     0,     0,  1003,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   436,   437,     0,     0,     0,     0,     0,   468,   469,
     470,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   436,   437,     0,
       0,     0,     0,     0,   468,   469,   776,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,   436,   437,     0,     0,     0,     0,     0,
     468,   469,   784,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   889,   438,
     439,   440,   441,   442,   436,   437,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   890,     0,     0,     0,     0,     0,
       0,     0,   678,   679,   680,   681,   682,   683,   684,   685,
     438,   439,   440,   441,   442,   686,   687,   443,   444,   445,
     446,   793,   447,   448,   449,   450,  -246,   436,   437,   451,
     689,   452,   453,   690,   691,     0,     0,   454,   455,   456,
     692,   693,   694,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   794,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,   678,   679,   680,   681,   682,
     683,   684,   685,   438,   439,   440,   441,   442,   686,   687,
     443,   444,   445,   446,   688,   447,   448,   449,   450,     0,
       0,     0,   451,   689,   452,   453,   690,   691,     0,     0,
     454,   455,   456,   692,   693,   694,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   695,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   436,   437,     0,
       0,   438,   439,   440,   441,   442,   468,   469,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,   582,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   436,   437,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   457,   778,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,   438,   439,   440,   441,   442,   468,   469,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   872,
       0,   436,   437,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,   438,
     439,   440,   441,   442,   468,   469,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,   438,   439,   440,
     441,   442,   468,   469,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,     0,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   437,     0,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,   438,   439,   440,   441,   442,
     468,   469,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   437,     0,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,   438,   439,   440,   441,   442,   468,   469,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,   438,   439,   440,   441,   442,   468,   469,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   437,     0,     0,     0,     0,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,   438,
     439,     0,     0,   442,   468,   469,   443,   444,   445,   446,
       0,   447,   448,   449,   450,   436,   437,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,     0,     0,
       0,   466,     0,     0,     0,     0,     0,   438,   439,   440,
     441,   442,   468,   469,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,   440,   441,   442,     0,     0,   443,     0,
       0,   446,     0,   447,   448,   449,   450,   215,     0,     0,
     451,     0,   452,   453,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,     0,     0,   241,   242,   243,   244,   245,   246,   417,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,   112,     0,     0,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
       0,     0,   257,   258,   113,     0,     0,     0,     0,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,   419,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,   114,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,     0,     0,     0,     0,     0,   112,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,   113,     0,     0,     0,     0,
     314,   315,   316,     0,     0,     0,     0,     0,     0,    51,
     114,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,   112,     0,     0,     0,     0,
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
      12,   168,   362,    73,   367,   477,   479,   420,   286,   393,
     288,   395,   290,   397,   174,   316,   471,   572,   488,   342,
     358,     7,    81,   500,   501,   580,    19,    14,    15,   499,
      51,   380,    21,    51,     0,    34,   131,    20,    21,    19,
       6,    32,   122,    19,    73,    18,    19,   592,    48,   116,
      44,    18,   119,   120,   117,    33,   139,   417,   139,   419,
     148,    73,   148,    29,   148,    31,    55,    33,    20,    21,
      82,   119,   120,    39,   148,   117,    44,    45,   117,   119,
     120,   169,    48,    61,   126,    41,   142,   126,    54,   566,
      14,    15,   187,   156,   157,   148,   159,    60,   184,   162,
     555,   148,   183,    97,   188,   188,   186,    73,   148,   108,
     109,   271,   179,   180,   188,   157,   148,   477,   157,    75,
     162,   156,   156,   106,   107,   108,   109,   487,    94,    97,
     148,   179,   180,   116,   187,   118,   119,   120,   121,   179,
     180,   188,   125,   157,   127,   128,   181,   148,   148,   148,
     182,   148,   186,   139,   132,   142,   108,   109,   186,   146,
     188,   148,   149,   156,   116,   186,   118,   119,   120,   121,
     148,   162,   521,   125,   644,   148,   156,   156,   648,   168,
     156,   148,   165,   166,   167,   168,   169,   188,   743,   180,
     179,   188,   662,   157,   180,   182,   179,   180,   189,   676,
     745,   186,   188,   188,   183,     4,     5,   530,     7,   279,
     280,   281,   282,   272,   180,   285,   554,   287,   142,   289,
     186,   291,   146,   117,   148,   149,   117,   179,   180,   156,
     300,   132,   126,    20,    21,   126,    35,   159,   132,   164,
     117,   711,   156,   182,   314,   315,   156,   148,   187,   126,
     279,   280,   281,   282,   157,   978,   285,   980,   287,   186,
     289,   186,   163,   157,   156,   187,   157,   279,   280,   281,
     282,   300,   186,   285,   132,   287,   164,   289,   156,   291,
     157,   156,   583,   186,   739,   314,   315,   357,   300,   156,
     156,   183,   181,   156,   132,   365,   751,   186,   186,   156,
     156,   159,   314,   315,   859,   183,   150,   151,   183,   159,
     148,    20,    21,   786,   869,   327,   183,   183,   181,   597,
     775,   108,   109,   490,   181,   181,   156,   156,   357,   116,
     743,   498,   119,   120,   121,   790,   365,    68,   125,   752,
     164,    72,   159,   159,   414,   357,   164,   164,   164,   159,
     159,   181,   148,   365,   164,   164,   150,   151,    89,    90,
      91,    92,   186,    11,   376,   182,   182,   148,   186,   148,
     530,   841,   182,   182,    22,    23,   156,    55,    55,   159,
      55,   148,   162,    61,    61,   414,    61,    32,   861,   773,
      55,   132,   179,   180,   406,   142,    61,   106,   107,   108,
     109,   110,   414,   179,   113,   114,   115,   116,   182,   118,
     119,   120,   121,    58,    59,    55,   125,    55,   127,   128,
     432,    61,   148,    61,   133,   134,   135,   150,   151,   152,
     153,   184,   185,   148,   187,   183,    32,   186,   186,   150,
     151,   152,   914,   148,   187,   187,   148,   148,    32,   158,
      97,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    55,    58,    59,   163,   535,   187,   163,   163,   539,
     179,   180,   117,   118,    58,    59,   163,   163,   187,   646,
     550,   126,   552,   163,   762,   182,   186,   557,   655,   559,
     560,   875,   562,   163,   163,   163,   163,   148,   148,    34,
      34,   182,   187,   139,   671,   148,   535,   148,    32,   181,
     539,   187,   157,   158,   206,   186,   148,   162,   181,   183,
     165,   550,   118,   535,   159,   186,   159,   539,   557,   159,
     559,   560,   181,   562,   118,   180,   899,   163,   550,   163,
     552,   186,   163,   182,   189,   557,   163,   559,   560,   163,
     562,   182,    41,   182,   914,   182,   139,   185,   180,   182,
     148,   157,   158,    20,    21,    36,   162,   187,   164,   165,
     148,   148,   642,   157,   158,   148,   186,   142,   162,   186,
     186,   165,   186,   186,   180,   186,   181,   148,   148,   148,
     164,   181,   164,   189,   164,   188,   180,   150,   156,   156,
     148,   163,   186,   182,   148,   189,   148,   148,     1,   122,
      13,   157,   159,   642,   142,     7,   185,    41,   148,   187,
     312,   313,   186,   148,   316,   181,   181,   181,   150,   796,
     642,   148,   324,   325,   326,   186,   328,   329,   188,   331,
     148,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   181,   148,   148,   125,   126,
     127,   128,   129,   130,   148,   367,   133,   134,   135,   136,
     137,   138,   182,   181,   181,   181,   148,   699,   183,   181,
     181,   761,   164,   181,   188,    32,   164,   181,   181,    51,
     157,   158,   181,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   188,   181,    63,   408,   884,   181,   411,
     186,   413,   179,   180,    37,   108,   418,    66,   420,   186,
     424,   977,   761,   900,   277,    64,    82,    20,    21,   699,
     805,     1,   882,   738,   913,   836,   438,   439,   481,   761,
     442,   443,   444,   445,   552,   447,   292,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   707,   469,    45,   284,
     947,   526,   300,   420,   951,   750,   969,    -1,    -1,    -1,
      -1,    -1,   484,    -1,    -1,    -1,   488,    -1,    -1,   491,
     492,    -1,    -1,    -1,    -1,   497,    -1,   499,    -1,    -1,
      -1,    -1,    -1,    -1,   506,   885,    -1,    -1,    -1,    -1,
      20,    21,    -1,   106,   107,   108,   109,   110,   995,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   885,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   885,    -1,    -1,   568,   569,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   579,    -1,    -1,
     582,   583,    -1,    -1,   586,    -1,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    20,    21,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   631,
      -1,    -1,   634,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   644,    -1,    -1,    -1,   648,    -1,   158,   971,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     662,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   990,   179,
     180,    -1,    -1,   183,    -1,    -1,   678,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,    -1,   697,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,   711,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   741,
     742,    -1,    -1,    -1,    -1,    -1,   748,    -1,   750,    -1,
     158,    -1,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,   765,    -1,    -1,    -1,    20,    21,    -1,
      -1,   179,   180,    -1,    -1,    -1,   778,    -1,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   793,   794,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   818,   819,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   827,   828,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   841,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,   893,    -1,    -1,    -1,   897,    -1,   899,    -1,    -1,
      -1,   903,    -1,    -1,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   920,    -1,
      -1,   923,   924,   925,   926,   927,   179,   180,   930,   931,
      -1,   933,    -1,   186,    -1,    -1,     1,    -1,    -1,   941,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   973,    37,    38,    -1,    40,    -1,    42,    43,    -1,
     982,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    -1,   997,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    -1,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    -1,   184,
     185,   186,   187,   188,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    -1,   172,   173,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   182,    -1,   184,   185,   186,
     187,   188,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
     108,   109,    46,    -1,    -1,    49,    -1,    51,   116,    53,
     118,   119,   120,   121,    58,    59,    -1,   125,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,   165,   166,   167,
     168,   169,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,   179,   180,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      32,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    58,    59,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,   173,
      14,    15,    16,    17,    18,    -1,   180,    -1,   182,    -1,
     184,   185,    26,   187,    -1,   189,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,   118,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,   165,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   118,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,    -1,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,   173,
      14,    15,    16,    17,    18,    -1,   180,    -1,   182,    -1,
     184,   185,    26,   187,    -1,   189,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
     108,   109,    46,    -1,    -1,    49,    -1,    -1,   116,    53,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
     168,   169,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,   173,
      14,    15,    16,    17,    18,    -1,   180,    -1,   182,    -1,
     184,   185,    26,   187,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,   157,
     158,    95,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   117,    -1,    -1,    -1,    -1,    -1,   123,
     124,   189,    32,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   165,   166,   167,    -1,   169,    -1,    26,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    38,
     184,   185,   186,   187,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,   118,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   108,
     109,    32,   162,    -1,   164,   165,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
     180,    -1,    -1,   132,    -1,    -1,    -1,    58,    59,   189,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   165,   166,   167,    32,
     169,    -1,    -1,   172,   173,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   182,    46,   184,   185,    49,   187,    -1,
      -1,    53,    -1,    -1,    -1,    58,    59,   118,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,   165,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   189,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    58,    59,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,   165,   165,   166,   167,    -1,   169,    -1,    -1,
     172,   173,    14,    15,    16,    17,    18,   180,   180,    -1,
     182,   183,   184,   185,    26,   187,   189,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,   118,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,   157,   158,    95,    -1,    -1,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    32,    -1,
      -1,    -1,    -1,    -1,   180,   118,   182,    -1,    -1,    -1,
      -1,   123,   124,   189,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    58,    59,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    14,
      15,    16,    17,    18,   157,   158,    -1,    -1,    -1,   162,
      -1,    26,   165,   165,   166,   167,    32,   169,    -1,    -1,
     172,   173,    -1,    38,    -1,    -1,    -1,   180,    43,    -1,
     182,    46,   184,   185,    49,   187,   189,    -1,    53,    -1,
      -1,    -1,    58,    59,   118,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,   108,   109,    32,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,   180,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,   189,    -1,   132,    -1,    -1,
      -1,    58,    59,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    14,    15,    16,    17,
      18,   157,   158,    -1,    -1,    -1,   162,    -1,    26,   165,
     165,   166,   167,    -1,   169,    -1,    -1,   172,   173,    -1,
      38,    -1,    -1,    -1,   180,    43,    -1,   182,    46,   184,
     185,    49,   187,   189,    -1,    53,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,
     108,   109,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   132,    -1,    -1,    -1,    58,    59,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   165,   166,   167,
      -1,   169,    -1,    -1,   172,   173,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   182,    46,   184,   185,    49,   187,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,   165,    -1,   108,   109,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,   132,    -1,    -1,    -1,    58,    59,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    26,    -1,   165,   166,   167,    -1,   169,    -1,
      -1,   172,   173,    -1,    38,    -1,    -1,    -1,    -1,    43,
     181,   182,    46,   184,   185,    49,   187,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,   165,    -1,   108,   109,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   132,    -1,
      -1,    -1,    58,    59,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   165,   166,   167,    32,   169,    -1,    -1,   172,   173,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   182,    46,
     184,   185,    49,   187,    -1,    -1,    53,    -1,    -1,    -1,
      58,    59,   118,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   157,   158,    -1,    -1,    -1,   162,    -1,   164,   165,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   189,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,    58,    59,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,   165,   165,   166,
     167,    -1,   169,    -1,    -1,   172,   173,    14,    15,    16,
      17,    18,   180,    -1,    -1,   182,   183,   184,   185,    26,
     187,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,   118,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,   157,   158,    95,    -1,
      -1,   162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    58,
      59,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   165,   166,
     167,    -1,   169,    -1,    -1,   172,   173,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   182,    46,   184,   185,    49,
     187,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,   165,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,    -1,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   182,    46,   184,   185,    49,   187,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
      -1,   184,   185,    -1,   187,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    20,
      21,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    20,    21,    -1,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,   180,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    20,    21,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,   180,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   106,   107,   108,   109,   110,   186,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   179,   180,   106,
     107,   108,   109,   110,   186,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   179,   180,    -1,    -1,   183,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     179,   180,    -1,    -1,   183,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,   179,   180,
      -1,    -1,   183,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   179,   180,    -1,    -1,
     183,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,   179,   180,    -1,    -1,   183,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   179,   180,    -1,    -1,   183,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     179,   180,    -1,    -1,   183,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   179,   180,
     181,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,   179,   180,   181,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     179,   180,   181,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    20,    21,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,    -1,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    20,    21,    -1,
      -1,   106,   107,   108,   109,   110,   179,   180,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,   131,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   179,   180,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    20,    21,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   179,   180,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   179,   180,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     179,   180,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   179,   180,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   179,   180,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,   106,
     107,    -1,    -1,   110,   179,   180,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    20,    21,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   179,   180,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,   118,   119,   120,   121,    37,    -1,    -1,
     125,    -1,   127,   128,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,    -1,   172,   173,    47,    -1,    -1,    -1,    -1,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    62,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    47,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   191,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   180,   186,   194,   202,   203,   207,   230,   234,
     250,   307,   312,   314,   320,   356,   358,    18,    19,   148,
     224,   225,   226,   142,   208,   209,   148,   169,   204,   205,
     148,   187,   310,   148,   184,   193,   359,   357,    33,    61,
     132,   148,   227,   228,   229,   242,     4,     5,     7,    35,
     318,    60,   305,   157,   156,   159,   156,   204,    21,    55,
     168,   179,   206,   157,   310,   311,   305,   148,   148,   148,
     132,   182,   156,   181,    55,    61,   235,   237,    55,    61,
     313,    55,    61,   319,    55,    61,   306,    14,    15,   142,
     146,   148,   149,   182,   196,   225,   142,   209,   148,   148,
     148,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   242,   323,   325,   326,
     328,   331,   186,   188,   310,    55,    61,   192,   187,   187,
     148,   226,   228,    34,   108,   109,   148,   233,   236,   148,
      97,   316,   317,   187,   308,   150,   151,   195,    14,    15,
     142,   146,   148,   196,   222,   223,   206,   163,   163,   163,
     163,   182,   163,   163,   163,   163,   163,   163,    32,    58,
      59,   118,   157,   158,   162,   165,   180,   189,   186,   148,
     327,   148,   297,   298,   183,    37,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   119,   120,   126,   127,   128,   129,
     130,   133,   134,   135,   136,   137,   138,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   172,   173,   179,
     180,    34,    34,   182,   231,   187,   238,   159,   187,   148,
     321,   304,   139,   286,   150,   151,   152,   156,   183,   334,
     336,   338,   332,   148,   329,   340,   342,   344,   346,   348,
     350,   352,   354,    32,    32,   162,   180,   189,    14,    15,
      16,    17,    18,    26,    38,    43,    46,    49,    53,    63,
      73,    95,   108,   109,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   149,   165,   166,   167,   169,   172,   173,
     181,   182,   184,   185,   198,   200,   238,   242,   244,   252,
     259,   262,   265,   269,   270,   273,   274,   275,   280,   283,
     323,   360,   364,   369,   371,   373,   186,   159,   186,   181,
     181,   183,   286,   293,   294,   159,   232,   240,    68,    72,
      89,    90,    91,    92,   324,   309,   159,   315,   187,   188,
     286,   226,   148,   303,   322,   222,   331,   331,   331,   331,
     183,   327,   331,   231,   331,   231,   331,   231,   148,   295,
     296,   331,   298,   181,   331,   360,   182,   163,   182,   163,
     163,   182,   163,   182,   163,   283,   283,    12,   331,    12,
     331,   283,   366,   370,   197,   283,   283,   283,   242,   283,
     283,   283,   185,   148,   182,   230,    20,    21,   106,   107,
     108,   109,   110,   113,   114,   115,   116,   118,   119,   120,
     121,   125,   127,   128,   133,   134,   135,   158,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   179,   180,
     181,   182,   148,   188,   331,   188,   297,    48,   300,   322,
     183,   186,   331,     1,     8,     9,    10,    12,    25,    27,
      28,    37,    40,    42,    50,    52,    56,    57,    63,    96,
     123,   124,   186,   188,   210,   211,   214,   215,   216,   217,
     218,   219,   220,   239,   241,   243,   245,   246,   247,   248,
     249,   250,   251,   271,   272,   283,   187,   148,   188,   242,
     291,   303,   181,    41,   156,   159,   162,   302,   164,   186,
     164,   164,   164,   186,   164,   232,   164,   232,   164,   232,
     159,   164,   186,   164,   186,   182,   242,   260,   283,   253,
     255,   283,   257,   283,   331,   322,    51,   148,   165,   182,
     283,   361,   362,   363,   365,   366,   367,   368,   322,   182,
     362,   368,   131,   186,   188,   152,   153,   195,   201,   183,
     163,   242,   180,   183,   263,   283,   139,   268,    18,   148,
     148,   283,   283,   283,   283,   283,   283,   148,   283,   148,
     283,   283,   283,   283,   283,   283,   283,   283,    21,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     119,   120,   148,   179,   180,   281,   283,   183,   263,   300,
     117,   157,   159,   162,   299,   293,   283,   322,   117,   186,
     241,   271,   283,   238,   283,   283,   148,   186,   142,    56,
     283,   238,   117,   241,   283,   185,   269,   269,    36,   186,
     186,   283,   186,   186,   186,   303,   122,   186,    98,    99,
     100,   101,   102,   103,   104,   105,   111,   112,   117,   126,
     129,   130,   136,   137,   138,   157,   309,   157,   186,   180,
     188,   230,   286,   290,   181,   148,   148,   331,   117,   126,
     157,   301,   335,   331,   339,   333,   330,   341,   164,   345,
     164,   349,   164,   331,   353,   295,   355,   183,   263,   163,
     283,   331,   183,   331,   331,   183,   331,   183,   164,    19,
     269,   126,   299,   183,   156,   186,   363,   181,   156,   181,
     186,    19,   183,   363,   188,   283,   366,   188,   283,   150,
     199,   276,   278,   148,   361,   156,   183,   117,   126,   157,
     162,   266,   267,   231,   163,   182,   181,   148,   159,   148,
     283,   148,   283,     1,   181,   183,   331,   241,   283,   238,
      19,   241,   283,   117,   157,   186,    13,   238,   157,   159,
     142,   241,   283,   186,   185,   187,   238,   269,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   123,   124,
     283,   283,   283,   283,   283,   283,   283,   123,   124,   283,
     188,   283,   229,     7,    44,    97,   287,   186,    41,   157,
     186,   301,   241,   283,   164,   343,   347,   351,   183,   148,
     183,   164,   164,   164,   164,   268,   263,   283,   283,   362,
     363,   148,   361,   181,   181,   283,   181,   367,   263,   362,
     188,   181,   154,   150,   331,   231,   181,   283,   148,   148,
     148,   148,   156,   181,   232,   284,   183,   263,   283,   181,
     181,   282,   157,   299,   263,   186,   238,   221,   186,   240,
      11,    22,    23,   212,   213,   283,   283,   283,   283,   186,
     181,    55,    61,   289,   289,   148,   241,   283,   186,   337,
     164,   186,   261,   254,   256,   258,   182,   186,   363,   181,
     126,   299,   181,   186,   363,   181,   164,   232,   183,   266,
     181,   131,   238,   264,   331,   183,   283,   186,   283,   188,
     238,   283,   186,    45,   287,   300,   186,   283,   148,   283,
     283,   283,   183,   283,   283,   181,   181,   283,   283,   283,
     188,   277,   164,   117,   283,   164,   238,   238,   292,    32,
     288,   183,   164,   183,   186,   372,   181,   372,   181,   242,
     279,   283,   285,   213,   236,   236,   283,    51,   181,   188,
     242,   186,   238,   183,   283,   181,   181
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   192,   192,   192,   193,   193,
     194,   195,   195,   195,   195,   196,   197,   197,   197,   198,
     199,   199,   201,   200,   202,   203,   204,   204,   204,   204,
     205,   205,   206,   206,   207,   208,   208,   209,   209,   210,
     211,   211,   212,   212,   213,   213,   213,   214,   214,   215,
     216,   217,   218,   219,   221,   220,   222,   222,   222,   222,
     222,   222,   223,   223,   224,   224,   224,   225,   225,   225,
     225,   225,   225,   225,   225,   226,   226,   227,   227,   227,
     228,   228,   229,   229,   230,   230,   231,   231,   231,   232,
     232,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     234,   235,   235,   235,   236,   237,   238,   238,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   240,   240,
     240,   241,   241,   241,   242,   242,   242,   243,   244,   244,
     244,   244,   245,   246,   247,   247,   247,   247,   247,   248,
     248,   248,   248,   249,   250,   250,   251,   253,   254,   252,
     255,   256,   252,   257,   258,   252,   260,   261,   259,   262,
     262,   262,   263,   263,   264,   264,   264,   265,   265,   265,
     266,   266,   266,   266,   267,   267,   268,   268,   269,   269,
     270,   270,   270,   270,   270,   270,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   272,   272,   272,   272,   273,
     274,   274,   275,   276,   277,   275,   278,   279,   275,   280,
     280,   281,   282,   280,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   284,   285,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   286,   286,
     287,   287,   287,   288,   288,   289,   289,   289,   290,   291,
     291,   292,   291,   291,   291,   293,   293,   294,   294,   295,
     295,   296,   296,   297,   298,   298,   299,   299,   300,   300,
     300,   300,   300,   300,   301,   301,   301,   302,   302,   303,
     303,   303,   303,   303,   304,   304,   305,   305,   306,   306,
     306,   307,   308,   307,   309,   309,   309,   310,   311,   311,
     312,   312,   313,   313,   313,   314,   314,   315,   315,   316,
     316,   317,   318,   318,   319,   319,   319,   321,   320,   322,
     322,   322,   322,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   324,   324,
     324,   324,   324,   324,   325,   326,   326,   327,   327,   329,
     330,   328,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   332,
     333,   331,   331,   334,   335,   331,   336,   337,   331,   338,
     339,   331,   331,   340,   341,   331,   342,   343,   331,   331,
     344,   345,   331,   346,   347,   331,   331,   348,   349,   331,
     350,   351,   331,   352,   353,   331,   354,   355,   331,   357,
     356,   359,   358,   360,   360,   360,   360,   361,   361,   361,
     361,   362,   362,   363,   363,   364,   364,   364,   364,   364,
     364,   365,   365,   366,   367,   367,   368,   368,   369,   369,
     370,   370,   371,   372,   372,   373,   373
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
       6,     2,     3,     3,     0,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     1,     1,     1,
       1,     4,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     2,     2,
       3,     0,     1,     1,     3,     3,     3,     7,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
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
       4,     0,     0,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     5,     4,     5,     3,     4,     1,
       3,     4,     3,     4,     2,     4,     4,     7,     8,     3,
       5,     0,     0,     8,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     4,     0,     4,
       0,     1,     1,     0,     1,     0,     1,     1,     4,     0,
       3,     0,     8,     8,     5,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     3,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     3,     5,     3,     2,     3,
       4,     2,     0,     1,     1,     7,     9,     0,     2,     0,
       1,     3,     1,     1,     0,     1,     1,     0,     8,     1,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     0,
       0,     6,     1,     1,     1,     1,     4,     3,     4,     2,
       2,     3,     2,     3,     2,     2,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     8,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     0,
       7,     0,     7,     1,     1,     1,     1,     3,     3,     5,
       5,     1,     3,     0,     2,     6,     5,     7,     8,     6,
       8,     1,     3,     3,     1,     1,     1,     3,     5,     5,
       1,     3,     4,     0,     3,    10,    10
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
        yyerror (&yylloc, scanner, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (scanner);
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
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, scanner);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
    case 148: /* "name"  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3484 "ds_parser.cpp"
        break;

    case 193: /* module_name  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3490 "ds_parser.cpp"
        break;

    case 195: /* character_sequence  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3496 "ds_parser.cpp"
        break;

    case 196: /* string_constant  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3502 "ds_parser.cpp"
        break;

    case 197: /* string_builder_body  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3508 "ds_parser.cpp"
        break;

    case 198: /* string_builder  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3514 "ds_parser.cpp"
        break;

    case 200: /* expr_reader  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3520 "ds_parser.cpp"
        break;

    case 204: /* require_module_name  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3526 "ds_parser.cpp"
        break;

    case 210: /* expression_label  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3532 "ds_parser.cpp"
        break;

    case 211: /* expression_goto  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3538 "ds_parser.cpp"
        break;

    case 213: /* expression_else  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3544 "ds_parser.cpp"
        break;

    case 215: /* expression_if_then_else  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3550 "ds_parser.cpp"
        break;

    case 216: /* expression_for_loop  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3556 "ds_parser.cpp"
        break;

    case 217: /* expression_unsafe  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3562 "ds_parser.cpp"
        break;

    case 218: /* expression_while_loop  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3568 "ds_parser.cpp"
        break;

    case 219: /* expression_with  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3574 "ds_parser.cpp"
        break;

    case 220: /* expression_with_alias  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3580 "ds_parser.cpp"
        break;

    case 222: /* annotation_argument_value  */
#line 159 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3586 "ds_parser.cpp"
        break;

    case 223: /* annotation_argument_value_list  */
#line 160 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3592 "ds_parser.cpp"
        break;

    case 224: /* annotation_argument_name  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3598 "ds_parser.cpp"
        break;

    case 225: /* annotation_argument  */
#line 159 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3604 "ds_parser.cpp"
        break;

    case 226: /* annotation_argument_list  */
#line 160 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3610 "ds_parser.cpp"
        break;

    case 227: /* annotation_declaration_name  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3616 "ds_parser.cpp"
        break;

    case 228: /* annotation_declaration  */
#line 161 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3622 "ds_parser.cpp"
        break;

    case 229: /* annotation_list  */
#line 162 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3628 "ds_parser.cpp"
        break;

    case 230: /* optional_annotation_list  */
#line 162 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3634 "ds_parser.cpp"
        break;

    case 231: /* optional_function_argument_list  */
#line 156 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3640 "ds_parser.cpp"
        break;

    case 232: /* optional_function_type  */
#line 157 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3646 "ds_parser.cpp"
        break;

    case 233: /* function_name  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3652 "ds_parser.cpp"
        break;

    case 238: /* expression_block  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp"
        break;

    case 239: /* expression_any  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 240: /* expressions  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 241: /* expr_pipe  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3676 "ds_parser.cpp"
        break;

    case 242: /* name_in_namespace  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3682 "ds_parser.cpp"
        break;

    case 243: /* expression_delete  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3688 "ds_parser.cpp"
        break;

    case 244: /* expr_new  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3694 "ds_parser.cpp"
        break;

    case 245: /* expression_break  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3700 "ds_parser.cpp"
        break;

    case 246: /* expression_continue  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3706 "ds_parser.cpp"
        break;

    case 247: /* expression_return  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3712 "ds_parser.cpp"
        break;

    case 248: /* expression_yield  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3718 "ds_parser.cpp"
        break;

    case 249: /* expression_try_catch  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3724 "ds_parser.cpp"
        break;

    case 251: /* expression_let  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3730 "ds_parser.cpp"
        break;

    case 252: /* expr_cast  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3736 "ds_parser.cpp"
        break;

    case 259: /* expr_type_decl  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3742 "ds_parser.cpp"
        break;

    case 262: /* expr_type_info  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3748 "ds_parser.cpp"
        break;

    case 263: /* expr_list  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3754 "ds_parser.cpp"
        break;

    case 264: /* block_or_simple_block  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3760 "ds_parser.cpp"
        break;

    case 266: /* capture_entry  */
#line 165 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3766 "ds_parser.cpp"
        break;

    case 267: /* capture_list  */
#line 166 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3772 "ds_parser.cpp"
        break;

    case 268: /* optional_capture_list  */
#line 166 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3778 "ds_parser.cpp"
        break;

    case 269: /* expr_block  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3784 "ds_parser.cpp"
        break;

    case 270: /* expr_numeric_const  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3790 "ds_parser.cpp"
        break;

    case 271: /* expr_assign  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3796 "ds_parser.cpp"
        break;

    case 272: /* expr_assign_pipe  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3802 "ds_parser.cpp"
        break;

    case 273: /* expr_named_call  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3808 "ds_parser.cpp"
        break;

    case 274: /* expr_method_call  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3814 "ds_parser.cpp"
        break;

    case 275: /* func_addr_expr  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3820 "ds_parser.cpp"
        break;

    case 280: /* expr_field  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3826 "ds_parser.cpp"
        break;

    case 283: /* expr  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3832 "ds_parser.cpp"
        break;

    case 286: /* optional_field_annotation  */
#line 160 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3838 "ds_parser.cpp"
        break;

    case 290: /* structure_variable_declaration  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3844 "ds_parser.cpp"
        break;

    case 291: /* struct_variable_declaration_list  */
#line 156 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3850 "ds_parser.cpp"
        break;

    case 293: /* function_argument_declaration  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3856 "ds_parser.cpp"
        break;

    case 294: /* function_argument_list  */
#line 156 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3862 "ds_parser.cpp"
        break;

    case 295: /* tuple_type  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3868 "ds_parser.cpp"
        break;

    case 296: /* tuple_type_list  */
#line 156 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3874 "ds_parser.cpp"
        break;

    case 297: /* variant_type  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3880 "ds_parser.cpp"
        break;

    case 298: /* variant_type_list  */
#line 156 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3886 "ds_parser.cpp"
        break;

    case 300: /* variable_declaration  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3892 "ds_parser.cpp"
        break;

    case 303: /* let_variable_declaration  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3898 "ds_parser.cpp"
        break;

    case 304: /* global_variable_declaration_list  */
#line 156 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3904 "ds_parser.cpp"
        break;

    case 309: /* enum_list  */
#line 164 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3910 "ds_parser.cpp"
        break;

    case 315: /* optional_structure_parent  */
#line 152 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3916 "ds_parser.cpp"
        break;

    case 322: /* variable_name_with_pos_list  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3922 "ds_parser.cpp"
        break;

    case 325: /* structure_type_declaration  */
#line 157 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3928 "ds_parser.cpp"
        break;

    case 326: /* auto_type_declaration  */
#line 157 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3934 "ds_parser.cpp"
        break;

    case 327: /* bitfield_bits  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3940 "ds_parser.cpp"
        break;

    case 328: /* bitfield_type_declaration  */
#line 157 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3946 "ds_parser.cpp"
        break;

    case 331: /* type_declaration  */
#line 157 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3952 "ds_parser.cpp"
        break;

    case 360: /* make_decl  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3958 "ds_parser.cpp"
        break;

    case 361: /* make_struct_fields  */
#line 163 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3964 "ds_parser.cpp"
        break;

    case 362: /* make_struct_dim  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3970 "ds_parser.cpp"
        break;

    case 363: /* optional_block  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3976 "ds_parser.cpp"
        break;

    case 364: /* make_struct_decl  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3982 "ds_parser.cpp"
        break;

    case 365: /* make_tuple  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3988 "ds_parser.cpp"
        break;

    case 366: /* make_map_tuple  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3994 "ds_parser.cpp"
        break;

    case 367: /* make_any_tuple  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4000 "ds_parser.cpp"
        break;

    case 368: /* make_dim  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4006 "ds_parser.cpp"
        break;

    case 369: /* make_dim_decl  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4012 "ds_parser.cpp"
        break;

    case 370: /* make_table  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4018 "ds_parser.cpp"
        break;

    case 371: /* make_table_decl  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4024 "ds_parser.cpp"
        break;

    case 372: /* array_comprehension_where  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4030 "ds_parser.cpp"
        break;

    case 373: /* array_comprehension  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4036 "ds_parser.cpp"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
#line 505 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4334 "ds_parser.cpp"
    break;

  case 16:
#line 506 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4340 "ds_parser.cpp"
    break;

  case 17:
#line 507 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4346 "ds_parser.cpp"
    break;

  case 18:
#line 511 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4352 "ds_parser.cpp"
    break;

  case 19:
#line 512 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4358 "ds_parser.cpp"
    break;

  case 20:
#line 516 "ds_parser.ypp"
    {
        g_Program->thisModule->isPublic = (yyvsp[0].b);
        g_Program->thisModule->isModule = true;
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das_yyerror("this module already has a name " + g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
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
#line 533 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4383 "ds_parser.cpp"
    break;

  case 22:
#line 534 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4389 "ds_parser.cpp"
    break;

  case 23:
#line 535 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4395 "ds_parser.cpp"
    break;

  case 24:
#line 536 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4401 "ds_parser.cpp"
    break;

  case 25:
#line 540 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4407 "ds_parser.cpp"
    break;

  case 26:
#line 544 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4417 "ds_parser.cpp"
    break;

  case 27:
#line 549 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4431 "ds_parser.cpp"
    break;

  case 28:
#line 558 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4441 "ds_parser.cpp"
    break;

  case 29:
#line 566 "ds_parser.ypp"
    {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),"");
            delete (yyvsp[-1].pExpression);
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_pointer_cast<ExprConstString>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),sconst->text);
            delete (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
#line 4459 "ds_parser.cpp"
    break;

  case 30:
#line 582 "ds_parser.ypp"
    {
        if ( !yyextra->g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
#line 4469 "ds_parser.cpp"
    break;

  case 31:
#line 587 "ds_parser.ypp"
    {
        if ( !yyextra->g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
#line 4479 "ds_parser.cpp"
    break;

  case 32:
#line 595 "ds_parser.ypp"
    {
        auto macros = g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das_yyerror("reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das_yyerror("too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das_yyerror("expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::syntax_error);
        } else {
            yyextra->g_ReaderMacro = macros.back().get();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das_yybegin_reader(scanner);
        }
    }
#line 4506 "ds_parser.cpp"
    break;

  case 33:
#line 616 "ds_parser.ypp"
    {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
#line 4518 "ds_parser.cpp"
    break;

  case 34:
#line 625 "ds_parser.ypp"
    {
        if ( g_Program->options.size() ) {
            g_Program->options.insert ( g_Program->options.begin(),
                (yyvsp[0].aaList)->begin(), (yyvsp[0].aaList)->end() );
        } else {
            swap ( g_Program->options, *(yyvsp[0].aaList) );
        }
        auto opt = g_Program->options.find("indenting", tInt);
        if (opt)
        {
            if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
                yyextra->das_tab_size = yyextra->das_def_tab_size;
            else
                yyextra->das_tab_size = opt->iValue ? opt->iValue : yyextra->das_def_tab_size;//0 is default
            yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
        }
        delete (yyvsp[0].aaList);
    }
#line 4541 "ds_parser.cpp"
    break;

  case 36:
#line 650 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4549 "ds_parser.cpp"
    break;

  case 37:
#line 653 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4558 "ds_parser.cpp"
    break;

  case 38:
#line 657 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4569 "ds_parser.cpp"
    break;

  case 39:
#line 663 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4580 "ds_parser.cpp"
    break;

  case 40:
#line 672 "ds_parser.ypp"
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-1].s), yyextra->g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.moduleName) ) {
            g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-1].s),"",(yyvsp[0].b),tokAt(scanner,(yylsp[-1]))));
            g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
            if ( !info.importName.empty() ) {
                auto ita = yyextra->das_module_alias.find(info.importName);
                if ( ita !=yyextra->das_module_alias.end() ) {
                    if ( ita->second != info.moduleName ) {
                        das_yyerror("module alias already used " + info.importName + " as " + ita->second,tokAt(scanner,(yylsp[-1])),
                            CompilationError::module_not_found);
                    }
                } else {
                    yyextra->das_module_alias[info.importName] = info.moduleName;
                }
            }
        } else {
            g_Program->allRequireDecl.push_back(make_tuple((Module *)nullptr,*(yyvsp[-1].s),"",(yyvsp[0].b),tokAt(scanner,(yylsp[-1]))));
            das_yyerror("required module not found " + *(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-1].s);
    }
#line 4608 "ds_parser.cpp"
    break;

  case 41:
#line 695 "ds_parser.ypp"
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-3].s), yyextra->g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.moduleName) ) {
            g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-3].s),*(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3]))));
            g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
            auto ita = yyextra->das_module_alias.find(*(yyvsp[-1].s));
            if ( ita !=yyextra->das_module_alias.end() ) {
                if ( ita->second != info.moduleName ) {
                    das_yyerror("module alias already used " + *(yyvsp[-1].s) + " as " + ita->second,tokAt(scanner,(yylsp[-3])),
                        CompilationError::module_not_found);
                }
            } else {
                yyextra->das_module_alias[*(yyvsp[-1].s)] = info.moduleName;
            }
        } else {
            g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-3].s),*(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3]))));
            das_yyerror("required module not found " + *(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-3].s);
        delete (yyvsp[-1].s);
    }
#line 4635 "ds_parser.cpp"
    break;

  case 42:
#line 720 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4641 "ds_parser.cpp"
    break;

  case 43:
#line 721 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4647 "ds_parser.cpp"
    break;

  case 47:
#line 734 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4655 "ds_parser.cpp"
    break;

  case 48:
#line 737 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4663 "ds_parser.cpp"
    break;

  case 49:
#line 743 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
#line 4671 "ds_parser.cpp"
    break;

  case 50:
#line 749 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
#line 4679 "ds_parser.cpp"
    break;

  case 51:
#line 752 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4687 "ds_parser.cpp"
    break;

  case 52:
#line 758 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4693 "ds_parser.cpp"
    break;

  case 53:
#line 759 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4699 "ds_parser.cpp"
    break;

  case 54:
#line 763 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4705 "ds_parser.cpp"
    break;

  case 55:
#line 764 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4711 "ds_parser.cpp"
    break;

  case 56:
#line 765 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4722 "ds_parser.cpp"
    break;

  case 57:
#line 774 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4728 "ds_parser.cpp"
    break;

  case 58:
#line 775 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4734 "ds_parser.cpp"
    break;

  case 59:
#line 779 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4745 "ds_parser.cpp"
    break;

  case 60:
#line 788 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt(scanner,(yylsp[-5])));
        pFor->visibility = tokAt(scanner,(yylsp[0]));
        for ( const auto & np : *(yyvsp[-4].pNameWithPosList) ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            pFor->iteratorsAt.push_back(np.at);
        }
        delete (yyvsp[-4].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4764 "ds_parser.cpp"
    break;

  case 61:
#line 805 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4774 "ds_parser.cpp"
    break;

  case 62:
#line 813 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4786 "ds_parser.cpp"
    break;

  case 63:
#line 823 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4797 "ds_parser.cpp"
    break;

  case 64:
#line 832 "ds_parser.ypp"
    { yyextra->das_need_oxford_comma=true; }
#line 4803 "ds_parser.cpp"
    break;

  case 65:
#line 832 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
#line 4812 "ds_parser.cpp"
    break;

  case 66:
#line 839 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4818 "ds_parser.cpp"
    break;

  case 67:
#line 840 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4824 "ds_parser.cpp"
    break;

  case 68:
#line 841 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4830 "ds_parser.cpp"
    break;

  case 69:
#line 842 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4836 "ds_parser.cpp"
    break;

  case 70:
#line 843 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4842 "ds_parser.cpp"
    break;

  case 71:
#line 844 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4848 "ds_parser.cpp"
    break;

  case 72:
#line 848 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4858 "ds_parser.cpp"
    break;

  case 73:
#line 853 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4868 "ds_parser.cpp"
    break;

  case 74:
#line 861 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4874 "ds_parser.cpp"
    break;

  case 75:
#line 862 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4880 "ds_parser.cpp"
    break;

  case 76:
#line 863 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4886 "ds_parser.cpp"
    break;

  case 77:
#line 867 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4892 "ds_parser.cpp"
    break;

  case 78:
#line 868 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4898 "ds_parser.cpp"
    break;

  case 79:
#line 869 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4904 "ds_parser.cpp"
    break;

  case 80:
#line 870 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4910 "ds_parser.cpp"
    break;

  case 81:
#line 871 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4916 "ds_parser.cpp"
    break;

  case 82:
#line 872 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4922 "ds_parser.cpp"
    break;

  case 83:
#line 873 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
#line 4928 "ds_parser.cpp"
    break;

  case 84:
#line 874 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4936 "ds_parser.cpp"
    break;

  case 85:
#line 880 "ds_parser.ypp"
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
#line 4955 "ds_parser.cpp"
    break;

  case 86:
#line 894 "ds_parser.ypp"
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
#line 4974 "ds_parser.cpp"
    break;

  case 87:
#line 911 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4980 "ds_parser.cpp"
    break;

  case 88:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4986 "ds_parser.cpp"
    break;

  case 89:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4992 "ds_parser.cpp"
    break;

  case 90:
#line 917 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 5005 "ds_parser.cpp"
    break;

  case 91:
#line 925 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 5020 "ds_parser.cpp"
    break;

  case 92:
#line 938 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5029 "ds_parser.cpp"
    break;

  case 93:
#line 942 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5038 "ds_parser.cpp"
    break;

  case 94:
#line 949 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 5044 "ds_parser.cpp"
    break;

  case 95:
#line 950 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 5050 "ds_parser.cpp"
    break;

  case 96:
#line 954 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5056 "ds_parser.cpp"
    break;

  case 97:
#line 955 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5062 "ds_parser.cpp"
    break;

  case 98:
#line 956 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5068 "ds_parser.cpp"
    break;

  case 99:
#line 960 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5076 "ds_parser.cpp"
    break;

  case 100:
#line 963 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 5085 "ds_parser.cpp"
    break;

  case 101:
#line 970 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5094 "ds_parser.cpp"
    break;

  case 102:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5100 "ds_parser.cpp"
    break;

  case 103:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5106 "ds_parser.cpp"
    break;

  case 104:
#line 976 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5112 "ds_parser.cpp"
    break;

  case 105:
#line 977 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5118 "ds_parser.cpp"
    break;

  case 106:
#line 978 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5124 "ds_parser.cpp"
    break;

  case 107:
#line 979 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5130 "ds_parser.cpp"
    break;

  case 108:
#line 980 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5136 "ds_parser.cpp"
    break;

  case 109:
#line 981 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5142 "ds_parser.cpp"
    break;

  case 110:
#line 982 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5148 "ds_parser.cpp"
    break;

  case 111:
#line 983 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5154 "ds_parser.cpp"
    break;

  case 112:
#line 984 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5160 "ds_parser.cpp"
    break;

  case 113:
#line 985 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5166 "ds_parser.cpp"
    break;

  case 114:
#line 986 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5172 "ds_parser.cpp"
    break;

  case 115:
#line 987 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5178 "ds_parser.cpp"
    break;

  case 116:
#line 988 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5184 "ds_parser.cpp"
    break;

  case 117:
#line 989 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5190 "ds_parser.cpp"
    break;

  case 118:
#line 990 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5196 "ds_parser.cpp"
    break;

  case 119:
#line 991 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5202 "ds_parser.cpp"
    break;

  case 120:
#line 992 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5208 "ds_parser.cpp"
    break;

  case 121:
#line 993 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5214 "ds_parser.cpp"
    break;

  case 122:
#line 994 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5220 "ds_parser.cpp"
    break;

  case 123:
#line 995 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5226 "ds_parser.cpp"
    break;

  case 124:
#line 996 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5232 "ds_parser.cpp"
    break;

  case 125:
#line 997 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5238 "ds_parser.cpp"
    break;

  case 126:
#line 998 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5244 "ds_parser.cpp"
    break;

  case 127:
#line 999 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5250 "ds_parser.cpp"
    break;

  case 128:
#line 1000 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5256 "ds_parser.cpp"
    break;

  case 129:
#line 1001 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5262 "ds_parser.cpp"
    break;

  case 130:
#line 1002 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5268 "ds_parser.cpp"
    break;

  case 131:
#line 1003 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5274 "ds_parser.cpp"
    break;

  case 132:
#line 1004 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5280 "ds_parser.cpp"
    break;

  case 133:
#line 1005 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5286 "ds_parser.cpp"
    break;

  case 134:
#line 1006 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5292 "ds_parser.cpp"
    break;

  case 135:
#line 1007 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5298 "ds_parser.cpp"
    break;

  case 136:
#line 1008 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5304 "ds_parser.cpp"
    break;

  case 137:
#line 1009 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5310 "ds_parser.cpp"
    break;

  case 138:
#line 1010 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5316 "ds_parser.cpp"
    break;

  case 139:
#line 1011 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5322 "ds_parser.cpp"
    break;

  case 140:
#line 1012 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5328 "ds_parser.cpp"
    break;

  case 141:
#line 1013 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5334 "ds_parser.cpp"
    break;

  case 142:
#line 1014 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5340 "ds_parser.cpp"
    break;

  case 143:
#line 1015 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5346 "ds_parser.cpp"
    break;

  case 144:
#line 1016 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5352 "ds_parser.cpp"
    break;

  case 145:
#line 1017 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5358 "ds_parser.cpp"
    break;

  case 146:
#line 1018 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5364 "ds_parser.cpp"
    break;

  case 147:
#line 1019 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5370 "ds_parser.cpp"
    break;

  case 148:
#line 1020 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5376 "ds_parser.cpp"
    break;

  case 149:
#line 1021 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5382 "ds_parser.cpp"
    break;

  case 150:
#line 1025 "ds_parser.ypp"
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
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
#line 5406 "ds_parser.cpp"
    break;

  case 151:
#line 1047 "ds_parser.ypp"
    { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5412 "ds_parser.cpp"
    break;

  case 152:
#line 1048 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5418 "ds_parser.cpp"
    break;

  case 153:
#line 1049 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5424 "ds_parser.cpp"
    break;

  case 154:
#line 1053 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt(scanner,(yylsp[-2]));
        pFunction->name = *(yyvsp[-2].s);
        pFunction->result = TypeDeclPtr((yyvsp[0].pTypeDecl));
        if ( (yyvsp[-1].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name_at.name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.name;
                            pVar->aka = name_at.aka;
                            pVar->at = name_at.at;
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
                            das_yyerror("function argument is already declared " + name_at.name,name_at.at,
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
#line 5465 "ds_parser.cpp"
    break;

  case 155:
#line 1092 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5475 "ds_parser.cpp"
    break;

  case 156:
#line 1100 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
#line 5484 "ds_parser.cpp"
    break;

  case 157:
#line 1104 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5497 "ds_parser.cpp"
    break;

  case 158:
#line 1115 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5503 "ds_parser.cpp"
    break;

  case 159:
#line 1116 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5509 "ds_parser.cpp"
    break;

  case 160:
#line 1117 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5515 "ds_parser.cpp"
    break;

  case 161:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5521 "ds_parser.cpp"
    break;

  case 162:
#line 1119 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5527 "ds_parser.cpp"
    break;

  case 163:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5533 "ds_parser.cpp"
    break;

  case 164:
#line 1121 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5539 "ds_parser.cpp"
    break;

  case 165:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5545 "ds_parser.cpp"
    break;

  case 166:
#line 1123 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5551 "ds_parser.cpp"
    break;

  case 167:
#line 1124 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5557 "ds_parser.cpp"
    break;

  case 168:
#line 1125 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5563 "ds_parser.cpp"
    break;

  case 169:
#line 1126 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5569 "ds_parser.cpp"
    break;

  case 170:
#line 1127 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5575 "ds_parser.cpp"
    break;

  case 171:
#line 1128 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5581 "ds_parser.cpp"
    break;

  case 172:
#line 1129 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5587 "ds_parser.cpp"
    break;

  case 173:
#line 1130 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5593 "ds_parser.cpp"
    break;

  case 174:
#line 1131 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5599 "ds_parser.cpp"
    break;

  case 175:
#line 1132 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5605 "ds_parser.cpp"
    break;

  case 176:
#line 1133 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5611 "ds_parser.cpp"
    break;

  case 177:
#line 1134 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5617 "ds_parser.cpp"
    break;

  case 178:
#line 1138 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5627 "ds_parser.cpp"
    break;

  case 179:
#line 1143 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5638 "ds_parser.cpp"
    break;

  case 180:
#line 1149 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5646 "ds_parser.cpp"
    break;

  case 181:
#line 1155 "ds_parser.ypp"
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
                    tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into function call or [[ make structure ]]",
                tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 5685 "ds_parser.cpp"
    break;

  case 182:
#line 1189 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5693 "ds_parser.cpp"
    break;

  case 183:
#line 1192 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5701 "ds_parser.cpp"
    break;

  case 184:
#line 1198 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5707 "ds_parser.cpp"
    break;

  case 185:
#line 1199 "ds_parser.ypp"
    {
            auto ita = yyextra->das_module_alias.find(*(yyvsp[-2].s));
            if ( ita == yyextra->das_module_alias.end() ) {
                *(yyvsp[-2].s) += "::";
            } else {
                *(yyvsp[-2].s) = ita->second + "::";
            }
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 5723 "ds_parser.cpp"
    break;

  case 186:
#line 1210 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5729 "ds_parser.cpp"
    break;

  case 187:
#line 1214 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5737 "ds_parser.cpp"
    break;

  case 188:
#line 1220 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5745 "ds_parser.cpp"
    break;

  case 189:
#line 1223 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5753 "ds_parser.cpp"
    break;

  case 190:
#line 1226 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5762 "ds_parser.cpp"
    break;

  case 191:
#line 1230 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5770 "ds_parser.cpp"
    break;

  case 192:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
#line 5776 "ds_parser.cpp"
    break;

  case 193:
#line 1240 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
#line 5782 "ds_parser.cpp"
    break;

  case 194:
#line 1244 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),nullptr);
    }
#line 5790 "ds_parser.cpp"
    break;

  case 195:
#line 1247 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5798 "ds_parser.cpp"
    break;

  case 196:
#line 1250 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5808 "ds_parser.cpp"
    break;

  case 197:
#line 1255 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5816 "ds_parser.cpp"
    break;

  case 198:
#line 1258 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5826 "ds_parser.cpp"
    break;

  case 199:
#line 1266 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5834 "ds_parser.cpp"
    break;

  case 200:
#line 1269 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5844 "ds_parser.cpp"
    break;

  case 201:
#line 1274 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5852 "ds_parser.cpp"
    break;

  case 202:
#line 1277 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5862 "ds_parser.cpp"
    break;

  case 203:
#line 1285 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5870 "ds_parser.cpp"
    break;

  case 204:
#line 1291 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5876 "ds_parser.cpp"
    break;

  case 205:
#line 1292 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5882 "ds_parser.cpp"
    break;

  case 206:
#line 1296 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt(scanner,(yylsp[-1]));
        pLet->atInit = tokAt(scanner,(yylsp[0]));
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name_at : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name_at.name) ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.name;
                    pVar->aka = name_at.aka;
                    pVar->at = name_at.at;
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
                    das_yyerror("local variable is already declared " + name_at.name,name_at.at,
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 5919 "ds_parser.cpp"
    break;

  case 207:
#line 1331 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 5925 "ds_parser.cpp"
    break;

  case 208:
#line 1331 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 5931 "ds_parser.cpp"
    break;

  case 209:
#line 1331 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5939 "ds_parser.cpp"
    break;

  case 210:
#line 1334 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 5945 "ds_parser.cpp"
    break;

  case 211:
#line 1334 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 5951 "ds_parser.cpp"
    break;

  case 212:
#line 1334 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5961 "ds_parser.cpp"
    break;

  case 213:
#line 1339 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 5967 "ds_parser.cpp"
    break;

  case 214:
#line 1339 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 5973 "ds_parser.cpp"
    break;

  case 215:
#line 1339 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5983 "ds_parser.cpp"
    break;

  case 216:
#line 1347 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 5989 "ds_parser.cpp"
    break;

  case 217:
#line 1347 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 5995 "ds_parser.cpp"
    break;

  case 218:
#line 1347 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6003 "ds_parser.cpp"
    break;

  case 219:
#line 1353 "ds_parser.ypp"
    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-2].s);
    }
#line 6018 "ds_parser.cpp"
    break;

  case 220:
#line 1363 "ds_parser.ypp"
    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 6034 "ds_parser.cpp"
    break;

  case 221:
#line 1374 "ds_parser.ypp"
    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 6051 "ds_parser.cpp"
    break;

  case 222:
#line 1389 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6059 "ds_parser.cpp"
    break;

  case 223:
#line 1392 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6067 "ds_parser.cpp"
    break;

  case 224:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6073 "ds_parser.cpp"
    break;

  case 225:
#line 1399 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6085 "ds_parser.cpp"
    break;

  case 226:
#line 1406 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6098 "ds_parser.cpp"
    break;

  case 227:
#line 1417 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6104 "ds_parser.cpp"
    break;

  case 228:
#line 1418 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6110 "ds_parser.cpp"
    break;

  case 229:
#line 1419 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6116 "ds_parser.cpp"
    break;

  case 230:
#line 1423 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6122 "ds_parser.cpp"
    break;

  case 231:
#line 1424 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6128 "ds_parser.cpp"
    break;

  case 232:
#line 1425 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6134 "ds_parser.cpp"
    break;

  case 233:
#line 1426 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6140 "ds_parser.cpp"
    break;

  case 234:
#line 1430 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6150 "ds_parser.cpp"
    break;

  case 235:
#line 1435 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6160 "ds_parser.cpp"
    break;

  case 236:
#line 1443 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6166 "ds_parser.cpp"
    break;

  case 237:
#line 1444 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6172 "ds_parser.cpp"
    break;

  case 238:
#line 1448 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6182 "ds_parser.cpp"
    break;

  case 239:
#line 1454 "ds_parser.ypp"
    {
        auto mkb = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-5].i)==1, (yyvsp[-5].i)==2);
        (yyval.pExpression) = mkb;
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name_at.name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.name;
                            pVar->aka = name_at.aka;
                            pVar->at = name_at.at;
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
                            das_yyerror("block argument is already declared " + name_at.name,
                                name_at.at,CompilationError::argument_already_declared);
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
                            das_yyerror("macro [" +pA->annotation->name + "] failed to apply to the block\n" + err, tokAt(scanner,(yylsp[-4])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("blocks are only allowed function macros", tokAt(scanner,(yylsp[-4])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-4].faList) );
            delete (yyvsp[-4].faList);
        }
    }
#line 6248 "ds_parser.cpp"
    break;

  case 240:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6254 "ds_parser.cpp"
    break;

  case 241:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6260 "ds_parser.cpp"
    break;

  case 242:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6266 "ds_parser.cpp"
    break;

  case 243:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6272 "ds_parser.cpp"
    break;

  case 244:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6278 "ds_parser.cpp"
    break;

  case 245:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
#line 6284 "ds_parser.cpp"
    break;

  case 246:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6290 "ds_parser.cpp"
    break;

  case 247:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6296 "ds_parser.cpp"
    break;

  case 248:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6302 "ds_parser.cpp"
    break;

  case 249:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6308 "ds_parser.cpp"
    break;

  case 250:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6314 "ds_parser.cpp"
    break;

  case 251:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6320 "ds_parser.cpp"
    break;

  case 252:
#line 1533 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6326 "ds_parser.cpp"
    break;

  case 253:
#line 1534 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6332 "ds_parser.cpp"
    break;

  case 254:
#line 1535 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6338 "ds_parser.cpp"
    break;

  case 255:
#line 1536 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6344 "ds_parser.cpp"
    break;

  case 256:
#line 1537 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6350 "ds_parser.cpp"
    break;

  case 257:
#line 1538 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6356 "ds_parser.cpp"
    break;

  case 258:
#line 1539 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6362 "ds_parser.cpp"
    break;

  case 259:
#line 1540 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6368 "ds_parser.cpp"
    break;

  case 260:
#line 1541 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6374 "ds_parser.cpp"
    break;

  case 261:
#line 1542 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6380 "ds_parser.cpp"
    break;

  case 262:
#line 1543 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6386 "ds_parser.cpp"
    break;

  case 263:
#line 1544 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6392 "ds_parser.cpp"
    break;

  case 264:
#line 1545 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6398 "ds_parser.cpp"
    break;

  case 265:
#line 1549 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6404 "ds_parser.cpp"
    break;

  case 266:
#line 1550 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6410 "ds_parser.cpp"
    break;

  case 267:
#line 1551 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6416 "ds_parser.cpp"
    break;

  case 268:
#line 1552 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6422 "ds_parser.cpp"
    break;

  case 269:
#line 1556 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6434 "ds_parser.cpp"
    break;

  case 270:
#line 1567 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6444 "ds_parser.cpp"
    break;

  case 271:
#line 1572 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6456 "ds_parser.cpp"
    break;

  case 272:
#line 1582 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6465 "ds_parser.cpp"
    break;

  case 273:
#line 1586 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 6471 "ds_parser.cpp"
    break;

  case 274:
#line 1586 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 6477 "ds_parser.cpp"
    break;

  case 275:
#line 1586 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6489 "ds_parser.cpp"
    break;

  case 276:
#line 1593 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 6495 "ds_parser.cpp"
    break;

  case 277:
#line 1593 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 6501 "ds_parser.cpp"
    break;

  case 278:
#line 1593 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-7])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = expr;
    }
#line 6517 "ds_parser.cpp"
    break;

  case 279:
#line 1607 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6526 "ds_parser.cpp"
    break;

  case 280:
#line 1611 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6535 "ds_parser.cpp"
    break;

  case 281:
#line 1615 "ds_parser.ypp"
    { yyextra->das_supress_errors=true; }
#line 6541 "ds_parser.cpp"
    break;

  case 282:
#line 1615 "ds_parser.ypp"
    { yyextra->das_supress_errors=false; }
#line 6547 "ds_parser.cpp"
    break;

  case 283:
#line 1615 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6556 "ds_parser.cpp"
    break;

  case 284:
#line 1622 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
#line 6562 "ds_parser.cpp"
    break;

  case 285:
#line 1623 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6568 "ds_parser.cpp"
    break;

  case 286:
#line 1624 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6574 "ds_parser.cpp"
    break;

  case 287:
#line 1625 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6580 "ds_parser.cpp"
    break;

  case 288:
#line 1626 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6586 "ds_parser.cpp"
    break;

  case 289:
#line 1627 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6592 "ds_parser.cpp"
    break;

  case 290:
#line 1628 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
#line 6598 "ds_parser.cpp"
    break;

  case 291:
#line 1629 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
#line 6604 "ds_parser.cpp"
    break;

  case 292:
#line 1630 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6610 "ds_parser.cpp"
    break;

  case 293:
#line 1631 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6616 "ds_parser.cpp"
    break;

  case 294:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6622 "ds_parser.cpp"
    break;

  case 295:
#line 1633 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6628 "ds_parser.cpp"
    break;

  case 296:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6634 "ds_parser.cpp"
    break;

  case 297:
#line 1635 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6640 "ds_parser.cpp"
    break;

  case 298:
#line 1636 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6646 "ds_parser.cpp"
    break;

  case 299:
#line 1637 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6652 "ds_parser.cpp"
    break;

  case 300:
#line 1638 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6658 "ds_parser.cpp"
    break;

  case 301:
#line 1639 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6664 "ds_parser.cpp"
    break;

  case 302:
#line 1640 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6670 "ds_parser.cpp"
    break;

  case 303:
#line 1641 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6676 "ds_parser.cpp"
    break;

  case 304:
#line 1642 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6682 "ds_parser.cpp"
    break;

  case 305:
#line 1643 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6688 "ds_parser.cpp"
    break;

  case 306:
#line 1644 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6694 "ds_parser.cpp"
    break;

  case 307:
#line 1645 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6700 "ds_parser.cpp"
    break;

  case 308:
#line 1646 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6706 "ds_parser.cpp"
    break;

  case 309:
#line 1647 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6712 "ds_parser.cpp"
    break;

  case 310:
#line 1648 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6718 "ds_parser.cpp"
    break;

  case 311:
#line 1649 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6724 "ds_parser.cpp"
    break;

  case 312:
#line 1650 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6730 "ds_parser.cpp"
    break;

  case 313:
#line 1651 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6736 "ds_parser.cpp"
    break;

  case 314:
#line 1652 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6742 "ds_parser.cpp"
    break;

  case 315:
#line 1653 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6748 "ds_parser.cpp"
    break;

  case 316:
#line 1654 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6754 "ds_parser.cpp"
    break;

  case 317:
#line 1655 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6760 "ds_parser.cpp"
    break;

  case 318:
#line 1656 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6766 "ds_parser.cpp"
    break;

  case 319:
#line 1657 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6772 "ds_parser.cpp"
    break;

  case 320:
#line 1658 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6778 "ds_parser.cpp"
    break;

  case 321:
#line 1659 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6784 "ds_parser.cpp"
    break;

  case 322:
#line 1660 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6790 "ds_parser.cpp"
    break;

  case 323:
#line 1661 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6796 "ds_parser.cpp"
    break;

  case 324:
#line 1662 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6802 "ds_parser.cpp"
    break;

  case 325:
#line 1663 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6808 "ds_parser.cpp"
    break;

  case 326:
#line 1664 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6814 "ds_parser.cpp"
    break;

  case 327:
#line 1665 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6820 "ds_parser.cpp"
    break;

  case 328:
#line 1666 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6826 "ds_parser.cpp"
    break;

  case 329:
#line 1667 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6832 "ds_parser.cpp"
    break;

  case 330:
#line 1668 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6841 "ds_parser.cpp"
    break;

  case 331:
#line 1672 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6850 "ds_parser.cpp"
    break;

  case 332:
#line 1676 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6858 "ds_parser.cpp"
    break;

  case 333:
#line 1679 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6866 "ds_parser.cpp"
    break;

  case 334:
#line 1682 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6872 "ds_parser.cpp"
    break;

  case 335:
#line 1683 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6878 "ds_parser.cpp"
    break;

  case 336:
#line 1684 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6884 "ds_parser.cpp"
    break;

  case 337:
#line 1685 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt(scanner,(yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6898 "ds_parser.cpp"
    break;

  case 338:
#line 1694 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt(scanner,(yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6912 "ds_parser.cpp"
    break;

  case 339:
#line 1703 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6918 "ds_parser.cpp"
    break;

  case 340:
#line 1704 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6926 "ds_parser.cpp"
    break;

  case 341:
#line 1707 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 6932 "ds_parser.cpp"
    break;

  case 342:
#line 1707 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 6938 "ds_parser.cpp"
    break;

  case 343:
#line 1707 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6946 "ds_parser.cpp"
    break;

  case 344:
#line 1710 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6955 "ds_parser.cpp"
    break;

  case 345:
#line 1714 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6964 "ds_parser.cpp"
    break;

  case 346:
#line 1718 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6973 "ds_parser.cpp"
    break;

  case 347:
#line 1722 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6979 "ds_parser.cpp"
    break;

  case 348:
#line 1723 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6985 "ds_parser.cpp"
    break;

  case 349:
#line 1724 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6991 "ds_parser.cpp"
    break;

  case 350:
#line 1725 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6997 "ds_parser.cpp"
    break;

  case 351:
#line 1726 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7003 "ds_parser.cpp"
    break;

  case 352:
#line 1727 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7009 "ds_parser.cpp"
    break;

  case 353:
#line 1728 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7015 "ds_parser.cpp"
    break;

  case 354:
#line 1729 "ds_parser.ypp"
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
            das_yyerror("can only lpipe into a function call",tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 7036 "ds_parser.cpp"
    break;

  case 355:
#line 1745 "ds_parser.ypp"
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
            das_yyerror("can only lpipe into a function call",tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    }
#line 7057 "ds_parser.cpp"
    break;

  case 356:
#line 1761 "ds_parser.ypp"
    {
        Enumeration * pEnum = nullptr;
        Expression * resConst = nullptr;
        auto enums = g_Program->findEnum(*(yyvsp[-1].s));
        auto aliases = g_Program->findAlias(*(yyvsp[-1].s));
        if ( enums.size()+aliases.size() > 1 ) {
            string candidates;
            if ( enums.size() ) candidates += g_Program->describeCandidates(enums);
            if ( aliases.size() ) candidates += g_Program->describeCandidates(aliases);
            das_yyerror("too many options for the " + *(yyvsp[-1].s) + "\n" + candidates, tokAt(scanner,(yylsp[-1])),
                CompilationError::type_not_found);
        } else if ( enums.size()==0 && aliases.size()==0 ) {
            das_yyerror("enumeration or bitfield not found " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
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
                    auto bitConst = new ExprConstBitfield(tokAt(scanner,(yylsp[0])), 1u << bit);
                    bitConst->bitfieldType = make_smart<TypeDecl>(*alias);
                    resConst = bitConst;
                } else {
                    das_yyerror("enumeration or bitfield not found " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                        CompilationError::bitfield_not_found);
                }
            } else {
                das_yyerror("expecting enumeration or bitfield " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                    CompilationError::syntax_error);
            }
        }
        if ( pEnum ) {
            auto ff = pEnum->find(*(yyvsp[0].s));
            if ( ff.second ) {
                auto td = make_smart<TypeDecl>(pEnum);
                resConst = new ExprConstEnumeration(tokAt(scanner,(yylsp[0])), *(yyvsp[0].s), td);
            } else {
                das_yyerror("enumeraiton value not found " + *(yyvsp[-1].s) + " " + *(yyvsp[0].s), tokAt(scanner,(yylsp[0])),
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
#line 7117 "ds_parser.cpp"
    break;

  case 357:
#line 1816 "ds_parser.ypp"
    {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 7127 "ds_parser.cpp"
    break;

  case 358:
#line 1824 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7133 "ds_parser.cpp"
    break;

  case 359:
#line 1825 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7139 "ds_parser.cpp"
    break;

  case 360:
#line 1829 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_NONE; }
#line 7145 "ds_parser.cpp"
    break;

  case 361:
#line 1830 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_OVERRIDE; }
#line 7151 "ds_parser.cpp"
    break;

  case 362:
#line 1831 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_SEALED; }
#line 7157 "ds_parser.cpp"
    break;

  case 363:
#line 1835 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7163 "ds_parser.cpp"
    break;

  case 364:
#line 1836 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7169 "ds_parser.cpp"
    break;

  case 365:
#line 1840 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7175 "ds_parser.cpp"
    break;

  case 366:
#line 1841 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7181 "ds_parser.cpp"
    break;

  case 367:
#line 1842 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7187 "ds_parser.cpp"
    break;

  case 368:
#line 1846 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7199 "ds_parser.cpp"
    break;

  case 369:
#line 1856 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7207 "ds_parser.cpp"
    break;

  case 370:
#line 1859 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7216 "ds_parser.cpp"
    break;

  case 371:
#line 1864 "ds_parser.ypp"
    { yyextra->das_force_oxford_comma=true; }
#line 7222 "ds_parser.cpp"
    break;

  case 372:
#line 1864 "ds_parser.ypp"
    {
        if ( !yyextra->g_thisStructure->isClass ) {
            das_yyerror("structure can't have a member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->name==yyextra->g_thisStructure->name || (yyvsp[-1].pFuncDecl)->name=="finalize" ) {
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
            (yyvsp[-1].pFuncDecl)->name = yyextra->g_thisStructure->name + "`" + (yyvsp[-1].pFuncDecl)->name;
            auto vars = new vector<VariableNameAndPosition>();
            vars->emplace_back(VariableNameAndPosition{varName,"",(yyvsp[-1].pFuncDecl)->at});
            TypeDecl * funcType = new TypeDecl(Type::tFunction);
            funcType->at = (yyvsp[-1].pFuncDecl)->at;
            swap ( funcType->firstType, (yyvsp[-1].pFuncDecl)->result );
            funcType->argTypes.reserve ( (yyvsp[-1].pFuncDecl)->arguments.size() );
            if ( yyextra->g_thisStructure->isClass ) {
                funcType->argTypes.push_back(make_smart<TypeDecl>(yyextra->g_thisStructure));
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
#line 7272 "ds_parser.cpp"
    break;

  case 373:
#line 1911 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        (yyvsp[-1].pFuncDecl)->body = (yyvsp[0].pExpression);
        if ( !yyextra->g_thisStructure ) {
            das_yyerror("internal error or invalid macro. member function is declared outside of a class",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( !yyextra->g_thisStructure->isClass ) {
            das_yyerror("structure can't have a member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            (yyvsp[-1].pFuncDecl)->privateFunction = yyextra->g_thisStructure->privateStructure;
            if ( (yyvsp[-1].pFuncDecl)->name != yyextra->g_thisStructure->name && (yyvsp[-1].pFuncDecl)->name != "finalize") {
                auto varName = (yyvsp[-1].pFuncDecl)->name;
                (yyvsp[-1].pFuncDecl)->name = yyextra->g_thisStructure->name + "`" + (yyvsp[-1].pFuncDecl)->name;
                auto vars = new vector<VariableNameAndPosition>();
                vars->emplace_back(VariableNameAndPosition{varName,"",(yyvsp[-1].pFuncDecl)->at});
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
                modifyToClassMember((yyvsp[-1].pFuncDecl), yyextra->g_thisStructure, false, (yyvsp[-2].b));
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
                    auto ctr = makeClassConstructor(yyextra->g_thisStructure, (yyvsp[-1].pFuncDecl));
                    if ( !g_Program->addFunction(ctr) ) {
                        das_yyerror("intializer is already defined " + ctr->getMangledName(),
                            ctr->at, CompilationError::function_already_declared);
                    }
                    (yyvsp[-1].pFuncDecl)->name = yyextra->g_thisStructure->name + "`" + yyextra->g_thisStructure->name;
                    modifyToClassMember((yyvsp[-1].pFuncDecl), yyextra->g_thisStructure, false, false);
                } else {
                    modifyToClassMember((yyvsp[-1].pFuncDecl), yyextra->g_thisStructure, true, false);
                }
            }
            assignDefaultArguments((yyvsp[-1].pFuncDecl));
            runFunctionAnnotations((yyvsp[-1].pFuncDecl), (yyvsp[-6].faList), tokAt(scanner,(yylsp[-6])));
            if ( !g_Program->addFunction((yyvsp[-1].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                    (yyvsp[-1].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[-1].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-7].pVarDeclList);
    }
#line 7341 "ds_parser.cpp"
    break;

  case 374:
#line 1975 "ds_parser.ypp"
    {
        das_yyerror("structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7352 "ds_parser.cpp"
    break;

  case 375:
#line 1984 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7362 "ds_parser.cpp"
    break;

  case 376:
#line 1989 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7372 "ds_parser.cpp"
    break;

  case 377:
#line 1997 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7378 "ds_parser.cpp"
    break;

  case 378:
#line 1998 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7384 "ds_parser.cpp"
    break;

  case 379:
#line 2002 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7392 "ds_parser.cpp"
    break;

  case 380:
#line 2005 "ds_parser.ypp"
    {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7403 "ds_parser.cpp"
    break;

  case 381:
#line 2014 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7409 "ds_parser.cpp"
    break;

  case 382:
#line 2015 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7415 "ds_parser.cpp"
    break;

  case 383:
#line 2019 "ds_parser.ypp"
    {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7426 "ds_parser.cpp"
    break;

  case 384:
#line 2028 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7432 "ds_parser.cpp"
    break;

  case 385:
#line 2029 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7438 "ds_parser.cpp"
    break;

  case 386:
#line 2034 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7444 "ds_parser.cpp"
    break;

  case 387:
#line 2035 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7450 "ds_parser.cpp"
    break;

  case 388:
#line 2039 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7461 "ds_parser.cpp"
    break;

  case 389:
#line 2045 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7472 "ds_parser.cpp"
    break;

  case 390:
#line 2051 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7480 "ds_parser.cpp"
    break;

  case 391:
#line 2054 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7489 "ds_parser.cpp"
    break;

  case 392:
#line 2058 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7500 "ds_parser.cpp"
    break;

  case 393:
#line 2064 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7511 "ds_parser.cpp"
    break;

  case 394:
#line 2073 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7517 "ds_parser.cpp"
    break;

  case 395:
#line 2074 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7523 "ds_parser.cpp"
    break;

  case 396:
#line 2075 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7529 "ds_parser.cpp"
    break;

  case 397:
#line 2079 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7535 "ds_parser.cpp"
    break;

  case 398:
#line 2080 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7541 "ds_parser.cpp"
    break;

  case 399:
#line 2084 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7549 "ds_parser.cpp"
    break;

  case 400:
#line 2087 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7559 "ds_parser.cpp"
    break;

  case 401:
#line 2092 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7569 "ds_parser.cpp"
    break;

  case 402:
#line 2097 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7582 "ds_parser.cpp"
    break;

  case 403:
#line 2105 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7595 "ds_parser.cpp"
    break;

  case 404:
#line 2116 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7603 "ds_parser.cpp"
    break;

  case 405:
#line 2119 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7613 "ds_parser.cpp"
    break;

  case 406:
#line 2127 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7619 "ds_parser.cpp"
    break;

  case 407:
#line 2128 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7625 "ds_parser.cpp"
    break;

  case 408:
#line 2132 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7631 "ds_parser.cpp"
    break;

  case 409:
#line 2133 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7637 "ds_parser.cpp"
    break;

  case 410:
#line 2134 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7643 "ds_parser.cpp"
    break;

  case 411:
#line 2138 "ds_parser.ypp"
    {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.name;
                    pVar->aka = name_at.aka;
                    if ( !name_at.aka.empty() ) {
                        das_yyerror("global variable " + name_at.name + " can't have an aka",name_at.at,
                            CompilationError::invalid_aka);
                    }
                    pVar->at = name_at.at;
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
                        das_yyerror("global variable is already declared " + name_at.name,name_at.at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7681 "ds_parser.cpp"
    break;

  case 412:
#line 2171 "ds_parser.ypp"
    { yyextra->das_force_oxford_comma=true; }
#line 7687 "ds_parser.cpp"
    break;

  case 413:
#line 2171 "ds_parser.ypp"
    {
        auto pDecl = (yyvsp[0].pVarDecl);
        if ( pDecl->pTypeDecl ) {
            for ( const auto & name_at : *pDecl->pNameList ) {
                VariablePtr pVar = make_smart<Variable>();
                pVar->name = name_at.name;
                pVar->aka = name_at.aka;
                if ( !name_at.aka.empty() ) {
                    das_yyerror("global variable " + name_at.name + " can't have an aka",name_at.at,
                        CompilationError::invalid_aka);
                }
                pVar->at = name_at.at;
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
                    das_yyerror("global variable is already declared " + name_at.name,name_at.at,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }
#line 7728 "ds_parser.cpp"
    break;

  case 414:
#line 2210 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7736 "ds_parser.cpp"
    break;

  case 415:
#line 2213 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7750 "ds_parser.cpp"
    break;

  case 416:
#line 2222 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7764 "ds_parser.cpp"
    break;

  case 417:
#line 2235 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7778 "ds_parser.cpp"
    break;

  case 422:
#line 2257 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7784 "ds_parser.cpp"
    break;

  case 423:
#line 2258 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7790 "ds_parser.cpp"
    break;

  case 424:
#line 2259 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7796 "ds_parser.cpp"
    break;

  case 425:
#line 2263 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt(scanner,(yylsp[-3]));
        pEnum->name = *(yyvsp[-3].s);
        pEnum->isPrivate = !(yyvsp[-4].b);
        if ( (yyvsp[-6].faList) ) {
            for ( auto pA : *(yyvsp[-6].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isEnumerationAnnotation() ) {
                        auto ann = static_pointer_cast<EnumerationAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->touch(pEnum, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("macro [" +pA->annotation->name + "] failed to finalize the enumeration " + pEnum->name + "\n" + err, tokAt(scanner,(yylsp[-6])),
                                CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *(yyvsp[-6].faList) );
            delete (yyvsp[-6].faList);
        }
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7829 "ds_parser.cpp"
    break;

  case 426:
#line 2291 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt(scanner,(yylsp[-5]));
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
                            das_yyerror("macro [" +pA->annotation->name + "] failed to finalize the enumeration " +pEnum->name + "\n" + err, tokAt(scanner,(yylsp[-8])),
                                CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *(yyvsp[-8].faList) );
            delete (yyvsp[-8].faList);
        }
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-5].s);
    }
#line 7863 "ds_parser.cpp"
    break;

  case 427:
#line 2323 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7869 "ds_parser.cpp"
    break;

  case 428:
#line 2324 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7875 "ds_parser.cpp"
    break;

  case 429:
#line 2328 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7881 "ds_parser.cpp"
    break;

  case 430:
#line 2329 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7887 "ds_parser.cpp"
    break;

  case 431:
#line 2333 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
                pStruct->parent = structs.back().get();
                if ( pStruct->parent->sealed ) {
                    das_yyerror("can't derive from a sealed class or structure "+*(yyvsp[0].s),tokAt(scanner,(yylsp[0])),
                        CompilationError::invalid_override);
                }
                pStruct->annotations.clear();
                pStruct->genCtor = false;
                pStruct->macroInterface = pStruct->parent && pStruct->parent->macroInterface;

            } else if ( structs.size()==0 ) {
                das_yyerror("parent structure not found "+*(yyvsp[0].s),tokAt(scanner,(yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = g_Program->describeCandidates(structs);
                das_yyerror("too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt(scanner,(yylsp[0])),
                    CompilationError::structure_not_found);
            }
            delete (yyvsp[0].s);
        }
        if ( !pStruct ) {
            pStruct = make_smart<Structure>(*(yyvsp[-1].s));
        }
        pStruct->sealed = (yyvsp[-2].b);
        if ( !g_Program->addStructure(pStruct) ) {
            das_yyerror("structure is already defined "+*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::structure_already_declared);
            (yyval.pStructure) = nullptr;
        } else {
            (yyval.pStructure) = pStruct.get();
            yyextra->g_thisStructure = pStruct.get();
        }
        delete (yyvsp[-1].s);
    }
#line 7933 "ds_parser.cpp"
    break;

  case 432:
#line 2377 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7939 "ds_parser.cpp"
    break;

  case 433:
#line 2378 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7945 "ds_parser.cpp"
    break;

  case 434:
#line 2382 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7951 "ds_parser.cpp"
    break;

  case 435:
#line 2383 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7957 "ds_parser.cpp"
    break;

  case 436:
#line 2384 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7963 "ds_parser.cpp"
    break;

  case 437:
#line 2389 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7969 "ds_parser.cpp"
    break;

  case 438:
#line 2389 "ds_parser.ypp"
    {
        if ( (yyvsp[-4].pStructure) ) {
            auto pStruct = (yyvsp[-4].pStructure);
            pStruct->at = tokAt(scanner,(yylsp[-4]));
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
                        das_yyerror("only class member can be private "+name_at.name,name_at.at,
                            CompilationError::invalid_private);
                    }
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name_at.name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything "+name_at.name,name_at.at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name_at.name, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, name_at.at);
                            pStruct->fields.back().privateField = pDecl->isPrivate;
                            pStruct->fields.back().sealed = pDecl->sealed;
                        }
                    } else {
                        if ( pDecl->sealed || pDecl->override ) {
                            if ( oldFd->sealed ) {
                                das_yyerror("structure field "+name_at.name+" is sealed",
                                    name_at.at, CompilationError::invalid_override);
                            }
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            oldFd->init = init;
                            oldFd->parentType = oldFd->type->isAuto();
                            oldFd->privateField = pDecl->isPrivate;
                            oldFd->sealed = pDecl->sealed;
                        } else {
                            das_yyerror("structure field is already declared "+name_at.name
                                +", use override to replace initial value instead",name_at.at,
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
                                    tokAt(scanner,(yylsp[-6])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-7].faList)->size()!=1 ) {
                                das_yyerror("structures are only allowed one structure macro", tokAt(scanner,(yylsp[-6])),
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror("handled structure is already defined "+pStruct->name,tokAt(scanner,(yylsp[-6])),
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
        yyextra->g_thisStructure = nullptr;
    }
#line 8067 "ds_parser.cpp"
    break;

  case 439:
#line 2485 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8079 "ds_parser.cpp"
    break;

  case 440:
#line 2492 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
#line 8093 "ds_parser.cpp"
    break;

  case 441:
#line 2501 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 8104 "ds_parser.cpp"
    break;

  case 442:
#line 2507 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
#line 8117 "ds_parser.cpp"
    break;

  case 443:
#line 2518 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 8123 "ds_parser.cpp"
    break;

  case 444:
#line 2519 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 8129 "ds_parser.cpp"
    break;

  case 445:
#line 2520 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8135 "ds_parser.cpp"
    break;

  case 446:
#line 2521 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8141 "ds_parser.cpp"
    break;

  case 447:
#line 2522 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8147 "ds_parser.cpp"
    break;

  case 448:
#line 2523 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 8153 "ds_parser.cpp"
    break;

  case 449:
#line 2524 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 8159 "ds_parser.cpp"
    break;

  case 450:
#line 2525 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 8165 "ds_parser.cpp"
    break;

  case 451:
#line 2526 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8171 "ds_parser.cpp"
    break;

  case 452:
#line 2527 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8177 "ds_parser.cpp"
    break;

  case 453:
#line 2528 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8183 "ds_parser.cpp"
    break;

  case 454:
#line 2529 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8189 "ds_parser.cpp"
    break;

  case 455:
#line 2530 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8195 "ds_parser.cpp"
    break;

  case 456:
#line 2531 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8201 "ds_parser.cpp"
    break;

  case 457:
#line 2532 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8207 "ds_parser.cpp"
    break;

  case 458:
#line 2533 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8213 "ds_parser.cpp"
    break;

  case 459:
#line 2534 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8219 "ds_parser.cpp"
    break;

  case 460:
#line 2535 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8225 "ds_parser.cpp"
    break;

  case 461:
#line 2536 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8231 "ds_parser.cpp"
    break;

  case 462:
#line 2537 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8237 "ds_parser.cpp"
    break;

  case 463:
#line 2538 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8243 "ds_parser.cpp"
    break;

  case 464:
#line 2539 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8249 "ds_parser.cpp"
    break;

  case 465:
#line 2540 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8255 "ds_parser.cpp"
    break;

  case 466:
#line 2541 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8261 "ds_parser.cpp"
    break;

  case 467:
#line 2542 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8267 "ds_parser.cpp"
    break;

  case 468:
#line 2546 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8273 "ds_parser.cpp"
    break;

  case 469:
#line 2547 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8279 "ds_parser.cpp"
    break;

  case 470:
#line 2548 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8285 "ds_parser.cpp"
    break;

  case 471:
#line 2549 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8291 "ds_parser.cpp"
    break;

  case 472:
#line 2550 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8297 "ds_parser.cpp"
    break;

  case 473:
#line 2551 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8303 "ds_parser.cpp"
    break;

  case 474:
#line 2555 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8316 "ds_parser.cpp"
    break;

  case 475:
#line 2566 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8325 "ds_parser.cpp"
    break;

  case 476:
#line 2570 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8337 "ds_parser.cpp"
    break;

  case 477:
#line 2580 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8349 "ds_parser.cpp"
    break;

  case 478:
#line 2587 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8360 "ds_parser.cpp"
    break;

  case 479:
#line 2596 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8366 "ds_parser.cpp"
    break;

  case 480:
#line 2596 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8372 "ds_parser.cpp"
    break;

  case 481:
#line 2596 "ds_parser.ypp"
    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror("only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
#line 8387 "ds_parser.cpp"
    break;

  case 482:
#line 2609 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
#line 8393 "ds_parser.cpp"
    break;

  case 483:
#line 2610 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8399 "ds_parser.cpp"
    break;

  case 484:
#line 2611 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8405 "ds_parser.cpp"
    break;

  case 485:
#line 2612 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8411 "ds_parser.cpp"
    break;

  case 486:
#line 2613 "ds_parser.ypp"
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
#line 8430 "ds_parser.cpp"
    break;

  case 487:
#line 2627 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8441 "ds_parser.cpp"
    break;

  case 488:
#line 2633 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8450 "ds_parser.cpp"
    break;

  case 489:
#line 2637 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8459 "ds_parser.cpp"
    break;

  case 490:
#line 2641 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8469 "ds_parser.cpp"
    break;

  case 491:
#line 2646 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8479 "ds_parser.cpp"
    break;

  case 492:
#line 2651 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8489 "ds_parser.cpp"
    break;

  case 493:
#line 2656 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8499 "ds_parser.cpp"
    break;

  case 494:
#line 2661 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8508 "ds_parser.cpp"
    break;

  case 495:
#line 2665 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8517 "ds_parser.cpp"
    break;

  case 496:
#line 2669 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8527 "ds_parser.cpp"
    break;

  case 497:
#line 2674 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8536 "ds_parser.cpp"
    break;

  case 498:
#line 2678 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8546 "ds_parser.cpp"
    break;

  case 499:
#line 2683 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8552 "ds_parser.cpp"
    break;

  case 500:
#line 2683 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8558 "ds_parser.cpp"
    break;

  case 501:
#line 2683 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8569 "ds_parser.cpp"
    break;

  case 502:
#line 2689 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8581 "ds_parser.cpp"
    break;

  case 503:
#line 2696 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8587 "ds_parser.cpp"
    break;

  case 504:
#line 2696 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8593 "ds_parser.cpp"
    break;

  case 505:
#line 2696 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8603 "ds_parser.cpp"
    break;

  case 506:
#line 2701 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8609 "ds_parser.cpp"
    break;

  case 507:
#line 2701 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8615 "ds_parser.cpp"
    break;

  case 508:
#line 2701 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8626 "ds_parser.cpp"
    break;

  case 509:
#line 2707 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8632 "ds_parser.cpp"
    break;

  case 510:
#line 2707 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8638 "ds_parser.cpp"
    break;

  case 511:
#line 2707 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8648 "ds_parser.cpp"
    break;

  case 512:
#line 2712 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8657 "ds_parser.cpp"
    break;

  case 513:
#line 2716 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8663 "ds_parser.cpp"
    break;

  case 514:
#line 2716 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8669 "ds_parser.cpp"
    break;

  case 515:
#line 2716 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8679 "ds_parser.cpp"
    break;

  case 516:
#line 2721 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8685 "ds_parser.cpp"
    break;

  case 517:
#line 2721 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8691 "ds_parser.cpp"
    break;

  case 518:
#line 2721 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8705 "ds_parser.cpp"
    break;

  case 519:
#line 2730 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8714 "ds_parser.cpp"
    break;

  case 520:
#line 2734 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8720 "ds_parser.cpp"
    break;

  case 521:
#line 2734 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8726 "ds_parser.cpp"
    break;

  case 522:
#line 2734 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8736 "ds_parser.cpp"
    break;

  case 523:
#line 2739 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8742 "ds_parser.cpp"
    break;

  case 524:
#line 2739 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8748 "ds_parser.cpp"
    break;

  case 525:
#line 2739 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8762 "ds_parser.cpp"
    break;

  case 526:
#line 2748 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8771 "ds_parser.cpp"
    break;

  case 527:
#line 2752 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8777 "ds_parser.cpp"
    break;

  case 528:
#line 2752 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8783 "ds_parser.cpp"
    break;

  case 529:
#line 2752 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8793 "ds_parser.cpp"
    break;

  case 530:
#line 2757 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8799 "ds_parser.cpp"
    break;

  case 531:
#line 2757 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8805 "ds_parser.cpp"
    break;

  case 532:
#line 2757 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8819 "ds_parser.cpp"
    break;

  case 533:
#line 2766 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8825 "ds_parser.cpp"
    break;

  case 534:
#line 2766 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8831 "ds_parser.cpp"
    break;

  case 535:
#line 2766 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8842 "ds_parser.cpp"
    break;

  case 536:
#line 2772 "ds_parser.ypp"
    { yyextra->das_arrow_depth ++; }
#line 8848 "ds_parser.cpp"
    break;

  case 537:
#line 2772 "ds_parser.ypp"
    { yyextra->das_arrow_depth --; }
#line 8854 "ds_parser.cpp"
    break;

  case 538:
#line 2772 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8865 "ds_parser.cpp"
    break;

  case 539:
#line 2781 "ds_parser.ypp"
    { yyextra->das_need_oxford_comma=false; }
#line 8871 "ds_parser.cpp"
    break;

  case 540:
#line 2781 "ds_parser.ypp"
    {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-4].s);
        vtype->at = tokAt(scanner,(yylsp[-4]));
        varDeclToTypeDecl(vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !g_Program->addAlias(vtype) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
    }
#line 8888 "ds_parser.cpp"
    break;

  case 541:
#line 2796 "ds_parser.ypp"
    { yyextra->das_need_oxford_comma=false; }
#line 8894 "ds_parser.cpp"
    break;

  case 542:
#line 2796 "ds_parser.ypp"
    {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt(scanner,(yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror("only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !g_Program->addAlias(btype) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
#line 8915 "ds_parser.cpp"
    break;

  case 543:
#line 2816 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8921 "ds_parser.cpp"
    break;

  case 544:
#line 2817 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8927 "ds_parser.cpp"
    break;

  case 545:
#line 2818 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8933 "ds_parser.cpp"
    break;

  case 546:
#line 2819 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8939 "ds_parser.cpp"
    break;

  case 547:
#line 2823 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8951 "ds_parser.cpp"
    break;

  case 548:
#line 2830 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8963 "ds_parser.cpp"
    break;

  case 549:
#line 2837 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8974 "ds_parser.cpp"
    break;

  case 550:
#line 2843 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8985 "ds_parser.cpp"
    break;

  case 551:
#line 2852 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8995 "ds_parser.cpp"
    break;

  case 552:
#line 2857 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9004 "ds_parser.cpp"
    break;

  case 553:
#line 2864 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9010 "ds_parser.cpp"
    break;

  case 554:
#line 2865 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9016 "ds_parser.cpp"
    break;

  case 555:
#line 2869 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 9027 "ds_parser.cpp"
    break;

  case 556:
#line 2875 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 9039 "ds_parser.cpp"
    break;

  case 557:
#line 2882 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 9052 "ds_parser.cpp"
    break;

  case 558:
#line 2890 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 9064 "ds_parser.cpp"
    break;

  case 559:
#line 2897 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9077 "ds_parser.cpp"
    break;

  case 560:
#line 2905 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9091 "ds_parser.cpp"
    break;

  case 561:
#line 2917 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 9099 "ds_parser.cpp"
    break;

  case 562:
#line 2920 "ds_parser.ypp"
    {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9115 "ds_parser.cpp"
    break;

  case 563:
#line 2934 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9126 "ds_parser.cpp"
    break;

  case 564:
#line 2943 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9132 "ds_parser.cpp"
    break;

  case 565:
#line 2944 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9138 "ds_parser.cpp"
    break;

  case 566:
#line 2948 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9148 "ds_parser.cpp"
    break;

  case 567:
#line 2953 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9157 "ds_parser.cpp"
    break;

  case 568:
#line 2960 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9167 "ds_parser.cpp"
    break;

  case 569:
#line 2965 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9179 "ds_parser.cpp"
    break;

  case 570:
#line 2975 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9189 "ds_parser.cpp"
    break;

  case 571:
#line 2980 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9198 "ds_parser.cpp"
    break;

  case 572:
#line 2987 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9212 "ds_parser.cpp"
    break;

  case 573:
#line 2999 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9218 "ds_parser.cpp"
    break;

  case 574:
#line 3000 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9224 "ds_parser.cpp"
    break;

  case 575:
#line 3004 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt(scanner,(yylsp[-8])));
        pFor->visibility = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        for ( const auto & np : *(yyvsp[-7].pNameWithPosList) ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            if ( !np.aka.empty() ) {
                das_yyerror("array comprehension can't have an aka",np.at,
                    CompilationError::invalid_aka);
            }
            pFor->iteratorsAt.push_back(np.at);
        }
        delete (yyvsp[-7].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt(scanner,(yylsp[-8])));
        pAC->generatorSyntax = true;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 9252 "ds_parser.cpp"
    break;

  case 576:
#line 3027 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt(scanner,(yylsp[-8])));
        pFor->visibility = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        for ( const auto & np : *(yyvsp[-7].pNameWithPosList) ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            if ( !np.aka.empty() ) {
                das_yyerror("array comprehension can't have an aka",np.at,
                    CompilationError::invalid_aka);
            }
            pFor->iteratorsAt.push_back(np.at);
        }
        delete (yyvsp[-7].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt(scanner,(yylsp[-8])));
        pAC->generatorSyntax = false;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 9280 "ds_parser.cpp"
    break;


#line 9284 "ds_parser.cpp"

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
      yyerror (&yylloc, scanner, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner);
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
                  yystos[yystate], yyvsp, yylsp, scanner);
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
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner);
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
#line 3052 "ds_parser.ypp"


void das_checkName(const string & name, const LineInfo &at) {
    if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
        g_Program->error("names starting with __ are reserved, " + name,"","",at,CompilationError::invalid_name);
    }
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,"","",at,cerr);
}

void das_yyfatalerror(DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr) {
    g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
        lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),cerr);
}

void das_yyerror(DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error) {
    if ( !yyextra->das_supress_errors ) {
        g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
            lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),
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

LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
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
                        (*pDecl->pNameList)[ai].at,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
                if ( needNames && pDecl->pNameList && !(*pDecl->pNameList)[ai].name.empty() ) {
                    if ( !(*pDecl->pNameList)[ai].aka.empty() ) {
                        das_yyerror("type declaration can't have an aka", (*pDecl->pNameList)[ai].at,
                            CompilationError::invalid_aka);
                    }
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
                        pType->argNames.push_back(name.name);
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
