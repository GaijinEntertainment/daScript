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
#line 56 "ds_parser.ypp"

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

#line 102 "ds_parser.cpp"

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
        extern ProgramPtr			    g_Program;
        extern FileAccessPtr            g_Access;
        extern vector<FileInfo *>	    g_FileAccessStack;
        extern das_map<string,string>   das_module_alias;
    }
    using namespace das;

    struct VariableDeclaration {
        VariableDeclaration ( const LineInfo & a, vector<string> * n, TypeDecl * t, Expression * i )
            : at(a), pNameList(n), pTypeDecl(t), pInit(i) {}
        virtual ~VariableDeclaration () {
            if ( pNameList ) delete pNameList;
            delete pTypeDecl;
            if ( pInit ) delete pInit;
            if ( annotation ) delete annotation;
        }
        LineInfo                at;
        vector<string>          *pNameList;
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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = false );

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;

#line 198 "ds_parser.cpp"

/* Token type.  */
#ifndef DAS_YYTOKENTYPE
# define DAS_YYTOKENTYPE
  enum das_yytokentype
  {
    LEXER_ERROR = 258,
    DAS_STRUCT = 259,
    DAS_LET = 260,
    DAS_DEF = 261,
    DAS_WHILE = 262,
    DAS_IF = 263,
    DAS_STATIC_IF = 264,
    DAS_ELSE = 265,
    DAS_FOR = 266,
    DAS_CATCH = 267,
    DAS_TRUE = 268,
    DAS_FALSE = 269,
    DAS_NEWT = 270,
    DAS_TYPEINFO = 271,
    DAS_TYPE = 272,
    DAS_IN = 273,
    DAS_IS = 274,
    DAS_AS = 275,
    DAS_ELIF = 276,
    DAS_STATIC_ELIF = 277,
    DAS_ARRAY = 278,
    DAS_RETURN = 279,
    DAS_NULL = 280,
    DAS_BREAK = 281,
    DAS_TRY = 282,
    DAS_OPTIONS = 283,
    DAS_TABLE = 284,
    DAS_EXPECT = 285,
    DAS_CONST = 286,
    DAS_REQUIRE = 287,
    DAS_OPERATOR = 288,
    DAS_ENUM = 289,
    DAS_FINALLY = 290,
    DAS_DELETE = 291,
    DAS_DEREF = 292,
    DAS_SCOPE = 293,
    DAS_TYPEDEF = 294,
    DAS_WITH = 295,
    DAS_CAST = 296,
    DAS_OVERRIDE = 297,
    DAS_UPCAST = 298,
    DAS_ITERATOR = 299,
    DAS_VAR = 300,
    DAS_ADDR = 301,
    DAS_CONTINUE = 302,
    DAS_WHERE = 303,
    DAS_PASS = 304,
    DAS_REINTERPRET = 305,
    DAS_MODULE = 306,
    DAS_PUBLIC = 307,
    DAS_LABEL = 308,
    DAS_GOTO = 309,
    DAS_IMPLICIT = 310,
    DAS_SHARED = 311,
    DAS_SMART_PTR = 312,
    DAS_TBOOL = 313,
    DAS_TVOID = 314,
    DAS_TSTRING = 315,
    DAS_TAUTO = 316,
    DAS_TINT = 317,
    DAS_TINT2 = 318,
    DAS_TINT3 = 319,
    DAS_TINT4 = 320,
    DAS_TUINT = 321,
    DAS_TBITFIELD = 322,
    DAS_TUINT2 = 323,
    DAS_TUINT3 = 324,
    DAS_TUINT4 = 325,
    DAS_TFLOAT = 326,
    DAS_TFLOAT2 = 327,
    DAS_TFLOAT3 = 328,
    DAS_TFLOAT4 = 329,
    DAS_TRANGE = 330,
    DAS_TURANGE = 331,
    DAS_TBLOCK = 332,
    DAS_TINT64 = 333,
    DAS_TUINT64 = 334,
    DAS_TDOUBLE = 335,
    DAS_TFUNCTION = 336,
    DAS_TLAMBDA = 337,
    DAS_TINT8 = 338,
    DAS_TUINT8 = 339,
    DAS_TINT16 = 340,
    DAS_TUINT16 = 341,
    DAS_TTUPLE = 342,
    DAS_TVARIANT = 343,
    DAS_GENERATOR = 344,
    DAS_YIELD = 345,
    ADDEQU = 346,
    SUBEQU = 347,
    DIVEQU = 348,
    MULEQU = 349,
    MODEQU = 350,
    ANDEQU = 351,
    OREQU = 352,
    XOREQU = 353,
    SHL = 354,
    SHR = 355,
    ADDADD = 356,
    SUBSUB = 357,
    LEEQU = 358,
    SHLEQU = 359,
    SHREQU = 360,
    GREQU = 361,
    EQUEQU = 362,
    NOTEQU = 363,
    RARROW = 364,
    LARROW = 365,
    QQ = 366,
    QDOT = 367,
    QBRA = 368,
    LPIPE = 369,
    LBPIPE = 370,
    LAPIPE = 371,
    RPIPE = 372,
    CLONEEQU = 373,
    ROTL = 374,
    ROTR = 375,
    ROTLEQU = 376,
    ROTREQU = 377,
    MAPTO = 378,
    BRABRAB = 379,
    BRACBRB = 380,
    CBRCBRB = 381,
    INTEGER = 382,
    LONG_INTEGER = 383,
    UNSIGNED_INTEGER = 384,
    UNSIGNED_LONG_INTEGER = 385,
    FLOAT = 386,
    DOUBLE = 387,
    NAME = 388,
    BEGIN_STRING = 389,
    STRING_CHARACTER = 390,
    END_STRING = 391,
    BEGIN_STRING_EXPR = 392,
    END_STRING_EXPR = 393,
    UNARY_MINUS = 394,
    UNARY_PLUS = 395,
    PRE_INC = 396,
    PRE_DEC = 397,
    POST_INC = 398,
    POST_DEC = 399,
    DEREF = 400,
    COLCOL = 401
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 83 "ds_parser.ypp"

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

#line 381 "ds_parser.cpp"

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
#define YYLAST   6728

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  114
/* YYNRULES -- Number of rules.  */
#define YYNRULES  436
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  799

#define YYUNDEFTOK  2
#define YYMAXUTOK   401

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
       2,     2,     2,   156,     2,   173,   167,   152,   145,     2,
     165,   166,   150,   149,   139,   148,   161,   151,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   142,   170,
     146,   140,   147,   141,   168,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   163,     2,   164,   144,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   171,   143,   172,   155,     2,     2,     2,
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
     135,   136,   137,   138,   153,   154,   157,   158,   159,   160,
     162,   169
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   395,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   410,   422,   423,   427,   431,   436,
     445,   453,   469,   491,   495,   498,   504,   513,   534,   557,
     558,   562,   566,   567,   571,   574,   580,   586,   589,   595,
     596,   600,   601,   602,   611,   612,   616,   625,   637,   647,
     656,   657,   658,   659,   660,   661,   665,   670,   678,   679,
     680,   681,   682,   683,   684,   685,   691,   704,   720,   721,
     725,   732,   744,   748,   755,   756,   760,   761,   762,   766,
     771,   778,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   821,   895,
     898,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   929,
     934,   940,   946,   957,   963,   964,   975,   979,   985,   988,
     991,   995,  1001,  1005,  1009,  1012,  1015,  1020,  1023,  1031,
    1034,  1039,  1042,  1050,  1056,  1057,  1061,  1062,  1066,  1100,
    1103,  1108,  1116,  1120,  1125,  1131,  1135,  1140,  1149,  1152,
    1158,  1159,  1166,  1177,  1178,  1182,  1187,  1242,  1243,  1244,
    1245,  1246,  1247,  1251,  1252,  1253,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1270,
    1271,  1275,  1286,  1291,  1301,  1305,  1312,  1326,  1327,  1328,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,
    1359,  1360,  1361,  1362,  1363,  1364,  1365,  1369,  1373,  1376,
    1379,  1380,  1381,  1382,  1387,  1392,  1393,  1396,  1399,  1403,
    1407,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1427,  1437,
    1471,  1472,  1476,  1477,  1481,  1489,  1492,  1499,  1504,  1512,
    1513,  1517,  1520,  1529,  1530,  1534,  1543,  1544,  1549,  1550,
    1554,  1559,  1562,  1566,  1572,  1581,  1582,  1583,  1587,  1588,
    1592,  1595,  1600,  1605,  1613,  1624,  1627,  1634,  1635,  1639,
    1666,  1666,  1678,  1681,  1690,  1703,  1715,  1716,  1720,  1724,
    1735,  1750,  1751,  1755,  1794,  1860,  1867,  1876,  1877,  1878,
    1879,  1880,  1881,  1882,  1883,  1884,  1885,  1886,  1887,  1888,
    1889,  1890,  1891,  1892,  1893,  1894,  1895,  1896,  1897,  1898,
    1899,  1900,  1904,  1905,  1906,  1907,  1908,  1909,  1913,  1924,
    1928,  1938,  1945,  1954,  1963,  1964,  1965,  1966,  1967,  1981,
    1987,  1991,  1996,  2001,  2006,  2013,  2018,  2022,  2026,  2031,
    2036,  2042,  2049,  2054,  2060,  2065,  2069,  2074,  2083,  2087,
    2092,  2101,  2105,  2110,  2119,  2125,  2134,  2140,  2155,  2156,
    2157,  2158,  2162,  2169,  2178,  2183,  2190,  2195,  2201,  2207,
    2214,  2225,  2228,  2242,  2251,  2252,  2256,  2261,  2268,  2273,
    2284,  2289,  2296,  2308,  2309,  2313,  2327
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"let\"", "\"def\"", "\"while\"", "\"if\"", "\"static_if\"", "\"else\"",
  "\"for\"", "\"recover\"", "\"true\"", "\"false\"", "\"new\"",
  "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"",
  "\"static_elif\"", "\"array\"", "\"return\"", "\"null\"", "\"break\"",
  "\"try\"", "\"options\"", "\"table\"", "\"expect\"", "\"const\"",
  "\"require\"", "\"operator\"", "\"enum\"", "\"finally\"", "\"delete\"",
  "\"deref\"", "\"scope\"", "\"typedef\"", "\"with\"", "\"cast\"",
  "\"override\"", "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"",
  "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"",
  "\"public\"", "\"label\"", "\"goto\"", "\"implicit\"", "\"shared\"",
  "\"smart_ptr\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
  "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
  "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"",
  "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"",
  "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"",
  "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"",
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"+=\"",
  "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"",
  "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS",
  "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'",
  "DEREF", "'['", "']'", "'('", "')'", "'$'", "'@'", "COLCOL", "';'",
  "'{'", "'}'", "'#'", "$accept", "program", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "expression_with", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "function_declaration", "expression_block",
  "expression_any", "expressions", "expr_pipe", "name_in_namespace",
  "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_yield",
  "expression_try_catch", "let_scope", "kwd_let", "expression_let",
  "expr_cast", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "expr_block", "expr_numeric_const", "expr_assign",
  "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_expr", "expr", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "$@1", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_type_declaration",
  "type_declaration", "variant_keyword", "variant_alias_declaration",
  "make_decl", "make_struct_fields", "make_struct_dim", "make_struct_decl",
  "make_tuple", "make_map_tuple", "make_any_tuple", "make_dim",
  "make_dim_decl", "make_table", "make_table_decl",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,    44,
      61,    63,    58,   124,    94,    38,    60,    62,    45,    43,
      42,    47,    37,   394,   395,   126,    33,   396,   397,   398,
     399,    46,   400,    91,    93,    40,    41,    36,    64,   401,
      59,   123,   125,    35
};
# endif

#define YYPACT_NINF -479

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-479)))

