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
#line 87 "ds_parser.ypp"

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

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

    extern Structure * g_thisStructure;

#line 223 "ds_parser.cpp"

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
    DAS_PRIVATE = 315,
    DAS_SMART_PTR = 316,
    DAS_UNSAFE = 317,
    DAS_TBOOL = 318,
    DAS_TVOID = 319,
    DAS_TSTRING = 320,
    DAS_TAUTO = 321,
    DAS_TINT = 322,
    DAS_TINT2 = 323,
    DAS_TINT3 = 324,
    DAS_TINT4 = 325,
    DAS_TUINT = 326,
    DAS_TBITFIELD = 327,
    DAS_TUINT2 = 328,
    DAS_TUINT3 = 329,
    DAS_TUINT4 = 330,
    DAS_TFLOAT = 331,
    DAS_TFLOAT2 = 332,
    DAS_TFLOAT3 = 333,
    DAS_TFLOAT4 = 334,
    DAS_TRANGE = 335,
    DAS_TURANGE = 336,
    DAS_TBLOCK = 337,
    DAS_TINT64 = 338,
    DAS_TUINT64 = 339,
    DAS_TDOUBLE = 340,
    DAS_TFUNCTION = 341,
    DAS_TLAMBDA = 342,
    DAS_TINT8 = 343,
    DAS_TUINT8 = 344,
    DAS_TINT16 = 345,
    DAS_TUINT16 = 346,
    DAS_TTUPLE = 347,
    DAS_TVARIANT = 348,
    DAS_GENERATOR = 349,
    DAS_YIELD = 350,
    DAS_SEALED = 351,
    ADDEQU = 352,
    SUBEQU = 353,
    DIVEQU = 354,
    MULEQU = 355,
    MODEQU = 356,
    ANDEQU = 357,
    OREQU = 358,
    XOREQU = 359,
    SHL = 360,
    SHR = 361,
    ADDADD = 362,
    SUBSUB = 363,
    LEEQU = 364,
    SHLEQU = 365,
    SHREQU = 366,
    GREQU = 367,
    EQUEQU = 368,
    NOTEQU = 369,
    RARROW = 370,
    LARROW = 371,
    QQ = 372,
    QDOT = 373,
    QBRA = 374,
    LPIPE = 375,
    LBPIPE = 376,
    LAPIPE = 377,
    LFPIPE = 378,
    RPIPE = 379,
    CLONEEQU = 380,
    ROTL = 381,
    ROTR = 382,
    ROTLEQU = 383,
    ROTREQU = 384,
    MAPTO = 385,
    COLCOL = 386,
    ANDAND = 387,
    OROR = 388,
    XORXOR = 389,
    ANDANDEQU = 390,
    OROREQU = 391,
    XORXOREQU = 392,
    BRABRAB = 393,
    BRACBRB = 394,
    CBRCBRB = 395,
    INTEGER = 396,
    LONG_INTEGER = 397,
    UNSIGNED_INTEGER = 398,
    UNSIGNED_LONG_INTEGER = 399,
    FLOAT = 400,
    DOUBLE = 401,
    NAME = 402,
    BEGIN_STRING = 403,
    STRING_CHARACTER = 404,
    STRING_CHARACTER_ESC = 405,
    END_STRING = 406,
    BEGIN_STRING_EXPR = 407,
    END_STRING_EXPR = 408,
    END_OF_READ = 409,
    UNARY_MINUS = 410,
    UNARY_PLUS = 411,
    PRE_INC = 412,
    PRE_DEC = 413,
    POST_INC = 414,
    POST_DEC = 415,
    DEREF = 416
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 119 "ds_parser.ypp"

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

#line 425 "ds_parser.cpp"

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
#define YYLAST   7622

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  189
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  182
/* YYNRULES -- Number of rules.  */
#define YYNRULES  573
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1001

#define YYUNDEFTOK  2
#define YYMAXUTOK   416

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
       2,     2,     2,   172,     2,   188,   183,   168,   161,     2,
     181,   182,   166,   165,   155,   164,   178,   167,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   158,   185,
     162,   156,   163,   157,   184,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   179,     2,   180,   160,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   186,   159,   187,   171,     2,     2,     2,
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
     169,   170,   173,   174,   175,   176,   177
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   483,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   499,   500,   501,   505,   506,
     510,   527,   528,   529,   530,   534,   538,   543,   552,   560,
     576,   581,   589,   589,   619,   641,   645,   648,   652,   658,
     667,   690,   715,   716,   720,   724,   725,   729,   732,   738,
     744,   747,   753,   754,   758,   759,   760,   769,   770,   774,
     783,   800,   808,   818,   827,   828,   829,   830,   831,   832,
     836,   841,   849,   850,   851,   855,   856,   857,   858,   859,
     860,   861,   862,   868,   882,   899,   900,   901,   905,   913,
     926,   930,   937,   938,   942,   943,   944,   948,   951,   958,
     962,   963,   964,   965,   966,   967,   968,   969,   970,   971,
     972,   973,   974,   975,   976,   977,   978,   979,   980,   981,
     982,   983,   984,   985,   986,   987,   988,   989,   990,   991,
     992,   993,   994,   995,   996,   997,   998,   999,  1000,  1001,
    1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1013,  1035,
    1036,  1037,  1041,  1080,  1088,  1092,  1103,  1104,  1105,  1106,
    1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,
    1117,  1118,  1119,  1120,  1121,  1125,  1130,  1136,  1142,  1176,
    1179,  1185,  1186,  1197,  1201,  1207,  1210,  1213,  1217,  1223,
    1227,  1231,  1234,  1237,  1242,  1245,  1253,  1256,  1261,  1264,
    1272,  1278,  1279,  1283,  1318,  1318,  1318,  1321,  1321,  1321,
    1326,  1326,  1326,  1334,  1334,  1334,  1340,  1350,  1361,  1376,
    1379,  1385,  1386,  1393,  1404,  1405,  1406,  1410,  1411,  1412,
    1413,  1417,  1422,  1430,  1431,  1435,  1440,  1505,  1506,  1507,
    1508,  1509,  1510,  1514,  1515,  1516,  1517,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1532,  1536,  1537,  1538,  1539,  1543,  1554,  1559,  1569,
    1573,  1573,  1573,  1580,  1580,  1580,  1594,  1598,  1602,  1602,
    1602,  1609,  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,
    1618,  1619,  1620,  1621,  1622,  1623,  1624,  1625,  1626,  1627,
    1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,  1637,
    1638,  1639,  1640,  1641,  1642,  1643,  1644,  1645,  1646,  1647,
    1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,  1659,  1663,
    1666,  1669,  1670,  1671,  1672,  1681,  1690,  1691,  1694,  1694,
    1694,  1697,  1701,  1705,  1709,  1710,  1711,  1712,  1713,  1714,
    1715,  1716,  1732,  1748,  1803,  1811,  1812,  1816,  1817,  1818,
    1822,  1823,  1827,  1828,  1829,  1833,  1843,  1846,  1851,  1850,
    1896,  1962,  1971,  1976,  1984,  1985,  1989,  1992,  2001,  2002,
    2006,  2015,  2016,  2021,  2022,  2026,  2032,  2038,  2041,  2045,
    2051,  2060,  2061,  2062,  2066,  2067,  2071,  2074,  2079,  2084,
    2092,  2103,  2106,  2114,  2115,  2119,  2120,  2121,  2125,  2158,
    2158,  2197,  2200,  2209,  2222,  2234,  2235,  2239,  2240,  2244,
    2245,  2246,  2250,  2278,  2310,  2311,  2315,  2316,  2320,  2364,
    2365,  2369,  2370,  2371,  2376,  2375,  2472,  2479,  2488,  2494,
    2505,  2506,  2507,  2508,  2509,  2510,  2511,  2512,  2513,  2514,
    2515,  2516,  2517,  2518,  2519,  2520,  2521,  2522,  2523,  2524,
    2525,  2526,  2527,  2528,  2529,  2533,  2534,  2535,  2536,  2537,
    2538,  2542,  2553,  2557,  2567,  2574,  2583,  2583,  2583,  2596,
    2597,  2598,  2599,  2600,  2614,  2620,  2624,  2628,  2633,  2638,
    2643,  2648,  2652,  2656,  2661,  2665,  2670,  2670,  2670,  2676,
    2683,  2683,  2683,  2688,  2688,  2688,  2694,  2694,  2694,  2699,
    2703,  2703,  2703,  2708,  2708,  2708,  2717,  2721,  2721,  2721,
    2726,  2726,  2726,  2735,  2739,  2739,  2739,  2744,  2744,  2744,
    2753,  2753,  2753,  2759,  2759,  2759,  2768,  2768,  2783,  2783,
    2803,  2804,  2805,  2806,  2810,  2817,  2824,  2830,  2839,  2844,
    2851,  2852,  2856,  2862,  2869,  2877,  2884,  2892,  2904,  2907,
    2921,  2930,  2931,  2935,  2940,  2947,  2952,  2962,  2967,  2974,
    2986,  2987,  2991,  3014
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
  "\"aka\"", "\"cast\"", "\"override\"", "\"abstract\"", "\"upcast\"",
  "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"", "\"where\"",
  "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"",
  "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"", "\"private\"",
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
     405,   406,   407,   408,   409,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   410,
     411,   126,    33,   412,   413,   414,   415,   416,    46,    91,
      93,    40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -559

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-559)))

#define YYTABLE_NINF -528

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -559,    43,  -559,  -559,    33,   -86,   -93,  -109,  -559,   -54,
    -559,  -559,    87,  -559,  -559,  -559,  -559,  -559,   317,  -559,
      46,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
     -34,  -559,     1,     6,    85,  -559,  -559,   -93,    29,  -559,
      49,   102,  -559,  -559,  -559,    46,   137,   193,  -559,  -559,
     211,   201,   180,  -559,   126,  -559,  -559,  -559,     8,   354,
     361,  -559,   363,    14,    33,   242,   -86,   225,   266,  -559,
     303,   319,  -559,  7388,   283,   -89,   368,   290,   295,  -559,
     322,    33,    87,  -559,  -559,  -559,    50,  -559,  -559,  -559,
     332,  -559,  -559,   388,  -559,  -559,   305,  -559,  -559,  -559,
    -559,  -559,   281,    88,  -559,  -559,  -559,  -559,   435,  -559,
    -559,   336,   337,   338,   340,  -559,  -559,  -559,   324,  -559,
    -559,  -559,  -559,  -559,   344,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,   347,  -559,  -559,  -559,   348,   349,
    -559,  -559,  -559,  -559,   350,   351,  -559,  -559,  -559,  -559,
    -559,  3181,  -559,  -559,   318,  -559,  -559,  -559,   360,   367,
    -559,   -82,  -559,  7000,   483,   484,  -559,   339,   353,   107,
    -559,   375,  -559,  -559,   385,  -559,  -559,   293,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,   -56,  -559,  -559,  -559,  -559,
    -559,   377,  -559,   175,   185,   191,  -559,  -559,  -559,  -559,
    -559,  -559,   498,  -559,  -559,    -7,  2967,  -559,  -559,  -559,
     342,   376,  -559,   352,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,   355,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
     362,  -559,  -559,   -96,   382,  -559,  -559,   280,  -559,   383,
     358,   -98,    33,   386,  -559,  -559,  -559,    88,  -559,  7388,
    7388,  7388,  7388,   371,   360,  7388,   339,  7388,   339,  7388,
     339,  7475,   367,  -559,  -559,  -559,   370,  -559,  -559,  -559,
    7301,   373,   393,  -559,   379,   394,   395,   384,   396,   390,
    -559,   404,  4606,  4606,  7127,  7214,  4606,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  4606,  4606,  4606,   210,  4606,  4606,
    -559,  4606,  -559,   389,  -559,  -559,  -559,   -80,  -559,  -559,
    -559,  -559,   397,  -559,  -559,  -559,  -559,  -559,  -559,  5796,
     391,  -559,  -559,  -559,  -559,  -559,   -66,  7388,   -62,  -559,
    -559,  -559,    -6,  -559,   -22,  7388,  -559,  1273,  -559,  -559,
    -559,  -559,  -559,  -559,   381,   -59,   210,  -559,  -559,  -559,
     386,   149,   528,  -559,   204,  -559,   563,   659,   584,   983,
    -559,    35,  1558,   382,  1717,   382,  1876,   382,   124,  -559,
      79,  3181,   133,  -559,  1939,  -559,   210,  -559,  4606,  -559,
    -559,  4606,  -559,  4606,  7388,   587,   587,   386,  1622,   386,
    1781,  6360,  -559,   195,   345,   587,   587,   -71,  -559,   587,
     587,   292,   112,  -559,  2397,   436,    38,   428,  4606,  4606,
    -559,  -559,  4606,  4606,  4606,  4606,   441,  4606,   446,  4606,
    4606,  4606,  4606,  4606,  4606,  4606,  4606,  3104,  4606,  4606,
    4606,  4606,  4606,  4606,  4606,  4606,  4606,  4606,   129,  4606,
    -559,  3241,  -559,  -559,  3181,  -559,  -559,   386,  -559,   -45,
    -559,   385,  3181,  -559,  4606,  -559,  -559,   386,  2099,  -559,
     353,  4606,  4606,  -559,   398,   453,  3400,   259,  2260,   289,
     289,  -559,   546,   412,   415,  4606,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,   416,   424,   432,  -559,  -559,  -559,   386,
    -559,   -91,  -559,  6292,  -559,   -37,  -559,  -559,    15,  -559,
     438,   477,   478,  7388,  -559,   -21,  -559,  7388,  -559,  -559,
    -559,   480,  -559,   467,  -559,   481,  -559,   482,  7388,  -559,
    7475,  -559,   367,  3537,  3696,  7388,  5250,  7388,  7388,  5328,
    7388,  5406,  2035,    16,   289,   141,  1940,  3833,  6360,   488,
     -17,   466,   492,  -559,  -559,   261,    18,  3992,   -17,   205,
    4606,  4606,   461,  -559,  4606,   312,   500,  -559,   192,  -559,
     505,  -559,   123,  6590,   -47,   339,   493,  -559,  -559,    59,
      59,   269,   269,  6916,  6916,   475,   444,  -559,  5872,   -55,
     -55,    59,    59,  6739,   830,  6658,   512,  6441,  6766,   748,
      25,   269,   269,   495,   495,   444,   444,   444,   513,  4606,
    -559,   517,  4606,   667,  5948,  -559,   143,  -559,  -559,  -559,
    7388,  -559,  2556,  -559,   472,    20,  2556,  -559,  -559,   550,
    1086,   662,  6590,   472,  -559,   518,   536,  6590,  -559,  2556,
    -559,  4696,   494,  -559,  -559,   496,  -559,  -559,   472,  -559,
    -559,  -559,  -559,   289,  -559,  4606,  4606,  4606,  4606,  4606,
    4606,  4606,  4606,  4606,  4606,  2693,  4606,  4606,  4606,  4606,
    4606,  4606,  2830,    54,  4606,  -559,    87,  -559,   672,    -9,
     501,  -559,  -559,   640,   657,  -559,  -559,  -559,  2556,  -559,
    2196,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    3181,  -559,  -559,  -559,  -559,   144,   538,  5484,  2337,  -559,
    2492,  2633,  -559,  2770,  -559,   436,  4606,  -559,  4606,  4606,
       3,   540,   505,   503,   510,  4606,   516,  4606,  4606,   505,
     506,   519,  6590,  -559,  -559,  6509,  -559,   543,  7388,   339,
      83,   154,  4606,  -559,   551,   553,   556,   557,  -559,   155,
     382,  -559,  4151,  -559,  -559,  4606,  -559,  6024,  -559,  6100,
    -559,  -559,  -559,  2907,  -559,  6190,  -559,  4606,  -559,  4797,
    4606,  4606,  -559,   353,  -559,  -559,  -559,  -559,  4898,  -559,
    -559,  -559,   356,  -559,  6590,  6590,  6590,  6590,  6590,  6590,
    6590,  6590,  6590,  6590,  4606,  4606,  6590,  6590,  6590,  6590,
    6590,  6590,  6590,  4606,  4606,  6590,  -559,   958,   159,   378,
    -559,  -559,   378,  -559,   562,   498,  -559,  2556,  -559,  4999,
    -559,  -559,  -559,  -559,  -559,   157,  -559,  -559,  -559,  -559,
    -559,   529,    61,  6590,  6590,   -17,   532,   138,   488,   541,
    -559,  6590,  -559,  -559,    77,   -17,   542,  -559,  -559,  -559,
    2909,   382,   515,  6590,  -559,  -559,  -559,  -559,   -47,   545,
     -94,  7388,  -559,   145,  6590,  -559,  -559,  -559,   498,  4606,
     106,  -559,  -559,  -559,  1460,   353,  -559,  -559,  4606,  -559,
    6841,  6590,  6841,  6590,  -559,   533,  -559,  -559,    22,   386,
    -559,  -559,  5100,  -559,  -559,  4606,   576,  -559,  4606,  4606,
    4606,  4310,  4606,   552,   554,  4606,  4606,  -559,  4606,   544,
    -559,  -559,   567,  -559,  -559,  -559,  4469,  -559,  -559,  3044,
    -559,  6590,   353,  -559,  -559,   472,  -559,  -559,   701,  -559,
    -559,  5562,   572,   269,   269,   269,  -559,  5640,  5175,   558,
    -559,  6590,  6590,  5175,   559,   210,  -559,  4606,  6590,  -559,
    -559,   356,    50,  -559,    50,  -559,  4606,  -559,   686,   564,
    -559,   566,  -559,  -559,   210,  6590,  -559,  -559,   565,   353,
    5718,  4606,   575,   577,  -559,  -559,  -559,  -559,  6590,  -559,
    -559
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    92,     1,   201,     0,     0,     0,     0,   202,     0,
     538,   536,     0,    14,     3,    10,     9,     8,     0,     7,
     403,     6,    11,     5,     4,    12,    13,    73,    74,    72,
      81,    83,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   418,    19,    18,   403,     0,     0,    86,    87,
       0,   181,    88,    90,     0,    85,   430,   429,   149,   419,
     431,   404,   405,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   183,
       0,     0,     0,    93,   151,   150,     0,   148,   421,   420,
       0,   433,   432,   426,   407,   406,   409,    79,    80,    77,
      78,    76,     0,     0,    75,    84,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   440,   460,   441,   472,   442,
     446,   447,   448,   449,   464,   453,   454,   455,   456,   457,
     458,   459,   461,   462,   509,   445,   452,   463,   516,   523,
     443,   450,   444,   451,     0,     0,   471,   479,   482,   480,
     481,   414,   415,   417,     0,    16,    17,    20,     0,     0,
     182,     0,    91,     0,     0,     0,    99,    94,     0,     0,
     427,     0,   434,   401,   355,    21,    22,     0,    68,    69,
      66,    67,    65,    64,    70,     0,    41,   500,   503,   506,
     496,     0,   476,   510,   517,   524,   530,   533,   487,   492,
     486,   499,     0,   495,   489,     0,     0,   491,   416,   474,
       0,     0,   381,     0,    89,   147,   102,   103,   105,   104,
     106,   107,   108,   109,   134,   135,   132,   133,   125,   136,
     137,   126,   123,   124,   145,     0,   146,   138,   139,   140,
     141,   113,   114,   115,   110,   111,   112,   128,   129,   127,
     121,   122,   117,   116,   118,   119,   120,   101,   100,   144,
       0,   130,   131,   355,    97,   175,   153,     0,   411,   424,
       0,   355,     0,     0,    23,    24,    25,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    94,     0,    94,     0,
      94,     0,     0,   494,   488,   490,     0,   493,   287,   288,
       0,     0,     0,   281,     0,     0,     0,     0,     0,     0,
     464,     0,     0,     0,     0,     0,     0,   237,   239,   238,
     240,   241,   242,    26,     0,     0,     0,     0,     0,     0,
     484,     0,   224,   225,   285,   284,   235,   282,   347,   346,
     345,   344,    92,   350,   283,   349,   348,   326,   289,     0,
       0,   286,   540,   541,   542,   543,     0,     0,     0,   143,
     142,    95,     0,   374,     0,     0,   152,     0,   465,   468,
     466,   469,   467,   470,     0,     0,     0,   428,   366,   408,
       0,     0,   436,   410,   394,    71,     0,     0,     0,     0,
     473,     0,     0,    97,     0,    97,     0,    97,   181,   378,
       0,   376,     0,   485,   185,   188,     0,   213,     0,   204,
     207,     0,   210,     0,     0,   315,   316,     0,   550,     0,
       0,     0,   567,     0,     0,   293,   292,   331,    32,   291,
     290,     0,   226,   353,     0,   233,     0,     0,     0,     0,
     317,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   278,     0,
     483,     0,   475,   539,   380,   537,   382,     0,   372,   385,
      96,   355,    98,   177,     0,    57,    58,     0,     0,   189,
       0,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,   156,   154,     0,     0,     0,   170,   165,   163,   162,
     164,   176,   157,     0,     0,     0,   168,   169,   171,     0,
     161,     0,   158,   243,   411,     0,   422,   425,   355,   402,
       0,     0,     0,     0,   395,     0,   501,     0,   507,   497,
     477,     0,   511,     0,   518,     0,   525,     0,     0,   531,
       0,   534,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,   558,   548,
     550,     0,   561,   562,   563,     0,     0,     0,   550,     0,
       0,     0,     0,    29,     0,    27,     0,   319,   270,   269,
       0,   327,     0,   219,     0,    94,     0,   341,   342,   294,
     295,   307,   308,   305,   306,     0,   336,   324,     0,   351,
     352,   296,   297,   312,   313,   314,     0,     0,   310,   311,
     309,   303,   304,   299,   298,   300,   301,   302,     0,     0,
     276,     0,     0,     0,     0,   329,     0,   373,   384,   383,
       0,   386,     0,   375,     0,     0,     0,   191,   194,     0,
     243,     0,   184,     0,   174,     0,     0,    51,    61,     0,
     198,   243,   225,   179,   180,     0,   172,   173,     0,   160,
     166,   167,   203,     0,   159,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   412,     0,   435,     0,   357,
       0,   356,   437,   438,     0,   392,   393,   391,     0,   502,
       0,   508,   498,   478,   512,   514,   519,   521,   526,   528,
     377,   532,   379,   535,   186,     0,     0,     0,     0,   332,
       0,     0,   333,     0,   354,   233,     0,   551,     0,     0,
     550,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   560,   568,   569,     0,    30,    33,     0,    94,
       0,     0,     0,   328,     0,     0,     0,     0,   231,     0,
      97,   338,     0,   322,   343,     0,   325,     0,   277,     0,
     279,   320,   330,   387,   390,   389,    62,     0,   195,   243,
       0,     0,   192,     0,    63,    49,    50,   199,   243,   196,
     226,   175,    54,   178,   253,   254,   256,   255,   257,   247,
     248,   249,   258,   259,     0,     0,   245,   246,   260,   261,
     250,   251,   252,     0,     0,   244,   423,     0,     0,   362,
     358,   359,   362,   367,     0,   391,   396,     0,   400,   243,
     504,   515,   522,   529,   187,     0,   216,   214,   205,   208,
     211,     0,     0,   545,   544,   550,     0,     0,   549,     0,
     553,   559,   565,   564,     0,   550,     0,   566,    28,    31,
       0,    97,     0,   220,   229,   230,   228,   227,     0,     0,
       0,     0,   267,     0,   337,   323,   321,   280,   383,     0,
       0,   193,   200,   197,     0,     0,    52,    53,     0,    59,
     264,   265,   262,   263,   413,    93,   363,   364,   357,     0,
     439,   398,   243,   399,   505,     0,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   552,     0,     0,
     556,   271,     0,   266,   232,   234,     0,   221,   236,     0,
     268,   388,     0,   155,    55,     0,   371,   368,   360,   365,
     397,     0,     0,   206,   209,   212,   334,     0,   570,     0,
     554,   547,   546,   570,     0,     0,   274,     0,   222,   339,
      60,    54,     0,   361,     0,   217,     0,   335,     0,     0,
     555,     0,   557,   272,     0,   223,   340,    56,     0,     0,
       0,     0,     0,     0,   275,   369,   370,   218,   571,   572,
     573
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -559,  -559,  -559,  -559,  -559,   313,   680,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,   719,  -559,   650,  -559,  -559,   694,
    -559,  -559,  -559,  -209,  -559,  -559,  -559,  -559,  -559,  -559,
     487,  -559,  -559,   703,   -58,  -559,   688,    75,  -322,  -276,
    -389,  -559,  -559,  -559,  -545,  -559,  -167,  -559,   -24,  -477,
     -12,  -559,  -559,  -559,  -559,  -559,  -559,  -559,   780,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -456,  -559,  -559,   -69,  -559,    52,  -473,  -559,  -362,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,   341,  -559,  -559,  -165,  -119,  -559,   -20,  -559,  -559,
    -559,   343,  -559,   267,  -559,  -339,   527,  -466,  -470,   122,
    -559,  -349,  -559,   782,  -559,  -559,  -559,   304,    71,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -360,   -70,  -559,  -559,  -559,   547,  -559,  -559,  -559,   -29,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,   530,  -558,
    -403,  -554,  -559,  -559,  -298,    93,   414,  -559,  -559,  -559,
    -128,  -559
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   157,    45,    14,   177,   183,   424,   334,   757,
     335,   586,    15,    16,    38,    39,    72,    17,    34,    35,
     503,   504,   898,   899,   505,   506,   507,   508,   509,   510,
     184,   185,    30,    31,    32,    52,    53,    54,    18,   264,
     366,   167,    19,    86,   168,    87,   336,   511,   367,   512,
     337,   513,   338,   514,   515,   516,   517,   518,   519,   520,
     339,   557,   918,   558,   919,   560,   920,   340,   555,   917,
     341,   592,   938,   342,   768,   769,   595,   343,   344,   649,
     522,   345,   346,   347,   758,   965,   759,   984,   348,   633,
     887,   593,   881,   986,   362,   832,   974,   908,   700,   528,
     972,   363,   364,   399,   400,   212,   213,   739,   478,   708,
     535,   383,   271,    62,    96,    21,   174,   375,    42,    75,
      22,    90,    23,   377,   171,   172,    60,    93,    24,   270,
     384,   350,   374,   148,   149,   210,   150,   284,   713,   401,
     282,   712,   279,   709,   280,   914,   281,   711,   285,   714,
     286,   841,   287,   716,   288,   842,   289,   718,   290,   843,
     291,   721,   292,   723,    25,    47,    26,    46,   351,   569,
     570,   571,   352,   572,   573,   574,   575,   353,   423,   354,
     979,   355
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   266,   479,   147,   543,   521,   545,   637,   547,   273,
     393,   648,   395,   642,   397,   636,   743,   578,   422,   476,
     435,   660,   -92,   161,   750,   294,   663,   664,    97,    98,
     673,   529,   761,   564,   830,   736,   936,   748,    40,   787,
     272,   477,   272,     2,   151,   436,   437,   448,   449,     3,
      68,    27,    28,   564,    36,    33,   596,   563,    40,   576,
     446,   146,    84,   448,   449,   830,   947,   433,    85,   764,
      55,   638,     4,    64,     5,    37,     6,    41,   765,   436,
     437,   472,     7,    69,   163,   211,   361,   831,   525,   379,
       8,   737,   265,    43,   674,   705,     9,   725,   153,   277,
     214,   434,   178,   179,   706,    61,   380,   468,   469,   766,
     532,   639,    74,   640,   767,    10,   641,   479,   831,   694,
      48,   473,    63,   468,   469,   475,   278,   645,   526,    44,
     438,   439,   440,   441,   442,   707,    11,   443,   444,   445,
     446,   382,   447,   448,   449,   450,   154,    49,   695,   451,
     760,   452,   453,   272,   295,    99,    64,   164,   165,   100,
     480,   101,   102,   481,    65,   784,   440,   441,   742,   788,
     672,   532,   296,   532,   446,   532,   447,   448,   449,   450,
      29,   297,   797,   451,   858,   597,   856,   461,   462,   463,
     464,   465,   466,   467,   696,   103,    70,   166,   540,   638,
     803,   525,   697,   468,   469,    73,   698,    71,   738,   147,
     147,   147,   147,   476,   381,   147,   762,   147,    50,   147,
     541,   147,    12,   463,   464,   465,   466,   467,    13,   180,
     147,   838,   762,   181,    51,   182,   102,   468,   469,   639,
      66,   826,   549,    50,   147,   147,   922,   628,   629,    40,
     386,   387,   388,   389,   638,    80,   392,   638,   394,    51,
     396,   762,   928,   925,   550,   267,   738,   146,   146,   146,
     146,   404,    80,   146,   588,   146,   630,   146,   762,   146,
     852,    82,   548,   753,    77,   418,   420,   147,   146,   436,
     437,   942,   864,   268,   639,   147,   551,   639,   762,   762,
     762,   923,   146,   146,    64,   763,    83,   631,   632,   741,
     878,   929,   436,   437,    82,   428,   883,   889,   552,   770,
     915,    56,    57,   651,    58,   782,   844,   940,   474,   530,
     658,   890,    80,  -513,   872,   879,   482,   855,  -513,   905,
      78,    50,   916,  -520,   147,   146,   865,   368,  -520,  -527,
    -273,   369,    59,   146,  -527,  -273,  -513,    51,    79,   532,
     911,    81,   533,   699,   527,   534,  -520,   895,   370,   371,
     372,   373,  -527,  -273,   438,   439,   440,   441,   896,   897,
     581,   880,   582,   106,   446,   562,   447,   448,   449,   450,
     747,   926,   751,   451,   554,   452,   453,   438,   439,   440,
     441,   442,   146,    71,   443,   444,   445,   446,    88,   447,
     448,   449,   450,   108,    89,    91,   451,    94,   452,   453,
     589,    92,   155,    95,   454,   455,   456,   988,   156,   989,
     175,   176,   906,   463,   464,   465,   466,   467,   907,   949,
     413,   746,   274,   275,   276,   265,   747,   468,   469,   457,
     109,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   274,   275,   147,   436,   437,   110,   147,   152,   160,
     468,   469,   332,   662,   587,   265,   158,   786,   147,   169,
     147,   159,   932,   871,   170,   147,   794,   147,   147,    69,
     147,   173,   436,   437,   175,   176,   583,   584,   187,   188,
     189,   802,   190,   208,   704,   191,   192,   209,   710,   193,
     194,   195,   196,   197,   211,   436,   437,   261,   262,   720,
     263,   146,   269,   272,   283,   146,   728,   356,   730,   731,
     293,   733,   521,   382,   357,   359,   146,   358,   146,   265,
     365,   376,   360,   146,   378,   146,   146,   349,   146,   479,
     403,   440,   441,   390,   406,   407,   409,   410,   412,   446,
     408,   447,   448,   449,   450,   411,   414,   524,   451,   531,
     147,   413,   471,   432,   594,   598,    12,   438,   439,   440,
     441,   442,   665,   654,   443,   444,   445,   446,   605,   447,
     448,   449,   450,   607,   655,   198,   451,   666,   452,   453,
     667,   669,   440,   441,   454,   455,   456,   436,   437,   670,
     446,   783,   447,   448,   449,   450,   198,   671,   701,   451,
     199,   200,   468,   469,   702,   703,   892,   472,   146,   457,
     715,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   199,   200,   741,   717,   719,   744,   745,   754,   756,
     468,   469,   760,   415,   416,   771,   772,   421,   265,   774,
     776,   465,   466,   467,   778,   425,   426,   427,   780,   429,
     430,   673,   431,   468,   469,   793,   795,   796,   800,   829,
     201,   834,   801,   859,    55,   845,   833,   857,   147,   198,
     860,   198,   869,   866,   440,   441,   862,   933,   874,   867,
     875,   201,   446,   876,   877,   448,   449,   450,   523,   910,
     921,   451,   924,   937,   199,   200,   199,   200,   946,   202,
     203,   927,   930,   952,   204,   935,   536,   205,   944,   870,
     966,   964,   959,   973,   960,   976,   991,   585,   980,   982,
     202,   203,   206,   104,   992,   204,   146,   538,   205,   556,
     995,   207,   559,   993,   561,   999,    67,  1000,   186,   568,
     107,   568,   987,   206,   385,   468,   469,   105,   436,   437,
     162,   828,   207,   705,   201,   970,   201,   894,   971,   599,
     600,    20,   706,   601,   602,   603,   604,   851,   606,   948,
     608,   609,   610,   611,   612,   613,   614,   615,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   627,   934,
     634,   147,   909,   835,   203,   202,   203,   722,   204,   402,
     204,   205,   996,   205,   643,   644,   837,    76,   693,   650,
     405,   391,   652,   653,   579,   981,   206,   657,   206,   661,
     863,     0,   836,     0,   537,   207,   668,   207,     0,     0,
     436,   437,   939,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,   146,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   727,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   425,   431,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   431,     0,
       0,   752,   421,     0,     0,   755,   468,   469,     0,     0,
       0,     0,     0,     0,     0,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,   983,   451,     0,   452,   453,     0,     0,
       0,     0,   454,     0,   456,     0,     0,     0,     0,     0,
     777,     0,   994,   779,     0,     0,     0,     0,   436,   437,
       0,     0,     0,   785,     0,     0,     0,   789,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
     798,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,   198,   804,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   816,   817,   818,   819,
     820,   821,   822,   825,     0,   827,     0,     0,     0,     0,
     199,   200,     0,     0,     0,     0,     0,     0,     0,   839,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,   853,
     854,     0,   451,     0,   452,   453,   861,     0,   568,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
     201,     0,     0,   873,     0,     0,   436,   437,     0,     0,
       0,     0,     0,     0,     0,   457,   884,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,   816,   825,     0,     0,     0,   468,   469,     0,   202,
     203,     0,     0,   904,   204,     0,   539,   205,     0,     0,
       0,     0,     0,     0,     0,   900,   901,     0,     0,     0,
       0,     0,   206,     0,   902,   903,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,   912,     0,
       0,     0,     0,   675,   676,   677,   678,   679,   680,   681,
     682,   438,   439,   440,   441,   442,   683,   684,   443,   444,
     445,   446,   790,   447,   448,   449,   450,     0,     0,     0,
     451,   686,   452,   453,   687,   688,     0,     0,   454,   455,
     456,   689,   690,   691,     0,     0,     0,     0,     0,     0,
     941,     0,     0,     0,     0,   523,     0,     0,     0,   945,
       0,     0,   791,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,   951,     0,     0,   953,
     954,   955,   957,   958,   468,   469,   961,   962,     0,   963,
       0,   792,     0,     0,   483,     0,     0,   968,     0,     3,
       0,   484,   485,   486,     0,   487,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,   488,   303,
     489,   490,     0,     0,     0,     0,     0,     0,   985,     0,
     491,   304,     0,   492,     0,   305,     0,   990,   306,     0,
       8,   307,   493,     0,   494,   308,     0,     0,   495,   496,
       0,     0,   998,     0,     0,   497,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   311,   498,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   499,   500,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,     0,     0,     0,     0,
       0,     0,     0,     0,   331,     0,   332,   333,   501,   265,
     502,   483,     0,     0,     0,     0,     3,     0,   484,   485,
     486,     0,   487,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,     0,     0,   488,   303,   489,   490,     0,
       0,     0,     0,     0,     0,     0,     0,   491,   304,     0,
     492,     0,   305,     0,     0,   306,     0,     8,   307,   493,
       0,   494,   308,     0,     0,   495,   496,     0,     0,     0,
       0,     0,   497,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,     0,     0,   311,   498,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,     0,     0,     0,     0,     0,     0,
     198,    50,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,     0,     0,     0,     0,   199,   200,     0,     0,     0,
       0,     0,     0,     0,   324,   325,   326,     0,   327,     0,
       0,   328,   329,     0,     0,     0,   298,   299,   300,   301,
     302,   331,     0,   332,   333,   501,   265,   943,   303,     0,
       0,     0,     0,     0,   198,     0,     0,     0,     0,     0,
     304,     0,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,   564,     0,   308,   201,     0,     0,     0,   199,
     200,     0,     0,     0,   309,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   310,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,   202,   203,   311,     0,     0,   204,
       0,   542,   205,     0,     0,     0,     0,     0,     0,   312,
     313,     0,     0,     0,     0,     0,     0,   206,     0,   201,
       0,     0,     0,     0,     0,     0,   207,     0,     0,   198,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   565,
     323,     0,     0,     0,   199,   200,     0,     0,   202,   203,
       0,     0,     0,   204,     0,     0,   566,   325,   326,     0,
     327,     0,     0,   328,   329,   298,   299,   300,   301,   302,
       0,   206,     0,   567,     0,   332,   333,   303,   265,     0,
     207,     0,     0,   198,     0,     0,     0,     0,     0,   304,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,   201,     0,     0,     0,   199,   200,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,   202,   203,   311,     0,     0,   204,     0,
     544,   205,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,   206,     0,   201,     0,
       0,     0,     0,     0,     0,   207,     0,     0,   198,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   565,   323,
       0,     0,     0,   199,   200,     0,     0,   202,   203,     0,
       0,     0,   204,     0,     0,   566,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
     206,     0,   577,     0,   332,   333,   303,   265,     0,   207,
       0,   198,   294,     0,     0,     0,     0,     0,   304,     0,
       0,     0,   305,     0,     0,   306,     0,     0,   307,     0,
       0,     0,   308,   201,     0,     0,   199,   200,     0,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,   202,   203,   311,     0,     0,   204,     0,   546,
     205,     0,     0,     0,     0,     0,     0,   312,   313,     0,
       0,     0,     0,     0,     0,   206,   201,     0,     0,     0,
       0,     0,     0,     0,   207,     0,     0,   198,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,     0,   199,   200,     0,   202,   203,     0,     0,     0,
     204,   295,     0,   205,   324,   325,   326,     0,   327,     0,
       0,   328,   329,   298,   299,   300,   301,   302,   206,   296,
     553,   331,     0,   332,   333,   303,   265,   207,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   304,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   311,     0,     0,   204,     0,   735,   205,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,   206,   646,     0,     0,     0,     0,
       0,   499,   500,   207,     0,     0,     0,     0,   198,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,     0,   199,   200,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,     0,     0,   298,   299,   300,   301,   302,     0,
     331,     0,   332,   333,   647,   265,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,   305,     0,     0,   306,     0,     0,   307,     0,
       0,     0,   308,   201,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,   202,   203,   311,     0,     0,   204,     0,   840,
     205,     0,     0,     0,     0,     0,     0,   312,   313,   198,
       0,     0,     0,     0,     0,   206,   659,     0,     0,     0,
       0,     0,   499,   500,   207,     0,     0,     0,     0,     0,
       0,    50,     0,     0,   199,   200,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
       0,     0,     0,   303,   324,   325,   326,     0,   327,     0,
       0,   328,   329,     0,     0,   304,     0,     0,     0,   305,
       0,   331,   306,   332,   333,   307,   265,     0,     0,   308,
       0,     0,     0,     0,   201,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,   202,   203,     0,     0,     0,   204,     0,
     847,   205,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,     0,     0,   198,   207,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,   199,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   590,     0,   331,   591,
     332,   333,   303,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,     0,   305,     0,
       0,   306,     0,     0,   307,     0,     0,     0,   308,   201,
       0,     0,     0,     0,     0,     0,     0,     0,   309,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,   202,   203,
     311,     0,     0,   204,     0,   848,   205,     0,     0,     0,
       0,     0,     0,   312,   313,   198,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,   499,   500,
     207,     0,     0,     0,     0,     0,     0,    50,     0,     0,
     199,   200,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,     0,   303,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
       0,   304,     0,     0,     0,   305,     0,   331,   306,   332,
     333,   307,   265,     0,     0,   308,     0,     0,     0,     0,
     201,     0,     0,     0,     0,   309,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   311,     0,   202,
     203,     0,     0,     0,   204,     0,   849,   205,     0,     0,
     312,   313,   198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   206,     0,     0,   814,   815,     0,     0,     0,
       0,   207,     0,     0,    50,     0,     0,   199,   200,     0,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,     0,     0,     0,   303,   324,   325,   326,
       0,   327,     0,     0,   328,   329,     0,     0,   304,     0,
       0,     0,   305,     0,   331,   306,   332,   333,   307,   265,
       0,     0,   308,     0,     0,     0,     0,   201,     0,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,     0,     0,   311,     0,   202,   203,     0,     0,
       0,   204,     0,   850,   205,     0,     0,   312,   313,   198,
       0,   198,     0,     0,     0,     0,     0,     0,     0,   206,
       0,     0,   823,   824,     0,     0,     0,     0,   207,     0,
       0,    50,     0,     0,   199,   200,   199,   200,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
       0,     0,     0,   303,   324,   325,   326,     0,   327,     0,
       0,   328,   329,     0,     0,   304,     0,     0,     0,   305,
       0,   331,   306,   332,   333,   307,   265,     0,     0,   308,
       0,     0,     0,   638,   201,     0,   201,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,   888,   203,   202,   203,     0,   204,     0,
     204,   205,   931,   205,   312,   313,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   206,     0,   206,     0,
       0,     0,     0,     0,     0,   207,     0,   207,    50,     0,
       0,   199,   200,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,   298,   299,
     300,   301,   302,     0,     0,   616,     0,     0,     0,     0,
     303,   324,   325,   326,     0,   327,     0,     0,   328,   329,
       0,     0,   304,     0,     0,     0,   305,   330,   331,   306,
     332,   333,   307,   265,     0,     0,   308,     0,     0,     0,
       0,   201,     0,     0,     0,     0,   309,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   311,     0,
     202,   203,     0,     0,     0,   204,     0,   969,   205,     0,
       0,   312,   313,   198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,     0,     0,     0,     0,     0,
       0,     0,   207,     0,     0,    50,     0,     0,   199,   200,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,   298,   299,   300,   301,   302,
       0,     0,     0,     0,     0,     0,     0,   303,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,   304,
       0,     0,     0,   305,     0,   331,   306,   332,   333,   307,
     265,     0,     0,   308,     0,     0,     0,     0,   201,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,   202,   203,     0,
       0,     0,   204,     0,     0,   205,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     206,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   635,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,   305,     0,     0,   306,     0,     0,   307,     0,
       0,     0,   308,     0,     0,   656,     0,     0,     0,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,     0,     0,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
       0,     0,     0,   303,   324,   325,   326,     0,   327,     0,
       0,   328,   329,     0,     0,   304,     0,     0,     0,   305,
       0,   331,   306,   332,   333,   307,   265,     0,     0,   308,
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
     298,   299,   300,   301,   302,     0,     0,     0,   331,   724,
     332,   333,   303,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,     0,   305,     0,
       0,   306,     0,     0,   307,     0,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,   726,   303,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
       0,   304,     0,     0,     0,   305,     0,   331,   306,   332,
     333,   307,   265,     0,     0,   308,     0,     0,     0,     0,
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
     302,     0,     0,     0,   331,   740,   332,   333,   303,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,     0,   305,     0,     0,   306,     0,     0,
     307,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   310,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    51,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   325,   326,     0,
     327,     0,     0,   328,   329,   298,   299,   300,   301,   302,
       0,     0,     0,   331,   749,   332,   333,   303,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
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
       0,     0,   331,   882,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,   305,     0,     0,   306,     0,     0,   307,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,     0,     0,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   325,   326,     0,   327,     0,
       0,   328,   329,   298,   299,   300,   301,   302,     0,     0,
       0,   331,   956,   332,   333,   303,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,   967,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
     298,   299,   300,   301,   302,     0,     0,     0,     0,     0,
       0,     0,   303,   324,   325,   326,     0,   327,     0,     0,
     328,   329,     0,     0,   304,     0,     0,     0,   305,     0,
     331,   306,   332,   333,   307,   265,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   313,     0,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
       0,     0,     0,     0,     0,     0,     0,   331,     0,   332,
     333,     0,   265,   675,   676,   677,   678,   679,   680,   681,
     682,   438,   439,   440,   441,   442,   683,   684,   443,   444,
     445,   446,   790,   447,   448,   449,   450,   436,   437,     0,
     451,   686,   452,   453,   687,   688,     0,     0,   454,   455,
     456,   689,   690,   691,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   791,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,   799,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   675,   676,   677,   678,   679,   680,
     681,   682,   438,   439,   440,   441,   442,   683,   684,   443,
     444,   445,   446,   790,   447,   448,   449,   450,   436,   437,
       0,   451,   686,   452,   453,   687,   688,     0,     0,   454,
     455,   456,   689,   690,   691,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   791,   457,     0,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,     0,     0,     0,
       0,     0,   891,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   675,   676,   677,   678,   679,
     680,   681,   682,   438,   439,   440,   441,   442,   683,   684,
     443,   444,   445,   446,   790,   447,   448,   449,   450,   436,
     437,     0,   451,   686,   452,   453,   687,   688,     0,     0,
     454,   455,   456,   689,   690,   691,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   791,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,   893,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   675,   676,   677,   678,
     679,   680,   681,   682,   438,   439,   440,   441,   442,   683,
     684,   443,   444,   445,   446,   790,   447,   448,   449,   450,
     436,   437,     0,   451,   686,   452,   453,   687,   688,     0,
       0,   454,   455,   456,   689,   690,   691,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   791,   457,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,   913,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   437,   675,   676,   677,
     678,   679,   680,   681,   682,   438,   439,   440,   441,   442,
     683,   684,   443,   444,   445,   446,   790,   447,   448,   449,
     450,     0,     0,     0,   451,   686,   452,   453,   687,   688,
       0,     0,   454,   455,   456,   689,   690,   691,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   791,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
     436,   437,     0,     0,     0,     0,     0,     0,   468,   469,
     438,   439,   440,   441,   442,   950,     0,   443,   444,   445,
     446,     0,   447,   448,   449,   450,     0,     0,     0,   451,
       0,   452,   453,     0,     0,     0,     0,   454,   455,   456,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,   436,   437,
       0,     0,     0,   468,   469,   438,   439,   440,   441,   442,
     978,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,   436,   437,   468,   469,
       0,     0,   729,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,   436,   437,   468,   469,     0,     0,
     732,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,   436,   437,   468,   469,     0,     0,   734,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,     0,
     436,   437,   468,   469,     0,     0,   846,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,   436,   437,
     468,   469,     0,     0,   975,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,   436,   437,   468,   469,
       0,     0,   977,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,   436,   437,     0,     0,   468,   469,     0,     0,
     997,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,   436,   437,
       0,     0,     0,     0,   468,   469,   470,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,   436,   437,     0,     0,     0,     0,
     468,   469,   773,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
     436,   437,     0,     0,     0,     0,   468,   469,   781,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   885,   438,   439,   440,   441,   442,
     436,   437,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     886,     0,     0,     0,     0,     0,     0,   675,   676,   677,
     678,   679,   680,   681,   682,   438,   439,   440,   441,   442,
     683,   684,   443,   444,   445,   446,   790,   447,   448,   449,
     450,  -243,   436,   437,   451,   686,   452,   453,   687,   688,
       0,     0,   454,   455,   456,   689,   690,   691,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   791,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   437,     0,     0,     0,     0,     0,     0,     0,   675,
     676,   677,   678,   679,   680,   681,   682,   438,   439,   440,
     441,   442,   683,   684,   443,   444,   445,   446,   685,   447,
     448,   449,   450,     0,     0,     0,   451,   686,   452,   453,
     687,   688,     0,     0,   454,   455,   456,   689,   690,   691,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   692,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   436,   437,     0,     0,   438,   439,   440,   441,   442,
     468,   469,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
     580,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   436,
     437,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,     0,   447,   448,
     449,   450,     0,     0,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   457,   775,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     436,   437,     0,     0,   438,   439,   440,   441,   442,   468,
     469,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,     0,     0,   451,     0,   452,   453,     0,     0,     0,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   868,     0,     0,     0,   457,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   436,   437,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   436,
     437,     0,     0,   438,   439,   440,   441,   442,   468,   469,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,   436,   437,     0,     0,
     454,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,   436,   437,   451,     0,   452,   453,     0,     0,     0,
       0,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,     0,
       0,     0,     0,     0,     0,     0,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,   436,   437,     0,     0,
       0,     0,     0,     0,   468,   469,   438,   439,     0,     0,
     442,     0,     0,   443,   444,   445,   446,     0,   447,   448,
     449,   450,     0,     0,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   457,     0,
     458,   459,   460,   461,   462,     0,     0,     0,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   438,   439,   440,   441,   442,     0,     0,   443,     0,
       0,   446,     0,   447,   448,   449,   450,   215,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,     0,     0,     0,   234,   235,
       0,     0,     0,     0,     0,   236,   237,   238,   239,   240,
       0,     0,   241,   242,   243,   244,   245,   246,     0,   417,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,   112,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,     0,
       0,   257,   258,   113,     0,     0,     0,     0,   259,   260,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,     0,     0,     0,   419,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,   114,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,   114,     0,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,     0,     0,     0,     0,     0,   112,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,   113,     0,     0,     0,     0,   314,
     315,   316,     0,     0,     0,     0,     0,     0,    51,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,   114,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398
};