#define YYTABLE_NINF -194

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -479,     7,  -479,  -479,   -63,   -20,    -4,   119,   -59,  -479,
     138,  -479,    -6,  -479,  -479,  -479,  -479,  -479,    69,  -479,
     125,  -479,  -479,  -479,  -479,   173,  -479,    60,  -479,   168,
     167,   171,  -479,  -479,     5,  -479,   -70,   185,  -479,  -479,
     144,   190,   159,  -479,    92,  -479,   194,    -9,  -479,   163,
     175,    17,   -63,   204,   -20,   206,  -479,   217,   219,  -479,
     236,  -479,   214,  -479,   -91,   222,  -479,   -63,    -6,  -479,
     215,   192,  6534,   323,   331,  -479,   200,  -479,   367,   237,
    -479,  -479,  -479,  -479,  -479,   234,    65,  -479,  -479,  -479,
    -479,   319,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
     207,   -89,  5700,  -479,  -479,  -479,   -75,  -479,   -93,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,   -98,   235,   -74,
     211,   240,  -479,   216,  -479,   143,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,     8,  -479,  -479,   -55,  -479,   238,   243,
     244,   250,  -479,  -479,  -479,   232,  -479,  -479,  -479,  -479,
    -479,   254,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,   256,  -479,  -479,  -479,   261,   262,  -479,  -479,  -479,
    -479,   264,   267,  -479,  -479,  -479,  -479,  -479,    75,  -479,
    -479,  -103,   -63,  -479,   -16,  -479,    93,  5700,   245,  -479,
    -479,  -479,   158,  -479,  5700,   -72,  -479,  -479,    65,  -479,
     -67,  4092,  -479,  5700,  5700,  5700,  5700,   282,   285,  5587,
    5587,  5587,  5777,   237,  -479,    16,  -479,  -479,  -479,  -479,
    2094,  -479,  -479,  -479,   377,   251,   121,   287,  -479,   -21,
    -479,   259,  1199,  -479,  -479,   290,  5700,  -479,   -30,  1199,
    -479,  -479,  -479,  -479,  -479,  -479,  5510,   263,  -479,   265,
     278,   280,   269,   281,  -479,   289,  4092,  4092,   794,  4220,
    4092,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  4092,  4092,
    4092,  4092,  4092,  4092,  -479,   268,  -479,  -479,   -62,  -479,
    -479,  -479,   274,  -479,  -479,  -479,  -479,  -479,  4854,   286,
    -479,  -479,  -479,  -479,  -479,    31,   471,  1262,  1407,   284,
    -479,   -39,   235,  1637,   235,  2220,   235,  2307,    48,  -479,
      72,  1199,   110,   302,  -479,   298,  -479,  -479,  5771,  -479,
     287,  -479,  -479,  -479,  -479,  -479,  5700,  2223,  -479,   943,
    -479,  6555,  -479,  -479,  -479,  2223,  2487,  -479,   321,  4092,
    5700,  5700,  4092,  5700,  5700,   182,   182,   287,  1263,   287,
    1408,  6163,  -479,    79,   180,   182,   182,   -79,   182,   182,
    4991,   -95,  -479,  1691,   200,    38,   330,  4092,  4092,  -479,
    -479,  4092,  4092,  4092,  4092,   332,  4092,   333,  4092,  4092,
    4092,  4092,  4092,  2361,  4092,  4092,  4092,  4092,  4092,  4092,
    4092,  4092,  4092,  4092,   334,  4092,  -479,  2490,  -479,  5700,
    -479,  -479,  -479,  -479,   335,   325,  -479,   327,  -479,   328,
    -479,  5700,  -479,  5777,  -479,   237,  -479,  -479,  -479,  -479,
      73,   137,  -479,   361,  5994,  -479,  4092,  -479,  -479,   287,
    1553,  -479,   245,  4092,  4092,  -479,   308,   352,  2628,  1836,
    -479,   445,   317,   318,  4092,  -479,  -479,  -479,  -479,  -479,
    -479,   320,   322,   324,  -479,  -479,  -479,   473,  -479,   -83,
    -479,  6098,  -479,  2223,  -479,  4309,  2757,  1965,  5060,  2574,
    2754,  5128,  2841,  2979,    10,   -87,   329,  2895,  6163,   350,
      27,   356,  -479,  -479,    98,    18,  3033,   160,   177,  4092,
    4092,   337,  -479,  4092,   365,  -479,  5587,  -479,   363,  -479,
      67,  6366,   235,  5700,  -479,  -479,   224,   224,   630,   630,
     292,   292,   336,   941,  -479,  5848,   -64,   -64,   224,   224,
     370,  6232,  5586,  6431,  6500,   630,   630,   347,   347,   941,
     941,   941,  -479,  5917,  -479,    71,  3117,  -479,  -479,  -479,
    -479,  1199,  -479,  4092,  -479,  -479,   137,  4092,  4092,  4092,
    4092,  4092,  4092,  4092,  4092,  4092,  4092,  4092,  4092,  4092,
    4092,  4092,  4213,    25,  2223,  -479,  -479,  4405,   492,  6366,
    4213,  -479,   364,   378,  6366,  2223,  -479,  4501,   340,  -479,
    -479,  4213,  -479,  -479,  -479,   474,   287,  -479,  3171,  3300,
    -479,  4597,  -479,  -479,    87,  5700,   380,  5196,  -479,  4092,
    4092,  -479,  4092,   351,  4092,  4092,  -479,   363,   382,   353,
     363,  4092,   354,  4092,  4092,   363,   355,   358,  6366,  -479,
    -479,  6297,   235,  3426,    63,   122,  4092,  -479,   -69,  1199,
    3429,  -479,  -479,  4092,  -479,  -479,  -479,  6366,  -479,  6366,
    6366,  6366,  6366,  6366,  6366,  6366,  6366,  6366,  6366,  6366,
    6366,  6366,  6366,  6366,  -479,  4092,  -479,  4693,  -479,   245,
    -479,  -479,  -479,  -479,  4789,  -479,  -479,   104,  -479,  -479,
    4092,  4092,  -479,  -479,  3513,   118,  -479,   630,   630,   630,
    3567,   -76,  6366,   117,    63,  -479,   350,  6366,  -479,  -479,
      -2,   181,  -479,  -479,  -479,   373,   -93,   357,  6366,  3705,
    -479,  -479,  -479,   101,  6366,    41,  -479,  -479,  -479,  1115,
     245,  -479,  -479,  4092,  -479,   518,   518,  -479,  3834,   392,
    -479,  5264,  4092,   366,  4092,  4092,   368,   -93,  -479,  -479,
    4092,  6366,  -479,   245,  -479,  -479,  4213,  5700,  5332,   384,
    -479,  4926,  -479,  6366,  4926,  -479,  -479,  6366,  -479,   104,
    3651,  -479,  3963,   479,   369,   362,  -479,  -479,  5700,  5400,
    4092,   371,   372,  4089,  -479,  6366,  -479,  -479,  -479
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    74,     1,   166,     0,     0,     0,     0,     0,   167,
       0,   406,     0,    13,     3,    10,     9,     8,     0,     7,
     317,     6,    11,     5,     4,     0,    12,    64,    66,    22,
      34,    31,    32,    24,    29,    23,     0,     0,    14,    69,
     144,     0,    70,    72,     0,    68,     0,     0,   318,   320,
       0,     0,     0,     0,     0,     0,    30,     0,     0,    27,
       0,   322,     0,   326,     0,     0,   146,     0,     0,    75,
     331,     0,     0,     0,     0,    81,    76,   315,     0,     0,
      62,    63,    60,    61,    59,     0,     0,    58,    67,    35,
      33,    29,    26,    25,   362,   365,   363,   366,   364,   367,
       0,     0,     0,   328,   327,   145,     0,    73,     0,   333,
     285,    84,    85,    87,    86,    88,    89,    90,    91,   110,
     111,   108,   109,   101,   112,   113,   102,    99,   100,   114,
     115,   116,   117,   104,   105,   103,    97,    98,    93,    92,
      94,    95,    96,    83,    82,   106,   107,   280,    79,     0,
       0,     0,   296,     0,    15,     0,    54,    55,    52,    53,
      51,    50,    56,     0,    28,   322,     0,   329,     0,     0,
       0,     0,   337,   357,   338,   369,   339,   343,   344,   345,
     346,   361,   350,   351,   352,   353,   354,   355,   356,   358,
     359,   395,   342,   349,   360,   398,   401,   340,   347,   341,
     348,     0,     0,   368,   374,   377,   375,   376,     0,    71,
     332,   280,     0,    77,     0,   289,     0,     0,     0,   335,
     319,   316,   308,   321,     0,     0,    16,    17,     0,    65,
       0,     0,   323,     0,     0,     0,     0,     0,     0,    76,
      76,    76,     0,     0,   381,     0,   387,   391,   389,   383,
       0,   325,   386,   334,   282,     0,     0,     0,   287,   300,
      78,   280,    80,   139,   118,     0,     0,   309,     0,   295,
     407,   297,    57,   330,   222,   223,     0,     0,   217,     0,
       0,     0,     0,     0,   361,     0,     0,     0,     0,     0,
       0,   187,   189,   188,   190,   191,   192,    18,     0,     0,
       0,     0,     0,     0,   183,   184,   220,   185,   218,   273,
     272,   271,    74,   276,   219,   275,   274,   255,     0,     0,
     221,   408,   409,   410,   411,     0,     0,     0,     0,     0,
     371,     0,    79,     0,    79,     0,    79,     0,   144,   293,
       0,   291,     0,   382,   385,     0,   388,   379,     0,   283,
       0,   286,   281,   288,   299,   298,     0,     0,   290,     0,
     336,     0,   306,   307,   305,     0,   148,   151,     0,     0,
       0,     0,     0,     0,     0,   246,   247,     0,     0,     0,
       0,     0,   430,     0,     0,   227,   226,   260,   225,   224,
       0,     0,   279,     0,    76,     0,     0,     0,     0,   248,
     249,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,   392,     0,
     394,   390,   370,   373,     0,     0,   396,     0,   399,     0,
     402,     0,   404,     0,   405,     0,   384,   380,   378,   284,
     301,     0,   304,     0,   303,   141,     0,    44,    45,     0,
       0,   152,     0,     0,     0,   153,     0,     0,     0,     0,
     121,   119,     0,     0,     0,   134,   129,   127,   128,   140,
     122,     0,     0,     0,   132,   133,   135,   164,   126,     0,
     123,   193,   310,     0,   314,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   144,     0,     0,   421,   414,
       0,   424,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,    19,   250,    76,   214,     0,   256,
       0,   178,    79,     0,   268,   269,   228,   229,   241,   242,
     239,   240,     0,   265,   254,     0,   277,   278,   230,   231,
       0,     0,   244,   245,   243,   237,   238,   233,   232,   234,
     235,   236,   253,     0,   258,     0,     0,   372,   397,   400,
     403,   292,   294,     0,   184,   143,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   154,   157,   193,     0,   147,
       0,   138,     0,     0,    38,     0,   161,   193,     0,   136,
     137,     0,   125,   130,   131,     0,     0,   124,     0,     0,
     312,   193,   313,   149,     0,     0,     0,     0,   261,     0,
       0,   262,     0,     0,     0,     0,   417,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   423,   431,
     432,     0,    79,     0,     0,     0,     0,   257,     0,   267,
       0,   252,   270,     0,   251,   259,   393,   302,   142,   200,
     201,   203,   202,   204,   197,   198,   199,   205,   206,   195,
     196,   207,   208,   194,    48,     0,   158,   193,   155,     0,
      49,    36,    37,   162,   193,   159,   139,    41,   165,   168,
       0,     0,   311,   150,     0,     0,   172,   169,   170,   171,
       0,     0,   412,     0,     0,   416,   415,   422,   428,   427,
       0,     0,   419,   429,    20,     0,     0,     0,   179,     0,
     180,   186,   212,     0,   266,     0,   156,   163,   160,     0,
       0,    39,    40,     0,    46,   210,   209,   175,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,   215,   211,
       0,   181,   213,     0,   120,    42,     0,     0,     0,     0,
     264,   433,   418,   413,   433,   420,   216,   182,    47,    41,
       0,   173,     0,     0,     0,     0,    43,   176,     0,     0,
       0,     0,     0,     0,   174,   434,   435,   436,   177
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -479,  -479,  -479,   145,   488,  -479,  -479,  -479,  -479,  -479,
    -479,   450,  -479,  -479,   489,  -479,  -479,  -479,  -234,  -479,
    -479,  -479,  -479,  -479,   338,  -479,   494,   -45,  -479,   481,
    -479,   241,  -225,  -323,  -479,  -479,  -212,  -479,  -149,  -347,
     -12,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,   549,
    -479,  -479,  -479,  -419,  -479,  -479,  -431,  -479,  -356,  -479,
    -479,  -479,  -479,   183,   345,  -479,  -479,  -479,   296,  -479,
     115,  -479,  -206,   316,  -257,  -230,   199,  -479,   -54,  -479,
    -479,  -479,  -479,   399,   501,  -479,  -479,  -479,  -479,  -479,
    -479,  -145,  -101,  -479,  -479,  -479,  -479,  -479,   -81,  -479,
    -479,   291,  -478,  -363,  -479,  -479,  -285,   -73,   188,  -479,
    -479,  -479,  -205,  -479
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    14,   155,   161,   384,   306,    15,    16,    34,
      35,    59,    17,    31,    32,   472,   473,   743,   744,   474,
     475,   476,   477,   478,   162,   163,    28,    29,    42,    43,
      44,    18,   148,   218,    76,    19,   307,   479,   359,   480,
     308,   481,   309,   482,   483,   484,   485,   486,   616,   487,
     488,   310,   311,   530,   731,   312,   313,   314,   453,   490,
     315,   316,   317,   531,   214,   350,   255,   211,   215,   216,
     339,   340,   152,   153,   635,   258,   365,   268,   221,   149,
      49,    21,    78,   101,    63,    64,    22,    23,   109,    71,
      24,   259,   319,   100,   205,   206,   331,   207,   341,    25,
      26,   320,   509,   510,   321,   511,   512,   513,   514,   322,
     383,   323,   784,   324
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   204,   357,   489,   222,   382,   264,     2,   565,   435,
     452,   437,     3,   439,   332,   334,   336,   517,   494,   271,
     575,   208,   106,   354,    72,    55,    39,   353,   634,   257,
      80,    81,   576,   407,   408,     4,   644,     5,    40,     6,
      40,     7,    62,   685,   166,   405,     8,   343,   407,   408,
     655,   526,     9,   355,   729,   533,    45,    56,    10,   219,
     212,   151,   244,   656,    52,   212,   166,   245,   213,   253,
      27,   392,    60,    46,    41,    47,    41,   624,   156,   157,
     362,   103,    65,   167,   425,   231,   246,   617,   363,   354,
     203,   209,    73,    74,   752,    11,   210,   424,   220,   425,
     270,    61,   263,   393,   244,   273,   244,    30,   433,   245,
     364,   245,    37,   596,   740,   232,   204,   219,   265,   355,
     449,   356,   606,   204,    75,   741,   742,    40,   246,    33,
     246,   434,   204,   204,   204,   204,   262,   656,   204,   204,
     204,   204,   247,   269,    82,   668,   620,   228,    83,   265,
      84,    85,   325,   326,   327,   328,    57,   265,   333,   335,
     337,   344,   716,    41,   265,   204,    58,   256,   755,   532,
      12,   534,   248,   354,   229,   204,   249,    13,   428,   345,
     656,    48,    86,   354,   247,   361,   247,   204,   204,   346,
     441,   639,   158,   573,   250,   366,   159,   640,   160,    85,
      51,   395,   396,   355,   252,   203,   656,   378,   380,   658,
     656,   763,   203,   355,   248,   711,   248,    65,   249,   442,
     249,   203,   203,   203,   203,   720,   656,   203,   203,   203,
     203,    68,   504,   657,   515,   649,   250,   665,   250,   271,
     656,   733,   443,   395,   396,   251,   252,   686,   252,   520,
     598,   521,    36,   703,   203,   204,    69,   444,   693,   260,
      52,   638,   642,   261,   203,   748,   735,   762,   643,   204,
     204,    38,   204,   204,   713,   450,   203,   203,   226,   227,
     445,   753,   721,   399,   400,   352,   727,   640,   749,   499,
     500,   405,   502,   503,   407,   408,   409,   265,    94,   410,
     266,   652,    95,   267,   304,   574,    50,    52,   263,    53,
      54,   395,   396,    65,   593,   154,   522,   523,    62,    96,
      97,    98,    99,    66,    67,   399,   400,    70,   204,   725,
     640,    89,   646,   405,    77,   406,   407,   408,   409,    91,
     204,   410,   204,   424,   203,   425,    79,   643,   566,   647,
      92,   640,    93,   756,   102,   105,   145,   108,   203,   203,
     571,   203,   203,   110,   146,   147,   395,   396,   150,   154,
     151,    56,   419,   420,   421,   422,   423,   217,   165,   527,
     684,   223,   224,   489,   233,   424,   225,   425,   690,   234,
     235,   397,   398,   399,   400,   401,   236,   237,   402,   697,
     238,   405,   239,   406,   407,   408,   409,   240,   241,   410,
     242,   411,   412,   243,   318,   329,   263,   203,   330,   349,
     219,   351,   212,   360,   370,   204,   371,   373,   368,   203,
     369,   203,   204,   348,   372,   374,   391,    12,   417,   418,
     419,   420,   421,   422,   423,   653,   730,   446,   399,   400,
     432,   427,   659,   424,   497,   425,   405,   754,   406,   407,
     408,   409,   447,   535,   410,   542,   544,   562,   567,   375,
     376,   222,   568,   381,   569,   570,   576,   737,   601,   602,
     608,   385,   386,   387,   388,   389,   390,   609,   610,   638,
     612,   615,   613,   636,   614,   641,   654,   421,   422,   423,
     226,   660,   244,   662,   689,   692,   691,   245,   424,   650,
     425,   696,   698,   705,   203,   714,   710,   715,   718,   722,
     757,   203,   723,   759,   204,   769,   246,   790,   765,   524,
     772,   782,   775,   791,   792,   796,   797,   395,   396,    87,
     454,   164,   491,    90,   704,   786,    88,   739,   495,   107,
      20,   778,   498,   394,   779,   501,   254,   358,   572,   342,
     493,   508,   699,   508,   230,   104,   272,   367,   518,   785,
     719,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   537,   247,     0,   538,   539,   540,   541,     0,   543,
       0,   545,   546,   547,   548,   549,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,     0,   563,     0,
       0,     0,   248,   203,     0,     0,   249,   397,   398,     0,
       0,   401,     0,     0,   402,   403,   404,   405,     0,   406,
     407,   408,   409,     0,   250,   410,     0,   411,   412,   592,
       0,   429,     0,   597,   252,     0,   599,   600,     0,   395,
     396,   604,   607,     0,     0,     0,     0,   611,     0,   413,
       0,   414,   415,   416,   417,   418,   204,     0,     0,   422,
     423,     0,     0,     0,     0,     0,   621,     0,     0,   424,
     627,   425,     0,     0,     0,     0,   780,   204,     0,     0,
     390,     0,     0,     0,     0,     0,     0,     0,     0,   390,
       0,     0,   648,   381,     0,     0,   651,   793,     0,     0,
       0,     0,     0,     0,   758,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,   399,   400,     0,     0,     0,     0,     0,     0,   405,
       0,   406,   407,   408,   409,   776,     0,   410,     0,   411,
     412,     0,     0,     0,     0,   203,   667,     0,     0,     0,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
     679,   680,   681,   682,   683,     0,   203,   687,   419,   420,
     421,   422,   423,     0,     0,     0,     0,     0,   694,     0,
       0,   424,     0,   425,     0,     0,     0,     0,     0,     0,
       0,   679,   683,     0,     0,   377,     0,     0,     0,     0,
       0,     0,   707,   708,     0,   709,     0,   168,   712,     0,
       0,     0,     0,   169,   717,     0,   508,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   728,
       0,     0,     0,     0,     0,     0,   734,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   745,   746,     0,     0,     0,     0,     0,
       0,     0,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   491,     0,     0,     0,   766,    40,     0,     0,
       0,   768,     0,     0,     0,   771,     0,   773,   774,     0,
       0,     0,     0,   777,   455,     0,     0,     0,     3,     0,
     456,   457,   458,     0,   459,     0,   274,   275,   276,   277,
     395,   396,     0,    41,     0,   789,     0,   460,   278,   461,
     462,     0,     0,   795,     0,     0,     0,     0,     0,   463,
     279,     0,     0,   464,   280,     0,   281,     0,     9,   282,
     465,     0,   466,   283,     0,     0,   467,   468,     0,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     284,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   192,   193,   194,     0,     0,   197,   198,   199,   200,
       0,     0,   285,   469,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,   286,   287,     0,     0,     0,     0,
     405,     0,   406,   407,   408,   409,     0,     0,   410,   451,
       0,     0,     0,     0,     0,     0,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,    40,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   299,   300,     0,     0,     0,     0,   301,   302,
       0,     0,   424,     0,   425,     0,     0,     0,   303,     0,
     304,   305,    41,   470,   263,   471,   455,     0,     0,     0,
       3,     0,   456,   457,   458,     0,   459,     0,   274,   275,
     276,   277,     0,     0,     0,     0,     0,     0,     0,   460,
     278,   461,   462,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   279,     0,     0,   464,   280,     0,   281,     0,
       9,   282,   465,     0,   466,   283,     0,     0,   467,   468,
       0,     0,     0,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   284,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   192,   193,   194,     0,     0,   197,   198,
     199,   200,     0,     0,   285,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   286,   287,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     244,   451,     0,     0,     0,   245,     0,     0,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,    40,   297,
       0,     0,     0,     0,   246,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   299,   300,     0,     0,     0,     0,
     301,   302,     0,     0,     0,     0,   274,   275,   276,   277,
     303,     0,   304,   305,    41,   470,   263,   764,   278,     0,
       0,     0,     0,   244,   244,     0,     0,     0,   245,   245,
     279,     0,     0,     0,   280,     0,   281,     0,     0,   282,
     247,     0,     0,   283,     0,     0,     0,   246,   246,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     284,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     248,   192,   193,   194,   249,     0,   197,   198,   199,   200,
       0,     0,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,     0,   286,   287,     0,     0,     0,     0,
       0,     0,   252,   247,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   505,   297,     0,     0,
       0,     0,     0,   248,   248,     0,     0,   249,   249,   430,
       0,   298,   299,   300,     0,     0,     0,     0,   301,   302,
       0,   274,   275,   276,   277,   250,   250,   506,   507,     0,
     304,   305,    41,   278,   263,   252,   252,     0,   244,   244,
       0,     0,     0,   245,   245,   279,     0,     0,     0,   280,
       0,   281,     0,     0,   282,     0,     0,     0,   283,     0,
       0,     0,   246,   246,     0,     0,   172,   173,   174,     0,
     176,   177,   178,   179,   180,   284,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   192,   193,   194,     0,
       0,   197,   198,   199,   200,     0,     0,   285,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   286,
     287,     0,     0,     0,     0,     0,     0,     0,   247,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   505,   297,     0,     0,     0,     0,     0,   248,   248,
       0,     0,   249,   249,   431,     0,   298,   299,   300,     0,
       0,     0,     0,   301,   302,     0,   274,   275,   276,   277,
     250,   250,     0,   516,     0,   304,   305,    41,   278,   263,
     252,   252,     0,     0,     0,     0,     0,     0,     0,     0,
     279,     0,     0,     0,   280,     0,   281,     0,     0,   282,
       0,     0,     0,   283,     0,     0,     0,     0,     0,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     284,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   192,   193,   194,     0,     0,   197,   198,   199,   200,
       0,     0,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,   287,     0,     0,     0,     0,
       0,     0,     0,   594,     0,     0,     0,     0,   244,   451,
       0,     0,     0,   245,     0,     0,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,    40,   297,     0,     0,
       0,     0,   246,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   299,   300,   274,   275,   276,   277,   301,   302,
       0,     0,     0,     0,     0,     0,   278,     0,   303,     0,
     304,   305,    41,   595,   263,     0,     0,     0,   279,     0,
       0,     0,   280,     0,   281,     0,     0,   282,     0,     0,
       0,   283,     0,     0,     0,     0,     0,     0,   247,   172,
     173,   174,     0,   176,   177,   178,   179,   180,   284,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   192,
     193,   194,     0,     0,   197,   198,   199,   200,   248,     0,
     285,     0,   249,     0,   436,     0,     0,     0,     0,     0,
       0,     0,   286,   287,     0,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,    40,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
     299,   300,     0,     0,     0,     0,   301,   302,     0,   274,
     275,   276,   277,     0,   528,     0,   303,   529,   304,   305,
      41,   278,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   279,     0,     0,     0,   280,     0,   281,
       0,     0,   282,     0,     0,     0,   283,     0,     0,     0,
       0,     0,     0,     0,   172,   173,   174,     0,   176,   177,
     178,   179,   180,   284,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   192,   193,   194,     0,     0,   197,
     198,   199,   200,     0,     0,   285,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   286,   287,     0,
       0,     0,     0,     0,     0,     0,   605,     0,     0,     0,
       0,     0,   451,     0,     0,     0,     0,     0,     0,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,    40,
     297,     0,     0,     0,     0,     0,     0,     0,   274,   275,
     276,   277,   625,     0,   298,   299,   300,     0,     0,     0,
     278,   301,   302,     0,     0,     0,     0,     0,     0,     0,
       0,   303,   279,   304,   305,    41,   280,   263,   281,     0,
       0,   282,     0,     0,     0,   283,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   284,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   192,   193,   194,     0,     0,   197,   198,
     199,   200,     0,     0,   285,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   286,   287,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,    40,   297,
       0,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   626,     0,   298,   299,   300,     0,     0,     0,   278,
     301,   302,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   279,   304,   305,    41,   280,   263,   281,     0,     0,
     282,     0,     0,     0,   283,     0,     0,     0,     0,     0,
       0,     0,   172,   173,   174,     0,   176,   177,   178,   179,
     180,   284,   182,   183,   184,   185,   186,   187,   188,   189,
     190,     0,   192,   193,   194,     0,     0,   197,   198,   199,
     200,     0,     0,   285,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   286,   287,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,    40,   297,     0,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   277,
       0,     0,   298,   299,   300,     0,     0,     0,   278,   301,
     302,   244,     0,     0,     0,     0,   245,     0,   347,   303,
     279,   304,   305,    41,   280,   263,   281,     0,     0,   282,
       0,     0,     0,   283,     0,   246,     0,     0,     0,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     284,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   192,   193,   194,     0,     0,   197,   198,   199,   200,
       0,     0,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,   287,     0,     0,     0,     0,
       0,   247,     0,     0,     0,     0,     0,     0,   244,   451,
       0,     0,     0,   245,     0,     0,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,    40,   297,     0,     0,
       0,   248,   246,     0,     0,   249,     0,   438,     0,     0,
       0,   298,   299,   300,   274,   275,   276,   277,   301,   302,
       0,   550,     0,   250,     0,     0,   278,     0,   303,     0,
     304,   305,    41,   252,   263,     0,     0,     0,   279,     0,
       0,     0,   280,     0,   281,     0,     0,   282,     0,     0,
       0,   283,     0,     0,     0,     0,     0,     0,   247,   172,
     173,   174,     0,   176,   177,   178,   179,   180,   284,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   192,
     193,   194,     0,     0,   197,   198,   199,   200,   248,     0,
     285,     0,   249,     0,   440,     0,     0,     0,     0,     0,
       0,     0,   286,   287,     0,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,    40,   297,     0,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,     0,     0,   298,
     299,   300,     0,     0,     0,   278,   301,   302,   244,     0,
       0,     0,     0,   245,     0,     0,   303,   279,   304,   305,
      41,   280,   263,   281,     0,     0,   282,     0,     0,     0,
     283,     0,   246,     0,     0,     0,     0,     0,   172,   173,
     174,     0,   176,   177,   178,   179,   180,   284,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   192,   193,
     194,     0,     0,   197,   198,   199,   200,     0,     0,   285,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,   287,     0,     0,     0,     0,     0,   247,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
     245,     0,     0,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,    40,   297,     0,     0,     0,   248,   246,
       0,     0,   249,     0,     0,     0,     0,     0,   298,   299,
     300,   274,   275,   276,   277,   301,   302,     0,     0,     0,
     250,     0,   496,   278,     0,   303,   564,   304,   305,    41,
     252,   263,     0,     0,     0,   279,     0,     0,     0,   280,
       0,   281,     0,     0,   282,     0,     0,     0,   283,     0,
       0,   603,     0,     0,     0,   247,   172,   173,   174,     0,
     176,   177,   178,   179,   180,   284,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   192,   193,   194,     0,
       0,   197,   198,   199,   200,   248,     0,   285,     0,   249,
       0,   629,     0,     0,     0,     0,     0,     0,     0,   286,
     287,     0,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,    40,   297,     0,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   277,     0,     0,   298,   299,   300,     0,
       0,     0,   278,   301,   302,   244,     0,     0,     0,     0,
     245,     0,     0,   303,   279,   304,   305,    41,   280,   263,
     281,     0,     0,   282,     0,     0,     0,   283,     0,   246,
       0,     0,     0,     0,     0,   172,   173,   174,     0,   176,
     177,   178,   179,   180,   284,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   192,   193,   194,     0,     0,
     197,   198,   199,   200,     0,     0,   285,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   286,   287,
       0,     0,     0,     0,     0,   247,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,   245,     0,     0,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
      40,   297,     0,     0,     0,   248,   246,     0,     0,   249,
       0,   630,     0,     0,     0,   298,   299,   300,   274,   275,
     276,   277,   301,   302,     0,     0,     0,   250,     0,     0,
     278,     0,   303,   623,   304,   305,    41,   252,   263,     0,
       0,     0,   279,     0,     0,     0,   280,     0,   281,     0,
       0,   282,     0,     0,     0,   283,     0,     0,     0,     0,
       0,     0,   247,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   284,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   192,   193,   194,     0,     0,   197,   198,
     199,   200,   248,     0,   285,     0,   249,     0,   632,     0,
       0,     0,     0,     0,     0,     0,   286,   287,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
     244,     0,     0,     0,   252,   245,     0,     0,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,    40,   297,
       0,     0,     0,     0,   246,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   299,   300,   274,   275,   276,   277,
     301,   302,     0,     0,     0,     0,     0,     0,   278,     0,
     303,   637,   304,   305,    41,     0,   263,     0,     0,     0,
     279,     0,     0,     0,   280,     0,   281,     0,     0,   282,
       0,     0,     0,   283,     0,     0,     0,     0,     0,     0,
     247,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     284,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   192,   193,   194,     0,     0,   197,   198,   199,   200,
     248,     0,   285,     0,   249,     0,   633,     0,     0,     0,
       0,     0,     0,     0,   286,   287,     0,     0,     0,     0,
       0,     0,   250,     0,     0,     0,     0,     0,   244,     0,
       0,     0,   252,   245,     0,     0,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,    40,   297,     0,     0,
       0,     0,   246,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   299,   300,   274,   275,   276,   277,   301,   302,
       0,     0,     0,     0,     0,     0,   278,     0,   303,   645,
     304,   305,    41,     0,   263,     0,     0,     0,   279,     0,
       0,     0,   280,     0,   281,     0,     0,   282,     0,     0,
       0,   283,     0,     0,     0,     0,     0,     0,   247,   172,
     173,   174,     0,   176,   177,   178,   179,   180,   284,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   192,
     193,   194,     0,     0,   197,   198,   199,   200,   248,     0,
     285,     0,   249,     0,   666,     0,     0,     0,     0,     0,
       0,     0,   286,   287,     0,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,   700,     0,     0,
     252,     0,     0,     0,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,    40,   297,     0,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,     0,     0,   298,
     299,   300,     0,     0,     0,   278,   301,   302,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   279,   304,   305,
      41,   280,   263,   281,     0,     0,   282,     0,     0,     0,
     283,     0,     0,     0,     0,     0,     0,     0,   172,   173,
     174,     0,   176,   177,   178,   179,   180,   284,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   192,   193,
     194,     0,     0,   197,   198,   199,   200,     0,     0,   285,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,   287,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   701,     0,     0,     0,
       0,     0,     0,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,    40,   297,     0,     0,     0,     0,     0,
       0,     0,   274,   275,   276,   277,     0,     0,   298,   299,
     300,     0,     0,     0,   278,   301,   302,   244,     0,     0,
       0,     0,   245,     0,     0,   303,   279,   304,   305,    41,
     280,   263,   281,     0,     0,   282,     0,     0,     0,   283,
       0,   246,     0,     0,     0,     0,     0,   172,   173,   174,
       0,   176,   177,   178,   179,   180,   284,   182,   183,   184,
     185,   186,   187,   188,   189,   190,     0,   192,   193,   194,
       0,     0,   197,   198,   199,   200,     0,     0,   285,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     286,   287,     0,     0,     0,     0,     0,   247,     0,     0,
       0,     0,     0,     0,   244,     0,     0,     0,     0,   245,
       0,     0,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    40,   297,     0,     0,     0,   248,   246,     0,
       0,   249,     0,   726,     0,     0,     0,   298,   299,   300,
     274,   275,   276,   277,   301,   302,     0,     0,     0,   250,
       0,     0,   278,     0,   303,   732,   304,   305,    41,   252,
     263,     0,     0,     0,   279,     0,     0,     0,   280,     0,
     281,     0,     0,   282,     0,     0,     0,   283,     0,     0,
       0,     0,     0,     0,   247,   172,   173,   174,     0,   176,
     177,   178,   179,   180,   284,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   192,   193,   194,     0,     0,
     197,   198,   199,   200,   248,     0,   285,     0,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   286,   287,
       0,     0,     0,     0,     0,     0,   250,     0,     0,   747,
       0,     0,   244,     0,     0,     0,   252,   245,     0,     0,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
      40,   297,     0,     0,     0,     0,   246,     0,     0,     0,
       0,     0,     0,     0,     0,   298,   299,   300,   274,   275,
     276,   277,   301,   302,     0,     0,     0,     0,     0,     0,
     278,     0,   303,   750,   304,   305,    41,     0,   263,     0,
       0,     0,   279,     0,     0,     0,   280,     0,   281,     0,
       0,   282,     0,     0,     0,   283,     0,     0,     0,     0,
       0,     0,   247,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   284,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   192,   193,   194,     0,     0,   197,   198,
     199,   200,   248,     0,   285,     0,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   286,   287,     0,     0,
       0,     0,     0,     0,   250,   760,     0,   787,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,    40,   297,
       0,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   767,     0,   298,   299,   300,     0,     0,     0,   278,
     301,   302,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   279,   304,   305,    41,   280,   263,   281,     0,     0,
     282,     0,     0,     0,   283,     0,     0,     0,     0,     0,
       0,     0,   172,   173,   174,     0,   176,   177,   178,   179,
     180,   284,   182,   183,   184,   185,   186,   187,   188,   189,
     190,     0,   192,   193,   194,     0,     0,   197,   198,   199,
     200,     0,     0,   285,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   286,   287,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,    40,   297,     0,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   277,
     788,     0,   298,   299,   300,     0,     0,     0,   278,   301,
     302,     0,     0,     0,     0,     0,     0,     0,     0,   303,
     279,   304,   305,    41,   280,   263,   281,     0,     0,   282,
       0,     0,     0,   283,     0,     0,     0,     0,     0,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     284,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   192,   193,   194,     0,     0,   197,   198,   199,   200,
       0,     0,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,   287,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,    40,   297,     0,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   277,     0,
       0,   298,   299,   300,     0,     0,     0,   278,   301,   302,
     244,     0,     0,     0,     0,   245,     0,     0,   303,   279,
     304,   305,    41,   280,   263,   281,     0,     0,   282,     0,
       0,     0,   283,     0,   246,     0,     0,     0,     0,     0,
     172,   173,   174,     0,   176,   177,   178,   179,   180,   284,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     192,   193,   194,     0,     0,   197,   198,   199,   200,     0,
       0,   285,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   286,   287,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,    40,   297,     0,     0,     0,
     248,   379,   395,   396,   249,     0,     0,     0,     0,     0,
     298,   299,   300,   168,     0,     0,     0,   301,   302,   169,
       0,     0,   250,     0,     0,   798,     0,   303,     0,   304,
     305,    41,   252,   263,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,     0,
       0,     0,   397,   398,   399,   400,   401,     0,     0,   402,
     403,   404,   405,     0,   406,   407,   408,   409,   395,   396,
     410,     0,   411,   412,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,   413,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
       0,     0,     0,     0,   263,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,   578,   579,   580,   581,   582,   583,   584,   397,   398,
     399,   400,   401,   585,   586,   402,   403,   404,   405,   587,
     406,   407,   408,   409,   395,   396,   410,   588,   411,   412,
     589,   590,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,     0,     0,     0,     0,     0,     0,     0,
     424,     0,   425,     0,     0,     0,     0,     0,     0,   622,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,   579,   580,
     581,   582,   583,   584,   397,   398,   399,   400,   401,   585,
     586,   402,   403,   404,   405,   587,   406,   407,   408,   409,
     395,   396,   410,   588,   411,   412,   589,   590,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,   413,     0,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,   425,     0,
       0,     0,     0,     0,     0,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   579,   580,   581,   582,   583,   584,
     397,   398,   399,   400,   401,   585,   586,   402,   403,   404,
     405,   587,   406,   407,   408,   409,   395,   396,   410,   588,
     411,   412,   589,   590,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   591,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,   425,     0,     0,     0,     0,     0,
       0,   695,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,   578,
     579,   580,   581,   582,   583,   584,   397,   398,   399,   400,
     401,   585,   586,   402,   403,   404,   405,   587,   406,   407,
     408,   409,   395,   396,   410,   588,   411,   412,   589,   590,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,   413,     0,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
     425,     0,     0,     0,     0,     0,     0,   702,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,   579,   580,   581,   582,
     583,   584,   397,   398,   399,   400,   401,   585,   586,   402,
     403,   404,   405,   587,   406,   407,   408,   409,   395,   396,
     410,   588,   411,   412,   589,   590,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,   413,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
       0,     0,     0,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,   396,     0,     0,     0,     0,     0,
     577,   578,   579,   580,   581,   582,   583,   584,   397,   398,
     399,   400,   401,   585,   586,   402,   403,   404,   405,   587,
     406,   407,   408,   409,     0,     0,   410,   588,   411,   412,
     589,   590,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,     0,     0,   395,   396,     0,     0,     0,
     424,     0,   425,   397,   398,   399,   400,   401,     0,   738,
     402,   403,   404,   405,     0,   406,   407,   408,   409,     0,
       0,   410,     0,   411,   412,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   413,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,     0,     0,     0,
     395,   396,     0,     0,     0,   424,     0,   425,     0,     0,
       0,     0,     0,     0,   426,   397,   398,   399,   400,   401,
       0,     0,   402,   403,   404,   405,     0,   406,   407,   408,
     409,     0,     0,   410,     0,   411,   412,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   413,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   395,
     396,     0,     0,     0,     0,     0,     0,   424,     0,   425,
     397,   398,   399,   400,   401,     0,   783,   402,   403,   404,
     405,     0,   406,   407,   408,   409,     0,     0,   410,     0,
     411,   412,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,     0,     0,     0,   395,   396,     0,
       0,     0,   424,     0,   425,     0,     0,   525,     0,   397,
     398,   399,   400,   401,     0,     0,   402,   403,   404,   405,
       0,   406,   407,   408,   409,     0,     0,   410,     0,   411,
     412,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   395,   396,     0,     0,     0,
       0,   424,     0,   425,     0,     0,   628,   397,   398,   399,
     400,   401,     0,     0,   402,   403,   404,   405,     0,   406,
     407,   408,   409,     0,     0,   410,     0,   411,   412,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   395,   396,     0,     0,     0,     0,   424,
       0,   425,     0,     0,   631,   397,   398,   399,   400,   401,
       0,     0,   402,   403,   404,   405,     0,   406,   407,   408,
     409,     0,     0,   410,     0,   411,   412,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   413,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   395,   396,     0,     0,     0,     0,   424,     0,   425,
       0,     0,   706,   397,   398,   399,   400,   401,     0,     0,
     402,   403,   404,   405,     0,   406,   407,   408,   409,     0,
       0,   410,     0,   411,   412,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   413,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   395,
     396,     0,     0,     0,     0,   424,     0,   425,     0,     0,
     770,   397,   398,   399,   400,   401,     0,     0,   402,   403,
     404,   405,     0,   406,   407,   408,   409,     0,     0,   410,
       0,   411,   412,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   413,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,   425,     0,     0,   781,   397,
     398,   399,   400,   401,     0,     0,   402,   403,   404,   405,
       0,   406,   407,   408,   409,     0,     0,   410,     0,   411,
     412,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,     0,     0,     0,     0,     0,   169,
       0,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,     0,   170,     0,     0,     0,     0,     0,
       0,   424,     0,   425,     0,     0,   794,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,     0,
       0,     0,     0,     0,     0,   395,   396,     0,     0,     0,
     168,     0,     0,     0,     0,     0,   169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,     0,     0,   288,   289,   290,     0,     0,     0,
       0,     0,     0,    40,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,     0,     0,     0,    41,
       0,     0,     0,     0,     0,   397,   398,   399,   400,   401,
       0,     0,   402,   403,   404,   405,     0,   406,   407,   408,
     409,     0,     0,   410,     0,   411,   412,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,     0,     0,   168,     0,     0,     0,     0,     0,   169,
     415,   416,   417,   418,   419,   420,   421,   422,   423,     0,
       0,     0,     0,     0,   170,     0,     0,   424,     0,   425,
       0,     0,   147,     0,     0,     0,    41,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,     0,
     395,   396,     0,     0,     0,     0,     0,     0,     0,     0,
     168,     0,     0,     0,     0,     0,   169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,     0,   395,   396,    41,
     397,   398,   399,   400,   401,     0,     0,   402,   403,   404,
     405,     0,   406,   407,   408,   409,     0,     0,   410,     0,
     411,   412,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,     0,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,   425,   448,   395,   396,     0,     0,
       0,     0,     0,     0,     0,     0,    41,   397,   398,   399,
     400,   401,     0,     0,   402,   403,   404,   405,     0,   406,
     407,   408,   409,     0,     0,   410,     0,   411,   412,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,     0,     0,     0,   424,
       0,   425,   661,   395,   396,     0,   397,   398,   399,   400,
     401,     0,     0,   402,   403,   404,   405,     0,   406,   407,
     408,   409,     0,     0,   410,     0,   411,   412,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
     425,   664,     0,     0,     0,   577,   578,   579,   580,   581,
     582,   583,   584,   397,   398,   399,   400,   401,   585,   586,
     402,   403,   404,   405,   587,   406,   407,   408,   409,  -193,
       0,   410,   588,   411,   412,   589,   590,   395,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   591,   413,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   396,     0,     0,     0,     0,     0,   577,
     578,   579,   580,   581,   582,   583,   584,   397,   398,   399,
     400,   401,   585,   586,   402,   403,   404,   405,   618,   406,
     407,   408,   409,     0,     0,   410,   588,   411,   412,   589,
     590,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   619,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   395,   396,     0,     0,     0,     0,     0,     0,   424,
       0,   425,   397,   398,   399,   400,   401,     0,     0,   402,
     403,   404,   405,     0,   406,   407,   408,   409,     0,     0,
     410,     0,   411,   412,     0,     0,   519,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   413,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   395,   396,     0,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
       0,   397,   398,   399,   400,   401,     0,     0,   402,   403,
     404,   405,     0,   406,   407,   408,   409,     0,     0,   410,
       0,   411,   412,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   413,   663,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   395,   396,     0,     0,     0,
       0,     0,     0,   424,     0,   425,   397,   398,   399,   400,
     401,     0,     0,   402,   403,   404,   405,     0,   406,   407,
     408,   409,     0,     0,   410,     0,   411,   412,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   724,     0,     0,   413,     0,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     395,   396,     0,     0,     0,     0,     0,     0,   424,     0,
     425,     0,     0,     0,     0,   397,   398,   399,   400,   401,
       0,     0,   402,   403,   404,   405,     0,   406,   407,   408,
     409,     0,     0,   410,     0,   411,   412,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   413,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   395,
     396,     0,     0,     0,     0,     0,     0,   424,     0,   425,
     397,   398,   399,   400,   401,     0,     0,   402,   403,   404,
     405,     0,   406,   407,   408,   409,     0,     0,   410,     0,
     411,   412,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   416,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   244,     0,     0,     0,
       0,   245,   424,     0,   425,     0,     0,     0,     0,   397,
     398,   399,   400,   401,     0,     0,   402,   403,   404,   405,
     246,   406,   407,   408,   409,     0,     0,   410,     0,   411,
     412,     0,     0,     0,     0,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,     0,     0,     0,   417,   418,   419,   420,
     421,   422,   423,   129,   130,   131,   132,     0,     0,     0,
       0,   424,     0,   425,     0,   362,   247,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,     0,     0,   143,
     144,     0,     0,     0,     0,   364,   248,     0,     0,     0,
     249,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,   492,     0,     0,   252
};

static const yytype_int16 yycheck[] =
{
      12,   102,   259,   359,   149,   290,   218,     0,   427,   332,
     357,   334,     5,   336,   239,   240,   241,   380,   365,   225,
     451,   102,    67,   110,    33,    20,    32,   257,    18,    45,
      13,    14,   115,   112,   113,    28,    18,    30,   133,    32,
     133,    34,   133,    18,   133,   109,    39,    31,   112,   113,
     528,   146,    45,   140,   123,    17,    68,    52,    51,   133,
     163,   133,    31,   139,   139,   163,   133,    36,   166,   172,
     133,   133,   142,     4,   169,     6,   169,   496,    13,    14,
     110,   172,   169,   172,   163,   140,    55,   170,   118,   110,
     102,   166,   101,   102,   170,    88,   108,   161,   172,   163,
     172,   171,   171,   165,    31,   172,    31,   127,   147,    36,
     140,    36,   171,   460,    10,   170,   217,   133,   139,   140,
     350,   142,   469,   224,   133,    21,    22,   133,    55,   133,
      55,   170,   233,   234,   235,   236,   217,   139,   239,   240,
     241,   242,   111,   224,   127,   576,   493,   139,   131,   139,
     133,   134,   233,   234,   235,   236,   151,   139,   239,   240,
     241,   145,   640,   169,   139,   266,   161,   212,   170,   394,
     163,   133,   141,   110,   166,   276,   145,   170,   147,   163,
     139,    56,   165,   110,   111,   266,   111,   288,   289,   173,
     142,   164,   127,   450,   163,   276,   131,   170,   133,   134,
     140,    19,    20,   140,   173,   217,   139,   288,   289,   532,
     139,   170,   224,   140,   141,   634,   141,   169,   145,   147,
     145,   233,   234,   235,   236,   644,   139,   239,   240,   241,
     242,   139,   377,   166,   379,   520,   163,   166,   163,   445,
     139,   660,   170,    19,    20,   170,   173,   594,   173,   170,
     462,   172,   133,   166,   266,   356,   164,   147,   605,   166,
     139,   139,   164,   170,   276,   147,   685,   166,   170,   370,
     371,   133,   373,   374,   637,   356,   288,   289,   135,   136,
     170,   164,   645,   101,   102,   164,   164,   170,   170,   370,
     371,   109,   373,   374,   112,   113,   114,   139,    62,   117,
     142,   526,    66,   145,   167,   168,   133,   139,   171,   142,
     139,    19,    20,   169,   459,   135,   136,   137,   133,    83,
      84,    85,    86,   133,   165,   101,   102,   133,   429,   652,
     170,   127,   172,   109,   171,   111,   112,   113,   114,   133,
     441,   117,   443,   161,   356,   163,   171,   170,   429,   172,
     133,   170,   133,   172,   140,   133,    33,   142,   370,   371,
     441,   373,   374,   171,    33,   165,    19,    20,     1,   135,
     133,    52,   148,   149,   150,   151,   152,   142,   171,   391,
     592,   170,   142,   739,   146,   161,   170,   163,   600,   146,
     146,    99,   100,   101,   102,   103,   146,   165,   106,   611,
     146,   109,   146,   111,   112,   113,   114,   146,   146,   117,
     146,   119,   120,   146,   231,   133,   171,   429,   133,    42,
     133,   170,   163,   133,   146,   526,   146,   146,   165,   441,
     165,   443,   533,   250,   165,   146,   168,   163,   146,   147,
     148,   149,   150,   151,   152,   526,   658,   145,   101,   102,
     166,   165,   533,   161,   133,   163,   109,   714,   111,   112,
     113,   114,   164,   133,   117,   133,   133,   133,   133,   286,
     287,   616,   147,   290,   147,   147,   115,   689,   170,   127,
      35,   298,   299,   300,   301,   302,   303,   170,   170,   139,
     170,    18,   170,   164,   170,   139,   133,   150,   151,   152,
     135,   165,    31,   133,    12,   127,   142,    36,   161,   172,
     163,   171,    38,   133,   526,   133,   165,   164,   164,   164,
     147,   533,   164,   166,   625,   133,    55,    48,   740,   384,
     164,   147,   164,   164,   172,   164,   164,    19,    20,    51,
     357,    91,   359,    54,   625,   779,    52,   696,   365,    68,
       1,   763,   369,   312,   766,   372,   211,   261,   443,   243,
     361,   378,   616,   380,   165,    64,   228,   276,   380,   774,
     643,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     397,   398,   111,    -1,   401,   402,   403,   404,    -1,   406,
      -1,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,    -1,   425,    -1,
      -1,    -1,   141,   625,    -1,    -1,   145,    99,   100,    -1,
      -1,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,   163,   117,    -1,   119,   120,   456,
      -1,   170,    -1,   460,   173,    -1,   463,   464,    -1,    19,
      20,   468,   469,    -1,    -1,    -1,    -1,   474,    -1,   141,
      -1,   143,   144,   145,   146,   147,   767,    -1,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,   161,
     497,   163,    -1,    -1,    -1,    -1,   767,   788,    -1,    -1,
     507,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,
      -1,    -1,   519,   520,    -1,    -1,   523,   788,    -1,    -1,
      -1,    -1,    -1,    -1,   726,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   111,   112,   113,   114,   757,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,   767,   573,    -1,    -1,    -1,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     587,   588,   589,   590,   591,    -1,   788,   594,   148,   149,
     150,   151,   152,    -1,    -1,    -1,    -1,    -1,   605,    -1,
      -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   618,   619,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,   629,   630,    -1,   632,    -1,    23,   635,    -1,
      -1,    -1,    -1,    29,   641,    -1,   643,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,   656,
      -1,    -1,    -1,    -1,    -1,    -1,   663,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   700,   701,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   710,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   729,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   739,    -1,    -1,    -1,   743,   133,    -1,    -1,
      -1,   748,    -1,    -1,    -1,   752,    -1,   754,   755,    -1,
      -1,    -1,    -1,   760,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
      19,    20,    -1,   169,    -1,   782,    -1,    24,    25,    26,
      27,    -1,    -1,   790,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   101,   102,    -1,    -1,    -1,    -1,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,   161,    -1,   163,    -1,    -1,    -1,   165,    -1,
     167,   168,   169,   170,   171,   172,     1,    -1,    -1,    -1,
       5,    -1,     7,     8,     9,    -1,    11,    -1,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    47,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,   116,    -1,    -1,    -1,    36,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
     165,    -1,   167,   168,   169,   170,   171,   172,    25,    -1,
      -1,    -1,    -1,    31,    31,    -1,    -1,    -1,    36,    36,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
     111,    -1,    -1,    50,    -1,    -1,    -1,    55,    55,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
     141,    78,    79,    80,   145,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   111,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,   141,   141,    -1,    -1,   145,   145,   147,
      -1,   148,   149,   150,    -1,    -1,    -1,    -1,   155,   156,
      -1,    13,    14,    15,    16,   163,   163,   164,   165,    -1,
     167,   168,   169,    25,   171,   173,   173,    -1,    31,    31,
      -1,    -1,    -1,    36,    36,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    55,    55,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,   141,   141,
      -1,    -1,   145,   145,   147,    -1,   148,   149,   150,    -1,
      -1,    -1,    -1,   155,   156,    -1,    13,    14,    15,    16,
     163,   163,    -1,   165,    -1,   167,   168,   169,    25,   171,
     173,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    31,   116,
      -1,    -1,    -1,    36,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    13,    14,    15,    16,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,    -1,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   111,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,   141,    -1,
      89,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,    -1,    -1,    -1,    -1,   155,   156,    -1,    13,
      14,    15,    16,    -1,   163,    -1,   165,   166,   167,   168,
     169,    25,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    17,    -1,   148,   149,   150,    -1,    -1,    -1,
      25,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    37,   167,   168,   169,    41,   171,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,
      16,   146,    -1,   148,   149,   150,    -1,    -1,    -1,    25,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    37,   167,   168,   169,    41,   171,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,    25,   155,
     156,    31,    -1,    -1,    -1,    -1,    36,    -1,   164,   165,
      37,   167,   168,   169,    41,   171,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    31,   116,
      -1,    -1,    -1,    36,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,   141,    55,    -1,    -1,   145,    -1,   147,    -1,    -1,
      -1,   148,   149,   150,    13,    14,    15,    16,   155,   156,
      -1,    20,    -1,   163,    -1,    -1,    25,    -1,   165,    -1,
     167,   168,   169,   173,   171,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   111,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,   141,    -1,
      89,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,   148,
     149,   150,    -1,    -1,    -1,    25,   155,   156,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,   165,    37,   167,   168,
     169,    41,   171,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    55,    -1,    -1,    -1,    -1,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    84,    85,    86,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,   141,    55,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,    13,    14,    15,    16,   155,   156,    -1,    -1,    -1,
     163,    -1,   165,    25,    -1,   165,   166,   167,   168,   169,
     173,   171,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,   111,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    84,    85,    86,   141,    -1,    89,    -1,   145,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    15,    16,    -1,    -1,   148,   149,   150,    -1,
      -1,    -1,    25,   155,   156,    31,    -1,    -1,    -1,    -1,
      36,    -1,    -1,   165,    37,   167,   168,   169,    41,   171,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    -1,
      83,    84,    85,    86,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,   141,    55,    -1,    -1,   145,
      -1,   147,    -1,    -1,    -1,   148,   149,   150,    13,    14,
      15,    16,   155,   156,    -1,    -1,    -1,   163,    -1,    -1,
      25,    -1,   165,   166,   167,   168,   169,   173,   171,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,   141,    -1,    89,    -1,   145,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   173,    36,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,    13,    14,    15,    16,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
     165,   166,   167,   168,   169,    -1,   171,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
     141,    -1,    89,    -1,   145,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   173,    36,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,    13,    14,    15,    16,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   165,   166,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   111,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,   141,    -1,
      89,    -1,   145,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,   148,
     149,   150,    -1,    -1,    -1,    25,   155,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    37,   167,   168,
     169,    41,   171,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    84,    85,    86,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    14,    15,    16,    -1,    -1,   148,   149,
     150,    -1,    -1,    -1,    25,   155,   156,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,   165,    37,   167,   168,   169,
      41,   171,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      -1,    -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,   141,    55,    -1,
      -1,   145,    -1,   147,    -1,    -1,    -1,   148,   149,   150,
      13,    14,    15,    16,   155,   156,    -1,    -1,    -1,   163,
      -1,    -1,    25,    -1,   165,   166,   167,   168,   169,   173,
     171,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    -1,
      83,    84,    85,    86,   141,    -1,    89,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,   166,
      -1,    -1,    31,    -1,    -1,    -1,   173,    36,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,    13,    14,
      15,    16,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   165,   166,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,   141,    -1,    89,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,   163,   110,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,
      16,    17,    -1,   148,   149,   150,    -1,    -1,    -1,    25,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    37,   167,   168,   169,    41,   171,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      17,    -1,   148,   149,   150,    -1,    -1,    -1,    25,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      37,   167,   168,   169,    41,   171,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,
      -1,   148,   149,   150,    -1,    -1,    -1,    25,   155,   156,
      31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   165,    37,
     167,   168,   169,    41,   171,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
     141,    11,    19,    20,   145,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,    23,    -1,    -1,    -1,   155,   156,    29,
      -1,    -1,   163,    -1,    -1,   166,    -1,   165,    -1,   167,
     168,   169,   173,   171,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    99,   100,   101,   102,   103,    -1,    -1,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    19,    20,
     117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    19,    20,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      19,    20,   117,   118,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    19,    20,   117,   118,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    19,    20,   117,   118,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    19,    20,
     117,   118,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
     161,    -1,   163,    99,   100,   101,   102,   103,    -1,   170,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,    -1,
      19,    20,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
      99,   100,   101,   102,   103,    -1,   170,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    -1,    19,    20,    -1,
      -1,    -1,   161,    -1,   163,    -1,    -1,   166,    -1,    99,
     100,   101,   102,   103,    -1,    -1,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,   161,    -1,   163,    -1,    -1,   166,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,   161,
      -1,   163,    -1,    -1,   166,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,    19,    20,    -1,    -1,    -1,    -1,   161,    -1,   163,
      -1,    -1,   166,    99,   100,   101,   102,   103,    -1,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
     166,    99,   100,   101,   102,   103,    -1,    -1,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,    -1,    -1,   166,    99,
     100,   101,   102,   103,    -1,    -1,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
      -1,   141,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,    -1,    -1,   166,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,   161,    -1,   163,
      -1,    -1,   165,    -1,    -1,    -1,   169,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    19,    20,   169,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,   141,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,   164,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    99,   100,   101,
     102,   103,    -1,    -1,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,   164,    19,    20,    -1,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,   164,    -1,    -1,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,    99,   100,   101,   102,   103,    -1,    -1,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,   103,    -1,    -1,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,    99,   100,   101,   102,
     103,    -1,    -1,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,   141,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,    -1,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,   161,    -1,   163,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,   103,    -1,    -1,   106,   107,   108,   109,
      55,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
     120,    -1,    -1,    -1,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    -1,    -1,    -1,   146,   147,   148,   149,
     150,   151,   152,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   161,    -1,   163,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,   173
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    88,   163,   170,   176,   181,   182,   186,   205,   209,
     223,   255,   260,   261,   264,   273,   274,   133,   200,   201,
     127,   187,   188,   133,   183,   184,   133,   171,   133,    32,
     133,   169,   202,   203,   204,   214,     4,     6,    56,   254,
     133,   140,   139,   142,   139,    20,    52,   151,   161,   185,
     142,   171,   133,   258,   259,   169,   133,   165,   139,   164,
     133,   263,    33,   101,   102,   133,   208,   171,   256,   171,
      13,    14,   127,   131,   133,   134,   165,   178,   200,   127,
     188,   133,   133,   133,    62,    66,    83,    84,    85,    86,
     267,   257,   140,   172,   258,   133,   201,   203,   142,   262,
     171,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   119,
     120,   121,   122,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   155,   156,    33,    33,   165,   206,   253,
       1,   133,   246,   247,   135,   177,    13,    14,   127,   131,
     133,   178,   198,   199,   185,   171,   133,   172,    23,    29,
      44,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   214,   266,   268,   269,   271,   272,   166,
     214,   241,   163,   166,   238,   242,   243,   142,   207,   133,
     172,   252,   265,   170,   142,   170,   135,   136,   139,   166,
     257,   140,   170,   146,   146,   146,   146,   165,   146,   146,
     146,   146,   146,   146,    31,    36,    55,   111,   141,   145,
     163,   170,   173,   172,   238,   240,   201,    45,   249,   265,
     166,   170,   272,   171,   210,   139,   142,   145,   251,   272,
     172,   246,   198,   172,    13,    14,    15,    16,    25,    37,
      41,    43,    46,    50,    67,    89,   101,   102,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   134,   148,   149,
     150,   155,   156,   165,   167,   168,   180,   210,   214,   216,
     225,   226,   229,   230,   231,   234,   235,   236,   237,   266,
     275,   278,   283,   285,   287,   272,   272,   272,   272,   133,
     133,   270,   206,   272,   206,   272,   206,   272,   133,   244,
     245,   272,   247,    31,   145,   163,   173,   164,   237,    42,
     239,   170,   164,   249,   110,   140,   142,   248,   242,   212,
     133,   272,   110,   118,   140,   250,   272,   275,   165,   165,
     146,   146,   165,   146,   146,   237,   237,    11,   272,    11,
     272,   237,   280,   284,   179,   237,   237,   237,   237,   237,
     237,   168,   133,   165,   205,    19,    20,    99,   100,   101,
     102,   103,   106,   107,   108,   109,   111,   112,   113,   114,
     117,   119,   120,   141,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   161,   163,   170,   165,   147,   170,
     147,   147,   166,   147,   170,   207,   147,   207,   147,   207,
     147,   142,   147,   170,   147,   170,   145,   164,   164,   249,
     272,   116,   213,   232,   237,     1,     7,     8,     9,    11,
      24,    26,    27,    36,    40,    47,    49,    53,    54,    90,
     170,   172,   189,   190,   193,   194,   195,   196,   197,   211,
     213,   215,   217,   218,   219,   220,   221,   223,   224,   232,
     233,   237,   170,   250,   213,   237,   165,   133,   237,   272,
     272,   237,   272,   272,   265,   133,   164,   165,   237,   276,
     277,   279,   280,   281,   282,   265,   165,   277,   282,   123,
     170,   172,   136,   137,   177,   166,   146,   214,   163,   166,
     227,   237,   206,    17,   133,   133,   237,   237,   237,   237,
     237,   237,   133,   237,   133,   237,   237,   237,   237,   237,
      20,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   133,   237,   166,   227,   272,   133,   147,   147,
     147,   272,   244,   248,   168,   230,   115,    91,    92,    93,
      94,    95,    96,    97,    98,   104,   105,   110,   118,   121,
     122,   140,   237,   265,   110,   170,   213,   237,   210,   237,
     237,   170,   127,    53,   237,   110,   213,   237,    35,   170,
     170,   237,   170,   170,   170,    18,   222,   170,   110,   140,
     213,   237,   170,   166,   227,    17,   146,   237,   166,   147,
     147,   166,   147,   147,    18,   248,   164,   166,   139,   164,
     170,   139,   164,   170,    18,   166,   172,   172,   237,   280,
     172,   237,   206,   272,   133,   276,   139,   166,   207,   272,
     165,   164,   133,   142,   164,   166,   147,   237,   230,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   210,    18,   213,   237,   170,    12,
     210,   142,   127,   213,   237,   170,   171,   210,    38,   252,
     116,   116,   170,   166,   272,   133,   166,   237,   237,   237,
     165,   227,   237,   277,   133,   164,   276,   237,   164,   281,
     227,   277,   164,   164,   138,   207,   147,   164,   237,   123,
     210,   228,   166,   227,   237,   227,   170,   210,   170,   212,
      10,    21,    22,   191,   192,   237,   237,   166,   147,   170,
     166,   237,   170,   164,   248,   170,   172,   147,   214,   166,
     110,   237,   166,   170,   172,   210,   237,    17,   237,   133,
     166,   237,   164,   237,   237,   164,   214,   237,   210,   210,
     272,   166,   147,   170,   286,   286,   192,   166,    17,   237,
      48,   164,   172,   272,   166,   237,   164,   164,   166
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   174,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   176,   177,   177,   178,   179,   179,
     179,   180,   181,   182,   183,   183,   183,   184,   184,   185,
     185,   186,   187,   187,   188,   188,   189,   190,   190,   191,
     191,   192,   192,   192,   193,   193,   194,   195,   196,   197,
     198,   198,   198,   198,   198,   198,   199,   199,   200,   200,
     200,   200,   200,   200,   200,   200,   201,   201,   202,   202,
     203,   203,   204,   204,   205,   205,   206,   206,   206,   207,
     207,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   209,   210,
     210,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   212,
     212,   212,   213,   213,   214,   214,   214,   215,   216,   216,
     216,   216,   217,   218,   219,   219,   219,   219,   219,   220,
     220,   220,   220,   221,   222,   222,   223,   223,   224,   225,
     225,   225,   226,   226,   226,   226,   226,   226,   227,   227,
     228,   228,   228,   229,   229,   230,   230,   231,   231,   231,
     231,   231,   231,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   234,   235,   235,   236,   236,   236,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     238,   238,   239,   239,   240,   241,   241,   242,   242,   243,
     243,   244,   244,   245,   245,   246,   247,   247,   248,   248,
     249,   249,   249,   249,   249,   250,   250,   250,   251,   251,
     252,   252,   252,   252,   252,   253,   253,   254,   254,   255,
     256,   255,   257,   257,   257,   258,   259,   259,   260,   261,
     261,   262,   262,   263,   264,   265,   265,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   267,   267,   267,   267,   267,   267,   268,   269,
     269,   270,   270,   271,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   273,   274,   275,   275,
     275,   275,   276,   276,   277,   277,   278,   278,   278,   278,
     278,   279,   279,   280,   281,   281,   282,   282,   283,   283,
     284,   284,   285,   286,   286,   287,   287
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     2,     3,     0,     2,
       4,     3,     2,     2,     1,     3,     3,     2,     4,     0,
       1,     2,     1,     3,     1,     3,     3,     3,     2,     1,
       1,     0,     2,     4,     1,     1,     4,     6,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     5,     1,     3,     1,     1,
       1,     4,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     3,
       7,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     2,     1,     3,     2,     2,     2,     4,
       5,     2,     1,     1,     2,     3,     4,     2,     3,     3,
       4,     2,     3,     4,     0,     2,     1,     1,     3,     5,
       5,     5,     5,     8,    10,     6,     9,    11,     1,     3,
       1,     2,     3,     1,     1,     1,     5,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     6,     5,     6,     3,     6,     7,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     4,     3,     3,     1,     3,     4,     3,     4,
       2,     4,     4,     6,     7,     3,     5,     4,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     3,     0,     1,     3,     0,     3,     2,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     5,     3,     3,     1,     1,     1,     0,     1,
       4,     6,     5,     5,     4,     0,     2,     0,     1,     5,
       0,     5,     0,     3,     5,     4,     1,     2,     4,     5,
       7,     0,     2,     2,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     3,     4,     1,     1,     1,     1,     4,     3,
       4,     2,     3,     2,     4,     3,     2,     2,     3,     2,
       4,     2,     4,     6,     4,     1,     4,     5,     1,     4,
       5,     1,     4,     5,     4,     4,     1,     6,     1,     1,
       1,     1,     3,     5,     1,     3,     5,     4,     7,     5,
       7,     1,     3,     3,     1,     1,     1,     3,     5,     5,
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
    case 133: /* "name"  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3122 "ds_parser.cpp"
        break;

    case 177: /* character_sequence  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3128 "ds_parser.cpp"
        break;

    case 178: /* string_constant  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3134 "ds_parser.cpp"
        break;

    case 179: /* string_builder_body  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3140 "ds_parser.cpp"
        break;

    case 180: /* string_builder  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3146 "ds_parser.cpp"
        break;

    case 183: /* require_module_name  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3152 "ds_parser.cpp"
        break;

    case 189: /* expression_label  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3158 "ds_parser.cpp"
        break;

    case 190: /* expression_goto  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3164 "ds_parser.cpp"
        break;

    case 192: /* expression_else  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3170 "ds_parser.cpp"
        break;

    case 194: /* expression_if_then_else  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3176 "ds_parser.cpp"
        break;

    case 195: /* expression_for_loop  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3182 "ds_parser.cpp"
        break;

    case 196: /* expression_while_loop  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3188 "ds_parser.cpp"
        break;

    case 197: /* expression_with  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3194 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_value  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3200 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3206 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3212 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3218 "ds_parser.cpp"
        break;

    case 202: /* annotation_declaration_name  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3224 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3230 "ds_parser.cpp"
        break;

    case 204: /* annotation_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3236 "ds_parser.cpp"
        break;

    case 205: /* optional_annotation_list  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3242 "ds_parser.cpp"
        break;

    case 206: /* optional_function_argument_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3248 "ds_parser.cpp"
        break;

    case 207: /* optional_function_type  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3254 "ds_parser.cpp"
        break;

    case 208: /* function_name  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3260 "ds_parser.cpp"
        break;

    case 210: /* expression_block  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3266 "ds_parser.cpp"
        break;

    case 211: /* expression_any  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3272 "ds_parser.cpp"
        break;

    case 212: /* expressions  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3278 "ds_parser.cpp"
        break;

    case 213: /* expr_pipe  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3284 "ds_parser.cpp"
        break;

    case 214: /* name_in_namespace  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3290 "ds_parser.cpp"
        break;

    case 215: /* expression_delete  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3296 "ds_parser.cpp"
        break;

    case 216: /* expr_new  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3302 "ds_parser.cpp"
        break;

    case 217: /* expression_break  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3308 "ds_parser.cpp"
        break;

    case 218: /* expression_continue  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3314 "ds_parser.cpp"
        break;

    case 219: /* expression_return  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3320 "ds_parser.cpp"
        break;

    case 220: /* expression_yield  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3326 "ds_parser.cpp"
        break;

    case 221: /* expression_try_catch  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3332 "ds_parser.cpp"
        break;

    case 224: /* expression_let  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3338 "ds_parser.cpp"
        break;

    case 225: /* expr_cast  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3344 "ds_parser.cpp"
        break;

    case 226: /* expr_type_info  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3350 "ds_parser.cpp"
        break;

    case 227: /* expr_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3356 "ds_parser.cpp"
        break;

    case 228: /* block_or_simple_block  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3362 "ds_parser.cpp"
        break;

    case 230: /* expr_block  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3368 "ds_parser.cpp"
        break;

    case 231: /* expr_numeric_const  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3374 "ds_parser.cpp"
        break;

    case 232: /* expr_assign  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3380 "ds_parser.cpp"
        break;

    case 233: /* expr_assign_pipe  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3386 "ds_parser.cpp"
        break;

    case 234: /* expr_named_call  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3392 "ds_parser.cpp"
        break;

    case 235: /* expr_method_call  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3398 "ds_parser.cpp"
        break;

    case 236: /* func_addr_expr  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3404 "ds_parser.cpp"
        break;

    case 237: /* expr  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3410 "ds_parser.cpp"
        break;

    case 238: /* optional_field_annotation  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3416 "ds_parser.cpp"
        break;

    case 240: /* structure_variable_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3422 "ds_parser.cpp"
        break;

    case 241: /* struct_variable_declaration_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3428 "ds_parser.cpp"
        break;

    case 242: /* function_argument_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3434 "ds_parser.cpp"
        break;

    case 243: /* function_argument_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3440 "ds_parser.cpp"
        break;

    case 244: /* tuple_type  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3446 "ds_parser.cpp"
        break;

    case 245: /* tuple_type_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3452 "ds_parser.cpp"
        break;

    case 246: /* variant_type  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3458 "ds_parser.cpp"
        break;

    case 247: /* variant_type_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3464 "ds_parser.cpp"
        break;

    case 249: /* variable_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3470 "ds_parser.cpp"
        break;

    case 252: /* let_variable_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3476 "ds_parser.cpp"
        break;

    case 253: /* global_variable_declaration_list  */
#line 111 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3482 "ds_parser.cpp"
        break;

    case 257: /* enum_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3488 "ds_parser.cpp"
        break;

    case 262: /* optional_structure_parent  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3494 "ds_parser.cpp"
        break;

    case 265: /* variable_name_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3500 "ds_parser.cpp"
        break;

    case 268: /* structure_type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3506 "ds_parser.cpp"
        break;

    case 269: /* auto_type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3512 "ds_parser.cpp"
        break;

    case 270: /* bitfield_bits  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3518 "ds_parser.cpp"
        break;

    case 271: /* bitfield_type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3524 "ds_parser.cpp"
        break;

    case 272: /* type_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3530 "ds_parser.cpp"
        break;

    case 275: /* make_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3536 "ds_parser.cpp"
        break;

    case 276: /* make_struct_fields  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3542 "ds_parser.cpp"
        break;

    case 277: /* make_struct_dim  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3548 "ds_parser.cpp"
        break;

    case 278: /* make_struct_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3554 "ds_parser.cpp"
        break;

    case 279: /* make_tuple  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3560 "ds_parser.cpp"
        break;

    case 280: /* make_map_tuple  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3566 "ds_parser.cpp"
        break;

    case 281: /* make_any_tuple  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3572 "ds_parser.cpp"
        break;

    case 282: /* make_dim  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3578 "ds_parser.cpp"
        break;

    case 283: /* make_dim_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3584 "ds_parser.cpp"
        break;

    case 284: /* make_table  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3590 "ds_parser.cpp"
        break;

    case 285: /* make_table_decl  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3596 "ds_parser.cpp"
        break;

    case 286: /* array_comprehension_where  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3602 "ds_parser.cpp"
        break;

    case 287: /* array_comprehension  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3608 "ds_parser.cpp"
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
  case 14:
#line 410 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3908 "ds_parser.cpp"
    break;

  case 15:
#line 422 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3914 "ds_parser.cpp"
    break;

  case 16:
#line 423 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3920 "ds_parser.cpp"
    break;

  case 17:
#line 427 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3926 "ds_parser.cpp"
    break;

  case 18:
#line 431 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 3936 "ds_parser.cpp"
    break;

  case 19:
#line 436 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3950 "ds_parser.cpp"
    break;

  case 20:
#line 445 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3960 "ds_parser.cpp"
    break;

  case 21:
#line 453 "ds_parser.ypp"
    {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(strb->at,"");
            delete (yyvsp[-1].pExpression);
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_pointer_cast<ExprConstString>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(strb->at,sconst->text);
            delete (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
#line 3978 "ds_parser.cpp"
    break;

  case 22:
#line 469 "ds_parser.ypp"
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
#line 4002 "ds_parser.cpp"
    break;

  case 24:
#line 495 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4010 "ds_parser.cpp"
    break;

  case 25:
#line 498 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4021 "ds_parser.cpp"
    break;

  case 26:
#line 504 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4032 "ds_parser.cpp"
    break;

  case 27:
#line 513 "ds_parser.ypp"
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
#line 4058 "ds_parser.cpp"
    break;

  case 28:
#line 534 "ds_parser.ypp"
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
#line 4083 "ds_parser.cpp"
    break;

  case 29:
#line 557 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4089 "ds_parser.cpp"
    break;

  case 30:
#line 558 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4095 "ds_parser.cpp"
    break;

  case 34:
#line 571 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4103 "ds_parser.cpp"
    break;

  case 35:
#line 574 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4111 "ds_parser.cpp"
    break;

  case 36:
#line 580 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4119 "ds_parser.cpp"
    break;

  case 37:
#line 586 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4127 "ds_parser.cpp"
    break;

  case 38:
#line 589 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4135 "ds_parser.cpp"
    break;

  case 39:
#line 595 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4141 "ds_parser.cpp"
    break;

  case 40:
#line 596 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4147 "ds_parser.cpp"
    break;

  case 41:
#line 600 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4153 "ds_parser.cpp"
    break;

  case 42:
#line 601 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4159 "ds_parser.cpp"
    break;

  case 43:
#line 602 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4170 "ds_parser.cpp"
    break;

  case 44:
#line 611 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4176 "ds_parser.cpp"
    break;

  case 45:
#line 612 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4182 "ds_parser.cpp"
    break;

  case 46:
#line 616 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4193 "ds_parser.cpp"
    break;

  case 47:
#line 625 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4207 "ds_parser.cpp"
    break;

  case 48:
#line 637 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4219 "ds_parser.cpp"
    break;

  case 49:
#line 647 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4230 "ds_parser.cpp"
    break;

  case 50:
#line 656 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4236 "ds_parser.cpp"
    break;

  case 51:
#line 657 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4242 "ds_parser.cpp"
    break;

  case 52:
#line 658 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4248 "ds_parser.cpp"
    break;

  case 53:
#line 659 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4254 "ds_parser.cpp"
    break;

  case 54:
#line 660 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4260 "ds_parser.cpp"
    break;

  case 55:
#line 661 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4266 "ds_parser.cpp"
    break;

  case 56:
#line 665 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4276 "ds_parser.cpp"
    break;

  case 57:
#line 670 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4286 "ds_parser.cpp"
    break;

  case 58:
#line 678 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4292 "ds_parser.cpp"
    break;

  case 59:
#line 679 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4298 "ds_parser.cpp"
    break;

  case 60:
#line 680 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4304 "ds_parser.cpp"
    break;

  case 61:
#line 681 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4310 "ds_parser.cpp"
    break;

  case 62:
#line 682 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4316 "ds_parser.cpp"
    break;

  case 63:
#line 683 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4322 "ds_parser.cpp"
    break;

  case 64:
#line 684 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4328 "ds_parser.cpp"
    break;

  case 65:
#line 685 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4336 "ds_parser.cpp"
    break;

  case 66:
#line 691 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
		if ( (yyvsp[0].aa)->type==Type::none ) {
			for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
				sarg.name = (yyvsp[0].aa)->name;
				(yyval.aaList)->push_back(sarg);
			}
			delete (yyvsp[0].aa)->aList;
		} else {
			(yyval.aaList)->push_back(*(yyvsp[0].aa));
		}
        delete (yyvsp[0].aa);
    }
#line 4354 "ds_parser.cpp"
    break;

  case 67:
#line 704 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
			if ( (yyvsp[0].aa)->type==Type::none ) {
				for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
					sarg.name = (yyvsp[0].aa)->name;
					(yyval.aaList)->push_back(sarg);
				}
				delete (yyvsp[0].aa)->aList;
			} else {
				(yyval.aaList)->push_back(*(yyvsp[0].aa));
			}
            delete (yyvsp[0].aa);
    }
#line 4372 "ds_parser.cpp"
    break;

  case 68:
#line 720 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4378 "ds_parser.cpp"
    break;

  case 69:
#line 721 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4384 "ds_parser.cpp"
    break;

  case 70:
#line 725 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4396 "ds_parser.cpp"
    break;

  case 71:
#line 732 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4410 "ds_parser.cpp"
    break;

  case 72:
#line 744 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4419 "ds_parser.cpp"
    break;

  case 73:
#line 748 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4428 "ds_parser.cpp"
    break;

  case 74:
#line 755 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4434 "ds_parser.cpp"
    break;

  case 75:
#line 756 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4440 "ds_parser.cpp"
    break;

  case 76:
#line 760 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4446 "ds_parser.cpp"
    break;

  case 77:
#line 761 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4452 "ds_parser.cpp"
    break;

  case 78:
#line 762 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4458 "ds_parser.cpp"
    break;

  case 79:
#line 766 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4468 "ds_parser.cpp"
    break;

  case 80:
#line 771 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4477 "ds_parser.cpp"
    break;

  case 81:
#line 778 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4486 "ds_parser.cpp"
    break;

  case 82:
#line 782 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4492 "ds_parser.cpp"
    break;

  case 83:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4498 "ds_parser.cpp"
    break;

  case 84:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4504 "ds_parser.cpp"
    break;

  case 85:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4510 "ds_parser.cpp"
    break;

  case 86:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4516 "ds_parser.cpp"
    break;

  case 87:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4522 "ds_parser.cpp"
    break;

  case 88:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4528 "ds_parser.cpp"
    break;

  case 89:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4534 "ds_parser.cpp"
    break;

  case 90:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4540 "ds_parser.cpp"
    break;

  case 91:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4546 "ds_parser.cpp"
    break;

  case 92:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4552 "ds_parser.cpp"
    break;

  case 93:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4558 "ds_parser.cpp"
    break;

  case 94:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4564 "ds_parser.cpp"
    break;

  case 95:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4570 "ds_parser.cpp"
    break;

  case 96:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4576 "ds_parser.cpp"
    break;

  case 97:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4582 "ds_parser.cpp"
    break;

  case 98:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4588 "ds_parser.cpp"
    break;

  case 99:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4594 "ds_parser.cpp"
    break;

  case 100:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4600 "ds_parser.cpp"
    break;

  case 101:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4606 "ds_parser.cpp"
    break;

  case 102:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4612 "ds_parser.cpp"
    break;

  case 103:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4618 "ds_parser.cpp"
    break;

  case 104:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4624 "ds_parser.cpp"
    break;

  case 105:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4630 "ds_parser.cpp"
    break;

  case 106:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4636 "ds_parser.cpp"
    break;

  case 107:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4642 "ds_parser.cpp"
    break;

  case 108:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4648 "ds_parser.cpp"
    break;

  case 109:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4654 "ds_parser.cpp"
    break;

  case 110:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4660 "ds_parser.cpp"
    break;

  case 111:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4666 "ds_parser.cpp"
    break;

  case 112:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4672 "ds_parser.cpp"
    break;

  case 113:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4678 "ds_parser.cpp"
    break;

  case 114:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4684 "ds_parser.cpp"
    break;

  case 115:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4690 "ds_parser.cpp"
    break;

  case 116:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4696 "ds_parser.cpp"
    break;

  case 117:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4702 "ds_parser.cpp"
    break;

  case 118:
#line 822 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-3]));
        pFunction->atDecl = tokRangeAt((yylsp[-5]), (yylsp[0]));
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
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
                            das_yyerror("function argument is already declared " + name,pDecl->at,
                                CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
        if ( (yyvsp[-5].faList) ) {
            for ( auto pA : *(yyvsp[-5].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(pFunction, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't apply annotation\n" + err,
                                tokAt((yylsp[-5])), CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("functions are only allowed function annotations",
                            tokAt((yylsp[-5])), CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( pFunction->annotations, *(yyvsp[-5].faList) );
            delete (yyvsp[-5].faList);
        }
        if ( pFunction->isGeneric() ) {
            if ( !g_Program->addGeneric(pFunction) ) {
                das_yyerror("generic function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-3])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        } else {
            if ( !g_Program->addFunction(pFunction) ) {
                das_yyerror("function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-3])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        }
        delete (yyvsp[-3].s);
    }
#line 4777 "ds_parser.cpp"
    break;

  case 119:
#line 895 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4785 "ds_parser.cpp"
    break;

  case 120:
#line 898 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4797 "ds_parser.cpp"
    break;

  case 121:
#line 908 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4803 "ds_parser.cpp"
    break;

  case 122:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4809 "ds_parser.cpp"
    break;

  case 123:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4815 "ds_parser.cpp"
    break;

  case 124:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4821 "ds_parser.cpp"
    break;

  case 125:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4827 "ds_parser.cpp"
    break;

  case 126:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4833 "ds_parser.cpp"
    break;

  case 127:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4839 "ds_parser.cpp"
    break;

  case 128:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4845 "ds_parser.cpp"
    break;

  case 129:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4851 "ds_parser.cpp"
    break;

  case 130:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4857 "ds_parser.cpp"
    break;

  case 131:
#line 918 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4863 "ds_parser.cpp"
    break;

  case 132:
#line 919 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4869 "ds_parser.cpp"
    break;

  case 133:
#line 920 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4875 "ds_parser.cpp"
    break;

  case 134:
#line 921 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4881 "ds_parser.cpp"
    break;

  case 135:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4887 "ds_parser.cpp"
    break;

  case 136:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4893 "ds_parser.cpp"
    break;

  case 137:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4899 "ds_parser.cpp"
    break;

  case 138:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4905 "ds_parser.cpp"
    break;

  case 139:
#line 929 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4915 "ds_parser.cpp"
    break;

  case 140:
#line 934 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4926 "ds_parser.cpp"
    break;

  case 141:
#line 940 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4934 "ds_parser.cpp"
    break;

  case 142:
#line 946 "ds_parser.ypp"
    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 4950 "ds_parser.cpp"
    break;

  case 143:
#line 957 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4958 "ds_parser.cpp"
    break;

  case 144:
#line 963 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4964 "ds_parser.cpp"
    break;

  case 145:
#line 964 "ds_parser.ypp"
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
#line 4980 "ds_parser.cpp"
    break;

  case 146:
#line 975 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4986 "ds_parser.cpp"
    break;

  case 147:
#line 979 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4994 "ds_parser.cpp"
    break;

  case 148:
#line 985 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5002 "ds_parser.cpp"
    break;

  case 149:
#line 988 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5010 "ds_parser.cpp"
    break;

  case 150:
#line 991 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5019 "ds_parser.cpp"
    break;

  case 151:
#line 995 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5027 "ds_parser.cpp"
    break;

  case 152:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5033 "ds_parser.cpp"
    break;

  case 153:
#line 1005 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5039 "ds_parser.cpp"
    break;

  case 154:
#line 1009 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5047 "ds_parser.cpp"
    break;

  case 155:
#line 1012 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5055 "ds_parser.cpp"
    break;

  case 156:
#line 1015 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5065 "ds_parser.cpp"
    break;

  case 157:
#line 1020 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5073 "ds_parser.cpp"
    break;

  case 158:
#line 1023 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5083 "ds_parser.cpp"
    break;

  case 159:
#line 1031 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5091 "ds_parser.cpp"
    break;

  case 160:
#line 1034 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5101 "ds_parser.cpp"
    break;

  case 161:
#line 1039 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5109 "ds_parser.cpp"
    break;

  case 162:
#line 1042 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5119 "ds_parser.cpp"
    break;

  case 163:
#line 1050 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5127 "ds_parser.cpp"
    break;

  case 164:
#line 1056 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5133 "ds_parser.cpp"
    break;

  case 165:
#line 1057 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5139 "ds_parser.cpp"
    break;

  case 166:
#line 1061 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5145 "ds_parser.cpp"
    break;

  case 167:
#line 1062 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5151 "ds_parser.cpp"
    break;

  case 168:
#line 1066 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
        pLet->inScope = (yyvsp[-1].b);
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name) ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name;
                    pVar->at = (yyvsp[0].pVarDecl)->at;
                    pVar->type = make_smart<TypeDecl>(*(yyvsp[0].pVarDecl)->pTypeDecl);
                    if ( (yyvsp[0].pVarDecl)->pInit ) {
                        pVar->init = (yyvsp[0].pVarDecl)->pInit->clone();
                        pVar->init_via_move = (yyvsp[0].pVarDecl)->init_via_move;
                        pVar->init_via_clone = (yyvsp[0].pVarDecl)->init_via_clone;
                    }
                    if ( (yyvsp[-2].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pLet->variables.push_back(pVar);
                } else {
                    das_yyerror("local variable is already declared " + name,tokAt((yylsp[-2])),
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 5187 "ds_parser.cpp"
    break;

  case 169:
#line 1100 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5195 "ds_parser.cpp"
    break;

  case 170:
#line 1103 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5205 "ds_parser.cpp"
    break;

  case 171:
#line 1108 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5215 "ds_parser.cpp"
    break;

  case 172:
#line 1116 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5224 "ds_parser.cpp"
    break;

  case 173:
#line 1120 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5234 "ds_parser.cpp"
    break;

  case 174:
#line 1125 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5245 "ds_parser.cpp"
    break;

  case 175:
#line 1131 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5254 "ds_parser.cpp"
    break;

  case 176:
#line 1135 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5264 "ds_parser.cpp"
    break;

  case 177:
#line 1140 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5275 "ds_parser.cpp"
    break;

  case 178:
#line 1149 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5283 "ds_parser.cpp"
    break;

  case 179:
#line 1152 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5291 "ds_parser.cpp"
    break;

  case 180:
#line 1158 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5297 "ds_parser.cpp"
    break;

  case 181:
#line 1159 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5309 "ds_parser.cpp"
    break;

  case 182:
#line 1166 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5322 "ds_parser.cpp"
    break;

  case 183:
#line 1177 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5328 "ds_parser.cpp"
    break;

  case 184:
#line 1178 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5334 "ds_parser.cpp"
    break;

  case 185:
#line 1182 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5344 "ds_parser.cpp"
    break;

  case 186:
#line 1188 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
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
                            das_yyerror("block argument is already declared " + name,
                                pDecl->at,CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
        if ( (yyvsp[-3].faList) ) {
            for ( auto pA : *(yyvsp[-3].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(closure, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't apply annotation\n" + err, tokAt((yylsp[-3])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("blocks are only allowed function annotations", tokAt((yylsp[-3])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-3].faList) );
            delete (yyvsp[-3].faList);
        }
    }
#line 5400 "ds_parser.cpp"
    break;

  case 187:
#line 1242 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5406 "ds_parser.cpp"
    break;

  case 188:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5412 "ds_parser.cpp"
    break;

  case 189:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5418 "ds_parser.cpp"
    break;

  case 190:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5424 "ds_parser.cpp"
    break;

  case 191:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5430 "ds_parser.cpp"
    break;

  case 192:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5436 "ds_parser.cpp"
    break;

  case 193:
#line 1251 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5442 "ds_parser.cpp"
    break;

  case 194:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5448 "ds_parser.cpp"
    break;

  case 195:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5454 "ds_parser.cpp"
    break;

  case 196:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5460 "ds_parser.cpp"
    break;

  case 197:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5466 "ds_parser.cpp"
    break;

  case 198:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5472 "ds_parser.cpp"
    break;

  case 199:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5478 "ds_parser.cpp"
    break;

  case 200:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5484 "ds_parser.cpp"
    break;

  case 201:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5490 "ds_parser.cpp"
    break;

  case 202:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5496 "ds_parser.cpp"
    break;

  case 203:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5502 "ds_parser.cpp"
    break;

  case 204:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5508 "ds_parser.cpp"
    break;

  case 205:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5514 "ds_parser.cpp"
    break;

  case 206:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5520 "ds_parser.cpp"
    break;

  case 207:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5526 "ds_parser.cpp"
    break;

  case 208:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5532 "ds_parser.cpp"
    break;

  case 209:
#line 1270 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5538 "ds_parser.cpp"
    break;

  case 210:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5544 "ds_parser.cpp"
    break;

  case 211:
#line 1275 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5556 "ds_parser.cpp"
    break;

  case 212:
#line 1286 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5566 "ds_parser.cpp"
    break;

  case 213:
#line 1291 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5578 "ds_parser.cpp"
    break;

  case 214:
#line 1301 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5587 "ds_parser.cpp"
    break;

  case 215:
#line 1305 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5599 "ds_parser.cpp"
    break;

  case 216:
#line 1312 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-5])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(expr->funcType.get(), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
		(yyval.pExpression) = expr;
    }
#line 5615 "ds_parser.cpp"
    break;

  case 217:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5621 "ds_parser.cpp"
    break;

  case 218:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5627 "ds_parser.cpp"
    break;

  case 219:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5633 "ds_parser.cpp"
    break;

  case 220:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5639 "ds_parser.cpp"
    break;

  case 221:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5645 "ds_parser.cpp"
    break;

  case 222:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5651 "ds_parser.cpp"
    break;

  case 223:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5657 "ds_parser.cpp"
    break;

  case 224:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5663 "ds_parser.cpp"
    break;

  case 225:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5669 "ds_parser.cpp"
    break;

  case 226:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5675 "ds_parser.cpp"
    break;

  case 227:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5681 "ds_parser.cpp"
    break;

  case 228:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5687 "ds_parser.cpp"
    break;

  case 229:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5693 "ds_parser.cpp"
    break;

  case 230:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5699 "ds_parser.cpp"
    break;

  case 231:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5705 "ds_parser.cpp"
    break;

  case 232:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5711 "ds_parser.cpp"
    break;

  case 233:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5717 "ds_parser.cpp"
    break;

  case 234:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5723 "ds_parser.cpp"
    break;

  case 235:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5729 "ds_parser.cpp"
    break;

  case 236:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5735 "ds_parser.cpp"
    break;

  case 237:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5741 "ds_parser.cpp"
    break;

  case 238:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5747 "ds_parser.cpp"
    break;

  case 239:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5753 "ds_parser.cpp"
    break;

  case 240:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5759 "ds_parser.cpp"
    break;

  case 241:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5765 "ds_parser.cpp"
    break;

  case 242:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5771 "ds_parser.cpp"
    break;

  case 243:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5777 "ds_parser.cpp"
    break;

  case 244:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5783 "ds_parser.cpp"
    break;

  case 245:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5789 "ds_parser.cpp"
    break;

  case 246:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5795 "ds_parser.cpp"
    break;

  case 247:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5801 "ds_parser.cpp"
    break;

  case 248:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5807 "ds_parser.cpp"
    break;

  case 249:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5813 "ds_parser.cpp"
    break;

  case 250:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5819 "ds_parser.cpp"
    break;

  case 251:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5825 "ds_parser.cpp"
    break;

  case 252:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5831 "ds_parser.cpp"
    break;

  case 253:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5837 "ds_parser.cpp"
    break;

  case 254:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5843 "ds_parser.cpp"
    break;

  case 255:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5849 "ds_parser.cpp"
    break;

  case 256:
#line 1365 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5858 "ds_parser.cpp"
    break;

  case 257:
#line 1369 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5867 "ds_parser.cpp"
    break;

  case 258:
#line 1373 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5875 "ds_parser.cpp"
    break;

  case 259:
#line 1376 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5883 "ds_parser.cpp"
    break;

  case 260:
#line 1379 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5889 "ds_parser.cpp"
    break;

  case 261:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5895 "ds_parser.cpp"
    break;

  case 262:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5901 "ds_parser.cpp"
    break;

  case 263:
#line 1382 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5911 "ds_parser.cpp"
    break;

  case 264:
#line 1387 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5921 "ds_parser.cpp"
    break;

  case 265:
#line 1392 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5927 "ds_parser.cpp"
    break;

  case 266:
#line 1393 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5935 "ds_parser.cpp"
    break;

  case 267:
#line 1396 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5943 "ds_parser.cpp"
    break;

  case 268:
#line 1399 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5952 "ds_parser.cpp"
    break;

  case 269:
#line 1403 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5961 "ds_parser.cpp"
    break;

  case 270:
#line 1407 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5970 "ds_parser.cpp"
    break;

  case 271:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5976 "ds_parser.cpp"
    break;

  case 272:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5982 "ds_parser.cpp"
    break;

  case 273:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5988 "ds_parser.cpp"
    break;

  case 274:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5994 "ds_parser.cpp"
    break;

  case 275:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6000 "ds_parser.cpp"
    break;

  case 276:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6006 "ds_parser.cpp"
    break;

  case 277:
#line 1417 "ds_parser.ypp"
    {
        if ( (yyvsp[-2].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-2].pExpression);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 6021 "ds_parser.cpp"
    break;

  case 278:
#line 1427 "ds_parser.ypp"
    {
        if ( (yyvsp[0].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[0].pExpression);
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = (yyvsp[0].pExpression);
        } else {
            das_yyerror("can only pipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    }
#line 6036 "ds_parser.cpp"
    break;

  case 279:
#line 1437 "ds_parser.ypp"
    {
		ExprConstEnumeration * resEnum = nullptr;
        auto enums = g_Program->findEnum(*(yyvsp[-1].s));
        if ( enums.size()!=1 ) {
            if ( enums.size() ) {
                string candidates = g_Program->describeCandidates(enums);
                das_yyerror("enumeraiton not found " + *(yyvsp[-1].s) + "\n" + candidates, tokAt((yylsp[-1])),
                    CompilationError::enumeration_not_found);
            } else {
                das_yyerror("enumeraiton not found " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                    CompilationError::enumeration_not_found);
            }
        } else {
            auto pEnum = enums.back();
            auto ff = pEnum->find(*(yyvsp[0].s));
            if ( ff.second ) {
				auto td = make_smart<TypeDecl>(pEnum);
                resEnum = new ExprConstEnumeration(tokAt((yylsp[0])), *(yyvsp[0].s), td);
            } else {
                das_yyerror("enumeraiton value not found " + *(yyvsp[-1].s) + " " + *(yyvsp[0].s), tokAt((yylsp[0])),
                    CompilationError::enumeration_not_found);
            }
        }
		if ( resEnum ) {
			(yyval.pExpression) = resEnum;
		} else {
			(yyval.pExpression) = new ExprConstInt(0);	// dummy
		}
        delete (yyvsp[-1].s);
        delete (yyvsp[0].s);
    }
#line 6072 "ds_parser.cpp"
    break;

  case 280:
#line 1471 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6078 "ds_parser.cpp"
    break;

  case 281:
#line 1472 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6084 "ds_parser.cpp"
    break;

  case 282:
#line 1476 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6090 "ds_parser.cpp"
    break;

  case 283:
#line 1477 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6096 "ds_parser.cpp"
    break;

  case 284:
#line 1481 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6106 "ds_parser.cpp"
    break;

  case 285:
#line 1489 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6114 "ds_parser.cpp"
    break;

  case 286:
#line 1492 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6123 "ds_parser.cpp"
    break;

  case 287:
#line 1499 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6133 "ds_parser.cpp"
    break;

  case 288:
#line 1504 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6143 "ds_parser.cpp"
    break;

  case 289:
#line 1512 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6149 "ds_parser.cpp"
    break;

  case 290:
#line 1513 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6155 "ds_parser.cpp"
    break;

  case 291:
#line 1517 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6163 "ds_parser.cpp"
    break;

  case 292:
#line 1520 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6174 "ds_parser.cpp"
    break;

  case 293:
#line 1529 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6180 "ds_parser.cpp"
    break;

  case 294:
#line 1530 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6186 "ds_parser.cpp"
    break;

  case 295:
#line 1534 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6197 "ds_parser.cpp"
    break;

  case 296:
#line 1543 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6203 "ds_parser.cpp"
    break;

  case 297:
#line 1544 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6209 "ds_parser.cpp"
    break;

  case 298:
#line 1549 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6215 "ds_parser.cpp"
    break;

  case 299:
#line 1550 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6221 "ds_parser.cpp"
    break;

  case 300:
#line 1554 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6231 "ds_parser.cpp"
    break;

  case 301:
#line 1559 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6239 "ds_parser.cpp"
    break;

  case 302:
#line 1562 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6248 "ds_parser.cpp"
    break;

  case 303:
#line 1566 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6259 "ds_parser.cpp"
    break;

  case 304:
#line 1572 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6270 "ds_parser.cpp"
    break;

  case 305:
#line 1581 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6276 "ds_parser.cpp"
    break;

  case 306:
#line 1582 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6282 "ds_parser.cpp"
    break;

  case 307:
#line 1583 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6288 "ds_parser.cpp"
    break;

  case 308:
#line 1587 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6294 "ds_parser.cpp"
    break;

  case 309:
#line 1588 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6300 "ds_parser.cpp"
    break;

  case 310:
#line 1592 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6308 "ds_parser.cpp"
    break;

  case 311:
#line 1595 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6318 "ds_parser.cpp"
    break;

  case 312:
#line 1600 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6328 "ds_parser.cpp"
    break;

  case 313:
#line 1605 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6341 "ds_parser.cpp"
    break;

  case 314:
#line 1613 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6354 "ds_parser.cpp"
    break;

  case 315:
#line 1624 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6362 "ds_parser.cpp"
    break;

  case 316:
#line 1627 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6371 "ds_parser.cpp"
    break;

  case 317:
#line 1634 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6377 "ds_parser.cpp"
    break;

  case 318:
#line 1635 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6383 "ds_parser.cpp"
    break;

  case 319:
#line 1639 "ds_parser.ypp"
    {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name;
                    pVar->at = pDecl->at;
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
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name,pDecl->at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6415 "ds_parser.cpp"
    break;

  case 320:
#line 1666 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6424 "ds_parser.cpp"
    break;

  case 321:
#line 1669 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6435 "ds_parser.cpp"
    break;

  case 322:
#line 1678 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6443 "ds_parser.cpp"
    break;

  case 323:
#line 1681 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6457 "ds_parser.cpp"
    break;

  case 324:
#line 1690 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6471 "ds_parser.cpp"
    break;

  case 325:
#line 1703 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6485 "ds_parser.cpp"
    break;

  case 329:
#line 1724 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-4]));
        pEnum->name = *(yyvsp[-3].s);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-3].s);
    }
#line 6501 "ds_parser.cpp"
    break;

  case 330:
#line 1735 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-5].s),tokAt((yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-6]));
        pEnum->name = *(yyvsp[-5].s);
		pEnum->baseType = (yyvsp[-3].type);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-5].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-5].s);
    }
#line 6518 "ds_parser.cpp"
    break;

  case 331:
#line 1750 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6524 "ds_parser.cpp"
    break;

  case 332:
#line 1751 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6530 "ds_parser.cpp"
    break;

  case 333:
#line 1755 "ds_parser.ypp"
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
            } else if ( structs.size()==0 ) {
                das_yyerror("parent structure not found"+*(yyvsp[0].s),tokAt((yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = g_Program->describeCandidates(structs);
                das_yyerror("too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt((yylsp[0])),
                    CompilationError::structure_not_found);

            }
            delete (yyvsp[0].s);
        } else {
            pStruct = make_smart<Structure>(*(yyvsp[-1].s));
        }
        if ( pStruct ) {
            if ( !g_Program->addStructure(pStruct) ) {
                das_yyerror("structure is already defined "+*(yyvsp[-1].s),tokAt((yylsp[-1])),
                    CompilationError::structure_already_declared);
                (yyval.pStructure) = nullptr;
            } else {
                (yyval.pStructure) = pStruct.get();
            }
        } else {
            (yyval.pStructure) = nullptr;
        }
        delete (yyvsp[-1].s);
    }
#line 6571 "ds_parser.cpp"
    break;

  case 334:
#line 1794 "ds_parser.ypp"
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure);
            pStruct->at = tokAt((yylsp[-4]));
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything, "+name,pDecl->at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, pDecl->at);
                        }
                    } else {
                        if ( pDecl->override ) {
                                auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                                oldFd->init = init;
                                oldFd->parentType = oldFd->type->isAuto();
                        } else {
                            das_yyerror("structure field is already declared "+name
                                +", use override to replace initial value instead",pDecl->at,
                                    CompilationError::invalid_override);
                        }
                    }
                }
            }
            if ( (yyvsp[-5].faList) ) {
                for ( auto pA : *(yyvsp[-5].faList) ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isStructureAnnotation() ) {
                            auto ann = static_pointer_cast<StructureAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->touch(pStruct, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                                das_yyerror("can't 'touch' with structure annotation\n" + err,
                                    tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-5].faList)->size()!=1 ) {
                                das_yyerror("structures are only allowed one structure type annotation", tokAt((yylsp[-4])),
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror("handled structure is already defined "+pStruct->name,tokAt((yylsp[-4])),
                                        CompilationError::structure_already_declared);
                                } else {
                                    pStruct->module->removeStructure(pStruct);
                                }
                            }
                        }
                    }
                }
				swap ( pStruct->annotations, *(yyvsp[-5].faList) );
                delete (yyvsp[-5].faList);
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6639 "ds_parser.cpp"
    break;

  case 335:
#line 1860 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6651 "ds_parser.cpp"
    break;

  case 336:
#line 1867 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6662 "ds_parser.cpp"
    break;

  case 337:
#line 1876 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6668 "ds_parser.cpp"
    break;

  case 338:
#line 1877 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6674 "ds_parser.cpp"
    break;

  case 339:
#line 1878 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6680 "ds_parser.cpp"
    break;

  case 340:
#line 1879 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6686 "ds_parser.cpp"
    break;

  case 341:
#line 1880 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6692 "ds_parser.cpp"
    break;

  case 342:
#line 1881 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6698 "ds_parser.cpp"
    break;

  case 343:
#line 1882 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6704 "ds_parser.cpp"
    break;

  case 344:
#line 1883 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6710 "ds_parser.cpp"
    break;

  case 345:
#line 1884 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6716 "ds_parser.cpp"
    break;

  case 346:
#line 1885 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6722 "ds_parser.cpp"
    break;

  case 347:
#line 1886 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6728 "ds_parser.cpp"
    break;

  case 348:
#line 1887 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6734 "ds_parser.cpp"
    break;

  case 349:
#line 1888 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6740 "ds_parser.cpp"
    break;

  case 350:
#line 1889 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6746 "ds_parser.cpp"
    break;

  case 351:
#line 1890 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6752 "ds_parser.cpp"
    break;

  case 352:
#line 1891 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6758 "ds_parser.cpp"
    break;

  case 353:
#line 1892 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6764 "ds_parser.cpp"
    break;

  case 354:
#line 1893 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6770 "ds_parser.cpp"
    break;

  case 355:
#line 1894 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6776 "ds_parser.cpp"
    break;

  case 356:
#line 1895 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6782 "ds_parser.cpp"
    break;

  case 357:
#line 1896 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6788 "ds_parser.cpp"
    break;

  case 358:
#line 1897 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6794 "ds_parser.cpp"
    break;

  case 359:
#line 1898 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6800 "ds_parser.cpp"
    break;

  case 360:
#line 1899 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6806 "ds_parser.cpp"
    break;

  case 361:
#line 1900 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 6812 "ds_parser.cpp"
    break;

  case 362:
#line 1904 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6818 "ds_parser.cpp"
    break;

  case 363:
#line 1905 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6824 "ds_parser.cpp"
    break;

  case 364:
#line 1906 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6830 "ds_parser.cpp"
    break;

  case 365:
#line 1907 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6836 "ds_parser.cpp"
    break;

  case 366:
#line 1908 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6842 "ds_parser.cpp"
    break;

  case 367:
#line 1909 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6848 "ds_parser.cpp"
    break;

  case 368:
#line 1913 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6861 "ds_parser.cpp"
    break;

  case 369:
#line 1924 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6870 "ds_parser.cpp"
    break;

  case 370:
#line 1928 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6882 "ds_parser.cpp"
    break;

  case 371:
#line 1938 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6894 "ds_parser.cpp"
    break;

  case 372:
#line 1945 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6905 "ds_parser.cpp"
    break;

  case 373:
#line 1954 "ds_parser.ypp"
    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-1].pNameList);
            (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
            delete (yyvsp[-1].pNameList);
    }
#line 6916 "ds_parser.cpp"
    break;

  case 374:
#line 1963 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6922 "ds_parser.cpp"
    break;

  case 375:
#line 1964 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6928 "ds_parser.cpp"
    break;

  case 376:
#line 1965 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6934 "ds_parser.cpp"
    break;

  case 377:
#line 1966 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6940 "ds_parser.cpp"
    break;

  case 378:
#line 1967 "ds_parser.ypp"
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
#line 6959 "ds_parser.cpp"
    break;

  case 379:
#line 1981 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6970 "ds_parser.cpp"
    break;

  case 380:
#line 1987 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6979 "ds_parser.cpp"
    break;

  case 381:
#line 1991 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6989 "ds_parser.cpp"
    break;

  case 382:
#line 1996 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6999 "ds_parser.cpp"
    break;

  case 383:
#line 2001 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7009 "ds_parser.cpp"
    break;

  case 384:
#line 2006 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7021 "ds_parser.cpp"
    break;

  case 385:
#line 2013 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7031 "ds_parser.cpp"
    break;

  case 386:
#line 2018 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7040 "ds_parser.cpp"
    break;

  case 387:
#line 2022 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7049 "ds_parser.cpp"
    break;

  case 388:
#line 2026 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7059 "ds_parser.cpp"
    break;

  case 389:
#line 2031 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7069 "ds_parser.cpp"
    break;

  case 390:
#line 2036 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7080 "ds_parser.cpp"
    break;

  case 391:
#line 2042 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7092 "ds_parser.cpp"
    break;

  case 392:
#line 2049 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7102 "ds_parser.cpp"
    break;

  case 393:
#line 2054 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7113 "ds_parser.cpp"
    break;

  case 394:
#line 2060 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7123 "ds_parser.cpp"
    break;

  case 395:
#line 2065 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7132 "ds_parser.cpp"
    break;

  case 396:
#line 2069 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7142 "ds_parser.cpp"
    break;

  case 397:
#line 2074 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7156 "ds_parser.cpp"
    break;

  case 398:
#line 2083 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7165 "ds_parser.cpp"
    break;

  case 399:
#line 2087 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7175 "ds_parser.cpp"
    break;

  case 400:
#line 2092 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7189 "ds_parser.cpp"
    break;

  case 401:
#line 2101 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7198 "ds_parser.cpp"
    break;

  case 402:
#line 2105 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7208 "ds_parser.cpp"
    break;

  case 403:
#line 2110 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7222 "ds_parser.cpp"
    break;

  case 404:
#line 2119 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7233 "ds_parser.cpp"
    break;

  case 405:
#line 2125 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7244 "ds_parser.cpp"
    break;

  case 406:
#line 2134 "ds_parser.ypp"
    {
        das_need_oxford_comma=false;
    }
#line 7252 "ds_parser.cpp"
    break;

  case 407:
#line 2140 "ds_parser.ypp"
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
#line 7269 "ds_parser.cpp"
    break;

  case 408:
#line 2155 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7275 "ds_parser.cpp"
    break;

  case 409:
#line 2156 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7281 "ds_parser.cpp"
    break;

  case 410:
#line 2157 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7287 "ds_parser.cpp"
    break;

  case 411:
#line 2158 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7293 "ds_parser.cpp"
    break;

  case 412:
#line 2162 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7305 "ds_parser.cpp"
    break;

  case 413:
#line 2169 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7316 "ds_parser.cpp"
    break;

  case 414:
#line 2178 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7326 "ds_parser.cpp"
    break;

  case 415:
#line 2183 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7335 "ds_parser.cpp"
    break;

  case 416:
#line 2190 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7345 "ds_parser.cpp"
    break;

  case 417:
#line 2195 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7356 "ds_parser.cpp"
    break;

  case 418:
#line 2201 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7367 "ds_parser.cpp"
    break;

  case 419:
#line 2207 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7379 "ds_parser.cpp"
    break;

  case 420:
#line 2214 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7392 "ds_parser.cpp"
    break;

  case 421:
#line 2225 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7400 "ds_parser.cpp"
    break;

  case 422:
#line 2228 "ds_parser.ypp"
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
#line 7416 "ds_parser.cpp"
    break;

  case 423:
#line 2242 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7427 "ds_parser.cpp"
    break;

  case 424:
#line 2251 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7433 "ds_parser.cpp"
    break;

  case 425:
#line 2252 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7439 "ds_parser.cpp"
    break;

  case 426:
#line 2256 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7449 "ds_parser.cpp"
    break;

  case 427:
#line 2261 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7458 "ds_parser.cpp"
    break;

  case 428:
#line 2268 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7468 "ds_parser.cpp"
    break;

  case 429:
#line 2273 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7481 "ds_parser.cpp"
    break;

  case 430:
#line 2284 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7491 "ds_parser.cpp"
    break;

  case 431:
#line 2289 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7500 "ds_parser.cpp"
    break;

  case 432:
#line 2296 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7514 "ds_parser.cpp"
    break;

  case 433:
#line 2308 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7520 "ds_parser.cpp"
    break;

  case 434:
#line 2309 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7526 "ds_parser.cpp"
    break;

  case 435:
#line 2313 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->iterators = *(yyvsp[-7].pNameList);
        delete (yyvsp[-7].pNameList);
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
#line 7545 "ds_parser.cpp"
    break;

  case 436:
#line 2327 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->iterators = *(yyvsp[-7].pNameList);
        delete (yyvsp[-7].pNameList);
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
#line 7564 "ds_parser.cpp"
    break;


#line 7568 "ds_parser.cpp"

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
#line 2343 "ds_parser.ypp"


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
                    pDecl->at,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
                if ( needNames && pDecl->pNameList && !(*pDecl->pNameList)[ai].empty() ) {
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
                        pType->argNames.push_back(name);
                    }
                } else {
                    pType->argNames.push_back(string());
                }
            }
        }
    }
}