static const yytype_int16 yycheck[] =
{
      12,   168,   362,    73,   393,   367,   395,   477,   397,   174,
     286,   488,   288,   479,   290,   471,   570,   420,   316,   358,
     342,   498,     7,    81,   578,    32,   499,   500,    14,    15,
     121,   380,   590,    50,    43,    19,   130,    19,   147,    19,
     138,    47,   138,     0,    73,    20,    21,   118,   119,     6,
      21,    18,    19,    50,   147,   141,    18,   417,   147,   419,
     115,    73,    54,   118,   119,    43,    44,   147,    60,   116,
      82,   116,    29,   155,    31,   168,    33,   186,   125,    20,
      21,   147,    39,    54,    34,   147,   182,    96,   147,   187,
      47,   564,   186,   147,   185,   116,    53,   553,   187,   155,
     182,   181,    14,    15,   125,    59,   271,   178,   179,   156,
     155,   156,    41,   158,   161,    72,   161,   477,    96,   156,
      33,   187,   156,   178,   179,   187,   182,   487,   187,   183,
     105,   106,   107,   108,   109,   156,    93,   112,   113,   114,
     115,   147,   117,   118,   119,   120,    75,    60,   185,   124,
     147,   126,   127,   138,   161,   141,   155,   107,   108,   145,
     182,   147,   148,   185,   158,   642,   107,   108,   185,   646,
     519,   155,   179,   155,   115,   155,   117,   118,   119,   120,
     147,   188,   659,   124,   742,   147,   740,   162,   163,   164,
     165,   166,   167,   168,   179,   181,   167,   147,   163,   116,
     673,   147,   187,   178,   179,   156,   528,   178,   125,   279,
     280,   281,   282,   552,   272,   285,   155,   287,   131,   289,
     185,   291,   179,   164,   165,   166,   167,   168,   185,   141,
     300,   708,   155,   145,   147,   147,   148,   178,   179,   156,
     155,   187,   163,   131,   314,   315,   185,   118,   119,   147,
     279,   280,   281,   282,   116,   131,   285,   116,   287,   147,
     289,   155,   185,   125,   185,   158,   125,   279,   280,   281,
     282,   300,   131,   285,   162,   287,   147,   289,   155,   291,
     736,   155,   158,   581,   147,   314,   315,   357,   300,    20,
      21,   185,   748,   186,   156,   365,   163,   156,   155,   155,
     155,   855,   314,   315,   155,   182,   180,   178,   179,   155,
     155,   865,    20,    21,   155,   327,   772,   783,   185,   595,
     163,     4,     5,   490,     7,   182,   182,   182,   357,   180,
     497,   787,   131,   158,   180,   180,   365,   740,   163,   180,
     147,   131,   185,   158,   414,   357,   749,    67,   163,   158,
     158,    71,    35,   365,   163,   163,   181,   147,   147,   155,
     837,   181,   158,   528,   376,   161,   181,    11,    88,    89,
      90,    91,   181,   181,   105,   106,   107,   108,    22,    23,
     185,   770,   187,   141,   115,   414,   117,   118,   119,   120,
     185,   857,   187,   124,   406,   126,   127,   105,   106,   107,
     108,   109,   414,   178,   112,   113,   114,   115,    54,   117,
     118,   119,   120,   147,    60,    54,   124,    54,   126,   127,
     432,    60,    54,    60,   132,   133,   134,   972,    60,   974,
     149,   150,    54,   164,   165,   166,   167,   168,    60,   909,
     181,   180,   149,   150,   151,   186,   185,   178,   179,   157,
     147,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   149,   150,   533,    20,    21,   147,   537,   185,   147,
     178,   179,   183,   184,   182,   186,   186,   644,   548,   147,
     550,   186,   871,   759,    96,   555,   653,   557,   558,    54,
     560,   186,    20,    21,   149,   150,   151,   152,   162,   162,
     162,   668,   162,   185,   533,   181,   162,   147,   537,   162,
     162,   162,   162,   162,   147,    20,    21,    34,    34,   548,
     181,   533,   147,   138,   147,   537,   555,   185,   557,   558,
      32,   560,   894,   147,   158,   180,   548,   185,   550,   186,
     158,   158,   180,   555,   186,   557,   558,   206,   560,   909,
     180,   107,   108,   182,   181,   162,   162,   162,   162,   115,
     181,   117,   118,   119,   120,   181,   162,   186,   124,    41,
     640,   181,   181,   184,   138,   147,   179,   105,   106,   107,
     108,   109,    36,   185,   112,   113,   114,   115,   147,   117,
     118,   119,   120,   147,   141,    32,   124,   185,   126,   127,
     185,   185,   107,   108,   132,   133,   134,    20,    21,   185,
     115,   640,   117,   118,   119,   120,    32,   185,   180,   124,
      57,    58,   178,   179,   147,   147,   793,   147,   640,   157,
     163,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    57,    58,   155,   163,   163,   180,   155,   187,   149,
     178,   179,   147,   312,   313,   162,   181,   316,   186,   147,
     147,   166,   167,   168,   147,   324,   325,   326,     1,   328,
     329,   121,   331,   178,   179,    13,   158,   141,   184,     7,
     117,    41,   186,   180,   696,   147,   185,   147,   758,    32,
     180,    32,   149,   187,   107,   108,   180,   182,   147,   180,
     147,   117,   115,   147,   147,   118,   119,   120,   367,   147,
     181,   124,   180,   880,    57,    58,    57,    58,   185,   156,
     157,   180,   180,   147,   161,   180,   163,   164,   895,   758,
     163,   187,   180,    32,   180,   163,    50,   424,   180,   180,
     156,   157,   179,    63,   180,   161,   758,   163,   164,   408,
     185,   188,   411,   187,   413,   180,    37,   180,   108,   418,
      66,   420,   971,   179,   277,   178,   179,    64,    20,    21,
      82,   696,   188,   116,   117,   942,   117,   801,   945,   438,
     439,     1,   125,   442,   443,   444,   445,   735,   447,   908,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   878,
     469,   881,   832,   156,   157,   156,   157,   550,   161,   292,
     161,   164,   989,   164,   481,   484,   704,    45,   524,   488,
     300,   284,   491,   492,   420,   963,   179,   496,   179,   498,
     747,    -1,   185,    -1,   185,   188,   505,   188,    -1,    -1,
      20,    21,   881,   105,   106,   107,   108,   109,    -1,    -1,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,   881,
      -1,    -1,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   554,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   566,   567,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   577,    -1,
      -1,   580,   581,    -1,    -1,   584,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,
      -1,    -1,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,   965,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,   134,    -1,    -1,    -1,    -1,    -1,
     629,    -1,   984,   632,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,   642,    -1,    -1,    -1,   646,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     659,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,    -1,    -1,    -1,    -1,    32,   675,   676,   677,   678,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   691,   692,    -1,   694,    -1,    -1,    -1,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   708,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,   738,
     739,    -1,   124,    -1,   126,   127,   745,    -1,   747,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,   762,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,   775,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,   790,   791,    -1,    -1,    -1,   178,   179,    -1,   156,
     157,    -1,    -1,   185,   161,    -1,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   814,   815,    -1,    -1,    -1,
      -1,    -1,   179,    -1,   823,   824,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,   837,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
     889,    -1,    -1,    -1,    -1,   894,    -1,    -1,    -1,   898,
      -1,    -1,   156,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   915,    -1,    -1,   918,
     919,   920,   921,   922,   178,   179,   925,   926,    -1,   928,
      -1,   185,    -1,    -1,     1,    -1,    -1,   936,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,   967,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,   976,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    -1,   991,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    89,    90,    91,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   165,   166,
      -1,   168,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,
     187,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    -1,    -1,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    89,
      90,    91,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      32,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,   165,   166,    -1,   168,    -1,
      -1,   171,   172,    -1,    -1,    -1,    14,    15,    16,    17,
      18,   181,    -1,   183,   184,   185,   186,   187,    26,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    50,    -1,    52,   117,    -1,    -1,    -1,    57,
      58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    -1,
      88,    89,    90,    91,   156,   157,    94,    -1,    -1,   161,
      -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    32,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    57,    58,    -1,    -1,   156,   157,
      -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    -1,    -1,   171,   172,    14,    15,    16,    17,    18,
      -1,   179,    -1,   181,    -1,   183,   184,    26,   186,    -1,
     188,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,   117,    -1,    -1,    -1,    57,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    83,    84,    85,    -1,    -1,    88,
      89,    90,    91,   156,   157,    94,    -1,    -1,   161,    -1,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    32,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    57,    58,    -1,    -1,   156,   157,    -1,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,   168,
      -1,    -1,   171,   172,    14,    15,    16,    17,    18,    -1,
     179,    -1,   181,    -1,   183,   184,    26,   186,    -1,   188,
      -1,    32,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,   117,    -1,    -1,    57,    58,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    89,
      90,    91,   156,   157,    94,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,   179,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    32,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    57,    58,    -1,   156,   157,    -1,    -1,    -1,
     161,   161,    -1,   164,   164,   165,   166,    -1,   168,    -1,
      -1,   171,   172,    14,    15,    16,    17,    18,   179,   179,
     181,   181,    -1,   183,   184,    26,   186,   188,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    -1,    88,    89,    90,
      91,   156,   157,    94,    -1,    -1,   161,    -1,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   116,    -1,    -1,    -1,    -1,
      -1,   122,   123,   188,    -1,    -1,    -1,    -1,    32,    -1,
     131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,   165,   166,    -1,   168,    -1,    -1,
     171,   172,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     181,    -1,   183,   184,   185,   186,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    89,
      90,    91,   156,   157,    94,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    32,
      -1,    -1,    -1,    -1,    -1,   179,   116,    -1,    -1,    -1,
      -1,    -1,   122,   123,   188,    -1,    -1,    -1,    -1,    -1,
      -1,   131,    -1,    -1,    57,    58,    -1,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,   164,   165,   166,    -1,   168,    -1,
      -1,   171,   172,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,   181,    45,   183,   184,    48,   186,    -1,    -1,    52,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    -1,    88,    89,    90,    91,    -1,
      -1,    94,    -1,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,   164,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   188,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,   165,   166,    -1,   168,    -1,    -1,   171,   172,
      14,    15,    16,    17,    18,    -1,   179,    -1,   181,   182,
     183,   184,    26,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    -1,    88,    89,    90,    91,   156,   157,
      94,    -1,    -1,   161,    -1,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,    32,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     188,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      57,    58,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
     164,   165,   166,    -1,   168,    -1,    -1,   171,   172,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,   181,    45,   183,
     184,    48,   186,    -1,    -1,    52,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    89,    90,    91,    -1,    -1,    94,    -1,   156,
     157,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,    -1,
     107,   108,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,   188,    -1,    -1,   131,    -1,    -1,    57,    58,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,   164,   165,   166,
      -1,   168,    -1,    -1,   171,   172,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,   181,    45,   183,   184,    48,   186,
      -1,    -1,    52,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    89,
      90,    91,    -1,    -1,    94,    -1,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,   164,    -1,    -1,   107,   108,    32,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,   188,    -1,
      -1,   131,    -1,    -1,    57,    58,    57,    58,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,   164,   165,   166,    -1,   168,    -1,
      -1,   171,   172,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,   181,    45,   183,   184,    48,   186,    -1,    -1,    52,
      -1,    -1,    -1,   116,   117,    -1,   117,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    -1,    88,    89,    90,    91,    -1,
      -1,    94,    -1,   156,   157,   156,   157,    -1,   161,    -1,
     161,   164,   163,   164,   107,   108,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   188,   131,    -1,
      -1,    57,    58,    -1,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      26,   164,   165,   166,    -1,   168,    -1,    -1,   171,   172,
      -1,    -1,    38,    -1,    -1,    -1,    42,   180,   181,    45,
     183,   184,    48,   186,    -1,    -1,    52,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    -1,    88,    89,    90,    91,    -1,    -1,    94,    -1,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,
      -1,   107,   108,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,   131,    -1,    -1,    57,    58,
      -1,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   164,   165,
     166,    -1,   168,    -1,    -1,   171,   172,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,   181,    45,   183,   184,    48,
     186,    -1,    -1,    52,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    83,    84,    85,    -1,    -1,    88,
      89,    90,    91,    -1,    -1,    94,    -1,   156,   157,    -1,
      -1,    -1,   161,    -1,    -1,   164,    -1,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,   165,   166,    -1,   168,
      -1,    -1,   171,   172,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   181,   182,   183,   184,    26,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    89,
      90,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,   164,   165,   166,    -1,   168,    -1,
      -1,   171,   172,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,   181,    45,   183,   184,    48,   186,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    -1,    88,    89,    90,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,   165,   166,    -1,   168,    -1,    -1,   171,   172,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   181,   182,
     183,   184,    26,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    -1,    88,    89,    90,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,   162,    26,
     164,   165,   166,    -1,   168,    -1,    -1,   171,   172,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,   181,    45,   183,
     184,    48,   186,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    89,    90,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   165,   166,
      -1,   168,    -1,    -1,   171,   172,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   181,   182,   183,   184,    26,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    -1,
      88,    89,    90,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,   165,   166,    -1,
     168,    -1,    -1,   171,   172,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   181,   182,   183,   184,    26,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    83,    84,    85,    -1,    -1,    88,
      89,    90,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,   165,   166,    -1,   168,
      -1,    -1,   171,   172,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   181,   182,   183,   184,    26,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    89,
      90,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,   165,   166,    -1,   168,    -1,
      -1,   171,   172,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   181,   182,   183,   184,    26,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    -1,    88,    89,    90,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,   164,   165,   166,    -1,   168,    -1,    -1,
     171,   172,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
     181,    45,   183,   184,    48,   186,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    -1,    88,    89,    90,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,   165,   166,    -1,   168,    -1,    -1,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,    -1,   186,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    20,    21,    -1,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,    -1,    -1,    -1,
      -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    20,    21,
      -1,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,   179,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    20,
      21,    -1,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      20,    21,    -1,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
     105,   106,   107,   108,   109,   185,    -1,   112,   113,   114,
     115,    -1,   117,   118,   119,   120,    -1,    -1,    -1,   124,
      -1,   126,   127,    -1,    -1,    -1,    -1,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,   178,   179,   105,   106,   107,   108,   109,
     185,    -1,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   178,   179,
      -1,    -1,   182,   105,   106,   107,   108,   109,    -1,    -1,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,   178,   179,    -1,    -1,
     182,   105,   106,   107,   108,   109,    -1,    -1,   112,   113,
     114,   115,    -1,   117,   118,   119,   120,    -1,    -1,    -1,
     124,    -1,   126,   127,    -1,    -1,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   178,   179,    -1,    -1,   182,   105,
     106,   107,   108,   109,    -1,    -1,   112,   113,   114,   115,
      -1,   117,   118,   119,   120,    -1,    -1,    -1,   124,    -1,
     126,   127,    -1,    -1,    -1,    -1,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,   178,   179,    -1,    -1,   182,   105,   106,   107,
     108,   109,    -1,    -1,   112,   113,   114,   115,    -1,   117,
     118,   119,   120,    -1,    -1,    -1,   124,    -1,   126,   127,
      -1,    -1,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     178,   179,    -1,    -1,   182,   105,   106,   107,   108,   109,
      -1,    -1,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   178,   179,
      -1,    -1,   182,   105,   106,   107,   108,   109,    -1,    -1,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,   178,   179,    -1,    -1,
     182,   105,   106,   107,   108,   109,    -1,    -1,   112,   113,
     114,   115,    -1,   117,   118,   119,   120,    -1,    -1,    -1,
     124,    -1,   126,   127,    -1,    -1,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   178,   179,   180,   105,   106,   107,
     108,   109,    -1,    -1,   112,   113,   114,   115,    -1,   117,
     118,   119,   120,    -1,    -1,    -1,   124,    -1,   126,   127,
      -1,    -1,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     178,   179,   180,   105,   106,   107,   108,   109,    -1,    -1,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   178,   179,   180,   105,
     106,   107,   108,   109,    -1,    -1,   112,   113,   114,   115,
      -1,   117,   118,   119,   120,    -1,    -1,    -1,   124,    -1,
     126,   127,    -1,    -1,    -1,    -1,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,   179,   180,   105,   106,   107,   108,   109,
      20,    21,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    20,    21,    -1,    -1,   105,   106,   107,   108,   109,
     178,   179,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,    -1,
     130,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
     109,    -1,    -1,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,
      -1,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      20,    21,    -1,    -1,   105,   106,   107,   108,   109,   178,
     179,   112,   113,   114,   115,    -1,   117,   118,   119,   120,
      -1,    -1,    -1,   124,    -1,   126,   127,    -1,    -1,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,
      -1,    -1,   112,   113,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    20,
      21,    -1,    -1,   105,   106,   107,   108,   109,   178,   179,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,   127,    20,    21,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,   113,   114,   115,    -1,   117,   118,   119,   120,
      -1,    20,    21,   124,    -1,   126,   127,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,   109,    -1,    -1,   112,   113,
     114,   115,    -1,   117,   118,   119,   120,    -1,    -1,    -1,
     124,    -1,   126,   127,    -1,    -1,    -1,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   178,   179,   105,   106,    -1,    -1,
     109,    -1,    -1,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,
      -1,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
     159,   160,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
     179,   105,   106,   107,   108,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,   117,   118,   119,   120,    37,    -1,    -1,
     124,    -1,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,   137,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,   171,   172,    46,    -1,    -1,    -1,    -1,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,    -1,    46,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,    -1,    -1,    -1,   147,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   190,     0,     6,    29,    31,    33,    39,    47,    53,
      72,    93,   179,   185,   193,   201,   202,   206,   227,   231,
     247,   304,   309,   311,   317,   353,   355,    18,    19,   147,
     221,   222,   223,   141,   207,   208,   147,   168,   203,   204,
     147,   186,   307,   147,   183,   192,   356,   354,    33,    60,
     131,   147,   224,   225,   226,   239,     4,     5,     7,    35,
     315,    59,   302,   156,   155,   158,   155,   203,    21,    54,
     167,   178,   205,   156,   307,   308,   302,   147,   147,   147,
     131,   181,   155,   180,    54,    60,   232,   234,    54,    60,
     310,    54,    60,   316,    54,    60,   303,    14,    15,   141,
     145,   147,   148,   181,   195,   222,   141,   208,   147,   147,
     147,    24,    30,    46,    61,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,   239,   320,   322,   323,
     325,   328,   185,   187,   307,    54,    60,   191,   186,   186,
     147,   223,   225,    34,   107,   108,   147,   230,   233,   147,
      96,   313,   314,   186,   305,   149,   150,   194,    14,    15,
     141,   145,   147,   195,   219,   220,   205,   162,   162,   162,
     162,   181,   162,   162,   162,   162,   162,   162,    32,    57,
      58,   117,   156,   157,   161,   164,   179,   188,   185,   147,
     324,   147,   294,   295,   182,    37,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   118,   119,   125,   126,   127,   128,
     129,   132,   133,   134,   135,   136,   137,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   171,   172,   178,
     179,    34,    34,   181,   228,   186,   235,   158,   186,   147,
     318,   301,   138,   283,   149,   150,   151,   155,   182,   331,
     333,   335,   329,   147,   326,   337,   339,   341,   343,   345,
     347,   349,   351,    32,    32,   161,   179,   188,    14,    15,
      16,    17,    18,    26,    38,    42,    45,    48,    52,    62,
      72,    94,   107,   108,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   148,   164,   165,   166,   168,   171,   172,
     180,   181,   183,   184,   197,   199,   235,   239,   241,   249,
     256,   259,   262,   266,   267,   270,   271,   272,   277,   280,
     320,   357,   361,   366,   368,   370,   185,   158,   185,   180,
     180,   182,   283,   290,   291,   158,   229,   237,    67,    71,
      88,    89,    90,    91,   321,   306,   158,   312,   186,   187,
     283,   223,   147,   300,   319,   219,   328,   328,   328,   328,
     182,   324,   328,   228,   328,   228,   328,   228,   147,   292,
     293,   328,   295,   180,   328,   357,   181,   162,   181,   162,
     162,   181,   162,   181,   162,   280,   280,    12,   328,    12,
     328,   280,   363,   367,   196,   280,   280,   280,   239,   280,
     280,   280,   184,   147,   181,   227,    20,    21,   105,   106,
     107,   108,   109,   112,   113,   114,   115,   117,   118,   119,
     120,   124,   126,   127,   132,   133,   134,   157,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   178,   179,
     180,   181,   147,   187,   328,   187,   294,    47,   297,   319,
     182,   185,   328,     1,     8,     9,    10,    12,    25,    27,
      28,    37,    40,    49,    51,    55,    56,    62,    95,   122,
     123,   185,   187,   209,   210,   213,   214,   215,   216,   217,
     218,   236,   238,   240,   242,   243,   244,   245,   246,   247,
     248,   268,   269,   280,   186,   147,   187,   239,   288,   300,
     180,    41,   155,   158,   161,   299,   163,   185,   163,   163,
     163,   185,   163,   229,   163,   229,   163,   229,   158,   163,
     185,   163,   185,   181,   239,   257,   280,   250,   252,   280,
     254,   280,   328,   319,    50,   147,   164,   181,   280,   358,
     359,   360,   362,   363,   364,   365,   319,   181,   359,   365,
     130,   185,   187,   151,   152,   194,   200,   182,   162,   239,
     179,   182,   260,   280,   138,   265,    18,   147,   147,   280,
     280,   280,   280,   280,   280,   147,   280,   147,   280,   280,
     280,   280,   280,   280,   280,   280,    21,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   118,   119,
     147,   178,   179,   278,   280,   182,   260,   297,   116,   156,
     158,   161,   296,   290,   280,   319,   116,   185,   238,   268,
     280,   235,   280,   280,   185,   141,    55,   280,   235,   116,
     238,   280,   184,   266,   266,    36,   185,   185,   280,   185,
     185,   185,   300,   121,   185,    97,    98,    99,   100,   101,
     102,   103,   104,   110,   111,   116,   125,   128,   129,   135,
     136,   137,   156,   306,   156,   185,   179,   187,   227,   283,
     287,   180,   147,   147,   328,   116,   125,   156,   298,   332,
     328,   336,   330,   327,   338,   163,   342,   163,   346,   163,
     328,   350,   292,   352,   182,   260,   162,   280,   328,   182,
     328,   328,   182,   328,   182,   163,    19,   266,   125,   296,
     182,   155,   185,   360,   180,   155,   180,   185,    19,   182,
     360,   187,   280,   363,   187,   280,   149,   198,   273,   275,
     147,   358,   155,   182,   116,   125,   156,   161,   263,   264,
     228,   162,   181,   180,   147,   158,   147,   280,   147,   280,
       1,   180,   182,   328,   238,   280,   235,    19,   238,   280,
     116,   156,   185,    13,   235,   158,   141,   238,   280,   185,
     184,   186,   235,   266,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   122,   123,   280,   280,   280,   280,
     280,   280,   280,   122,   123,   280,   187,   280,   226,     7,
      43,    96,   284,   185,    41,   156,   185,   298,   238,   280,
     163,   340,   344,   348,   182,   147,   182,   163,   163,   163,
     163,   265,   260,   280,   280,   359,   360,   147,   358,   180,
     180,   280,   180,   364,   260,   359,   187,   180,   153,   149,
     328,   228,   180,   280,   147,   147,   147,   147,   155,   180,
     229,   281,   182,   260,   280,   180,   180,   279,   156,   296,
     260,   185,   235,   185,   237,    11,    22,    23,   211,   212,
     280,   280,   280,   280,   185,   180,    54,    60,   286,   286,
     147,   238,   280,   185,   334,   163,   185,   258,   251,   253,
     255,   181,   185,   360,   180,   125,   296,   180,   185,   360,
     180,   163,   229,   182,   263,   180,   130,   235,   261,   328,
     182,   280,   185,   187,   235,   280,   185,    44,   284,   297,
     185,   280,   147,   280,   280,   280,   182,   280,   280,   180,
     180,   280,   280,   280,   187,   274,   163,   116,   280,   163,
     235,   235,   289,    32,   285,   182,   163,   182,   185,   369,
     180,   369,   180,   239,   276,   280,   282,   212,   233,   233,
     280,    50,   180,   187,   239,   185,   235,   182,   280,   180,
     180
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   189,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   191,   191,   191,   192,   192,
     193,   194,   194,   194,   194,   195,   196,   196,   196,   197,
     198,   198,   200,   199,   201,   202,   203,   203,   203,   203,
     204,   204,   205,   205,   206,   207,   207,   208,   208,   209,
     210,   210,   211,   211,   212,   212,   212,   213,   213,   214,
     215,   216,   217,   218,   219,   219,   219,   219,   219,   219,
     220,   220,   221,   221,   221,   222,   222,   222,   222,   222,
     222,   222,   222,   223,   223,   224,   224,   224,   225,   225,
     226,   226,   227,   227,   228,   228,   228,   229,   229,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   231,   232,
     232,   232,   233,   234,   235,   235,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   237,   237,   237,   238,   238,
     238,   239,   239,   239,   240,   241,   241,   241,   241,   242,
     243,   244,   244,   244,   244,   244,   245,   245,   245,   245,
     246,   247,   247,   248,   250,   251,   249,   252,   253,   249,
     254,   255,   249,   257,   258,   256,   259,   259,   259,   260,
     260,   261,   261,   261,   262,   262,   262,   263,   263,   263,
     263,   264,   264,   265,   265,   266,   266,   267,   267,   267,
     267,   267,   267,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   269,   269,   269,   269,   270,   271,   271,   272,
     273,   274,   272,   275,   276,   272,   277,   277,   278,   279,
     277,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   281,   282,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   283,   283,   284,   284,   284,
     285,   285,   286,   286,   286,   287,   288,   288,   289,   288,
     288,   288,   290,   290,   291,   291,   292,   292,   293,   293,
     294,   295,   295,   296,   296,   297,   297,   297,   297,   297,
     297,   298,   298,   298,   299,   299,   300,   300,   300,   300,
     300,   301,   301,   302,   302,   303,   303,   303,   304,   305,
     304,   306,   306,   306,   307,   308,   308,   309,   309,   310,
     310,   310,   311,   311,   312,   312,   313,   313,   314,   315,
     315,   316,   316,   316,   318,   317,   319,   319,   319,   319,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   321,   321,   321,   321,   321,
     321,   322,   323,   323,   324,   324,   326,   327,   325,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   329,   330,   328,   328,
     331,   332,   328,   333,   334,   328,   335,   336,   328,   328,
     337,   338,   328,   339,   340,   328,   328,   341,   342,   328,
     343,   344,   328,   328,   345,   346,   328,   347,   348,   328,
     349,   350,   328,   351,   352,   328,   354,   353,   356,   355,
     357,   357,   357,   357,   358,   358,   358,   358,   359,   359,
     360,   360,   361,   361,   361,   361,   361,   361,   362,   362,
     363,   364,   364,   365,   365,   366,   366,   367,   367,   368,
     369,   369,   370,   370
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
       1,     0,     1,     1,     0,     8,     1,     3,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     3,     0,     0,     6,     1,
       1,     1,     1,     4,     3,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     8,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     0,     7,     0,     7,
       1,     1,     1,     1,     3,     3,     5,     5,     1,     3,
       0,     2,     6,     5,     7,     8,     6,     8,     1,     3,
       3,     1,     1,     1,     3,     5,     5,     1,     3,     4,
       0,     3,    10,    10
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
    case 147: /* "name"  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3485 "ds_parser.cpp"
        break;

    case 192: /* module_name  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3491 "ds_parser.cpp"
        break;

    case 194: /* character_sequence  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3497 "ds_parser.cpp"
        break;

    case 195: /* string_constant  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3503 "ds_parser.cpp"
        break;

    case 196: /* string_builder_body  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3509 "ds_parser.cpp"
        break;

    case 197: /* string_builder  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3515 "ds_parser.cpp"
        break;

    case 199: /* expr_reader  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3521 "ds_parser.cpp"
        break;

    case 203: /* require_module_name  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3527 "ds_parser.cpp"
        break;

    case 209: /* expression_label  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3533 "ds_parser.cpp"
        break;

    case 210: /* expression_goto  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3539 "ds_parser.cpp"
        break;

    case 212: /* expression_else  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3545 "ds_parser.cpp"
        break;

    case 214: /* expression_if_then_else  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3551 "ds_parser.cpp"
        break;

    case 215: /* expression_for_loop  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3557 "ds_parser.cpp"
        break;

    case 216: /* expression_unsafe  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3563 "ds_parser.cpp"
        break;

    case 217: /* expression_while_loop  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3569 "ds_parser.cpp"
        break;

    case 218: /* expression_with  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3575 "ds_parser.cpp"
        break;

    case 219: /* annotation_argument_value  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3581 "ds_parser.cpp"
        break;

    case 220: /* annotation_argument_value_list  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3587 "ds_parser.cpp"
        break;

    case 221: /* annotation_argument_name  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3593 "ds_parser.cpp"
        break;

    case 222: /* annotation_argument  */
#line 155 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3599 "ds_parser.cpp"
        break;

    case 223: /* annotation_argument_list  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3605 "ds_parser.cpp"
        break;

    case 224: /* annotation_declaration_name  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3611 "ds_parser.cpp"
        break;

    case 225: /* annotation_declaration  */
#line 157 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3617 "ds_parser.cpp"
        break;

    case 226: /* annotation_list  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3623 "ds_parser.cpp"
        break;

    case 227: /* optional_annotation_list  */
#line 158 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3629 "ds_parser.cpp"
        break;

    case 228: /* optional_function_argument_list  */
#line 152 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3635 "ds_parser.cpp"
        break;

    case 229: /* optional_function_type  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3641 "ds_parser.cpp"
        break;

    case 230: /* function_name  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3647 "ds_parser.cpp"
        break;

    case 235: /* expression_block  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3653 "ds_parser.cpp"
        break;

    case 236: /* expression_any  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3659 "ds_parser.cpp"
        break;

    case 237: /* expressions  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3665 "ds_parser.cpp"
        break;

    case 238: /* expr_pipe  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3671 "ds_parser.cpp"
        break;

    case 239: /* name_in_namespace  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3677 "ds_parser.cpp"
        break;

    case 240: /* expression_delete  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3683 "ds_parser.cpp"
        break;

    case 241: /* expr_new  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3689 "ds_parser.cpp"
        break;

    case 242: /* expression_break  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3695 "ds_parser.cpp"
        break;

    case 243: /* expression_continue  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3701 "ds_parser.cpp"
        break;

    case 244: /* expression_return  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3707 "ds_parser.cpp"
        break;

    case 245: /* expression_yield  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3713 "ds_parser.cpp"
        break;

    case 246: /* expression_try_catch  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3719 "ds_parser.cpp"
        break;

    case 248: /* expression_let  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3725 "ds_parser.cpp"
        break;

    case 249: /* expr_cast  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3731 "ds_parser.cpp"
        break;

    case 256: /* expr_type_decl  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3737 "ds_parser.cpp"
        break;

    case 259: /* expr_type_info  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3743 "ds_parser.cpp"
        break;

    case 260: /* expr_list  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3749 "ds_parser.cpp"
        break;

    case 261: /* block_or_simple_block  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3755 "ds_parser.cpp"
        break;

    case 263: /* capture_entry  */
#line 161 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3761 "ds_parser.cpp"
        break;

    case 264: /* capture_list  */
#line 162 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3767 "ds_parser.cpp"
        break;

    case 265: /* optional_capture_list  */
#line 162 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3773 "ds_parser.cpp"
        break;

    case 266: /* expr_block  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3779 "ds_parser.cpp"
        break;

    case 267: /* expr_numeric_const  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3785 "ds_parser.cpp"
        break;

    case 268: /* expr_assign  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3791 "ds_parser.cpp"
        break;

    case 269: /* expr_assign_pipe  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3797 "ds_parser.cpp"
        break;

    case 270: /* expr_named_call  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3803 "ds_parser.cpp"
        break;

    case 271: /* expr_method_call  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3809 "ds_parser.cpp"
        break;

    case 272: /* func_addr_expr  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3815 "ds_parser.cpp"
        break;

    case 277: /* expr_field  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3821 "ds_parser.cpp"
        break;

    case 280: /* expr  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3827 "ds_parser.cpp"
        break;

    case 283: /* optional_field_annotation  */
#line 156 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3833 "ds_parser.cpp"
        break;

    case 287: /* structure_variable_declaration  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3839 "ds_parser.cpp"
        break;

    case 288: /* struct_variable_declaration_list  */
#line 152 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3845 "ds_parser.cpp"
        break;

    case 290: /* function_argument_declaration  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3851 "ds_parser.cpp"
        break;

    case 291: /* function_argument_list  */
#line 152 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3857 "ds_parser.cpp"
        break;

    case 292: /* tuple_type  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3863 "ds_parser.cpp"
        break;

    case 293: /* tuple_type_list  */
#line 152 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3869 "ds_parser.cpp"
        break;

    case 294: /* variant_type  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3875 "ds_parser.cpp"
        break;

    case 295: /* variant_type_list  */
#line 152 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3881 "ds_parser.cpp"
        break;

    case 297: /* variable_declaration  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3887 "ds_parser.cpp"
        break;

    case 300: /* let_variable_declaration  */
#line 151 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3893 "ds_parser.cpp"
        break;

    case 301: /* global_variable_declaration_list  */
#line 152 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3899 "ds_parser.cpp"
        break;

    case 306: /* enum_list  */
#line 160 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3905 "ds_parser.cpp"
        break;

    case 312: /* optional_structure_parent  */
#line 148 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3911 "ds_parser.cpp"
        break;

    case 319: /* variable_name_with_pos_list  */
#line 150 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3917 "ds_parser.cpp"
        break;

    case 322: /* structure_type_declaration  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3923 "ds_parser.cpp"
        break;

    case 323: /* auto_type_declaration  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3929 "ds_parser.cpp"
        break;

    case 324: /* bitfield_bits  */
#line 149 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3935 "ds_parser.cpp"
        break;

    case 325: /* bitfield_type_declaration  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3941 "ds_parser.cpp"
        break;

    case 328: /* type_declaration  */
#line 153 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3947 "ds_parser.cpp"
        break;

    case 357: /* make_decl  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3953 "ds_parser.cpp"
        break;

    case 358: /* make_struct_fields  */
#line 159 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3959 "ds_parser.cpp"
        break;

    case 359: /* make_struct_dim  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3965 "ds_parser.cpp"
        break;

    case 360: /* optional_block  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3971 "ds_parser.cpp"
        break;

    case 361: /* make_struct_decl  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3977 "ds_parser.cpp"
        break;

    case 362: /* make_tuple  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3983 "ds_parser.cpp"
        break;

    case 363: /* make_map_tuple  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3989 "ds_parser.cpp"
        break;

    case 364: /* make_any_tuple  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3995 "ds_parser.cpp"
        break;

    case 365: /* make_dim  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4001 "ds_parser.cpp"
        break;

    case 366: /* make_dim_decl  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4007 "ds_parser.cpp"
        break;

    case 367: /* make_table  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4013 "ds_parser.cpp"
        break;

    case 368: /* make_table_decl  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4019 "ds_parser.cpp"
        break;

    case 369: /* array_comprehension_where  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4025 "ds_parser.cpp"
        break;

    case 370: /* array_comprehension  */
#line 154 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 4031 "ds_parser.cpp"
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
#line 499 "ds_parser.ypp"
    { (yyval.b) = g_Program->policies.default_module_public; }
#line 4323 "ds_parser.cpp"
    break;

  case 16:
#line 500 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4329 "ds_parser.cpp"
    break;

  case 17:
#line 501 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4335 "ds_parser.cpp"
    break;

  case 18:
#line 505 "ds_parser.ypp"
    { (yyval.s) = new string("$"); }
#line 4341 "ds_parser.cpp"
    break;

  case 19:
#line 506 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4347 "ds_parser.cpp"
    break;

  case 20:
#line 510 "ds_parser.ypp"
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
#line 4366 "ds_parser.cpp"
    break;

  case 21:
#line 527 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4372 "ds_parser.cpp"
    break;

  case 22:
#line 528 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4378 "ds_parser.cpp"
    break;

  case 23:
#line 529 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4384 "ds_parser.cpp"
    break;

  case 24:
#line 530 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4390 "ds_parser.cpp"
    break;

  case 25:
#line 534 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4396 "ds_parser.cpp"
    break;

  case 26:
#line 538 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4406 "ds_parser.cpp"
    break;

  case 27:
#line 543 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4420 "ds_parser.cpp"
    break;

  case 28:
#line 552 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4430 "ds_parser.cpp"
    break;

  case 29:
#line 560 "ds_parser.ypp"
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
#line 4448 "ds_parser.cpp"
    break;

  case 30:
#line 576 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4458 "ds_parser.cpp"
    break;

  case 31:
#line 581 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4468 "ds_parser.cpp"
    break;

  case 32:
#line 589 "ds_parser.ypp"
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
#line 4495 "ds_parser.cpp"
    break;

  case 33:
#line 610 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4507 "ds_parser.cpp"
    break;

  case 34:
#line 619 "ds_parser.ypp"
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
#line 4531 "ds_parser.cpp"
    break;

  case 36:
#line 645 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4539 "ds_parser.cpp"
    break;

  case 37:
#line 648 "ds_parser.ypp"
    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4548 "ds_parser.cpp"
    break;

  case 38:
#line 652 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4559 "ds_parser.cpp"
    break;

  case 39:
#line 658 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4570 "ds_parser.cpp"
    break;

  case 40:
#line 667 "ds_parser.ypp"
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
#line 4598 "ds_parser.cpp"
    break;

  case 41:
#line 690 "ds_parser.ypp"
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
#line 4625 "ds_parser.cpp"
    break;

  case 42:
#line 715 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4631 "ds_parser.cpp"
    break;

  case 43:
#line 716 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4637 "ds_parser.cpp"
    break;

  case 47:
#line 729 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4645 "ds_parser.cpp"
    break;

  case 48:
#line 732 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4653 "ds_parser.cpp"
    break;

  case 49:
#line 738 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4661 "ds_parser.cpp"
    break;

  case 50:
#line 744 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4669 "ds_parser.cpp"
    break;

  case 51:
#line 747 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4677 "ds_parser.cpp"
    break;

  case 52:
#line 753 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4683 "ds_parser.cpp"
    break;

  case 53:
#line 754 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4689 "ds_parser.cpp"
    break;

  case 54:
#line 758 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4695 "ds_parser.cpp"
    break;

  case 55:
#line 759 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4701 "ds_parser.cpp"
    break;

  case 56:
#line 760 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4712 "ds_parser.cpp"
    break;

  case 57:
#line 769 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4718 "ds_parser.cpp"
    break;

  case 58:
#line 770 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4724 "ds_parser.cpp"
    break;

  case 59:
#line 774 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4735 "ds_parser.cpp"
    break;

  case 60:
#line 783 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->visibility = tokAt((yylsp[0]));
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
#line 4754 "ds_parser.cpp"
    break;

  case 61:
#line 800 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4764 "ds_parser.cpp"
    break;

  case 62:
#line 808 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4776 "ds_parser.cpp"
    break;

  case 63:
#line 818 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4787 "ds_parser.cpp"
    break;

  case 64:
#line 827 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4793 "ds_parser.cpp"
    break;

  case 65:
#line 828 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4799 "ds_parser.cpp"
    break;

  case 66:
#line 829 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4805 "ds_parser.cpp"
    break;

  case 67:
#line 830 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4811 "ds_parser.cpp"
    break;

  case 68:
#line 831 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4817 "ds_parser.cpp"
    break;

  case 69:
#line 832 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4823 "ds_parser.cpp"
    break;

  case 70:
#line 836 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4833 "ds_parser.cpp"
    break;

  case 71:
#line 841 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4843 "ds_parser.cpp"
    break;

  case 72:
#line 849 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4849 "ds_parser.cpp"
    break;

  case 73:
#line 850 "ds_parser.ypp"
    { (yyval.s) = new string("type"); }
#line 4855 "ds_parser.cpp"
    break;

  case 74:
#line 851 "ds_parser.ypp"
    { (yyval.s) = new string("in"); }
#line 4861 "ds_parser.cpp"
    break;

  case 75:
#line 855 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4867 "ds_parser.cpp"
    break;

  case 76:
#line 856 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4873 "ds_parser.cpp"
    break;

  case 77:
#line 857 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4879 "ds_parser.cpp"
    break;

  case 78:
#line 858 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4885 "ds_parser.cpp"
    break;

  case 79:
#line 859 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4891 "ds_parser.cpp"
    break;

  case 80:
#line 860 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt((yylsp[-2]))); delete (yyvsp[-2].s); }
#line 4897 "ds_parser.cpp"
    break;

  case 81:
#line 861 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt((yylsp[0]))); delete (yyvsp[0].s); }
#line 4903 "ds_parser.cpp"
    break;

  case 82:
#line 862 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt((yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 4911 "ds_parser.cpp"
    break;

  case 83:
#line 868 "ds_parser.ypp"
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
#line 4930 "ds_parser.cpp"
    break;

  case 84:
#line 882 "ds_parser.ypp"
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
#line 4949 "ds_parser.cpp"
    break;

  case 85:
#line 899 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4955 "ds_parser.cpp"
    break;

  case 86:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4961 "ds_parser.cpp"
    break;

  case 87:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("private"); }
#line 4967 "ds_parser.cpp"
    break;

  case 88:
#line 905 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt((yylsp[0]));
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4980 "ds_parser.cpp"
    break;

  case 89:
#line 913 "ds_parser.ypp"
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
#line 4995 "ds_parser.cpp"
    break;

  case 90:
#line 926 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5004 "ds_parser.cpp"
    break;

  case 91:
#line 930 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5013 "ds_parser.cpp"
    break;

  case 92:
#line 937 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 5019 "ds_parser.cpp"
    break;

  case 93:
#line 938 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 5025 "ds_parser.cpp"
    break;

  case 94:
#line 942 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5031 "ds_parser.cpp"
    break;

  case 95:
#line 943 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 5037 "ds_parser.cpp"
    break;

  case 96:
#line 944 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5043 "ds_parser.cpp"
    break;

  case 97:
#line 948 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5051 "ds_parser.cpp"
    break;

  case 98:
#line 951 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5060 "ds_parser.cpp"
    break;

  case 99:
#line 958 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5069 "ds_parser.cpp"
    break;

  case 100:
#line 962 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 5075 "ds_parser.cpp"
    break;

  case 101:
#line 963 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 5081 "ds_parser.cpp"
    break;

  case 102:
#line 964 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 5087 "ds_parser.cpp"
    break;

  case 103:
#line 965 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 5093 "ds_parser.cpp"
    break;

  case 104:
#line 966 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 5099 "ds_parser.cpp"
    break;

  case 105:
#line 967 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 5105 "ds_parser.cpp"
    break;

  case 106:
#line 968 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 5111 "ds_parser.cpp"
    break;

  case 107:
#line 969 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 5117 "ds_parser.cpp"
    break;

  case 108:
#line 970 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 5123 "ds_parser.cpp"
    break;

  case 109:
#line 971 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 5129 "ds_parser.cpp"
    break;

  case 110:
#line 972 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 5135 "ds_parser.cpp"
    break;

  case 111:
#line 973 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 5141 "ds_parser.cpp"
    break;

  case 112:
#line 974 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 5147 "ds_parser.cpp"
    break;

  case 113:
#line 975 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 5153 "ds_parser.cpp"
    break;

  case 114:
#line 976 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5159 "ds_parser.cpp"
    break;

  case 115:
#line 977 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5165 "ds_parser.cpp"
    break;

  case 116:
#line 978 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5171 "ds_parser.cpp"
    break;

  case 117:
#line 979 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5177 "ds_parser.cpp"
    break;

  case 118:
#line 980 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5183 "ds_parser.cpp"
    break;

  case 119:
#line 981 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5189 "ds_parser.cpp"
    break;

  case 120:
#line 982 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5195 "ds_parser.cpp"
    break;

  case 121:
#line 983 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5201 "ds_parser.cpp"
    break;

  case 122:
#line 984 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5207 "ds_parser.cpp"
    break;

  case 123:
#line 985 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5213 "ds_parser.cpp"
    break;

  case 124:
#line 986 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5219 "ds_parser.cpp"
    break;

  case 125:
#line 987 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5225 "ds_parser.cpp"
    break;

  case 126:
#line 988 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5231 "ds_parser.cpp"
    break;

  case 127:
#line 989 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5237 "ds_parser.cpp"
    break;

  case 128:
#line 990 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5243 "ds_parser.cpp"
    break;

  case 129:
#line 991 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5249 "ds_parser.cpp"
    break;

  case 130:
#line 992 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5255 "ds_parser.cpp"
    break;

  case 131:
#line 993 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5261 "ds_parser.cpp"
    break;

  case 132:
#line 994 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5267 "ds_parser.cpp"
    break;

  case 133:
#line 995 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5273 "ds_parser.cpp"
    break;

  case 134:
#line 996 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5279 "ds_parser.cpp"
    break;

  case 135:
#line 997 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5285 "ds_parser.cpp"
    break;

  case 136:
#line 998 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5291 "ds_parser.cpp"
    break;

  case 137:
#line 999 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5297 "ds_parser.cpp"
    break;

  case 138:
#line 1000 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5303 "ds_parser.cpp"
    break;

  case 139:
#line 1001 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5309 "ds_parser.cpp"
    break;

  case 140:
#line 1002 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5315 "ds_parser.cpp"
    break;

  case 141:
#line 1003 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5321 "ds_parser.cpp"
    break;

  case 142:
#line 1004 "ds_parser.ypp"
    { (yyval.s) = new string("[]"); }
#line 5327 "ds_parser.cpp"
    break;

  case 143:
#line 1005 "ds_parser.ypp"
    { (yyval.s) = new string("?[]"); }
#line 5333 "ds_parser.cpp"
    break;

  case 144:
#line 1006 "ds_parser.ypp"
    { (yyval.s) = new string("."); }
#line 5339 "ds_parser.cpp"
    break;

  case 145:
#line 1007 "ds_parser.ypp"
    { (yyval.s) = new string("?."); }
#line 5345 "ds_parser.cpp"
    break;

  case 146:
#line 1008 "ds_parser.ypp"
    { (yyval.s) = new string("clone"); }
#line 5351 "ds_parser.cpp"
    break;

  case 147:
#line 1009 "ds_parser.ypp"
    { (yyval.s) = new string("finalize"); }
#line 5357 "ds_parser.cpp"
    break;

  case 148:
#line 1013 "ds_parser.ypp"
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
#line 5381 "ds_parser.cpp"
    break;

  case 149:
#line 1035 "ds_parser.ypp"
    { (yyval.b) = g_thisStructure ? !g_thisStructure->privateStructure : g_Program->thisModule->isPublic; }
#line 5387 "ds_parser.cpp"
    break;

  case 150:
#line 1036 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5393 "ds_parser.cpp"
    break;

  case 151:
#line 1037 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5399 "ds_parser.cpp"
    break;

  case 152:
#line 1041 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-2]));
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
#line 5440 "ds_parser.cpp"
    break;

  case 153:
#line 1080 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5450 "ds_parser.cpp"
    break;

  case 154:
#line 1088 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5459 "ds_parser.cpp"
    break;

  case 155:
#line 1092 "ds_parser.ypp"
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
#line 1103 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5478 "ds_parser.cpp"
    break;

  case 157:
#line 1104 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5484 "ds_parser.cpp"
    break;

  case 158:
#line 1105 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5490 "ds_parser.cpp"
    break;

  case 159:
#line 1106 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5496 "ds_parser.cpp"
    break;

  case 160:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5502 "ds_parser.cpp"
    break;

  case 161:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5508 "ds_parser.cpp"
    break;

  case 162:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5514 "ds_parser.cpp"
    break;

  case 163:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5520 "ds_parser.cpp"
    break;

  case 164:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5526 "ds_parser.cpp"
    break;

  case 165:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5532 "ds_parser.cpp"
    break;

  case 166:
#line 1113 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5538 "ds_parser.cpp"
    break;

  case 167:
#line 1114 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5544 "ds_parser.cpp"
    break;

  case 168:
#line 1115 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5550 "ds_parser.cpp"
    break;

  case 169:
#line 1116 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5556 "ds_parser.cpp"
    break;

  case 170:
#line 1117 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5562 "ds_parser.cpp"
    break;

  case 171:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5568 "ds_parser.cpp"
    break;

  case 172:
#line 1119 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5574 "ds_parser.cpp"
    break;

  case 173:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5580 "ds_parser.cpp"
    break;

  case 174:
#line 1121 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5586 "ds_parser.cpp"
    break;

  case 175:
#line 1125 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5596 "ds_parser.cpp"
    break;

  case 176:
#line 1130 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5607 "ds_parser.cpp"
    break;

  case 177:
#line 1136 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5615 "ds_parser.cpp"
    break;

  case 178:
#line 1142 "ds_parser.ypp"
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
#line 1176 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5662 "ds_parser.cpp"
    break;

  case 180:
#line 1179 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5670 "ds_parser.cpp"
    break;

  case 181:
#line 1185 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5676 "ds_parser.cpp"
    break;

  case 182:
#line 1186 "ds_parser.ypp"
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
#line 1197 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5698 "ds_parser.cpp"
    break;

  case 184:
#line 1201 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5706 "ds_parser.cpp"
    break;

  case 185:
#line 1207 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5714 "ds_parser.cpp"
    break;

  case 186:
#line 1210 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5722 "ds_parser.cpp"
    break;

  case 187:
#line 1213 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5731 "ds_parser.cpp"
    break;

  case 188:
#line 1217 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5739 "ds_parser.cpp"
    break;

  case 189:
#line 1223 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5745 "ds_parser.cpp"
    break;

  case 190:
#line 1227 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5751 "ds_parser.cpp"
    break;

  case 191:
#line 1231 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5759 "ds_parser.cpp"
    break;

  case 192:
#line 1234 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5767 "ds_parser.cpp"
    break;

  case 193:
#line 1237 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5777 "ds_parser.cpp"
    break;

  case 194:
#line 1242 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5785 "ds_parser.cpp"
    break;

  case 195:
#line 1245 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5795 "ds_parser.cpp"
    break;

  case 196:
#line 1253 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5803 "ds_parser.cpp"
    break;

  case 197:
#line 1256 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5813 "ds_parser.cpp"
    break;

  case 198:
#line 1261 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5821 "ds_parser.cpp"
    break;

  case 199:
#line 1264 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5831 "ds_parser.cpp"
    break;

  case 200:
#line 1272 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5839 "ds_parser.cpp"
    break;

  case 201:
#line 1278 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5845 "ds_parser.cpp"
    break;

  case 202:
#line 1279 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5851 "ds_parser.cpp"
    break;

  case 203:
#line 1283 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-1]));
        pLet->atInit = tokAt((yylsp[0]));
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
#line 5888 "ds_parser.cpp"
    break;

  case 204:
#line 1318 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5894 "ds_parser.cpp"
    break;

  case 205:
#line 1318 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5900 "ds_parser.cpp"
    break;

  case 206:
#line 1318 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5908 "ds_parser.cpp"
    break;

  case 207:
#line 1321 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5914 "ds_parser.cpp"
    break;

  case 208:
#line 1321 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5920 "ds_parser.cpp"
    break;

  case 209:
#line 1321 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5930 "ds_parser.cpp"
    break;

  case 210:
#line 1326 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5936 "ds_parser.cpp"
    break;

  case 211:
#line 1326 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5942 "ds_parser.cpp"
    break;

  case 212:
#line 1326 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5952 "ds_parser.cpp"
    break;

  case 213:
#line 1334 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5958 "ds_parser.cpp"
    break;

  case 214:
#line 1334 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5964 "ds_parser.cpp"
    break;

  case 215:
#line 1334 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt((yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5972 "ds_parser.cpp"
    break;

  case 216:
#line 1340 "ds_parser.ypp"
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
#line 5987 "ds_parser.cpp"
    break;

  case 217:
#line 1350 "ds_parser.ypp"
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
#line 6003 "ds_parser.cpp"
    break;

  case 218:
#line 1361 "ds_parser.ypp"
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
#line 6020 "ds_parser.cpp"
    break;

  case 219:
#line 1376 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6028 "ds_parser.cpp"
    break;

  case 220:
#line 1379 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6036 "ds_parser.cpp"
    break;

  case 221:
#line 1385 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6042 "ds_parser.cpp"
    break;

  case 222:
#line 1386 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6054 "ds_parser.cpp"
    break;

  case 223:
#line 1393 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6067 "ds_parser.cpp"
    break;

  case 224:
#line 1404 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 6073 "ds_parser.cpp"
    break;

  case 225:
#line 1405 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 6079 "ds_parser.cpp"
    break;

  case 226:
#line 1406 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 6085 "ds_parser.cpp"
    break;

  case 227:
#line 1410 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6091 "ds_parser.cpp"
    break;

  case 228:
#line 1411 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6097 "ds_parser.cpp"
    break;

  case 229:
#line 1412 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6103 "ds_parser.cpp"
    break;

  case 230:
#line 1413 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6109 "ds_parser.cpp"
    break;

  case 231:
#line 1417 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6119 "ds_parser.cpp"
    break;

  case 232:
#line 1422 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6129 "ds_parser.cpp"
    break;

  case 233:
#line 1430 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 6135 "ds_parser.cpp"
    break;

  case 234:
#line 1431 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6141 "ds_parser.cpp"
    break;

  case 235:
#line 1435 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6151 "ds_parser.cpp"
    break;

  case 236:
#line 1441 "ds_parser.ypp"
    {
        auto mkb = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-5].i)==1, (yyvsp[-5].i)==2);
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
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6223 "ds_parser.cpp"
    break;

  case 238:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6229 "ds_parser.cpp"
    break;

  case 239:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6235 "ds_parser.cpp"
    break;

  case 240:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6241 "ds_parser.cpp"
    break;

  case 241:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6247 "ds_parser.cpp"
    break;

  case 242:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6253 "ds_parser.cpp"
    break;

  case 243:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6259 "ds_parser.cpp"
    break;

  case 244:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6265 "ds_parser.cpp"
    break;

  case 245:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6271 "ds_parser.cpp"
    break;

  case 246:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6277 "ds_parser.cpp"
    break;

  case 247:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6283 "ds_parser.cpp"
    break;

  case 248:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6289 "ds_parser.cpp"
    break;

  case 249:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6295 "ds_parser.cpp"
    break;

  case 250:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6301 "ds_parser.cpp"
    break;

  case 251:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6307 "ds_parser.cpp"
    break;

  case 252:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp"
    break;

  case 253:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6319 "ds_parser.cpp"
    break;

  case 254:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6325 "ds_parser.cpp"
    break;

  case 255:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6331 "ds_parser.cpp"
    break;

  case 256:
#line 1527 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp"
    break;

  case 257:
#line 1528 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6343 "ds_parser.cpp"
    break;

  case 258:
#line 1529 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6349 "ds_parser.cpp"
    break;

  case 259:
#line 1530 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6355 "ds_parser.cpp"
    break;

  case 260:
#line 1531 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6361 "ds_parser.cpp"
    break;

  case 261:
#line 1532 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6367 "ds_parser.cpp"
    break;

  case 262:
#line 1536 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6373 "ds_parser.cpp"
    break;

  case 263:
#line 1537 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6379 "ds_parser.cpp"
    break;

  case 264:
#line 1538 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6385 "ds_parser.cpp"
    break;

  case 265:
#line 1539 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6391 "ds_parser.cpp"
    break;

  case 266:
#line 1543 "ds_parser.ypp"
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
#line 1554 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6413 "ds_parser.cpp"
    break;

  case 268:
#line 1559 "ds_parser.ypp"
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
#line 1569 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6434 "ds_parser.cpp"
    break;

  case 270:
#line 1573 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6440 "ds_parser.cpp"
    break;

  case 271:
#line 1573 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6446 "ds_parser.cpp"
    break;

  case 272:
#line 1573 "ds_parser.ypp"
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
#line 1580 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6464 "ds_parser.cpp"
    break;

  case 274:
#line 1580 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6470 "ds_parser.cpp"
    break;

  case 275:
#line 1580 "ds_parser.ypp"
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
#line 1594 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6495 "ds_parser.cpp"
    break;

  case 277:
#line 1598 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6504 "ds_parser.cpp"
    break;

  case 278:
#line 1602 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6510 "ds_parser.cpp"
    break;

  case 279:
#line 1602 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6516 "ds_parser.cpp"
    break;

  case 280:
#line 1602 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6525 "ds_parser.cpp"
    break;

  case 281:
#line 1609 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6531 "ds_parser.cpp"
    break;

  case 282:
#line 1610 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6537 "ds_parser.cpp"
    break;

  case 283:
#line 1611 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6543 "ds_parser.cpp"
    break;

  case 284:
#line 1612 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6549 "ds_parser.cpp"
    break;

  case 285:
#line 1613 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6555 "ds_parser.cpp"
    break;

  case 286:
#line 1614 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6561 "ds_parser.cpp"
    break;

  case 287:
#line 1615 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6567 "ds_parser.cpp"
    break;

  case 288:
#line 1616 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6573 "ds_parser.cpp"
    break;

  case 289:
#line 1617 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6579 "ds_parser.cpp"
    break;

  case 290:
#line 1618 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6585 "ds_parser.cpp"
    break;

  case 291:
#line 1619 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6591 "ds_parser.cpp"
    break;

  case 292:
#line 1620 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 293:
#line 1621 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 294:
#line 1622 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 295:
#line 1623 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6615 "ds_parser.cpp"
    break;

  case 296:
#line 1624 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6621 "ds_parser.cpp"
    break;

  case 297:
#line 1625 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6627 "ds_parser.cpp"
    break;

  case 298:
#line 1626 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6633 "ds_parser.cpp"
    break;

  case 299:
#line 1627 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6639 "ds_parser.cpp"
    break;

  case 300:
#line 1628 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6645 "ds_parser.cpp"
    break;

  case 301:
#line 1629 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6651 "ds_parser.cpp"
    break;

  case 302:
#line 1630 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6657 "ds_parser.cpp"
    break;

  case 303:
#line 1631 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6663 "ds_parser.cpp"
    break;

  case 304:
#line 1632 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6669 "ds_parser.cpp"
    break;

  case 305:
#line 1633 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6675 "ds_parser.cpp"
    break;

  case 306:
#line 1634 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6681 "ds_parser.cpp"
    break;

  case 307:
#line 1635 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6687 "ds_parser.cpp"
    break;

  case 308:
#line 1636 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6693 "ds_parser.cpp"
    break;

  case 309:
#line 1637 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6699 "ds_parser.cpp"
    break;

  case 310:
#line 1638 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6705 "ds_parser.cpp"
    break;

  case 311:
#line 1639 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6711 "ds_parser.cpp"
    break;

  case 312:
#line 1640 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6717 "ds_parser.cpp"
    break;

  case 313:
#line 1641 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6723 "ds_parser.cpp"
    break;

  case 314:
#line 1642 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6729 "ds_parser.cpp"
    break;

  case 315:
#line 1643 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6735 "ds_parser.cpp"
    break;

  case 316:
#line 1644 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6741 "ds_parser.cpp"
    break;

  case 317:
#line 1645 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6747 "ds_parser.cpp"
    break;

  case 318:
#line 1646 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6753 "ds_parser.cpp"
    break;

  case 319:
#line 1647 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6759 "ds_parser.cpp"
    break;

  case 320:
#line 1648 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6765 "ds_parser.cpp"
    break;

  case 321:
#line 1649 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6771 "ds_parser.cpp"
    break;

  case 322:
#line 1650 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6777 "ds_parser.cpp"
    break;

  case 323:
#line 1651 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 6783 "ds_parser.cpp"
    break;

  case 324:
#line 1652 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6789 "ds_parser.cpp"
    break;

  case 325:
#line 1653 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 6795 "ds_parser.cpp"
    break;

  case 326:
#line 1654 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6801 "ds_parser.cpp"
    break;

  case 327:
#line 1655 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6810 "ds_parser.cpp"
    break;

  case 328:
#line 1659 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6819 "ds_parser.cpp"
    break;

  case 329:
#line 1663 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),tokAt((yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 6827 "ds_parser.cpp"
    break;

  case 330:
#line 1666 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),tokAt((yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6835 "ds_parser.cpp"
    break;

  case 331:
#line 1669 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6841 "ds_parser.cpp"
    break;

  case 332:
#line 1670 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6847 "ds_parser.cpp"
    break;

  case 333:
#line 1671 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6853 "ds_parser.cpp"
    break;

  case 334:
#line 1672 "ds_parser.ypp"
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
#line 1681 "ds_parser.ypp"
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
#line 1690 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6887 "ds_parser.cpp"
    break;

  case 337:
#line 1691 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6895 "ds_parser.cpp"
    break;

  case 338:
#line 1694 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6901 "ds_parser.cpp"
    break;

  case 339:
#line 1694 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6907 "ds_parser.cpp"
    break;

  case 340:
#line 1694 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6915 "ds_parser.cpp"
    break;

  case 341:
#line 1697 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6924 "ds_parser.cpp"
    break;

  case 342:
#line 1701 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6933 "ds_parser.cpp"
    break;

  case 343:
#line 1705 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6942 "ds_parser.cpp"
    break;

  case 344:
#line 1709 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6948 "ds_parser.cpp"
    break;

  case 345:
#line 1710 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6954 "ds_parser.cpp"
    break;

  case 346:
#line 1711 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6960 "ds_parser.cpp"
    break;

  case 347:
#line 1712 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6966 "ds_parser.cpp"
    break;

  case 348:
#line 1713 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6972 "ds_parser.cpp"
    break;

  case 349:
#line 1714 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6978 "ds_parser.cpp"
    break;

  case 350:
#line 1715 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6984 "ds_parser.cpp"
    break;

  case 351:
#line 1716 "ds_parser.ypp"
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
#line 1732 "ds_parser.ypp"
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
#line 1748 "ds_parser.ypp"
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
#line 1803 "ds_parser.ypp"
    {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 7096 "ds_parser.cpp"
    break;

  case 355:
#line 1811 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 7102 "ds_parser.cpp"
    break;

  case 356:
#line 1812 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7108 "ds_parser.cpp"
    break;

  case 357:
#line 1816 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_NONE; }
#line 7114 "ds_parser.cpp"
    break;

  case 358:
#line 1817 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_OVERRIDE; }
#line 7120 "ds_parser.cpp"
    break;

  case 359:
#line 1818 "ds_parser.ypp"
    { (yyval.i) = OVERRIDE_SEALED; }
#line 7126 "ds_parser.cpp"
    break;

  case 360:
#line 1822 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7132 "ds_parser.cpp"
    break;

  case 361:
#line 1823 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7138 "ds_parser.cpp"
    break;

  case 362:
#line 1827 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7144 "ds_parser.cpp"
    break;

  case 363:
#line 1828 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7150 "ds_parser.cpp"
    break;

  case 364:
#line 1829 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7156 "ds_parser.cpp"
    break;

  case 365:
#line 1833 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7168 "ds_parser.cpp"
    break;

  case 366:
#line 1843 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7176 "ds_parser.cpp"
    break;

  case 367:
#line 1846 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7185 "ds_parser.cpp"
    break;

  case 368:
#line 1851 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7191 "ds_parser.cpp"
    break;

  case 369:
#line 1851 "ds_parser.ypp"
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
            auto vars = new vector<VariableNameAndPosition>();
            vars->emplace_back(VariableNameAndPosition{varName,"",(yyvsp[-1].pFuncDecl)->at});
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
#line 7241 "ds_parser.cpp"
    break;

  case 370:
#line 1898 "ds_parser.ypp"
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
#line 7310 "ds_parser.cpp"
    break;

  case 371:
#line 1962 "ds_parser.ypp"
    {
        das_yyerror("structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt((yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7321 "ds_parser.cpp"
    break;

  case 372:
#line 1971 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7331 "ds_parser.cpp"
    break;

  case 373:
#line 1976 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7341 "ds_parser.cpp"
    break;

  case 374:
#line 1984 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7347 "ds_parser.cpp"
    break;

  case 375:
#line 1985 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7353 "ds_parser.cpp"
    break;

  case 376:
#line 1989 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7361 "ds_parser.cpp"
    break;

  case 377:
#line 1992 "ds_parser.ypp"
    {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt((yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7372 "ds_parser.cpp"
    break;

  case 378:
#line 2001 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7378 "ds_parser.cpp"
    break;

  case 379:
#line 2002 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7384 "ds_parser.cpp"
    break;

  case 380:
#line 2006 "ds_parser.ypp"
    {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt((yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7395 "ds_parser.cpp"
    break;

  case 381:
#line 2015 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7401 "ds_parser.cpp"
    break;

  case 382:
#line 2016 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7407 "ds_parser.cpp"
    break;

  case 383:
#line 2021 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7413 "ds_parser.cpp"
    break;

  case 384:
#line 2022 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7419 "ds_parser.cpp"
    break;

  case 385:
#line 2026 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7430 "ds_parser.cpp"
    break;

  case 386:
#line 2032 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7441 "ds_parser.cpp"
    break;

  case 387:
#line 2038 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7449 "ds_parser.cpp"
    break;

  case 388:
#line 2041 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7458 "ds_parser.cpp"
    break;

  case 389:
#line 2045 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7469 "ds_parser.cpp"
    break;

  case 390:
#line 2051 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7480 "ds_parser.cpp"
    break;

  case 391:
#line 2060 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7486 "ds_parser.cpp"
    break;

  case 392:
#line 2061 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7492 "ds_parser.cpp"
    break;

  case 393:
#line 2062 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7498 "ds_parser.cpp"
    break;

  case 394:
#line 2066 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7504 "ds_parser.cpp"
    break;

  case 395:
#line 2067 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7510 "ds_parser.cpp"
    break;

  case 396:
#line 2071 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7518 "ds_parser.cpp"
    break;

  case 397:
#line 2074 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7528 "ds_parser.cpp"
    break;

  case 398:
#line 2079 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7538 "ds_parser.cpp"
    break;

  case 399:
#line 2084 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7551 "ds_parser.cpp"
    break;

  case 400:
#line 2092 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7564 "ds_parser.cpp"
    break;

  case 401:
#line 2103 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7572 "ds_parser.cpp"
    break;

  case 402:
#line 2106 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7582 "ds_parser.cpp"
    break;

  case 403:
#line 2114 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7588 "ds_parser.cpp"
    break;

  case 404:
#line 2115 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7594 "ds_parser.cpp"
    break;

  case 405:
#line 2119 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7600 "ds_parser.cpp"
    break;

  case 406:
#line 2120 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7606 "ds_parser.cpp"
    break;

  case 407:
#line 2121 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7612 "ds_parser.cpp"
    break;

  case 408:
#line 2125 "ds_parser.ypp"
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
#line 7650 "ds_parser.cpp"
    break;

  case 409:
#line 2158 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 7656 "ds_parser.cpp"
    break;

  case 410:
#line 2158 "ds_parser.ypp"
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
#line 7697 "ds_parser.cpp"
    break;

  case 411:
#line 2197 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7705 "ds_parser.cpp"
    break;

  case 412:
#line 2200 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7719 "ds_parser.cpp"
    break;

  case 413:
#line 2209 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7733 "ds_parser.cpp"
    break;

  case 414:
#line 2222 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 7747 "ds_parser.cpp"
    break;

  case 419:
#line 2244 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7753 "ds_parser.cpp"
    break;

  case 420:
#line 2245 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7759 "ds_parser.cpp"
    break;

  case 421:
#line 2246 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7765 "ds_parser.cpp"
    break;

  case 422:
#line 2250 "ds_parser.ypp"
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
#line 7798 "ds_parser.cpp"
    break;

  case 423:
#line 2278 "ds_parser.ypp"
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
#line 7832 "ds_parser.cpp"
    break;

  case 424:
#line 2310 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7838 "ds_parser.cpp"
    break;

  case 425:
#line 2311 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7844 "ds_parser.cpp"
    break;

  case 426:
#line 2315 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7850 "ds_parser.cpp"
    break;

  case 427:
#line 2316 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7856 "ds_parser.cpp"
    break;

  case 428:
#line 2320 "ds_parser.ypp"
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
#line 7902 "ds_parser.cpp"
    break;

  case 429:
#line 2364 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7908 "ds_parser.cpp"
    break;

  case 430:
#line 2365 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7914 "ds_parser.cpp"
    break;

  case 431:
#line 2369 "ds_parser.ypp"
    { (yyval.b) = g_Program->thisModule->isPublic; }
#line 7920 "ds_parser.cpp"
    break;

  case 432:
#line 2370 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7926 "ds_parser.cpp"
    break;

  case 433:
#line 2371 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7932 "ds_parser.cpp"
    break;

  case 434:
#line 2376 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 7938 "ds_parser.cpp"
    break;

  case 435:
#line 2376 "ds_parser.ypp"
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
#line 8036 "ds_parser.cpp"
    break;

  case 436:
#line 2472 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt((yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8048 "ds_parser.cpp"
    break;

  case 437:
#line 2479 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
#line 8062 "ds_parser.cpp"
    break;

  case 438:
#line 2488 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt((yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 8073 "ds_parser.cpp"
    break;

  case 439:
#line 2494 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt((yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
#line 8086 "ds_parser.cpp"
    break;

  case 440:
#line 2505 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 8092 "ds_parser.cpp"
    break;

  case 441:
#line 2506 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 8098 "ds_parser.cpp"
    break;

  case 442:
#line 2507 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8104 "ds_parser.cpp"
    break;

  case 443:
#line 2508 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8110 "ds_parser.cpp"
    break;

  case 444:
#line 2509 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8116 "ds_parser.cpp"
    break;

  case 445:
#line 2510 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 8122 "ds_parser.cpp"
    break;

  case 446:
#line 2511 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 8128 "ds_parser.cpp"
    break;

  case 447:
#line 2512 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 8134 "ds_parser.cpp"
    break;

  case 448:
#line 2513 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 8140 "ds_parser.cpp"
    break;

  case 449:
#line 2514 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8146 "ds_parser.cpp"
    break;

  case 450:
#line 2515 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8152 "ds_parser.cpp"
    break;

  case 451:
#line 2516 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8158 "ds_parser.cpp"
    break;

  case 452:
#line 2517 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 8164 "ds_parser.cpp"
    break;

  case 453:
#line 2518 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 8170 "ds_parser.cpp"
    break;

  case 454:
#line 2519 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 8176 "ds_parser.cpp"
    break;

  case 455:
#line 2520 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 8182 "ds_parser.cpp"
    break;

  case 456:
#line 2521 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 8188 "ds_parser.cpp"
    break;

  case 457:
#line 2522 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 8194 "ds_parser.cpp"
    break;

  case 458:
#line 2523 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 8200 "ds_parser.cpp"
    break;

  case 459:
#line 2524 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 8206 "ds_parser.cpp"
    break;

  case 460:
#line 2525 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 8212 "ds_parser.cpp"
    break;

  case 461:
#line 2526 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 8218 "ds_parser.cpp"
    break;

  case 462:
#line 2527 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 8224 "ds_parser.cpp"
    break;

  case 463:
#line 2528 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 8230 "ds_parser.cpp"
    break;

  case 464:
#line 2529 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 8236 "ds_parser.cpp"
    break;

  case 465:
#line 2533 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 8242 "ds_parser.cpp"
    break;

  case 466:
#line 2534 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 8248 "ds_parser.cpp"
    break;

  case 467:
#line 2535 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 8254 "ds_parser.cpp"
    break;

  case 468:
#line 2536 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 8260 "ds_parser.cpp"
    break;

  case 469:
#line 2537 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 8266 "ds_parser.cpp"
    break;

  case 470:
#line 2538 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 8272 "ds_parser.cpp"
    break;

  case 471:
#line 2542 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8285 "ds_parser.cpp"
    break;

  case 472:
#line 2553 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8294 "ds_parser.cpp"
    break;

  case 473:
#line 2557 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8306 "ds_parser.cpp"
    break;

  case 474:
#line 2567 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8318 "ds_parser.cpp"
    break;

  case 475:
#line 2574 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8329 "ds_parser.cpp"
    break;

  case 476:
#line 2583 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8335 "ds_parser.cpp"
    break;

  case 477:
#line 2583 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8341 "ds_parser.cpp"
    break;

  case 478:
#line 2583 "ds_parser.ypp"
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
#line 8356 "ds_parser.cpp"
    break;

  case 479:
#line 2596 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 8362 "ds_parser.cpp"
    break;

  case 480:
#line 2597 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8368 "ds_parser.cpp"
    break;

  case 481:
#line 2598 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8374 "ds_parser.cpp"
    break;

  case 482:
#line 2599 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8380 "ds_parser.cpp"
    break;

  case 483:
#line 2600 "ds_parser.ypp"
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
#line 8399 "ds_parser.cpp"
    break;

  case 484:
#line 2614 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8410 "ds_parser.cpp"
    break;

  case 485:
#line 2620 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8419 "ds_parser.cpp"
    break;

  case 486:
#line 2624 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8428 "ds_parser.cpp"
    break;

  case 487:
#line 2628 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8438 "ds_parser.cpp"
    break;

  case 488:
#line 2633 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8448 "ds_parser.cpp"
    break;

  case 489:
#line 2638 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8458 "ds_parser.cpp"
    break;

  case 490:
#line 2643 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8468 "ds_parser.cpp"
    break;

  case 491:
#line 2648 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8477 "ds_parser.cpp"
    break;

  case 492:
#line 2652 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8486 "ds_parser.cpp"
    break;

  case 493:
#line 2656 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8496 "ds_parser.cpp"
    break;

  case 494:
#line 2661 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8505 "ds_parser.cpp"
    break;

  case 495:
#line 2665 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8515 "ds_parser.cpp"
    break;

  case 496:
#line 2670 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8521 "ds_parser.cpp"
    break;

  case 497:
#line 2670 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8527 "ds_parser.cpp"
    break;

  case 498:
#line 2670 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8538 "ds_parser.cpp"
    break;

  case 499:
#line 2676 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8550 "ds_parser.cpp"
    break;

  case 500:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8556 "ds_parser.cpp"
    break;

  case 501:
#line 2683 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8562 "ds_parser.cpp"
    break;

  case 502:
#line 2683 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8572 "ds_parser.cpp"
    break;

  case 503:
#line 2688 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8578 "ds_parser.cpp"
    break;

  case 504:
#line 2688 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8584 "ds_parser.cpp"
    break;

  case 505:
#line 2688 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8595 "ds_parser.cpp"
    break;

  case 506:
#line 2694 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8601 "ds_parser.cpp"
    break;

  case 507:
#line 2694 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8607 "ds_parser.cpp"
    break;

  case 508:
#line 2694 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8617 "ds_parser.cpp"
    break;

  case 509:
#line 2699 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8626 "ds_parser.cpp"
    break;

  case 510:
#line 2703 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8632 "ds_parser.cpp"
    break;

  case 511:
#line 2703 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8638 "ds_parser.cpp"
    break;

  case 512:
#line 2703 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8648 "ds_parser.cpp"
    break;

  case 513:
#line 2708 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8654 "ds_parser.cpp"
    break;

  case 514:
#line 2708 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8660 "ds_parser.cpp"
    break;

  case 515:
#line 2708 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8674 "ds_parser.cpp"
    break;

  case 516:
#line 2717 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8683 "ds_parser.cpp"
    break;

  case 517:
#line 2721 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8689 "ds_parser.cpp"
    break;

  case 518:
#line 2721 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8695 "ds_parser.cpp"
    break;

  case 519:
#line 2721 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8705 "ds_parser.cpp"
    break;

  case 520:
#line 2726 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8711 "ds_parser.cpp"
    break;

  case 521:
#line 2726 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8717 "ds_parser.cpp"
    break;

  case 522:
#line 2726 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8731 "ds_parser.cpp"
    break;

  case 523:
#line 2735 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8740 "ds_parser.cpp"
    break;

  case 524:
#line 2739 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8746 "ds_parser.cpp"
    break;

  case 525:
#line 2739 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8752 "ds_parser.cpp"
    break;

  case 526:
#line 2739 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8762 "ds_parser.cpp"
    break;

  case 527:
#line 2744 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8768 "ds_parser.cpp"
    break;

  case 528:
#line 2744 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8774 "ds_parser.cpp"
    break;

  case 529:
#line 2744 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8788 "ds_parser.cpp"
    break;

  case 530:
#line 2753 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8794 "ds_parser.cpp"
    break;

  case 531:
#line 2753 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8800 "ds_parser.cpp"
    break;

  case 532:
#line 2753 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8811 "ds_parser.cpp"
    break;

  case 533:
#line 2759 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8817 "ds_parser.cpp"
    break;

  case 534:
#line 2759 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8823 "ds_parser.cpp"
    break;

  case 535:
#line 2759 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8834 "ds_parser.cpp"
    break;

  case 536:
#line 2768 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8840 "ds_parser.cpp"
    break;

  case 537:
#line 2768 "ds_parser.ypp"
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
#line 8857 "ds_parser.cpp"
    break;

  case 538:
#line 2783 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8863 "ds_parser.cpp"
    break;

  case 539:
#line 2783 "ds_parser.ypp"
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
#line 8884 "ds_parser.cpp"
    break;

  case 540:
#line 2803 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8890 "ds_parser.cpp"
    break;

  case 541:
#line 2804 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8896 "ds_parser.cpp"
    break;

  case 542:
#line 2805 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8902 "ds_parser.cpp"
    break;

  case 543:
#line 2806 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8908 "ds_parser.cpp"
    break;

  case 544:
#line 2810 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8920 "ds_parser.cpp"
    break;

  case 545:
#line 2817 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8932 "ds_parser.cpp"
    break;

  case 546:
#line 2824 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8943 "ds_parser.cpp"
    break;

  case 547:
#line 2830 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8954 "ds_parser.cpp"
    break;

  case 548:
#line 2839 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8964 "ds_parser.cpp"
    break;

  case 549:
#line 2844 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8973 "ds_parser.cpp"
    break;

  case 550:
#line 2851 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8979 "ds_parser.cpp"
    break;

  case 551:
#line 2852 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8985 "ds_parser.cpp"
    break;

  case 552:
#line 2856 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8996 "ds_parser.cpp"
    break;

  case 553:
#line 2862 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 9008 "ds_parser.cpp"
    break;

  case 554:
#line 2869 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 9021 "ds_parser.cpp"
    break;

  case 555:
#line 2877 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 9033 "ds_parser.cpp"
    break;

  case 556:
#line 2884 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9046 "ds_parser.cpp"
    break;

  case 557:
#line 2892 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9060 "ds_parser.cpp"
    break;

  case 558:
#line 2904 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 9068 "ds_parser.cpp"
    break;

  case 559:
#line 2907 "ds_parser.ypp"
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
#line 9084 "ds_parser.cpp"
    break;

  case 560:
#line 2921 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9095 "ds_parser.cpp"
    break;

  case 561:
#line 2930 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9101 "ds_parser.cpp"
    break;

  case 562:
#line 2931 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9107 "ds_parser.cpp"
    break;

  case 563:
#line 2935 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9117 "ds_parser.cpp"
    break;

  case 564:
#line 2940 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9126 "ds_parser.cpp"
    break;

  case 565:
#line 2947 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9136 "ds_parser.cpp"
    break;

  case 566:
#line 2952 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9148 "ds_parser.cpp"
    break;

  case 567:
#line 2962 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9158 "ds_parser.cpp"
    break;

  case 568:
#line 2967 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9167 "ds_parser.cpp"
    break;

  case 569:
#line 2974 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9181 "ds_parser.cpp"
    break;

  case 570:
#line 2986 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 9187 "ds_parser.cpp"
    break;

  case 571:
#line 2987 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9193 "ds_parser.cpp"
    break;

  case 572:
#line 2991 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->visibility = tokRangeAt((yylsp[-3]),(yylsp[0]));
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
        auto pAC = new ExprArrayComprehension(tokAt((yylsp[-8])));
        pAC->generatorSyntax = true;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 9221 "ds_parser.cpp"
    break;

  case 573:
#line 3014 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->visibility = tokRangeAt((yylsp[-3]),(yylsp[0]));
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
        auto pAC = new ExprArrayComprehension(tokAt((yylsp[-8])));
        pAC->generatorSyntax = false;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 9249 "ds_parser.cpp"
    break;


#line 9253 "ds_parser.cpp"

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
#line 3039 "ds_parser.ypp"


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
